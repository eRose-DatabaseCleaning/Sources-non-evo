#ifndef _CItem_
#define _CItem_

#include <list>
#include "../Common/CItem.h"
#include "../../TGameCtrl/TObject.h"
#include "../interface/CInfo.h"

class CIcon;
class CTCommand;
class CItemFragment;
class CItem : public CTObject
{
	friend	CItemFragment;
public:
	CItem(void);
	CItem(tagITEM* pItemData );
	virtual ~CItem(void);

	virtual void		GetToolTip( CInfo& ToolTip,  DWORD dwDialogType, DWORD dwType );
	void				GetToolTipRidePart_BattleCart( CInfo& ToolTip,  DWORD dwDialogType, DWORD dwType );
	virtual int			GetQuantity();
	virtual tagITEM&	GetItem();
	virtual int			GetIndex();
	virtual bool		IsEnable();


	int				GetType();
	void			Excute();
	void			Clear();
	
	void			SetIndex( int iIndex );


	void			SetIndexType( int iType );
	int				GetIndexType();
	void			SubQuantity( int iQuantity );
	void			SetLife( short nLife );
	bool			IsEmpty();
	
	const char*		GetName();
	int				GetItemNo();
	bool			IsEnableDupCNT();
	CIcon*			CreateItemIcon();

	//int			SubItemQuantity( int iQuantity ); 

	void		SetDefaultIcon( CIcon* pIcon );


	void		SetItem( tagITEM& Item );
	void		SetCommand( CTCommand* pCmd );
	CTCommand*	GetCommand();

	void		SetLastExecTime( DWORD dwTime );
	DWORD		GetLastExecTime();

	virtual void		SetUnitPrice( int iPrice );
	virtual int			GetUnitPrice();

	static	int			GetItemRareType( int type, int no );
	static  DWORD		GetItemNameColor( int type, int no );
	///INDEX TYPE
	enum{
		IT_NONE,
		IT_INVENTORY,
		IT_STORE,
	};
protected:

	///<-ToolTip������ Method
	void AddItemName( tagITEM& sItem, CInfo& Info );
	void AddItemWeight( tagITEM& sItem, CInfo& Info );
	void AddItemPatUseFuelRate( tagITEM& sItem, CInfo& Info );
	void AddItemPatAttackRange( tagITEM& sItem, CInfo& Info );
	void AddItemPatAttackPowSpeed( tagITEM& sItem, CInfo& Info );
	void AddItemPatCartGauage( tagITEM& sItem, CInfo& Info );
	void AddItemPatDesc( tagITEM& sItem, CInfo& Info );
	void AddItemPatVer( tagITEM& sItem, CInfo& Info );
	void AddItemPatClass( tagITEM& sItem, CInfo& Info );
	void AddItemPatEquipRequireJob( tagITEM& sItem, CInfo& Info );

	void AddItemUseNeedData( tagITEM& sItem, CInfo& Info );
	void AddItemMoveSpeed( tagITEM& sItem, CInfo& Info );
	void AddItemSpecialJob( tagITEM& sItem, CInfo& Info );
	void AddItemSpecialUnion( tagITEM& sItem, CInfo& Info );
	void AddItemEquipRequireJob( tagITEM& sItem, CInfo& Info );
	void AddItemEquipRequireUnion( tagITEM& sItem, CInfo& Info );
	void AddItemEquipRequireCondition( tagITEM& sItem, CInfo& Info );
	void AddItemDefaultBottomText( tagITEM& sItem, CInfo& Info );
	void AddItemDefaultTopTextDefenceItem( tagITEM& sITEM ,CInfo& Info );
	void AddItemDefence( tagITEM& sItem , CInfo& Info );
	void AddItemDefaultTopText( tagITEM& sItem, CInfo& Info );
	void AddItemOption( tagITEM& sItem, CInfo& Info );
	void AddItemPrice( tagITEM& sItem, DWORD dwDlgType, DWORD dwType, CInfo& Info );
	void AddItemLifeDuration( tagITEM& sItem, CInfo& Info );
	void AddItemPatAddAbility( tagITEM& sItem, CInfo& Info );




	short GetAttackSpeedType( short nSpeed );	// ���� ���ǵ� Ÿ��( �ſ����, ����... )

	void GetAttackSpeedString( short nSpeed, std::string& strMsg );
	int	 GetAttackPower( tagITEM& sItem );
	int	 GetDefence( tagITEM& sItem );
	int	 GetOption( tagITEM& sItem, short nOption, short nCol );
	const char* GetAddDataValueString( tagITEM& sItem, int iColumn );

	void	SetEnable();
	void	SetDisable();

protected:
	DWORD					m_dwLastExecTime;
	CTCommand*				m_pCommand;
	
	int						m_iIndex;
	int						m_iIndexType;

	CIcon*					m_pDefaultIcon;
	int						m_iStateLocation;
	int						m_iStateAction;
	tagITEM					m_Item;				///Location State�� LS_NONE�ϰ�� ���ȴ�.

	///�κ��丮�� �ִ� �������� �ٸ������� �κ������� ������̶�� ���Ұ��� �Ѵ�.
	/// ItemCommand, IconCommand, SlotCommand�� ��� ���´�.
	bool					m_bEnable;
	int						m_iUnitPrice;
};

///*----------------------------------------------------------------------------------/
/// �κ��丮���� �������� ������ ������ ������( ��: �Ÿ�, ��ȯ, ����â�� �÷��� ������ )
/// ��κ��� ����� ���������� ���� ��� ������ ó���ϰ� �ϴ� ������ 
/// ���� �ſ� �Ҿ��� Ŭ���� �̴�. ��Ȳ�� ���� CItem�� �ٸ� Method���� ���������־�� �Ұ��̴�.
/// . Constructor���� param���� ���� CItem�� SetDisable�ϰ� Destuctor���� SetEnable��Ų��.
class CItemFragment : public CItem
{
public:
	CItemFragment( CItem* pItem );
	virtual ~CItemFragment();

	virtual void		GetToolTip( CInfo& ToolTip,  DWORD dwDialogType, DWORD dwType );
	virtual int			GetQuantity();
	virtual tagITEM&	GetItem();
	virtual int			GetIndex();
	virtual bool		IsEnable();
	virtual void		SetUnitPrice( int iPrice );
	virtual int			GetUnitPrice();

	void			SetQuantity( int iQuantity );
	//void			SetUnitPrice( int iUnitPrice );
	//int				GetUnitPrice();

	CItem*			GetInnerCItem();
	CIcon*			CreateItemIcon();


protected:
	//int			m_iUnitPrice;
	int			m_iQuantity;
	CItem*		m_pItem;
};
#endif  