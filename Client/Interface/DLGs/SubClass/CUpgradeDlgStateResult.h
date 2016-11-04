#ifndef _CUpgradeDlgStateResult_
#define _CUpgradeDlgStateResult_

#include "cupgradedlgstate.h"

class CGuage;
class CUpgradeDlg;

/**
* �����������̽� â�� State Class : �����κ��� ���ÿ�û�� ���� ������ �ް� ����ϴ� ����
*
* @Author		������
* @Date			2005/9/15
*/
class CUpgradeDlgStateResult : public CUpgradeDlgState
{
public:
	CUpgradeDlgStateResult( CUpgradeDlg* pParent );
	virtual ~CUpgradeDlgStateResult(void);

	virtual void Enter();
	virtual void Leave();
	virtual void Update( POINT ptMouse );
	virtual void Draw();
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam ){ return 1; }
	virtual void MoveWindow( POINT ptMouse );
private:
	CUpgradeDlg*	m_pParent;
	CGuage*			m_pResultGuage;
	DWORD			m_dwPrevTime;

	int				m_iRedGuageImageID;
	int				m_iGreenGuageImageID;
	int				m_iVirtualSuccessPoint;

	bool			m_bWaitUserinput;

};
#endif