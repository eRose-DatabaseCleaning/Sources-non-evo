#ifndef _TCOMMANDQ_
#define _TCOMMANDQ_
#include "TGameCtrl.h"
#include <list>
using namespace std;

class CTCommand;

/**
* CTCommand�� ������ Container Class
*		- ����(2005/8/30) TDialog�� ������ �ִ�
*
* @Author	������
*
* @Date		2005/8/30
*/
class TGAMECTRL_API CTCommandQ
{
public:
	CTCommandQ(void);
	virtual ~CTCommandQ(void);

	void		Push_Back( CTCommand* pCmd);
	void		Push_Front( CTCommand* pCmd);
	CTCommand*	GetCommand();
	void		Clear();

protected:
	list<CTCommand*>	m_listCommand;
};
#endif