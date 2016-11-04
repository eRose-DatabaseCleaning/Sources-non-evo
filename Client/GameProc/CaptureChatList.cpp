#include "stdafx.h"
#include ".\capturechatlist.h"
#include "../Interface/it_MGR.h"
#include "../Interface/DLGs/ChattingDLG.h"


CCaptureChatList::CCaptureChatList(void)
{
	m_Fp = NULL;
}

CCaptureChatList::~CCaptureChatList(void)
{
	CloseCapturedFile();
}

const char* strCapturedChatFile = "ChatCapture.txt";
/// ChatDLG �κ��� �� ����Ʈ�� ���ͼ� �����Ŀ� �����Ѵ�.
bool CCaptureChatList::DoCaptureChatList()
{
	CChatDLG* pChatDLG = g_itMGR.GetChatDLG();
	if( pChatDLG )
	{
		int iLineCount = pChatDLG->GetLineCount();

		/// ������ ������ ����.
		OpenCapturedFile( strCapturedChatFile );

		for( int i = 0; i < iLineCount; i++ )
		{
			const char* strText = pChatDLG->GetLineString( i );
			if( strText )
			{
				char* strOutput = EncodingString( strText );
				if( strOutput )
				{
					WriteStringToFile( strOutput );					
				}
			}
		}

		/// ������ ������ �ݴ´�.
		CloseCapturedFile();
	}

	return true;
}

char* CCaptureChatList::EncodingString( const char* strInput )
{
	return (char*)strInput;
}

//--------------------------------------------------------------------------
/// ������ ������ ����.
//--------------------------------------------------------------------------
bool CCaptureChatList::OpenCapturedFile( const char* strFileName )
{
	if( m_Fp )
	{
		CloseCapturedFile();
	}

	m_Fp = fopen( strFileName, "wt" );
	if( m_Fp )
		return true;

	return false;
}

//--------------------------------------------------------------------------
/// ������ ������ �ݴ´�.
//--------------------------------------------------------------------------
void CCaptureChatList::CloseCapturedFile()
{
	if( m_Fp )
	{
		fclose( m_Fp );
		m_Fp = NULL;
	}
}

//--------------------------------------------------------------------------
/// ��Ʈ���� ���Ͽ� ����.
//--------------------------------------------------------------------------
void CCaptureChatList::WriteStringToFile( char* strInput )
{
	if( m_Fp )
	{
		fwrite( strInput, strlen( strInput ) + 1, 1, m_Fp );
		WORD wCRLF = 0x0A;
		fwrite( &wCRLF, sizeof( WORD ), 1, m_Fp );		
	}else
	{
		assert( 0 && "FILE is invalid" );
	}
}