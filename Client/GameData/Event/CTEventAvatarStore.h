#ifndef _CTEventAvatarStore_
#define _CTEventAvatarStore_
#include "ctevent.h"
class CTEventAvatarStore :	public CTEvent
{
public:
	CTEventAvatarStore(void);
	virtual ~CTEventAvatarStore(void);
	enum{
		EID_NONE,
		EID_DELETE_ITEM_SELLLIST,///�Ǹ��ǿ� �ִ� �������� ��������.
	};
	void	SetIndex( int iIndex );
	int		GetIndex();
protected:
	int	m_iIndex;
};
#endif