#ifndef	_TDIALOG_
#define	_TDIALOG_

#include "winctrl.h"
#include <string>
#include "TCommandQ.h"

class CTCaption;
class CTCommand;
class CTStatusBar;
class CTImage;
/**
* WinCtrl���� child ������ ó���ϴ� Class - �������̽�ó������ ���� �⺻�� �Ǵ� Class�̴�
* 
* @Author		������
*
* @Date			2005/8/30 
*/
class TGAMECTRL_API CTDialog : public CWinCtrl
{
public:
	CTDialog(void);
	virtual ~CTDialog(void);
	bool     Create( int iScrX, int iScrY, int iWidth, int iHeight );
	virtual bool	Create( const char* IDD );
	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual	void	Update( POINT ptMouse );
	virtual void	Draw();
	virtual	void	Show();
	virtual	void	Hide();
	virtual void    MoveWindow( POINT pt );
	virtual void    MoveWindow( int x, int y );
	virtual void	SetInterfacePos_After();
	
	/// ��� ���̾�α� �ΰ�?
	virtual bool    IsModal();													
	
	/// ���̾�α��� �Ϻ� �Ӽ��� ������ ������ �ϱ����� virtual method : client���� �������ؼ� ����Ѵ�.
	virtual void	PostMsg( unsigned msg, unsigned param1, unsigned param2 ){}	

	/// ���̾�α��� ��밡���� üũ�ϱ����� virtual method : Client���� �������Ͽ� ��� - 	if( return value == 1 ) Valid 
	virtual int		IsInValidShow(){ return 0; }

	///WinCtrl �߰�/����/ã�� Method
	void		Add( CWinCtrl* pChild );
	bool		Remove( int iID );
	CWinCtrl*	Find( int iID );
	CWinCtrl*	Find( const char * strName );

	/// �����ϰ� �ִ�.
	void		SetStatusBar( CTStatusBar* pStatusBar );				

	///ĸ�� ����
	void		SetCaption( CTCaption* pCaption );
	long		GetChildCount();
	CTCaption*	GetCaption(){ return m_pCaption; }
	void		ChangeCaptionRect( RECT rc );


	///Ư�� child���� Show/Hide/Enable
	bool		ShowChild( unsigned int iID );
	bool		HideChild( unsigned int iID );
	bool		ShowChild( const char * strName  );
	bool		HideChild( const char * strName  );
	bool		SetEnableChild( unsigned int iID, bool bEnable );

	virtual void RefreshDlg();


	/// Dialog������ ���� ǥ�� ��ġ�� ����� ��ġ Ÿ��: X,Y
	enum{
		TDXP_LEFT,
		TDXP_CENTER,
		TDXP_RIGHT
	};
	
	enum{
		TDYP_TOP,
		TDYP_CENTER,
		TDYP_BOTTOM
	};

	DWORD		GetDialogType(){ return m_dwDialogType; }
	void		SetDialogType( DWORD iDialogType ){ m_dwDialogType = iDialogType;}

	///	Command Pattern���� ���Ǵ� Command�� �߰�/����/����
	void		Push_Back_Cmd( CTCommand* pCmd );
	void		Push_Front_Cmd( CTCommand* pCmd );
	void		ClearCommandQ();
	CTCommand*	GetTCommand();

	/// �������ӳ����� ���콺�� �ڽ��� �ٷ� ���� �ִ�( �ٸ� control�� ���°�� ) TDialog�� ������ ����
	static  void		SetProcessMouseOverDialog( CTDialog* pTDialog ){ m_pProcessMouseOverDialog = pTDialog; }
	static  CTDialog*	GetProcessMouseOverDialog(){ return m_pProcessMouseOverDialog; }

	///����
	void		SetSoundShowID( int i ){ m_iShowSoundID = i; }	/// Dialog�� Show�� Play�� Sound ID Set
	void		SetSoundHideID( int i ){ m_iHideSoundID = i; }	/// Dialog�� Hide�� Play�� Sound ID Set

	/// Default Position ���� 
	void		SetDefaultPosX( int iPosX ){ m_iXPos = iPosX; }
	void		SetDefaultPosY( int iPosY ){ m_iYPos = iPosY; }
	void		SetDefaultAdjustPosX( int iAdjustPosX ){ m_iAdjustXPos = iAdjustPosX; }
	void		SetDefaultAdjustPosY( int iAdjustPosY ){ m_iAdjustYPos = iAdjustPosY; }

	int			GetDefaultPosX(){ return m_iXPos; }
	int			GetDefaultPosY(){ return m_iYPos; }
	int			GetDefaultAdjustPosX(){ return m_iAdjustXPos; }
	int			GetDefaultAdjustPosY(){ return m_iAdjustYPos; }

	void		SetDefaultVisible(bool b) { m_bDefaultVisible = b; }
	bool		IsDefaultVisible(){ return m_bDefaultVisible; }


	void		SetModal();																/// Dialog�� ��޷� 
	void		SetModeless();															/// Dialog�� ��޸�����


	void		SetMinimizeRect( const RECT& rc );

	void		SetExtent( int i );
	void		SetImage( CTImage* pImage );


protected:
	void		MoveCtrl2ListEnd( int iID );											/// �ش� ID�� child�� ����Ʈ�� �ǵڷ� �̵�
	void		ProcessLButtonDown();													/// Dialog������ LButtonDown�� �Ͼ���� ó���� Method


protected:
	WINCTRL_LIST	m_listChild;														/// WinCtrl�� ������ Container;
	WINCTRL_LIST	m_OwnerDrawChildren;												/// ������

	CTCaption*		m_pCaption;															/// TCaption Object
	CTStatusBar*	m_pStatusBar;														/// ������
	CTImage*		m_pImage;															/// ������	


	POINT			m_ptCaptionClicked;													/// ĸ���� ������������ Mouse �� Dialog Left-Top���� �Ÿ� 

	DWORD			m_dwDialogType;

	bool			m_bModal;															/// Modal Or Modaless : Default - Modaless
	int				m_iShowSoundID;														/// Dialog::Show() �� Play�� Sound ID	
	int				m_iHideSoundID;														/// Dialog::Hide() �� Play�� Sound ID	

	CTCommandQ		m_CommandQ;															/// TCommand�� ���� - Hide�� �Ҹ�ÿ� �ڵ����� Clear�ǰ� �Ѵ�.

	///Default Position
	int				m_iXPos;															/// LEFT-CENTER-RIGHT( ����� X )
	int				m_iYPos;															/// TOP-CENTER-BOTTOM( ����� Y )
	int				m_iAdjustXPos;														/// ���� X��ġ - �ȼ�
	int				m_iAdjustYPos;														/// ���� Y��ġ - �ȼ�
	bool			m_bDefaultVisible;													/// Ŭ���̾�Ʈ���� ���� ���ӽ���� �����ٰ��ΰ�?
	

	int				m_iExtent;															///	ClientRect�� �׷��� Image�� ����(Row) - �ߺ��� Image�� ������ �׸��⿡ ����Ϸ��� ������ ���� 1�� ����

	static CTDialog* m_pProcessMouseOverDialog;											/// �� �����Ӿȿ��� ���콺�� �ٷ� �ڽ� ���� �ִ� Dialog�� ������( Update�� ��ó�� clear�����־�� �Ѵ� )

};
#endif	///_TDIALOG_
