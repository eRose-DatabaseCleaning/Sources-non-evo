#ifndef _TARGET_MANAGER_
#define _TARGET_MANAGER_

#include "../Util/JSingleton.h"


class CTargetManager  : public CJSingleton< CTargetManager >
{
private:
	int					m_iCurrentMouseTargetObject;			/// ���콺 �̵��� ���� ���콺�� ����Ű�� ������Ʈ	

public:
	CTargetManager(void);
	~CTargetManager(void);


	void		SetMouseTargetObject( int iObjectIDX );


	void		Proc();
	void		Draw();
};


#endif //_TARGET_MANAGER_