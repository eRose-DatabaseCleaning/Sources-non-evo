#ifndef _CIconItem_
#define _CIconItem_
#include "cicon.h"

struct tagITEM;
class CItem;


/**
* Item Icon Class
*	- CTRL + MOUSE LBUTTONDOWN ��  ���ø���Ʈ�� �߰��ϴ� ��� ~ CIconItem::Process
*
*
* @Warning		�����̹��� Texture�� load�� unload�� ���۷��� ī�������� ó���Ѵ�.
* @Author		������
* @Date			2005/9/6
*/
class CIconItem : public CIcon
{
public:
	CIconItem(void);
	CIconItem( tagITEM* pItemData );
	CIconItem( CItem* pItem );
	virtual ~CIconItem(void);

	virtual void		Draw();
	virtual void		Update( POINT ptMouse );
	virtual void		ExecuteCommand();
	virtual void		GetToolTip( CInfo& strToolTip,  DWORD dwDialogType, DWORD dwType );
	virtual unsigned	Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

	virtual CIcon*		Clone();
	virtual void		Clear();
	
	virtual int			GetQuantity();
	virtual tagITEM&	GetItem();
	virtual bool		IsEnable();
	virtual const char*	GetName();
	virtual int			GetIndex();


	CItem*				GetCItem();
	int					GetItemNo();

	bool				IsEmpty();



protected:
	CItem*				m_pItem;							/// �������� ������ ���������ۿ� ���� ������ < CItem�� �ִ� ��� : ��) �κ��丮�� ������ >
	bool				m_bDeleteItemData;					/// ���� �������� ������� ����� �����۵���Ÿ
	static	HNODE		m_hSocketTexture;					/// ������ ���� �������� ��� ���� ǥ���ϱ� ���� Texture�� ���
	static  int			m_iSocketTextureRefCount;			/// ���۷��� ī������ ���ؼ� 0�ϰ�� �����Ѵ�.
};

#endif