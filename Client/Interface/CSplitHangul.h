/////////////////////////////////////////////////////////////////////////////////////////////
//
//	�־��� ��Ʈ���� �־��� ������ ��Ʈ������ �����ϴ� Ŭ����
//	�ѱ��� ��� ��Ȯ�� ������ �ʿ��ϴ�..
//
//	���� : CSplitHangul( char* Str, int iLength )�� �����ؼ� ����ϰų�..
//			 ���� ������.. DoSplit( char* Str, int iLength ); ���..
//			  �и��Ŀ��� GetLineCount �� ���� ������ ���..
//			  �� ���ο� ���� ��Ʈ���� GetString( Index ) �� ��´�.
//
//	������ �ϼ̰ų�.. ���׸� �߰��Ͻø�.. jeddli ���� �ֱ����ּ���...
//
//		2002. 9. 13		jeddli
//
/////////////////////////////////////////////////////////////////////////////////////////////




#ifndef _SPLITHANGUL_
#define _SPLITHANGUL_

#include <string>
#include <vector>

using namespace std;

// ������ ��Ʈ���� �ѱ��� ©�� �����ΰ� �Ǻ�..( IsDBCSLeadByte ������ �Һи���.. )
inline BOOL IsDBCSTrailByte(char *base, char *p)
 {
        int lbc = 0; // lead byte count

        while (p > base)
        {
                if (!IsDBCSLeadByte(*(--p)))
                        break;
                lbc++;  
        }

        return (lbc & 1);
}


#define MAX_BUF_SIZE	1024
#define MAX_STACK_DEPTH	50

class CSplitHangul
{
	vector< string >	m_StringList;	
	char				m_TempBuf[ MAX_BUF_SIZE ];

	int					m_iStackDepth;

public:
	CSplitHangul();
	CSplitHangul( char* Str, int iLength );

	~CSplitHangul();
	

	int	GetLineCount( ){ return (int)m_StringList.size(); };
	const char* GetString( int iIndex )
	{
		if( iIndex >= m_StringList.size() )
			return NULL;

		return m_StringList[ iIndex ].c_str();	
	}


	bool DoSplit( char* Str, int iLength );

	// �ѱ��� �־��� ũ��� �ڸ���..
	bool SplitHangul( char* Str, int iLength );
	void Clear();
};




#endif //_SPLITHANGUL_