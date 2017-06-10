#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <tchar.h>

#include "UnitTest++/UnitTest++.h"

#include "STEPlugin.h"

#include "SMFUtil.h"

extern void readMetaEvent(FILE_INFO *pFileMP3, FILE *fp, SMFUtil::MetaEvent *events, int type);

SUITE(SMFUtilTest)
{
    TEST(findMetaEvents)
    {
        FILE *fp;
        SMFUtil::MetaEvent *events;
        errno_t err = _tfopen_s(&fp, _T("test.mid"), _T("rb"));
        events = (SMFUtil::MetaEvent *)malloc(sizeof(SMFUtil::MetaEvent) * SMFUtil::META_MAX);
        CHECK_EQUAL(0, SMFUtil::findMetaEvents(fp, events));
        free(events);
        fclose(fp);
    }
}

int main(int argc, char *argv[])
{
    return UnitTest::RunAllTests();
}
