#ifndef _STEPLUGIN_H_
#define _STEPLUGIN_H_

#include "STEP_api.h"

//extern HINSTANCE g_hInstance;
//extern HINSTANCE hLib;

/* プラグインから呼び出せる関数 */
extern "C" {
extern STEP_API UINT (WINAPI *STEPGetCommandID)(void);
extern STEP_API HMENU (WINAPI *STEPGetMenu)(UINT);
extern STEP_API void (WINAPI *STEPAddToolBarButton)(HBITMAP, UINT, char*);
extern STEP_API UINT (WINAPI *STEPRegisterExt)(UINT, LPCTSTR, HBITMAP);
extern STEP_API UINT (WINAPI *STEPKeyAssign)(UINT, LPCTSTR, LPCTSTR);
extern STEP_API const char* (WINAPI *STEPGetGenreNameSIF)(BYTE byGenre);
extern STEP_API BYTE (WINAPI *STEPGetGenreCode)(const char* szGenre);
extern STEP_API bool (WINAPI *STEPIsUserGenre)(const char* szGenre);
extern STEP_API int (WINAPI *STEPGetNumericTrackNumber)(const char* szTrackNumber, char* szNumericNumber, int size); /* STEP 037 */
extern STEP_API int (WINAPI *STEPGetIntegerTrackNumber)(const char* szTrackNumber); /* STEP 037 */
extern STEP_API int (WINAPI *STEPGetNumericDiskNumber)(const char* szDiskNumber, char* szNumericNumber, int size); /* STEP 037 */
extern STEP_API int (WINAPI *STEPGetIntegerDiskNumber)(const char* szDiskNumber); /* STEP 037 */

extern STEP_API void (WINAPI *STEPProcessSelectedFiles)(LPCTSTR, STEPProcessSelectedFilesCB);
extern STEP_API void (WINAPI *STEPProcessSelectedFilesForUpdate)(LPCTSTR, STEPProcessSelectedFilesCB);

extern STEP_API bool (WINAPI *STEPIsRangeSelected)(void);
extern STEP_API int (WINAPI *STEPGetSelectedCount)(void);
extern STEP_API int (WINAPI *STEPGetSelectedItem)(void);
extern STEP_API bool (WINAPI *STEPIsCurrentCellEditOK)(void);
extern STEP_API bool (WINAPI *STEPItemHasChildren)(int);
extern STEP_API bool (WINAPI *STEPGetSelectedRange)(int*, int*, int* , int*);
extern STEP_API bool (WINAPI *STEPIsItemFile)(int);
extern STEP_API void (WINAPI *STEPChangeSubItemText)(int nItem, int nColumn, LPCTSTR szText);
extern STEP_API const char* (WINAPI *STEPGetSubItemText)(int nItem, int nColumn);
extern STEP_API void (WINAPI *STEPGetFileInfo)(int nItem, FILE_INFO* info);
extern STEP_API UINT (WINAPI *STEPGETColumnType)(int nColumn);
extern STEP_API bool (WINAPI *STEPIsCurrentCellEditOK)(void);
extern STEP_API bool (WINAPI *STEPIsNumeric)(const char* szText); /* STEP 037 */
extern STEP_API void (WINAPI *STEPConvSiFieldToId3tag)(FILE_INFO* pFileInfo); /* STEP 037 */

extern STEP_API void (WINAPI *STEPInitDataSIF)(FILE_INFO* info);
extern STEP_API void (WINAPI *STEPInitDataID3)(FILE_INFO* info); /* STEP 029 */
extern STEP_API void (WINAPI *STEPInitData)(FILE_INFO* info);
extern STEP_API bool (WINAPI *STEPFileNameChange)(FILE_INFO* pFileInfo, LPCTSTR szNewFileName);
extern STEP_API bool (WINAPI *STEPWriteTag)(FILE_INFO* pFileInfo);

extern STEP_API bool (WINAPI *STEPUpdateCellInfo)(void);
}

extern bool Initialize(void);
extern void Finalize(void);

/* タグ情報の取得／設定 */
extern bool isModify(FILE_INFO*);
extern void SetModifyFlag(FILE_INFO* pInfo, bool bModify);
extern bool isChecked(FILE_INFO* pInfo);
extern void SetCheckFlag(FILE_INFO* pInfo, bool bCheck);
extern bool isExistLyricFile(FILE_INFO* pInfo);
extern void SetExistLyricFile(FILE_INFO* pInfo, bool bExist);
extern long GetFileSize(FILE_INFO* pInfo);
extern void SetFileSize(FILE_INFO* pInfo, long size);
extern UINT GetFormat(FILE_INFO* pInfo);
extern void SetFormat(FILE_INFO* pInfo, UINT nFormat);
extern int  GetPlayTime(FILE_INFO* pInfo);
extern void SetPlayTime(FILE_INFO* pInfo, int nPlayTime);
extern BYTE GetBTrackNumber(FILE_INFO* pInfo);
extern void SetBTrackNumber(FILE_INFO* pInfo, BYTE nTrackNumber);
extern BYTE GetBDiskNumber(FILE_INFO* pInfo);
extern void SetBDiskNumber(FILE_INFO* pInfo, BYTE nDiskNumber);
extern BYTE GetBGenre(FILE_INFO* pInfo);
extern void SetBGenre(FILE_INFO* pInfo, BYTE nGenre);
extern void* GetFileSpecificInfo(FILE_INFO* pInfo);
extern void SetFileSpecificInfo(FILE_INFO* pInfo, void* info);

extern LPCTSTR GetValue(FILE_INFO* pInfo, FIELDTYPE nField);
extern void SetValue(FILE_INFO* pInfo, FIELDTYPE nField, LPCTSTR szValue);

inline LPCTSTR GetFullPath(FILE_INFO* info) {
	return GetValue(info, FIELD_FULL_PATH_NAME);
}
inline LPCTSTR GetFileName(FILE_INFO* info) {
	return GetValue(info, FIELD_FILE_NAME);
}
inline LPCTSTR GetFilePath(FILE_INFO* info) {
	return GetValue(info, FIELD_FILE_PATH);
}
inline LPCTSTR GetVisualFormat(FILE_INFO* info) {
	return GetValue(info, FIELD_VISUAL_FORMAT);
}
inline LPCTSTR GetAudioFormat(FILE_INFO* info) {
	return GetValue(info, FIELD_AUDIO_FORMAT);
}
inline LPCTSTR GetTrackName(FILE_INFO* info) {
	return GetValue(info, FIELD_TRACK_NAME);
}
inline LPCTSTR GetArtistName(FILE_INFO* info) {
	return GetValue(info, FIELD_ARTIST_NAME);
}
inline LPCTSTR GetAlbumName(FILE_INFO* info) {
	return GetValue(info, FIELD_ALBUM_NAME);
}
inline LPCTSTR GetComment(FILE_INFO* info) {
	return GetValue(info, FIELD_COMMENT);
}
inline LPCTSTR GetYear(FILE_INFO* info) {
	return GetValue(info, FIELD_YEAR);
}
inline LPCTSTR GetTrackNumber(FILE_INFO* info) {
	return GetValue(info, FIELD_TRACK_NUMBER);
}
inline LPCTSTR GetDiskNumber(FILE_INFO* info) {
	return GetValue(info, FIELD_DISK_NUMBER);
}
inline LPCTSTR GetGenre(FILE_INFO* info) {
	return GetValue(info, FIELD_GENRE);
}
inline LPCTSTR GetTrackNameSI(FILE_INFO* info) {
	return GetValue(info, FIELD_TRACK_NAME_SI);
}
inline LPCTSTR GetArtistNameSI(FILE_INFO* info) {
	return GetValue(info, FIELD_ARTIST_NAME_SI);
}
inline LPCTSTR GetAlbumNameSI(FILE_INFO* info) {
	return GetValue(info, FIELD_ALBUM_NAME_SI);
}
inline LPCTSTR GetCommentSI(FILE_INFO* info) {
	return GetValue(info, FIELD_COMMENT_SI);
}
inline LPCTSTR GetYearSI(FILE_INFO* info) {
	return GetValue(info, FIELD_YEAR_SI);
}
inline LPCTSTR GetTrackNumberSI(FILE_INFO* info) {
	return GetValue(info, FIELD_TRACK_NUMBER_SI);
}
inline LPCTSTR GetDiskNumberSI(FILE_INFO* info) {
	return GetValue(info, FIELD_DISK_NUMBER_SI);
}
inline LPCTSTR GetGenreSI(FILE_INFO* info) {
	return GetValue(info, FIELD_GENRE_SI);
}
inline LPCTSTR GetCopyrightSI(FILE_INFO* info) {
	return GetValue(info, FIELD_COPYRIGHT_SI);
}
inline LPCTSTR GetEngineerSI(FILE_INFO* info) {
	return GetValue(info, FIELD_ENGINEER_SI);
}
inline LPCTSTR GetSourceSI(FILE_INFO* info) {
	return GetValue(info, FIELD_SOURCE_SI);
}
inline LPCTSTR GetSoftwareSI(FILE_INFO* info) {
	return GetValue(info, FIELD_SOFTWARE_SI);
}
inline LPCTSTR GetKeywordSI(FILE_INFO* info) {
	return GetValue(info, FIELD_KEYWORD_SI);
}
inline LPCTSTR GetTechnicianSI(FILE_INFO* info) {
	return GetValue(info, FIELD_TECHNICIAN_SI);
}
inline LPCTSTR GetLyricSI(FILE_INFO* info) {
	return GetValue(info, FIELD_LYRIC_SI);
}
inline LPCTSTR GetCommissionSI(FILE_INFO* info) {
	return GetValue(info, FIELD_COMMISSION_SI);
}
inline LPCTSTR GetWriterSI(FILE_INFO* info) {
	return GetValue(info, FIELD_WRITER_SI);
}
inline LPCTSTR GetComposerSI(FILE_INFO* info) {
	return GetValue(info, FIELD_COMPOSER_SI);
}
inline LPCTSTR GetAlbumArtistSI(FILE_INFO* info) {
	return GetValue(info, FIELD_ALBM_ARTIST_SI);
}
inline LPCTSTR GetOrigArtistSI(FILE_INFO* info) {
	return GetValue(info, FIELD_ORIG_ARTIST_SI);
}
inline LPCTSTR GetURLSI(FILE_INFO* info) {
	return GetValue(info, FIELD_URL_SI);
}
inline LPCTSTR GetEncodest(FILE_INFO* info) {
	return GetValue(info, FIELD_ENCODEST);
}
inline LPCTSTR GetOther(FILE_INFO* info) {
	return GetValue(info, FIELD_OTHER);
}
inline LPCTSTR GetFileTypeName(FILE_INFO* info) {
	return GetValue(info, FILED_FILE_TYPE_NAME);
}

inline void SetFullPathName(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_FULL_PATH_NAME, szValue);
}
inline void SetFileName(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_FILE_NAME, szValue);
}
inline void SetFilePath(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_FILE_PATH, szValue);
}
inline void SetVisualFormat(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_VISUAL_FORMAT, szValue);
}
inline void SetAudioFormat(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_AUDIO_FORMAT, szValue);
}
inline void SetTrackName(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_TRACK_NAME, szValue);
}
inline void SetArtistName(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_ARTIST_NAME, szValue);
}
inline void SetAlbumName(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_ALBUM_NAME, szValue);
}
inline void SetComment(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_COMMENT, szValue);
}
inline void SetYear(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_YEAR, szValue);
}
inline void SetTrackNumber(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_TRACK_NUMBER, szValue);
}
inline void SetDiskNumber(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_DISK_NUMBER, szValue);
}
inline void SetGenre(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_GENRE, szValue);
}
inline void SetTrackNameSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_TRACK_NAME_SI, szValue);
}
inline void SetArtistNameSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_ARTIST_NAME_SI, szValue);
}
inline void SetAlbumNameSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_ALBUM_NAME_SI, szValue);
}
inline void SetCommentSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_COMMENT_SI, szValue);
}
inline void SetYearSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_YEAR_SI, szValue);
}
inline void SetTrackNumberSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_TRACK_NUMBER_SI, szValue);
}
inline void SetDiskNumberSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_DISK_NUMBER_SI, szValue);
}
inline void SetGenreSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_GENRE_SI, szValue);
}
inline void SetCopyrightSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_COPYRIGHT_SI, szValue);
}
inline void SetEngineerSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_ENGINEER_SI, szValue);
}
inline void SetSourceSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_SOURCE_SI, szValue);
}
inline void SetSoftwareSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_SOFTWARE_SI, szValue);
}
inline void SetKeywordSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_KEYWORD_SI, szValue);
}
inline void SetTechnicianSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_TECHNICIAN_SI, szValue);
}
inline void SetLyricSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_LYRIC_SI, szValue);
}
inline void SetCommissionSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_COMMISSION_SI, szValue);
}
inline void SetWriterSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_WRITER_SI, szValue);
}
inline void SetComposerSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_COMPOSER_SI, szValue);
}
inline void SetAlbumArtistSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_ALBM_ARTIST_SI, szValue);
}
inline void SetOrigArtistSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_ORIG_ARTIST_SI, szValue);
}
inline void SetURLSI(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_URL_SI, szValue);
}
inline void SetEncodest(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_ENCODEST, szValue);
}
inline void SetOther(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FIELD_OTHER, szValue);
}
inline void SetFileTypeName(FILE_INFO* info, LPCTSTR szValue) {
	SetValue(info, FILED_FILE_TYPE_NAME, szValue);
}
#endif