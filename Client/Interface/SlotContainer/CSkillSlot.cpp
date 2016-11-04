#include "stdafx.h"
#include "CSkillSlot.h"
#include "../../Common/IO_Skill.h"
#include "../../GameCommon/Skill.h"
#include "../../Country.h"



//----------------------------------------------------------------------------------------------------
/// @param
/// @brief constructor
//----------------------------------------------------------------------------------------------------

CSkillSlot::CSkillSlot()
{
	memset( m_SkillSlot, 0, sizeof( m_SkillSlot ) );	
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief destructor
//----------------------------------------------------------------------------------------------------

CSkillSlot::~CSkillSlot()
{
	// Destruct
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief delete all skill slot
//----------------------------------------------------------------------------------------------------
void CSkillSlot::ClearSlot()
{
	for( int i = 0; i < MAX_LEARNED_SKILL_CNT ; i++ )
	{
		if( m_SkillSlot[ i ] != NULL )
		{
			m_Event.SetID( CTEventSkill::EID_DEL_SKILL );
			m_Event.SetIndex( i );
			SetChanged();
			NotifyObservers( &m_Event );
			
			delete m_SkillSlot[ i ];
			m_SkillSlot[ i ] = NULL;
		}
	}
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ��ų ���� �ʱ�ȭ
//----------------------------------------------------------------------------------------------------

void CSkillSlot::InitSlot()
{
	ClearSlot();
}


//----------------------------------------------------------------------------------------------------
/// �ָ����� Ÿ�̸��� ������ ���ؼ���
//----------------------------------------------------------------------------------------------------
void CSkillSlot::UpdateSkillSlot()
{
	CSkill* pSkill = NULL;

	for( int i = 0; i < MAX_LEARNED_SKILL_CNT ; i++ )
	{
		pSkill = GetSkill( i );
		if( pSkill != NULL )
		{
			pSkill->Process();
		}
	}
}



//----------------------------------------------------------------------------------------------------
/// @param nSlotIndex ��ų ���� �ε���
/// @param nSlotIndex ��ų �ε���
/// @param nSlotIndex ��ų ����
/// @param nSlotIndex ��ų ���� �ε���
/// @brief ���ο� ��ų�� ��ų ���Կ� ���
//----------------------------------------------------------------------------------------------------

void CSkillSlot::SetSkillSlot(short nSlotIndex, short nSkillIndex, short nSkillLevel )
{
	if( m_SkillSlot[ nSlotIndex ] != NULL )
	{
		return;
	}

	CSkill* pSkill = g_SkillManager.CreateNewSkill( SKILL_SLOT_MAIN, nSkillIndex );
	if( pSkill == NULL )
	{
		//assert( 0 && "SetSkillSlot[ pSkill == NULL ]" );
		return;
	}

	pSkill->SetSkillSlot( nSlotIndex );
	m_SkillSlot[ nSlotIndex ] = pSkill;	

	m_Event.SetID( CTEventSkill::EID_ADD_SKILL );
	m_Event.SetIndex( nSlotIndex );
	SetChanged();
	NotifyObservers( &m_Event );
}
//----------------------------------------------------------------------------------------------------
/// @param nSlotIndex ��ų ���� �ε���
/// @param nSlotIndex ��ų �ε���
/// @brief ��ų�������� �ϸ鼭 Ÿ���� ����Ǵ� ��ų�ϰ�� �������� ����� ���� ������־�� �Ѵ�.
//----------------------------------------------------------------------------------------------------
void CSkillSlot::SkillLevelUp( int iSkillSlotNo, int iSkillIndex )
{
	assert( m_SkillSlot[ iSkillSlotNo ] ); 
	if( m_SkillSlot[ iSkillSlotNo ] == NULL )
		return;


	int iOldSkillType = SKILL_TYPE( iSkillIndex - 1 );
	int iNewSkillType = SKILL_TYPE( iSkillIndex );

	if( iOldSkillType != iNewSkillType )
	{
		CSkill* pSkill = g_SkillManager.CreateNewSkill( SKILL_SLOT_MAIN, iSkillIndex );
		if( pSkill == NULL )
			return;

		pSkill->SetSkillSlot( iSkillSlotNo );
		delete m_SkillSlot[ iSkillSlotNo ];
		m_SkillSlot[ iSkillSlotNo ] = pSkill;	

	}

	m_Event.SetID( CTEventSkill::EID_LEVELUP );
	m_Event.SetIndex( iSkillSlotNo );
	SetChanged();
	NotifyObservers( &m_Event );

}
//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ��ų��ȣ�� ��ų������ ��� ���� ��ų��ü�� ���´�.
///			������ ���ʹ� ��ų��ȣ�� �ޱ� ������ ������ ��ȣ�� ������ ���� �ʿ䰡 �ִ�.( Ÿ�̸� ���� )
//----------------------------------------------------------------------------------------------------

CSkill* CSkillSlot::GetSkillBySkillIDX( int iSkillIDX )
{
	CSkill* pSkill = NULL;

	for( int i = 0; i < MAX_LEARNED_SKILL_CNT ; i++ )
	{
		pSkill = GetSkill( i );
		if( pSkill != NULL )
		{
			if( pSkill->GetSkillIndex() == iSkillIDX )
				return pSkill;
		}
	}

	return pSkill;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ��ų��ȣ�� ��ų������ ��� ���� ��ų��ü�� ���´�.
///			������ ���ʹ� ��ų��ȣ�� �ޱ� ������ ������ ��ȣ�� ������ ���� �ʿ䰡 �ִ�.( Ÿ�̸� ���� )
//----------------------------------------------------------------------------------------------------
CSkill* CSkillSlot::GetSkillByBaseSkillIDX( int iBaseSkillIDX )
{
	CSkill* pSkill = NULL;

	for( int i = 0; i < MAX_LEARNED_SKILL_CNT ; i++ )
	{
		if( pSkill = GetSkill( i ) )
		{
			if( SKILL_1LEV_INDEX( pSkill->GetSkillIndex() ) == iBaseSkillIDX )
				return pSkill;
		}
	}
	return NULL;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �ش� ������ ��ų ��ȣ�� ���´�.
//----------------------------------------------------------------------------------------------------

short CSkillSlot::GetSkillIndex(short nSlotIndex)
{
	if( nSlotIndex >= MAX_LEARNED_SKILL_CNT 
		|| nSlotIndex < 0 
		|| m_SkillSlot[ nSlotIndex ] == NULL )
		return 0;
	
	return m_SkillSlot[ nSlotIndex ]->GetSkillIndex();
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief
//----------------------------------------------------------------------------------------------------

short CSkillSlot::GetSkillIndex( short nIndex, short nTabIndex )
{
	if( nIndex < 0 )
		return 0;

	if( nTabIndex*( MAX_LEARNED_SKILL_CNT/4 ) + nIndex >= MAX_LEARNED_SKILL_CNT 
		|| m_SkillSlot[ nTabIndex*( MAX_LEARNED_SKILL_CNT/4 ) + nIndex ] == NULL )
		return 0;

	return m_SkillSlot[ nTabIndex*( MAX_LEARNED_SKILL_CNT/4 ) + nIndex ]->GetSkillIndex();
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �ش� ������ ������ ���´�.
//----------------------------------------------------------------------------------------------------

short CSkillSlot::GetSkillLevel(short nSlotIndex)
{
	if( nSlotIndex >= MAX_LEARNED_SKILL_CNT 
		|| nSlotIndex < 0
		|| m_SkillSlot[ nSlotIndex ] == NULL )
		return 0;

	return m_SkillSlot[ nSlotIndex ]->GetSkillLevel();
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief
//----------------------------------------------------------------------------------------------------

short CSkillSlot::GetSkillLevel( short nIndex, short nTabIndex )
{
	if( nIndex < 0 )
		return 0;

	if( nTabIndex*( MAX_LEARNED_SKILL_CNT/4 ) + nIndex >= MAX_LEARNED_SKILL_CNT 
		|| m_SkillSlot[ nTabIndex*( MAX_LEARNED_SKILL_CNT/4 ) + nIndex ] == NULL )
		return 0;

	return m_SkillSlot[ nTabIndex*( MAX_LEARNED_SKILL_CNT/4 ) + nIndex ]->GetSkillLevel();
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �ش� ������ Delay time�� ���´�.
//----------------------------------------------------------------------------------------------------

short CSkillSlot::GetSkillDelayTime(short nSlotIndex)
{
	if( nSlotIndex >= MAX_LEARNED_SKILL_CNT 
		|| nSlotIndex < 0
		|| m_SkillSlot[ nSlotIndex ] == NULL )
		return 0;

	return m_SkillSlot[ nSlotIndex ]->GetSkillDelayTime();
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief
//----------------------------------------------------------------------------------------------------

short CSkillSlot::GetSkillDelayTime( short nIndex, short nTabIndex )
{
	if( nIndex < 0 )
		return 0;

	if( nTabIndex*( MAX_LEARNED_SKILL_CNT/4 ) + nIndex >= MAX_LEARNED_SKILL_CNT 
		|| m_SkillSlot[ nTabIndex*( MAX_LEARNED_SKILL_CNT/4 ) + nIndex ] == NULL )
		return 0;

	return m_SkillSlot[ nTabIndex*( MAX_LEARNED_SKILL_CNT/4 ) + nIndex ]->GetSkillDelayTime();
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ������ ��ȣ�� ���´�.
//----------------------------------------------------------------------------------------------------

short CSkillSlot::GetSkillIconIndex(short nSlotIndex)
{
	short nSkillIndex = GetSkillIndex( nSlotIndex );

	if( nSkillIndex == 0 )
		return 0;

	short nIconIndex = SKILL_ICON_NO( nSkillIndex );

	return nIconIndex;
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief
//----------------------------------------------------------------------------------------------------

short CSkillSlot::GetSkillIconIndex( short nIndex, short nTabIndex )
{
	short nSkillIndex = GetSkillIndex( nIndex, nTabIndex );

	if( nSkillIndex == 0 )
		return 0;

	short nIconIndex = SKILL_ICON_NO( nSkillIndex );

	return nIconIndex;
}

void CSkillSlot::RemoveBySlotIndex( short nSlotIndex )
{
	assert( nSlotIndex >= 0 && nSlotIndex < MAX_LEARNED_SKILL_CNT );
	if( nSlotIndex >= 0 && nSlotIndex < MAX_LEARNED_SKILL_CNT )
	{
		if( m_SkillSlot[ nSlotIndex ] != NULL )
		{
			m_Event.SetID( CTEventSkill::EID_DEL_SKILL );
			m_Event.SetIndex( nSlotIndex );
			SetChanged();
			NotifyObservers( &m_Event );
			
			delete m_SkillSlot[ nSlotIndex ];
			m_SkillSlot[ nSlotIndex ] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------/
/// Patž�� ���ο� ���� ��ų Ȱ��ȭ / ��Ȱ��ȭ ���� : 2005/7/29 - nAvy
///
//--------------------------------------------------------------------------------------------------------/
void CSkillSlot::SetActiveSkillEnableByRideState( bool ride )
{
	_RPT0( _CRT_WARN," CSkillSlot::SetActiveSkillEnableByRideState()\n ");
	///��Ʋ īƮ�� ������� �ʴ� ���������� �÷����� Ʋ����.

#if defined(_GBC)
	if( ride )///pat ��ų�� Enable �������� Disable
	{
		if( false == g_pAVATAR->m_Inventory.m_ItemRIDE[0].IsEmpty() )
		{
			int item_type = ITEM_TYPE( g_pAVATAR->m_Inventory.m_ItemRIDE[0].GetTYPE(), g_pAVATAR->m_Inventory.m_ItemRIDE[0].GetItemNO() );
			switch( item_type )
			{
			case TUNING_PART_BODY_CART:///īƮ ��ü
				for( int index = MAX_LEARNED_SKILL_PER_PAGE; index < MAX_LEARNED_SKILL_PER_PAGE * 2; ++index )
				{
					if( m_SkillSlot[ index ] != NULL )
					{
						if( SKILL_ATTRIBUTE_CART == SKILL_ATTRIBUTE( m_SkillSlot[ index ]->GetSkillIndex() ) )
							m_SkillSlot[ index ]->SetEnable();
						else
							m_SkillSlot[ index ]->SetDisable();
					}
				}
				break;
			case TUNING_PART_BODY_CASTLEGEAR:///ĳ����� ��ü
				for( int index = MAX_LEARNED_SKILL_PER_PAGE; index < MAX_LEARNED_SKILL_PER_PAGE * 2; ++index )
				{
					if( m_SkillSlot[ index ] != NULL )
					{
						if( SKILL_ATTRIBUTE_CASTLEGEAR == SKILL_ATTRIBUTE( m_SkillSlot[ index ]->GetSkillIndex() ) )
							m_SkillSlot[ index ]->SetEnable();
						else
							m_SkillSlot[ index ]->SetDisable();
					}
				}
				break;
			default:
				_RPTF0( _CRT_ASSERT,"Invalid Type");
				break;
			}
		}
	}
	else///�ƹ�Ÿ ���� ��ų�� Enable �������� Disable
	{
		for( int index = MAX_LEARNED_SKILL_PER_PAGE; index < MAX_LEARNED_SKILL_PER_PAGE * 2; ++index )
		{
			if( m_SkillSlot[ index ] != NULL )
			{
				if( SKILL_ATTRIBUTE_AVATAR == SKILL_ATTRIBUTE( m_SkillSlot[ index ]->GetSkillIndex() ) )
					m_SkillSlot[ index ]->SetEnable();
				else
					m_SkillSlot[ index ]->SetDisable();
			}
		}
	}
#endif

}

//2�ν� īƮ
void CSkillSlot::SetActiveSkillEnableByRideState_Aid( bool ride )
{
// �������� ������ ���� ��ų �̿ܿ��� ������� ���Ѵ�.
// Stb 5��(��ų ���� ����) �÷� ���� 1 : ���� ��ų.
#if defined(_GBC)

	if( ride )
	{
		for( int index = 0; index < MAX_LEARNED_SKILL_CNT; ++index )
		{
			if( m_SkillSlot[ index ] != NULL )
			{			
				// �̸�Ƽ�� ��� ����.
				if( SKILL_TYPE(m_SkillSlot[ index ]->GetSkillIndex())==SKILL_EMOTION_ACTION )
				{
					m_SkillSlot[ index ]->SetEnable();
				}
				else if( SKILL_TYPE(m_SkillSlot[ index ]->GetSkillIndex())==SKILL_BASE_ACTION )
				{
					// �⺻��ų�߿� 13:īƮ �¿�� ��밡��.
					if( SKILL_DISTANCE(m_SkillSlot[ index ]->GetSkillIndex())==13)
					{
						m_SkillSlot[ index ]->SetEnable();
					}
					else
					{
						m_SkillSlot[ index ]->SetDisable();
					}				
				}
				else
				{	
					m_SkillSlot[ index ]->SetDisable();
				}			
			}
		}
	}
	else
	{
		for( int index = 0; index < MAX_LEARNED_SKILL_CNT; ++index )
		{
			if( m_SkillSlot[ index ] != NULL )
			{			
				m_SkillSlot[ index ]->SetEnable();				
			}				
		}
	}	

#endif
}


