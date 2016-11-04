#ifndef _CGameStateLoginVirtual_
#define _CGameStateLoginVirtual_
#include "cgamestate.h"

/**
* CGameStateSelectAvatar���� CGameStateSelectSvr�� �̵��ϱ� ���� �߰� State Class
*	- �α��μ����� �ٽ� �����ؾ��ϱ� ������ ���̵�� �н����带 �����Ͽ��ٰ� �ڵ����� ��������â���� ����Ǵ� �����̴�
*
* @Author		������
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