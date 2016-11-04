
#include "stdAFX.h"
#include "SHO_WS_LIB.h"
#include "WS_SocketLSV.h"
#include "CWS_Client.h"
#include "CWS_Server.h"


//-------------------------------------------------------------------------------------------------
bool WSLSV_Socket::WndPROC( WPARAM wParam, LPARAM lParam )
{
	return g_pSockLSV->Proc_SocketMSG( wParam, lParam );
}

//-------------------------------------------------------------------------------------------------
WS_lsvSOCKET::WS_lsvSOCKET() //: CCriticalSection( 4000 )
{
	m_pRecvPket = (t_PACKET*) new char[ MAX_PACKET_SIZE ];

	m_bActiveMODE = false;
	m_pReconnectTimer = NULL;
	m_bTryCONN = false;
}
WS_lsvSOCKET::~WS_lsvSOCKET ()
{
	SAFE_DELETE( m_pReconnectTimer );

	SAFE_DELETE_ARRAY( m_pRecvPket );
}

//-------------------------------------------------------------------------------------------------
void WS_lsvSOCKET::Init (HWND hWND, char *szLoginServerIP, int iLoginServerPort, UINT uiSocketMSG, bool bAutoActive)
{
	m_bActiveMODE = bAutoActive;
	m_hMainWND = hWND;
	m_LoginServerIP.Set( szLoginServerIP );
	m_LoginServerPORT = iLoginServerPort;
	m_uiSocketMSG = uiSocketMSG;

	if ( NULL == m_pReconnectTimer )
		m_pReconnectTimer = new CTimer( m_hMainWND, WS_TIMER_LSV, RECONNECT_TIME_TICK, (TIMERPROC)WS_TimerProc );
}

//-------------------------------------------------------------------------------------------------
bool WS_lsvSOCKET::Connect()
{
	return m_SockLSV.Connect( m_hMainWND, m_LoginServerIP.Get(), m_LoginServerPORT, m_uiSocketMSG);
}
void WS_lsvSOCKET::Disconnect ()
{
	if ( m_pReconnectTimer )
		m_pReconnectTimer->Stop ();

//	this->Lock ();
	{
		m_SockLSV.Close ();
	}
//	this->Unlock ();
}


//-------------------------------------------------------------------------------------------------
void WS_lsvSOCKET::Send_srv_ACTIVE_MODE (bool bActive)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return;

	pCPacket->m_HEADER.m_wType = SRV_ACTIVE_MODE;
	pCPacket->m_HEADER.m_nSize = sizeof( srv_ACTIVE_MODE );

	pCPacket->m_srv_ACTIVE_MODE.m_bActive = bActive;

	m_SockLSV.Packet_Register2SendQ( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
}

//-------------------------------------------------------------------------------------------------
void WS_lsvSOCKET::Send_zws_SERVER_INFO ()
{
	SHO_WS *pGSV = SHO_WS::GetInstance();

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return;
//	this->Lock ();
	{
		pCPacket->m_HEADER.m_wType = ZWS_SERVER_INFO;
		pCPacket->m_HEADER.m_nSize = sizeof( zws_SERVER_INFO );

		pCPacket->m_zws_SERVER_INFO.m_wPortNO = (short)pGSV->GetListenPort();
		pCPacket->m_zws_SERVER_INFO.m_dwSeed  = pGSV->GetRandomSeed();
/*
		�α��� �������� ������ �ʿ� ����.
		pCPacket->m_zws_SERVER_INFO.m_Channel.m_btChannelNO	= (short)pGSV->GetChannelNO();
		pCPacket->m_zws_SERVER_INFO.m_Channel.m_btLowAGE	= pGSV->GetLowAGE ();
		pCPacket->m_zws_SERVER_INFO.m_Channel.m_btHighAGE	= pGSV->SetHighAGE ();
*/
		pCPacket->AppendString( pGSV->GetServerName() );
		pCPacket->AppendString( pGSV->GetServerIP() );

		m_SockLSV.Packet_Register2SendQ( pCPacket );

		// �α��� ������ ���߿� ��������� ���...
		// ���� ���ӵǾ� �ִ� ����� ����Ʈ�� ����...
	}
//	this->Unlock ();
	Packet_ReleaseNUnlock( pCPacket );

	this->Send_wls_ACCOUNT_LIST ();
	this->Send_wls_CHANNEL_LIST ();
}

//-------------------------------------------------------------------------------------------------
void WS_lsvSOCKET::Send_wls_CHANNEL_LIST ()
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return;

	// �α��� ������ ä�� ���� ���� ����.
	g_pListSERVER->Make_wls_CHANNEL_LIST( pCPacket );
	m_SockLSV.Packet_Register2SendQ( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
}



//-------------------------------------------------------------------------------------------------
void WS_lsvSOCKET::Send_zws_CONFIRM_ACCOUNT_REQ (DWORD dwSocketID, t_PACKET *pPacket)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return;
//	this->Lock ();
	{
		// �α��� ������ GSV_ADD_USER_REQ ���� �Ѵ�...
		pCPacket->m_HEADER.m_wType = ZWS_CONFIRM_ACCOUNT_REQ;
		pCPacket->m_HEADER.m_nSize = sizeof( zws_CONFIRM_ACCOUNT_REQ );

		pCPacket->m_zws_CONFIRM_ACCOUNT_REQ.m_dwServerID	= pPacket->m_cli_JOIN_SERVER_REQ.m_dwID;
		pCPacket->m_zws_CONFIRM_ACCOUNT_REQ.m_dwClientID	= dwSocketID;

		::CopyMemory(pCPacket->m_zws_CONFIRM_ACCOUNT_REQ.m_dwMD5Password, pPacket->m_cli_JOIN_SERVER_REQ.m_MD5Password, sizeof(DWORD)*8);
		// pCPacket->AppendString ( szAccount ) szAccont ���Ҽ� ����.

		m_SockLSV.Packet_Register2SendQ( pCPacket );
	}
//	this->Unlock ();
	Packet_ReleaseNUnlock( pCPacket );
}

//-------------------------------------------------------------------------------------------------
void WS_lsvSOCKET::Send_zws_SUB_ACCOUNT (DWORD dwLSID, char *szAccount)
{
	//if ( !dwLSID )	LS���� ������û�� ������ WS�� ��ã����� dwLSID = 0�̴�.
	//	return;

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return;

	pCPacket->m_HEADER.m_wType	= ZWS_SUB_ACCOUNT;
	pCPacket->m_HEADER.m_nSize	= sizeof( zws_SUB_ACCOUNT );
	pCPacket->AppendString( szAccount );

	m_SockLSV.Packet_Register2SendQ( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
}

//-------------------------------------------------------------------------------------------------
void WS_lsvSOCKET::Send_wls_ACCOUNT_LIST ()
{
	// ���� ���ӵǾ� �ִ� ��� ������ �α��� ������ ������.
	g_pUserLIST->Send_wls_ACCOUNT_LIST ();
}

//-------------------------------------------------------------------------------------------------
void WS_lsvSOCKET::Recv_lsv_CHECK_ALIVE ()
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return;
//	this->Lock ();
	{
		pCPacket->m_HEADER.m_wType = GSV_CHECK_ALIVE;
		pCPacket->m_HEADER.m_nSize = sizeof( lsv_CHECK_GSV );

		m_SockLSV.Packet_Register2SendQ( pCPacket );
	}
//	this->Unlock ();
	Packet_ReleaseNUnlock( pCPacket );
}

//-------------------------------------------------------------------------------------------------
void WS_lsvSOCKET::Recv_wls_CONFIRM_ACCOUNT_REPLY ()
{
	// �α��� ������ ���� ������ �޾Ҵ�.
	short nOffset=sizeof( wls_CONFIRM_ACCOUNT_REPLY );
	char *szAccount = Packet_GetStringPtr( m_pRecvPket, nOffset );

	g_pUserLIST->Add_ACCOUNT( m_pRecvPket->m_wls_CONFIRM_ACCOUNT_REPLY.m_dwWSID, m_pRecvPket, szAccount );
}

void WS_lsvSOCKET::Recv_lsv_ANNOUNCE_CHAT ()
{
/*
	m_pRecvPket->m_HEADER.m_wType = GSV_ANNOUNCE_CHAT;
	g_pZoneLIST->Send_wsv_ANNOUNCE_CHAT( m_pRecvPket );
*/
}

//-------------------------------------------------------------------------------------------------
// �ٸ� ����ڿ� ���� �α��εƴ�... ���� ���� ��Ŵ...
void WS_lsvSOCKET::Recv_wls_KICK_ACCOUNT ()
{
	short nOffset   = sizeof( t_PACKETHEADER );
	char *szAccount = Packet_GetStringPtr (m_pRecvPket, nOffset);

	CWS_Client *pUSER = (CWS_Client*)g_pUserLIST->Find_ACCOUNT( szAccount );
	if ( pUSER ) {
		// ���弭���� ������ ��� �ִ�..
		pUSER->CloseSocket ();
	} else {
		// ��� ä�� ������ ©��� ����...
		classPACKET *pCPacket = Packet_AllocNLock ();
		if ( pCPacket ) {
			pCPacket->m_HEADER.m_wType = WLS_KICK_ACCOUNT;
			pCPacket->m_HEADER.m_nSize = sizeof( wls_KICK_ACCOUNT );
			pCPacket->AppendString( szAccount );

			g_pListSERVER->Send_ToWORLD( pCPacket );

			Packet_ReleaseNUnlock( pCPacket );
		}

		// ä�μ����� �������۾��� �׾������... ���弭�� ��Ʈ�� �����ǰ� ä�μ��� ��Ʈ�� ��������� �ִ�.
		// ������ ���ų� ������ pAccount->m_pCLIENT==NULL�̴�...�� ä�μ������� ���� ����ƴ�.
		g_pUserLIST->Del_ACCOUNT( szAccount, BIT_LOGIN_GS );
	}
}

//-------------------------------------------------------------------------------------------------
bool WS_lsvSOCKET::Proc_SocketMSG (WPARAM wParam, LPARAM lParam)
{
	int nErrorCode = WSAGETSELECTERROR(lParam);
	switch ( WSAGETSELECTEVENT(lParam) ) {
		case FD_READ:
		{
			m_SockLSV.OnReceive( nErrorCode );

			// ���� ��Ŷ ó��..
			while( m_SockLSV.Peek_Packet( m_pRecvPket, true ) ) {
				// LogString( LOG_DEBUG, "Handle LS Packet: Type[ 0x%x ], Size[ %d ]\n", m_pRecvPket->m_HEADER.m_wType, m_pRecvPket->m_HEADER.m_nSize);
				switch( m_pRecvPket->m_HEADER.m_wType ) {
					case LSV_CHECK_ALIVE :
						Recv_lsv_CHECK_ALIVE ();
						break;

					case WLS_CONFIRM_ACCOUNT_REPLY :
						Recv_wls_CONFIRM_ACCOUNT_REPLY ();
						break;

					case LSV_ANNOUNCE_CHAT :
						Recv_lsv_ANNOUNCE_CHAT ();
						break;
					case WLS_KICK_ACCOUNT :
						Recv_wls_KICK_ACCOUNT ();
						break;

					default :
						g_LOG.CS_ODS( 0xffff, "Undefined Login server packet... Type[ 0x%x ], Size[ %d ]\n", m_pRecvPket->m_HEADER.m_wType, m_pRecvPket->m_HEADER.m_nSize);
				}
			}
			break;
		}
		case FD_WRITE:
			m_SockLSV.OnSend ( nErrorCode );
			break;
		case FD_OOB:
			m_SockLSV.OnOutOfBandData ( nErrorCode );
			break;
		/*
		case FD_ACCEPT:
			m_SockLSV.OnAccept ( nErrorCode );
			break;
		*/
		case FD_CONNECT: 
		{
			m_SockLSV.OnConnect ( nErrorCode );
			if ( !nErrorCode ) {
				g_LOG.CS_ODS( 0xffff, "Connected to login server \n");
				m_bTryCONN = false;

				if ( m_pReconnectTimer )
					m_pReconnectTimer->Stop ();
				this->Send_zws_SERVER_INFO ();

				if ( m_bActiveMODE )
					this->Send_srv_ACTIVE_MODE( true );
			} else {
				if ( m_pReconnectTimer )
					m_pReconnectTimer->Start ();

				if ( !m_bTryCONN ) {
					m_bTryCONN = true;
					g_LOG.CS_ODS( 0xffff, "Connect failed to login server\n");
				}
			}
			break;
		}
		case FD_CLOSE:		// Close()�Լ��� ȣ���ؼ� ����ɶ��� �߻� ���Ѵ�.
		{
			m_SockLSV.OnClose ( nErrorCode );

			g_LOG.CS_ODS( 0xffff, "Disconnected from login server\n");

			if ( m_pReconnectTimer )
				m_pReconnectTimer->Start ();
		}
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
