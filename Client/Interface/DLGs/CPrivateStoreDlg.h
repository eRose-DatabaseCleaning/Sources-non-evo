#ifndef _CPrivateStoreDlg_
#define _CPrivateStoreDlg_
#include "tdialog.h"
#include "../../GameCommon/IObserver.h"
#include "../../GameData/CPrivateStore.h"
#include "subclass/CSlot.h"

const	int c_iMaxSlotCount = 30;
class CDragItem;

/**
* �ڽ��� ������ ���λ����� ���� Dialog
*	- Observable : CPrivateStore
*
* @Author		������
* @Date			2005/9/12
*/
class CPrivateStoreDlg : public CTDialog, public IObserver
{
public:
	CPrivateStoreDlg( int iDlgType );
	virtual ~CPrivateStoreDlg(void);

	virtual void		Show();
	virtual void		Hide();
	virtual void		Draw();
	virtual void		Update( POINT ptMouse );
	virtual void		MoveWindow( POINT pt );
	virtual unsigned	Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

	virtual void		Update( CObservable* pObservable, CTObject* pObj );



	void SortItemSellList();

	int		GetTabType();
	enum{
		TAB_SELL,
		TAB_BUY,
	};


private:
	void	OnLButtonDown( unsigned uiProcID, WPARAM wParam, LPARAM lParam );
	void	OnLButtonUp( unsigned uiProcID, WPARAM wParam, LPARAM lParam );


private:
	enum{
		IID_EDITBOX		= 19,
		IID_BTN_CLOSE	= 20,
		IID_BTN_START	= 21,
		IID_BTN_STOP	= 22,	
		IID_TEXT_CLOSED	= 23,
		IID_TEXT_OPENED	= 24,
		IID_BTN_ADD_BUYLIST = 25,
		IID_RADIOBOX	= 30,
		IID_BTN_SELL	= 31,
		IID_BTN_BUY		= 32,
	};

	int m_iTab;

	CSlot						m_SellSlots[c_iMaxSlotCount];					/// ���� ��� �������� �������� Attach �� ���Ե�
	CSlotBuyPrivateStore		m_BuySlots[c_iMaxSlotCount];					/// �Ǹ� ��� �������� �������� Attach �� ���Ե�

	CDragItem*	m_pSellDragItem;
	CDragItem*	m_pBuyDragItem;
};
#endif