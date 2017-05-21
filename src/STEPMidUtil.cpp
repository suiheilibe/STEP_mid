#include <windows.h>
#include <tchar.h>

#include <stdio.h>
#include <stdlib.h>

#include "STEPlugin.h"

#include "SMFUtil.h"
#include "STEPMidUtil.h"
#include "debug.h"

static void (*saSetFunc[])(FILE_INFO*,LPCTSTR) = {SetComment, SetArtistName, SetTrackName};
static LPCTSTR (*saGetFunc[])(FILE_INFO*) = {GetComment, GetArtistName, GetTrackName};

static char staticBuf[STEPMidUtil::STATIC_META_BUFFER_SIZE];
#ifdef STEP_K
static WCHAR staticWbuf[STEPMidUtil::STATIC_META_BUFFER_SIZE];
#endif

static unsigned long getMaxMetaEventLength(SMFUtil::MetaEvent *events)
{
    unsigned long maxLength = 0;
    unsigned long length;
    for (int i = 0; i < SMFUtil::META_MAX; i++)
    {
        length = events[i].length;
        if (maxLength < length) {
            maxLength = length;
        }
    }

    return maxLength;
}

int STEPMidUtil::readMetaEvent(FILE_INFO *pFileMP3, FILE *fp, SMFUtil::MetaEvent *events)
{
    int ret = 0;
    unsigned long maxLength = getMaxMetaEventLength(events);
    if (maxLength > SIZE_MAX / sizeof(char) - 1) {
        maxLength = SIZE_MAX / sizeof(char) - 1;
    }
    unsigned long maxLengthWithNull = maxLength + 1;
    char *buf = staticBuf, *heapBuf = nullptr;
#ifdef STEP_K
    WCHAR *wbuf = staticWbuf, *heapWbuf = nullptr;
    unsigned long maxWcharLengthWithNull = maxLengthWithNull;
    if (maxWcharLengthWithNull > SIZE_MAX / sizeof(WCHAR)) {
        maxWcharLengthWithNull = SIZE_MAX / sizeof(WCHAR);
    }
#endif
    DEBUGOUT("maxLengthWithNull = %d\n", maxLengthWithNull);
    if (maxLengthWithNull > STATIC_META_BUFFER_SIZE) {
        heapBuf = (char *)malloc(maxLengthWithNull * sizeof(char));
        if (heapBuf == nullptr) {
            ret = -1;
            goto finish;
        }
        buf = heapBuf;
#ifdef STEP_K
        heapWbuf = (WCHAR *)malloc(maxLengthWithNull * sizeof(WCHAR));
        if (heapWbuf == nullptr) {
            ret = -1;
            goto finish;
        }
        wbuf = heapWbuf;
#endif
    }

    for (int i = 0; i < SMFUtil::META_MAX; i++)
    {
        SMFUtil::MetaEvent *p = &events[i];
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
            int mbtowcRet = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buf, -1, nullptr, 0);
            if (mbtowcRet < 0) {
                DEBUGOUT("'MultiByteToWideChar' returns %d\n", mbtowcRet);
                break;
            }
            unsigned long wcharLengthWithNull = (unsigned long)mbtowcRet;
            if (wcharLengthWithNull > SIZE_MAX / sizeof(WCHAR)) {
                wcharLengthWithNull = SIZE_MAX / sizeof(WCHAR);
            }
            DEBUGOUT("wcharLengthWithNull = %d, maxWcharLengthWithNull = %d\n", wcharLengthWithNull, maxWcharLengthWithNull);
            if (wcharLengthWithNull > maxWcharLengthWithNull) {
                size_t newSize = wcharLengthWithNull * sizeof(WCHAR);
                if (heapWbuf == nullptr) {
                    heapWbuf = (WCHAR *)malloc(newSize);
                } else {
                    heapWbuf = (WCHAR *)realloc(heapWbuf, newSize);
                }
                if (heapWbuf == nullptr) {
                    ret = -1;
                    goto finish;
                }
                wbuf = heapWbuf;
                maxWcharLengthWithNull = (unsigned long)wcharLengthWithNull;
            }
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buf, -1, wbuf, wcharLengthWithNull);
            (saSetFunc[i])(pFileMP3, wbuf);
#else
            (saSetFunc[i])(pFileMP3, buf);
#endif
        }
    }

finish:
    if (heapBuf != nullptr) free(heapBuf);
#ifdef STEP_K
    if (heapWbuf != nullptr) free(heapWbuf);
#endif

    return ret;
}