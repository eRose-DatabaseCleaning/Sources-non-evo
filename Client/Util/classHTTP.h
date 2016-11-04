#ifndef	__CLASS_HTTPDOWN_H
#define	__CLASS_HTTPDOWN_H
#include <winINET.h>
//-------------------------------------------------------------------------------------------------

#define	MAX_READ_BUFF_SIZE	( 1024*16 )	// 16k  ���� 8K

class classHTTP 
{
protected:
	HINTERNET	m_hInternet;
	HINTERNET	m_hURL;

	DWORD		m_dwStatusCODE;

public   :
	classHTTP ();
	~classHTTP ();

			bool Open ();
			void Close ();
	virtual bool OpenURL (char *szFileURL);
			void CloseURL ();

	static  char*StatusCodeString (DWORD dwCode);
} ;

//-------------------------------------------------------------------------------------------------

class classHTTPDOWN : public classHTTP
{
private :
	DWORD		m_dwTotalSIZE;
	DWORD		m_dwTotalRECV;

	DWORD		m_dwREAD;
	BYTE		m_btBUFF[ MAX_READ_BUFF_SIZE ];

public  :
	classHTTPDOWN ();
	~classHTTPDOWN ();

	bool	OpenURL (char *szFileURL);

	DWORD	GetStatusCODE ()	{	return m_dwStatusCODE;	}
	DWORD	GetFileSIZE ()		{	return m_dwTotalSIZE;	}
	DWORD	GetRecvSIZE ()		{	return m_dwTotalRECV;	}

//	DWORD	ReadFILE ();
    DWORD   ReadFILE (void *pBUFF, DWORD dwBuffSIZE);
} ;

//-------------------------------------------------------------------------------------------------
#endif
