#ifndef _CMAKESTATEWAIT_
#define _CMAKESTATEWAIT_
#include "cmakestate.h"

/**
* �����������̽� â�� State Class : ���������� ������ ��û�ϰ� ����� ����ϴ� ����
*
* @Warning		�� ������ ���� ������ �Է¿� ���� ó���� ���� �ʵ��� �Ѵ�.
* @Author		������
* @Date			2005/9/15
*/
class CMakeStateWait :public CMakeState
{
public:

	CMakeStateWait( CMakeDLG* pParent);
	virtual ~CMakeStateWait();

	virtual void Draw();
	virtual void Update( POINT ptMouse);
	virtual void Show();
	virtual void Hide();
	virtual void Init();
	virtual unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);
	virtual void MoveWindow(POINT ptPos ){}
	virtual bool IsModal();
};
#endif