#ifndef _CBankWindowDlg_
#define _CBankWindowDlg_
#include "tdialog.h"
#include "SinglelineString.h"


/**
* ���࿡�� ���� �����ϰų� ã���� �ٸ����� �Է��ϴ� ���̾�α�
*
* @Author		������
* @Date			2005/9/12
*/
class CBankWindowDlg :	public CTDialog
{
public:
	CBankWindowDlg(void);
	virtual ~CBankWindowDlg(void);
	
	virtual void		Draw();
	virtual unsigned	Process( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	virtual void		PostMsg( unsigned msg, unsigned param1, unsigned param2 );
	virtual void		Update( POINT ptMouse );

	enum{
		IID_EDITBOX = 10,
		IID_BTN_OK  = 20,
		IID_BTN_CANCEL = 21
	};

	enum{
		TYPE_NONE		= 0,
		TYPE_SAVE,
		TYPE_WITHDRAW
	};

	enum{
		MSG_SETYPE = 1
	};
private:
	void OnLButtonUp( unsigned proc_id );

private:
	int					m_Type;					/// ����/ã�� ����
	CSinglelineString   m_Title;				/// ���̾�α� ĸ�ǿ� ������ �ؽ�Ʈ
	
};
#endif