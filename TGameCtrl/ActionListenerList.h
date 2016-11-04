#ifndef _CActionListenerList_
#define _CActionListenerList_

#include "TGameCtrl.h"
#include <list>

class IActionListener;

//---------------------------------------------------------------------------------------------------
/// Client���� CWinCtrl�� IActionListener�� �߰��� �����ϴ� Class
///
///	* Client���� CWinCtrl�� ActionListenerList�� IActionListener�� �߰��ؼ� 
///		CWinCtrl::Process���� ���� Client�� ó�� ��ƾ�� ȣ���Ҽ� �ֵ��� �ϱ� ���� System�� ���
///
/// * ���� Class : CActionListenerList, IActionListener, CActionEvent
///
/// @Author ������
/// @Date 2005/8/19 
//---------------------------------------------------------------------------------------------------
class TGAMECTRL_API CActionListenerList
{
public:
	CActionListenerList(void);
	~CActionListenerList(void);
	
	void	Add( IActionListener* pListener );
	void	Remove( IActionListener* pListener );
	int		GetCount();
	bool	IsEmpty();
	std::list< IActionListener* >& GetListenerList( );
protected:
	std::list< IActionListener* >	m_list;
};
#endif