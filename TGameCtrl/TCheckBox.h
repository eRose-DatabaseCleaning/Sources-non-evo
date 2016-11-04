#ifndef _CTCheckBox_
#define _CTCheckBox_
#include "winctrl.h"

/**
* üũ�ڽ� Class
*
* @Author	������
*
* @Date		2005/8/30
*/
class TGAMECTRL_API CTCheckBox : public CWinCtrl
{
public:
	CTCheckBox(void);
	virtual ~CTCheckBox(void);
	bool		Create(int iScrX, int iScrY, int iWidth, int iHeight , int iModuleID, int iCheckImageID, int iUncheckImageID );
	virtual unsigned	Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual void		Draw();

	bool	IsCheck();
	void	SetCheck();
	void	SetUncheck();


protected:
	enum{
		CBS_UNCHECK,
		CBS_CHECK
	};


	int	m_iCheckState;					/// üũ Yes_or_Not
	int	m_iModuleID;					/// �̹������� ��� ID
	int m_iCheckImageID;				/// üũ �����ϰ�� �̹��� ID
	int m_iUncheckImageID;				/// üũ �ȵ� �����ϰ�� �̹��� ID

};
#endif