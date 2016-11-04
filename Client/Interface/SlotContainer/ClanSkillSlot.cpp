#include "stdafx.h"
#include ".\clanskillslot.h"
#include "../../Common/IO_Skill.h"
#include "../../GameCommon/Skill.h"


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief constructor
//----------------------------------------------------------------------------------------------------

CClanSkillSlot::CClanSkillSlot(void)
{
	memset( m_SkillSlot, 0, sizeof( m_SkillSlot ) );	
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief constructor
//----------------------------------------------------------------------------------------------------

CClanSkillSlot::~CClanSkillSlot(void)
{
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief delete all skill slot
//----------------------------------------------------------------------------------------------------
void CClanSkillSlot::ClearSlot()
{
	for( int i = 0; i < MAX_CLAN_SKILL_SLOT ; i++ )
	{
		if( m_SkillSlot[ i ] != NULL )
		{
			m_Event.SetID( CTEventClanSkill::EID_DEL_SKILL );
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

void CClanSkillSlot::InitSlot()
{
	ClearSlot();
}


//----------------------------------------------------------------------------------------------------
/// �ָ����� Ÿ�̸��� ������ ���ؼ���
//----------------------------------------------------------------------------------------------------
void CClanSkillSlot::UpdateSkillSlot()
{
	CSkill* pSkill = NULL;

	for( int i = 0; i < MAX_CLAN_SKILL_SLOT ; i++ )
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

void CClanSkillSlot::SetSkillSlot(short nSlotIndex, short nSkillIndex, short nSkillLevel, DWORD expire_time_abssec )
{
	if( m_SkillSlot[ nSlotIndex ] != NULL )
		RemoveBySlotIndex( nSlotIndex );


	CSkill* pSkill = g_SkillManager.CreateNewSkill( SKILL_SLOT_CLAN, nSkillIndex );
	if( pSkill == NULL )
	{
		//assert( 0 && "SetSkillSlot[ pSkill == NULL ]" );
		return;
	}

	if( pSkill->HasExpiredTime() )
		pSkill->SetExpiredTime( expire_time_abssec );

	pSkill->SetSkillSlot( nSlotIndex );
	m_SkillSlot[ nSlotIndex ] = pSkill;	

	m_Event.SetID( CTEventClanSkill::EID_ADD_SKILL );
	m_Event.SetIndex( nSlotIndex );
	SetChanged();
	NotifyObservers( &m_Event );
}
//----------------------------------------------------------------------------------------------------
/// @param nSlotIndex ��ų ���� �ε���
/// @param nSlotIndex ��ų �ε���
/// @brief ��ų�������� �ϸ鼭 Ÿ���� ����Ǵ� ��ų�ϰ�� �������� ����� ���� ������־�� �Ѵ�.
//----------------------------------------------------------------------------------------------------
void CClanSkillSlot::SkillLevelUp( int iSkillSlotNo, int iSkillIndex )
{
	assert( m_SkillSlot[ iSkillSlotNo ] ); 
	if( m_SkillSlot[ iSkillSlotNo ] == NULL )
		return;


	int iOldSkillType = SKILL_TYPE( iSkillIndex - 1 );
	int iNewSkillType = SKILL_TYPE( iSkillIndex );

	if( iOldSkillType != iNewSkillType )
	{
		CSkill* pSkill = g_SkillManager.CreateNewSkill( SKILL_SLOT_CLAN, iSkillIndex );
		if( pSkill == NULL )
			return;

		pSkill->SetSkillSlot( iSkillSlotNo );
		delete m_SkillSlot[ iSkillSlotNo ];
		m_SkillSlot[ iSkillSlotNo ] = pSkill;	

	}

	m_Event.SetID( CTEventClanSkill::EID_LEVELUP );
	m_Event.SetIndex( iSkillSlotNo );
	SetChanged();
	NotifyObservers( &m_Event );

}
//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ��ų��ȣ�� ��ų������ ��� ���� ��ų��ü�� ���´�.
///			������ ���ʹ� ��ų��ȣ�� �ޱ� ������ ������ ��ȣ�� ������ ���� �ʿ䰡 �ִ�.( Ÿ�̸� ���� )
//----------------------------------------------------------------------------------------------------

CSkill* CClanSkillSlot::GetSkillBySkillIDX( int iSkillIDX )
{
	CSkill* pSkill = NULL;

	for( int i = 0; i < MAX_CLAN_SKILL_SLOT ; i++ )
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
CSkill* CClanSkillSlot::GetSkillByBaseSkillIDX( int iBaseSkillIDX )
{
	CSkill* pSkill = NULL;

	for( int i = 0; i < MAX_CLAN_SKILL_SLOT ; i++ )
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

short CClanSkillSlot::GetSkillIndex(short nSlotIndex)
{
	if( nSlotIndex >= MAX_CLAN_SKILL_SLOT 
		|| nSlotIndex < 0 
		|| m_SkillSlot[ nSlotIndex ] == NULL )
		return 0;
	
	return m_SkillSlot[ nSlotIndex ]->GetSkillIndex();
}



//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �ش� ������ ������ ���´�.
//----------------------------------------------------------------------------------------------------

short CClanSkillSlot::GetSkillLevel(short nSlotIndex)
{
	if( nSlotIndex >= MAX_CLAN_SKILL_SLOT 
		|| nSlotIndex < 0
		|| m_SkillSlot[ nSlotIndex ] == NULL )
		return 0;

	return m_SkillSlot[ nSlotIndex ]->GetSkillLevel();
}




//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �ش� ������ Delay time�� ���´�.
//----------------------------------------------------------------------------------------------------

short CClanSkillSlot::GetSkillDelayTime(short nSlotIndex)
{
	if( nSlotIndex >= MAX_CLAN_SKILL_SLOT 
		|| nSlotIndex < 0
		|| m_SkillSlot[ nSlotIndex ] == NULL )
		return 0;

	return m_SkillSlot[ nSlotIndex ]->GetSkillDelayTime();
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ������ ��ȣ�� ���´�.
//----------------------------------------------------------------------------------------------------

short CClanSkillSlot::GetSkillIconIndex(short nSlotIndex)
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

void CClanSkillSlot::RemoveBySlotIndex( short nSlotIndex )
{
	assert( nSlotIndex >= 0 && nSlotIndex < MAX_CLAN_SKILL_SLOT );
	if( nSlotIndex >= 0 && nSlotIndex < MAX_CLAN_SKILL_SLOT )
	{
		if( m_SkillSlot[ nSlotIndex ] != NULL )
		{
			m_Event.SetID( CTEventClanSkill::EID_DEL_SKILL );
			m_Event.SetIndex( nSlotIndex );
			SetChanged();
			NotifyObservers( &m_Event );
			
			delete m_SkillSlot[ nSlotIndex ];
			m_SkillSlot[ nSlotIndex ] = NULL;
		}
	}
}