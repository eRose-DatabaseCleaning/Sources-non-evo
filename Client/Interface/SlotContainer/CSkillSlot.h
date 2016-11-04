#ifndef _CSKILLSLOT_
#define _CSKILLSLOT_


#include "../../Common/CUserDATA.h"	
#include "../../gamecommon/cobservable.h"
#include "../../gamedata/event/CTEventSkill.h"
class CSkill;

/**
* CItemSlot�� ���� ������ ������� ��ų �����̳� Ŭ����
*
* @Author		�̵���, ������
* @Date			2005/9/15
*/
class CSkillSlot : public CObservable
{	
private:
	CSkill*						m_SkillSlot[ MAX_LEARNED_SKILL_CNT ];	
	CTEventSkill				m_Event;
	

public:
	CSkillSlot();
	~CSkillSlot();


	void	ClearSlot();
	void	InitSlot();

	//----------------------------------------------------------------------------------------------------
	/// �ָ����� Ÿ�̸��� ������ ���ؼ���
	//----------------------------------------------------------------------------------------------------
	void	UpdateSkillSlot();


	void	SetSkillSlot( short nSlotIndex, short nSkillIndex, short nSkillLevel );
	void	RemoveBySlotIndex( short nSlotIndex );
	void	SkillLevelUp( int iSkillSlotNo, int iSkillIndex );

	CSkill* GetSkill( short nSlotIndex ){ return m_SkillSlot[ nSlotIndex ]; }
	CSkill* GetSkillBySkillIDX( int iSkillIDX );
	CSkill* GetSkillByBaseSkillIDX( int iBaseSkillIDX );

	short	GetSkillIndex( short nSlotIndex );
	short	GetSkillIndex( short nIndex, short nTabIndex );
	short	GetSkillLevel( short nSlotIndex );
	short	GetSkillLevel( short nIndex, short nTabIndex );
	short	GetSkillDelayTime( short nSlotIndex );
	short	GetSkillDelayTime( short nIndex, short nTabIndex );

	//----------------------------------------------------------------------------------------------------
	/// method for UI interface
	//----------------------------------------------------------------------------------------------------
	short	GetSkillIconIndex( short nSlotIndex );
	short	GetSkillIconIndex( short nIndex, short nTabIndex );	


	//----------------------------------------------------------------------------------------------------
	/// Pat ž�¿��ο� ���� ��Ƽ�꽺ų�� Ȱ��ȭ / ��Ȱ��ȭ
	//----------------------------------------------------------------------------------------------------
	void	SetActiveSkillEnableByRideState( bool ride );

	//----------------------------------------------------------------------------------------------------
	/// 2�ν� īƮ Pat ������ ž�¿��ο� ���� ��Ƽ�꽺ų�� Ȱ��ȭ / ��Ȱ��ȭ
	//----------------------------------------------------------------------------------------------------
	void	SetActiveSkillEnableByRideState_Aid( bool ride );
};


#endif //_CSKILLSLOT_