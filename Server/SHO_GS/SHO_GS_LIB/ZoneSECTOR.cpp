
#include "LIB_gsMAIN.h"

#include "ZoneSECTOR.h"
#include "GS_USER.h"

int	CZoneSECTOR::m_iSectorCNT=0;

//-------------------------------------------------------------------------------------------------
CZoneSECTOR::CZoneSECTOR () 
#ifdef	__USE_CS_SECTOR
							: m_csSECTOR( 4000 )
#endif
{
	m_iSectorCNT ++;
}

CZoneSECTOR::~CZoneSECTOR ()
{
	classDLLNODE< CGameOBJ* > *pObjNODE;

    this->LockSector ();
	while( pObjNODE= m_ObjLIST.GetHeadNode() ) {
		m_ObjLIST.DeleteNode( &pObjNODE->DATA->m_SectorNODE );
	}
    this->UnlockSector ();

	m_iSectorCNT --;
}

//-------------------------------------------------------------------------------------------------
void CZoneSECTOR::SetCenterPOS(int iCenterX, int iCenterY)
{
	m_iCenterX = iCenterX;
	m_iCenterY = iCenterY;
}

//-------------------------------------------------------------------------------------------------
void CZoneSECTOR::InsToSEC( CGameOBJ *pObj )
{
    this->LockSector ();
    m_ObjLIST.AppendNode( &pObj->m_SectorNODE );
	pObj->m_pGroupSECTOR = this;
    this->UnlockSector ();
}

void CZoneSECTOR::DelFromSEC( CGameOBJ *pObj )
{
    this->LockSector ();
    m_ObjLIST.DeleteNode( &pObj->m_SectorNODE );
	pObj->m_pGroupSECTOR = NULL;
    this->UnlockSector ();
}



//-------------------------------------------------------------------------------------------------
classDLLNODE< CGameOBJ* > *CNormalSECTOR::FindFirstCHAR (CGameOBJ *pObjFINDER)
{
	m_pObjNODE = m_ObjLIST.GetHeadNode ();

	while( m_pObjNODE ) {
		if ( m_pObjNODE->DATA->IsCHAR() )
			return m_pObjNODE;

		m_pObjNODE = m_ObjLIST.GetNextNode( m_pObjNODE );
	}
	return NULL;
}
classDLLNODE< CGameOBJ* > *CNormalSECTOR::FindNextCHAR (classDLLNODE< CGameOBJ* > *pCurNODE, CGameOBJ *pObjFINDER)
{
	m_pObjNODE = m_ObjLIST.GetNextNode( pCurNODE );

	while( m_pObjNODE ) {
		if ( m_pObjNODE->DATA->IsCHAR() )
			return m_pObjNODE;

		m_pObjNODE = m_ObjLIST.GetNextNode( m_pObjNODE );
	}
	return NULL;
}

//-------------------------------------------------------------------------------------------------
void CNormalSECTOR::SendPacketToSECTOR( classPACKET *pCPacket, CGameOBJ *pObjSENDER )
{
	this->LockSector ();

	classDLLNODE< CGameOBJ* > *pNode;
    for (pNode=m_ObjLIST.GetHeadNode(); pNode; pNode=m_ObjLIST.GetNextNode( pNode ) ) {
        pNode->DATA->SendPacket( pCPacket );
	}

	this->UnlockSector ();
}

//-------------------------------------------------------------------------------------------------
void CNormalSECTOR::SendTeamPacketToSECTOR( classPACKET *pCPacket, int iTeamNo, CGameOBJ *pObjSENDER )
{
	this->LockSector ();

	classDLLNODE< CGameOBJ* > *pNode;
    for (pNode=m_ObjLIST.GetHeadNode(); pNode; pNode=m_ObjLIST.GetNextNode( pNode ) ) {
		if ( iTeamNo == pNode->DATA->Get_TEAM() )
			pNode->DATA->SendPacket( pCPacket );
	}

	this->UnlockSector ();
}

//-------------------------------------------------------------------------------------------------
void CNormalSECTOR::SendAddUserPacket( classUSER *pUSER, classPACKET *pCPacket )
{
	this->LockSector ();

    classPACKET *pNewPacket;

	classDLLNODE< CGameOBJ* > *pNode;
    for (pNode=m_ObjLIST.GetHeadNode(); pNode; pNode=m_ObjLIST.GetNextNode( pNode ) ) {
        // ���� ����ڿ��� pUSER���� ����.
        pNode->DATA->SendPacket( pCPacket );

        // pUSER���� ���� ����� ���� ����.
        pNewPacket = Packet_AllocNLock ();

		// �������� ��� ������ ������ Object index �� �ٲ�� �����Ƿ�...
		pNode->DATA->Update_OwnerObjIDX( pUSER );
        if ( pNode->DATA->Make_gsv_ADD_OBJECT( pNewPacket ) )
            pUSER->SendPacket( pNewPacket );

        Packet_ReleaseNUnlock( pNewPacket );
	}

	this->UnlockSector ();
}

//-------------------------------------------------------------------------------------------------
void CNormalSECTOR::SendSubUserPacket( classUSER *pUSER, classPACKET *pCPacket )
{
	static const int iMaxLeftCnt = (MAX_PACKET_SIZE-sizeof(t_PACKETHEADER)) / sizeof(WORD);

    classPACKET *pNewPacket;
	int iLeftCnt=0;

	this->LockSector ();

    // pUSER���� ���� ����ڸ�...
    pNewPacket = Packet_AllocNLock ();
	pNewPacket->m_HEADER.m_wType = GSV_SUB_OBJECT;
	pNewPacket->m_HEADER.m_nSize = sizeof( t_PACKETHEADER );			// �ݵ�� sizeof( t_PACKETHEADER )��...

	classDLLNODE< CGameOBJ* > *pNode;
    for (pNode=m_ObjLIST.GetHeadNode(); pNode; pNode=m_ObjLIST.GetNextNode( pNode ) ) {
        // ���� ����ڿ��� pUSER��...
        pNode->DATA->SendPacket( pCPacket );

        pNewPacket->m_gsv_SUB_OBJECT.m_wObjectIDX[ iLeftCnt++ ] = pNode->DATA->Get_INDEX ();
        pNewPacket->m_HEADER.m_nSize += sizeof(WORD);

		if ( iLeftCnt >= iMaxLeftCnt ) {
            pUSER->SendPacket( pNewPacket );
            Packet_ReleaseNUnlock( pNewPacket );

            iLeftCnt = 0;

            pNewPacket = Packet_AllocNLock ();
            pNewPacket->m_HEADER.m_wType = GSV_SUB_OBJECT;
            pNewPacket->m_HEADER.m_nSize = sizeof( t_PACKETHEADER );			// �ݵ�� sizeof( t_PACKETHEADER )��...
        }
	}

    if ( iLeftCnt ) {
        pUSER->SendPacket( pNewPacket );
    }

    Packet_ReleaseNUnlock( pNewPacket );

	this->UnlockSector ();
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
classDLLNODE< CGameOBJ* > *CAgitSECTOR::FindFirstCHAR (CGameOBJ *pObjFINDER)
{
	if ( pObjFINDER->IsUSER() ) {
		m_pObjNODE = m_ObjLIST.GetHeadNode ();
		while( m_pObjNODE ) {
			if ( m_pObjNODE->DATA->IsUSER() ) {
				if ( ( (classUSER*)pObjFINDER )->GetClanID() == ( (classUSER*)m_pObjNODE->DATA)->GetClanID() )
					return m_pObjNODE;
			}

			m_pObjNODE = m_ObjLIST.GetNextNode( m_pObjNODE );
		}
	}
	return NULL;
}
classDLLNODE< CGameOBJ* > *CAgitSECTOR::FindNextCHAR (classDLLNODE< CGameOBJ* > *pCurNODE, CGameOBJ *pObjFINDER)
{
	assert( pObjFINDER->IsUSER() );

	m_pObjNODE = m_ObjLIST.GetNextNode( pCurNODE );
	while( m_pObjNODE ) {
		if ( m_pObjNODE->DATA->IsUSER() ) {
			if ( ( (classUSER*)pObjFINDER )->GetClanID() == ( (classUSER*)m_pObjNODE->DATA)->GetClanID() )
				return m_pObjNODE;
		}

		m_pObjNODE = m_ObjLIST.GetNextNode( m_pObjNODE );
	}
	return NULL;
}

//-------------------------------------------------------------------------------------------------
void CAgitSECTOR::SendPacketToSECTOR( classPACKET *pCPacket, CGameOBJ *pObjSENDER )
{
	this->LockSector ();

	classUSER *pDestUser;
	classDLLNODE< CGameOBJ* > *pNode;

	DWORD dwClanID;

	if ( pObjSENDER->IsUSER() ) {
		classUSER *pSourUser = (classUSER*)pObjSENDER;
		dwClanID = pSourUser->GetClanID();
	} else 
	if ( pObjSENDER->IsITEM() ) {
		CObjITEM *pSourItem = (CObjITEM*)pObjSENDER;
		dwClanID = pSourItem->m_dwDropperClanID;
	} else {
		dwClanID = 0;
	}

	if ( dwClanID ) {
		for (pNode=m_ObjLIST.GetHeadNode(); pNode; pNode=m_ObjLIST.GetNextNode( pNode ) ) {
			if ( pNode->DATA->IsUSER() ) {
				pDestUser = (classUSER*)pNode->DATA;
				if ( dwClanID == pDestUser->GetClanID() ) {
					// ���� Ŭ���̸�...
					pNode->DATA->SendPacket( pCPacket );
				}
			}
		}
	} else 
	if ( pObjSENDER->IsA( OBJ_NPC ) ) {
		for (pNode=m_ObjLIST.GetHeadNode(); pNode; pNode=m_ObjLIST.GetNextNode( pNode ) ) {
			pNode->DATA->SendPacket( pCPacket );
		}
	} else
	if ( pObjSENDER->IsUSER() ) {
		// 2005.3.30 ����Ʈ���� Ŭ�� �����Ϳ� ���� ����� ����~~~ ������ ����~~
		classUSER *pSourUser = (classUSER*)pObjSENDER;
		if ( pSourUser->m_bInAGIT ) {
			pSourUser->m_bInAGIT = false;

			short nZoneNO   = pSourUser->m_nReviveZoneNO;
			tPOINTF	PosWARP = pSourUser->m_PosREVIVE;

			PosWARP.x += ( RANDOM(1001) - 500 );	// ���� 5����..
			PosWARP.y += ( RANDOM(1001) - 500 );

			pSourUser->Send_gsv_RELAY_REQ( RELAY_TYPE_RECALL, nZoneNO, PosWARP );
		}
	}

	this->UnlockSector ();
}

//-------------------------------------------------------------------------------------------------
void CAgitSECTOR::SendTeamPacketToSECTOR( classPACKET *pCPacket, int iTeamNo, CGameOBJ *pObjSENDER )
{
	this->LockSector ();

	classDLLNODE< CGameOBJ* > *pNode;
    for (pNode=m_ObjLIST.GetHeadNode(); pNode; pNode=m_ObjLIST.GetNextNode( pNode ) ) {
		if ( iTeamNo == pNode->DATA->Get_TEAM() ) {
			pNode->DATA->SendPacket( pCPacket );
		}
	}

	this->UnlockSector ();
}

//-------------------------------------------------------------------------------------------------
void CAgitSECTOR::SendAddUserPacket( classUSER *pUSER, classPACKET *pCPacket )
{
	pUSER->m_bInAGIT = true;

	this->LockSector ();

    classPACKET *pNewPacket;
	classUSER *pDestUser;
	CObjITEM *pSourItem;

	classDLLNODE< CGameOBJ* > *pNode;
    for (pNode=m_ObjLIST.GetHeadNode(); pNode; pNode=m_ObjLIST.GetNextNode( pNode ) ) {
        // ���� ����ڿ��� pUSER���� ����.
		if ( pNode->DATA->IsUSER() ) {
			// ���� Ŭ���ϰ�쿡�� ����...
			pDestUser = (classUSER*)pNode->DATA;
			if ( pDestUser->GetClanID() != pUSER->GetClanID() )
				continue;
		} else
		if ( pNode->DATA->IsITEM() ) {
			pSourItem = (CObjITEM*)pNode->DATA;
			if ( pSourItem->m_dwDropperClanID != pUSER->GetClanID() )
				continue;
		} else
		if ( !pNode->DATA->IsA( OBJ_NPC ) ) 
			continue;

        pNode->DATA->SendPacket( pCPacket );

        // pUSER���� ���� ����� ���� ����.
        pNewPacket = Packet_AllocNLock ();

		// �������� ��� ������ ������ Object index �� �ٲ�� �����Ƿ�...
		pNode->DATA->Update_OwnerObjIDX( pUSER );
        if ( pNode->DATA->Make_gsv_ADD_OBJECT( pNewPacket ) )
            pUSER->SendPacket( pNewPacket );

        Packet_ReleaseNUnlock( pNewPacket );
	}

	this->UnlockSector ();
}

//-------------------------------------------------------------------------------------------------
void CAgitSECTOR::SendSubUserPacket( classUSER *pUSER, classPACKET *pCPacket )
{
	static const int iMaxLeftCnt = (MAX_PACKET_SIZE-sizeof(t_PACKETHEADER)) / sizeof(WORD);

    classPACKET *pNewPacket;
	int iLeftCnt=0;

	this->LockSector ();

    // pUSER���� ���� ����ڸ�...
    pNewPacket = Packet_AllocNLock ();
	pNewPacket->m_HEADER.m_wType = GSV_SUB_OBJECT;
	pNewPacket->m_HEADER.m_nSize = sizeof( t_PACKETHEADER );			// �ݵ�� sizeof( t_PACKETHEADER )��...

	classUSER *pDestUser;
	classDLLNODE< CGameOBJ* > *pNode;
    for (pNode=m_ObjLIST.GetHeadNode(); pNode; pNode=m_ObjLIST.GetNextNode( pNode ) ) {
        // ���� ����ڿ��� pUSER��...
		if ( !pNode->DATA->IsUSER() )
			continue;

		// ���� Ŭ���ϰ�쿡�� ����...
		pDestUser = (classUSER*)pNode->DATA;
		if ( pDestUser->GetClanID() != pUSER->GetClanID() )
			continue;

        pNode->DATA->SendPacket( pCPacket );

        pNewPacket->m_gsv_SUB_OBJECT.m_wObjectIDX[ iLeftCnt++ ] = pNode->DATA->Get_INDEX ();
        pNewPacket->m_HEADER.m_nSize += sizeof(WORD);

		if ( iLeftCnt >= iMaxLeftCnt ) {
            pUSER->SendPacket( pNewPacket );
            Packet_ReleaseNUnlock( pNewPacket );

            iLeftCnt = 0;

            pNewPacket = Packet_AllocNLock ();
            pNewPacket->m_HEADER.m_wType = GSV_SUB_OBJECT;
            pNewPacket->m_HEADER.m_nSize = sizeof( t_PACKETHEADER );			// �ݵ�� sizeof( t_PACKETHEADER )��...
        }
	}

    if ( iLeftCnt ) {
        pUSER->SendPacket( pNewPacket );
    }

    Packet_ReleaseNUnlock( pNewPacket );

	this->UnlockSector ();
}
