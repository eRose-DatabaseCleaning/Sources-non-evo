#ifndef _ITFONT_
#define _ITFONT_

#include "TGameCtrl.h"
#include <crtdbg.h>

//------------------------------------------------------------------------------------------------------------------------
/// Client�� ��ӹ޾� �����ؾ��� Draw Font Interface - TGameCtrl�� �� �������̽��� ���ؼ� Font�� Draw�Ѵ�.
///
/// @Author		������
/// @Date		2005/08/23 
//------------------------------------------------------------------------------------------------------------------------

class TGAMECTRL_API ITFont
{
public:
	virtual void Draw( int iFont, bool bUseSprite, RECT* pRect, D3DCOLOR Color, DWORD dwFormat, const char * pMsg ) = 0 /*{ _ASSERT( 0 && "ITFont::Draw1" ); }*/;
	virtual void Draw( int iFont, bool bUseSprite, int x, int y, D3DCOLOR Color, const char * pMsg ) = 0 /*{ _ASSERT( 0 && "ITFont::Draw2"); }*/;
	virtual int  GetFontHeight( int iFont ) = 0/* { _ASSERT( 0 && "ITFont::GetFontHeight" ); return 0; }*/;
	virtual SIZE GetFontTextExtent ( int iFont, const char* pszText ) = 0;
	virtual void SetTransformSprite( float x, float y, float z = 0.0f) = 0/* { _ASSERT( 0 && "ITFont::SetTransformSprite" );}*/;
};

#endif