#ifndef _CDialogNpcScriptItem_
#define _CDialogNpcScriptItem_

#include "winctrl.h"
#include "JStringParser.h"
/**
* NPC���� ��ȭ�� NPC SCRIPT�� ��µǴ� ZLISTBOX�� ���� Item Class
*
* @Author		������
* @Date			2005/9/15
*/
class CDialogNpcScriptItem : public CWinCtrl
{
public:
	CDialogNpcScriptItem( int iIndex, char* pszScript ,int iImageID, int iLineHeight, int iWidth , int iBorderWidth );
	virtual ~CDialogNpcScriptItem(void);
	virtual void Draw();

protected:
	int	m_iModuleID;
	int m_iImageID;
	int m_iLineCount;///�̹��� �׸� ����ī��Ʈ
	int m_iLineHeight;
	int m_iBorderWidth;

	CJStringParser	m_Script;
};
#endif