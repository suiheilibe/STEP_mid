#ifndef _SMFUTIL_H
#define _SMFUTIL_H

#define SIG_SIZE 4

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

bool findMetaEvents(FILE *fp, MetaEvent *events);

#endif
