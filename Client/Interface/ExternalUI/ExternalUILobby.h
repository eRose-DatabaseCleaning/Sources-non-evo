#ifndef _EXTERNALUILOBBY_
#define _EXTERNALUILOBBY_

#include "CExternalUIManager.h"
//#include "../../../TGameCtrl/TImage.h"

#include <string>

class CTDialog;
class CMsgBox;

/**
* �ܺ� �������̽�( �α��� ~ ĳ���� ����, ���ӿ� ����������)�� �����ϴ� Class
*
* @Author		������
* @Date			2005/9/6
*/

class CExternalUILobby
{
private:
	CTDialog*				m_pCurrDialog;
	CMsgBox*				m_pMsgBox;

public:
	///
	/// Member for close test
	///
	CExternalUIManager		m_EUIManager;

	bool					m_bLoginSuccess;
	bool					m_bSelectServer;
	bool					m_bWaitAvata;
	bool					m_bSelectAvata;
	bool					m_bCreateAvata;
	bool					m_bWaitJoinServer;


private:	

public:
	CExternalUILobby();
	~CExternalUILobby();

	//--------------------------------------------------------------------
	// Get method for external uies..
	//--------------------------------------------------------------------
	CTDialog*		GetEUI( int iType ){ return m_EUIManager.GetEUI( iType ); };

	//--------------------------------------------------------------------
	// Member function for close test
	//--------------------------------------------------------------------

	void Update();
	void Draw();
	bool MsgProc(UINT uiMsg,WPARAM wParam,LPARAM lParam);

	bool InitEUIManager();
	void Free();
	// Login
	bool CreateLoginDlg();
	void CloseLoginDlg();

	// Warning
	bool CreateWarningDlg();
	void CloseWarningDlg();

	// Select server
	bool CreateServerListDlg();
	void CloseServerListDlg();

	// Wait Avata
	bool CreateWaitAvataListDlg();	
	void CloseWaitAvataListDlg();

	// Select Avata
	bool CreateAvataListDlg();
	void ReSetAvataListBox();
	void CloseAvataListDlg();

	// Create Avata
	bool CreateCreateAvataDlg();
	void CloseCreateAvataDlg();
	void FailCreateAvata();

	// ���� ĳ�� ������ �� ĳ���� ���õɶ����� ���..
	bool CreateWaitJoinServerDlg();
	void CloseWaitJoinServerDlg();
	///���� �޼��� �ڽ� ����

	bool ShowDialog( int iType );
	bool HideDialog( int iType );

	void ShowMsgBox( char* szMsg, short int iButtonType, bool bModal ,unsigned int iInvorkerDlgType , CTCommand* pCmdOk = NULL, CTCommand* pCmdCancel = NULL );
	void ShowWaitMsgBox();
	void HideMsgBox();
	bool MsgBoxProc( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	void ServerDisconnected();
	void SendReqFirstServerChannelList();
	enum{
		MSG_TYPE_WAIT,
		MSG_TYPE_EXIT
	};
	void AddTCommand( int iInvorkerDlgID, CTCommand* pCmd );

	CExternalUIManager& GetExternalUIManager();
protected:
	void DrawMsgBox();
};

extern CExternalUILobby		g_EUILobby;

#endif //_EXTERNALUILOBBY_