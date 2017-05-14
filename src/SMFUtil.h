#ifndef _SMFUTIL_H
#define _SMFUTIL_H

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
        long trkLenOffset;
        // 先頭からのアドレス
        long offset;
        long length;
    };
    static const int SIG_SIZE = 4;
    static bool findMetaEvents(FILE *fp, MetaEvent *events);
};

#endif
