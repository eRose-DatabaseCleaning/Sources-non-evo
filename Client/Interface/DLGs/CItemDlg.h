#ifndef _CItemDlg_
#define _CItemDlg_
#include "tdialog.h"

#include <map>
#include "subclass/CSlot.h"
#include "../../gamecommon/IObserver.h"
#include "../../CClientStorage.h"

#include "IActionListener.h"
class CDragItem;
class CTCmdNumberInput;
/**
* ������ �����۹� ������ �������� ������ �����ִ� ���̾�α�
*	- �����ʹ� ������ Ŭ���̾�Ʈ������ �κ��丮�� �������� ��ġ�� �̵���ų�� ������ ����Ǿ� ����.( �翬�� �̵��� �������� ��ġ�� ���� PC������ ����ȴ� )
*	- Observable : CItemSlot
*
* @Author		������
* @Date			2005/9/12
*/
class CItemDlg : public CTDialog, public IObserver, public IActionListener
{
public:
	CItemDlg( int iType );
	virtual ~CItemDlg(void);

	virtual void Draw();
	virtual void Update( POINT ptMouse );
	virtual void Show();
	virtual void MoveWindow( POINT pt );
	virtual unsigned	Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );


	virtual void		Update( CObservable* pObservable, CTObject* pObj );


	virtual unsigned	ActionPerformed( CActionEvent* e );


	bool		IsInsideInven( POINT pt );											/// ��ǥ�� �κ��丮 ���� ����	���ΰ�?
	bool		IsInsideEquip( POINT pt );											/// ��ǥ�� ���� ���� ���� ���ΰ�?

	
	int			GetEquipSlot( POINT pt );											/// ��ǥ�� ��� ����â ���� ��ȣ�� ���Ѵ�.
	CSlot*		GetInvenSlot( POINT pt );											/// ��ǥ�� �κ��丮 ���� ��ȣ�� ���Ѵ�.

	void		ApplySavedVirtualInventory( std::list<S_InventoryData>& list );
	void		GetVirtualInventory( std::list<S_InventoryData>& list );

	void		AddActionEventListener2Slots( );
	void		RemoveActionEventListener2Slots( );

private:
	void		SwitchIcon( int iReal, int iVirtual );								/// ���� �κ��丮 �ε����� ���� �κ��丮�ε����� ������ ��ġ �̵�
	CSlot*		GetInvenSlotByRealIndex( int iIndex );								/// ���� �κ��丮 �ε����� ���� �ε����� ���Ѵ�.
	
	void		OnLButtonDown( unsigned uiProcID, WPARAM wParam, LPARAM lParam );	
	void		OnLButtonUp( unsigned uiProcID, WPARAM wParam, LPARAM lParam );		
	
	bool		ProcessSlots( unsigned uiMsg, WPARAM wParam, LPARAM lParam );		/// ���Ե��� CTDialog::Processó��

	CWinCtrl*	FindChildInPane( unsigned uiPaneID, unsigned uiChildID );
	void		Minimize();															/// �ּ�ȭ ó��
	void		Maximize();															/// �ִ�ȭ ó��


	bool		IsAvailableRepair(  CIcon* pIcon  );								/// ���� ������ �������ΰ�?



private:
	enum{
		IID_BTN_CLOSE				= 10,
		IID_BTN_ICONIZE				= 11,
		IID_BTN_MONEY				= 12,
		IID_BTN_EQUIP_PAT			= 23,
		IID_BTN_EQUIP_AVATAR		= 33,
		IID_TABBEDPANE_INVEN_ITEM	= 50,
		IID_BTN_INVEN_EQUIP			= 53,
		IID_BTN_INVEN_USE			= 63,
		IID_BTN_INVEN_ETC			= 73,
		IID_TABBEDPANE_INVEN_PAT	= 100,
		IID_PANE_EQUIP				= 200,
		IID_BTN_MINIMIZE			= 213,
		IID_BTN_MAXIMIZE			= 214,
		IID_PANE_INVEN				= 300
	};
	int		m_iEquipTab;													/// 0 ~ 1
	int		m_iInventoryTab;												/// 0 ~ 2

	CSlot	m_AvatarEquipSlots[MAX_EQUIP_IDX-1];							/// ĳ���� ������ ���� ����
	CSlot	m_BulletEquipSlots[MAX_SHOT_TYPE];								/// �Ҹ�ź ������ ����

	CSlot	m_PatEquipSlots[MAX_RIDING_PART];								/// PAT ������ ���� ����

	CSlot	m_ItemSlots[MAX_INV_TYPE][INVENTORY_PAGE_SIZE];					/// ��� �κ��丮 ������ ����


	CDragItem*		m_pInvenDragItem;										/// �κ��丮�� ������ ������ �巡�� ������
	CDragItem*		m_pEquipDragItem;										/// �������� ������ ������ �巡�� ������

	CTCmdNumberInput*	m_pCmdDropMoney;
	CTCmdNumberInput*	m_pCmdAddMyMoney2Exchange;

	bool				m_bMinimize;										/// �ּ�ȭ or �ִ�ȭ

};
#endif