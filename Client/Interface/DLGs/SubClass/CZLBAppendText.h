#ifndef _CZLBAppendText_
#define _CZLBAppendText_
#include "winctrl.h"
#include "JStringParser.h"
/**
* ZListBox�� �±׵��� �����ؼ� Append �ϱ� ���� ������ Ŭ����.
*
* @Author		������
* @Date			2006/10/24
*/
class CZLBAppendText :	public CWinCtrl
{
public:
	CZLBAppendText();

	CZLBAppendText( const char* pszText,int iWidth, DWORD dwColor = D3DCOLOR_ARGB( 255, 255, 255, 255));
	CZLBAppendText( CJString ptJString, int iWidth, DWORD dwColor = D3DCOLOR_ARGB( 255, 255, 255, 255));

	~CZLBAppendText(void);

	void Draw();

protected: 

	std::string		m_pszText;
	int				m_iLineHeight;

	int				m_iBorderWidth;

	CJStringParser	m_jString;

	CJString*		m_pJString;	
};
#endif