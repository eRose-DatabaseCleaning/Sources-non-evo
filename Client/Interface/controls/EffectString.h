#ifndef _CEffectString_
#define _CEffectString_

#include "TSplitString.h"
/**
* �ð��� ���� ��Ʈ ����� ���İ��� ����Ǵ� String��¿� Class
*	- Outline Font�� ��� Alpha���� ����ɰ�� Outline�� ���� ������� �ʴ� ��Ȳ���� ���ȵǰ� �ִ�.
*	- Update()�� �����ϸ�  Alpha���� ������ ����ǵ��� ��밡���ϴ�
*
* @Author		������
* @Date			2005/9/6
*/
class CEffectString
{
	CTSplitString	m_parser;			/// �ؽ�Ʈ ���� 
	POINT			m_draw_position;	/// ȭ�� ��ǥ
	DWORD			m_time_display;		/// ȭ�鿡 ǥ���� �ѽð�
	DWORD			m_time_set;			/// ���� ������Ʈ �ð�
	BYTE			m_color_red;
	BYTE			m_color_green;
	BYTE			m_color_blue;
	BYTE			m_color_alpha;
	int				m_font;

	int				m_type;				/// ��ü�� �����ϱ� ���� Ÿ��
public:
	CEffectString(void);
	virtual ~CEffectString(void);
	void	SetColor( BYTE red, BYTE green, BYTE blue );
	bool	SetString( int font, char* message, POINT draw_position, int display_width, DWORD display_time );

	int		Update();
	void	Draw();

	void	SetType( int type );
	int		GetType();
};
#endif