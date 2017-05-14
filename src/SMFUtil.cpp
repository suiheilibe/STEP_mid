#include <stdio.h>
#include <string.h>

#include "SMFUtil.h"

#include "debug.h"

static long readDelta(FILE *fp)
{
    long val = 0;
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
    DEBUGOUT(_T("%s: offset = %x, val = %d, i = %d, f = %d\n"),  __func__, ftell(fp) - i, val, i, f);
    // 32ビット以上なら不正扱い
    if ( (i == 5 && f > 0x07) || i > 5 )
    {
        DEBUGOUT(_T("%s: Error\n"),  __func__);
        return -1;
    }
    return val;
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

static int getNumMessageBytes(int status)
{
    int index = ((status & 0xf0) >> 4) - 8;
    if (index < 0 || 6 < index) {
        DEBUGOUT(_T("%s: Invalid status: status = %02x\n"),  __func__, status);
        return -1;
    }
    // 8n, 9n, an, bn, cn, dn, en
    return (const int[]){ 2, 2, 2, 2, 1, 1, 2 }[index];
}

bool SMFUtil::findMetaEvents(FILE *fp, MetaEvent *events)
{
    int i;
    char buf[SIG_SIZE];
    long curTrack = 0;
    long trkLenOffset = 0;
    bool baFound[META_MAX];// メタイベントが見つかったかどうか
    int status; // For dealing with running status rule
    // ヘッダ
    fread(buf, 1, SIG_SIZE, fp);
    if ( strncmp((const char *)buf, "MThd", 4) )
    {
        return false;
    }
    fseek(fp, 10, SEEK_CUR);
    // 初期化
    for (i = 0; i < META_MAX; i++)
    {
        events[i].offset = -1;
        baFound[i] = false;
    }
    for (;;)
    {
        DEBUGOUT(_T("%s: Reading MTrk: offset = %x\n"),  __func__, ftell(fp));
        if ( fread(buf, 1, SIG_SIZE, fp) < SIG_SIZE || feof(fp) )
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
        fseek(fp, 4, SEEK_CUR);// サイズは無視
        for (;;)
        {
            // デルタタイムは読み飛ばす
            if ( readDelta(fp) < 0 )
            {
                return false;
            }
            int c = fgetc(fp);
            if ( c == EOF )
            {
                break;// 本来ここで終了するのはエラーのはずだが
            }
            if (c & 0x80)
            {
                status = c;
                if ( (c & 0xf0) == 0xf0 )
                {
                    if ( c == 0xff )
                    {
                        // メタイベント
                        int type = fgetc(fp);
                        DEBUGOUT(_T("%s: Meta event starts: offset = %x, type = %d, status = %02x\n"),  __func__, ftell(fp) - 2, type, c);
                        long length = readDelta(fp);
                        DEBUGOUT(_T("%s: Meta event length: %d\n"),  __func__, length);
                        if ( length < 0 )
                        {
                            return false;
                        }
                        if ( type >= 1 && type <= 3 )
                        {
                            // テキスト(コメント), 著作権(作曲者), シーケンス名(曲名)
                            // 最初のトラックでなければ曲名として扱わない
                            if ( type != 3 || curTrack == 0 )
                            {
                                MetaEvent *p = &events[type -1];
                                if ( p->offset < 0 )
                                {
                                    p->length = length;
                                    p->offset = ftell(fp);
                                    p->trkLenOffset = trkLenOffset;
                                }
                                baFound[type - 1] = true;
                                if ( baFound[META_COMMENT] && baFound[META_COPYRIGHT] && baFound[META_SEQNAME] )
                                {
                                    return true;
                                }
                            }
                        }
                        else if ( type == 0x2f )
                        {
                            DEBUGOUT(_T("%s: End of track %d\n"),  __func__, curTrack + 1);
                            // エンドオブトラック
                            break;
                        }
                        fseek(fp, length, SEEK_CUR);// length分読み飛ばし
                    }
                    else
                    {
                        DEBUGOUT(_T("%s: SysEx starts: offset = %x, status = %02x\n"),  __func__, ftell(fp) - 1, c);
                        // SysExは読み飛ばす
                        long length = readDelta(fp);
                        DEBUGOUT(_T("%s: SysEx length: %d\n"),  __func__, length);
                        if ( length < 0 )
                        {
                            return false;
                        }
                        //if (c == 0xf7) 
                        fseek(fp, length, SEEK_CUR);
                    }
                }
                else
                {
                    int n = getNumMessageBytes(status);
                    DEBUGOUT(_T("%s: Channel message: offset = %x, status = %02x, size = %d\n"),  __func__, ftell(fp) - 1, c, n + 1);
                    fseek(fp, n, SEEK_CUR);// 3バイトのチャンネルメッセージ
                }
            }
            else
            {
                int n = getNumMessageBytes(status);
                if (n < 0) {
                    // Invalid status which violates the running status rule
                    return false;
                }
                DEBUGOUT(_T("%s: Channel message (RS): offset = %x, status = %02x, size = %d\n"),  __func__, ftell(fp) - 1, c, n);
                fseek(fp, n - 1, SEEK_CUR);// ランニングステータスルール適用チャンネルメッセージ
            }
        }
        // 最初のトラックでシーケンス名以外全て見つかった
        if ( baFound[META_COMMENT] && baFound[META_COPYRIGHT] )
        {
            break;
        }
        curTrack++;
    }
    return true;
}
