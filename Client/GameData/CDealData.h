#ifndef _CDealData_
#define _CDealData_
#include "../gamecommon/cobservable.h"
#include "../gamecommon/iobserver.h"

enum {
	DEAL_SELL,		//0,�Ǹ� 
	DEAL_BUY,		//1.���� 
	MAX_DEAL_SELLBUY
};

#define MAX_DEAL_X				5
#define MAX_DEAL_Y				2
#define TOTAL_DEAL_INVENTORY	MAX_DEAL_X * MAX_DEAL_Y


class CItemFragment;
class CItem;
class CTEventDeal;


/**
* NPC ���� �̿�� 2���̻��� �������� ����/�Ǹ��ϱ� ���� �������̽��� Data Class
*	- �̱���
*	- Observable	: CItemSlot
*	- Observer		: CDealDLG
*
* @Author		������
* @Date			2005/9/15
*/
class CDealData : public CObservable, public IObserver
{
	CDealData(void);
public:
	static CDealData& GetInstance();
	virtual ~CDealData(void);

	virtual void Update( CObservable* pObservable, CTObject* pObj );

	void AddItem2BuyList( int iIndex, int iQuantity );
	void AddItem2SellList( int iIndex, int iQuantity );
	void RemoveItemFromBuyList( CItem* pItem );
	void RemoveItemFromSellList( CItem* pItem );

	void ClearTradeList();

	void SendTradeReq();

	unsigned GetTradeItemCnt();


	long GetTotal_BUY();
	long GetTotal_SELL();

private:
	unsigned GetBuyItemCnt();
	unsigned GetSellItemCnt();

private:
	std::vector< CItemFragment* > m_BuyItems;
	std::vector< CItemFragment* > m_SellItems;

	CTEventDeal*		m_pEvent;
};
#endif