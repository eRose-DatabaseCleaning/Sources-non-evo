#ifndef _ChatRoomDlg_
#define _ChatRoomDlg_
#include "TDialog.h"
#include "../../GameData/CChatRoom.h"
#include "../../GameCommon/IObserver.h"
#include "IActionListener.h"
/**
* Ŀ�´�Ƽ â���� ä�÷���  ���� UI Dialog
*	- Observable : CChatRoom
* 
* @Author		������
* @Date			2005/9/12
*/
class CChatRoomDlg : public CTDialog, public IObserver, public IActionListener
{
public:
	CChatRoomDlg( int iDlgType );
	virtual ~CChatRoomDlg(void);
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		Create( const char* IDD );
	virtual void		Draw();
	virtual void		Show();
	virtual void		Hide();

	virtual void		Update( CObservable* pObservable, CTObject* pObj );

	virtual unsigned	ActionPerformed( CActionEvent* e );							

	void				RecvChatMsg( WORD wUserID, const char* pszMsg );			/// ������ ���� ���� ä�÷��� ä�ø޼��� ó��


private:
	void		AddChatMsg( const char* pszMsg, DWORD dwColor );

	void		OnLButtonUp( unsigned uiProcID, WPARAM wParam, LPARAM lParam );
	void		OnLButtonDown( unsigned uiProcID, WPARAM wParam, LPARAM lParam );
	void		SendChatMsg();

	void		Minimize();															/// �ּ�ȭ�� ó��
	void		Maximize();															/// �ִ�ȭ�� ó��

	CWinCtrl*	FindChildInPane( unsigned uiPaneID, unsigned uiChildID );
private:
	enum{
		IID_BTN_CLOSE		 = 10,
		IID_CHAT_EDIT		 = 11,
		IID_CHAT_LIST		 = 12,
		IID_CHAT_SCROLLBAR	 = 13,
		IID_TABBEDPANE		 = 20,
		IID_TABBUTTON_MEMBER = 25,
		IID_EDITBOX_TITLE	 = 39,
		IID_EDITBOX_PWD		 = 34,
		IID_TABBUTTON_CONFIG = 35,
		IID_COMBOBOX_MAXUSER = 36,
		IID_BTN_MAKE		 = 32,
		IID_COMBOBOX_TYPE	 = 136,
		IID_PANE_TOP		 = 100,
		IID_PANE_BOTTOM		 = 200,
		IID_BTN_MINIMIZE	 = 113,
		IID_BTN_MAXIMIZE	 = 114,

	};
	enum{
		TAB_CONFIG	= 0,
		TAB_MEMBER	= 1,
	};

	bool	m_bMinimize;															/// �ּ�ȭ/�ִ�ȭ ����
	int		m_iTab;																	/// �� ����
	std::list< CChatMember >	m_Members;											/// ä�÷� ��� ���� �����̳�
};
#endif