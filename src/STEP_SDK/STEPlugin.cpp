//#include "stdafx.h"
#include "windows.h"

#include "STEPlugin.h"

HINSTANCE g_hInstance = NULL;
HINSTANCE hLib = NULL;

/* プラグインから呼び出せる関数 */
extern "C" {
UINT (WINAPI *STEPGetCommandID)(void);
HMENU (WINAPI *STEPGetMenu)(UINT);
void (WINAPI *STEPAddToolBarButton)(HBITMAP, UINT, char*);
UINT (WINAPI *STEPRegisterExt)(UINT, LPCTSTR, HBITMAP);
UINT (WINAPI *STEPKeyAssign)(UINT, LPCTSTR, LPCTSTR);
const char* (WINAPI *STEPGetGenreNameSIF)(BYTE byGenre);
BYTE (WINAPI *STEPGetGenreCode)(const char* szGenre);
bool (WINAPI *STEPIsUserGenre)(const char* szGenre);
int (WINAPI *STEPGetNumericTrackNumber)(const char* szTrackNumber, char* szNumericNumber, int size); /* STEP 037 */
int (WINAPI *STEPGetIntegerTrackNumber)(const char* szTrackNumber); /* STEP 037 */
int (WINAPI *STEPGetNumericDiskNumber)(const char* szDiskNumber, char* szNumericNumber, int size); /* STEP 037 */
int (WINAPI *STEPGetIntegerDiskNumber)(const char* szDiskNumber); /* STEP 037 */

bool (WINAPI *STEPGetBooleanValue)(FILE_INFO*, UINT);
void (WINAPI *STEPSetBooleanValue)(FILE_INFO*, UINT, bool);
long (WINAPI *STEPGetLongValue)(FILE_INFO*, UINT);
void (WINAPI *STEPSetLongValue)(FILE_INFO*, UINT, long);
int (WINAPI *STEPGetIntValue)(FILE_INFO*, UINT);
void (WINAPI *STEPSetIntValue)(FILE_INFO*, UINT, int);
void* (WINAPI *STEPGetVoidValue)(FILE_INFO*, UINT);
void (WINAPI *STEPSetVoidValue)(FILE_INFO*, UINT, void*);
LPCTSTR (WINAPI *STEPGetValue)(FILE_INFO*, FIELDTYPE);
void (WINAPI *STEPSetValue)(FILE_INFO*, FIELDTYPE, LPCTSTR);
void (WINAPI *STEPProcessSelectedFiles)(LPCTSTR, STEPProcessSelectedFilesCB);
void (WINAPI *STEPProcessSelectedFilesForUpdate)(LPCTSTR, STEPProcessSelectedFilesCB);

bool (WINAPI *STEPIsRangeSelected)(void);
int (WINAPI *STEPGetSelectedCount)(void);
int (WINAPI *STEPGetSelectedItem)(void);
bool (WINAPI *STEPIsCurrentCellEditOK)(void);
bool (WINAPI *STEPItemHasChildren)(int);
bool (WINAPI *STEPGetSelectedRange)(int*, int*, int* , int*);
bool (WINAPI *STEPIsItemFile)(int);
void (WINAPI *STEPChangeSubItemText)(int nItem, int nColumn, LPCTSTR szText);
const char* (WINAPI *STEPGetSubItemText)(int nItem, int nColumn);
void (WINAPI *STEPGetFileInfo)(int nItem, FILE_INFO* info);
UINT (WINAPI *STEPGETColumnType)(int nColumn);
bool (WINAPI *STEPIsNumeric)(const char* szText); /* STEP 037 */
void (WINAPI *STEPConvSiFieldToId3tag)(FILE_INFO* pFileInfo); /* STEP 037 */

void (WINAPI *STEPInitDataSIF)(FILE_INFO* info);
void (WINAPI *STEPInitDataID3)(FILE_INFO* info); /* STEP 029 */
void (WINAPI *STEPInitData)(FILE_INFO* info);
bool (WINAPI *STEPFileNameChange)(FILE_INFO* pFileInfo, LPCTSTR szNewFileName);
bool (WINAPI *STEPWriteTag)(FILE_INFO* pFileInfo);

bool (WINAPI *STEPUpdateCellInfo)(void);
}

bool Initialize(void)
{
	hLib = LoadLibrary("SuperTagEditor.exe");
	if(hLib == NULL)	return false;
	(FARPROC&)STEPGetCommandID = GetProcAddress(hLib, "_STEPGetCommandID@0");
	(FARPROC&)STEPGetMenu = GetProcAddress(hLib, "_STEPGetMenu@4");
	(FARPROC&)STEPAddToolBarButton = GetProcAddress(hLib, "_STEPAddToolBarButton@12");
	(FARPROC&)STEPRegisterExt = GetProcAddress(hLib, "_STEPRegisterExt@12");
	(FARPROC&)STEPKeyAssign = GetProcAddress(hLib, "_STEPKeyAssign@12");
	(FARPROC&)STEPGetGenreNameSIF = GetProcAddress(hLib, "_STEPGetGenreNameSIF@4");
	(FARPROC&)STEPGetGenreCode = GetProcAddress(hLib, "_STEPGetGenreCode@4");
	(FARPROC&)STEPIsUserGenre = GetProcAddress(hLib, "_STEPIsUserGenre@4");
	(FARPROC&)STEPGetNumericTrackNumber = GetProcAddress(hLib, "_STEPGetNumericTrackNumber@12"); /* STEP 037 */
	(FARPROC&)STEPGetIntegerTrackNumber = GetProcAddress(hLib, "_STEPGetIntegerTrackNumber@4"); /* STEP 037 */
	(FARPROC&)STEPGetNumericDiskNumber = GetProcAddress(hLib, "_STEPGetNumericDiskNumber@12"); /* STEP 037 */
	(FARPROC&)STEPGetIntegerDiskNumber = GetProcAddress(hLib, "_STEPGetIntegerDiskNumber@4"); /* STEP 037 */

	(FARPROC&)STEPGetBooleanValue = GetProcAddress(hLib, "_STEPGetBooleanValue@8");
	(FARPROC&)STEPSetBooleanValue = GetProcAddress(hLib, "_STEPSetBooleanValue@12");
	(FARPROC&)STEPGetLongValue = GetProcAddress(hLib, "_STEPGetLongValue@8");
	(FARPROC&)STEPSetLongValue = GetProcAddress(hLib, "_STEPSetLongValue@12");
	(FARPROC&)STEPGetIntValue = GetProcAddress(hLib, "_STEPGetIntValue@8");
	(FARPROC&)STEPSetIntValue = GetProcAddress(hLib, "_STEPSetIntValue@12");
	(FARPROC&)STEPGetVoidValue = GetProcAddress(hLib, "_STEPGetVoidValue@8");
	(FARPROC&)STEPSetVoidValue = GetProcAddress(hLib, "_STEPSetVoidValue@12");
	(FARPROC&)STEPGetValue = GetProcAddress(hLib, "_STEPGetValue@8");
	(FARPROC&)STEPSetValue = GetProcAddress(hLib, "_STEPSetValue@12");
	(FARPROC&)STEPProcessSelectedFiles = GetProcAddress(hLib, "_STEPProcessSelectedFiles@8");
	(FARPROC&)STEPProcessSelectedFilesForUpdate = GetProcAddress(hLib, "_STEPProcessSelectedFilesForUpdate@8");

	(FARPROC&)STEPIsRangeSelected = GetProcAddress(hLib, "_STEPIsRangeSelected@0");
	(FARPROC&)STEPGetSelectedCount = GetProcAddress(hLib, "_STEPGetSelectedCount@0");
	(FARPROC&)STEPGetSelectedItem = GetProcAddress(hLib, "_STEPGetSelectedItem@0");
	(FARPROC&)STEPIsCurrentCellEditOK = GetProcAddress(hLib, "_STEPIsCurrentCellEditOK@0");
	(FARPROC&)STEPItemHasChildren = GetProcAddress(hLib, "_STEPItemHasChildren@4");
	(FARPROC&)STEPGetSelectedRange = GetProcAddress(hLib, "_STEPGetSelectedRange@16");
	(FARPROC&)STEPIsItemFile = GetProcAddress(hLib, "_STEPIsItemFile@4");
	(FARPROC&)STEPChangeSubItemText = GetProcAddress(hLib, "_STEPChangeSubItemText@12");
	(FARPROC&)STEPGetSubItemText = GetProcAddress(hLib, "_STEPGetSubItemText@8");
	(FARPROC&)STEPGetFileInfo = GetProcAddress(hLib, "_STEPGetFileInfo@8");
	(FARPROC&)STEPGETColumnType = GetProcAddress(hLib, "_STEPGETColumnType@0");
	(FARPROC&)STEPIsNumeric = GetProcAddress(hLib, "_STEPIsNumeric@4"); /* STEP 037 */
	(FARPROC&)STEPConvSiFieldToId3tag = GetProcAddress(hLib, "_STEPConvSiFieldToId3tag@4"); /* STEP 037 */

	(FARPROC&)STEPInitDataSIF = GetProcAddress(hLib, "_STEPInitDataSIF@4");
	(FARPROC&)STEPInitDataID3 = GetProcAddress(hLib, "_STEPInitDataID3@4"); /* STEP 029 */
	(FARPROC&)STEPInitData = GetProcAddress(hLib, "_STEPInitData@4");
	(FARPROC&)STEPFileNameChange = GetProcAddress(hLib, "_STEPFileNameChange@8");
	(FARPROC&)STEPWriteTag = GetProcAddress(hLib, "_STEPWriteTag@4");

	(FARPROC&)STEPUpdateCellInfo = GetProcAddress(hLib, "_STEPUpdateCellInfo@0");

	return true;
}

void Finalize(void) {
	if (hLib != NULL) {
		FreeLibrary(hLib);
		hLib = NULL;
	}
}

LPCTSTR	GetValue(FILE_INFO* pInfo, FIELDTYPE nField) {
	return STEPGetValue(pInfo, nField);
}
void	SetValue(FILE_INFO* pInfo, FIELDTYPE nField, LPCTSTR szValue) {
	STEPSetValue(pInfo, nField, szValue);
}
bool	isModify(FILE_INFO* pInfo) {
	return STEPGetBooleanValue(pInfo,  0);
}
void	SetModifyFlag(FILE_INFO* pInfo, bool bModify) {
	STEPSetBooleanValue(pInfo, 0, bModify);
}
bool	isChecked(FILE_INFO* pInfo) {
	return STEPGetBooleanValue(pInfo,  1);
}
void	SetCheckFlag(FILE_INFO* pInfo, bool bCheck) {
	STEPSetBooleanValue(pInfo, 1, bCheck);
}
bool	isExistLyricFile(FILE_INFO* pInfo) {
	return STEPGetBooleanValue(pInfo,  2);
}
void	SetExistLyricFile(FILE_INFO* pInfo, bool bExist) {
	STEPSetBooleanValue(pInfo, 2, bExist);
}
long	GetFileSize(FILE_INFO* pInfo) {
	return STEPGetLongValue(pInfo, 0);
}
void	SetFileSize(FILE_INFO* pInfo, long size) {
	STEPSetLongValue(pInfo, 0, size);
}
UINT	GetFormat(FILE_INFO* pInfo) {
	return (UINT)STEPGetIntValue(pInfo, 0);
}
void	SetFormat(FILE_INFO* pInfo, UINT nFormat) {
	STEPSetIntValue(pInfo, 0, nFormat);
}
int		GetPlayTime(FILE_INFO* pInfo) {
	return (UINT)STEPGetIntValue(pInfo, 1);
}
void	SetPlayTime(FILE_INFO* pInfo, int nPlayTime) {
	STEPSetIntValue(pInfo, 1, nPlayTime);
}
BYTE	GetBTrackNumber(FILE_INFO* pInfo) {
	return (BYTE)STEPGetIntValue(pInfo, 2);
}
void	SetBTrackNumber(FILE_INFO* pInfo, BYTE nTrackNumber) {
	STEPSetIntValue(pInfo, 2, nTrackNumber);
}
BYTE	GetBDiskNumber(FILE_INFO* pInfo) {
	return (BYTE)STEPGetIntValue(pInfo, 4);
}
void	SetBDiskNumber(FILE_INFO* pInfo, BYTE nDiskNumber) {
	STEPSetIntValue(pInfo, 4, nDiskNumber);
}
BYTE	GetBGenre(FILE_INFO* pInfo) {
	return (BYTE)STEPGetIntValue(pInfo, 3);
}
void	SetBGenre(FILE_INFO* pInfo, BYTE nGenre) {
	STEPSetIntValue(pInfo, 3, nGenre);
}
void*	GetFileSpecificInfo(FILE_INFO* pInfo) {
	return STEPGetVoidValue(pInfo, 0);
}
void	SetFileSpecificInfo(FILE_INFO* pInfo, void* info) {
	STEPSetVoidValue(pInfo, 0, info);
}
