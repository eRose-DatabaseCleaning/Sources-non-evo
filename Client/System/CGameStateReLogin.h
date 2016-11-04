#ifndef _CGAMESTATERELOGIN_
#define _CGAMESTATERELOGIN_
#include "cgamestate.h"

/**
* CGameStateMain���� CGameStateLogin�� �ٲ�� �߰� State Class
*	- Next State : CGameStateLogin
*
* @Author		������
* @Date			2005/10/20
*/
class CGameStateReLogin :	public CGameState
{
	HANDLE		m_hThread;
public:
	CGameStateReLogin(int iID);
	~CGameStateReLogin(void);

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