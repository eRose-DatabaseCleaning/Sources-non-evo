#ifndef _CGAMESTATE_
#define _CGAMESTATE_



/**
* Ŭ���̾�Ʈ�� ����Ŭ������ ���̽�Ŭ����
*	- ó�� ����Ÿ �ε��� ȭ�� ����� thread�� �����Ϸ��� �õ��Ͽ� ��Ƽ ������ �ڵ尡 �ּ�ó���Ǿ� �ִ�( ����Ÿ �ε��� DirectX�� ������ ���� )
*
* @Author		������
* @Date			2005/9/15
*/
class CGameState
{
public:
	CGameState(void);
	virtual ~CGameState(void);

	virtual int	Update(	bool bLostFocus ) = 0;
	virtual int Enter( int iPrevStateID ) = 0;
	virtual int Leave( int iNextStateID ) = 0;
	
	///CGame�� Msg Queue�� �ֱ� ���� ��� ó���ؾ��� Msg�� ó���ϴ� Method
	///�� State�� Method�� Overriding�Ͽ� ó���ؾ��� Message�� ó���Ѵ�
	virtual int ProcWndMsgInstant( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

	virtual void ServerDisconnected();
	virtual void WorldServerDisconnected(){}
	virtual void AcceptedConnectLoginSvr(){}

	int GetStateID(){ return m_iStateID; }

	virtual int ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam ) = 0;
	virtual int ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam ) = 0;

protected:
	int	m_iStateID;
};
#endif