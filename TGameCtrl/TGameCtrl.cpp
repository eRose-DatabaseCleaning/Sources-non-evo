// TGameCtrl.cpp : DLL ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "TGameCtrl.h"
#include "TControlMgr.h"
#include "ResourceMgr.h"
#include "TIme2.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

TGAMECTRL_API void it_SetKeyboardInputType( short nInputType )
{
	CTControlMgr::GetInstance()->SetKeyboardInputType( nInputType );
}

TGAMECTRL_API short it_GetKeyboardInputType()
{
	return CTControlMgr::GetInstance()->GetKeyboardInputType();
}
TGAMECTRL_API bool it_Init( HWND hWnd ,ITDraw* pDrawMgr ,ITSound* pSoundMgr , ITFont* pFontMgr )
{
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		//DialogBox(NULL,"Fail Initialize UI Library","Error:Init",MB_OK );
		return false;

	}
	CTControlMgr::GetInstance()->SetDrawMgr( pDrawMgr );
	CTControlMgr::GetInstance()->SetWindowHandle( hWnd );
	CTControlMgr::GetInstance()->SetSoundMgr( pSoundMgr );
	CTControlMgr::GetInstance()->SetFontMgr( pFontMgr );
//	g_CIME.InitIme( hWnd );
//	AllocConsole();
	return true;
}



TGAMECTRL_API  void it_Destroy()
{
//	FreeConsole();
	CoUninitialize();
	CTControlMgr::GetInstance()->Destroy();
	CResourceMgr::GetInstance()->Destroy();

}


TGAMECTRL_API void it_SetCodePage( unsigned uiCodePage )
{
	CTControlMgr::GetInstance()->SetCodePage( uiCodePage );
}
/*
void	TWriteConsole(const char *fmt,...)
{
	char buff[1024] = {0,};
	DWORD nChars;

	va_list vaArglist;

	va_start(vaArglist,fmt); 
	int count = vsprintf(buff,fmt,vaArglist);

	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), 
					(void *)buff , 
					count, 
					&nChars, 
					NULL );

	/* �׳� �̷��� ���ָ� �˾Ƽ� �߰ڰŴ� �ϴ�... �� ������ ����  /(^0^)/ */

	/* ���� ������ ��.��  */
	 
//	va_end(vaArglist);
//}
TGAMECTRL_API bool	it_IsVisibleImeCandidateWindow()
{
	return CTIme::GetInstance().IsVisibleCandidateWindow();
}