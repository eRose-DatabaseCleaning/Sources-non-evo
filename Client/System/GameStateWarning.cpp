#include "stdafx.h"
#include "GameStateWarning.h"
#include "CGame.h"

#include "../Game.h"
#include "../TGameCtrl/TMsgBox.h"
#include "../Network/CNetwork.h"

#include "../Interface/ExternalUI/ExternalUILobby.h"
#include "../Interface/CTFontImpl.h"
#include "../Interface/IO_ImageRes.h"
#include "../interface/CUIMediator.h"
#include "../Interface/IT_MGR.H"
#include "../Interface/Command/CTCmdHotExec.h"
#include "../Interface/Cursor/CCursor.h"

#include "../Common/IO_Quest.h"

#include "../IO_Terrain.h"
#include "../CCamera.h"
#include "../CSkyDOME.H"
//#include "../nProtect/nProtect.h"
#include "../Misc/GameUtil.h"
#include "TIme2.h"
#include "SystemProcScript.h"



CGameStateWarning::CGameStateWarning(int iID)
{
	m_iStateID = iID;
}

CGameStateWarning::~CGameStateWarning(void)
{	
}


int	CGameStateWarning::Update( bool bLostFocus )
{
	g_EUILobby.Update();	

	::updateScene ();

	// processing  ...
	if ( !bLostFocus ) 
	{
		//Numenor: Constant rendering. Ideally we would like to add an option for this. But for the moment, let's just comment it :)
		//NB: bLostFocus is when we close the game while beginScene is really if the window is running or not.
		/*
		if ( !::beginScene() ) //  If the device is lost, the status returns 0, so skip all the rendering
		{
			return 0;
		}
		*/

		::clearScreen();
		::renderScene();


		::beginSprite( D3DXSPRITE_ALPHABLEND );

		/// Screen message display
		g_UIMed.Draw();

		/// UI display
		g_EUILobby.Draw();

		CTIme::GetInstance().Draw();


		::endSprite();		


		::endScene ();
		::swapBuffers();
	}
	return 0;

}

int CGameStateWarning::ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_EUILobby.MsgProc( uiMsg ,wParam, lParam) )
		return 1;

	return 0;
}

int CGameStateWarning::ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uiMsg ) 
	{
	case WM_CHAR :
		{
			switch ( wParam )
			{
			case '1':
				{
				}
				break;
			case '2':
				break;
			}
		}
		break;
	}

	if( g_EUILobby.MsgProc( uiMsg ,wParam, lParam) )
		return 1;	

	return 0;
}

int CGameStateWarning::Enter( int iPrevStateID )
{	
	g_EUILobby.InitEUIManager();
	g_EUILobby.CreateWarningDlg();

	return 0;
}

int CGameStateWarning::Leave( int iNextStateID )
{
	return 0;
}


