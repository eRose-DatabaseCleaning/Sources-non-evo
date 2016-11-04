#ifndef _CTPUSHBUTTON_
#define _CTPUSHBUTTON_
#include "winctrl.h"

/**
* üũ ��ưó�� �ش��ư�� Ŭ���� Normal�� Down�� �ݺ��Ǵ� Button
*	- client���� 1:1�ŷ�â���� �����( 2005/8/30)
*
* @Author	������
*
* @Data		2005/8/30
*/
class CTPushButton : public CWinCtrl
{
public:
	CTPushButton(void);
	virtual ~CTPushButton(void);

	bool	Create( int iScrX, int iScrY, int iWidth, int iHeight, int iNormalGraphicID, int iOverGraphicID, int iDownGraphicID ,int iModuleID );
	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual	void Update( POINT ptMouse );
	virtual void Draw();
	enum{
		BS_NORMAL,
		BS_MOUSEOVER,
		BS_CLICKED,
	};

	void		SetButtonState( int iState ){ m_iButtonState = iState; }
	int			GetButtonState(){ return m_iButtonState; }
	void		SetText( char* szText );
	const char*	GetText(){ return m_stText.c_str(); }

	void		SetSoundOverID( int i ){ m_iOverSoundID = i; }
	void		SetSoundClickID( int i ){ m_iClickSoundID = i; }

protected:
	std::string		m_stText;
	int				m_iButtonState;
	int				m_iModuleID;
	int				m_iNormalGraphicID;
	int				m_iOverGraphicID;
	int				m_iDownGraphicID;

	int				m_iOverSoundID;
	int				m_iClickSoundID;
};
#endif
