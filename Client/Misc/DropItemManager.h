#ifndef _DROPITEMMANAGER_
#define _DROPITEMMANAGER_

#include <list>


struct stDROPITEM
{
	int		m_iDamageOwner;
	int		m_iDropItemIndex;
};

///
/// ��Ȯ�� Ÿ�ֿ̹� �������� ����ϱ� ���� Ŭ����..
///

class CDropItemManager
{
private:
	std::list< stDROPITEM >			m_DropItemList;


public:
	CDropItemManager(void);
	~CDropItemManager(void);

	void	PushDropItem( stDROPITEM& DropItem ){ m_DropItemList.push_back( DropItem ); }
	/// Drop item ������ ���ŵǾ���.
	int	UpdateDropItemList( int iDropItemIndex );
};

extern CDropItemManager	g_DropItemManager;

#endif // _DROPITEMMANAGER_	