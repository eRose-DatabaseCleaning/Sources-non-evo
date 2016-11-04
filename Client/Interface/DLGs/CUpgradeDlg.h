#ifndef _CUpgradeDlg_
#define _CUpgradeDlg_
#include "tdialog.h"
#include "subclass/CSlot.h"
#include "../../GameCommon/IObserver.h"

class CDragItem;
class CUpgradeDlgState;
class CUpgradeDlgStateNormal;
class CUpgradeDlgStateWait;
class CUpgradeDlgStateResult;

/**
* ���ÿ� ���Ǵ� ���̾�α�
*	- Observable : CUpgrade
* 
* @Author		������
* @Date			2005/9/14
*/
class CUpgradeDlg :	public CTDialog, public IObserver
{
	friend CUpgradeDlgStateNormal;
	friend CUpgradeDlgStateWait;
	friend CUpgradeDlgStateResult;
public:
	CUpgradeDlg( int iType );
	virtual ~CUpgradeDlg(void);
 
	virtual void Update( POINT ptMouse );
	virtual void Show();
	virtual void Hide();
	virtual void Draw();
	virtual void MoveWindow( POINT pt );
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

	virtual void Update( CObservable* pObservable, CTObject* pObj );

	void	ChangeState( int iID );


private:

	enum{
		IID_TEXT_COST = 5
	};

	enum STATE_TYPE{
		STATE_NORMAL,
		STATE_WAIT,
		STATE_RESULT,
		STATE_MAX
	};

	CUpgradeDlgState*			m_pCurrState;
	CUpgradeDlgState*			m_pStates[STATE_MAX];

	CDragItem*					m_pDragItemTarget;
	CDragItem*					m_pDragItemMaterial;
	
	CSlot						m_TargetItemSlot;
	CSlot						m_MaterialSlots[3];
};
#endif