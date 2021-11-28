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
        long trkLenOffset;    // Position of the track length
        long offset;          // Position of the meta event
        unsigned long length; // Length of the meta event
    };
    static const int SIG_SIZE = 4;
    static int findMetaEvents(FILE* const fp, MetaEvent* const events);
};

#endif
