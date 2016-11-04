#ifndef _CUpgrade_
#define _CUpgrade_
#include <vector>
#include "../gamecommon/CObservable.h"
#include "../gamecommon/IObserver.h"
#include "Net_Prototype.h"
#include "event/CTEventUpgrade.h"
class CItem;
class CItemFragment;
class CRequireMaterial;
/**
* ���ÿ� Data Class
*
* @Author		������
* @Date			2005/9/15
*/

class CUpgrade : public CObservable, public IObserver
{
	CUpgrade(void);
	~CUpgrade(void);
public:
	static CUpgrade& GetInstance();
	virtual void Update( CObservable* pObservable, CTObject* pObj );


	bool    SetItem( CItem* pItem );

	
	void	RemoveTargetItem();
	void	RemoveMaterialItem( CItem* pItem );

	void	SetNpcSvrIdx( int iNpcSvrIdx );
	void	SetSkillSlotIdx( int iSkillSlotIdx );
	void	SetUseItemInvenIdx( int iUseItemInvenIdx );


	bool	Send_CRAFT_UPGRADE_REQ();

	int		GetRequireMp();
	int		GetSuccessProb();
	const char*	GetRequireMaterialName( int iIndex );
	int		GetRequireMaterialCount( int iIndex );
	const char* GetTargetItemName();

	CItemFragment* GetTargetItem();

	bool   SendPacketUpgrade();

	enum{
		ALL_OK,
		NOTENOUGH_MP,		/// MP ����.
		NOTENOUGH_MATERIAL, /// ���������� ���� ����.
		INVALID_MATERIAL,   /// �ʿ��� ���������� ����.
		INVALID_TARGETITEM, /// ������ �������� ���ų� �߸��� �������̴�.
		NOTENOUGH_MONEY,
		INVALID_TYPE,
	};

	void ClearResultItemSet();
	void AddResultItemSet( tag_SET_INVITEM& Item );
	void SetResult( BYTE btResult );

	void SetResultSuccessProb( int iSuccessProb );
	int	 GetResultSuccessProb();

	BYTE GetResult();
	void ApplyResultItemSet();

	int		GetRequireMoney();
	int		GetType();
	WORD	GetNpcSvrIdx();
	enum{
		TYPE_NONE,
		TYPE_NPC,
		TYPE_SKILL
	};

private:
	int		IsValidUpgradeCondition();
	void	SetTargetItem( CItem* pItem );
	void	SetMaterialItem( CItem* pItem );

	int		ComputeRequireMp();
	int		ComputeSuccessProb();


private:
	CItemFragment*					m_pTargetItem;			/// �����ϰ��� �ϴ� Ÿ�� ������
	CItemFragment*					m_pMaterialItems[3];       /// ������ �κ��丮���� �÷����� �������۸���Ʈ
	CRequireMaterial*				m_pRequireMaterialItems[3];/// ���ÿ� �䱸�Ǵ� �����۸���Ʈ
	
	std::list<tag_SET_INVITEM>		m_ResultItemSet;
	BYTE							m_btResult;
	int								m_iResultSuccessProb;

	CTEventUpgrade					m_Event;

	int								m_iNpcSvrIdx;
	int								m_iSkillSlotIdx;
	int								m_iUseItemInvenIdx;

	int								m_iRequireMoney;
	int								m_iRequireMp;
	int								m_iSuccessProb;

};
#endif