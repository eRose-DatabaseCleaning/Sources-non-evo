#ifndef _ChatRoomListItem_
#define _ChatRoomListItem_
#include "winctrl.h"

/**
* Ŀ�´�Ƽ â�� ��ȭ�� List�� ���Ǵ� item
*
* @Author		������
* @Date			2005/9/15
*/

class CChatRoomListItem : public CWinCtrl
{
public:
	CChatRoomListItem( WORD wID, BYTE btRoomType, BYTE btUserCount, const char* pszTitle );
	virtual ~CChatRoomListItem(void);
	virtual void Draw();
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual void SetSelected();

protected:
	WORD		m_wID;
	BYTE		m_btRoomType;
	BYTE		m_btUserCount;
	std::string m_strTitle;

};
#endif