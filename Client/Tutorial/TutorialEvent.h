#ifndef _TUTORIAL_EVENT_
#define _TUTORIAL_EVENT_

#include "../Common/IO_STB.h"


//-----------------------------------------------------------------------------------
///
/// CTutorialEvent
/// Ʃ�丮�� ������ ���� �̺�Ʈ ó�� Ŭ����
///
//-----------------------------------------------------------------------------------

class CTutorialEvent
{
public:
	CTutorialEvent(void);
	~CTutorialEvent(void);

	bool			Init();
	void			Release();

	void			CheckEvent();
};


#endif _TUTORIAL_EVENT_