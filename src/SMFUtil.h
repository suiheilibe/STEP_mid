#ifndef _SMFUTIL_H
#define _SMFUTIL_H

enum MetaEventType
{
    META_COMMENT = 0,
    META_COPYRIGHT,
    META_SEQNAME,
    META_MAX,
};

// 構造体
typedef struct _MetaEvent
{
    // トラックの長さのアドレス
    int trkLenOffset;
    // 先頭からのアドレス
    int offset;
    int length;
} MetaEvent;

class SMFUtil
{
public:
    static const int SIG_SIZE = 4;
    static bool findMetaEvents(FILE *fp, MetaEvent *events);
};

#endif
