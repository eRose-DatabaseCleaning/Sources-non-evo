#ifndef _CGAMESTATETITLE_
#define _CGAMESTATETITLE_
#include "cgamestate.h"

/**
* Ŭ���̾�Ʈ ������ �α��� ���� ����, Ÿ��Ʋ�� �����ָ� ����Ÿ�� �ε��ϴ� State Class
*
* @Author	������
* @Date		2005/9/15
*/
class CGameStateTitle :	public CGameState
{
	HANDLE		m_hThread;
public:
	CGameStateTitle(int iID);
	~CGameStateTitle(void);

	virtual int Update( bool bLostFocus );
	virtual int Enter( int iPrevStateID );
	virtual int Leave( int iNextStateID );

	virtual void ServerDisconnected(){}

	virtual int ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam ){return 0;}
	virtual int ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam ){ return 0;}

	
protected:
	static unsigned __stdcall ThreadFunc( void* pArguments );
	void Draw();

	///
	/// system ��濡 ���� ����ȣ
	///
	static int	m_iBackGroundZone;
	HNODE		m_hTitleTexture;
};


#endif