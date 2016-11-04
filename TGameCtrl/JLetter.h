#ifndef _CJLetter_
#define _CJLetter_

#include <string>
#include "TGameCtrl.h"

//------------------------------------------------------------------------------------------------------------------------
///	���ڸ� ������ Ŭ����
/// - ����class			CJString
///
/// @Author				������
///
/// @Date				2005/08/30
//------------------------------------------------------------------------------------------------------------------------

class TGAMECTRL_API CJLetter
{
public:
	CJLetter();
	~CJLetter(){}

	void	SetLetter( const char* pszLetter );
	void	SetSize( SIZE& size );
	void	SetFont( int iFont );
	void	SetColor( DWORD dwColor );

	const SIZE& GetSize();
	int			GetFont();
	DWORD		GetColor();
	const char* GetLetter();


private:
	int				m_iFont;						/// Client�� ���ǵ� Font Index : �߸��� Index�ϰ�� �ȳ����ų� �߸� ���ü� �ִ�(Client�� ������ ���� Ʋ�� )
	DWORD			m_dwColor;						/// ����				
	SIZE            m_Size;							/// Draw�� Width,Height
	std::string		m_Letter;						

};
#endif