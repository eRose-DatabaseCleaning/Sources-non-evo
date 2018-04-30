#ifndef _CWINCTRL_
#define _CWINCTRL_


#include "itcontrol.h"
#include "itdraw.h"
#include "ActionListenerList.h"
#include <string>
#include "SinglelineString.h"

#define STATUS_VISION			0x0001
#define STATUS_FOCUS			0x0002
#define STATUS_ACTIVE			0x0004
#define STATUS_ENABLE			0x0008
#define STATUS_ALL				STATUS_VISION | STATUS_FOCUS | STATUS_ACTIVE | STATUS_ENABLE

#define CTRL_IMAGE				0x0100
#define CTRL_BUTTON				0x0200
#define CTRL_EDITBOX			0x0300
#define CTRL_LISTBOX			0x0400
#define CTRL_SCROLLBAR			0x0500
#define	CTRL_COMBOBOX			0x0600
#define CTRL_DIALOG				0x0700
#define CTRL_STATIC				0x0800
#define CTRL_SCROLLBOX			0x0900
#define CTRL_CAPTION			0x0A00
#define CTRL_RADIOBOX			0x0B00
#define CTRL_RADIOBUTTON		0x0C00
#define CTRL_MSGBOX				0x0D00
#define CTRL_PUSHBUTTON			0x0E00
#define CTRL_TREE				0x0F00
#define CTRL_GUAGE				0x1000
#define CTRL_JCOMBOBOX			0x1100
#define CTRL_JLISTBOX			0x1200
#define CTRL_TABBEDPANE			0x1300
#define CTRL_STATUSBAR			0x1400
#define CTRL_ZLISTBOX			0x1500
#define CTRL_CHECKBOX			0x1600
#define CTRL_PANE				0x1700
#define CTRL_SLOT				0x1800
#define CTRL_TABLE				0x1900

class  CWinCtrl;
class  CTDialog;

class IActionListener;

typedef list< CWinCtrl* >				WINCTRL_LIST;	
typedef WINCTRL_LIST::iterator			WINCTRL_LIST_ITOR;
typedef WINCTRL_LIST::reverse_iterator  WINCTRL_LIST_RITOR;


typedef map< unsigned int,CWinCtrl* >	WINCTRL_MAP;	
typedef WINCTRL_MAP::iterator			WINCTRL_MAP_ITOR;



/**
*�������̽����� ���Ǵ� Control���� �⺻�� �Ǵ� Class
*
* @Author	������
* @Date		2005/8/30
*/

class TGAMECTRL_API CWinCtrl : public CTObject, public ITControl, public CSinglelineString
{

protected:

	DWORD			m_dwCurrentTime;
	
	CWinCtrl*		m_pParent;			/// �ڽ��� �����ϰ� �մ� Parent�� ������( Parent�� �������� ��� ������ �ִ� )
	CWinCtrl*		m_pChild;			/// �ڽ�

	DWORD			m_dwStatus;			/// ���º��� 
	DWORD			m_dwCtrlType;		///
	POINT			m_sPosition;		///
	POINT			m_ptOffset;			/// ��Ʈ���� Child(Dialog��)�ϰ�� Parent�� Left-Top��ǥ���� �Ÿ�
	int				m_iWidth;
	int				m_iHeight;
	unsigned int	m_iControlID;		/// ���ͷ� ���̵�
	std::string		m_strControlName;	/// ��Ʈ�� �̸�.

	int				m_iParentID;
	ITDraw*			m_pDraw;
	bool			m_bSelected;
	

	static CWinCtrl*		m_pMouseExclusiveCtrl;      /// ���콺�� �����ϰ� �ִ� ��Ʈ��
	static CWinCtrl*		m_pProcessMouseOverCtrl;	// �� �����ӿ��� ���콺�� �ڽ��� �ٷ� ���� �ִ� WinCtrl�� ������ : �������� ó���� Clear�Ǿ�� �Ѵ�.
	static bool				m_bProcessMouseOver;

	bool					m_bOwnerDraw;///Client���� ������ �ڵ�� �׷����� �ɰ�� true ( Default: false )

	float					m_fScaleWidth;
	float					m_fScaleHeight;
	BYTE					m_btAlphaValue;
	CActionListenerList		m_ActionListenerList;
	bool			m_bSizeFit;							/// draw���� ������ with, height�� Fit.

public:
	CWinCtrl(void);
	virtual ~CWinCtrl(void);

	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual	void Update( POINT ptMouse ){};
	
	virtual	void Draw();
	virtual void Draw( POINT ptDraw );
	virtual void Draw_Font();

	virtual	bool	IsInside( int x, int y );
	virtual	bool	IsInside( LPARAM lParam );
	
	virtual void	SetText(const char * szText);
	virtual void	SetTextColor(DWORD dwColor);
	virtual void	SetAlign(DWORD dwAlign);
	virtual void	SetFont(int iFont);

	/// �ʱ�ȭ 
	virtual void	Init( DWORD dwType,int iScrX, int iScrY, int iWidth, int iHeight );	//�ʱ�ȭ�Լ� 

	virtual CWinCtrl*	Find( int iID );
	virtual CWinCtrl*	Find( const char * szName );

	///m_bOwnerDraw == true�϶� �׸��� �ʴ� Draw ��ƾ , 
	///Layer������ �߰�, ListBox�� ComboBox���� ���
	///Client�� ������ �ñ⿡ Draw�ϴ� ��ƾ
	virtual void	OwnerDraw(){};
	virtual void	SetSelected();
	virtual void	SetDeselected();
	bool			IsSelected();

	///���콺 ���� ���� operation
	static	bool		IsExclusive();
	static	void		SetMouseExclusiveCtrl( CWinCtrl* pCtrl );
	static  CWinCtrl*	GetMouseExclusiveCtrl();
	
	/// ���콺 ���� ���� ó�� operation : Client���� ��ü Interface�� Update�ϱ����� false�� Setting�ϰ�
	/// ó���� Ctrl���� �̸� true�� Set�Ͽ� �ٸ� Ctrl�� ó������ �ʰԲ� �Ѵ�.
	static  bool		IsProcessMouseOver(){ return m_bProcessMouseOver; }
	static  void		SetProcessMouseOver( bool b ){ m_bProcessMouseOver = b; }
	static  void		SetProcessMouseOverCtrl( CWinCtrl* pCtrl ){ m_pProcessMouseOverCtrl = pCtrl; }
	static  CWinCtrl*	GetProcessMouseOverCtrl(){ return m_pProcessMouseOverCtrl; }

	//////////////////////////////////////////////////////////////////////////
	/// get, set method...
	//////////////////////////////////////////////////////////////////////////


	void			SetControlName(const char * szName)	{	m_strControlName = szName;	}
	const char *	GetControlName()					{	return m_strControlName.c_str();	}

	int				GetControlType(){ return ( m_dwCtrlType ); }
	void			SetControlType( DWORD dwType ){ m_dwCtrlType = dwType; }
	void			SetControlID( unsigned int iID ) { m_iControlID = iID; };
	unsigned int	GetControlID( ){ return m_iControlID; };

	/// ���º��� �Լ� 
	void			SetCtrlStatus(DWORD	dwStatus)
	{
		if( IsLock() ) return;
		m_dwStatus = dwStatus; 
	}
	virtual	void	Show()							
	{
		if( IsLock() ) return;
		m_dwStatus |=STATUS_VISION;  
	}
	virtual	void	Hide()							
	{
		if( IsLock() ) return;
		m_dwStatus &= ~STATUS_VISION; 
	}	
	virtual	void	ShowMe()
	{
		if( IsLock() ) return;
		m_dwStatus |=STATUS_VISION;  
	}	//�ڽŸ� Show.
	virtual	void	HideMe()
	{
		if( IsLock() ) return;
		m_dwStatus &= ~STATUS_VISION; 
	}	//�ڽŸ� Hide.

	virtual void	SetFocus(bool bFocus)
	{ 
		if( IsLock() ) return;
		if(bFocus)
			m_dwStatus |=STATUS_FOCUS; 
		else
			m_dwStatus &=~STATUS_FOCUS;
	}

	void			SetActive(bool bActive)
	{
		if( IsLock() ) return;
		if(bActive)
			m_dwStatus |=STATUS_ACTIVE; 
		else
			m_dwStatus &=~STATUS_ACTIVE; 
	}


	void			SetEnable( bool bEnable )
	{
		if( IsLock() ) return;
		if( bEnable )
			m_dwStatus |= STATUS_ENABLE;
		else
			m_dwStatus &= ~STATUS_ENABLE;
	}
	/// ���������Լ� 	
	inline  bool	IsFocus()				{ return (m_dwStatus&STATUS_FOCUS)?true:false;  }
	inline  bool	IsActive()				{ return (m_dwStatus&STATUS_ACTIVE)?true:false; }
	virtual bool	IsVision()				{ return (m_dwStatus&STATUS_VISION)?true:false; }
	inline  bool    IsEnable()				{ return (m_dwStatus&STATUS_ENABLE)?true:false; }

	inline  POINT	GetPosition()			{ return m_sPosition; }
	inline	void	SetPosition( POINT pt )	{ m_sPosition = pt ; }
	inline	void	SetPosition( int x, int y) { m_sPosition.x = x; m_sPosition.y = y; }
	inline	void	SetPositionX( int x ) { m_sPosition.x = x; }
	inline	void	SetPositionY( int y ) { m_sPosition.y = y; }	


	void    SetOwnerDraw( bool b );//{ m_bOwnerDraw = b; }
	bool	IsOwnerDraw();

	///Parent ���
	void		SetParent( CWinCtrl* pParent );
	CWinCtrl*	GetParent();
	///OffSet
	virtual void    SetOffset( POINT pt);
	virtual void	SetOffset( int x, int y );
	virtual void	SetOffsetX( int x );
	virtual void	SetOffsetY( int y );


	POINT			GetOffset();
	///
	int				GetWidth(){ return m_iWidth; }
	int				GetHeight(){ return m_iHeight; }
	virtual void			SetWidth(int i){ m_iWidth = i; }
	virtual void			SetHeight( int i ){ m_iHeight = i;}
	///
	virtual void    MoveWindow( POINT pt );
	virtual void    MoveWindow( int x, int y );
	
	///Register  Draw Object
	void			SetDraw( ITDraw* pDraw ){ m_pDraw = pDraw; }
	ITDraw			* GetDraw()	{	return m_pDraw;	}
	void			SetScaleWidth( float fScale );
	void			SetScaleHeight( float fScale );
	void			SetAlphaValue( BYTE btValue );


	void			AddActionListener( IActionListener* pListener );
	void			RemoveActionListener( IActionListener* pListener );

	void			SetSizeFit( bool bFit );
	bool			GetSizeFit();

	void			Lock();
	void			UnLock();
	bool			IsLock();
	bool			m_bLock;

protected:
	void	DrawHeader();
	void	DrawBody();
	void	DrawFooter();
};


#endif /// _CWINCTRL_	