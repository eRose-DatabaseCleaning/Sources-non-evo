#ifndef _CMsgBoxSpecial_
#define _CMsgBoxSpecial_
#include "tmsgbox.h"

/**
* �������� ���� ��⸦ ǥ���ϱ� ���� ���̾�α�
*	- �Ϲ� �޼��� �ڽ��� �����ϱ� ���Ͽ� ������ Ŭ������ �����Ͽ���
*
* @Author		������
* @Date			2005/9/12
*/
class CMsgBoxSpecial :	public CTMsgBox
{
public:
	CMsgBoxSpecial(void);
	virtual ~CMsgBoxSpecial(void);

	virtual unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);
	virtual void Hide();

	void SetCommand( CTCommand* pCmdOk, CTCommand* pCmdCancel );
	void SetInvokerDlgID( unsigned int iDlgID );
	enum{
		IID_BTN_OK		= 255,
		IID_BTN_CANCEL	= 256
	};
protected:
	void SendCommand( unsigned int iDlgID, CTCommand* pCmd );
	void ClearCommand();

protected:
	CTCommand*			m_pCmdOk;			/// Ok Btn�� �������� ó���� Command
	CTCommand*			m_pCmdCancel;		/// Cancel Btn�� �������� ó���� Command
	unsigned int		m_iInvokerDlgID;	/// Command �� ó���� TDialogID
};
#endif