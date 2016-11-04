#ifndef _UICOMMAND_
#define _UICOMMAND_

#include "../../common/CItem.h"
#include "net_prototype.h"
#include "tcommand.h"

class	CMakeDLG;
class	CItem;
/*--------------------------------------------------------------------------------------------------------*/
/// �κ��丮���� ����â���� ������ �̵�
class CTCmdTakeInItem2MakeDlg : public CTCommand
{
public:
	CTCmdTakeInItem2MakeDlg(){}
	~CTCmdTakeInItem2MakeDlg( void ){}
	virtual bool Exec( CTObject* pObj );
};

/// ����â���� �κ��丮�� �������� ����.
class CTCmdTakeOutItemFromMakeDlg : public CTCommand
{
public:
	CTCmdTakeOutItemFromMakeDlg(){}
	~CTCmdTakeOutItemFromMakeDlg( void ){}
	virtual bool Exec( CTObject* pObj );
};

/// �и�/����â���� �������� ����.
class CTCmdTakeOutItemFromSeparateDlg : public CTCommand
{
public:
	CTCmdTakeOutItemFromSeparateDlg(){}
	virtual ~CTCmdTakeOutItemFromSeparateDlg(){}
	virtual bool Exec( CTObject* pObj );
};

/// Pat������ ����
class CTCmdAssembleRideItem : public CTCommand
{
public:
	CTCmdAssembleRideItem(void){};
	virtual ~CTCmdAssembleRideItem( void ){}
	virtual bool Exec( CTObject* pObj );
};


/// Pat������ Ż��
class CTCmdDisAssembleRideItem : public CTCommand
{
public:
	CTCmdDisAssembleRideItem(void){}
	virtual ~CTCmdDisAssembleRideItem(void){}
	virtual bool Exec( CTObject* pObj );

};
///*-----------------------------------------------------------------------------------------
/// ���� �Է±� ����
class CTCmdNumberInput;
class CTCmdOpenNumberInputDlg : public CTCommand
{
public:
	CTCmdOpenNumberInputDlg(void);
	virtual ~CTCmdOpenNumberInputDlg(void){}
	virtual bool Exec( CTObject* pObj );

	void	SetCommand( CTCmdNumberInput* pCmd );
	void	SetMaximum( __int64 i64Maximum );
protected:
	CTCmdNumberInput*		m_pCmd;
	__int64					m_i64Maximum;
};



/// ������â���� �ٸ� ������ ���λ����� ��Ǯ�� �ȶ� ���Ǵ� Command
class CTCmdDragItem2AvatarStoreDlg : public CTCmdOpenNumberInputDlg
{
public:
	CTCmdDragItem2AvatarStoreDlg(void){}
	virtual ~CTCmdDragItem2AvatarStoreDlg(void){}
	virtual bool Exec( CTObject* pObj );
};

/// ģ�� ��� ��û ����
class CTCmdAcceptAddFriend : public CTCommand
{
public:
	CTCmdAcceptAddFriend( WORD wUserIdx, BYTE btStatus, const char* pszName );
	virtual ~CTCmdAcceptAddFriend(void){}
	virtual bool Exec(CTObject* pObj );

protected:
	WORD		m_wUserIdx;
	BYTE		m_btStatus;
	std::string m_strName;
};

/// ģ�� ��� ��û �ź�
class CTCmdRejectAddFriend : public CTCommand
{
public:
	CTCmdRejectAddFriend( WORD wUserIdx , const char* pszName);
	virtual ~CTCmdRejectAddFriend(void){}
	virtual bool Exec( CTObject* pObj );

protected:
	WORD		m_wUserIdx;
	std::string m_strName;
};

/// ģ�� ����Ʈ���� ģ�� ����
class CTCmdRemoveFriend : public CTCommand
{
public:
	CTCmdRemoveFriend( DWORD dwUserTag );
	virtual ~CTCmdRemoveFriend(){}
	virtual bool Exec( CTObject* pObj );

protected:
	DWORD	m_dwUserTag;
};
/// ����Ʈ ����
class CTCmdAbandonQuest : public CTCommand
{
public:
	CTCmdAbandonQuest( int iQuestSlotIdx,int iQuestID );
	virtual ~CTCmdAbandonQuest(){}
	virtual bool Exec( CTObject* pObj );

protected:
	int m_iQuestSlotIdx;
	int m_iQuestID;
};

/// �������� ������ ��Ŷ ����
class CTCmdSendPacketDropItem : public CTCommand
{
public:
	CTCmdSendPacketDropItem( short nInvenIdx, int iQuantity );
	virtual ~CTCmdSendPacketDropItem(){}
	virtual bool Exec( CTObject* pObj );
protected:
	short	m_nInvenIdx;
	int		m_iQuantity;
};


/// Dialog Icon�� �޴�â���� �巡�׽���
class CTCmdMoveDialogIcon2GroundFromMenu : public CTCommand
{
public:
	CTCmdMoveDialogIcon2GroundFromMenu(){}
	virtual ~CTCmdMoveDialogIcon2GroundFromMenu(){}
	virtual bool Exec( CTObject* pObj );
};

/// Dialog Icon�� ����ȭ�鿡�� �巡�׽���
class CTCmdMoveDialogIcon2Ground : public CTCommand
{
public:
	CTCmdMoveDialogIcon2Ground(){}
	virtual ~CTCmdMoveDialogIcon2Ground(){}
	virtual bool Exec( CTObject* pObj );
};

/// Dialog Icon�� ����ȭ�鿡�� ���ٿ� ���
class CTCmdRegistDialogIcon2QuickBar : public CTCommand
{
public:
	CTCmdRegistDialogIcon2QuickBar(){}
	virtual ~CTCmdRegistDialogIcon2QuickBar(){}
	virtual bool Exec( CTObject* pObj );
};

/// Dilaog Icons�� It_Mgr�� Container���� ���� Command
class CTCmdRemoveDialogIcon : public CTCommand
{
public:
	CTCmdRemoveDialogIcon( int iDialogType );
	virtual ~CTCmdRemoveDialogIcon(){}
	virtual bool Exec( CTObject* pObj );
protected:
	int		m_iDialogType;

};

/// �ش� �������� Wish List�� �߰��ϴ� Command
class CTCmdAddItem2WishList : public CTCommand
{
public:
	CTCmdAddItem2WishList( tagITEM& Item );
	virtual ~CTCmdAddItem2WishList(){}
	virtual bool Exec( CTObject* pObj );
protected:
	tagITEM	m_Item;
};

/// �ش� �������� Wish List�� �߰��ϴ� Command
class CTCmdChangeStateUpgradeDlg : public CTCommand
{
public:
	CTCmdChangeStateUpgradeDlg( int iState );
	virtual ~CTCmdChangeStateUpgradeDlg(){};
	virtual bool Exec( CTObject* pObj );

protected:
	int m_iState;
};

/// ActionListener���� ���� EndRepair/EndAppraisal�� ȣ��� ������ .. �񵿱������� ������ ���� Ŀ���
/// ���� ����
class CTCmdEndRepair : public CTCommand
{
public:
	CTCmdEndRepair(){}
	virtual ~CTCmdEndRepair(){}
	virtual bool Exec( CTObject* pObj );
};

/// ������ ���� ����
class CTCmdEndAppraisal : public CTCommand
{
public:
	CTCmdEndAppraisal(){}
	virtual ~CTCmdEndAppraisal(){}
	virtual bool Exec( CTObject* pObj );
};

/// ������û
class CTCmdSendAppraisalReq : public CTCommand
{
public:
	CTCmdSendAppraisalReq( int iIndex );
	virtual ~CTCmdSendAppraisalReq(){}
	virtual bool Exec( CTObject* pObj );
protected:
	int		m_iIndex;
};

/// �ٸ� ������ ���λ������� �Ĵ� ��ǰ ����Ʈ ��û
class CTCmdQuerySellItem2PrivateStore : public CTCommand
{
public:
	CTCmdQuerySellItem2PrivateStore( WORD wSellerSvrIdx, int iItemCount, tagSELL_ITEM& Item );
	virtual ~CTCmdQuerySellItem2PrivateStore(){}
	virtual bool Exec( CTObject* pObj );

protected:
	WORD			m_wSellerSvrIdx;
	int				m_iItemCount;
	tagSELL_ITEM	m_SellItem;
};

/// �ٸ� ������ ���λ������� ��� ��ǰ ����Ʈ ��û
class CTCmdQueryBuyItemFromPrivateStore : public CTCommand
{
public:
	CTCmdQueryBuyItemFromPrivateStore( WORD wSellerSvrIdx, int iItemCount, tagPS_SLOT_ITEM& Item );
	virtual ~CTCmdQueryBuyItemFromPrivateStore(){}
	virtual bool Exec( CTObject* pObj );
protected:
	WORD			m_wSellerSvrIdx;
	int				m_iItemCount;
	tagPS_SLOT_ITEM m_BuyItem;
};

/// Ŭ�� ���� ��û �³�
class CTCmdAcceptReqJoinClan : public CTCommand
{
public:
	CTCmdAcceptReqJoinClan( const char* pszMasterName );
	virtual ~CTCmdAcceptReqJoinClan(){};
	virtual bool Exec( CTObject* pObj );
protected:
	std::string			m_strMasterName;
};

/// Ŭ�� ���� ��û �ź�
class CTCmdRejectReqJoinClan : public CTCommand
{
public:
	CTCmdRejectReqJoinClan( const char* pszMasterName );
	virtual ~CTCmdRejectReqJoinClan(){};
	virtual bool Exec( CTObject* pObj );
protected:
	std::string			m_strMasterName;
};

/// ������ Ŭ�� ��ɾ� ����
class CTCmdClanCommand : public CTCommand
{
public:
	CTCmdClanCommand( BYTE btCmd, const char* pszMsg );
	virtual ~CTCmdClanCommand(){}
	virtual bool Exec( CTObject* pObj );

protected:
	BYTE				m_btCmd;
	std::string			m_strMsg;
};

/// �������� ���� ���� ����� ���
class CTCmdCancelWaitDisconnect : public CTCommand
{
public:
	CTCmdCancelWaitDisconnect(){}
	virtual ~CTCmdCancelWaitDisconnect(){}
	virtual bool Exec( CTObject* pObj );
};

/// �̸� �Է��� ������ ���̴� Ŀ�ǵ��� �⺻ Ŭ����
class CTCmdInputName : public CTCommand
{
public:
	CTCmdInputName(){}
	virtual ~CTCmdInputName(){}
	void SetName( const char* pszName );
	virtual bool Exec( CTObject* pObj );
protected:
	std::string  m_name;
};

/// �������� â���� ���� ����� �����ϴ��� ������ �Ƿڿ�û
class CTCmdInputNameGiftReceiver : public CTCmdInputName
{
public:
	CTCmdInputNameGiftReceiver(){}
	virtual ~CTCmdInputNameGiftReceiver(){}
	virtual bool Exec( CTObject* pObj );
};

/// �������� â���� �������� �ٸ� �������� ������.
class CTCmdGiftMallItem : public CTCommand
{
public:
	CTCmdGiftMallItem( BYTE slotindex, const char* name );
	virtual ~CTCmdGiftMallItem(){}
	virtual bool Exec( CTObject* pObj );
protected:
	BYTE		m_slotindex;
	std::string m_name;
};

/// Ŭ����ũ�� ����Ѵ�.( ������ )
class CTCmdRegisterClanMark : public CTCommand
{
public:
	CTCmdRegisterClanMark( int clan_id, const char* filename );
	virtual ~CTCmdRegisterClanMark(){}
	virtual bool Exec( CTObject* pObj );
protected:
	int		m_clan_id;
	std::string m_filename;
};

/// IT_MGR�� STATE�� �����Ѵ�.
class CTCmdChangeStateInterface : public CTCommand
{
public:
	CTCmdChangeStateInterface( int state );
	virtual ~CTCmdChangeStateInterface(){}
	virtual bool Exec( CTObject* pObj );
protected:
	int		m_state;
};

#endif