#ifndef classPACKETH
#define classPACKETH

#include "NET_Packet.h"
#include "NET_Prototype.h"
//---------------------------------------------------------------------------

// ���� ������ ��Ŷ�� ���������� �����Ҽ������Ƿ� �Ӱ迵���� ���� ī��Ʈ�� �ּ� ����
class CPacket : public t_PACKET {
private :
	CRITICAL_SECTION			m_csPACKET;

public :
	int							m_iRefCnt;

	CPacket ()                  {   ::InitializeCriticalSection( &m_csPACKET ); }
	~CPacket ()                 {   ::DeleteCriticalSection( &m_csPACKET );     }
	inline void LockPacket()	{	::EnterCriticalSection( &m_csPACKET );	    }
	inline void UnlockPacket()	{	::LeaveCriticalSection( &m_csPACKET );	    }

	char *GetStringPtr	(short *pOffset);
	BYTE *GetDataPtr	(short *pOffset, short nSize);
	bool  AppendString	(char *pStr);
	bool  AppendData	(void *pData, short nLen);
} ;

//---------------------------------------------------------------------------

CPacket *Packet_AllocOnly ();
void     Packet_ReleaseOnly (CPacket *pCPacket);
CPacket *Packet_AllocNLock ();
void     Packet_ReleaseNUnlock (CPacket *pCPacket);
void     Packet_DecRefCount (CPacket *pCPacket);

//---------------------------------------------------------------------------
#endif
