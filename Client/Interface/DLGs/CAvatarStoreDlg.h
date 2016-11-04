#ifndef _CAvatarStoreDlg_
#define _CAvatarStoreDlg_
#include "TDialog.h"
#include "subclass/CSlot.h"
#include "Net_Prototype.h"
#include "../../GameCommon/CObservable.h"
#include "../../GameData/Event/CTEventAvatarStore.h"


#include "SinglelineString.h"


const int c_iAvatarStoreMaxSlotCount = 30;
class CDragItem;
class CItem;
class CTCmdNumberInput;

/**
* �ٸ� ������ ������ �̿��Ҷ� ���Ǵ� Dialog( UI + Data )
*	- Observer : CDragNDropMgr
*
* @Author		������
* @Date			2005/9/12
*/
class CAvatarStoreDlg :	public CTDialog, public CObservable
{
public:
	CAvatarStoreDlg( int iDlgType );
	virtual ~CAvatarStoreDlg(void);
	virtual void Show();
	virtual void Hide();
	virtual void Draw();
	virtual void MoveWindow( POINT pt );
	virtual void Update( POINT ptMouse );
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

	void	SetMasterSvrObjIdx( WORD wSvrObjIdx );
	WORD    GetMasterSvrObjIdx();
	void	SetTitle( const char* pszTitle );
	
	void    AddItem2SellList( tagPS_SLOT_PRICE& SlotITEM );						/// �������� ���� �Ǹ� ��� ������ �߰�
	void	AddItem2BuyList( tagPS_SLOT_PRICE& SlotITEM );						/// �������� ���� ���� ��� ������ �߰�

	void    UpdateSellList( tagPS_SLOT_ITEM& SlotITEM );						/// �Ǹ� ��������� ����Ʈ�� ���������� ������� ������Ʈ
	void	UpdateBuyList( tagPS_SLOT_ITEM& SlotITEM );							/// ���� ��������� ����Ʈ�� ���������� ������� ������Ʈ

	int		IsBuyItem( tagITEM& Item );											/// ���λ����� ���� �ȷ��� �������� �����������Ʈ�� �ִ� �������ΰ�?	

	int		GetWishIndex( tagITEM& Item ,int& iUnitPrice );

private:
	void	OnLButtonUp( unsigned uiProcID, WPARAM wParam, LPARAM lParam );
	void	OnLButtonDown( unsigned uiProcID, WPARAM wParam, LPARAM lParam );

private:
	enum{
		IID_BTN_CLOSE	= 20,
		IID_RADIOBOX	= 30,
		IID_BTN_SELL	= 31,
		IID_BTN_BUY		= 32,
	};
	int					m_iTab;										/// ���� ����/�Ǹ��� ���� ����

	CSinglelineString	m_title;									/// ���� ����
	WORD				m_wMasterSvrObjIdx;							/// ���������� �����ε���

	CSlot				m_SellSlots[c_iAvatarStoreMaxSlotCount];	/// �Ǹ������������ �������� Attach�� ���Ե� 
	CSlot				m_BuySlots[c_iAvatarStoreMaxSlotCount];		/// ��������������� �������� Attach�� ���Ե�

	CItem*				m_SellItems[c_iAvatarStoreMaxSlotCount];	/// �Ǹ���� �����۵�
	CItem*				m_BuyItems[c_iAvatarStoreMaxSlotCount];		/// ������� �����۵�

	CDragItem*			m_pSellDragItem;							/// �巡��&����� ���� CDragItem


	CTCmdNumberInput*	m_pBuyItemAtAvatarStore;					/// �Ǹ� ��� �������� �����Ҷ� ���� TCommand( ���� �Է±⸦ ���� )

	CTEventAvatarStore  m_Event;									/// Observer���� ���� �̺�Ʈ
};
#endif