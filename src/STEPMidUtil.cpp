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

static long getMaxMetaEventLength(SMFUtil::MetaEvent *events)
{
    long maxLength = 0;
    long length;
    for (int i = 0; i < SMFUtil::META_MAX; i++)
    {
        length = events[i].length;
        if (maxLength < length) {
            maxLength = length;
        }
    }

    return maxLength;
}

bool STEPMidUtil::readMetaEvent(FILE_INFO *pFileMP3, FILE *fp, SMFUtil::MetaEvent *events)
{
    bool ret = true;
    long maxLength = getMaxMetaEventLength(events);
    long maxLengthWithNull = maxLength + 1;
    char *buf = staticBuf, *heapBuf = nullptr;
#ifdef STEP_K
    WCHAR *wbuf = staticWbuf, *heapWbuf = nullptr;
    long maxWcharLengthWithNull = maxLengthWithNull;
#endif
    DEBUGOUT("maxLengthWithNull = %d\n", maxLengthWithNull);
    if (maxLengthWithNull > STATIC_META_BUFFER_SIZE) {
        heapBuf = (char *)malloc(maxLengthWithNull * sizeof(char));
        if (heapBuf == nullptr) {
            ret = false;
            goto finish;
        }
        buf = heapBuf;
#ifdef STEP_K
        heapWbuf = (WCHAR *)malloc(maxLengthWithNull * sizeof(WCHAR));
        if (heapWbuf == nullptr) {
            ret = false;
            goto finish;
        }
        wbuf = heapWbuf;
#endif
    }

    for (int i = 0; i < SMFUtil::META_MAX; i++)
    {
        SMFUtil::MetaEvent *p = &events[i];
        if ( p->offset >= 0 )
        {
            long length = p->length;
            fseek(fp, p->offset, SEEK_SET);
            if (fread(buf, sizeof(char), length, fp) < length) {
                ret = false;
                goto finish;
            }
            buf[length] = '\0';
#ifdef STEP_K
            long wcharLengthWithNull = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buf, -1, nullptr, 0);
            DEBUGOUT("wcharLengthWithNull = %d, maxWcharLengthWithNull = %d\n", wcharLengthWithNull, maxWcharLengthWithNull);
            if (wcharLengthWithNull > maxWcharLengthWithNull) {
                long newSize = wcharLengthWithNull * sizeof(WCHAR);
                if (heapWbuf == nullptr) {
                    heapWbuf = (WCHAR *)malloc(newSize);
                } else {
                    heapWbuf = (WCHAR *)realloc(heapWbuf, newSize);
                }
                if (heapWbuf == nullptr) {
                    ret = false;
                    goto finish;
                }
                wbuf = heapWbuf;
                maxWcharLengthWithNull = wcharLengthWithNull;
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