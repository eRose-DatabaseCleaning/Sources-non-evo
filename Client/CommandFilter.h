#ifndef _COMMAND_FILTER_
#define _COMMAND_FILTER_

#include "Util/JSingleton.h"
#include "ObjectCommand/ObjCommand.h"


//----------------------------------------------------------------------------------------------------
///
/// class CCommandFilter
/// ������ ������ ����� ���͸� �մϴ�.
///
//----------------------------------------------------------------------------------------------------

class CCommandFilter : public CJSingleton< CCommandFilter >
{
private:
	/// Object command pool
	CObjCommand*		m_ObjCommandPool[ OBJECT_COMMAND_MAX ];
	CObjCommand*		m_pPrevCommand;
	


public:
	CCommandFilter(void);
	~CCommandFilter(void);

	CObjCommand*		GetCommandObject( int iObjectType );
	void				SetPrevCommand( CObjCommand* pCommand ){ m_pPrevCommand = pCommand; }

	bool				CanSendAttackCommand();

	//----------------------------------------------------------------------------------------------------
	/// ��ų���� ����� ĳ������ ���۵Ǹ� ������ų����� ����
	//----------------------------------------------------------------------------------------------------
	bool				CanSendSelfSkillCommand( int iSkillSlot );	
	bool				CanSendTargetSkillCommand( int iClientTarget, int iSkillSlot );	
	bool				CanSendPositionSkillCommand( D3DVECTOR &PosTO, int iSkillSlot );

};


#define g_CommandFilter (CCommandFilter::GetSingleton())

#endif //_COMMAND_FILTER_
