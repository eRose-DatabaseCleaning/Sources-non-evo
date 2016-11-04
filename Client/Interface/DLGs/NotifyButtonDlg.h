#ifndef _CNotifyButtonDlg_
#define _CNotifyButtonDlg_
#include "TDialog.h"
#include <list>
class CTButton;

/**
* �˸���ư���� �����ֱ� ���� �������̽�
*	- CTDialog�� ��ӹ޾����� TDialog�� ����� ���� �Ⱦ��� �ִ�
*
* @Author		������
* @Date			2005/9/15
*/
class CNotifyButtonDlg : public CTDialog
{
public:
	CNotifyButtonDlg(void);
	virtual ~CNotifyButtonDlg(void);

	virtual unsigned Process( unsigned uiMsg, WPARAM wParam ,LPARAM lParam );
	virtual void Draw();
	virtual void Update( POINT ptMouse );

	void    Clear();
	void	AddButton( CTButton* pBtn );
	void	UpdatePosition();

protected:


	std::list< CTButton* > m_Children;
};
#endif