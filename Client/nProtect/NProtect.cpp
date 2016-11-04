/******************************************************************************
  class	CNProtectSys 

  desc:  nProtect ó���ϴ� Ŭ���� 

  Date:  2005. 05. 19�� 

  �ۼ���:  �� ��ȣ.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
#include "stdafx.h"
#include "nProtect.h"
#include "../Country.h"
#include "../Network/CNetwork.h"
 

CNProtectSys	m_nProtectSys;


#if defined(_EU)
	CNPGameLib npgl("RoseOnlineUK"); //nProtect ���� ���� 
#else
	CNPGameLib npgl("RoseOnlinePH"); 
#endif 
        
    

/********************************************************************************
    Func: NPGameMonCallback()

	Desc: nProtect ���� ����ϴ� ������ �Լ�  
********************************************************************************/
bool CALLBACK NPGameMonCallback(DWORD dwMsg,DWORD dwArg)
{
	
	return m_nProtectSys._NPGameMonCallback(dwMsg,dwArg);

}


/********************************************************************************
    Func: GetWindowsErrorMsg()

	Desc: DWORD �� �����ڵ带 ���ڿ��� ������ �޼��� â�� �����Ѵ�. 
********************************************************************************/
void GetWindowsErrorMsg(DWORD dwError)
{

	LPVOID lpMsg = NULL;


	if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
					  NULL,dwError,MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT),(LPTSTR)lpMsg,0,
					  NULL))
		return;


	MessageBox(NULL,(LPTSTR)lpMsg,"System Error",MB_OK);

}





/********************************************************************************
    Func: CNProtectSys()

	Desc: ������ 
********************************************************************************/
CNProtectSys::CNProtectSys()
{
	
	m_IsGameExit	= FALSE;
	m_IsCreate		= FALSE;
	m_IsUse			= TRUE;

	m_curTick	= GetTickCount();
	m_oldTick	= 0;
	m_delayTick = 7000;

	ZeroMemory(m_HackString,sizeof(TCHAR)*MAX_BUF_LEN);

}


/********************************************************************************
    Func: ~CNProtectSys()

	Desc: �Ҹ��� 
********************************************************************************/
CNProtectSys::~CNProtectSys()
{
	
	Release();

}


/********************************************************************************
    Func: Release()

	Desc: �޸� ����
********************************************************************************/
void CNProtectSys::Release(void)
{

	ZeroMemory(m_HackString,sizeof(TCHAR)*MAX_BUF_LEN);

	m_IsCreate = FALSE;

}


/********************************************************************************
    Func: Permission_Country()

	Desc: ��� ������ �������� üũ �Ѵ�.  

	����( 2005/5/20) �ʸ��ɿ����� �����ϵ��� ����
********************************************************************************/
void CNProtectSys::Permission_Country(void)
{

	//����  üũ �� ����� �����Ѵ�. 
	if(CCountry::GetSingleton().IsPhilipplines() ||
		CCountry::GetSingleton().IsEU())
		m_IsUse = TRUE;
	else
		m_IsUse = FALSE;
}



/********************************************************************************
    Func: InitProtect()

	Desc: nProtect  �ʱ�ȭ 
********************************************************************************/
BOOL CNProtectSys::InitProtect(void)
{

	//����� �������� üũ�Ѵ�. 
	Permission_Country();


	if(!m_IsUse)
		return TRUE;

	if(m_IsCreate)
		Release();

    //nProtect �ʱ�ȭ 
	DWORD	dwResult		= npgl.Init();
	LPCSTR	lpszMsg			= NULL;
	TCHAR	rtitleMsg[256]	= { 0 };
	

	//�����̶��...
	if(dwResult == NPGAMEMON_SUCCESS)
	{
		m_IsCreate = TRUE;
		return m_IsCreate;
	}


////////////////////////////////////////////////////////////////////////////////
	//���� �޼��� ó��  :
	// !! ���ڿ� ������ ������ ������!! â���� ������ �����ϴ� !!
////////////////////////////////////////////////////////////////////////////////
	switch(dwResult)
	{
		case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
		case NPGAMEMON_ERROR_NFOUND_GG:
		case NPGAMEMON_ERROR_AUTH_INI:
		case NPGAMEMON_ERROR_NFOUND_INI:
			{
				lpszMsg ="GameGuard file does not exist or corrupted.\nPlease try installing GameGuard setup file.";
				break;
			}

		case NPGAMEMON_ERROR_CRYPTOAPI:
			{
				lpszMsg ="Some system files of Windows are corrupted.\nPlease try reinstalling Internet Explorer(IE).";
				break;
			}

		case NPGAMEMON_ERROR_EXECUTE:
			{
				lpszMsg = "Fail to run GameGuard. Please try installing GameGuard setup file again.";
				break;
			}


		case NPGAMEMON_ERROR_ILLEGAL_PRG:
			{
				lpszMsg = "Prohibited program was found.\nPlease end unnecessary programs and try again.";
				break;
			}

		case NPGMUP_ERROR_ABORT:
			{
				lpszMsg = "Fail to connect GameGuard update server.\nPlease try again  after a while, or check the settings of internet or persona firewall.";
				break;
			}
		
		case NPGMUP_ERROR_CONNECT:
		case NPGMUP_ERROR_DOWNCFG:
			{		
				lpszMsg =  "Fail to complete GameGuard updating.\nCheck virus first or adjust settings of PC management if any.";
				break;
			}

		case NPGMUP_ERROR_AUTH:
			{	
				lpszMsg =  "Fail to complete GameGuard updating.\nCheck virus first, or adjust settings of PC management if any.";
				break;
			}

		default:
			{
				lpszMsg = "GameGuard execution error occurred.\nPlease send all files  with extension of '.erl' in GameGuard\nfolder located in the game's  folder to game@inca.co.kr.";
				break;
			}
			
	}
////////////////////////////////////////////////////////////////////////////////


	//���� �޼��� �ڽ� ��� 
	wsprintf(rtitleMsg,"GameGuard execution error: %lu",dwResult);
	MessageBox(NULL,lpszMsg,rtitleMsg,MB_OK);

	return m_IsCreate;

}


/********************************************************************************
    Func: Set_hWnd()

	Desc: nProtect�� �ڵ��� �����Ѵ�. 
********************************************************************************/
void CNProtectSys::Set_hWnd(HWND hWnd)
{

	if(m_IsUse)
		npgl.SetHwnd(hWnd);
		
}


/********************************************************************************
    Func: Set_UserID()

	Desc: nProtect�� ���� ID�� �����Ѵ�. . 
********************************************************************************/
void CNProtectSys::Set_UserID(const char* userID)
{

	if(m_IsUse && (userID != NULL))
		npgl.Send(userID);
	
}


/********************************************************************************
    Func: _NPGameMonCallback(DWORD dwMsg,DWORD dwArg)

	Desc: nProtect ������ �޼��� ó�� �Լ� 
********************************************************************************/
bool CNProtectSys::_NPGameMonCallback(DWORD dwMsg,DWORD dwArg)
{

	if(!m_IsUse)
		return TRUE;


	//���ڿ� �ʱ�ȭ 
	ZeroMemory(m_HackString,sizeof(TCHAR)*MAX_BUF_LEN);


	switch(dwMsg)
	{
		case NPGAMEMON_COMM_ERROR:
		case NPGAMEMON_COMM_CLOSE:
			{	m_IsGameExit = TRUE;	return false;		}

		case NPGAMEMON_INIT_ERROR:
			{
				wsprintf(m_HackString,"GameGuard Initialize Error: %lu",dwArg);
				m_IsGameExit = TRUE; 
				break;
			}

		case NPGAMEMON_SPEEDHACK:
			{
				wsprintf(m_HackString,"GameGuard Initialize Error: %lu",dwArg);
				m_IsGameExit = TRUE; 
				break;
			}

		case NPGAMEMON_GAMEHACK_KILLED:
			{
				wsprintf(m_HackString,"Hacking Tool is Found!: %lu",dwArg);
				m_IsGameExit = TRUE; 
				break;
			}
		
		case NPGAMEMON_GAMEHACK_DETECT:
			{
				wsprintf(m_HackString,"Hacking Tool is Found!: %lu",dwArg);
				m_IsGameExit = TRUE; 
				break;
			}
	
		case NPGAMEMON_GAMEHACK_DOUBT:
			{
				wsprintf(m_HackString,"Game or GameGuard was Altered.: %lu",dwArg);
				m_IsGameExit = TRUE; 
				break;
			}

		case NPGAMEMON_CHECK_CSAUTH2:
			{
				memcpy(&m_AuthData,(PVOID)dwArg,sizeof(GG_AUTH_DATA));
				g_pNet->Send_AuthMsg();
				m_IsGameExit = FALSE;
				break;
			}

	}
	

	//������ ���� �Ǿ�� �Ѵٸ�  nProtect�� FALSE�� �ѱ��. 
	if(m_IsGameExit)
		return FALSE;


	return true;

}


/********************************************************************************
    Func: Check_nProtectSafe();

	Desc: ���� Tick ���� nProtect�� ��ȿ���� �˻��Ѵ�. 
********************************************************************************/
void CNProtectSys::	Check_nProtectSafe(DWORD ckTick)
{   

	if(!m_IsUse)
		return;

	//���ڷ� tick�� �޾Ҵٸ� ���� 
	if(ckTick != 0)
		m_delayTick = ckTick;

	m_curTick = GetTickCount();

	DWORD tempTick = (m_curTick - m_oldTick);


	//���� ������ƽ�� �Ǿ��ٸ� �˻� 
	if(tempTick >= m_delayTick)
	{
		npgl.Check();
		m_oldTick = m_curTick;
	}

}


/********************************************************************************
    Func: Auth_FromServer();

	Desc: ���Ӽ����� ���� nProtect ������ �޴´�. 
********************************************************************************/
void  CNProtectSys::Auth_FromServer(WORD nType,GG_AUTH_DATA* pAuthData)
{

	if(!m_IsUse)
		return;

	//AUTH_MODULE_nPROTECT == 241
	if((nType != 241) || (pAuthData == NULL))
		return;
	
	npgl.Auth2(pAuthData);
	
}




//
///********************************************************************************
//
// Class CNProtectKC   : Ű���� ��ȣȭ�� ó���ϴ� Ŭ���� 
//
//********************************************************************************/
//
//
//CNProtectKC	m_npKeyCrypt;
///********************************************************************************
//    Func: CNProtectKC()
//
//	Desc: ������ 
//********************************************************************************/
//CNProtectKC::CNProtectKC()
//{
//	
//	m_IsCreate		= FALSE;
//	m_IsUse			= TRUE;
//
//	m_hKCrypt		= 0;	
//
//}
//
//
///********************************************************************************
//    Func: ~CNProtectKC()
//
//	Desc: �Ҹ��� 
//********************************************************************************/
//CNProtectKC::~CNProtectKC()
//{
//
//	Release();
//
//}
//
//
///********************************************************************************
//    Func: Release()
//
//	Desc: �޸� ���� 
//********************************************************************************/
//void CNProtectKC::Release(void)
//{
//
//	if(!m_IsCreate)
//		return;
//
//	CloseHandle();
//
//	m_IsCreate = FALSE;
//
//}
//
//
///********************************************************************************
//    Func: InitKeyCrypt()
//
//	Desc: �ʱ�ȭ 
//********************************************************************************/
//BOOL CNProtectKC::InitKeyCrypt(void)
//{
//
//
////���� nProtect ���� �ȴٸ� Ȱ��ȭ 
//#ifndef NO_GAMEGUARD
//	  m_IsUse = TRUE;
//#else
//	  m_IsUse = FALSE;
//#endif
//
//	//������� �ʴ´ٸ� ���� 
//	if(!m_IsUse)
//		return TRUE;
//
//
//	Release();
//
//	//DLL PATH ���� 
//	TCHAR szFileName[MAX_PATH] = {0};
//	GetModuleFileName(NULL, szFileName, sizeof(szFileName));
//
//
//	//ȣȯ�� ���� 
//	if(!ChekAppCompatFlag(szFileName))
//		return FALSE;
//
//
//	char *ptr = strrchr(szFileName,'\\');
//		if(ptr != NULL)
//			*ptr = 0;
//
//	//���ҽ� PATH ���� 
//	NPKSetDrvPath(szFileName);
//	
//	//NPK Open
//	m_hKCrypt = NPKOpenDriver();
//	
///////////////////////////////////////////////////////////////////////////////////////////
////	nProtect ���� �ǰ� �ϴ� ����ó���� �ҽ� ���� 
///////////////////////////////////////////////////////////////////////////////////////////
//		if(m_hKCrypt <= 0)
//		{	
//			DWORD	dwError = GetLastError();
//			
//			//NPK ǥ�ؿ��� 
//			if((dwError & NPK_ERROR_BASE) == NPK_ERROR_BASE)
//			{
//				
//				LPCTSTR	eMsg[] = 
//				{
//					{ "Adminustrator�� �α��� �Ͻñ� �ٶ��ϴ�." },
//					{ "nProtect KeyCrypt ������ ���� �ʽ��ϴ�.\n����� �� ���� ���� �ּ���."},
//				 	{ "WinXP ȣȯ�� ��带 ����ϰ� ��ʴϴ�.\nȣȯ�� ��带 ���� �Ͽ����ϴ�. �ٽ� ������ �ּ���" }  
//				};
//				
//				LPCTSTR msg = NULL;	
//			
//
//				switch(dwError)
//				{
//					//Admin ������ �ƴҶ� �ý��� ����۽� Admin������ ��� ������ ������.
//					case NPK_ERROR_NOTADMIN:
//						{
//							NPKLoadAtStartup(TRUE);
//							msg = eMsg[0];
//							break;
//						}
//					
//					//����̹� ������ �ٸ� 
//					case NPK_ERROR_DRIVERVERSION:
//						{	msg = eMsg[1];	break; }
//
//					//ȣȯ�� ��� ���� 
//					case NPK_ERROR_VERIFYVERSION:
//						{
//							//GetModuleFileName(NULL,szFileName,sizeof(szFileName));
//							//NPKSetAppCompatFlag(szFileName,m_apl);
//							msg = eMsg[2];	break;
//						}
//				}
//				
//                MessageBox(NULL,msg,"nProtect KeyCrypt",MB_OK);
//				
//			}
//			//�ý��� ���� 
//			else
//				GetWindowsErrorMsg(dwError);
//
//			return FALSE;
//		}
/////////////////////////////////////////////////////////////////////////////////////////////
//	
//	NPKLoadAtStartup(TRUE);
//
//	m_IsCreate = TRUE;
//
//	return m_IsCreate;
//
//
//}
//
//
///********************************************************************************
//    Func: CloseHandle()
//
//	Desc: KeyCrypt �ڵ��� �ݴ´�. 
//********************************************************************************/
//void CNProtectKC::CloseHandle(void)
//{
//
//	if(!m_IsUse)
//		return;
//
//	if(!m_IsCreate)
//		return;
//
//	NPKCloseDriver(m_hKCrypt);
//	
//}
//
//
///********************************************************************************
//    Func: ChekAppCompatFlag()
//
//	Desc: ȣȯ���� üũ �� �̻� �ִٸ� ������ �����Ѵ�. 
//********************************************************************************/
//BOOL CNProtectKC::ChekAppCompatFlag(char* szGameFileName)
//{
//
//	//ȣȯ������ "apcNone"�� �ƴϸ� ���� �� �� ���� 
//	if(NPKGetAppCompatFlag(szGameFileName) != apcfNone)
//    {
//		
//		NPKSetAppCompatFlag(szGameFileName,apcfNone);
//
//		MessageBox(NULL,"���� �� �ٽ� ����� ���ּ���","nProtect KeyCrypt",MB_OK);
//		//PostQuitMessage(0);
//		return FALSE;
//	}
//
//	return TRUE;
//
//}


/********************************************************************************
    Func:

	Desc:
********************************************************************************/

/********************************************************************************
    Func:

	Desc:
********************************************************************************/

/********************************************************************************
    Func:

	Desc:
********************************************************************************/