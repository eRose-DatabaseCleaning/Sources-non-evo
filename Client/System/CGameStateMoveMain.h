#ifndef _CGAMESTATEMOVEMAIN_
#define _CGAMESTATEMOVEMAIN_

#include "CGameState.h"


/**
* ĳ���� ������ ���ӿ� ������ ī�޶� �̵����� State Class
*	- Next State : CGameStatePrepareMain
*
* @Author		������
* @Date			2005/9/15
*/
class CGameStateMoveMain : public CGameState
{
public:
	CGameStateMoveMain(int iID);
	~CGameStateMoveMain(void);

	virtual int	Update( bool bLostFocus );
	virtual int	Enter( int iPrevStateID );
	virtual int	Leave( int iNextStateID );
	
	///CGame�� Msg Queue�� �ֱ� ���� ��� ó���ؾ��� Msg�� ó���ϴ� Method
	///�� State�� Method�� Overriding�Ͽ� ó���ؾ��� Message�� ó���Ѵ�
	virtual int ProcWndMsgInstant( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

	virtual void ServerDisconnected();

	virtual int ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	virtual int ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );

};
#endif