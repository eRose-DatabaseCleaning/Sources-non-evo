#ifndef _CCursorStateWindow_
#define _CCursorStateWindow_
#include "ccursorstate.h"


/**
* ������ �⺻ Ŀ���� ������� ����ϴ�  State Class
* 
*
* @Author		������
*
* @Date			2005/9/6
*/
class CCursorStateWindow : public CCursorState
{
public:
	CCursorStateWindow(void);
	virtual ~CCursorStateWindow(void);
	virtual void Draw( POINT ptMouse ){}
	virtual int SetCursorType( int iType );

};
#endif