#ifndef STEP_API_H
#define STEP_API_H

#define STEP_API //__declspec(dllexport)

#define STEP_API_VERSION 0x000200 /* STEP 029 */

/* �������� */
enum {
	STEP_SUCCESS,			// ����
	STEP_UNKNOWN_FORMAT,	// ���m�̃t�@�C���`��
	STEP_ERROR				// �G���[
};

/* ���j���[��� */
enum {
	MENU_FILE,
	MENU_EDIT,
	MENU_DISP,
	MENU_CONV,
	MENU_PLAYLIST,
	MENU_PLAYER,
	MENU_OPTION,
	/*MENU_OPTION_PLUGIN,*/
	MENU_HELP,
	// �������牺�� STEPGetMenu() �ł͎擾�ł��Ȃ� STEPOnLoadMenu() �Ŏg��
	MENU_CELL_RANGE,
	MENU_FILE_EDIT_OK,
	MENU_FILE_RANGE,
	MENU_SELECT_FOLDER,
};

/* �t�@�C���`�� */
enum	{
	FILE_FORMAT_UNKNOWN = 1,	// �t�@�C���`���F���m�̌`��
	// ���̑��̓v���O�C�����ɒ�`
};

/* �Z�����̓��͗p�R���g���[����� */
enum CONTROLTYPE {edit/*default*/, combobox, invalid, multiedit, multieditreadonly};
#define _NULL			invalid
#define _EDIT			edit
#define _CBOX			combobox
#define _MEDIT			multiedit
#define _MEDITR			multieditreadonly

/* �\���J���� */
enum COLUMNTYPE {
	COLUMN_TREE_ITEM = 0,
	COLUMN_FULL_PATH_NAME = 0,
	COLUMN_FILE_NAME,
	COLUMN_TRACK_NAME,		// �Ȗ�
	COLUMN_ARTIST_NAME,		// �A�[�e�B�X�g��
	COLUMN_ALBUM_NAME,		// �A���o�����^���i��
	COLUMN_TRACK_NUMBER,	// �g���b�N�ԍ�(ID3 tag v1.1�̂�)
    COLUMN_TRACK_TOTAL,     // �g���b�N��(by Kobarin)
	COLUMN_DISC_NUMBER,		// �f�B�X�N�ԍ�
    COLUMN_DISC_TOTAL,      // �f�B�X�N��(by Kobarin)
	COLUMN_YEAR,			// �����[�X�N��
	COLUMN_GENRE,			// �W������
	COLUMN_COMMENT,			// �R�����g������
	COLUMN_COPYRIGHT,		// ���쌠
	COLUMN_ENGINEER,		// �G���W�j�A(�����)
	COLUMN_SOURCE,			// �\�[�X
	COLUMN_SOFTWARE,		// �\�t�g�E�F�A
	COLUMN_KEYWORD,			// �L�[���[�h
	COLUMN_TECHNICIAN,		// �Z�p��
	COLUMN_LYRIC,			// �̎�
	COLUMN_COMMISSION,		// �R�~�b�V����
	COLUMN_WRITER,			// �쎌
	COLUMN_COMPOSER,		// ���
	COLUMN_ALBM_ARTIST,		// Albm.�A�[�e�B�X�g
	COLUMN_ORIG_ARTIST,		// Orig.�A�[�e�B�X�g
	COLUMN_URL,				// URL
	COLUMN_ENCODEST,		// �G���R�[�h�����l
	COLUMN_FILE_SIZE,		// �t�@�C���T�C�Y
	COLUMN_FILE_TIME,		// �t�@�C���^�C���X�^���v
	COLUMN_FILE_PATH,		// �t�@�C���p�X��
	COLUMN_PLAY_TIME,		// ���t����
	COLUMN_LYRIC_FILE,		// �̎��t�@�C��
	COLUMN_FILE_TYPE,		// �t�@�C���̎��
	COLUMN_FORMAT,			// �t�H�[�}�b�g
	COLUMN_OTHER,			// ���̑�
	COLUMN_FILE_CTIME,		// �t�@�C���쐬�� /* 2003.06.19 add */
	COLUMN_MAX,
};
#define COLUMN_EDIT_MAX COLUMN_OTHER

/* �^�O���擾�^�ݒ�p�̃t�B�[���h�� */
enum FIELDTYPE {
	FIELD_FULL_PATH_NAME,			// �t�@�C����(�t���p�X)
	FIELD_FILE_NAME,				// strFullPathName �̃t�@�C��������
	FIELD_FILE_PATH,				// strFullPathName �̃p�X����
	FIELD_VISUAL_FORMAT,			// �f���t�H�[�}�b�g
	FIELD_AUDIO_FORMAT,				// �����t�H�[�}�b�g
	FIELD_TRACK_NAME,				// �g���b�N��
	FIELD_ARTIST_NAME,				// �A�[�e�B�X�g��
	FIELD_ALBUM_NAME,				// �A���o����
	FIELD_COMMENT,					// �R�����g
	FIELD_YEAR,						// �����[�X�N��
	FIELD_TRACK_NUMBER,				// �g���b�N�ԍ�
	FIELD_GENRE,					// �W��������
	FIELD_TRACK_NAME_SI,			// �Ȗ�
	FIELD_ARTIST_NAME_SI,			// �A�[�e�B�X�g��
	FIELD_ALBUM_NAME_SI,			// ���i��
	FIELD_COMMENT_SI,				// �R�����g������
	FIELD_YEAR_SI,					// �����[�X�N��
	FIELD_GENRE_SI,					// �W��������
	FIELD_TRACK_NUMBER_SI,			// �g���b�N�ԍ�
	FIELD_TRACK_TOTAL_SI,			// �g���b�N��(by Kobarin)
	FIELD_Disc_NUMBER_SI,			// �f�B�X�N�ԍ�
	FIELD_DISC_TOTAL_SI,			// �f�B�X�N��(by Kobarin)
	FIELD_COPYRIGHT_SI,				// ���쌠
	FIELD_ENGINEER_SI,				// �G���W�j�A
	FIELD_SOURCE_SI,				// �\�[�X
	FIELD_SOFTWARE_SI,				// �\�t�g�E�F�A
	FIELD_KEYWORD_SI,				// �L�[���[�h
	FIELD_TECHNICIAN_SI,			// �Z�p��
	FIELD_LYRIC_SI,					// �̎�
	FIELD_COMMISSION_SI,			// �R�~�b�V����
	FIELD_WRITER_SI,				// �쎌
	FIELD_COMPOSER_SI,				// ���
	FIELD_ALBM_ARTIST_SI,			// Albm.�A�[�e�B�X�g
	FIELD_ORIG_ARTIST_SI,			// Orig.�A�[�e�B�X�g
	FIELD_URL_SI,					// URL
	FIELD_ENCODEST,					// �G���R�[�h�����l
	FIELD_OTHER,					// ���̑�
	FILED_FILE_TYPE_NAME,			// �t�@�C���^�C�v������
	FIELD_MAX
};

/* �t�@�C����� */
typedef struct _FILE_INFO {
	void*	pFileMP3;
} FILE_INFO;

/**--------------------------------------------------------------------**/
/* �v���O�C���ɕK���������Ȃ���΂����Ȃ��֐� */
extern "C" {
STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder);
STEP_API void WINAPI STEPFinalize();
STEP_API UINT WINAPI STEPGetAPIVersion(void);
STEP_API LPCTSTR WINAPI STEPGetPluginName(void);
STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void);
}

/* �v���O�C�����^�O�̓ǂݏ������T�|�[�g����ꍇ�Ɏ������Ȃ���΂����Ȃ��֐� */
extern "C" {
STEP_API CONTROLTYPE WINAPI STEPGetControlType(UINT, COLUMNTYPE, bool);
STEP_API UINT WINAPI STEPGetColumnMax(UINT, COLUMNTYPE, bool);
STEP_API UINT WINAPI STEPLoad(FILE_INFO*, LPCTSTR);
STEP_API UINT WINAPI STEPSave(FILE_INFO*);
}

/* �v���O�C���Ɏ����ł���֐� */
extern "C" {
STEP_API LPCTSTR WINAPI STEPGetToolTipText(UINT nID);
STEP_API LPCTSTR WINAPI STEPGetStatusMessage(UINT nID);
STEP_API bool WINAPI STEPOnUpdateCommand(UINT nID);
STEP_API bool WINAPI STEPOnCommand(UINT nID, HWND hWnd);
STEP_API bool WINAPI STEPSupportSIF(UINT nFormat);
STEP_API bool WINAPI STEPSupportTrackNumberSIF(UINT nFormat);
STEP_API bool WINAPI STEPSupportGenreSIF(UINT nFormat);
STEP_API void WINAPI STEPShowOptionDialog(HWND);
STEP_API void WINAPI STEPOnLoadMenu(HMENU, UINT);
STEP_API void WINAPI STEPOnLoadMainMenu();
STEP_API LPCTSTR WINAPI STEPGetColumnName(UINT, COLUMNTYPE);
STEP_API bool WINAPI STEPHasSpecificColumnName(UINT);
STEP_API void WINAPI STEPInitFileSpecificInfo(FILE_INFO*);
STEP_API bool WINAPI STEPOnConvSiFieldToId3tag(FILE_INFO*);
STEP_API bool WINAPI STEPOnLoadStart();
STEP_API bool WINAPI STEPOnSaveStart();
STEP_API void WINAPI STEPOnLoadEnd();
STEP_API void WINAPI STEPOnSaveEnd();
}

// �t�@�C�����ɍs���R�[���o�b�N�֐� */
typedef bool WINAPI STEPProcessSelectedFilesCB(FILE_INFO*, int, HWND);

#endif