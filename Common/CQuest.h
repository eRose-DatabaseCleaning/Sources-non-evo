/**
 * \ingroup SHO_GS
 * \file	CQuest.h
 * \brief	������ ����Ʈ ����Ÿ ����
 */
#ifndef	__QUEST_H
#define	__QUEST_H
#include "CBITArray.h"
#include "CItem.h"
//-------------------------------------------------------------------------------------------------

const BYTE g_btSwitchBitMask[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80	};


#define	QUEST_SWITCH_PER_QUEST		32	// ����Ʈ�� ����ġ
#define	QUEST_VAR_PER_QUEST			10	// ����Ʈ�� ����
#define	QUEST_ITEM_PER_QUEST		6	// ����Ʈ�� �κ��丮 ��

typedef	short				t_qstvar;

class CQUEST {
private:
	enum {	BIT_SHIFT=3, WHICH_BIT=0x07 };

	// Total 2 + 2 + 2*10 + 1*4 = 28 bytes !!!
#pragma pack (push, 1)
	WORD		m_wID;
	DWORD		m_dwExpirationTIME;							// ���� �ð�...	0�̸� ������
	t_qstvar    m_pVAR[ QUEST_VAR_PER_QUEST ];				// 10�� ����
	union {
		BYTE	m_btSWITCHES[ QUEST_SWITCH_PER_QUEST/8 ];	// 32�� ����ġ
		DWORD	m_dwSWITCHES;
	} ;
	tagBaseITEM	m_ITEMs[ QUEST_ITEM_PER_QUEST ];
#pragma pack (pop)

	void ClearAllSwitch ();
	BYTE GetBit (int iIndex);
	void SetBit (int iIndex);
	void ClearBit (int iIndex);
	BYTE FlipBit (int iIndex);

public :
	void Init ();
	void SetID (WORD wID, bool bResetTimer);
	WORD GetID (void)							{	return	m_wID;	}

	void Set_VAR (int iVarNO, int iValue);
	int  Get_VAR (int iVarNO);

	DWORD	Get_SWITCHs ()						{	return this->m_dwSWITCHES;			} 
	void	Set_SWITCHs (DWORD dwSwitchs)		{	this->m_dwSWITCHES = dwSwitchs;		}

	void Set_SWITCH (int iSwitchNO, int iValue);
	int  Get_SWITCH (int iSwitchNO);
	int  Flip_SWITCH(int iSwitchNO);

	DWORD	GetExpirationTIME()			{	return m_dwExpirationTIME;		}
	DWORD	GetRemainTIME();

	tagBaseITEM*GetSameITEM( WORD wItemHEADER );
	tagBaseITEM*GetSlotITEM( int iSlotNo );
	bool		SubITEM( tagITEM &sITEM );
	bool		AddITEM( tagITEM &sITEM );

	void		CheckExpiredTIME ();
} ;

//-------------------------------------------------------------------------------------------------
#endif
