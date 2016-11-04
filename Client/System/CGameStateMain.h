#ifndef _CGAMESTATEMAIN_
#define _CGAMESTATEMAIN_
#include "cgamestate.h"

/**
* ĳ���� ������ ���� �������� ����
*
* @Author		������
* @Date			2005/9/15
*/
class CGameStateMain : public CGameState
{
public:
	CGameStateMain(int iID);
	~CGameStateMain(void);

	virtual int Update( bool bLostFocus );
	virtual int Enter( int iPrevStateID );
	virtual int Leave( int iNextStateID );
	virtual void ServerDisconnected();
	virtual int ProcWndMsgInstant( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual int ProcMouseInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	virtual int ProcKeyboardInput( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	bool RightClickedOnChar;

protected:

	bool On_WM_CHAR (WPARAM wParam, LPARAM lParam);
	bool On_WM_KEYDOWN (WPARAM wParam, LPARAM lParam);
	bool On_WM_LBUTTONDOWN (WPARAM wParam, LPARAM lParam);
	bool On_WM_RBUTTONDOWN (WPARAM wParam, LPARAM lParam);
	bool On_WM_LBUTTONDBCLICK (WPARAM wParam, LPARAM lParam);	
	bool On_WM_MOUSEWHEEL (WPARAM wParam, LPARAM lParam);

	void UpdateCheckFrame();
	void Render_GameMENU();
	/// ���� ���콺�� � ��ġ�� �ִ°�?
	int	CheckMouseRegion( int x, int y );

	/// �������� ���� �ؾߵǴ� �͵�
	void UpdateCameraPositionByMouse();///���콺�� ���¹� ��ġ�� ī�޶� �̵�ó��
	void Pick_POSITION(/*LPARAM	lParam*/);

private:
	DWORD		m_dwEnterTime;

	tPOINT16	m_PosRButtonClick;
	D3DXVECTOR3	m_PosPICK;
	tPOINT16	m_ScreenPOS;
	bool		m_bPickedPOS;
	int			m_iPickedOBJ;
};
#endif