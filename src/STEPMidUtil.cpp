// to use std::min with MSVC
#define NOMINMAX
#include <windows.h>
#include <tchar.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>

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

static int getMaxMetaEventLength(const SMFUtil::MetaEvent* const& events)
{
    int maxLength = 0;

    for (int i = 0; i < SMFUtil::META_MAX; i++)
    {
        int length = events[i].length;
        if (maxLength < length) {
            maxLength = length;
        }
    }

    return maxLength;
}

struct WcharBufferInfo {
    void *buf;
    void *heapBuf;
    int wcharLengthWithNullLimit;
};

#ifdef STEP_K
static int mbtowcAndUpdateBufferInfo(const char* const buf, struct WcharBufferInfo* const binfo)
{
    WCHAR *wbuf = (WCHAR *)binfo->buf;
    WCHAR *heapWbuf = (WCHAR *)binfo->heapBuf;
    int wcharLengthWithNullLimit = binfo->wcharLengthWithNullLimit;

    int mbtowcRet = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buf, -1, nullptr, 0);
    if (mbtowcRet <= 0) {
        DEBUGOUT("'MultiByteToWideChar' returns %d\n", mbtowcRet);
        //break;
        return -1;
    }
    int wcharLengthWithNull = mbtowcRet;
    if (wcharLengthWithNull > (int)std::min(SIZE_MAX / sizeof(WCHAR), (size_t)INT_MAX)) {
        wcharLengthWithNull = (int)std::min(SIZE_MAX / sizeof(WCHAR), (size_t)INT_MAX);
    }
    DEBUGOUT("wcharLengthWithNull = %d, wcharLengthWithNullLimit = %d\n", wcharLengthWithNull, wcharLengthWithNullLimit );

    if (wcharLengthWithNull > wcharLengthWithNullLimit) {
        WCHAR *newHeapWbuf = nullptr;
        size_t newSize = wcharLengthWithNull * sizeof(WCHAR);
        if (heapWbuf == nullptr) {
            newHeapWbuf = (WCHAR *)malloc(newSize);
        } else {
            newHeapWbuf = (WCHAR *)realloc(heapWbuf, newSize);
        }
        if (newHeapWbuf == nullptr) {
            DEBUGOUT("Heap allocation failed\n");
            return -1;
        } else {
            DEBUGOUT("Heap allocated\n");
            heapWbuf = newHeapWbuf;
            wbuf = heapWbuf;
            wcharLengthWithNullLimit = wcharLengthWithNull;
        }
    }

    binfo->buf = wbuf;
    binfo->heapBuf = heapWbuf;
    binfo->wcharLengthWithNullLimit = wcharLengthWithNullLimit;

    if (MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buf, -1, wbuf, wcharLengthWithNull) == 0) {
        return -1;
    }

    // for the corner case where `mbtowcRet` is larger than (SIZE_MAX / sizeof(WCHAR)) (but probably impossible)
    wbuf[wcharLengthWithNull - 1] = L'\0';

    return 0;
}
#endif

int STEPMidUtil::readMetaEvent(FILE_INFO* const pFileMP3, FILE* const fp, const SMFUtil::MetaEvent* const events)
{
    int ret = 0;
    int maxLength = getMaxMetaEventLength(events);
    if (maxLength > (int)std::min(SIZE_MAX / sizeof(char), (size_t)INT_MAX) - 1) {
        maxLength = (int)std::min(SIZE_MAX / sizeof(char), (size_t)INT_MAX) - 1;
    }
    int lengthWithNullLimit = maxLength + 1;
    char *buf = staticBuf, *heapBuf = nullptr;
#ifdef STEP_K
    struct WcharBufferInfo wcharBufferInfo = {
        staticWbuf,
        nullptr,
        STEPMidUtil::STATIC_META_BUFFER_SIZE / sizeof(WCHAR)
    };
#endif
    DEBUGOUT("lengthWithNullLimit = %d\n", lengthWithNullLimit);
    if (lengthWithNullLimit > STEPMidUtil::STATIC_META_BUFFER_SIZE) {
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
            int length = p->length;
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