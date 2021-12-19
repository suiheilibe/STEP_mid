#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <tchar.h>

#include "CppUTest/CommandLineTestRunner.h"

#include "STEPlugin.h"

#include "SMFUtil.h"
#include "STEPMidUtil.h"
#include "STEPMidTls.h"
#include "debug.h"

static void my_SetValue(FILE_INFO *fi, LPCTSTR str) {
    DEBUGOUT("FILE_INFO = %p, value = %p\n", fi, str);
}

TEST_GROUP(STEPMidTestGroup) {
    void setup() {
        errno_t err = _tfopen_s(&fp, _T("test_metaevents.mid"), _T("rb"));
        events = (SMFUtil::MetaEvent *)malloc(sizeof(SMFUtil::MetaEvent) * SMFUtil::META_MAX);
        STEPMidTls::initialize();
        STEPMidTls::allocAndSet(sizeof(STEPMidUtil::TlsData));
    }

    void teardown() {
        free(events);
        fclose(fp);
        ptr = STEPMidTls::get();
        if (ptr != nullptr) {
            STEPMidTls::free(ptr);
        }
        STEPMidTls::deinitialize();
    }

    FILE *fp = nullptr;
    SMFUtil::MetaEvent *events = nullptr;
    void (*saSetFunc[SMFUtil::META_MAX])(FILE_INFO *, LPCTSTR) = {my_SetValue, my_SetValue, my_SetValue};
    void *ptr;
};

TEST(STEPMidTestGroup, TestMain) {
    CHECK_EQUAL(0, SMFUtil::findMetaEvents(fp, events));
    // Text Event
    CHECK_EQUAL(542, events[SMFUtil::MetaEventType::META_COMMENT].length);
    // Copyright Notice
    CHECK_EQUAL(1553, events[SMFUtil::MetaEventType::META_COPYRIGHT].length);
    // Sequence/Track Name
    CHECK_EQUAL(16361, events[SMFUtil::MetaEventType::META_SEQNAME].length);
    CHECK_EQUAL(0, STEPMidUtil::readMetaEvent(nullptr, fp, events, saSetFunc));
}

int main(int argc, char *argv[])
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
