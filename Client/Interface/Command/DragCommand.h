#ifndef _DragCommand_
#define _DragCommand_
#include "tcommand.h"

#include "../InterfaceType.h"

/// �κ��丮���� �и�/���� â���� ������ �̵�
class CTCmdDragItem2SeparateDlg : public CTCommand
{
public:
	CTCmdDragItem2SeparateDlg(void){}
	virtual ~CTCmdDragItem2SeparateDlg(void){}
	virtual bool Exec( CTObject* pObj );
};

/// �������� ���ٿ� ���
class CTCmdDragInven2QuickBar : public CTCommand
{
public:
#ifdef _NEW_UI
		CTCmdDragInven2QuickBar(void){	m_nType = DLG_TYPE_QUICKBAR;	}
	CTCmdDragInven2QuickBar( short nType )	{	m_nType = nType;	}
#else
	CTCmdDragInven2QuickBar(void){};
	virtual ~CTCmdDragInven2QuickBar(void){};
#endif
	virtual bool	Exec( CTObject* pObj );
	short			m_nType;
};

/// ��ų�� ���ٿ� ���
class CTCmdDragSkill2QuickBar : public CTCommand
{
public:

#ifdef _NEW_UI
	CTCmdDragSkill2QuickBar(void){	m_nType = DLG_TYPE_QUICKBAR;	};
	CTCmdDragSkill2QuickBar( short nType )	{	m_nType = nType;	}
#else
		CTCmdDragSkill2QuickBar(void){	m_nType = DLG_TYPE_QUICKBAR;	}
	CTCmdDragSkill2QuickBar( short nType )	{	m_nType = nType;	}
#endif

	virtual ~CTCmdDragSkill2QuickBar(void){}
	virtual bool	Exec( CTObject* pObj );
	short			m_nType; 
};


/// Ŭ�� ��ų��  ���ٿ� ��ų ���
class CTCmdDragClanSkill2QuickBar : public CTCommand
{
public:
	CTCmdDragClanSkill2QuickBar(void){};
	virtual ~CTCmdDragClanSkill2QuickBar(void){}
	virtual bool	Exec( CTObject* pObj );
};

/// ���ٿ��� ������(��ų�� ����)�� ����.
class CTCmdDragItemFromQuickBar : public CTCommand
{
public:
#ifdef _NEW_UI
		CTCmdDragItemFromQuickBar(void){	m_nType = DLG_TYPE_QUICKBAR;	}
	CTCmdDragItemFromQuickBar( short nType )	{	m_nType = nType;	}
#else
	CTCmdDragItemFromQuickBar(void){};
	virtual ~CTCmdDragItemFromQuickBar(void){};
#endif
	virtual bool	Exec( CTObject* pObj );
	short			m_nType;
};

/// ���پȿ��� ��ϵ� ������(��ų�� ����)�� ��ġ �̵��Ѵ�.
class CTCmdMoveIconInQuickBar : public CTCommand
{
#ifdef _NEW_UI
	public:
	CTCmdMoveIconInQuickBar(void){	m_nType = DLG_TYPE_QUICKBAR;	}
	CTCmdMoveIconInQuickBar( short nType )	{	m_nType = nType;	}
	
	virtual ~CTCmdMoveIconInQuickBar(void){};
	virtual bool	Exec( CTObject* pObj );
	short			m_nType;
#else
public:
	CTCmdMoveIconInQuickBar(void){};
	virtual ~CTCmdMoveIconInQuickBar(void){};
	virtual bool	Exec( CTObject* pObj );

#endif
};


/// �κ��丮���� ĳ���� â������ ����
class CTCmdDragItemEquipFromInven : public CTCommand
{
public:
	CTCmdDragItemEquipFromInven(void){}
	virtual ~CTCmdDragItemEquipFromInven(void){}
	virtual bool Exec( CTObject* pObj );
};


/// ������â �ȿ��� ������ �������� Drag�ؼ� Ż��
class CTCmdDragItemFromEquipInItemDlg : public CTCommand
{
public:
	CTCmdDragItemFromEquipInItemDlg(void){}
	virtual ~CTCmdDragItemFromEquipInItemDlg(void){}
	virtual bool Exec( CTObject* pObj );
};

/// ������â �ȿ��� �κ��丮�� �ִ� �������� Drag�ϴ� Command
class CTCmdDragItemFromInvenInItemDlg : public CTCommand
{
public:
	CTCmdDragItemFromInvenInItemDlg(void){}
	virtual ~CTCmdDragItemFromInvenInItemDlg(void){}
	virtual bool Exec( CTObject* pObj );

};

/// ������â�� �κ��丮���� ���� ������ ���λ������� �������� �̵��ϴ� Command
class CTCmdDragItem2PrivateStoreDlg : public CTCommand
{
public:
	CTCmdDragItem2PrivateStoreDlg(void){}
	virtual ~CTCmdDragItem2PrivateStoreDlg(void){}
	virtual bool Exec( CTObject* pObj );
};

/// WishList�� ������ �߰�
class CTCmdDragItem2WishList : public CTCommand
{
public:
	CTCmdDragItem2WishList(void){}
	virtual ~CTCmdDragItem2WishList(void){}
	virtual bool Exec( CTObject* pObj );
};

/// ���� ������ ���λ������� �Ǹ� ��ǰ�� ���� Command
class CTCmdDragSellItemFromPrivateStoreDlg : public CTCommand
{
public:
	CTCmdDragSellItemFromPrivateStoreDlg(void){}
	virtual ~CTCmdDragSellItemFromPrivateStoreDlg(void){}
	virtual bool Exec( CTObject* pObj );
};

/// ���� ������ ���λ������� ���� ��ǰ�� ���� Command
class CTCmdDragBuyItemFromPrivateStoreDlg : public CTCommand
{
public:
	CTCmdDragBuyItemFromPrivateStoreDlg(void){}
	virtual ~CTCmdDragBuyItemFromPrivateStoreDlg(void){}
	virtual bool Exec( CTObject* pObj );
};

/// �ٸ� �ƹ�Ÿ�� ���λ������� ��ǰ�� �춧 �巡���ϴ�   Command
class CTCmdDragSellItemFromAvatarStoreDlg : public CTCommand
{
public:
	CTCmdDragSellItemFromAvatarStoreDlg(void){}
	virtual ~CTCmdDragSellItemFromAvatarStoreDlg(void){}
	virtual bool Exec( CTObject* pObj );
};


/// ���ÿ��� �巡�׾ص������ Ÿ�� �������� ���� Command
class CTCmdDragTakeoutTargetItemFromUpgradeDlg : public CTCommand
{
public:
	CTCmdDragTakeoutTargetItemFromUpgradeDlg(){}
	virtual ~CTCmdDragTakeoutTargetItemFromUpgradeDlg(){}
	virtual bool Exec( CTObject* pObj );
};


/// ���ÿ��� �巡�׾ص������ ��� �������� ���� Command
class CTCmdDragTakeoutMaterialItemFromUpgradeDlg : public CTCommand
{
public:
	CTCmdDragTakeoutMaterialItemFromUpgradeDlg(){}
	virtual ~CTCmdDragTakeoutMaterialItemFromUpgradeDlg(){}
	virtual bool Exec( CTObject* pObj );
};

/// ������ â���� ����â���� �������� �巡�׾ص���ϴ� Command
class CTCmdDragTakeinItem2UpgradeDlg : public CTCommand
{
public:
	CTCmdDragTakeinItem2UpgradeDlg(){}
	virtual ~CTCmdDragTakeinItem2UpgradeDlg(){}
	virtual bool Exec( CTObject* pObj );
};
#endif