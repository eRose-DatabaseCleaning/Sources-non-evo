#ifndef _CJString_
#define _CJString_

#include "JLetter.h"
#include "TGameCtrl.h"
#include <list>

//------------------------------------------------------------------------------------------------------------------------
/// ���ڿ���  ����� Draw�ϴ� Ŭ����
///
/// @Author				������
///
/// @Date				2005/08/30
//------------------------------------------------------------------------------------------------------------------------

class TGAMECTRL_API CJString
{
public:
	CJString(void);
	~CJString(void);
	void	AddString( const char* pszString, SIZE sizeString, int iFont, DWORD dwColor );
	void    Clear();
	void	Draw( int x, int y , bool bTransform, DWORD force_text_color = 0, bool bTrans2 = false);
	bool	IsEmpty();
	int		GetStringHeight();

protected:
	int						m_iMaxHeight;													/// ���ڿ��� �ִ� ���� , AddString�ÿ� �־��� sizeString�� ���ؼ� �ִ밪�� �����Ѵ�.
	std::list< CJLetter >	m_Letters;

};
#endif