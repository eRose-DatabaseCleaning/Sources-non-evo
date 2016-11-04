
#include "stdAFX.h"
#if defined(__SHO_WS) || defined(__INC_WORLD)


#ifdef	__INC_WORLD
#include "LIB_gsMAIN.h"
#include "GS_ListUSER.h"
#endif

#ifdef	__SHO_WS
#include "CWS_Client.h"
#endif

#include "CThreadMSGR.h"
#include "classLOG.h"

//-------------------------------------------------------------------------------------------------
CThreadMSGR::CThreadMSGR() : CSqlTHREAD( true )//, m_csListCMD( 4000 )
{
	m_pListBUFF = new BYTE[ 2048 ];
}
CThreadMSGR::~CThreadMSGR ()
{
	SAFE_DELETE_ARRAY( m_pListBUFF );
}


//-------------------------------------------------------------------------------------------------
void CThreadMSGR::Execute ()
{
	CDLList< tagMSGR_CMD >::tagNODE *pCmdNODE;

	g_LOG.CS_ODS( 0xffff, ">  > >> CThreadMSGR::Execute() ThreadID: %d(0x%x)\n", this->ThreadID, this->ThreadID );

    while( TRUE ) {
		if ( !this->Terminated ) {
			m_pEVENT->WaitFor( INFINITE );
		} else {
			int iReaminCNT;
			this->m_CS.Lock ();
				iReaminCNT = m_WaitCMD.GetNodeCount();
			this->m_CS.Unlock ();

			if ( iReaminCNT <= 0 )
				break;
		}

		this->m_CS.Lock ();
		m_ProcCMD.AppendNodeList( &m_WaitCMD );
		m_WaitCMD.Init ();
		m_pEVENT->ResetEvent ();
		this->m_CS.Unlock ();

		for( pCmdNODE = m_ProcCMD.GetHeadNode(); pCmdNODE; ) {
			this->Run_MessengerPACKET( &pCmdNODE->m_VALUE );

			SAFE_DELETE_ARRAY( pCmdNODE->m_VALUE.m_pPacket );
			m_ProcCMD.DeleteNFree( pCmdNODE );
			pCmdNODE = m_ProcCMD.GetHeadNode();
		}
	}

	int iCnt = m_AddPACKET.GetNodeCount();
	_ASSERT( iCnt == 0 );

	g_LOG.CS_ODS( 0xffff, "<<<< CWS_ThreadSQL::Execute() ThreadID: %d(0x%x)\n", this->ThreadID, this->ThreadID );
}


//-------------------------------------------------------------------------------------------------
void CThreadMSGR::Add_MessengerCMD (CWS_Client *pOwner, BYTE btCMD, t_PACKET *pPacket)
{
	CDLList< tagMSGR_CMD >::tagNODE *pNewNODE;

	pNewNODE = new CDLList< tagMSGR_CMD >::tagNODE;

	pNewNODE->m_VALUE.m_iSocketIDX	= pOwner->m_iSocketIDX;
	pNewNODE->m_VALUE.m_pMSGR		= pOwner->m_pMessenger;
	pNewNODE->m_VALUE.m_btCMD		= btCMD;
	if ( pPacket ) {
		pNewNODE->m_VALUE.m_pPacket	= (t_PACKET*) new BYTE[ pPacket->m_HEADER.m_nSize ];
		::CopyMemory( pNewNODE->m_VALUE.m_pPacket, pPacket, pPacket->m_HEADER.m_nSize );
	} else
		pNewNODE->m_VALUE.m_pPacket	= NULL;

	this->m_CS.Lock ();
	m_WaitCMD.AppendNode( pNewNODE );
	this->m_CS.Unlock ();

	this->Set_EVENT ();
}


//-------------------------------------------------------------------------------------------------
bool CThreadMSGR::Run_MessengerPACKET( tagMSGR_CMD *pMsgCMD )
{
	switch( pMsgCMD->m_btCMD ) {
		case MSGR_CMD_LOGIN			:
			this->LogIN( pMsgCMD->m_iSocketIDX );
			return true;
		case MSGR_CMD_LOGOUT		:
			this->LogOUT( pMsgCMD->m_pMSGR );
			return true;

		case MSGR_CMD_APPEND_ACCEPT	:
		{
		#ifdef __SHO_WS
			CWS_Client *pDestUSER = (CWS_Client*)g_pListCLIENT->GetSOCKET( pMsgCMD->m_pPacket->m_cli_MCMD_APPEND_REPLY.m_wUserIDX );
		#else
			CWS_Client *pDestUSER = (CWS_Client*)g_pUserLIST->GetSOCKET( pMsgCMD->m_pPacket->m_cli_MCMD_APPEND_REPLY.m_wUserIDX );
		#endif
			if ( pDestUSER && pDestUSER->m_pMessenger ) {
				pMsgCMD->m_pMSGR->MSGR_Add( pDestUSER->m_pMessenger );
				pDestUSER->m_pMessenger->MSGR_Add( pMsgCMD->m_pMSGR );
			}
			return true;
		}

		case MSGR_CMD_DELETE		:	
			pMsgCMD->m_pMSGR->MSGR_Del( pMsgCMD->m_pPacket->m_cli_MCMD_TAG.m_dwUserTAG );
			return true;

		case MSGR_CMD_REFUSE		:
			pMsgCMD->m_pMSGR->MSGR_Ban( pMsgCMD->m_pPacket->m_cli_MCMD_TAG.m_dwUserTAG );
			return true;

		case MSGR_CMD_CHANGE_STATUS	:
			// ������ ����... ��ϵ� ��� ģ������...
			pMsgCMD->m_pPacket->m_cli_MCMD_STATUS_REQ.m_btStatus;
			break;
		case 0x0ff :
			pMsgCMD->m_pMSGR->MSGR_Msg( pMsgCMD->m_pPacket );
			return true;
	}

	return false;
}


//-------------------------------------------------------------------------------------------------
bool CThreadMSGR::LogIN (int iSocketIDX)
{
	CWS_Client *pClient;

	#ifdef	__SHO_GS
	pClient = (CWS_Client *)g_pUserLIST->GetSOCKET( iSocketIDX );
	#endif
	#ifdef	__SHO_WS
	pClient = (CWS_Client *)g_pListCLIENT->GetSOCKET( iSocketIDX );
	#endif
	if ( NULL == pClient )
		return false;

	DWORD dwDBID = pClient->m_dwDBID;
	if ( !this->m_pSQL->QuerySQL( "SELECT intFriendCNT, blobFRIENDS FROM tblWS_FRIEND WHERE intCharID=%d", dwDBID ) ) {
		g_LOG.CS_ODS(LOG_NORMAL, "Query ERROR:: %s \n", m_pSQL->GetERROR() );
		return false;
	}

	CMessenger *pMSGR = new CMessenger( dwDBID, pClient );
	if ( !this->m_pSQL->GetNextRECORD() ) {
		// insert !!!
		if ( this->m_pSQL->ExecSQL("INSERT tblWS_FRIEND ( intCharID ) VALUES(%d);", dwDBID ) < 1 ) {
			g_LOG.CS_ODS(LOG_NORMAL, "SQL Exec ERROR:: INSERT %s friend : %s \n", pClient->Get_NAME(), m_pSQL->GetERROR() );
			delete pMSGR;
			return true;
		}
		// ģ�� ����.
	} else {
		int iFriendCNT = this->m_pSQL->GetInteger( 0 );
		if ( iFriendCNT > 0 ) {
			BYTE *pDATA = this->m_pSQL->GetDataPTR( 1 );
			pMSGR->MSGR_LogIN( iFriendCNT, pDATA );
		} 
	}

	pMSGR->m_btMsgrSTATUS  = FRIEND_STATUS_ONLINE;
	pClient->m_pMessenger = pMSGR;

	return true;
}

//-------------------------------------------------------------------------------------------------
void CThreadMSGR::LogOUT (CMessenger *pMSGR)
{
	// update db...
	int iFriendCNT = pMSGR->Get_FriendCNT();
	int iBuffLEN = pMSGR->MSGR_LogOUT( this->m_pListBUFF );

	if ( pMSGR->MSGR_IsUPDATE() ) {
		if ( iFriendCNT > 0 ) {
			this->m_pSQL->BindPARAM( 1, this->m_pListBUFF, iBuffLEN );
			this->m_pSQL->MakeQuery( "UPDATE tblWS_FRIEND SET blobFRIENDS=",
															MQ_PARAM_BINDIDX,	1,
					MQ_PARAM_ADDSTR, ",intFriendCNT=",		MQ_PARAM_INT,	iFriendCNT,
					MQ_PARAM_ADDSTR, "WHERE intCharID=",	MQ_PARAM_INT,	pMSGR->Get_DBID(),
															MQ_PARAM_END );
		} else {
			this->m_pSQL->MakeQuery( "UPDATE tblWS_FRIEND SET blobFRIENDS=NULL",
					MQ_PARAM_ADDSTR, ",intFriendCNT=",		MQ_PARAM_INT,	iFriendCNT,
					MQ_PARAM_ADDSTR, "WHERE intCharID=",	MQ_PARAM_INT,	pMSGR->Get_DBID(),
															MQ_PARAM_END );
		}
		if ( this->m_pSQL->ExecSQLBuffer() < 0 ) {
			// ��ġ�� ���� !!!
			g_LOG.CS_ODS(LOG_NORMAL, "SQL Exec ERROR:: UPDATE messenger:%d %s \n", pMSGR->Get_DBID(), m_pSQL->GetERROR() );
		}
	}

	SAFE_DELETE( pMSGR );
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void CMessenger::MSGR_LogIN (int iCount, BYTE *pLIST)
{
	classPACKET *pToFR = Packet_AllocNLock ();
	classPACKET *pToME = Packet_AllocNLock ();

	pToFR->m_HEADER.m_wType = WSV_MESSENGER;
	pToFR->m_HEADER.m_nSize = sizeof( wsv_MCMD_STATUS_REPLY );

	pToFR->m_wsv_MCMD_STATUS_REPLY.m_btCMD = MSGR_CMD_CHANGE_STATUS;
	pToFR->m_wsv_MCMD_STATUS_REPLY.m_dwUserTAG = this->Get_DBID ();
	pToFR->m_wsv_MCMD_STATUS_REPLY.m_btStatus  = FRIEND_STATUS_ONLINE;

	
	pToME->m_HEADER.m_wType = WSV_MESSENGER;
	pToME->m_HEADER.m_nSize = sizeof( wsv_MCMD_LIST );

	pToME->m_wsv_MCMD_LIST.m_btCMD = MSGR_CMD_LIST;
	pToME->m_wsv_MCMD_LIST.m_btFriendCNT = iCount;
	
	tagFriend_H sFrStatus;

	char *pName;
	tagFriend_H *pFrH;

	CWS_Client *pClient;
	CDLList< tagMyFriend >::tagNODE *pNODE;
	for (int iC=0; iC<iCount; iC++) {
		pFrH = (tagFriend_H*)pLIST;
		pName = (char*)( pLIST+sizeof(tagFriend_H) );

		pNODE = m_ListFRIEND.AllocNAppend();

		pNODE->m_VALUE.m_dwDBID   = pFrH->m_dwDBID;
		pNODE->m_VALUE.m_btSTATUS = pFrH->m_btSTATUS;
		pNODE->m_VALUE.m_Name.Set( pName );

		pLIST += ( sizeof(tagFriend_H)+pNODE->m_VALUE.m_Name.BuffLength()+1 );

		// ���� �α׾ƿ� �߿� ģ���� ���� ������ ���...
		// ���� �÷��װ� �������� �ʾ� ģ���� �� ����Ʈ�� ����ߴµ�...
		// ģ���� �����鼭 ������ �α׾ƿ��� �������� �ʾ� �� ��Ͽ� �α��� �����̸�,
		// ���� ģ���� ���� �α׾ƿ��ϰ�, ���� ģ���� �����Ұ�� ��~~~~
		if ( !(FRIEND_STATUS_REFUSED & pNODE->m_VALUE.m_btSTATUS) ) {
			#ifdef	__SHO_GS
			pClient = g_pUserLIST->Find_CHAR( pName );
			#endif
			#ifdef	__SHO_WS
			pClient = g_pListCLIENT->Find_CHAR( pName );
			#endif

			if ( pClient && pClient->m_pMessenger ) {
				// ģ���� ��Ͽ� ���� �α�Ȱɷ� ���� �������� ���¸� �����Ѵ�.
				pNODE->m_VALUE.m_btSTATUS = pClient->m_pMessenger->MSGR_OnOffLine( pToFR, this, this->Get_DBID(), FRIEND_STATUS_ONLINE );
				if ( FRIEND_STATUS_DELETED != pNODE->m_VALUE.m_btSTATUS ) {
					// 2004. 7. 22 ���� �������� �ʾҴٸ�... ���� ���� �����ߴٸ� ���� �α׾ƿ� �ϸ鼭
					// ������ �뺸�� ���ϰ�, ���� ���� ������ ��~~~~�߻�
					pNODE->m_VALUE.m_pFRIEND  = pClient->m_pMessenger;
				}
			} else {
				pNODE->m_VALUE.m_btSTATUS = FRIEND_STATUS_OFFLINE;
			}
		} /* else {
			// ���� �����ϰų� ���� �����ѳ��̱� ���� �� ���� �뺸�� �ʿ����.
			// ���� �̳��� �α׾ƿ� ���·� ���̰���...
		} */

		sFrStatus.m_dwDBID   = pNODE->m_VALUE.m_dwDBID;
		sFrStatus.m_btSTATUS = pNODE->m_VALUE.m_btSTATUS;

		COMPILE_TIME_ASSERT( sizeof( tagFriend_H ) == 5 );

		pToME->AppendData( &sFrStatus, sizeof( tagFriend_H ) );
		pToME->AppendString( pName );
	}

	this->m_pOWNER->SendPacket( pToME );

	Packet_ReleaseNUnlock( pToME );
	Packet_ReleaseNUnlock( pToFR );
}


//-------------------------------------------------------------------------------------------------
/*
2. �α׾ƿ���
	. ���� ����� ģ������Ʈ ��ȸ..
		NODE->m_VALUE.m_pUSER != NULL �̸� �α׾ƿ� ����.
*/
int CMessenger::MSGR_LogOUT (BYTE *pOutBUFF)
{
	classPACKET *pCPacket = Packet_AllocNLock ();

	pCPacket->m_HEADER.m_wType = WSV_MESSENGER;
	pCPacket->m_HEADER.m_nSize = sizeof( wsv_MCMD_STATUS_REPLY );

	pCPacket->m_wsv_MCMD_STATUS_REPLY.m_btCMD = MSGR_CMD_CHANGE_STATUS;
	pCPacket->m_wsv_MCMD_STATUS_REPLY.m_dwUserTAG = this->Get_DBID ();
	pCPacket->m_wsv_MCMD_STATUS_REPLY.m_btStatus  = FRIEND_STATUS_OFFLINE;


	CDLList< tagMyFriend >::tagNODE *pNODE;
	pNODE = m_ListFRIEND.GetHeadNode();

	int iBuffLEN = 0;
	tagFriend_H *pFR;

	while( pNODE ) {
		pFR = (tagFriend_H*)&pOutBUFF[ iBuffLEN ];

		pFR->m_btSTATUS = pNODE->m_VALUE.m_btSTATUS;
		pFR->m_dwDBID   = pNODE->m_VALUE.m_dwDBID;

		iBuffLEN += sizeof( tagFriend_H );
		::CopyMemory( &pOutBUFF[ iBuffLEN ], pNODE->m_VALUE.m_Name.Get(), pNODE->m_VALUE.m_Name.BuffLength() );
		iBuffLEN += pNODE->m_VALUE.m_Name.BuffLength();
		pOutBUFF[ iBuffLEN++ ] = 0;

		if ( !(FRIEND_STATUS_REFUSED & pNODE->m_VALUE.m_btSTATUS) ) {
			if ( pNODE->m_VALUE.m_pFRIEND ) {	// 2007. 7. 17 �Ʒ� �Լ����� ���ѷ���... NULL == pNODE->m_VALUE.m_pFRIEND�Ǿ� �Ǵ� ��Ȳ����..
				pNODE->m_VALUE.m_pFRIEND->MSGR_OnOffLine( pCPacket, NULL, this->Get_DBID(), FRIEND_STATUS_OFFLINE );
			}
		}

		m_ListFRIEND.DeleteNFree( pNODE );
		pNODE = m_ListFRIEND.GetHeadNode();
	}

	Packet_ReleaseNUnlock( pCPacket );

	return iBuffLEN;
}


bool CMessenger::MSGR_Add( CMessenger *pFriend )
{
	CDLList< tagMyFriend >::tagNODE *pNODE;

	pNODE = m_ListFRIEND.GetHeadNode ();
	while( pNODE ) {
		if ( pNODE->m_VALUE.m_dwDBID == pFriend->Get_DBID() ) {
			// �̹� ��ϵ� ����ڴ�... �����Ǿ������� �ٽ� �߰��� ���..
			pNODE->m_VALUE.m_btSTATUS   = pFriend->m_btMsgrSTATUS;
			pNODE->m_VALUE.m_pFRIEND	= pFriend;
			return true;
		}
		pNODE = pNODE->GetNext();
	}

	if ( pFriend->m_pOWNER && m_ListFRIEND.GetNodeCount() < MAX_FRIEND_COUNT ) {
		this->m_bFriendUPDATE = true;
		pNODE = m_ListFRIEND.AllocNAppend();
		pNODE->m_VALUE.m_btSTATUS   = pFriend->m_btMsgrSTATUS;
		pNODE->m_VALUE.m_dwDBID		= pFriend->Get_DBID();
		pNODE->m_VALUE.m_pFRIEND	= pFriend;
		pNODE->m_VALUE.m_Name.Set( pFriend->m_pOWNER->Get_NAME() );

		classPACKET *pCPacket = Packet_AllocNLock ();

		pCPacket->m_HEADER.m_wType = WSV_MESSENGER;
		pCPacket->m_HEADER.m_nSize = sizeof( wsv_MCMD_APPEND_ACCEPT );

		pCPacket->m_wsv_MCMD_APPEND_ACCEPT.m_btCMD	   = MSGR_CMD_APPEND_ACCEPT;
		pCPacket->m_wsv_MCMD_APPEND_ACCEPT.m_dwUserTAG = pNODE->m_VALUE.m_dwDBID;
		pCPacket->m_wsv_MCMD_APPEND_ACCEPT.m_btStatus  = pNODE->m_VALUE.m_btSTATUS;
		pCPacket->AppendString( pNODE->m_VALUE.m_Name.Get() );

		this->m_pOWNER->SendPacket( pCPacket );

		Packet_ReleaseNUnlock( pCPacket );
		return true;
	}
	return false;	
}

void CMessenger::MSGR_Del( DWORD dwDBID )
{
	CDLList< tagMyFriend >::tagNODE *pNODE;

	pNODE = m_ListFRIEND.GetHeadNode ();
	while( pNODE ) {
		if ( dwDBID == pNODE->m_VALUE.m_dwDBID ) {
			// "���ʴ� �˾Ƽ� �����϶��...
			if ( pNODE->m_VALUE.m_pFRIEND ) {
				// "����� �����Ȼ��·� �����ϰ� �ݵ�� m_pFRIEND = NULL�� ������...... " )
				pNODE->m_VALUE.m_pFRIEND->MSGR_Status2ONE( this->Get_DBID(), FRIEND_STATUS_DELETED, NULL );
			}

			m_ListFRIEND.DeleteNFree( pNODE );
			this->m_bFriendUPDATE = true;
			break;
		}
		pNODE = pNODE->GetNext ();
	}
}

void CMessenger::MSGR_Ban( DWORD dwDBID )
{
	CDLList< tagMyFriend >::tagNODE *pNODE;

	pNODE = m_ListFRIEND.GetHeadNode ();
	while( pNODE ) {
		if ( dwDBID == pNODE->m_VALUE.m_dwDBID ) {
			// �̳��� �޼��� ����..
			if ( FRIEND_STATUS_DELETED != pNODE->m_VALUE.m_btSTATUS ) {
				if ( FRIEND_STATUS_REFUSED & pNODE->m_VALUE.m_btSTATUS ) {
					if ( pNODE->m_VALUE.m_pFRIEND )
						pNODE->m_VALUE.m_btSTATUS = pNODE->m_VALUE.m_pFRIEND->m_btMsgrSTATUS;
					else
						pNODE->m_VALUE.m_btSTATUS = FRIEND_STATUS_OFFLINE;
				} else
					pNODE->m_VALUE.m_btSTATUS = FRIEND_STATUS_REFUSED;
			}

			#pragma COMPILE_TIME_MSG ( " �ٲ� ���� ����??? " )

			break;
		}
		pNODE = pNODE->GetNext ();
	}
}

//-------------------------------------------------------------------------------------------------
BYTE CMessenger::MSGR_OnOffLine ( classPACKET *pCPacket, CMessenger *pFriend, DWORD dwDBID, BYTE btNewStatus )
{
	CDLList< tagMyFriend >::tagNODE *pNODE;

	pNODE = m_ListFRIEND.GetHeadNode ();
	while( pNODE ) {
		if ( dwDBID == pNODE->m_VALUE.m_dwDBID ) {	// 2004. 7. 22 LogOUT->MSGR_LogOUT->MSGR_OnOffLine ��~
			pNODE->m_VALUE.m_pFRIEND = pFriend;

			if ( !( FRIEND_STATUS_REFUSED & pNODE->m_VALUE.m_btSTATUS ) ) {
				// ���� �ź��߰ų�, ���� ������ ���̾ƴϸ�...
				pNODE->m_VALUE.m_btSTATUS = btNewStatus;
				this->m_pOWNER->SendPacket( pCPacket );
				return this->m_btMsgrSTATUS;
			}
			
			return FRIEND_STATUS_OFFLINE;
		}
		pNODE = pNODE->GetNext ();
	}

	return FRIEND_STATUS_DELETED;
}


//-------------------------------------------------------------------------------------------------
void CMessenger::MSGR_Status2ALL( BYTE btNewStatus )
{
	classPACKET *pCPacket = Packet_AllocNLock ();

	pCPacket->m_HEADER.m_wType = WSV_MESSENGER;
	pCPacket->m_HEADER.m_nSize = sizeof( wsv_MCMD_STATUS_REPLY );

	pCPacket->m_wsv_MCMD_STATUS_REPLY.m_btCMD = MSGR_CMD_CHANGE_STATUS;
	pCPacket->m_wsv_MCMD_STATUS_REPLY.m_dwUserTAG = this->Get_DBID ();
	pCPacket->m_wsv_MCMD_STATUS_REPLY.m_btStatus  = btNewStatus;

	CDLList< tagMyFriend >::tagNODE *pNODE;

	pNODE = m_ListFRIEND.GetHeadNode ();
	while( pNODE ) {
		if ( pNODE->m_VALUE.m_pFRIEND && pNODE->m_VALUE.m_pFRIEND->m_pOWNER ) {
			pNODE->m_VALUE.m_pFRIEND->m_pOWNER->SendPacket( pCPacket );
		}
		pNODE = pNODE->GetNext ();
	}

	Packet_ReleaseNUnlock( pCPacket );
}

void CMessenger::MSGR_Status2ONE( DWORD dwDBID, BYTE btNewStatus, CMessenger *pMessenger )
{
	// ����Ͽ��� dwDBID�� ã�� btNewStatus���·� �ٲ۴�.
	// dwDBID�� ���� �����Ѱ��̶��... �� ����� m_pFRIEND�� NULL !!!�� �����ؾ���.
	CDLList< tagMyFriend >::tagNODE *pNODE;

	pNODE = m_ListFRIEND.GetHeadNode ();
	while( pNODE ) {
		if ( dwDBID == pNODE->m_VALUE.m_dwDBID ) {
			pNODE->m_VALUE.m_pFRIEND = pMessenger;	// ���� �����Ҷ��� pMessenger = NULL�� ��

			if ( !(FRIEND_STATUS_REFUSED & pNODE->m_VALUE.m_btSTATUS ) ) {
				// ���� �ź��� ���°ų�, ���� ���� ������ ���°� �ƴϸ�??? (���� ������ ���� ������ ����-������ ���)
				this->m_bFriendUPDATE = ( FRIEND_STATUS_REFUSED & ( pNODE->m_VALUE.m_btSTATUS | btNewStatus ) ) ? true : false;
				pNODE->m_VALUE.m_btSTATUS = btNewStatus;

				classPACKET *pCPacket = Packet_AllocNLock ();

				pCPacket->m_HEADER.m_wType = WSV_MESSENGER;
				pCPacket->m_HEADER.m_nSize = sizeof( wsv_MCMD_STATUS_REPLY );

				pCPacket->m_wsv_MCMD_STATUS_REPLY.m_btCMD = MSGR_CMD_CHANGE_STATUS;
				pCPacket->m_wsv_MCMD_STATUS_REPLY.m_dwUserTAG = dwDBID;
				pCPacket->m_wsv_MCMD_STATUS_REPLY.m_btStatus  = btNewStatus;

				this->m_pOWNER->SendPacket( pCPacket );

				Packet_ReleaseNUnlock( pCPacket );
			}
			break;
		}
		pNODE = pNODE->GetNext ();
	}
}

void CMessenger::MSGR_Msg( t_PACKET *pPacket )
{
	CDLList< tagMyFriend >::tagNODE *pNODE;

	pNODE = m_ListFRIEND.GetHeadNode ();
	while( pNODE ) {
		if ( pNODE->m_VALUE.m_dwDBID == pPacket->m_cli_MESSENGER_CHAT.m_dwUserTAG ) {
			if ( pNODE->m_VALUE.m_pFRIEND && pNODE->m_VALUE.m_pFRIEND->m_pOWNER ) {
				classPACKET *pCPacket = Packet_AllocNLock ();
				if ( pCPacket ) {
					pCPacket->m_HEADER.m_wType = WSV_MESSENGER_CHAT;
					pCPacket->m_HEADER.m_nSize = sizeof( wsv_MESSENGER_CHAT );
					pCPacket->m_wsv_MESSENGER_CHAT.m_dwUserTAG = this->m_dwDBID;
					pCPacket->AppendString( pPacket->m_cli_MESSENGER_CHAT.m_szMSG );

					pNODE->m_VALUE.m_pFRIEND->m_pOWNER->SendPacket( pCPacket );
					Packet_ReleaseNUnlock( pCPacket );
				}
			}
			break;
		}

		pNODE = pNODE->GetNext ();
	}
}
//-------------------------------------------------------------------------------------------------
#endif