#ifndef _RADIOBUTTON_
#define _RADIOBUTTON_

#include "winctrl.h"
#include "SinglelineString.h"

class CTRadioBox;
class ITFont;

/**
* ���� ��ư Class
*	- �����ڽ��� ����� �� �����ڽ��� ��ϵ� ��ư��� ����Ǿ� ���۵ȴ�.
*	- ��ư�� Ŭ���Ǿ����� ���� �ڽ��� EventChangeSelect()�� ȣ���Ͽ� �ٸ� ��ư���� ���� ����
*	- ����Class : CTRadioBox
*
* @Author			������
*
* @Data				2005/8/30
*/
class TGAMECTRL_API CTRadioButton :	public CWinCtrl
{
public:
	CTRadioButton(void);
	virtual ~CTRadioButton(void);

	bool	Create( int iScrX, int iScrY, int iWidth, int iHeight, int iNormalGraphicID, int iOverGraphicID, int iDownGraphicID ,int iModuleID);
	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual	void Update( POINT ptMouse );
	virtual void Draw();

	void	SetButtonState( int iState );
	short	GetButtonState(){ return m_iButtonState; }

	void    SetRadioBox( CTRadioBox* pRadioBox ){ m_pRadioBox = pRadioBox; }
	enum{
		BS_NORMAL,
		BS_MOUSEOVER,
		BS_CLICKED
	};

	void				SetText( char* szText );
	CSinglelineString&	GetText();

	void		SetSoundDisableID( int i );
	void		SetGhost();

protected:
	CSinglelineString	m_text;					/// ��ư���� ��µ� ��Ʈ��

	short			m_iButtonState;

	int				m_iModuleID;
	int				m_iNormalGraphicID;
	int				m_iOverGraphicID;
	int				m_iDownGraphicID;

	int				m_iDisableSoundID;			/// Disable�� ���¿��� Ŭ���� Play�� Sound ID
	CTRadioBox*		m_pRadioBox;				/// ��ϵ� RadioBox
	ITFont*			m_pFontImpl;
	bool			m_bGhost;					/// ������ ������ �ڸ��� ��� ���� �뵵�� ���� ��� ( CTabbedPane���� ���� ��ư�� �־�� Tab�� ��ϵǾ )
};
#endif