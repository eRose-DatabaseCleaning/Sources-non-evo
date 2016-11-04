#ifndef _TCONTAINER_
#define _TCONTAINER_

#include "TObject.h"
#include <deque>

/**
* TObject�� Item���� ������ Container Class
*	- Subscript�� 1-Base�� �����Ѵ�.
*	- ���������� �������� ������ �� �ִ�.
*
* @Author	������
*
* @Date		2005/8/30 
*/
class TGAMECTRL_API CTContainer : public CTObject
{
	std::deque< CTObject* >	m_listItem;
	unsigned				m_uSelectedItemIndex;

public:
	CTContainer(void);
	~CTContainer(void);

	void	Clear();
	void	AddItem( CTObject* item );

	const CTObject*	GetItem( unsigned iSubscript );
	const CTObject*	GetSelectedItem();
	
	 
	unsigned		GetSelectedItemIndex();
	void			SelectItem( unsigned iSubscript );
	
	unsigned		GetSize();
	std::deque< CTObject* >&	GetItemList();
};
#endif