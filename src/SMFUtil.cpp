#include <stdio.h>
#include <string.h>

#include "SMFUtil.h"

#include "debug.h"

static int readVLV(FILE* const fp, unsigned long* const ret)
{
    unsigned long val = 0;
    int c;
    int f;
    int i;

    c = fgetc(fp);
    f = c & 0x7f;
    for (i = 1; ; i++) {
        val = (val << 7) | (c & 0x7f);
        if ( (c & 0x80) == 0 || c == EOF ) {
            break;
        }
        c = fgetc(fp);
    }
    DEBUGOUT("offset = %#010x, val = %d, i = %d, f = %d\n", ftell(fp) - i, val, i, f);
    // The decode result should be lower than 32 bits
    if ( (i == 5 && f > 0x07) || i > 5 ) {
        DEBUGOUT("Error\n");
        return -1;
    }

    if (ret != nullptr) {
        *ret = val;
    }

    return 0;
}

static void writeDelta(FILE* const fp, const long val)
{
    // Less than or equal to 0 is 0
    if ( val <= 0 ) {
        fputc(0, fp);
        return;
    }

    int a[5];
    int i;

    {
        long x = val;
        for (i = 0; i < 5; i++) {
            a[i] = x & 0x7f;
            x >>= 7;
            if (x == 0) {
                break;
            }
        }
    }

    for (; i>= 0; i--) {
        if ( i == 0 ) {
            fputc(a[i], fp);
        } else {
            fputc(a[i] | 0x80, fp);
        }
    }
}

static int fseekULong(FILE* const stream, const unsigned long offset, const int origin)
{
    if (offset > LONG_MAX) {
        DEBUGOUT("Offset excesses 'LONG_MAX'\n");
        return -1;
    }
    return fseek(stream, (long)offset, origin);
}

static int getNumMessageBytes(const int status, unsigned long* const ret)
{
    int index = ((status & 0xf0) >> 4) - 8;
    if (status < 0 || index < 0 || 6 < index) {
        DEBUGOUT("Invalid status: status = %#04x\n", status);
        return -1;
    }
    if (ret != nullptr) {
        // 8n, 9n, an, bn, cn, dn, en
        const static unsigned long msgByteLength[] = { 2, 2, 2, 2, 1, 1, 2 };
        *ret = msgByteLength[index];
    }
    return 0;
}

int SMFUtil::findMetaEvents(FILE* const fp, MetaEvent* const events) {
    char sigBuf[SIG_SIZE + 1];
    int curTrack = 0;
    long trkLenOffset = 0;
    bool baFound[META_MAX]; // baFound[<meta event type>] == true : Corresponding meta event is found
    int runningStatus;      // For dealing with running status rule

    // SMF header check
    if (fread(sigBuf, sizeof(char), SIG_SIZE, fp) != SIG_SIZE) {
        return -1;
    }
    sigBuf[SIG_SIZE] = '\0';
    if (strncmp((const char *)sigBuf, "MThd", 4)) {
        return -1;
    }

    if (fseek(fp, 10, SEEK_CUR)) {
        return -2;
    }
    // Initialize
    for (int i = 0; i < META_MAX; i++) {
        events[i].offset = -1;
        baFound[i] = false;
    }
    for (;;) {
        runningStatus = -1;
        DEBUGOUT("Reading MTrk: offset = %#010x\n", ftell(fp));
        if ( fread(sigBuf, sizeof(char), SIG_SIZE, fp) != SIG_SIZE || feof(fp) ) {
            break;
        }
        sigBuf[SIG_SIZE] = '\0';
        if ( strncmp((const char *)sigBuf, "MTrk", SIG_SIZE) ) {
            // Allow junk data which follows an end of track
            break;
        }
        trkLenOffset = ftell(fp); // Position of the track length
        if (trkLenOffset < 0) {
            return -2;
        }
        // Skip size
        if ( fseek(fp, 4, SEEK_CUR) ) {
            return -2;
        }
        for (;;) {
            unsigned long seekOffset = 0;
            // Skip delta time
            if ( readVLV(fp, nullptr) < 0 ) {
                return -1;
            }
            int c = fgetc(fp);
            if ( c == EOF ) {
                break;
            }
            if ( c & 0x80 ) {
                if ( (c & 0xf0) == 0xf0 ) {
                    if ( c == 0xff ) {
                        // Meta event
                        int type = fgetc(fp);
                        DEBUGOUT("Meta event starts: offset = %#010x, type = %#04x\n", ftell(fp) - 2, type);
                        unsigned long length;
                        if (readVLV(fp, &length) < 0) {
                            return -1;
                        }
                        DEBUGOUT("Meta event length: %d\n", length);
                        if ( type >= 1 && type <= 3 ) {
                            // Text (as Comment), Copyright (as Composer), Sequence name (as Track mame)
                            // Treat a "Sequence name" event as "Track name" only if it appears on the first track
                            if ( type != 3 || curTrack == 0 ) {
                                int index = type - 1;
                                MetaEvent *p = &events[index];
                                if ( baFound[index] == false ) {
                                    long offset = ftell(fp);
                                    if (offset < 0) {
                                        return -2;
                                    }
                                    p->trkLenOffset = trkLenOffset;
                                    p->offset = offset;
                                    p->length = length;
                                }
                                baFound[index] = true;
                            }
                        }
                        else if ( type == 0x2f ) {
                            DEBUGOUT("End of track %d\n", curTrack + 1);
                            // End of track
                            break;
                        }
                        seekOffset = length;
                    } else {
                        DEBUGOUT("SysEx starts: offset = %#010x, status = %#04x\n", ftell(fp) - 1, runningStatus);
                        // Skip any SysEx
                        unsigned long length;
                        if (readVLV(fp, &length) < 0) {
                            return -1;
                        }
                        DEBUGOUT("SysEx length: %d\n", length);
                        seekOffset = length;
                    }
                } else {
                    unsigned long n;
                    runningStatus = c;
                    if ( getNumMessageBytes(runningStatus, &n) < 0 ) {
                        // Impossible
                        return -1;
                    }
                    DEBUGOUT("Channel message: offset = %#010x, status = %#04x, size = %d\n", ftell(fp) - 1, runningStatus, n + 1);
                    seekOffset = n;
                }
            } else {
                unsigned long n;
                if ( getNumMessageBytes(runningStatus, &n) < 0 ) {
                    // Invalid runningStatus which violates the running status rule
                    return -1;
                }
                DEBUGOUT("Channel message (RS): offset = %#010x, status = %#04x, size = %d\n", ftell(fp) - 1, runningStatus, n);
                seekOffset = n - 1;
            }
            if ( fseekULong(fp, seekOffset, SEEK_CUR) ) {
                return -2;
            }
        }
        // All target meta events except the sequence name are found on the first track
        if ( baFound[META_COMMENT] && baFound[META_COPYRIGHT] ) {
            break;
        }
        curTrack++;
    }

    return 0;
}
