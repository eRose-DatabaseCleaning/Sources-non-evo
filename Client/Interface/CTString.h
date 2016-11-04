#ifndef _CTString_
#define _CTString_

/**
* �ʱ⿡ �ۼ��� string class : CInfo���� ������̴�
*
* @Author		������
* @Date			2005/9/5
*/
class CTString
{
public:
	CTString(void);
	~CTString(void);

	void SetString( const char* pszTxt );
	void SetColor( DWORD dwColor );
	void SetFormat( UINT uFormat );
	void SetFont( HNODE hFont );
	void Draw( RECT& rcDraw );

	std::string	m_strText;
	DWORD		m_dwColor;
	UINT		m_uFormat;
	HNODE		m_hFont;

};
#endif