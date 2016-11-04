#ifndef _CMAKESTATE_
#define _CMAKESTATE_


class CMakeDLG;
/**
* �����������̽� â�� State Base Class : 
*
* @Author		������
* @Date			2005/9/15
*/
class CMakeState
{
public:
	CMakeState(void);
	~CMakeState(void);

	virtual void Draw() = 0;
	virtual void Update( POINT ptMouse) = 0;
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual void Init() = 0;
	virtual void MoveWindow(POINT ptPos ) = 0;
	virtual bool IsModal() = 0;
	virtual unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam ) = 0;

protected:
	CMakeDLG*	m_pParent;

};
#endif