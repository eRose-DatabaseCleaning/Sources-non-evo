#include "stdafx.h"
#include ".\questrewardqueue.h"
#include <algorithm>
#include "../Object.h"
#include "../Interface/it_MGR.h"


CQuestRewardQueue g_QuestRewardQueue;


CQuestRewardQueue::CQuestRewardQueue(void)
{
	Clear();
}

CQuestRewardQueue::~CQuestRewardQueue(void)
{
	Clear();
}

void CQuestRewardQueue::Clear()
{
//	m_InvItemList.clear();
	m_SetAbilityList.clear();
	m_AddAbilityList.clear();
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ��� ����Ʈ�� ����..
//----------------------------------------------------------------------------------------------------

void CQuestRewardQueue::ApplyReward()
{
	//----------------------------------------------------------------------------------------------------
	/// ������ ����
	//----------------------------------------------------------------------------------------------------	
	//std::for_each( m_InvItemList.begin(), m_InvItemList.end(), CQuestRewardQueue::RewardItem );
	
	//----------------------------------------------------------------------------------------------------
	/// Set ability
	//----------------------------------------------------------------------------------------------------	
	std::for_each( m_SetAbilityList.begin(), m_SetAbilityList.end(), CQuestRewardQueue::SetAbility );

	//----------------------------------------------------------------------------------------------------
	/// Add alility
	//----------------------------------------------------------------------------------------------------	
	std::for_each( m_AddAbilityList.begin(), m_AddAbilityList.end(), CQuestRewardQueue::AddAbility );


	/// Reset
	Clear();
}

//void CQuestRewardQueue::PushSetInvItem( tag_SET_INVITEM& sSetInvItem )
//{
//	m_InvItemList.push_back( sSetInvItem );
//}

void CQuestRewardQueue::PushSetAbility( gsv_SET_ABILITY& sSetAbility )
{
	m_SetAbilityList.push_back( sSetAbility );
}

void CQuestRewardQueue::PushAddAbility( gsv_SET_ABILITY& sAddAbility )
{
	m_AddAbilityList.push_back( sAddAbility );
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ������ �����Լ� for std::algorithm
//----------------------------------------------------------------------------------------------------

//void CQuestRewardQueue::RewardItem( tag_SET_INVITEM sInvITEM )
//{
//	char *szMsg = NULL;
//
//	tagITEM *pInvITEM = NULL;
//	tagITEM *pSrvITEM = NULL;
//
//	pSrvITEM = &sInvITEM.m_ITEM;
//	if ( pSrvITEM->IsEnableDupCNT() ) 
//	{
//		pInvITEM = &g_pAVATAR->m_Inventory.m_ItemLIST[ sInvITEM.m_btInvIDX ];
//		szMsg = CStr::Printf("%s %d���� ���� �޾ҽ��ϴ�.", ITEM_NAME( pSrvITEM->GetTYPE(), pSrvITEM->GetItemNO() ), pSrvITEM->GetQuantity() - pInvITEM->GetQuantity() );
//	} else {
//		szMsg = CStr::Printf("%s�� ���� �޾ҽ��ϴ�.", ITEM_NAME( pSrvITEM->GetTYPE(), pSrvITEM->GetItemNO() ) );
//	}
//	g_itMGR.AppendChatMsg( szMsg, g_dwWHITE );
//}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �ƹ�Ÿ �ɷ�ġ ����
//----------------------------------------------------------------------------------------------------

void CQuestRewardQueue::SetAbility( gsv_SET_ABILITY sSetAbility )
{
	CStr::Printf( F_STR_SET_ABILITY, 
		CStringManager::GetSingleton().GetAbility( sSetAbility.m_wAbilityTYPE ),
			sSetAbility.m_iValue, g_dwWHITE );

	g_pAVATAR->Set_AbilityValue( sSetAbility.m_wAbilityTYPE, sSetAbility.m_iValue );
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief Add Avatar ability
//----------------------------------------------------------------------------------------------------

void CQuestRewardQueue::AddAbility( gsv_SET_ABILITY sSetAbility )
{
	CStr::Printf( F_STR_INCREASE_ABILITY, 
			CStringManager::GetSingleton().GetAbility( sSetAbility.m_wAbilityTYPE ),
			sSetAbility.m_iValue, g_dwWHITE );

	g_pAVATAR->Add_AbilityValue( sSetAbility.m_wAbilityTYPE, sSetAbility.m_iValue );
}