#ifndef _CSYSTEMMSGDLG_
#define _CSYSTEMMSGDLG_

#include "TDialog.h"
#include "TSplitString.h"

/**
* ����ȭ�鿡�� ��� �߰��� ������ ���� ���������� �����ֱ� ���� ���̾�α�
* 
* @Author		������
* @Date			2005/9/14
*/ 
class CSystemMsgDlg : public CTDialog
{
public:
	CSystemMsgDlg(void);
	virtual ~CSystemMsgDlg(void);

	virtual void Draw();
	virtual void Update( POINT ptMouse );
	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );


	bool SetMessage( const char* szTitle,const char* szMsg, short iType, D3DCOLOR color = NULL, DWORD dwShowTime = 10000 ,HNODE hFont = NULL);

	enum{
		MSG_TYPE_NOTICE,	/// ��������
		MSG_TYPE_HELP		/// ��
	};

protected: 
	HNODE			m_hFont;			/// �̸� ������� �ִ� ��Ʈ�� ���� ��常 ������ �ִ´�.
	CTSplitString	m_Notice;			/// ��µ� ����( ���� + ���� )
	D3DCOLOR		m_Color;			/// ��µ� ����
	short			m_iMsgType;			/// ���� or ��
	DWORD			m_dwShowTime;		/// ȭ�鿡 ������ �ð�
	DWORD			m_dwMsgSetTime;		/// ȭ�鿡 �������� ������ �ð�

	std::string		m_stTitle;			/// ���� ��Ʈ��
	std::string		m_stMsg;			/// ���� ��Ʈ��
	int				m_iImageIndex;		/// ��濡 �׷��� �̹��� �׷��� ID
};
#endif