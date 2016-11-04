#ifndef _CTOOLTIP_
#define _CTOOLTIP_

#include "SplitHangul.h"
#include "TGameCtrl.h"
class CTImage;
class CWinCtrl;
/// CWinCtrl�� ���ԵǴ� Tooltip Class
/// Client���� �׸������� ���� �����ؾ��ϴ� ������ ���
/// DrawBody�� DrawText�� Overriden�Ͽ� �����ؼ� CWinCtrl�� SetToolTip���ش�.
class TGAMECTRL_API CToolTip
{
public:
	CToolTip(void);
	virtual ~CToolTip(void);

	void	Draw( POINT pt );

	void	SetText( char* szText );

	void	SetImageTop( CTImage* pImage );
	void	SetImageMiddle( CTImage* pImage );
	void	SetImageBottom( CTImage* pImage );

	void	SetFont( int i ){ m_iFont = i; }
	void	SetMarginLeft( int i ){ m_iMarginLeft = i ; }
	void	SetMarginTop( int i ){ m_iMarginTop = i ; }
	void	SetLineSpace( int i ){ m_iLineSpace = i ; }
	void	SetColor( DWORD color ){ m_dwColor = color; }

	int		GetMarginLeft(){ return m_iMarginLeft; }
	int		GetMarginTop(){ return m_iMarginTop; }
	int		GetLineSpace(){ return m_iLineSpace; }
	int		GetFont(){ return m_iFont; }

protected:
	void	DrawTopImage( POINT pt );
	void	DrawBody( POINT pt );
	void	DrawBottomImage( POINT pt );

	void	DrawText( int iFont, POINT pt , D3DCOLOR color, const char* szText );

protected:
	int		m_iFont;			
	int		m_iMarginLeft;		
	int		m_iMarginTop;		
	int		m_iLineSpace;		
	int		m_iMaxLength;
	DWORD	m_dwColor;
	
	std::string		m_stMsg;
	CSplitHangul	m_splitMsg;


	CTImage* m_pImageTop;
	CTImage* m_pImageMiddle;
	CTImage* m_pImageBottom;

};
#endif