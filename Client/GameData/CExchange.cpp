#include "stdafx.h"
#include ".\cexchange.h"
#include "event/CTEventExchange.h"
#include "../gamecommon/item.h"
#include "../Network/CNetwork.h"
#include "../interface/it_mgr.h"
//#include "../gamecommon/citemequip.h"
//#include "../gamecommon/citemuse.h"
//#include "../gamecommon/citemetc.h"
//#include "../gamecommon/citempat.h"
#include "../gamecommon/item.h"
#include "Event/CTEventItem.h"

CExchange::CExchange(void)
{
	m_bExchange = false;
	for( int iSlot = 0; iSlot < TOTAL_EXCHANGE_INVENTORY; ++iSlot )
	{
		m_MyItems[iSlot] = NULL;
		m_OtherItems[iSlot] = NULL;
	}
	m_pEventExchange = new CTEventExchange;
}

CExchange::~CExchange(void)
{
	SAFE_DELETE( m_pEventExchange );
	for( int iSlot = 0; iSlot < TOTAL_EXCHANGE_INVENTORY; ++iSlot )
	{
		SAFE_DELETE( m_MyItems[iSlot] );
		SAFE_DELETE( m_OtherItems[iSlot] );
	}
}

CExchange& CExchange::GetInstance()
{
	static CExchange s_Instance;
	return s_Instance;
}

///��ȯ�� ��û�ϴ� Packet
bool CExchange::SendCliTradeReq( WORD wObjSvrIdx )
{
	return g_pNet->Send_cli_TRADE_P2P( wObjSvrIdx, RESULT_TRADE_REQUEST );
}

const std::string&	CExchange::GetOtherName()
{
	return m_strTargetAvatarName;
}
__int64 CExchange::GetMyTradeMoney()
{
	return m_i64MyTradeMoney;
}

__int64 CExchange::GetOtherTradeMoney()
{
	return m_i64OtherTradeMoney;
}

void CExchange::SetReadyMe( bool bReady )
{
	m_bReadyMe = bReady;
}

void CExchange::SetReadyOther( bool bReady )
{
	m_bReadyOther = bReady;
	m_pEventExchange->SetID( CTEventExchange::EID_CHANGE_READYOTHER );
	m_pEventExchange->SetReadyOther( bReady );
	SetChanged();
	NotifyObservers( m_pEventExchange );
}

bool CExchange::IsReadyMe()
{
	return m_bReadyMe;
}

bool CExchange::IsReadyOther()
{
	return m_bReadyOther;
}

bool CExchange::IsReadyAll()
{
	return ( m_bReadyMe && m_bReadyOther );
}

/////////////////////////////////////////////////////////////////
/// @brief ��ȯ�� ���۵ɶ��� ��ȯ�� �������� Data�� Clear
/////////////////////////////////////////////////////////////////
void CExchange::Clear()
{
	m_bReadyMe		= false;
	m_bReadyOther	= false;
	m_i64MyTradeMoney	= 0;
	m_i64OtherTradeMoney = 0;



	for( int iSlot = 0; iSlot < TOTAL_EXCHANGE_INVENTORY; ++iSlot )
	{
		if( m_MyItems[iSlot] )
		{
			m_pEventExchange->SetID( CTEventExchange::EID_REMOVE_MYITEM );
			m_pEventExchange->SetSlotIndex( iSlot );
			SetChanged();
			NotifyObservers( m_pEventExchange );

			delete m_MyItems[iSlot];
			m_MyItems[iSlot] = NULL;
		}

		if( m_OtherItems[iSlot] )
		{
			m_pEventExchange->SetID( CTEventExchange::EID_REMOVE_OTHERITEM );
			m_pEventExchange->SetSlotIndex( iSlot );
			SetChanged();
			NotifyObservers( m_pEventExchange );

			delete m_OtherItems[iSlot];
			m_OtherItems[iSlot] = NULL;
		}
	}
}

void CExchange::StartExchange( WORD wServerObjectIdx, const char* pszName )
{
	m_bExchange = true;
	assert( pszName );
	m_wOtherServerObjectIdx = wServerObjectIdx;
	m_strTargetAvatarName = pszName;
	Clear();
}

void CExchange::EndExchange()
{
	Clear();
	m_bExchange = false;
	m_wOtherServerObjectIdx = 0;
	m_strTargetAvatarName.clear();
}
//////////////////////////////////////////////////////////////////////////////
/// @brief ���� ��ȯ���ΰ�?
bool CExchange::IsExchange()
{
	return m_bExchange;
}

void CExchange::SendTradePacket( BYTE btResult, char cSlotIdx )
{
	g_pNet->Send_cli_TRADE_P2P( m_wOtherServerObjectIdx, btResult, cSlotIdx );

}
//
//bool CExchange::IsItemInTradeList( short iItemInvenIdx )
//{
//	for( int iSlot = 0; iSlot < TOTAL_EXCHANGE_INVENTORY; ++iSlot )
//		if( m_MyItems[iSlot] && m_MyItems[iSlot]->GetIndex() == iItemInvenIdx )
//			return true;
//
//	return false;
//}
//
//int  CExchange::GetItemQuantityInTradeList( short iItemInvenIdx )
//{
//	for( int iSlot = 0; iSlot < TOTAL_EXCHANGE_INVENTORY; ++iSlot )
//		if( m_MyItems[iSlot] && m_MyItems[iSlot]->GetIndex() == iItemInvenIdx )
//			return m_MyItems[iSlot]->GetQuantity();
//
//	return 0;
//}
//

///�κ�Ÿ�Ժ��� ������ �����۰������� �������� ����( ItemNo�� ������츸 )����
int CExchange::GetCountTradeItem(t_InvTYPE InvenType )
{
	int iCount = 0;
	for( int i = 0; i < TOTAL_EXCHANGE_INVENTORY ; ++i)
	{
		if( m_OtherItems[i] )
		{
			tagITEM& Item = m_OtherItems[i]->GetItem();
			if( CInventory::GetInvPageTYPE( Item ) == InvenType )
				++iCount;
		}
	}

	for( int i = 0; i < TOTAL_EXCHANGE_INVENTORY; ++i )
	{
		if( m_MyItems[i] )
		{
			tagITEM& Item = m_MyItems[i]->GetItem();
			if( CInventory::GetInvPageTYPE( Item ) == InvenType )
				--iCount;
		}
	}
	return iCount;
}

/// �κ��丮�� �󽽷� - ����ȯ�����۰��� >= ���汳ȯ�����۰����� �Ǵ°�?
//bool	CExchange::IsEnoughEmptyInvenSlots( tagITEM& item )
//{
//	t_InvTYPE InvType = CInventory::GetInvPageTYPE( item );
//
//	CInventory* pInven = g_pAVATAR->GetInventory();
//
//	if( pInven->GetEmptyInvenSlotCount( InvType ) >= GetCountTradeItem( InvType ))
//		return true;
//
//	return false;
//}

void CExchange::SetMyTradeMoney( __int64 i64Money )
{
	m_i64MyTradeMoney = i64Money;
	g_pNet->Send_cli_TRADE_P2P_ITEM( TRADE_MONEY_SLOT_NO, 0, (DWORD)i64Money );
}

void CExchange::AddMyItem( CItem* pItem, int iQuantity )
{
	assert( pItem );
	
	tagITEM& Item = pItem->GetItem();

	if( !Item.IsEnableExchange() )
	{
		g_itMGR.AppendChatMsg(STR_DONT_EXCHANGE_ITEM, IT_MGR::CHAT_TYPE_SYSTEM );
		return;
	}

	if(  IsReadyMe() ) 
		return;

	//if( CInventory::GetInvPageTYPE( Item ) == INV_WEAPON )
	//{
	int iSlot = 0;
	///���� �������ϰ�� ����
	for( iSlot = 0; iSlot < TOTAL_EXCHANGE_INVENTORY ; ++iSlot)
		if( m_MyItems[iSlot] && m_MyItems[iSlot]->GetIndex() == pItem->GetIndex() )
			return ;

	for( iSlot = 0; iSlot < TOTAL_EXCHANGE_INVENTORY ; ++iSlot)
	{
		if( m_MyItems[iSlot] == NULL )
		{
			CItemFragment* pFragmentItem = new CItemFragment( pItem );
			pFragmentItem->SetQuantity( iQuantity );

			m_pEventExchange->SetID( CTEventExchange::EID_ADD_MYITEM );
			m_pEventExchange->SetItem( pFragmentItem );
			m_pEventExchange->SetSlotIndex( iSlot );
			
			m_MyItems[iSlot] = pFragmentItem;

			SetChanged();
			NotifyObservers( m_pEventExchange );

			g_pNet->Send_cli_TRADE_P2P_ITEM( (char)iSlot, pItem->GetIndex(), iQuantity );
			return;
		}
	}
	//}
	//else
	//{
	//	int i;
	//	///���� ������ �������� �������
	//	for( i = 0; i < TOTAL_EXCHANGE_INVENTORY ; ++i)
	//	{
	//		if( pExchangeDlg->m_MySlots[i].m_nInvenIdx == m_ExchangeItem.m_nInvenIdx )
	//		{
	//			pExchangeDlg->m_MySlots[i].m_sItem.m_iQuantity += m_iNumber;
	//			pExchangeDlg->SetMeReadyState( false );
	//			g_pNet->Send_cli_TRADE_P2P_ITEM( (char)i, m_ExchangeItem.m_nInvenIdx, pExchangeDlg->m_MySlots[i].m_sItem.m_iQuantity );
	//			return true;
	//		}
	//	}


	//	for( i = 0; i < TOTAL_EXCHANGE_INVENTORY ; ++i)
	//	{
	//		if( pExchangeDlg->m_MySlots[i].m_sItem.GetItemNO() <= 0 )
	//		{
	//			m_ExchangeItem.m_iQuantity	= (int)m_iNumber;
	//			pExchangeDlg->m_MySlots[i]  = m_ExchangeItem;
	//			pExchangeDlg->SetMeReadyState( false );
	//			g_pNet->Send_cli_TRADE_P2P_ITEM( (char)i, m_ExchangeItem.m_nInvenIdx, m_ExchangeItem.m_iQuantity );
	//			break;
	//		}
	//	}
	//}
	//
	//
}

void CExchange::RemoveMyItemByInvenIndex( int iInvenIndex )
{
	for( int iSlot = 0; iSlot < TOTAL_EXCHANGE_INVENTORY; ++iSlot )
	{
		if( m_MyItems[iSlot] && m_MyItems[iSlot]->GetIndex() == iInvenIndex )
		{
			RemoveMyItemBySlotIndex( iSlot );
			return;
		}
	}
}

void CExchange::RemoveMyItemBySlotIndex( int iSlotIndex , bool bSendPacket )
{
	if( iSlotIndex < 0 || iSlotIndex >= TOTAL_EXCHANGE_INVENTORY )
	{
		assert( 0 && "Invalid SlotIndex @CExchange::RemoveMyItem" );	
		return;
	}

	if( m_MyItems[iSlotIndex] == NULL )
	{
		assert( m_MyItems[iSlotIndex] && "�󽽷��� �������� ������ �մϴ� @CExchange::RemoveMyItem" );
		return;
	}
	
	m_pEventExchange->SetID( CTEventExchange::EID_REMOVE_MYITEM );
	m_pEventExchange->SetSlotIndex( iSlotIndex );

	SetChanged();
	NotifyObservers( m_pEventExchange );///Ȥ�� Event���
	
	if( bSendPacket )
		g_pNet->Send_cli_TRADE_P2P_ITEM( (char)iSlotIndex, m_MyItems[iSlotIndex]->GetIndex(), 0 );

	delete m_MyItems[iSlotIndex];
	m_MyItems[iSlotIndex] = NULL;
}

bool CExchange::UpdateOtherItem( int iIndex, tagITEM& item )
{
	if( iIndex < 0 || iIndex >= MAX_TRADE_ITEM_SLOT )
		return false;

	if( iIndex == TRADE_MONEY_SLOT_NO )//��
	{
		if( item.GetTYPE() == 0 )
			m_i64OtherTradeMoney = 0;
		else
			m_i64OtherTradeMoney = item.GetMoney();
	}
	else
	{
		if( item.GetTYPE() == 0 )
		{
			///����
			if( m_OtherItems[ iIndex ] )
			{
				///UI���� �˸���
				m_pEventExchange->SetID( CTEventExchange::EID_REMOVE_OTHERITEM );
				m_pEventExchange->SetSlotIndex( iIndex );
				SetChanged();
				NotifyObservers( m_pEventExchange );

				delete m_OtherItems[iIndex];
				m_OtherItems[iIndex] = NULL;
			}
			else
			{
				assert( m_OtherItems[ iIndex ] && "���� �������� ������ �մϴ� @CExchange::UpdateOtherItem " );
			}
		}
		else
		{
			if( m_OtherItems[ iIndex ] )
			{
				assert( m_OtherItems[ iIndex ] && "��� ���� ���� ���Կ� �������� �������� �մϴ� @CExchange::UpdateOtherItem " );
			}
			else
			{
				CItem*	pItem = new CItem;
				pItem->SetItem( item );
				pItem->SetIndex( iIndex );

				m_OtherItems[ iIndex ] = pItem;
				///UI���� �˸���.
				m_pEventExchange->SetID( CTEventExchange::EID_ADD_OTHERITEM );
				m_pEventExchange->SetItem( pItem );
				m_pEventExchange->SetSlotIndex( iIndex );

				SetChanged();
				NotifyObservers( m_pEventExchange );

				///���� ������ �ø� ǰ���� ���� - ���� �ø� ǰ���� ������ ���� �κ��丮�� �� ���԰������� Ŭ���
				///���濡�� �� �κ��� ��á�ٰ� �˷��ְ� �̹� �ø����� ����� �Ѵ�.
//				if( !(CExchange::GetInstance().IsEnoughEmptyInvenSlots( item )) )

				std::list< tagITEM > appendItems;
				appendItems.push_back( item );

				if( g_pAVATAR && g_pAVATAR->IsInventoryFull( appendItems ) )
				{	
					SendTradePacket( RESULT_TRADE_OUT_OF_INV,iIndex );
					g_itMGR.OpenMsgBox(STR_NOT_ENOUGH_INVENTORY_SPACE);
				}
			}
		}
	}
	///SetOtherReadyState( false ); �� �̰� ���־�� �ϳ� ??????
	return true;
}
//
//CItem* CExchange::CreateItem( tagITEM& Item )
//{
//	CItem* pItem = NULL;
//	switch( Item.GetTYPE() )
//	{
//	case ITEM_TYPE_FACE_ITEM:
//	case ITEM_TYPE_HELMET:
//	case ITEM_TYPE_ARMOR:
//	case ITEM_TYPE_GAUNTLET:
//	case ITEM_TYPE_BOOTS:
//	case ITEM_TYPE_KNAPSACK:
//	case ITEM_TYPE_JEWEL:
//	case ITEM_TYPE_WEAPON:
//	case ITEM_TYPE_SUBWPN:
//		pItem = new CItemEquip;
//		break;
//	case ITEM_TYPE_USE:
//		pItem = new CItemUse;
//		break;
//	case ITEM_TYPE_ETC:
//	case ITEM_TYPE_NATURAL:
//		pItem = new CItemEtc;
//		break;
//	case ITEM_TYPE_RIDE_PART:
//		pItem = new CItemPat;
//		break;
//	default:
//		assert( 0 && "Invalid Item Type @CBank::CreatItem" );
//		break;
//	}
//
//	return pItem;	
//
//}

void CExchange::Update( CObservable* pObservable, CTObject* pObj )
{
	assert( pObservable );
	if( pObj && strcmp( pObj->toString(), "CTEventItem") == 0 )
	{

		CTEventItem* pEvent = (CTEventItem*)pObj;
		int iIndex = pEvent->GetIndex();
		switch( pEvent->GetID() )
		{
		case CTEventItem::EID_DEL_ITEM:
			{
				for( int i = 0 ;i < TOTAL_EXCHANGE_INVENTORY; ++i )
				{
					if( m_MyItems[i] && m_MyItems[i]->GetIndex() == iIndex )
					{
						RemoveMyItemBySlotIndex( i , false );
						return;
					}
				}
				break;
			}
		case CTEventItem::EID_CHANGE_ITEM :
			{
				CItem* pItem = pEvent->GetItem();
				for( int i = 0 ;i < TOTAL_EXCHANGE_INVENTORY; ++i )
				{
					if( m_MyItems[i] && m_MyItems[i]->GetIndex() == iIndex )
					{
						if( pItem->GetQuantity() < m_MyItems[i]->GetQuantity() )
							RemoveMyItemBySlotIndex( i );
						return;
					}
				}
				break;
			}
		default:
			break;
		}
	}
	else
	{
		assert( 0 && "pObj is NULL or Invalid Type@CExchange::Update" );
	}

}