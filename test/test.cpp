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
    CHECK(SMFUtil::findMetaEvents(fp, events));
}

int main(int argc, char *argv[])
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}

/*
class STEPMidFixture: public hayai::Fixture
{
public:
    virtual void SetUp()
    {
        this->fp = fopen("test.mid", _T("rb"));
        this->events = (MetaEvent *)malloc(sizeof(MetaEvent) * META_MAX);
        SMFUtil::findMetaEvents(this->fp, this->events);
    }
    
    virtual void TearDown()
    {
        free(this->events);
        fclose(this->fp);
    }
    
    FILE *fp;
    MetaEvent *events;
};

BENCHMARK_F(STEPMidFixture, readMetaEvent_0, 10, 100)
{
    readMetaEvent(NULL, fp, events, 0);
}

BENCHMARK_F(STEPMidFixture, readMetaEvent_1, 10, 100)
{
    readMetaEvent(NULL, fp, events, 1);
}

BENCHMARK_F(STEPMidFixture, readMetaEvent_2, 10, 100)
{
    readMetaEvent(NULL, fp, events, 2);
}

BENCHMARK(STEP_mid, STEPLoad, 10, 100)
{
    STEPLoad(NULL, "");
}
*/
