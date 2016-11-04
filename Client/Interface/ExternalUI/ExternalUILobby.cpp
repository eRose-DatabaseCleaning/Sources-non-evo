#include "stdafx.h"
#include "ExternalUILobby.h"

#include "IO_Terrain.h"
#include "Game.h"
#include "Game_FUNC.h"

#include "..\\..\\Network\\CNetwork.h"
#include "../../../TGameCtrl/ResourceMgr.h"
#include "../../../TGameCtrl/TDialog.h"
#include "../../../TGameCtrl/TMsgBox.h"
#include "../../../TGameCtrl/TListBox.h"
#include "../../../TGameCtrl/TCommand.h"
#include "../IO_ImageRes.h"
#include "../../Game.h"
#include "../../CClientStorage.h"
#include "../../System/CGame.h"
#include "../it_mgr.h"
#include "../../CCamera.h"
#include "../../CSkyDome.h"
#include "../Dlgs/CMsgBox.h"
#include "../Command/CTCmdHotExec.h"
#include "../cursor/ccursor.h"
CExternalUILobby		g_EUILobby;

void	ResetAvataList()
{
	LogString (LOG_DEBUG, "Deplicated Operation :: ResetAvataList\n" );
//	g_EUILobby.ReSetAvataListBox();
}

CExternalUILobby::CExternalUILobby()
{
	m_bLoginSuccess = false;	
	m_bSelectServer = false;
	m_bSelectAvata = false;
	m_bCreateAvata = false;
	m_pCurrDialog	= NULL;
	m_pMsgBox		= NULL;
}

CExternalUILobby::~CExternalUILobby()
{
}

void CExternalUILobby::Free()
{
	if( m_pMsgBox )
	{
		delete m_pMsgBox;
		m_pMsgBox = NULL;
	}
}

bool CExternalUILobby::InitEUIManager()
{
	m_EUIManager.InitExternalUIs();
	m_pMsgBox = new CMsgBox;
	m_pMsgBox->Create("MsgBox");
	return true;
}


///////////////////////////////////////////////////////////////////////////////////
/// Login

bool CExternalUILobby::CreateLoginDlg()
{
//	::CreateLoginDlg();
	m_pCurrDialog = (CTDialog*)m_EUIManager.GetEUI( EUI_LOGIN );
	m_pCurrDialog->Show();

	return true;
}

void CExternalUILobby::CloseLoginDlg()
{
	((CTDialog*)m_EUIManager.GetEUI( EUI_LOGIN ))->Hide();
}


///////////////////////////////////////////////////////////////////////////////////
// Warning
bool CExternalUILobby::CreateWarningDlg()
{
	m_pCurrDialog = (CTDialog*)m_EUIManager.GetEUI( EUI_WARNING );
	m_pCurrDialog->Show();

	return true;
}
void CExternalUILobby::CloseWarningDlg()
{
	((CTDialog*)m_EUIManager.GetEUI( EUI_WARNING ))->Hide();
}

///////////////////////////////////////////////////////////////////////////////////
/// Select server

bool CExternalUILobby::CreateServerListDlg()
{
	m_pCurrDialog->Hide();
	m_pCurrDialog = (CTDialog*)m_EUIManager.GetEUI( EUI_SELECT_SERVER );
	m_pCurrDialog->Show();
	return true;
}
void CExternalUILobby::CloseServerListDlg()
{
	((CTDialog*)m_EUIManager.GetEUI( EUI_SELECT_SERVER ))->Hide();
}

bool CExternalUILobby::ShowDialog( int iType )
{
	assert( iType >= EUI_LOGIN && iType < EUI_MAX );
	if( iType >= EUI_LOGIN && iType < EUI_MAX )
	{
		m_pCurrDialog = m_EUIManager.GetEUI( iType );
		m_pCurrDialog->Show();
		return true;
	}
	return false;
}

bool CExternalUILobby::HideDialog( int iType )
{
	assert( iType >= EUI_LOGIN && iType < EUI_MAX );
	if( iType >= EUI_LOGIN && iType < EUI_MAX )
	{
		((CTDialog*)m_EUIManager.GetEUI( iType ))->Hide();
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////
/// Wait Avata
bool CExternalUILobby::CreateWaitAvataListDlg()
{
//	CreateWaitAvataDlg();
//	ShowWaitMsgBox();
//	ShowMsgBox("�ƹ�Ÿ ����Ʈ�� �޽��ϴ�",CTMsgBox::BT_CANCEL, CTMsgBox::DT_MODAL, EUI_MAX, MSG_TYPE_WAIT );
	return true;
}

void CExternalUILobby::CloseWaitAvataListDlg()
{
	HideMsgBox();
	CreateAvataListDlg();
//	DestroyWaitAvataDlg();
}


///////////////////////////////////////////////////////////////////////////////////
/// Select Avata

bool CExternalUILobby::CreateAvataListDlg()
{
//	CreateSelectAvataDlg();
	m_pCurrDialog->Hide();
	m_pCurrDialog = (CTDialog*)m_EUIManager.GetEUI( EUI_SELECT_AVATA );
	m_pCurrDialog->Show();
	return true;
}
void CExternalUILobby::CloseAvataListDlg()
{
	((CTDialog*)m_EUIManager.GetEUI( EUI_SELECT_AVATA ))->Hide();
}


// Create Avata
bool CExternalUILobby::CreateCreateAvataDlg()
{
	m_pCurrDialog->Hide();
	m_pCurrDialog = (CTDialog*)m_EUIManager.GetEUI( EUI_CREATE_AVATA );
	m_pCurrDialog->Show();
	return true;
}


void CExternalUILobby::CloseCreateAvataDlg()
{
	((CTDialog*)m_EUIManager.GetEUI( EUI_CREATE_AVATA ))->Hide();
}


///////////////////////////////////////////////////////////////////////////////////
// ���� ĳ�� ������ �� ĳ���� ���õɶ����� ���..
bool CExternalUILobby::CreateWaitJoinServerDlg()
{
	ShowWaitMsgBox();
	return true;
}

void CExternalUILobby::CloseWaitJoinServerDlg()
{
	HideMsgBox();
}

void CExternalUILobby::Update()
{
	POINT ptMouse;

	CGame::GetInstance().Get_MousePos( ptMouse );
		
	/// Control���� ���������� ó���ϴ� ���� �ʱ�ȭ 
	/// Update�� MouseOveró�� ���� �ʱ�ȭ
	CWinCtrl::SetProcessMouseOver( false );
	CTDialog::SetProcessMouseOverDialog( NULL );
	///
	if( !m_pMsgBox->IsVision() )
	{
		m_pCurrDialog->Update( ptMouse );
	}
	else
	{
		m_pMsgBox->Update( ptMouse );
		if( !m_pMsgBox->IsModal() )
			m_pCurrDialog->Update( ptMouse );
	}

}


void CExternalUILobby::Draw()
{
	/*D3DXMATRIX mat;	
	float fScaleWidth  = (float)(g_GameDATA.m_nScrWidth / 1024.);
	float fScaleHeight = (float)(g_GameDATA.m_nScrHeight / 768.);
	D3DXMatrixScaling( &mat, fScaleWidth, fScaleHeight, 0.0f );

	HNODE hBG = CGame::GetInstance().GetExUIBackGround();
	
	::setTransformSprite( mat );
	::drawSprite( hBG, 
					NULL,
					NULL,
					&D3DXVECTOR3( 0, 0, 0 ),								
					D3DCOLOR_RGBA( 255, 255, 255, 255 ) );*/

	if ( !g_GameDATA.m_bNoUI ) 
	{
		m_pCurrDialog->Draw();
		DrawMsgBox();
	}

	POINT ptMouse;
	CGame::GetInstance().Get_MousePos( ptMouse );
	CCursor::GetInstance().Draw( ptMouse );
}

bool CExternalUILobby::MsgProc(UINT uiMsg,WPARAM wParam,LPARAM lParam)
{
	/// Ctrl���� ����ó���� ���� Setting
	if( uiMsg == WM_LBUTTONUP )
		CWinCtrl::SetMouseExclusiveCtrl( NULL );

	if( m_pMsgBox->Process( uiMsg, wParam, lParam ) )
		return true;
	if( m_pMsgBox && m_pMsgBox->IsVision() && m_pMsgBox->IsModal() )
		return true;

	return (m_pCurrDialog->Process( uiMsg, wParam, lParam )?true:false);
}

void CExternalUILobby::ReSetAvataListBox()
{
	CTDialog* pSelectAvatarDlg = (CTDialog*)m_EUIManager.GetEUI( EUI_SELECT_AVATA );
	//if( pSelectAvatarDlg )
	//{
	//	((CSelectAvata*)pSelectAvatarDlg)->Clear();
	//}
}
void CExternalUILobby::ShowMsgBox( char* szMsg, short int iButtonType, bool bModal ,unsigned int iInvorkerDlgType , CTCommand* pCmdOk, CTCommand* pCmdCancel)
{
	if( m_pMsgBox )
	{
		m_pMsgBox->SetButtonType( iButtonType );

		if( bModal )
			m_pMsgBox->SetModal();
		else
			m_pMsgBox->SetModeless();

		m_pMsgBox->SetCommand( pCmdOk, pCmdCancel );
		m_pMsgBox->SetInvokerDlgID( iInvorkerDlgType );
		m_pMsgBox->SetString( szMsg );
		m_pMsgBox->Show();

		POINT pt = { 
			g_pCApp->GetWIDTH() / 2 - m_pMsgBox->GetWidth() / 2,
			g_pCApp->GetHEIGHT() / 2 - m_pMsgBox->GetWidth() / 2
		};

		m_pMsgBox->MoveWindow(pt);
	}
}
void CExternalUILobby::HideMsgBox()
{
	if( m_pMsgBox )
		m_pMsgBox->Hide();
}
void CExternalUILobby::DrawMsgBox()
{
	if( m_pMsgBox && m_pMsgBox->IsVision() )
	{
		m_pMsgBox->Draw();
	
	}
}

void CExternalUILobby::FailCreateAvata()
{
	CTDialog* pDialog = m_EUIManager.GetEUI( EUI_CREATE_AVATA );
	if( pDialog )
		((CCreateAvata*)pDialog)->FailCreateAvata();
}

void CExternalUILobby::ShowWaitMsgBox()
{
	ShowMsgBox("Waiting...", 0, true, EUI_MAX );
}

void CExternalUILobby::ServerDisconnected()
{
// 05. 10. 27 - ������
// __SRVDISCONNECTRELOGIN(����������Ʈ�� �Ǿ��� ��쿡 �α���â���� �̵��Ұ��ΰ�
// �ƴϸ�... ���α׷��� �����Ұ��ΰ�..

#ifdef __SRVDISCONNECTRELOGIN
		CTCommand* pCmd = new CTCmdReLogin; // �̰� ��α���
#else
		CTCommand* pCmd = new CTCmdExit; // �̰� ����
#endif
	ShowMsgBox( "Server Disconnected", CTMsgBox::BT_OK , true , EUI_MAX , pCmd );
}

void CExternalUILobby::AddTCommand( int iInvorkerDlgID, CTCommand* pCmd )
{
	if( pCmd == NULL )
		return;

	//��� ���� Ŀ�ǵ�=>IT_MGR�� CommandQ�� ������ �����ȴ�. Q�� �ְ�Update���� �����Ű��
	//�ܺδ� ��� �������???????????
	if( iInvorkerDlgID == EUI_MAX )
	{
		pCmd->Exec(NULL);
		delete pCmd;
	}
	else
	{
		if( m_pCurrDialog->GetDialogType() == iInvorkerDlgID )
			m_pCurrDialog->Push_Back_Cmd( pCmd );
		else
			delete pCmd;
	}
	
}
void CExternalUILobby::SendReqFirstServerChannelList()
{
	((CSelectServer*)m_EUIManager.GetEUI( EUI_SELECT_SERVER ))->SendReqFirestServerChannelList();
}

CExternalUIManager& CExternalUILobby::GetExternalUIManager()
{
	return m_EUIManager;
}