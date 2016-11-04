#ifndef _CItemSlot_
#define _CItemSlot_
#include <vector>
#include "../../common/citem.h"
#include "../../gamecommon/cobservable.h"
class CItem;
class CTCommand;
class CTEventItem;


/**
* CItem �����̳� Ŭ����
*	- ������ ������ �ҽ��� �����ؼ� ����( ����:2005/9/15�� �и���) tagITEM�� ������ CInventory�� �����������̽� ������ ���� �κп��� ������ �־�
*		tagITEM�� CItem���� CInventory�� CItemSlot���� wrapping�Ͽ� ����Ѵ�.
*	- Observable�ν� ������ �������� ��/Ż��, ����, ������ ���� Observer�鿡�� �˷��ش�.
*
* @Author		������
* @Date			2005/9/15
*/
class CItemSlot : public CObservable
{
public:
	CItemSlot(void);
	~CItemSlot(void);

	CItem*	AddItem( int iInvenIdx, tagITEM& Item );
	CItem*	AddItem( tagITEM& Item );
	bool	DelItem( int iInvenIdx );
	int		SetItem( int iInvenIdx, tagITEM& Item );
	void	SubItem( int iInvenIdx, tagITEM& Item );
	void	UpdateItem( int iInvenIdx, tagITEM& Item );
	void	ChangeItemStatus( int iInvenIdx );
	CItem*	GetItem( int iInvenIdx );
	int		GetEmptySlot( int iStartIdx, int iEndIdx );
	void	Clear();
private:
	CItem*	CreateItem( int iInvenIdx, tagITEM& Item );

private:
	
	std::vector< CItem*	>	m_listItems;

	CTCommand*	m_pCmdItemEquipInInventory;
	CTCommand*  m_pCmdItemUseInInventory;
	CTCommand*  m_pCmdItemEtcInInventory;
	CTCommand*	m_pCmdItemPatInInventory;
	CTCommand*	m_pCmdItemEquiped;
	CTCommand*  m_pCmdItemBullet;
	CTCommand*  m_pCmdItemPatEquiped;

	CTEventItem*	m_pEvent;
};
#endif