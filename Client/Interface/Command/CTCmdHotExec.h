#ifndef _CTCMDHOTEXEC_
#define _CTCMDHOTEXEC_
#include "tcommand.h"

class CTObject;

/// Ŭ���̾�Ʈ ���� 
class CTCmdExit : public CTCommand
{
public:
	CTCmdExit(void);
	~CTCmdExit(void);
	virtual bool	Exec( CTObject* );
};

/// ���� ������Ʈ �� ��α���( __SRVDISCONNECTRELOGIN ��ó���Ⱑ ����Ǿ����� ��츸 �۵�)
class CTCmdReLogin : public CTCommand
{
public:
	CTCmdReLogin(void);
	~CTCmdReLogin(void);
	virtual bool	Exec( CTObject* );
};

/// Ư�� Dialog �ݱ�
class CTCmdHideDialog : public CTCommand
{
public:
	CTCmdHideDialog( void ){}
	CTCmdHideDialog( int iDialogID ){ m_iDialogID = iDialogID; }
	~CTCmdHideDialog( void ){}
	virtual bool Exec( CTObject* );
protected:
	int m_iDialogID;
};

/// NPC���������� �ŷ� ����
class CTCmdCloseStore : public CTCommand
{
public:
	CTCmdCloseStore( void ){}
	~CTCmdCloseStore(void){}
	virtual bool Exec( CTObject* );
};

/// 1:1 �ŷ� ��û �³�
class CTCmdAcceptTradeReq : public CTCommand
{
public:
	CTCmdAcceptTradeReq( void ){}
	CTCmdAcceptTradeReq( WORD  w ){ m_wReqServerObjectIdx = w; }
	~CTCmdAcceptTradeReq(void){}
	virtual bool Exec( CTObject* );
protected:
	WORD m_wReqServerObjectIdx;
};

/// 1:1 �ŷ� ��û �ź� 
class CTCmdRejectTradeReq : public CTCommand
{
public:
	CTCmdRejectTradeReq(void){}
	CTCmdRejectTradeReq( WORD w ){ m_wReqServerObjectIdx = w; }
	~CTCmdRejectTradeReq(void){}
	virtual bool Exec( CTObject* );

protected:
	WORD m_wReqServerObjectIdx;
};

/// ��Ƽ ��û�� �Ѵ�.
class CTCmdSendPacketPartyReq : public CTCommand
{
public:
	CTCmdSendPacketPartyReq( void ){}
	CTCmdSendPacketPartyReq( BYTE btRequest, DWORD dwObjectIDXorTAG )
	{
		m_btRequest = btRequest; 
		m_dwObjectIDXorTAG = dwObjectIDXorTAG; 
	}
	~CTCmdSendPacketPartyReq( void ){}
	virtual bool	Exec( CTObject* );

protected:
	BYTE m_btRequest;
	DWORD m_dwObjectIDXorTAG;
};

/// ��Ƽ ��û�� ���� ������ ������.
class CTCmdSendPacketPartyReply : public CTCommand
{
public:
	CTCmdSendPacketPartyReply( void ){}
	CTCmdSendPacketPartyReply( BYTE btReply, WORD wObjSvrIdx ){ m_btReply = btReply; m_wObjSvrIdx = wObjSvrIdx; }
	~CTCmdSendPacketPartyReply( void ){}
	virtual bool	Exec( CTObject* );
protected:
	BYTE m_btReply;
	WORD m_wObjSvrIdx;
};

/// ��Ƽ �ʴ� �³�
class CTCmdAcceptPartyJoin : public CTCommand
{
public:
	CTCmdAcceptPartyJoin( void ){}
	CTCmdAcceptPartyJoin( WORD wDestObjSvrIdx ){ m_wDestObjSvrIdx = wDestObjSvrIdx; }
	~CTCmdAcceptPartyJoin( void ){}
	virtual bool Exec( CTObject* );

protected:
	WORD m_wDestObjSvrIdx;
};

/// ��Ƽ �ʴ� �ź�
class CTCmdRejectPartyJoin : public CTCommand
{
public:
	CTCmdRejectPartyJoin( void ){}
	CTCmdRejectPartyJoin( WORD wDestObjSvrIdx ){ m_wDestObjSvrIdx = wDestObjSvrIdx; }
	~CTCmdRejectPartyJoin( void ){}
	virtual bool Exec( CTObject* );
protected:
	WORD m_wDestObjSvrIdx;
};

/// ���� Dialog�� ���� ����
class CTCmdChangeState : public CTCommand
{
public:
	CTCmdChangeState(int i){ m_iState = i; }
	~CTCmdChangeState(void){};
	virtual bool Exec( CTObject* );
protected:
	int			m_iState;
};

/// ��Ƽ�� ������.
class CTCmdLeaveParty : public CTCommand
{
public:
	CTCmdLeaveParty(void){}
	virtual ~CTCmdLeaveParty(void){}
	virtual bool Exec( CTObject* );
};


// 2�ν� īƮ
class CTCmdAcceptCartRide : public CTCommand
{
public:
	CTCmdAcceptCartRide( void ){}
	CTCmdAcceptCartRide( WORD wOwnerIDX, WORD wGuestIDX ){ m_wOwnerIDX = wOwnerIDX;	m_wGuestIDX = wGuestIDX;  }
	~CTCmdAcceptCartRide( void ){}
	virtual bool Exec( CTObject* );

protected:
	WORD m_wOwnerIDX;
	WORD m_wGuestIDX;
};

// 2�ν� īƮ
class CTCmdRejectCartRide : public CTCommand
{
public:
	CTCmdRejectCartRide( void ){}
	CTCmdRejectCartRide( WORD wOwnerIDX, WORD wGuestIDX ){ m_wOwnerIDX = wOwnerIDX;	m_wGuestIDX = wGuestIDX;  }
	~CTCmdRejectCartRide( void ){}
	virtual bool Exec( CTObject* );

protected:
	WORD m_wOwnerIDX;
	WORD m_wGuestIDX;
};


#endif