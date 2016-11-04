#ifndef _CSeparateDlg_
#define _CSeparateDlg_
#include "tdialog.h"
#include "../../GameCommon/IObserver.h"
#include "subclass/CSlot.h"
#include <vector>
class CIconItem;
class CDragItem;
/**
* �и�/���ؿ� ���Ǵ� ���̾�α�
*	- Observable : CSeparate
* 
* @Author		������
* @Date			2005/9/14
*/
class CSeparateDlg : public CTDialog, public IObserver
{
public:
	CSeparateDlg(void);
	virtual ~CSeparateDlg(void);

	virtual void Update( POINT ptMouse );
	virtual void Show();
	virtual void Draw();
	virtual void MoveWindow( POINT pt );
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

	virtual void Update( CObservable* pObservable, CTObject* pObj );

protected:
	bool	HasEnoughInventoryEmptySlot( CIconItem* pItemIcon, std::vector<CSlot>& OutputItems );

protected:
	enum{
		IID_TEXT_COST = 5,
		IID_BTN_START = 10,
		IID_BTN_CLOSE = 11,
	};

	CSlot					m_MaterialItemSlot;/// �и�/������ �������� �� ����
	std::vector< CSlot >	m_OutputItemSlots; /// �и�/������ �������� �� ���� 

	CDragItem*				m_pDragItem;
};
#endif