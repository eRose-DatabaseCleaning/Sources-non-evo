#ifndef	_TLISTBOX_
#define _TLISTBOX_
#include "winctrl.h"
#include "IScrollModel.h"
#include <deque>

using namespace std;

typedef struct {
	char		m_szTxt[MAX_PATH];
	D3DCOLOR	m_dwColor;
	bool		m_bDrawn;
}t_list_item;


typedef deque<t_list_item>						li_item_vec;
typedef deque<t_list_item>::iterator			li_item_vec_itor;
typedef deque<t_list_item>::const_iterator		li_item_vec_const_itor;

class CTScrollBar;
class ITFont;

/**
* ListBox Class : t_list_item�� item���� ������ �ִ� ListBox
*	- ScrollBar�� ���ϼ� �ֵ��� IScrollModel�� Implement�Ѵ�.
*
* @Author	������
*
* @Date		2003/11/26
*/
class TGAMECTRL_API CTListBox :	public CWinCtrl, public IScrollModel
{
public:
	CTListBox(void);
	virtual ~CTListBox(void);

	bool					Create( int iScrX, int iScrY, int iWidth, int iHeight ,int iExtent,int iCharWidth, int iCharHeight );
	virtual unsigned int	Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual	void			Update( POINT ptMouse );
	virtual void			Draw();

	/// IScrollModel ������ 
	virtual int GetValue();
	virtual int GetExtent();
	virtual int GetMaximum();
	virtual int GetMinimum();
	
	virtual void SetValue( int i );
	virtual void SetExtent( int i );
	virtual void SetMaximum( int i );
	virtual void SetMinimum( int i );

	virtual RECT GetWindowRect();
	virtual bool IsLastItemDrawn();
	///�ؽ�Ʈ �߰� ����
	void	AppendText(const char* szTxt,D3DCOLOR dwColor,bool bAutoIncValue = true );
	void	SetText( int index, const char* text, D3DCOLOR dwColor );
	void	DeleteText(int iLineNo);
	const char*	GetText(int iLineNo);
	t_list_item	GetItem( int iLineNo );
	void    ClearText();///��� Item���� �����.
	///
	const   li_item_vec&	GetList(){ return m_ITM; }

	///������ ����
	bool	IsSelectable(){ return m_bSelectable; }
	void	SetSelectable( bool b ){ m_bSelectable = b; }
	short   GetSelectedItemID(){ return m_iSelectedItem; }
	const char* GetSelectedItem();
	void    SetSelectedItem( short i){ m_iSelectedItem = i; }
	///���ν����̽� ( �ణ )
	void    SetLineSpace( short i ){ m_iLineSpace = i; }
	short   GetLineSpace(){ return m_iLineSpace; }
	///
	void	SetMaxSize( int i ){ m_iMaxSize = i; }
	int		GetMaxSize(){ return m_iMaxSize; }
	///��Ʈ ũ��
	void    SetCharHeight( short i ){ m_iCharHeight = i; }
	short   GetCharHeight(){ return m_iCharHeight; }

	void    SetCharWidth( short i );
	short   GetCharWidth(){ return m_iCharWidth; }

	void	SetFont( int iFont );

protected:
	///����Ʈ�� ���ð����� Ÿ���ϰ�� �޼��� ó���ϴ� method
	bool	ProcessSelectable(UINT uiMsg,WPARAM wParam,LPARAM lParam);

protected:
	li_item_vec			m_ITM;			///����Ʈ������ 
	short				m_nPutLnNum;	///���� ���� ù��° ���ι�ȣ 
	short				m_nMaxLnCnt;	///�ִ� ������ �ִ� ���μ� 
	short				m_nMaxPutChar;	///�Ѷ��ο��������ִ� �ִ빮�ڼ� 
	short				m_iSelectedItem;///���õ� ������
	bool				m_bSelectable;  ///�������� ���ð����Ѱ�?
	short				m_iLineSpace;   ///�ణ
	short				m_iCharHeight;  ///��Ʈ ���� 
	short				m_iCharWidth;   ///��Ʈ ����
	int					m_iMaxSize;		///����Ʈ�� �߰��ɼ� �ִ� �������� �ִ� ����
	ITFont*				m_pFontMgr;
	int					m_iFont;

};
#endif ///_TLISTBOX_