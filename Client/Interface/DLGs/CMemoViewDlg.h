#ifndef _CMemoViewDlg_
#define _CMemoViewDlg_
#include "TDialog.h"

/**
* ���� ���� ����� ���̾�α�
*
* @Author		������
* @Date			2005/9/12
*/
class CMemoViewDlg : public CTDialog
{
public:
	CMemoViewDlg(void);
	virtual ~CMemoViewDlg(void);
	virtual bool Create( const char* IDD );
	virtual void Draw();
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );


	void	SetTargetName( const char* pszName );
	void	SetContent( const char* pszContent );
	void	SetID( int iID );

private:
	enum{
		IID_BTN_CLOSE	= 10,
		IID_BTN_OK		= 11,
		IID_BTN_DELETE  = 12,
	};

	int			m_iID;							/// �������� ������ ���� �ε���
	std::string	m_strName;						/// ���� ��� �̸�
	std::string m_strContent;					/// ���� ����

};
#endif