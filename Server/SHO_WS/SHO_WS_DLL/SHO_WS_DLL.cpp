/*
	**	��� Ŭ���̾�Ʈ�� ������ �����Ѵ�.
		1. Ŭ���̾�Ʈ�� ������ �ɸ��� ���� ó��
		1. �ɸ� ������ �� ������ �̵�.
		1. ���ê ó��
		1. �Ӹ� ó��
		1. ģ������

	**	��� �� ������ ������ ����
		1. ���� IP/PORT ����
*/
#include "stdafx.h"
#include "SHO_WS_DLL.h"
#include "SHO_WS_LIB.h"

#pragma	comment	(lib, "WS2_32.LIB")


#ifdef	_DEBUG
	#pragma comment (lib, "util_d.lib") 
	#pragma comment (lib, "d_LIB_Server.lib") 
#else
	#pragma comment (lib, "util_r.lib")
	#pragma comment (lib, "LIB_Server.lib") 
#endif


//---------------------------------------------------------------------------------------
static int s_iCalledCnt = 0;
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
			if ( s_iCalledCnt < 2 ) {
				Sleep( 300 );
				s_iCalledCnt ++;
			}

		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


SHO_WS *g_GSApp=NULL;

//---------------------------------------------------------------------------------------
DLL_WS_API bool __stdcall WS_Init (HINSTANCE hInstance, char *szBaseDataDIR, int iLangType, EXE_WS_API *pAPI)
{
	if ( NULL == g_GSApp ) {
		g_GSApp = g_GSApp->InitInstance( hInstance, szBaseDataDIR, pAPI, iLangType );
	}

	return true;
}

//---------------------------------------------------------------------------------------
DLL_WS_API void __stdcall WS_Free ()
{
	if ( g_GSApp )  {
		g_GSApp->Shutdown ();
		g_GSApp->Destroy ();
		g_GSApp = NULL;
	}
}


//---------------------------------------------------------------------------------------
DLL_WS_API bool __stdcall WS_ConnectDB (char *szServerIP, char *szDBName, char *szDBUser, char *szDBPw, char *szLogDBUser, char *szLogDBPw)
{
	#define	DB_USER			"icarus"
	#define	DB_PASSWORD		"1111"
	char *szUser, *szPw;

	szUser = ( szDBUser ) ? szDBUser : DB_USER;
	szPw   = ( szDBPw   ) ? szDBPw   : DB_PASSWORD;

	if ( !g_GSApp->ConnectDB(szServerIP, szDBName, szUser, szPw, szLogDBUser, szLogDBPw ) ) {
		return false;
	}

	return true;
}


//---------------------------------------------------------------------------------------
DLL_WS_API bool __stdcall WS_Start(HWND hWnd,
						  char *szWorldNAME,
						  char *szLoginServerIP, int iLoginServerPORT,
						  char *szLogServerIP,	 int iLogServerPORT,
						  int   iZoneListenPORT, int iUserListenPORT, bool bBlockCreateCHAR)
{
	return g_GSApp->Start( hWnd, 
				szLoginServerIP,	iLoginServerPORT,
				szLogServerIP,		iLogServerPORT,
				szWorldNAME,
				iZoneListenPORT, 
				iUserListenPORT, bBlockCreateCHAR);
}

DLL_WS_API void __stdcall WS_Active( bool bActive )
{
    g_GSApp->Active( bActive );
}

//---------------------------------------------------------------------------------------
DLL_WS_API void __stdcall WS_CloseAllUSER ()
{
}

//---------------------------------------------------------------------------------------
DLL_WS_API void __stdcall WS_Shutdown ()
{
	if ( g_GSApp )  {
		g_GSApp->Shutdown ();
	}
}

//---------------------------------------------------------------------------------------
DLL_WS_API void __stdcall WS_AnnounceChat( char *szMsg )
{
	if ( g_GSApp )
		g_GSApp->Send_ANNOUNCE( szMsg );
}


DLL_WS_API void __stdcall WS_StartCLI_SOCKET ()
{
	if ( g_GSApp )
		g_GSApp->StartCLI_SOCKET ();
}

DLL_WS_API void __stdcall WS_ShutdownCLI_SOCKET ()
{
	if ( g_GSApp )
		g_GSApp->ShutdownCLI_SOCKET ();
}

DLL_WS_API void __stdcall WS_ToggleChannelActive(int iChannelNo)
{
	if ( g_GSApp )
		g_GSApp->ToggleChannelActive( iChannelNo );
}

DLL_WS_API void __stdcall WS_DisconnectChannelServer(int iChannelNo)
{
	// ä�� ���� ������߿� ���⼭ ä�� ������ �����ϸ�.... �α����� �Ҽ� ������
	// �ɸ��ʹ� ������Ʈ ���� �ʴ´�...
	if ( g_GSApp )
		g_GSApp->DeleteChannelServer( iChannelNo );
}

//---------------------------------------------------------------------------------------
