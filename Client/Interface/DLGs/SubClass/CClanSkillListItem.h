#ifndef _CClanSkillListItem_
#define _CClanSkillListItem_

#include "winctrl.h"
#include "CSlot.h"
class CIconSkillClan;
class CDragItem;

/**
* Ŭ��â���� Ŭ�� ��ų ����Ʈ�� item class
*
* @Author		������
* @Date			2005/9/15
*/


class CClanSkillListItem :	public CWinCtrl
{
public:
	CClanSkillListItem(void);
	virtual ~CClanSkillListItem(void);

	virtual void Draw();
	virtual void MoveWindow( POINT pt );
	virtual void Update( POINT ptMouse );
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual void Show();
	virtual void Hide();
	
	void			SetIcon( CIconSkillClan* pIcon );
	CIconSkillClan*	GetIcon();

protected:
	CSlot		m_Slot;
	//CTButton*	m_pButton;
	CDragItem*	m_pDragItem;
};
#endif