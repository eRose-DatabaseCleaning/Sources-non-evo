#include "stdafx.h"
#include ".\checkhack.h"
//#include "TRose.h"
/*
TCheck_Init g_fpTRInit	= NULL;	
TCheck_Free g_fpTRFree	= NULL;
HMODULE		g_hChkLIB	= NULL;

/// for singleton
CCheckHack checkHack;

bool __stdcall CallBackFindHack ( char *szFileName )
{
	// return true�� ��ŷ�� ���μ��� ���� ����..
	if ( IDOK == ::MessageBox( NULL, "���ÿ� �����Ҽ� ���� ���μ����� �������Դϴ�.", szFileName, MB_OK ) )
		return true;
	

	// ��ŷ�� ���� �ȵ� :: ���� ���μ����� ����..
	return false;
}


CCheckHack::CCheckHack(void)
{
	m_bCanStartAPP = false;	
}

CCheckHack::~CCheckHack(void)
{		
}

bool CCheckHack::CanStartApp()
{
	return m_bCanStartAPP;
}

bool CCheckHack::Check()
{
	DWORD dwVersion = ::GetVersion();
	if ( dwVersion < 0x80000000 ) 
	{
 		g_hChkLIB = ::LoadLibrary( "TCheck.dll" );
 		if ( g_hChkLIB ) 
		{
 			g_fpTRInit = (TCheck_Init)::GetProcAddress( g_hChkLIB, "TRose_Init" );
 			g_fpTRFree = (TCheck_Free)::GetProcAddress( g_hChkLIB, "TRose_Free" );
 			if ( g_fpTRInit && g_fpTRFree )
			{
 				g_fpTRInit ( 5, CallBackFindHack );
			}
 			else 
			{
 				::MessageBox( NULL, "invalid TCheck.dll", "...", MB_OK );
 				return false;	// ���� ����
 			}
 		} else 
		{
 			::MessageBox( NULL, "TCheck.dll not found", "...", MB_OK );
 			return false;		// ���� ����
 		}
 	} else 
	{
 		// NT platform �� �ƴϸ� ����
 		///::MessageBox( NULL, "not active on noe NT platfrom..", "...", MB_OK );
		return true;
 	}

    return true;
}

void CCheckHack::Release()
{
	if ( g_fpTRFree )
 		g_fpTRFree ();
 	if ( g_hChkLIB )
 		::FreeLibrary( g_hChkLIB );
}
*/