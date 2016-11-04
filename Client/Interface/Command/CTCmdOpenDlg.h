#ifndef _CTCMDOPENDLG_
#define _CTCMDOPENDLG_
#include "tcommand.h"
#include "../dlgs/cmsgbox.h"

class CTCmdNumberInput;
class CTDialog;
class CIcon;


/// ���� �����ϰ� �ִ�.
class CTCmdOpenDlg : public CTCommand
{
public:
	CTCmdOpenDlg(void);
	~CTCmdOpenDlg(void);

	void	SetOpenDlgID(int iID){ m_iOpenDlgID = iID; }
	void	SetInvokerDlgID(int iID ){ m_iInvokerDlgID = iID; }
	virtual bool	Exec( CTObject* pObj ){ return true; }
protected:
	int		m_iOpenDlgID;
	int		m_iInvokerDlgID;
};

struct CreateMsgBoxData{
	bool		bModal;		///Modal/Modeless
	int			iMsgType;	///�ߺ��� �Ұ����Ұ�� 
	int			iButtonType;
	int			iInvokerDlgID;
	CTCommand*  pCmdOk;
	CTCommand*	pCmdCancel;
	std::string	strMsg;
	std::list<CIcon*>	m_Icons;///�޼��� �ڽ��� �������� �׸��� ���Ҷ� ���

	WORD		parm1;
	WORD		parm2;

	CreateMsgBoxData(){
		pCmdOk		= NULL;
		pCmdCancel	= NULL;
		bModal		= true;
		iMsgType	= 0;
		iButtonType = CMsgBox::BT_OK;
		iInvokerDlgID   = 0;
		strMsg			= "MessageBox";

		parm1			= 0;
		parm2			= 0;
	}
};

/// �޼��� �ڽ� ������ ����
class CTCmdCreateMsgBox : public CTCommand
{
public:
	CTCmdCreateMsgBox( CreateMsgBoxData& Data );
	virtual ~CTCmdCreateMsgBox(void);

	virtual bool Exec( CTObject* pObj );

protected:
	CreateMsgBoxData	m_Data;

};
/// �޼��� �ڽ� ����
class CTCmdDeleteMsgBox : public CTCommand
{
public:
	CTCmdDeleteMsgBox( CTDialog* pDlg );
	virtual ~CTCmdDeleteMsgBox(void ){};
	virtual bool Exec( CTObject* pObj );

protected:
	CTDialog*	m_pDialog;
};
/// 1:1 ��ȭâ ������ ����
class CTCmdOpenPrivateChatDlg : public CTCommand
{
public:
	CTCmdOpenPrivateChatDlg( DWORD dwUserTag , BYTE btStatus, const char* pszName );
	virtual ~CTCmdOpenPrivateChatDlg(){}
	virtual bool Exec( CTObject* pObj );

protected:
	DWORD		m_dwUserTag;
	BYTE		m_btStatus;
	std::string m_strName;
};

/// 1:1 ��ȭâ ����
class CTCmdClosePrivateChatDlg : public CTCommand
{
public:
	CTCmdClosePrivateChatDlg( DWORD dwUserTag );
	virtual ~CTCmdClosePrivateChatDlg(){}
	virtual bool Exec( CTObject* pObj );
protected:
	DWORD	m_dwUserTag;
};
#endif