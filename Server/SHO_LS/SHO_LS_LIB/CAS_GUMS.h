#ifndef	__CAS_GUMS_H
#define	__CAS_GUMS_H
#include "CClientSOCKET.h"
#include "CSocketWND.h"
#include "classSYNCOBJ.h"
#include "classTIME.h"
//-------------------------------------------------------------------------------------------------

enum eGumsNATION {
	GUMS_KOREA = 0,
	GUMS_JAPAN
} ;



#define	GUMS001_REQ_INITIALIZED								1
#define	GUMS002_REQ_SERVER_KEEP_ALIVE						2
#define	GUMS010_REQ_LOGIN									10
#define	GUMS020_REQ_LOGOUT									20
#define	GUMS030_REQ_USER_KEEP_ALIVE							30
#define	GUMS040_REQ_REMAIN_TIME								40
#define	GUMS110_REQ_PRELOGIN_TYPE							110

// Response Message type 1 
#define	GUMS011_REPLY_LOGIN_SUCCESS							11
#define	GUMS111_REPLY_FREE_LOGIN_SUCCESS					111		// ����/������ �α��� ����

// Response Message type 1 - JAPAN
#define	GUMS1011_REPLY_LOGIN_SUCCESS						1011	// *
#define	GUMS1111_REPLY_FREE_LOGIN_SUCCESS					1111	// * ����/������ �α��� ����



// Response Message type 2
#define	GUMS012_REPLY_LOGIN_FAILED_INVALID_ID_OR_PW			12		// ������� ID�� PW�� Ʋ�����
#define	GUMS013_REPLY_LOGIN_FAILED_NO_RIGHT					13		// ������ ������ ���� �����
#define	GUMS014_REPLY_LOGIN_FAILED_TIME_EXPIRED				14		// ��� �Ⱓ ����

#define	GUMS018_REPLY_LOGIN_FAILED_ONE_DAY_EXPIRED			18		// ���� ��� �Ⱓ�� ����� �����
#define	GUMS019_REPLY_LOGIN_FAILED_ONE_DAY_EXPIRED2			19		// ���� ��� �Ⱓ�� ����� �����

#define	GUMS015_REPLY_LOGIN_FAILED_ALREADY_LOGIN			15		// �̹� �α��� �� ����ڶ�� �˷��ָ鼭 �α����� �ȵǴ°ű���..
#define	GUMS016_REPLY_LOGIN_FAILED_IP_FULL_USED				16		// ������ IP�� ������ ��� ������� ���(�׹�)
#define	GUMS116_REPLY_FREE_LOGIN_FAILED_IP_FULL_USED		116		// ������ IP�� ������ ��� ������� ���(�׹�)

#define	GUMS017_REPLY_LOGIN_FAILED_KICKED_USER				17		// ���� �α��� �Ǿ� �ִ� ����ڸ� �α׾ƿ� ���״ٰ� �˷��ְ� �α����� �ȵǴ� �̴ϴ�.

#define	GUMS021_REPLY_LOGOUT_SUCCESS						21		// ����� �α׾ƿ� ����
#define	GUMS022_REPLY_LOGOUT_FAILED							22		// ����� �α׾ƿ� ����

#define	GUMS088_REPLY_DO_INIT_FIRST							88		// ���ؼǿ� ���� �ʱ�ȭ ������ �Ǿ����� �ʾ� �������� �޽��� ó�� �Ұ�
#define	GUMS099_REPLY_INVALID_MSG_FORMAT					99		// �޼��� ������ Ʋ���� ���

#define	GUMS044_REPLY_NETWORK_IS_NOT_GOOD					44		// ��Ʈ�� ����Ȳ�� ���� �ʾ� �޽��� ó���� �Ұ���.

#define	GUMS100_REPLY_INIT_SUCCESS							100		// �ʱ�ȭ ����
#define	GUMS112_REPLY_FREE_SRV_ONLY							112		// �������� �α� ����...

#define	GUMS1112_REPLY_FREE_SRV_ONLY						1112	// ����/������ �α��� ����


#define	GUMS113_REPLY_LOGIN_FAILED							113		// ����/������ ��� �α� �Ұ�..

// Response Message type 3
#define	GUMS041_REPLY_EXPIRE_DAY							41		// �ش� ������� �������ڸ� �����ش�
#define	GUMS042_REPLY_EXPIRE_DAY2							42		// �ش� ������� ���� �ð��� �����ش�.

// Response Message type 3 - JAPAN
#define	GUMS1041_REPLY_EXPIRE_DAY							1041	// �ش� ������� �������ڸ� �����ش�
#define	GUMS1042_REPLY_EXPIRE_DAY2							1042	// �ش� ������� ���� �ð��� �����ش�.

// Callback Messages
#define	GUMS051_REPLY_DAY_EXPIRED_ALERT						51		// ���ð�(����) ���� ��� �޽���
#define	GUMS052_REPLY_ONE_DAY_TIME_OVER						52		// ���� ���ð� �ѵ� �ʰ� ��� �޼���
#define	GUMS053_REPLY_ONE_DAY_TIME_OVER2					53		// ���� ���ð� �ѵ� �ʰ� ��� �޼���
#define	GUMS054_REPLY_TIME_EXPIRED_ALERT					54		// ���ð�(����) ���� ��� �޼���

// Callback Messages - JAPAN
#define	GUMS1051_REPLY_DAY_EXPIRED_ALERT					1051	// * ���ð�(����) ���� ��� �޽���
#define	GUMS1052_REPLY_ONE_DAY_TIME_OVER					1052	// * ���� ���ð� �ѵ� �ʰ� ��� �޼���
#define	GUMS1053_REPLY_ONE_DAY_TIME_OVER2					1053	// * ���� ���ð� �ѵ� �ʰ� ��� �޼���
#define	GUMS1054_REPLY_TIME_EXPIRED_ALERT					1054	// * ���ð�(����) ���� ��� �޼���


#define	GUMS061_REPLY_DAY_EXPIRED_DIS						61		// ���Ⱓ(����) ���� ���� ���� �޼���
#define	GUMS062_REPLY_ONE_DAY_EXPIRED_DIS					62		// ���� ���ð� �ѵ� �ʰ� ���� ���� �޽���
#define	GUMS063_REPLY_ONE_DAY_EXPIRED_DIS2					63		// ���� �ѵ��ð� �ѵ� �ʰ� ���� ���� �޽���
#define	GUMS064_REPLY_DUP_USERID_DIS						64		// �ٸ� ������� �䱸�� ���� ����
#define	GUMS065_REPLY_USER_NOT_FOUND						65		// ����� Live ��ȣ�� ������ ����ڰ� �޸� ���� ���� ��� �����ִ� �޽���
#define	GUMS066_REPLY_TIME_EXPIRED_DIS						66		// ���ð�(����) ���� ���� ���� �޽���


#define	GUMS1061_REPLY_DAY_EXPIRED_DIS						1061	// * ���Ⱓ(����) ���� ���� ���� �޼���
#define	GUMS1062_REPLY_ONE_DAY_EXPIRED_DIS					1062	// * ���� ���ð� �ѵ� �ʰ� ���� ���� �޽���
#define	GUMS1063_REPLY_ONE_DAY_EXPIRED_DIS2					1063	// * ���� �ѵ��ð� �ѵ� �ʰ� ���� ���� �޽���
#define	GUMS1064_REPLY_DUP_USERID_DIS						1064	// * �ٸ� ������� �䱸�� ���� ����
#define	GUMS1065_REPLY_USER_NOT_FOUND						1065	// * ����� Live ��ȣ�� ������ ����ڰ� �޸� ���� ���� ��� �����ִ� �޽���
#define	GUMS1066_REPLY_TIME_EXPIRED_DIS						1066	// * ���ð�(����) ���� ���� ���� �޽���



class ASGUM_Socket : public CClientSOCKET {
public :
	bool WndPROC ( WPARAM wParam, LPARAM lParam );
} ;

class AS_gumSOCKET 
{
public :
	ASGUM_Socket	m_SockGUM;

private:
	CTimer		*m_pReconnectTimer;
	CTimer		*m_pSendAliveTimer;

	bool		 m_bTryCONN;
	bool		 m_bGumsINIT;

	t_PACKET	*m_pRecvPket;

	HWND		 m_hMainWND;
	UINT		 m_uiSocketMSG;
	CStrVAR		 m_GumServerIP;
	int			 m_GumServerPORT;

	// iGameID	:: GUMS�������� �����ϴ� Gamd ID	������ 1	Gum ���� 1�뿡 �������� ���� �Ҷ�...
	// �����...������ 1, �Ϸ� �����ð� ���� �ִ� ���ἷ�� 2 !!!
	// iCpID	:: ȸ�� ������ �и��� ��� ��ü ID	������ 1
//	int			 m_iGameID;
	int			 m_iCpID;

	long		 m_lGumsSqlNO;
	//char		 m_szGumHeader[ 32 ];
	//CCriticalSection	m_csHEAD;
	//	char		 m_szGumBody[ 512 ];
//	CCriticalSection	m_csBODY;

	DWORD	GetPlayingFLAG( eGumsNATION eNation, int iPayType );

	int		GetPayType( char *szPayType );
	DWORD	GetPayFlag( char *szPayType );

	eGumsNATION	Gums_NATION( int iMsgType )
	{
		if ( iMsgType > 1000 ) {
			return GUMS_JAPAN;
		}
		return GUMS_KOREA;
	}

	CCriticalSection	 m_csLOG;
	FILE				*m_fpLOG;
	void Log( bool bRecv, char *szLog );

	static AS_gumSOCKET *m_pInstance;

public :
	AS_gumSOCKET ();
	~AS_gumSOCKET ();

	static AS_gumSOCKET *GetInstance ()		{	return m_pInstance;		}

	void Init (HWND hWND, char *szGumServerIP, int iGumServerPort, UINT uiSocketMSG);

	bool Connect ();
	void Disconnect ();

	bool Send_GumsPACKET (int iMsgType, char *szString, int iSeqNO=0, bool bCheckInit=true);
	void Recv_GumsPACKET (char *szString, int iStrLen);
	bool Proc_SocketMSG (WPARAM wParam, LPARAM lParam);


	bool Send_IntializedMessage (char *szServerName, int iServerGroupNumber, int iServerNumber, int iZoneNumber, int iGumsKeepAlive, int iUserKeepAlive );
	bool Send_ServerKeepAliveMessage ();
	bool Send_UserKeepAliveMessage (int iGameID, char *szUserID);
	bool Send_RequestReaminTime (int iGameID, char *szUserID);

	bool Send_PreLogin (int iGameID, char *szUserID, char *szRealIP, char *szVirtualIP=NULL, int iUserTAG=0 );

	bool Send_RequestLOGIN (bool bKickAlreadyLogin, int iGameID, char *szUserID, char *szPassword, char *szRealIP, char *szVirtualIP=NULL);
	bool Send_RequestLOGOUT (int iGameID, char *szUserID);


	bool RecvGums_LoginSUC (eGumsNATION eNation, char *szAccount, char *szGumsPKET, char *szUserTAG, int iPayType);
	bool RecvGums_LogInFailedNoRIGHT (int iMsgTYPE, char *szAccount, char *szUserTAG);

	bool RecvGums_LoginOnlyFREE(int iMsgTYPE, char *szAccount, char *szUserTAG);
	bool RecvGums_LogInFailedOutOfIP (int iMsgTYPE, char *szAccount, char *szUserTAG);

#ifndef	__SHO_LS_LIB
	bool RecvGums_TimeOUT  (int iMsgTYPE, char *szAccount);

	bool RecvGums_PreLoginFAILED (int iMsgTYPE, char *szAccount);

	bool RecvGums_LogInFailedKickedUSER (int iMsgTYPE, char *szUserID);
	bool RecvGums_LogInFailedTimeEXPIRED (int iMsgTYPE, char *szAccount);
	bool RecvGums_LogInFailedOneDayEXPIRED (int iMsgTYPE, char *szAccount);
	bool RecvGums_LogInFailedAlreadyLOGIN (int iMsgTYPE, char *szAccount);
	bool RecvGums_LogInFailedTimeEXPIRED (int iMsgTYPE, char *szAccount, char *szString );

	bool RecvGums_TimeALERT(int iMsgTYPE, char *szAccount, char *szRemainMin);
	bool RecvGums_TimeALERT_EXT( int iMsgTYPE, char *szAccount, char *szRemainMin, DWORD dwWarnPlayFlag );

	bool RecvGums_TimeEXPIRED (int iMsgTYPE, char *szAccount, char *szString );
	bool RecvGums_TimeEXPIRED_EXT (int iMsgTYPE, char *szUserID, DWORD dwDelPlayFlag );

	bool RecvGums_DupUserID (int iMsgTYPE, char *szAccount);

	bool RecvGums_SetExpireDATEorTIME( int iMsgTYPE, char *szAccount, char *szMsg );
	bool RecvGums_SetExpireDATEorTIME_EXT( int iMsgTYPE, char *szAccount, char *szMsg );
#endif
} ;

//-------------------------------------------------------------------------------------------------
#endif