#ifndef _CTSplitString_
#define _CTSplitString_
#include "TGameCtrl.h"
#include <vector>
#include <string>
using namespace std;

/**
*	CSplitHangul�� �����ϰ� ���������� ���ڰ����� �ƴ� ǥ�õ� �ʺ� ���� ��Ʈ���� �ڸ��� Class
*		- znzin�� ���� ǥ�� ���̸� query�ϱ� ������ ������ �ִ�. �����Ӹ��� �������� �ʵ��� �����Ұ�
*
* @Author		������
* @Date			2005/8/30
*/

class TGAMECTRL_API CTSplitString
{
	vector< string >	m_StringList;	
	std::string		m_strNull;
	SIZE m_szStrSize;	

public:
	CTSplitString(void);
	CTSplitString( int iFont, char* pszStr, int iDisplayWidth ,unsigned uiCodePage );
	~CTSplitString(void);

	int	GetLineCount();
	const char* GetString( int iIndex );

	bool Split( int iFont, char* pszStr, int iDisplayWidth ,unsigned uiCodePage);
	void Clear();
	
	SIZE & GetSizeText( int iFont, const char * szStr );

};




#endif