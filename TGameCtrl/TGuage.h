#ifndef _CTGUAGE_
#define _CTGUAGE_

#include "winctrl.h"
#include "ITFont.h"
#include "ITDraw.h"

/**
* ������ Class
*
* @Author	������
*
* @Date		2005/8/30
*/
class TGAMECTRL_API CTGuage :	public CWinCtrl
{
public:
	CTGuage();
	~CTGuage(void);

	virtual void	Draw();
	bool Create( int iScrX, int iScrY, int iWidth, int iHeight, int iGraphicID ,int iBGraphicID, int iModuleID );
	
	void	SetText( const char* szTxt);
	const   char* GetText();

	void	SetValue( int iValue );
	int		GetValue();

	void	SetSizeFit( bool bFit );
	bool	GetSizeFit();
	
	void	SetGuageImageID( int iImageID );
	enum {
		UPDATE_DELETE,									// PARENT���� �����޶�� ��û ( ��뿡 ���� )
		UPDATE_NORMAL,		
		UPDATE_END										// UPDATE���� ������ �� ���´�( PARENT�� �˾Ƽ� ������ �ؾ��Ұ͵��� ó�� )
	};

protected:
	///Image
	int				m_iBGImageID;
	int				m_iGuageImageID;
	int				m_iModuleID;
	///Data
	std::string		m_strText;


	///Guage�� �׸� Width
	int				m_iValue;							///���� �������� �׷������� %(0 ~ 100)
	bool			m_bSizeFit;							/// draw���� ������ with, height�� Fit.

	ITFont*			m_pFontImpl;
	ITDraw*			m_pDrawImpl;
};

#endif
