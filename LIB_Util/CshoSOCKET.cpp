
#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>
#include "CPacketCODEC.h"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
struct tPOINTF {
	float	x;
	float	y;
} ;

#define	__POINTF
#define	__SERVER
//#define	__SKIP_SRV_PROTOTYPE

#include "CshoSOCKET.h"

CshoClientSOCK::CshoClientSOCK ()
{
	m_hEncSOCK = (HANDLE)new CPacketCODEC;

	m_iSendSEQ = 0;
}
CshoClientSOCK::~CshoClientSOCK ()
{
	delete (CPacketCODEC*)m_hEncSOCK;
}

//-------------------------------------------------------------------------------------------------
void CshoClientSOCK::Set_NetSTATUS (BYTE btStatus)
{
	if ( NETWORK_STATUS_CONNECT == btStatus ) {
		// �ʱ�ȭ...
		m_iSendSEQ = 0;
	}

	CClientSOCKET::Set_NetSTATUS( btStatus );
}

void CshoClientSOCK::OnAccepted(int *pSendSEQ)
{
	m_iSendSEQ = pSendSEQ[ 0 ];
}

void CshoClientSOCK::mF_Init (DWORD dwInitCODE)
{	
//	CPacketCODEC::Instance ();
	if ( dwInitCODE )
		((CPacketCODEC*)m_hEncSOCK)->Init( dwInitCODE );
	else
		((CPacketCODEC*)m_hEncSOCK)->Default ();
}

WORD CshoClientSOCK::mF_ESP (t_PACKETHEADER *pPacket)
{
	// ������ ���� ��Ŷ ��ȣȭ....
	return ((CPacketCODEC*)m_hEncSOCK)->Encode_SendClientPACKET( pPacket, m_iSendSEQ );
}
WORD CshoClientSOCK::mF_DRH (t_PACKETHEADER *pPacket)
{
	// �������� ���� ��Ŷ��� ��ȣȭ...
	return ((CPacketCODEC*)m_hEncSOCK)->Decode_RecvServerHEADER( pPacket );
}
short CshoClientSOCK::mF_DRB (t_PACKETHEADER *pPacket)
{
	// �������� ���� ��Ŷ��ü ��ȣȭ...
	return ((CPacketCODEC*)m_hEncSOCK)->Decode_RecvServerBODY( pPacket );
}


//-------------------------------------------------------------------------------------------------
bool CshoClientSOCK::WndPROC (WPARAM wParam, LPARAM lParam)
{
	int nErrorCode = WSAGETSELECTERROR(lParam);
	switch ( WSAGETSELECTEVENT(lParam) ) {
		case FD_READ:
		{
			this->OnReceive( nErrorCode );
			break;
		}
		case FD_WRITE:
			this->OnSend ( nErrorCode );
			break;
		case FD_CONNECT: 
		{
			this->OnConnect ( nErrorCode );

			if ( !nErrorCode ) 
				this->Set_NetSTATUS( NETWORK_STATUS_CONNECT );
			else 
				this->Set_NetSTATUS( NETWORK_STATUS_DERVERDEAD );
			break;
		}
		case FD_CLOSE:		// Close()�Լ��� ȣ���ؼ� ����ɶ��� �߻� ���Ѵ�.
		{
			this->OnClose ( nErrorCode );
			this->Set_NetSTATUS( NETWORK_STATUS_DISCONNECT );
		}
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
