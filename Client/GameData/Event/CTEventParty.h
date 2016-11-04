#ifndef _CTEventParty_
#define _CTEventParty_
#include "ctevent.h"
//*---------------------------------------------------------//
/// ��Ƽ���� ������ �˸��� Event( CParty, CPartyDlg���� ���)
//*---------------------------------------------------------//
class CTEventParty : public CTEvent
{
public:
	CTEventParty(void);
	virtual ~CTEventParty(void);
	enum{
		EID_NONE,
		EID_JOIN_MEMBER,				/// ����Ʈ�� �߰�
		EID_LEAVE_MEMBER,				/// ����Ʈ���� ����
		EID_CHANGE_LEADER,				/// ���� ��ũǥ�� ����, Entrust,Ban Button Enable/Disable
		EID_CHANGE_OBJECT_SERVERINDEX,	/// OBJECT SERVER INDEX ����
		EID_BAN_MEMBER,					/// ���� �ƴ� �ٸ� ����� ��Ƽ���� �߹� ���������
		EID_CLEAR,						/// ��Ƽ ����
		EID_INIT,						/// ��Ƽ ����
		EID_MEMBER_DISCONNECTED,			/// ��Ƽ���� ����������
		EID_CHANGE_RULE,
	};
	void	SetObjSvrIdx( WORD iObjSvrIdx );
	WORD	GetObjSvrIdx();


	void	SetName( const char* pszName );
	const char* GetName();

	void	SetObjectTag( DWORD dwObjectTag );
	DWORD   GetObjectTag();

	void	SetRule( BYTE btRule );
	BYTE	GetRule();
protected:
	WORD				m_wObjSvrIdx;				///EID_CHANGE_OBJECT_SERVERINDEX�ÿ��� ����		ObjSvrIndx
	std::string			m_strName;
	DWORD				m_dwObjectTag;
	BYTE				m_btRule;
};
#endif