#include "stdAFX.h"
#include "IO_STB.h"
#include "CUserDATA.h"
#include "Object.h"
#include "Calculation.h"

#ifndef __SERVER
	#include "IO_PAT.h"
	#include "../Network/NET_Prototype.h"
	#include "../Interface/Dlgs/ChattingDlg.h"
	#include "../Interface/it_mgr.h"
	#include "../Game.h"
#endif
#define	MAX_INT		0x07fffffff

//-------------------------------------------------------------------------------------------------
	
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
short tagBankData::Get_EmptySlot (short nStartSlotNO, short nEndSlotNO)
{
	for (short nI=nStartSlotNO; nI<nEndSlotNO; nI++) 
		if ( 0 == this->m_ItemLIST[ nI ].GetHEADER() )
			return nI;

	return -1;
}

//-------------------------------------------------------------------------------------------------
short tagBankData::Add_ITEM(tagITEM &sITEM, short nStartSlotNO, short nEndSlotNO)
{
	if ( 0 == sITEM.GetTYPE() || sITEM.GetTYPE() >= ITEM_TYPE_MONEY ) {
		return -1;
	}

	short nI;
	if ( sITEM.GetTYPE() >= ITEM_TYPE_USE ) {
		// Áßº¹ µÉ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÌ´Ù.
		for (nI=0; nI<nEndSlotNO; nI++) {
			if ( this->m_ItemLIST[ nI ].GetHEADER() != sITEM.GetHEADER() )
				continue;
			
			if ( this->m_ItemLIST[ nI ].GetQuantity() + sITEM.GetQuantity() <= MAX_DUP_ITEM_QUANTITY ) {
				this->m_ItemLIST[ nI ].m_uiQuantity += sITEM.GetQuantity();
				return nI;
			}
		}
	}
	
	nI = this->Get_EmptySlot( nStartSlotNO, nEndSlotNO );
	if ( nI >= nStartSlotNO ) {
		this->m_ItemLIST[ nI ] = sITEM;
		return nI;
	}

	return -1;
}

short tagBankData::Add_ITEM(short nSlotNO, tagITEM &sITEM)
{
	if ( sITEM.GetTYPE() >= ITEM_TYPE_USE ) {
		// Áßº¹ µÉ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÌ´Ù.
		this->m_ItemLIST[ nSlotNO ].m_uiQuantity += sITEM.GetQuantity();
		return nSlotNO;
	}

	this->m_ItemLIST[ nSlotNO ] = sITEM;
	if ( this->m_ItemLIST[ nSlotNO ].GetQuantity() > MAX_DUP_ITEM_QUANTITY )
		this->m_ItemLIST[ nSlotNO ].m_uiQuantity = MAX_DUP_ITEM_QUANTITY;

	return nSlotNO;
}

short tagBankData::Sub_ITEM(short nSlotNO, tagITEM &sITEM)
{
	if ( 0 == sITEM.GetTYPE() || sITEM.GetTYPE() >= ITEM_TYPE_MONEY ) {
		return -1;
	}

	if ( sITEM.GetTYPE() >= ITEM_TYPE_USE ) {
		// Áßº¹ µÉ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÌ´Ù.

		if ( this->m_ItemLIST[ nSlotNO ].GetQuantity() > sITEM.GetQuantity() ) {
			this->m_ItemLIST[ nSlotNO ].m_uiQuantity -= sITEM.GetQuantity();
			return nSlotNO;
		}

		sITEM.m_uiQuantity = this->m_ItemLIST[ nSlotNO ].GetQuantity();
	}

	// ½½·Ô ºñ¿ò.
	this->m_ItemLIST[ nSlotNO ].Clear();

	return nSlotNO;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void CUserDATA::Cal_AddAbility (tagITEM &sITEM, short nItemTYPE)
{
	if ( sITEM.GetItemNO() < 1 || sITEM.GetLife() < 1 )		/// ¾ÆÀÌÅÛÀÌ ¾ø°Å³ª ¼ö¸íÀÌ ´ÙÇÑ°ÍÀº Åë°ú~
		return;

	short nI, nC, nType, nValue;

	// ¿É¼Ç/¹ÚÈù º¸¼®¿¡ ´ëÇØ¼­...
	if ( sITEM.GetGemNO() && ( sITEM.IsAppraisal() || sITEM.HasSocket() ) ) {
		for (nI=0; nI<2; nI++) {
			nC = sITEM.GetGemNO();
			nType  = GEMITEM_ADD_DATA_TYPE ( nC, nI );
			nValue = GEMITEM_ADD_DATA_VALUE( nC, nI );

			_ASSERT( nType <= AT_MAX );

			this->m_iAddValue[ nType ] += nValue;
		}
	}

	for (nI=0; nI<2; nI++) {
		nType = ITEM_NEED_UNION(nItemTYPE, sITEM.m_nItemNo, nI);
		if ( nType && ( nType != this->GetCur_JOHAP() ) )
			continue;

		nType = ITEM_ADD_DATA_TYPE	(nItemTYPE, sITEM.m_nItemNo, nI);
		nValue= ITEM_ADD_DATA_VALUE	(nItemTYPE, sITEM.m_nItemNo, nI);

		this->m_iAddValue[ nType ] += nValue;
	}
}

/// ¾ÆÀÌÅÛ¿¡ ºÙÀº Ãß°¡ ´É·ÂÄ¡ °è»ê.
void CUserDATA::Cal_AddAbility ()
{
	tagITEM	sITEM;

	sITEM = this->Get_EquipITEM( EQUIP_IDX_FACE_ITEM );
	Cal_AddAbility(sITEM, ITEM_TYPE_FACE_ITEM	);

	sITEM = this->Get_EquipITEM( EQUIP_IDX_HELMET );
	Cal_AddAbility(sITEM, ITEM_TYPE_HELMET);

	sITEM = this->Get_EquipITEM( EQUIP_IDX_ARMOR );
	Cal_AddAbility(sITEM, ITEM_TYPE_ARMOR);

	sITEM = this->Get_EquipITEM( EQUIP_IDX_KNAPSACK );
	Cal_AddAbility(sITEM, ITEM_TYPE_KNAPSACK);

	sITEM = this->Get_EquipITEM( EQUIP_IDX_GAUNTLET );
	Cal_AddAbility(sITEM, ITEM_TYPE_GAUNTLET);

	sITEM = this->Get_EquipITEM( EQUIP_IDX_BOOTS );
	Cal_AddAbility(sITEM, ITEM_TYPE_BOOTS	);

	sITEM = this->Get_EquipITEM( EQUIP_IDX_WEAPON_R );
	Cal_AddAbility(sITEM, ITEM_TYPE_WEAPON	);

	sITEM = this->Get_EquipITEM( EQUIP_IDX_WEAPON_L );
	Cal_AddAbility(sITEM, ITEM_TYPE_SUBWPN	);

	sITEM = this->Get_EquipITEM( EQUIP_IDX_NECKLACE );
	Cal_AddAbility(sITEM, ITEM_TYPE_JEWEL	);

	sITEM = this->Get_EquipITEM( EQUIP_IDX_RING );
	Cal_AddAbility(sITEM, ITEM_TYPE_JEWEL	);

	/// 2004/02/21 icarus Ãß°¡ : ±Í°ÉÀÌ 2¹øÂ° ´É·ÂÄ¡ Àû¿ë¾ÈµÇ´ø»çÇ×.
	sITEM = this->Get_EquipITEM( EQUIP_IDX_EARRING );
	Cal_AddAbility(sITEM, ITEM_TYPE_JEWEL	);

	if ( this->GetCur_MOVE_MODE() > MOVE_MODE_RUN ) {
		// Å¾½Â ¸ðµåÀÏ°æ¿ì ÃÖ´ë ¹«°Ô Áõ°¡Ä¡´Â ÄÉ¸¯ÅÍ¿¡¼­ »Ì¾Æ¼­ Àû¿ë.
		int iCurAddWgt = this->m_iAddValue[ AT_WEIGHT ];
		// ¹«°Ô»©°í ÃÊ±âÈ­.
		::ZeroMemory (this->m_iAddValue,	sizeof(int) * AT_MONEY );
		this->m_iAddValue[ AT_WEIGHT ] = iCurAddWgt;

		for (short nI=0; nI<MAX_RIDING_PART; nI++) {
			sITEM = this->m_Inventory.m_ItemRIDE[ nI ];
			Cal_AddAbility(sITEM, ITEM_TYPE_RIDE_PART);
		}
	}
}

//-------------------------------------------------------------------------------------------------
void CUserDATA::Cal_RecoverHP ()
{
	m_btRecoverHP = this->GetPassiveSkillValue( AT_PSV_RECOVER_HP ) + (short)( m_iAddValue[ AT_RECOVER_HP  ] * this->GetPassiveSkillRate( AT_PSV_RECOVER_HP ) / 100.f );
}

//-------------------------------------------------------------------------------------------------
void CUserDATA::Cal_BattleAbility ()
{
	int iDefDura = 0;

	tagITEM *pITEM;
	if ( this->GetCur_MOVE_MODE() <= MOVE_MODE_RUN ) { // Å¸ÀÌ¿ÏÀÌ¸é ³»±¸µµ´Â Ä³¸¯ÅÍ ²¬·Î...
		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_HELMET ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_ARMOR ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_KNAPSACK ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_GAUNTLET ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_BOOTS ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_WEAPON_L ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();
	} 
	else if( this->GetCur_MOVE_MODE() > MOVE_MODE_RUN && IsTAIWAN() ) {
		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_HELMET ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_ARMOR ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_KNAPSACK ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_GAUNTLET ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_BOOTS ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_WEAPON_L ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();
	} else {
		pITEM = &this->m_Inventory.m_ItemRIDE[ RIDE_PART_BODY ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemRIDE[ RIDE_PART_ENGINE ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemRIDE[ RIDE_PART_LEG ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();

		pITEM = &this->m_Inventory.m_ItemRIDE[ RIDE_PART_ARMS ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();
#ifdef __KCHS_BATTLECART__
		pITEM = &this->m_Inventory.m_ItemRIDE[ RIDE_PART_ABIL ];
		if ( pITEM->m_wHeader && pITEM->GetLife() )	iDefDura += pITEM->GetDurability();
#endif
	}
	m_Battle.m_iDefDurabity = iDefDura;

	::ZeroMemory (this->m_iAddValue,	sizeof(int) * AT_MONEY );
	// ÆÐ½Ãºê º¸Á¤¿µ¿ª Á¦¿Ü..
	::ZeroMemory (&this->m_iAddValue[ AT_AFTER_PASSIVE_SKILL ],		sizeof(int)*(AT_MAX-AT_AFTER_PASSIVE_SKILL) );

	::ZeroMemory (this->m_nPassiveRate,	sizeof(short) * AT_MONEY );
	// ÆÐ½Ãºê º¸Á¤¿µ¿ª Á¦¿Ü..
	::ZeroMemory (&this->m_nPassiveRate[ AT_AFTER_PASSIVE_SKILL ],	sizeof(short)*(AT_MAX-AT_AFTER_PASSIVE_SKILL) );


	// ¾ÆÀÌÅÛ¿¡ ºÙÀº Ãß°¡ ´É·ÂÄ¡ °è»ê
	Cal_AddAbility ();		// Ç×»ó ¾Õ¼­µµ·Ï !!!

	// ÆÐ½Ãºê ½ºÅ³ Ãß°¡ ´É·ÂÄ¡ °ªÀ» Ãß°¡ ´É·ÂÄ¡ °ª¿¡ Àû¿ë...
	Cal_RecoverHP ();
	Cal_RecoverMP ();
	if ( this->GetCur_MOVE_MODE() <= MOVE_MODE_RUN ) {
		m_iAddValue[ AT_STR	  ] += m_PassiveAbilityFromValue[ AT_STR	-AT_STR ];
		m_iAddValue[ AT_DEX	  ] += m_PassiveAbilityFromValue[ AT_DEX	-AT_STR ];
		m_iAddValue[ AT_INT	  ] += m_PassiveAbilityFromValue[ AT_INT	-AT_STR ];
		m_iAddValue[ AT_CON	  ] += m_PassiveAbilityFromValue[ AT_CON	-AT_STR ];
		m_iAddValue[ AT_CHARM ] += m_PassiveAbilityFromValue[ AT_CHARM	-AT_STR ];
		m_iAddValue[ AT_SENSE ] += m_PassiveAbilityFromValue[ AT_SENSE	-AT_STR ];
	}

	// Ãß°¡ ´É·ÂÄ¡ °è»êÀÌ ¿Ï·áµÈÈÄ Ã³¸® ÇØ¾ßµÊ...
	Cal_MaxHP ();
	Cal_MaxMP ();
	Cal_ATTACK ();
	Cal_HIT ();
	Cal_DEFENCE ();
	Cal_RESIST ();
	Cal_MaxWEIGHT ();
	Cal_AvoidRATE ();	// Cal_DEFENCEº¸´Ù µÚ¿¡ ¿À°Ô..
	Cal_CRITICAL ();
#ifdef	__APPLY_2ND_JOB
	Cal_IMMUNITY();
#endif

	short nI;
	/// ÇöÀç ¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛµéÀÇ ¹«°Ô¸¦ °è»ê...
	m_Battle.m_nWEIGHT = 0;
	for (nI=EQUIP_IDX_FACE_ITEM; nI<INVENTORY_TOTAL_SIZE; nI++) {
		m_Battle.m_nWEIGHT += m_Inventory.GetWEIGHT( nI );
	}

	Cal_DropRATE ();
	m_Battle.m_fRateUseMP = ( 100 - this->GetCur_SaveMP() ) / 100.f;

	if ( GetCur_HP() > GetCur_MaxHP() ) SetCur_HP( GetCur_MaxHP() );

	if ( IsTAIWAN() && this->GetCur_MOVE_MODE() <= MOVE_MODE_RUN ) {
		// ´ë¸¸ º¸Çà¸ðµå¸é...
		int iCurAbility;
		for (nI=0; nI<BA_MAX; nI++) {
			iCurAbility = this->m_BasicAbility.m_nBasicA[ nI ] + m_iAddValue[ AT_STR + nI ];
			m_PassiveAbilityFromRate[ nI ] = (int)( iCurAbility * m_nPassiveRate[ AT_STR+nI ] / 100.f );
		}
	}

#ifdef __KCHS_BATTLECART__
	Cal_PatMaxHP();
#endif

	/// 2Â÷ ÀüÁ÷ ´É·ÂÄ¡ Àû¿ë
	Apply_2ndJob_Ability();
}

//-------------------------------------------------------------------------------------------------
void CUserDATA::Cal_RecoverMP ()
{
	m_btRecoverMP = this->GetPassiveSkillValue( AT_PSV_RECOVER_MP ) + (short)( m_iAddValue[ AT_RECOVER_MP  ] * this->GetPassiveSkillRate( AT_PSV_RECOVER_MP ) / 100.f );
}


#ifdef __KCHS_BATTLECART__
void CUserDATA::Cal_PatMaxHP ()
{
	m_Battle.m_nPatMaxHP = 0;
	for (short nI=0; nI<MAX_RIDING_PART; nI++) {
		tagITEM sITEM = this->m_Inventory.m_ItemRIDE[ nI ];
		m_Battle.m_nPatMaxHP += PAT_ITEM_HP( sITEM.GetItemNO() );		
	}

	if( this->m_GrowAbility.m_nPatHP > m_Battle.m_nPatMaxHP )
		this->m_GrowAbility.m_nPatHP = m_Battle.m_nPatMaxHP;
}
#endif

//-------------------------------------------------------------------------------------------------
extern bool IsTAIWAN();
int CUserDATA::Cal_MaxHP ()
{
	int iA, iM1, iM2;
	if ( IsTAIWAN() ) {
		float fC;
		switch( this->GetCur_JOB() ) {
			case CLASS_SOLDIER_111 :	// ¼ÖÁ®(111)		[(LV+5)*SQRT(LV+20)*3.5+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡ 
										iA=5,	iM1=20, fC=3.5f;	break;
			case CLASS_SOLDIER_121 :	// ³ªÀÌÆ®(121)		[(LV+5)*SQRT(LV+28)*3.5+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡ 
										iA=5,	iM1=28,	fC=3.5f;	break;
			case CLASS_SOLDIER_122 :	// Ã¨ÇÁ(122			[(LV+5)*SQRT(LV+22)*3.5+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡ 
										iA=5,	iM1=22,	fC=3.5f;	break;

			case CLASS_MAGICIAN_211 :	// ¹ÂÁî(211)		[(LV+4)*SQRT(LV+26)*2.36+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡ 
										iA=4,	iM1=26,	fC=2.36f;	break;
			case CLASS_MAGICIAN_221 :	// ¸ÅÁö¼Ç(221)		[(LV+5)*SQRT(LV+26)*2.37+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡ 
										iA=5,	iM1=26,	fC=2.37f;	break;
			case CLASS_MAGICIAN_222 :	// Å¬·¹¸¯(222)		[(LV+7)*SQRT(LV+26)*2.4+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡ 
										iA=7,	iM1=26,	fC=2.4f;	break;

			case CLASS_MIXER_311	 :	// È£Ä¿(311)		[(LV+5)*SQRT(LV+20)*2.7+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡
										iA=5,	iM1=20,	fC=2.7f;	break;
			case CLASS_MIXER_321	 :	// ·¹ÀÌ´õ(321)		[(LV+5)*SQRT(LV+23)*3+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡
										iA=5,	iM1=23,	fC=3.f;		break;
			case CLASS_MIXER_322	 :	// ½ºÄ«¿ìÆ®(322)	[(LV+5)*SQRT(LV+21)*2.7+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡
										iA=5,	iM1=21,	fC=2.7f;	break;

			case CLASS_MERCHANT_411 :	// µô·¯(411)		[(LV+5)*SQRT(LV+20)*2.7+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡
										iA=5,	iM1=20,	fC=2.7f;	break;
			case CLASS_MERCHANT_421 :	// ºÎ·çÁî¾Æ(421)	[(LV+5)*SQRT(LV+20)*2.7+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡
										iA=5,	iM1=20,	fC=2.7f;	break;
			case CLASS_MERCHANT_422 :	// ¾ÆÆ¼ÀÜ(422)		[(LV+5)*SQRT(LV+20)*2.7+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡
										iA=5,	iM1=20,	fC=2.7f;	break;

			//case CLASS_VISITOR :		// ¹«Á÷	ºñÁöÅÍ(00)	[(LV+4)*SQRT(LV+26)*2.36+(STR*2)] + ¾ÆÀÌÅÛ Áõ°¡Ä¡
			default :					iA=4,	iM1=26,	fC=2.36f;	break;
		}

		m_Battle.m_nMaxHP  = (short)( ( this->GetCur_LEVEL()+iA ) * sqrtf(this->GetCur_LEVEL()+iM1) * fC + ( this->GetCur_STR()*2 ) + this->m_iAddValue[ AT_MAX_HP ] );
	} else {
		switch( this->GetCur_JOB() ) {
			case CLASS_SOLDIER_111 :	iA=7,	iM1=12, iM2=2;	break;
			case CLASS_SOLDIER_121 :	iA=-3,	iM1=14,	iM2=2;	break;
			case CLASS_SOLDIER_122 :	iA=2,	iM1=13,	iM2=2;	break;

			case CLASS_MAGICIAN_211 :	iA=11,	iM1=10,	iM2=2;	break;
			case CLASS_MAGICIAN_221 :	iA=11,	iM1=10,	iM2=2;	break;
			case CLASS_MAGICIAN_222 :	iA=5,	iM1=11,	iM2=2;	break;

			case CLASS_MIXER_311	 :	iA=10,	iM1=11,	iM2=2;	break;
			case CLASS_MIXER_321	 :	iA=2,	iM1=13,	iM2=2;	break;
			case CLASS_MIXER_322	 :	iA=11,	iM1=11,	iM2=2;	break;

			case CLASS_MERCHANT_411 :	iA=12,	iM1=10,	iM2=2;	break;
			case CLASS_MERCHANT_421 :	iA=13,	iM1=10,	iM2=2;	break;
			case CLASS_MERCHANT_422 :	iA=6,	iM1=11,	iM2=2;	break;

			//case CLASS_VISITOR :
			default :					iA=12,	iM1=8,	iM2=2;	break;
		}
		m_Battle.m_nMaxHP  = ( this->GetCur_LEVEL()+iA ) * iM1 + ( this->GetCur_STR()*iM2 ) + this->m_iAddValue[ AT_MAX_HP ];
	}
	iA = this->GetPassiveSkillValue( AT_PSV_MAX_HP ) + (short)( m_Battle.m_nMaxHP * this->GetPassiveSkillRate( AT_PSV_MAX_HP ) / 100.f );
	m_Battle.m_nMaxHP += iA;
	this->Cal_AruaMaxHP ();

	return m_Battle.m_nMaxHP;
}

//-------------------------------------------------------------------------------------------------
int CUserDATA::Cal_MaxMP ()
{
	int   iA, iM2;
	float fM1;

	switch( this->GetCur_JOB() ) {
		case CLASS_SOLDIER_111 :	iA=3,	fM1=4.0f,	iM2=4;	break;
		case CLASS_SOLDIER_121 :	iA=0,	fM1=4.5f,	iM2=4;	break;
		case CLASS_SOLDIER_122 :	iA=-6,	fM1=5.0f,	iM2=4;	break;

		case CLASS_MAGICIAN_211 :	iA=0,	fM1=6.0f,	iM2=4;	break;
		case CLASS_MAGICIAN_221 :	iA=-7,	fM1=7.0f,	iM2=4;	break;
		case CLASS_MAGICIAN_222 :	iA=-4,	fM1=6.5f,	iM2=4;	break;

		case CLASS_MIXER_311	 :	iA=4,	fM1=4.0f,	iM2=4;	break;
		case CLASS_MIXER_321	 :	iA=4,	fM1=4.0f,	iM2=4;	break;
		case CLASS_MIXER_322	 :	iA=0,	fM1=4.5f,	iM2=4;	break;

		case CLASS_MERCHANT_411 :	iA=3,	fM1=4.0f,	iM2=4;	break;
		case CLASS_MERCHANT_421 :	iA=3,	fM1=4.0f,	iM2=4;	break;
		case CLASS_MERCHANT_422 :	iA=0,	fM1=4.5f,	iM2=4;	break;

		//case CLASS_VISITOR :
		default :					iA=4,	fM1=3.0f,	iM2=4;	break;
	}

	m_Battle.m_nMaxMP  = ( short) ( ( this->GetCur_LEVEL()+iA ) * fM1 + ( this->GetCur_INT()*iM2 ) ) + this->m_iAddValue[ AT_MAX_MP ];
	iA = this->GetPassiveSkillValue( AT_PSV_MAX_MP ) + (short)( m_Battle.m_nMaxMP * this->GetPassiveSkillRate( AT_PSV_MAX_MP ) / 100.f );
	m_Battle.m_nMaxMP += iA;
	this->Cal_AruaMaxHP ();

	return m_Battle.m_nMaxMP;
}


//-------------------------------------------------------------------------------------------------
int CUserDATA::Cal_MaxWEIGHT ()
{
	// * ITEM = 800 + (LV * 4) + (STR * 6) + ½ºÅ³ ¼ÒÁö·®
	this->m_Battle.m_nMaxWEIGHT  = (int) ( 1100 + ( this->GetCur_LEVEL() * 5 ) + ( this->GetCur_STR() * 6 ) );
	this->m_Battle.m_nMaxWEIGHT += this->m_iAddValue[ AT_WEIGHT ];

	tagITEM *pITEM = &m_Inventory.m_ItemEQUIP[ EQUIP_IDX_KNAPSACK ];
	if ( pITEM->GetHEADER() && pITEM->GetLife() && ITEM_TYPE( pITEM->GetTYPE(), pITEM->GetItemNO() ) == 162 ) {
		// µîÁü ¹è³¶.
		short nW = this->GetPassiveSkillValue( AT_PSV_WEIGHT ) + (short)( this->m_Battle.m_nMaxWEIGHT * this->GetPassiveSkillRate( AT_PSV_WEIGHT ) / 100.f );
		this->m_Battle.m_nMaxWEIGHT += nW;
	}

	return this->m_Battle.m_nMaxWEIGHT;
}


//-------------------------------------------------------------------------------------------------
int CUserDATA::Cal_AvoidRATE ()
{
	if ( this->GetCur_MOVE_MODE() > MOVE_MODE_RUN && !IsTAIWAN() ) {
		m_Battle.m_nAVOID = (short)( (GetCur_DEX()+10) * 0.8f + GetCur_LEVEL() * 0.5f );
	} else {
		//AVO = [ (DEX*1.6 + LV*0.3 + 5) * 0.4 + (¹æ¾î±¸ Ç°ÁúÇÕ * 0.3) ]
		m_Battle.m_nAVOID = (short)( ( GetCur_DEX()*1.9f + GetCur_LEVEL()*0.3f + 10 ) * 0.4f ) + 
			(short)( this->GetTot_DEF_DURABITY()*0.3f ) + this->GetTot_DEF_GRADE();
	}
	m_Battle.m_nAVOID += this->m_iAddValue[ AT_AVOID ];

	int iPsv = this->GetPassiveSkillValue( AT_PSV_AVOID ) + (short)( this->m_Battle.m_nAVOID * this->GetPassiveSkillRate( AT_PSV_AVOID ) / 100.f );
	this->m_Battle.m_nAVOID += iPsv;

	this->Cal_AruaAVOID();

	return m_Battle.m_nAVOID;
}

//-------------------------------------------------------------------------------------------------
int CUserDATA::Cal_CRITICAL ()
{

	if ( this->GetCur_MOVE_MODE() > MOVE_MODE_RUN && IsTAIWAN() ) {
		m_Battle.m_iCritical  = (int)( GetCur_SENSE() + ( GetCur_CON() + 20 ) * 0.2f );
	} else if ( this->GetCur_MOVE_MODE() > MOVE_MODE_RUN ) {
		m_Battle.m_iCritical = (int)( GetCur_SENSE()*0.8f + GetCur_LEVEL() * 0.3f );
	} else {
		m_Battle.m_iCritical  = (int)( GetCur_SENSE() + ( GetCur_CON() + 20 ) * 0.2f );
	}
	m_Battle.m_iCritical += this->m_iAddValue[ AT_CRITICAL ];

	int iPsv = this->GetPassiveSkillValue( AT_PSV_CRITICAL ) + (short)( this->m_Battle.m_iCritical * this->GetPassiveSkillRate( AT_PSV_CRITICAL ) / 100.f );
	this->m_Battle.m_iCritical += iPsv;

	this->Cal_AruaCRITICAL ();

	return m_Battle.m_iCritical;
}

//-------------------------------------------------------------------------------------------------
int CUserDATA::Cal_RESIST ()
{
	int iTotRES=0, iTotGradeRES=0;

	// ¸ðµç ¾ÆÀÌÅÛ¿¡ Ç×¸¶·ÂÀÌ ÀÔ·ÂµÉ¼ö ÀÖ´Ù..
	for (short nE=EQUIP_IDX_NULL+1; nE<MAX_EQUIP_IDX; nE++) {
		if ( m_Inventory.m_ItemEQUIP[ nE ].GetLife() < 1 || 0 == m_Inventory.m_ItemEQUIP[ nE ].GetTYPE() )
			continue;

		if ( ITEM_RESISTENCE( m_Inventory.m_ItemEQUIP[ nE ].GetTYPE(), m_Inventory.m_ItemEQUIP[ nE ].GetItemNO() ) ) {
			iTotRES		 += ITEM_RESISTENCE( m_Inventory.m_ItemEQUIP[ nE ].GetTYPE(), m_Inventory.m_ItemEQUIP[ nE ].GetItemNO() );
			iTotGradeRES += ITEMGRADE_RES( m_Inventory.m_ItemEQUIP[ nE ].GetGrade() );
		}
	}

//#ifndef	__INC_WORLD
//	this->m_Battle.m_nRES = (int) ( iTotRES + (iTotGradeRES) + ( this->GetCur_INT()+5 ) * 0.4f + (this->GetCur_LEVEL()+15) * 0.9f );
//#else
	this->m_Battle.m_nRES = (int) ( iTotRES + (iTotGradeRES) + ( this->GetCur_INT()+5 ) * 0.6f + (this->GetCur_LEVEL()+15) * 0.8f );
//#endif
	this->m_Battle.m_nRES += this->m_iAddValue[ AT_RES ];

	iTotRES = this->GetPassiveSkillValue( AT_PSV_RES ) + (short)( this->m_Battle.m_nRES * this->GetPassiveSkillRate( AT_PSV_RES ) / 100.f );
	this->m_Battle.m_nRES += iTotRES;

	return this->m_Battle.m_nRES;
}

//-------------------------------------------------------------------------------------------------
int CUserDATA::Cal_HIT ()
{
	int iHitRate;
	tagITEM*pRightWPN; 

	if ( this->GetCur_MOVE_MODE() <= MOVE_MODE_RUN ) {
		pRightWPN = this->Get_EquipItemPTR( EQUIP_IDX_WEAPON_R );
		if ( pRightWPN->GetItemNO() && pRightWPN->GetLife() > 0 ) {
			iHitRate = (int) ( ( GetCur_CON() + 10 )*0.8f ) + 
					   (int) ( ( ITEM_QUALITY(ITEM_TYPE_WEAPON,pRightWPN->GetItemNO()) ) * 0.6f + ITEMGRADE_HIT(pRightWPN->GetGrade()) + pRightWPN->GetDurability()*0.8f );
		} else {
			// ¸Ç¼Õ
			iHitRate = (int)( ( GetCur_CON() + 10 )*0.5f + 15 );
		}
	} else if ( this->GetCur_MOVE_MODE() > MOVE_MODE_RUN && IsTAIWAN() ) { // Å¾½Â¸ðµåÀÌ°í Å¸ÀÌ¿ÏÀÌ¸é..
		pRightWPN = this->Get_EquipItemPTR( EQUIP_IDX_WEAPON_R );
		if ( pRightWPN->GetItemNO() && pRightWPN->GetLife() > 0 ) {
			iHitRate = (int) ( ( GetCur_CON() + 10 )*0.8f ) + 
					   (int) ( ( ITEM_QUALITY(ITEM_TYPE_WEAPON,pRightWPN->GetItemNO()) ) * 0.6f + ITEMGRADE_HIT(pRightWPN->GetGrade()) + pRightWPN->GetDurability()*0.8f );
		} else {
			// ¸Ç¼Õ
			iHitRate = (int)( ( GetCur_CON() + 10 )*0.5f + 15 );
		}
	} else {
		pRightWPN = &this->m_Inventory.m_ItemRIDE[ RIDE_PART_ARMS ];
		if ( pRightWPN->GetLife() > 0 )
			iHitRate = (int) ( ( GetCur_CON()+10 ) * 0.8f ) + ( GetCur_LEVEL() * 0.5f ) +
					   (int) ( ITEM_QUALITY( ITEM_TYPE_RIDE_PART, pRightWPN->GetItemNO() ) * 1.2f );
		else
			iHitRate = 0;
	}

	this->m_Battle.m_nHIT = iHitRate + this->m_iAddValue[ AT_HIT ];

	iHitRate = this->GetPassiveSkillValue( AT_PSV_HIT ) + (short)( this->m_Battle.m_nHIT * this->GetPassiveSkillRate( AT_PSV_HIT ) / 100.f );
	this->m_Battle.m_nHIT += iHitRate;

	this->Cal_AruaHIT ();
	return this->m_Battle.m_nHIT;
}

#ifdef	__APPLY_2ND_JOB
// kchs : 2005-08-20 , ¸é¿ª·Â Ãß°¡, ±×¸®°í ÆÐ½Ãºê ´É·ÂÄ¡ Àû¿ë
int CUserDATA::Cal_IMMUNITY ()
{
	this->m_Battle.m_nImmunity += (short)this->GetPassiveSkillValue( AT_PSV_IMMUNITY ) + (short)( this->m_Battle.m_nImmunity * this->GetPassiveSkillRate( AT_PSV_IMMUNITY ) / 100.f );	 
	return this->m_Battle.m_nImmunity;
}
#endif

//-------------------------------------------------------------------------------------------------
int CUserDATA::Cal_DEFENCE ()
{
	int iTotDEF=0, iTotGradeDEF=0, iTotGRADE=0;

	// ¸ðµç ¾ÆÀÌÅÛ¿¡ ¹æ¾î·ÂÀÌ ÀÔ·ÂµÉ¼ö ÀÖ´Ù..
	short nE;
	for (nE=EQUIP_IDX_NULL+1; nE<MAX_EQUIP_IDX; nE++) {
		if ( 0 == m_Inventory.m_ItemEQUIP[ nE ].GetLife() || 0 == m_Inventory.m_ItemEQUIP[ nE ].GetTYPE() )
			continue;
		
		if ( ITEM_DEFENCE( m_Inventory.m_ItemEQUIP[ nE ].GetTYPE(), m_Inventory.m_ItemEQUIP[ nE ].GetItemNO() ) ) {
			iTotDEF		 += ITEM_DEFENCE( m_Inventory.m_ItemEQUIP[ nE ].GetTYPE(), m_Inventory.m_ItemEQUIP[ nE ].GetItemNO() );
			iTotGradeDEF += ITEMGRADE_DEF( m_Inventory.m_ItemEQUIP[ nE ].GetGrade() );
			iTotGRADE	 += m_Inventory.m_ItemEQUIP[ nE ].GetGrade();
		}
	}

	if ( this->GetCur_MOVE_MODE() <= MOVE_MODE_RUN ) {
		this->m_Battle.m_iDefGrade = iTotGRADE;

		this->m_Battle.m_nDEF = (int)(( iTotDEF + (iTotGradeDEF) + ( this->GetCur_STR()+5 ) * 0.35f + (this->GetCur_LEVEL()+15) * 0.7f ) ) + this->m_iAddValue[ AT_DEF ];
	} else {
		for (nE=0; nE<MAX_RIDING_PART; nE++) {
			if ( 0 == m_Inventory.m_ItemRIDE[ nE ].GetLife() || 0 == m_Inventory.m_ItemRIDE[ nE ].GetTYPE() )
				continue;
			
			iTotGradeDEF += ITEMGRADE_DEF( m_Inventory.m_ItemRIDE[ nE ].GetGrade() );
			iTotGRADE	 += m_Inventory.m_ItemRIDE[ nE ].GetGrade();
		}
		this->m_Battle.m_iDefGrade = iTotGRADE;

		this->m_Battle.m_nDEF = (int)(( iTotDEF + (iTotGradeDEF) + ( this->GetCur_STR()+5 ) * 0.35f + (this->GetCur_LEVEL()+15) * 0.7f )*0.8f) + this->m_iAddValue[ AT_DEF ];
	}

	nE = this->GetPassiveSkillValue( AT_PSV_DEF_POW ) + (short)( this->m_Battle.m_nDEF * this->GetPassiveSkillRate( AT_PSV_DEF_POW ) / 100.f );
	this->m_Battle.m_nDEF += nE;

	//	2004. 7. 22 ¹æÆÐÂø¿ë½Ã¸é Àû¿ëµÇ¸é ¹æ¾î·Â ÆÐ½Ãºê ½ºÅ³À» ¹«Á¶°Ç Àû¿ëÀ¸·Î ¼öÁ¤...
	//  2005. 8. 19 ¹æÆÐ ÆÐ½Ãºê ½ºÅ³ Ãß°¡...
	tagITEM *pITEM = &m_Inventory.m_ItemEQUIP[ EQUIP_IDX_WEAPON_L ];
	if ( pITEM->GetHEADER() && pITEM->GetLife() && ITEM_TYPE( pITEM->GetTYPE(), pITEM->GetItemNO() ) == 261 )  {
		// ¹æÆÐ.
		int iPsv = this->GetPassiveSkillValue( AT_PSV_SHIELD_DEF ) + (short)( this->m_Battle.m_nDEF * this->GetPassiveSkillRate( AT_PSV_SHIELD_DEF ) / 100.f );
		this->m_Battle.m_nDEF += iPsv;
	}

	return this->m_Battle.m_nDEF;
}


//-------------------------------------------------------------------------------------------------
int CUserDATA::Cal_ATTACK ()
{
	int iAP = 0;

	tagITEM	*pRightWPN = this->Get_EquipItemPTR( EQUIP_IDX_WEAPON_R );
	int iWeaponAP;

	if ( pRightWPN->GetHEADER() && pRightWPN->GetLife() > 0 ) 
		iWeaponAP = WEAPON_ATTACK_POWER( pRightWPN->m_nItemNo );
	else
		iWeaponAP = 0;		/// ¼ö¸íÀÌ ´ÙÇÑ ¹«±â´Â ¹«±âÀÇ ±âº» °ø°Ý·ÂÀ» 0 À¸·Î..

	if ( this->GetCur_MOVE_MODE() <= MOVE_MODE_RUN ) {		
		t_eSHOT ShotTYPE = pRightWPN->GetShotTYPE();
		if ( ShotTYPE < MAX_SHOT_TYPE ) {
			tagITEM *pShotITEM = &this->m_Inventory.m_ItemSHOT[ ShotTYPE ];

			short nItemQ, nItemW;
			if ( pShotITEM->IsEtcITEM() ) {
				nItemQ = ITEM_QUALITY(pShotITEM->GetTYPE(), pShotITEM->GetItemNO() );
				nItemW = ITEM_WEIGHT (pShotITEM->GetTYPE(), pShotITEM->GetItemNO() );
			} else {
				nItemQ = nItemW = 0;
			}

			// ¼Ò¸ðÅº¿¡ µû¸¥ °ø°Ý·Â °è»ê...
			switch( ShotTYPE ) {
				case SHOT_TYPE_ARROW :
					if ( IsTAIWAN() ) {
						// È­»ìÀåÂø:[  (DEX*0.52 + STR*0.1 + LV*0.1+ÃÑ¾ËÇ°Áú*0.5)+{(¹«±â°ø°Ý·Â+ ¹«±âµî±ÞÃß°¡Ä¡) * (DEX*0.04+SEN*0.03+29) / 30} ] + ¾ÆÀÌÅÛ Áõ°¡Ä¡
						iAP = (int)( ( GetCur_DEX()*0.52f + GetCur_STR()*0.1f + GetCur_LEVEL()*0.1f + nItemQ*0.5f ) +
									( ( iWeaponAP+ ITEMGRADE_ATK(pRightWPN->GetGrade()) ) * ( GetCur_DEX()*0.04f + GetCur_SENSE()*0.03f+29 ) / 30 ) ) ;
					} else {
						iAP = (int)( ( GetCur_DEX()*0.62f + GetCur_STR()*0.2f + GetCur_LEVEL()*0.2f + nItemQ ) +
									( ( ( iWeaponAP+ ITEMGRADE_ATK(pRightWPN->GetGrade()) )+nItemQ*0.5f+8 ) * ( GetCur_DEX()*0.04f + GetCur_SENSE()*0.03f + 29 ) / 30.f ) ) ;
					}
					break;
				case SHOT_TYPE_BULLET:
					if( IsTAIWAN() ) // kchs-to-modify 
						iAP = (int)( ( GetCur_DEX()*0.3f + GetCur_CON()*0.47f + GetCur_LEVEL()*0.1f + nItemQ * 0.8f) +
						( ( (iWeaponAP+ITEMGRADE_ATK(pRightWPN->GetGrade()) ) ) * ( GetCur_CON()*0.04f + GetCur_SENSE()*0.05f+29) / 30 ) );
					else
						iAP = (int)( ( GetCur_DEX()*0.4f + GetCur_CON()*0.5f + GetCur_LEVEL()*0.2f + nItemQ ) +
								( ( (iWeaponAP+ITEMGRADE_ATK(pRightWPN->GetGrade()) )+nItemQ*0.6f+8 ) * ( GetCur_CON()*0.03f + GetCur_SENSE()*0.05f+29) / 30 ) );
					break;

				case SHOT_TYPE_THROW :
					if( IsTAIWAN() ) // kchs-to-modify
						iAP = (int)( ( GetCur_STR()*0.32f + GetCur_CON()*0.45f + GetCur_LEVEL()*0.1f + nItemQ * 0.8f ) +
						( ( (iWeaponAP+ITEMGRADE_ATK(pRightWPN->GetGrade()) ) ) * ( GetCur_CON()*0.04f + GetCur_SENSE()*0.05f+29) / 30 ) );
					else
						iAP = (int)( ( GetCur_STR()*0.52f + GetCur_CON()*0.5f + GetCur_LEVEL()*0.2f + nItemQ ) +
						( ( (iWeaponAP+ITEMGRADE_ATK(pRightWPN->GetGrade()) )+nItemQ+12 ) * ( GetCur_CON()*0.04f + GetCur_SENSE()*0.05f+29) / 30 ) );
					break;
			} 
		} else {
			switch ( WEAPON_TYPE( pRightWPN->m_nItemNo ) / 10 ) {
				case 21 :	// ÇÑ¼Õ
				case 22 :	// ¾ç¼Õ		// ±ÙÁ¢ ¹«±â
					iAP = (int)( ( GetCur_STR()*0.75f + GetCur_LEVEL()*0.2f ) + ( (iWeaponAP+ITEMGRADE_ATK(pRightWPN->GetGrade()) ) * ( GetCur_STR()*0.05f+29) / 30.f ) );
					break;
				case 24 :				// ¸¶¹ý ¹«±â
					if ( 241 == WEAPON_TYPE( pRightWPN->m_nItemNo ) ) {	
						// ¸¶¹ý ÁöÆÎÀÌ.
						iAP = (int)( ( GetCur_STR()*0.4f + GetCur_INT()*0.4f + GetCur_LEVEL()*0.2f ) + ( (iWeaponAP+ITEMGRADE_ATK(pRightWPN->GetGrade()) ) * ( GetCur_INT()*0.05f + 29 ) / 30.f ) );
					} else {
						iAP = (int)( ( GetCur_INT()*0.6f + GetCur_LEVEL()*0.2f ) + ( (iWeaponAP+ITEMGRADE_ATK(pRightWPN->GetGrade()) ) * ( GetCur_SENSE()*0.1f + 26 ) / 27.f ) );
					}
					break;
				case 25 :				// Ä«Å¸¸£
					if ( 252 == WEAPON_TYPE( pRightWPN->m_nItemNo ) ) {	
						// ÀÌµµ·ù
						iAP = (int)( ( GetCur_STR()*0.63f + GetCur_DEX()*0.45f + GetCur_LEVEL()*0.2f ) + ( (iWeaponAP+ITEMGRADE_ATK(pRightWPN->GetGrade()) ) * ( GetCur_DEX()*0.05f + 25 ) / 26.f ) );
					} else {
						iAP = (int)( ( GetCur_STR()*0.42f + GetCur_DEX()*0.55f + GetCur_LEVEL()*0.2f ) + ( (iWeaponAP+ITEMGRADE_ATK(pRightWPN->GetGrade()) ) * ( GetCur_DEX()*0.05f + 20 ) / 21.f ) );
					}
					break;
				case 0  :				// ½ºÅ³¸Ç¼Õ °ø°Ý·Â : ¹«µµ°¡ Á÷¾÷¿¡ Æ¯ÇÑµÈ ½ºÅ³·Î, ÆÐ½Ãºê ¼º°ÝÀÇ ¸Ç¼Õ °ø°Ý·ÂÀ» Á¦°øÇÑ´Ù.
					iAP = (int)( GetCur_STR()*0.5f + GetCur_DEX()*0.3f + GetCur_LEVEL()*0.2f );
					break;
			}
		}

		// this->m_nPassiveAttackSpeed = this->GetPassiveSkillAttackSpeed( pRightWPN->m_nItemNo );

		iAP += this->m_iAddValue[ AT_ATK ];
		this->m_Battle.m_nATT = iAP + this->GetPassiveSkillAttackPower( iAP, pRightWPN->m_nItemNo );
	} 
	else // PAT Å¾½Â ¸ðµå
	{
		t_eSHOT ShotTYPE = pRightWPN->GetShotTYPE();
		/// ¼ö¸íÀÌ ´ÙÇÑ ¹«±â´Â ¹«±âÀÇ ±âº» °ø°Ý·ÂÀ» 0 À¸·Î..
		if ( pRightWPN->GetHEADER() && pRightWPN->GetLife() > 0 ) 
			iWeaponAP = WEAPON_ATTACK_POWER( pRightWPN->m_nItemNo );
		else
			iWeaponAP = 0;
		int iWeaponTERM = iWeaponAP + ITEMGRADE_ATK(pRightWPN->GetGrade()) + PAT_ITEM_ATK_POW(this->m_Inventory.m_ItemRIDE[ RIDE_PART_ARMS ].GetItemNO());

		if( IsTAIWAN() ) // 2005-07-25 Ãß°¡
		{
			if( ShotTYPE < MAX_SHOT_TYPE ) { // Å¸ÀÌ¿Ï ¿ø°Å¸® ¹«±â
				switch( ShotTYPE )
				{
				case SHOT_TYPE_ARROW :
					iAP = ( ( GetCur_DEX()*0.52f + GetCur_STR()*0.1f + GetCur_LEVEL()*0.1f + 3 ) + 
						iWeaponTERM *
						( GetCur_DEX()*0.04f + GetCur_SENSE()*0.03f + 29 ) / 60.f ) * 1.03f;
					break;
				case SHOT_TYPE_BULLET:
					iAP = ( (GetCur_DEX()*0.3f+GetCur_CON()*0.47+GetCur_LEVEL()*0.1f+6.4f) + iWeaponTERM * 
						( GetCur_CON()*0.04f+GetCur_SENSE()*0.05f+29) / 60.f ) * 1.032f;
					break;

				case SHOT_TYPE_THROW :
					iAP = ( (GetCur_STR()*0.32f + GetCur_CON()*0.45f+GetCur_LEVEL()*0.1f + 8) + iWeaponTERM *
						(GetCur_CON() * 0.04f + GetCur_SENSE() * 0.05f + 29) / 60.f ) * 1.033f ;
					break;
				}
			} 
			else // Å¸ÀÌ¿Ï ±Ù°Å¸® ¹«±â
			{
				switch ( WEAPON_TYPE( pRightWPN->m_nItemNo ) / 10 ) {
				case 21 :	// ÇÑ¼Õ
				case 22 :	// ¾ç¼Õ		// ±ÙÁ¢ ¹«±â
					iAP = ( (GetCur_STR()*0.75f + GetCur_LEVEL() * 0.2f) + iWeaponTERM *( GetCur_STR() * 0.05f + 29) / 60.f ) * 1.033f;
					break;
				case 24 :				// ¸¶¹ý ¹«±â
					{
						if( WEAPON_TYPE( pRightWPN->m_nItemNo ) == 241) {
							// ¸¶¹ý ÁöÆÎÀÌ
							iAP = ( (GetCur_STR()*0.4f + GetCur_INT()*0.4f + GetCur_LEVEL() * 0.2f) + iWeaponTERM *
								(GetCur_INT()*0.05f + 29) / 60.f ) * 1.03f;
							break;
						} else {	// ¸¶¹ýµµ±¸
							iAP = ( (GetCur_INT()*0.6f + GetCur_LEVEL() * 0.2f ) + iWeaponTERM * ( GetCur_SENSE() * 0.1f + 26 )/ 54.f ) * 1.025f;
						}
					}
					break;
				case 25 :				
					if ( 252 == WEAPON_TYPE( pRightWPN->m_nItemNo ) ) {	 // ÀÌµµ·ù
						iAP = ( (GetCur_STR() * 0.63f + GetCur_DEX() * 0.45f + GetCur_LEVEL() * 0.2f ) + iWeaponTERM  * 
							(GetCur_DEX() * 0.05f + 25 ) / 52.f ) * 1.032f;
					} else { // Ä«Å¸¸£
						iAP = ( (GetCur_STR() * 0.42f + GetCur_DEX() * 0.55f + GetCur_LEVEL() * 0.2f ) + iWeaponTERM * 
							(GetCur_DEX() * 0.05f + 20 ) / 42.f ) * 1.032f;
					}
					break;
				case 0  :
					// [(STR*0.5 + DEX*0.3 + LV*0.2) + (Ä«Æ®¹«±â°ø°Ý·Â)]/2+ ¾ÆÀÌÅÛ Áõ°¡Ä¡
					iAP = GetCur_STR() * 0.5f + GetCur_DEX() * 0.3f + GetCur_LEVEL() * 0.2f + 
						 PAT_ITEM_ATK_POW(this->m_Inventory.m_ItemRIDE[ RIDE_PART_ARMS ].GetItemNO()) / 2.f ;
					break;
				}
			}
			iAP += this->m_iAddValue[ AT_ATK ];
			this->m_Battle.m_nATT = iAP + this->GetPassiveSkillAttackPower( iAP, pRightWPN->m_nItemNo );
		}
		else {
			iAP = ( GetCur_LEVEL() * 3 ) + GetCur_CON() + PAT_ITEM_ATK_POW( this->m_Inventory.m_ItemRIDE[ RIDE_PART_ARMS ].GetItemNO() );
			this->m_Battle.m_nATT = iAP + this->m_iAddValue[ AT_ATK ];
		}

	}

	this->Cal_AruaATTACK();

	return this->m_Battle.m_nATT;
}

//-------------------------------------------------------------------------------------------------
float CUserDATA::Cal_RunSPEED ()
{
/*
	* ¶Û ¶§ÀÇ ÀÌµ¿ ¼Óµµ. (°È±â´Â µðÆúÆ® ¼Óµµ·Î ¸ðµç ¾Æ¹ÙÅ¸°¡ °°´Ù)
	* ÀåÂøµÇ¾î ÀÖ´Â ¹æ¾î±¸³ª ¹«±âÀÇ ¹«°ÔÀÇ ÇÕÀÌ ³ôÀ»¼ö·Ï ÀÌµ¿¼Óµµ°¡ ÀúÇÏµÈ´Ù.
	* DEX°¡ ³ôÀ»¼ö·Ï, ÀåÂøµÈ ½Å¹ßÀÇ ÀÌµ¿·ÂÀÌ ³ôÀ»¼ö·Ï ÀÌµ¿¼Óµµ°¡ ³ô¾ÆÁø´Ù.
	* SPE = { (½Å¹ßÀÇ ÀÌµ¿·Â + 15) * (DEX+400) - (ÀåÂøµÈ ¹«°Ô+50)*4 } / 70
*/
	float fMoveSpeed;
	if ( this->GetCur_MOVE_MODE() <= MOVE_MODE_RUN ) {
		short nItemSpeed, nItemNo;

		//nItemNo = this->GetCur_PartITEM(BODY_PART_BOOTS);
		nItemNo = this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_BOOTS ].GetItemNO();
		if ( nItemNo > 0 && this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_BOOTS ].GetLife() ) {
			nItemSpeed = BOOTS_MOVE_SPEED( nItemNo );
		} else {
			nItemSpeed = BOOTS_MOVE_SPEED( 0 );			/// ¼ö¸íÀÌ ´ÙÇÑ ½Å¹ßÀº ¸Ç¹ß ¼Óµµ·Î ´ëÃ¼...
		}

		//nItemNo = this->GetCur_PartITEM(BODY_PART_KNAPSACK);
		nItemNo = this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_KNAPSACK ].GetItemNO();
		if ( nItemNo > 0 && this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_KNAPSACK ].GetLife() ) {
			nItemSpeed += BACKITEM_MOVE_SPEED( nItemNo );
		}
		nItemSpeed += 20;
		if ( IsTAIWAN() ) {
			// SPE = {(½Å¹ß ÀÌµ¿·Â + µîÀåÂø ÀÌµ¿·Â+20)*5
			//fMoveSpeed = nItemSpeed * 5 + this->m_iAddValue[ AT_SPEED ];
			fMoveSpeed = nItemSpeed * ( GetCur_DEX() + 500.f ) / 100.f + this->m_iAddValue[ AT_SPEED ];
		} else {
			fMoveSpeed = nItemSpeed * ( GetCur_DEX() + 500.f ) / 100.f + this->m_iAddValue[ AT_SPEED ];
		}

		float fPsvSpd = GetPassiveSkillValue( AT_PSV_MOV_SPD ) + fMoveSpeed * GetPassiveSkillRate( AT_PSV_MOV_SPD )/100.f;
		return ( fMoveSpeed + fPsvSpd );
	} else {
		tagITEM *pLEG = &this->m_Inventory.m_ItemRIDE[ RIDE_PART_LEG	];
		tagITEM *pENG = &this->m_Inventory.m_ItemRIDE[ RIDE_PART_ENGINE	];

		if ( pLEG->GetLife() && pENG->GetLife() )
			fMoveSpeed = PAT_ITEM_MOV_SPD( pLEG->GetItemNO() ) * PAT_ITEM_MOV_SPD( pENG->GetItemNO() ) / 10.f;
		else
			fMoveSpeed = 200;	/// ÀÌµ¿ ÃÖ¼Ò ¼Óµµ

		fMoveSpeed += this->m_iAddValue[ AT_SPEED ];
		if ( this->Get_WeightRATE() >= WEIGHT_RATE_WALK && fMoveSpeed > 300.f )
			fMoveSpeed = 300.f;
	}
	this->Cal_AruaRunSPD ();
	
	return fMoveSpeed;
}

//-------------------------------------------------------------------------------------------------
// ÆÐ½Ãºê ½ºÅ³¿¡¼­ ÂüÁ¶ÇÒ ¹öÇÁÀüÀÇ ´É·ÂÄ¡...
/*
int CUserDATA::GetOri_AbilityValue (WORD wType)
{
	switch( wType ) {
		// ÆÐ½Ãºê ½ºÅ³ ÂüÁ¶½Ã ±âº» ´É·ÂÄ¡´Â ¹Ù·Î ¹è¿­·Î °è»êÇÑ´Ù.
		//case AT_STR	:		return GetCur_STR ();
		//case AT_DEX	:		return GetCur_DEX ();
		//case AT_INT	:		return GetCur_INT ();
		//case AT_CON	:		return GetCur_CON ();
		//case AT_CHARM	:		return GetCur_CHARM ();
		//case AT_SENSE	:		return GetCur_SENSE ();

		case AT_ATK		:		return GetDef_ATK ();
		case AT_DEF		:		return GetDef_DEF ();
		case AT_HIT		:		return GetDef_HIT ();
		case AT_RES		:		return GetDef_RES ();
		case AT_AVOID	:		return GetDef_AVOID ();
		case AT_SPEED	:		return (int)GetCur_MOVE_SPEED ();
		case AT_ATK_SPD :		return GetDef_ATK_SPD ();
		case AT_WEIGHT	:		return GetDef_WEIGHT ();
		case AT_CRITICAL   :	return GetDef_CRITICAL ();

		case AT_RECOVER_HP :	
		case AT_RECOVER_MP :	break;

		case AT_MAX_HP :		return GetDef_MaxHP ();
		case AT_MAX_MP :		return GetDef_MaxMP ();
	}

	return 0;
}
*/

//-------------------------------------------------------------------------------------------------
bool CUserDATA::Set_AbilityValue (WORD nType, int iValue)
{
	switch( nType ) 
	{
		case AT_SEX			:	SetCur_SEX ( iValue );		return true;
		case AT_FACE		:	SetCur_FACE( iValue );		return true;
		case AT_HAIR		:	SetCur_HAIR( iValue );		return true;

		case AT_CLASS		:	SetCur_JOB( iValue );		return true;	
		/*
		case AT_BIRTHSTONE :		
		case AT_RANK	:			
		case AT_FAME	:			
		case AT_FACE	:
		case AT_HAIR	:			
			break;
		*/
		case AT_UNION		:	SetCur_UNION( iValue );		return true;

		case AT_STR			:	SetDef_STR  ( iValue );		return true;
		case AT_DEX			:	SetDef_DEX  ( iValue );		return true;
		case AT_INT			:	SetDef_INT  ( iValue );		return true;
		case AT_CON			:	SetDef_CON	( iValue );		return true;
		case AT_CHARM		:	SetDef_CHARM( iValue );		return true;
		case AT_SENSE		:	SetDef_SENSE( iValue );		return true;

		case AT_HP			:	SetCur_HP( iValue );		return true;
		case AT_MP			:	SetCur_MP( iValue );		return true;
		case AT_STAMINA		:	SetCur_STAMINA( iValue );	return true;

		/*
		case AT_ATK		:			
		case AT_DEF		:			
		case AT_HIT		:			
		case AT_RES		:			
		case AT_AVOID	:			
		case AT_SPEED	:			
		case AT_ATK_SPD :		
		case AT_WEIGHT	:
		case AT_CRITICAL   :
		case AT_RECOVER_HP :
		case AT_RECOVER_MP :
		*/
		case AT_LEVEL		:	SetCur_LEVEL( (WORD)iValue );	return true;		
		case AT_EXP			:	SetCur_EXP( iValue );			return true;
		/*
		case AT_BONUSPOINT	:
		case AT_SKILLPOINT	:
		case AT_CHAOS	:
		case AT_PK_LEV	:		

		case AT_HEAD_SIZE :		
		case AT_BODY_SIZE :		
		// case AT_BATTLE_LEV :
			break;
		*/

		case AT_PK_FLAG :	SetCur_PK_FLAG( iValue );			return true;
		case AT_TEAM_NO :	SetCur_TeamNO( iValue );			return true;	

		default :
			if ( nType >= AT_UNION_POINT1 && nType <= AT_UNION_POINT10 ) {
				SetCur_JoHapPOINT( nType-AT_UNION_POINT1 + 1, iValue );
			}
	}

	return false;
}

void CUserDATA::Add_AbilityValue (WORD wType, int iValue)
{
	switch( wType ) {
		/*
		case AT_BIRTHSTONE :
		case AT_CLASS	:			
		case AT_UNION	:			
		case AT_RANK	:			
		case AT_FAME	:			

		case AT_FACE	:
		case AT_HAIR	:			
			break;
		*/
		case AT_STR			:	SetDef_STR  ( GetDef_STR()   + iValue );	return;
		case AT_DEX			:	SetDef_DEX  ( GetDef_DEX()   + iValue );	return;
		case AT_INT			:	SetDef_INT  ( GetDef_INT()   + iValue );	return;
		case AT_CON			:	SetDef_CON	( GetDef_CON()   + iValue );	return;
		case AT_CHARM		:	SetDef_CHARM( GetDef_CHARM() + iValue );	return;
		case AT_SENSE		:	SetDef_SENSE( GetDef_SENSE() + iValue );	return;

		case AT_HP			:	AddCur_HP( iValue );						return;
		case AT_MP			:	AddCur_MP( iValue );						return;
		case AT_STAMINA		:	AddCur_STAMINA( iValue );					return;

		case AT_GUILD_SCORE	:	AddClanSCORE( iValue );						return;

		case AT_EXP			:	
			AddCur_EXP(iValue);							
#ifdef	__INC_WORLD
			g_LOG.CS_ODS( 0xffff, "			>>>> ´É·ÂÄ¡ º¸»óÁß °æÇèÄ¡ :: %d \n", iValue );
#endif
			return;
		case AT_BONUSPOINT	:	AddCur_BonusPOINT(iValue);					return;
		case AT_SKILLPOINT	:	AddCur_SkillPOINT(iValue);					return;

		case AT_MONEY		:	
			if ( iValue < 0 && iValue > GetCur_MONEY() )
				break;
			Add_CurMONEY(iValue);	
#ifdef	__INC_WORLD
			g_LOG.CS_ODS( 0xffff, "			>>>> ´É·ÂÄ¡ º¸»óÁß ÁÙ¸® º¸»ó:: %d \n", iValue );
#endif
			break;

		default :
			if ( wType >= AT_UNION_POINT1 && wType <= AT_UNION_POINT10 ) {
				AddCur_JoHapPOINT( wType-AT_UNION_POINT1 +1, iValue );
			}
	}
}


//-------------------------------------------------------------------------------------------------
bool CUserDATA::Check_JobCollection( short nClassStbIDX )
{
	if ( 0 == CLASS_INCLUDE_JOB( nClassStbIDX, 0 ) ) {
		// Ã¹¹øÂ° ÀÔ·ÂµÈ °ªÀÌ ¾øÀ¸¸é Åë°ú...
		return true;
	}

	for (short nI=0; nI<CLASS_INCLUDE_JOB_CNT; nI++) {
		if ( 0 == CLASS_INCLUDE_JOB( nClassStbIDX, nI ) ) {
			// ÀÔ·ÂµÈ °ªÁß¿£ ¾ø¾ú´Ù..
			return false;
		}

		if ( this->GetCur_JOB() == CLASS_INCLUDE_JOB( nClassStbIDX, nI ) )
			return true;
	}

	return false;
}

//-------------------------------------------------------------------------------------------------
bool CUserDATA::Check_PatEquipCondition( tagITEM &sITEM, short nEquipIdx )
{
	if( sITEM.GetTYPE() != ITEM_TYPE_RIDE_PART || 0 == sITEM.GetItemNO() )
		return false;

	if ( RIDE_PART_ENGINE != PAT_ITEM_PART_IDX( sITEM.GetItemNO() ) && sITEM.GetLife() < 1 )
		return false;

	if( 0 != PAT_ITEM_NEED_ABILITY_IDX( sITEM.GetItemNO() ) )
	{
		int iValue = GetCur_AbilityValue( PAT_ITEM_NEED_ABILITY_IDX( sITEM.GetItemNO() )  );
		if ( iValue < PAT_ITEM_NEED_ABILITY_VALUE( sITEM.GetItemNO() ) )
			return false;
	}

	// ÀåÂø À§Ä¡°¡ ¸Â³Ä ??
	if ( nEquipIdx != PAT_ITEM_PART_IDX( sITEM.GetItemNO() ) )
		return false;

#ifdef __KCHS_BATTLECART__
	short nClassStbIDX = PAT_ITEM_EQUIP_REQUIRE_CLASS( sITEM.GetItemNO() );
	if( PAT_ITEM_EQUIP_REQUIRE_CLASS( sITEM.GetItemNO() ))
	{
		for (short nI=0; nI<CLASS_INCLUDE_JOB_CNT; nI++) {
			if ( 0 == CLASS_INCLUDE_JOB( nClassStbIDX, nI ) ) {
				// ÀÔ·ÂµÈ °ªÁß¿£ ¾ø¾ú´Ù..
				return false;
			}

			if ( this->GetCur_JOB() == CLASS_INCLUDE_JOB( nClassStbIDX, nI ) )
				return true;
		}
	}
#endif
	
	return true;
}
//-------------------------------------------------------------------------------------------------
const short s_Equip2ItemTYPE[ MAX_EQUIP_IDX ] = {
	0,						// EQUIP_IDX_NULL
	ITEM_TYPE_FACE_ITEM,	// EQUIP_IDX_FACE_ITEM	1,
	ITEM_TYPE_HELMET,		// EQUIP_IDX_HELMET		2,
	ITEM_TYPE_ARMOR,		// EQUIP_IDX_ARMOR,		3
	ITEM_TYPE_KNAPSACK,		// EQUIP_IDX_KNAPSACK	4,
	ITEM_TYPE_GAUNTLET,		// EQUIP_IDX_GAUNTLET	5,
	ITEM_TYPE_BOOTS,		// EQUIP_IDX_BOOTS,		6
	ITEM_TYPE_WEAPON,		// EQUIP_IDX_WEAPON_R	7,
	ITEM_TYPE_SUBWPN,		// EQUIP_IDX_WEAPON_L	8	
	ITEM_TYPE_JEWEL,		// EQUIP_IDX_NECKLACE	9
	ITEM_TYPE_JEWEL,		// EQUIP_IDX_RING		10
	ITEM_TYPE_JEWEL,		// EQUIP_IDX_EARRING	11
} ;	
bool CUserDATA::Check_EquipCondition (tagITEM &sITEM, short nEquipIdx)
{
	if ( sITEM.GetTYPE() == 0 || sITEM.GetLife() < 1 )	/// ¼ö¸íÀÌ ´ÙÇÑ ¾ÆÀÌÅÛÀº ÀåÂø ºÒ°¡~
		return false;

	// ÀåÂø ¾ÆÀÌÅÛ Å¸ÀÔÀ¸·Î ÀåÂø À§Ä¡°¡ ¸Â³Ä???
	if ( sITEM.GetTYPE() != s_Equip2ItemTYPE[ nEquipIdx ] )
		return false;

	if ( ITEM_TYPE_JEWEL == sITEM.GetTYPE() ) {
		// ¹ÝÁö, ¸ñ°ÉÀÌ, ±Í°ÉÀÌ -- ¹ÝÁö·Î ¸ñ°ÉÀÌ,±Í°ÉÀÌ¿¡ ÀåÂøÇÏ´Â ÇØÅ· ¹æÁö...
		switch( ITEM_TYPE( sITEM.GetTYPE(),sITEM.GetItemNO() ) ) {
			case JEWEL_ITEM_RING	:	if ( nEquipIdx != EQUIP_IDX_RING )		return false;
				break;
			case JEWEL_ITEM_NECKLACE:	if ( nEquipIdx != EQUIP_IDX_NECKLACE )	return false;
				break;
			case JEWEL_ITEM_EARRING	:	if ( nEquipIdx != EQUIP_IDX_EARRING	)	return false;
				break;
		}
	}

	/// Ã¼Å©ÇÒ Á÷¾÷ÀÌ ÀÖ´Â°¡ ?
	if ( !Check_JobCollection( ITEM_EQUIP_REQUIRE_CLASS( sITEM.m_cType, sITEM.m_nItemNo ) ) ) 
		return false;

	///Á¶ÇÕ Ã¼Å©<-2004.3.18.nAvy
	int		iCheckCount = 0;
	bool	bCorrect	= false;
	for( int nl = 0; nl < ITEM_EQUIP_REQUIRE_UNION_CNT; nl++ )
	{
		if( 0 != ITEM_EQUIP_REQUIRE_UNION( sITEM.GetTYPE(), sITEM.GetItemNO(), nl ))
		{
			++iCheckCount;
			if( ITEM_EQUIP_REQUIRE_UNION( sITEM.GetTYPE(), sITEM.GetItemNO(), nl ) == this->GetCur_JOHAP() )
			{
				bCorrect = true;
				break;
			}
		}
	}

	if( iCheckCount && !bCorrect)
		return false;
	///->

	int iValue;
	for( short nI = 0; nI < ITEM_NEED_DATA_CNT; ++nI )
	{
		if ( ITEM_NEED_DATA_TYPE ( sITEM.m_cType, sITEM.m_nItemNo, nI) ) {
			iValue = GetCur_AbilityValue( ITEM_NEED_DATA_TYPE ( sITEM.m_cType, sITEM.m_nItemNo, nI) );
			if ( iValue < ITEM_NEED_DATA_VALUE( sITEM.m_cType, sITEM.m_nItemNo , nI) )
				return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------------
/// USEITEM_NEED_DATA_TYPE : ¾î¶² ´É·ÂÄ¡¸¦ ºñ±³ÇÒ°ÍÀÎ°¡?
/// USEITEM_NEED_DATA_VALUE : ÇÊ¿ä´É·ÂÄ¡.
/// USEITEM_ADD_DATA_TYPE : Àû¿ë´É·Â
/// USEITEM_ADD_DATA_VALUE : Àû¿ë ¼öÄ¡
/*
bool CUserDATA::Use_ITEM (WORD wUseItemNO)
{
#ifndef	__SERVER
	int iValue = GetCur_AbilityValue( USEITEM_NEED_DATA_TYPE(wUseItemNO) );
	if ( iValue < USEITEM_NEED_DATA_VALUE(wUseItemNO) )
		return false;
#endif

	// ÇöÀç ¹«°Ô °¨¼Ò..
	//m_Battle.m_nWEIGHT -= ITEM_WEIGHT( ITEM_TYPE_USE, wUseItemNO );

	if ( 0 == USEITME_STATUS_STB( wUseItemNO ) ) 
	{
		// Áö¼ÓÇü »óÅÂ·Î ¼Ò¸ðµÇ´Â ¾ÆÀÌÅÛÀÌ ¾Æ´Ï¸é...
		Add_AbilityValue( USEITEM_ADD_DATA_TYPE( wUseItemNO ), USEITEM_ADD_DATA_VALUE( wUseItemNO ) );
	}

	return true;
}
*/

///
/// ¸ö¿¡ ºÙ¾î¾ßÇÒ ¾ÆÀÌÅÛµéÀº ¸ö¿¡ ºÙ¿© ÁØ´Ù.
/// ¾Æ´Ñ ¾ÆÀÌÅÛÀº ..?
///
///

bool CUserDATA::Set_EquipITEM (short nEquipIDX, tagITEM &sITEM)
{
	// ºñ¾î ÀÖÁö ¾ÊÀ½?
	//if ( m_Inventory.m_ItemEQUIP[ nEquipIDX ].m_cType )
	//	return false;
	//m_Inventory.m_ItemEQUIP[ nEquipIDX ] = sITEM;
	this->Set_ITEM( nEquipIDX, sITEM );
//	m_Inventory.AppendITEM( nEquipIDX, sITEM, m_Battle.m_nWEIGHT );

	switch ( nEquipIDX ) {
		case EQUIP_IDX_FACE_ITEM :
			SetCur_PartITEM( BODY_PART_FACE_ITEM,	sITEM );
			break;
		case EQUIP_IDX_HELMET :
			SetCur_PartITEM( BODY_PART_HELMET,		sITEM );
			break;
		case EQUIP_IDX_ARMOR :
			SetCur_PartITEM( BODY_PART_ARMOR,		sITEM );
			break;
		case EQUIP_IDX_KNAPSACK :
			SetCur_PartITEM( BODY_PART_KNAPSACK,	sITEM );			
			break;		
		case EQUIP_IDX_GAUNTLET :
			SetCur_PartITEM( BODY_PART_GAUNTLET,	sITEM );
			break;
		case EQUIP_IDX_BOOTS :
			SetCur_PartITEM( BODY_PART_BOOTS,		sITEM );
			break;
		case EQUIP_IDX_WEAPON_R :
			SetCur_PartITEM( BODY_PART_WEAPON_R,	sITEM );
			break;
		case EQUIP_IDX_WEAPON_L :
			SetCur_PartITEM( BODY_PART_WEAPON_L,	sITEM );
			break;
	} 

	// ÀåÂø ¾ÆÀÌÅÛÀÌ ¹Ù²î¾úÀ¸´Ï... ÇØÁà¾ßÁö...
	this->UpdateCur_Ability ();
	this->InitPassiveSkill ();

#ifndef	__SERVER
	/// ¸ðµ¨ µ¥ÀÌÅÍ °»½Å
	UpdateModelData();
#endif

	return true;
}


//-------------------------------------------------------------------------------------------------
// ½ºÅ³ »ç¿ë½Ã ¼Ò¸ðµÉ ´É·ÂÄ¡ÀÇ ¼öÄ¡¸¦ ¾ò´Â´Ù.
int CUserDATA::Skill_ToUseAbilityVALUE( short nSkillIDX, short nPropertyIDX )
{
	int iValue = SKILL_USE_VALUE( nSkillIDX, nPropertyIDX );
	if ( AT_MP == SKILL_USE_PROPERTY(nSkillIDX, nPropertyIDX) ) {
		return (int)( iValue * this->GetCur_RateUseMP() );
	}

#ifdef __KCHS_BATTLECART__
	if( AT_PATHP == SKILL_USE_PROPERTY(nSkillIDX, nPropertyIDX) )
	{
		if( GetCur_PatHP() < iValue && GetCur_PatHP() > 0 )
			return GetCur_PatHP();		// __KCHS_BATTLECART__ ÀÌ·¸°Ô ÇÏÁö ¾ÊÀ¸¸é Ä«Æ® Ã¼·ÂÀº ¿µ¿øÈ÷ 0ÀÌ ¾È µÉ¼ö ÀÖ°í. ±×·¡¼­ ÄðÅ¸ÀÓ Àý´ë·Î ¾È µ¹¾Æ°£´Ù.
	}
#endif

	return iValue;
}

//-------------------------------------------------------------------------------------------------
bool CUserDATA::Skill_UseAbilityValue( short nSkillIDX )
{
	int iValue;
	for (short nI=0; nI<SKILL_USE_PROPERTY_CNT; nI++) {
		if ( 0 == SKILL_USE_PROPERTY( nSkillIDX, nI ) )
			break;

		iValue = Skill_ToUseAbilityVALUE( nSkillIDX, nI );

		switch( SKILL_USE_PROPERTY( nSkillIDX, nI ) ) {
			case AT_STAMINA :	this->SetCur_STAMINA( GetCur_STAMINA()-iValue );	break;
			case AT_HP		:	this->SubCur_HP( iValue );							break;
			case AT_MP		:	this->SubCur_MP( iValue );							break;
			case AT_EXP		:	this->SetCur_EXP( GetCur_EXP() - iValue );			break;
			case AT_MONEY	:	this->SetCur_MONEY( GetCur_MONEY() - iValue );		break;

			case AT_FUEL	:	this->SubCur_FUEL( iValue );						break;
#ifdef __KCHS_BATTLECART__
			case AT_PATHP	:	this->SubCur_PatHP( iValue );						break;
#endif
		}
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
int CUserDATA::Skill_GetAbilityValue( short nAbilityType )
{
	switch( nAbilityType ) 
	{
		case AT_LEVEL	:		return this->GetCur_LEVEL();

		case AT_STR		:		return GetCur_STR ();
		case AT_DEX		:		return GetCur_DEX ();
		case AT_INT		:		return GetCur_INT ();
		case AT_CON		:		return GetCur_CON ();
		case AT_CHARM	:		return GetCur_CHARM ();
		case AT_SENSE	:		return GetCur_SENSE ();

		case AT_HP		:		return ( GetCur_HP ()-1 );	//// ½ÇÁ¦º¸´Ù 1ÀÛ°Ô...ÇÇÀÎ °æ¿ì ¼Ò¸ð½Ã ´Ù»©¸é µØÁø´Ù..
		case AT_MP		:		return GetCur_MP ();
		case AT_EXP		:		return GetCur_EXP();
		case AT_MONEY	:		
			if ( GetCur_MONEY () > MAX_INT )
				return MAX_INT;
			return (int)GetCur_MONEY();
		case AT_STAMINA :		return GetCur_STAMINA ();


		case AT_FUEL	:		return GetCur_FUEL ();
		// case AT_SKILLPOINT :	return GetCur_SkillPOINT ();
#ifdef __KCHS_BATTLECART__
		case AT_PATHP	:		return GetCur_PatHP ();
#endif
	}

	return -999;
}

//-------------------------------------------------------------------------------------------------
short CUserDATA::Skill_FindEmptySlot( short nSkillIDX )
{
	short nSlot=-1;
	char cPageIDX = SKILL_TAB_TYPE( nSkillIDX );

	for (short nI=0; nI<MAX_LEARNED_SKILL_PER_PAGE; nI++) {
		if ( nSkillIDX == m_Skills.m_nPageIndex[ cPageIDX ][ nI ] )
			return ( cPageIDX*MAX_LEARNED_SKILL_PER_PAGE + nI );

		if ( nSlot < 0 && 0 == m_Skills.m_nPageIndex[ cPageIDX ][ nI ] ) {
			nSlot = cPageIDX*MAX_LEARNED_SKILL_PER_PAGE + nI;
		}
	}

	return nSlot;
}


//-------------------------------------------------------------------------------------------------
short CUserDATA::Skill_FindLearnedLevel( short nSkillIDX )
{
	short nFindSlot;

	nFindSlot = this->Skill_FindLearnedSlot( nSkillIDX );
	if ( nFindSlot >= 0 ) {
		return SKILL_LEVEL( this->m_Skills.m_nSkillINDEX[ nFindSlot ] );
	}

	return -1;
}

short CUserDATA::Skill_FindLearnedSlot ( short nSkillIDX )
{
	char cPageIDX = SKILL_TAB_TYPE( nSkillIDX );
	short nI, n1LevSkillIDX = SKILL_1LEV_INDEX( nSkillIDX );

	for (nI=0; nI<MAX_LEARNED_SKILL_PER_PAGE; nI++) {
		if ( 0 == m_Skills.m_nPageIndex[ cPageIDX ][ nI ] )
			continue;

		if ( n1LevSkillIDX == SKILL_1LEV_INDEX( m_Skills.m_nPageIndex[ cPageIDX ][ nI ] ) ) {
			return ( cPageIDX*MAX_LEARNED_SKILL_PER_PAGE + nI );
		}
	}

	return -1;
}


//-------------------------------------------------------------------------------------------------
bool CUserDATA::Skill_CheckJOB( short nSkillIDX )
{
	// 2004.3.18... icarus
	// Á÷¾÷ Ã¼Å©...
	if ( !Check_JobCollection( SKILL_AVAILBLE_CLASS_SET(nSkillIDX) )	)
		return false;

	// ¼Ò¼Ó Á¶ÇÕ Ã¼Å©...
	if ( 0 == SKILL_AVAILBLE_UNION(nSkillIDX, 0 ) ) {
		// Ã¹¹øÂ° ÀÔ·ÂµÈ °ªÀÌ ¾øÀ¸¸é Åë°ú...
		return true;
	}

	for (short nI=0; nI<SKILL_AVAILBLE_UNION_CNT; nI++) {
		if ( 0 == SKILL_AVAILBLE_UNION(nSkillIDX, nI ) ) {
			// ÀÔ·ÂµÈ °ªÁß¿£ ¾ø¾ú´Ù..
			return false;
		}
		
		if ( this->GetCur_JOHAP() == SKILL_AVAILBLE_UNION(nSkillIDX, nI) )
			return true;
	}
	return false;
}
bool CUserDATA::Skill_CheckLearnedSKILL( short nSkillIDX )
{
	if ( SKILL_NEED_SKILL_INDEX( nSkillIDX, 0 ) ) {
		short nValue;
		for (short nI=0; nI<SKILL_NEED_SKILL_CNT; nI++) {
			nValue = SKILL_NEED_SKILL_INDEX( nSkillIDX, nI );
			if ( 0 == nValue )
				return true;// goto _SKILL_IS_MATCH;

			nValue = this->Skill_FindLearnedLevel( nValue );
			if ( nValue < SKILL_NEDD_SKILL_LEVEL( nSkillIDX, nI ) )
				return false;	// RESULT_SKILL_LEARN_NEED_SKILL;
		}

//_SKILL_IS_MATCH:	;
	}
	return true;
}
bool CUserDATA::Skill_CheckNeedABILITY( short nSkillIDX )
{
	if ( SKILL_NEED_ABILITY_TYPE( nSkillIDX,0 ) ) {
		int iValue;
		for (short nI=0; nI<SKILL_NEED_ABILITY_TYPE_CNT; nI++) {
			if ( 0 == SKILL_NEED_ABILITY_TYPE( nSkillIDX,nI ) )
				break;

			iValue = Skill_GetAbilityValue( SKILL_NEED_ABILITY_TYPE( nSkillIDX,nI ) );
			if ( iValue < SKILL_NEED_ABILITY_VALUE(nSkillIDX,nI) )
				return false;//RESULT_SKILL_LEARN_NEED_ABILITY;
		}
	}

	return true;
}

// ½ºÅ³ ½Àµæ Á¶°Ç ...
// 1. Á÷¾÷ Á¶°Ç..
// 2. º¸À¯ÇÑ ½ºÅ³ ¸ñ·Ï °Ë»ö...
// 3. Á¶°Ç ´É·ÂÄ¡ ÆÇ´Ü...
BYTE CUserDATA::Skill_LearnCondition( short nSkillIDX )
{
	if ( nSkillIDX < 1 || nSkillIDX >= g_SkillList.Get_SkillCNT() )
		return RESULT_SKILL_LEARN_INVALID_SKILL;

	// 0. ÀÌ¹Ì º¸À¯ÇÑ ½ºÅ³ÀÎÁö ÆÇ´Ü..
	if ( Skill_FindLearnedSlot( nSkillIDX ) >= 0 )
		return RESULT_SKILL_LEARN_FAILED;

	// 1. ½ºÅ³ Æ÷ÀÎÆ® ºÎÁ·...
	if ( this->GetCur_SkillPOINT() < SKILL_NEED_LEVELUPPOINT(nSkillIDX) )
		return RESULT_SKILL_LEARN_OUTOFPOINT;

	// 2. Á÷¾÷ Á¶°Ç Ã¼Å©
	if ( !this->Skill_CheckJOB( nSkillIDX ) ) {
		return RESULT_SKILL_LEARN_NEED_JOB;
	}

	// 3. ½ÀµæÇÑ ½ºÅ³·¹º§ ÆÇ´Ü...
	if ( !this->Skill_CheckLearnedSKILL( nSkillIDX ) ) {
		return RESULT_SKILL_LEARN_NEED_SKILL;
	}

	// 4. Á¶°Ç ´É·ÂÄ¡ ÆÇ´Ü...
	if ( !this->Skill_CheckNeedABILITY( nSkillIDX ) ) {
		return RESULT_SKILL_LEARN_NEED_ABILITY;
	}

	return RESULT_SKILL_LEARN_SUCCESS;
}


//-------------------------------------------------------------------------------------------------
// ½ºÅ³ ·¹º§¾÷ Á¶°Ç ...
// 1. ÇÊ¿ä ½ºÅ³ Æ÷ÀÎÆ®..
// 2. Á¶°Ç ´É·ÂÄ¡ ÆÇ´Ü.
BYTE CUserDATA::Skill_LevelUpCondition( short nCurLevelSkillIDX, short nNextLevelSkillIDX )
{
	if ( nNextLevelSkillIDX >= g_SkillList.Get_SkillCNT() ) {
		// ´õÀÌ»ó ·¹º§¾÷ ÇÒ¼ö ¾ø´Ù.
		return RESULT_SKILL_LEVELUP_FAILED;
	}

/*
	½ºÅ³ ·¹º§¾÷ Á¦ÇÑ »èÁ¦...
	if ( SKILL_LEVEL( nCurLevelSkillIDX ) >= 10 ) {
		// ´õÀÌ»ó ·¹º§¾÷ ÇÒ¼ö ¾ø´Ù.
		return RESULT_SKILL_LEVELUP_FAILED;
	}
*/
	// °°Àº Á¾·ùÀÇ ½ºÅ³ÀÌ°í ¹è¿ì·Á´Â ·¹º§ÀÌ ÇöÀç ·¹º§ÀÇ ´ÙÀ½ ·¹º§ÀÎ°¡ ??
	if ( SKILL_1LEV_INDEX( nCurLevelSkillIDX) != SKILL_1LEV_INDEX( nNextLevelSkillIDX ) ||
		 SKILL_LEVEL( nCurLevelSkillIDX )+1   != SKILL_LEVEL( nNextLevelSkillIDX ) ) {
		return RESULT_SKILL_LEVELUP_FAILED;
	}

	// 1. ½ºÅ³ Æ÷ÀÎÆ® ºÎÁ·...
	if ( this->GetCur_SkillPOINT() < SKILL_NEED_LEVELUPPOINT(nNextLevelSkillIDX) )
		return RESULT_SKILL_LEVELUP_OUTOFPOINT;

	// 2. Á÷¾÷ Á¶°Ç Ã¼Å©
	if ( !this->Skill_CheckJOB( nNextLevelSkillIDX ) ) {
		return RESULT_SKILL_LEVELUP_NEED_JOB;
	}

	// 3. ½ÀµæÇÑ ½ºÅ³·¹º§ ÆÇ´Ü...
	if ( !this->Skill_CheckLearnedSKILL( nNextLevelSkillIDX ) ) {
		return RESULT_SKILL_LEVELUP_NEED_SKILL;
	}

	// 4. Á¶°Ç ´É·ÂÄ¡ ÆÇ´Ü...
	if ( !this->Skill_CheckNeedABILITY( nNextLevelSkillIDX ) ) {
		return RESULT_SKILL_LEVELUP_NEED_ABILITY;
	}

	return RESULT_SKILL_LEVELUP_SUCCESS;
}


#ifndef __SERVER
#include "Game.h"
#endif 
//-------------------------------------------------------------------------------------------------
// ½ºÅ³ »ç¿ë½Ã Á¶°Ç ...
// 1. ¼Ò¸ð ¼öÄ¡..
// 2. ¹«±â
bool CUserDATA::Skill_ActionCondition( short nSkillIDX )
{
	short nI;

	if ( SKILL_TYPE( nSkillIDX ) >= SKILL_TYPE_07 &&
		 SKILL_TYPE( nSkillIDX ) <= SKILL_TYPE_13 ) {
		if ( FLAG_ING_DUMB & this->GetCur_IngStatusFLAG() ) 
		{
#ifndef __SERVER
			AddMsgToChatWND( STR_CANT_CASTING_STATE, g_dwRED ,CChatDLG::CHAT_TYPE_SYSTEM);
#endif
			// º¡¾î¸® »óÅÂ¿¡¼­ »ç¿ëÇÒ¼ö ¾ø´Â ½ºÅ³ÀÌ´Ù.
			return false;
		}
	} else
	if ( SKILL_TYPE_14 == SKILL_TYPE( nSkillIDX ) ) {
		if ( FLAG_ING_DUMB & this->GetCur_IngStatusFLAG() ) 
		{
#ifndef __SERVER
			AddMsgToChatWND( STR_CANT_CASTING_STATE, g_dwRED ,CChatDLG::CHAT_TYPE_SYSTEM);
#endif
			// º¡¾î¸® »óÅÂ¿¡¼­ »ç¿ëÇÒ¼ö ¾ø´Â ½ºÅ³ÀÌ´Ù.
			return false;
		}

		short nNeedSummonCNT = NPC_NEED_SUMMON_CNT( SKILL_SUMMON_PET(nSkillIDX) ); 
		if ( this->GetCur_SummonCNT() + nNeedSummonCNT > this->GetMax_SummonCNT() ) {
			// ÃÖ´ë ¼ÒÈ¯ ¸÷ °¡´É °¹¼ö ÃÊ°ú...
#ifndef __SERVER
			AddMsgToChatWND( STR_CANT_SUMMON_NPC, g_dwRED ,CChatDLG::CHAT_TYPE_SYSTEM);
#endif
			return false;
		}
	}

	// 1. ¸¶³ª or .....
	int iCurValue;
	for (nI=0; nI<SKILL_USE_PROPERTY_CNT; nI++) 
	{
		if ( 0 == SKILL_USE_PROPERTY( nSkillIDX, nI ) )
			break;

		iCurValue = this->Skill_GetAbilityValue( SKILL_USE_PROPERTY( nSkillIDX, nI ) );
		if ( iCurValue < this->Skill_ToUseAbilityVALUE( nSkillIDX, nI ) )
		{
#ifndef __SERVER
			AddMsgToChatWND( STR_NOT_ENOUGH_PROPERTY, g_dwRED ,CChatDLG::CHAT_TYPE_SYSTEM);
#endif
			return false;
		}
	}

	// 2.
	short nNeedWPN, nRWPN, nLWPN, nKnapsack;
	
	if ( this->GetCur_MOVE_MODE() <= MOVE_MODE_RUN ) {
		nRWPN = WEAPON_TYPE( this->GetCur_R_WEAPON() ); 
		nLWPN = SUBWPN_TYPE( this->GetCur_L_WEAPON() );
		nKnapsack = BACKITEM_TYPE( this->GetCur_KNAPSACK() );
	} else {
		// ž�½ÿ��� �ʿ� �ٵ� üũ...
		nRWPN = PAT_ITEM_TYPE( this->GetCur_PET_BODY() );
		nLWPN = -1;
	}


	for (nI=0; nI<SKILL_NEED_WEAPON_CNT; nI++) 
	{
		nNeedWPN = SKILL_NEED_WEAPON( nSkillIDX, nI );
		
		if ( 0 == nNeedWPN )
		{
			/// Ã¹¹øÂ° ÇÊ¿äÀåºñ°¡ 0 ÀÌ¶ó¸é.. ÇÊ¿äÀåºñ°¡ ÇÊ¿ä¾ø´Ù.( ¸ðµç Ä®·³ÀÌ 0 ÀÌ¶õ ¾à¼Ó )
			if( nI == 0 )
				return true;
			else /// ¸ðµç ÇÊ¿ä¹«±â´Â ¾ÕÂÊºÎÅÍ Ã¤¿öÁø´Ù.
				break;
		}

		if ( nRWPN == nNeedWPN || nLWPN == nNeedWPN || nKnapsack == nNeedWPN ) {
			return true;
		}
	}

#ifndef __SERVER
	AddMsgToChatWND( STR_MISMATCH_CASTING_NEED_EQUIP, g_dwRED ,CChatDLG::CHAT_TYPE_SYSTEM);
#endif

	return false;
}

//-------------------------------------------------------------------------------------------------
short CUserDATA::GetPassiveSkillAttackSpeed( float fCurSpeed, short nRightWeaponItemNo )
{
	t_AbilityINDEX eIndex;
	switch ( WEAPON_TYPE(nRightWeaponItemNo) ) {
		case 231 :	eIndex = AT_PSV_ATK_SPD_BOW;	break;	// È°
		case 233 :	// ÅõÃ´±â
		case 232 :	eIndex = AT_PSV_ATK_SPD_GUN;	break;	// ÃÑ
		case 251 :	// Ä«Å¸¸£
		case 252 :	eIndex = AT_PSV_ATK_SPD_PAIR;	break;	// ÀÌµµ·ù 

		default  :
			return 0;
	}

	return this->GetPassiveSkillValue( eIndex ) + (short)(fCurSpeed*this->GetPassiveSkillRate( eIndex )/100.f);
}
short CUserDATA::GetPassiveSkillAttackPower( int iCurAP, short nRightWeaponItemNo )
{
	t_AbilityINDEX eIndex;
	if ( 0 == nRightWeaponItemNo ) {
		// ¸Ç¼Õ
		eIndex = AT_PSV_ATK_POW_NO_WEAPON;
	} else {
		switch ( WEAPON_TYPE(nRightWeaponItemNo) ) {
			// ÇÑ¼Õ°Ë, ÇÑ¼ÕµÐ±â, 
			case 211 : case 212 :				eIndex = AT_PSV_ATK_POW_1HAND_WPN;		break;
			// ¾ç¼Õ°Ë, ¾ç¼ÕÃ¢, ¾ç¼Õµµ³¢
			case 221 : case 222 : case 223 :	eIndex = AT_PSV_ATK_POW_2HAND_WPN;		break;
			// È°
			case 231 :							eIndex = AT_PSV_ATK_POW_BOW;			break;
			// ÃÑ, ÅõÃ´±â, ½ÖÃÑ
			case 232 : case 233 : case 253 :	eIndex = AT_PSV_ATK_POW_GUN;			break;
			// ¸¶¹ýÁöÆÎÀÌ	// ¸¶¹ý¿Ïµå
			case 241 : case 242 :				eIndex = AT_PSV_ATK_POW_STAFF_WAND;		break;
			// Ä«Å¸¸£, ÀÌµµ·ù
			case 251 : case 252 :				eIndex = AT_PSV_ATK_POW_KATAR_PAIR;		break;
			// ¼®±Ã
			case 271 :							eIndex = AT_PSV_ATK_POW_AUTO_BOW;		break;

			default :
				return 0;
		}
	}

	return ( this->GetPassiveSkillValue( eIndex ) + (short)( iCurAP * this->GetPassiveSkillRate( eIndex ) / 100.f ) );
}


//-------------------------------------------------------------------------------------------------
/// ½ºÅ³ ½Àµæ & ·¹º§¾÷½Ã..!!! :: 
/// return == trueÀÌ¸é CObjAVT->Update_SPEED() È£Ãâ ÇÊ¿ä !!!
BYTE CUserDATA::Skill_LEARN( short nSkillSLOT, short nSkillIDX, bool bSubPOINT )
{
	// btReturn & 0x02¸é ÀüÃ¼ °»½Å, & 0x01ÀÌ¸é ¼Óµµ °»½Å
	BYTE btReturn = 0;

	// ½ºÅ³ Æ÷ÀÎÆ® ¼Ò¸ð - ½Àµæ½Ã¿¡µµ ¼Ò¸ðµÇµµ·Ï ¼öÁ¤, 2004. 3. 16
	if ( bSubPOINT ) {
		this->SetCur_SkillPOINT( this->GetCur_SkillPOINT() - SKILL_NEED_LEVELUPPOINT(nSkillIDX) );
	}
	short nBeforeSkill = this->m_Skills.m_nSkillINDEX[ nSkillSLOT ];
	this->m_Skills.m_nSkillINDEX[ nSkillSLOT ] = nSkillIDX;		// ½ºÅ³ ½Àµæ !!!

	// ÆÐ½Ãºê ½ºÅ³ÀÌ¸é ???
	short nPassiveTYPE;
	if ( SKILL_TYPE_PASSIVE == SKILL_TYPE( nSkillIDX ) ) {
		for (short nI=0; nI<SKILL_INCREASE_ABILITY_CNT; nI++) {
			if ( 0 == SKILL_INCREASE_ABILITY_VALUE( nSkillIDX, nI ) &&
				 0 == SKILL_CHANGE_ABILITY_RATE( nSkillIDX, nI ) )
				continue;

			// ¼öÄ¡·Î ¿Ã¸² : ÆÐ½Ãºê ½ºÅ³Àº ºñÀ²·Î´Â ¸ø¿Ã¸² !!!
			if ( SKILL_INCREASE_ABILITY( nSkillIDX, nI ) >= AT_STR  &&
				 SKILL_INCREASE_ABILITY( nSkillIDX, nI ) <= AT_SENSE ) {
				nPassiveTYPE = SKILL_INCREASE_ABILITY( nSkillIDX, nI ) - AT_STR;

				if ( SKILL_CHANGE_ABILITY_RATE( nSkillIDX, nI ) ) {
					// 05.05.26 ºñÀ²°è»ê Ãß°¡...
					short nValue = SKILL_CHANGE_ABILITY_RATE( nSkillIDX, nI );
					if ( nBeforeSkill ) {
						nValue -= SKILL_CHANGE_ABILITY_RATE( nBeforeSkill, nI );
					}

					int iCurAbility = this->m_BasicAbility.m_nBasicA[ nPassiveTYPE ] + m_iAddValue[ SKILL_INCREASE_ABILITY( nSkillIDX, nI ) ];
					this->AddPassiveSkillRate( SKILL_INCREASE_ABILITY( nSkillIDX, nI ), nValue );
					m_PassiveAbilityFromRate[ nPassiveTYPE	] = (int)( iCurAbility * m_nPassiveRate[ SKILL_INCREASE_ABILITY( nSkillIDX, nI ) ] / 100.f );
				} else {
					short nValue = SKILL_INCREASE_ABILITY_VALUE( nSkillIDX, nI );
					if ( nBeforeSkill ) {
						nValue -= SKILL_INCREASE_ABILITY_VALUE( nBeforeSkill, nI );
					}

					this->m_iAddValue[ SKILL_INCREASE_ABILITY( nSkillIDX, nI ) ] -= this->m_PassiveAbilityFromValue[ nPassiveTYPE ];
					this->m_PassiveAbilityFromValue[ nPassiveTYPE ] += nValue;
					this->m_iAddValue[ SKILL_INCREASE_ABILITY( nSkillIDX, nI ) ] += this->m_PassiveAbilityFromValue[ nPassiveTYPE ];
				}

				return 0x03;
			} else {
				nPassiveTYPE = SKILL_INCREASE_ABILITY( nSkillIDX, nI );
				if ( (nPassiveTYPE >= AT_PSV_ATK_POW_NO_WEAPON && nPassiveTYPE < AT_AFTER_PASSIVE_SKILL ) ||
					 (nPassiveTYPE >= AT_PSV_RES && nPassiveTYPE < AT_AFTER_PASSIVE_SKILL_2ND ) ) {
					// ÆÐ½Ãºê¿¡ÀÇÇØ º¸Á¤µÇ´Â °ªµé...
					if ( SKILL_CHANGE_ABILITY_RATE( nSkillIDX, nI ) ) {
						// ºñÀ² °è»êÀÌ¸é ???
						short nValue = SKILL_CHANGE_ABILITY_RATE( nSkillIDX, nI );
						if ( nBeforeSkill ) {
							nValue -= SKILL_CHANGE_ABILITY_RATE( nBeforeSkill, nI );
						}
						this->AddPassiveSkillRate ( nPassiveTYPE, nValue );
					} else {
						short nValue = SKILL_INCREASE_ABILITY_VALUE( nSkillIDX, nI );
						if ( nBeforeSkill ) {
							nValue -= SKILL_INCREASE_ABILITY_VALUE( nBeforeSkill, nI );
						}
						this->AddPassiveSkillValue( nPassiveTYPE, nValue );
					}

					switch( nPassiveTYPE ) {
						case AT_PSV_MAX_HP :		Cal_MaxHP ();			btReturn |= 0x01;	break;	// ÁÖº¯¿¡ Åëº¸ ÇÊ¿ä
						case AT_PSV_MAX_MP :		Cal_MaxMP ();								break;	// ÁÖº¯¿¡ Åëº¸ ¾øÀÌ Àç°è»ê....
						case AT_PSV_DEF_POW :		Cal_DEFENCE ();								break;	// ÁÖº¯¿¡ Åëº¸ ¾øÀÌ Àç°è»ê....
						case AT_PSV_WEIGHT :		// ¹«°Ô ¹Ù²ñ
							// this->m_btWeightRate = ;
							Cal_MaxWEIGHT ();
							btReturn |= 0x01;
							break;
						case AT_PSV_MOV_SPD :								btReturn |= 0x01;	break;	// ÀÌ¼Ó.. ÁÖº¯¿¡ Åëº¸ ÇÊ¿ä
						case AT_PSV_RECOVER_HP :	// ÆÄÆ¼¿øµéÇÑÅ× º¯°æ »çÇ× Åëº¸ ÇÊ¿ä...
							Cal_RecoverHP ();
							btReturn |= 0x01;
							break;
						case AT_PSV_RECOVER_MP :	// ÆÄÆ¼¿øµéÇÑÅ× º¯°æ »çÇ× Åëº¸ ÇÊ¿ä...
							Cal_RecoverMP ();
							btReturn |= 0x01;
							break;
						case AT_PSV_SAVE_MP :		// MP Àý°¨ ºñÀ²
							m_Battle.m_fRateUseMP	= ( 100 - this->GetCur_SaveMP() ) / 100.f;
							break;
						case AT_PSV_DROP_RATE :
							Cal_DropRATE ();
							break;

						case AT_PSV_RES :
							Cal_RESIST ();
							break;
						case AT_PSV_HIT	:
							Cal_HIT ();
							break;
						case AT_PSV_CRITICAL :
							Cal_CRITICAL ();
							break;
						case AT_PSV_AVOID :
							Cal_AvoidRATE ();
							break;
						case AT_PSV_SHIELD_DEF :
							Cal_DEFENCE ();
							break;
						case AT_PSV_IMMUNITY :
							;
							;
							break;

						default :
							if ( nPassiveTYPE <= AT_PSV_ATK_POW_KATAR_PAIR ) {
								// °ø°Ý·Â º¯È­...
								this->Cal_ATTACK ();
							} else 
							if ( nPassiveTYPE < AT_PSV_MOV_SPD ) {
								// °ø¼Ó º¯°æ...
								//tagITEM *pITEM = &m_Inventory.m_ItemEQUIP[ EQUIP_IDX_WEAPON_R ];
								//if ( pITEM->GetHEADER() ) {
								//	this->m_nPassiveAttackSpeed = this->GetPassiveSkillAttackSpeed( pITEM->GetItemNO() );
								//}
								btReturn |= 0x01;
							}
							break;
					}
					/// ¿©±â¼­ 2Â÷ÀûÁø ´É·ÂÄ¡ Àû¿ë½ÃÄÑ ÁÖÁö ¾ÊÀ¸¸é ¾È ¸Â´Ù.
					this->Cal_BattleAbility();
				}
			}
		}
	}

	return btReturn;
}

//-------------------------------------------------------------------------------------------------
bool CUserDATA::Skill_DELETE( short nSkillSLOT, short nSkillIDX )
{
	if ( nSkillIDX == this->m_Skills.m_nSkillINDEX[ nSkillSLOT ] ) {
		this->m_Skills.m_nSkillINDEX[ nSkillSLOT ] = 0;			/// ½ºÅ³ »èÁ¦ !!!
		return true;
	}
	return false;
}


//-------------------------------------------------------------------------------------------------
void CUserDATA::InitPassiveSkill ()
{
	::ZeroMemory(&m_iAddValue   [ AT_PSV_ATK_POW_NO_WEAPON ],	sizeof(int)  *( AT_AFTER_PASSIVE_SKILL-AT_PSV_ATK_POW_NO_WEAPON ) );
	::ZeroMemory(&m_nPassiveRate[ AT_PSV_ATK_POW_NO_WEAPON ],	sizeof(short)*( AT_AFTER_PASSIVE_SKILL-AT_PSV_ATK_POW_NO_WEAPON ) );

	::ZeroMemory(&m_iAddValue   [ AT_PSV_RES ],	sizeof(int)  *( AT_AFTER_PASSIVE_SKILL_2ND-AT_PSV_RES ) );
	::ZeroMemory(&m_nPassiveRate[ AT_PSV_RES ],	sizeof(short)*( AT_AFTER_PASSIVE_SKILL_2ND-AT_PSV_RES ) );

	::ZeroMemory( m_PassiveAbilityFromRate,		sizeof(short) * BA_MAX );
	::ZeroMemory( m_PassiveAbilityFromValue,	sizeof(short) * BA_MAX );

	short nSlot, nSkillIDX, nI, nPassiveTYPE;
	for (nSlot=0; nSlot<MAX_LEARNED_SKILL_CNT; nSlot++) {
		if ( 0 == this->m_Skills.m_nSkillINDEX[ nSlot ] )
			continue;

		nSkillIDX = this->m_Skills.m_nSkillINDEX[ nSlot ];
		if ( SKILL_TYPE_PASSIVE == SKILL_TYPE( nSkillIDX ) ) {
			for (nI=0; nI<SKILL_INCREASE_ABILITY_CNT; nI++) {
				if ( 0 == SKILL_INCREASE_ABILITY_VALUE( nSkillIDX, nI ) &&
					 0 == SKILL_CHANGE_ABILITY_RATE( nSkillIDX, nI ) )
					continue;
				
				// Increased by numerical value: passive skill is not able to rate!
				if ( SKILL_INCREASE_ABILITY( nSkillIDX, nI ) >= AT_STR &&
					 SKILL_INCREASE_ABILITY( nSkillIDX, nI ) <= AT_SENSE ) {
					if ( SKILL_CHANGE_ABILITY_RATE( nSkillIDX, nI ) ) {
						// 05.05.26 Add ratio calculation ...
						this->AddPassiveSkillRate ( SKILL_INCREASE_ABILITY( nSkillIDX, nI ), SKILL_CHANGE_ABILITY_RATE( nSkillIDX, nI ) );
					} else {
						nPassiveTYPE = SKILL_INCREASE_ABILITY( nSkillIDX, nI ) - AT_STR;
						this->m_PassiveAbilityFromValue[ nPassiveTYPE ] += SKILL_INCREASE_ABILITY_VALUE( nSkillIDX, nI );
					}
				} else {
					nPassiveTYPE = SKILL_INCREASE_ABILITY( nSkillIDX, nI );
					if ( (nPassiveTYPE >= AT_PSV_ATK_POW_NO_WEAPON && nPassiveTYPE <= AT_AFTER_PASSIVE_SKILL ) ||
						 (nPassiveTYPE >= AT_PSV_RES && nPassiveTYPE < AT_AFTER_PASSIVE_SKILL_2ND ) ) {
						if ( SKILL_CHANGE_ABILITY_RATE( nSkillIDX, nI ) ) {
							// 05.05.26 ºñÀ²°è»ê ÀÌ¸é ???
							this->AddPassiveSkillRate ( nPassiveTYPE, SKILL_CHANGE_ABILITY_RATE( nSkillIDX, nI ) );
						} else {
							this->AddPassiveSkillValue( nPassiveTYPE, SKILL_INCREASE_ABILITY_VALUE( nSkillIDX, nI ) );
						}
					}
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
/// 2004 / 2 /10 :nAvy¼öÁ¤( return 0¿¡¼­ ÇØ´ç SkillIdx·Î STB¿¡¼­ ÇÊ¿ä Æ÷ÀÎÆ®¸¦ °¡Á®¿Â´Ù)
/// 2004 / 6 /10 :nAvy¼öÁ¤( STB ±¸Á¶º¯°æ - ´ÙÀ½·¹º§¾÷À» À§ÇÑ Æ÷ÀÎÆ®´Â ´ÙÀ½·¹º§¿¡¼­ °¡Á®¿Â´Ù )
short CUserDATA::Get_NeedPoint2SkillUP (short nSkillSLOT) {
	_ASSERT( nSkillSLOT >= 0 && nSkillSLOT <= MAX_LEARNED_SKILL_CNT );
	short nSkillIDX = m_Skills.m_nSkillINDEX[ nSkillSLOT ];

	short nNextLevelSkillIDX = nSkillIDX + 1;

	if ( nNextLevelSkillIDX >= g_SkillList.Get_SkillCNT() ) {
		// ´õÀÌ»ó ·¹º§¾÷ ÇÒ¼ö ¾ø´Ù.
		return 0;
	}

	// °°Àº Á¾·ùÀÇ ½ºÅ³ÀÌ°í ¹è¿ì·Á´Â ·¹º§ÀÌ ÇöÀç ·¹º§ÀÇ ´ÙÀ½ ·¹º§ÀÎ°¡ ??
	if ( SKILL_1LEV_INDEX( nSkillIDX) != SKILL_1LEV_INDEX( nNextLevelSkillIDX ) ||
		 SKILL_LEVEL( nSkillIDX )+1   != SKILL_LEVEL( nNextLevelSkillIDX ) ) {
		return 0;
	}


	//TODO:: ¿©±â¼­ skill stbÀÇ ÄÃ·³¿¡ ÀÖ´Â °ªÀ» Àü¼Û...
	return SKILL_NEED_LEVELUPPOINT( nNextLevelSkillIDX );
}

//-------------------------------------------------------------------------------------------------
BYTE CUserDATA::Quest_GetRegistered( int iQuestIDX )
{
	if ( iQuestIDX ) {
		for (BYTE btI=0; btI<QUEST_PER_PLAYER; btI++) {
			if ( iQuestIDX == m_Quests.m_QUEST[ btI ].GetID() )
				return btI;
		}
	}

	return QUEST_PER_PLAYER;
}

//-------------------------------------------------------------------------------------------------
short CUserDATA::Quest_Append ( int iQuestIDX )
{
	if ( iQuestIDX ) {
		short nI;
		for (nI=0; nI<QUEST_PER_PLAYER; nI++) {
			if ( iQuestIDX == m_Quests.m_QUEST[ nI ].GetID() ) {
				m_Quests.m_QUEST[ nI ].Init ();
				m_Quests.m_QUEST[ nI ].SetID( iQuestIDX, true );
				return nI;
			}
		}
		for (nI=0; nI<QUEST_PER_PLAYER; nI++) {
			if ( 0 == m_Quests.m_QUEST[ nI ].GetID() ) {
				m_Quests.m_QUEST[ nI ].Init ();
				m_Quests.m_QUEST[ nI ].SetID( iQuestIDX, true );
				return nI;
			}
		}
	}

	return -1;
}
bool CUserDATA::Quest_Append ( BYTE btSlot, int iQuestIDX )
{
	if ( btSlot >= QUEST_PER_PLAYER )
		return false;

	if ( m_Quests.m_QUEST[ btSlot ].GetID() )
		return false;

	m_Quests.m_QUEST[ btSlot ].Init ();
	m_Quests.m_QUEST[ btSlot ].SetID( iQuestIDX, true );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CUserDATA::Quest_Delete( int iQuestIDX )
{
	for (short nI=0; nI<QUEST_PER_PLAYER; nI++) {
		if ( iQuestIDX == m_Quests.m_QUEST[ nI ].GetID() ) {
			m_Quests.m_QUEST[ nI ].Init ();
			return true;
		}
	}
	return false;
}
bool CUserDATA::Quest_Delete ( BYTE btSLOT, int iQuestID )
{
	if ( btSLOT >= QUEST_PER_PLAYER )
		return false;

	if ( iQuestID != m_Quests.m_QUEST[ btSLOT ].GetID() )
		return false;

	m_Quests.m_QUEST[ btSLOT ].Init ();
	return true;
}

bool	CUserDATA::Quest_SubITEM( tagITEM &sSubITEM )
{
	short nFindSLOT = m_Inventory.FindITEM( sSubITEM );
	if ( nFindSLOT >= 0 ) 
	{
		this->Sub_ITEM( nFindSLOT, sSubITEM );
#ifndef __SERVER
		g_itMGR.AppendChatMsg( sSubITEM.SubtractQuestMESSAGE(), g_dwWHITE );
#endif
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------
#ifndef	__SERVER
#include "Game_FUNC.h"
#endif
bool CUserDATA::Reward_InitSKILL (void)
{
	// Initialize the skill of the character.
	if ( IsTAIWAN() ) {
		short nSkillIDX, n1LevSkillIDX;
		int iRecoverySP = 0;

		for (int iS=MAX_LEARNED_SKILL_PER_PAGE; iS<MAX_LEARNED_SKILL_CNT; iS++) {
			nSkillIDX = this->m_Skills.m_nSkillINDEX[ iS ];
			if ( 0 == nSkillIDX )
				continue;

			this->m_Skills.m_nSkillINDEX[ iS ] = 0;

			n1LevSkillIDX = SKILL_1LEV_INDEX( nSkillIDX );
			do {
				iRecoverySP += SKILL_NEED_LEVELUPPOINT( nSkillIDX );
				nSkillIDX --;
			} while( nSkillIDX > 0 && n1LevSkillIDX == SKILL_1LEV_INDEX( nSkillIDX ) );
		}
		this->AddCur_SkillPOINT( iRecoverySP );
	} else {
		short nNewSP = (int)( this->GetCur_LEVEL() * ( this->GetCur_LEVEL()+4 ) * 0.25f ) - 1;
		this->SetCur_SkillPOINT( nNewSP );
	}

	// Put the basic skill on page 0.
	::ZeroMemory( &m_Skills.m_nSkillINDEX[ MAX_LEARNED_SKILL_PER_PAGE ], sizeof(short) * ( MAX_LEARNED_SKILL_CNT-MAX_LEARNED_SKILL_PER_PAGE ) );

	// It's initialized so let's send it ...
	this->InitPassiveSkill ();
	this->UpdateCur_Ability ();
	this->Quest_CHANGE_SPEED ();

	return true;
}

bool CUserDATA::Reward_InitSTATUS (void)
{
	// ÄÉ¸¯ÅÍÀÇ ½ºÅÝÀ» ÃÊ±âÈ­ ÇÑ´Ù. 
	// º¸»óµÇ´Â Æ÷ÀÎÆ®½Ä ¼öÁ¤... 2004. 6. 7 by icarus
	int iBPoint = (int)( ( this->GetCur_LEVEL()-1 ) * ( this->GetCur_LEVEL()+24 ) * 0.4f + ( this->GetCur_LEVEL()-1 )*0.8f + 0.5f );
	this->SetCur_BonusPOINT( iBPoint );
	
	short nRace = this->GetCur_RACE();

	this->SetDef_STR   ( AVATAR_STR  ( nRace ) );
	this->SetDef_DEX   ( AVATAR_DEX  ( nRace ) );
	this->SetDef_INT   ( AVATAR_INT  ( nRace ) );
	this->SetDef_CON   ( AVATAR_CON  ( nRace ) );
	this->SetDef_CHARM ( AVATAR_CHARM( nRace ) );
	this->SetDef_SENSE ( AVATAR_SENSE( nRace ) );

	// ÃÊ±âÈ­ µÆÀ¸´Ï »óÅÂ¸¦ º¸³»ÀÚ...
	this->InitPassiveSkill ();
	this->UpdateCur_Ability ();
	this->Quest_CHANGE_SPEED ();

#ifndef	__SERVER
	int iEffect = GF_GetEffectUseFile( "3DData\\Effect\\healing_01.eft" );
	GF_EffectOnObject( g_pAVATAR->Get_INDEX(), iEffect );
#endif

	return true;
}

bool CUserDATA::Reward_ITEM( tagITEM &sITEM, BYTE btRewardToParty, BYTE btQuestSLOT )
{
	if ( btRewardToParty ) {
		;
		;
		;
	} else {
		if ( ITEM_TYPE_QUEST == sITEM.GetTYPE() ) {
			// Äù½ºÆ® ÀÎº¥Åä¸®¿¡ ³Ö±â...
			if ( btQuestSLOT >= QUEST_PER_PLAYER )
				return false;

			this->m_Quests.m_QUEST[ btQuestSLOT ].AddITEM( sITEM );
		} else {
			if ( !sITEM.IsEnableDupCNT() && 0 == sITEM.GetOption() ) {
				// ÀåºñÀÌ°í ¿É¼ÇÀÌ ¾øÀ¸¸é : ¼ÒÄÏ °áÁ¤.
				switch( ITEM_RARE_TYPE( sITEM.GetTYPE(), sITEM.GetItemNO() ) ) {
					case 1 :	// ¹«Á¶°Ç
						sITEM.m_bHasSocket = 1;
						sITEM.m_bIsAppraisal = 1;
						break;
					case 2 :	// °è»ê
						if ( ITEM_QUALITY( sITEM.GetTYPE(), sITEM.GetItemNO() ) + 60 - RANDOM(400) > 0 ) {
							sITEM.m_bHasSocket = 1;
							sITEM.m_bIsAppraisal = 1;
							break;
						}
				}
			}

			this->Add_ItemNSend( sITEM );
		}
#ifndef __SERVER
		g_itMGR.AppendChatMsg( sITEM.GettingQuestMESSAGE(), g_dwWHITE );
#endif

	}

	return true;
}

bool CUserDATA::Reward_ABILITY( bool bSetOrAdj, int iType, int iValue, BYTE btRewardToParty )
{
	if ( btRewardToParty ) {
		;
		;
		;
	} else {
		if ( bSetOrAdj )
			this->Set_AbilityValueNSend( iType, iValue );
		else
			this->Add_AbilityValueNSend( iType, iValue );
	}

	return true;
}

bool CUserDATA::Reward_CalEXP( BYTE btEquation, int iBaseValue, BYTE btRewardToParty )
{
	int iR;

	if ( btRewardToParty ) {
		;
		;
		;
	} else {
		iR = CCal::Get_RewardVALUE( btEquation, iBaseValue, this, 0 );
		this->Add_ExpNSend( iR );
#ifdef	__INC_WORLD
			g_LOG.CS_ODS( 0xffff, "			>>>> °æÇèÄ¡ º¸»ó:: %d \n", iR );
#endif
	}

	return true;
}

bool CUserDATA::Reward_CalMONEY( BYTE btEquation, int iBaseValue, BYTE btRewardToParty, short nDupCNT )
{
	int iR;

	if ( btRewardToParty ) {
		;
		;
		;
	} else {
		iR = CCal::Get_RewardVALUE( btEquation, iBaseValue, this, nDupCNT );
		this->Add_MoneyNSend( iR, GSV_REWARD_MONEY );
#ifdef	__INC_WORLD
		g_LOG.CS_ODS( 0xffff, "			>>>> µ· º¸»ó:: %d \n", iR );
#endif
	}
	return true;
}

bool CUserDATA::Reward_CalITEM( BYTE btEquation, int iBaseValue, BYTE btRewardToParty, int iItemSN, short nItemOP, BYTE btQuestSLOT )
{
	tagITEM sITEM;

	sITEM.Init( iItemSN );

	int iR;
	if ( btRewardToParty ) {
		;
		;
		;
	} else {
		if ( sITEM.IsEnableDupCNT() ) {
			iR = CCal::Get_RewardVALUE( btEquation, iBaseValue, this, 0 );
			if ( iR > 0 ) {
				sITEM.m_uiQuantity = iR;		// ¼ö·®...
				if ( ITEM_TYPE_QUEST == sITEM.GetTYPE() ) {
					if ( btQuestSLOT >= QUEST_PER_PLAYER )
						return false;

					this->m_Quests.m_QUEST[ btQuestSLOT ].AddITEM( sITEM );
				} else {
					this->Add_ItemNSend( sITEM );
				}
			}
		} else {
			// Àåºñ
			if ( nItemOP && nItemOP < 300 ) {
				sITEM.m_bIsAppraisal = 1;
				sITEM.m_bHasSocket   = 0;
				sITEM.m_nGEM_OP		 = nItemOP;
			} else {
				// Àåºñ°í ¿É¼ÇÀÌ ¾ø´Â °æ¿ì´Ù...¼ÒÄÏ °áÁ¤.
				switch( ITEM_RARE_TYPE( sITEM.GetTYPE(), sITEM.GetItemNO() ) ) {
					case 1 :	// ¹«Á¶°Ç
						sITEM.m_bHasSocket = 1;
						sITEM.m_bIsAppraisal = 1;
						break;
					case 2 :	// °è»ê
						if ( ITEM_QUALITY( sITEM.GetTYPE(), sITEM.GetItemNO() ) + 60 - RANDOM(400) > 0 ) {
							sITEM.m_bHasSocket = 1;
							sITEM.m_bIsAppraisal = 1;
							break;
						}
				}
			}
#ifdef	__INC_WORLD
			g_LOG.CS_ODS( 0xffff, "			>>>> ¾ÆÀÌÅÛ º¸»ó:: Type:%d, No:%d, Quantity: %d \n", sITEM.GetTYPE(), sITEM.GetItemNO(), sITEM.GetQuantity () );
#endif
			this->Add_ItemNSend( sITEM );
		}
	}

	return true;
}


//-------------------------------------------------------------------------------------------------
/// 2004 / 2/ 19 : Ãß°¡ nAvy
/// 2004 / 5/ 24 : ¼öÁ¤ nAvy - Sub_ITEM => ClearITEM
void CUserDATA::Set_ITEM(short nListRealNO, tagITEM& sITEM)
{
	tagITEM oldItem = Get_InventoryITEM (nListRealNO);
	if( !oldItem.IsEmpty() )
		ClearITEM( nListRealNO );

	if( !sITEM.IsEmpty() )
		Add_ITEM( nListRealNO, sITEM );
}

short	CUserDATA::Add_ITEM(short nListRealNO, tagITEM &sITEM)
{	
	return  m_Inventory.AppendITEM( nListRealNO, sITEM, m_Battle.m_nWEIGHT);		
}

void	CUserDATA::Sub_ITEM(short nListRealNO, tagITEM &sITEM)
{	
	m_Inventory.SubtractITEM( nListRealNO, sITEM, m_Battle.m_nWEIGHT);			
}

short	CUserDATA::Add_CatchITEM (short nListRealNO, tagITEM &sITEM)
{	
	return m_Inventory.AppendITEM( nListRealNO, sITEM, m_Battle.m_nWEIGHT );
}
//-------------------------------------------------------------------------------------------------
void  CUserDATA::SetCur_HP (short nValue)
{	

#ifndef __SERVER
	assert( 0 != nValue );
#endif

	this->m_GrowAbility.m_nHP=nValue;				
}	// »ý¸í·Â

//-------------------------------------------------------------------------------------------------
/// 2Â÷ÀüÁ÷ ¶§¹®¿¡ »ó½ÂµÇ´Â ´É·ÂÄ¡ Àû¿ë
void CUserDATA::Apply_2ndJob_Ability ( void )
{
#ifdef	__APPLY_2ND_JOB
	this->m_Battle.m_nImmunity = 0;
	switch( GetCur_JOB() ) {
		case CLASS_SOLDIER_121	:	// 2Â÷ ³ªÀÌÆ®
		case CLASS_SOLDIER_122	:	// 2Â÷ Ã¨ÇÁ
		case CLASS_MAGICIAN_221	:	// 2Â÷ ¸ÞÁö¼Ç
		case CLASS_MAGICIAN_222	:	// 2Â÷ Å¬·¯¸¯
		case CLASS_MIXER_321	:	// 2Â÷ ·¹ÀÌ´õ
		case CLASS_MIXER_322	:	// 2Â÷ ½ºÄ«¿ìÆ®
		case CLASS_MERCHANT_421	:	// 2Â÷ ºÎÁîÁÖ¾Æ
		case CLASS_MERCHANT_422	:	// 2Â÷ ¾ÆÆ¼Àò
			this->m_Battle.m_nMaxHP += 300;
			this->m_Battle.m_nATT   += 30;
			this->m_Battle.m_nDEF   += 25;
			this->m_Battle.m_nRES   += 20;
			// this->m_Battle.m_nImmunity = 30;
			break;
	}
#endif
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
