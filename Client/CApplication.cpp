/*
	$Header: /Client/CApplication.cpp 82    05-10-13 11:32a Choo0219 $
*/

#include "stdAFX.h"
#include "Resource.h"
#include "CApplication.h"
//#include "CMouse.h"
//#include "CKeyboard.h"
#include "Network\CNetwork.h"
#include "Game.h"
#include "TGameCtrl.h"

#include "Interface\\it_MGR.h"
#include "System/CGame.h"
#include "interface/cursor/ccursor.h"
#include "Sound/MusicMgr.h"
#include "Sound/DirectMusicPlayer.h"
#include "util/classMD5.h"
#include "CClientStorage.h"

#include "TIme2.h"
CApplication* CApplication::m_pInstance = NULL;
int CApplication::m_nScrX;
int CApplication::m_nScrY;

//#define DEFAULT_WINDOWED_STYLE ( WS_OVERLAPPEDWINDOW )
//#define DEFAULT_WINDOWED_STYLE ( WS_OVERLAPPEDWINDOW | ~WS_MAXIMIZEBOX )

//---------------------------------------------------------------------------------------------------------
/// WS_OVERLAPPEDWINDOW - WS_MAXIMIZEBOX
//---------------------------------------------------------------------------------------------------------
#define DEFAULT_WINDOWED_STYLE ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX )

#define DEFAULT_FULLSCREEN_STYLE (/* WS_SYSMENU |*/ WS_VISIBLE | WS_POPUP /*| WS_MAXIMIZE */)

//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
LRESULT CALLBACK Window_Proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    return g_pCApp->MessageProc( hWnd, uMsg, wParam, lParam );
}


//-------------------------------------------------------------------------------------------------
int CApplication::wm_COMMAND (WPARAM wParam)
{
	int wmId, wmEvent;

	wmId    = LOWORD(wParam); 
	wmEvent = HIWORD(wParam); 
	// Parse the menu selections:
	switch (wmId)
	{
		case IDM_WIRE :
			//--------------------------[ engine related ]-----------------------//
			m_bViewWireMode = !m_bViewWireMode;
			useWireMode( m_bViewWireMode );
			break;
		case IDM_EXIT :
			DestroyWindow(this->m_hWND);
			break;
	}
	return 1;
}

//-------------------------------------------------------------------------------------------------
LRESULT CApplication::MessageProc ( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	if( CTIme::GetInstance().Process( hWnd, uiMsg, wParam, lParam ) )
		return S_OK;

	// Ű���� �޼��� ����
	if ( CGame::GetInstance().AddWndMsgQ(uiMsg, wParam, lParam) )
		return 0;
	
    switch( uiMsg ) 
	{
		case WM_SYSCHAR:///systemkey�� �Ϲ� Ű�� �����ؼ� ������ "��"�Ҹ� ���ֱ�
			return 0;
		case WM_SETCURSOR:						
			if( CCursor::GetInstance().RefreshCursor() )
				return S_OK;
			break;
		case WM_ACTIVATE :
			{
				m_wActive = ( ( LOWORD( wParam ) != WA_INACTIVE ) && ( HIWORD( wParam ) == 0 ) ); // INVACIVE �� �ƴϰ�, �̴ϸ���� �ƴ� ��쿡�� Ȱ��ȭ
				m_wActive = true; //Numenor: This enables constant rendering!!
				
				LogString( LOG_DEBUG, "WM_ACTIVATE: [%s]\n",
					( HIWORD(wParam) != 0) ? "[MINIMIZED]" :
						( LOWORD(wParam) == WA_ACTIVE ) ? "ACTIVATE" : 
							( LOWORD(wParam) == WA_CLICKACTIVE ) ? "CLICKACTIVE" : "DEACTIVE" );

				//-----------------------------------------------------------------------------------------
				///
				//-----------------------------------------------------------------------------------------
				CMusicMgr& musicMgr = CMusicMgr::GetSingleton();
				if( musicMgr.bIsReady() )
				{
					if ( m_wActive )
					{
						musicMgr.Run ();
					}
					else 
					{
						musicMgr.Pause ();
					}
					
				}

			}
			
			break;

		case WM_MUSIC_EVENT :
			{
				//-----------------------------------------------------------------------------------------
				///
				//-----------------------------------------------------------------------------------------
				CMusicMgr& musicMgr = CMusicMgr::GetSingleton();
				if( musicMgr.bIsReady() )
				{
					musicMgr.HandleEvent ();				
				}
			}

			break;

		case WM_CLOSE   :
			g_pCApp->SetExitGame();
			//CGame::GetInstance().ProcWndMsg( uiMsg, wParam, lParam );
			return 0;

		case WM_ERASEBKGND:
		case WM_SYSKEYUP   :					// ALTŰ �������� ���� ���� !!!
		case WM_PALETTECHANGED :
		case WM_QUERYNEWPALETTE :
			return 0;
		
		case WM_MOVE :
			if ( !IsFullScreenMode() ) {
				RECT sRECT;
				::GetClientRect ( hWnd, &sRECT );
				::ClientToScreen( hWnd, (POINT*)&sRECT );
				::ClientToScreen( hWnd, (POINT*)&sRECT+1 );
			}
			break;		
		case WM_COMMAND:
			this->wm_COMMAND (wParam);
			break;
	}
	return ::DefWindowProc(hWnd, uiMsg, wParam, lParam);
}


//-------------------------------------------------------------------------------------------------
CApplication::CApplication ()
{
	m_hWND = NULL; 

	m_bExitGame = false;

	m_bViewWireMode   = false;
	m_bFullScreenMode = true;

	m_nScrWidth = 0;
	m_nScrHeight = 0;
	m_nScrWidth_original = 0;
	m_nScrHeight_original = 0;

	m_nScrX = 0;
	m_nScrY = 0;

	m_wActive = false;

	CStr::Init ();
}
CApplication::~CApplication ()
{
	if ( m_hWND ) {
	    ::DestroyWindow( m_hWND );
	}

	CStr::Free ();
} 

//-------------------------------------------------------------------------------------------------
CApplication* CApplication::Instance ()
{
	if ( m_pInstance == NULL ) {
		m_pInstance = new CApplication;
	}

	return m_pInstance;
}
void CApplication::Destroy ()
{
	SAFE_DELETE( m_pInstance );
}

//-------------------------------------------------------------------------------------------------
void CApplication::ErrorBOX (char *szText, char *szCaption, UINT uType)
{
// #ifdef	_DEBUG???
	::MessageBox (m_hWND, szText, szCaption, uType);
//	_ASSERT( 0 );
}


//-------------------------------------------------------------------------------------------------
void  CApplication::SetCaption (char *szStr)
{
	m_Caption.Set( szStr );
	::SetWindowText(m_hWND, szStr);
}

//-------------------------------------------------------------------------------------------------
DWORD CApplication::DisplayFrameRate (void)
{
	static DWORD	l_dwFrameCount;
	static DWORD	l_dwFrameTime=0;
	static DWORD	l_dwFrames=0;

    DWORD       time2;

    l_dwFrameCount++;
	time2 = g_GameDATA.GetGameTime() - l_dwFrameTime;
    if( time2 >= 1000 ) {
        l_dwFrames     = (l_dwFrameCount*1000)/time2;
        l_dwFrameTime  = g_GameDATA.GetGameTime();
        l_dwFrameCount = 0;
    }

	if ( !m_bFullScreenMode ) {
		char *pStr = CStr::Printf ("[ %s ] FPS: %d", m_Caption.Get(), l_dwFrames);
		::SetWindowText(m_hWND, pStr);
	}

	return l_dwFrames;
}

//-------------------------------------------------------------------------------------------------
bool CApplication::ParseArgument (char *pStr)
{
	char *pToken;
	char *pDelimiters = " ,\t\n";

	bool bPassLuncher = false;
	bool bIDisSet = false;
	bool isMD5 = false;
#ifdef _DEBUG
	bPassLuncher = true;
#endif


	g_GameDATA.m_nServerID = 0;
	g_GameDATA.m_nSightRange = 13;

	g_GameDATA.m_bDirectLogin = false;
	g_GameDATA.m_bCheckDupRUN = true;

	pToken = CStr::GetTokenFirst (pStr, pDelimiters);
	while ( pToken ) 
	{
		if ( !strcmp( pToken, "@TRIGGER_SOFT@" ) ) 
		{
			bPassLuncher = true;
		}

		if ( !strcmp( pToken, "_WINDOW_MODE_") )
			m_bFullScreenMode = false;
		if ( !strcmp( pToken, "_800" ) ) {
			m_nScrWidth = 800;
			m_nScrHeight = 600;
		}

		if ( !strcmp( pToken, "_1024" ) ) {
			m_nScrWidth = 1024;
			m_nScrHeight = 768;
		}

		if ( !strcmp( pToken, "_1280" ) ) {
			m_nScrWidth = 1280;
			m_nScrHeight = 1024;
		}

		if ( !strcmp( pToken, "_1280P" ) ) {
			m_nScrWidth = 1280;
			m_nScrHeight = 720;
		}

		if ( !strcmp( pToken, "_width" ) ) {
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken ) {
				m_nScrWidth = atoi(pToken);
			}
		}

		if ( !strcmp( pToken, "_height" ) ) {
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken ) {
				m_nScrHeight = atoi(pToken);
			}
		}

		//Numenor: Add an option to give the initial position of the game window
		if ( !strcmpi( pToken, "_posX" ) ) {
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken ) {
				m_nScrX = atoi(pToken);
			}
		}

		if ( !strcmpi( pToken, "_posY" ) ) {
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken ) {
				m_nScrY = atoi(pToken);
			}
		}

		if ( !strcmpi( pToken, "_sight" ) ) {
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken ) {
				g_GameDATA.m_nSightRange = atoi( pToken );
			}
		}
		if ( !strcmpi( pToken, "_noui" ) ) { // �������̽� ���߱�. - zho
			g_GameDATA.m_bNoUI = true;
		}
		
		if ( !strcmpi( pToken, "_server" ) ) {
			// Get server ip...
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken ) {
				g_GameDATA.m_ServerIP.Set( pToken );
			}
		}

		if ( !strcmpi( pToken, "_port" ) ) {
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken ) {
				g_GameDATA.m_wServerPORT = atoi( pToken );
			}
		}

		if( !strcmpi( pToken, "_serverID" ) ) {
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken ) {
				g_GameDATA.m_nServerID = atoi( pToken );
			}
		}

		if( !strcmpi( pToken, "_direct" ) ) 
		{			
			g_GameDATA.m_bDirectLogin = true;			
		}

		if( !strcmpi( pToken,"_test" ) )
			g_GameDATA.m_bForOpenTestServer = true;

		if( !strcmpi( pToken, "_dup" ) )
			g_GameDATA.m_bCheckDupRUN = false;

		/// ���� �Ϻ� NHN JAPAN�� ���� Argument Setting( 2005/5/18 )
		if( !strcmpi( pToken,"_RCODE_JP_HG" ) )
			g_GameDATA.m_is_NHN_JAPAN = true;


		//Numenor: Writes your user name in the ID box. If _pw or _pwMD5 is given, it logs you automatically
		if ( !strcmpi( pToken, "_userid" ) ) 
		{
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken ) {
				g_GameDATA.m_Account.Set( pToken );
				bIDisSet = true;
				if(isMD5) g_GameDATA.m_is_NHN_JAPAN = true; //Numenor: all those conditions allow the options to be set in any order.
			}
		}

		//Numenor: Writes your password in the password box. If _userid is given, it logs you automatically
		if ( !strcmpi( pToken, "_pw" ) ) 
		{
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken )
			{
				//GetMD5 ( g_GameDATA.m_PasswordMD5, (unsigned char*)pToken, strlen(pToken) );
				g_GameDATA.m_Password.Set( pToken );
				//CopyMemory( g_GameDATA.m_PasswordMD5, pToken, strlen( pToken ) );
				//Numenor: if _pw is set, force it to use encryption
				g_GameDATA.m_is_NHN_JAPAN = false;
				isMD5 = false;
			}
		}

		//Numenor: if _userid find an argument, you can use _pdMD5 to give your password in md5 to log automatically..
		//Numenor: if _pw is already set or if _userid is not set, this option won't be used.
		if ( !strcmpi( pToken, "_pwMD5" ) && !g_GameDATA.m_Password.Get() )
		{
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken )
			{
				isMD5 = true;
				//GetMD5 ( g_GameDATA.m_PasswordMD5, (unsigned char*)pToken, strlen(pToken) ); //transform token into md5
				if(bIDisSet) g_GameDATA.m_is_NHN_JAPAN = true; //Numenor: if NHN_JAPAN is true, then sends passwords already in md5 format
				CopyMemory( g_GameDATA.m_PasswordMD5, pToken, strlen( pToken ) );
			}
		}

		//Numenor: Gives the name of your char to auto-log it
		if ( !strcmpi( pToken, "_charname" ) ) 
		{
			pToken = CStr::GetTokenNext (pDelimiters);
			if ( pToken ) {
				g_GameDATA.m_CharName.Set(pToken);
			}
		}

		//----------------------------------------------------------/

//#ifdef	_DEBUG
//		if ( !strcmpi( pToken, "_id" ) ) {
//			pToken = CStr::GetTokenNext (pDelimiters);
//			if ( pToken ) {
//				g_GameDATA.m_Account.Set( pToken );
//			}
//		}
//
//		if ( !strcmpi( pToken, "_pw" ) ) {
//			pToken = CStr::GetTokenNext (pDelimiters);
//			if ( pToken ) {
//				g_GameDATA.m_Password.Set( pToken );
//			}
//		}
//
//#endif

		pToken = CStr::GetTokenNext (pDelimiters);
	}


	if( !bPassLuncher )
		return false;

	return true;
}

//-----------------------------------------------------------------------------------------------------------------
/// 1. resetScreen()�� MoveWindow���� ���� ȣ��Ǿ�� client Window�� Size������ ����� �ȴ�.
/// resetScreen�� MoveWindow���� ���߿� �ɰ�� MoveWindow���� Size������  Window�� ����ɼ� �ִ� �ִ�����
/// ���� ���ϴ� ������ �۰� �Ǿ� Size ���濡 �����Ѵ�( navy : 2005/3/11 )
/// 2. ���� ���������� �ػ󵵺��� ����ũ�� �̻����� �����츦 �����ϰų� ������ ������ ���� �ʴ´�.
///		- ���� �������� �ػ󵵸� ���ؼ� �����ϰ����ϴ� ũ�⸦ ������
//-----------------------------------------------------------------------------------------------------------------
void CApplication::ResizeWindowByClientSize(int& iClientWidth, int& iClientHeight, int iDepth , bool update_engine, int iClientX, int iClientY)
{

	if (m_bFullScreenMode) 
	{
		iClientWidth = GetSystemMetrics (SM_CXSCREEN);
		iClientHeight = GetSystemMetrics (SM_CYSCREEN);
		if( update_engine )
		{
			setScreen ( iClientWidth, iClientHeight, iDepth, g_pCApp->IsFullScreenMode() ); // getFullScreen() );
			setBuffer ( iClientWidth, iClientHeight, iDepth );
			resetScreen();
		}

		MoveWindow( m_hWND, 0,0, GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN), FALSE );
		SetWIDTH( iClientWidth );
		SetHEIGHT( iClientHeight );
			
		t_OptionVideo option;
		g_ClientStorage.GetVideoOption( option );
		option.tResolution.iWidth = iClientWidth;
		option.tResolution.iHeight = iClientHeight;
		g_ClientStorage.SetVideoOption( option );
	}
	else
	{
		_RPT2( _CRT_WARN,"want rect %d %d\n", iClientWidth, iClientHeight );
		_RPT2( _CRT_WARN,"max rect %d %d\n", GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN) );

		if( iClientWidth > GetSystemMetrics (SM_CXSCREEN) )
			iClientWidth = GetSystemMetrics (SM_CXSCREEN);

		if( iClientHeight > GetSystemMetrics (SM_CYSCREEN) )
			iClientHeight = GetSystemMetrics (SM_CYSCREEN);

		RECT client_rect = { 0, 0, iClientWidth, iClientHeight };
		if( AdjustWindowRect( &client_rect, DEFAULT_WINDOWED_STYLE, FALSE ) )
		{
			int window_width = client_rect.right - client_rect.left;
			int window_height = client_rect.bottom - client_rect.top;

			_RPT2( _CRT_WARN,"window rect %d %d\n", window_width, window_height );
			_RPT2( _CRT_WARN,"client rect %d %d\n", iClientWidth, iClientHeight );

			if( update_engine )
			{
				setScreen ( iClientWidth, iClientHeight, iDepth, g_pCApp->IsFullScreenMode() ); // getFullScreen() );
				setBuffer ( iClientWidth, iClientHeight, iDepth );
				resetScreen();
			}

			MoveWindow( m_hWND, iClientX, iClientY, window_width, window_height, TRUE ); //Numenor: I've changed this function to be able to create windows at different positions
			
			SetWIDTH( iClientWidth );
			SetHEIGHT( iClientHeight );
		}
		else
		{
			assert( 0 && "AdjustWindowRect is Failed" );
		}
	}

	ShowWindow( m_hWND, SW_SHOW);
	UpdateWindow( m_hWND );


//	// ������ ����� ���� �ػ󵵿� �µ��� ������
//    int iScreenWidth  = GetSystemMetrics (SM_CXSCREEN);
//    int iScreenHeight = GetSystemMetrics (SM_CYSCREEN);
//
//	if( iScreenWidth < iClientWidth )
//	{
//		iClientWidth = iScreenWidth;
//		SetWIDTH( iClientWidth );
//	}
//
//	if( iScreenHeight < iClientHeight )
//	{
//		iClientHeight = iScreenHeight;
//		SetHEIGHT( iClientHeight );
//	}
//
//	RECT rRect;
//	::SetWindowPos( m_hWND, HWND_TOP, 0, 0, iClientWidth, iClientHeight, (SWP_FRAMECHANGED) );
//
//	GetClientRect( m_hWND, &rRect ); // rRect.left = rRect.top = 0
//	if (( (rRect.right - rRect.left) != iClientWidth) || ( (rRect.bottom-rRect.top) != iClientHeight))
//	{
//		int cx, cy;
//		cx = iClientWidth + iClientWidth - ( rRect.right - rRect.left );
//		cy = iClientHeight + iClientHeight - ( rRect.bottom - rRect.top );
//
//		::SetWindowPos( m_hWND, HWND_TOP, 0, 0, cx, cy, 0 );
//
//		GetClientRect( m_hWND, &rRect ); // rRect.left = rRect.top = 0
//
//		SetWIDTH( rRect.right - rRect.left );
//		SetHEIGHT( rRect.bottom - rRect.top );
//
//		/*SetWIDTH( cx );
//		SetHEIGHT( cy );*/
////
////#ifdef _DEBUG
////		::GetClientRect( m_hWND, &rRect );
////		_RPT2( _CRT_WARN,"right: %d, left: %d\n", rRect.right, rRect.left );
////		_RPT2( _CRT_WARN,"top  : %d, bottom:%d\n", rRect.top, rRect,bottom );
////		_RPT2( _CRT_WARN,"Width: %d, Height:%d\n", iClientWidth, iClientHeight );
////		assert( (rRect.right - rRect.left) == iClientWidth );
////		assert( (rRect.bottom - rRect.top) == iClientHeight );
////#endif
//	}


}

//-------------------------------------------------------------------------------------------------
bool CApplication::CreateWND(char *szClassName, char *szWindowName, short nWidth, short nHeight,int iDepth, HINSTANCE hInstance, int iClientX, int iClientY)
{
	//Numenor: by default : int iClientX = m_nScrX, int iClientY = m_nScrY (see declaration of CreateWND)

/*    if ( ::FindWindow (szClassName, szWindowName) ) {
        return false;
	}*/

	//Numenor: if resolution option passed by launcher, write the resolution found in the .ini file into "original", so we can use it to check if the resolution has changed or not compared to the .ini file.
	if(!m_nScrWidth)	m_nScrWidth  = nWidth;	//Numenor: if no option passed, then use .ini file instead of launcher option.
	else m_nScrWidth_original = nWidth;
	if(!m_nScrHeight)	m_nScrHeight = nHeight;
	else m_nScrHeight_original = nHeight;

    WNDCLASSEX  wcex;

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = (WNDPROC)Window_Proc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
    wcex.hIcon          = LoadIcon(hInstance, (LPCTSTR)IDI_CLIENT);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName   = ( m_bFullScreenMode ) ? NULL : (LPCTSTR)IDC_CLIENT;
    wcex.lpszClassName  = szClassName;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

    RegisterClassEx(&wcex);

	m_Caption.Set( szWindowName );

    if ( !m_bFullScreenMode ) {
        m_hWND = ::CreateWindowEx (
            0,                      // extended window style
            szClassName,			// pointer to registered class name
            szWindowName,			// pointer to window name
            DEFAULT_WINDOWED_STYLE,	// window style
            iClientX,					// (GetSystemMetrics (SM_CXSCREEN)-START_SCR_XLEN)/2,     // horizontal position of window
            iClientY,						//(GetSystemMetrics (SM_CYSCREEN)-START_SCR_YLEN)/2,     // vertical position of window
            m_nScrWidth,					// window width
            m_nScrHeight,				// window height
            NULL,                   // handle to parent or owner window
            NULL,                   // handle to menu, or child-window identifier
            hInstance,              // handdle to application instance
            NULL );                 // pointer to window-creation data


			int client_width  = m_nScrWidth;
			int client_height = m_nScrHeight;
			ResizeWindowByClientSize(client_width, client_height, iDepth , false, iClientX, iClientY );


	} else {
        m_hWND = ::CreateWindowEx (
            WS_EX_TOPMOST,          // extended window style
            szClassName,			// pointer to registered class name
            szWindowName,			// pointer to window name
            DEFAULT_FULLSCREEN_STYLE ,  // window style
            0,                      // horizontal position of window
            0,                      // vertical position of window
            GetSystemMetrics (SM_CXSCREEN), // window width
            GetSystemMetrics (SM_CYSCREEN), // window height
            NULL,					// handle to parent or owner window
            NULL,                   // handle to menu, or child-window identifier
            hInstance,              // handdle to application instance
            NULL );                 // pointer to window-creation data

	}

	if ( m_hWND ) {
		::ShowWindow   (m_hWND, SW_SHOWNORMAL);
		::UpdateWindow (m_hWND);
		::SetFocus     (m_hWND);
	}


	m_hINS = hInstance;

	/*HRSRC hRes=FindResource( hInstance, MAKEINTRESOURCE(IDR_ANIMOUSE1),"ANIMOUSE");
	DWORD dwSize = SizeofResource( hInstance, hRes );
	HGLOBAL hGlob=LoadResource( hInstance, hRes );
	LPBYTE pBytes=(LPBYTE)LockResource( hGlob ); 

	g_GameDATA.m_hCursor[ CURSOR_DEFAULT ] = (HCURSOR)CreateIconFromResource( pBytes, dwSize, FALSE, 0x00030000);	*/

	//g_GameDATA.m_hCursor = LoadCursor( NULL, IDC_WAIT );

//	g_pCMouse->LoadUserCursor( hInstance );	

//	g_pCMouse->SetUserCursor( CURSOR_DEFAULT );	
		//g_itMGR.InitInterfacePos();

	//Numenor: Warn the world that your selution has changed
	if ( !m_bFullScreenMode ) {
		t_OptionVideo option;
		g_ClientStorage.GetVideoOption( option );
		option.tResolution.iWidth = m_nScrWidth;
		option.tResolution.iHeight = m_nScrHeight;
		g_ClientStorage.SetVideoOption( option );
	}
	return ( m_hWND != NULL );
}
//-------------------------------------------------------------------------------------------------
void CApplication::DestroyWND ()
{
	if ( m_hWND ) {
	    ::DestroyWindow( m_hWND );
	}
	m_hWND = NULL;
}

//-------------------------------------------------------------------------------------------------
bool CApplication::GetMessage (void)
{
	while ( ::PeekMessage (&m_Message, NULL, 0, 0, PM_REMOVE) )
    {
		if ( m_Message.message == WM_QUIT ) {
			SetExitGame();
			return false;
		}

		::TranslateMessage (&m_Message);
		::DispatchMessage  (&m_Message);
    }

    if ( this->IsActive() ) {
		//
        // Game processing ...
		//
		#pragma message ("TODO:: restore surface ..." __FILE__)

        return false;
    }

    return true;
}

void CApplication::SetExitGame()
{	
	m_bExitGame = true;			
}

void CApplication::ResetExitGame()
{	
	m_bExitGame = false;		
}

void CApplication::SetFullscreenMode (bool bFullScreenMode)
{
	if (m_bFullScreenMode == bFullScreenMode) return; // already fullscreen mode

    if( bFullScreenMode )
    {
		SetWindowLongPtr( m_hWND, GWL_STYLE, DEFAULT_FULLSCREEN_STYLE );
		::SetWindowPos( m_hWND, HWND_TOP, 0, 0, 0 ,0 ,SWP_NOMOVE | SWP_NOSIZE /*| SWP_NOZORDER*/ | SWP_FRAMECHANGED );

    }
	else 
	{
        SetWindowLongPtr( m_hWND, GWL_STYLE, DEFAULT_WINDOWED_STYLE );
		::SetWindowPos( m_hWND, HWND_NOTOPMOST, 0, 0, 0 ,0 ,SWP_NOMOVE | SWP_NOSIZE /*| SWP_NOZORDER*/ | SWP_FRAMECHANGED );
	}
	m_bFullScreenMode = bFullScreenMode;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
