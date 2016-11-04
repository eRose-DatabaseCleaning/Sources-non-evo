#ifndef _CChatRoom_
#define _CChatRoom_

/**
* Ŀ�´�Ƽ������ ä�÷��� ��� Ŭ����
*
* @Author		������
* @Date			2005/9/12
*/
class CChatMember
{
public:
	CChatMember(){}
	CChatMember(WORD wServerIdx, const char* pszName );
	~CChatMember(void){}

	void SetServerIdx( WORD wServerIdx );
	void SetName( const char* pszName );

	WORD GetServerIdx();
	const char* GetName();
protected:
	WORD		m_wServerIdx;
	std::string	m_strName;
};

#include "../GameCommon/CObservable.h"
#include "Event/CTEventChatRoom.h"
/**
* Ŀ�´�Ƽ������ ä�÷뿡 ���� Data Class
*	- Observer : CChatRoomDlg
*
* @Author		������
* @Date			2005/9/12
*/
class CChatRoom : public CObservable
{
	CChatRoom(void);
	~CChatRoom(void);
public:
	static CChatRoom& GetInstance();

	enum{
		STATE_DEACTIVATED,		/// �ƹ� ���� �ƴ�. ui�� Open�Ҽ� ������ Open�Ǿ� �ִٸ� �ݴ´�.
		//STATE_MAKE,				/// ��ȭ���� �����ϱ� ���ؼ� â�� �ٿ� ���� - �漳���� �����ϰ� Ȯ�ν� ������ ������ ����Ѵ�.(���º���)
		STATE_WAIT_MAKE,		/// ��ȭ���� ����⸦ �������� ��û�ϰ� ������,��� �������̽��� ��ܾ� �Ѵ�.
		STATE_WAIT_JOIN,
		STATE_ACTIVATED			/// ��ȭ���� ����/���εǰ� ��ȭ���� ����,, �����ϰ�쿡�� ���������ϴ�.
	};

	int	GetState();
	void	SetState( int iState );

	void	JoinMember( WORD wServerIdx, const char* pszName );
	void	LeaveMember( WORD wServerIdx );
	void    LeaveMember( const char* pszName );

	void	Join();	///������ ä�ù濡 �����Ѵ�.
	void	Made( WORD wMyID ); ///���� ����� ��û�� ���� ���������.
	void	Leave();///ä�ù濡�� ������.
	void	SetTitle( const char* pszTitle );
	void	SendReqMakeRoom( BYTE btRoomType, BYTE btMaxUser, char* pszTitle, char* pszPwd );
	void	SendReqJoinRoom( BYTE btRoomType, WORD wRoomID, char* pszPwd );
	void	SetMaster( WORD wServerIdx );

	const char* GetTitle();
private:

	std::list< CChatMember >	m_Members;
	CTEventChatRoom	m_Event;

	int			m_iState;
	int			m_iMasterServerIdx;
	std::string	m_strTitle;
};
#endif