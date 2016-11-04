#ifndef	__CLS_CLIENT_H
#define	__CLS_CLIENT_H
#include "SHO_LS_LIB.h"
//-------------------------------------------------------------------------------------------------

#define CLIENT_STEP_CONNECT     0
#define	CLIENT_STEP_LOGIN_WAIT	1
#define CLIENT_STEP_LOGIN_REQ	2
#define CLIENT_STEP_LOGEDIN     3
#define CLIENT_STEP_SELECT_GS   4

extern CPacketCODEC	*g_pPacketCODEC;

//-------------------------------------------------------------------------------------------------
#define	__VIEW_ACCOUNT

class CLS_Client : public iocpSOCKET
{
private:
    bool    Recv_cli_ACCEPT_REQ ();
    bool    Recv_cli_LOGIN_REQ ( t_PACKET *pPacket );
	bool	Recv_cli_CHANNEL_LIST_REQ ( t_PACKET *pPacket );
    bool    Recv_cli_SELECT_SERVER ( t_PACKET *pPacket );
    
	bool	Recv_mon_SERVER_LIST_REQ( t_PACKET *pPacket, bool bHideIP );
	bool	Recv_mon_SERVER_STATUS_REQ( t_PACKET *pPacket );

public :
    bool    Send_srv_ANNOUNCE_TEXT (char *szText);



public :

	CStrVAR		m_Account;				/// ����
    DWORD       m_dwMD5Pass[ 8 ];		/// ���
    DWORD       m_dwLastLoginTIME;		/// ������ ���ӽð�

    DWORD       m_dwRIGHT;				/// ������/GM��... ���ӳ����� ġƮ�Ǵ� ���� ����
	DWORD		m_dwPayFLAG;			/// ���� ������ �÷���

    short       m_nProcSTEP;
	char		m_pJuMinNO[ 9 ];		/// �ֹι�ȣ ���ڸ�~���ڸ� 1��°
	bool		m_bFreeServerOnly;		/// ������������ ���� �������� ����...

	DWORD		m_dwConnTIME;			/// ���ӽð�
	bool		m_bMonClient;			/// ����͸� Ŭ���̾�Ʈ�ΰ� ??
	bool		m_bAdmClient;			/// Raven - Server Admin Tool

	CDLList< CLS_Client* >::tagNODE *m_pConnNODE;

#ifdef	__VIEW_ACCOUNT
	void	   *m_pCliListITEM;
#endif

	CLS_Client ();
	~CLS_Client ();

	void Init ()
	{
		m_bFreeServerOnly = false;
		m_iRecvSeqNO = 0;
		m_pConnNODE = NULL;

		m_bMonClient = false;
		m_bAdmClient = false;

		// m_pListITEM = NULL;
		m_nProcSTEP = CLIENT_STEP_CONNECT;
		m_dwRIGHT = 0;
		m_dwPayFLAG = 0;

		m_dwConnTIME = classTIME::GetCurrentAbsSecond();

#ifdef	__VIEW_ACCOUNT
		m_pCliListITEM = NULL;
#endif
	}
	void Free ()
	{
		// _ASSERT( 0 == this->m_iSocketIDX );	������.... IROSE���� �߻�...
		this->Clear_LIST ();
	}

	void Set_ACCOUNT(char *szAccount, DWORD *pMD5Pass)
	{
#ifdef	__VIEW_ACCOUNT
		this->LockSOCKET ();
		if ( m_pCliListITEM ) {
			SHO_LS::ExeAPI()->SetListItemSTR( m_pCliListITEM, -1, szAccount );
		}
		this->UnlockSOCKET ();
#endif
		m_Account.Set( szAccount );
        ::CopyMemory( m_dwMD5Pass, pMD5Pass, sizeof(DWORD)*8 );
	}

	bool Send_lsv_LOGIN_REPLY(BYTE btResult, int iPayType);

private :
	//----------------------- virtual function inherit from iocpSOCKET
    bool HandlePACKET( t_PACKETHEADER *pPacket );

    int     m_iRecvSeqNO;

	WORD  E_SendP (t_PACKETHEADER *pPacket)
	{
		return g_pPacketCODEC->Encode_SendServerPACKET( pPacket );
	}
	WORD  D_RecvH (t_PACKETHEADER *pPacket)
	{
		return g_pPacketCODEC->Decode_RecvClientHEADER( pPacket, m_iRecvSeqNO );
	}
	short D_RecvB (t_PACKETHEADER *pPacket)
	{
		return g_pPacketCODEC->Decode_RecvClientBODY( pPacket );
	}
	WORD  P_Length(t_PACKETHEADER *pPacket)
	{	
		return g_pPacketCODEC->GetDecodedPacketLength( pPacket );
	}
} ;


//-------------------------------------------------------------------------------------------------
/// ���ӵ� ��ü Ŭ���̾�Ʈ ����Ʈ
class CLS_ListCLIENT : public IOCPSocketSERVER, public CDataPOOL< CLS_Client >
{
public:
	int						m_iLimitUserCNT;
	CCriticalSection		m_CS;
	CDLList< CLS_Client* >	m_ConnLIST;

public :
	// Worker Thread ���� = CPU * 2 + 2
	CLS_ListCLIENT( UINT uiInitDataCNT, UINT uiIncDataCNT );
	~CLS_ListCLIENT()
	{
		;
	}

	bool Send_lsv_LOGIN_REPLY (int iSocketIDX, BYTE btResult, int iPayType=0);
	void SetLimitUserCNT( int iLimitUserCNT )
	{
		m_iLimitUserCNT = iLimitUserCNT;
	}

	void Delete_IdleSOCKET ()
	{
		DWORD dwConnTIME = classTIME::GetCurrentAbsSecond();

		CDLList< CLS_Client* >::tagNODE *pNode;
		this->m_CS.Lock ();
		pNode = m_ConnLIST.GetHeadNode ();
		while( pNode ) {
			switch( pNode->m_VALUE->m_nProcSTEP ) {
				case CLIENT_STEP_CONNECT :	
					// �������� ���� ������ CLI_ACCEPT_REQ�� 60�� �ȿ� �Ⱥ����� ©��~~~
					if ( dwConnTIME - pNode->m_VALUE->m_dwConnTIME > 60 ) {
						pNode->m_VALUE->CloseSocket();
						LogString (LOG_NORMAL, "1 Min TimeOut[%s]\n", pNode->m_VALUE->Get_IP() );
					}
					break;
				case CLIENT_STEP_LOGEDIN :	// �α��� �� ����...
					if ( !pNode->m_VALUE->m_bMonClient && dwConnTIME - pNode->m_VALUE->m_dwConnTIME > 10*60 ) {
						// 10�е��� �������� �̵����� ���� ����...
						pNode->m_VALUE->CloseSocket ();
						//g_LOG.CS_ODS (LOG_NORMAL, "10 Min TimeOut[%s]\n", pNode->m_VALUE->Get_IP() );
					}
					break;
				default :					
					if ( !pNode->m_VALUE->m_bMonClient && dwConnTIME - pNode->m_VALUE->m_dwConnTIME > 5*60 ) {
						// 5�е��� �α��� �ȵ� ����...
						pNode->m_VALUE->CloseSocket ();
						//g_LOG.CS_ODS (LOG_NORMAL, "5 Min TimeOut[%s]\n", pNode->m_VALUE->Get_IP() );
					}
					break;
			}
			pNode = pNode->GetNext ();
		}
		this->m_CS.Unlock ();
	}
	//-------------------------------------------------------------------------------------------------
	/// Inherited from IOCPSocketSERVER
	iocpSOCKET*	AllocClientSOCKET()
	{
		// �޸��Ҵ�
		CLS_Client *pSOCKET = this->Pool_Alloc ();
		if ( pSOCKET ) {
			pSOCKET->Init ();

			this->m_CS.Lock ();
				pSOCKET->m_pConnNODE = m_ConnLIST.AllocNAppend( pSOCKET );
			this->m_CS.Unlock ();
		}
		return (iocpSOCKET*)pSOCKET;
	}
	void InitClientSOCKET( iocpSOCKET *pCLIENT );
	void FreeClientSOCKET( iocpSOCKET *pSOCKET )
	{
		CLS_Client *pClient = (CLS_Client*)pSOCKET;

		this->m_CS.Lock ();
			m_ConnLIST.DeleteNFree( pClient->m_pConnNODE );
		this->m_CS.Unlock ();

#ifdef	__VIEW_ACCOUNT
		pClient->LockSOCKET();
		if ( pClient->m_pCliListITEM ) {
			SHO_LS::ExeAPI()->DelConnectorITEM( pClient->m_pCliListITEM );
			pClient->m_pCliListITEM = NULL;
		}
		pClient->UnlockSOCKET();
#endif
		//LogString (LOG_DEBUG, "    >>> Delete CLIENT socket : %s, %s UsedCnt: %d \n", 
		//				pSOCKET->m_IP.Get(), 
		//				this->GetPoolNAME(), 
		//				this->GetUsedCNT()-1 );
		// �������� �޸� ����
		pClient->Free ();
		this->Pool_Free( pClient );
	}
	void ClosedClientSOCKET( iocpSOCKET *pSOCKET )
	{
		this->FreeClientSOCKET( pSOCKET );
		// ������ �����ƴ�.. �˾Ƽ� �޸� �����Ұ�...
	}

	bool IsMaxiumUSER ()
	{
		if ( m_iLimitUserCNT && this->GetUsedSocketCNT() > m_iLimitUserCNT )
			return true;

		return false;
	}
} ;
extern CLS_ListCLIENT   *g_pListCLIENT;

//-------------------------------------------------------------------------------------------------
#endif