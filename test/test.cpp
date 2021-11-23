#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <tchar.h>

#include "CppUTest/CommandLineTestRunner.h"

#include "STEPlugin.h"

#include "SMFUtil.h"
#include "STEPMidUtil.h"
#include "debug.h"

extern "C" void(WINAPI *STEPSetValue)(FILE_INFO *, FIELDTYPE, LPCTSTR);

TEST_GROUP(STEPMidTestGroup) {
    void setup() {
        STEPSetValue = [](FILE_INFO *fi, FIELDTYPE ft, LPCTSTR str) {
            DEBUGOUT("FILE_INFO = %p, FIELDTYPE = %d, value = %p\n", fi, ft, str);
        };
        errno_t err = _tfopen_s(&fp, _T("test_metaevents.mid"), _T("rb"));
        events = (SMFUtil::MetaEvent *)malloc(sizeof(SMFUtil::MetaEvent) * SMFUtil::META_MAX);
    }

    void teardown() {
        free(events);
        fclose(fp);
    }

    FILE *fp = nullptr;
    SMFUtil::MetaEvent *events = nullptr;
};

TEST(STEPMidTestGroup, TestMain) {
    CHECK_EQUAL(0, SMFUtil::findMetaEvents(fp, events));
    // Text Event
    CHECK_EQUAL(542, events[SMFUtil::MetaEventType::META_COMMENT].length);
    // Copyright Notice
    CHECK_EQUAL(1553, events[SMFUtil::MetaEventType::META_COPYRIGHT].length);
    // Sequence/Track Name
    CHECK_EQUAL(16361, events[SMFUtil::MetaEventType::META_SEQNAME].length);
    CHECK_EQUAL(0, STEPMidUtil::readMetaEvent(nullptr, fp, events));
}

int main(int argc, char *argv[])
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
