#ifndef _CTFrame_
#define _CTFrame_

#include "winctrl.h"
class CTImage;

/**
* 2D �ִϸ��̼ǿ� ���Ǵ� Frame Class
*	- CTImage�� ǥ�õǾ��� �ð��� ������ �ִ�
*   - ����Class : CTAnimation
*
* @Author		������
*
* @Date			2005/8/30
*/
class TGAMECTRL_API CTFrame :	public CWinCtrl
{
public:
	CTFrame(void);
	virtual ~CTFrame(void);
	virtual void Show();
	virtual void Hide();
	virtual void Draw();
	void	SetPosition( int x, int y );
	DWORD	get_time();
	void	set_time( DWORD time );
	void	set_image( CTImage* image );

protected:
	CTImage*	m_Image;					 
	DWORD		m_time;						/// ȭ�鿡 ��µǾ��� �ð�( �и������� )
	
};
#endif