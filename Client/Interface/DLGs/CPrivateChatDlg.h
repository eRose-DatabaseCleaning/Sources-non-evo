#ifndef _PrivateChatDlg_
#define _PrivateChatDlg_
#include "TDialog.h"
#include "IActionListener.h"

/**
* 1:1��ȭ�� ���Ǵ� ���̾�α�
*
* @Author		������
* @Date			2005/9/12
*/
class CPrivateChatDlg :	public CTDialog, public IActionListener
{
public:
	CPrivateChatDlg( int iDlgType );
	virtual ~CPrivateChatDlg(void);

	virtual void		Draw();
	virtual void		Hide();
	virtual bool		Create( const char* IDD );
	virtual unsigned	Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

	virtual unsigned ActionPerformed( CActionEvent* e );


	void	SetOther( DWORD dwUserTag, BYTE btStatus, const char* pszName );
	void	RecvChatMsg( DWORD dwUserTag, const char* pszMsg );

private:
	void	SendChatMsg();
	void	AddChatMsg( const char* pszMsg );

private:
	enum{
		IID_BTN_CLOSE	= 10,
		IID_EDITBOX		= 20,
		IID_LISTBOX		= 30,
		IID_SCROLLBAR	= 31,
	};

	DWORD		m_dwUserTag;		/// ������ ���� �±�
	std::string m_strName;			/// ������ �̸�
	BYTE		m_btStatus;			/// ������ ����
};
#endif