#ifndef _CTARGETMENU_
#define _CTARGETMENU_
#include "../../../TGameCtrl/TDialog.h"

class CTargetMenu :	public CTDialog
{
public:
	CTargetMenu(void);
	virtual ~CTargetMenu(void);

	virtual void Update(POINT ptMouse);
	virtual void Hide();
	virtual void Show();
	virtual unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);
	enum{
		IID_BTN_FRIEND = 1,
		IID_BTN_EXCHANGE,
		IID_BTN_PARTY,
		IID_BTN_INFORMATION
	};
	///���� �ٸ� �ƹ�Ÿ�� Ÿ������ ���õǾ��� ������ �� �ƹ�Ÿ�� Idx
	int		GetTargetAvatarID(){ return m_iTargetAvatarID; }
	void	SetTargetAvatarID( int iTargetAvatarID ){ m_iTargetAvatarID = iTargetAvatarID; }
protected:
	bool	OnLButtonUp( unsigned iProcID );

protected:
	int		m_iTargetAvatarID;

};
#endif