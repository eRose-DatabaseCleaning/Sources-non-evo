#ifndef _CInputNameDlg_
#define _CInputNameDlg_

#include "tdialog.h"

class CTCmdInputName;


/**
* �̸��� ��Ʈ�� �Է½� �������� ���Ǵ� ���̾�α�
* 
* @Warning		�׻� �� ���̾�α׸� ����ϰ� ������ �ʴ�.
* @Author		������
* @Date			2005/9/14
*/
class CInputNameDlg : public CTDialog
{
public:
	CInputNameDlg(void);
	virtual ~CInputNameDlg(void);

	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

	virtual void Hide();
	void	SetCommand( CTCmdInputName* m_command );
private:
	bool	ExecuteCommand();
	enum{
		IID_BTN_CLOSE	= 10,
		IID_BTN_CONFIRM = 11,
		IID_EDITBOX		= 20
	};

	CTCmdInputName* m_command;
};

#endif