#ifndef _RESTARTDLG_
#define _RESTARTDLG_


#include "TDialog.h"

/**
* �׾����� ��Ȱ ������ �����ϴ� ���̾�α�
*
* @Warning		�븸���������� ���� �ʵ��Ȱ ������ �Ҽ� ����( XML���� ��ư�κ��� ���� )
* @Author		������
* @Date			2005/9/14
*/
class CRestartDLG  : public CTDialog
{
public:
	CRestartDLG();
	~CRestartDLG();
	
	virtual void Show();
	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );

protected:

	enum
	{
		BTN_RESTART_SAVE_POSITION = 3,			/// ����� ������ 
		BTN_RESTART_CURRENT_FIELD,				/// ���� �ʵ忡�� ( �븸���������� ������: XML Date)
		BTN_RESTART_DUNGEON_GATE,				/// �����Ա��� ( �븸�������� �����ȿ��� �׾������ �����Ա����� ��Ȱ�Ҽ� �ֵ��� ��ư �߰� )
	};


	bool On_LButtonUP( unsigned iProcID);
};


#endif //_RESTARTDLG_