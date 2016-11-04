#ifndef _CGAMESTATEPREPARESELECTAVATAR_
#define _CGAMESTATEPREPARESELECTAVATAR_
#include "cgamestate.h"



/**
* CGameStateSelectAvatar�� �ʿ��� ��ó���� �ϴ� State Class
*	- Next State : CGameStateSelectAvatar
* @Author	������	
* @Date		2005/9/15
*/
class CGameStatePrepareSelectAvatar : public CGameState
{
	HANDLE		m_hThread;
public:
	CGameStatePrepareSelectAvatar(int iID);
	~CGameStatePrepareSelectAvatar(void);

	virtual int Update( bool bLostFocus );
	virtual int Enter( int iPrevStateID );
	virtual int Leave( int iNextStateID );

	virtual int ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam ){ return 0; }
	virtual int ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam ){ return 0; }



protected:
	static unsigned __stdcall ThreadFunc( void* pArguments );
	void		Draw();

};
#endif