#ifndef _CTFONTIMPL_
#define _CTFONTIMPL_
#include "../../TGameCtrl/ITFont.h"


/**
* �������̽� ���̺귯��(TGameCtrl)���� �˷��� ITFont�� Implemented Class
*  : TGameCtrl���������� ���ڸ� �׸��� �� �������̽��� ���ؼ� znzin�� �ش� �������̽��� ȣ���Ѵ�. 
*
* @Author		������
*
* @Date			2005/9/5
*/

class CTFontImpl :	public ITFont
{
public:
	CTFontImpl(void);
	~CTFontImpl(void);

	virtual void Draw( int iFont, bool bUseSprite, RECT* pRect, D3DCOLOR Color, DWORD dwFormat, const char * pMsg );
	virtual void Draw( int iFont, bool bUseSprite, int x, int y, D3DCOLOR Color,  const char * pMsg );

	/// �ش� �ε����� ��Ʈ�� ���� �׸����� Height�� ���ϴ� Method
	virtual int  GetFontHeight( int iFont );

	/// �ش� �ε����� ��Ʈ�� ���� �ؽ�Ʈ ��½� �ʺ�� ���̸� ���ϴ� Method
	virtual SIZE GetFontTextExtent ( int iFont, const char* pszText );
	
	/// �ؽ�Ʈ�� ��µ� ���� ��ġ�� �����ϴ� Method
	virtual void SetTransformSprite( float x, float y, float z );
};

extern CTFontImpl	g_FontImpl;
#endif