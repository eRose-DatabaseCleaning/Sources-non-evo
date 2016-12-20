#include "stdafx.h"
#include ".\cgamestateselectsvr.h"
#include "../Interface/ExternalUI/ExternalUILobby.h"
#include "../CCamera.h"
#include "../IO_Terrain.h"
#include "../interface/CUIMediator.h"



#include "../Game.h"
#include "../System/CGame.h"
#include "../TGameCtrl/TMsgBox.h"
#include "../Network/CNetwork.h"

#include "../Interface/ExternalUI/ExternalUILobby.h"
#include "../Interface/CTFontImpl.h"
#include "../Interface/IO_ImageRes.h"

#include "../Interface/IT_MGR.H"
#include "../Interface/Command/CTCmdHotExec.h"
#include "../Interface/Cursor/CCursor.h"

#include "../Common/IO_Quest.h"



#include "../CSkyDOME.H"
#include "TIme2.h"
#include "SystemProcScript.h"

CGameStateSelectSvr::CGameStateSelectSvr( int iID )
{
	m_iStateID = iID;
	m_iPrevState = 0;
}

CGameStateSelectSvr::~CGameStateSelectSvr(void)
{
}
int CGameStateSelectSvr::Update( bool bLostFocus )
{
#ifdef _ONLY_ONE_SERVER
	static bool OneTimeProc = true;
		if(OneTimeProc)
		{
			OneTimeProc = false;
			CTDialog* pDlg = g_EUILobby.GetEUI( EUI_SELECT_SERVER ) ;
			assert( pDlg );
			if( pDlg )
			{
				CSelectServer*	pLoginDlg = (CSelectServer*)pDlg;
				pLoginDlg->SendSelectChannel();
			}
		}
#endif
	g_EUILobby.Update();
	
	g_pCamera->Update ();

	D3DVECTOR PosENZIN = g_pCamera->Get_Position ();	
	g_pTerrain->SetCenterPosition( PosENZIN.x, PosENZIN.y );

	g_DayNNightProc.ShowAllEffect( false );

	::updateScene ();

	// processing  ...
	if ( !bLostFocus ) 
	{
		
		if ( ::beginScene() ) //  ����̽��� �սǵ� ���¶�� 0�� �����ϹǷ�, ��� ������ ��ŵ
		{
			::clearScreen();
			::renderScene();
			
		
			::beginSprite( D3DXSPRITE_ALPHABLEND );

			/// Screen message display
			g_UIMed.Draw();

			/// UI display
			g_EUILobby.Draw();
			
			::endSprite();		

			::endScene ();
			::swapBuffers();
		}
	}
	return 0;
}

int CGameStateSelectSvr::Enter( int iPrevStateID )
{
	m_iPrevState = iPrevStateID;
	return 1;
}

int CGameStateSelectSvr::Leave( int iNextStateID )
{
	
	return 1;
}

void CGameStateSelectSvr::ServerDisconnected()
{
	g_EUILobby.ServerDisconnected();
}

int CGameStateSelectSvr::ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_EUILobby.MsgProc( uiMsg ,wParam, lParam) )
		return 1;	

	return 0;
}

int CGameStateSelectSvr::ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_EUILobby.MsgProc( uiMsg ,wParam, lParam) )
		return 1;

	return 0;
}