
#include "stdAFX.h"

#include "CMessenger.h"

//-------------------------------------------------------------------------------------------------
/*
1. �α׿½�
	. ���� ��Ͻ�Ų �ɸ��� ���ӵǾ� �ִ°� ???
		. YES
			. ���� ģ���� ����ϰ� �ִ°� ?
				. YES
					. ģ���� online����
				. NO
					. ���� ���� ģ������ �����ߴ�.
					. ģ���� online���� ??
					. ���� ģ������Ʈ���� ���� ���� ??
					. ģ�� ������ ���� ���Ѵٰ� �뺸 ??
		. NO
			. ģ���� offline����...


	. ���� ��� ��Ų �ɸ��� ���ӵǾ� �ִ°� ??? 
		. ���ӵǾ� ������ =>
			. ���� ģ���� ����ϰ� �ִ°� ?
				. YES
					. ���� online���� �Ȱ��� �뺸.
				. NO
					. ģ������ ���� ģ�� ��Ͽ��� �����ߴ�.
					  => ���� ��Ͻ�Ų ����Ʈ���� ģ������ ����
*/

/*
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return;

	pCPacket->m_HEADER.m_wType = WSV_MESSENGER;
	pCPacket->m_HEADER.m_nSize = sizeof( wsv_MESSENGER );
	pCPacket->m_wsv_MESSENGER.m_btCMD = MSGR_CHANGE_STATUS;
	pCPacket->m_wsv_MESSENGER.m_wServerID = xxx;

	SendPacketToSocketIDX( iSocketIDX, pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
*/
void CMessenger::When_LogIN (int iCount, BYTE *pLIST)
{
	// 1. ���� ����� ģ���� ���� ���� ����...
	// 2. ���� ����� ģ���� ���� ģ���� ���� �ֳ� ?
	//		YES: �ڽſ��� ģ�� ���� ����, ģ������ ���� ���� ����
	//		NO : ���� ��Ͽ��� ģ�� ����? �����׸� �˷�? �����׵� �Ⱦ˷�?
	char *pName;
	int iNameLEN;
	classUSER *pClient;

	for (int iC=0; iC<iCount; iC++) {
		pName = (char*)pLIST;
		iNameLEN = strlen( pName );

#ifdef	__SHO_GS
		pClient = g_pUserLIST->Find_CHAR( pName );
#endif
#ifdef	__SHO_WS
		pClient = g_pListCLIENT->Find_CHAR( pName );
#endif

		if ( pClient ) {
			// online ���ӻ���...
			this->When_APPEND (0, pClient->m_iSocketIDX, pName, pClient->m_HashCHAR);
		} else {
			// offline 
			this->When_APPEND (0, 0, pName);
		}

		pName += ( iNameLEN+1 );
	}
}


//-------------------------------------------------------------------------------------------------
/*
2. �α׾ƿ���
	. ���� ��Ͻ�Ű�� �ִ� ģ������Ʈ ��ȸ..
		. ���� ���¸� offline����...
*/
void CMessenger::When_LogOUT ()
{
	CDLList< tagMyFriend >::tagNODE *pNODE;

	m_csFRIEND.Lock ();
	{
		pNODE = m_ListFRIEND.GetHeadNode();
		while( pNODE ) {
			if ( pNODE->m_VALUE.m_iSocketIDX ) {

			}

			pNODE = pNODE->GetNext ();
		}
	}
	m_csFRIEND.Unlock ();
}


//-------------------------------------------------------------------------------------------------
/*
3. ģ����Ͻ� : ģ���� �ݵ�� �¶��� ���¿��� �Ѵ�.
	. ���� ģ����Ͽ� �߰�
	. ģ���ڽ��� ��Ͻ�Ų �ɸ� ����Ʈ�� ���� �߰�.
*/
void CMessenger::When_APPEND (BYTE btStatus, int iSocketIDX, char *szName, t_HASHKEY HashName)
{
	CDLList< tagMyFriend >::tagNODE *pNODE;

	this->m_bUpdate = true;

	m_csFRIEND.Lock ();
	{
		pNODE = m_ListFRIEND.AllocNAppend();
		pNODE->m_VALUE.m_btSTATUS = btStatus;
		pNODE->m_VALUE.m_iSocketIDX = iSocketIDX;
		pNODE->m_VALUE.m_Name.Set( szName );
		pNODE->m_VALUE.m_HashNAME = ( 0 == HashName ) ? CStr::GetHASH( szName ) : HashName;
	}
	m_csFRIEND.Unlock ();
}


//-------------------------------------------------------------------------------------------------
/*
4. ģ�������� : 
	. ���� ģ����Ͽ��� ����
	. ���� ����� �¶����̸�
		. YES
			. ��������� �ڽ��� ����� ��� ����Ʈ���� ���� ����.
		. NO
			. ����? or DB���� �о ������Ʈ?
*/
void CMessenger::When_DELETE (t_HASHKEY HashName, char *szName)
{
	CDLList< tagMyFriend >::tagNODE *pNODE;

	this->m_bUpdate = true;

	m_csFRIEND.Lock ();
	{
		pNODE = m_ListFRIEND.GetHeadNode();
		while( pNODE ) {
			if ( pNODE->m_VALUE.m_HashNAME == HashName ) {
				if ( !strcmpi( pNODE->m_VALUE.m_Name.Get(), szName ) ) {
					m_ListFRIEND.DeleteNFree( pNODE );
					break;
				}
			}

			pNODE = pNODE->GetNext ();
		}
	}
	m_csFRIEND.Unlock ();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
