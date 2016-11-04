#ifndef	__CMESSENGER_H
#define	__CMESSENGER_H
#include "CDLList.h"

/*
#if !defined( __SHO_WS ) && !defined( __SHO_GS )
	#error >>> ERROR :: must defined __SHO_WS or __SHO_GS
#endif

#ifdef	__SHO_WS
	#include "CWS_Client.h"
	typedef	CWS_Client	classUSER;
#endif

#ifdef	__SHO_GS
	#include "GS_ListUSER.h"
#endif
*/

class CWS_Client;
class CMessenger;

//-------------------------------------------------------------------------------------------------

#define	MAX_FRIEND_COUNT		40	// < ( max_packet_size / max_char_name_len )

struct tagFriend_H {
	DWORD		m_dwDBID;
	BYTE		m_btSTATUS;
} ;

// ���� ����� ģ�� ����Ʈ
struct tagMyFriend : public tagFriend_H {
	CMessenger *m_pUSER;
	CStrVAR		m_Name;
	tagMyFriend ()
	{
		m_pUSER = NULL;
	}
} ;


class CMessenger {
private:
	CDLList< tagMyFriend >	m_ListFRIEND;
	CCriticalSection		m_csFRIEND;
	bool					m_bFriendUPDATE;

public :
	BYTE					m_btMsgrSTATUS;

	CMessenger ()
	{
		m_bFriendUPDATE = false;
	}

	bool MSGR_IsUPDATE ()					{	return m_bFriendUPDATE;	}	// db �����ؾ� �ϳ�?

	bool MSGR_Add( CWS_Client *pFriend );	// ģ�� �߰�
	
	void MSGR_Del( DWORD dwDBID );			// ģ�� ����
	void MSGR_Ban( DWORD dwDBID );			// ä�� ����
	void MSGR_Status( DWORD dwDBID, BYTE btNewStatus );

	void MSGR_LogIN (int iCount, BYTE *pLIST);
	void MSGR_LogOUT ();

	BYTE MSGR_OnOffLine ( classPACKET *pCPacket, CMessenger *pFriend, DWORD dwDBID, BYTE btStatus );

	virtual DWORD Get_DBID ()=0;
    virtual bool SendPacket( classPACKET *pCPacket )=0;
} ;

//-------------------------------------------------------------------------------------------------
#endif
