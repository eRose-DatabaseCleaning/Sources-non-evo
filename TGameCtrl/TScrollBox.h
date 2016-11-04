#ifndef	_TSCROLLBOX_
#define _TSCROLLBOX_

#include "winctrl.h"
#include "TScrollBarType.h"
class CTScrollBar;
class CTScrollBoxType;
class CTImage;

/**
* TScrollBar������ ��ũ�ѹڽ�( ��ư ) Class
* - ����Class : CTScrollBoxType, CTScrollBar
*
* @Author		������
* @Date			2005/8/30
*/
class TGAMECTRL_API CTScrollBox : public CWinCtrl
{
public:
	CTScrollBox(void);
	virtual ~CTScrollBox(void);

	bool					Create(int iWidth, int iHeight, int iGraphicID,int iModuleID, int iType = CTScrollBarType::TSBT_VERTICAL );
	virtual unsigned int	Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual	void			Update( POINT ptMouse );
	virtual void			Draw();

	void		SetTickMove( int i );
	void		SetScrollBar( CTScrollBar* pScrollBar ){ m_pScrollBar = pScrollBar; }
	void		MoveBoxPosition( int iValue, int iMax ,int iExtent );
	void		MoveBoxPositionByScreenPoint( POINT pt );
	void		SetBlink();
	void		SetUnblink();
	void		SetBlinkImage( CTImage* pImage );
protected:


protected:
//	BYTE			m_bBlink
	bool			m_bBlink;
	int				m_iModuleID;
	int				m_iGraphicID;
	CTScrollBar*	m_pScrollBar;

	///��ũ�ѹڽ��� �̵��� ���� �׷��Ȱ� ��ư�ٿ� ���콺�����Ϳ��� �Ÿ����� �����صд�.
	///Update���� �ش� ���̸�Ŭ ���� �׷����� �̵���Ų��.


	bool				m_bClicked;
	CTScrollBoxType*	m_pScrollBoxType;
	CTImage*			m_pBlinkImage;
};
#endif ///_TSCROLLBOX_