#ifndef STEP_MID_STEPMIDUTIL_H_
#define STEP_MID_STEPMIDUTIL_H_

#include <stdio.h>

#include "STEPlugin.h"

#include "SMFUtil.h"

class STEPMidUtil
{
public:
    static const int STATIC_META_BUFFER_SIZE = 1024;
    struct TlsData {
        char metadataBuf[STATIC_META_BUFFER_SIZE];
#ifdef STEP_K
        WCHAR metadataWbuf[STATIC_META_BUFFER_SIZE];
#endif
    };
    static int readMetaEvent(FILE_INFO *const pFileMP3, FILE *const fp,
                             const SMFUtil::MetaEvent *const events,
                             void (*saSetFunc[SMFUtil::META_MAX])(FILE_INFO *, LPCTSTR));
};

#endif