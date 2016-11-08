#include "stdafx.h"
#include "cgamestateexitmain.h"
#include <process.h>
#include "CGame.h"
#include "../IO_Terrain.h"
#include "../CCamera.h"
#include "../interface/CLoading.h"
#include "../interface/dlgs/CItemDlg.h"
#include "../Network/CNetwork.h"
#include "System_FUNC.h"
#include "SystemProcScript.h"
#include "../CSkyDOME.h"
#include "../gamedata/cprivatestore.h"
#include "../gamedata/cclan.h"
#include "../gamedata/cparty.h"
#include "../interface/it_mgr.h"
#include "../interface/ClanMarkManager.h"
#include "../interface/dlgs/QuickToolBAR.h"
#include "../interface/dlgs/CCommDlg.h"
#include "../CClientStorage.h"


int CGameStateExitMain::m_iBackGroundZone  = 1;

CGameStateExitMain::CGameStateExitMain( int iID )
{
	m_iStateID  = iID;
	m_iBackGroundZone = 1;
}

CGameStateExitMain::~CGameStateExitMain(void)
{

}


int	CGameStateExitMain::Update( bool bLostFocus )
{	
	return 0;
}

int CGameStateExitMain::Enter( int iPrevStateID )
{ 
	/// ������� ����� �� ��ȣ�� ���´�.
	m_iBackGroundZone = SC_GetBGZoneNO();

//	m_hThread = (HANDLE)_beginthreadex( NULL, 0, &ThreadFunc, NULL, CREATE_SUSPENDED, NULL );
////	SetThreadPriority( m_hThread,THREAD_PRIORITY_HIGHEST  );
//
//	if( m_hThread )
//	{
//		ResumeThread( m_hThread );
//	}
//	else///Thread ���� ���н� ���ξ����忡�� �ε��ϰ� State�� �ٲپ� �ش�.
	{
		g_pCamera->Detach();
		g_pTerrain->FreeZONE();
		g_Loading.LoadTexture( m_iBackGroundZone, ZONE_PLANET_NO( m_iBackGroundZone ) );

		Draw();

		/// Clear �� �����Կ� �ִ� Data �� ���� �������־�� �Ѵ�.
		/// Item�̳� ��ų�� �������� ����Ǿ� �ִ� ������(��ų)���� �ڵ�����
		/// ������ ��Ŷ�� ������ ����. 2004/8/20
		CClan::GetInstance().Clear();
		CPrivateStore::GetInstance().Clear();
		CParty::GetInstance().Leave();

	


		if( CTDialog* pDlg = g_itMGR.FindDlg( DLG_TYPE_COMMUNITY ) )
		{
			CCommDlg* pCommDlg = (CCommDlg*)pDlg;
			pCommDlg->ClearFriendList();
			pCommDlg->ClearMemoList();
			pCommDlg->ClearMemoList();
		}


		if( CTDialog* pDlg = g_itMGR.FindDlg( DLG_TYPE_ITEM ) )
		{
			CItemDlg* pItemDlg = (CItemDlg*)pDlg;
			std::list<S_InventoryData> list;
			pItemDlg->GetVirtualInventory( list );

			///CClientDB::GetInstance().SetInventoryData( g_pAVATAR->Get_NAME(), list );
			g_ClientStorage.SetInventoryData( g_pAVATAR->Get_NAME(), list );
		}

		if( CTDialog* pDlg = g_itMGR.FindDlg( DLG_TYPE_QUICKBAR ) )
		{
			CQuickBAR* pQuickbarDlg = (CQuickBAR*)pDlg;
			pQuickbarDlg->Clear();
		}

#ifdef _NEW_UI //2nd SkillBar
		if( CTDialog* pDlg = g_itMGR.FindDlg( DLG_TYPE_QUICKBAR_EXT ) )
		{
			CQuickBAR* pQuickbarDlg = (CQuickBAR*)pDlg;
			pQuickbarDlg->Clear();
		}	
#endif

		CItemSlot* pItemSlot = g_pAVATAR->GetItemSlot();
		pItemSlot->Clear();

		CSkillSlot* pSkillSlot = g_pAVATAR->GetSkillSlot();
		pSkillSlot->ClearSlot();

		CClanSkillSlot* pClanSkillSlot = CClan::GetInstance().GetClanSkillSlot();
		pClanSkillSlot->ClearSlot();

		g_pAVATAR->ResetClan();
		g_pObjMGR->Del_Object( g_pAVATAR );
		
		g_pAVATAR = NULL;
 

		CClanMarkManager::GetSingleton().Free();
		ThreadFunc(NULL);
		
		g_pNet->Send_cli_CHAR_LIST();


		g_itMGR.CloseDialogAllExceptDefaultView();
		g_itMGR.ClearNotifyButtons();



		SYSTEMTIME time;
		ZeroMemory( &time, sizeof( SYSTEMTIME ));
		CClan::GetInstance().SetClanMarkRegTime( time );
	}
	
	return 0; 
}

int CGameStateExitMain::Leave( int iNextStateID )
{ 
	g_Loading.UnloadTexture();

	::setDelayedLoad( 0 );
	g_pTerrain->LoadZONE( CGameStateExitMain::m_iBackGroundZone );
	///
	/// ī�޶� ����� 32_32 �������� ���������.. ��������� ���ؼ� �����Ѵ�.
	///
	D3DVECTOR PosENZIN;
	PosENZIN.x = 520000.0f;
	PosENZIN.y = 520000.0f;
	PosENZIN.z = 0.0f;

	g_pTerrain->InitZONE( PosENZIN.x, PosENZIN.y );

	CSkyDOME::Init( g_GameDATA.m_hShader_sky, g_GameDATA.m_hLight, 0 );

	HNODE camera_motion = loadMotion( "camera_motion", "3DData\\Title\\Camera01_inSelect01.zmo", 1, 4, 3, 1, 0 );
	HNODE motion_camera = loadCamera( "motion_camera", "cameras/camera01.zca", camera_motion );

	g_pCamera->Init( motion_camera );
	g_pCamera->Set_Position( PosENZIN );

	controlAnimatable( motion_camera, 1 );
	setRepeatCount( motion_camera, 1 );

	g_DayNNightProc.ShowAllEffect( false );

	::setDelayedLoad( 2 );	
	::setDelayedLoad( 0 );	
	return 0;
}

unsigned __stdcall CGameStateExitMain::ThreadFunc( void* pArguments )
{
//	::setDelayedLoad( 0 );

//	g_pTerrain->SetLoadingMode( true );


	return 0;
}

void CGameStateExitMain::Draw()
{
	if( g_pCApp->IsActive() )
	{
		//Numenor: Constant rendering. Ideally we would like to add an option for this. But for the moment, let's just comment it :)
		//NB: bLostFocus is when we close the game while beginScene is really if the window is running or not.
		/*
		if ( !::beginScene() ) //  If the device is lost, the status returns 0, so skip all the rendering
		{
			return;
		}
		*/

		::clearScreen();

		::beginSprite( D3DXSPRITE_ALPHABLEND );	

		g_Loading.Draw();

		::endSprite();

		::endScene();
		::swapBuffers();
	}
}