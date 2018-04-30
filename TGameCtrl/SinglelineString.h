#ifndef _CSinglelineString_
#define _CSinglelineString_

#include "TGameCtrl.h"
#include <string>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/// ǥ���� String�� �ش� ������ ��� ��� �ڿ� ".."�� ������ �����ȿ� �� ǥ�õǵ��� �ϴ� Class
///		- Client���� get_rect()�� is_ellipsis()�� ���콺�� ��ġ�� ©���� üũ�Ͽ� �������� ��ü ��Ʈ���� �����ִ� �� ����ϰ� �ִ�.
///
/// @Author					������
/// @Date					2005/8/30
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TGAMECTRL_API CSinglelineString
{
public:
	CSinglelineString(void);
	~CSinglelineString(void);
	void	set_string( const char* msg, const RECT& rect );				// ���ڿ�
	void	set_string( const char* msg, const RECT& rect , int ifont );	// ���ڿ�
	void	set_rect( const RECT& rect );									// ����
	void	set_color( DWORD color );										// ����
	void	set_format( DWORD format );										// ����
	void	set_font( int iFont );											// ��Ʈ

	const char* get_string();
	const RECT&	get_rect();

	void	draw();
	bool	is_ellipsis();
	bool	empty();
	void	clear();


protected:
	std::string		m_msg;			/// ���� ��Ʈ��
	std::string		m_ellipsis_msg; /// �����ȿ� �� ǥ�ð� �ȵɰ�� �����ȿ� ǥ���� string�� ���� �����صδ� string
	DWORD			m_format;		/// 
	RECT			m_rect;			/// ��Ʈ���� ǥ���� ����
	bool			m_end_ellipsis;	/// ��Ʈ���� �ش� �����ȿ� �� ǥ�ð� �ȵǴ°�?
	int				m_font;			/// ��Ʈ
	DWORD			m_color;		/// ��Ʈ ����
};
#endif