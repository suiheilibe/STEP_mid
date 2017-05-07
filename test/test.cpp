#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <tchar.h>

#include "CppUTest/CommandLineTestRunner.h"

#include "STEPlugin.h"

#include "SMFUtil.h"

extern void readMetaEvent(FILE_INFO *pFileMP3, FILE *fp, MetaEvent *events, int type);

TEST_GROUP(SMFUtilTestGroup)
{
    void setup()
    {
        fp = _tfopen(TEXT("test.mid"), TEXT("rb"));
        events = (MetaEvent *)malloc(sizeof(MetaEvent) * META_MAX);
    }
    
    void teardown()
    {
        free(events);
        fclose(fp);
    }
    
    FILE *fp;
    MetaEvent *events;
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
        this->fp = fopen("test.mid", TEXT("rb"));
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
