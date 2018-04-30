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
* WinCtrlµéÀ» child °¡Áö°í Ã³¸®ÇÏ´Â Class - ÀÎÅÍÆäÀÌ½ºÃ³¸®¿¡¼­ °¡Àå ±âº»ÀÌ µÇ´Â ClassÀÌ´Ù
* 
* @Author		ÃÖÁ¾Áø
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
	
	/// ¸ð´Þ ´ÙÀÌ¾ó·Î±× ÀÎ°¡?
	virtual bool    IsModal();													
	
	/// ´ÙÀÌ¾ó·Î±×ÀÇ ÀÏºÎ ¼Ó¼º¹× °£´ÜÇÑ ÇàÀ§¸¦ ÇÏ±âÀ§ÇÑ virtual method : client¿¡¼­ ÀçÁ¤ÀÇÇØ¼­ »ç¿ëÇÑ´Ù.
	virtual void	PostMsg( unsigned msg, unsigned param1, unsigned param2 ){}	

	/// ´ÙÀÌ¾ó·Î±×ÀÇ »ç¿ë°¡´ÉÀ» Ã¼Å©ÇÏ±âÀ§ÇÑ virtual method : Client¿¡¼­ ÀçÁ¤ÀÇÇÏ¿© »ç¿ë - 	if( return value == 1 ) Valid 
	virtual int		IsInValidShow(){ return 0; }

	///WinCtrl Ãß°¡/»èÁ¦/Ã£±â Method
	void		Add( CWinCtrl* pChild );
	bool		Remove( int iID );
	CWinCtrl*	Find( int iID );
	CWinCtrl*	Find( const char * strName );

	/// »ç¿ë¾ÈÇÏ°í ÀÖ´Ù.
	void		SetStatusBar( CTStatusBar* pStatusBar );				

	///Ä¸¼Ç °ü·Ã
	void		SetCaption( CTCaption* pCaption );
	long		GetChildCount();
	CTCaption*	GetCaption(){ return m_pCaption; }
	void		ChangeCaptionRect( RECT rc );


	///Æ¯Á¤ child¸¸À» Show/Hide/Enable
	bool		ShowChild( unsigned int iID );
	bool		HideChild( unsigned int iID );
	bool		ShowChild( const char * strName  );
	bool		HideChild( const char * strName  );
	bool		SetEnableChild( unsigned int iID, bool bEnable );

	virtual void RefreshDlg();


	/// Dialog»ý¼ºÈÄ ÃÖÃÊ Ç¥½Ã À§Ä¡ÀÇ »ó´ëÀû À§Ä¡ Å¸ÀÔ: X,Y
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

	///	Command PatternÀ¸·Î »ç¿ëµÇ´Â CommandÀÇ Ãß°¡/»èÁ¦/°ü¸®
	void		Push_Back_Cmd( CTCommand* pCmd );
	void		Push_Front_Cmd( CTCommand* pCmd );
	void		ClearCommandQ();
	CTCommand*	GetTCommand();

	/// ÇÑÇÁ·¹ÀÓ³»¿¡¼­ ¸¶¿ì½º°¡ ÀÚ½ÅÀÇ ¹Ù·Î À§¿¡ ÀÖ´Â( ´Ù¸¥ controlÀÌ ¾ø´Â°æ¿ì ) TDialogÀÇ Æ÷ÀÎÅÍ °ü¸®
	static  void		SetProcessMouseOverDialog( CTDialog* pTDialog ){ m_pProcessMouseOverDialog = pTDialog; }
	static  CTDialog*	GetProcessMouseOverDialog(){ return m_pProcessMouseOverDialog; }

	///»ç¿îµå
	void		SetSoundShowID( int i ){ m_iShowSoundID = i; }	/// Dialog°¡ Show½Ã PlayµÉ Sound ID Set
	void		SetSoundHideID( int i ){ m_iHideSoundID = i; }	/// Dialog°¡ Hide½Ã PlayµÉ Sound ID Set

	/// Default Position °ü·Ã 
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


	void		SetModal();																/// Dialog¸¦ ¸ð´Þ·Î 
	void		SetModeless();															/// Dialog¸¦ ¸ð´Þ¸®½º·Î


	void		SetMinimizeRect( const RECT& rc );

	void		SetExtent( int i );
	void		SetImage( CTImage* pImage );


protected:
	void		MoveCtrl2ListEnd( int iID );											/// ÇØ´ç IDÀÇ child¸¦ ¸®½ºÆ®ÀÇ ¸ÇµÚ·Î ÀÌµ¿
	void		ProcessLButtonDown();													/// DialogÀ§¿¡¼­ LButtonDownÀÌ ÀÏ¾î³µÀ»¶§ Ã³¸®ÇÒ Method


protected:
	WINCTRL_LIST	m_listChild;														/// WinCtrlÀ» º¸°üÇÒ Container;
	WINCTRL_LIST	m_OwnerDrawChildren;												/// »ç¿ë¾ÈÇÔ

	CTCaption*		m_pCaption;															/// TCaption Object
	CTStatusBar*	m_pStatusBar;														/// »ç¿ë¾ÈÇÔ
	CTImage*		m_pImage;															/// »ç¿ë¾ÈÇÔ	


	POINT			m_ptCaptionClicked;													/// Ä¸¼ÇÀÌ ´­·ÁÁ³À»¶§ÀÇ Mouse ¿Í Dialog Left-Top°úÀÇ °Å¸® 

	DWORD			m_dwDialogType;

	bool			m_bModal;															/// Modal Or Modaless : Default - Modaless
	int				m_iShowSoundID;														/// Dialog::Show() ½Ã PlayµÉ Sound ID	
	int				m_iHideSoundID;														/// Dialog::Hide() ½Ã PlayµÉ Sound ID	

	CTCommandQ		m_CommandQ;															/// TCommand¸¦ ÀúÀå - Hide¹× ¼Ò¸ê½Ã¿¡ ÀÚµ¿À¸·Î ClearµÇ°Ô ÇÑ´Ù.

	///Default Position
	int				m_iXPos;															/// LEFT-CENTER-RIGHT( »ó´ëÀû X )
	int				m_iYPos;															/// TOP-CENTER-BOTTOM( »ó´ëÀû Y )
	int				m_iAdjustXPos;														/// º¸Á¤ XÀ§Ä¡ - ÇÈ¼¿
	int				m_iAdjustYPos;														/// º¸Á¤ YÀ§Ä¡ - ÇÈ¼¿
	bool			m_bDefaultVisible;													/// Å¬¶óÀÌ¾ðÆ®¿¡¼­ ÃÖÃÊ °ÔÀÓ½ÇÇà½Ã º¸¿©ÁÙ°ÍÀÎ°¡?
	

	int				m_iExtent;															///	ClientRect¿¡ ±×·ÁÁú ImageÀÇ °³¼ö(Row) - Áßº¹µÈ ImageÀÇ ¿©·¯¹ø ±×¸®±â¿¡ »ç¿ëÇÏ·Á°í ÇßÀ¸³ª ÇöÀç 1·Î °íÁ¤

	static CTDialog* m_pProcessMouseOverDialog;											/// ÇÑ ÇÁ·¹ÀÓ¾È¿¡¼­ ¸¶¿ì½º°¡ ¹Ù·Î ÀÚ½Å À§¿¡ ÀÖ´Â DialogÀÇ Æ÷ÀÎÅÍ( Update½Ã ¸ÇÃ³À½ clear½ÃÄÑÁÖ¾î¾ß ÇÑ´Ù )

};
#endif	///_TDIALOG_
