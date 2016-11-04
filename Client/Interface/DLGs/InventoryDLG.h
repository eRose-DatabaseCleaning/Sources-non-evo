
#ifndef _INVENTORY_DLG_H
#define _INVENTORY_DLG_H

#include "../../../TGameCtrl/TDialog.h"
#include "../../gamecommon/IObserver.h"
#include "../Icon/CIconItem.h"
#include "subclass/CSlot.h"

// *--------------------------------------------------------------------* //
// �κ��丮 ������ 
// *--------------------------------------------------------------------* //
enum invDLG {
	INV_IMG,							//0,�ǳ�
	INV_BTN_CLOSE = 3,					//1,�ݱ� 
	INV_BTN_TAB1  = 5,					//2,��1
	INV_BTN_TAB2  = 6,					//3,��2
	INV_BTN_TAB3  = 7,					//4,��3
	INV_BTN_MONEY = 10,					//���� �̵��� ���Ǵ� ��ư
	INV_MAX_BTN_CNT
};
class CDragItem;
class CTCmdNumberInput;

class CInventoryDLG : public CTDialog
{
private:
	CSlot				m_ItemSlots[ MAX_INV_TYPE ][ INVENTORY_PAGE_SIZE ];
	short				m_nInvType;	///< Item type[ wasting item, Equipment item, .. ]	
	
public:
	CInventoryDLG( int iType );
	virtual ~CInventoryDLG();

	virtual void Draw();
	virtual void MoveWindow( POINT ptMouse );
	virtual unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);
	virtual void Hide();
	virtual void Show();
	virtual void Update( POINT pt );

	enum{
		INVEN_TYPE_EQUIP,
		INVEN_TYPE_USE,
		INVEN_TYPE_ETC
	};

	void	MoveIcon( int iInvenIdx );
	void	MoveIcon( int iVirtualInvenIdx1 , int iVirtualInvenIdx2 ,int iInvenType );
/*
	void	UpdateInventory();	
	/// ������ ���..
	bool	UseItem( short nRealIndex );
*/
	///���ڸ� �̿��ؼ� �ش��ϴ� ù��° �������� ���� �κ��丮 �ε����� ���Ѵ�.
	///@return  
	///	0 > :Real Inventory Index, 
	///	-1	:NotFound
	int		FindItem( int iItemType , int iItemNo );///��з��� ������ No( STB Line No )�� ã��
	int		FindItem( int iClass );///3�ڸ��� �ߺз��� ã��


private:
	bool	On_LButtonUP( unsigned iProcID, WPARAM wParam, LPARAM lParam );
	void	UpdateSlotPosition( int iType );

/*	POINT	GetVirtualSlotIdxByRealInvenIdx(int iInvenIdx );
	short   GetVirtualSlotIdxByRealInvenIdxNType( int iType, int iInvenIdx );
	int		GetVirtualSlotIdxByPoint( POINT pt );
	int		GetVirtualEmptySlot( int iInvenType );

	short	GetMouse_PointITEM(tagITEM& sITEM,POINT ptPoint);

	void	CreateVirtualInventory();
	void    SaveVirtualInventoryTable();
*/
	void	DrawWeightANDGold();
/*
	/// ������ ���䱸�� ������Ű�°�?
	bool	CheckBUseItem( short nSelectNo );
	*/
	CDragItem*	m_pDragItem;

	CTCmdNumberInput*	m_pCmdDropMoney;
	CTCmdNumberInput*	m_pCmdAddMyMoney2Exchange;

};




#endif