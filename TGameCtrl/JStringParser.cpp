#include "StdAfx.h"
#include ".\jstringparser.h"
#include "TControlMgr.h"
#include "crtdbg.h"
#include "ITFont.h"

const D3DCOLOR c_TextColor[19] = 
	{
		D3DCOLOR_ARGB( 255,   0,     0,    0),//		������
		D3DCOLOR_ARGB( 255, 0x80,    0,    0),//���� ������
		D3DCOLOR_ARGB( 255,   0,  0x80,    0),//���� �ʷϻ�
		D3DCOLOR_ARGB( 255,   0,     0, 0x80),//���� �Ķ���
		D3DCOLOR_ARGB( 255, 0x80, 0x80,    0),//���� �����
		D3DCOLOR_ARGB( 255, 0x80,    0, 0x80),//���� �а���
		D3DCOLOR_ARGB( 255,    0, 0x80, 0x80),//���� �ϴû�
		D3DCOLOR_ARGB( 255, 0x80, 0x80, 0x80),//���� ȸ��
		D3DCOLOR_ARGB( 255, 0xC0, 0xC0, 0xC0),//���� ȸ��
		D3DCOLOR_ARGB( 255, 0xC0, 0xDC, 0xC0),//�ʷϻ�
		D3DCOLOR_ARGB( 255, 0xC0, 0xC0, 0xDC),//���λ�
		D3DCOLOR_ARGB( 255, 0xA6, 0xCA, 0xF0),//���� �Ķ���
		D3DCOLOR_ARGB( 255, 0xFF,    0,    0),//������
		D3DCOLOR_ARGB( 255,	   0, 0xFF,    0),//���
		D3DCOLOR_ARGB( 255,    0,    0, 0xFF),//�Ķ���
		D3DCOLOR_ARGB( 255, 0xFF, 0xFF,    0),//�����
		D3DCOLOR_ARGB( 255,    0, 0xFF, 0xFF),//�ϴû�
		D3DCOLOR_ARGB( 255, 0xFF, 0xFB, 0xF0),//ũ����
		D3DCOLOR_ARGB( 255, 0xFF, 0xFF, 0xFF)//�Ͼ��
	};

CJStringParser::CJStringParser(void)
{
	m_split_type	= SPLIT_LETTER;
	m_iDefaultFont  = 0;
	m_dwDefaultColor = D3DCOLOR_ARGB( 255, 0xFF, 0xFF, 0xFF);
}

CJStringParser::~CJStringParser(void)
{

}

CJString* CJStringParser::GetString( int index )
{
	_ASSERT( index >= 0 && index < (int)m_Strings.size() );
	return &m_Strings[index];
}

int CJStringParser::GetStringCount()
{
	return m_Strings.size();
}

int CJStringParser::SetString( const char* pszString ,int iDisplayWidth)
{
	_ASSERT( pszString );
	if( pszString == NULL ) return 0;


	Clear();


	DWORD dwTextColor   = m_dwDefaultColor;


	CJString String;
	int iStringWidth				= 0;
	int iStringWidthOnelinePrint	= 0;
	TagInfo	tag_info;
	tag_info.m_linefeed		= false;
	tag_info.m_color		= m_dwDefaultColor;
	tag_info.m_font			= m_iDefaultFont;
	tag_info.m_pCurrChar	= (char*)pszString;

	SIZE size = CTControlMgr::GetInstance()->GetFontMgr()->GetFontTextExtent( tag_info.m_font, pszString );

	char* pCurrChar = tag_info.m_pCurrChar;
	char* pNextChar = NULL;

//  CLocalizing::GetSingleton().GetCurrentCodePageNO()
	if( m_split_type == SPLIT_WORD )
		pNextChar = GetNextWord( CP_ACP, pCurrChar, 0 );
	else
		pNextChar = CharNextExA( CP_ACP, pCurrChar, 0 );

	std::string strTemp,strTempInner;
	
	while( pNextChar != pCurrChar )
	{
		if( pCurrChar[0] == '{' )
		{
			tag_info.m_pCurrChar = pCurrChar;

			Tag( tag_info );

			if( tag_info.m_linefeed )
			{
				if( !String.IsEmpty() )
					m_Strings.push_back( String );	
			
				String.Clear();
				iStringWidth = 0;
				tag_info.m_linefeed = false;
			}
			pCurrChar = tag_info.m_pCurrChar;//����Ǿ����� �ִ�.
		}
		else
		{
			strTemp.clear();
			for( ; pCurrChar < pNextChar; ++pCurrChar )
				strTemp.push_back( *pCurrChar );

			size = CTControlMgr::GetInstance()->GetFontMgr()->GetFontTextExtent( tag_info.m_font, strTemp.c_str() );
		
			///�� �ܾ�( �����̽� ������ ���ڿ�)�� ���̰� �ִ� ���� ������� 
			if( m_split_type == SPLIT_WORD && size.cx > iDisplayWidth )
			{
				tag_info.m_pCurrChar = pCurrChar;///�̸� ó������ �����͸� �����Ѵ�.
				pCurrChar = (char*)strTemp.c_str();
				pNextChar = CharNextExA( CP_ACP, pCurrChar, 0);
				char* pTempChar = pCurrChar;

				while( pNextChar != pCurrChar )
				{
					strTempInner.clear();					
					for( ; pCurrChar < pNextChar; ++pCurrChar )
						strTempInner.push_back( *pCurrChar );

					size = CTControlMgr::GetInstance()->GetFontMgr()->GetFontTextExtent( tag_info.m_font, strTempInner.c_str() );

					iStringWidth				+= size.cx;
					iStringWidthOnelinePrint	+= size.cx;
					
					if( iStringWidth  > iDisplayWidth )
					{
						m_Strings.push_back( String );
						String.Clear();
						iStringWidth	= 0;
						pCurrChar		= pTempChar;
					}
					else
					{
						String.AddString( strTempInner.c_str(), size , tag_info.m_font, tag_info.m_color );
						pTempChar = pCurrChar;
					}
					pNextChar = CharNextExA( CP_ACP, pCurrChar, 0);
				}
				///���⼭ ó���� �ȵ� String�� ��� ������ ó���� �ȴ�.
				pCurrChar = tag_info.m_pCurrChar;///�̸� �����صξ��� �����͸� �ٽ� �����Ѵ�.
			}
			else
			{
				iStringWidth				+= size.cx;
				iStringWidthOnelinePrint	+= size.cx;

				if( iStringWidth  > iDisplayWidth )
				{
					m_Strings.push_back( String );
					String.Clear();
					iStringWidth	= 0;
					pCurrChar		= tag_info.m_pCurrChar;
				}
				else
				{
					String.AddString( strTemp.c_str(), size , tag_info.m_font, tag_info.m_color );
					tag_info.m_pCurrChar = pCurrChar;
				}
			}
		}

		if( m_split_type == SPLIT_WORD )
			pNextChar = GetNextWord( CP_ACP, pCurrChar, 0 );
		else
			pNextChar = CharNextExA( CP_ACP, pCurrChar, 0 );
	}

	if( !String.IsEmpty() )
		m_Strings.push_back( String );///String�� ���̰� �ִ� ǥ�ñ��̸� �Ѵ��� üũ�Ѵ�.			

	return iStringWidthOnelinePrint;
}
///"<"���ڷ� Tag�� ���۵Ǿ���.
///">"���ڰ� �����ų� ���ڿ��� ������ �����Ѵ�.
void  CJStringParser::Tag( TagInfo& tag_info )
{
	std::string strTemp;
	int iState = TAG;//0.Tag
	char* pCurrChar = tag_info.m_pCurrChar;
	int iLen = strlen( pCurrChar);

	for( int i = 0; i < iLen; ++i )
	{
		if( pCurrChar[i] == ' ' )
			continue;
	
		switch( iState )
		{
		case TAG:
			if( pCurrChar[i+1] == '/' )
				iState = END_TAG;
			else
				iState = START_TAG;
			break;
		case START_TAG:
			{
				std::string strTemp;
				for( int j = i; j < iLen; ++j )
				{
					if( pCurrChar[j] == '}' )
					{
						TagStartProcess( strTemp, tag_info );
						tag_info.m_pCurrChar += j+1;
						return;
					}
					else
					{
						strTemp.push_back( pCurrChar[j] );
					}
				}
				tag_info.m_pCurrChar += i;
				return;
			}
			break;
		case END_TAG:
			{
				std::string strTemp;
				for( int j = i; j < iLen; ++j )
				{
					if( pCurrChar[j] == '}' )
					{
						TagEndProcess( strTemp, tag_info );
						tag_info.m_pCurrChar += j+1;
						return;
					}
					else
					{
						strTemp.push_back( pCurrChar[j] );
					}
				}
				tag_info.m_pCurrChar += i;
				return;
			}
			break;
		default:
			break;
		}
	}
	return;
}
void CJStringParser::Clear()
{
	m_Strings.clear();
}

void CJStringParser::SetDefaultFont( int iFont )
{
	_ASSERT( iFont >= 0 );
	if( iFont < 0 )
		iFont = 0;

	m_iDefaultFont = iFont;
}

void CJStringParser::SetDefaultColor( DWORD dwColor )
{
	m_dwDefaultColor = dwColor;
}

void CJStringParser::TagStartProcess( std::string& Tag,  TagInfo& tag_info )
{
	std::string strTag = strupr( (char*)Tag.c_str() );
	std::string::size_type index;

	index = strTag.find("FC" );
	if( index != std::string::npos )///found "FC"
	{
		index = strTag.find("=", index );
		if( index != std::string::npos )///found "="
		{
			std::string strColor = strTag.substr( index+1, Tag.length() - (index+1) );
			
			int iColor = atoi( strColor.c_str() );
			if( iColor >= 0 && iColor < ( sizeof(c_TextColor) / sizeof(c_TextColor[0])) )
				tag_info.m_color = c_TextColor[ iColor ];
			else
				tag_info.m_color = m_dwDefaultColor;
		}
		return;
	}

	index = strTag.find( "BR" );
	if( index != std::string::npos )
	{
		tag_info.m_linefeed = true;
		return;
	}

	index = strTag.find("B");
	if( index != std::string::npos )
	{
		tag_info.m_font = m_iDefaultFont + 3;
		_ASSERT( tag_info.m_font >= 0 );
		if( tag_info.m_font < 0 )
			tag_info.m_font = 0;
	}
}

void CJStringParser::TagEndProcess( std::string& Tag, TagInfo& tag_info )
{
	std::string strTag = strupr( (char*)Tag.c_str() );

	std::string::size_type index;

	index = strTag.find( "FC" );
	if( index != std::string::npos )//found
	{
		tag_info.m_color = m_dwDefaultColor;
		return;
	}
		
	index = strTag.find("B");
	if( index != std::string::npos )
		tag_info.m_font = m_iDefaultFont;

}

///���� ��ġ���� ���� �ܾ��� ù��° ��ġ�� ã�´�.
LPSTR CJStringParser::GetNextWord( WORD CodePage,LPSTR pCurrChar, DWORD dwFlags )
{
	LPSTR pNextChar = CharNextExA( CodePage, pCurrChar, dwFlags );

	while( pNextChar != pCurrChar )
	{
		if( pNextChar[0] == '{' )
			return pNextChar;

		if( pNextChar[0] == ' ' )
		{
			pCurrChar = pNextChar;
			pNextChar = CharNextExA( CodePage, pCurrChar, dwFlags );

			while( pNextChar != pCurrChar )
			{
				if( pNextChar[0] != ' ' )
					return pNextChar;

				pCurrChar = pNextChar;
				pNextChar = CharNextExA( CodePage, pCurrChar, dwFlags );
			}
			return pNextChar;
		}
		pCurrChar = pNextChar;
		pNextChar = CharNextExA( CodePage, pCurrChar, dwFlags );
	}
	return pNextChar;
}

void CJStringParser::SetSplitType( int type )
{
	m_split_type = type;
}
