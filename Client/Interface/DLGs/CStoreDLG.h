#ifndef _STOREDLG_
#define _STOREDLG_

#include "../../../TGameCtrl/TDialog.h"
#include "../../Common/CItem.h"
#include "../../gamecommon/IObserver.h"
#include "subclass/cslot.h"
#include "../../GameData/CStore.h"
//enum StoreDLG 
//{
//	STORE_IMG,								//0,�ǳ�
//	STORE_BTN_CLOSE = 3,					//1,�ݱ� 
//	STORE_RADIOBOX  = 4,
//	STORE_BTN_TAB1  = 5,					//2,��1
//	STORE_BTN_TAB2  = 6,					//3,��2
//	STORE_BTN_TAB3  = 7,					//4,��3
//	STORE_BTN_TAB4  = 8,
//	SROTE_MAX_BTN_CNT
//};


class CTCmdNumberInput;
class CTCmdOpenNumberInputDlg;

/**
* NPC������ ���̾�α�
*	- Observable : CStore
*
* @Author		������
* @Date			2005/9/14
**/
class CStoreDLG : public CTDialog, public IObserver
{
private:
	CSlot						m_Slots[MAX_INV_TYPE][c_iSlotCountPerTab];	/// �Ǹ� �������� Attach�� Slot��
	short						m_nInvType;									///< Item type[ wasting item, Equipment item, .. ]	

	CTCmdNumberInput*			m_pCmdNumberInput;
	CTCmdOpenNumberInputDlg*	m_pCmdOpenNumberInput;

	CDragItem*					m_pDragItem;

public:
	CStoreDLG( int iType );
	virtual ~CStoreDLG();

	virtual void Draw();
	virtual void Update( POINT ptMouse );
	virtual void Show();
	virtual void MoveWindow( POINT pt );
	virtual int	 IsInValidShow();
	
	virtual unsigned int Process( UINT uiMsg, WPARAM wParam, LPARAM lParam );

	virtual void Update( CObservable* pObservable, CTObject* pObj );

	void SetTab(short nInvType)			{ m_nInvType = nInvType; }

protected:
	enum{
		IID_BTN_CLOSE	= 20,
		IID_RADIOBOX	= 30,
		IID_BTN_TAB1	= 31,
		IID_BTN_TAB2	= 32,
		IID_BTN_TAB3	= 33,
		IID_BTN_TAB4	= 34
	};

	bool On_LButtonUP( unsigned iProcID, WPARAM wParam, LPARAM lParam );

	void SetEnableTabButton( int iInvType, bool bEnable );					
	void SetTabButtonText( short iIndex, const char* szText );
	void UpdateSlotPosition( int iType );

};


#endif //_STOREDLG_