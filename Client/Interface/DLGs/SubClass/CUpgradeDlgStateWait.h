#ifndef _CUpgradeDlgStateWait_
#define _CUpgradeDlgStateWait_

#include "cupgradedlgstate.h"

/**
* �����������̽� â�� State Class : ����ڰ� �������� ������ ��û�ϰ� ������ ����ϴ� ����
*	
* @Warning		�̻����� ��� ��� ������ �Է��� �����Ѵ�.
* @Author		������
* @Date			2005/9/15
*/
class CUpgradeDlgStateWait : public CUpgradeDlgState
{
public:
	CUpgradeDlgStateWait(void);
	virtual ~CUpgradeDlgStateWait(void);

	virtual void Enter(){}
	virtual void Leave(){}
	virtual void Update( POINT ptMouse ){}
	virtual void Draw(){}
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam ){ return 1; }

};
#endif