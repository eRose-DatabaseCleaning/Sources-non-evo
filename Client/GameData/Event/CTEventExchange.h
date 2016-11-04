#ifndef _CTEventExchange_
#define _CTEventExchange_
#include "ctevent.h"
class CItem;
class CTEventExchange :	public CTEvent
{
public:
	CTEventExchange(void);
	virtual ~CTEventExchange(void);

	enum{
		EID_NONE = 0,
		EID_ADD_MYITEM,				/// �������� �߰�
		EID_REMOVE_MYITEM,			/// �������� ����
		EID_ADD_OTHERITEM,			/// ���� ������ �߰�
		EID_REMOVE_OTHERITEM,		/// ���� ������ ����
		EID_CHANGE_READYOTHER		/// ���� ���� ���� ����
	};

	void    SetSlotIndex( int iIndex );
	void	SetItem( CItem* pItem );
	void	SetReadyOther( bool bReady );

	CItem*  GetItem();
	bool	GetReadyOther();
	int		GetSlotIndex();
protected:
	int			m_iSlotIndex;
	CItem*		m_pItem;
	bool		m_bReadyOther;
};


#endif