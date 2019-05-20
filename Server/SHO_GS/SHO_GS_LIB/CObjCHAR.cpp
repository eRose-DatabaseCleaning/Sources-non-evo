//---------------------------------------------------------------------------

#include "LIB_gsMAIN.h"
#include "CObjCHAR.h"
#include "GS_ThreadZONE.h"
#include "ZoneLIST.h"
#include "CUserDATA.h"
//#include "CVector.h"
//#include "CObjAVT.h"
//#include "Calculation.h"
//#include "GS_PARTY.h"
//#include "GS_ThreadLOG.h"
//#include "IO_Quest.h"

//-------------------------------------------------------------------------------------------------
CAI_OBJ* CObjCHAR::AI_FindFirstOBJ( tPOINTF &PosCENTER, int iDistance )
{
	if ( this->GetZONE() )
		return this->GetZONE()->FindFirstCHAR( PosCENTER, iDistance, this );

	return NULL;
}
CAI_OBJ* CObjCHAR::AI_FindFirstOBJ( int iDistance )
{
	if ( this->GetZONE() )
		return this->GetZONE()->FindFirstCHAR( this, iDistance, this );

	return NULL;
}
CAI_OBJ* CObjCHAR::AI_FindNextOBJ ()
{
	return this->GetZONE()->FindNextCHAR ();
}

CAI_OBJ* CObjCHAR::Find_LocalNPC( int iNpcNO )
{
	return g_pZoneLIST->Get_LocalNPC( iNpcNO );
}

//-------------------------------------------------------------------------------------------------
int	CObjCHAR::Get_EconomyVAR (short nVarIDX)
{
	return this->GetZONE()->Get_EconomyVAR( nVarIDX );
}
int	CObjCHAR::Get_WorldVAR  (short nVarIDX)
{
	return ::Get_WorldVAR( nVarIDX );
}

void CObjCHAR::Set_EconomyVAR(short nVarIDX, int iValue)
{
#pragma	COMPILE_TIME_MSG ( "TODO:: CObjCHAR::Set_EconomyVAR(short nVarIDX, int iValue) " )
}
void CObjCHAR::Set_WorldVAR  (short nVarIDX, int iValue)
{
	g_pZoneLIST->Set_WorldVAR( nVarIDX, iValue );
}

//-------------------------------------------------------------------------------------------------
DWORD CObjCHAR::GetCurAbsSEC ()
{
	if ( this->GetZONE() )
		this->GetZONE()->GetCurAbsSEC();

	return classTIME::GetCurrentAbsSecond();
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Create_PET (int iCharIDX, float fPosX, float fPosY, int iRange, BYTE btSetOwner, bool bImmediate)
{
	CObjMOB *pNewPET;
	pNewPET = this->GetZONE()->RegenCharacter( fPosX, fPosY, iRange, iCharIDX, 1, this->Get_TeamNO()/* TEAMNO_MOB */, bImmediate );
	if ( pNewPET ) {
		if ( btSetOwner ) {
			// ¼ÒÈ¯ ÁÖÃ¼¸¦ ¿À³Ê·Î ¼³Á¤...
			pNewPET->m_iCallerMobObjIDX = this->Get_INDEX();
		}
		if ( this->m_IngSTATUS.IsSET( FLAG_ING_DEC_LIFE_TIME ) ) {
			// ¼ÒÈ¯µÈ ÄÉ¸¯ÀÌ ¼ÒÈ¯ÇÒ °æ¿ì...
			pNewPET->m_IngSTATUS.UpdateIngSTATUS( pNewPET, 
						IDX_ING_STB_DEC_LIFE_TIME, 
						0 /* SKILL_DURATION(nSkillIDX) */, 
						0 /* nAdjValue */, 
						0 /* nSkillIDX */ );
		}
	}

	return true;
}


//-------------------------------------------------------------------------------------------------
BYTE CObjCHAR::Is_DAY (void)
{
	return this->GetZONE()->Is_DAY ();
}

//-------------------------------------------------------------------------------------------------
CObjCHAR::CObjCHAR ()
{
	m_IngSTATUS.Reset( true );

	m_iHP = 0;
	m_iMP = 0;
	m_fCurAniSPEED = 1.0f;

	m_fScale = 1.0f;
}
CObjCHAR::~CObjCHAR ()
{
//	LogString (LOG_NORMAL, "~CObjCHAR()  ObjTYPE: %d, ObjIDX: %d \n", Get_TYPE(), Get_INDEX() );
}


//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_SKILL_CANCEL( BYTE btWhy )
{	
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType	= GSV_SKILL_CANCEL;
	pCPacket->m_HEADER.m_nSize	= sizeof( gsv_SKILL_CANCEL );
	pCPacket->m_gsv_SKILL_CANCEL.m_wObjectIDX = this->Get_INDEX();
	pCPacket->m_gsv_SKILL_CANCEL.m_btWHY = btWhy;

	this->GetZONE()->SendPacketToSectors( this, pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_CHAT( char *szMsg )
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_CHAT;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_CHAT );
    pCPacket->m_gsv_CHAT.m_wObjectIDX = this->Get_INDEX();
    pCPacket->AppendString( szMsg );

    this->GetZONE()->SendPacketToSectors( this, pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}
bool CObjCHAR::Send_gsv_SHOUT( char *szMsg )
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_SHOUT;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_SHOUT );
    pCPacket->AppendString( this->Get_NAME() );
    pCPacket->AppendString( szMsg );

	this->GetZONE()->SendShout( this, pCPacket );
    Packet_ReleaseNUnlock( pCPacket );
	return true;
}

//-------------------------------------------------------------------------------------------------
void CObjCHAR::Send_gsv_ANNOUNCE_CHAT( char *szMsg )
{
	// ÇöÀç ¸Ê °øÁö
	g_pZoneLIST->Send_gsv_ANNOUNCE_CHAT( this->GetZONE()->Get_ZoneNO(), szMsg, this->Get_NAME() );
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_STOP ()
{
    assert( this->GetZONE() );
    
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType			= GSV_STOP;
	pCPacket->m_HEADER.m_nSize			= sizeof( gsv_STOP );
	pCPacket->m_gsv_STOP.m_wObjectIDX	= this->Get_INDEX ();
	pCPacket->m_gsv_STOP.m_PosCUR		= this->m_PosCUR;

    this->GetZONE()->SendPacketToSectors( this, pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_MOUSECMD (int iTargetObject)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType				= GSV_MOUSECMD;
	pCPacket->m_HEADER.m_nSize				= sizeof( gsv_MOUSECMD);
	pCPacket->m_gsv_MOUSECMD.m_wSourObjIDX	= this->Get_INDEX ();
	pCPacket->m_gsv_MOUSECMD.m_wDestObjIDX	= iTargetObject;
	pCPacket->m_gsv_MOUSECMD.m_PosTO		= this->m_PosGOTO;
	pCPacket->m_gsv_MOUSECMD.m_wSrvDIST		= (WORD)::distance( m_PosCUR, m_PosGOTO );
	pCPacket->m_gsv_MOUSECMD.m_nPosZ		= this->m_nPosZ;

    this->GetZONE()->SendPacketToSectors( this, pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_MOVE ()
{
//	LogString( 0xffff, "Send_gsv_MOVE( %s:%.0f ) , (%.0f,%.0f) => (%.0f,%.0f) \n", this->Get_NAME(), m_fCurMoveSpeed, m_PosCUR.x, m_PosCUR.y, m_PosGOTO.x, m_PosGOTO.y);

    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType			= GSV_MOVE;
	pCPacket->m_HEADER.m_nSize			= sizeof( gsv_MOVE );
	pCPacket->m_gsv_MOVE.m_wSourObjIDX	= this->Get_INDEX ();
	pCPacket->m_gsv_MOVE.m_wDestObjIDX	= this->Get_TargetIDX();
	pCPacket->m_gsv_MOVE.m_PosTO		= this->m_PosGOTO;
	pCPacket->m_gsv_MOVE.m_wSrvDIST		= (WORD)::distance( m_PosCUR, m_PosGOTO );
	pCPacket->m_gsv_MOVE.m_btMoveMODE	= this->Get_MoveMODE();

    this->GetZONE()->SendPacketToSectors( this, pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_ATTACK (int iTargetObject)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType			= GSV_ATTACK;
	pCPacket->m_HEADER.m_nSize			= sizeof( gsv_ATTACK );
	pCPacket->m_gsv_ATTACK.m_wAtkObjIDX	= this->Get_INDEX ();
	pCPacket->m_gsv_ATTACK.m_wDefObjIDX	= iTargetObject;
	pCPacket->m_gsv_ATTACK.m_wSrvDIST	= (WORD)::distance( m_PosCUR, m_PosGOTO );
	pCPacket->m_gsv_ATTACK.m_PosTO		= this->m_PosGOTO;		// 2003. 12. 26 Ãß°¡...

    this->GetZONE()->SendPacketToSectors( this, pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_TOGGLE( BYTE btTYPE, bool bUpdateSpeed )
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType			= GSV_TOGGLE;
	pCPacket->m_HEADER.m_nSize			= sizeof( gsv_TOGGLE );

	pCPacket->m_gsv_TOGGLE.m_wObjectIDX	= this->Get_INDEX ();
	pCPacket->m_gsv_TOGGLE.m_btTYPE		= btTYPE;

	if ( bUpdateSpeed ) {
		pCPacket->m_HEADER.m_nSize += sizeof(short);
		pCPacket->m_gsv_TOGGLE.m_nRunSPEED[ 0 ] = this->GetOri_RunSPEED();
	}

    this->GetZONE()->SendPacketToSectors( this, pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_SELF_SKILL( short nSkillIDX, short nNpcSkillMotion )
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType				 = GSV_SELF_SKILL;
	pCPacket->m_gsv_SELF_SKILL.m_wSourObjIDX = this->Get_INDEX ();
	pCPacket->m_gsv_SELF_SKILL.m_nSkillIDX	 = nSkillIDX;

	if ( nNpcSkillMotion >= 0 ) {
		pCPacket->m_HEADER.m_nSize	= sizeof(char) + sizeof( gsv_SELF_SKILL );
		pCPacket->m_gsv_SELF_SKILL.cNpcSkillMOTION[ 0 ] = (char)nNpcSkillMotion;
	} else {
		pCPacket->m_HEADER.m_nSize	= sizeof( gsv_SELF_SKILL );
	}

    this->GetZONE()->SendPacketToSectors( this, pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_TARGET_SKILL (short nSkillIDX, short nNpcSkillMotion )
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType				   = GSV_TARGET_SKILL;
	pCPacket->m_gsv_TARGET_SKILL.m_wSourObjIDX = this->Get_INDEX ();
	pCPacket->m_gsv_TARGET_SKILL.m_wDestObjIDX = this->Get_TargetIDX();
	pCPacket->m_gsv_TARGET_SKILL.m_nSkillIDX   = nSkillIDX;
	pCPacket->m_gsv_TARGET_SKILL.m_wSrvDIST	   = (WORD)::distance( m_PosCUR, m_PosGOTO );
	pCPacket->m_gsv_TARGET_SKILL.m_PosTO	   = this->m_PosGOTO;

	if ( nNpcSkillMotion >= 0 ) {
		pCPacket->m_HEADER.m_nSize	= sizeof(char) + sizeof( gsv_TARGET_SKILL );
		pCPacket->m_gsv_TARGET_SKILL.cNpcSkillMOTION[ 0 ] = (char)nNpcSkillMotion;
	} else {
		pCPacket->m_HEADER.m_nSize	= sizeof( gsv_TARGET_SKILL );
	}

    this->GetZONE()->SendPacketToSectors( this, pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_POSITION_SKILL (short nSkillIDX)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType				    = GSV_POSITION_SKILL;
	pCPacket->m_HEADER.m_nSize					= sizeof( gsv_POSITION_SKILL );
	pCPacket->m_gsv_POSITION_SKILL.m_wObjectIDX = this->Get_INDEX ();
	pCPacket->m_gsv_POSITION_SKILL.m_nSkillIDX  = nSkillIDX;
	pCPacket->m_gsv_POSITION_SKILL.m_PosTARGET	= this->m_PosGOTO;;

    this->GetZONE()->SendPacketToSectors( this, pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_CLEAR_STATUS (DWORD dwClearedFLAG)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	BYTE btIDX=0;	
	if ( dwClearedFLAG & FLAG_ING_HP )
		pCPacket->m_gsv_CLEAR_STATUS.m_nAdjVALUE[ btIDX++ ] = this->Get_HP();

	if ( dwClearedFLAG & FLAG_ING_MP )
		pCPacket->m_gsv_CLEAR_STATUS.m_nAdjVALUE[ btIDX++ ] = this->Get_MP();

/*
	if ( dwClearedFLAG & FLAG_ING_INC_MOV_SPEED )
		pCPacket->m_gsv_CLEAR_STATUS.m_nAdjVALUE[ btIDX++ ] = this->m_IngSTATUS.Inc_RUN_SPEED ();

	if ( dwClearedFLAG & FLAG_ING_DEC_MOV_SPEED ) 
		pCPacket->m_gsv_CLEAR_STATUS.m_nAdjVALUE[ btIDX++ ] = this->m_IngSTATUS.Dec_RUN_SPEED ();

	if ( dwClearedFLAG & FLAG_ING_INC_ATK_SPEED )
		pCPacket->m_gsv_CLEAR_STATUS.m_nAdjVALUE[ btIDX++ ] = this->m_IngSTATUS.Inc_ATK_SPEED ();
	
	if ( dwClearedFLAG & FLAG_ING_DEC_ATK_SPEED )
		pCPacket->m_gsv_CLEAR_STATUS.m_nAdjVALUE[ btIDX++ ] = this->m_IngSTATUS.Dec_ATK_SPEED ();
*/

	pCPacket->m_HEADER.m_wType				    = GSV_CLEAR_STATUS;
	pCPacket->m_HEADER.m_nSize					= sizeof( gsv_CLEAR_STATUS ) + btIDX * sizeof(short);
	pCPacket->m_gsv_CLEAR_STATUS.m_wObjectIDX	= this->Get_INDEX ();
	pCPacket->m_gsv_CLEAR_STATUS.m_dwStatusFLAG = this->m_IngSTATUS.GetFLAGs();

	if ( dwClearedFLAG & ~(FLAG_ING_HP|FLAG_ING_MP) ) {
		// ±âÅ¸ Àû¿ëÀÌ¸é ÁÖº¯¿¡ Àü¼Û, ÆÄÆ¼Á¦¿Ü... ÁÖº¯ ÆÄÆ¼¿øÀ» ¾Ë¾Æ¼­, ¸Ö¸®ÀÖ´Â ÆÄÆ¼¿øÀº ¸ô¶ó..
		this->GetZONE()->SendPacketToSectors( this, pCPacket );
		// this->SendPacketToPartyExceptME( pCPacket );
	} else {
		// ¹°¾à Àû¿ëÀÌ¸é ÆÄÆ¼¿ø ÇÑÅ×¸¸...
		this->SendPacketToPARTY( pCPacket );
	}

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}


//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_SET_MOTION( WORD wValue, short nMotionNO )
{
	if ( wValue & BIT_MOTION_STOP_CMD ) {
		if ( !CObjAI::SetCMD_STOP () )
			return true;
	}

    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType				= GSV_SET_MOTION;
	pCPacket->m_HEADER.m_nSize				= sizeof( gsv_SET_MOTION );
	pCPacket->m_gsv_SET_MOTION.m_wObjectIDX = this->Get_INDEX ();
	pCPacket->m_gsv_SET_MOTION.m_wValue		= wValue;
	pCPacket->m_gsv_SET_MOTION.m_nMotionNO  = nMotionNO;

    this->GetZONE()->SendPacketToSectors( this, pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_DAMAGE2Sector( int iAttackObject, WORD wDamage, CObjITEM *pDropITEM)
{
	// iAttackObject°¡ ÀÚ½ÅÀ» Á×ÀÏ¶§...
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType			= GSV_DAMAGE;

	pCPacket->m_gsv_DAMAGE.m_wAtkObjIDX	= iAttackObject;
	pCPacket->m_gsv_DAMAGE.m_wDefObjIDX	= this->Get_INDEX ();
	pCPacket->m_gsv_DAMAGE.m_Damage.m_wDamage = wDamage;

	if ( NULL == pDropITEM ) {
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_DAMAGE );
	} else {
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_DAMAGE ) + sizeof( tag_DROPITEM );

		pDropITEM->SetACTIVE ();

		pCPacket->m_gsv_DAMAGE.m_DropITEM[0].m_PosCUR			= pDropITEM->m_PosCUR;
		pCPacket->m_gsv_DAMAGE.m_DropITEM[0].m_ITEM				= pDropITEM->m_ITEM;
		pCPacket->m_gsv_DAMAGE.m_DropITEM[0].m_wServerItemIDX	= pDropITEM->Get_INDEX();
		pCPacket->m_gsv_DAMAGE.m_DropITEM[0].m_wOwnerObjIDX		= pDropITEM->m_iOwnerObjIDX;
		//pCPacket->m_gsv_DAMAGE.m_FieldITEM[0].m_wRemainTIME = pDropITEM->m_iRemainTIME;
	}

    this->GetZONE()->SendPacketToSectors( this, pCPacket );

	// ÀÚ½ÅÀ» ¼ÒÈ¯ÇÑ ÄÉ¸¯ÀÌ ÀÖ´Ù¸é...
	if ( this->GetCallerUsrIDX() ) {
		CObjCHAR *pOwner = g_pObjMGR->Get_CharOBJ( this->GetCallerUsrIDX(), true );
		if ( pOwner && pOwner->Get_CharHASH() == this->GetCallerHASH() ) {
			if ( !this->IsNEIGHBOR( pOwner ) )		// ÆÐÅ¶ÀÌ Àü¼ÛµÇ´Â ¿µ¿ª ¹Û¿¡ ÀÖ³ª?
				pOwner->SendPacket( pCPacket );
		}
	}

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_DAMAGE2Target( CObjCHAR *pAtkOBJ, WORD wDamage )
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType			= GSV_DAMAGE;
	pCPacket->m_HEADER.m_nSize			= sizeof( gsv_DAMAGE );
	pCPacket->m_gsv_DAMAGE.m_wAtkObjIDX	= pAtkOBJ->Get_INDEX();
	pCPacket->m_gsv_DAMAGE.m_wDefObjIDX	= this->Get_INDEX ();
	pCPacket->m_gsv_DAMAGE.m_Damage.m_wDamage = wDamage;

	// ¾Æ¹ÙÅ¸ ÀÚ½ÅÀÌ ¸ÂÀº °æ¿ì¿¡ ¼Ò¼ÓµÈ ÆÄÆ¼¿¡ Àü¼Û, ¶§¸®³ÑÀÌ ¾Æ¹ÙÅ¸ÀÌ¸é( PvsP ) ¶§¸®³Ñ ÇÑÅ×µµ Àü¼Û.
	// ¸÷ÀÌ ¸ÂÀ» °æ¿ì ÀÚ½ÅÀ» Å¸°ÙÆÃÇÏ°í ÀÖ´Â À¯Àúµé¿¡°Ô Àü¼Û...
	this->SendPacketToTARGET( pAtkOBJ, pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
void CObjCHAR::Init_ADD_CHAR( classPACKET *pCPacket )
{
    pCPacket->m_tag_ADD_CHAR.m_wObjectIDX	= this->Get_INDEX ();
    pCPacket->m_tag_ADD_CHAR.m_PosCUR		= this->m_PosCUR;
    pCPacket->m_tag_ADD_CHAR.m_PosTO		= this->m_PosGOTO;
    pCPacket->m_tag_ADD_CHAR.m_wCommand		= this->Get_COMMAND ();
    pCPacket->m_tag_ADD_CHAR.m_wTargetOBJ	= this->Get_TargetIDX();
    pCPacket->m_tag_ADD_CHAR.m_iHP			= this->Get_HP ();

	pCPacket->m_tag_ADD_CHAR.m_btMoveMODE	= this->Get_MoveMODE();
	pCPacket->m_tag_ADD_CHAR.m_dwStatusFALG = this->m_IngSTATUS.GetFLAGs();

#ifdef	__APPLY_04_10_15_TEAMNO
	pCPacket->m_tag_ADD_CHAR.m_iTeamNO		= this->Get_TeamNO();
#else
	pCPacket->m_tag_ADD_CHAR.m_cTeamNO		= this->Get_TeamNO();
#endif
}
void CObjCHAR::Add_ADJ_STATUS( classPACKET *pCPacket )
{
	if ( this->m_IngSTATUS.GetFLAGs() & 
		  ( FLAG_ING_MAX_HP |
			FLAG_ING_INC_MOV_SPEED | FLAG_ING_DEC_MOV_SPEED |
			FLAG_ING_INC_ATK_SPEED | FLAG_ING_DEC_ATK_SPEED |
			FLAG_ING_DEC_LIFE_TIME ) ) {
		BYTE btIDX=0;
		short *pAdjSTATUS = (short*)&pCPacket->m_pDATA[ pCPacket->m_HEADER.m_nSize ];

		if ( this->m_IngSTATUS.IsSET( FLAG_ING_MAX_HP ) )
			pAdjSTATUS[ btIDX++ ] = this->m_IngSTATUS.Inc_MAX_HP();

		if ( this->m_IngSTATUS.IsSET( FLAG_ING_INC_MOV_SPEED ) )
			pAdjSTATUS[ btIDX++ ] = this->m_IngSTATUS.Inc_RUN_SPEED();

		if ( this->m_IngSTATUS.IsSET( FLAG_ING_DEC_MOV_SPEED ) )
			pAdjSTATUS[ btIDX++ ] = this->m_IngSTATUS.Dec_RUN_SPEED();

		if ( this->m_IngSTATUS.IsSET( FLAG_ING_INC_ATK_SPEED ) )
			pAdjSTATUS[ btIDX++ ] = this->m_IngSTATUS.Inc_ATK_SPEED();

		if ( this->m_IngSTATUS.IsSET( FLAG_ING_DEC_ATK_SPEED ) )
			pAdjSTATUS[ btIDX++ ] = this->m_IngSTATUS.Dec_ATK_SPEED();

		if ( this->m_IngSTATUS.IsSET( FLAG_ING_DEC_LIFE_TIME ) ) {
			( (WORD*)pAdjSTATUS )[ btIDX++ ] = this->GetCallerUsrIDX ();
			if ( this->GetCallerUsrIDX () ) {
				pAdjSTATUS[ btIDX++ ] = this->GetSummonedSkillIDX ();
			}
		}

		pCPacket->m_HEADER.m_nSize += sizeof(short) * btIDX;
	}
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Send_gsv_EFFECT_OF_SKILL (int iSpellOBJ, short nSkillIDX, BYTE btResult, short nSpellerINT, short iSpellerSKILL_DURATION)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_EFFECT_OF_SKILL;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_EFFECT_OF_SKILL );

	pCPacket->m_gsv_EFFECT_OF_SKILL.m_wObjectIDX	= this->Get_INDEX();
	pCPacket->m_gsv_EFFECT_OF_SKILL.m_wSpellObjIDX	= iSpellOBJ;
	pCPacket->m_gsv_EFFECT_OF_SKILL.m_nSkillIDX		= nSkillIDX;
	pCPacket->m_gsv_EFFECT_OF_SKILL.m_btSuccessBITS	= btResult;
	pCPacket->m_gsv_EFFECT_OF_SKILL.m_nINT			= nSpellerINT;
	
	if(iSpellerSKILL_DURATION) pCPacket->m_gsv_EFFECT_OF_SKILL.m_iSpellerSKILL_DURATION	= iSpellerSKILL_DURATION;	//Numenor: Store USER true skill duration inside packets
	else pCPacket->m_gsv_EFFECT_OF_SKILL.m_iSpellerSKILL_DURATION = SKILL_DURATION(nSkillIDX);	//this means that if the skill as a duration of 0, probably because of a virtualization, then take the original value (normally everything works fine, but it's always good to add a security :) )

	this->GetZONE()->SendPacketToSectors( this, pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}
bool CObjCHAR::Send_gsv_DAMAGE_OF_SKILL (int iSpellOBJ, short nSkillIDX, WORD wDamage, BYTE btResult, short nSpellerINT, CObjITEM *pDropITEM)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_DAMAGE_OF_SKILL;

	pCPacket->m_gsv_DAMAGE_OF_SKILL.m_wObjectIDX	= this->Get_INDEX();
	pCPacket->m_gsv_DAMAGE_OF_SKILL.m_wSpellObjIDX	= iSpellOBJ;
	pCPacket->m_gsv_DAMAGE_OF_SKILL.m_nSkillIDX		= nSkillIDX;
	pCPacket->m_gsv_DAMAGE_OF_SKILL.m_btSuccessBITS = btResult;
	pCPacket->m_gsv_DAMAGE_OF_SKILL.m_nINT			= nSpellerINT;
	pCPacket->m_gsv_DAMAGE_OF_SKILL.m_wDamage		= wDamage;

	if ( NULL == pDropITEM ) {
	    pCPacket->m_HEADER.m_nSize = sizeof( gsv_DAMAGE_OF_SKILL );
	} else {
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_DAMAGE_OF_SKILL ) + sizeof( tag_DROPITEM );

		pDropITEM->SetACTIVE ();

		pCPacket->m_gsv_DAMAGE_OF_SKILL.m_DropITEM[0].m_PosCUR			= pDropITEM->m_PosCUR;
		pCPacket->m_gsv_DAMAGE_OF_SKILL.m_DropITEM[0].m_ITEM			= pDropITEM->m_ITEM;
		pCPacket->m_gsv_DAMAGE_OF_SKILL.m_DropITEM[0].m_wServerItemIDX	= pDropITEM->Get_INDEX();
		pCPacket->m_gsv_DAMAGE_OF_SKILL.m_DropITEM[0].m_wOwnerObjIDX	= pDropITEM->m_iOwnerObjIDX;
	}

	this->GetZONE()->SendPacketToSectors( this, pCPacket );

	// ÀÚ½ÅÀ» ¼ÒÈ¯ÇÑ ÄÉ¸¯ÀÌ ÀÖ´Ù¸é...
	if ( this->GetCallerUsrIDX() ) {
		CObjCHAR *pOwner = g_pObjMGR->Get_CharOBJ( this->GetCallerUsrIDX(), true );
		if ( pOwner && pOwner->IsUSER() && pOwner->Get_CharHASH() == this->GetCallerHASH() ) {
			if ( !this->IsNEIGHBOR( pOwner ) )		// ÆÐÅ¶ÀÌ Àü¼ÛµÇ´Â ¿µ¿ª ¹Û¿¡ ÀÖ³ª?
				pOwner->SendPacket( pCPacket );
		}
	}

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}
//-------------------------------------------------------------------------------------------------
void CObjCHAR::Send_gsv_RESULT_OF_SKILL (short nSkillIDX)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return;
	
	pCPacket->m_HEADER.m_wType = GSV_RESULT_OF_SKILL;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_RESULT_OF_SKILL );

	pCPacket->m_gsv_RESULT_OF_SKILL.m_wObjectIDX = this->Get_INDEX();
	pCPacket->m_gsv_RESULT_OF_SKILL.m_nSkillIDX  = nSkillIDX;

	this->GetZONE()->SendPacketToSectors( this, pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::SetCMD_STOP ()
{
	if ( CObjAI::SetCMD_STOP () )
		return this->Send_gsv_STOP ();

	return false;
}
bool CObjCHAR::SetCMD_MOVE (tPOINTF &PosCUR, tPOINTF &PosTO, int iTargetObjIDX)
{
	if ( CObjAI::SetCMD_MOVE (PosCUR, PosTO, iTargetObjIDX) ) {
		// À§Àå¼úµµ ¾ÈÇ®·Á~
		return this->Send_gsv_MOUSECMD( iTargetObjIDX );
	}
	return false;
}
bool CObjCHAR::SetCMD_MOVE2D (float fPosX, float fPosY, BYTE btRunMODE)
{
	// ÀÎ°øÁö´É¿¡¼­ È£ÃâµÇ´Â ÀÌµ¿ÇÔ¼ö !!!
	if ( !this->GetZONE()->IsMovablePOS( (int)fPosX, (int)fPosY ) )
		return false;

	// Å¸¿îÆ® »óÅÂ¸é Ç®¾î¶ó...
	if ( this->m_IngSTATUS.IsSET( FLAG_ING_TAUNT ) ) {
		this->m_IngSTATUS.ExpireSTATUS( ING_TAUNT );
		//this->m_IngSTATUS.ClearSTATUS( ING_TAUNT );
	}

	if ( CObjAI::SetCMD_MOVE2D (fPosX, fPosY, btRunMODE) ) {
		return this->Send_gsv_MOVE ();
	}
	return false;
}
bool CObjCHAR::SetCMD_ATTACK (int iTargetObject)
{
/*
	if ( this->m_iTargetObject != iTargetObject ) {
		if ( this->m_iTargetObject ) {
			pEnemy->Del_ENEMY( this );
		}
	}
*/
	if ( CObjAI::SetCMD_ATTACK( iTargetObject ) ) {
		if ( CMD_ATTACK == this->Get_COMMAND() ) {
			// À§Àå¼ú ÇØÁ¦...
			if ( this->m_IngSTATUS.IsSET( FLAG_ING_DISGUISE ) ) {	// À§ÀåÁßÀÌ¸é ÇØÁ¦...
				this->m_IngSTATUS.ClearStatusFLAG( FLAG_ING_DISGUISE );
			}
			this->Send_gsv_ATTACK( iTargetObject );
		} else
			this->Send_gsv_STOP ();

		return true;
	}
	return false;
}


//-------------------------------------------------------------------------------------------------
bool CObjCHAR::SetCMD_Skill2SELF (short nSkillIDX)
{
	if ( SKILL_TYPE_01 != SKILL_TYPE( nSkillIDX ) ) {
		if ( CObjAI::SetCMD_Skill2SELF( nSkillIDX ) ) {
			// À§Àå¼ú/Åõ¸í ÇØÁ¦...
			if ( this->m_IngSTATUS.IsSET( FLAG_ING_DISGUISE ) ) {	// À§ÀåÁßÀÌ¸é ÇØÁ¦...
				this->m_IngSTATUS.ClearStatusFLAG( FLAG_ING_DISGUISE );
			}
			return this->Send_gsv_SELF_SKILL( nSkillIDX );
		}
	}
	return false;
}
bool CObjCHAR::SetCMD_Skill2OBJ (int iTargetObjIDX, short nSkillIDX)
{
	if ( CObjAI::SetCMD_Skill2OBJ( iTargetObjIDX, nSkillIDX ) ) {
		// À§Àå¼ú ÇØÁ¦...
		if ( this->m_IngSTATUS.IsSET( FLAG_ING_DISGUISE ) ) {	// À§ÀåÁßÀÌ¸é ÇØÁ¦...
			this->m_IngSTATUS.ClearStatusFLAG( FLAG_ING_DISGUISE );
		}
		return this->Send_gsv_TARGET_SKILL( nSkillIDX );
	}
	return false;
}
bool CObjCHAR::SetCMD_Skill2POS (tPOINTF &PosGOTO, short nSkillIDX)
{
	if ( CObjAI::SetCMD_Skill2POS( PosGOTO, nSkillIDX ) ) {
		// À§Àå¼ú ÇØÁ¦...
		if ( this->m_IngSTATUS.IsSET( FLAG_ING_DISGUISE ) ) {	// À§ÀåÁßÀÌ¸é ÇØÁ¦...
			this->m_IngSTATUS.ClearStatusFLAG( FLAG_ING_DISGUISE );
		}
		return this->Send_gsv_POSITION_SKILL( nSkillIDX );
	}
	return false;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Set_MOTION (short nActionIdx, float fMoveSpeed, float fAniSpeed, bool bAttackMotion)
{
/*
	if ( !bAttackMotion && m_bAttackSTART )
		Attack_END ();
*/
	if ( fAniSpeed <= 0.f || fAniSpeed > 6.f ) {
		g_LOG.CS_ODS( 0xffff, ">>> ERROR:: Invalid motion speed: ActionIdx:%d, MoveSpeed:%f, AniApeed:%f, bAtk:%d \n", nActionIdx, fMoveSpeed, fAniSpeed, bAttackMotion );
		// assert( fAniSpeed > 0.f && fAniSpeed <= 5.f );
	}

	tagMOTION *pMotion = this->Get_MOTION (nActionIdx);
	if ( !pMotion )
		return false;

	if ( this->Chg_CurMOTION( pMotion ) ) {
		m_fCurAniSPEED = fAniSpeed;
	}
	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Dead (CObjCHAR *pKiller)
{
    if ( Get_HP() <= DEAD_HP )
        return false;

	// ±âÀý »óÅÂ·Î ¸¸µé¾î ¸í·É ºÒº¹ ½ÃÅ²´Ù.
	this->m_IngSTATUS.SetFLAG( FLAG_ING_FAINTING );

	this->Set_HP( DEAD_HP );
	this->SetCMD_DIE ();

	return true;
}

//-------------------------------------------------------------------------------------------------
#define SEND_DAMAGE_TO_NULL		0x00	// Miss
#define	SEND_DAMAGE_TO_TARGET	0x01
#define	SEND_DAMAGE_TO_SECTOR	0x02
BYTE CObjCHAR::Apply_DAMAGE (CObjCHAR *pTarget, uniDAMAGE &sDamage, CObjITEM **ppOutITEM, bool bDropItem)
{
	if ( pTarget->m_IngSTATUS.IsSET( FLAG_ING_REVIVE ) ) {
		// Å¸°ÙÀÌ Á×¾ú´Ù°¡ ±Ý¹æ ºÎÈ°ÇÑ »óÅÂ¸é... µ©Áö ¾Èµé¾î ¿À°Ô...
		return SEND_DAMAGE_TO_NULL;
	}

	int  iDamage = sDamage.m_wVALUE;

	//// À§Àå¼ú/Åõ¸í ÇØÁ¦...
	//if ( this->m_IngSTATUS.IsSET( FLAG_ING_DISGUISE | FLAG_ING_TRANSPARENT) ) {	// À§ÀåÁßÀÌ¸é ÇØÁ¦...
	//	this->m_IngSTATUS.ClearStatusFLAG( FLAG_ING_DISGUISE | FLAG_ING_TRANSPARENT );
	//}

	// Normal Attack !
	if ( iDamage ) {
		if ( this->GetCallerUsrIDX() ) {
			//if ( NULL == this->Get_OWNER() )	ÁÖÀÎÀÌ Á×¾úÀ»¶§ ¼ÒÈ¯¸÷ÀÌ ÀÚ»ì½Ã ½ºÅµµÇ¾î »ý·«..
			//	return SEND_DAMAGE_TO_NULL;
			if ( DIV02( iDamage ) > 0 )	// iDamge > 2 
				pTarget->Save_Damage( this->GetCallerUsrIDX(), DIV02( iDamage ) );		// ¼ÒÈ¯µÈ ³ÑÀº °æÄ¡¸¦ 1/2·Î...
		} else
			pTarget->Save_Damage( this->Get_INDEX(), iDamage );

		// ¹«Àû »óÅÂÀÎ°¡ ???
		if ( pTarget->m_IngSTATUS.IsSubSET( FLAG_CHEAT_INVINCIBLE ) ) {
			if ( pTarget->Get_HP() <= iDamage ) {
				pTarget->Set_HP( iDamage+100 );
			}
		}

		this->Stamp_AttackTIME ();

		if ( pTarget->Sub_HP( iDamage ) <= 0 ) {
			// °ø°ÝÀÚ°¡ »ç¶÷¶Ç´Â »ç¶÷ÀÌ ¼ÒÈ¯ÇÑ ¸÷ !!!  °æÇèÄ¡ & ·¹º§¾÷
			if ( this->IsUSER() || this->GetCallerUsrIDX() ) {
				if ( pTarget->IsUSER() || pTarget->GetCallerUsrIDX() ) {		// USER ¶Ç´Â ¼ÒÈ¯µÈ¸÷
					// TODO:: PK !!!!
				} else
				if ( pTarget->IsNPC () ) {		// MOB or NPC
					classUSER *pMobOWNER = pTarget->Give_EXP ();

					pTarget->Do_DeadAI( pMobOWNER, iDamage );	// pMobOWNER == NULLÀÏ¶§µµ ¼öÇàµÇµµ·Ï...

					if ( pMobOWNER ) {
						if ( NPC_DEAD_EVENT( pTarget->Get_CharNO() ) )  {
							// TargetÁ×À»¶§ ÀÌº¥Æ® ÀÖÀ¸¸é Å¬¶óÀÌ¾ðÆ®¿¡ Ã¼Å©ÇØ ºÁ¶ó Àü¼Û...
							pMobOWNER->Send_gsv_CHECK_NPC_EVENT( pTarget->Get_CharNO() );
						}

						if ( bDropItem ) {
							// ¾ÆÀÌÅÛ µå·Ó, Áö¿ª ¸¶¹ý¿¡ ÀÇ¿¡¼­´Â ¾ÆÀÌÅÛ»ý¼º ¾ÈÇÔ..¿Ö ? ÆÐÅ¶ °ü¸®°¡ ±ÍÂú¾Æ¼­...
							tagITEM sITEM;
#ifdef FRAROSE
							if ( CCal::Get_DropITEM ( pMobOWNER->Get_LEVEL()-pTarget->Get_LEVEL(), (CObjMOB*)pTarget, sITEM, this->GetZONE()->Get_ZoneNO(), pMobOWNER->GetCur_DropRATE() * pMobOWNER->m_GrowAbility.GetDropBoost( pMobOWNER->GetCurAbsSEC() ), pMobOWNER->Get_CHARM() ) )
#else
							if ( CCal::Get_DropITEM ( pMobOWNER->Get_LEVEL()-pTarget->Get_LEVEL(), (CObjMOB*)pTarget, sITEM, this->GetZONE()->Get_ZoneNO(), pMobOWNER->GetCur_DropRATE(), pMobOWNER->Get_CHARM() ) ) 
#endif
							{
								// Create field item ...
								*ppOutITEM = new CObjITEM;
								if ( *ppOutITEM ) {
									tPOINTF PosSET;
									PosSET.x = pTarget->m_PosCUR.x + RANDOM( 201 ) - 100;
									PosSET.y = pTarget->m_PosCUR.y + RANDOM( 201 ) - 100;

									// this´Â »ç¿ëÀÚ³ª »ç¿ëÀÚ°¡ ¼ÒÈ¯ÇÑ ¼ÒÈ¯¼ö´Ù.
									CObjCHAR *pSummonOWNER = (CObjCHAR*)this->Get_CALLER();
									CParty *pCParty = ( pSummonOWNER ) ? pSummonOWNER->GetPARTY() : NULL;

									// pMobOWNER->Set_ItemSN( (*ppOutITEM)->m_ITEM );	// ¸÷ Á×À»¶§ => ½ÀµæÇÒ¶§·Î º¯°æ...

									(*ppOutITEM)->InitItemOBJ( NULL, PosSET, pTarget->m_PosSECTOR, sITEM, pMobOWNER, false, pCParty );	// Á×À»¶§ ¶³±À
									this->GetZONE()->Add_DIRECT( *ppOutITEM );		// µå·Ó ¾ÆÀÌÅÛ
								}
							}
						}
					}
				}
			} else {
				this->Do_KillAI( pTarget, iDamage );
			}

			pTarget->Dead( this );

			sDamage.m_wACTION &= ~DMG_ACT_HITTED;
			sDamage.m_wACTION |= DMG_ACT_DEAD;

			return SEND_DAMAGE_TO_SECTOR;
		} else {
			// ¼ö¸é »óÅÂ¿¡¼­ »ó´ë¹æÀÇ °ø°ÝÀ» ¹ÞÀ¸¸é Ç®¸®°ÔµÈ´Ù.
			if ( pTarget->m_IngSTATUS.IsSET( FLAG_ING_SLEEP ) ) {
				pTarget->m_IngSTATUS.ClearStatusFLAG( FLAG_ING_SLEEP );
				pTarget->Send_gsv_CLEAR_STATUS( FLAG_ING_SLEEP );
			}

			pTarget->Do_DamagedAI ( this, iDamage );

			if ( sDamage.m_wACTION & DMG_ACT_HITTED ) {
				// ¸Â´Â µ¿ÀÛÇØ¾ß...
				if ( !(pTarget->Get_STATE() & CS_BIT_INT2) ) {
					pTarget->Set_MOTION( pTarget->GetANI_Hit() );
					pTarget->Set_STATE( CS_HIT );
				}
			}

			return SEND_DAMAGE_TO_TARGET;
		}
	} else {
		// ´ë»óÀÌ »ç¿ëÀÚ ÀÏ°æ¿ì Miss½Ã¿¡µµ ¹æ¾î±¸ °¨¼Ò Ã³¸®..
		if ( pTarget->IsUSER() ) {
			pTarget->Save_Damage( this->Get_INDEX(), iDamage );
		}

		// ¼ö¸é »óÅÂ¿¡¼­ »ó´ë¹æÀÇ °ø°ÝÀ» ¹ÞÀ¸¸é Ç®¸®°ÔµÈ´Ù.
		if ( pTarget->m_IngSTATUS.IsSET( FLAG_ING_SLEEP ) ) {
			pTarget->m_IngSTATUS.ClearStatusFLAG( FLAG_ING_SLEEP );
			pTarget->Send_gsv_CLEAR_STATUS( FLAG_ING_SLEEP );
		}

		pTarget->Do_DamagedAI ( this, 0 );
	}

	return SEND_DAMAGE_TO_NULL;
}

//-------------------------------------------------------------------------------------------------
void CObjCHAR::Give_DAMAGE (CObjCHAR *pTarget, uniDAMAGE sDamage, bool bDropItem)
{
	if ( this->m_IngSTATUS.IsSET( FLAG_ING_REVIVE ) ) {
		this->m_IngSTATUS.ExpireSTATUS( ING_REVIVE );
	}

	CObjITEM *pOutITEM=NULL;

	switch( Apply_DAMAGE (pTarget, sDamage, &pOutITEM, bDropItem) ) {
		case SEND_DAMAGE_TO_SECTOR :
			pTarget->Send_gsv_DAMAGE2Sector( this->Get_INDEX(), sDamage.m_wDamage, pOutITEM );
			return;

		case SEND_DAMAGE_TO_TARGET :
			// µ¥¹ÌÁö Àü¼Û :: ¾Æ¹ÙÅ¸¸¸ ÇØ´ç ...
			pTarget->Send_gsv_DAMAGE2Target( this, sDamage.m_wDamage ); 

			if ( pTarget->m_IngSTATUS.IsSET( FLAG_ING_SHIELD_DAMAGE ) ) {
				// ¹æÆÐ µ¥¹ÌÁö ¼³Á¤µÆ´Ù !!!
				if ( !this->m_IngSTATUS.IsSET( FLAG_ING_SHIELD_DAMAGE ) ) {
					// ¿Þ¼Õ¿¡ ¹æÆÐ ¾ÆÀÌÅÛ Á¾·ù 261¹ø ÀåÂøÇÏ°í ÀÖ¾î¾ß :: ½ºÅ³ ½ÃÀÛ Á¶°Ç¿¡ µé¾î ÀÖ°ÚÁö...
					if ( pTarget->IsInRANGE (this, 4000) ) {	// °Å¸®°¡ 4¹ÌÅÍ ÀÌ³» ÀÌ¸é...
						short nShieldSKILL = pTarget->m_IngSTATUS.GetSkillIDX( ING_SHIELD_DAMAGE );
						sDamage.m_wDamage = (WORD) ( sDamage.m_wVALUE * SKILL_POWER( nShieldSKILL ) / 100.f );

						if ( sDamage.m_wDamage > 0 ) {
							sDamage.m_wDamage |= DMG_BIT_IMMEDIATE;
							pTarget->Give_DAMAGE( this, sDamage );
						}
					}
				}
			}
			return;
	}
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Skill_IsPassFilter( CObjCHAR *pTarget, short nSkillIDX )
{
	if ( NULL == pTarget )
		return false;

	bool bResult=false;
	switch( SKILL_CLASS_FILTER( nSkillIDX ) ) {
		case SKILL_TARGET_FILTER_GROUP		:///< ±×·ì(ÆÄÆ¼)¿ø //Numenor: I'm changing this to include summons in this group.
			if (pTarget->IsUSER()){
				if ( this->GetPARTY() ) {
					bResult = ( pTarget->GetPARTY() == this->GetPARTY() );
					break;
				}
			}
			else{
				if (this->Is_ALLIED( pTarget ) ){
					CObjCHAR *pSummon_OWNER = (CObjCHAR*)pTarget->Get_CALLER();
					if (pSummon_OWNER == this){ bResult = true; break;}
					else{
						if ( this->GetPARTY() ) {
							bResult = ( pSummon_OWNER->GetPARTY() == this->GetPARTY() );
							break;
						}
					}
				}
			}
				// else ¾Æ·¡¿¡¼­ ÆÄÆ¼°¡ ¾øÀ¸¸é ÀÚ½ÅÀÎÁö ÆÇ´Ù...

		case SKILL_TARGET_FILTER_SELF		:///< ÀÚ±â ÀÚ½Å
			bResult = ( pTarget == this );
			break;

		case SKILL_TARGET_FILTER_GUILD		:///< ±æµå¿ø
			bResult = ( this->GetGUILD() && pTarget->GetGUILD() == this->GetGUILD() );
			break;

		case SKILL_TARGET_FILTER_FRIEND_ALL :///< ¾Æ±º (¾Æ¹ÙÅ¸, ¼ÒÈ¯¸÷)
			bResult = ( this->Is_ALLIED( pTarget ) );
			break;

		case SKILL_TARGET_FILTER_MOB		:///< ¸ðµç ¸ó½ºÅÍ (¼ÒÈ¯¸÷,Àû±º¸÷)
			bResult = pTarget->IsA( OBJ_MOB );
			break;

		case SKILL_TARGET_FILTER_ENEMY_ALL	:///< Àû PC + ¸ó½ºÅÍ 
			bResult = ( !this->Is_ALLIED( pTarget ) );
			break;

		case SKILL_TARGET_FILTER_ENEMY_PC	:///< Àû PC( Àû »óÅÂ Æ÷ÇÔ )
			bResult = ( pTarget->IsUSER() && !this->Is_ALLIED( pTarget ) );
			break;

		case SKILL_TARGET_FILTER_ALL_PC		:///< ¸ðµç PC
			bResult = ( pTarget->IsUSER() );
			break;

		case SKILL_TARGET_FILTER_DEAD_USER	:// Á×Àº ÇÃ·¹ÀÌ¾î
			return ( pTarget->Get_HP() == DEAD_HP && pTarget->IsUSER() && this->Is_ALLIED( pTarget ) );

		case SKILL_TARGET_FILTER_ENEMY_MOB	:// Àû ¸ó½ºÅÍ.
			bResult = ( !this->Is_ALLIED( pTarget ) && pTarget->IsA( OBJ_MOB ) );
			break;

		case SKILL_TARGET_FILTER_ALL_CHAR :
			bResult = true;
			break;

		default :
			return false;
	}

	if ( pTarget->Get_HP() > 0 )
		return bResult;
	return false;
}

//-------------------------------------------------------------------------------------------------
/*
BYTE CObjCHAR::Skill_IncAbilityValue( short nSkillIDX )
{
	BYTE btResult=0;
	for (short nI=0; nI<SKILL_INCREASE_ABILITY_CNT; nI++) {
		switch( SKILL_INCREASE_ABILITY( nSkillIDX, nI ) ) {
			case AT_HP	:
				this->Add_HP( SKILL_INCREASE_ABILITY_VALUE( nSkillIDX, nI ) );
				btResult |= ( 1<<nI );
				continue;
			case AT_MP	:
				this->Add_MP( SKILL_INCREASE_ABILITY_VALUE( nSkillIDX, nI ) );
				btResult |= ( 1<<nI );
				continue;
		}
	}

	return btResult;
}
*/

//-------------------------------------------------------------------------------------------------
// ´É·ÂÄ¡ÀÇ Áõ/°¨À» ÀÏÁ¤½Ã°£ Áö¼Ó(¼¿ÇÁ)
// ´É·ÂÄ¡ÀÇ Áõ/°¨À» ÀÏÁ¤½Ã°£ Áö¼Ó(Å¸°Ù)
// »óÅÂ Áö¼ÓÇü(¼¿ÇÁ)	Áßµ¶, º¡¾î¸®, ±âÀý, Åõ¸í, ¹æÆÐµ¥¹ÌÁö, Ãß°¡ µ¥¹ÌÁö
// »óÅÂ Áö¼ÓÇü(Å¸°Ù)	Áßµ¶, º¡¾î¸®, ±âÀý, Åõ¸í, ¹æÆÐµ¥¹ÌÁö, Ãß°¡ µ¥¹ÌÁö
BYTE CObjCHAR::Skill_ApplyIngSTATUS(short nSkillIDX, CObjCHAR *pSpeller)
{
	if ( this->Get_HP() <= 0 )
		return 0;

	if ( SKILL_HARM( nSkillIDX ) ) {
		// Àû´ëÀ¯¹ß...
		if ( this->m_IngSTATUS.IsSET( FLAG_ING_REVIVE ) ) {
			// Å¸°ÙÀÌ Á×¾ú´Ù°¡ ±Ý¹æ ºÎÈ°ÇÑ »óÅÂ¸é... µ©Áö ¾Èµé¾î ¿À°Ô...
			return 0;
		}

		this->Do_DamagedAI ( pSpeller, 0 );

		this->Stamp_AttackTIME ();
		pSpeller->Stamp_AttackTIME ();
	}

	short nI, nIngSTB, nAdjValue;

	BYTE btSuccessBITS=0;	// ¼º°ø¿©ºÎ
	for (nI=0; nI<2; nI++) {
		nIngSTB = SKILL_STATE_STB( nSkillIDX, nI );
		if ( 0 == nIngSTB )	{
			switch( SKILL_INCREASE_ABILITY( nSkillIDX, nI ) ) {
				case AT_STAMINA	:
					this->Add_STAMINA( SKILL_INCREASE_ABILITY_VALUE( nSkillIDX, nI ) );
					break;
				case AT_MONEY	:
					this->Add_MONEY( SKILL_INCREASE_ABILITY_VALUE( nSkillIDX, nI ) );
					break;
				case AT_HP		:
				{
					nAdjValue = CCal::Get_SkillAdjustVALUE( this, nSkillIDX, nI, pSpeller->Get_INT() );
					this->Add_HP( nAdjValue );
					break;
				}
				case AT_MP		:
					this->Add_MP( SKILL_INCREASE_ABILITY_VALUE( nSkillIDX, nI ) );
					break;
				default			:
					continue;
			}
			btSuccessBITS |= ( 1<<nI );
			continue;
		}

		if ( SKILL_SUCCESS_RATIO( nSkillIDX ) ) {
			// ¼º°øµµ 0 ÀÏ°æ¿ì´Â °è»ê¾øÀÌ ¹«Á¶°Ç ¼º°ø...
			if ( STATE_PRIFITS_LOSSES( nIngSTB ) ) {
				// ºÒ¸®
				// »ó´ë ·¹º§ * 0.4 + (»ó´ë Ç×¸¶·Â-10) * 0.5 - ÀÚ½Å·¹º§ * 0.4 - (ÀÚ½Å Ç×¸¶·Â-10) * 0.5
//#ifndef	__INC_WORLD
//				if ( SKILL_SUCCESS_RATIO( nSkillIDX ) < ( this->Get_LEVEL() * 0.4f + ( this->Get_RES()-10 ) * 0.5f - pSpeller->Get_LEVEL() * 0.4f - ( pSpeller->Get_RES()-10 ) *0.5f + (1+RANDOM(100)) ) )
//					continue;
//#else
#ifdef	__APPLY_2ND_JOB
				// ÈÄ¿¡ ¸é¿ª·Â ¼öÄ¡ ³Ö¾î¼­ °è»ê½Ä ¼öÁ¤ÇÒ°÷...
				if ( SKILL_SUCCESS_RATIO( nSkillIDX ) * ( pSpeller->Get_LEVEL() * 2 + pSpeller->Get_INT()+20 ) / ( this->Get_RES()*0.6f + 5 + this->Get_AVOID() ) <= (1+RANDOM(100)) ) 
					continue;
#else
				if ( SKILL_SUCCESS_RATIO( nSkillIDX ) * ( pSpeller->Get_LEVEL() * 2 + pSpeller->Get_INT()+20 ) / ( this->Get_RES()*0.6f + 5 + this->Get_AVOID() ) <= (1+RANDOM(100)) ) 
					continue;
#endif
//#endif
			} else {
				// À¯¸®
				// ¼º°ø·ã(12¹øÄ®·³) > [»ó´ë ·¹º§ - ÀÚ½Å·¹º§ + ·£´ý¼öÄ¡(1~100) ]  ÀÏ°æ¿ì »óÅÂÀû¿ë ¼º°ø
				if ( SKILL_SUCCESS_RATIO( nSkillIDX ) < this->Get_LEVEL() - pSpeller->Get_LEVEL() + (1+RANDOM(100)) )
					continue;
			}
		}

		nAdjValue = CCal::Get_SkillAdjustVALUE( this, nSkillIDX, nI, pSpeller->Get_INT() );
		// Àû¿ë °¡´É ÇÑ°¡ ???
		if ( !this->m_IngSTATUS.IsEnableApplay( nIngSTB, nAdjValue ) )	continue;

		btSuccessBITS |= ( 1<<nI );

		short true_skill_duration =0;
		if( pSpeller->IsUSER() ) true_skill_duration = pSpeller->Cur_SKILL_DURATION(nSkillIDX);	//if user
		else true_skill_duration = SKILL_DURATION(nSkillIDX);	//if char or monster

		if ( this->m_IngSTATUS.UpdateIngSTATUS( this, nIngSTB, true_skill_duration, nAdjValue, nSkillIDX, pSpeller->Get_INDEX() ) ) {

			// ¸í·ÉÀ» STOPÀ¸·Î ¹Ù²Ü²¨... ±âÀý, ¼ö¸é °É¸®¸é Á¤Áö ¸í·É ¼³Á¤...
			this->Del_ActiveSKILL();
			CObjAI::SetCMD_STOP ();
		}

		// Å¸¿îÆ® ½ºÅ³ÀÌ¸é...°ø°Ý ´ë»ó ¹Ù²Þ...
		if ( ING_TAUNT == STATE_TYPE(nIngSTB) ) {
			this->SetCMD_RUNnATTACK( pSpeller->Get_INDEX() );
		}
	}

	return btSuccessBITS;
}

//-------------------------------------------------------------------------------------------------
void CObjCHAR::Skill_ChangeIngSTATUS (CObjCHAR *pTarget)
{
	short nResultCNT=0;
	if ( pTarget->Get_RideMODE() < MOVE_MODE_DRIVE 
#ifdef __KCHS_BATTLECART__
		|| IsTAIWAN() // In Taiwan, you can use it in Pat mode.
#endif
		) {	
		// Do not apply the property after the damage is applied and the state change comes to death ~~~
		BYTE btResult;
		if ( 0 == SKILL_SCOPE( this->Get_ActiveSKILL() ) ) {
			if ( this->Skill_IsPassFilter( pTarget, this->Get_ActiveSKILL() ) ) {
				btResult = pTarget->Skill_ApplyIngSTATUS( this->Get_ActiveSKILL(), this );
				if ( btResult )  {
					int true_skill_duration = 0;
					if(this->IsUSER()) true_skill_duration = this->Cur_SKILL_DURATION(this->Get_ActiveSKILL());
					else true_skill_duration = SKILL_DURATION(this->Get_ActiveSKILL()); //Numenor: case of monster skills...
					pTarget->Send_gsv_EFFECT_OF_SKILL( this->Get_INDEX(), this->Get_ActiveSKILL(), btResult, this->Get_INT(), true_skill_duration);
					nResultCNT ++;
				}
			}
		} else {
			CObjCHAR *pFindCHAR;
			pFindCHAR = (CObjCHAR*)pTarget->AI_FindFirstOBJ( SKILL_SCOPE( Get_ActiveSKILL() ) );
			while( pFindCHAR ) {
				if ( this->Skill_IsPassFilter( pFindCHAR, Get_ActiveSKILL() ) ) {
					btResult = pFindCHAR->Skill_ApplyIngSTATUS( Get_ActiveSKILL(), this );
					if ( btResult ) {
						int true_skill_duration = 0;
						if(this->IsUSER()) true_skill_duration = this->Cur_SKILL_DURATION(this->Get_ActiveSKILL());
						else true_skill_duration = SKILL_DURATION(this->Get_ActiveSKILL()); //Numenor: case of monster skills...
						pFindCHAR->Send_gsv_EFFECT_OF_SKILL( this->Get_INDEX(), Get_ActiveSKILL(), btResult, this->Get_INT(), true_skill_duration );
						nResultCNT ++;
					}
				}

				pFindCHAR = (CObjCHAR*)pTarget->AI_FindNextOBJ ();
			}
		}
	}

	this->Send_gsv_RESULT_OF_SKILL( Get_ActiveSKILL() /*, nResultCNT */ );
}

//-------------------------------------------------------------------------------------------------
// 7, 17¹ø À¯Çü ½ºÅ³...
bool CObjCHAR::Skill_DamageToAROUND ( tPOINTF &PosCENTER )
{
	// ¹üÀ§¾ÈÀÇ Àû¿ëÇÒ Å¸ÄÏÀ» ¸ðµÎ ±¸ÇÑ´Ù...
	// °¢°¢ Å¸°Ù¿¡ µ¥¹ÌÁö Àû¿ë...
	BYTE btResult;
	uniDAMAGE sDamage;
	CObjCHAR *pFindCHAR;
	short nResultCNT=0;

	CObjITEM *pOutITEM;
	// PosGOTOÁÂÇ¥¸¦ Áß½ÉÀ¸·Î Å½»ö...
	pFindCHAR = (CObjCHAR*)this->AI_FindFirstOBJ( PosCENTER, SKILL_SCOPE( Get_ActiveSKILL() ) );
	while( pFindCHAR ) {
		if ( this->Skill_IsPassFilter( pFindCHAR, Get_ActiveSKILL() ) ) {
			sDamage.m_wDamage  = CCal::Get_SkillDAMAGE( this, pFindCHAR, Get_ActiveSKILL(), 1 );

			pOutITEM = NULL;
			this->Apply_DAMAGE (pFindCHAR, sDamage, &pOutITEM, true);

			if ( this->Skill_IsPassFilter( pFindCHAR, Get_ActiveSKILL() ) ) {
				btResult = pFindCHAR->Skill_ApplyIngSTATUS( Get_ActiveSKILL(), this );
			} else btResult = 0;

			if ( sDamage.m_wDamage || btResult ) {
				sDamage.m_wDamage |= DMG_BIT_IMMEDIATE;		// ½ºÅ³ µ©Áã´Â Áï°¢ ¹ÝÀÀ...
				pFindCHAR->Send_gsv_DAMAGE_OF_SKILL( this->Get_INDEX(), Get_ActiveSKILL(), sDamage.m_wDamage, btResult, this->Get_INT(), pOutITEM );
				nResultCNT ++;
			}
		}

		pFindCHAR = (CObjCHAR*)this->AI_FindNextOBJ ();
	}

	this->Send_gsv_RESULT_OF_SKILL( Get_ActiveSKILL() /*, nResultCNT */ );

	// °ø°ÝÀÚ ¹«±â ¼ö¸í °¨¼Ò...
	if ( 3 != SKILL_DAMAGE_TYPE( Get_ActiveSKILL() ) ) {
		this->Dec_WeaponLife ();
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
void CObjCHAR::Skill_START_03_04_05 (CObjCHAR *pTarget)
{
	assert( m_pCurMOTION->m_wTatalAttackFrame );

	// 03~06 :: Àû ÄÉ¸¯ÅÍ °ø°Ý ½ºÅ³...
	// @bug:: ½ºÅ³À» »ç¿ëÇÏ±â Àü¿¡ ÇÊÅÍ¸µÀ» ÇÏÁö¸¸... 
	// ½ºÅ³ »ç¿ëÁß ´Ù¸¥ ½ºÅ³ ¸í·ÉÀÌ ¿Ã°æ¿ì Áß°£¿¡ ½ºÅ³ÀÌ ¹Ù²î¾î
	// ÇÊÅÍ¸µÀ» Àû¿ëÇÏÁö ¾ÊÀ¸¸é »¶³­´Ù.
	if ( this->Skill_IsPassFilter( pTarget, Get_ActiveSKILL() ) ) 
	{
		uniDAMAGE sDamage;

		sDamage.m_wDamage = CCal::Get_SkillDAMAGE( this, pTarget, Get_ActiveSKILL(), m_pCurMOTION->m_wTatalAttackFrame);

		CObjITEM *pOutITEM=NULL;
		this->Apply_DAMAGE (pTarget, sDamage, &pOutITEM, true);
		pTarget->Send_gsv_DAMAGE_OF_SKILL( this->Get_INDEX(), Get_ActiveSKILL(), sDamage.m_wDamage, 0, 0, pOutITEM );
		this->Skill_ChangeIngSTATUS( pTarget );
	}

	// °ø°ÝÀÚ ¹«±â ¼ö¸í °¨¼Ò...
	if ( 3 != SKILL_DAMAGE_TYPE( Get_ActiveSKILL() ) ) {
		this->Dec_WeaponLife ();
	}
}
//-------------------------------------------------------------------------------------------------
void CObjCHAR::Skill_START_10_11 (CObjCHAR *pTarget)
{
	short nResultCNT=0;
	BYTE btResult;
	if ( 0 == SKILL_SCOPE( Get_ActiveSKILL() ) ) {
		if ( this->Skill_IsPassFilter( pTarget, Get_ActiveSKILL() ) ) {
			//btResult = pTarget->Skill_IncAbilityValue( Get_ActiveSKILL() );
			btResult = pTarget->Skill_ApplyIngSTATUS( Get_ActiveSKILL(), this );
			if ( btResult ) {
				int true_skill_duration = 0;
				if(this->IsUSER()) true_skill_duration = this->Cur_SKILL_DURATION(this->Get_ActiveSKILL());
				else true_skill_duration = SKILL_DURATION(this->Get_ActiveSKILL()); //Numenor: case of monster skills...
				pTarget->Send_gsv_EFFECT_OF_SKILL( this->Get_INDEX(), Get_ActiveSKILL(), btResult, this->Get_INT(), true_skill_duration );
				nResultCNT ++;
			}
		}
	} else {
		CObjCHAR *pFindCHAR;
		pFindCHAR = (CObjCHAR*)pTarget->AI_FindFirstOBJ( SKILL_SCOPE( Get_ActiveSKILL() ) );
		while( pFindCHAR ) {
			if ( this->Skill_IsPassFilter( pFindCHAR, Get_ActiveSKILL() ) ) {
				// btResult = pFindCHAR->Skill_IncAbilityValue( Get_ActiveSKILL() );
				btResult = pFindCHAR->Skill_ApplyIngSTATUS( Get_ActiveSKILL(), this );
				if ( btResult ) {
					int true_skill_duration = 0;
					if(this->IsUSER()) true_skill_duration = this->Cur_SKILL_DURATION(this->Get_ActiveSKILL());
					else true_skill_duration = SKILL_DURATION(this->Get_ActiveSKILL()); //Numenor: case of monster skills...
					pFindCHAR->Send_gsv_EFFECT_OF_SKILL( this->Get_INDEX(), Get_ActiveSKILL(), btResult, this->Get_INT(), true_skill_duration );
					nResultCNT ++;
				}
			}

			pFindCHAR = (CObjCHAR*)pTarget->AI_FindNextOBJ ();
		}
	}

	this->Send_gsv_RESULT_OF_SKILL( Get_ActiveSKILL() /*, nResultCNT */ );
}
//-------------------------------------------------------------------------------------------------
void CObjCHAR::Skill_START_19 (CObjCHAR *pTarget)
{
	// 03~06 :: Àû ÄÉ¸¯ÅÍ °ø°Ý ½ºÅ³...
	// @bug:: ½ºÅ³À» »ç¿ëÇÏ±â Àü¿¡ ÇÊÅÍ¸µÀ» ÇÏÁö¸¸... 
	// ½ºÅ³ »ç¿ëÁß ´Ù¸¥ ½ºÅ³ ¸í·ÉÀÌ ¿Ã°æ¿ì Áß°£¿¡ ½ºÅ³ÀÌ ¹Ù²î¾î
	// ÇÊÅÍ¸µÀ» Àû¿ëÇÏÁö ¾ÊÀ¸¸é »¶³­´Ù.
	if ( this->Skill_IsPassFilter( pTarget, Get_ActiveSKILL() ) ) 
	{
		uniDAMAGE sDamage;

		sDamage.m_wDamage = CCal::Get_SkillDAMAGE( this, pTarget, Get_ActiveSKILL(), m_pCurMOTION->m_wTatalAttackFrame);

		CObjITEM *pOutITEM=NULL;
		this->Apply_DAMAGE (pTarget, sDamage, &pOutITEM, true);
		if ( sDamage.m_wVALUE > 0 ) {
			BYTE btResult = this->Skill_ApplyIngSTATUS( Get_ActiveSKILL(), pTarget );
			pTarget->Send_gsv_DAMAGE_OF_SKILL( this->Get_INDEX(), Get_ActiveSKILL(), sDamage.m_wDamage, btResult, this->Get_INT(), pOutITEM );
		} else {
			pTarget->Send_gsv_DAMAGE_OF_SKILL( this->Get_INDEX(), Get_ActiveSKILL(), sDamage.m_wDamage, 0, 0, pOutITEM );
		}
		this->Send_gsv_RESULT_OF_SKILL( Get_ActiveSKILL() /*, nResultCNT */ );
//		this->Skill_ChangeIngSTATUS( pTarget );
	}

	// °ø°ÝÀÚ ¹«±â ¼ö¸í °¨¼Ò...
	if ( 3 != SKILL_DAMAGE_TYPE( Get_ActiveSKILL() ) ) {
		this->Dec_WeaponLife ();
	}
}
bool CObjCHAR::Skill_START (CObjCHAR *pTarget)
{
	// On the server, you only need to process the results after the skill is completed ....
	switch( SKILL_TYPE( Get_ActiveSKILL() ) ) {
		/*
		case SKILL_TYPE_01 :	// Basic action skill
		case SKILL_TYPE_02 :	// Window creation skill
		case SKILL_TYPE_15 :	// Passive
		case SKILL_TYPE_16 :	// emotion
		case SKILL_TYPE_18 :	// warp
			break;
		*/

		case SKILL_TYPE_03 :	// Attack behavior change type
		case SKILL_TYPE_04 :	// Weapon state change type
		case SKILL_TYPE_05 :	// Bullet changes Hacked
			this->Skill_START_03_04_05( pTarget );
			return true;

		case SKILL_TYPE_06 :	// Launch magic
		{
			if ( this->Skill_IsPassFilter( pTarget, Get_ActiveSKILL() ) ) 
			{
				uniDAMAGE sDamage;

				sDamage.m_wDamage = CCal::Get_SkillDAMAGE( this, pTarget, Get_ActiveSKILL(), m_pCurMOTION->m_wTatalAttackFrame);
				this->Give_DAMAGE( pTarget, sDamage );
				this->Skill_ChangeIngSTATUS( pTarget );
			}

			// Reducing attacker weapon life ...
			if ( 3 != SKILL_DAMAGE_TYPE( Get_ActiveSKILL() ) ) {
				this->Dec_WeaponLife ();
			}
			return true;
		}

		case SKILL_TYPE_07 :	// Area Magical Attack
			return this->Skill_DamageToAROUND( this->m_PosGOTO );
		case SKILL_TYPE_17 :	// self & damage ...
			return this->Skill_DamageToAROUND( this->m_PosCUR );

		case SKILL_TYPE_10 :	// Ability change type (self): Put in one shot
			if ( pTarget ) {
				this->CloseNETWORK ();
				return false;
			}
			this->Skill_START_10_11( this );
			return true;
		case SKILL_TYPE_11 :	// Ability Change Type (Target): Put in one shot
			this->Skill_START_10_11( pTarget );
			return true;

		case SKILL_TYPE_08 :	// Increase / decrease of ability value for a certain time (self)
		case SKILL_TYPE_12 :	// Stateful (self) poisoning, dumb, stunned, transparent, shield damage, additional damage
			/*
			if ( pTarget ) {
				this->CloseNETWORK ();
				return false;
			}
			*/
			this->Skill_ChangeIngSTATUS( this );
			return true;
			
		case SKILL_TYPE_09 :	// Increase / decrease of ability value for a certain time (target)
		case SKILL_TYPE_13 :	// State-persistent (target) addiction, dumb, stunned, transparent, shield damage, additional damage
			this->Skill_ChangeIngSTATUS( pTarget );
			return true;

		case SKILL_TYPE_14 :	// Summon NPC with your team number ...
		{
			CObjSUMMON *pNewPET;
			pNewPET = new CObjSUMMON;
			
			if ( pNewPET && this->GetZONE()->RegenCharacter( pNewPET, this->m_PosCUR.x, this->m_PosCUR.y, 150, SKILL_SUMMON_PET( Get_ActiveSKILL() ), this->Get_TeamNO() ) ) {
				// pNewPET->m_btSummonCMD = this->Get_SummonCMD();
				pNewPET->SetCallerOBJ( this->Get_INDEX(), this->Get_CharHASH(), this->Get_LEVEL(), this->Get_ActiveSKILL() , this); // 2005-06-30(kchs) ´ë¸¸ ¼ÒÈ¯¼ö ´É·ÂÄ¡ ¼öÁ¤ °ü·Ã...

				pNewPET->m_IngSTATUS.UpdateIngSTATUS( pNewPET, 
							IDX_ING_STB_DEC_LIFE_TIME, 
							0 /* SKILL_DURATION(nSkillIDX) */, 
							0 /* nAdjValue */, 
							0 /* nSkillIDX */ );
				short nNeedValue = NPC_NEED_SUMMON_CNT( pNewPET->Get_CharNO() ); 
				this->Add_SummonCNT( nNeedValue );
				this->Send_gsv_RESULT_OF_SKILL( Get_ActiveSKILL() /*, 1*/ );

				// Summoned Caric AI
				pNewPET->Do_CreatedAI ();
			} else
				SAFE_DELETE( pNewPET );
			return true;
		}

		case SKILL_TYPE_19 :	// (HP, MP takeover concept while attacking target)
			this->Skill_START_19( pTarget );
			return true;

		case SKILL_TYPE_20 :	// resurrection
		{
			if ( this->Skill_IsPassFilter( pTarget, Get_ActiveSKILL() ) ) {
				int true_skill_duration = 0;
				if(this->IsUSER()) true_skill_duration = this->Cur_SKILL_DURATION(this->Get_ActiveSKILL());
				else true_skill_duration = SKILL_DURATION(this->Get_ActiveSKILL()); //Numenor: case of monster skills...
				pTarget->Send_gsv_EFFECT_OF_SKILL( this->Get_INDEX(), Get_ActiveSKILL(), 0x01, this->Get_INT(), true_skill_duration );
				this->Send_gsv_RESULT_OF_SKILL( Get_ActiveSKILL() /*, 1*/ );
				( (CObjAVT*)pTarget )->Resurrection( Get_ActiveSKILL() );
			}
			return true;
		}
		default :
			this->CloseNETWORK ();
			return false;
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjCHAR::Attack_START (CObjCHAR *pTarget)
{
	if ( !m_pCurMOTION->m_wTatalAttackFrame ) {
#ifdef	__INC_WORLD
		assert( 0 != m_pCurMOTION->m_wTatalAttackFrame );
#endif
		return false;
	}

	// ¼Ò¸ðÅº Ã¼Å©...
	if ( !this->Dec_ShotITEM( (BYTE)( m_pCurMOTION->m_wTatalAttackFrame ) ) )
		return true;

	uniDAMAGE sDamage;

	sDamage.m_wDamage = CCal::Get_DAMAGE( this, pTarget, m_pCurMOTION->m_wTatalAttackFrame );
	this->Give_DAMAGE (pTarget, sDamage);

	// °ø°ÝÀÚ ¹«±â ¼ö¸í °¨¼Ò...
	this->Dec_WeaponLife ();

	return true;
}

void CObjCHAR::ActionEVENT (int iActionIDX)
{
	// ¼­¹ö´Â ÀÏ·ç ¸ø¿Í !!!
	assert( 0 && "Invalid function called" );
}

bool CObjCHAR::Casting_START (CObjCHAR *pTarget)
{
	m_bCastingSTART = true;

	if ( CMD_SKILL2OBJ == this->Get_COMMAND() ) {
		// ½ºÅ³ Å¸°ÙÀÌ ÀÌµ¿ÁßÀÏ°æ¿ì Å¬¶óÀÌ¾ðÆ®°¡ ÄÉ½ºÆÃ ½ÃÀÛ À§Ä¡¸¦ ¾ËÁö ¸øÇÏ¹Ç·Î...
		classPACKET *pCPacket = Packet_AllocNLock ();
		if ( !pCPacket ) 
			return false;

		pCPacket->m_HEADER.m_wType = GSV_SKILL_START;
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_SKILL_START );
		pCPacket->m_gsv_SKILL_START.m_wObjectIDX = this->Get_INDEX ();

		this->GetZONE()->SendPacketToSectors( this, pCPacket );
		Packet_ReleaseNUnlock( pCPacket );
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
/*
void CObjCHAR::Move_COMPLETED ()
{
	m_PosCUR = m_PosGOTO;
}
*/


int CObjCHAR::RealProc (DWORD dwPassTIME)
{
	if ( ( CS_BIT_MOV | CS_BIT_MOV2 ) & this->Get_STATE() ) {
		m_PosCUR += ( m_MoveVEC * dwPassTIME );
		if ( !this->GetZONE()->UpdateSECTOR( this ) ) {	// ¼½ÅÍ º¯È¯ÀÌ ÀÖÀ»°æ¿ì È£ÃâÇÏ´Â ÇÔ¼ö.
			// ÁÂÇ¥°¡ ÀÌ»óÇÑ³ÑÀÌ´Ù Â©·¯ !!!
			return 0;
		}

		this->UpdateCartGuest();
	}

	//if ( this->Get_MoveMODE() < MOVE_MODE_DRIVE && this->Get_HP() > 0 ) {
	//	// Å¾½Â ¸ðµå°¡ ¾Æ´Ï°í ¾É±â ¸ðµå¸é...
	//	this->RecoverHPnMP (dwPassTIME);
	//}

	bool bFrameING;
	bFrameING = this->ProcMotionFrame ();
	if ( bFrameING && ( Get_STATE()&CS_BIT_INT ) )	{	// ÀÌµ¿ÀÛÁß¿¡´Â ¾Ï°ÅµÎ ÇÒ¼ö ¾ø´Ù.
		return 1;
	}

	if ( 2 == Get_PatHP_MODE() ) 
		Set_PatHP_MODE( 3 );

	switch( Get_COMMAND() ) {
		case CMD_DIE		:
			return this->IsUSER ();

		case CMD_SIT		:
			return ProcCMD_SIT ();

		case CMD_STOP		:
			return ProcCMD_STOP ();

		case CMD_MOVE		:
			return ProcCMD_MOVE ();

		case CMD_RUNAWAY	:
		case CMD_PICK_ITEM	:
			return ProcCMD_PICK_ITEM ();

		case CMD_ATTACK		:
			return ProcCMD_ATTACK ();

		case CMD_SKILL2SELF :
			return ProcCMD_Skill2SELF ();

		case CMD_SKILL2POS	:
			return ProcCMD_Skill2POSITION ();

		case CMD_SKILL2OBJ	:
			return ProcCMD_Skill2OBJECT ();
	}

	return 1;
}

void CObjCHAR::Proc_IngSTATUS (DWORD dwPassTime)
{
	// ¹«Àû »©°í Áö¼Ó»óÅÂ°¡ ¼³Á¤µÆ´Ù.
	DWORD dwClearedFLAGS = this->m_IngSTATUS.Proc( this, dwPassTime );
	if ( dwClearedFLAGS ) {
		if ( dwClearedFLAGS & ( ING_INC_MOV_SPD | ING_DEC_MOV_SPD ) ) {
			// ÀÌ¼Ó¿¡ º¯È­°¡ »ý°å°í ÇöÀç ¸ð¼ÇÀÌ ÀÌµ¿ ¸ð¼ÇÀÌ¸é...ÀÌ¼Ó ¼öÁ¤
			if ( m_pCurMOTION == this->Get_MOTION( this->GetANI_Move() ) ) {
				m_fCurMoveSpeed = this->Get_MoveSPEED();
			}
		}
		if ( dwClearedFLAGS & ( ING_INC_ATK_SPD | ING_DEC_ATK_SPD ) ) {
			// °ø¼Ó¿¡ º¯È­°¡ »ý°å°í ÇöÀç ¸ð¼ÇÀÌ °ø°Ý ¸ð¼ÇÀÌ¸é...°ø¼Ó ¼öÁ¤
			if ( m_pCurMOTION == this->Get_MOTION(this->GetANI_Attack() ) ) {
				m_fCurAniSPEED = ( this->Get_nAttackSPEED() / 100.f );	
			}
		}

		// »óÅÂ°¡ ¹Ù²î¾ú´Ù....
		this->Send_gsv_CLEAR_STATUS( dwClearedFLAGS );
	}
}

int CObjCHAR::Proc (void)
{
	if ( NULL == m_pCurMOTION ) {
		g_LOG.CS_ODS( 0xffff, "		**** ERROR:: m_pCurMOTION == NULL CObjCHAR::Proc[ %s ] ..\n", this->Get_NAME() );
		return 0;
	}

	DWORD dwNeedTIME, dwPassTIME = (this->GetZONE())->GetPassTIME ();
	WORD  wRamainFRAME;

	// Áö¼Ó»óÅÂ Ã³¸®...
	if ( this->Get_HP() > 0 && this->m_IngSTATUS.GetFLAGs() ) {
		this->Proc_IngSTATUS( dwPassTIME );
		//// ¹«Àû »©°í Áö¼Ó»óÅÂ°¡ ¼³Á¤µÆ´Ù.
		//dwNeedTIME = this->m_IngSTATUS.Proc( this, dwPassTIME );
		//if ( dwNeedTIME ) {
		//	if ( dwNeedTIME & ( ING_INC_MOV_SPD | ING_DEC_MOV_SPD ) ) {
		//		// ÀÌ¼Ó¿¡ º¯È­°¡ »ý°å°í ÇöÀç ¸ð¼ÇÀÌ ÀÌµ¿ ¸ð¼ÇÀÌ¸é...ÀÌ¼Ó ¼öÁ¤
		//		if ( m_pCurMOTION == this->Get_MOTION( this->GetANI_Move() ) ) {
		//			m_fCurMoveSpeed = this->Get_MoveSPEED();
		//		}
		//	}
		//	if ( dwNeedTIME & ( ING_INC_ATK_SPD | ING_DEC_ATK_SPD ) ) {
		//		// °ø¼Ó¿¡ º¯È­°¡ »ý°å°í ÇöÀç ¸ð¼ÇÀÌ °ø°Ý ¸ð¼ÇÀÌ¸é...°ø¼Ó ¼öÁ¤
		//		if ( m_pCurMOTION == this->Get_MOTION(this->GetANI_Attack() ) ) {
		//			m_fCurAniSPEED = ( this->Get_nAttackSPEED() / 100.f );	
		//		}
		//	}

		//	// »óÅÂ°¡ ¹Ù²î¾ú´Ù....
		//	this->Send_gsv_CLEAR_STATUS( dwNeedTIME );
		//}
	}

	// ÇöÀç µ¿ÀÛÀÇ ³²Àº ÇÁ·¹ÀÓÀ» °è»ê...
	wRamainFRAME = m_pCurMOTION->Get_ReaminFRAME( m_iCurMotionFRAME );

	//assert( m_pCurMOTION->Get_TotalFRAME() == ( m_iCurMotionFRAME + wRamainFRAME ) );
	//assert( m_fCurAniSPEED != 0.f );

	// ³²Àº ÇÁ·¹ÀÓµ¿¾È ¼Ò¿äµÉ ½Ã°£°è»ê...
	dwNeedTIME = m_pCurMOTION->Get_NeedTIME ( wRamainFRAME, m_fCurAniSPEED );

	// 2003.09.19 >= ¸¦ >·Î ÇØ¼­ ÇÁ·¹ÀÓÀÌ ³Ñ¾î°¡´Â ¿À·ù°¡ ÀÖ¾úÀ½...
	while( dwPassTIME >= dwNeedTIME ) {
		// ¸ð¼ÇÀÌ ¿Ï·áµÈ´Ù..
		m_iCurMotionFRAME += wRamainFRAME;
		if ( !this->RealProc( dwNeedTIME ) )
			return 0;

		dwPassTIME  -= dwNeedTIME;

		m_iCurMotionFRAME = 0, m_fAccMotionFRAME = 0.f;

		wRamainFRAME = m_pCurMOTION->Get_TotalFRAME ();
		dwNeedTIME   = m_pCurMOTION->Get_NeedTIME( wRamainFRAME, m_fCurAniSPEED );
	}

	// ÇöÀçµ¿ÀÛÀ» dwPassTIME¸¸Å­ ÁøÇà...
	// ÃÊ´ç 30 frame ÀÏ°æ¿ì ÀüÃ¼ ÇÁ·¹ÀÓÀÌ 30º¸´Ù ÀÛÀ¸¸é???
	float fPassFrame = m_pCurMOTION->Get_PassFRAME( dwPassTIME, m_fCurAniSPEED );
	m_fAccMotionFRAME += fPassFrame;
	wRamainFRAME = (WORD)m_fAccMotionFRAME;
	if ( wRamainFRAME ) {
		m_fAccMotionFRAME -= wRamainFRAME;
		m_iCurMotionFRAME += wRamainFRAME;
		return this->RealProc( dwPassTIME );
	}

	return 1;
}

bool CObjCHAR::UpdateMinLEVInParty(){
	CParty* myParty = this->GetPARTY();
	if(myParty != NULL && myParty->isShareParty()){
		m_iMinLEVInParty = myParty->GetMinLEV();
		return true;
	}
	else{
		m_iMinLEVInParty = 0;
		return false;
	}
}

//-------------------------------------------------------------------------------------------------
