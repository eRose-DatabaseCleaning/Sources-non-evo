#ifndef _CTAnimation_
#define _CTAnimation_

#include "winctrl.h"
#include <deque>

class CTFrame;

//---------------------------------------------------------------------------------------------------------------------
/// 2D �ִϸ��̼� Class
///		- Ŭ���̾�Ʈ���� �̴ϸ��� Indicator�� ���ǰ� �ִ�
///     - ����Class						CTFrame
///
/// @Author				������
///
/// @Date				2005/8/30
//---------------------------------------------------------------------------------------------------------------------
class TGAMECTRL_API CTAnimation :	public CWinCtrl
{
public:
	CTAnimation(void);
	virtual ~CTAnimation(void);
	virtual void Show();
	virtual void Hide();
	virtual void Draw();
	virtual void Update( POINT ptMouse );

	void	SetPosition( int x, int y );									/// Draw �� ȭ�� ��ǥ Set
	void	add_frame( CTFrame* frame );									/// Frame�߰�
	void	set_loop( bool loop );											/// �ִϸ��̼��� Loop �ɼ� Set 
	void	set_curr_frame( unsigned i );									/// Frame�� ���� �̵�

		

protected:
	 
	std::deque< CTFrame* >::size_type						m_curr_frame;	/// ���� Draw�� Frame�� �ε���
	bool					m_loop;											/// �ִϸ��̼��� Loop On/Off
	std::deque< CTFrame* >	m_frames;										/// Frame Container
	DWORD					m_prev_update_time;								/// Update�� �ٷ� ������ �ð�
};
#endif