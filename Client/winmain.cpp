// Client.cpp : Defines the entry point for the application.


//*-------------------------------------------------------------------------------------------------------------------*
// 2005 / 06 / 20 : nAvy 
// ����� ���� ��ó���� ���� : 
//  1. _TAIWAN  : TAIWAN, PHILIPHIN ���ӽ� �α��μ������� ���� �ڵ�( ���� 950 )�� �������� �����ϱ� ���� ����
//  2. _DE		: �ߺ����� ������ ���������� ���� ����( KR )���� �����Ų��.
//
//  * ������ �ַ�� ����
//	1. �ѱ� 
//      1) ����   : Release_DE
//		2) ������ : Release
//
//  2. �Ϻ�, �̱� : Release
//
//  3. �븸, �ʸ��� :Release_TAIWAN
//*-------------------------------------------------------------------------------------------------------------------*

#include "stdafx.h"
#define AROSE

#include "CApplication.h"
#include "Game.h"
//#include "CMouse.h"
//#include "CKeyboard.h"
#include "Network\\CNetwork.h"
#include "Util\\VFSManager.h"
#include "Util\\SystemInfo.h"
#include "CClientStorage.h"
#include "System/CGame.h"
#include "Interface/ExternalUI/CLogin.h"
#include "Debug.h"
#include "TriggerInfo.h"

#include "Util/CheckHack.h"
#include "Country.h"
//#include "nProtect/nProtect.h"

#include "MiniDumper.h"

MiniDumper g_MiniDump;

///#include "Util/JDebugNew.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>



//*--------------------------------------------------------------------------------------*/
// 95,98, me���� Unicode���� Api Wrapper dll Load
//HMODULE LoadUnicowsProc(void); 
//HMODULE g_hUnicows = NULL;
//#ifdef _cplusplus
//extern "C" {
//#endif
//extern FARPROC _PfnLoadUnicows = (FARPROC) &LoadUnicowsProc;
//#ifdef _cplusplus
//}
//#endif
//
//
//// Unicode Load
//HMODULE LoadUnicowsProc(void)
//{
//	g_hUnicows = LoadLibraryA("unicows.dll");
//    return g_hUnicows;
//}
//*--------------------------------------------------------------------------------------*/


//-------------------------------------------------------------------------------------------------
bool Init_DEVICE (void)
{
	bool bRet = false;

	//--------------------------[ engine related ]-----------------------//
	::initZnzin();	
	::openFileSystem("data.idx");


	::doScript("scripts/init.lua");

	t_OptionResolution Resolution = g_ClientStorage.GetResolution();
	::setDisplayQualityLevel( c_iPeformances[g_ClientStorage.GetVideoPerformance()] );
	t_OptionVideo Video;
	g_ClientStorage.GetVideoOption(Video);
	setFullSceneAntiAliasing( Video.iAntiAlising );


	if(!g_pCApp->IsFullScreenMode()) 
	{
		RECT ClientRt;
		GetClientRect(g_pCApp->GetHWND(),&ClientRt);
		::setScreen(ClientRt.right, ClientRt.bottom, Resolution.iDepth, g_pCApp->IsFullScreenMode() );		
	}
	else
		::setScreen(g_pCApp->GetWIDTH(), g_pCApp->GetHEIGHT(), Resolution.iDepth, g_pCApp->IsFullScreenMode() );


	bRet = ::attachWindow((const void*)g_pCApp->GetHWND());


	CD3DUtil::Init( );

	g_pSoundLIST = new CSoundLIST( g_pCApp->GetHWND() );
	g_pSoundLIST->Load ( "3DDATA\\STB\\FILE_SOUND.stb" );

	return bRet;
}

//-------------------------------------------------------------------------------------------------
void Free_DEVICE (void)
{	
	delete g_pSoundLIST;

	CD3DUtil::Free ();

	//--------------------------[ engine related ]-----------------------//
	::detachWindow();

	::closeFileSystem();
	::destZnzin();  
}
//------------------------------------------------------------------------------------------------
// 2005. 5. 6. ��ȣ��
// �ߺ� ���� üũ�� ���� ����
SOCKET listener;

void CloseDuplicateAppSocket (void)
{
	closesocket(listener);
	::WSACleanup();
}

// �ߺ� ���� üũ : Ư�� ��Ʈ�� �ߺ� ���� �ȵǴ� ���� �̿�.
bool IsDuplicateApp (void)
{
	WSADATA wsadata;
	::WSAStartup( MAKEWORD( 2, 2 ), &wsadata );

	// socket ����
	listener = ::socket(AF_INET, SOCK_STREAM, 0);
	// listening
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(7777);

	int result = ::bind(listener, (sockaddr*) &addr, sizeof(sockaddr_in));
	result = ::listen(listener, 5);

	if( result == SOCKET_ERROR )
	{
		CloseDuplicateAppSocket();
		MessageBox(NULL, "�̹� ������ ���� ���Դϴ� !", "����", MB_OK );
		return TRUE;
	}

	return FALSE;
}

//-------------------------------------------------------------------------------------------------
int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef _USE_BG
	SetExceptionReport();
#endif

// #ifdef AROSE
// #define initval 5465463
// 	if(!InitVFS(0x536577))
// 		MessageBox(NULL, "Init Error", "VFS Init Error", 0);
// #endif // AROSE

	// *-------------------------------------------------------------------* //
	// 2005. 5. 6. ��ȣ��
#if 0
	//#ifndef _DE
	if( IsDuplicateApp() )
		return FALSE;
#endif
	// *-------------------------------------------------------------------* //

	//-------------------------------------------------------------------------------
	/// Hack check..
	//-------------------------------------------------------------------------------	
	///	if( CCheckHack::GetSingleton().Check() == false )
	///		return 0;


	/*
	_CrtSetDbgFlag (
	_CRTDBG_ALLOC_MEM_DF |
	_CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode ( _CRT_ERROR,
	_CRTDBG_MODE_DEBUG);
	//*/


	//-------------------------------------------------------------------------------
	/// �����ڵ� �ν�
	//-------------------------------------------------------------------------------
	CCountry::GetSingleton().CheckCountry();

	//nProtect ���� 
	//ȫ�� : ������ ���Ӱ��� ���� ��Ű�� �ʴ´�.
	/*#ifndef _DEBUG
	if( !m_nProtectSys.InitProtect() )
	return 0; 
	#endif*/

	//KeyCrypt ���� 
	//if( !m_npKeyCrypt.InitKeyCrypt() )
	//	return 0;
	///m_npKeyCrypt.IsGetUse() = FALSE;


	g_SystemInfo.CollectingSystemInfo();
	int iWindowVersion = g_SystemInfo.GetWindowsVersion();

	//-------------------------------------------------------------------------------
	/// Init Trigger VFS
	//-------------------------------------------------------------------------------
	VHANDLE hVFS = OpenVFS( "data.idx", (iWindowVersion == WINDOWS_98)?"r":"mr" );	
	(CVFSManager::GetSingleton()).SetVFS( hVFS );
	(CVFSManager::GetSingleton()).InitVFS( VFS_TRIGGER_VFS );	


	//-------------------------------------------------------------------------------
	/// Get Time
	//-------------------------------------------------------------------------------
	GetLocalTime(	&g_GameDATA.m_SystemTime );	


	//-------------------------------------------------------------------------------
	/// Init System object
	//-------------------------------------------------------------------------------
	g_pCApp		= CApplication::Instance ();
	g_pNet		= CNetwork::Instance (hInstance);
	g_pCRange	= CRangeTBL::Instance ();


	//-------------------------------------------------------------------------------
	/// Load Range table
	//-------------------------------------------------------------------------------
	if ( !g_pCRange->Load_TABLE ("3DDATA\\TERRAIN\\O_Range.TBL") ) {
		g_pCApp->ErrorBOX ( "3DDATA\\TERRAIN\\O_Range.TBL file open error", CUtil::GetCurrentDir (), MB_OK);
		return 0;
	}	


	//-------------------------------------------------------------------------------
	/// IP/Port Setting
	//-------------------------------------------------------------------------------
	g_GameDATA.m_wServerPORT = TCP_LSV_PORT;
	g_GameDATA.m_ServerIP.Set( TCP_LSV_IP );

	if ( !g_pCApp->ParseArgument( lpCmdLine ) )
		return 0;

	//-------------------------------------------------------------------------------
	/// ������ ������ �ػ󵵿� ���� Data�� �ʿ��Ͽ� �̰����� �ε��Ѵ�.
	//-------------------------------------------------------------------------------	
	g_TblResolution.Load2( "3DDATA\\STB\\RESOLUTION.STB",	false, false );
	g_TblCamera.Load2( "3DDATA\\STB\\LIST_CAMERA.STB" ,false, false );

	//-------------------------------------------------------------------------------
	///Ŭ���̾�Ʈ�� ����� Data�� �ε��Ѵ�.
	//-------------------------------------------------------------------------------
	g_ClientStorage.Load();

	//-------------------------------------------------------------------------------
	///���� �ɼǿ��� ������ �ػ��� �ε����� �����ͼ� g_TblResolution�� �����Ͽ�
	///�ػ󵵸� �����Ѵ�.
	//-------------------------------------------------------------------------------	
#pragma message("����� �ػ󵵸� �׽�Ʈ�ؼ� ���� ���డ������ �Ǵ��ϰ� �ȵǸ� �޼��� �ڽ� ���� TriggerDetect�� �����Ų��")
	t_OptionResolution Resolution = g_ClientStorage.GetResolution();
	/// ������ üũ 
	UINT iFullScreen = g_ClientStorage.GetVideoFullScreen();

	g_pCApp->SetFullscreenMode( iFullScreen );
	g_pCApp->CreateWND ("classCLIENT", "RoseAs [loading]", Resolution.iWidth, Resolution.iHeight,Resolution.iDepth, hInstance);



#ifndef _DEBUG
	// �ý��� ������ ����
	TI_ReadSysInfoFile ();

#endif


	// *-------------------------------------------------------------------* //
	g_pObjMGR = CObjectMANAGER::Instance ();
	g_pCApp->ResetExitGame();

	bool bDeviceInitialized = Init_DEVICE();

	if ( bDeviceInitialized ) {

		///nProtect				�� ��ȣ
		//m_nProtectSys.Set_hWnd(g_pCApp->GetHWND());

		///�ߺ����డ�� ����	
#ifndef singleclient
		CGame::GetInstance().GameLoop();
#else
		///�ߺ����ื�� ����
		HANDLE  hMUTEX = NULL;
		const char szMUTEX[] = "ROSEonline";

		/// �ߺ����� üũ...		2004. 9. 30 by icarus
#ifdef	_DEBUG
		if ( !g_GameDATA.m_bCheckDupRUN ) 
		{
			CGame::GetInstance().GameLoop();
		} 
		else
#endif
		{
			hMUTEX = ::CreateMutex( NULL, true, szMUTEX );
			switch( GetLastError() ) {
				case ERROR_SUCCESS	:
					CGame::GetInstance().GameLoop();

					::ReleaseMutex( hMUTEX );
					::CloseHandle( hMUTEX );
					break;
				case ERROR_ALREADY_EXISTS :
					// �̹� �ٸ� Ŭ���̾�Ʈ�� �������̴�..
				default :
					// ���� ???
					break;
			}				
			/*	
			hMUTEX = ::OpenMutex( MUTEX_ALL_ACCESS, false, szMUTEX );
			if ( NULL == hMUTEX ) {
			hMUTEX = ::CreateMutex( NULL, true, szMUTEX );

			CGame::GetInstance().GameLoop();

			::ReleaseMutex( hMUTEX );
			::CloseHandle( hMUTEX );
			} /// else �̹� �ٸ� Ŭ���̾�Ʈ�� �������̴�..
			*/
		}
#endif
	}
	else {
#ifdef _USE_BG
		ReportZnzinLog( "�ʱ�ȭ�� �����Ͽ����ϴ�. ���������� �����Ͻðڽ��ϱ�?", 10 );
#endif
	}

	// *-------------------------------------------------------------------* //
	// 2005. 5. 6. ��ȣ��
#if !defined(_DE) && !defined(_TAIWAN)
	//#ifndef _DE
	CloseDuplicateAppSocket();
#endif
	// *-------------------------------------------------------------------* //

	Free_DEVICE ();


	// *-------------------------------------------------------------------* //
	g_TblCamera.Free();
	g_TblResolution.Free();
	// *-------------------------------------------------------------------* //

	g_pCApp->Destroy ();
	g_pNet->Destroy ();

	//-------------------------------------------------------------------------------
	///2004/3/26/nAvy:Release �������� Debug Mode(F5�ν���)�� ����� �α��� ȭ�鿡�� ����ϸ�
	///Error����. ���� �� , ������ �׳� ����ÿ��� (bat���Ϸ� Ȥ�� ctrl+f5) Error�� �ȳ���.
	//-------------------------------------------------------------------------------		
	g_pCRange->Destroy ();	

	/*if( g_hUnicows )
	{
	FreeLibrary( g_hUnicows );
	g_hUnicows = NULL;
	}*/

	//CCheckHack::GetSingleton().Release();

	/*if( m_nProtectSys.IsGetUse() && m_nProtectSys.IsGameExit() )
	MessageBox( NULL, m_nProtectSys.GetHacking_Msg(),"GameGuard Error",MB_OK );*/

	return 0;
}

//-------------------------------------------------------------------------------------------------
