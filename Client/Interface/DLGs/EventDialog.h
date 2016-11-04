#ifndef _CEventDialog_
#define _CEventDialog_

#include "TDialog.h"
#include "JStringParser.h"


/**
* �����ۻ��� ������ ��ȭâ�� Event �� ������ ���̾�α�
*	- �⺻ ������ CDialogDlg�� �����ϴ�
* 
* @Warning		�븸������ ������������ ǥ�� ����� �ٸ���(XML����Ÿ�� Ʋ���� �����ڵ�� �ڵ尡 ���еȴ�)
* @Author		������
* @Date			2005/9/14
*/

class CEventDialog : public CTDialog
{
public:
	CEventDialog(void);
	virtual ~CEventDialog(void);

	virtual void Hide();
	virtual void Draw();
	virtual bool Create( const char* IDD );
	virtual void Show();

	virtual unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam );


	void SetScript( char* pszScript );
	void AddAnswerExample( char* pszScript, int iEventID, void (*fpEventHandle)(int iEventID) );
	void SetTargetNpcClientObjectIndex( short iIndex );
	int	 IsInValidShow();

protected:
	enum{
		IID_BG_IMAGE				= 1,
		IID_BTN_CLOSE				= 10,
		IID_ZLISTBOX_NPCSCRIPT		= 20,
		IID_ZLISTBOX_ANSWER_EXAMPLE = 21
	};


	short			m_nTargetClientIdx;
	std::string		m_strNpcName;
	std::string		m_strTempScript;
	HNODE			m_hNpcFace;
	int				m_widthNpcFace;
	int				m_heightNpcFace;

	CJStringParser	m_Script;


};
#endif