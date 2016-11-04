#ifndef _COPTIONDLG_
#define _COPTIONDLG_

#include "../../../TGameCtrl/TDialog.h"
#include "../../CClientStorage.h"

///���� �����Ǵ� ��ũ�� �ػ󵵹� ���ļ���( Key�� �ػ󵵷� �Ѵ� )
struct SupportedScreen{
	int	m_iWidth;
	int m_iHeight;
	int m_iDepth;
	std::vector< int >	m_Frequencys;
	bool	IsEqual( int iWidth, int iHeight, int iDepth )
	{
		if( m_iWidth == iWidth && m_iHeight == iHeight && m_iDepth == iDepth )
			return true;

		return false;
	}
};

struct SupportedAntiAlising
{
	int	m_iAntiAlising;
	bool	IsEqual( int iAnti )
	{
		if( iAnti == m_iAntiAlising )
			return true;

		return false;
	}
};

struct AdapterInformation;
class CTComboBox;


/**
* �ɼ������� ����/�����ֱ� ���� ���̾�α�
*	- CClientStorage�� �����Ͽ� Ŭ���̾�Ʈ�� Save/Load�� �����Ѵ�.
*
* @Warning		Ŭ���̾�Ʈ ���� ����� TriggerDetect�� �����Ͽ� �ʿ��� ����ȭ���� �������� �ʾ����� Create�κп��� ���� �߻�
* @Todo			�ʿ��� �ý������� ȭ���� ���ٸ� �޼��� �ڽ��� ������ ����ȭ���� �����ϵ��� ��������.
* @Author		������
* @Date			2005/9/12
*/
class COptionDlg :public CTDialog
{
public:
	COptionDlg(void);
	virtual ~COptionDlg(void);

	virtual bool Create( const char* szIDD );
	virtual void Update( POINT ptMouse );
	virtual void Draw();
	virtual void Show();
	virtual unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);


	void ChangeScreenModeByHotKey();										/// Window���� FullScreen��� ��ȯ

protected:
	enum{
		IID_BTN_CLOSE			= 10,
		IID_BTN_ICONIZE			= 11,
		IID_BTN_CONFIRM			= 12,
		IID_BTN_INITIALIZE		= 13,
		IID_TABBEDPANE			= 20,
		IID_TABBUTTON_VIDEO		= 22,
		IID_COMBO_RESOLUTION	= 23,
		IID_COMBO_FREQUENCY		= 24,
		IID_LIST_CAMERA			= 25,
		IID_SCROLLBAR_CAMERA	= 26,
		IID_LIST_ZNZIN			= 27,
		IID_SCROLLBAR_PERFORMANCE = 28,
		IID_RADIOBOX_FULLSCREEN = 29,
		IID_TABBUTTON_AUDIO		= 32,
		IID_LIST_BGM			= 33,
		IID_SCROLLBAR_BGM		= 34,
		IID_LIST_EFFECTSOUND		= 35,
		IID_SCROLLBAR_EFFECTSOUND	= 36,
		IID_TABBUTTON_PLAY			= 42,
		IID_RADIOBOX_CONTROL		= 43,
		IID_RADIOBUTTON_DEFAULTCONTROL		= 44,
		IID_RADIOBUTTON_SEVENHEARTSCONTROL	= 45,
		IID_CHECKBOX_SHOWPCNAME		= 46,
		IID_CHECKBOX_SHOWNPCNAME	= 47,

		IID_TABBUTTON_COMMUNITY		= 52,
		IID_RADIOBOX_WHISPER		= 55,
		IID_RADIOBTN_WHISPER_ON		= 56,
		IID_RADIOBTN_WHISPER_OFF	= 57,

		IID_RADIOBOX_ADDFRIEND		= 60,
		IID_RADIOBTN_ADDFRIEND_ON	= 61,
		IID_RADIOBTN_ADDFRIEND_OFF	= 62,

		IID_RADIOBOX_EXCHANGE		= 65,
		IID_RADIOBTN_EXCHANGE_ON	= 66,
		IID_RADIOBTN_EXCHANGE_OFF	= 67,

		IID_RADIOBOX_PARTY			= 70,
		IID_RADIOBTN_PARTY_ON		= 71,
		IID_RADIOBTN_PARTY_OFF		= 72,

		IID_RADIOBOX_MESSANGER		= 75,
		IID_RADIOBTN_MESSANGER_ON	= 76,
		IID_RADIOBTN_MESSANGER_OFF	= 77,

		IID_RADIOBUTTON_FULLSCREEN	= 121,
		IID_RADIOBUTTON_WINDOWMODE	= 122,
		IID_LIST_GAMMA				= 127,
		IID_SCROLLBAR_GAMMA			= 128,

		IID_TABBUTTON_KEYBOARD		= 155,
		IID_RADIOBOX_KEYBOARD		= 160,
		IID_RADIOBTN_ENTERCHATTING  = 161,
		IID_RADIOBTN_NORMALCHATTING = 162,

		IID_LIST_ANTIALSING			= 170,
	};

	enum{
		IID_TABAUDIO		= 0,
		IID_TABPLAY			= 1,
		IID_TABCOMMUNITY	= 2,
		IID_TABKEYBOARD		= 3,
		IID_TABVIDEO		= 4,
	};

	
	void SaveCurrentOption();						///CClientStorage�� ���� ����� �ɼ��� Set�ϰ� File�� �����Ѵ�.
	void GetCurrentOption();						///CClientStorage���� ���� �ɼ��� ������ �´�.

	void ChangeOption();

	void InitOption();

	void OnLButtonUp( unsigned iProcID );
	void OnLButtonDown( unsigned iProcID );


	void ChangeResolution( t_OptionResolution Resolution );
	

	CTComboBox*	GetComboBox( int ComboBoxID );
	void DoCancel();

protected:
	void ChangeVideoOption();
	void ChangeAudioOption();
	void ChangePlayOption();
	void ChangeCommunityOption();
	void ChangeKeyboardOption();
	void InitVideoOption();
	void InitAudioOption();
	void InitPlayOption();
	void InitCommunityOption();

	void AddSupportedScreen( int iWidth, int iHeight, int iDepth, int iFrequency );
	void AddSupportedAntiAlising( int iOption );

	int  GetCurrentResolutionIndex();
protected:
	int								m_iTab;						/// �Ǳ���
	t_OptionVideo					m_VideoOption;
	t_OptionSound					m_SoundOption;
	t_OptionPlay					m_PlayOption;
	t_OptionCommunity				m_CommunityOption;
	t_OptionKeyboard				m_KeyboardOption;

	AdapterInformation*				m_pAdptInfo;				/// ���� ����̹� ����
	std::vector<SupportedScreen>	m_SupportedScreens;			/// Ŭ���̾�Ʈ �ϵ��� �����ϴ� �ػ󵵵� 
	std::vector< SupportedAntiAlising >				m_SupportedAntiAlising;		/// AntiAlising
};
#endif