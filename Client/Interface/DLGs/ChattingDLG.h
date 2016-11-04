
#ifndef _CHATTING_DLG_H
#define _CHATTING_DLG_H
#include <vector>
#include <queue>
#include "TDialog.h"
#include "IActionListener.h"

/// 0: false, 1: true, 2:true(can't change)
struct ChatFilter{
	BYTE	Filters[6];
};


/**
* ä�� ��/����� ���� ���̾�α�
*	- ä�ø�ɾ�( ��ġ��� )�� 2����Ʈ���ڿ����� ���۰����ϵ��� �����ڵ�� 2���� ����
*	- GM��ɾ��� ä�ñ��� ��� ����( Ŭ���̾�Ʈ������ �����Ǿ� ������ �� ���ӽ� �ش� ������� )
*
* @Todo			GM��ɾ��� ä�ñ�������� �������� �����ϰų� ������ �ٽ� �������� ������� ���� �ʿ�
* @Author		������
* @Date			2005/9/12
*/
class CChatDLG : public CTDialog, public IActionListener
{

public:
	CChatDLG();
	virtual ~CChatDLG();

	virtual bool			Create( const char* IDD );
	virtual unsigned int	Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual void			Update( POINT ptMouse );
	virtual void			Show();

	virtual unsigned		ActionPerformed( CActionEvent* e );


	ChatFilter& GetCurrentTabFilter( );
	void		SetCurrentTabFilter( ChatFilter& filter );


	enum{
		IID_BTN_FILTER = 10,
		IID_EDITBOX	= 15,

		IID_LISTBOX_ALL = 20,
		IID_SCROLLBAR_ALL = 21,

		IID_LISTBOX_WHISPER = 25,
		IID_SCROLLBAR_WHISPER = 26,

		IID_LISTBOX_TRADE	= 30,
		IID_SCROLLBAR_TRADE = 31,

		IID_LISTBOX_PARTY = 35,
		IID_SCROLLBAR_PARTY = 36,

		IID_LISTBOX_CLAN = 40,
		IID_SCROLLBAR_CLAN = 41,

		IID_LISTBOX_ALLIED = 45,
		IID_SCROLLBAR_ALLIED = 46,

		IID_RADIOBOX	= 50,
		IID_BTN_ALL		= 51,
		IID_BTN_WHISPER = 52,
		IID_BTN_TRADE   = 53,
		IID_BTN_PARTY	= 54,
		IID_BTN_CLAN	= 55,
		IID_BTN_ALLIED  = 56,


		IID_BTN_CAPTURE = 60,

		IDD_LISTBOX_TOP = 75,
		IDD_SCROLLBAR_TOP = 76,
		IDD_BTN_SCROLLDOWN = 100,
	};
	

	enum
	{
		FILTER_NORMAL,
		FILTER_SYSTEM,
		FILTER_TRADE,
		FILTER_PARTY,
		FILTER_CLAN,
		FILTER_WHISPER,
		FILTER_ALLIED,
	};

	void		SetChatBlock( DWORD BlockTime );

	void		SetChatUnBlock();

	void		AppendMsg( const char* pszMsg, DWORD color, int iType );
	void		AppendMsg2( const char* pszMsg, DWORD color, int iType );
	void		AppendMsg2System( const char* pszMsg, DWORD color);
	void		SendChatMsgRepeat();
	void		SetEnableShoutRestrict();									/// ��ġ�� �ð� ���� ����
	void		SetDisableShoutRestrict();									/// ��ġ�� �ð� ���� ����

	int			GetLineCount();
	const char* GetLineString( int index );

protected:

	bool	On_LButtonUP( unsigned iProcID);
	void	OnLButtonDown( unsigned uiProcID, WPARAM wParam , LPARAM lParam );

	void	SendChatMsg( char* szMsg );


	enum{
		TAB_ALL = 1,
		TAB_WHISPER,
		TAB_TRADE,
		TAB_PARTY,
		TAB_CLAN,
		TAB_ALLIED,
		TAB_MAX
	};
	void	TabSelected( int iTab );




	enum{
		CHAT_NORMAL,
		CHAT_SHOUT,
		CHAT_WHISPER,
		CHAT_EXCHANGE,
		CHAT_PARTY,
		CHAT_OPEN_HELP,
		CHAT_CLAN,
		CHAT_TRADE,
		CHAT_ALLIED,
	};

	int		ChatParser( string stMsg, string& stRealMsg, string& stTargetID );
	
	bool	IsChatBlock();
	void	SetFocusEditBox( unsigned iID );
	int		GetActiveListBox();
	int		GetActiveScrollBar();
	void	AppendMsg2sys( const char* pszMsg, DWORD dwColor , int iFilterType );
	void	AppendMsg2All( const char* pszMsg, DWORD dwColor , int iFilterType );
	void	AppendMsg2Whisper( const char* pszMsg, DWORD dwColor , int iFilterType );
	void	AppendMsg2Trade( const char* pszMsg, DWORD dwColor , int iFilterType );
	void	AppendMsg2Party( const char* pszMsg, DWORD dwColor , int iFilterType );
	void	AppendMsg2Clan( const char* pszMsg, DWORD dwColor , int iFilterType );
	void	AppendMsg2Allied( const char* pszMsg, DWORD dwColor , int iFilterType );
	void	AppendMsg2ListBox( int iListBoxID, int iScrollBarID , const char* pszMsg, DWORD dwColor );

	void	ActiveListBoxMoveEnd();

protected:
	/// ���� ���� ��ɿ� ���Ǵ� �����
	queue< DWORD >	m_PrevSendMessageTimes;
	int				m_iCheckChatCount;///3
	DWORD			m_dwCheckChatTime;///5 * 1000

	/// GM��ɾ��� ä�ñ����� ���Ǵ� �����
	bool			m_bChatBlock;				
	DWORD			m_dwChatBlockStartTime;
	DWORD			m_dwChatBlockTime;

	/// ��ġ�� �ð� ���ѿ� ���Ǵ� ���
	bool	m_bCheckShoutRestrict;
	DWORD   m_dwShoutRestrictTime;
	DWORD   m_dwPrevUseShoutTime;
	


	//DWORD   m_dwChatRestrictTime;	///������
	//DWORD   m_dwPrevSendChatTime; ///������

	std::string				m_strLastSendMsg;					/// ������ ������ ���� �Է±� ����
	int						m_iSelectedTab;						/// ���� �� ����

	short					m_nChatLineCNT;						/// ä�� ���μ� 
	static					int m_iCaptureCount;	

	vector< ChatFilter >	m_Filters;				


	/// �ٱ��� ������ : 2����Ʈ���ڿ����� �����Ҽ� �ֵ��� �����ڵ�� ���
	std::vector< std::wstring >		m_ShoutCommands;			
	std::vector< std::wstring > 	m_TradeCommands;
	std::vector< std::wstring >		m_WhisperCommands;
	std::vector< std::wstring >		m_ClanCommands;
	std::vector< std::wstring >		m_PartyCommands;
	std::vector< std::wstring >		m_HelpCommands;
	std::vector< std::wstring >		m_AlliedCommands;
	std::vector< std::wstring >		m_Spaces;
};
#endif