#ifndef _CGAMESTATEEXITMAIN_
#define _CGAMESTATEEXITMAIN_
#include "cgamestate.h"

/**
* CGameStateMain���� CGameStateSelectAvatar�� �ٲ�� �߰� State Class
*	- Next State : CGameStateSelectAvatar
*
* @Author		������
* @Date			2005/9/15
*/
class CGameStateExitMain :	public CGameState
{
	HANDLE		m_hThread;
public:
	CGameStateExitMain(int iID);
	~CGameStateExitMain(void);

	virtual int Update( bool bLostFocus );
	virtual int Enter( int iPrevStateID );
	virtual int Leave( int iNextStateID );

	virtual void ServerDisconnected(){}////�� ���¿����� ������ ���ܵ� �ƹ��͵� ���� �ʴ´�.

	virtual int ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam ){return 0;}
	virtual int ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam ){ return 0;}

	
protected:
	static unsigned __stdcall ThreadFunc( void* pArguments );
	void Draw();

	///
	/// system ��濡 ���� ����ȣ
	///
	static int	m_iBackGroundZone;
};


#endif