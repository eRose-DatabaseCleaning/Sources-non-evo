
#include "LIB_gsMAIN.h"
#include "GS_SocketASV.h"
#include "GS_ListUSER.h"

bool GSASV_Socket::WndPROC( WPARAM wParam, LPARAM lParam )
{
	return g_pSockASV->Proc_SocketMSG( wParam, lParam );
}


//-------------------------------------------------------------------------------------------------
GS_asvSOCKET::GS_asvSOCKET()
{
	m_pRecvPket = (t_PACKET*) new char[ MAX_PACKET_SIZE ];
//	m_pSendPket = (t_PACKET*) new char[ MAX_PACKET_SIZE ];

	m_pReconnectTimer = NULL;
	m_bTryCONN = false;
}
GS_asvSOCKET::~GS_asvSOCKET ()
{
	SAFE_DELETE( m_pReconnectTimer );

	SAFE_DELETE_ARRAY( m_pRecvPket );
//	SAFE_DELETE_ARRAY( m_pSendPket );
}

//-------------------------------------------------------------------------------------------------
void GS_asvSOCKET::Init (HWND hWND, char *szLoginServerIP, int iLoginServerPort, UINT uiSocketMSG)
{
	m_hMainWND = hWND;
	m_AccountServerIP.Set( szLoginServerIP );
	m_AccountServerPORT = iLoginServerPort;
	m_uiSocketMSG = uiSocketMSG;

	if ( NULL == m_pReconnectTimer )
		m_pReconnectTimer = new CTimer( m_hMainWND, GS_TIMER_ASV, RECONNECT_TIME_TICK, (TIMERPROC)GS_TimerProc );
}

//-------------------------------------------------------------------------------------------------
bool GS_asvSOCKET::Connect()
{
	return m_SockASV.Connect( m_hMainWND, m_AccountServerIP.Get(), m_AccountServerPORT, m_uiSocketMSG);
}
void GS_asvSOCKET::Disconnect ()
{
	if ( m_pReconnectTimer )
		m_pReconnectTimer->Stop ();

//	this->Lock ();
	{
		m_SockASV.Close ();
	}
//	this->Unlock ();
}


//-------------------------------------------------------------------------------------------------
bool GS_asvSOCKET::Send_zws_ADD_ACCOUNT ( char *szAccount, char *szMD5Pass, char *szIP )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = ZWS_ADD_ACCOUNT;
	pCPacket->m_HEADER.m_nSize = sizeof( zws_ADD_ACCOUNT );
	pCPacket->AppendString( szAccount );
	pCPacket->AppendString( szMD5Pass );
	pCPacket->AppendString( szIP );

	m_SockASV.Packet_Register2SendQ( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
void GS_asvSOCKET::Send_zas_SUB_ACCOUNT (char *szAccount, BYTE btWhy, short nLevel, short nZoneNo)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return;
	pCPacket->m_HEADER.m_wType	= ZAS_SUB_ACCOUNT;
	pCPacket->m_HEADER.m_nSize	= sizeof( zas_SUB_ACCOUNT );

	pCPacket->m_zas_SUB_ACCOUNT.m_btWhy = btWhy;
	pCPacket->m_zas_SUB_ACCOUNT.m_nLevel = nLevel;
	pCPacket->m_zas_SUB_ACCOUNT.m_nZoneNo = nZoneNo;

	pCPacket->AppendString( szAccount );

	m_SockASV.Packet_Register2SendQ( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
}

//-------------------------------------------------------------------------------------------------
void GS_asvSOCKET::Send_zas_CHECK_ACCOUNT ( BYTE btType, char *szAccount )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return;
	pCPacket->m_HEADER.m_wType	= ZAS_CHECK_ACCOUNT;
	pCPacket->m_HEADER.m_nSize	= sizeof( zas_CHECK_ACCOUNT );

	pCPacket->m_zas_CHECK_ACCOUNT.m_btType = btType;
	pCPacket->AppendString( szAccount );

	m_SockASV.Packet_Register2SendQ( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
}

//-------------------------------------------------------------------------------------------------
bool GS_asvSOCKET::Recv_zas_KICK_ACCOUNT ()
{
	short nOffset   = sizeof( zas_KICK_ACCOUNT );
	char *szAccount = Packet_GetStringPtr (m_pRecvPket, nOffset);

	classUSER *pUser = g_pUserLIST->Find_ACCOUNT (szAccount);
	if ( pUser ) {
		pUser->Send_gsv_BILLING_MESSAGE( m_pRecvPket->m_zas_KICK_ACCOUNT.m_btWHY, "." );

		// �����ð��Ŀ� ���� �����....
		pUser->m_bKickOutFromGUMS = true;
		pUser->m_btWishLogOutMODE = LOGOUT_MODE_LEFT;
		pUser->m_dwTimeToLogOUT   = ::timeGetTime() + 10 * 1000;
	} else {
	// if ( !g_pUserLIST->Kick_ACCOUNT( szAccount ) ) {
		// ��ã�Ҵ�...
		g_LOG.CS_ODS( 0xffff, "Recv Kick Account from GUMS but [%s] not found", szAccount );
	}
	return true;
}
bool GS_asvSOCKET::Recv_zas_MESSAGE ()
{
	short nOffset   = sizeof( zas_MESSAGE );
	char *szAccount = Packet_GetStringPtr (m_pRecvPket, nOffset);
	char *szMsg     = Packet_GetStringPtr (m_pRecvPket, nOffset);

	classUSER *pUser = g_pUserLIST->Find_ACCOUNT (szAccount);
	if ( pUser ) {
		pUser->Send_gsv_BILLING_MESSAGE( m_pRecvPket->m_zas_MESSAGE.m_btTYPE, szMsg );
	}
	return true;
}
bool GS_asvSOCKET::Recv_zas_MESSAGE_EXT ()
{
	short nOffset   = sizeof( zas_MESSAGE_EXT );
	char *szAccount = Packet_GetStringPtr (m_pRecvPket, nOffset);
	char *szMsg     = Packet_GetStringPtr (m_pRecvPket, nOffset);

	classUSER *pUser = g_pUserLIST->Find_ACCOUNT (szAccount);
	if ( pUser ) {
		switch( m_pRecvPket->m_zas_MESSAGE_EXT.m_wMsgTYPE ) {
			case EXT_BILLING_MSG_PAY_KOREA	:
			case EXT_BILLING_MSG_PAY_JAPAN	:
			case EXT_BILLING_MSG_PAY_TAIWAN	:
				pUser->m_dwPayFLAG = m_pRecvPket->m_zas_MESSAGE_EXT.m_dwPlayingFlag[0];
				break;
			case EXT_BILLING_MSG_TYPE_TIME_EXPIRED :
				pUser->m_dwPayFLAG &= ~m_pRecvPket->m_zas_MESSAGE_EXT.m_dwPlayingFlag[0];
				break;
		}
		pUser->Send_gsv_BILLING_MESSAGE_EXT( 
				m_pRecvPket->m_zas_MESSAGE_EXT.m_wMsgTYPE, 
				m_pRecvPket->m_zas_MESSAGE_EXT.m_dwPayType,
				m_pRecvPket->m_zas_MESSAGE_EXT.m_dwPlayingFlag,
				szMsg );
	}
	return true;
}

bool GS_asvSOCKET::Recv_zas_CHECK_ACCOUNT ()
{
	short nOffset   = sizeof( zas_CHECK_ACCOUNT );
	char *szAccount = Packet_GetStringPtr (m_pRecvPket, nOffset);

	switch( m_pRecvPket->m_zas_CHECK_ACCOUNT.m_btType ) {
		case ZAS_CHECK_DISCONNECT :
		{
			// Ŭ���̾�Ʈ���� Keep alive�ȿͼ� Gums���� Ÿ�Ӿƿ� �ɷȴ�...
			// ���ӻ� ������ �ִ� �����̴�... ���ӵ� �������� ��Ŷ ��������...
			// gums�� �ٽ� �α� ��û..
			bool bAlive=false;
			classUSER *pUser = g_pUserLIST->Find_ACCOUNT (szAccount);
			if ( pUser ) {
				classPACKET *pCPacket = Packet_AllocNLock ();
				if ( pCPacket ) {
					// ��� �ִ��� ��Ŷ�� ���� ����...
					pCPacket->m_HEADER.m_wType = SRV_ERROR;
					pCPacket->m_HEADER.m_nSize = sizeof( gsv_ERROR );
					pCPacket->m_gsv_ERROR.m_wErrorCODE = 0;
					bAlive = pUser->Send_Start( pCPacket );
					Packet_ReleaseNUnlock( pCPacket );
				}
			}
			
			if ( bAlive ) {
				//AS�� ������ ���� �ֱ⶧����.... �ٷ� Send_zws_ADD_ACCOUNT �ȵǳ�..
				this->Send_zas_CHECK_ACCOUNT( ZAS_CHECK_RELOGIN, szAccount );
			} else {
				this->Send_zas_SUB_ACCOUNT( szAccount, 0xff, -1, -1 );
			}
			break;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------------
bool GS_asvSOCKET::Send_cli_ALIVE ( char *szAccount )
{
	// GUMS�� alive ��Ŷ ����.
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;
	pCPacket->m_HEADER.m_wType	= CLI_ALIVE;
	pCPacket->m_HEADER.m_nSize	= sizeof( t_PACKETHEADER );
	pCPacket->AppendString( szAccount );

	m_SockASV.Packet_Register2SendQ( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}
bool GS_asvSOCKET::Send_zas_SERVER_TYPE ( BYTE btType )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType	= ZAS_SERVER_TYPE;
	pCPacket->m_HEADER.m_nSize	= sizeof( zas_SERVER_TYPE );

	pCPacket->m_zas_SERVER_TYPE.m_btServerTYPE = btType;

	m_SockASV.Packet_Register2SendQ( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
bool GS_asvSOCKET::Proc_SocketMSG (WPARAM wParam, LPARAM lParam)
{
	int nErrorCode = WSAGETSELECTERROR(lParam);
	switch ( WSAGETSELECTEVENT(lParam) ) {
		case FD_READ:
		{
			m_SockASV.OnReceive( nErrorCode );

			// ���� ��Ŷ ó��..
			while( m_SockASV.Peek_Packet( m_pRecvPket, true ) ) {
				// LogString( LOG_DEBUG, "Handle LS Packet: Type[ 0x%x ], Size[ %d ]\n", m_pRecvPket->m_HEADER.m_wType, m_pRecvPket->m_HEADER.m_nSize);
				switch( m_pRecvPket->m_HEADER.m_wType ) {
				/*
					case LSV_CHECK_ALIVE :
						Recv_lsv_CHECK_ALIVE ();
						break;
				*/
					case ZAS_CHECK_ACCOUNT :
						Recv_zas_CHECK_ACCOUNT ();
						break;

					case ZAS_MESSAGE :
						Recv_zas_MESSAGE ();
						break;

					case ZAS_MESSAGE_EXT :
						Recv_zas_MESSAGE_EXT ();
						break;

					case ZAS_KICK_ACCOUNT :
						Recv_zas_KICK_ACCOUNT ();
						break;

					default :
						g_LOG.CS_ODS( 0xffff, "Undefined Account server packet... Type[ 0x%x ], Size[ %d ]\n", m_pRecvPket->m_HEADER.m_wType, m_pRecvPket->m_HEADER.m_nSize);
				}
			}
			break;
		}
		case FD_WRITE:
			m_SockASV.OnSend ( nErrorCode );
			break;

		case FD_CONNECT: 
		{
			m_SockASV.OnConnect ( nErrorCode );
			if ( !nErrorCode ) {
				g_LOG.CS_ODS( 0xffff, "Connected to ACCOUNT server \n");

				m_bTryCONN = false;

				if ( m_pReconnectTimer )
					m_pReconnectTimer->Stop ();

				// �׽�Ʈ ������ ???
				if ( CLIB_GameSRV::GetInstance()->IsTestServer() ) {
					this->Send_zas_SERVER_TYPE( 1 );
				}
				g_pUserLIST->Send_zws_ACCOUNT_LIST( &this->m_SockASV, true );
 			} else {
				if ( m_pReconnectTimer )
					m_pReconnectTimer->Start ();
				if ( !m_bTryCONN ) {
					m_bTryCONN = true;
					g_LOG.CS_ODS( 0xffff, "Connect failed to ACCOUNT server \n");
				}
			}
			break;
		}
		case FD_CLOSE:		// Close()�Լ��� ȣ���ؼ� ����ɶ��� �߻� ���Ѵ�.
		{
			m_SockASV.OnClose ( nErrorCode );

			g_LOG.CS_ODS( 0xffff, "Disconnected form ACCOUNT server \n");

			if ( m_pReconnectTimer )
				m_pReconnectTimer->Start ();
		}
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
