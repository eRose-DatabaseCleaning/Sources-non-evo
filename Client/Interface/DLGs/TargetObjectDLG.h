#ifndef _TARGETOBJECTDLG_
#define _TARGETOBJECTDLG_


#include "../../../TGameCtrl/TDialog.h"


/**
* ���� Ŭ���� Ÿ�ٿ� ���� ������ �����ִ� ���̾�α�
*
* @Warning		����( 2005/9/15)������� �ʰ� �ִ�.
* @Author		������
* @Date			2005/9/15
*/
class CTargetObjectDLG : public CTDialog
{

public:
	CTargetObjectDLG();
	virtual ~CTargetObjectDLG();

	
	// *---------------------------------------------------* //
	// Overrided function
	// *---------------------------------------------------* //
	virtual void Draw();
	virtual unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);
	// *---------------------------------------------------* //

};



#endif //_TARGETOBJECTDLG_