
#include "stdAFX.h"

#include "CChatROOM.h"
#if	defined( __SHO_WS ) || defined( __INC_WORLD )

//-------------------------------------------------------------------------------------------------
bool CChatROOM::ReqMake( classUSER *pUSER, cli_CHAT_ROOM_MAKE *pMake, char *szTitle, char *szPasswd)
{
	m_btMaxUSER  = pMake->m_btMaxUSER;
	m_btRoomTYPE = pMake->m_btRoomTYPE;

	if ( m_btMaxUSER >= MAX_CHAT_ROOM_USERS )
		m_btMaxUSER = MAX_CHAT_ROOM_USERS;
	else
	if ( m_btMaxUSER < 2 )
		m_btMaxUSER = 2;

	m_szTITLE.Set( szTitle );
	m_szPASSWD.Set( szPasswd );

	m_ListUSER.AppendNode( pUSER->m_pNodeChatROOM );

	return true;
}
bool CChatROOM::ReqJoin( classUSER *pUSER )
{
	CDLList< classUSER* >::tagNODE *pNode;

	classPACKET *pCPketADD  = Packet_AllocNLock ();
	classPACKET *pCPketJOIN = Packet_AllocNLock ();

	pCPketADD->m_HEADER.m_wType = WSV_CHATROOM;
	pCPketADD->m_HEADER.m_nSize = sizeof( wsv_CHAT_ROOM_USER );
	pCPketADD->m_wsv_CHAT_ROOM_USER.m_btCMD = CHAT_REPLY_USER_ADD;

	pCPketADD->m_wsv_CHAT_ROOM_USER.m_wUserID = pUSER->m_iSocketIDX;
	pCPketADD->AppendString( pUSER->Get_NAME() );


	pCPketJOIN->m_HEADER.m_wType = WSV_CHATROOM;
	pCPketJOIN->m_HEADER.m_nSize = sizeof( wsv_CHAT_ROOM_USER );
	pCPketJOIN->m_wsv_CHAT_ROOM_USER.m_btCMD = CHAT_REPLY_USER_ADD;

	WORD wObjTAG;
	m_ListCS.Lock ();
	{	
		pNode = m_ListUSER.GetHeadNode ();
		while( pNode ) {
			pNode->m_VALUE->SendPacket( pCPketADD );

			wObjTAG = pNode->m_VALUE->m_iSocketIDX;

			pCPketJOIN->AppendData( &wObjTAG, sizeof( WORD ) );
			pCPketJOIN->AppendString( pNode->m_VALUE->Get_NAME() );

			pNode = pNode->GetNext ();
		} ;

		m_ListUSER.AppendNode( pUSER->m_pNodeChatROOM );
	}
	m_ListCS.Unlock ();

	pUSER->SendPacket( pCPketJOIN );

	Packet_ReleaseNUnlock( pCPketJOIN );
	Packet_ReleaseNUnlock( pCPketADD );

	return true;
}
int CChatROOM::ReqLeft( classUSER *pUSER )
{
	m_ListCS.Lock ();
	{
		m_ListUSER.DeleteNode( pUSER->m_pNodeChatROOM );
		this->Send_wsv_CHATROOM( CHAT_REPLY_USER_LEFT, pUSER->m_iSocketIDX, NULL );
	}
	m_ListCS.Unlock ();

	return this->GetRoomUSERS();
}
bool CChatROOM::ReqKick( classUSER *pUSER, t_HASHKEY HashUSER )
{
	m_ListCS.Lock ();

	if ( pUSER->m_pNodeChatROOM == m_ListUSER.GetHeadNode () ) {
		// �����ΰ� ?
		CDLList< classUSER* >::tagNODE *pNode;
		pNode = pUSER->m_pNodeChatROOM->GetNext();
		while( pNode ) {
			if ( pNode->m_VALUE->m_HashCHAR == HashUSER ) {
				// �� �i���...
				pNode->m_VALUE->Send_wsv_CHATROOM( CHAT_REPLY_KICKED, pNode->m_VALUE->m_iSocketIDX, NULL );

				this->ReqLeft( pNode->m_VALUE );
			}
		}
	}

	m_ListCS.Unlock ();

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CChatROOM::Send_wsv_CHATROOM (BYTE btCMD, WORD wUserID, char *szSTR)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = WSV_CHATROOM;
	pCPacket->m_HEADER.m_nSize = sizeof( tag_CHAT_HEADER );
	pCPacket->m_tag_CHAT_HEADER.m_btCMD = btCMD;

	if ( wUserID ) {
		pCPacket->m_HEADER.m_nSize += sizeof(WORD);
		WORD *pWPtr = (WORD*)(&pCPacket->m_pDATA[ sizeof( tag_CHAT_HEADER ) ] );
		*pWPtr = wUserID;
	}

	if ( szSTR ) {
		pCPacket->AppendString( szSTR );
	}

	CDLList< classUSER* >::tagNODE *pNode;
	m_ListCS.Lock ();
	{
		pNode = m_ListUSER.GetHeadNode();
		while( pNode ) {
			pNode->m_VALUE->SendPacket( pCPacket );
			pNode = pNode->GetNext ();
		} 
	}
	m_ListCS.Unlock ();

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CChatROOM::Send_wsv_CHATROOM_MSG( WORD wObjectID, char *szMSG )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = WSV_CHATROOM_MSG;
	pCPacket->m_HEADER.m_nSize = sizeof( wsv_CHATROOM_MSG );
	pCPacket->AppendString( szMSG );

	CDLList< classUSER* >::tagNODE *pNode;
	this->m_ListCS.Lock ();
		pNode = m_ListUSER.GetHeadNode ();
		while( pNode ) {
			pNode->m_VALUE->SendPacket( pCPacket );

			pNode = pNode->GetNext ();
		}
	this->m_ListCS.Unlock ();
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
CChatRoomLIST::CChatRoomLIST (WORD wMaxRoomCNT) : CIndexARRAY< CChatROOM* > ( "ChatRoom", wMaxRoomCNT ), m_RoomsCS( 4000 )
{
	m_pROOMs = new CChatROOM[ wMaxRoomCNT ];
}
CChatRoomLIST::~CChatRoomLIST ()
{
	SAFE_DELETE_ARRAY( m_pROOMs );
}


//-------------------------------------------------------------------------------------------------
bool CChatRoomLIST::Recv_cli_CHATROOM_MSG( classUSER *pUSER, t_PACKET *pPacket )
{
	if ( pUSER->m_iChatRoomID ) { 
		CChatROOM *pRoom;
		this->m_RoomsCS.Lock ();
		pRoom = this->GetData( pUSER->m_iChatRoomID );
		this->m_RoomsCS.Unlock ();
		if ( pRoom ) {
			return pRoom->Send_wsv_CHATROOM_MSG( pUSER->m_iSocketIDX, pPacket->m_cli_CHATROOM_MSG.m_szMSG );
		}

		// ��..���� ����
		pUSER->m_iChatRoomID = 0;
	}
	// ���� ���� ??? 

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CChatRoomLIST::Recv_cli_CHATROOM( classUSER *pUSER, t_PACKET *pPacket )
{
	switch( pPacket->m_tag_CHAT_HEADER.m_btCMD ) {
		case CHAT_REQ_MAKE		:
		{
			short nOffset = sizeof( cli_CHAT_ROOM_MAKE );

			char *szTitle, *szPasswd;
			szTitle = Packet_GetStringPtr( pPacket, nOffset );
			if ( !szTitle )
				return false;

			szPasswd= Packet_GetStringPtr( pPacket, nOffset );

			int iRoomSLOT;
			this->m_RoomsCS.Lock ();
			iRoomSLOT = this->GetEmptySlot();
			if ( iRoomSLOT > 0 ) {
				this->m_pINDEX[ iRoomSLOT ] = &m_pROOMs[ iRoomSLOT ];
				this->m_iUsedSlot ++; 
				m_pROOMs[ iRoomSLOT ].ReqMake( pUSER, &pPacket->m_cli_CHAT_ROOM_MAKE, szTitle, szPasswd );

				pUSER->m_iChatRoomID = iRoomSLOT;
				
				pUSER->Send_wsv_CHATROOM( CHAT_REPLY_ROOM_MAKED, 0, NULL );
			} else
				pUSER->Send_wsv_CHATROOM( CHAT_REPLY_MAKE_FAILED, 0, NULL );

			this->m_RoomsCS.Unlock ();
			return true;
		}
		case CHAT_REQ_JOIN		:
		{
			short nOffset = sizeof( cli_CHAT_ROOM_JOIN );

			CChatROOM *pRoom;
			this->m_RoomsCS.Lock ();
			pRoom = this->GetData( pPacket->m_cli_CHAT_ROOM_JOIN.m_wRoomID );
			if ( pRoom ) {
				if ( pRoom->GetRoomUSERS() < pRoom->m_btMaxUSER ) {
					if ( pRoom->m_szPASSWD.BuffLength() ) {
						// ����ִ�.
						char *szPasswd = Packet_GetStringPtr( pPacket, nOffset );
						if ( !szPasswd || strcmp( pRoom->m_szPASSWD.Get(), szPasswd ) ) {
							// ����
							pUSER->Send_wsv_CHATROOM( CHAT_REPLY_INVALIED_PASS, 0, NULL );
							goto _RET_JOIN;
						}
					}

					pRoom->ReqJoin( pUSER );
					pUSER->m_iChatRoomID = pPacket->m_cli_CHAT_ROOM_JOIN.m_wRoomID;
				} else {
					// ����
					pUSER->Send_wsv_CHATROOM( CHAT_REPLY_FULL_USERS, 0, NULL );
				}
			} else {
				// ����� 
				pUSER->Send_wsv_CHATROOM( CHAT_REPLY_ROOM_NOT_FOUND, 0, NULL );
			}
		_RET_JOIN :
			this->m_RoomsCS.Unlock ();
			return true;
		}
		case CHAT_REQ_LEFT		:
		{
			return this->LeftUSER( pUSER );
		}
		case CHAT_REQ_KICK_USER :
		{
			CChatROOM *pRoom = this->GetData( pUSER->m_iChatRoomID );
			if ( pRoom ) {
				pRoom->ReqKick( pUSER, pPacket->m_cli_CHAT_ROOM_KICK.m_HashUSER );
			}
			return true;
		}
		case CHAT_REQ_ROOM_LIST :
		{
			#define	MAX_ROOM_LIST_CNT	15

			CChatROOM *pRoom;
			int iCnt=0,iR;
			tag_CHAT_ROOM sRoom;

			classPACKET *pCPacket = Packet_AllocNLock ();

			pCPacket->m_HEADER.m_wType = WSV_CHATROOM;
			pCPacket->m_HEADER.m_nSize = sizeof( wsv_CHAT_ROOM_LIST	);
			pCPacket->m_wsv_CHAT_ROOM_LIST.m_btCMD = CHAT_REPLY_ROOM_LIST;

			this->m_RoomsCS.Lock ();
			for (iR=pPacket->m_cli_CHAT_ROOM_LIST.m_wFromRoomID; iR<this->m_iBuffSize; iR++) {
				pRoom = this->GetData( iR );
				if ( !pRoom )
					continue;

				if ( pPacket->m_cli_CHAT_ROOM_LIST.m_btRoomTYPE && pPacket->m_cli_CHAT_ROOM_LIST.m_btRoomTYPE != pRoom->m_btRoomTYPE )
					continue;

				// ������ ...
				sRoom.m_btRoomTYPE = pRoom->m_btRoomTYPE;
				sRoom.m_cUserCNT   = (char)pRoom->GetRoomUSERS ();
				pCPacket->AppendData  ( &sRoom, sizeof(tag_CHAT_ROOM) );
				pCPacket->AppendString( pRoom->m_szTITLE.Get() );

				if ( ++iCnt >= MAX_ROOM_LIST_CNT )
					break;
			}
			this->m_RoomsCS.Unlock ();

			pCPacket->m_wsv_CHAT_ROOM_LIST.m_cRoomCNT = iCnt;

			pUSER->SendPacket( pCPacket );
			Packet_ReleaseNUnlock( pCPacket );

			return true;
		}
	}

	return false;
}

//-------------------------------------------------------------------------------------------------
bool CChatRoomLIST::LeftUSER ( classUSER *pUSER )
{
	if ( pUSER->m_iChatRoomID ) {
		CChatROOM *pRoom = this->GetData( pUSER->m_iChatRoomID );

		this->m_RoomsCS.Lock ();

		if ( pRoom ) {
			if ( pRoom->ReqLeft( pUSER ) <= 0 ) {
				// �� �ǰ���...
				this->m_pINDEX[ pUSER->m_iChatRoomID ] = NULL;
				this->m_iUsedSlot --; 
			}
			pUSER->m_iChatRoomID = 0;
		}

		this->m_RoomsCS.Unlock ();
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#endif