#include <windows.h>

#include <stdio.h>
#include <string.h>

#include "STEPlugin.h"

#include "SMFUtil.h"

#include "resource.h"

#ifdef UNICODE
#ifdef _WIN32
#define fopen _wfopen
#endif
#endif

#define PLUGINNAME TEXT("STEP_mid")
#define META_BUFFER_SIZE 1024

static UINT nPluginID;
static UINT nFileTypeMID;

static void (*saSetFunc[])(FILE_INFO*,LPCTSTR) = {SetComment, SetArtistName, SetTrackName};
static LPCTSTR (*saGetFunc[])(FILE_INFO*) = {GetComment, GetArtistName, GetTrackName};

static HINSTANCE hDLL;
static TCHAR plugininfo[256];

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        hDLL = hinstDLL;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void readMetaEvent(FILE_INFO *pFileMP3, FILE *fp, MetaEvent *events, int type)
{
    MetaEvent *p = &events[type];
    TCHAR buf[META_BUFFER_SIZE];
    if ( p->offset >= 0 )
    {
        int length = p->length;
        if (length >= META_BUFFER_SIZE) {
            length = META_BUFFER_SIZE - 1;
        }
        fseek(fp, p->offset, SEEK_SET);
        fread(buf, length, 1, fp);
        buf[length] = '\0';
        (saSetFunc[type])(pFileMP3, buf);
    }
}

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
    return plugininfo;
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
    if (Initialize() == false)	return false;
    nPluginID = pID;

    LoadString(hDLL, IDS_PLUGININFO, plugininfo, sizeof(plugininfo)/sizeof(plugininfo[0]));
    // INIファイルの読み込み
    /*
    strINI = szPluginFolder;
    strINI += "STEP_ogg.ini";
    bOptGenreListSelect = GetPrivateProfileInt("OGGVorbis", "GenreListSelect", 0, strINI) ? true : false;

    HBITMAP hOGGBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_OGG));
    nFileTypeOGG = STEPRegisterExt(nPluginID, "ogg", hOGGBitmap);
    DeleteObject(hOGGBitmap);*/

    nFileTypeMID = STEPRegisterExt(nPluginID, TEXT("mid"), NULL);
    return true;
}

STEP_API void WINAPI STEPFinalize()
{
    Finalize();
}

STEP_API UINT WINAPI STEPGetAPIVersion(void)
{
    return STEP_API_VERSION;
}

STEP_API LPCTSTR WINAPI STEPGetPluginName(void)
{
    return PLUGINNAME;
}

STEP_API bool WINAPI STEPSupportSIF(UINT nFormat)
{
    return false;
}
/*
STEP_API bool WINAPI STEPSupportTrackNumberSIF(UINT nFormat)
{
    return true;
}
*/
STEP_API CONTROLTYPE WINAPI STEPGetControlType(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF)
{
    if (!isEditSIF)
    {
        return _NULL;
    }
    switch (nColumn)
    {
    case COLUMN_TRACK_NAME:
    case COLUMN_ARTIST_NAME:
        return _EDIT;
    case COLUMN_COMMENT:
        return _MEDIT;
    default:
        break;
    }
    return _NULL;
}

STEP_API UINT WINAPI STEPGetColumnMax(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF)
{
    switch (nColumn)
    {
    case COLUMN_TRACK_NAME:
        return META_BUFFER_SIZE - 1;
    case COLUMN_ARTIST_NAME:
        return META_BUFFER_SIZE - 1;
    case COLUMN_COMMENT:
        return META_BUFFER_SIZE - 1;
    default:
        break;
    }
    return 0;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
    FILE *fp = fopen(GetFullPath(pFileMP3), TEXT("rb"));
    if ( !fp )
    {
        return STEP_ERROR;
    }
    MetaEvent events[sizeof(MetaEvent) * META_MAX];
    memset(events, 0, sizeof(events));
    if ( !events )
    {
        fclose(fp);
        return STEP_ERROR;
    }
    UINT ret;
    if ( SMFUtil::findMetaEvents(fp, events) )
    {
        int i;
        for (i = 0; i < META_MAX; i++)
        {
            readMetaEvent(pFileMP3, fp, events, i);
        }
        SetFormat(pFileMP3, nFileTypeMID);
        SetFileTypeName(pFileMP3, TEXT("MIDI"));
        ret = STEP_SUCCESS;
    }
    else
    {
        ret = STEP_UNKNOWN_FORMAT;
    }
    fclose(fp);
    return ret;

    /*
    if (stricmp(szExt, "ogg") == 0)
    {
        extern	bool LoadAttributeFileOGG(FILE_INFO *pFile);
        if (LoadAttributeFileOGG(pFileMP3) == false)
        {
            CString	strMsg;
            strMsg.Format("%s の読み込みに失敗しました", GetFullPath(pFileMP3));
            MessageBox(NULL, strMsg, "OggVorbisファイルの読み込み失敗", MB_ICONSTOP|MB_OK|MB_TOPMOST);
            return STEP_ERROR;
        }
        else
        {
            SetFormat(pFileMP3, nFileTypeOGG);
            SetFileTypeName(pFileMP3, "OggVorbis");
            return STEP_SUCCESS;
        }
    }
    return STEP_UNKNOWN_FORMAT;*/
}

STEP_API UINT WINAPI STEPSave(FILE_INFO *pFileMP3)
{
    /*
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    UINT nFormat = GetFormat(pFileMP3);

    if (nFormat == nFileTypeOGG)
    {
        extern bool WriteAttributeFileOGG(FILE_INFO *pFileMP3);
        if (WriteAttributeFileOGG(pFileMP3) == false)
        {
            CString	strMsg;
            strMsg.Format("%s の書き込みに失敗しました", GetFullPath(pFileMP3));
            MessageBox(NULL, strMsg, "OggVorbisファイルの書き込み失敗", MB_ICONSTOP|MB_OK|MB_TOPMOST);
            return STEP_ERROR;
        }
        return STEP_SUCCESS;
    }
    */
    return STEP_UNKNOWN_FORMAT;
}

STEP_API void WINAPI STEPShowOptionDialog(HWND hWnd)
{
    /*
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CDlgSetup dlg1;
    CPropertySheet page;
    dlg1.m_bGenreListSelect = bOptGenreListSelect;
    page.AddPage(&dlg1);
    page.SetTitle(CString(STEPGetPluginName()) + " オプション設定");
    if (page.DoModal() == IDOK) {
    	bOptGenreListSelect = dlg1.m_bGenreListSelect ? true : false;

    	WritePrivateProfileString("OGGVorbis", "GenreListSelect", bOptGenreListSelect ? "1" : "0", strINI);
    }
    */
}


/*
STEP_API LPCTSTR WINAPI STEPGetToolTipText(UINT nID)
{
	return NULL;
}
*/

/*
STEP_API LPCTSTR WINAPI STEPGetStatusMessage(UINT nID)
{
	return NULL;
}
*/

/*
STEP_API bool WINAPI STEPOnUpdateCommand(UINT nID)
{
	return false;
}
*/

/*
STEP_API bool WINAPI STEPOnCommand(UINT nID, HWND hWnd)
{
	return false;
}
*/

/*
STEP_API void WINAPI STEPOnLoadMenu(HMENU hMenu, UINT nType)
{
}
*/

/*
STEP_API LPCTSTR WINAPI STEPGetColumnName(UINT nFormatType, COLUMNTYPE nColumn)
{
	return NULL;
}

STEP_API bool WINAPI STEPHasSpecificColumnName(UINT)
{
	return true;
}
*/

STEP_API void WINAPI STEPInitFileSpecificInfo(FILE_INFO* pFileMP3)
{
    /*
       CTag_Ogg* fileOGG = (CTag_Ogg*)GetFileSpecificInfo(pFileMP3);
       if (fileOGG != NULL)	delete fileOGG;*/
    void* p = GetFileSpecificInfo(pFileMP3);
    free(p);
}

