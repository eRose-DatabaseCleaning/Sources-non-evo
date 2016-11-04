#ifndef	_TCAPTION_
#define _TCAPTION_

#include "winctrl.h"

class CTImage;
class CTButton;

/**
* Windows�� Window Caption���� ����� Caption Class
*	- ���� CTDialog�� CTMsgBox�α������� �ߺ��� ó���� ���ְ� �ִ�. �����Ұ�
*	: CTCaption���� ó���ɼ� �ֵ��� �����ʿ�
*
* @Author	������
* @Date		2003/12/11
*/
class TGAMECTRL_API CTCaption :	public CWinCtrl
{
public:
	CTCaption(void);
	virtual ~CTCaption(void);

	bool					Create(int iScrX, int iScrY, int iWidth, int iHeight);
	virtual unsigned int	Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual	void			Update( POINT ptMouse );
	virtual void			Draw();
	virtual void			Hide();
	virtual void			MoveWindow(POINT pt);
	bool		IsClicked();

	void		SetString( const char* pszString );
	void		SetIcon( CTImage* pImage );
	void		SetImage( CTImage* pImage );
	void		SetCloseButton( CTButton* pButton );
	void		SetIconButton( CTButton* pButton );

	enum{
		IID_IMAGE_ICON	= 2,
		IID_IMAGE_BG	= 3,
		IID_BTN_ICON	= 4,
		IID_BTN_CLOSE	= 5
	};

protected:
	bool		m_bClicked;

	/// ���� ���� �����Ǿ� ���� �ʴ�.( ����ϰ� ���� �ʴٴ� : 2005/8/30 ������ )
	std::string	m_strTitle;									///Ÿ��Ʋ �ٿ� ������ �׷����� String
	CTImage*	m_pIcon;									/// ���� �տ� ������ �������̹���
	CTImage*	m_pImage;									/// ĸ�� Image
	CTButton*	m_pCloseButton;								///Dialog Close
	CTButton*	m_pIconButton;								/// Dialog Iconize(??)������ȭ
};
#endif	///_TCAPTION_