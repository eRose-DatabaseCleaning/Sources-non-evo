#ifndef _CIconSkillClan_
#define _CIconSkillClan_
#include "ciconskill.h"

/**
* Ŭ����ųǥ�ÿ� ���Ǵ�  Icon
*
* @Author	������
* @Date		2005/9/12
*/
class CIconSkillClan : public CIconSkill
{
	int							m_iSkillSlot;
public:
	CIconSkillClan(void);
	CIconSkillClan( int iSkillSlot );
	virtual ~CIconSkillClan(void);

	virtual int GetIndex();										/// GetSkillSlotFromIcon()
	void		SetSkillSlotToIcon( int iSkillSlot );
	int			GetSkillSlotFromIcon();							/// SetSkillSlotToIcon���� �����صξ��� ��ų���� �ε����� �����Ѵ�.
	virtual int	GetSkillLevel();
	virtual int	GetSkillIndex();								/// CSkill���� ��ų�ε���(STB���γѹ�)�� ���ؼ� �����Ѵ�.
	CSkill*		GetSkill();

	//----------------------------------------------------------------------------------------------------
	/// Overrided from CIcon
	//----------------------------------------------------------------------------------------------------
	virtual void		Draw();
	virtual void		ExecuteCommand();    
	virtual CIcon*		Clone();
	virtual void		GetToolTip( CInfo& ToolTip,  DWORD dwDialogType, DWORD dwType );
	virtual const char*	GetName();
};
#endif