#ifndef _CJComboBox_
#define _CJComboBox_

#include "winctrl.h"

class CTButton;
class CTImage;
class CJListBox;
class ITFont;
class CTObject;

//------------------------------------------------------------------------------------------------------------------------
///	�޺� �ڽ� Class - CTComboBox�� ������( ��������� ũ�� �ٸ����� ���� );
///
/// @Author				������
///
/// @Date				2005/08/23
//------------------------------------------------------------------------------------------------------------------------

class TGAMECTRL_API CJComboBox : public CWinCtrl
{
public:
	CJComboBox(void);
	~CJComboBox(void);

	virtual		unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual		void Update( POINT ptMouse );
	virtual		void Draw();

	virtual		void Show();
	virtual		void Hide();
	virtual		void SetOffset( POINT pt );
	virtual		void MoveWindow( POINT pt );
	virtual		void OwnerDraw();												/// Ư���� ��Ȳ�� ���� �������� ����ڽ��� �׸��� ���Ͽ� Draw�κ��� ���еǾ��ִ�
																				/// �ٸ� WinCtrl�� �ڿ� �׷����ٸ� ����ڽ��� �ڿ� �������� ��Ȳ


	bool		Create(int iScrX, int iScrY, int iWidth, int iHeight);			
	
	void		SetButton( CTButton* pButton );									/// ����ڽ� ��ư Set
	void		SetImage( CTImage* pTop, CTImage* pMiddle, CTImage* pBottom );	/// ����ڽ��� �׸� �̹���(3�κ�)�� Set
	void        SetListBox( CJListBox* pListBox );								/// �����۵��� ������ JListBox�� Set 

	void		AddItem( CTObject* pItem );										/// ����ڽ��� �������� �߰�
	void		SelectItem( unsigned iSubscript );								/// �ش� �ε����� �������� ����
	unsigned	GetSelectedItemIndex();											/// ���� ���õ� �������� �ε��� ����

	CJListBox*	GetListBoxPtr();												/// ����ڽ����� �������� �����ϴ� ����Ʈ�ڽ��� ������ ����
	const CTObject*	GetSelectedItem();											/// ���� ���õǾ��� �������� ���Ѵ�.

	bool			IsShowDropBox();											/// ���� ����ڽ��� �׷��� �ϴ°�?
	void			DrawDropBoxImage();											/// 3�κ����� �������� Images�� �׸���.
	void			ClearItem();												/// DropBox�� �ִ� ��� Item���� �����.

protected:
	void		ToggleShowDropBox();											/// ����ڽ��� Show/Hide�Ѵ�
	void		DrawSelectedItem();												/// ���õ� �������� ����ڽ� ���� �׸���.

protected:
	ITFont*		m_pFontMgr;														/// TControlMgr���� ���� Client�� ������ ITFont�� Implemented-Class�� ������ �ӽú���
	CTButton*	m_pDropButton;
	CTImage*	m_pDropBoxTopImage;
	CTImage*	m_pDropBoxMiddleImage;
	CTImage*	m_pDropBoxBottomImage;
	CJListBox*	m_pListBox;

};
#endif