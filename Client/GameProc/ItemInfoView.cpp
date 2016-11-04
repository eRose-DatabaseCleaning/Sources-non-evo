#include "stdafx.h"
#include ".\iteminfoview.h"
#include <algorithm>
#include <functional>

#include "../CObjGROUND.h"
#include "../GameCommon/Item.h"


void DeleteDropItemInfo( CDropItemInfo* pInfo )
{
	if( pInfo )
	{
		delete pInfo;
		pInfo = NULL;
	}
}



//----------------------------------------------------------------------------------------------------
/// class CDropItemInfo
/// ����������� ������ ����� ���� ���ͷ�
//----------------------------------------------------------------------------------------------------
CDropItemInfo::CDropItemInfo()
{
}

CDropItemInfo::~CDropItemInfo()
{
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief Ŭ���� ������ �ɹ� �ʱ�ȭ
//----------------------------------------------------------------------------------------------------

void CDropItemInfo::Reset()
{
	m_pDropItem = NULL;
	m_Info.Clear();
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ������ ���� ����
//----------------------------------------------------------------------------------------------------

void CDropItemInfo::SetDropItemInfo( CObjITEM* pItem )
{
	Reset();

	m_pDropItem = pItem;

	m_Info.AddString( m_pDropItem->Get_NAME(), CItem::GetItemNameColor( pItem->m_ITEM.GetTYPE(), pItem->m_ITEM.GetItemNO()  ) );

	D3DVECTOR PosSCR;
	m_pDropItem->GetScreenPOS( PosSCR );
	POINT pt = { PosSCR.x, PosSCR.y };
	m_Info.SetPosition( pt );
}

void CDropItemInfo::Draw()
{
	m_Info.Draw();
}







CItemInfoView::CItemInfoView(void)
{	
}

CItemInfoView::~CItemInfoView(void)
{
}

void CItemInfoView::ReserverDropItemPool( int iPoolSize )
{
	for( int i = 0; i < iPoolSize ; i++ )
	{
		CDropItemInfo* pInfo = new CDropItemInfo;
		m_DropItemInfoPool.push_back( pInfo );
	}
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ��� ���÷��� ����Ʈ Ŭ����. �׸��� Ǯ�� Ŭ����
//----------------------------------------------------------------------------------------------------

void CItemInfoView::Clear()
{
	std::for_each( m_DropItemInfoPool.begin(), m_DropItemInfoPool.end(), DeleteDropItemInfo );	
	m_DropItemInfoPool.clear();
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �丮��Ʈ ����
//----------------------------------------------------------------------------------------------------

void CItemInfoView::Reset()
{
	m_DropItemInfoPool.insert( m_DropItemInfoPool.begin(), m_ViewedItemList.begin(), m_ViewedItemList.end() );
	m_ViewedItemList.clear();
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ���ο� ������ ������ �߰��Ѵ�.
//----------------------------------------------------------------------------------------------------

bool CItemInfoView::PushDropItemInfo( CObjITEM* pItem )
{
	if( pItem == NULL )
		return false;

	CDropItemInfo* pInfo = NULL;
	if( m_DropItemInfoPool.empty() )
	{
		pInfo = new CDropItemInfo;
	}else
	{
		pInfo = m_DropItemInfoPool.back();
		m_DropItemInfoPool.pop_back();
	}
	
	pInfo->SetDropItemInfo( pItem );
	m_ViewedItemList.push_back( pInfo );

	return true;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ��� ���÷��� ����Ʈ ������
//----------------------------------------------------------------------------------------------------

void CItemInfoView::Draw()
{
	int i = m_ViewedItemList.size();

	std::for_each( m_ViewedItemList.begin(), m_ViewedItemList.end(), 
		std::mem_fun< void, CDropItemInfo >( &CDropItemInfo::Draw ) );
}
