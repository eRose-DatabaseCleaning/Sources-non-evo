#ifndef	_TBUTTON_
#define	_TBUTTON_

#include "winctrl.h"
#include "SinglelineString.h"

//---------------------------------------------------------------------------------------------------------------------
/// Button Class
///
/// @Author	������
/// @Date	2005/8/30
//---------------------------------------------------------------------------------------------------------------------
class  TGAMECTRL_API CTButton : public CWinCtrl
{
public:
	CTButton(void);
	virtual ~CTButton(void);
	bool	Create( int iScrX, int iScrY, int iWidth, int iHeight, int iNormalGraphicID, int iOverGraphicID, int iDownGraphicID , int iModuleID );
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

	void				SetText( char* szText );						
	CSinglelineString&	GetText();
	void				SetTextColor( DWORD color );

	void		SetSoundOverID( int i ){ m_iOverSoundID = i; }
	void		SetSoundClickID( int i ){ m_iClickSoundID = i; }
	void		SetDisableGrahicID( int i ){ m_iDisableGrahicID = i; }

	void		SetNoImage();
	void		SetBlink( int iBlink , int iBlinkTime );

	void		SetBlinkGid( int iGid );
	
protected:
	CSinglelineString	m_text;							/// ��ư���� ��µ� Text

	int				m_iButtonState;						/// BS_NORMAL, BS_MOUSEOVER, BS_CLICKED

	int				m_iModuleID;						/// Images�� ��� ID - �ִ� 5�� ������ �̹����� �����Ҽ� ������ ����� �Ѱ��̴�(����� ��⿡ �־�� �Ѵ�)
	
	int				m_iNormalGraphicID;					/// ������¿����� �̹��� ID
	int				m_iOverGraphicID;					/// ���콺�� ��ư�������� �������� �̹��� ID
	int				m_iDownGraphicID;					///	������ �Է����� ��ư�� ���������� �̹��� ID
	int				m_iDisableGrahicID;					/// Disable�����϶��� �̹��� ID	
	int				m_iBlinkGraphicID;					/// �����ӻ����϶� �븻���¿� ������ ǥ�õ� �̹��� ID
	int				m_iSourceNormalGraphicID;			/// �����ӻ����϶� �ӽ÷� ���� �븻������ �̹��� ID�� �����ϰ� ���� ���� 


	

	
	int				m_iOverSoundID;						/// ��ư�� 
	int				m_iClickSoundID;					/// ��ư Ŭ���� Play�� ���� ID
	int				m_iBlink;							/// ������ �����ΰ�?
	int				m_iBlinkTime;						/// �����ӻ����ϰ�� �ð� ����
	DWORD			m_dwPrevUpdateTime;
	bool			m_bNoImage;							/// �̹��� ���� ��ư�ΰ�?
	DWORD			m_textcolor;						/// �ؽ�Ʈ Į��

};
#endif/// _TBUTTON_
