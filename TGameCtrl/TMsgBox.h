#ifndef _CTMSGBOX_
#define _CTMSGBOX_

#include "winctrl.h"
#include "TDialog.h"
#include "JStringParser.h"
class CTListBox;
class CTButton;
class CTImage;
class CTCaption;

/**
* ���ӳ����� �޼����ڽ��� ���� Ư�� Dialog 
*	- TDialog�� ��� �޾Ƽ� ��� TDialog�� ����� ������ ������, Ok,Cancel��ư, 3���� �������� �̹��� �κ��� �߰��Ǿ� �ִ�.
*
* @Author	������
*
* @Data		2005/8/30	
*/

class TGAMECTRL_API CTMsgBox : public CTDialog
{
public:
	CTMsgBox(void);
	virtual ~CTMsgBox(void);

	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual	void Update( POINT ptMouse );
	virtual	void Draw();
	virtual void MoveWindow( POINT pt );
	virtual void Show();
	virtual void Hide();
	virtual bool Create( char* szIDD );


	void		SetButtonType( short int iType );
	short int	GetButtonType(){ return m_iButtonType; }



	enum{
		BT_NONE		= 0x0000,
		BT_OK		= 0x0001,
		BT_CANCEL	= 0x0002
	};

	void SetButtons( CTButton* pBtnOk, CTButton* pBtnCancel );
	void SetImages( CTImage* pImageTop, CTImage* pImageMiddle, CTImage* pImageBottom );
	virtual void SetString( const char* szString );
	void SetMsgType( unsigned int iType ){ m_iMsgType = iType; }


	unsigned int GetMsgType(){ return m_iMsgType; }


	void	AddExtraHeight( int iHeight );
	int		GetExtraHeightStartPoint();
protected:
	void DrawImages();

protected:

	CTButton*	m_pButtonOk;									// Ok Button
	CTButton*	m_pButtonCancel;								// Cancel Button
	
	CTImage*	m_pImageTop;									// ����̹����� ���
	CTImage*	m_pImageMiddle;									// ����̹����� ���( �޼��� �ڽ��� ǥ�õ� ��Ʈ���� ���̿� ���� �ڵ����� �ݺ��Ǿ� �׷����� )
	CTImage*	m_pImageBottom;									// ����̹����� �ϴ�
	
	short int   m_iButtonType;									// BT_NONE, BT_OK, BT_CANCEL

	unsigned int m_iMsgType;
	int			 m_iExtraHeight;								// �߰����� ��������
	int			 m_iExtraHeightStartPoint;

	CJStringParser m_message_parser;							// �޼��� �ڽ��� ��µ� ��Ʈ��
};
#endif
