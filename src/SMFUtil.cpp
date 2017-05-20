#include <stdio.h>
#include <string.h>

#include "SMFUtil.h"

#include "debug.h"

static int readVLV(FILE *fp, unsigned long *ret)
{
    unsigned long val = 0;
    int c;
    int f;
    int i;

    c = fgetc(fp);
    f = c & 0x7f;
    for (i = 1; ; i++)
    {
        val = (val << 7) | (c & 0x7f);
        if ( (c & 0x80) == 0 || c == EOF )
        {
            break;
        }
        c = fgetc(fp);
    }
    DEBUGOUT("offset = %#010x, val = %d, i = %d, f = %d\n", ftell(fp) - i, val, i, f);
    // 32ビット以上なら不正扱い
    if ( (i == 5 && f > 0x07) || i > 5 )
    {
        DEBUGOUT("Error\n");
        return -1;
    }

    if (ret != nullptr) {
        *ret = val;
    }

    return 0;
}

static void writeDelta(FILE *fp, long val)
{
    // 0以下は0扱い
    if ( val <= 0 )
    {
        fputc(0, fp);
        return;
    }
    {
        int a[5];
        int i;

        for (i = 0; i < 5; i++)
        {
            a[i] = val & 0x7f;
            val >>= 7;
            if ( val == 0 )
            {
                break;
            }
        }
        for (; i>= 0; i--)
        {
            if ( i == 0 )
            {
                fputc(a[i], fp);
            }
            else
            {
                fputc(a[i] | 0x80, fp);
            }
        }
    }
}

static int fseekULong(FILE *stream, unsigned long offset, int origin)
{
    if (offset > LONG_MAX) {
        DEBUGOUT("Offset excesses 'LONG_MAX'\n");
        return -1;
    }
    return fseek(stream, (long)offset, origin);
}

static int getNumMessageBytes(int status, unsigned long *ret)
{
    int index = ((status & 0xf0) >> 4) - 8;
    if (status < 0 || index < 0 || 6 < index) {
        DEBUGOUT("Invalid status: status = %#04x\n", status);
        return -1;
    }
    // 8n, 9n, an, bn, cn, dn, en
    *ret = (const unsigned long[]){ 2, 2, 2, 2, 1, 1, 2 }[index];
    return 0;
}

int SMFUtil::findMetaEvents(FILE *fp, MetaEvent *events)
{
    char buf[SIG_SIZE];
    int curTrack = 0;
    long trkLenOffset = 0;
    bool baFound[META_MAX];// メタイベントが見つかったかどうか
    int runningStatus; // For dealing with running status rule
    // ヘッダ
    if (fread(buf, sizeof(char), SIG_SIZE, fp) < SIG_SIZE) {
        return -1;
    }
    if ( strncmp((const char *)buf, "MThd", 4) )
    {
        return -1;
    }
    if (fseek(fp, 10, SEEK_CUR)) {
        return -2;
    }
    // 初期化
    for (int i = 0; i < META_MAX; i++)
    {
        events[i].offset = -1;
        baFound[i] = false;
    }
    for (;;)
    {
        runningStatus = -1;
        DEBUGOUT("Reading MTrk: offset = %#010x\n", ftell(fp));
        if ( fread(buf, sizeof(char), SIG_SIZE, fp) < SIG_SIZE || feof(fp) )
        {
            // ここで終了することがない？
            break;
        }
        if ( strncmp((const char *)buf, "MTrk", SIG_SIZE) )
        {
            // Allow junk data which follows an end of track
            break;
        }
        trkLenOffset = ftell(fp);// トラックのサイズを書く場所
        if (trkLenOffset < 0) {
            return -2;
        }
        // サイズは無視
        if (fseek(fp, 4, SEEK_CUR)) {
            return -2;
        }
        for (;;)
        {
            unsigned long seekOffset = 0;
            // デルタタイムは読み飛ばす
            if ( readVLV(fp, nullptr) < 0 )
            {
                return -1;
            }
            int c = fgetc(fp);
            if ( c == EOF )
            {
                break;// 本来ここで終了するのはエラーのはずだが
            }
            if (c & 0x80)
            {
                if ( (c & 0xf0) == 0xf0 )
                {
                    if ( c == 0xff )
                    {
                        // メタイベント
                        int type = fgetc(fp);
                        DEBUGOUT("Meta event starts: offset = %#010x, type = %#04x\n", ftell(fp) - 2, type);
                        unsigned long length;
                        if (readVLV(fp, &length) < 0) {
                            return -1;
                        }
                        DEBUGOUT("Meta event length: %d\n", length);
                        if ( type >= 1 && type <= 3 )
                        {
                            // テキスト(コメント), 著作権(作曲者), シーケンス名(曲名)
                            // 最初のトラックでなければ曲名として扱わない
                            if ( type != 3 || curTrack == 0 )
                            {
                                int index = type - 1;
                                MetaEvent *p = &events[index];
                                if ( baFound[index] == false )
                                {
                                    p->length = length;
                                    p->offset = ftell(fp);
                                    p->trkLenOffset = trkLenOffset;
                                }
                                baFound[index] = true;
                                if ( baFound[META_COMMENT] && baFound[META_COPYRIGHT] && baFound[META_SEQNAME] )
                                {
                                    return 0;
                                }
                            }
                        }
                        else if ( type == 0x2f )
                        {
                            DEBUGOUT("End of track %d\n", curTrack + 1);
                            // エンドオブトラック
                            break;
                        }
                        seekOffset = length;
                    }
                    else
                    {
                        DEBUGOUT("SysEx starts: offset = %#010x, status = %#04x\n", ftell(fp) - 1, runningStatus);
                        // SysExは読み飛ばす
                        unsigned long length;
                        if (readVLV(fp, &length) < 0) {
                            return -1;
                        }
                        DEBUGOUT("SysEx length: %d\n", length);
                        seekOffset = length;
                    }
                }
                else
                {
                    unsigned long n;
                    runningStatus = c;
                    if (getNumMessageBytes(runningStatus, &n) < 0) {
                        // Impossible
                        return -1;
                    }
                    DEBUGOUT("Channel message: offset = %#010x, status = %#04x, size = %d\n", ftell(fp) - 1, runningStatus, n + 1);
                    seekOffset = n;
                }
            }
            else
            {
                unsigned long n;
                if (getNumMessageBytes(runningStatus, &n) < 0) {
                    // Invalid runningStatus which violates the running status rule
                    return -1;
                }
                DEBUGOUT("Channel message (RS): offset = %#010x, status = %#04x, size = %d\n", ftell(fp) - 1, runningStatus, n);
                seekOffset = n - 1;
            }
            if (fseekULong(fp, seekOffset, SEEK_CUR)) {
                return -2;
            }
        }
        // 最初のトラックでシーケンス名以外全て見つかった
        if ( baFound[META_COMMENT] && baFound[META_COPYRIGHT] )
        {
            break;
        }
        curTrack++;
    }
    return 0;
}
