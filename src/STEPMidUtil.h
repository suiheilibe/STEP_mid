#ifndef _STEP_MID_STEPMIDUTIL_H_
#define _STEP_MID_STEPMIDUTIL_H_

#include <stdio.h>

#include "STEPlugin.h"

#include "SMFUtil.h"

class STEPMidUtil
{
public:
    static const int STATIC_META_BUFFER_SIZE = 1024;
    static int readMetaEvent(FILE_INFO* const& pFileMP3, FILE* const& fp, const SMFUtil::MetaEvent * const& events);
};

#endif