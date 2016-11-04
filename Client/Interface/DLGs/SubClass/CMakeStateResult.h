#ifndef _CMAKESTATERESULT_
#define _CMAKESTATERESULT_
#include "cmakestate.h"
#include <vector>
class CGuage;

/**
* �����������̽� â�� State Class : ������ ���� ���� ��������� ����ϴ� ����
*
* @Author		������
* @Date			2005/9/15
*/
class CMakeStateResult : public CMakeState
{
public:

	CMakeStateResult( CMakeDLG* pParent);
	virtual ~CMakeStateResult(void);
	virtual void Draw();
	virtual void Update( POINT ptMouse);
	virtual void Show();
	virtual void Hide();
	virtual void Init();
	virtual unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual void MoveWindow(POINT ptPos);
	virtual bool IsModal();

protected:
	std::vector<CGuage*>	m_listGuage;
	int			m_iRedGuageImageID;
	int			m_iGreenGuageImageID;
	int			m_dwPrevTime;
	bool		m_bWaitState;///�޼��� �ڽ��� ���������� ��ٸ���.

};
#endif