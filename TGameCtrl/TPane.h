#ifndef _CTPane_
#define _CTPane_
#include "winctrl.h"
#include "JContainer.h"

/**
* Dialog�������� �������� child control���� �����ϱ� ���� Container class
*   - Ŭ���̾�Ʈ�� CChatRoomDlg���� ��/�Ʒ� ������ �� Container class�� ���еǾ� �ִ�.
*
* @Author		������
* @Data			2005/8/30
*/
class TGAMECTRL_API CTPane : public CWinCtrl
{
public:
	CTPane(void);
	virtual ~CTPane(void);

	virtual void Update( POINT ptMouse );
	virtual void Draw();
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual void MoveWindow( POINT pt );
	virtual void Show();
	virtual void Hide();

	void		AddChild( CWinCtrl* pCtrl );
	CWinCtrl*	FindChild( unsigned iID );
	CWinCtrl*	Find( const char * szName );
	//CJContainer* CTPane::FindChildPane();

protected:
	CJContainer		m_Children;
};
#endif