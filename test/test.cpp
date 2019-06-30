#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <tchar.h>

#include "CppUTest/CommandLineTestRunner.h"

#include "STEPlugin.h"

#include "SMFUtil.h"

extern void readMetaEvent(FILE_INFO *pFileMP3, FILE *fp, SMFUtil::MetaEvent *events, int type);

TEST_GROUP(SMFUtilTestGroup)
{
    void setup()
    {
        errno_t err = _tfopen_s(&fp, _T("test_metaevents.mid"), _T("rb"));
        events = (SMFUtil::MetaEvent *)malloc(sizeof(SMFUtil::MetaEvent) * SMFUtil::META_MAX);
    }

    void teardown()
    {
        free(events);
        fclose(fp);
    }
    
    FILE *fp;
    SMFUtil::MetaEvent *events;
};

TEST(SMFUtilTestGroup, findMetaEvents)
{
    CHECK_EQUAL(0, SMFUtil::findMetaEvents(fp, events));
    // Text Event
    CHECK_EQUAL(542, events[SMFUtil::MetaEventType::META_COMMENT].length);
    // Copyright Notice
    CHECK_EQUAL(1553, events[SMFUtil::MetaEventType::META_COPYRIGHT].length);
    // Sequence/Track Name
    CHECK_EQUAL(16361, events[SMFUtil::MetaEventType::META_SEQNAME].length);
}

int main(int argc, char *argv[])
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
