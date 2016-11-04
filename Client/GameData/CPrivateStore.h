#ifndef _CPrivateStore_
#define _CPrivateStore_
#include "../GameCommon/CObservable.h"
#include "../GameCommon/IObserver.h"
#include "../Common/CItem.h"
#include "../Common/CUserData.h"
#include "Net_Prototype.h"
class CItem;
class CItemFragment;
struct t_PACKET;
/**
* ���λ����� Data Class
* - ���λ����� UI�� Show�ÿ� Observer�� ����ϰ� Hide�ÿ� �����Ѵ�.
* - Observer�� ������� WishList�� �������� ���Ը���Ʈ�� �߰��ϰ�, BuyList, SellList( in UI )�� Clear�Ѵ�.
* - CPrivateStore�� Buy, Sell List�� UI�� Show or Hide�ÿ� ������ Clear���ش�.
* - �ǸŹ�ǰ�� �ȷȰų�, ���Թ�ǰ�� ������ �Ϸ�Ǿ��� ��츦 �������
* - �ǸŹ�ǰ�� �ȷȴٸ� �ǸŸ���Ʈ���� ���� ���Թ�ǰ�� ���Եȴٸ� ������ ���� ���Ը���Ʈ���� ����.
*		-- CTEventItem : EID_DEL_ITEM,EID_CHANGE_ITEM�� ó���ϰ� CItemSlot�� Observer�� Open�ÿ� ���, Close�ÿ� �����Ѵ�.
* - ����Ʈ�� �ִ� �������� ������ �ִ� ���� �������� ��쿡 ���ؼ��� �����غ���
*
* - ���λ����� �ڽ��� ������ �̿��Ҷ��� �����ؾ� �Ѵ�.( Data or UI ?)
*		-- �ϴ� Dialog�� 2���� ������ �ۼ��Ѵ�.( ������ �̿� 2���� )
*		-- Data�� CPrivateStore�� ���°����� �ϰ� �̿뿡 ��������.( WishList�� �׻� �����ϰ�, BuyList�� SellList�� Clear���ش�.)
*
* - ��Ʈ��ũ( 2004 / 6 / 9 )
*		-- ���λ��������� �ǸŹ�ǰ 
*			--- �����ڿ��Դ� gsv_SET_MONEYnINV �� gsv_P_STORE_RESULT�� �´�.
*			--- ���λ��������ڿ��Դ� gsv_P_STORE_RESULT�� �´�. ->���⼭ �κ��丮���� ����, ���� �ǸŸ���Ʈ������
*		-- ���λ��������� ���Թ�ǰ
*			--- �̿��ڿ� �����ڿ��� gsv_SET_MOENYnINV�� gsv_P_STORE_RESULT�� ��� �´�.
*
*
* @Author		������
* @Date			2005/9/15
*/
class CPrivateStore : public CObservable, public IObserver
{
	CPrivateStore(void);
	~CPrivateStore(void);
public:
	static CPrivateStore& GetInstance();

	virtual void Update( CObservable* pObservable, CTObject* pObj );

	void	AddItemSellList( CItem* pItem, int iQuantity, int iUnitPrice );
	void	SortItemSellList();
	void	RemoveItemSellList( int iIndex );
	CItem*	GetItemSellList( int iIndex );
	void	UpdateSellList( tagPS_SLOT_ITEM& SlotITEM );
	void	UpdateBuyList( tagPS_SLOT_ITEM& SlotITEM );


	void	AddItemBuyList( int iIndex, int iUnitCost, int iQuantity );
	void	RemoveItemBuyList( int iIndex );
	//CItem*	GetItemBuyList( int iIndex );


	void	AddItemWishList( tagITEM& Item , bool bSendPacket, int iSlotIndex = -1 );
	void	RemoveItemWishList( int iIndex );

	void	ClearSellList();
	void	ClearBuyList();


	bool	Open();
	void	Close();
	void	Clear();

	const	char* GetTitle();
	void	SetTitle( const char* pszTitle );

	CItem*	GetWishItem( int iIndex );
	bool	IsOpened();
	bool	IsValidOpen();
private:


private:
	std::string			m_strTitle;
	
	std::vector< CItemFragment* >		m_SellItems;///�Ǹ� ������ ����Ʈ
	/// ���� ������ ����Ʈ
	std::list< CItemFragment* >			m_BuyItems; 
	CItem* 								m_WishItems[MAX_WISH_ITEMS];///�򸮽�Ʈ - �׻� ����
	bool				m_bOpened;	///���� ������ �����ִ°�?
	
};
#endif