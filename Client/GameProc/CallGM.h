#ifndef _CALLGM_
#define _CALLGM_

#include <string>

///
/// ������ GM ȣ�� ��û�� ������ ����( �� ������ �Խ��� )
///

class CCallGM
{
public:
	CCallGM(void);
	~CCallGM(void);

	void		CallGM( std::string& strServerName, std::string& strChannelName, std::string& strMsg  );
};

#endif //_CALLGM_
