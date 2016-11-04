#include "stdAFX.h"
#include "CQuest.h"
#include "IO_Quest.h"

#ifndef	__SERVER
#include "..\GameProc\CDayNNightProc.h"
#include "../Game.h"
#include "../interface/it_mgr.h"
#else
extern DWORD Get_WorldPassTIME();
#endif

#define	MAX_ASSOCIATION		20	// �ִ� ���� ����

	// ���� ����Ʈ ���� ��ġ
	// ģ�� ��ġ[ MAX_ASSOCIATION ] .	ó�� ���� �� : 0
	// ����Ʈ ���� ��ġ[ MAX_ASSOCIATION ]
	/*
	// �������� ����Ʈ
	{
		����Ʈ ���� ���� 10��
		����Ʈ ����ġ 8��
		����Ʈ Ÿ�̸� 
		����Ʈ ������ 
	}
	*/

//-------------------------------------------------------------------------------------------------
void CQUEST::Init ()
{
	m_wID = 0;
	ClearAllSwitch ();
	::ZeroMemory (m_pVAR,	sizeof(m_pVAR)  );
	::ZeroMemory (m_ITEMs,	sizeof(m_ITEMs) );
}

void CQUEST::SetID (WORD wID, bool bResetTimer)
{	
	m_wID = wID;

	if ( bResetTimer ) {
		if ( QUEST_TIME_LIMIT( wID ) ) {
			m_dwExpirationTIME = ::Get_WorldPassTIME() + QUEST_TIME_LIMIT( wID );
		} else
			m_dwExpirationTIME = 0;		// ������..
	} else 
	if ( QUEST_TIME_LIMIT( wID ) ) {
		// ������ �ʿ���� ����ε�...�ٲ�°��� �ð� ������ �ʿ��ϸ�...
		m_dwExpirationTIME = ::Get_WorldPassTIME() + QUEST_TIME_LIMIT( wID );
	}
}

//-------------------------------------------------------------------------------------------------
DWORD CQUEST::GetRemainTIME()
{
	DWORD dwCurTime = ::Get_WorldPassTIME();

	if ( dwCurTime >= m_dwExpirationTIME ) {
		// ���� �ð� ����.
		return 0;
	}

	return ( m_dwExpirationTIME - dwCurTime );
}

//-------------------------------------------------------------------------------------------------
void CQUEST::Set_VAR (int iVarNO, int iValue)
{
    if ( iVarNO < 0 || iVarNO >= QUEST_VAR_PER_QUEST )
        return;

    m_pVAR[ iVarNO ] = iValue;
}
int  CQUEST::Get_VAR (int iVarNO)
{
    if ( iVarNO < 0 || iVarNO >= QUEST_VAR_PER_QUEST )
        return 0;
    return m_pVAR[ iVarNO ];
}


//-------------------------------------------------------------------------------------------------
void CQUEST::ClearAllSwitch ()
{
	::ZeroMemory (m_btSWITCHES, sizeof(m_btSWITCHES));
}
BYTE CQUEST::GetBit (int iIndex)
{
	return ( m_btSWITCHES[ iIndex >> BIT_SHIFT ] & g_btSwitchBitMask[ iIndex & WHICH_BIT ] ) ? 1 : 0;
}
void CQUEST::SetBit (int iIndex)
{
	m_btSWITCHES[ iIndex >> BIT_SHIFT ] |= g_btSwitchBitMask[ iIndex & WHICH_BIT ];
}
void CQUEST::ClearBit (int iIndex)
{
	m_btSWITCHES[ iIndex >> BIT_SHIFT ] &= ~(g_btSwitchBitMask[ iIndex & WHICH_BIT ]);
}
BYTE CQUEST::FlipBit (int iIndex)
{
	if ( this->GetBit( iIndex ) )
		this->ClearBit( iIndex );
	else
		this->SetBit ( iIndex );

	return this->GetBit( iIndex );
}

//-------------------------------------------------------------------------------------------------
void CQUEST::Set_SWITCH (int iSwitchNO, int iValue)
{
    if ( iSwitchNO < 0 || iSwitchNO >= QUEST_SWITCH_PER_QUEST )
        return;

    if ( iValue )
        this->SetBit( iSwitchNO );
    else
        this->ClearBit( iSwitchNO );
}
int  CQUEST::Get_SWITCH (int iSwitchNO)
{
    if ( iSwitchNO < 0 || iSwitchNO >= QUEST_SWITCH_PER_QUEST )
        return -1;

    return this->GetBit( iSwitchNO );
}
int  CQUEST::Flip_SWITCH(int iSwitchNO)
{
    if ( iSwitchNO < 0 || iSwitchNO >= QUEST_SWITCH_PER_QUEST )
        return -1;

    return this->FlipBit( iSwitchNO );
}

//-------------------------------------------------------------------------------------------------
tagBaseITEM* CQUEST::GetSlotITEM( int iSlotNo )
{
	if( iSlotNo < 0 || iSlotNo >= QUEST_ITEM_PER_QUEST )
		return NULL;

	return &(m_ITEMs[ iSlotNo ]);
}
tagBaseITEM*CQUEST::GetSameITEM( WORD wItemHEADER )
{
	for (short nI=0; nI<QUEST_ITEM_PER_QUEST; nI++) {
		if ( this->m_ITEMs[ nI ].GetHEADER() == wItemHEADER )
			return &this->m_ITEMs[ nI ];
	}
	return NULL;
}
bool CQUEST::SubITEM( tagITEM &sITEM )
{
	for (short nI=0; nI<QUEST_ITEM_PER_QUEST; nI++) {
		if ( this->m_ITEMs[ nI ].GetHEADER() == sITEM.GetHEADER() ) 
		{
			tagITEM sOriITEM;
			sOriITEM = this->m_ITEMs[ nI ];
			sOriITEM.SubtractOnly( sITEM );
			this->m_ITEMs[ nI ] = sOriITEM;

#ifndef __SERVER
			g_itMGR.AppendChatMsg( sITEM.SubtractQuestMESSAGE(), IT_MGR::CHAT_TYPE_QUEST );
#endif
			return true;
		}
	}
	return false;
}

//-------------------------------------------------------------------------------------------------
bool CQUEST::AddITEM( tagITEM &sITEM )
{
	short nI;

	if ( sITEM.IsEnableDupCNT() ) {
		// �ߺ� ����, ���� �������� �ִ°� ??
		for (nI=0; nI<QUEST_ITEM_PER_QUEST; nI++) {
			if ( this->m_ITEMs[ nI ].GetHEADER() == sITEM.GetHEADER() ) {
				this->m_ITEMs[ nI ].m_uiQuantity += sITEM.GetQuantity();
				return true;
			}
		}
	}
	
	// ����� �߰�..
	for (nI=0; nI<QUEST_ITEM_PER_QUEST; nI++) {
		if ( 0 == this->m_ITEMs[ nI ].GetHEADER() ) {
			this->m_ITEMs[ nI ] = sITEM;
			return true;
		}
	}

	return false;
}

//-------------------------------------------------------------------------------------------------
