#ifndef _CNUMBERINPUTDLG_
#define _CNUMBERINPUTDLG_

#include "../../../TGameCtrl/TDialog.h"
class CTCmdNumberInput;

/**
* �������� �̵��ó� ��Ÿ ���� �Է½� ���Ǵ� ���̾�α�
*
* @Warning		CTCmdNumberInput�� ��ӹ��� Command������ �Է��� ������ Command�� �����Ҽ� �ִ�.
* @Author		������
* @Date			2005/9/12
*/

class CNumberInputDlg :	public CTDialog
{
public:
	CNumberInputDlg(void);
	virtual ~CNumberInputDlg(void);

	virtual void Update( POINT ptMouse );
	virtual void Show();
	virtual void Hide();
	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );

	enum{
		IID_EDITBOX			= 2,
		IID_BUTTON_CLOSE	= 3,
		IID_BTN_DEL			= 4,
		IID_BTN_OK			= 5,
		IID_BTN_MAX			= 6,
		IID_BTN_0			= 10,
		IID_BTN_1			= 11,
		IID_BTN_2			= 12,
		IID_BTN_3			= 13,
		IID_BTN_4			= 14,
		IID_BTN_5			= 15,
		IID_BTN_6			= 16,
		IID_BTN_7			= 17,
		IID_BTN_8			= 18,
		IID_BTN_9			= 19,
	};
	
	__int64		GetLastInputNumber(){ return m_iLastInputNumber; }

	void		SetMaxNumber( __int64 iMaxNumber){ m_iMaxNumber = iMaxNumber; }
	void		SetCommand( CTCmdNumberInput* pCmd , CTObject* pObj );

protected:

	void		OnLButtonUp( unsigned iProcID );

	void		AppendText( char* szText );
	void		DeleteOneChar();

	__int64		GetNumberFromEditBox();

	//void		ChangeButtonState( unsigned int iID, int iState ); ������
	void		SetFocusEditBox();
	void		SendCommand();
	void		ClearCommand();

protected:
	CTCmdNumberInput*	m_pCmd;										/// �Է� �Ϸ�� ������ CTCommand
	CTObject*			m_pCommandParam;							/// m_pCmd ó���� �ʿ��� ���ڸ� �ӽ� ����

	__int64				m_iLastInputNumber;							/// ������ �Էµ� ����
	__int64				m_iMaxNumber;								/// �ִ� �Է°����� ����
};
#endif