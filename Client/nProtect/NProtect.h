/******************************************************************************
  class	CNProtectSys 

  desc:  nProtect ó���ϴ� Ŭ���� 

  Date:  2005. 05. 19�� 

  �ۼ���:  �� ��ȣ.

  ��������:  GameGuard.des, RoseOnlinePH.ini ���ϵ��� ���� �������ϰ� 
             ���� ���丮�� �����Ѵ�. 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
#ifndef		__NPROTECT_SYS__
#define		__NPROTECT_SYS__

#define		MAX_BUF_LEN			200


//ȫ�� : ������ ���Ӱ��� ���� ��Ű�� �ʴ´�.
#ifdef _DEBUG

#define	NO_GAMEGUARD

#endif



//nProtect ���̺귯�� ���� 
#include "NPGameLib.h"
#include "npkcrypt.h"

#pragma comment(lib,"../nProtect/NPGameLib.lib")
//#pragma comment(lib,"../nProtect/npkcrypt.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Winmm.lib")









class CNProtectSys
{

	BOOL	m_IsGameExit;				//��w������ ���� 
	BOOL	m_IsCreate;					//protect �������� 
	BOOL	m_IsUse;					//protect �������   

	DWORD	m_curTick,m_oldTick;		//�˻� tick ī���� 
	DWORD	m_delayTick;
	
	GG_AUTH_DATA	m_AuthData;			//���� ����ü 

public:
	TCHAR	m_HackString[MAX_BUF_LEN];	//��ŷ �޼��� ���� 


public:
	CNProtectSys();
	~CNProtectSys();

	void  Release(void);
	void  Permission_Country(void);
	BOOL  InitProtect(void);
	

	void  Set_hWnd(HWND hWnd);
	void  Set_UserID(const char* userID);

	void  Check_nProtectSafe(DWORD ckTick=0);
	void  Auth_FromServer(WORD nType,GG_AUTH_DATA* pAuthData=NULL);
	bool  _NPGameMonCallback(DWORD dwMsg,DWORD dwArg);	

	
	GG_AUTH_DATA GetAuthData(void)	{  return m_AuthData;		}
	BOOL& IsCreate(void)			{  return m_IsCreate;		}
	BOOL& IsGetUse(void)			{  return m_IsUse;			}
	BOOL  IsGameExit(void)			{  return m_IsGameExit;	}

	void  SetDelayTick(DWORD t)		{  m_delayTick = t;			}
	TCHAR* GetHacking_Msg(void)		{  return m_HackString;	}
 

};
 



bool CALLBACK NPGameMonCallback(DWORD dwMsg,DWORD dwArg);		//nProtec�ȿ��� ����ϴ� ������ �Լ� 
void GetWindowsErrorMsg(DWORD dwError);							



extern CNProtectSys		m_nProtectSys;		
extern CNPGameLib		npgl;				




//
//
///********************************************************************************
//
// Class CNProtectKC   : Ű���� ��ȣȭ�� ó���ϴ� Ŭ���� 
//
// ��������:   npkcrypt, npkcrypt.dll, npkcrypt.vxd, npkcusb, npkpdb.dll
//             ���ϵ��� ���� �������ϰ� ���� ���丮�� �����Ѵ�. 
//********************************************************************************/
//class CNProtectKC
//{
//
//	BOOL m_IsCreate;
//	BOOL m_IsUse;
//
//	HKCRYPT	m_hKCrypt;				//Űũ��Ʈ �ڵ�  
//	APPCOMPATFLAG	m_apl;			//ȣȯ�� �÷��� 
//
//public:
//	
//
//
//
//public:
//	CNProtectKC();
//	~CNProtectKC();
//	
//	void Release(void);
//	BOOL InitKeyCrypt(void);
//	void CloseHandle(void);
//	BOOL ChekAppCompatFlag(char* szGameFileName);
//
//	
//	BOOL& IsCreate(void)			{  return m_IsCreate;		}
//	BOOL& IsGetUse(void)			{  return m_IsUse;			}
//
//};
//
//
//
//
//extern CNProtectKC	m_npKeyCrypt;
//


#endif