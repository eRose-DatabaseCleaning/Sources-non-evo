#ifndef _CClanRegistNotice_
#include "tdialog.h"

/**
* Ŭ�� �������� ��Ͽ� ���Ǵ� ���̾�α�
*
* @Author		������
* @Date			2005/9/12
*/
class CClanRegistNotice : public CTDialog
{
public:
	CClanRegistNotice(void);
	virtual ~CClanRegistNotice(void);
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

private:
	enum{
		IID_BTN_CONFIRM = 10,
		IID_BTN_CLOSE	= 11,
		IID_BTN_CANCEL	= 12,
		IID_EDIT_NOTICE_CONTENT = 22,
	};

};
#endif