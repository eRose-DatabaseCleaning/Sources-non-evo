#ifndef _CITStateNpcDialog_
#define _CITStateNpcDialog_
#include "citstate.h"



/**
* NPC��ȭ�� �������̽�(IT_MGR)����
* - 2005/9/6�� ���� �븸 ���������� ����Ѵ�.
* - DLG_TYPE_DIALOG,  DLG_TYPE_SELECTEVENT , DLG_TYPE_EVENTDIALOG  ���߿� �Ѱ��� Show�Ǿ� ����
*
* @Author		������
*
* @Date			2005/9/6
*/
class CITStateNpcDialog : public CITState
{
public:
	CITStateNpcDialog(void);
	virtual ~CITStateNpcDialog(void);

	virtual void Enter();
	virtual void Leave();

	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual void Update( POINT ptMouse );
	virtual bool Draw();
};
#endif