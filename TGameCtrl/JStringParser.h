#ifndef _CJStringParser_
#define _CJStringParser_

#include "JString.h"
#include <vector>

/// Tag �Ľ̽� �ʿ��� Data ����ü
struct TagInfo{
	DWORD	m_color;
	int		m_font;
	bool	m_linefeed;
	char*   m_pCurrChar;
};

//--------------------------------------------------------------------------------------------------------------------------------------
///	���ڿ� �Ľ� Class
///		1. ���� TAG : 
///			- {BR}	: �ٳѱ�
///			- {B}	: ����		* ���� : Ŭ���̾�Ʈ���� ���� ��Ʈ�� �Ϲ���Ʈ �ε���+3�̶�� �����Ͽ� �����Ǿ� �ִ�
///			- {FC=?}: ���ڻ�����
///	
///		2. �ڵ��ٳѱ�
///			- �ܾ(ȭ��Ʈ �����̽� ������ ���ڿ��� ����)
///			- ���ں�
///
///		- ����class			CJString
///
/// @Author				������
///
/// @Date				2005/08/30
//--------------------------------------------------------------------------------------------------------------------------------------
class TGAMECTRL_API CJStringParser
{
public:
	CJStringParser(void);
	~CJStringParser(void);
	int			SetString( const char* pszString ,int iDisplayWidth);
	CJString*	GetString( int index );
	int			GetStringCount();													
	void		Clear();

	void		SetDefaultFont( int iFont );									/// Default ��Ʈ Set
	void		SetDefaultColor( DWORD dwColor );								/// Default ��Ʈ ���� Set
	void		SetSplitType( int type );										/// �ڵ� �ٳѱ� Ÿ���� Set

	enum{																		/// �ڵ� �ٳѱ� Ÿ��
		SPLIT_LETTER,															/// ���ں�(Default)
		SPLIT_WORD,																/// �ܾ
	};
protected:
	std::vector< CJString >	m_Strings;											/// 
	int			m_iDefaultFont;													/// Default ��Ʈ
	int			m_split_type;													/// �ٳѱ� Ÿ��
	DWORD		m_dwDefaultColor;												/// Default ��Ʈ ����


	/// ���� Tag�� �Ľ��ϱ� ���� Method��
	void		Tag( TagInfo& tag_info );
	void		TagStartProcess( std::string& Tag, TagInfo& tag_info );
	void		TagEndProcess( std::string& Tag, TagInfo& tag_info );
	enum{
		TAG,
		START_TAG,
		END_TAG
	};

	LPSTR		GetNextWord( WORD CodePage,LPSTR pCurrChar, DWORD dwFlags );
};
#endif