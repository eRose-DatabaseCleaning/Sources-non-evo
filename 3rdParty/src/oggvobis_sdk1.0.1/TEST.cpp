// TEST.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "TEST.h"
#include "CDSound.h"
#include "ADSStream.h"

#define MAX_LOADSTRING 100

// ���� �����Դϴ�.
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

HWND g_HWND = NULL;
AOggSTREAM *g_pOGG;
CDSOUND g_SOUND;


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TEST);


	g_SOUND._Init ( g_HWND, DSSCL_PRIORITY, 2, 22050, 16);

	g_pOGG = new AOggSTREAM;
	g_pOGG->OpenSoundStreamFile( "OGG\\Prologue.ogg", g_SOUND.m_pDS, 15, 16, false );
	g_pOGG->PlayStreamBuffer( true );

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	delete g_pOGG;

	g_SOUND._Free ();

	return (int) msg.wParam;
}



//
// �Լ�: MyRegisterClass()
//
// ����: â Ŭ������ ����մϴ�.
//
// ����:
//
// Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
// �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
// �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
// �ش� ���� ���α׷��� �����
// '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TEST);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_TEST;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
// �Լ�: InitInstance(HANDLE, int)
//
// ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
// ����:
//
// �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
// �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   g_HWND = hWnd;

   return TRUE;
}

//
// �Լ�: WndProc(HWND, unsigned, WORD, LONG)
//
// ����: �� â�� �޽����� ó���մϴ�.
//
// WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
// WM_PAINT	- �� â�� �׸��ϴ�.
// WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static char *szMusic[] = {
		"OGG\\Prologue.ogg",
		"OGG\\Town01_junon.ogg",
		"OGG\\Dungeon_junon.ogg",
		"OGG\\Canyon01_junon.ogg",
		"OGG\\Event01_junon.ogg",
		"OGG\\Plain01_junon.ogg",
		"OGG\\Town01_lunar.ogg",
		"OGG\\Town02_junon.ogg",
		NULL,
	} ;
	static int iMusicIDX=0;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;


	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// �޴��� ���� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			if ( g_pOGG ) 
				delete g_pOGG;

			iMusicIDX ++;
			if ( !szMusic[ iMusicIDX ] ) iMusicIDX = 0;
			g_pOGG = new AOggSTREAM;
			g_pOGG->OpenSoundStreamFile( szMusic[ iMusicIDX ], g_SOUND.m_pDS, 15, 16, false );
			g_pOGG->PlayStreamBuffer( true );
			// DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
