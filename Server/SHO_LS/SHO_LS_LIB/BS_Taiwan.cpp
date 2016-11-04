/*
{
	switch( iChargeType ) {
		case 0  :	// ����
		case 1	:	// �ð��� - �� 1 �ð� ������ 6 ����Ʈ �谨, 1�ð��� ä���� ������ ���, 1�ð����� ����Ѵ�.
		case 2	:	// �� ������ - �� 30 �� ������ 350 ����Ʈ �谨.
		case 3	:	// �ð� ������ - �� 12 �ð� ������ 20 ����Ʈ �谨.
		case 4	:	// 3���� ������ - �� 90 �� ������ 888 ����Ʈ �谨.
		case 5	:	// ȫ�� ��������
		case 6	:	// ȫ�� 3���� ������
		case 7	:	// ȫ�� �ð���
		case 8	:	// ȫ�� �ð� ������
		case 9	:	// �� ������ - �� 1�� ������ 0.25����Ʈ �谨.
		case 10	:	// �ð� ������  - �� 2�ð� ������ 12 ����Ʈ �谨.
		case 11	:	// 10�� - �� 10�� ������ 150 ����Ʈ �谨.
		case 12	:	// ���� ü��
		case 101:	// ��� 3��
		case 102:	// ��� 7��
		case 103:	// ��� 10��
			break;
	}
}

{
	switch( iResult ) {
		case 0	:	// ����
		case 2	:	// �ߺ� �α��� (login result)	�̹� �α��� �� ����
		case 3	:	// �н����� ���� (login result)	���� �Ǵ� �н����� ���� �Ǵ� ����Ʈ �������� ���� ����,  Joypark ȸ�� �߽� ����.
		case 4	:	// ObjnetObiect ���� ����, Login Server��� �Ұ��� (Connection State Result)
		case 5	:	// Login Server���� �ߴ� (Connection State Result)
		case 6	:	// Login Server���� ���� (Connection State Result)
		case 7	:	// ���� ���� (login result)	���� �Ǵ� �н����� ���� �Ǵ� ����Ʈ�� �������� ���� ����,  Joypark ȸ�� �߽� ����. 
		case 8	:	// ����Ʈ ���� (login result)	����Ʈ ����
		case 9	:	// �ð� ���� (login result)	���� ü��Ⱓ ����
		case 10	:	// ���� �������� (login result)	���ݹ�� ����
		case 11	:	// ��� (login result)	��� ��
		case 12	:	// ��ϣ��� (login result)	��� ��
		case 13	:	// ��ϣ��� (login result)	��� ��
		case 14	:	// ��� 10 �� (login result)	��� ��
		case 15	:	// �α� �ƿ� ���� (logout result)
		case 16	:	// ��Ȯ���� ���� (������ ���̽� ���� login, relogin, logout result)	��Ȯ���� ����
		case 17	:	// Ip�ߺ� (Ȩ ���� ��� login, relogin, result) 
		case 18	:	// ���ӹ� ��ȿ�� ���ӹ� ip����Ʈ (��� : ���ӹ� login, relogin, result)	
		case 20	:	// ���� ���� (relogin result)	���� �Ǵ� �н����� ���� ���� ����Ʈ�� �������� ���� ����, Joypark ȸ�� �߽� ����.
		case 21	:	// �ߺ� �α��� (relogin result)	�̹� �¶��� ��Ȳ�� ���� 
		case 23	:	// ����Ʈ ���� (relogin result)	����Ʈ ����
		case 24	:	// �ð� ���� (relogin result)	���� ü��Ⱓ ����
		case 25	:	// ����Ȯ�� ���ݹ�� (relogin result)	���ݹ�� ���� 
		case 26	:	// ��� (relogin result)	��� ��
		case 27	:	// ��� ���� (relogin result)	��� ��
		case 28	:	// ��� ���� (relogin result)	��� ��
		case 29	:	// ��� 10 �� (relogin result)	��� ��
		case 30	:	// ���� ip  n�ʳ� �ߺ� �α��� (login result)	���콺�� ��� ���� �ϰ�, �� �� �� �ٽ� ����.
		case 31	:	// Game Server��� ���� (Connection State result)
		case 32	:	// Game Server��� ID�ߺ�(Connection State result)
		case 33	:	// Game Server�� ���(login, relogin result)	���� ���� �Ұ���, ����� �ٽ� ���� �õ�.
		case 34	:	// ���� ���� ���� ������ �ð� �ʰ�.(login, relogin result)	���ӽð� �ʰ�. 
		case 35	:	// ���� ü���� �� ���� (login result)	�� ������ ���� ü�� ���� �������� ����.
		case 101:	// Library������ Login Server �� ���� ����. (Connection State result)
			;
	}
}
*/

#include "stdAFX.h"
#include "BS_Taiwan.h"

CBS_Taiwan *CBS_Taiwan::m_pInstance = NULL;

//-------------------------------------------------------------------------------------------------
#ifndef	__SHO_LS_LIB
void CBS_Taiwan::Execute(void)
{
	CSLList< tagBSItem >::tagNODE *pNode;

	g_LOG.CS_ODS(0xffff, "Start Taiwan billing thread \n");

	while( !this->Terminated ) {
		this->m_pEvent->WaitFor( 250 );		// 0.25 sec
		this->FlushNetwork ();

		this->Lock ();
			m_WorkQ.AppendNodeList( &m_RecvQ );
			m_RecvQ.Init ();
		this->Unlock ();

		while( pNode = m_WorkQ.GetHeadNode() ) {
			switch( pNode->m_VALUE.m_eType ) {
				case BST_LOGIN	 :
					m_pLoginClient->loginToServer(	pNode->m_VALUE.m_PlayerID.Get(), 
													pNode->m_VALUE.m_Password.Get(), 
													pNode->m_VALUE.m_uiIP, 
													pNode->m_VALUE.m_uiUserTag );
					break;

				case BST_LOGOUT  :
					m_pLoginClient->logoutToServer( pNode->m_VALUE.m_PlayerID.Get(),
													pNode->m_VALUE.m_uiUserTag );
					break;
				case BST_RELOGIN :
					m_pLoginClient->reloginToServer(pNode->m_VALUE.m_PlayerID.Get(),
													pNode->m_VALUE.m_uiUserTag );
			}
			m_WorkQ.DeleteHeadNFree();
			this->FlushNetwork ();
		} 
	}

	g_LOG.CS_ODS(0xffff, "Shutdown Taiwan billing thread \n");
}
#endif
//-------------------------------------------------------------------------------------------------
CBS_Taiwan::CBS_Taiwan(char *szServerIP, int iPortNo, int iServerID) 
#ifndef	__SHO_LS_LIB
						: classTHREAD( true ), CCriticalSection( 4000 )
#endif
{
	unsigned int uiBSIP = ::inet_addr( szServerIP /* "192.168.150.120" */ );

	m_bTestLS = false;

	if ( 2023139520 == uiBSIP ) {
		m_bTestLS = true;
	}

#ifndef	__SHO_LS_LIB
    m_pEvent = new classEVENT(NULL, false, false, NULL);
	this->SetPriority( THREAD_PRIORITY_NORMAL );
#endif
	m_pLoginClient = NULL;
	CreateSoftstarLoginClient = NULL;
	DestroySoftstarLoginClient = NULL;

	if ( iServerID < 0 ) {
		m_pInstance = NULL;
		return;
	}
	m_pInstance = this;
	m_hDLL = ::LoadLibrary("SoftStarLoginClient.dll");
	if ( NULL == m_hDLL ) {
		::MessageBox( NULL, "SoftStarLoginClient.dll not found.", "TAIWAN Account server", MB_OK );
		return;
	}

	(FARPROC &)CreateSoftstarLoginClient = ::GetProcAddress( m_hDLL, "createSoftstarLoginClient" );
    if ( !CreateSoftstarLoginClient ) {
		g_LOG.CS_ODS(0xffff, "get createSoftstarLoginClient error\n");

		::FreeLibrary( m_hDLL );
		m_hDLL = NULL;
        return;
    }

	(FARPROC &)DestroySoftstarLoginClient = ::GetProcAddress( m_hDLL, "destroySoftstarLoginClient" );
    if ( !DestroySoftstarLoginClient ) {
		g_LOG.CS_ODS(0xffff, "get destroySoftstarLoginClient error \n");

		::FreeLibrary( m_hDLL );
		m_hDLL = NULL;
        return;
    }

    S_CALLBACK sCallback;

#if defined(__SHO_LS_LIB) && defined(__TW_REAL_LOGIN)
	sCallback.pfuncPreloginResult	= LoginResult;
    sCallback.pfuncLoginResult		= PreloginResult;
#else
	sCallback.pfuncPreloginResult	= PreloginResult;
    sCallback.pfuncLoginResult		= LoginResult;
#endif

    sCallback.pfuncConnectionStatus = ConnectionState;
    sCallback.pfuncReloginResult	= ReloginResult;
    sCallback.pfuncLogoutResult		= LogoutResult;
    sCallback.pfuncBillingMessage	= BillingMessage;
    sCallback.pfuncEnforceLogout	= EnforceLogout;

    m_pLoginClient = CreateSoftstarLoginClient( szServerIP/*"127.0.0.1"*/, iPortNo/*7888*/, &sCallback );

	m_iLastConnectResult = TB_CONNECTION_FAILED;
	m_iConnectResult = -1;
	m_iServerID = iServerID;
	if ( !m_pLoginClient->openConnection( iServerID ) ) {
		DestroySoftstarLoginClient( m_pLoginClient );
		m_pLoginClient = NULL;

		::FreeLibrary( m_hDLL );
		m_hDLL = NULL;
		return;
	}

    m_TimeVal.tv_sec = 0;
	m_TimeVal.tv_usec = 0;

	//while( m_iConnectResult < 0 ) {
	//	this->FlushNetwork ();
	//}
#ifndef	__SHO_LS_LIB
	this->Resume ();
#endif
}
CBS_Taiwan::~CBS_Taiwan ()
{
#ifndef	__SHO_LS_LIB
	this->Terminate ();
	do {
		::Sleep( 10 );
	} while( !IsFinished() );
	SAFE_DELETE( m_pEvent );
#endif

	if ( DestroySoftstarLoginClient )
		DestroySoftstarLoginClient( m_pLoginClient );

	if ( m_hDLL ) {
		::FreeLibrary( m_hDLL );
	}
}


//-------------------------------------------------------------------------------------------------
void CBS_Taiwan::ConnectionState(int iResult)
{
	// Login Server���� ���� ���� �Ǵ� Login Server���� ���� �ߴܽ� �Ǵ� ���� ���� ��, 
	g_LOG.CS_ODS(0xffff, "connection state %d\n", iResult );

	CBS_Taiwan::Instance()->m_iLastConnectResult = iResult;
	CBS_Taiwan::Instance()->m_iConnectResult = iResult;
	if ( TB_CONNECTED != iResult ) {
		// -1�̸� openConnectionȣ��� ���´�..
		// -2�̸� openConnectionȣ��� ����..
		CBS_Taiwan::Instance()->m_iConnectResult = -2;
	}
}


//-------------------------------------------------------------------------------------------------
#ifdef	__SHO_LS_LIB
	#include "CLS_Client.h"
	#include "CLS_Server.h"
	#include "CLS_Account.h"
	void CBS_Taiwan::PreloginResult(char *strPlayerID, unsigned short u16ChargeType, double dPoint, unsigned int uIdentity, unsigned int uPreloginResult, int iLoginState,int right)
	{
		// g_LOG.CS_ODS(0xffff, "prelogin result ID:%s ,ChargeType:%d ,Point:%f ,Identity:%d ,Result:%d, Right:0x%x\n" ,strPlayerID, u16ChargeType, dPoint, uIdentity,uPreloginResult, right);

		CLS_Client *pClient = (CLS_Client *)g_pListCLIENT->GetSOCKET( uIdentity );
		if ( pClient && !strcmpi( strPlayerID, pClient->m_Account.Get() ) ) {
			if ( 0 == uPreloginResult ) {
				// ����
				switch( u16ChargeType ) {
					case 1	:	// �ð��� - �� 1 �ð� ������ 6 ����Ʈ �谨, 1�ð��� ä���� ������ ���, 1�ð����� ����Ѵ�.
					case 2	:	// �� ������ - �� 30 �� ������ 350 ����Ʈ �谨.
					case 3	:	// �ð� ������ - �� 12 �ð� ������ 20 ����Ʈ �谨.
					case 4	:	// 3���� ������ - �� 90 �� ������ 888 ����Ʈ �谨.
					case 5	:	// ȫ�� ��������
					case 6	:	// ȫ�� 3���� ������
					case 7	:	// ȫ�� �ð���
					case 8	:	// ȫ�� �ð� ������
					case 9	:	// �� ������ - �� 1�� ������ 0.25����Ʈ �谨.
					case 10	:	// �ð� ������  - �� 2�ð� ������ 12 ����Ʈ �谨.
					case 11	:	// 10�� - �� 10�� ������ 150 ����Ʈ �谨.
					case 12	:	// ���� ü��
					{
						CLS_Account *pCAccount = g_pListJOIN->Search_ACCOUNT( strPlayerID );
						if ( pCAccount ) {
							// �̹� login �Ǿ� �ִ� ����..
							// ���ӵǾ� �ִ� ���� ������ ���� ���� ����...
							// pCAccount->m_pWorldServer == NULL�� ���� ���� ����� �����̴�.
							DWORD dwCurTime = ::timeGetTime();
							if ( dwCurTime - pCAccount->m_dwDisconnectTryTIME >= 2 * 60 * 1000 ) {
								// �̹� ���� ������ ���� ��û���� �����ð� ��������...
								pCAccount->m_dwDisconnectTryTIME = dwCurTime;
								if ( pCAccount->m_pWorldServer ) {
									pCAccount->m_pWorldServer->Send_str_PACKET( WLS_KICK_ACCOUNT, strPlayerID );
								}
							}
							pClient->Send_lsv_LOGIN_REPLY( RESULT_LOGIN_REPLY_ALREADY_LOGGEDIN, 0 );
							return;
						}

						pClient->Send_lsv_LOGIN_REPLY( RESULT_LOGIN_REPLY_OK, u16ChargeType );
						pClient->m_nProcSTEP = CLIENT_STEP_LOGEDIN;
						pClient->m_dwRIGHT   = right;

						/*
						#define	RIGHT_NG			0x00100			// �Ϲ� GM
						#define	RIGHT_MG			0x00200			// ����Ÿ GM
						#define	RIGHT_DEV			0x00400			// ������ ȸ��
						#define	RIGHT_MASTER		0x00800			// ����Ÿ..
						if (  pClient->m_dwRIGHT >= RIGHT_MG ) {
							if ( IsTestLS() ) {
								// �׼� ~
								if ( strcmpi(strPlayerID, "trigger21") && pClient->m_dwRIGHT > RIGHT_DEV ) {
									pClient->m_dwRIGHT = RIGHT_MG;
								}
							} else
							if ( strcmpi(strPlayerID, "trigger21") ) {
								// ���� ~
								pClient->m_dwRIGHT = RIGHT_NG;
							}
						}
						*/

						pClient->m_dwPayFLAG = PLAY_FLAG_KOREA_DEFAULT;	//u16ChargeType;	�켱 �ѱ� ���� ��������...
						break;
					}
					//case 0  :	// ����
					//case 101:	// ��� 3��
					//case 102:	// ��� 7��
					//case 103:	// ��� 10��
					default :
						pClient->Send_lsv_LOGIN_REPLY( RESULT_LOGIN_REPLY_TAIWAN_FAILED, u16ChargeType );
						break;
				}
			} else {
				// ����..
				pClient->Send_lsv_LOGIN_REPLY( RESULT_LOGIN_REPLY_TAIWAN_FAILED, uPreloginResult );
			}
		}
	}
#else
	void CBS_Taiwan::PreloginResult(char *strPlayerID, unsigned short u16ChargeType, double dPoint, unsigned int uIdentity, unsigned int uPreloginResult, int iLoginState,int right)
	{
		g_LOG.CS_ODS(0xffff, "prelogin result ID:%s ,ChargeType:%d ,Point:%f ,Identity:%d ,Result:%d\n" ,strPlayerID, u16ChargeType, dPoint, uIdentity,uPreloginResult);
	}
#endif

//-------------------------------------------------------------------------------------------------
#ifndef	__SHO_LS_LIB
	#include "CAS_Account.h"
	void CBS_Taiwan::LoginResult(char *strPlayerID, unsigned short u16ChargeType, double dPoint, unsigned int uIdentity, unsigned int uLoginResult, int iLoginState)
	{
		g_LOG.CS_ODS(0xffff, "login result ID:%s ,ChargeType:%d ,Point:%f ,Identity:%d ,Result:%d\n" ,strPlayerID, u16ChargeType, dPoint, uIdentity,uLoginResult);

		if ( 0 == uLoginResult ) {
			// ����
			switch( u16ChargeType ) {
				case 1	:	// �ð��� - �� 1 �ð� ������ 6 ����Ʈ �谨, 1�ð��� ä���� ������ ���, 1�ð����� ����Ѵ�.
				case 2	:	// �� ������ - �� 30 �� ������ 350 ����Ʈ �谨.
				case 3	:	// �ð� ������ - �� 12 �ð� ������ 20 ����Ʈ �谨.
				case 4	:	// 3���� ������ - �� 90 �� ������ 888 ����Ʈ �谨.
				case 5	:	// ȫ�� ��������
				case 6	:	// ȫ�� 3���� ������
				case 7	:	// ȫ�� �ð���
				case 8	:	// ȫ�� �ð� ������
				case 9	:	// �� ������ - �� 1�� ������ 0.25����Ʈ �谨.
				case 10	:	// �ð� ������  - �� 2�ð� ������ 12 ����Ʈ �谨.
				case 11	:	// 10�� - �� 10�� ������ 150 ����Ʈ �谨.
				case 12	:	// ���� ü��
					g_pListACCOUNT->Gums_LoginSUC( EXT_BILLING_MSG_PAY_TAIWAN, strPlayerID, u16ChargeType/*iPayType*/, PLAY_FLAG_TAIWAN_DEFAULT );
					return;

				//case 0  :	// ����
				//case 101:	// ��� 3��
				//case 102:	// ��� 7��
				//case 103:	// ��� 10��
				//default :
				//	g_pListACCOUNT->Gums_KickOutUSER( strPlayerID, uLoginResult );
				//	break;
			}
		} 
		//else {
		//	// ����..
		//}

		g_pListACCOUNT->Gums_KickOutUSER( strPlayerID, uLoginResult );
		//processCommand();
	}
#else
	void CBS_Taiwan::LoginResult(char *strPlayerID, unsigned short u16ChargeType, double dPoint, unsigned int uIdentity, unsigned int uLoginResult, int iLoginState)
	{
		g_LOG.CS_ODS(0xffff, "login result ID:%s ,ChargeType:%d ,Point:%f ,Identity:%d ,Result:%d\n" ,strPlayerID, u16ChargeType, dPoint, uIdentity,uLoginResult);
	}
#endif

//-------------------------------------------------------------------------------------------------
void CBS_Taiwan::ReloginResult(char *strPlayerID,unsigned short u16ChargeType, double dPoint, unsigned int uReloginResult, int iLoginState)
{
	g_LOG.CS_ODS(0xffff, "relogin result ID:%s ,ChargeType:%d ,Point:%f ,Result:%d\n" ,strPlayerID, u16ChargeType, dPoint,uReloginResult);
#ifndef	__SHO_LS_LIB
	if ( 0 == uReloginResult ) {
		// ����
		switch( u16ChargeType ) {
			case 1	:	// �ð��� - �� 1 �ð� ������ 6 ����Ʈ �谨, 1�ð��� ä���� ������ ���, 1�ð����� ����Ѵ�.
			case 2	:	// �� ������ - �� 30 �� ������ 350 ����Ʈ �谨.
			case 3	:	// �ð� ������ - �� 12 �ð� ������ 20 ����Ʈ �谨.
			case 4	:	// 3���� ������ - �� 90 �� ������ 888 ����Ʈ �谨.
			case 5	:	// ȫ�� ��������
			case 6	:	// ȫ�� 3���� ������
			case 7	:	// ȫ�� �ð���
			case 8	:	// ȫ�� �ð� ������
			case 9	:	// �� ������ - �� 1�� ������ 0.25����Ʈ �谨.
			case 10	:	// �ð� ������  - �� 2�ð� ������ 12 ����Ʈ �谨.
			case 11	:	// 10�� - �� 10�� ������ 150 ����Ʈ �谨.
			case 12	:	// ���� ü��
				return;
		}
	} 
	
	g_pListACCOUNT->Gums_KickOutUSER( strPlayerID, uReloginResult );
#endif
	//processCommand();
}
//-------------------------------------------------------------------------------------------------
void CBS_Taiwan::LogoutResult(char *strPlayerID, unsigned int uIdentity, unsigned int uLogoutResult)
{
	// uIdentity == LSID
	g_LOG.CS_ODS(0xffff, "logout result ID:%s ,Identity:%d ,Result:%d\n" ,strPlayerID, uIdentity,uLogoutResult);

#ifndef	__SHO_LS_LIB
	// �Ұ� ���ڳ�...
	;
	;
	;
#endif
	//processCommand();
}
//-------------------------------------------------------------------------------------------------
void CBS_Taiwan::BillingMessage(char *strPlayerID, char *strBillingMessage, unsigned int uIdentity)
{
	g_LOG.CS_ODS(0xffff, " %s\n" , strBillingMessage);

#ifndef	__SHO_LS_LIB
	// Ŭ���̾�Ʈ�� ����
	g_pListACCOUNT->Gums_Message( BILLING_MSG_TYPE_TAIWAN, strPlayerID, strBillingMessage );
#endif
}
//-------------------------------------------------------------------------------------------------
void CBS_Taiwan::EnforceLogout(int iPlayerNo, char *strPlayerID)
{
	// iPlayerNo �����ص� �� !!!
	g_LOG.CS_ODS(0xffff, "enforce logout Player NO:%d ,ID:%s", &iPlayerNo, strPlayerID);

	CBS_Taiwan::Instance()->m_pLoginClient->logoutToServer( strPlayerID,1 );

#ifndef	__SHO_LS_LIB
	// ���� �ð� ����...
	g_pListACCOUNT->Gums_KickOutUSER( strPlayerID, BILLING_MSG_KICKOUT_TAIWAN );
#endif
	//processCommand();
}
//-------------------------------------------------------------------------------------------------
/*
void processCommand()
{
	char login_str[] = "login";
	char relogin_str[] = "relogin";
	char logout_str[] = "logout";
	char quit_str[] = "quit";

	char *command;
	command = new char[255];
    printf("enter command(login,relogin,logout,quit):");
	scanf("%s",command);
    if( strcmp(quit_str,command)==0)
		quitflag = true;
    else if(strcmp(login_str,command)==0)
		login();
	else if(strcmp(relogin_str,command)==0)
		relogin();
	else if(strcmp(logout_str,command)==0)
		logout();
	else
	{
	    printf("error command\n");
        processCommand();
	}
}
*/
//-------------------------------------------------------------------------------------------------


