#ifndef _CGAMESTATESELECTAVATAR_
#define _CGAMESTATESELECTAVATAR_
#include "cgamestate.h"

/**
* ĳ���� ���� State Class
*	- Next State : CGameStateCreateAvatar, CGameStateMoveMain, CGameStateLoginVirtual
*
* @Author	������	
* @Date		2005/9/15
*/
class CGameStateSelectAvatar : public CGameState
{
public:
	CGameStateSelectAvatar(int iID);
	~CGameStateSelectAvatar(void);
	
	virtual int Update( bool bLostFocus );
	virtual int Enter( int iPrevStateID );
	virtual int Leave( int iNextStateID );
	
	virtual void ServerDisconnected();

	virtual int ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	virtual int ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );

};
#endif