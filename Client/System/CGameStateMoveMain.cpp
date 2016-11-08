#include "stdafx.h"
#include ".\cgamestateMoveMain.h"
#include "CGame.h"
#include "SystemProcScript.h"
#include "../IO_Terrain.h"
#include "../CCamera.h"
#include "../interface/CUIMediator.h"
CGameStateMoveMain::CGameStateMoveMain( int iID )
{
	m_iStateID = iID;
}

CGameStateMoveMain::~CGameStateMoveMain(void)
{

}

void CGameStateMoveMain::ServerDisconnected()
{
	g_pCApp->SetExitGame();
}

int CGameStateMoveMain::ProcWndMsgInstant( unsigned uiMsg, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

int CGameStateMoveMain::ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

int CGameStateMoveMain::ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

int	CGameStateMoveMain::Update( bool bLostFocus )
{
	g_pCamera->Update ();

	D3DVECTOR PosENZIN = g_pCamera->Get_Position ();	
	g_pTerrain->SetCenterPosition( PosENZIN.x, PosENZIN.y );

	::updateScene ();

	// processing  ...
	if ( !bLostFocus ) 
	{
		//Numenor: Constant rendering. Ideally we would like to add an option for this. But for the moment, let's just comment it :)
		//NB: bLostFocus is when we close the game while beginScene is really if the window is running or not.

		//if ( ::beginScene() ) // ������ ��쿡�� ������
		//{
			::clearScreen();
			::renderScene();


			::beginSprite( D3DXSPRITE_ALPHABLEND );

			/// Screen message display
			g_UIMed.Draw();
			/// UI display
			///ī�޶� ����� ����������쿡�� ���̰� �Ѵ�.

			::endSprite();		


			::endScene ();
			::swapBuffers();
		//}
	}

	if( getAnimatableState( g_pCamera->GetZHANDLE() ) == 0 )
		CGame::GetInstance().ChangeState( CGame::GS_PREPAREMAIN );

	return 0;	
	
}

int	CGameStateMoveMain::Enter( int iPrevStateID )
{
	CSystemProcScript::GetSingleton().CallLuaFunction( "EnterMoveMain", ZZ_PARAM_END );
	return 0;
}

int	CGameStateMoveMain::Leave( int iNextStateID )
{
	CSystemProcScript::GetSingleton().CallLuaFunction( "LeaveMoveMain", ZZ_PARAM_END );	
	return 0;
}
