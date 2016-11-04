#ifndef _CUpgradeDlgStateNormal_
#define _CUpgradeDlgStateNormal_

#include "cupgradedlgstate.h"

class CUpgradeDlg;
/**
* �����������̽� â�� State Class : ����ڰ� �����۹� ��Ḧ �غ��ϴ� ����
*
* @Author		������
* @Date			2005/9/15
*/
class CUpgradeDlgStateNormal : public CUpgradeDlgState
{
public:
	CUpgradeDlgStateNormal( CUpgradeDlg* pParent );
	virtual ~CUpgradeDlgStateNormal(void);

	virtual void Enter();
	virtual void Leave();
	virtual void Update( POINT ptMouse );
	virtual void Draw();
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

private:
	enum{
		IID_BTN_START	= 10,
		IID_BTN_CLOSE	= 11
	};

	CUpgradeDlg*	m_pParent;
};

#endif