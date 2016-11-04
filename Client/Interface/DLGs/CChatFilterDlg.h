#ifndef _CChatFilterDlg_
#define _CChatFilterDlg_

#include "tdialog.h"
#include "Chattingdlg.h"


/**
* ä���Է�â���� �Է��Ǻ��� ������ ä�� Ÿ���� �����Ҽ� �ִ� ���̾�α�
*
* @Author		������
* @Date			2005/9/12
*/
class CChatFilterDlg : public CTDialog
{
public:
	CChatFilterDlg(void);
	virtual ~CChatFilterDlg(void);
	virtual void Show();
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam , LPARAM lParam );

	ChatFilter m_filter;					/// �Ǻ� ���� �������� ���� ����									

protected:
	enum{
		CHECKBOX_ALL	= 10,
		CHECKBOX_SYSTEM = 11,
		CHECKBOX_TRADE  = 12,
		CHECKBOX_PARTY  = 13,
		CHECKBOX_CLAN   = 14,
		CHECKBOX_ALLIED = 15,
	};
};
#endif