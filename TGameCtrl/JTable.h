#ifndef _CJTable_
#define _CJTable_

#include <vector>
#include "winctrl.h"
#include "IScrollModel.h"

//--------------------------------------------------------------------------------------------------------------------------------------
///	���̺� Class
///	- children�� ���δ� �÷� ������ ������ ó���Ѵ�.
///
/// @Author				������
/// @Date				2005/08/30
//--------------------------------------------------------------------------------------------------------------------------------------
class TGAMECTRL_API CJTable :	public CWinCtrl, public IScrollModel
{
public:
	CJTable(void);
	virtual ~CJTable(void);

	/// CWinCtrl
	virtual void Draw();
	virtual void Update( POINT ptMouse );
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual void MoveWindow( POINT pt );

	/// IScrollModel
	virtual int GetValue();
	virtual int GetExtent();
	virtual int GetMaximum();
	virtual int GetMinimum();
	
	virtual void SetValue( int iValue );
	virtual void SetExtent( int iExtent );
	virtual void SetMaximum( int iMaximum );
	virtual void SetMinimum( int iMinimum );

	virtual RECT GetWindowRect();


	void	Add( CWinCtrl* pCtrl );
	void	SetColumnCount( int iCount );										/// ���δ� Cell ���� Set
	void	SetCellWidth( int iWidth );											/// Cell Width Set
	void	SetCellHeight( int iHeight );										/// Cell Height Set
	void	SetRowMargin( int iMargin );										/// ���ΰ� Space Set
	void	SetColMargin( int iMargin );										/// Cell�� Space Set
	CWinCtrl*	GetItem( int iIndex );											
	int		GetSelectedItemID();			
protected:
	int		m_iValue;															/// ȭ�鿡�� ������ �׷��� Row Index
	int		m_iExtent;															/// �� ȭ�鿡 �׷��� Rows
	int		m_iColumnCount;														/// ���δ� Cell ����
	int		m_iCellWidth;														/// Cell Width
	int		m_iCellHeight;														/// Cell Height
	int		m_iRowMargin;														/// Row�� Space
	int		m_iColMargin;														/// Cell�� Space

	int		m_iSelectedItemID;													/// ���õǾ��� Item�� ID

	std::vector< CWinCtrl* > m_Items;
	std::vector< CWinCtrl* > m_ViewItems;
};
#endif