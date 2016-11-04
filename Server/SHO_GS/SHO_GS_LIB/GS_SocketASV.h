/**
 * \ingroup SHO_GS
 * \file	GS_SocketASV.h
 * \brief	���������� ����ϴ� ���� Ŭ����
 */
#ifndef	__GS_SOCKETASV_H
#define	__GS_SOCKETASV_H
#include "classSYNCOBJ.h"
#include "classTIME.h"
//-------------------------------------------------------------------------------------------------
class GSASV_Socket : public CClientSOCKET {
public :
	bool WndPROC ( WPARAM wParam, LPARAM lParam );
} ;

/**
 * \ingroup SHO_GS_LIB
 * \class	GS_asvSOCKET
 * \author	wookSang.Jo
 * \brief	���� �������� sho_as�� ��Ź� ó���ϴ� Ŭ����
 */
class GS_asvSOCKET	// : private CCriticalSection
{
public :
	GSASV_Socket	 m_SockASV;
	
private:
	CTimer			*m_pReconnectTimer;
	bool			 m_bTryCONN;

	t_PACKET	*m_pRecvPket;
//	t_PACKET	*m_pSendPket;

	HWND		m_hMainWND;
	UINT		m_uiSocketMSG;
	CStrVAR		m_AccountServerIP;
	int			m_AccountServerPORT;

public :
	GS_asvSOCKET ();
	~GS_asvSOCKET ();

	void Init (HWND hWND, char *szAccountServerIP, int iAccountServerPort, UINT uiSocketMSG);

	bool Connect ();
	void Disconnect ();

	bool Send_cli_ALIVE ( char *szAccount );

	bool Send_zas_SERVER_TYPE ( BYTE btType );
	bool Send_zws_ADD_ACCOUNT ( char *szAccount, char *szMD5Pass, char *szIP );
	void Send_zas_SUB_ACCOUNT ( char *szAccount, BYTE btWhy, short nLevel, short nZoneNo );
	void Send_zas_CHECK_ACCOUNT ( BYTE btType, char *szAccount );

	bool Recv_zas_CHECK_ACCOUNT ();
	bool Recv_zas_KICK_ACCOUNT ();
	bool Recv_zas_MESSAGE ();
	bool Recv_zas_MESSAGE_EXT ();

	bool Proc_SocketMSG (WPARAM wParam, LPARAM lParam);
} ;
extern GS_asvSOCKET	*g_pSockASV;

//-------------------------------------------------------------------------------------------------
#endif
