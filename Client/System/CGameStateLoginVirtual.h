#ifndef _CGameStateLoginVirtual_
#define _CGameStateLoginVirtual_
#include "cgamestate.h"

/**
* Intermediate State Class for moving from CGameStateSelectAvatar to CGameStateSelectSvr
* - Because you need to connect to the login server again, it keeps the ID and password and automatically goes to the server selection window
*
* @Author		√÷¡æ¡¯
* @Date			2005/9/15
*/
class CGameStateLoginVirtual :	public CGameState
{
public:
	CGameStateLoginVirtual( int iID );
	virtual ~CGameStateLoginVirtual(void);
	virtual int	Update(	bool bLostFocus );
	virtual int Enter( int iPrevStateID );
	virtual int Leave( int iNextStateID );
	virtual int ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	virtual int ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	virtual void ServerDisconnected();
	virtual void WorldServerDisconnected();
	virtual void AcceptedConnectLoginSvr();
};
#endif