#include <stdio.h>
#include <string.h>

#include <windows.h>

#include <hayai/hayai.hpp>

#include "STEPlugin.h"

#include "SMFUtil.h"

extern void readMetaEvent(FILE_INFO *pFileMP3, FILE *fp, MetaEvent *events, int type);

class SMFUtilFixture: public hayai::Fixture
{
public:
    virtual void SetUp()
    {
        this->fp = fopen("test.mid", TEXT("rb"));
        this->events = (MetaEvent *)malloc(sizeof(MetaEvent) * META_MAX);
    }
    
    virtual void TearDown()
    {
        free(this->events);
        fclose(this->fp);
    }
    
    FILE *fp;
    MetaEvent *events;
};

BENCHMARK_F(SMFUtilFixture, findMetaEvents, 10, 100)
{
    findMetaEvents(fp, events);
}

class STEPMidFixture: public hayai::Fixture
{
public:
    virtual void SetUp()
    {
        this->fp = fopen("test.mid", TEXT("rb"));
        this->events = (MetaEvent *)malloc(sizeof(MetaEvent) * META_MAX);
        findMetaEvents(this->fp, this->events);
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

LPCTSTR GetValue(FILE_INFO* pInfo, FIELDTYPE nField)
{
    return "./test.mid";
}

void SetValue(FILE_INFO* pInfo, FIELDTYPE nField, LPCTSTR szValue)
{
}

bool Initialize(void)
{
    return true;
}

void Finalize(void)
{
}

UINT WINAPI _STEPRegisterExt(UINT, LPCTSTR, HBITMAP)
{
    return 0;
}

UINT (WINAPI *STEPRegisterExt)(UINT, LPCTSTR, HBITMAP) = _STEPRegisterExt;

void SetFormat(FILE_INFO* pInfo, UINT nFormat)
{
}

void* GetFileSpecificInfo(FILE_INFO* pInfo) {
	return NULL;
}
