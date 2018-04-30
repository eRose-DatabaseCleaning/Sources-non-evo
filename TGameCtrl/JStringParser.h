#ifndef _CJStringParser_
#define _CJStringParser_

#include "JString.h"
#include <vector>

/// Tag ÆÄ½Ì½Ã ÇÊ¿äÇÑ Data ±¸Á¶Ã¼
struct TagInfo{
	DWORD	m_color;
	int		m_font;
	bool	m_linefeed;
	char*   m_pCurrChar;
};

//--------------------------------------------------------------------------------------------------------------------------------------
///	¹®ÀÚ¿­ ÆÄ½Ì Class
///		1. Áö¿ø TAG : 
///			- {BR}	: ÁÙ³Ñ±è
///			- {B}	: º¼µå		* ÁÖÀÇ : Å¬¶óÀÌ¾ðÆ®¿¡¼­ º¼µå ÆùÆ®°¡ ÀÏ¹ÝÆùÆ® ÀÎµ¦½º+3ÀÌ¶ó´Â °¡Á¤ÇÏ¿¡ ±¸ÇöµÇ¾î ÀÖ´Ù
///			- {FC=?}: ±ÛÀÚ»öº¯°æ
///	
///		2. ÀÚµ¿ÁÙ³Ñ±è
///			- ´Ü¾îº°(È­ÀÌÆ® ½ºÆäÀÌ½º »çÀÌÀÇ ¹®ÀÚ¿­·Î °¡Á¤)
///			- ±ÛÀÚº°
///
///		- ¿¬°üclass			CJString
///
/// @Author				ÃÖÁ¾Áø
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
	CJString	GetJString( int index ); // ����� �����ͷ� �ѱ�°� �ƴϴ� _CrtlValidHeapPoint ���?
	int			GetStringCount();													
	void		Clear();

	void		SetDefaultFont( int iFont );									/// Default ÆùÆ® Set
	void		SetDefaultColor( DWORD dwColor );								/// Default ÆùÆ® »ö»ó Set
	void		SetSplitType( int type );										/// ÀÚµ¿ ÁÙ³Ñ±è Å¸ÀÔÀ» Set
	void		Push_Back(CJString JString);
	
	enum{																		/// ÀÚµ¿ ÁÙ³Ñ±è Å¸ÀÔ
		SPLIT_LETTER,															/// ¹®ÀÚº°(Default)
		SPLIT_WORD,																/// ´Ü¾îº°
	};
protected:
	std::vector< CJString >	m_Strings;											/// 
	int			m_iDefaultFont;													/// Default ÆùÆ®
	int			m_split_type;													/// ÁÙ³Ñ±è Å¸ÀÔ
	DWORD		m_dwDefaultColor;												/// Default ÆùÆ® »ö»ó


	/// ÀÌÇÏ Tag¸¦ ÆÄ½ÌÇÏ±â À§ÇÑ Methodµé
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