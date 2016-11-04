#ifndef _CJListBox_
#define _CJListBox_

#include "winctrl.h"
#include "IScrollModel.h"

class IListCellRenderer;
class CTContainer;

//------------------------------------------------------------------------------------------------------------------------
///	����Ʈ �ڽ� class
///
/// @Author	������
///
/// @Date	2005/08/30
//------------------------------------------------------------------------------------------------------------------------
class TGAMECTRL_API CJListBox : public CWinCtrl, public IScrollModel
{

public:
	CJListBox(void);
	~CJListBox(void);

	bool	Create( int iScrX, int iScrY, int iWidth, int iHeight , int iItemHeight );

	virtual void Draw();
	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	///Renderer
	void	SetCellRenderer( IListCellRenderer* pRenderer );									/// Item�� Draw�� ���ǵ� Class�� Set
	void	SetContainer( CTContainer* pContainer );											/// Item�� ������ Container�� Set
	
	///Container
	void		Clear();
	void		AddItem( CTObject* pObject );

	/// IScrollModel
	virtual int GetValue();
	virtual int GetExtent();
	virtual int GetMaximum();
	virtual int GetMinimum();
	
	virtual void SetValue( int );
	virtual void SetExtent( int );
	virtual void SetMaximum( int );
	virtual void SetMinimum( int );

	virtual RECT GetWindowRect();


	///operation
	const CTObject*	GetSelectedItem();																/// ���õ� Item�� �����Ѵ�.
	unsigned		GetSelectedItemIndex();															/// ���õ� Item�� �ε����� �����Ѵ�.
	unsigned		GetSize();																		

	CWinCtrl*		GetRendererComponent( const CTObject* pObject, bool bSelect, bool bMouseOver ); /// Item�� Draw�� ���ǵ� Class�� ����
	void			SelectItem( unsigned subscript );												/// �ش� Item�� �����Ѵ�.


	void			SetCellHeight( int iHeight );													/// Draw�� Item�� Space�� Set
private:
	IListCellRenderer*		m_pListCellRenderer;													/// Item�� Draw�� ���ǵ� Class
	CTContainer*			m_pContainer;															/// Item�� ������ Container


	int						m_iCellHeight;															/// Draw�� Item�� Space
	int						m_iExtent;																/// �ѹ��� ������ Item�� ��
	int						m_iValue;																/// ���� ȭ�鿡�� ���� ���� ������ Item�� Index

};
#endif