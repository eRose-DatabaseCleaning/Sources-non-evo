#include "stdafx.h"
#include ".\dropitemmanager.h"
#include <assert.h>


CDropItemManager	g_DropItemManager;


CDropItemManager::CDropItemManager(void)
{
	m_DropItemList.clear();
}

CDropItemManager::~CDropItemManager(void)
{
}

/// Drop item ������ ���ŵǾ���.
/// @return ������ ����� �����, 0�̶�� �ƹ�Ÿ
int CDropItemManager::UpdateDropItemList( int iDropItemIndex )
{
	assert( m_DropItemList.size() < 20 );

	std::list< stDROPITEM >::iterator		begin = m_DropItemList.begin();

	for( ; begin != m_DropItemList.end(); ++begin )
	{
		stDROPITEM& dropItem = *begin;
		if( iDropItemIndex == dropItem.m_iDropItemIndex )
		{
			m_DropItemList.erase( begin );
			return dropItem.m_iDamageOwner;
		}
	}

	return 0;
}
