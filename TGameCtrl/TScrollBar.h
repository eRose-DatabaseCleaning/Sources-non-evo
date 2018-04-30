#ifndef	_TSCROLLBAR_
#define _TSCROLLBAR_

#include "winctrl.h"
#include "TScrollBarType.h"


class CTButton;
class CTScrollBox;
class IScrollModel;
class CTScrollBarType;
class CTImage;

/**
* ��ũ�� �� Class
*	- ����Class		CTScrollBarType, CTScrollBox
*
* @Author	������
*
* @Data		2005/8/30
*/
class TGAMECTRL_API CTScrollBar : public CWinCtrl
{
public:
	CTScrollBar(void);
	virtual ~CTScrollBar(void);
	bool					Create(int iScrX, int iScrY, int iWidth, int iHeight,int iType = CTScrollBarType::TSBT_VERTICAL );
	virtual unsigned int	Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual	void			Update( POINT ptMouse );
	virtual void			Draw();

	void SetBackImage( CTImage* pImg );
	void SetPrevButton( CTButton* pButton );
	void SetNextButton( CTButton* pButton );
	void SetScrollBox( CTScrollBox* pThumb );

	virtual void	MoveWindow( POINT pt );
	virtual void	SetOffset( POINT pt );
	void			SetTickMove( int i );
	RECT			GetThumbMoveRange();
	void			MoveEnd();
	void			MoveHome();

	///	Scroll�� ���̱� ���� Class(IScrollModel�� ��ӹ���)�� ������
	void			SetModel( IScrollModel* pModel );

	/// ScrollBox�� ScrollModel�� ���� �˱� ���Ͽ� �߰����ִ� operation
	int				GetExtent();
	int				GetMaximum();
	void			SetModelValue( int i );
	virtual	void	Show();
	virtual void	Hide();

	virtual void			SetHeight( int i );


	/// IScrollModel�� ScrollBox(Thumb)�� ����( Prev,Next��ư,wheel ó���� )
	void			SetValue(int iValue);

protected:
	CTButton*		m_pPrevButton;				// "����" ��ư						
	CTButton*		m_pNextButton;				// "����" ��ư
	CTScrollBox*	m_pScrollBox;				// ����,������ ���� ������Ʈ ���ϻ��
	CTImage*		m_pBackImage;

	IScrollModel*		m_pScrollModel;			// ScrollBar�� ���� ���۵� Model�� ������
	CTScrollBarType*	m_pScrollBarType;		// ����,������ ���� ��Ʈ��Ƽ�� ���� ���
};
#endif ///_TSCROLLBAR_