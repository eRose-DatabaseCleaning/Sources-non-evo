#ifndef _CDeliveryStoreDlg_
#define _CDeliveryStoreDlg_

#include "tdialog.h"
#include "subclass/cslot.h"

const int c_deliverystore_slot_count = 48;

class CDragItem;
struct tagBaseITEM;



/**
* �ѱ������� ���ϸ��� ������ â��( �������� â��) �� ���̾�α�
*
* @Warning		������ ����� �̸��� ������ ���� �ִ��� üũ�ؼ� �ִٴ� ��Ŷ�� �ް� �������ΰ��� �������� �ٽ� ����� �����̴�.
* @Author		������
* @Date			2005/9/14
*/
class CDeliveryStoreDlg : public CTDialog
{
public:
	CDeliveryStoreDlg(void);
	virtual ~CDeliveryStoreDlg(void);
	virtual bool Create( const char* IDD );
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual void Update( POINT ptMouse );
	virtual void MoveWindow( POINT pt );
	virtual void Draw();
	virtual void Show();
	virtual void Hide();

	void	AddItem( tagBaseITEM* pItem, const char* pszFrom, const char* pszDesc, const char* pszTo );
	void	SetItem( int slotindex, tagBaseITEM& Item );
	void	ClearItem(  );

	void	save_receiver_name( const char* pszName );
	int		get_selecteditem_slotindex();
	const char* GetSelectedItemName();
	const char* get_receiver_name();

private:
	enum{
		IID_BTN_CLOSE   = 20,
		IID_BTN_GIFT	= 21
	};

	CDragItem*					m_pDragItem;
	CSlot						m_Slots[c_deliverystore_slot_count];
	int							m_emptyslot;

	std::string					m_receiver_name;						/// �ٸ� �������� 
	short						m_npc_client_object_index;
};

#endif