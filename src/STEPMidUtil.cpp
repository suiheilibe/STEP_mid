#include <windows.h>
#include <tchar.h>

#include <stdio.h>
#include <stdlib.h>

#include "STEPlugin.h"

#include "SMFUtil.h"
#include "STEPMidUtil.h"

static void (*saSetFunc[])(FILE_INFO*,LPCTSTR) = {SetComment, SetArtistName, SetTrackName};
static LPCTSTR (*saGetFunc[])(FILE_INFO*) = {GetComment, GetArtistName, GetTrackName};

static char staticBuf[STEPMidUtil::STATIC_META_BUFFER_SIZE];
#ifdef STEP_K
static WCHAR staticWbuf[STEPMidUtil::STATIC_META_BUFFER_SIZE];
#endif

static long getMetaEventMaxLength(SMFUtil::MetaEvent *events)
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
    long maxLength = getMetaEventMaxLength(events);
    long minBufSize = maxLength + 1;
    char *buf, *heapBuf = nullptr;
#ifdef STEP_K
    WCHAR *wbuf, *heapWbuf = nullptr;
#endif
    if (minBufSize > STATIC_META_BUFFER_SIZE) {
        heapBuf = (char *)malloc(minBufSize);
        if (heapBuf == nullptr) {
            ret = false;
            goto finish;
        }
#ifdef STEP_K
        heapWbuf = (WCHAR *)malloc(minBufSize * sizeof(WCHAR));
        if (heapWbuf == nullptr) {
            ret = false;
            goto finish;
        }
#endif
    } else {
        buf = staticBuf;
#ifdef STEP_K
        wbuf = staticWbuf;
#endif
    }

    for (int i = 0; i < SMFUtil::META_MAX; i++)
    {
        SMFUtil::MetaEvent *p = &events[i];
        if ( p->offset >= 0 )
        {
            long length = p->length;
            fseek(fp, p->offset, SEEK_SET);
            fread(buf, length, 1, fp);
            buf[length] = '\0';
#ifdef STEP_K
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buf, length + 1, wbuf, STATIC_META_BUFFER_SIZE);
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