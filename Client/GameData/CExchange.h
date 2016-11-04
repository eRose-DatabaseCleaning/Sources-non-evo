#ifndef _CExchange_
#define _CExchange_
#include "../gamecommon/CObservable.h"
#include "../gamecommon/IObserver.h"
#include "../common/citem.h"

#define MAX_EXCHANGE_ROW				2
#define MAX_EXCHANGE_COL				5
#define TOTAL_EXCHANGE_INVENTORY	MAX_EXCHANGE_ROW * MAX_EXCHANGE_COL

typedef struct t_exchange_item
{
	tagITEM		m_sItem;		/// ������ 
	short		m_nInvenIdx;
	int			m_iQuantity;
	t_exchange_item(){
		ZeroMemory( this, sizeof( t_exchange_item ));
		m_nInvenIdx = 0;
		m_iQuantity = 0;
	}
}t_exchange_item;


const int	COUNT_ROW_SLOT	= 3;

class CTEventExchange;
class CItemFragment;
class CItem;
/**
* ������ 1:1 ��ȯ�� ���Ǵ� Data Class
*	- �̱���
*	- Observer		: CExchangeDLG
*	- Observable	: CItemSlot
*
* @Author		������
* @Date			2005/9/15
*/
class CExchange : public CObservable, public IObserver
{
	CExchange(void);
public:
	static CExchange& GetInstance();
	virtual ~CExchange(void);
	virtual void Update( CObservable* pObservable, CTObject* pObj );

	
	const std::string&	GetOtherName();
	__int64				GetMyTradeMoney();
	__int64				GetOtherTradeMoney();
	void				SetMyTradeMoney( __int64 i64Money );

	void				SetReadyMe( bool bReady );
	void				SetReadyOther( bool bReady );
	bool				IsReadyMe();
	bool				IsReadyOther();
	///���� ������ ��� �غ�����ΰ�?
	bool				IsReadyAll();


	///���� ��ȯ���ΰ��� Flag
	void				StartExchange( WORD wServerObjectIdx, const char* pszName );
	void				EndExchange();
	bool				IsExchange();

	///�ٸ� �������̽����� ���Ǵ� 
	//bool				IsItemInTradeList( short iItemInvenIdx );
	//int					GetItemQuantityInTradeList( short iItemInvenIdx );

	//bool				IsEnoughEmptyInvenSlots( tagITEM& item );
	///

	/// Network�� Method;
	bool				SendCliTradeReq( WORD wObjSvrIdx );
	void				SendTradePacket( BYTE btResult, char cSlotIdx = 0 );



	void				AddMyItem( CItem* pItem, int iQuantity );
	void				RemoveMyItemBySlotIndex( int iSlotIndex ,bool bSendPacket = true );
	void				RemoveMyItemByInvenIndex( int iInvenIndex );
	bool				UpdateOtherItem( int iIndex, tagITEM& item );

private:
	void				Clear();

	///	Ʈ���̵� �Ϸ�� ���� �κ��丮�� �߰��� �������� ����:��ȯ�� �κ��丮�� ������ �������� ������ ���°�� ����
	int					GetCountTradeItem(t_InvTYPE InvenType );

	//CItem*				CreateItem( tagITEM& Item );

private:
	CItemFragment*		m_MyItems[TOTAL_EXCHANGE_INVENTORY];
	CItem*				m_OtherItems[TOTAL_EXCHANGE_INVENTORY];

	bool				m_bExchange;///��ȯ���ΰ�?
	bool				m_bReadyMe;
	bool				m_bReadyOther;

	__int64				m_i64MyTradeMoney;
	__int64				m_i64OtherTradeMoney;
	
	WORD				m_wOtherServerObjectIdx;
	std::string			m_strTargetAvatarName;

	CTEventExchange*	m_pEventExchange;
};
#endif