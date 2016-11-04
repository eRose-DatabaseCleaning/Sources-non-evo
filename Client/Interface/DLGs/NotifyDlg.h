#ifndef _CNotifyDlg_
#include "TDialog.h"
#include "JStringParser.h"

/**
* �˸���ưŬ���� �ش� ������ ǥ���ϱ� ���� ���̾�α� 
*	- 2005/9/15�� ���� ä��â �����ʿ� ǥ�õǰ� �ִ�
*
* @Author		������
* @Date			2005/9/15
*/
class CNotifyDlg :	public CTDialog
{
public:
	CNotifyDlg(void);
	virtual ~CNotifyDlg(void);
	virtual void Draw();
	virtual void Hide();
	virtual bool Create( const char* IDD );
	virtual void Show();
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam ,LPARAM lParam );

	
	void	SetAdjPos();

	void	SetScript( const char* pszScript );
	void	Clear();

private:

	enum{
		IID_BTN_CLOSE = 10,
		IID_BTN_PREV  = 11,
		IID_BTN_NEXT  = 12
	};
	CJStringParser	 m_Script;
	int				 m_iExtentLine;
	int				 m_iDrawLine;

	
	

};
#endif