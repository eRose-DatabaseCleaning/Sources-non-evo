/*
    $Header: /Client/Game.cpp 293   05-06-13 9:03p Navy $
*/

#include "stdAFX.h"

#include "Game.h"
//#include "CMouse.h" 
//#include "CKeyboard.h"
#include "IO_Basic.h"
#include "IO_Terrain.h"
#include "IO_Effect.h" 
#include "IO_Event.h"
#include "CModelCHAR.h"
#include "CCamera.h"
#include "Network/CNetwork.h"
#include "OBJECT.h"
#include "BULLET.h"
#include "CSkyDOME.h"
#include "util\classMD5.h"
#include "Common/IO_AI.h"
#include "Common/IO_STB.h"
#include "TGameCtrl.h"
#include "InterFace/it_mgr.h"
#include "InterFace/IO_ImageRes.h"

#include "Game_FUNC.h"
#include "CViewMSG.h"
#include "Debug.h"
#include "Common/IO_Skill.h"
#include "Common/IO_Quest.h"
#include "Interface/CLoading.h"
#include "Interface/CKeyMove.h"	
//#include "Interface\\CUITextManager.h"
#include "Interface/TypeResource.h"
#include "Interface/CUIMediator.h"
#include "Interface/ExternalUI/ExternalUILobby.h"
#include "../TGameCtrl/ResourceMgr.h"
#include "Interface/IO_ImageRes.h"
#include "Interface/CTDrawImpl.h"
#include "Interface/CTSoundImpl.h"
#include "Interface/CTFontImpl.h"
#include "CClientStorage.h"
#include "GameProc/CDayNNightProc.h"
#include "System/CGame.h"



CGAMEDATA _GameData;



CObjUSER	   *g_pAVATAR=NULL;


D3DCOLOR	g_dwRED		= D3DCOLOR_ARGB( 255, 255, 0, 0);
D3DCOLOR	g_dwGREEN	= D3DCOLOR_ARGB( 255, 0, 255, 0);
D3DCOLOR	g_dwBLUE	= D3DCOLOR_ARGB( 255, 0,  0,255);
D3DCOLOR	g_dwBLACK	= D3DCOLOR_ARGB( 255, 0, 0, 0);
D3DCOLOR	g_dwWHITE	= D3DCOLOR_ARGB( 255, 255, 255, 255);
D3DCOLOR	g_dwYELLOW  = D3DCOLOR_ARGB( 255, 255, 255, 0);
D3DCOLOR	g_dwGRAY    = D3DCOLOR_ARGB( 255, 150, 150, 150);
D3DCOLOR	g_dwVIOLET  = D3DCOLOR_ARGB( 255, 255,   0, 255);
D3DCOLOR	g_dwORANGE  = D3DCOLOR_ARGB( 255, 255, 128, 0);
D3DCOLOR	g_dwPINK	= D3DCOLOR_ARGB( 255, 255, 136, 200 );
D3DCOLOR g_dwCOLOR[] = { g_dwRED,
						g_dwGREEN,
						g_dwBLUE,
						g_dwBLACK,
						g_dwWHITE,
						g_dwYELLOW,
						g_dwGRAY,
						g_dwVIOLET,
						g_dwORANGE,
						g_dwPINK
						};			 

//// Util �� Output string �� ����
void WriteLOG( char* )
{
	;
}



//
//void AddMsgToChatWND (char *szMsg, D3DCOLOR Color, int iType )
//{
//	if ( NULL != szMsg ) 
//	{
//		g_itMGR.AppendChatMsg(szMsg, Color , iType );						
//	}
//}


void DrawLoadingImage()
{
	//bool bLostFocus = g_pCApp->GetMessage ();
	//
	//if( !bLostFocus )
	{
		//Numenor: Constant rendering. Ideally we would like to add an option for this. But for the moment, let's just comment it :)
		//NB: bLostFocus is when we close the game while beginScene is really if the window is running or not.
		/*
		if ( !::beginScene() ) //  If the device is lost, the status returns 0, so skip all the rendering
		{
			return;
		}
		*/

		g_Loading.LoadTexture();

		::clearScreen();

		::beginSprite( D3DXSPRITE_ALPHABLEND );	

		g_Loading.Draw();

		::endSprite();

		::endScene();
		::swapBuffers();
		g_Loading.UnloadTexture();
	}		
}

void DestroyWaitDlg()
{
	CGame::GetInstance().ProcWndMsg(WM_USER_CLOSE_MSGBOX,0,0 ); 
}



//-------------------------------------------------------------------------------------------------

CGAMEDATA::CGAMEDATA () 
{
	m_bWireMode				= false;
	m_bDrawBoundingVolume	= false;
	m_bNoUI					= false;
	m_bTranslate			= false;
	m_bNoWeight				= false;;///�������� ���� flag: ���� Ŭ���̾�Ʈ���� ���� 
	m_fTestVal				= 0.0f;
	m_iPvPState				= PVP_CANT;

	m_dwGameStartTime		= GetTime();		/// ���� ���� �ð�.
	m_dwFrevFrameTime		= 0;				/// ���� ������ Ÿ��
	m_dwElapsedGameTime		= 0;				/// ���� ������ ����ð�.
	m_dwElapsedFrameTime	= 0;				/// ���� �����ӿ����� ����ð�..
	m_bForOpenTestServer    = false;

	GetSystemTime( &m_SystemTime );

	RandomSeedInit( m_SystemTime.wMilliseconds );

#ifdef _DEBUG
	m_bShowCurPos			= false;
#endif

	m_bFilmingMode			= false;
	m_bShowCursor			= true;
	m_bShowDropItemInfo		= false;

	m_iWorldStaminaVal		= 0;


	m_dwElapsedGameFrame	= 0;

	m_bJustObjectLoadMode	= false;

#ifdef _DEBUG
	m_bObserverCameraMode	= false;; /// ������ ������ ī�޶�..
#endif

	m_iReceivedAvatarEXP	= 0;
	m_is_NHN_JAPAN			= false;
}

CGAMEDATA::~CGAMEDATA ()
{
}

void CGAMEDATA::UpdateGameTime()
{
	m_dwElapsedGameTime = GetTime() - m_dwGameStartTime;

	m_dwElapsedFrameTime = m_dwElapsedGameTime - m_dwFrevFrameTime;
	m_dwFrevFrameTime = m_dwElapsedGameTime;
}

void CGAMEDATA::Update()
{
	UpdateGameTime();

	m_dwElapsedGameFrame++;
}

int CGAMEDATA::AdjustAvatarStamina( int iGetExp )
{
	int iLostExp = ( ( iGetExp + 20) / ( g_pAVATAR->Get_LEVEL() + 4 ) + 5 ) * ( m_iWorldStaminaVal ) / 100;
	g_pAVATAR->m_GrowAbility.m_nSTAMINA -= iLostExp;

	if( g_pAVATAR->m_GrowAbility.m_nSTAMINA < 0 )
		g_pAVATAR->m_GrowAbility.m_nSTAMINA = 0;

	return g_pAVATAR->m_GrowAbility.m_nSTAMINA;
}