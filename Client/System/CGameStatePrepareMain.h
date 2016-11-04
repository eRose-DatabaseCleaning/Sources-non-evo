#ifndef _CGAMESTATEPREPAREMAIN_
#define _CGAMESTATEPREPAREMAIN_

#include "cgamestate.h"

/**
* ĳ���� ������ CGameStateMain�� ó���� �ϴ� State Class
*	- CGameStateWarp�� ��������� �ٸ� �κ��� �־� �и��Ǿ���.
*
* @Author		������
* @Date			2005/9/15
*/
class CGameStatePrepareMain : public CGameState
{
	int m_iPrevStateID;
	HANDLE m_hThread;
public:
	CGameStatePrepareMain(int iID);
	~CGameStatePrepareMain(void);
	virtual int Update( bool bLostFocus );
	virtual int Enter( int iPrevStateID );
	virtual int Leave( int iNextStateID );
	virtual int ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam ) { return 0;}
	virtual int ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam ) { return 0;}


protected:
	static unsigned __stdcall ThreadFunc( void* pArguments );
	void Draw();

};
#endif
