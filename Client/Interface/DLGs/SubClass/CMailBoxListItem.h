#ifndef _CMailBoxListItem_
#define _CMailBoxListItem_
#include "winctrl.h"

/**
* Ŀ�´�Ƽ â�� ������ List�� ���Ǵ� item class
*
* @Author		������
* @Date			2005/9/15
*/

class CMailBoxListItem : public CWinCtrl
{
public:
	CMailBoxListItem( const char* pszName, const char* pszMemo ,DWORD dwRecvTime );
	virtual ~CMailBoxListItem(void);
	virtual void Draw();
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual void SetSelected();

protected:
	std::string	m_strName;
	std::string m_strMemo;
	SYSTEMTIME	m_RecvTime;
};
#endif