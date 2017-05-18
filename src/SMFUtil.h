#ifndef _STEP_MID_SMFUTIL_H_
#define _STEP_MID_SMFUTIL_H_

class SMFUtil
{
public:
    enum MetaEventType
    {
        META_COMMENT = 0,
        META_COPYRIGHT,
        META_SEQNAME,
        META_MAX,
    };
    struct MetaEvent
    {
        // トラックの長さのアドレス
        unsigned long trkLenOffset;
        // 先頭からのアドレス
        unsigned long offset;
        unsigned long length;
    };
    static const int SIG_SIZE = 4;
    static int findMetaEvents(FILE *fp, MetaEvent *events);
};

#endif
