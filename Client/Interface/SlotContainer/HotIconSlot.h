#ifndef _CHOTICONSLOT_
#define _CHOTICONSLOT_

#include "../../Common/CHotICON.h"

class CIcon;
class CHotICONS;


/**
* CItemSlot�� ���� ������ ������� ����â������ �����̳� Ŭ����
*
* @Author		�̵���, ������
* @Date			2005/9/15
*/
class CHotIconSlot
{
private:


public:
	CHotIconSlot(void);
	~CHotIconSlot(void);

	void			SetHotIcons( CHotICONS* pHotIcon );
	void			AddQuickItem( int iSlot, int iType, int iIndex );

	CIcon*			GetHotIcon( int iSlot );

	tagHotICON		GetHotItem( int iSlot );
};


#endif //_CHOTICONSLOT_
