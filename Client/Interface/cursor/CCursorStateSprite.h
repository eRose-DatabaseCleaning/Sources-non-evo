#ifndef _CCursorStateSprite_
#define _CCursorStateSprite_

#include "ccursorstate.h"

/**
* �������� �̹����� Draw�ϴ� State Class( �⺻ Ŀ���� ���� )
* 
*
* @Author		������
*
* @Date			2005/9/6
*/

class CCursorStateSprite : public CCursorState
{
public:
	CCursorStateSprite(void);
	virtual ~CCursorStateSprite(void);
	virtual void Enter();
	virtual void Leave();
	virtual void Draw( POINT ptMouse );
	virtual int  SetCursorType( int iType );

private:
	HNODE			m_hTexture;
};
#endif