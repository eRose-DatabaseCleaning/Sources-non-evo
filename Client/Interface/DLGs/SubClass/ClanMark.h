#ifndef _CClanMark_
#define _CClanMark_
#include "winctrl.h"

/**
* Ŭ�� â���� ������ �����Ҽ� �ִ� Ŭ����ũǥ�� class
*
* @Author		������
* @Date			2005/9/15
*/
class CClanMark : public CWinCtrl
{
public:
	CClanMark(void);
	virtual ~CClanMark(void);
	virtual void Draw();
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	void SetGraphicID( int iID );
	void SetModuleID( int iID );

protected:
	int	m_iGraphicID;
	int m_iModuleID;
};
#endif