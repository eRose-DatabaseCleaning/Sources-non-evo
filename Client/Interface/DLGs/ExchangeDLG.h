#ifndef _EXCHANGEDLG_
#define _EXCHANGEDLG_

#include "../../../TGameCtrl/TDialog.h"
#include "../../GameCommon/IObserver.h"
#include "../../GameData/CExchange.h"
#include "subclass/cslot.h"


class CObservable;
class CTObject;
class CTCmdNumberInput;
class CTCommand;
class CDragItem;

/**
* �ٸ� ������ 1:1�ŷ��� ���Ǵ� ���̾�α�
*	- ��⸦ ���� ���� �Ѵ� �غ��ư�� �������� Ȯ�� ��ư�� Ȱ��ȭ �Ǵ� 2�� Ȯ���� �Ѵ�.
*	- Observable : CExchange
*
* @Author		������
* @Date			2005/9/14
*/

class CExchangeDLG : public CTDialog, public IObserver
{
public:
	CExchangeDLG( int iType );
	virtual ~CExchangeDLG(void);

	virtual void Draw();
	virtual void Show();
	virtual unsigned int Process( UINT uiMsg, WPARAM wParam, LPARAM lParam );

	virtual void Hide();
	virtual int  IsInValidShow();	
	virtual void MoveWindow( POINT pt );
	virtual void Update( POINT ptMouse );


	virtual void Update( CObservable* pObservable, CTObject* pObj );
	


	enum{
		IID_BTN_CLOSE	 = 10,
		IID_BTN_EXCHANGE = 11,
		IID_BTN_OK		 = 12,
		IID_BTN_OTHER_CONFIRM = 13,
		IID_BTN_MONEY	 = 14
	};




protected:
	void	OnLButtonDown( unsigned iProcID, WPARAM wParam, LPARAM lParam );
	void	OnLButtonUp( unsigned iProcID, WPARAM wParam, LPARAM lParam );

	void	InitExchangeState();
	void	SetOtherReadyState(bool b );
	void	SetMeReadyState( bool b );

protected:
	CSlot					m_MySlots[TOTAL_EXCHANGE_INVENTORY];
	CSlot					m_OtherSlots[TOTAL_EXCHANGE_INVENTORY];

	CTCommand*				m_pCmdRemoveMyItemFromExchange;
	CTCmdNumberInput*		m_pCmdRemoveMyMoneyFromExchange;
	CDragItem*				m_pDragItem;
	CDragItem*				m_pOtherItemDragItem;

};


#endif // _EXCHANGEDLG_	