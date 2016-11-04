#ifndef _IActionListener_
#define _IActionListener_
//---------------------------------------------------------------------------------------------------
/// Client���� CWinCtrl�� CActionListenerList�� ó����ƾ �߰��� ��ӹ޾ƾ��� �������̽�
///
///	* Client���� CWinCtrl�� ActionListenerList�� IActionListener�� �߰��ؼ� 
///		CWinCtrl::Process���� ���� Client�� ó�� ��ƾ�� ȣ���Ҽ� �ֵ��� �ϱ� ���� System�� ���
///
/// * ���� Class : CActionListenerList, IActionListener, CActionEvent
///
/// @Author ������
/// @Date	2005/8/19 
//---------------------------------------------------------------------------------------------------


#include "TGameCtrl.h"
class CActionEvent;
class TGAMECTRL_API IActionListener
{
public:
	virtual unsigned ActionPerformed (CActionEvent*  e) = 0;
};
#endif