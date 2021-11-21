#include <windows.h>
#include <tchar.h>

#include <stdio.h>
#include <stdlib.h>

#include "STEPlugin.h"

#include "SMFUtil.h"
#include "STEPMidUtil.h"
#include "debug.h"

static void (*saSetFunc[])(FILE_INFO*,LPCTSTR) = {SetComment, SetArtistName, SetTrackName};
// static LPCTSTR (*saGetFunc[])(FILE_INFO*) = {GetComment, GetArtistName, GetTrackName};

static char staticBuf[STEPMidUtil::STATIC_META_BUFFER_SIZE];
#ifdef STEP_K
static WCHAR staticWbuf[STEPMidUtil::STATIC_META_BUFFER_SIZE];
#endif

static size_t getMaxMetaEventLength(const SMFUtil::MetaEvent* const& events)
{
    size_t maxLength = 0;

    for (int i = 0; i < SMFUtil::META_MAX; i++)
    {
        size_t length = events[i].length;
        if (maxLength < length) {
            maxLength = length;
        }
    }

    return maxLength;
}

struct BufferInfo {
    void *buf;
    void *heapBuf;
    size_t lengthWithNullLimit;
};

#ifdef STEP_K
static int mbtowcAndUpdateBufferInfo(char* const& buf, struct BufferInfo* const binfo)
{
    WCHAR *wbuf = (WCHAR *)binfo->buf;
    WCHAR *heapWbuf = (WCHAR *)binfo->heapBuf;
    size_t wcharLengthWithNullLimit = binfo->lengthWithNullLimit;

    int mbtowcRet = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buf, -1, nullptr, 0);
    if (mbtowcRet <= 0) {
        DEBUGOUT("'MultiByteToWideChar' returns %d\n", mbtowcRet);
        //break;
        return -1;
    }
    size_t wcharLengthWithNull = (size_t)mbtowcRet;
    if (wcharLengthWithNull > SIZE_MAX / sizeof(WCHAR)) {
        wcharLengthWithNull = SIZE_MAX / sizeof(WCHAR);
    }
    DEBUGOUT("wcharLengthWithNull = %d, wcharLengthWithNullLimit = %d\n", wcharLengthWithNull, wcharLengthWithNullLimit );

    if (wcharLengthWithNull > wcharLengthWithNullLimit) {
        size_t newSize = wcharLengthWithNull * sizeof(WCHAR);
        if (heapWbuf == nullptr) {
            heapWbuf = (WCHAR *)malloc(newSize);
        } else {
            heapWbuf = (WCHAR *)realloc(heapWbuf, newSize);
        }
        if (heapWbuf == nullptr) {
            DEBUGOUT("Heap allocation failed\n");
            return -1;
        }
        DEBUGOUT("Heap allocated\n");
        wbuf = heapWbuf;
        wcharLengthWithNullLimit = wcharLengthWithNull;
    }

    binfo->buf = wbuf;
    binfo->heapBuf = heapWbuf;
    binfo->lengthWithNullLimit = wcharLengthWithNullLimit;

    if (MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buf, -1, wbuf, wcharLengthWithNull) == 0) {
        return -1;
    }

    return 0;
}
#endif

int STEPMidUtil::readMetaEvent(FILE_INFO* const& pFileMP3, FILE* const& fp, const SMFUtil::MetaEvent* const& events)
{
    int ret = 0;
    size_t maxLength = getMaxMetaEventLength(events);
    if (maxLength > SIZE_MAX / sizeof(char) - 1) {
        maxLength = SIZE_MAX / sizeof(char) - 1;
    }
    size_t lengthWithNullLimit = maxLength + 1;
    char *buf = staticBuf, *heapBuf = nullptr;
#ifdef STEP_K
    struct BufferInfo wcharBufferInfo = {
        staticWbuf,
        nullptr,
        STEPMidUtil::STATIC_META_BUFFER_SIZE / sizeof(WCHAR)
    };
#endif
    DEBUGOUT("lengthWithNullLimit = %d\n", lengthWithNullLimit);
    if (lengthWithNullLimit >  STEPMidUtil::STATIC_META_BUFFER_SIZE) {
        heapBuf = (char *)malloc(lengthWithNullLimit * sizeof(char));
        if (heapBuf == nullptr) {
            ret = -1;
            goto finish;
        }
        buf = heapBuf;
    }

    for (int i = 0; i < SMFUtil::META_MAX; i++)
    {
        const SMFUtil::MetaEvent *p = &events[i];
        if ( p->length > 0 )
        {
            unsigned long length = p->length;
            if (length > maxLength) {
                length = maxLength;
            }
            fseek(fp, p->offset, SEEK_SET);
            if (fread(buf, sizeof(char), length, fp) < length) {
                ret = -1;
                goto finish;
            }
            buf[length] = '\0';
#ifdef STEP_K
            if (mbtowcAndUpdateBufferInfo(buf, &wcharBufferInfo) < 0) {
                ret = -1;
                goto finish;
            }
            (saSetFunc[i])(pFileMP3, (WCHAR *)wcharBufferInfo.buf);
#else
            (saSetFunc[i])(pFileMP3, buf);
#endif
        }
    }

finish:
    if (heapBuf != nullptr) free(heapBuf);
#ifdef STEP_K
    if (wcharBufferInfo.heapBuf != nullptr) free(wcharBufferInfo.heapBuf);
#endif

    return ret;
}