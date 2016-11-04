#ifndef _CGAMESTATECREATEAVATAR_
#define _CGAMESTATECREATEAVATAR_
#include "cgamestate.h"
#include <list>

class CJustModelAVT;

/**
* ĳ���� ���� State Class
*	- Next State  : CGameStateSelectAvatar
*
* @Author		������
* @Date			2005/9/15
*/
class CGameStateCreateAvatar : public CGameState
{
public:
	CGameStateCreateAvatar(int iID);
	~CGameStateCreateAvatar(void);

	virtual int Update( bool bLostFocus );
	virtual int Enter( int iPrevStateID );
	virtual int Leave( int iNextStateID );

	virtual int ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	virtual int ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );

};
#endif