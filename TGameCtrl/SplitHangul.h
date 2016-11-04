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
#include "TGameCtrl.h"
#include <string>
#include <vector>

using namespace std;

class TGAMECTRL_API CSplitHangul
{
	vector< string >	m_StringList;	
	std::string			m_strNull;
public:
	CSplitHangul();
	CSplitHangul( char* Str, int iLength ,unsigned uiCodePage );

	~CSplitHangul();
	
	int	GetLineCount();
	const char* GetString( int iIndex );

	bool DoSplit( char* pStr, int iLength ,unsigned uiCodePage);

	// �ѱ��� �־��� ũ��� �ڸ���..
	bool SplitHangul( char* pStr, int iLength ,unsigned uiCodePage);
	void Clear();
};

#endif //_SPLITHANGUL_