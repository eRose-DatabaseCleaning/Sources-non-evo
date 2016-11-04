#ifndef _CIconQuick_
#define _CIconQuick_
#include "cicon.h"

/**
* ���ٿ� ���Ǵ� Icon, ���������� �ٸ� �������� ������ �ִ�.
* 
* @Warning		�׻� CIcon�� virtual method�� �������־�� �Ѵ�.( ����� ������ �ִ� CIcon�� �����ϵ��� )
* @Author		������
* @Date			2005/9/6
*/
class CIconQuick : public CIcon
{
public:
	CIconQuick( CIcon* pIcon );
	virtual ~CIconQuick(void);

	virtual void	Draw();
	virtual void	Update( POINT ptMouse );
	virtual void	ExecuteCommand();
	virtual CIcon*	Clone();
	virtual void	Clear();
	virtual void	SetPosition( POINT pt );
	virtual void	GetToolTip( CInfo& ToolTip,  DWORD dwDialogType, DWORD dwType );
	virtual int		GetIndex();

	void			SetQuickBarSlotIndex( int iIndex );
	int				GetQuickBarSlotIndex();
	const			CIcon*	GetIcon();

protected:
	CIcon*			m_pIcon;															/// ���� ��� ó���� �� �����ܿ� ���� ������
	int				m_iQuickBarSlotIndex;												/// ���ٿ����� ���� �ε���
};
#endif