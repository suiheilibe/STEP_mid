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
        errno_t err = _tfopen_s(&fp, _T("test.mid"), _T("rb"));
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
    CHECK(SMFUtil::findMetaEvents(fp, events) == 0);
}

int main(int argc, char *argv[])
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
