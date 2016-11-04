#ifndef _CActionEvent_
#define _CActionEvent_

#include "TGameCtrl.h"

class CWinCtrl;

//---------------------------------------------------------------------------------------------------
/// CWinCtrl�� CActionListenerList�� �߰��Ǿ� �ִ� IActionListener�� ȣ��� ���ڷ� �ѱ� Class
///
///			* Client���� CWinCtrl�� ActionListenerList�� IActionListener�� �߰��ؼ� 
///				CWinCtrl::Process���� ���� Client�� ó�� ��ƾ�� ȣ���Ҽ� �ֵ��� �ϱ� ���� System�� ���
///
/// ���� Class : CActionListenerList, IActionListener, CActionEvent
///
/// @author	������
/// @date 2005/8/19
//---------------------------------------------------------------------------------------------------
class TGAMECTRL_API CActionEvent
{
public:
	CActionEvent();
	CActionEvent( CWinCtrl* pSource , unsigned uiID , unsigned wParam ,unsigned lParam );
	virtual ~CActionEvent(void);


	CWinCtrl*	GetSource();
	unsigned	GetID();
	unsigned	GetWParam();
	unsigned	GetLParam();

	void		SetSource( CWinCtrl* pSource );
	void		SetID( unsigned uiID );
	void		SetWParam( unsigned wParam );
	void		SetLParam( unsigned lParam );
protected:

	///Event�� ó���� CWinCtrl�� ������
	CWinCtrl*		m_pSource;	
	///Event ID : Window Message�ϰ�� Message ID
	unsigned		m_uiID;		
	///Window Message�ϰ��WParam : �ƴҰ�� ���Ƿ� ��밡��
	unsigned		m_uiWParam; 
	///Window Message�ϰ��LParam : �ƴҰ�� ���Ƿ� ��밡��
	unsigned		m_uiLParam; 
};
#endif