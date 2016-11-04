#ifndef _CGAMESTATELOGIN_
#define _CGAMESTATELOGIN_
#include "cgamestate.h"


/**
* �α��� State Class
*	- Next State : CGameStateSelectSvr
*
* @Author		������
* @Date			2005/9/15
*/
class CGameStateLogin :	public CGameState
{
private:


public:
	CGameStateLogin(int iID);
	~CGameStateLogin(void);

	int Update( bool bLostFocus );
	int Enter( int iPrevStateID );
	int Leave( int iNextStateID );
	virtual void ServerDisconnected();
	virtual void AcceptedConnectLoginSvr();
	virtual int ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	virtual int ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );


};
#endif