#ifndef _CBANKDLG_
#define _CBANKDLG_

#include "../../../TGameCtrl/TDialog.h"
#include "../../Common/CItem.h"
#include "../../gamedata/cbank.h"
#include "../../gamecommon/iobserver.h"
#include "subclass/CSlot.h"

class CObservable;
class CDragItem;
/**
* ���࿡ ���Ǵ� ���̾�α�( �������̽� )
*	- Observable : CBank( ���� Data Class )
*
*
* @Author		������
* @Date			2005/9/12
*/
class CBankDlg : public CTDialog, public IObserver
{
public:
	CBankDlg( int iType );
	virtual ~CBankDlg(void);

	virtual void			Show();
	virtual void			Draw();
	virtual void			Update( POINT ptMouse );
	virtual unsigned int	Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);
	virtual bool			Create( const char* szIDD );
	virtual void			MoveWindow( POINT pt );


	virtual void			Update( CObservable* pObservable, CTObject* pObj );

	int						GetCurrentTab();										
	bool					IsFullFirstTab();									/// ���� ù��° ���� �� �� �ִ°�?

	enum{
		IID_BTN_CLOSE		= 20,
		IID_BTN_SAVE		= 21, ///�ٸ� ���� ��ư
		IID_BTN_WITHDRAW	= 22, ///�ٸ� ã�� ��ư
		IID_RADIOBOX		= 30,
		IID_BTN_TAB1		= 31,
		IID_BTN_TAB2		= 32,
		IID_BTN_TAB3		= 33,
		IID_BTN_TAB4		= 34,
	};


private:
	void OnLButtonUp( unsigned iProcID, WPARAM wParam, LPARAM lParam );
	void OnLButtonDown( unsigned iProcID, WPARAM wParam, LPARAM lParam );
	void SetTabButtonText( int iID, char* szText );

private:
	int			m_iCurrBankPage;												/// ���� ��

	CSlot		m_Slots[g_iPageCount][g_iSlotCountPerPage];
	CDragItem*	m_pDragItem;
};
#endif