#ifndef _CSeparate_
#define _CSeparate_

#include "../GameCommon/CObservable.h"
#include "../GameCommon/IObserver.h"
#include "../Common/CItem.h"
#include "../interface/dlgs/subclass/CSlot.h"
#include "event/CTEventSeparate.h"
#include <vector>

class CItemFragment;
class CItem;



/**
* �и�/���ؽ� ���Ǵ� Data Class
*
* @Author		������
* @Date			2005/9/15
*/
class CSeparate : public CObservable, public IObserver
{
	CSeparate(void);
	~CSeparate(void);

public:
	static CSeparate& GetInstance();

	virtual void Update( CObservable* pObservable, CTObject* pObj );

	void	SetItem( CItem* pItem );
	void	RemoveItem();

	int		GetRequireMp();
	int		GetRequireMoney();

	enum{
		TYPE_NONE			= 0,
		TYPE_SEPARATE		= 1,///�и� - ����
		TYPE_DECOMPOSITION	= 2,///���� - ������ Ȥ�� ����ȣ���� �ִ� ������
	};
	
	enum{
		TYPE_SKILL,
		TYPE_NPC
	};
	void Send_CRAFT_BREAKUP_REQ();
	void SetSkillSlotIdx( int iSkillSlotIdx );
	void SetNpcSvrIdx( WORD wNpcSvrIdx );
	WORD GetNpcSvrIdx();
	int	 GetType();

private:
	int	GetQuantity( tagITEM& Item, int iRequireQuantity );

private:
	CTEventSeparate			m_Event;
	CItemFragment*			m_pMaterialItem;///�и� �ϰ��� �ϴ� ������
	std::vector< CItem* >	m_pItems;		///�и��� ������ �����۵�
	int						m_iType;
	int						m_iRequireMp;
	int						m_iRequireMoney;
	int						m_iSkillSlotIdx;
	WORD					m_wNpcSvrIdx;
	bool					m_bWaitServerReply;
};
#endif