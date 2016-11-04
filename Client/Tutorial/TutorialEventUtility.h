#ifndef _TUTORIAL_EVENT_UTILITY_
#define _TUTORIAL_EVENT_UTILITY_

#include "../Common/IO_STB.h"
#include "../Util/JSingleton.h"


//---------------------------------------------------------------------------------------
///
/// class CTutorialEventUtility
///
///	�̺�Ʈ ���࿡ �ʿ��� ��ƿ��Ƽ Ŭ�����Դϴ�.
///
//---------------------------------------------------------------------------------------

class CTutorialEventUtility : public CJSingleton< CTutorialEventUtility >
{
private:
	STBDATA					m_TblNotifyButtonEvent;	/// �˸���ư �̺�Ʈ

public:
	CTutorialEventUtility(void);
	~CTutorialEventUtility(void);

	bool			Init();
	void			Release();

	/// �˸� ��ư �̺�Ʈ�� �����Ѵ�.
	bool			CreateNotifyEventButton( int iEventIdx );
	/// EventButton.STB�� ������ �˸� ��ư �̺�Ʈ�� �����Ѵ�.
	bool			ExecNotifyEvent( int iEventIdx );
};

#endif //_TUTORIAL_EVENT_UTILITY_