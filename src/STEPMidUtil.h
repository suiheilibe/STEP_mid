#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>

#include "STEPlugin.h"

#include "SMFUtil.h"

class STEPMidUtil
{
public:
    static const int STATIC_META_BUFFER_SIZE = 1024;
    static bool readMetaEvent(FILE_INFO *pFileMP3, FILE *fp, SMFUtil::MetaEvent *events);
};

#endif