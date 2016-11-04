#ifndef _CCursorStateDx_
#define _CCursorStateDx_
#include "ccursorstate.h"
#include "CCursor.h"

/**
* ������ Ŀ���� ���ӻ�Ȳ�� ���� �����Ͽ� ����Ѵ�( ����Ʈ )
*  - ������ Znzin���� ó���ϰԲ� �����Ǿ��ٰ� �������� �ذ�ȵǾ� ����� �ٲ� -> ReloadCursor()�� �ذ�
*
* @Author		������
*
* @Date			2005/9/6
*/


class CCursorStateDx : public CCursorState
{
public:
	CCursorStateDx(void);
	virtual ~CCursorStateDx(void);
	virtual void Draw( POINT ptMouse ){}
	virtual int SetCursorType( int iType );
	virtual void ReloadCursor();
private:
	void LoadCursor();
	void UnloadCursor();
	HCURSOR			m_hCursor[ CCursor::CURSOR_MAX ];
	HNODE			m_hDxCursor[ CCursor::CURSOR_MAX ];

};
#endif