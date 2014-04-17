#include <windows.h>

#include <stdio.h>
#include <string.h>

#include "STEPlugin.h"

using namespace std;

enum MetaEventType
{
    META_COMMENT = 0,
    META_COPYRIGHT,
    META_SEQNAME,
    META_MAX,
};

// 構造体
typedef struct _MetaEvent
{
    // トラックの長さのアドレス
    int trkLenOffset;
    int offset;
    int length;
} MetaEvent;

static UINT nPluginID;
static UINT nFileTypeMID;
static const char *spMidFileType = "MThd";

static void (*saSetFunc[])(FILE_INFO*,LPCSTR) = {SetComment, SetArtistName, SetTrackName};

void readMetaEvent(FILE_INFO *pFileMP3, FILE *fp, MetaEvent *events, int type)
{
    MetaEvent *p = &events[type];
    if (p->offset >= 0)
    {
        int length = p->length;
        char *buf = (char *)malloc(length + 1);
        if (!buf)
        {
            return;
        }
        fseek(fp, p->offset, SEEK_SET);
        fread(buf, length, 1, fp);
        buf[length] = '\0';
        (saSetFunc[type])(pFileMP3, buf);
        free(buf);
    }
}

int varVal2Int(FILE *fp)
{
    int val = 0;
    int c;
    int f;
    int i;

    c = fgetc(fp);
    f = c & 0x7f;
    for (i = 0; ; i++)
    {
        val = (val << 7) | (c & 0x7f);
        if ((c & 0x80) == 0 || c == EOF)
        {
            break;
        }
        c = fgetc(fp);
    }
    // 32ビット以上なら不正扱い
    if ((i == 4 && f > 0x07) || i > 4)
    {
        return -1;
    }
    return val;
}

void int2VarVal(FILE *fp, int val)
{
    // 0以下は0扱い
    if (val <= 0)
    {
        fputc(0, fp);
        return;
    }
    {
        int a[5];
        int i;

        for (i = 0; ; i++)
        {
            a[i] = val & 0x7f;
            val >>= 7;
            if (val == 0)
            {
                break;
            }
        }
        for (; i>= 0; i--)
        {
            if (i == 0)
            {
                fputc(a[i], fp);
            }
            else
            {
                fputc(a[i] | 0x80, fp);
            }
        }
    }
}

// 最初のトラックの先頭にファイルポインタがあるときに実行
static MetaEvent* findThreeMetaEvents(FILE *fp)
{
    int i;
    char buf[4];
    int curTrack = 0;
    int trkLenOffset = 0;
    int c;
    // ヘッダ
    fread(buf, 4, 1, fp);
    if (strncmp((const char *)buf, spMidFileType, 4))
    {
        return NULL;
    }
    fseek(fp, 10, SEEK_CUR);
    MetaEvent *event = (MetaEvent *)malloc(sizeof(MetaEvent) * 3);
    if (!event)
    {
        return NULL;
    }
    // 初期化
    for (i = 0; i < META_MAX; i++)
    {
        event[i].offset = -1;
    }
    for (;;)
    {
        if (fread(buf, 4, 1, fp) < 1 || feof(fp))
        {
            // ここで終了することがない？
            break;
        }
        if (strncmp((const char *)buf, "MTrk", 4))
        {
            free(event);
            return NULL;
        }
        trkLenOffset = ftell(fp);
        // サイズは無視
        fseek(fp, 4, SEEK_CUR);
        for (;;)
        {
            // デルタタイムは読み飛ばす
            varVal2Int(fp);

            c = fgetc(fp);
            if (c == EOF)
            {
                // 本来ここで終了するのはエラーのはずだが
                break;
            }
            if (c & 0x80)
            {
                if (c & 0xf0)
                {
                    if (c == 0xff)
                    {
                        // メタイベント
                        int type = fgetc(fp);
                        int length = varVal2Int(fp);
                        if (length < 0)
                        {
                            free(event);
                            return NULL;
                        }

                        if (type >= 1 && type <= 3)
                        {
                            // テキスト(コメント), 著作権(作曲者), シーケンス名(曲名)
                            // 最初のトラックでなければ曲名として扱わない
                            if (type != 3 || curTrack == 0)
                            {
                                MetaEvent *p = &event[type -1];
                                if (p->offset < 0)
                                {
                                    p->length = length;
                                    p->offset = ftell(fp);
                                    p->trkLenOffset = trkLenOffset;
                                }
                            }
                        }
                        else if (type == 0x2f)
                        {
                            // エンドオブトラック
                            break;
                        }
                        // length分読み飛ばし
                        fseek(fp, length, SEEK_CUR);
                    }
                    else
                    {
                        // SysExは読み飛ばす
                        int length = varVal2Int(fp);
                        if (length < 0)
                        {
                            free(event);
                            return NULL;
                        }
                        fseek(fp, length, SEEK_CUR);
                    }
                }
                else
                {
                    // 3バイトのチャンネルメッセージ
                    fseek(fp, 2, SEEK_CUR);
                }
            }
            else
            {
                // ランニングステータスルール適用チャンネルメッセージ
                fseek(fp, 1, SEEK_CUR);
            }
        }
        curTrack++;
    }
    return event;
}

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
    return "Version 0.01\r\nMIDIファイルをサポートしています";
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
    if (Initialize() == false)	return false;
    nPluginID = pID;
    // INIファイルの読み込み
    /*
    strINI = szPluginFolder;
    strINI += "STEP_ogg.ini";
    bOptGenreListSelect = GetPrivateProfileInt("OGGVorbis", "GenreListSelect", 0, strINI) ? true : false;

    HBITMAP hOGGBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_OGG));
    nFileTypeOGG = STEPRegisterExt(nPluginID, "ogg", hOGGBitmap);
    DeleteObject(hOGGBitmap);*/

    nFileTypeMID = STEPRegisterExt(nPluginID, "mid", NULL);
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
    return "STEP_mid";
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
        return 1024;
    case COLUMN_ARTIST_NAME:
        return 1024;
    case COLUMN_COMMENT:
        return 1024;
    default:
        break;
    }
    return 0;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
    FILE *fp = fopen(GetFullPath(pFileMP3), "rb");
    if (!fp)
    {
        return STEP_ERROR;
    }
    MetaEvent *events = findThreeMetaEvents(fp);
    if (events)
    {
        int i;
        for (i = 0; i < META_MAX; i++)
        {
            readMetaEvent(pFileMP3, fp, events, i);
        }
        fclose(fp);
        SetFormat(pFileMP3, nFileTypeMID);
        SetFileTypeName(pFileMP3, "MIDI");
        return STEP_SUCCESS;
    }
    fclose(fp);
    return STEP_UNKNOWN_FORMAT;

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

