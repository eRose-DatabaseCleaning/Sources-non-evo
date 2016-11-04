#ifndef _CDragNDropMgr_
#define _CDragNDropMgr_

#include "../gamecommon/iobserver.h"

class CObservable;
class CDragItem;
class CTObject;

/**
* �巡��N��� ������
*	: �巡�� ����, �巡�׽� Draw, �巡�����ΰ�?, ���콺��ư ���� ���ó���� �Ѵ�.
*		- �巡��N����� CIcon�� �Ǹ� CSlot, CIconDialog, CMenuDlg���� ���۵ȴ�.
*		- CDragItem�� ����ɼ� �ִ� Target�� ����������� TCommand�� ������ �ִٰ� �ش� Target�� ����Ǿ����� TCommand�� �����Ѵ�.
*		- Target�� Ground, All, �� �������̽� ���̾�α��̴�
*
*
* @Author		������
* @Date			2005/9/5
*
*/

class CDragNDropMgr : public IObserver
{
	CDragNDropMgr(void);
public:
	~CDragNDropMgr(void);

	static CDragNDropMgr&	GetInstance();

	virtual void Update( CObservable* pObservable, CTObject* pObj );	/// �巡������ �������� Data�� ����� ó��( ��ų, �����۵� )

	void Draw( POINT ptMouse );				/// �巡�� ���� CIcon�� �׸���.
	void DragStart( CDragItem* pItem );		/// �巡�� ����
	void DragEnd( int iTargetType );		/// ���
	bool IsDraging();						/// �巡�� ���ΰ�?

private:
	CDragItem*	m_pItem;					/// �巡�� ���� CIcon �� ������ �ӽ� ����
};
#endif