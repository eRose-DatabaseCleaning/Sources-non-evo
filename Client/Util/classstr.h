/*
	$Header: /7HeartsOnline/Client/Util/classSTR.h 4     04-09-13 12:32a Icarus $
*/
#ifndef	__CLASSSTR_H
#define	__CLASSSTR_H
#ifndef	_FILE_DEFINED
#include <stdio.h>
#endif


//-------------------------------------------------------------------------------------------------

#define	CSTRING_BUFFER_CNT	20
#define	CSTRING_BUFFER_LEN	256	// MAX_PATH = 260


class CStr {
private :
	static bool				 m_bInit; 
	static unsigned short    m_wStaticStrLen;
	static char				*m_pStaticStr;

	static char	  **m_ppStr;	// [	CSTRING_BUFFER_CNT ][ CSTRING_BUFFER_LEN ]; 
	static short	m_nStrBuffLEN;
	static short	m_nStrBuffCNT;
	static short	m_nStrIdx;

public  :
	static void  Init (short nStrBufferCNT=CSTRING_BUFFER_CNT, short nStrBufferLEN=CSTRING_BUFFER_LEN);
	static void  Free (void);

	static void  SetBufferLength (short nStrBufferLEN);
	static void  SetBufferCount (short nStrBufferCNT);

	static short GetBufferCount ()	{	return m_nStrBuffCNT;	}
	static short GetBufferLength ()	{	return m_nStrBuffLEN;	}

	static char *GetString ();
	static char *Printf (char *fmt, ...);
	static void  DebugString (char *fmt, ...);

	static int   ParamCount (char *pStr);
	static char *ParamStr (char *pStr, int iIndex);

	// ���ڿ� �Ľ�.
	// pStr :: �Ľ��� ���ڿ�
	// pDelimiters :: ��ū�� ������ ���ڵ��� �迭, ex: char *pDelimiters = " .:,\t\n";
	static char *GetTokenFirst (char *pStr, char *pDelimiters);
	static char *GetTokenNext  (char *pDelimiters);

	static char *ReadString ( FILE* fp, bool bIgnoreWhiteSpace=true);
	static bool  SaveString (char *szFileName, char *szString);
	static unsigned int GetHASH(char const *szString);

	static short StrLen( const char * str, short nMaxStrLEN )
	{
		const char *eos = str;
		short nLen=0;
		while( *eos++ && ++nLen <= nMaxStrLEN ) ;
		return nLen;
	}
} ;
inline char *CStr::GetString ()
{
#ifdef	_DEBUG
	if ( !m_bInit ) {
		::MessageBox (NULL, "must call CStr::Init () !!!", "Error", MB_OK);
		return NULL;
	}
#endif
	if ( m_nStrIdx >= CSTRING_BUFFER_CNT ) 
		m_nStrIdx = 0;

	m_ppStr[ m_nStrIdx ][ 0 ] = 0;
	return m_ppStr[ m_nStrIdx++ ];
}


//-------------------------------------------------------------------------------------------------
class CStrVAR {
private :
	char   *m_pStaticStr;
	WORD	m_wStaticStrLen;
	char   *m_pStr;
	WORD	m_wStrLen;

public  :
	CStrVAR(WORD wStrLen=0, WORD wStaticStrLen=0);
	~CStrVAR()						{	this->Del();					}

	char *Alloc (WORD wStrLen, WORD wStaticStrLen=0);
	void  Set (char *szStrIn);
	char *Get (void)				{	return m_pStr;					}
	void  Del (void);
	WORD  Length (void)			{	return m_wStrLen;				}
	WORD  BuffLength (void)			{	return m_wStrLen;				}

	unsigned int GetHASH (void)		{	return this->GetHASH( m_pStr );	}

	char *Printf (char *fmt, ...);
	char *GetTokenFirst (char *pStr, char *pDelimiters);
	char *GetTokenNext  (char *pDelimiters);

	char *ReadString (FILE *fp, bool bIgnoreWhiteSpace=true);
	bool  SaveString (char *szFileName, char *szString);

public :
	static unsigned int GetHASH (char *szStr);
} ;

//-------------------------------------------------------------------------------------------------
#endif