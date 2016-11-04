#ifndef _CTabbedPanel_
#define _CTabbedPanel_

#include "winctrl.h"

class CJContainer;
class CTRadioButton;
class CTRadioBox;
class CTButton;

struct S_TabbedPane{
	int				m_iIndex;
	CTRadioButton*	m_pRadioButton;
	CJContainer*	m_pContainer;
};

//--------------------------------------------------------------------------------------*/
/// Tab Container Control
///		- S_TabbedPane�� child�� ������ �Ѱ��� S_TabbedPane ������Ʈ�� �Ѱ��� Tab�̵ȴ�
/// 
/// @Author				������
/// @Date				2005/8/30
//--------------------------------------------------------------------------------------*/
class TGAMECTRL_API CTabbedPane :	public CWinCtrl
{
public:
	CTabbedPane(void);
	virtual ~CTabbedPane(void);

	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual	void Update( POINT ptMouse );
	virtual	void Draw();
	virtual void Show();
	virtual void Hide();
	virtual void MoveWindow( POINT pt );
	virtual bool IsInside( int x, int y );

	unsigned AddTab( CTRadioButton* pButton, CJContainer* pContainer );		/// Tab �߰� - �ε����� �ο��ȴ�( 0-Base )
	CJContainer*	GetTabContainer( int iIndex );							/// �ش� �ε����� S_TabbedPane�� �����Ѵ�.

	void	 RemoveAll();													
	int		 GetSelectedIndex();											/// ���� ���õ� S_TabbedPane�� ����
	bool	 SetSelectedIndex( unsigned index );							/// �ش��ε����� S_TabbedPane�� �����Ѵ�

	/// â�� �ּ�ȭ ����� ���� �߰����� Method
	void	 SetMinimizeRect( const RECT& rc );								/// �ּ�ȭ�� ���� Set
	bool	 IsMinimize();													/// �ּ�ȭ �����ΰ�?
	void	 SetMinimize( bool bMinimize );									/// �ּ�/�ִ�ȭ
	void	 SetMinimizeButton( CTButton* pButton );						/// �ּ�ȭ ��ư Set


protected:
	std::list< S_TabbedPane >	m_Tabs;										/// child�� ������ Container
	CTRadioBox*					m_pRadioBox;								/// �ǹ�ư�� ���� ��ư���� ���� �ڽ��� ����Ǿ� �ִ�
	CTButton*					m_pMinimizeButton;							/// �ּ�ȭ ��ư

	bool						m_bMinimize;								/// �ּ�ȭ��
	RECT						m_rcMinimize;								/// �ּ�ȭ�� ����
};
#endif