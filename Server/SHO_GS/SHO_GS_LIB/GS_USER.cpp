

#include "LIB_gsMAIN.h"

#include "GS_USER.h"
#include "ZoneLIST.h"
#include "GS_ThreadSQL.h"
#include "GS_ThreadLOG.h"
#include "CRandom.h"
#include "GS_ListUSER.h"
#include "GS_SocketLSV.h"
#include "GS_SocketASV.h"
#include "GS_Party.h"
#include "IO_Quest.h"
#include "Calculation.h"
#include "GS_ThreadMALL.h"
#include "DEF_STB.h"

#ifdef	__INC_WORLD
	#include "CChatROOM.h"
#endif
#include "CThreadGUILD.h"

#define	RET_FAILED		false	
#define	RET_OK			true
#define	RET_SKIP_PROC	2

#include "classFILE.h"
extern classLogFILE	 g_ChatLOG;
extern classLogFILE	 g_ChatGMLOG;

extern bool IsJAPAN ();


//-------------------------------------------------------------------------------------------------
classUSER::classUSER ()
{	
	COMPILE_TIME_ASSERT( sizeof(tagITEM) == (6+sizeof(__int64)) );

#ifdef	__INC_WORLD
	m_pNodeChatROOM = new CDLList< classUSER* >::tagNODE;
	m_pNodeChatROOM->m_VALUE = this;
#endif

	this->InitUSER ();
}

//-------------------------------------------------------------------------------------------------
classUSER::~classUSER ()
{
#ifdef	__INC_WORLD
	SAFE_DELETE( m_pNodeChatROOM );
#endif
}

//-------------------------------------------------------------------------------------------------
bool classUSER::IsHacking (char *szDesc, char *szFile, int iLine)
{
	g_pThreadLOG->When_ERROR ( this, szFile, iLine, szDesc );
	g_LOG.CS_ODS (0xfff, "IsHacking[ID:%s, IP:%s Char:%s] %s() : %s, %d \n", this->m_Account.Get(), this->m_IP.Get(), this->Get_NAME(), szDesc, szFile, iLine);
	this->CloseSocket ();
	return false;
}

/// Äù½ºÆ® º¸»óÀ¸·Î Á¸ ÀÌµ¿ ½ÃÄÑÁÜ
bool classUSER::Reward_WARP( int iZoneNO, tPOINTF &PosGOTO)
{	
	if ( !g_pZoneLIST->IsValidZONE( iZoneNO ) ) {
		return false;
	}

	// Çà¼ºÀÌµ¿ °ú±Ý Ã¼Å©...
	if ( !this->Check_WarpPayment(iZoneNO) )
		return false;

	if ( g_pZoneLIST->IsAgitZONE(iZoneNO) ) {
		if ( this->GetClanID() ) {
			CZoneTHREAD *pZone = g_pZoneLIST->GetZONE( iZoneNO );
			if ( pZone ) {
				tPOINTF PosNEW = pZone->Get_AgitPOS( this->GetClanID() );

				return this->Send_gsv_RELAY_REQ( RELAY_TYPE_RECALL, iZoneNO, PosNEW );
			}
		}
		return false;
	}
	
	return this->Send_gsv_RELAY_REQ( RELAY_TYPE_RECALL, iZoneNO, PosGOTO );
}

//---------------------------------------------------------------------------
/// ¼Ò¼Ó Å¬·£ÀÇ Å¬·£ º¯¼ö°ª º¯°æ
bool classUSER::Send_gsv_ADJ_CLAN_VAR (BYTE btVarType, int iValue)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( pCPacket ) {
		pCPacket->m_HEADER.m_wType = GSV_ADJ_CLAN_VAR;
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_ADJ_CLAN_VAR );

		pCPacket->m_gsv_ADJ_CLAN_VAR.m_btCMD	 = GCMD_ADJ_VAR;
		pCPacket->m_gsv_ADJ_CLAN_VAR.m_dwClanID	 = this->GetClanID();
		pCPacket->m_gsv_ADJ_CLAN_VAR.m_btVarType = btVarType;	// CLVAR_INC_LEV;
		pCPacket->m_gsv_ADJ_CLAN_VAR.m_iAdjValue = iValue;		// 1;

		g_pThreadGUILD->Add_ClanCMD( GCMD_ADJ_VAR, this->m_iSocketIDX, (t_PACKET*)( pCPacket->m_pDATA ), this->Get_NAME() );
		Packet_ReleaseNUnlock( pCPacket );
		return true;
	}
	return false;
} 

/// ¹è¿î Å¬·£ ½ºÅ³À» Ã£À½
BYTE classUSER::FindClanSKILL(short nSkillNo1, short nSkillNo2)
{
#ifdef	MAX_CLAN_SKILL_SLOT
	for (short nI=0; nI<MAX_CLAN_SKILL_SLOT; nI++) {
		if ( this->m_CLAN.m_ClanBIN.m_SKILL[ nI ].m_nSkillIDX >= nSkillNo1 &&
			 this->m_CLAN.m_ClanBIN.m_SKILL[ nI ].m_nSkillIDX <= nSkillNo2 ) {
			if ( SKILL_NEED_LEVELUPPOINT( this->m_CLAN.m_ClanBIN.m_SKILL[ nI ].m_nSkillIDX ) ) {
				// ¸¸·á ³¯Â¥ Ã¼Å©...
				if ( this->GetCurAbsSEC() >= this->m_CLAN.m_ClanBIN.m_SKILL[ nI ].m_dwExpiredAbsSEC ) {
					this->m_CLAN.m_ClanBIN.m_SKILL[ nI ].m_nSkillIDX = 0;
					this->m_CLAN.m_ClanBIN.m_SKILL[ nI ].m_dwExpiredAbsSEC = 0;
					return MAX_CLAN_SKILL_SLOT;
				}
			}
			return (BYTE)nI;
		}
	}
	return MAX_CLAN_SKILL_SLOT;
#else
	return 0;
#endif
}

/// Å¬·£ ½ºÅ³ Ãß°¡
bool classUSER::AddClanSKILL (short nSkillNo)
{
#ifdef	MAX_CLAN_SKILL_SLOT
	if ( MAX_CLAN_SKILL_SLOT == this->FindClanSKILL(nSkillNo,nSkillNo) ) {
		// µî·ÏµÈ ½ºÅ³ÀÌ ¾Æ´Ï´Ù...
		if ( this->Send_gsv_ADJ_CLAN_VAR( CLVAR_ADD_SKILL, nSkillNo ) ) {
			g_pThreadLOG->When_LearnSKILL( this, nSkillNo );
			return true;
		}
	}
#endif
	return false;
}

/// Å¬·£ ½ºÅ³ »èÁ¦
bool classUSER::DelClanSKILL (short nSkillNo)
{
#ifdef	MAX_CLAN_SKILL_SLOT
	if ( MAX_CLAN_SKILL_SLOT != this->FindClanSKILL(nSkillNo,nSkillNo) ) {
		if ( this->Send_gsv_ADJ_CLAN_VAR( CLVAR_DEL_SKILL, nSkillNo ) ) {
			//g_pThreadLOG->When_LearnSKILL( this, (-1) * nSkillIDX );
		}
	}
#endif
	return false;
}


/// Å¬·£ ·¹º§ Áõ°¡
void classUSER::IncClanLEVEL ()
{
	if ( this->GetClanID() ) {
		// ÇöÀç Å¬·£ ·¹º§ ¿Ã¸² & db¿¡ ±â·Ï & WS·Î Àü¼Û->¸ðµç Å¬·£¿ø¿¡°Ô Åëº¸
		this->Send_gsv_ADJ_CLAN_VAR( CLVAR_INC_LEV, 1 );
	}
}

/// Å¬·£ ¼ÒÀ¯ÀÇ ÁÙ¸® Ãß°¡
void classUSER::AddClanMONEY(int iMoney)
{
	if ( this->GetClanID() ) {
		// ÇöÀç Å¬·£ ¸Ó´Ï Á¶Á¤ & db¿¡ ±â·Ï & WS·Î Àü¼Û->¸ðµç Å¬·£¿ø¿¡°Ô Åëº¸
		this->Send_gsv_ADJ_CLAN_VAR( CLVAR_ADD_ZULY, iMoney );
	}
}

/// Å¬·£ Á¡¼ö Áõ°¡
void classUSER::AddClanSCORE(int iScore)
{
	if ( this->GetClanID() ) {
		// ÇöÀç Å¬·£ Á¡¼ö Á¶Á¤ & db¿¡ ±â·Ï & WS·Î Àü¼Û->¸ðµç Å¬·£¿ø¿¡°Ô Åëº¸
		this->Send_gsv_ADJ_CLAN_VAR( CLVAR_ADD_SCORE, iScore );
	}
}

//-------------------------------------------------------------------------------------------------
/// Å¬·£ÀÇ Ã¢¼³ Á¶°ÇÀÌ ¸Â´ÂÁö Ã¼Å©..
bool classUSER::CheckClanCreateCondition (char cStep)
{
	#define	NEED_CLAN_CREATE_LEVEL	30
	#define	NEED_CLAN_CREATE_MONEY	1000000

	if ( this->Get_TRUE_LEVEL() < NEED_CLAN_CREATE_LEVEL )
		return false;

	bool bResult = true;
	switch( cStep ) {
		case 0 :	// »ý¼º ½ÃÀÛ Á¶°Ç¸¸ Ã¼Å©...
			if ( this->GetCur_MONEY() < NEED_CLAN_CREATE_MONEY ) {
				bResult = false;
				break;
			}

			g_pThreadLOG->When_gs_CLAN (this, "Start Create", NEWLOG_CLAN_CREATE_START );	// µ· »©±âÀü¿¡..

			this->LockSOCKET ();
				this->Sub_CurMONEY( NEED_CLAN_CREATE_MONEY );
				this->m_iClanCreateMoney = NEED_CLAN_CREATE_MONEY;
			this->UnlockSOCKET ();
			break;

		case 1 :	// »ý¼º½Ã 
		{
			char szTmp[ 128 ];
			this->LockSOCKET ();
				if ( NEED_CLAN_CREATE_MONEY != this->m_iClanCreateMoney ) {
					bResult = false;
					sprintf(szTmp, "Failed Create is Hacking?? NeedZuly: %d / %d", this->m_iClanCreateMoney, NEED_CLAN_CREATE_MONEY );
				}
				this->m_iClanCreateMoney = 0;
			this->UnlockSOCKET ();

			g_pThreadLOG->When_gs_CLAN (this, ( bResult ) ? "Success Create" : szTmp, ( bResult )?NEWLOG_CLAN_CREATE_SUCCESS:NEWLOG_CLAN_CREATE_FAILED );
			break;
		}

		case 2 :	// »ý¼º ½ÇÆÐ
			g_pThreadLOG->When_gs_CLAN (this, "Failed Create", NEWLOG_CLAN_CREATE_FAILED );	// µ· º¹±¸ÇÑ ÈÄ¿¡...

			this->LockSOCKET ();
				this->Add_CurMONEY( this->m_iClanCreateMoney );
				this->m_iClanCreateMoney = 0;
			this->UnlockSOCKET ();
			break;
	}

	return bResult;
}

//-------------------------------------------------------------------------------------------------
/// Å¬¶óÀÌ¾ðÆ®·Î ºÎÅÍ ¹ÞÀº Å¬·£ ¸¶Å© ¼³Á¤ ÆÐÅ¶ Ã³¸®
bool classUSER::Recv_cli_CLANMARK_SET( t_PACKET *pPacket )
{
	if ( this->GetClanID() ) {
		g_pThreadGUILD->Add_ClanCMD( GCMD_CLANMARK_SET, this->m_iSocketIDX, pPacket );
	}

	return true;
}

/// Å¬¶óÀÌ¾ðÆ®·Î ºÎÅÍ ¹ÞÀº Å¬·£ ¸¶Å© ¿äÃ» ÆÐÅ¶ Ã³¸®
bool classUSER::Recv_cli_CLANMARK_REQ( t_PACKET *pPacket )
{
	g_pThreadGUILD->Add_ClanCMD( GCMD_CLANMARK_GET, this->m_iSocketIDX, pPacket );
	return true;
}

/// Å¬¶óÀÌ¾ðÆ®°¡ ¿äÃ»ÇÑ Å¬·£ ¸¶Å© µî·Ï ³¯Â¥/½Ã°£ Àü¼Û..
bool classUSER::Recv_cli_CLANMARK_REG_TIME( t_PACKET *pPacket )
{
	g_pThreadGUILD->Add_ClanCMD( GCMD_CLANMARK_REGTIME, this->m_iSocketIDX, pPacket );
	return true;
}

/// Å¬¶óÀÌ¾ðÆ®°¡ ¿äÃ»ÇÑ Å¬·£ °ü·Ã ÆÐÅ¶¿¡ ´ëÇÑ ÀÀ´ä ÆÐÅ¶ »ý¼ºÈÄ Àü¼Û
bool classUSER::Send_wsv_CLANMARK_REPLY( DWORD dwClanID, WORD wMarkCRC, BYTE *pMarkData, short nDataLen )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = WSV_CLANMARK_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( wsv_CLANMARK_REPLY );

	pCPacket->m_wsv_CLANMARK_REPLY.m_dwClanID   = dwClanID;
	pCPacket->m_wsv_CLANMARK_REPLY.m_wMarkCRC16 = wMarkCRC;
	pCPacket->AppendData( pMarkData, nDataLen );

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
	return true;
}

//-------------------------------------------------------------------------------------------------
/// ¼­¹ö¿¡ Áßº¹µÇÁö ¾ÊÀº ÆÀ¹øÈ£ ¼³Á¤
bool classUSER::Set_TeamNoFromUNIQUE ()
{
	this->SetCur_TeamNO( 100 + this->Get_INDEX() );
	return true;
}

/// Å¬·£´ÜÀ§·Î ÆÀ¹øÈ£ ¼³Á¤
bool classUSER::Set_TeamNoFromClanIDX ()
{
	if ( this->GetClanID() ) {
		this->SetCur_TeamNO( 100 + this->GetClanID() );
		return true;
	}
	return false;
}

/// ÆÄÆ¼´ÜÀ§·Î ÆÀ¹øÈ£ ¼³Á¤
bool classUSER::Set_TeamNoFromPartyIDX ()
{
	if ( this->GetPARTY() ) {
		this->SetCur_TeamNO( 100 + this->GetPARTY()->m_wPartyWSID );
		return true;
	}
	return false;
}

/// ºÎÈ° À§Ä¡ ¼³Á¤
bool classUSER::Set_RevivePOS( int iXPos, int iYPos )
{
	if ( !this->GetZONE() )
		return false;

	this->m_bSetImmediateRevivePOS = true;
	this->m_PosImmediateRivive.x = (float)iXPos;
	this->m_PosImmediateRivive.y = (float)iYPos;

	return true;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Ä¡Æ® ÄÚÆ® ±¸¹® ºÐ¼®
short classUSER::Parse_CheatCODE (char *szCode)
{
	short nProcMODE=0;

	char *pToken, *pArg1, *pArg2, *pArg3;
	char *pDelimiters = " ";

	CStrVAR *pStrVAR = this->GetZONE()->GetStrVAR ();

	pToken = pStrVAR->GetTokenFirst (szCode, pDelimiters);
	pArg1  = pStrVAR->GetTokenNext  (pDelimiters);
	if ( NULL == pToken )
		return CHEAT_INVALID;

	if ( pArg1 ) {
#ifdef	__NEW_LOG
		if ( !strcmpi( pToken, "/logtest" ) ) {
//			g_pThreadLOG->When_DeletedITEM( xxx );

			//g_pThreadLOG->When_TagItemLOG( int iAction, classUSER *pSourAVT, tagITEM *pItem, short nQuantity=1, __int64 biTradeZuly=0, classUSER *pDestAVT=NULL);
			//g_pThreadLOG->When_ObjItemLOG( int iAction, classUSER *pSourAVT, CObjITEM *pItemOBJ);

			//g_pThreadLOG->When_CreateOrDestroyITEM ( classUSER *pSourAVT, tagITEM *pOutItem, tagITEM *pUseItem, short nUseCNT, BYTE btMakeOrBreak, BYTE btSucOrFail );
			//g_pThreadLOG->When_DieBY( CObjCHAR *pKillOBJ, classUSER *pDeadAVT );

			//// ·¹º§¾÷
			//g_pThreadLOG->When_UpgradeITEM	( this, tagITEM *pEquipITEM, BYTE btBeforeGrade, bool bSuccess );
			//g_pThreadLOG->When_GemmingITEM	( this,	tagITEM *pEquipITEM, tagITEM *pJewelITEM, BYTE btGemming, BYTE btSuccess );
			//g_pThreadLOG->When_CheatCODE ( this, szCode );

			return 1;
		}
#endif

#ifdef	__INC_WORLD
		if ( !strcmpi( pToken, "/test_view" ) ) {
			int iNpcNO = atoi( pArg1 );
			if ( iNpcNO > 0 && iNpcNO < g_TblNPC.m_nDataCnt ) {
				CObjNPC *pNPC = g_pZoneLIST->Get_LocalNPC( iNpcNO );
				if ( pNPC ) {
					pNPC->VSet_SHOW( 2 );
				}
			}
		} else
		if ( !strcmpi( pToken, "/mall" ) ) {
			classPACKET *pCPacket = Packet_AllocNLock ();
			if ( !pCPacket )
				return 0;
			pCPacket->m_HEADER.m_wType = CLI_MALL_ITEM_REQ;
			pCPacket->m_HEADER.m_nSize = sizeof( cli_MALL_ITEM_REQ );
			pCPacket->m_cli_MALL_ITEM_REQ.m_btReqTYPE = 0;
		
			pArg2 = pStrVAR->GetTokenNext (pDelimiters);
			if ( !strcmpi( pArg1, "list" ) ) {
				pCPacket->m_cli_MALL_ITEM_REQ.m_btReqTYPE = REQ_MALL_ITEM_LIST;
			} else
			if ( !strcmpi( pArg1, "get" ) ) {
				// index
				int iInvIDX=0;
				if ( pArg2 ) {
					iInvIDX = atoi( pArg2 );
				}
				pCPacket->m_HEADER.m_nSize = sizeof( cli_MALL_ITEM_REQ ) + 1;
				pCPacket->m_cli_MALL_ITEM_REQ.m_btReqTYPE = REQ_MALL_ITEM_BRING;
				pCPacket->m_cli_MALL_ITEM_REQ.m_nDupCnt   = 2;
				pCPacket->m_cli_MALL_ITEM_REQ.m_btInvIDX[0] = iInvIDX;
			} else
			if ( !strcmpi( pArg1, "check" ) ) {
				// char name
				pCPacket->m_cli_MALL_ITEM_REQ.m_btReqTYPE = REQ_MALL_ITEM_FIND_CHAR;
				if ( pArg2 )
					pCPacket->AppendString( pArg2 );
			} else
			if ( !strcmpi( pArg1, "give" ) ) {
				// index
				pCPacket->m_HEADER.m_nSize = sizeof( cli_MALL_ITEM_REQ ) + 1;
				pCPacket->m_cli_MALL_ITEM_REQ.m_btReqTYPE = REQ_MALL_ITEM_GIVE;
                pArg3 = pStrVAR->GetTokenNext (pDelimiters);
				if ( pArg3 ) {
					pCPacket->m_cli_MALL_ITEM_REQ.m_nDupCnt = 2;
					pCPacket->m_cli_MALL_ITEM_REQ.m_btInvIDX[0] = atoi( pArg3 );
					pCPacket->AppendString( pArg2 );

	                pArg3 = pStrVAR->GetTokenNext (pDelimiters);	// desc[]
					pCPacket->AppendString( pArg3 );
				}
			} 

			this->Recv_cli_MALL_ITEM_REQ( (t_PACKET*)( pCPacket->m_pDATA ) );
			Packet_ReleaseNUnlock( pCPacket );
			return 1;
		}
#endif
		if ( !strcmpi( pToken, "/pay" ) ) {
			int iPayType = atoi( pArg1 );
			if ( iPayType > 0 && iPayType <= BILLING_MSG_PAY_IQ ) {
				this->Send_gsv_BILLING_MESSAGE( iPayType, "200512312400" );
			}
		}
		if ( !strcmpi( pToken, "/team" ) ) {
			int iTeamNo = atoi( pArg1 );
			if ( iTeamNo >= 0 ) {
				this->m_iTeamNO = iTeamNo;
			}
		}
		if ( !strcmpi( pToken, "/where" ) ) {
			pArg2 = pStrVAR->GetTokenNext (pDelimiters);	// account
			return Cheat_where( pStrVAR, pArg1, pArg2, szCode );
		}
		if ( !strcmpi( pToken, "/account" ) ) {
			return Cheat_account( pArg1, szCode );
		}
		if ( !strcmpi( pToken, "/move" ) ) {
			pArg2 = pStrVAR->GetTokenNext (pDelimiters);	// account
			return Cheat_move( pArg1, pArg2, szCode );
		}
		// ¸ÊÀÌµ¿..
		if ( !strcmpi( pToken, "/mm" ) ) {
			// pArg1 // zone no
			short nZoneNO = atoi( pArg1 );
			if( pArg1[0] == 'h' || pArg1[0] == '?') {
				//Numenor: print help and the list of zone where you can tp
				this->Send_gsv_WHISPER( "<SERVER>::", "The command /mm will tp you on an other map.");
				this->Send_gsv_WHISPER( "<SERVER>::", "Usage 1: /mm mapNo" );
				this->Send_gsv_WHISPER( "<SERVER>::", "Usage 2: /mm mapNo [Xcoordinate] [Ycoordinate]" );

				for( short zone = 1; zone < g_pZoneLIST->GetZoneCNT(); zone++){
					if( g_pZoneLIST->IsValidZONE(zone) )this->Send_gsv_WHISPER( "<SERVER>::", CStr::Printf("%d : %s", zone, g_pZoneLIST->GetZONE(zone)->Get_NAME() ));
				}
			}
			else if(nZoneNO){
				pArg2 = pStrVAR->GetTokenNext (pDelimiters);	// x pos	
				pArg3 = pStrVAR->GetTokenNext (pDelimiters);	// y pos
				if ( pArg2 != 0 && pArg3 != 0 ) {
					return Cheat_mm( nZoneNO, pArg2, pArg3 );
				} else{
					return Cheat_mm( nZoneNO, "0", "0" ); // LZO added the else if option, in case we don't provide XY position. Numenor: Be careful, the function asks for a string, so "0" and not 0.
				}
				}

		}
		if ( !strcmpi( pToken, "/NPC") ) {
			// pArg1 == npc no
			int iNpcIDX = atoi( pArg1 );
			if ( iNpcIDX ) {
				CObjNPC *pNPC = g_pZoneLIST->Get_LocalNPC( iNpcIDX );
				if ( pNPC ) {
					pArg2 = pStrVAR->GetTokenNext (pDelimiters);	// move, call, var
					pArg3 = pStrVAR->GetTokenNext (pDelimiters);	// event id, var idx
					char *pArg4 = pStrVAR->GetTokenNext (pDelimiters);	// var value
					return Cheat_npc( pStrVAR, pNPC, iNpcIDX, pArg2, pArg3, pArg4 );
				}
			}
			return CHEAT_INVALID;
		}

		if ( !strcmpi( pToken, "/ADD" ) ) {
			// Æ÷ÀÎÆ® »ó½Â Ä¡Æ®ÄÚµå...
			pArg2 = pStrVAR->GetTokenNext (pDelimiters);
			pArg3 = pStrVAR->GetTokenNext (pDelimiters);
			return Cheat_add( pArg1, pArg2, pArg3, szCode );
		}
		if ( !strcmpi( pToken, "/DEL" ) ) {
			// Æ÷ÀÎÆ® »ó½Â Ä¡Æ®ÄÚµå...
			pArg2 = pStrVAR->GetTokenNext (pDelimiters);
			pArg3 = pStrVAR->GetTokenNext (pDelimiters);
			nProcMODE = Cheat_del( pStrVAR, pArg1, pArg2, pArg3 );
		}
		// ¾ÆÀÌÅÛ °ü·Ã Ä¡Æ®ÄÚµå...
		if ( !strcmpi( pToken, "/ITEM" ) ) {
			pArg2 = pStrVAR->GetTokenNext (pDelimiters);
			pArg3 = pStrVAR->GetTokenNext (pDelimiters);
			if( pArg1[0] == 'h' || pArg1[0] == '?') {
				//Numenor: print help and the list of zone where you can tp
				this->Send_gsv_WHISPER( "<SERVER>::", "The command /item adds an item to your inventory.");
				this->Send_gsv_WHISPER( "<SERVER>::", "Equipment: /item Type ID Stat (-1 is socketted)" );
				this->Send_gsv_WHISPER( "<SERVER>::", "Other: /item Type ID Amount" );
				this->Send_gsv_WHISPER( "<SERVER>::", "Write /item Type h or /item Type ? to get a list of all items of this type" );

				std::vector<char*> list_item;
				list_item.push_back("Mask"); //1
				list_item.push_back("Hat"); //2
				list_item.push_back("Chest"); //3
				list_item.push_back("Gloves"); //4
				list_item.push_back("Shoes"); //5
				list_item.push_back("Back Item"); //6
				list_item.push_back("Jewelry"); //7
				list_item.push_back("Weapon"); //8
				list_item.push_back("Shield"); //9
				list_item.push_back("Consumable"); //10
				list_item.push_back("Gem"); //11
				list_item.push_back("Etc"); //12
				list_item.push_back("Quest?"); //13
				list_item.push_back("PAT"); //14

				for( short item = 0; item < list_item.size(); item++){
					this->Send_gsv_WHISPER( "<SERVER>::", CStr::Printf("%d : %s", item+1, list_item[item] ));
				}
				return CHEAT_NOLOG;
			}
			else if( pArg2 && (pArg2[0] == 'h' || pArg2[0] == '?')) {
				int iItemTYPE = atoi( pArg1 );
				bool printInfo = true;
				if ( iItemTYPE < 1 || iItemTYPE >= ITEM_TYPE_QUEST ) {
					if ( iItemTYPE > ITEM_TYPE_QUEST && iItemTYPE <= ITEM_TYPE_RIDE_PART ) {
					;
					} else{
						this->Send_gsv_WHISPER( "<SERVER>::", CStr::Printf("Invalid type for /item command."));
						printInfo=false;
					}
				}
				if(printInfo){
					for (int item = 1; item < g_pTblSTBs[ iItemTYPE ]->m_nDataCnt; item++){
						if( g_pTblSTBs[ iItemTYPE ]->m_ppNAME[item] != NULL ) this->Send_gsv_WHISPER( "<SERVER>::", CStr::Printf("%d : %s", item, g_pTblSTBs[ iItemTYPE ]->m_ppNAME[item] ));
					}
				}
				return CHEAT_NOLOG;

			}
			else{
				if ( !pArg2 || !pArg3 )
					return CHEAT_INVALID;
				char *pArg4 = pStrVAR->GetTokenNext (pDelimiters);
				return Cheat_item( pArg1, pArg2, pArg3, pArg4 );
			}
		}

		if ( !strcmpi( pToken, "/GET" ) ) {
			pArg2 = pStrVAR->GetTokenNext (pDelimiters);	// account
			return Cheat_get ( pStrVAR, pArg1, pArg2, szCode );
		}
	} else {
		if ( !strcmpi( pToken, "/respawn" ) ) {
			// ÀúÀåµÈ ºÎÈ°Àå¼Ò¿¡¼­ »ì¾Æ³ª±â..
			this->Recv_cli_REVIVE_REQ( REVIVE_TYPE_SAVE_POS );
			return CHEAT_NOLOG;
		}
	}

	if ( C_Cheater() ) {
		if ( pArg1 ) {
			if ( !strcmpi( pToken, "/nc" ) ) {
				// ¼­¹ö ÀüÃ¼ °øÁö
				g_pZoneLIST->Send_gsv_ANNOUNCE_CHAT( &szCode[4], this->Get_NAME() );
				return CHEAT_PROCED;
			} else
			if ( !strcmpi( pToken, "/nz" ) ) {		
				// ÇöÀç ¸Ê °øÁö
				g_pZoneLIST->Send_gsv_ANNOUNCE_CHAT( this->GetZONE()->Get_ZoneNO(), &szCode[4], this->Get_NAME() );
				return CHEAT_PROCED;
			}

			// ¸÷ ¼ÒÈ¯
			if ( !strcmpi( pToken, "/mon" ) ) {
				// pArg1 Mob IDX
				if( pArg1[0] == 'h' || pArg1[0] == '?') {
					//Numenor: print help and the list of monsters you can summon
					this->Send_gsv_WHISPER( "<SERVER>::", "The command /mon summons monster.");
					this->Send_gsv_WHISPER( "<SERVER>::", "Usage: /mon ID Amount" );
					this->Send_gsv_WHISPER( "<SERVER>::", "Write /mon h or /mon ? to get a list of all available monsters" );

					int iItemTYPE = atoi( pArg1 );
					for (int npc = 1; npc < g_TblNPC.m_nDataCnt; npc++){
						if( NPC_NAME(npc) != NULL ) this->Send_gsv_WHISPER( "<SERVER>::", CStr::Printf("%d : %s", npc, NPC_NAME(npc) ));
					}
					return CHEAT_NOLOG;
				}
				pArg2 = pStrVAR->GetTokenNext (pDelimiters);	// mob cnt
				if ( pArg2 ) {
					return Cheat_mon ( pArg1, pArg2 );
				}
			} 
			
			// ¸÷ »èÁ¦
			if ( !strcmpi( pToken, "/damage" ) ) {
				// pArg1 Distance
				// pArg2 Damage
				// pArg3 DamageType
				pArg2 = pStrVAR->GetTokenNext (pDelimiters);
				if ( pArg2 ) {
					pArg3 = pStrVAR->GetTokenNext (pDelimiters);
					return Cheat_damage( pArg1, pArg2, pArg3 );
				}
			}
		} else {	// !pArg1
			// ¹«Àû Ä¡Æ®ÄÚµå
			if ( !strcmpi( pToken, "/invincible" ) && ( GM_Cheater () || TWGM_Cheater() )) {
				m_IngSTATUS.ToggleSubFLAG( FLAG_CHEAT_INVINCIBLE );
				this->Send_gsv_WHISPER( "SERVER", this->m_IngSTATUS.IsSubSET( FLAG_CHEAT_INVINCIBLE ) ? "Invincible Mode" : "Normal Mode" );
				return CHEAT_NOLOG;
			} else
			if ( !strcmpi( pToken, "/dead" ) ) {
				this->Set_HP( 10 );
				this->SetCMD_ATTACK( this->Get_INDEX() );
				return CHEAT_NOLOG;
			}
		}
	}

	if( C_Cheater() || TWGM_Cheater() )
	{
		if( pArg1 )
		{
			if ( !strcmpi( pToken, "/na" ) ) {
			#ifdef	__INC_WORLD
				g_pZoneLIST->Send_gsv_ANNOUNCE_CHAT( &szCode[4], this->Get_NAME() );
			#else
				g_pSockLSV->Send_gsv_CHEAT_REQ( this, this->m_dwWSID, 0, szCode );
			#endif
			} else 
			// Å¸°Ù ÄÉ¸¯ÅÍ ¼ÒÈ¯...
			if ( !strcmpi( pToken, "/call" ) ) {
				if ( this->GetZONE() ) {
					pArg2 = pStrVAR->GetTokenNext (pDelimiters);	// account
					return Cheat_call ( pArg1, pArg2, szCode );
				}

				return CHEAT_NOLOG;
			} else
			if ( !strcmpi( pToken, "/out" ) ) {
				// °­Á¦ Á¢¼Ó Á¾·á...
				pArg2 = pStrVAR->GetTokenNext (pDelimiters);	// account
				return Cheat_out ( pArg1, pArg2, szCode );
			} else
			if ( !strcmpi( pToken, "/shut" ) ) {
				// ¸»ÇÏ±â ±ÝÁö...
				pArg2 = pStrVAR->GetTokenNext (pDelimiters);	// block time
				pArg3 = pStrVAR->GetTokenNext (pDelimiters);	// account
				return Cheat_shut( pArg1, pArg2, pArg3, szCode );
			}
		}
	}

	if( TWGM_Cheater() )
	{
		if( pArg1 )
		{
			if ( !strcmpi( pToken, "/speed" ) ) {
				// ¸»ÇÏ±â ±ÝÁö...
				return Cheat_speed( pArg1 );
			} else
			// ¸÷ ¼ÒÈ¯
			if ( !strcmpi( pToken, "/mon2" ) ) {
				// pArg1 Mob IDX
				char * pArg2 = pStrVAR->GetTokenNext (pDelimiters);	// X
				char * pArg3 = pStrVAR->GetTokenNext (pDelimiters);	// Y
				char * pArg4 = pStrVAR->GetTokenNext (pDelimiters);	// ¸¶¸®¼ö , pArg1 : ¸ó½ºÅÍ ÀÎµ¦½º
				if ( pArg2 && pArg3 && pArg4 ) {
					return Cheat_mon2 ( pArg1, pArg2, pArg3, pArg4 );
				}
			} 
		}

		if ( !strcmpi( pToken, "/HIDE" ) ) {
			this->m_IngSTATUS.ToggleSubFLAG( FLAG_SUB_HIDE );
			DWORD dwFlag = this->m_IngSTATUS.m_dwSubStatusFLAG;
			Send_gsv_CHARSTATE_CHANGE( dwFlag );
			return CHEAT_NOLOG;
		} 
	}

	if ( pArg1 ) {
		// Äù½ºÆ® Ã¼Å©...
		if ( !strcmpi(pToken, "/QUEST" ) ) {
			if ( !strcmpi( pArg1, "all" ) ) {
				g_QuestList.CheckAllQuest( this );
				return CHEAT_NOLOG;
			}
			return Cheat_quest ( pStrVAR, pArg1 );
		} 

		// ´É·ÂÄ¡ »ó½Â Ä¡Æ®ÄÚµå...
		if ( !strcmpi(pToken, "/FULL") && B_Cheater() ) {
			if ( !strcmpi(pArg1, "HP") ) {
				this->Set_HP( this->Get_MaxHP() );
				nProcMODE = CHEAT_SEND;
			} else
			if ( !strcmpi(pArg1, "MP") ) {
				this->Set_MP( this->Get_MaxMP() );
				nProcMODE = CHEAT_SEND;
			}
		} else
		if ( !strcmpi( pToken, "/SET" ) ) {
			pArg2 = pStrVAR->GetTokenNext (pDelimiters);
			if ( pArg2 ) {
				classUSER *pUSER=NULL;
				pArg3 = pStrVAR->GetTokenNext (pDelimiters);	// ´ë»ó.
				if ( pArg3 ) {
					pUSER = g_pUserLIST->Find_CHAR( pArg3 );
				}
				nProcMODE = Cheat_set( pUSER, pArg1, pArg2, pArg3 );
			} else
				return CHEAT_INVALID;
		}
		if ( !strcmpi( pToken, "/TOGGLE" ) ) {
			return Cheat_toggle( pStrVAR, pArg1 );
		}
		if ( !strcmpi( pToken, "/kill_all" ) && B_Cheater() ) {
			this->GetZONE()->Kill_AllMOB ( this );
			return CHEAT_NOLOG;
		}
		if ( !strcmpi( pToken, "/RESET" ) && A_Cheater()  ) {
			if ( !strcmpi( pArg1, "QUEST" ) ) {
				pStrVAR->Printf( "Reset quest result: %d", g_QuestList.LoadQuestTable () );
				this->Send_gsv_WHISPER( "SERVER", pStrVAR->Get() );
			} else
			if ( !strcmpi( pArg1, "REGEN" ) ) {
				this->GetZONE()->Reset_REGEN ();
				pStrVAR->Printf( "Zone %d Reset regen point", this->GetZONE()->Get_ZoneNO() );
				this->Send_gsv_WHISPER( "SERVER", pStrVAR->Get() );
			}
			return CHEAT_NOLOG;
		} else
		if ( !strcmpi( pToken, "/REGEN" ) && A_Cheater() ) {
			pArg2 = pStrVAR->GetTokenNext (pDelimiters);
			return Cheat_regen ( pStrVAR, pArg1, pArg2 );
		} else
		if ( !strcmpi( pToken, "/HIDE" ) && C_Cheater() ) {
			this->m_IngSTATUS.ToggleSubFLAG( FLAG_SUB_HIDE );
			return CHEAT_NOLOG;
		}
	} else {
		if ( !strcmpi( pToken, "/revive" ) ) {
			// ÇöÀç Á¸ÀÇ ºÎÈ°Àå¼Ò¿¡¼­..
			this->Recv_cli_REVIVE_REQ( REVIVE_TYPE_REVIVE_POS );
			return CHEAT_NOLOG;
		} else 
		if ( !strcmpi( pToken, "/alive" ) ) {
			// ÇöÀç Á¸ÀÇ ºÎÈ°Àå¼Ò¿¡¼­..
			this->Set_HP( this->GetCur_MaxHP() );
			this->Recv_cli_REVIVE_REQ( REVIVE_TYPE_CURRENT_POS );
			return CHEAT_NOLOG;
		} else
		if ( !strcmpi( pToken, "/SAVE" ) ) {
			g_pThreadSQL->Add_BackUpUSER( this );
			return CHEAT_NOLOG;
		}
	}

	if ( CHEAT_SEND == nProcMODE ) {
        classPACKET *pCPacket = Packet_AllocNLock ();

        pCPacket->m_HEADER.m_wType = GSV_CHEAT_CODE;
        pCPacket->m_HEADER.m_nSize = sizeof( gsv_CHEAT_CODE );
        pCPacket->m_gsv_CHEAT_CODE.m_wObjectIDX = this->Get_INDEX();
        pCPacket->AppendString( szCode );

        this->SendPacket( pCPacket );

        Packet_ReleaseNUnlock( pCPacket );
		return CHEAT_SEND;
	}

	return CHEAT_INVALID;
}

short classUSER::Check_CheatCODE (char *szCode)
{
/*
	if ( this->GetPARTY() && this == this->GetPARTY()->GetPartyOWNER() ) {
		if ( !strncmp( szCode, "/pexp", 5 ) ) {
			if ( this->GetPARTY()->m_btPartyRULE & BIT_PARTY_RULE_EXP_PER_PLAYER ) {
				this->GetPARTY()->m_btPartyRULE &= ~BIT_PARTY_RULE_EXP_PER_PLAYER;
				this->GetPARTY()->SendWhisperToPartyMembers( "»ç³É½Ã ¾ò´Â °æÇèÄ¡°¡ ÀÏÁ¤ÇÏ°Ô ºÐ¹èµË´Ï´Ù." );
			} else {
				this->GetPARTY()->m_btPartyRULE |= BIT_PARTY_RULE_EXP_PER_PLAYER;
				this->GetPARTY()->SendWhisperToPartyMembers( "»ç³É½Ã ¾ò´Â °æÇèÄ¡°¡ ·¹º§ºñ·Ê·Î ºÐ¹èµË´Ï´Ù." );
			}
			return 1;
		} else
		if ( !strncmp( szCode, "/pitem", 6 ) ) {
			if ( this->GetPARTY()->m_btPartyRULE & BIT_PARTY_RULE_ITEM_TO_ORDER ) {
				this->GetPARTY()->m_btPartyRULE &= ~BIT_PARTY_RULE_ITEM_TO_ORDER;
				this->GetPARTY()->SendWhisperToPartyMembers( "ÆÄÆ¼¼ÒÀ¯ÀÇ ¾ÆÀÌÅÛ ¿ì¼±±ÇÀÌ ¸ðµç ÆÄÆ¼¿ø¿¡°Ô ÀÖ½À´Ï´Ù." );
			} else {
				this->GetPARTY()->m_btPartyRULE |= BIT_PARTY_RULE_ITEM_TO_ORDER;
				this->GetPARTY()->SendWhisperToPartyMembers( "ÆÄÆ¼¼ÒÀ¯ÀÇ ¾ÆÀÌÅÛ ½Àµæ½Ã ¼øÂ÷ÀûÀ¸·Î ÀÚµ¿ ºÐ¹èµË´Ï´Ù." );
			}
			return 1;
		}
	}
	if ( this->m_dwRIGHT < CHEAT_GM || !this->GetZONE() ) {
		return 0;
	}
*/
	if ( 0 == C_Cheater() && 0 == TWGM_Cheater() )
		return 0; 

	if ( !strncmp( szCode, "/clan ", 5 ) ) {
		this->GuildCMD( &szCode[5] );
		return 1;
	} else
	if ( !strncmp( szCode, "/guild ", 7 ) ) {
		this->GuildCMD( &szCode[7] );
		return 1;
	}

	short nRet = this->Parse_CheatCODE( szCode );
	if ( nRet > CHEAT_NOLOG) {
		g_pThreadLOG->When_CheatCODE( this, szCode );
	}
	return nRet;
}


//-------------------------------------------------------------------------------------------------
/// Ã¤ÆÃÀ¸·Î ¹ÞÀº Å¬·£ ¸í·É Ã³¸®..
short classUSER::GuildCMD (char *szCMD)
{
	char *pArg1, *pArg2, *pArg3;
	char *pDelimiters = " ";

	CStrVAR *pStrVAR = this->GetZONE()->GetStrVAR ();

	pArg1 = pStrVAR->GetTokenFirst (szCMD, pDelimiters);
	if ( NULL == pArg1 )
		return 0;

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return 0;

	if ( !strcmpi(pArg1, "create") ) {	//±æµå»ý¼º, /guildcreate [±æµå¸í]
		pArg2 = pStrVAR->GetTokenNext (pDelimiters);
		pArg3 = pStrVAR->GetTokenNext (pDelimiters);
		if ( pArg2 && pArg3 ) {
			pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
			pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_CREATE );
			pCPacket->m_cli_CLAN_CREATE.m_btCMD	= GCMD_CREATE;

			pCPacket->m_cli_CLAN_CREATE.m_wMarkIDX[ 0 ] = 1;
			pCPacket->m_cli_CLAN_CREATE.m_wMarkIDX[ 1 ] = 2;

			pCPacket->AppendString( pArg2 );
			pCPacket->AppendString( pArg3 );
			this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
		}
	} else
	if ( !strcmpi(pArg1, "info") ) {	//±æµåÁ¤º¸, /ginfo - ±æµå¿¡ ´ëÇÑ ±âº»ÀûÀÎ Á¤º¸
		pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
		pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_COMMAND );
		pCPacket->m_cli_CLAN_COMMAND.m_btCMD	= GCMD_INFO;

		this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
	} else 
	if ( !strcmpi(pArg1, "invite") ) {	//±æµåÃÊ´ë, /ginvite <ÇÃ·¹ÀÌ¾î> - ±æµå¿¡ ÇØ´ç ÇÃ·¹ÀÌ¾î ÃÊ´ëÇÏ±â
		pArg2 = pStrVAR->GetTokenNext (pDelimiters);
		if ( pArg2 ) {
			pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
			pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_COMMAND );
			pCPacket->m_cli_CLAN_COMMAND.m_btCMD	= GCMD_INVITE;

			pCPacket->AppendString( pArg2 );
			this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
		}
	} else
	if ( !strcmpi(pArg1, "remove") ) {	//±æµåÃß¹æ, /gremove <ÇÃ·¹ÀÌ¾î> - ±æµå¿¡¼­ ÇØ´ç ÇÃ·¹ÀÌ¾î Ãß¹æÇÏ±â
		pArg2 = pStrVAR->GetTokenNext (pDelimiters);
		if ( pArg2 ) {
			pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
			pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_COMMAND );
			pCPacket->m_cli_CLAN_COMMAND.m_btCMD	= GCMD_REMOVE;

			pCPacket->AppendString( pArg2 );
			this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
		}
	} else
	if ( !strcmpi(pArg1, "promote") ) {	//±æµå½Â±Þ, /gpromote <ÇÃ·¹ÀÌ¾î> - ÇØ´ç ÇÃ·¹ÀÌ¾î ±æµå µî±Þ ¿Ã¸®±â
		pArg2 = pStrVAR->GetTokenNext (pDelimiters);
		if ( pArg2 ) {
			pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
			pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_COMMAND );
			pCPacket->m_cli_CLAN_COMMAND.m_btCMD	= GCMD_PROMOTE;

			pCPacket->AppendString( pArg2 );
			this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
		}
	} else 
	if ( !strcmpi(pArg1, "demote") ) {	//±æµå°­µî, /gdemote <ÇÃ·¹ÀÌ¾î> - ÇØ´ç ÇÃ·¹ÀÌ¾î ±æµå µî±Þ ³»¸®±â
		pArg2 = pStrVAR->GetTokenNext (pDelimiters);
		if ( pArg2 ) {
			pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
			pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_COMMAND );
			pCPacket->m_cli_CLAN_COMMAND.m_btCMD	= GCMD_DEMOTE;

			pCPacket->AppendString( pArg2 );
			this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
		}
	} else 
	if ( !strcmpi(pArg1, "slogan") ) {
		pArg2 = pStrVAR->GetTokenNext (pDelimiters);
		if ( pArg2 ) {
			pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
			pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_COMMAND );
			pCPacket->m_cli_CLAN_COMMAND.m_btCMD = GCMD_SLOGAN;

			pCPacket->AppendString( pArg2 );
			this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
		}
	} else 
	if ( !strcmpi(pArg1, "motd") ) {	//±æµå°øÁö, /gmotd <ÇÒ¸»> - ¿À´ÃÀÇ ±æµå ¸Þ½ÃÁö Á¤ÇÏ±â
		pArg2 = pStrVAR->GetTokenNext (pDelimiters);
		if ( pArg2 ) {
			pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
			pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_COMMAND );
			pCPacket->m_cli_CLAN_COMMAND.m_btCMD	= GCMD_MOTD;

			pCPacket->AppendString( pArg2 );
			this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
		}
	} else 
	if ( !strcmpi(pArg1, "quit") ) {	//±æµåÅ»Åð, /gquit - ±æµå¿¡¼­ Å»ÅðÇÏ±â
		pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
		pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_COMMAND );
		pCPacket->m_cli_CLAN_COMMAND.m_btCMD	= GCMD_QUIT;
		this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
	} else 
	if ( !strcmpi(pArg1, "roster") ) {	//±æµå¸ñ·Ï, /groster - ÀüÃ¼ ±æµå¿ø ¸ñ·Ï º¸±â
		pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
		pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_COMMAND );
		pCPacket->m_cli_CLAN_COMMAND.m_btCMD	= GCMD_ROSTER;

		this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
	} else 
	if ( !strcmpi(pArg1, "leader") ) {	//±æµåÀ§ÀÓ, /gleader <ÇÃ·¹ÀÌ¾î> - ´Ù¸¥ ÇÃ·¹ÀÌ¾î¿¡°Ô ±æµåÀå À§ÀÓÇÏ±â (±æµåÀå Àü¿ë)
		pArg2 = pStrVAR->GetTokenNext (pDelimiters);
		if ( pArg2 ) {
			pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
			pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_COMMAND );
			pCPacket->m_cli_CLAN_COMMAND.m_btCMD	= GCMD_LEADER;

			pCPacket->AppendString( pArg2 );
			this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
		}
	} else 
	if ( !strcmpi(pArg1, "disband") ) {	//±æµåÇØÃ¼, /gdisband - ±æµå ÇØÃ¼ÇÏ±â (±æµåÀå Àü¿ë) 
		pCPacket->m_HEADER.m_wType = CLI_CLAN_COMMAND;
		pCPacket->m_HEADER.m_nSize = sizeof( cli_CLAN_COMMAND );
		pCPacket->m_cli_CLAN_COMMAND.m_btCMD	= GCMD_DISBAND;
		this->Recv_cli_CLAN_COMMAND( (t_PACKET*)( pCPacket->m_pDATA ) );
	} else
	if ( !strcmpi(pArg1, "addskill") ) {
		pArg2 = pStrVAR->GetTokenNext (pDelimiters);
		if ( pArg2 )
			this->AddClanSKILL( atoi(pArg2) );
	} else
	if ( !strcmpi(pArg1, "delskill") ) {
		pArg2 = pStrVAR->GetTokenNext (pDelimiters);
		if ( pArg2 )
			this->DelClanSKILL( atoi(pArg2) );
	}


	if ( this->B_Cheater() ) {
// #ifdef	__INC_WORLD
		if ( !strcmpi(pArg1, "score") ) {
			pArg2 = pStrVAR->GetTokenNext (pDelimiters);
			if ( pArg2 )
				this->AddClanSCORE( atoi(pArg2) );
		} else
		if ( !strcmpi(pArg1, "money") ) {
			pArg2 = pStrVAR->GetTokenNext (pDelimiters);
			if ( pArg2 )
				this->AddClanMONEY( atoi(pArg2) );
		} else
		if ( !strcmpi(pArg1, "level") ) {
			this->IncClanLEVEL ();
		}
// #endif
	}

	Packet_ReleaseNUnlock( pCPacket );
	return 0;
}


//-------------------------------------------------------------------------------------------------
/// ÀÎº¥Åä¸®°¡ ²ËÂ÷¼­ ´õÀÌ»ó ÀÌÀÌÅÛÀ» ¼ÒÀ¯ÇÒ¼ö ¾ø´Â »óÅÂ¿¡¼­ ºÎµæÀÌ Äù½ºÆ® º¸»óµî¿¡¼­ ¾ÆÀÌÅÛÀº ¹Ù´Ú¿¡
/// ¶³±¸°í ¼ÒÀ¯½Ã°£À» Çã¹ú³ª°Ô ±æ°Ô...
void classUSER::Save_ItemToFILED (tagITEM &sDropITEM, int iRemainTime)
{
	switch( sDropITEM.GetTYPE() ) {
		case ITEM_TYPE_QUEST :
		case ITEM_TYPE_MONEY :
			// µ·À» ´õÀÌ»ó ¼ÒÁöÇÏÁö ¸øÇØ µå·ÓµÇ´Â °æ¿ì´Â ¾øÀ½.
			return;
	}

	CObjITEM *pObjITEM = new CObjITEM;
	if ( pObjITEM ) {
		tPOINTF PosSET;
		PosSET.x = this->m_PosCUR.x + RANDOM( 201 ) - 100;
		PosSET.y = this->m_PosCUR.y + RANDOM( 201 ) - 100;

		pObjITEM->InitItemOBJ( this, PosSET, this->m_PosSECTOR, sDropITEM, this, true, NULL );		// »ç¿ëÀÚ µå·Ó.
		pObjITEM->m_iRemainTIME = iRemainTime;	// 30ºÐ°£ À¯È¿ ÇÏµµ·Ï...

		#ifdef	__NEW_LOG
			g_pThreadLOG->When_ObjItemLOG( LIA_DROP, this, pObjITEM );
		#else
			g_pThreadLOG->When_DropITEM( this, pObjITEM );
		#endif
		this->GetZONE()->Add_DIRECT( pObjITEM );		// µå·Ó ¾ÆÀÌÅÛ
	}
}

//-------------------------------------------------------------------------------------------------
/// °æÇèÄ¡ Áõ°¡½ÃÅ´
void classUSER::Add_EXP (__int64 iGetExp, bool bApplyStamina, WORD wFromObjIDX)
{	
	if ( this->Get_HP() <= 0 )
		return;

	__int64 iExp;
	if ( bApplyStamina ) {
		iExp = iGetExp;

		iGetExp= (__int64)( ( (iExp + 100) / (this->Get_TRUE_LEVEL()+6) ) * ( ::Get_WorldSTAMINA() ) / 80.f );
		short nNewStamina = this->GetCur_STAMINA() + iGetExp;
		this->SetCur_STAMINA( nNewStamina>=MAX_STAMINA? MAX_STAMINA : nNewStamina );

		//if ( this->GetCur_STAMINA() >= GREEN_STAMINA ) {
		//	iExp = iGetExp;

		//	// ¼Ò¸ðµÉ ½ºÅ×¹Ì³ª :: {(È¹µæ °æÇèÄ¡ + 80) / (A_LV+5) } * (WORLD_STAMINA) / 100
		//	iGetExp= (int)( ( (iExp + 80) / (this->Get_LEVEL()+5) ) * ( ::Get_WorldSTAMINA() ) / 100.f );
		//	if ( iGetExp > 0 ) {
		//		short nNewStamina = this->GetCur_STAMINA() - iGetExp;
		//		this->SetCur_STAMINA( nNewStamina>0 ? nNewStamina : 0 );
		//	}
		//} else
		//if ( this->GetCur_STAMINA() >= YELLOW_STAMINA ) {
		//	iExp = (int) ( iGetExp * this->GetCur_STAMINA()/10000.f );
		//	if ( iExp <= 0 ) return;

		//	iGetExp= (int)( ( (iExp + 80) / (this->Get_LEVEL()+5) ) * ( ::Get_WorldSTAMINA() ) / 100.f );
		//	// ¼Ò¸ðµÉ ½ºÅ×¹Ì³ª :: {(È¹µæ °æÇèÄ¡ + 80) / (A_LV+5) } * (WORLD_STAMINA) / 100
		//	iGetExp= (int)( ( (iExp + 80) / (this->Get_LEVEL()+5) ) * ( ::Get_WorldSTAMINA() ) / 100.f );
		//	if ( iGetExp > 0 ) {
		//		short nNewStamina = this->GetCur_STAMINA() - iGetExp;
		//		if ( nNewStamina < YELLOW_STAMINA && this->GetPARTY() ) {
		//			// ÆÄÆ¼¿ø¿¡°Ô ½ºÅ×¹Ì³Ê Á¤º¸ Àü¼Û.
		//			this->m_pPartyBUFF->Change_ObjectIDX( this );
		//		}
		//		this->SetCur_STAMINA( nNewStamina>0 ? nNewStamina : 0 );
		//	}
		//} else {
		//	iExp = (int) ( iGetExp * 0.3f );
		//	if ( iExp <= 0 ) return;

		//	// ¼Ò¸ðµÉ ½ºÅ×¹Ì³ª :: {(È¹µæ °æÇèÄ¡ + 80) / (A_LV+5) } * (WORLD_STAMINA) / 100
		//	iGetExp= (int)( ( (iExp + 80) / (this->Get_LEVEL()+5) ) * ( ::Get_WorldSTAMINA() ) / 100.f );
		//	if ( iGetExp > 0 ) {
		//		short nNewStamina = this->GetCur_STAMINA() - iGetExp;
		//		this->SetCur_STAMINA( nNewStamina>0 ? nNewStamina : 0 );
		//	}
		//}
	} else {
		iExp = iGetExp;
	}

#ifdef FRAROSE
	iExp *= this->m_GrowAbility.GetEXPBoost( this->GetCurAbsSEC() );
	if( !this->m_GrowAbility.IsReceiveEXP( this->GetCurAbsSEC() ) )
		iExp *= 0;
#endif
	if ( m_GrowAbility.m_lEXP + iExp > MAX_EXPERIENCE ) {
		m_GrowAbility.m_lEXP = this->Get_NeedEXP( MAX_LEVEL );
	} else
		m_GrowAbility.m_lEXP += iExp;

	__int64  iNeedEXP = this->Get_NeedEXP( this->Get_TRUE_LEVEL() );
	bool bLevelUp=false;

	short nBeforeLEV = this->Get_TRUE_LEVEL();
	while ( m_GrowAbility.m_lEXP >= iNeedEXP ) {
		// ·¹º§ Á¦ÇÑ 
		if ( this->Get_TRUE_LEVEL() < MAX_LEVEL ) {
			this->Set_LEVEL( this->Get_TRUE_LEVEL() + 1 );
		}

		this->AddCur_BonusPOINT( (short)( this->Get_TRUE_LEVEL() * 0.8 ) + 10 );

		if ( IsTAIWAN() ) {
			for (short nD=0; nD<g_TblSkillPoint.m_nDataCnt ; nD++) {
				if ( SP_LEVEL(nD) == this->Get_TRUE_LEVEL() ) {
					this->AddCur_SkillPOINT( SP_POINT(nD) );
					break;
				}
			}
		} else {
			// ½ºÅ³ Æ÷ÀÎÆ® =  (LV + 4) * 0.5 - 1 
			this->AddCur_SkillPOINT( (short)( ( this->Get_TRUE_LEVEL() + 4 ) * 0.5f ) - 1 );
		}
		m_GrowAbility.m_lEXP -= iNeedEXP;

		m_GrowAbility.m_lPenalEXP = 0;
		iNeedEXP = this->Get_NeedEXP( this->Get_TRUE_LEVEL() );
		bLevelUp = true;
	}

	if ( bLevelUp ) {
		g_pThreadLOG->When_LevelUP( this, iExp );
		this->Send_gsv_LEVELUP( this->Get_TRUE_LEVEL()-nBeforeLEV );
		if ( g_pUserLIST->Get_LevelUpTRIGGER( this->Get_TRUE_LEVEL() ) )
			this->Do_QuestTRIGGER( g_pUserLIST->Get_LevelUpTRIGGER( this->Get_TRUE_LEVEL() ) );
	} else
		this->Send_gsv_SETEXP( wFromObjIDX );
}

/**
 * \brief	Á×¾ú´Ù...
 * \param	pKiller	:: Á×ÀÎ³Ñ...
 */
bool classUSER::Dead (CObjCHAR *pKiller)
{
	if ( this->GetZONE()->GetGlobalFLAG() & ZONE_FLAG_PK_ALLOWED ) {
		// ÇöÀç Á¸ÀÌ PK ¼³Á¤µÇ¾î ÀÖÀ¸¸é...
		CObjCHAR *pKillerOBJ = (CObjCHAR*)( pKiller->Get_CALLER() );

		if ( pKillerOBJ && pKillerOBJ->IsUSER() ) {
			if ( this->GetZONE()->Get_HashKillTRIGGER() )
				((classUSER*)pKillerOBJ)->Do_QuestTRIGGER( this->GetZONE()->Get_HashKillTRIGGER() );

			if ( this->GetZONE()->Get_HashDeadTRIGGER() )
				this->Do_QuestTRIGGER( this->GetZONE()->Get_HashDeadTRIGGER() );
		}
	}

	// »ç¿ëÀÚ°¡ Á×À»¶§ ³²±â´Â ·Î±×...
	g_pThreadLOG->When_DieBY( pKiller, (classUSER*)this );

	if ( this->m_iLinkedCartObjIDX ) {
		classUSER *pUSER = g_pObjMGR->Get_UserOBJ( this->m_iLinkedCartObjIDX );
		if ( pUSER ) {
			if ( RIDE_MODE_GUEST == pUSER->m_btRideMODE ) {
				// µÚ¿¡ Å¸°í ÀÖ´ø »ç¿ëÀÚ¸é ¿îÀüÀÚ°¡ Á×¾úÀ¸´Ï ³»·Á¾ßÁö...
				pUSER->m_btRideMODE = 0;
				pUSER->m_btRideATTR = RIDE_ATTR_NORMAL;
			}
			pUSER->m_iLinkedCartObjIDX = 0;
		}
	}

	this->m_iLinkedCartObjIDX = 0;
	this->m_btRideATTR = RIDE_ATTR_NORMAL;
	this->m_btRideMODE = 0;
	this->m_STORE.m_bActive = false;

	this->m_btTradeBIT = 0;
	this->m_iTradeUserIDX = 0;

	this->Del_ActiveSKILL();
	this->m_IngSTATUS.Reset( false );	// 2005. 03. 30 »óÁ¡»óÅÂ »ç¸Á½Ã »óÁ¡ÇÃ·¡±× ¾ÈÇ®¸®´ø ¹ö±× ¼öÁ¤ :: this->m_IngSTATUS.ClearALL();

	this->Clear_SummonCNT ();			// Á×À»¶§...ÀÚ½ÅÀÌ ¼ÒÈ¯½ÃÅ² °¹¼ö 0°³·Î...

	this->Update_SPEED ();				// »óÅÂ¶§¹®¿¡ Àû¿ëµÆ´ø ÀÌµ¿¼Óµµ 

	this->m_iAppliedPenaltyEXP = 0;
	if ( CObjCHAR::Dead(NULL) ) {
		// ¼ÒÈ¯µÈ ¸÷ÀÌ ¾Æ´Ï¸é...
		if ( !pKiller->IsUSER() && !pKiller->GetSummonedSkillIDX() /* 0 == ZONE_PVP_STATE( this->m_nZoneNO ) */ ) {
			// °æÇèÄ¡ ÆÐ³ÎÄ¡ Àû¿ë.
			this->Set_PenalEXP( PENALTY_EXP_TOWN );
		}
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------------------------------
/// ¼¿ÇÁ ½ºÅ³ ½ÇÇà
bool classUSER::Do_SelfSKILL (short nSkillIDX)
{
	if ( !IsTAIWAN() ) {
		if ( this->GetCur_RIDE_MODE() ) {
			short nSkillType = SKILL_TYPE( nSkillIDX );
			if ( nSkillType >= SKILL_TYPE_03 && nSkillType <= SKILL_TYPE_07 ) 
				return false;

			if ( nSkillType == SKILL_TYPE_17 )
				return false;
		}
	}

	if ( this->Skill_ActionCondition( nSkillIDX ) ) {
		// ½ÇÁ¦ ÇÊ¿ä ¼öÄ¡ ¼Ò¸ð Àû¿ë...
		if ( this->SetCMD_Skill2SELF( nSkillIDX ) ) {
			// this->Skill_UseAbilityValue( nSkillIDX );
			return true;
		}
	}

	return false;	// Á¢¼Ó ²÷±âÁö ¾ÊÀ½
}
/// Å¸°Ù ½ºÅ³ ½ÇÇà
bool classUSER::Do_TargetSKILL (int iTargetObject, short nSkillIDX)
{
	if ( !IsTAIWAN() ) {
		if ( this->GetCur_RIDE_MODE() ) {
			short nSkillType = SKILL_TYPE( nSkillIDX );
			if ( nSkillType >= SKILL_TYPE_03 && nSkillType <= SKILL_TYPE_07 ) 
				return false;

			if ( nSkillType == SKILL_TYPE_17 )
				return false;
		}
	}

	// ¸®Á®·º¼Ç ½ºÅ³·Î ÀÎÇØ HPÃ¼Å© ¾ÈÇÏ°Ô...
	CObjCHAR *pDestCHAR = g_pObjMGR->Get_ClientCharOBJ( iTargetObject, false /* true */ );
	if ( pDestCHAR ) {
		if ( !this->Skill_IsPassFilter( pDestCHAR, nSkillIDX ) )
			return false;

		if ( this->Skill_ActionCondition( nSkillIDX ) ) {
			if ( this->SetCMD_Skill2OBJ ( iTargetObject, nSkillIDX ) ) {
				// ½ºÅ³ ¼º°øÇÏ¸é...½ÇÁ¦ ÇÊ¿ä ¼öÄ¡ ¼Ò¸ð Àû¿ë...
				// this->Skill_UseAbilityValue( nSkillIDX );
				return true;
			}
		}
	}

	return false;	// Á¢¼Ó ²÷±âÁö ¾ÊÀ½
}

//-------------------------------------------------------------------------------------------------
/// »ç¿ëÀÚ°¡ ·Î±×¾Æ¿ô ¿äÃ»½Ã¿¡ Å¬¶óÀÌ¾ðÆ®¿¡ ´ë±â ÇØ¾ßÇÒ ½Ã°£ Åëº¸
bool classUSER::Send_gsv_LOGOUT_REPLY( WORD wWaitSec )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_LOGOUT_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_LOGOUT_REPLY );
	pCPacket->m_gsv_LOGOUT_REPLY.m_wWaitSec = wWaitSec;

	this->Send_Start( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
	return true;
}

//-------------------------------------------------------------------------------------------------
/// »ç¿ëÀÚ¿¡°Ô npc¿¡¼­ ¹ßµ¿ÇÒ¼ö ÀÖ´Â ÀÌº¥Æ®¸¦ ¼öÇàÇÏ¶ó°í Åëº¸
bool classUSER::Send_gsv_CHECK_NPC_EVENT( short nNpcIDX )
{
	if ( NPC_QUEST_TYPE(nNpcIDX) ) {
		// ÆÄÆ¼ Àü¿ë Äù½ºÆ®´Ù...
		if ( this->GetPARTY() ) {
			return this->m_pPartyBUFF->Send_gsv_CHECK_NPC_EVENT( this, nNpcIDX );
		}
		return true;
	}
	
	m_iLastEventNpcIDX = nNpcIDX;
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_CHECK_NPC_EVENT;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_CHECK_NPC_EVENT );

	pCPacket->m_gsv_CHECK_NPC_EVENT.m_nNpcIDX = nNpcIDX;
	this->Send_Start( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// »ç¿ëÀÚ¿¡°Ô ¼­¹öÀÇ ÇöÀç ÇÇ/¿¥À» Åëº¸
bool classUSER::Send_gsv_SET_HPnMP (BYTE btApply)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_SET_HPnMP;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_HPnMP );

	pCPacket->m_gsv_SET_HPnMP.m_wObjectIDX = this->Get_INDEX();
	
	pCPacket->m_gsv_SET_HPnMP.m_nHP = ( btApply & 0x01 ) ? this->Get_HP() : -1;
	pCPacket->m_gsv_SET_HPnMP.m_nMP = ( btApply & 0x02 ) ? this->Get_MP() : -1;

	this->SendPacketToPARTY( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// Ã¤ÆÃ·ë ÆÐÅ¶~
bool classUSER::Send_wsv_CHATROOM (BYTE btCMD, WORD wUserID, char *szSTR)
{
#ifdef ENABLE_CHATROOM
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = WSV_CHATROOM;
	pCPacket->m_tag_CHAT_HEADER.m_btCMD = btCMD;

	if ( wUserID ) {
		pCPacket->m_HEADER.m_nSize = sizeof( wsv_CHAT_ROOM_USER );
		pCPacket->m_wsv_CHAT_ROOM_USER.m_wUserID  = wUserID;
	} else
		pCPacket->m_HEADER.m_nSize = sizeof( tag_CHAT_HEADER );

	if ( szSTR )
		pCPacket->AppendString( szSTR );

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
#endif
	return true;
}

/// ÀÚ½Å¿¡°Ô ¼Ò¸ð ¾ÆÀÌÅÛÀ» »ç¿ëµÆ°í ¼Ò¸ðµÈ ¾ÆÀÌÅÛÀÇ °¹¼ö¸¦ Åëº¸
bool classUSER::Send_gsv_USE_ITEM( short nItemNO, short nInvIDX )
{
	// ÀÚ½Å ÇÑÅ×¸¸ ¼Ò¸ð¼º ¾ÆÀÌÅÛ »ç¿ëÇß´Ù°í Àü¼Û.
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_USE_ITEM;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_USE_ITEM ) + sizeof( BYTE );
	pCPacket->m_gsv_USE_ITEM.m_wObjectIDX = this->Get_INDEX();
	pCPacket->m_gsv_USE_ITEM.m_nUseItemNO = nItemNO;
	pCPacket->m_gsv_USE_ITEM.m_btInvIDX[ 0 ] = (BYTE)nInvIDX;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
	return true;
}

/// ¼Ò¸ð ¾ÆÀÌÅÛ »ç¿ëÀ» ÁÖº¯¿¡ ¾Ë·ÁÁÖ°í ¾ÆÀÌÅÛ Ã³¸®
bool classUSER::Use_pITEM( tagITEM *pITEM )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_USE_ITEM;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_USE_ITEM );
	pCPacket->m_gsv_USE_ITEM.m_wObjectIDX = this->Get_INDEX();
	pCPacket->m_gsv_USE_ITEM.m_nUseItemNO = pITEM->m_nItemNo;

	this->GetZONE()->SendPacketToSectors( this, pCPacket );
	// ÆÄÆ¼¿ø ÇÑÅ×µµ ???
	// this->SendPacketToPartyExecpNearUSER( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	// Áö¼Ó¼º È¿°ú ¾ÆÀÌÅÛÀÎ°¡ ??
	if ( USEITME_STATUS_STB( pITEM->m_nItemNo ) ) {
		short nIngSTB = USEITME_STATUS_STB( pITEM->m_nItemNo );
		// short nDuringTime;
		for (short nI=0; nI<STATE_APPLY_ABILITY_CNT; nI++) {
			if ( !STATE_APPLY_ABILITY_VALUE( nIngSTB, nI ) )
				continue;
			
			if ( this->m_IngSTATUS.IsEnableApplayITEM( STATE_APPLY_ING_STB( nIngSTB, nI ) ) ) {
				this->m_IngSTATUS.UpdateIngPOTION(	STATE_APPLY_ING_STB( nIngSTB, nI ), 
													USEITEM_ADD_DATA_VALUE( pITEM->m_nItemNo ),
													STATE_APPLY_ABILITY_VALUE( nIngSTB, nI ) );
			}
		}
	} else {
		// this->Use_ITEM( pITEM->m_nItemNo );
		this->Add_AbilityValue( USEITEM_ADD_DATA_TYPE( pITEM->m_nItemNo ), USEITEM_ADD_DATA_VALUE( pITEM->m_nItemNo ) );
	}

	return true;
}

/// ¼Ò¸ð¾ÆÀÌÅÛ »ç¿ë ÆÐÅ¶ Ã³¸® :: »ç¿ëÁ¶°Çµî µûÁü
bool classUSER::Use_InventoryITEM( t_PACKET *pPacket )
{
	short nInventoryIDX = pPacket->m_cli_USE_ITEM.m_nInventoryIndex;

	tagITEM *pITEM = &this->m_Inventory.m_ItemLIST[ nInventoryIDX ];
	if ( pITEM->GetQuantity() < 1 ) {
		return true;
	}

	if ( pITEM->GetTYPE() != ITEM_TYPE_USE ) {
		return IS_HACKING( this, "Use_InventoryITEM-1 :: pITEM->GetTYPE() != ITEM_TYPE_USE" );
	}

	if ( pITEM->m_nItemNo >= g_TblUSEITEM.m_nDataCnt ) {
		return IS_HACKING( this, "Recv_cli_USE_ITEM-2 : pITEM->m_nItemNo >= g_TblUSEITEM.m_nDataCnt" );
	}

	/// ÄðÅ¸ÀÓ Àû¿ë..
	DWORD dwCurTime;
	short nCoolTimeType;
	if ( IsTAIWAN() ) {
		dwCurTime = this->GetCurAbsSEC();
		nCoolTimeType = USEITEM_COOLTIME_TYPE(pITEM->m_nItemNo);
		if ( nCoolTimeType ) {
			if ( dwCurTime - this->m_dwCoolTIME[ nCoolTimeType ] <= static_cast<DWORD>( USEITEM_COOLTIME_DELAY(pITEM->m_nItemNo) ) ) {
			#ifdef	__INC_WORLD
				char szTmp[ MAX_PATH ];
				sprintf( szTmp, "ignore use item:: CurTime:%d, LastTime:%d, Reamin:%dsec", 
								dwCurTime, 	this->m_dwCoolTIME[ nCoolTimeType ], dwCurTime-this->m_dwCoolTIME[ nCoolTimeType ] );
				this->Send_gsv_WHISPER( "Cool time", szTmp );			// xxxx
			#endif
				return true;
			}
		}
	} else {
		dwCurTime = 0;
		nCoolTimeType = 0;
	}

	/// »ç¿ë Á¶°Ç Ã¼Å©...
	int iValue = GetCur_AbilityValue( USEITEM_NEED_DATA_TYPE(pITEM->m_nItemNo) );
	if ( AT_CURRENT_PLANET == USEITEM_NEED_DATA_TYPE(pITEM->m_nItemNo) ) {
		// »ç¿ëÇÒ¼ö ÀÖ´Â Çà¼ºÀ» Ã¼Å©ÇÏ´Â °ÍÀÎ°¡ ????
		if ( iValue != USEITEM_NEED_DATA_VALUE(pITEM->m_nItemNo) )
			return true;
	}
	if ( iValue < USEITEM_NEED_DATA_VALUE(pITEM->m_nItemNo) ) {
		return true;
	}

//	DWORD dwClearedSTATUS = 0;
	if ( USE_ITEM_SKILL_DOING == ITEM_TYPE( ITEM_TYPE_USE, pITEM->m_nItemNo ) ) {
		if ( !(this->m_dwPayFLAG & PLAY_FLAG_BATTLE) ) {
			/// °ø°Ý ¸øÇØ... µ·³»¾ßÇÔ
			return true;
		}

		/// ½ºÅ³ »ç¿ëÇÏ´Â ¾ÆÀÌÅÛ...
		short nSkillIDX = USEITEM_SCROLL_USE_SKILL( pITEM->m_nItemNo );

		if ( this->Is_SelfSKILL( nSkillIDX ) ) {
			if ( !this->Do_SelfSKILL( nSkillIDX ) )		// ´É·ÂÄ¡ ºÎÁ·...
				return true;
		} else
		if ( this->Is_TargetSKILL( nSkillIDX ) ) {
			if ( pPacket->m_HEADER.m_nSize != (sizeof(WORD)+sizeof(cli_USE_ITEM)) )
				return true;

			WORD wDestObjIDX = *( (WORD*)( pPacket->m_pDATA+ sizeof(cli_USE_ITEM) ) );
			if ( !this->Do_TargetSKILL( wDestObjIDX, nSkillIDX ) )
				return true;
		} else {
			switch( SKILL_TYPE( nSkillIDX ) ) {
				case SKILL_TYPE_18 :	// ¿öÇÁ !!!
				{
					if ( SKILL_WARP_PLANET_NO( nSkillIDX ) != ZONE_PLANET_NO( this->GetZONE()->Get_ZoneNO() ) ) {
						/// ¿öÇÁ ¾ÆÀÌÅÛÀº °°Àº Çà¼ºÀ¸·Î¸¸ »ç¿ë°¡´ÉÇÏ´Ù..
						return true;
					}

					// MP¾ç = (ÇöÀç¼ÒÁö·®) * 0.3
					int iNeedMP = (int)( this->Get_WEIGHT() * 0.05f );
					if ( this->Get_MP() < iNeedMP )
						return true;
					this->Sub_MP( iNeedMP );

					tPOINTF PosGOTO;
					PosGOTO.x = SKILL_WARP_ZONE_XPOS( nSkillIDX ) * 1000.f;
					PosGOTO.y = SKILL_WARP_ZONE_YPOS( nSkillIDX ) * 1000.f;
					this->Send_gsv_RELAY_REQ( RELAY_TYPE_RECALL, SKILL_WARP_ZONE_NO( nSkillIDX ), PosGOTO );
					break;
				}
				case SKILL_TYPE_16 :	// emotion
					// ¼Ò¸ðµÇµµ·Ï¸¸...
					break;
				default :
					return true;
			}
		}
	} else
	if ( USE_ITEM_SKILL_LEARN == ITEM_TYPE( ITEM_TYPE_USE, pITEM->m_nItemNo ) ) {
		/// ½ºÅ³ ÀÍÈ÷´Â ¾ÆÀÌÅÛ...
		switch( this->Send_gsv_SKILL_LEARN_REPLY( USEITEM_SCROLL_LEARN_SKILL( pITEM->m_nItemNo ) ) ) {
			case RESULT_SKILL_LEARN_SUCCESS :
				break;
			case RESULT_SKILL_LEARN_NEED_JOB   :
			case RESULT_SKILL_LEARN_NEED_SKILL :	// ÇÊ¿ä ½ºÅ³ÀÌ ÀÖ´Âµ¥ ¿äÃ»ÇÏ¸é Â©·¯~~~ ÀÖÀ»¼ö ¾ø´Â °æ¿ì...
				return false;
			default :	// ¹è¿ìÁö ¸øÇÑ ±âÅ¸ »çÀ¯¸é ¾ÆÀÌÅÛ ±×´ë·Î º¸À¯
				return true;
		}
	} else 
	if ( USE_ITEM_FUEL == ITEM_TYPE( ITEM_TYPE_USE, pITEM->m_nItemNo ) ) {
		// Ä«Æ®/Ä³½½±â¾î ¿¬·á ¾ÆÀÌÅÛ...
		tagITEM *pEngine = &m_Inventory.m_ItemRIDE[ RIDE_PART_ENGINE ];
		if ( ITEM_TYPE_RIDE_PART != pEngine->GetTYPE() ) {
			return true;
		}
		int iAddValue = USEITEM_ADD_DATA_VALUE( pITEM->m_nItemNo ) * 10;
		if ( pEngine->m_nLife + iAddValue > MAX_ITEM_LIFE )
			pEngine->m_nLife = MAX_ITEM_LIFE;
		else
			pEngine->m_nLife += iAddValue;

		this->Send_gsv_SET_ITEM_LIFE( INVENTORY_RIDE_ITEM0 + RIDE_PART_ENGINE, pEngine->GetLife () );
	} else
	if ( USE_ITEM_MAINTAIN_ITEM == ITEM_TYPE( ITEM_TYPE_USE, pITEM->m_nItemNo ) ) {
		/// ½Ã°£ ÁöÁ¤ ÄíÆù ¾ÆÀÌÅÛ....
		switch( USEITEM_ADD_DATA_TYPE( pITEM->m_nItemNo ) ) {
			case AT_BANK_FREE	:
			case AT_BANK_ADDON	:
			case AT_STORE_SKIN	:
#ifdef	__INC_PLATINUM
				this->m_GrowAbility.UpdateSTATUS( this->GetCurAbsSEC(), 
						USEITEM_ADD_DATA_TYPE( pITEM->m_nItemNo ),		// x ´É·ÂÄ¡¸¦
						USEITEM_ADD_DATA_VALUE( pITEM->m_nItemNo ),		// y ÀÌÈÄ...
						USEITEM_STORE_SKIN( pITEM->m_nItemNo ) );		// z °ªÀ¸·Î ( °³ÀÎ»óÁ¡ ½ºÅ² )
#endif
				break;
		}
		//USEITEM_STORE_SKIN( pITEM->m_nItemNo );		// ¼³Á¤ÇÒ ...
		//USEITEM_ADD_DATA_VALUE( pITEM->m_nItemNo );	// Áö¼ÓµÉ ½Ã°£...
	} else {
		/// ÁÖº¯¿¡ »ç¿ë °¡´ÉÇÑ ¾ÆÀÌÅÛÀÌ¸é... °¹¼ö °¨¼Ò...
		this->Use_pITEM( pITEM );
	}

	/// ¸¶Áö¸· »ç¿ëÇÑ ½Ã°£ ÀúÀå...
	this->m_dwCoolTIME[ nCoolTimeType ] = dwCurTime;

	// ¼ö·® °¨¼Ò
	if ( --pITEM->m_uiQuantity <= 0 ) {
		/// ´Ù ¼Ò¸ðÇß´Ù..
		m_Inventory.DeleteITEM( nInventoryIDX );

		this->Send_gsv_SET_INV_ONLY( (BYTE)nInventoryIDX, pITEM );
	} else {
		/// ÀÚ½Å ÇÑÅ×¸¸ ¼Ò¸ð¼º ¾ÆÀÌÅÛ »ç¿ëÇß´Ù°í Àü¼Û.
		this->Send_gsv_USE_ITEM( pITEM->m_nItemNo, nInventoryIDX );	/// ÀÚ½Å ÇÑÅ×¸¸ ¼Ò¸ð¼º ¾ÆÀÌÅÛ »ç¿ëÇß´Ù°í Àü¼Û.
	}

    return true;
}


//-------------------------------------------------------------------------------------------------
/// gsv_SET_MONEYnINV ÆÐÅ¶ ÇÒ´ç¹× ÃÊ±âÈ­
classPACKET *classUSER::Init_gsv_SET_MONEYnINV (WORD wType)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( pCPacket ) {
		pCPacket->m_HEADER.m_wType = wType;
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_MONEYnINV );
		pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT = 0;
	}

	return pCPacket;
}
/// gsv_SET_MONEYnINV ÆÐÅ¶ Àü¼Û
bool classUSER::Send_gsv_SET_MONEYnINV( classPACKET *pCPacket )
{
	if ( pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ) {
		// º¯°æµÈ ÀÎº¥Åä¸®°¡ ÀÖÀ»°æ¿ì¸¸ Àü¼ÛÇÑ´Ù.
		pCPacket->m_HEADER.m_nSize += ( pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT * sizeof( tag_SET_INVITEM ) );
		pCPacket->m_gsv_SET_MONEYnINV.m_i64Money  = this->GetCur_MONEY();

		this->SendPacket( pCPacket );
	}

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}


//-------------------------------------------------------------------------------------------------
/// Ã¤ÆÃ±ÝÁö, °­Á¦ Á¢¼Ó Á¾·áµî... GMÀÇ ¸í·É Àü¼Û
bool classUSER::Send_gsv_GM_COMMAND( char *szAccount, BYTE btCMD, WORD wBlockTIME)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_GM_COMMAND;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_GM_COMMAND );
	pCPacket->m_gsv_GM_COMMAND.m_btCMD = btCMD;
	pCPacket->m_gsv_GM_COMMAND.m_wBlockTIME = wBlockTIME;

	pCPacket->AppendString ( szAccount );

	this->SendPacket( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
	return true;
}

//-------------------------------------------------------------------------------------------------
/// ¼­¹öÀÇ ÇöÀç µ·¾çÀ» Åëº¸ :: Äù½ºÆ® º¸»óµî µ·ÀÇ ¾çÀÌ Æ²·Á Á³À»°æ¿ì È£ÃâµÊ
bool classUSER::Send_gsv_SET_MONEYONLY (WORD wType)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = wType;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_MONEY_ONLY );
	pCPacket->m_gsv_SET_MONEY_ONLY.m_i64Money  = this->GetCur_MONEY();

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ¿ùµå¼­¹ö¿¡¼­ Ã¤³Î¼­¹ö·Î ÀÌµ¿ÈÄ ¼­¹ö¿¡ µé¾î¿Ã¼ö ÀÖ´ÂÁö °ËÁõÆÐÅ¶ Åëº¸
bool classUSER::Send_srv_JOIN_SERVER_REPLY (t_PACKET *pRecvPket, char *szAccount)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = SRV_JOIN_SERVER_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( srv_JOIN_SERVER_REPLY );
	pCPacket->m_srv_JOIN_SERVER_REPLY.m_btResult = pRecvPket->m_wls_CONFIRM_ACCOUNT_REPLY.m_btResult;

	DWORD dwRecvSeqNO;
	if ( RESULT_CONFIRM_ACCOUNT_OK == pRecvPket->m_wls_CONFIRM_ACCOUNT_REPLY.m_btResult ) {
		dwRecvSeqNO = ::timeGetTime();	// CRandom::GetInstance()->Get();
		pCPacket->m_srv_JOIN_SERVER_REPLY.m_dwID = dwRecvSeqNO;
		pCPacket->m_srv_JOIN_SERVER_REPLY.m_dwPayFLAG = 0;

		// CUserLIST::Add_ACCOUNT ¾È¿¡¼­ ¼³Á¤µÇ´ø °èÁ¤À» ÀÌ°÷À¸·Î ¿Å±è
		// ¿©±â¼­ º¸³»´Â ÆÐÅ¶ÀÇ ÀÀ´ä ÆÐÅ¶ÀÌ CUserLIST::Add_ACCOUNT¾È¿¡¼­ °èÁ¤ÀÌ
		// ¼³Á¤µÇ±â Àü¿¡ Ã³¸®µÇ¸é »¶³²~~~~
		this->m_HashACCOUNT= CStrVAR::GetHASH( szAccount );
		this->Set_ACCOUNT( szAccount );
	} else
		dwRecvSeqNO = -1;

	this->SendPacket( pCPacket );
	this->m_iRecvSeqNO = dwRecvSeqNO;
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}


//-------------------------------------------------------------------------------------------------
// bool classUSER::Send_gsv_SELECT_CHAR (void)
/// Å¬¶óÀÌ¾ðÆ®¿¡ Á¸¿¡ ÀÔÀåÇã°¡ ÆÐÅ¶ Àü¼Û
bool classUSER::Send_gsv_JOIN_ZONE (CZoneTHREAD *pZONE)
{
	if ( IsTAIWAN() ) {
		if ( this->m_btRideMODE && ZONE_RIDING_REFUSE_FLAG( pZONE->Get_ZoneNO() ) ) {
			// 0x01 : Ä«Æ® ºÒ°¡, 0x02 : Ä³½½±â¾î ºÒ°¡
			if ( ITEM_TYPE_RIDE_PART == this->m_Inventory.m_ItemRIDE[ RIDE_PART_BODY ].GetTYPE() ) {
				int iType = ITEM_TYPE( ITEM_TYPE_RIDE_PART, this->m_Inventory.m_ItemRIDE[ RIDE_PART_BODY ].GetItemNO() );
				if ( (iType%3) & ZONE_RIDING_REFUSE_FLAG( this->GetZONE()->Get_ZoneNO() ) ) {
					// Å¾½Â Á¦ÇÑ~
					this->m_btRideMODE = 0;
				}
			} else {
				this->m_btRideMODE = 0;		// ¹¹³Ä ÀÌ°Ç ??
			}

			// °­Á¦ ÇØÁ¦ µÆ³ª ?
			/*if ( 0 == this->m_btRideMODE ) {
				this->UpdateAbility ();		// Å¾½Â Åä±Û...
			}*/
			if ( 0 == this->m_btRideMODE ) 
			{			
				//	2006.03.16/±è´ë¼º - Ä«Æ®³ª Ä³½½±â¾î¸¦ Å»¼ö ¾ø´Â Áö¿ªÀÌ ÀÖ´Ù. (°íºí¸°µ¿±¼)
				//	- Ä«Æ®¸¦ Å¸°í Ä«Æ®¸¦ Å»¼ö ¾ø´Â Áö¿ª¿¡ µé¾î°¡¸é ½ºÅ³½ÃÀüÀÌ ¾ÈµÇ´Â ¹ö±×
				if ( ZONE_RIDING_REFUSE_FLAG( pZONE->Get_ZoneNO() ) > 0 )	// 1:Ä«Æ® ºÒ°¡, 2:Ä³½½±â¾î ºÒ°¡, 3:¸ðµÎ ºÒ°¡
				{
					this->m_btRideMODE = 0;
					this->m_btRideATTR = RIDE_ATTR_NORMAL;		// ¿©±â°¡ Áß¿ä
					this->m_iLinkedCartObjIDX = 0;
					//	±è¿µÈ¯ 2006.8.29ÀÏ Ã¤Å© À§Ä¡ º¸Á¤
					// 2006.05.30/±è´ë¼º/Ãß°¡
				}

				this->UpdateAbility ();		// Å¾½Â Åä±Û...
				this->Send_gsv_SPEED_CHANGED ();	// ½ºÇÇµå º¯°æµÈ °ªÀ» Àü¼Û : Á¸ ¸Þ½ÃÁö·Î Àü¼Û.
				//-------------------------------------
			}
		}
	}

	m_bSetImmediateRevivePOS = false;

	this->m_iTeamNO = TEAMNO_USER;
	if ( this->GetZONE()->Get_HashJoinTRIGGER() ) {
		// Á¸¿¡ Á¶ÀÎ½Ã ¹ß»ýÇÒ Æ®¸®°Å...
		if ( 0 == this->m_IngSTATUS.IsSubSET( FLAG_CHEAT_INVINCIBLE ) ) {
			this->Do_QuestTRIGGER( this->GetZONE()->Get_HashJoinTRIGGER() );
		}
	}

    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_JOIN_ZONE;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_JOIN_ZONE );

	pCPacket->m_gsv_JOIN_ZONE.m_wServerObjectIndex = this->Get_INDEX ();
	pCPacket->m_gsv_JOIN_ZONE.m_nCurHP = this->Get_HP ();
	pCPacket->m_gsv_JOIN_ZONE.m_nCurMP = this->Get_MP ();
	pCPacket->m_gsv_JOIN_ZONE.m_lCurEXP   = this->m_GrowAbility.m_lEXP;
	pCPacket->m_gsv_JOIN_ZONE.m_lPenalEXP = this->m_GrowAbility.m_lPenalEXP;

	pCPacket->m_gsv_JOIN_ZONE.m_VAR.m_nWorld_PRODUCT = ::Get_WorldPROD();
//	pCPacket->m_gsv_JOIN_ZONE.m_VAR.m_nWorld_STAMINA = ::Get_WorldSTAMINA();
	pCPacket->m_gsv_JOIN_ZONE.m_VAR.m_dwUpdateTIME = pZONE->GetEconomyTIME();
	pCPacket->m_gsv_JOIN_ZONE.m_VAR.m_nWorld_RATE  = ::Get_WorldRATE();
	pCPacket->m_gsv_JOIN_ZONE.m_VAR.m_btTOWN_RATE  = pZONE->m_Economy.Get_TownRATE();
	pCPacket->m_gsv_JOIN_ZONE.m_VAR.m_dwGlobalFLAGS= pZONE->GetGlobalFLAG();

	::CopyMemory( pCPacket->m_gsv_JOIN_ZONE.m_VAR.m_btItemRATE, pZONE->m_Economy.m_btItemRATE, sizeof(BYTE)*MAX_PRICE_TYPE );

	pCPacket->m_gsv_JOIN_ZONE.m_dwAccWorldTIME = ::Get_WorldPassTIME();
#ifdef	__APPLY_04_10_15_TEAMNO
	pCPacket->m_gsv_JOIN_ZONE.m_iTeamNO		   = this->Get_TeamNO();
#endif

    this->SendPacket( pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	// ÀÚ½Å¿¡°Ô ¹«°èºñÀ² ¼³Á¤.
	return this->Recv_cli_SET_WEIGHT_RATE( this->m_btWeightRate );
}

//-------------------------------------------------------------------------------------------------
/// ÀÎº¥Åä¸®¹× Äù½ºÆ® µ¥ÀÌÅ¸ Àü¼Û
bool classUSER::Send_gsv_INVENTORYnQUEST_DATA (void)
{
	COMPILE_TIME_ASSERT( (sizeof(__int64) + sizeof(tagBaseITEM)*INVENTORY_TOTAL_SIZE ) < MAX_PACKET_SIZE );
	COMPILE_TIME_ASSERT( sizeof( gsv_QUEST_ONLY ) < MAX_PACKET_SIZE );
//	COMPILE_TIME_ASSERT( sizeof( gsv_QUEST_DATA ) < MAX_PACKET_SIZE );

    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	int iC;

    pCPacket->m_HEADER.m_wType = GSV_INVENTORY_DATA;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_INVENTORY_DATA );

	// pCPacket->m_gsv_INVENTORY_DATA.m_INV = this->m_Inventory;
	pCPacket->m_gsv_INVENTORY_DATA.m_INV.m_i64Money = this->m_Inventory.m_i64Money;
	for (iC=0; iC<INVENTORY_TOTAL_SIZE; iC++)
		pCPacket->m_gsv_INVENTORY_DATA.m_INV.m_ItemLIST[ iC ] = this->m_Inventory.m_ItemLIST[ iC ];

    this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

#ifdef	__APPLY_EXTAND_QUEST_VAR
	////////////------------------ ÀÓ½Ã... ¾Æ·¡ ÆÐÅ¶ÀÌ ¾È°¡¸é Å¬¶óÀÌ¾ðÆ®¿¡¼­ ÁøÇà¾ÈµÊ
	//pCPacket = Packet_AllocNLock ();
	//if ( pCPacket ) {
	//	ZeroMemory( pCPacket->m_pDATA, 1020 );

	//	pCPacket->m_HEADER.m_wType = GSV_QUEST_DATA;
	//	pCPacket->m_HEADER.m_nSize = 1020;

	//	// ::CopyMemory( &pCPacket->m_gsv_QUEST_DATA.m_Quests, &this->m_Quests, sizeof( tagQuestData ) );
	//	// ::CopyMemory( &pCPacket->m_gsv_QUEST_DATA.m_WishLIST, &this->m_WishLIST, sizeof( tagWishLIST ) );

	//	this->SendPacket( pCPacket );
	//	Packet_ReleaseNUnlock( pCPacket );
	//}
	////////////------------------

	// Äù½ºÆ® µ¥ÀÌÅ¸
    pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_QUEST_ONLY;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_QUEST_ONLY );

	::CopyMemory( &pCPacket->m_gsv_QUEST_ONLY.m_Quests, &this->m_Quests, sizeof( tagQuestData ) );
    this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	// ±¸ÀÔ Èñ¸Á ¸ñ·Ï
    pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_WISH_LIST;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_WISH_LIST );

	::CopyMemory( &pCPacket->m_gsv_WISH_LIST.m_WishLIST, &this->m_WishLIST, sizeof( tagWishLIST ) );
    this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );
#else
	pCPacket = Packet_AllocNLock ();
	if ( pCPacket ) {
		pCPacket->m_HEADER.m_wType = GSV_QUEST_DATA;
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_QUEST_DATA );

		::CopyMemory( &pCPacket->m_gsv_QUEST_DATA.m_Quests, &this->m_Quests, sizeof( tagQuestData ) );
		::CopyMemory( &pCPacket->m_gsv_QUEST_DATA.m_WishLIST, &this->m_WishLIST, sizeof( tagWishLIST ) );

		this->SendPacket( pCPacket );
		Packet_ReleaseNUnlock( pCPacket );
	}
#endif

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ¼­¹öÀÇ °æÇèÄ¡¸¦ Àü¼Û :: »ç¿ëÀÚ °æÇèÄ¡°¡ º¯µ¿µÇ¾úÀ»°æ¿ì..
bool classUSER::Send_gsv_SETEXP (WORD wFromObjIDX)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType	= GSV_SETEXP;
	pCPacket->m_HEADER.m_nSize	= sizeof( gsv_SETEXP );
	pCPacket->m_gsv_SETEXP.m_lCurEXP = this->GetCur_EXP ();
	pCPacket->m_gsv_SETEXP.m_nCurSTAMINA = this->GetCur_STAMINA ();
	pCPacket->m_gsv_SETEXP.m_wFromObjIDX = wFromObjIDX;

	this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ·¹º§¾÷ µÇ¾úÀ»°æ¿ì È£ÃâµÇ´Â ÇÔ¼ö
bool classUSER::Send_gsv_LEVELUP (short nLevelDIFF)
{
/*	Á¸ ºÐÇÒ·Î ¿î¿µ½Ã ¿ùµå ¼­¹ö¿¡¼­ ÆÄÆ¼ ¿î¿µµÉ¼ö ÀÖµµ·Ï Àü¼ÛÇÏ´ø...
#ifndef	__INC_WORLD
	g_pSockLSV->Send_gsv_LEVEL_UP( LEVELUP_OP_USER, this->m_dwWSID, this->Get_TRUE_LEVEL(), m_GrowAbility.m_lEXP );
#endif
*/
	// ·¹º§¾÷½Ã È¹µæ Æ÷ÀÎÆ®...
	this->UpdateAbility ();		// levelup

	if ( this->Get_HP() > 0 ) {
		// Á×¾úÀ»¶§ ·¹º§¾÷ÇÏ¸é »ì¾Æ³ª´ø ¹ö±× ¼öÁ¤...
		this->Set_HP ( this->Get_MaxHP() );
		this->Set_MP ( this->Get_MaxMP() );
	}

    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	pCPacket->m_HEADER.m_wType			= GSV_LEVELUP;
	pCPacket->m_HEADER.m_nSize			= sizeof( gsv_LEVELUP );
	pCPacket->m_gsv_LEVELUP.m_wObjectIDX= this->Get_INDEX ();
	pCPacket->m_gsv_LEVELUP.m_nCurLevel	= this->Get_TRUE_LEVEL ();
	pCPacket->m_gsv_LEVELUP.m_lCurEXP   = m_GrowAbility.m_lEXP;
	pCPacket->m_gsv_LEVELUP.m_nBonusPoint = this->GetCur_BonusPOINT ();
	pCPacket->m_gsv_LEVELUP.m_nSkillPoint = this->GetCur_SkillPOINT ();

	//TODO:: ÀÌµ¿ ½ºÇÇµå°¡ ¹Ù²î¾úÀ¸¸é ÁÖº¯¿¡ Àüµ¿...
	this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

    pCPacket = Packet_AllocNLock ();
	if ( !pCPacket ) 
		return false;

	// ÁÖº¯ »ç¿ëÀÚµé¿¡°Ô´Â ´Ü¼øÈ÷ È¿°ú¸¸ º¸ÀÌÀÚ...
	pCPacket->m_HEADER.m_wType			= GSV_LEVELUP;
	pCPacket->m_HEADER.m_nSize			= sizeof( t_PACKETHEADER ) + sizeof(WORD);
	pCPacket->m_gsv_LEVELUP.m_wObjectIDX= this->Get_INDEX ();

    this->GetZONE()->SendPacketToSectors( this, pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	if ( this->GetPARTY() ) {
		this->m_pPartyBUFF->Member_LevelUP( this->m_nPartyPOS, nLevelDIFF );
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
/// Å¬¶óÀÌ¾ðÆ®¿¡ ¿öÇÁ ÇÏ¶ó´Â ¸í·É Àü¼Û
bool classUSER::Send_gsv_TELEPORT_REPLY (tPOINTF &PosWARP, short nZoneNO )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_TELEPORT_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_TELEPORT_REPLY );

	pCPacket->m_gsv_TELEPORT_REPLY.m_nZoneNO    = nZoneNO;
	pCPacket->m_gsv_TELEPORT_REPLY.m_PosWARP    = PosWARP;
	pCPacket->m_gsv_TELEPORT_REPLY.m_wObjectIDX = this->Get_INDEX();
	pCPacket->m_gsv_TELEPORT_REPLY.m_btRunMODE  = this->m_bRunMODE;		// °È±â ¶Ù±â »óÅÂ
	pCPacket->m_gsv_TELEPORT_REPLY.m_btRideMODE = this->m_btRideMODE;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
	
	return true;
}

/// °ú±ÝÀÌ ÁöºÒµÇ¾î¾ß¸¸ ÀÌµ¿°¡´ÉÇÑ Á¸ÀÎÁö Ã¼Å©...
bool  classUSER::Check_WarpPayment (short nZoneNO)
{
	if ( ZONE_IS_UNDERGROUND( nZoneNO ) ) {
		// ´øÁ¯À¸·ç ¸ø°¡ !!!
		if ( ZONE_PLANET_NO(nZoneNO) <= 1 ) {
			if ( !(this->m_dwPayFLAG & PLAY_FLAG_BATTLE ) ) {
				this->Send_gsv_BILLING_MESSAGE2( BILLING_MSG_JPN_NEED_CHARGE, 'I', PLAY_FLAG_BATTLE );
				return false;
			}
		} else
		if ( !(this->m_dwPayFLAG & PLAY_FLAG_DUNGEON_ADV ) ) {
			this->Send_gsv_BILLING_MESSAGE2( BILLING_MSG_JPN_NEED_CHARGE, 'Q', PLAY_FLAG_DUNGEON_ADV );
			return false;
		}
	}
	if ( ZONE_PLANET_NO(nZoneNO) >= 4 ) {
		if ( AGIT_ZONE_TYPE == ZONE_PVP_STATE( nZoneNO) ) {
			// ¾ÆÁöÆ®Á¸Àº ¿£Æ®¸®.
			if ( !(this->m_dwPayFLAG & PLAY_FLAG_BATTLE ) ) {
				// 3¹øÂ° Çà¼º±îÁö ÀÌµ¿Àº ¿£Æ®¸®·Î
				this->Send_gsv_BILLING_MESSAGE2( BILLING_MSG_JPN_NEED_CHARGE, 'H', PLAY_FLAG_BATTLE );
				return false;
			}
		} else
		if ( !(this->m_dwPayFLAG & PLAY_FLAG_STARSHIP_PASS ) ) {
			// Çà¼ºÀÌµ¿ ¸øÇØ !!!
			this->Send_gsv_BILLING_MESSAGE2( BILLING_MSG_JPN_NEED_CHARGE, 'P', PLAY_FLAG_STARSHIP_PASS );
			return false;
		}
	} else
	if ( ZONE_PLANET_NO(nZoneNO) != 1 && !(this->m_dwPayFLAG & PLAY_FLAG_BATTLE ) ) {
		// 3¹øÂ° Çà¼º±îÁö ÀÌµ¿Àº ¿£Æ®¸®·Î
		this->Send_gsv_BILLING_MESSAGE2( BILLING_MSG_JPN_NEED_CHARGE, 'I', PLAY_FLAG_BATTLE );
		return false;
	}
	return true;
}

/// ¿öÇÁ Ã³¸®...
short classUSER::Proc_TELEPORT (short nZoneNO, tPOINTF &PosWARP, bool bSkipPayment)	
{
	if ( NULL == this->GetZONE() ) {
		IS_HACKING( this, "Send_gsv_TELEPORT_REPLY :: must !this->GetZONE()" );
		return RET_FAILED;
	}

	if ( !g_pZoneLIST->IsValidZONE( nZoneNO ) ) {
		return RET_OK;
	}

	// Çà¼ºÀÌµ¿ °ú±Ý Ã¼Å©...
	if ( !bSkipPayment && !this->Check_WarpPayment(nZoneNO) )
		return true;

	if ( g_pZoneLIST->GetZONE( nZoneNO ) ) {
		// ·ÎÄÃ·Î ¿î¿µµÇ´Â Á¸ÀÌ´Ù.
		if ( !this->Send_gsv_TELEPORT_REPLY(PosWARP, nZoneNO) )
			return RET_FAILED;

		// Á¸¿¡¼­ »©¹ö¸®°í ´ÙÀ½ Á¸À§Ä¡ ÀúÀå....
		this->GetZONE()->Dec_UserCNT ();
		this->GetZONE()->Sub_DIRECT( this );

		this->m_nZoneNO = nZoneNO;
		this->m_PosCUR  = PosWARP;

		if ( this->Is_CartDriver() ) {
			// Ä«Æ® µå¶óÀÌ¹ö°¡ ¿öÇÁÇÒ¶§½Ã´Â µÚ¿¡ Å¾½ÂÀÚµµ °°ÀÌ ¿öÇÁ~
			classUSER *pUSER = g_pObjMGR->Get_UserOBJ( this->m_iLinkedCartObjIDX );
			if ( pUSER ) {
				pUSER->m_iLinkedCartObjIDX = 0;
				pUSER->Send_gsv_RELAY_REQ( RELAY_TYPE_RECALL, nZoneNO, PosWARP );
			}

			this->m_iLinkedCartObjIDX = 0;
		} else
		if ( this->Is_CartGuest() && this->m_iLinkedCartObjIDX ) {
			// ÀÌ·± °æ¿ì´Â ÀÖÀ¸¸é ¾ÈµÇÁö¸¸..È¤½Ã Ä¡Æ®·Î ¼ÒÈ¯À» ÇÑ´Ù´ø°¡ ÇÒ°æ¿ì
			classUSER *pUSER = g_pObjMGR->Get_UserOBJ( this->m_iLinkedCartObjIDX );
			if ( pUSER ) {
				pUSER->m_iLinkedCartObjIDX = 0;
			}

			// °­Á¦ ³»¸®±â
			this->m_btRideMODE = 0;
			this->m_btRideATTR = RIDE_ATTR_NORMAL;
			this->m_iLinkedCartObjIDX = 0;
		}

		return RET_SKIP_PROC;
	} 

	// ´Ù¸¥ ¼­¹ö¿¡ ÀÖ´Â Á¸À¸·Î ÀÌµ¿ÇÑ´Ù.
	// DB¿¡ ±â·ÏÈÄ ¿ùµå ¼­¹ö¿¡ ¿öÇÁ ÆÐÅ¶ Àü¼Û...
	this->GetZONE()->Dec_UserCNT ();
	this->GetZONE()->Sub_DIRECT( this );

	this->m_nZoneNO = nZoneNO;
	this->m_PosCUR  = PosWARP;

	g_pUserLIST->DeleteUSER( this, LOGOUT_MODE_WARP );

	return RET_SKIP_PROC;
}

//-------------------------------------------------------------------------------------------------
/// Å¬¶óÀÌ¾ðÆ®ÀÇ ÁÂÇ¥¸¦ º¸Á¤ÇÏ¶ó´Â ÆÐÅ¶...
bool classUSER::Send_gsv_ADJUST_POS (bool bOnlySelf)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_ADJUST_POS;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_ADJUST_POS );

	pCPacket->m_gsv_ADJUST_POS.m_wObjectIDX = this->Get_INDEX ();
	pCPacket->m_gsv_ADJUST_POS.m_PosCUR		= this->m_PosGOTO;
	pCPacket->m_gsv_ADJUST_POS.m_nPosZ		= this->m_nPosZ;

	if ( bOnlySelf )
		this->SendPacket( pCPacket );
	else
		this->GetZONE()->SendPacketToSectors( this, pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ½ºÅ³À» ½ÀµæÇßÀ»¶§ °á°ú Åëº¸
BYTE classUSER::Send_gsv_SKILL_LEARN_REPLY (short nSkillIDX, bool bCheckCOND)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return RESULT_SKILL_LEARN_FAILED;

	// °°Àº Á¾·ùÀÇ ½ºÅ³À» ÀÌ¹Ì º¸À¯ÇÏ°í ÀÖ´Ù¸é ???
	BYTE btResult;
	short nSkillSLOT;

	if ( bCheckCOND )
		btResult = this->Skill_LearnCondition ( nSkillIDX );
	else {
		// Äù½ºÆ®¿¡ ÀÇÇØ º¸»óÀ¸·Î ÁÖ¾îÁö´Â ½ºÅ³ÀÏ °æ¿ì Á¶°Ç ¹«½Ã...
		btResult = RESULT_SKILL_LEARN_SUCCESS;
	}

	if ( btResult == RESULT_SKILL_LEARN_SUCCESS ) {
		nSkillSLOT = this->Skill_FindEmptySlot( nSkillIDX );
		if ( nSkillSLOT >= 0 ) {
			// ½ºÅ³ ½Àµæ...
			g_pThreadLOG->When_LearnSKILL( this, nSkillIDX );

			switch( this->Skill_LEARN( nSkillSLOT, nSkillIDX, bCheckCOND ) ) {
				case 0x03 :
					this->Cal_BattleAbility ();
				case 0x01 :
					this->Update_SPEED ();
					this->Send_gsv_SPEED_CHANGED ();
			}
		} else
			btResult = RESULT_SKILL_LEARN_OUTOFSLOT;

		pCPacket->m_gsv_SKILL_LEARN_REPLY.m_btSkillSLOT = (BYTE)nSkillSLOT;
	}

	pCPacket->m_HEADER.m_wType = GSV_SKILL_LEARN_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SKILL_LEARN_REPLY );

	pCPacket->m_gsv_SKILL_LEARN_REPLY.m_btResult    = btResult;
	pCPacket->m_gsv_SKILL_LEARN_REPLY.m_nSkillIDX   = nSkillIDX;
	pCPacket->m_gsv_SKILL_LEARN_REPLY.m_nSkillPOINT = this->GetCur_SkillPOINT ();

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return btResult;
}

//-------------------------------------------------------------------------------------------------
/// Å¬¶óÀÌ¾ðÆ®°¡ ¿äÃ»ÇÑ ÄÉ¸¯ÅÍÀÇ HP Àü¼Û
bool classUSER::Send_gsv_HP_REPLY (int iObjectIDX, int iHP)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_HP_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_HP_REPLY );
	pCPacket->m_gsv_HP_REPLY.m_wObjectIDX = iObjectIDX;
	pCPacket->m_gsv_HP_REPLY.m_iHP = iHP;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ±Ó¼Ó¸» ÆÐÅ¶ ..
bool classUSER::Send_gsv_WHISPER (char *szFromAccount, char *szMessage)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_WHISPER;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_WHISPER );
	pCPacket->AppendString( szFromAccount );
	pCPacket->AppendString( szMessage );

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// °³ÀÎ 1:1 °Å·¡ÀÇ °á°ú ÆÐÅ¶ Àü¼Û
bool classUSER::Send_gsv_TRADE_P2P (int iObjectIDX, BYTE btResult, char cTradeSLOT)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_TRADE_P2P;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_TRADE_P2P );
	pCPacket->m_gsv_TRADE_P2P.m_wObjectIDX = iObjectIDX;
	pCPacket->m_gsv_TRADE_P2P.m_btRESULT = btResult;
	pCPacket->m_gsv_TRADE_P2P.m_cTradeSLOT = cTradeSLOT;

	this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

/// 1:1 °Å·¡½Ã °Å·¡ ½½·ÔÀÌ ¹Ù²î¾úÀ» °æ¿ì ¼­¹öÀÇ °Å·¡½½·Ô ¾ÆÀÌÅÛ Àü¼Û
bool classUSER::Send_gsv_TRADE_P2P_ITEM (char cTradeSLOT, tagITEM &sITEM)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_TRADE_P2P_ITEM;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_TRADE_P2P_ITEM );

	pCPacket->m_gsv_TRADE_P2P_ITEM.m_cTradeSLOT = cTradeSLOT;
	pCPacket->m_gsv_TRADE_P2P_ITEM.m_ITEM = sITEM;

	this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// Ã¢°íÀÇ ¸ðµç ¾ÆÀÌÅÛ Á¤º¸ Àü¼Û
bool classUSER::Send_gsv_BANK_ITEM_LIST (bool bNewBank)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	classPACKET *pCPacket2 = NULL;
	if ( !pCPacket )
		return false;

	this->m_btBankData = BANK_LOADED;
	pCPacket->m_gsv_BANK_LIST_REPLY.m_btREPLY = BANK_REPLY_INIT_DATA;
	pCPacket->m_gsv_BANK_LIST_REPLY.m_btItemCNT = 0;

	if ( !bNewBank ) {
		for (short nI=0; nI<BANKSLOT_PLATINUM_0; nI++) {
			if ( !this->m_Bank.m_ItemLIST[ nI ].GetTYPE() )
				continue;

			pCPacket->m_gsv_BANK_LIST_REPLY.m_sInvITEM[ pCPacket->m_gsv_BANK_LIST_REPLY.m_btItemCNT ].m_btInvIDX = (BYTE)nI;
			pCPacket->m_gsv_BANK_LIST_REPLY.m_sInvITEM[ pCPacket->m_gsv_BANK_LIST_REPLY.m_btItemCNT ].m_ITEM = this->m_Bank.m_ItemLIST[ nI ];
			pCPacket->m_gsv_BANK_LIST_REPLY.m_btItemCNT ++;
		}

#ifdef	__INC_PLATINUM
		pCPacket2 = Packet_AllocNLock ();
		if ( pCPacket2 ) {
			// ÇÃ·¹Æ¼³Ñ Ã¢°í ¾ÆÀÌÅÛ...
			pCPacket2->m_gsv_BANK_LIST_REPLY.m_btREPLY = BANK_REPLY_PLATINUM;
			pCPacket2->m_gsv_BANK_LIST_REPLY.m_btItemCNT = 0;
			
			for (short nI=BANKSLOT_PLATINUM_0; nI<BANKSLOT_TOTAL; nI++) {
				if ( !this->m_Bank.m_ItemLIST[ nI ].GetTYPE() )
					continue;

				pCPacket2->m_gsv_BANK_LIST_REPLY.m_sInvITEM[ pCPacket2->m_gsv_BANK_LIST_REPLY.m_btItemCNT ].m_btInvIDX = (BYTE)nI;
				pCPacket2->m_gsv_BANK_LIST_REPLY.m_sInvITEM[ pCPacket2->m_gsv_BANK_LIST_REPLY.m_btItemCNT ].m_ITEM = this->m_Bank.m_ItemLIST[ nI ];
				pCPacket2->m_gsv_BANK_LIST_REPLY.m_btItemCNT ++;
			}

			if ( pCPacket2->m_gsv_BANK_LIST_REPLY.m_btItemCNT ) {
				pCPacket2->m_HEADER.m_wType = GSV_BANK_LIST_REPLY;
				pCPacket2->m_HEADER.m_nSize = sizeof( gsv_BANK_LIST_REPLY ) + sizeof(tag_SET_INVITEM) * pCPacket2->m_gsv_BANK_LIST_REPLY.m_btItemCNT;
			} else {
				// 0°³¶ó º¸³¾ ÇÊ¿ä ¾ø´ç
				Packet_ReleaseNUnlock( pCPacket2 );
				pCPacket2 = NULL;
			}
		}
#endif
	}

    pCPacket->m_HEADER.m_wType = GSV_BANK_LIST_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_BANK_LIST_REPLY ) + sizeof(tag_SET_INVITEM) * pCPacket->m_gsv_BANK_LIST_REPLY.m_btItemCNT;
	// Ã¢°í¿¡ º¸°üµÈ ÁÙ¸®...
	pCPacket->AppendData( &this->m_Bank.m_i64ZULY, sizeof(__int64) );

	this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	if ( pCPacket2 ) {
		this->SendPacket( pCPacket2 );
		Packet_ReleaseNUnlock( pCPacket2 );
	}

	return true;
}

/// Ã¢°í ¸®½ºÆ®¿¡ ´ëÇÑ °á°ú ÆÐÅ¶ Àü¼Û
bool classUSER::Send_gsv_BANK_LIST_REPLY (BYTE btReply)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_BANK_LIST_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_BANK_LIST_REPLY );

	pCPacket->m_gsv_BANK_LIST_REPLY.m_btREPLY	= btReply;
	pCPacket->m_gsv_BANK_LIST_REPLY.m_btItemCNT = 0;

	this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ÆÄÆ¼ ¿äÃ» ÆÐÅ¶ Àü¼Û
bool classUSER::Send_gsv_PARTY_REQ(int iObjectIDXorTAG, BYTE btReq, bool bShareParty)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_PARTY_REQ;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_PARTY_REQ );
	pCPacket->m_gsv_PARTY_REQ.m_btREQUEST = btReq;
	pCPacket->m_gsv_PARTY_REQ.m_dwFromIDXorTAG = iObjectIDXorTAG;
	pCPacket->m_gsv_PARTY_REQ.m_bShareParty = bShareParty;
	this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}
/// ÆÄÆ¼ ¿äÃ»¿¡ ´ëÇÑ °á°ú ÆÐÅ¶ Àü¼Û
bool classUSER::Send_gsv_PARTY_REPLY(int iObjectIDXorTAG, BYTE btReply)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_PARTY_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_PARTY_REPLY );
	pCPacket->m_gsv_PARTY_REPLY.m_btREPLY = btReply;
	pCPacket->m_gsv_PARTY_REPLY.m_dwFromIDXorTAG = iObjectIDXorTAG;

	this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// 1°³ÀÇ ÀÎº¥Åä¸®ÀÇ º¯°æµÈ ¾ÆÀÌÅÛ Àü¼Û
bool classUSER::Send_gsv_SET_INV_ONLY (BYTE btInvIDX, tagITEM *pITEM, WORD wType)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = wType;//GSV_SET_INV_ONLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_INV_ONLY ) + sizeof( tag_SET_INVITEM );

	pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT = 1;
	pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_btInvIDX = btInvIDX;
	pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_ITEM     = *pITEM;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// 2°³ÀÇ ÀÎº¥Åä¸®ÀÇ º¯°æµÈ ¾ÆÀÌÅÛ Àü¼Û
bool classUSER::Send_gsv_SET_TWO_INV_ONLY (WORD wType, BYTE btInvIdx1, tagITEM *pITEM1, BYTE btInvIdx2, tagITEM *pITEM2)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = wType;	// GSV_SET_INV_ONLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_INV_ONLY ) + sizeof( tag_SET_INVITEM ) * 2;

	pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT = 2;

	pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_btInvIDX = btInvIdx1;
	pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_ITEM     = *pITEM1;

	pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_btInvIDX = btInvIdx2;
	pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_ITEM     = *pITEM2;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ¼­¹ö¿¡¼­ Ã³¸® ¼ø¼­»ó ¹Ù·ÎÃ³¸®ÇÏ±â °ï¶õÇÑ ÆÐÅ¶ÀÇ °æ¿ì Å¬¶óÀÌ¾ðÆ®·Î ¿äÃ»ÇÏ¶ó´Â ÆÐÅ¶À» Àü¼ÛÇÏ¸é
/// Å¬¶óÀÌ¾î¾ðÆ®´Â ±×¿¡ »óÀÀÇÏ´Â ¿äÃ»ÆÐÅ¶À» ¼­¹ö¿¡ Àü¼ÛÇÏ°í ¼­¹ö´Â ±× ÆÐÅ¶À» ¹Þ¾ÒÀ»½Ã Ã³¸®ÇÏ±âÀ§ÇØ »ç¿ëµÇ´Â ÇÔ¼ö
bool classUSER::Send_gsv_RELAY_REQ( WORD wRelayTYPE, short nZoneGOTO, tPOINTF &PosGOTO )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_RELAY_REQ;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_RELAY_REQ );

	pCPacket->m_gsv_RELAY_REQ.m_wRelayTYPE  = wRelayTYPE;
	pCPacket->m_gsv_RELAY_REQ.m_nCallZoneNO = nZoneGOTO;
	pCPacket->m_gsv_RELAY_REQ.m_PosCALL		= PosGOTO;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
	return true;
}

//-------------------------------------------------------------------------------------------------
bool classUSER::Recv_cli_STRESS_TEST( t_PACKET *pPacket )
{
	/*
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = CLI_STRESS_TEST;
    pCPacket->m_HEADER.m_nSize = sizeof( cli_STRESS_TEST );
	pCPacket->AppendString( "Reply STRESS_TEST" );
	pCPacket->m_HEADER.m_nSize = pPacket->m_HEADER.m_nSize;

	_ASSERT( pCPacket->m_HEADER.m_nSize >= sizeof(t_PACKETHEADER ) );

	g_pUserLIST->Send_cli_STRESS_TEST( pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
	*/
	return false;
}

//-------------------------------------------------------------------------------------------------
/// Å¬¶óÀÌ¾ðÆ®¿¡ Áß°èÇÏ¶ó´Â ÆÐÅ¶À» Àü¼ÛÈÄ¿¡ ¹Þ´Â ÀÀ´ä ÆÐÅ¶ Ã³¸®...
short classUSER::Recv_cli_RELAY_REPLY( t_PACKET *pPacket )
{
	switch( pPacket->m_cli_RELAY_REPLY.m_wRelayTYPE ) {
		case RELAY_TYPE_RECALL :
			return Proc_TELEPORT( pPacket->m_cli_RELAY_REPLY.m_nCallZoneNO,	// Recv_cli_RELAY_REPLY
											pPacket->m_cli_RELAY_REPLY.m_PosCALL );
	}

	return RET_FAILED;
}

//-------------------------------------------------------------------------------------------------
/// ¿ùµå ¼­¹ö¿¡¼­ Ã¤³Î¼­¹ö·Î ÀÌµ¿ÈÄ ÇÃ·¹ÀÌµÇ±âÀü¿¡ ÀÎÁõÀ» ¿äÃ»ÇÏ´Â ÆÐÅ¶
/// ÀÌ ÆÐÅ¶À» ¹ÞÀ¸¸é Ã¤³Î¼­¹ö°¡ ¿ùµå¼­¹ö·Î ºÎÅÍ ÀÎÁõÀ» ¹Þ°í ÀÀ´äÀ» Å¬¶óÀÌ¾ðÆ®·Î º¸³¿
bool classUSER::Recv_cli_JOIN_SERVER_REQ( t_PACKET *pPacket )
{
	// GUMS¿¡ Àü¼ÛÇÏ±â À§ÇØ¼­ ºñ¹ø ÀúÀå...
	::CopyMemory( this->m_dwMD5PassWD, pPacket->m_cli_JOIN_SERVER_REQ.m_MD5Password, sizeof(DWORD)*8 );
	this->m_szMD5PassWD[ 32 ] = 0;

	// ·Î±×ÀÎ ¼­¹ö ¶Ç´Â ¿ùµå ¼­¹ö¿¡ ÀÎÁõ ¿äÃ»..
	g_pSockLSV->Send_zws_CONFIRM_ACCOUNT_REQ( this->m_iSocketIDX, pPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ÄÉ¸¯ÅÍ ¸®½ºÆ® ¿äÃ»Ã³¸® :: °³ÀÎ ¼­¹ö¿¡¼­¸¸ »ç¿ëµÊ
bool classUSER::Recv_cli_CHAR_LIST( t_PACKET *pPacket )
{
	return g_pThreadSQL->Add_SqlPacketWithACCOUNT (this, pPacket );
}

//-------------------------------------------------------------------------------------------------
/// ÄÉ¸¯ÅÍ ¼±ÅÃ ¿äÃ»Ã³¸® :: °³ÀÎ ¼­¹ö¿¡¼­¸¸ »ç¿ëµÊ
bool classUSER::Recv_cli_SELECT_CHAR( t_PACKET *pPacket, bool bFirstZONE, BYTE btRunMODE, BYTE btRideMODE )
{
	// 1 == pSelCharPket->m_cli_SELECT_CHAR.m_btCharNO
	// ÀÌ¸é »ç¿ëÀÚÇÑÅ× DB¿¡¼­ ÄÉ¸¯ÅÍ ÀÐ¾î¼­ Àü¼Û...
	pPacket->m_cli_SELECT_CHAR.m_btCharNO   = bFirstZONE;
	pPacket->m_cli_SELECT_CHAR.m_btRunMODE  = btRunMODE;
	pPacket->m_cli_SELECT_CHAR.m_btRideMODE = btRideMODE;

	return g_pThreadSQL->Add_SqlPacketWithACCOUNT (this, pPacket );
}

//-------------------------------------------------------------------------------------------------
/// ÄÉ¸¯ÅÍ »ý¼º ¿äÃ»Ã³¸® :: °³ÀÎ ¼­¹ö¿¡¼­¸¸ »ç¿ëµÊ
bool classUSER::Recv_cli_CREATE_CHAR( t_PACKET *pPacket )
{
	return g_pThreadSQL->Add_SqlPacketWithACCOUNT (this, pPacket );
}

//-------------------------------------------------------------------------------------------------
/// ÄÉ¸¯ÅÍ »èÁ¦ ¿äÃ»Ã³¸® :: °³ÀÎ ¼­¹ö¿¡¼­¸¸ »ç¿ëµÊ
bool classUSER::Recv_cli_DELETE_CHAR( t_PACKET *pPacket )
{
	short nOffset=sizeof(cli_DELETE_CHAR);

	char *pCharName=Packet_GetStringPtr( pPacket, nOffset );
	if ( !pCharName || !this->Get_ACCOUNT() ) {
		return false;
	}

#ifdef	__NEW_LOG
	g_pThreadLOG->When_CharacterLOG( this, pCharName, NEWLOG_DEL_START_CHAR );
#else
	g_pThreadLOG->When_DeleteCHAR( this, pCharName );
#endif

	return g_pThreadSQL->Add_SqlPacketWithACCOUNT (this, pPacket );
}


//-------------------------------------------------------------------------------------------------
/// Å¬¶óÀÌ¾ðÆ®¿¡¼­ Á¸¿¡ ÀÔÀåÇÏ°Ú´Ù´Â ÆÐÅ¶
bool classUSER::Recv_cli_JOIN_ZONE ( t_PACKET *pPacket )
{
	if ( this->GetZONE() || this->Get_INDEX() ) {
		IS_HACKING( this, "Recv_cli_JOIN_ZONE:: must !this->GetZONE() && !this->Get_INDEX() " );
		return false;
	}

	CZoneTHREAD *pZONE = g_pZoneLIST->GetZONE( m_nZoneNO );
	if ( pZONE ) {
		this->m_btWeightRate = pPacket->m_cli_JOIN_ZONE.m_btWeightRate;
		if ( this->m_btWeightRate >= WEIGHT_RATE_WALK ) {
			// ¶Ù±â ºÒ´É.
			this->m_bRunMODE = false;
		}
		this->m_nPosZ		 = pPacket->m_cli_JOIN_ZONE.m_nPosZ;

		this->Del_ActiveSKILL ();
		this->CObjAI::SetCMD_STOP ();
		this->Clear_SummonCNT ();			// Á¸ ¿öÇÁ½Ã...

		if ( pZONE->Add_OBJECT( this ) ) {	// »ç¿ëÀÚ Á¸¿¡ Âü°¡ ¿äÃ» 
			pZONE->Inc_UserCNT ();

			this->m_dwRecoverTIME = 0;

			if ( this->GetPARTY() ) {
				this->m_pPartyBUFF->Change_ObjectIDX( this );
			}
			return true;
		}
	}

	return false;
}


//-------------------------------------------------------------------------------------------------
/// »ç¸Á½Ã ºÎÇÒ ¿äÃ» ÆÐÅ¶ Ã³¸®
short classUSER::Recv_cli_REVIVE_REQ( BYTE btReviveTYPE, bool bApplyPenalty, bool bSkipPayment )
{
	short nZoneNO;
	tPOINTF	PosREVIVE;

	// Á×À»¶§ °É¾î³õÀº »óÅÂ¸¸ ÇØÁö...
	this->m_IngSTATUS.ClearStatusFLAG( ING_FAINTING );
	this->m_dwRecoverTIME = 0;

	switch( btReviveTYPE ) {
		case REVIVE_TYPE_SAVE_POS :		// ÀúÀåµÈ ºÎÈ°Àå¼Ò¿¡¼­ »ì¾Æ³ª±â..
			if ( ZONE_PLANET_NO(this->m_nReviveZoneNO) == ZONE_PLANET_NO(this->m_nZoneNO) ) {
				// ÀúÀåµÈ Á¸ÀÇ Çà¼º°ú Á×Àº Á¸ÀÇ Çà¼ºÀÌ °°À¸¸é ºÎÈ°Á¸À¸·Î ....
				nZoneNO   = this->m_nReviveZoneNO;
				PosREVIVE = this->m_PosREVIVE;
				if ( IsTAIWAN() ) {
					if ( ZONE_REVIVE_ZONENO( this->m_nZoneNO ) ) {
						nZoneNO		= ZONE_REVIVE_ZONENO( this->m_nZoneNO );
						PosREVIVE.x = ZONE_REVIVE_X_POS( this->m_nZoneNO ) * 1000.f;
						PosREVIVE.y = ZONE_REVIVE_Y_POS( this->m_nZoneNO ) * 1000.f;
					}
				}

				PosREVIVE.x += ( RANDOM(1001) - 500 );	// ·£´ý 5¹ÌÅÍ..
				PosREVIVE.y += ( RANDOM(1001) - 500 );
				break;
			}

		case REVIVE_TYPE_REVIVE_POS :	// ÇöÀç Á¸ÀÇ ºÎÈ°Àå¼Ò¿¡¼­ »ì¾Æ³ª±â
			nZoneNO   = this->m_nZoneNO;

			if ( !m_bSetImmediateRevivePOS ) {
				PosREVIVE = this->GetZONE()->Get_RevivePOS( this->m_PosCUR );
			} else {
				PosREVIVE = this->m_PosImmediateRivive;
			}

			PosREVIVE.x += ( RANDOM(1001) - 500 );	// ·£´ý 5¹ÌÅÍ..
			PosREVIVE.y += ( RANDOM(1001) - 500 );

			// 2%´õ °æÄ¡¸¦ ¾ò¾î¾ß ÇÑ´Ù.
			//if ( bApplyPenalty && 0 == ZONE_PVP_STATE( this->m_nZoneNO ) ) {
			//	// °æÇèÄ¡ ÆÐ³ÎÄ¡ Àû¿ë.
			//	this->Set_PenalEXP( PENALTY_EXP_FIELD-PENALTY_EXP_TOWN );
			//}
			break;

		case REVIVE_TYPE_CURRENT_POS :	// ÇöÀç Àå¼Ò¿¡¼­...
			nZoneNO   = this->m_nZoneNO;
			PosREVIVE = this->m_PosCUR;
			break;

		case REVIVE_TYPE_START_POS :	// ÇöÀçÁ¸ÀÇ ½ÃÀÛÀ§Ä¡¿¡¼­
			nZoneNO	  = this->m_nZoneNO;
			PosREVIVE = this->GetZONE()->Get_StartPOS ();
			PosREVIVE.x += ( RANDOM(1001) - 500 );	// ·£´ý 5¹ÌÅÍ..
			PosREVIVE.y += ( RANDOM(1001) - 500 );
			break;

		default :
			return RET_FAILED;
	}

	// zone server ip ...
	if ( bApplyPenalty ) {
		#define	STB_ING_REVIVE_ROW		57	
		if ( ZONE_PVP_STATE( this->m_nZoneNO ) ) {
			// PVPÁ¸ÀÌ¸é 10FPS * 30ÃÊ°£ ¹«Àû...
			this->m_IngSTATUS.UpdateIngSTATUS( this, STB_ING_REVIVE_ROW, 30, 1, 0 );
		}

		// ÃÖ´ë HPÀÇ 30% 
		this->Set_HP( 3*this->GetCur_MaxHP() / 10 );
	}

	// 2005. 09. 09 Á×¾ú´Ù »ì¾Æ³ª´Â °æ¿ì´Â Á¸¿¡ ÀÔÀå½Ã °ú±Ý Ã¼Å© ¾ÈÇÏµµ·Ï...
	return this->Proc_TELEPORT( nZoneNO, PosREVIVE, bSkipPayment );
}
/// ºÎÈ° Á¸ º¯°æ ÆÐÅ¶
bool classUSER::Recv_cli_SET_REVIVE_POS ()
{
	if ( !this->GetZONE() ) {
		assert( 0  && "Recv_cli_SET_REVIVE_POS" );
		return false;
	}

	this->m_nReviveZoneNO = this->m_nZoneNO;
	this->m_PosREVIVE = this->GetZONE()->Get_RevivePOS( this->m_PosCUR );

	return true;
}

bool classUSER::Recv_cli_SET_VAR_REQ( t_PACKET *pPacket )
{
	/// Å¬¶óÀÌ¾ðÆ®¿¡¼­ »ç¿ëÇÏÁö ¾ÊÀ½À¸·Î ÇØÅ·ÀÌ´Ù.
	IS_HACKING( this, "classUSER::cli_SET_VAR_REQ( Script hacking.. )" );
	return false;
/*
	int iValue = pPacket->m_cli_SET_VAR_REQ.m_iValue;

	switch( pPacket->m_cli_SET_VAR_REQ.m_btVarTYPE ) {
		case SV_FAME :		this->SetCur_FAME (iValue);		break;
		case SV_UNION :		this->SetCur_UNION (iValue);	break;

//		case SV_SEX :		this->Set_SEX (iValue);			break;
//		case SV_LEVEL :		this->Set_LEVEL (iValue);		break;
//		case SV_GRADE :		g_pAVATAR->Set_RANK (iValue);	break;

		case SV_CLASS :		this->SetCur_JOB (iValue);		break;
//		±âº» ´É·ÂÄ¡´Â Å¬¶óÀÌ¾ðÆ® ÀÓÀÇ·Î ¹Ù²Ü¼ö ¾ø´Ù.
//		case SV_STR :		this->SetDef_STR (iValue);		break;
//		case SV_DEX :		this->SetDef_DEX (iValue);		break;
//		case SV_WIS :		this->SetDef_INT (iValue);		break;
//		case SV_CON :		this->SetDef_CON (iValue);		break;
//		case SV_CHARM :		this->SetDef_CHARM (iValue);	break;
//		case SV_EMOTION :	this->SetDef_SENSE (iValue);	break;
		default :
			return false;
	}

	classPACKET *pCPacket = ::Packet_AllocNLock ();

	pCPacket->m_HEADER.m_wType = GSV_SET_VAR_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_VAR_REPLY );

	// ÀûÀýÄ¡ ¸øÇÑ °ªÀÏ°æ¿ì VarTYPE¿¡ REPLY_GSV_SET_VAR_FAIL_BIT or ½ÃÄÑ ¸®ÅÏÇÑ´Ù.
	pCPacket->m_gsv_SET_VAR_REPLY.m_btVarTYPE = pPacket->m_cli_SET_VAR_REQ.m_btVarTYPE;
	pCPacket->m_gsv_SET_VAR_REPLY.m_iValue	  = iValue;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
*/
}


//-------------------------------------------------------------------------------------------------
/// °È±â/¶Ù±â, Ä«Æ® Å¸±â/³»¸®±â »óÅÂ º¯°æ
bool classUSER::Recv_cli_TOGGLE( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;
	return this->SetCMD_TOGGLE( pPacket->m_cli_TOGGLE.m_btTYPE );
}
/// Æ¯Á¤ ¸ð¼ÇÀ» ÃëÇÑ´Ù´Â ÆÐÅ¶( °¨Á¤°ü·Ã µ¿ÀÛµî... )
bool classUSER::Recv_cli_SET_MOTION( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;
	this->Send_gsv_SET_MOTION( pPacket->m_cli_SET_MOTION.m_wValue, pPacket->m_cli_SET_MOTION.m_nMotionNO );
	if(pPacket->m_cli_SET_MOTION.m_iSkillIDX) this->Skill_UseAbilityValue(pPacket->m_cli_SET_MOTION.m_iSkillIDX);
	return true;
}

//-------------------------------------------------------------------------------------------------
/// µ¿¸Í Ã¤ÆÃ
short classUSER::Recv_cli_ALLIED_CHAT( t_PACKET *pPacket )
{
    char *szMsg;
    short nOffset=sizeof( cli_CHAT );

    szMsg = Packet_GetStringPtr( pPacket, nOffset );
    if ( szMsg ) {
		if ( szMsg[ 0 ] == '/' ) {
			if ( this->Check_CheatCODE( szMsg ) ) {
				return ( !this->GetZONE() ? RET_SKIP_PROC : RET_OK );
			}
		} else
		if ( !this->m_IngSTATUS.IsSET( FLAG_SUB_STORE_MODE ) ) {
			if ( szMsg[ 0 ] == '^' ) {
				// Å¸ÀÌÆ² ¼³Á¤
				if ( strlen( &szMsg[1] ) >= MAX_USER_TITLE ) {
					strncpy( this->m_szUserTITLE, &szMsg[1], MAX_USER_TITLE );
					this->m_szUserTITLE[ MAX_USER_TITLE ] = 0;
				} else {
					strcpy( this->m_szUserTITLE, &szMsg[1] );
				}

				this->m_IngSTATUS.SetSubFLAG( FLAG_SUB_INTRO_CHAT );
			} else 
			if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_INTRO_CHAT ) ) {
				this->m_IngSTATUS.ClearSubFLAG( FLAG_SUB_INTRO_CHAT );
			}
		}

		classPACKET *pCPacket = Packet_AllocNLock ();
		if ( pCPacket ) {
			pCPacket->m_HEADER.m_wType = GSV_ALLIED_CHAT;
			pCPacket->m_HEADER.m_nSize = sizeof( gsv_ALLIED_CHAT );
			pCPacket->m_gsv_ALLIED_CHAT.m_wObjectIDX = this->Get_INDEX();
			pCPacket->m_gsv_ALLIED_CHAT.m_iTeamNO = this->Get_TeamNO();
			pCPacket->AppendString( szMsg );

			this->GetZONE()->SendTeamPacketToSectors( this, pCPacket, this->Get_TeamNO() );
			Packet_ReleaseNUnlock( pCPacket );
		}
    }

    return RET_OK;
}

//-------------------------------------------------------------------------------------------------
/// µ¿¸Í ¿ÜÄ¡±â
short classUSER::Recv_cli_ALLIED_SHOUT( t_PACKET *pPacket )
{
    char *szMsg;
    short nOffset=sizeof( cli_SHOUT );

    szMsg = Packet_GetStringPtr( pPacket, nOffset );
    if ( szMsg ) {
		if ( szMsg[ 0 ] == '/' ) {
			if ( this->Check_CheatCODE( szMsg ) ) {
				return ( !this->GetZONE() ? RET_SKIP_PROC : RET_OK );
			}
		}

		classPACKET *pCPacket = Packet_AllocNLock ();
		if ( pCPacket ) {
			pCPacket->m_HEADER.m_wType = GSV_ALLIED_SHOUT;
			pCPacket->m_HEADER.m_nSize = sizeof( gsv_ALLIED_SHOUT );
			pCPacket->m_gsv_ALLIED_SHOUT.m_iTeamNO = this->Get_TeamNO();
			pCPacket->AppendString( this->Get_NAME() );
			pCPacket->AppendString( szMsg );

			this->GetZONE()->SendShout( this, pCPacket, this->Get_TeamNO() );
			Packet_ReleaseNUnlock( pCPacket );
		}
    }

    return RET_OK;
}

//-------------------------------------------------------------------------------------------------
/// ÀÏ¹Ý Ã¤ÆÃ
short classUSER::Recv_cli_CHAT( t_PACKET *pPacket )
{
    char *szMsg;
    short nOffset=sizeof( cli_CHAT ), nOutStrLen;

    szMsg = Packet_GetStringPtr( pPacket, nOffset, nOutStrLen );
    if ( szMsg ) {
		this->LogCHAT( szMsg, NULL , "CHAT" );
		if ( szMsg[ 0 ] == '/' ) {
			if ( this->Check_CheatCODE( szMsg ) ) {
				return ( !this->GetZONE() ? RET_SKIP_PROC : RET_OK );
			}
		} else
		if ( !this->m_IngSTATUS.IsSubSET( FLAG_SUB_STORE_MODE ) ) {
			if ( szMsg[ 0 ] == '^' ) {
				// Å¸ÀÌÆ² ¼³Á¤
				if ( strlen( &szMsg[1] ) >= MAX_USER_TITLE ) {
					strncpy( this->m_szUserTITLE, &szMsg[1], MAX_USER_TITLE );
					this->m_szUserTITLE[ MAX_USER_TITLE ] = 0;
				} else {
					strcpy( this->m_szUserTITLE, &szMsg[1] );
				}

				this->m_IngSTATUS.SetSubFLAG( FLAG_SUB_INTRO_CHAT );
			} else 
			if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_INTRO_CHAT ) ) {
				this->m_IngSTATUS.ClearSubFLAG( FLAG_SUB_INTRO_CHAT );
			}
		}

		if(nOutStrLen > 129)
		{
			return IS_HACKING(this, "Recv_cli_CHAT: Client crash packet recv'd and filtered.");
		}

		this->Send_gsv_CHAT( szMsg );
    }

    return RET_OK;
}

//-------------------------------------------------------------------------------------------------
/// ÀÏ¹Ý ¿ÜÄ¡±â...
short classUSER::Recv_cli_SHOUT( t_PACKET *pPacket )
{
//	if ( this->m_dwPayFLAG & 
    char *szMsg;
    short nOffset=sizeof( cli_SHOUT ), nOutStrLen;

    szMsg = Packet_GetStringPtr( pPacket, nOffset, nOutStrLen );
    if ( szMsg ) {
		this->LogCHAT( szMsg, NULL , "SHOUT" );
		if ( szMsg[ 0 ] == '/' ) {
			if ( this->Check_CheatCODE( szMsg ) ) {
				return ( !this->GetZONE() ? RET_SKIP_PROC : RET_OK );
			}
		}

		if(nOutStrLen > 129)
		{
			return IS_HACKING(this, "Recv_cli_SHOUT: Client crash packet recv'd and filtered\n");
		}

		this->Send_gsv_SHOUT ( szMsg );
    }

    return RET_OK;
}

//-------------------------------------------------------------------------------------------------
/// ±Ó¼Ó¸»
bool classUSER::Recv_cli_WHISPER( t_PACKET *pPacket )
{
    char *szDest, *szMsg;
    short nOffset=sizeof( cli_WHISPER );

    szDest = Packet_GetStringPtr( pPacket, nOffset );
    szMsg  = Packet_GetStringPtr( pPacket, nOffset );

	if ( NULL == szMsg || 0 == szMsg[ 0 ] )
		return true;

    if ( szDest && szMsg ) {
        // ¿ùµå ¼­¹ö¿¡ ±Ó¼Ó¸» ¸Þ¼¼Áö Àü¼Û.
		classUSER *pUser = g_pUserLIST->Find_CHAR( szDest );
		if ( pUser ) {
			this->LogCHAT( szMsg , pUser->Get_NAME(), "WHISPER" );
			pUser->Send_gsv_WHISPER( this->Get_NAME(), szMsg );
		} else {
			this->LogCHAT( szMsg , NULL, "WHISPER");
			this->Send_gsv_WHISPER( szDest, NULL );
		}
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
/// ÆÄÆ¼Ã¤ÆÃ
short classUSER::Recv_cli_PARTY_CHAT( t_PACKET *pPacket )
{
    char *szMsg;
    short nOffset=sizeof( cli_PARTY_CHAT );

    szMsg = Packet_GetStringPtr( pPacket, nOffset );
    if ( szMsg ) {
		this->LogCHAT( szMsg , NULL, "PARTY_CHAT");
		if ( szMsg[ 0 ] == '/' ) {
			if ( this->Check_CheatCODE( szMsg ) ) {
				return ( !this->GetZONE() ? RET_SKIP_PROC : RET_OK );
			}
		}

        classPACKET *pCPacket = Packet_AllocNLock ();

        pCPacket->m_HEADER.m_wType = GSV_PARTY_CHAT;
        pCPacket->m_HEADER.m_nSize = sizeof( gsv_PARTY_CHAT );
        pCPacket->m_gsv_PARTY_CHAT.m_wObjectIDX = this->Get_INDEX();
        pCPacket->AppendString( szMsg );

        this->SendPacketToPARTY( pCPacket );
        Packet_ReleaseNUnlock( pCPacket );
    }

	return RET_OK;
}

//-------------------------------------------------------------------------------------------------
/// Á¤Áö ¸í·É
bool classUSER::Recv_cli_STOP( t_PACKET *pPacket )
{
	this->SetCMD_STOP ();
    return true;
}

//-------------------------------------------------------------------------------------------------
/// °ø°Ý ¸í·É
bool classUSER::Recv_cli_ATTACK( t_PACKET *pPacket )
{
	if ( !(this->m_dwPayFLAG & PLAY_FLAG_BATTLE) ) {
		// °ø°Ý ¸øÇØ... µ·³»¾ßÇÔ
		return true;
	}

//	if ( this->Get_ActiveSKILL() ) return true;	// ½ºÅ³ ÄÉ½ºÆÃ ÁßÀÏ¶© ¸ø¹Ù²ã
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() || this->Get_WeightRATE() >= WEIGHT_RATE_CANT_ATK ) {
		// ¹«°Ì´Ù.. ¸í·É ºÒ°¡...
		return true;
	}

	// Å¾½Â ¸ðµåÀÌ°í ¾ÏÁî ¾ÆÀÌÅÛ¿¡ °ø°Ý°Å¸®°¡ ÀÖ¾î¾ß ÇÑ´Ù.
	if ( this->GetCur_MOVE_MODE() == MOVE_MODE_DRIVE ) {
		// °ø°Ý ¹«±â°¡ ÀÖ³Ä ?
		if ( m_RideITEM[ RIDE_PART_ARMS ].m_nItemNo <= 0 || PAT_ITEM_ATK_RANGE( m_RideITEM[ RIDE_PART_ARMS ].m_nItemNo ) <= 0 )
			return true;

		// Ä«Æ®/Ä³½½±â¾î Å¾½ÂÁßÀÌ°í ¿¬·á°¡ ÀÖ³Ä ?
		tagITEM *pItem = &m_Inventory.m_ItemRIDE[ RIDE_PART_ENGINE ];
		if ( pItem->GetLife() <=0 )
			return true;
	} else
	if ( this->GetCur_MOVE_MODE() > MOVE_MODE_DRIVE ) {
		return true;
	} else
	if ( this->m_pShotITEM ) {
		// ¼Ò¸ðÅº ÇÊ¿ä½Ã °¹¼ö Ã¼Å©...
		if ( this->m_pShotITEM->GetQuantity() < 1 ) {
			return true;
		}
	}

	this->SetCMD_ATTACK( pPacket->m_cli_ATTACK.m_wTargetObjectIndex );
    return true;
}

//-------------------------------------------------------------------------------------------------
bool classUSER::Recv_cli_DAMAGE( t_PACKET *pPacket )
{
    return true;
}

//-------------------------------------------------------------------------------------------------
/// ÀÌµ¿ ¶Ç´Â ¾ÆÀÌÅÛ ½Àµæ ¸í·É
bool classUSER::Recv_cli_MOUSECMD( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;

	// Ä«Æ®/Ä³½½±â¾î Å¾½ÂÁßÀÌ°í ¿¬·á°¡ ÀÖ³Ä ?
	if ( this->Get_RideMODE() ) {
		if ( this->GetCur_MOVE_MODE() > MOVE_MODE_DRIVE ) {
			// ¾ò¾î Å¸°í ÀÖ´Â³ÑÀÌ´Ù.
			return true;
		}

		tagITEM *pItem = &m_Inventory.m_ItemRIDE[ RIDE_PART_ENGINE ];
		if ( pItem->GetLife() <=0 )
			return true;
	}

	int iDistance = distance ((int)m_PosCUR.x, (int)m_PosCUR.y, (int)pPacket->m_cli_MOUSECMD.m_PosTO.x, (int)pPacket->m_cli_MOUSECMD.m_PosTO.y);
	if ( iDistance > 1000 * 15 )	// 150 m //Numenor: if we want we can move further with one click ^^
		return true;

	this->m_nPosZ = pPacket->m_cli_MOUSECMD.m_nPosZ;

	this->SetCMD_MOVE( this->m_PosCUR, pPacket->m_cli_MOUSECMD.m_PosTO, pPacket->m_cli_MOUSECMD.m_wTargetObjectIDX );
	return true;
}

//-------------------------------------------------------------------------------------------------
/// Å¬¶óÀÌ¾ðÆ®°¡ Àå¾Ö¹°¿¡ ºÎµóÃÄ ´õÀÌ»ó ÀÌµ¿ ¸øÇÒ¶§...
bool classUSER::Recv_cli_CANTMOVE( t_PACKET *pPacket )
{
	int iCliDist = ::distance ((int)m_PosMoveSTART.x, (int)m_PosMoveSTART.y, (int)pPacket->m_cli_CANTMOVE.m_PosCUR.x, (int)pPacket->m_cli_CANTMOVE.m_PosCUR.y);
	if ( iCliDist > m_iMoveDistance ) {
		// ¼­¹ö¿¡ ¼³Á¤µÈ ÃÖÁ¾ À§Ä¡º¸´Ù ´õ ¸Ö¸® °¥¼ö ÀÖ³ª ????
		return true;// false;
	}

	int iSrvDist;
/*
	iSrvDist = ::distance ((int)m_PosMoveSTART.x, (int)m_PosMoveSTART.y, (int)m_PosCUR.x, (int)m_PosCUR.y);
	if ( iSrvDist >= iCliDist ) {
*/
	this->m_nPosZ = pPacket->m_cli_MOUSECMD.m_nPosZ;

	iSrvDist = ::distance ((int)m_PosGOTO.x, (int)m_PosGOTO.y, (int)m_PosCUR.x, (int)m_PosCUR.y);
	iCliDist = ::distance ((int)m_PosGOTO.x, (int)m_PosGOTO.y, (int)pPacket->m_cli_CANTMOVE.m_PosCUR.x, (int)pPacket->m_cli_CANTMOVE.m_PosCUR.y);
	if ( iCliDist > iSrvDist ) {
		// ÃÖÁ¾ ¸ñÀûÁö¿¡¼­ ´õ ¸Ö´Ù...
		this->m_PosGOTO = pPacket->m_cli_CANTMOVE.m_PosCUR;
		// Å¬¶óÀÌ¾ðÆ®¿¡¼­ Á¤Áö À§Ä¡·Î °­Á¦ ÀÌµ¿ ½ÃÅ²´Ù.
		this->m_PosCUR = pPacket->m_cli_CANTMOVE.m_PosCUR;
		// TODO:: update sector ???

		// ¼­¹ö°¡ Å¬¶óÀÌ¾ðÆ®¿¡¼­ ¸·Èù À§Ä¡ º¸´Ù ´õ ¸ÖÀÌ ÀÌµ¿Çß´Ù.
		// ´õÀÌ»ó ÀÌµ¿ ¸øÇÏµµ·Ï...
		m_iMoveDistance = -1;
		m_MoveVEC.x		= 0;
		m_MoveVEC.y		= 0;

		if ( !CObjAI::SetCMD_STOP () )
			return true;
	} else {
		// ÀÌµ¿ÇÒ ÃÖÁ¾ ÁÂÇ¥ ¼öÁ¤.
		m_iMoveDistance = iCliDist;
		// °ø°Ý ¸í·ÉÀÏ °æ¿ìµµ ÀÌµ¿ ¸í·ÉÈÄ Á¤Áö·Î ...
		if ( !CObjAI::SetCMD_MOVE( this->m_PosCUR, pPacket->m_cli_SETPOS.m_PosCUR, 0 ) )
			return true;
	}

	// º¸Á¤ ÁÂÇ¥ Àü¼Û.
	return this->Send_gsv_ADJUST_POS ();
}

//-------------------------------------------------------------------------------------------------
/// Å¬¶óÀÌ¾ðÆ®ÀÇ ¹«°Ô ºñÀ²ÀÌ º¯µ¿µÇ¾úÀ»°æ¿ì...
bool classUSER::Recv_cli_SET_WEIGHT_RATE( BYTE btWeightRate/*t_PACKET *pPacket*/, bool bSend2Around )
{
	this->m_btWeightRate = btWeightRate; /* pPacket->m_cli_SET_WEIGHT_RATE.m_btWeightRate; */

	if ( this->m_btWeightRate >= WEIGHT_RATE_WALK ) {
		// ¶Ù±â ºÒ´É.
		this->m_bRunMODE = false;

		// ´Ù½Ã ÀÌµ¿½Ã ÀÌµ¿¼Óµµ °è»êµÈ´Ù, ÇöÀç ÀÌµ¿ »óÅÂ´Â Áö¼Ó..
		//if ( this->m_btWeightRate >= WEIGHT_RATE_CANT_ATK ) {
		//	this->Del_ActiveSKILL ();
		//	CObjAI::SetCMD_STOP ();
		//}
		if ( this->GetCur_MOVE_MODE() > MOVE_MODE_RUN && this->GetOri_RunSPEED() > 300 ) {
			this->m_nRunSPEED = 300;
		}
	}

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_SET_WEIGHT_RATE;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_WEIGHT_RATE );
	pCPacket->m_gsv_SET_WEIGHT_RATE.m_wObjectIDX   = this->Get_INDEX();
	pCPacket->m_gsv_SET_WEIGHT_RATE.m_btWeightRate = this->m_btWeightRate;

	if ( bSend2Around ) {
		this->GetZONE()->SendPacketToSectors( this, pCPacket );
	} else {
		this->SendPacket( pCPacket );
	}
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}


//-------------------------------------------------------------------------------------------------
/// Å¬¶óÀÌ¾ðÆ®°¡ À§Ä¡ º¸Á¤À» ¿äÃ» ÇßÀ»¶§...
bool classUSER::Recv_cli_SETPOS( t_PACKET *pPacket )
{
	int iCliDist;

	iCliDist = ::distance ((int)m_PosMoveSTART.x, (int)m_PosMoveSTART.y, (int)pPacket->m_cli_SETPOS.m_PosCUR.x, (int)pPacket->m_cli_SETPOS.m_PosCUR.y);
	if ( iCliDist > m_iMoveDistance ) {
		// ¼­¹ö¿¡ ¼³Á¤µÈ ÃÖÁ¾ À§Ä¡º¸´Ù ´õ ¸Ö¸® °¥¼ö ÀÖ³ª ????
		// Â©¶ó¶ó !!!
		return false;
	}

	// ½ºÇÇµåÇÙÀÌ »ç¿ëµÇ°í ÀÖ´Â°¡ ???
	int iSrvDist = ::distance ((int)m_PosMoveSTART.x, (int)m_PosMoveSTART.y, (int)m_PosCUR.x, (int)m_PosCUR.y);
	if ( iCliDist >= iSrvDist+1000 ) {
		// iCliDist > iSrvDistº¸´Ù Å«°æ¿ì´Â ¼­¹ö¿¡ ºÎÇÏ°¡ ¹ß»ýÇÏ°Å³ª ½ºÇÁµåÇÙ...
		this->Send_gsv_ADJUST_POS ();
	} else
	if ( iSrvDist >= iCliDist+1000 ) {
		// ³×Æ®¿÷ ·¢ÀÌ ¹ß»ýÇÑ °æ¿ì´Â iSrvDist > iCliDist ...
		// ¼­¹ö°¡ Å¬¶óÀÌ¾ðÆ®º¸´Ù 10m ´õ °¬´Ù.
		this->Send_gsv_ADJUST_POS ();
	}

	return true;
}


//-------------------------------------------------------------------------------------------------
/// ¾ó±¼, ¸Ó¸®ÅÐ, ¼ºº° º¯È­µî ÁÖÀ§¿¡ Åëº¸~
bool classUSER::Send_gsv_CHANGE_SKIN( WORD wAbilityTYPE, int iValue )
{
	classPACKET *pCPacket = Packet_AllocNLock ();

	pCPacket->m_HEADER.m_wType = GSV_CHANGE_SKIN;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_CHANGE_SKIN );

	pCPacket->m_gsv_CHANGE_SKIN.m_wObjectIDX	= this->Get_INDEX();
	pCPacket->m_gsv_CHANGE_SKIN.m_wAbilityTYPE  = wAbilityTYPE;
	pCPacket->m_gsv_CHANGE_SKIN.m_iValue		= iValue;

	this->GetZONE()->SendPacketToSectors( this, pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// When the item is changed...
bool classUSER::Send_gsv_EQUIP_ITEM (short nEquipInvIDX, tagITEM *pEquipITEM)
{
	

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_EQUIP_ITEM;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_EQUIP_ITEM );
	pCPacket->m_gsv_EQUIP_ITEM.m_wObjectIDX  = this->Get_INDEX();
	pCPacket->m_gsv_EQUIP_ITEM.m_nEquipIndex = nEquipInvIDX;
	
	pCPacket->m_gsv_EQUIP_ITEM.m_PartITEM.m_nItemNo	  = pEquipITEM->GetItemNO();
	pCPacket->m_gsv_EQUIP_ITEM.m_PartITEM.m_nGEM_OP	  = pEquipITEM->GetGemNO();
	pCPacket->m_gsv_EQUIP_ITEM.m_PartITEM.m_cGrade	  = pEquipITEM->GetGrade();
	pCPacket->m_gsv_EQUIP_ITEM.m_PartITEM.m_bHasSocket= pEquipITEM->m_bHasSocket;
	
	if ( 0 == this->GetCur_RIDE_MODE() ) {
		pCPacket->m_HEADER.m_nSize += sizeof( short );
		pCPacket->m_gsv_EQUIP_ITEM.m_nRunSPEED[0] = this->GetOri_RunSPEED();	// Only by passive Calibration / sustained correction Excluded value.
	}
	
	this->GetZONE()->SendPacketToSectors( this, pCPacket );
    Packet_ReleaseNUnlock( pCPacket );
	return true;
}
//-------------------------------------------------------------------------------------------------
/// Change mounting item
bool classUSER::Change_EQUIP_ITEM (short nEquipInvIDX, short nWeaponInvIDX)
{
	if ( nEquipInvIDX < 1 || nEquipInvIDX >= MAX_EQUIP_IDX )
		return false;

	// Do not lower your two-handed weapon when you wear a left-hand shield when you are wearing a two-handed weapon.
	// The equipment removal ...
	if ( EQUIP_IDX_WEAPON_L == nEquipInvIDX && nWeaponInvIDX ) {
		if ( m_Inventory.m_ItemLIST[ EQUIP_IDX_WEAPON_R ].IsTwoHands() ) {
			// Sometimes clients do not get filtered ...
			return true;
		}
	}

	BYTE btCurCON = this->GetCur_CON();
	BYTE btRecvHP = this->m_btRecoverHP;
	BYTE btRecvMP = this->m_btRecoverMP;

	bool bResult = true;
	// Transfer changed inventory structure ..
	classPACKET *pCPacket = Packet_AllocNLock ();

	pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT = 0;
	tagITEM *pEquipITEM = &m_Inventory.m_ItemLIST[ nEquipInvIDX ];
	if ( nWeaponInvIDX == 0 ) {
		// Removing equipment ...
		if ( pEquipITEM->GetTYPE() && pEquipITEM->GetTYPE() < ITEM_TYPE_USE ) {
			short nInvIDX = this->Add_ITEM( *pEquipITEM  );
			if ( nInvIDX > 0 ) {
				pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT = 2;

				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_btInvIDX = (BYTE)nEquipInvIDX;
				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_ITEM.Clear();

				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_btInvIDX =  (BYTE)nInvIDX;
				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_ITEM     = *pEquipITEM;

				// * PEquipITEM is cleared when invoked under the Order Notes.
				this->ClearITEM( nEquipInvIDX );

				if ( EQUIP_IDX_WEAPON_R == nEquipInvIDX ) {
					// Disarm wastes from right hand weapon ...
					this->m_pShotITEM = NULL;
				}
			} else {
				// I can not take my equipment off because I do not have empty inventory ...
				goto _RETURN;
			}
		} else {
			#pragma COMPILE_TIME_MSG( "2004. 7. 16 A phenomenon that appears to some users in the fourth stage of Cube ..." )
			// IS_HACKING( this, "Change_EQUIP_ITEM-1" );	// ¹¹³Ä ???
			// bResult = false;
			goto _RETURN;
		}
	} else {
		// Equipped with equipment ...
		tagITEM WeaponITEM = m_Inventory.m_ItemLIST[ nWeaponInvIDX ];
		if ( this->Check_EquipCondition( WeaponITEM, nEquipInvIDX ) && WeaponITEM.IsEquipITEM() ) {
			if ( WeaponITEM.IsTwoHands() && m_Inventory.m_ItemEQUIP[ EQUIP_IDX_WEAPON_L ].GetTYPE() ) {
				// If you have both hands and you have a shield on your left hand
				// You must have one empty space in your equipment inventory to remove your left hand weapon.
				short nEmptyInvIDX = m_Inventory.GetEmptyInventory( INV_WEAPON );
				if ( nEmptyInvIDX < 0 )
					goto _RETURN;

				// Left hand weapon inventory ...
				tagITEM *pSubWPN = &m_Inventory.m_ItemLIST[ EQUIP_IDX_WEAPON_L ];
				m_Inventory.m_ItemLIST[ nEmptyInvIDX ] = *pSubWPN;

				// Delete left-handed item ..
				pSubWPN->Clear();
				this->SetPartITEM( BODY_PART_WEAPON_L, *pSubWPN );		// Left hand model item force delete !!!

				pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT = 4;

				// Delete left hand weapon ...
				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 2 ].m_btInvIDX = EQUIP_IDX_WEAPON_L;
				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 2 ].m_ITEM.Clear();

				// Transfer of inventory information into the equipment.
				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 3 ].m_btInvIDX = (BYTE)nEmptyInvIDX;
				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 3 ].m_ITEM     = m_Inventory.m_ItemLIST[ nEmptyInvIDX ];

				// Because it is equipped with two-handed weapons :: Increase armor, increase armor, remove shield daisy ...
				this->m_IngSTATUS.ClearSTATUS( ING_INC_DPOWER );
				this->m_IngSTATUS.ClearSTATUS( ING_INC_RES );
				this->m_IngSTATUS.ClearSTATUS( ING_SHIELD_DAMAGE );
			} else {
				// General Equipment & One Hand Weapon
				pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT = 2;
			}

			m_Inventory.m_ItemLIST[ nWeaponInvIDX ] = *pEquipITEM;
			*pEquipITEM = WeaponITEM;

			// Equipment exchange.
			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_btInvIDX = (BYTE)nEquipInvIDX;
			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_ITEM     = m_Inventory.m_ItemLIST[ nEquipInvIDX ];

			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_btInvIDX = (BYTE)nWeaponInvIDX;
			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_ITEM     = m_Inventory.m_ItemLIST[ nWeaponInvIDX ];

			if ( EQUIP_IDX_WEAPON_R == nEquipInvIDX ) {
				// One of the right-handed weapons is consumed?
				this->Set_ShotITEM ();			// classUSER::Change_EQUIP_ITEM 
			}
		} // else It is not an equipment item ...
	}


	if ( pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT ) {
		// When changing weapons
		switch( nEquipInvIDX ) {
			case EQUIP_IDX_WEAPON_R :
				// Increases attack power, increases hit rate, increases attack speed, increases critical damage ...
				this->m_IngSTATUS.ClearStatusIfENABLED( ING_INC_APOWER );
				this->m_IngSTATUS.ClearStatusIfENABLED( ING_INC_HIT );
				this->m_IngSTATUS.ClearStatusIfENABLED( ING_INC_ATK_SPD );
				this->m_IngSTATUS.ClearStatusIfENABLED( ING_INC_CRITICAL );
				break;

			case EQUIP_IDX_WEAPON_L :
				// Increase armor, increase armor, remove shield daisy ...
				this->m_IngSTATUS.ClearStatusIfENABLED( ING_INC_DPOWER );
				this->m_IngSTATUS.ClearStatusIfENABLED( ING_INC_RES );
				this->m_IngSTATUS.ClearStatusIfENABLED( ING_SHIELD_DAMAGE );
				break;
		}

		// Update the character status .. parts do not change ..
		this->SetPartITEM( nEquipInvIDX );

		pCPacket->m_HEADER.m_wType = GSV_SET_INV_ONLY;
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_INV_ONLY ) + pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT * sizeof( tag_SET_INVITEM );
		this->SendPacket( pCPacket );

		// Notice the weapon changes around ...
		this->Send_gsv_EQUIP_ITEM( nEquipInvIDX, pEquipITEM );

		if ( this->GetPARTY() ) {
			// If the option is attached by a change and the recovery is changed, it is sent to the party member.
			if ( btCurCON != this->GetCur_CON()  ||
				 btRecvHP != this->m_btRecoverHP ||
				 btRecvMP != this->m_btRecoverMP ) {
				this->m_pPartyBUFF->Change_ObjectIDX( this );
			}
		}
	}

_RETURN :
    Packet_ReleaseNUnlock( pCPacket );
	this->InitPassiveSkill ();

	return bResult;
}


/// ÀåÂø ¾ÆÀÌÅÛ º¯°æ ¿äÃ» ÆÐÅ¶Ã³¸®
bool classUSER::Recv_cli_EQUIP_ITEM( t_PACKET *pPacket )
{
	if ( this->Get_ActiveSKILL() ) return true;	// ½ºÅ³ ÄÉ½ºÆÃ ÁßÀÏ¶© ¸ø¹Ù²ã
	if ( this->m_IngSTATUS.IsSET( FLAG_ING_IGNORE_ALL ) ) return true;

	if ( pPacket->m_cli_EQUIP_ITEM.m_nEquipInvIDX < 1 ||
		 pPacket->m_cli_EQUIP_ITEM.m_nEquipInvIDX >= MAX_EQUIP_IDX ) {
		return IS_HACKING (this, "Recv_cli_EQUIP_ITEM-1 :: Invalied Equip IDX" ); 
	}
	if ( pPacket->m_cli_EQUIP_ITEM.m_nWeaponInvIDX && 
		(pPacket->m_cli_EQUIP_ITEM.m_nWeaponInvIDX  < MAX_EQUIP_IDX ||
		 pPacket->m_cli_EQUIP_ITEM.m_nWeaponInvIDX >= MAX_EQUIP_IDX+INVENTORY_PAGE_SIZE*(1+INV_WEAPON)) ) {
		return IS_HACKING (this, "Recv_cli_EQUIP_ITEM-2 :: Invalid Weapon Inv IDX" ); 
	}

	return this->Change_EQUIP_ITEM( pPacket->m_cli_EQUIP_ITEM.m_nEquipInvIDX, pPacket->m_cli_EQUIP_ITEM.m_nWeaponInvIDX );
}

//-------------------------------------------------------------------------------------------------
#define	PAT_ITEM_INV0	( MAX_EQUIP_IDX + INV_RIDING*INVENTORY_PAGE_SIZE )
/// Ä«Æ®/Ä³½½±â¾î ¾ÆÀÌÅÛ º¯°æ ¿äÃ»
bool classUSER::Recv_cli_ASSEMBLE_RIDE_ITEM( t_PACKET *pPacket )
{
	if ( this->Get_ActiveSKILL() ) return true;	// ½ºÅ³ ÄÉ½ºÆÃ ÁßÀÏ¶© ¸ø¹Ù²ã
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;

	short nEquipInvIDX = pPacket->m_cli_ASSEMBLE_RIDE_ITEM.m_nRidingPartIDX;
	if ( nEquipInvIDX < 0 || nEquipInvIDX >= MAX_RIDING_PART )
		return false;

	short nFromInvIDX  = pPacket->m_cli_ASSEMBLE_RIDE_ITEM.m_nRidingInvIDX;
	if ( nFromInvIDX && ( nFromInvIDX < PAT_ITEM_INV0 || nFromInvIDX >= PAT_ITEM_INV0+INVENTORY_PAGE_SIZE ) )
		return false;

	// ÀÌ¸§		: ±èÃ¢¼ö
	// ³¯Â¥		: Sep. 27 2005 				
	// ¼³¸í		: Ä«Æ®°ÔÀÌÁö°¡ 0ÀÏ¶§ PATÀåºñ Å»Âø ºÒ°¡
#ifdef __KCHS_BATTLECART__
	if( GetCur_PatHP() <= 0 && GetCur_PatMaxHP() > 0 ) {
		return true;
	}
#endif

	bool bResult = true;
	// ¹Ù²ï ÀÎº¥Åä¸® ±¸Á¶ Àü¼Û..
	classPACKET *pCPacket = Packet_AllocNLock ();

	pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT = 0;
	tagITEM *pEquipITEM = &m_Inventory.m_ItemRIDE[ nEquipInvIDX ];
	if ( nFromInvIDX == 0 ) {
		// ¾ÆÀÌÅÛ Å»°Å
		// ½ÂÂ÷ ¸ðµå¿¡¼± ÇÒ¼ö ¾ø´Ù.
		if ( this->GetCur_MOVE_MODE() == MOVE_MODE_DRIVE ) return true;

		if ( ITEM_TYPE_RIDE_PART == pEquipITEM->GetTYPE() ) {
			short nInvIDX = this->Add_ITEM( *pEquipITEM  );
			if ( nInvIDX > 0 ) {
				pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT = 2;

				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_btInvIDX = (BYTE)( INVENTORY_RIDE_ITEM0 + nEquipInvIDX );
				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_ITEM.Clear();

				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_btInvIDX = (BYTE)nInvIDX;
				pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_ITEM     = *pEquipITEM;

				// ¼ø¼­ ÁÖÀÇ ¹Ø¿¡²¨ È£ÃâµÇ¸é *pEquipITEMÀÌ »èÁ¦µÈ´Ù.
				this->ClearITEM( INVENTORY_RIDE_ITEM0+nEquipInvIDX );
			} else {
				// ºó ÀÎº¥Åä¸®°¡ ¾ø¾î¼­ Àåºñ¸¦ ¹þÀ»¼ö ¾ø´Ù...
				goto _RETURN;
			}
		} // else {
		//	IS_HACKING( this, "Recv_cli_ASSEMBLE_RIDE_ITEM-1" );	// ¹¹³Ä ???
		//	bResult = false;
		//	goto _RETURN;
		//}
	} else {
		// ¾ÆÀÌÅÛ ÀåÂø...
		tagITEM RideITEM = m_Inventory.m_ItemLIST[ nFromInvIDX ];
		if ( this->Check_PatEquipCondition( RideITEM, nEquipInvIDX ) ) {
			pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT = 2;

			m_Inventory.m_ItemLIST[ nFromInvIDX ] = *pEquipITEM;
			*pEquipITEM = RideITEM;

			// Àåºñ ±³È¯.
			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_btInvIDX = (BYTE)( INVENTORY_RIDE_ITEM0 + nEquipInvIDX );
			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_ITEM     = m_Inventory.m_ItemRIDE[ nEquipInvIDX ];

			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_btInvIDX = (BYTE)nFromInvIDX;
			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_ITEM     = m_Inventory.m_ItemLIST[ nFromInvIDX ];

			this->UpdateAbility(); // ÀÌ°Å ¾È ÇÏ¸é.. Àåºñ ±³Ã¼ ÇÒ¶§ Ä«Æ®Max PatHP °è¼Ó 0ÀÌ±â ¶§¹®¿¡... 
#ifdef __KCHS_BATTLECART__
			if( GetCur_PatMaxHP() > 0 ) // ÄðÅ¸ÀÓÀÌ µ¹°Ô ÇÒ·Á¸é...
				Set_PatHP_MODE( 3 );
#endif
		} 
		/*
		else {
			IS_HACKING( this, "Recv_cli_ASSEMBLE_RIDE_ITEM-2" );	// ¹¹³Ä ???
			bResult = false;
			goto _RETURN;
		}
		*/
	}

	if ( pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT ) {
		pCPacket->m_HEADER.m_wType = GSV_SET_INV_ONLY;
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_INV_ONLY ) + pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT * sizeof( tag_SET_INVITEM );
		this->SendPacket( pCPacket );
		Packet_ReleaseNUnlock( pCPacket );

		this->SetRideITEM( nEquipInvIDX );

		// ÁÖº¯¿¡ ¹Ù²ñÀ» Åëº¸...
		pCPacket = Packet_AllocNLock ();

		pCPacket->m_HEADER.m_wType = GSV_ASSEMBLE_RIDE_ITEM;
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_ASSEMBLE_RIDE_ITEM );

		pCPacket->m_gsv_ASSEMBLE_RIDE_ITEM.m_wObjectIDX		= this->Get_INDEX();
		pCPacket->m_gsv_ASSEMBLE_RIDE_ITEM.m_nRidingPartIDX = nEquipInvIDX;
	//  pCPacket->m_gsv_ASSEMBLE_RIDE_ITEM.m_nItemNO		= this->m_nRideItemIDX[ nEquipInvIDX ];
		pCPacket->m_gsv_ASSEMBLE_RIDE_ITEM.m_RideITEM		= this->m_RideITEM[ nEquipInvIDX ];

		if ( this->GetCur_RIDE_MODE() ) {
			pCPacket->m_HEADER.m_nSize += sizeof( short );
			pCPacket->m_gsv_ASSEMBLE_RIDE_ITEM.m_nRunSPEED[0] = this->GetOri_RunSPEED();	// ÆÐ½Ãºê¿¡ÀÇÇØ¼­¸¸ º¸Á¤/ Áö¼Óº¸Á¤ Á¦¿ÜµÈ°ª.
		} 

		this->GetZONE()->SendPacketToSectors( this, pCPacket );
	}

_RETURN :
    Packet_ReleaseNUnlock( pCPacket );

	return bResult;
}

//-------------------------------------------------------------------------------------------------
/// ÀÌµ¿¼Óµµ º¯°æÀ» ÁÖº¯¿¡ Åëº¸
bool classUSER::Send_gsv_SPEED_CHANGED (bool bUpdateSpeed)
{
	// ¼Óµµ °»½Å...
	if ( bUpdateSpeed )
		this->Update_SPEED ();

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_SPEED_CHANGED;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SPEED_CHANGED );

	pCPacket->m_gsv_SPEED_CHANGED.m_wObjectIDX   = this->Get_INDEX ();
	pCPacket->m_gsv_SPEED_CHANGED.m_nRunSPEED	 = this->GetOri_RunSPEED ();
	pCPacket->m_gsv_SPEED_CHANGED.m_nPsvAtkSPEED = this->GetPsv_ATKSPEED ();
	pCPacket->m_gsv_SPEED_CHANGED.m_btWeightRate = this->m_btWeightRate;

	this->GetZONE()->SendPacketToSectors( this, pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	// ÆÄÆ¼¿ø ÇÑÅ×´Â max hp/con/recover_xx ... !!!
	if ( this->GetPARTY() ) {
		this->m_pPartyBUFF->Change_ObjectIDX( this );
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
/// NPC »óÁ¡ °Å·¡ÀÀ´ä ÆÐÅ¶
bool classUSER::Send_gsv_STORE_TRADE_REPLY( BYTE btResult )
{
	classPACKET *pCPacket = Packet_AllocNLock ();

	pCPacket->m_HEADER.m_wType = GSV_STORE_TRADE_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_STORE_TRADE_REPLY );

	pCPacket->m_gsv_STORE_TRADE_REPLY.m_btRESULT = btResult;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
#define	MAX_TRADE_DISTANCE		( 1000 * 6 )	// 6 M
/// NPC »óÁ¡ °Å·¡ ¿äÃ» ÆÐÅ¶
bool classUSER::Recv_cli_STORE_TRADE_REQ( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;

	if ( NULL == this->GetZONE() )
		return false;

	if ( pPacket->m_cli_STORE_TRADE_REQ.m_dwEconomyTIME != this->GetZONE()->GetEconomyTIME() ) {
		// °æÁ¦ µ¥ÀÌÅ¸°¡ Æ²¸®´Ù...
		return this->Send_gsv_STORE_TRADE_REPLY( STORE_TRADE_RESULT_PRICE_DIFF );
	}

	CObjNPC *pCharNPC;
	pCharNPC = (CObjNPC*)g_pObjMGR->Get_GameOBJ( pPacket->m_cli_STORE_TRADE_REQ.m_wNPCObjIdx, OBJ_NPC );
	if ( !pCharNPC ) {
		// »óÁ¡ ÁÖÀÎÀÌ ¾ø¾î???
		return this->Send_gsv_STORE_TRADE_REPLY( STORE_TRADE_RESULT_NPC_NOT_FOUND );
	}

	BYTE btUnionIDX = NPC_UNION_NO( pCharNPC->Get_CharNO() );
	if ( btUnionIDX && btUnionIDX != this->GetCur_JOHAP() ) {	
		// Á¶ÇÕ »óÁ¡ÀÎµ¥ ¼Ò¼Ó Á¶ÇÕÀÌ Æ²¸®¸é...
		return this->Send_gsv_STORE_TRADE_REPLY( STORE_TRADE_RESULT_NOT_UNION_USER );
	}

	// »óÁ¡ npc¿ÍÀÇ °Å·¡ Ã¼Å©...
	int iDistance = distance ((int)m_PosCUR.x, (int)m_PosCUR.y, (int)pCharNPC->m_PosCUR.x, (int)pCharNPC->m_PosCUR.y);
	if ( iDistance > MAX_TRADE_DISTANCE )	{ 
		return this->Send_gsv_STORE_TRADE_REPLY( STORE_TRADE_RESULT_TOO_FAR );
	}

	short nOffset=sizeof( cli_STORE_TRADE_REQ );

	tag_BUY_ITEM  *pBuyITEMs;
	tag_SELL_ITEM *pSellITEMs;

	if ( pPacket->m_cli_STORE_TRADE_REQ.m_cBuyCNT > 0 ) {
		pBuyITEMs  = (tag_BUY_ITEM*) Packet_GetDataPtr( pPacket, nOffset, sizeof( tag_BUY_ITEM  ) * pPacket->m_cli_STORE_TRADE_REQ.m_cBuyCNT  );
		if ( !pBuyITEMs )
			return IS_HACKING( this, "Recv_cli_STORE_TRADE_REQ-1 :: Invalid Buy Items" );
	}

    classPACKET *pCPacket = Packet_AllocNLock ();

	pCPacket->m_HEADER.m_wType = GSV_SET_MONEYnINV;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_MONEYnINV );
	pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT = 0;

	bool bResult = true;
	int iPriceEA, iTotValue;
	short nC;
	if ( pPacket->m_cli_STORE_TRADE_REQ.m_cSellCNT > 0 ) {
		pSellITEMs = (tag_SELL_ITEM*)Packet_GetDataPtr( pPacket, nOffset, sizeof( tag_SELL_ITEM ) * pPacket->m_cli_STORE_TRADE_REQ.m_cSellCNT );

		// ¹°°Ç ÆÈÀÚ !!!
		if ( !pSellITEMs ) {
			IS_HACKING( this, "Recv_cli_STORE_TRADE_REQ-2 :: Invalid Sell Items" );
			bResult = false;
			goto _RETURN;
		}

		tagITEM *pITEM;
		for (nC=0; nC<pPacket->m_cli_STORE_TRADE_REQ.m_cSellCNT; nC++) {
			if ( pSellITEMs[ nC ].m_btInvIDX >= INVENTORY_TOTAL_SIZE ) {
				IS_HACKING( this, "Recv_cli_STORE_TRADE_REQ-2-2 :: Invalid inventory index" );
				bResult = false;
				goto _RETURN;
			}

			pITEM   = &m_Inventory.m_ItemLIST[ pSellITEMs[ nC ].m_btInvIDX ];
			if ( !pITEM->IsEnableSELL() ) {
				// »óÁ¡ ÆÇ¸Å ºÒ°¡ ¾ÆÀÌÅÛÀÌ´Ù.
				if ( pITEM->GetHEADER() ) {
					IS_HACKING( this, "Recv_cli_STORE_TRADE_REQ-3 :: Cant sell to store" );
					bResult = false;
				}
				goto _RETURN;
			}

			iPriceEA = this->GetZONE()->Get_ItemSellPRICE( *pITEM, this->GetSellSkillVALUE() ) ;

			// ÆÇ¸ÅÇÑ ¾ÆÀÌÅÛ ·Î±×¸¦ ³²±è...
			if ( pITEM->IsEnableDupCNT() ) {
				if ( pSellITEMs[ nC ].m_wDupCNT >= pITEM->GetQuantity() ) {
					this->GetZONE()->SellITEMs( pITEM, pITEM->GetQuantity() );

					iTotValue = iPriceEA * pITEM->GetQuantity();

					#ifdef	__NEW_LOG
						g_pThreadLOG->When_TagItemLOG( LIA_SELL2NPC, this, pITEM, pITEM->GetQuantity(), iTotValue );
					#else
						g_pThreadLOG->When_NpcTRADE( this, pITEM, pCharNPC, "Sell2NPC", iTotValue, pITEM->GetQuantity() );
					#endif

					this->ClearITEM( pSellITEMs[ nC ].m_btInvIDX );
				} else {
					this->GetZONE()->SellITEMs( pITEM, pSellITEMs[ nC ].m_wDupCNT );

					iTotValue = iPriceEA * pSellITEMs[ nC ].m_wDupCNT;
					#ifdef	__NEW_LOG
						g_pThreadLOG->When_TagItemLOG( LIA_SELL2NPC, this, pITEM, pSellITEMs[ nC ].m_wDupCNT, iTotValue );
					#else
						g_pThreadLOG->When_NpcTRADE( this, pITEM, pCharNPC, "Sell2NPC", iTotValue, pSellITEMs[ nC ].m_wDupCNT );
					#endif

					pITEM->m_uiQuantity -= pSellITEMs[ nC ].m_wDupCNT;
				}
			} else {
				iTotValue = iPriceEA;
				#ifdef	__NEW_LOG
					g_pThreadLOG->When_TagItemLOG( LIA_SELL2NPC, this, pITEM, 1, iTotValue );
				#else
					g_pThreadLOG->When_NpcTRADE( this, pITEM, pCharNPC, "Sell2NPC", iTotValue, 1 );
				#endif
				this->ClearITEM( pSellITEMs[ nC ].m_btInvIDX );
			}
			this->Add_CurMONEY( abs(iTotValue) );

			// º¯°æµÈ ÀÎº¥Åä¸® Á¤º¸ »ý¼º...
			pCPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_btInvIDX =  pSellITEMs[ nC ].m_btInvIDX;
			pCPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_ITEM	 = *pITEM;
			pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ++;
		}
	}

	short nInvIDX;
	tagITEM sBuyITEM;
	for (nC=0; nC<pPacket->m_cli_STORE_TRADE_REQ.m_cBuyCNT; nC++) {
		// ¹«°Ç ±¸ÀÔ !!!
		if ( !pCharNPC->Get_SellITEM( pBuyITEMs[ nC ].m_cTabNO, pBuyITEMs[ nC ].m_cColNO, sBuyITEM ) ) {
			break;
		}

		if ( btUnionIDX ) {
			// Á¶ÇÕ»óÁ¡...Á¶ÇÕ Æ÷ÀÎÆ®¸¦ ¼Ò¸ð...
			iPriceEA = ITEM_MAKE_DIFFICULT( sBuyITEM.m_cType, sBuyITEM.m_nItemNo );
			if ( tagITEM::IsEnableDupCNT( sBuyITEM.GetTYPE() ) ) {
				iTotValue = iPriceEA*pBuyITEMs[ nC ].m_wDupCNT;
				sBuyITEM.m_uiQuantity = pBuyITEMs[ nC ].m_wDupCNT;
			} else {
				iTotValue = iPriceEA;
			}

			if ( this->GetCur_JoHapPOINT( btUnionIDX ) < iTotValue ) {
				// Á¶ÇÕ Æ÷ÀÎÆ® ¾ø´Ù.
				this->Send_gsv_STORE_TRADE_REPLY( STORE_TRADE_RESULT_OUT_OF_POINT );
				break;
			}
		} else {
			iPriceEA = this->GetZONE()->Get_ItemBuyPRICE( sBuyITEM.m_cType, sBuyITEM.m_nItemNo, this->GetBuySkillVALUE() );
			if ( tagITEM::IsEnableDupCNT( sBuyITEM.GetTYPE() ) ) {
				iTotValue = iPriceEA*pBuyITEMs[ nC ].m_wDupCNT;
				sBuyITEM.m_uiQuantity = pBuyITEMs[ nC ].m_wDupCNT;
			} else {
				iTotValue = iPriceEA;
			}
			if ( this->GetCur_MONEY() < iTotValue ) {
				// µ· ¾ø´Ù.
				this->Send_gsv_STORE_TRADE_REPLY( STORE_TRADE_RESULT_OUT_OF_MONEY );
				break;
			}

			this->GetZONE()->BuyITEMs( sBuyITEM );
		}
		this->Set_ItemSN( sBuyITEM );	// »óÁ¡¿¡¼­ ±¸ÀÔ½Ã...
	
		// ¼Ò¸ð...
		if ( btUnionIDX ) {
			// ±¸ÀÔÇÑ ¾ÆÀÌÅÛ ·Î±×¸¦ ³²±è...
			#ifdef	__NEW_LOG
				g_pThreadLOG->When_TagItemLOG( LIA_BUYFROMUNION, this, &sBuyITEM, pBuyITEMs[ nC ].m_wDupCNT, iTotValue, NULL, true );
			#else
				g_pThreadLOG->When_NpcTRADE( this, &sBuyITEM, pCharNPC, "BuyFromUNION", iTotValue, pBuyITEMs[ nC ].m_wDupCNT );
			#endif
			this->SubCur_JoHapPOINT( btUnionIDX, iTotValue );
		} else {
			// ±¸ÀÔÇÑ ¾ÆÀÌÅÛ ·Î±×¸¦ ³²±è...
			#ifdef	__NEW_LOG
				g_pThreadLOG->When_TagItemLOG( LIA_BUYFROMNPC, this, &sBuyITEM, pBuyITEMs[ nC ].m_wDupCNT, iTotValue );
			#else
				g_pThreadLOG->When_NpcTRADE( this, &sBuyITEM, pCharNPC, "BuyFromNPC", iTotValue, pBuyITEMs[ nC ].m_wDupCNT );
			#endif
			this->Sub_CurMONEY( iTotValue ) ;
		}

		nInvIDX = this->Add_ITEM( sBuyITEM );
		if ( nInvIDX > 0 ) {
			pCPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_btInvIDX = (BYTE)nInvIDX;
			pCPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_ITEM	 = m_Inventory.m_ItemLIST[ nInvIDX ];
			pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ++;
		} else {
			this->Save_ItemToFILED( sBuyITEM );	// »óÁ¡ ±¸ÀÔÈÄ ÀÎº¥Åä¸®°¡ Ãß°¡°¡ ºÒ°¡´ÉÇØ¼­ ¹Û¿¡ 30ºÐ°£ º¸°ü...
		}
	}

	pCPacket->m_HEADER.m_nSize += ( pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT * sizeof( tag_SET_INVITEM ) );
	pCPacket->m_gsv_SET_MONEYnINV.m_i64Money = this->GetCur_MONEY();

    this->SendPacket( pCPacket );

_RETURN :
    Packet_ReleaseNUnlock( pCPacket );

	return bResult;
}


//-------------------------------------------------------------------------------------------------
/// ´ÜÃà¾ÆÀÌÄÜ ¼³Á¤ ÆÐÅ¶
bool classUSER::Recv_cli_SET_HOTICON( t_PACKET *pPacket )
{
	if ( pPacket->m_cli_SET_HOTICON.m_btListIDX >= MAX_HOT_ICONS )
		return false;

	if ( this->m_HotICONS.RegHotICON( pPacket->m_cli_SET_HOTICON.m_btListIDX, pPacket->m_cli_SET_HOTICON.m_HotICON ) ) {
        classPACKET *pCPacket = Packet_AllocNLock ();

        pCPacket->m_HEADER.m_wType = GSV_SET_HOTICON;
        pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_HOTICON );
        pCPacket->m_gsv_SET_HOTICON.m_btListIDX = pPacket->m_cli_SET_HOTICON.m_btListIDX;
        pCPacket->m_gsv_SET_HOTICON.m_HotICON   = this->m_HotICONS.m_IconLIST[ pPacket->m_cli_SET_HOTICON.m_btListIDX ];

        this->SendPacket( pCPacket );
        Packet_ReleaseNUnlock( pCPacket );

		return true;
	}

	return false;
}

//-------------------------------------------------------------------------------------------------
/// Å¬¶óÀÌ¾ðÆ®¿ÍÀÇ µ¿±âÈ­¸¦ ¸ÂÃß±â À§ÇØ ÇöÀç ¼³Á¤µÈ ³²Àº ÃÑ¾ËÀÇ °¹¼ö Àü¼Û
bool classUSER::Send_BulltCOUNT ()
{
	tagITEM *pShotITEM = &this->m_Inventory.m_ItemSHOT[ this->m_btShotTYPE ];
	BYTE btInvIDX = INVENTORY_SHOT_ITEM0+this->m_btShotTYPE;

	return this->Send_gsv_SET_INV_ONLY (btInvIDX, pShotITEM);
}

/// ÃÑ¾Ë Å¸ÀÔ º¯°æ Àü¼Û
bool classUSER::Send_gsv_SET_BULLET( BYTE btShotTYPE )
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_SET_BULLET;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_BULLET );

	pCPacket->m_gsv_SET_BULLET.m_wObjectIDX = this->Get_INDEX();

	pCPacket->m_gsv_SET_BULLET.m_sShot.m_cType   = btShotTYPE;
	if ( this->m_Inventory.m_ItemSHOT[ btShotTYPE ].GetQuantity() > 0 ) {
		pCPacket->m_gsv_SET_BULLET.m_sShot.m_nItemNo = this->m_Inventory.m_ItemSHOT[ btShotTYPE ].GetItemNO();
	} else {
		pCPacket->m_gsv_SET_BULLET.m_sShot.m_nItemNo = 0;
	}

	this->GetZONE()->SendPacketToSectors( this, pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

/// ÃÑ¾Ë º¯°æ ¿äÃ» ÆÐÅ¶
bool classUSER::Recv_cli_SET_BULLET( t_PACKET *pPacket )
{
	if ( this->Get_ActiveSKILL() ) return true;	// ½ºÅ³ ÄÉ½ºÆÃ ÁßÀÏ¶© ¸ø¹Ù²ã
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;

    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	bool bResult = true;
	if ( pPacket->m_cli_SET_BULLET.m_wInventoryIDX ) {
		// ±³È¯ ? ÀåÂø ? Ãß°¡ ?
		tagITEM *pInvITEM  = &m_Inventory.m_ItemLIST[ pPacket->m_cli_SET_BULLET.m_wInventoryIDX ];
		if ( 0 == pInvITEM->GetHEADER() ) {
			goto _RETURN;
		}
		// ÀåÂø À§Ä¡°¡ ¸Â´Â ¼Ò¸ðÅºÀÎ°¡ ???
		if ( !pInvITEM->IsEtcITEM() ) {
			IS_HACKING( this, "Invalid bullet position" );
			bResult = false;
			goto _RETURN;
		}

		switch( pPacket->m_cli_SET_BULLET.m_wShotTYPE ) {
			case SHOT_TYPE_ARROW  :
				if ( 431 != ITEM_TYPE( pInvITEM->GetTYPE(), pInvITEM->GetItemNO() ) ) {
					IS_HACKING( this, "bullet type must SHOT_TYPE_ARROW" );
					bResult = false;
					goto _RETURN;
				}
				break;

			case SHOT_TYPE_BULLET :
				if ( 432 != ITEM_TYPE( pInvITEM->GetTYPE(), pInvITEM->GetItemNO() ) ) {
					IS_HACKING( this, "bullet type must SHOT_TYPE_BULLET" );
					bResult = false;
					goto _RETURN;
				}
				break;;

			case SHOT_TYPE_THROW  :
				switch( ITEM_TYPE( pInvITEM->GetTYPE(), pInvITEM->GetItemNO() ) ) {
					case 421 :	case 422 :	case 423 :	case 433 :
						break;
					default :
						IS_HACKING( this, "bullet type must SHOT_TYPE_THROW" );
						bResult = false;
						goto _RETURN;
				}
		}

		tagITEM *pShotITEM = &m_Inventory.m_ItemSHOT[ pPacket->m_cli_SET_BULLET.m_wShotTYPE ];

		if ( pShotITEM->GetHEADER() == pInvITEM->GetHEADER() ) {
			// °°Àº ¾ÆÀÌÅÛÀº °¹¼ö¸¦ ´õÇÑ´Ù.
			if ( pShotITEM->GetQuantity() + pInvITEM->GetQuantity() > MAX_DUP_ITEM_QUANTITY ) {
				// ´ú¾î ³õÀÚ...
				pInvITEM->m_uiQuantity -= (MAX_DUP_ITEM_QUANTITY - pShotITEM->GetQuantity() );
				pShotITEM->m_uiQuantity = MAX_DUP_ITEM_QUANTITY;
			} else {
				// ´õÇÏ°í »èÁ¦.
				pShotITEM->m_uiQuantity += pInvITEM->GetQuantity();
				this->ClearITEM( pPacket->m_cli_SET_BULLET.m_wInventoryIDX );
			}
		} else {
			// ±³Ã¼...
			tagITEM sTmpItem;
			sTmpItem = *pShotITEM;
			*pShotITEM = *pInvITEM;
			*pInvITEM = sTmpItem;
		}

		pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_btInvIDX = INVENTORY_SHOT_ITEM0 + pPacket->m_cli_SET_BULLET.m_wShotTYPE;
		pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_ITEM     =*pShotITEM;

		pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_btInvIDX = pPacket->m_cli_SET_BULLET.m_wInventoryIDX;
		pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_ITEM     =*pInvITEM;
	} else {
		// Å»°Å...
		// Àåºñ Å»°Å...
		tagITEM *pShotITEM = &m_Inventory.m_ItemSHOT[ pPacket->m_cli_SET_BULLET.m_wShotTYPE ];

		if ( 0 == pShotITEM->GetHEADER() ) {
			// ¸ðµÎ ¼Ò¸ðµÆ´Âµ¥ Å¬¶óÀÌ¾ðÆ®¿¡¼­´Â ³²Àº°ÍÀ¸·Î ¿ÀÀÎ... »èÁ¦ÇÏ¶ó´Â ÆÐÅ¶ Àü¼Û.
			this->Send_gsv_SET_INV_ONLY( INVENTORY_SHOT_ITEM0 + pPacket->m_cli_SET_BULLET.m_wShotTYPE, pShotITEM );
			goto _RETURN;
		}

		short nInvIDX = this->Add_ITEM( *pShotITEM  );
		if ( nInvIDX > 0 ) {
			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_btInvIDX = INVENTORY_SHOT_ITEM0 + pPacket->m_cli_SET_BULLET.m_wShotTYPE;
			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 0 ].m_ITEM.Clear();

			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_btInvIDX =  (BYTE)nInvIDX;
			pCPacket->m_gsv_SET_INV_ONLY.m_sInvITEM[ 1 ].m_ITEM     = *pShotITEM;

			// ¼ø¼­ ÁÖÀÇ ¹Ø¿¡²¨ È£ÃâµÇ¸é *pEquipITEMÀÌ »èÁ¦µÈ´Ù.
			this->ClearITEM( INVENTORY_SHOT_ITEM0 + pPacket->m_cli_SET_BULLET.m_wShotTYPE );
		} else {
			// ºó ÀÎº¥Åä¸®°¡ ¾ø¾î¼­ ÃÑ¾Ë Å»°Å ¸øÇÔ...
			goto _RETURN;
		}
	}

	pCPacket->m_HEADER.m_wType = GSV_SET_INV_ONLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_INV_ONLY ) + 2 * sizeof( tag_SET_INVITEM );
	pCPacket->m_gsv_SET_INV_ONLY.m_btItemCNT = 2;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	{	// ·ÎÄÃ º¯¼ö ¼±¾ð...

		t_eSHOT eShotTYPE = this->m_Inventory.m_ItemEQUIP[ EQUIP_IDX_WEAPON_R ].GetShotTYPE();
		if ( eShotTYPE == this->m_btShotTYPE ) {
			// ¹Ù²ï ÃÑ¾ËÀÌ ÇöÀç ¹«±â¿Í ¿¬°ü µÇ´Â°¡?
			this->Set_ShotITEM ( eShotTYPE );				// classUSER::Recv_cli_SET_BULLET
	}

	return this->Send_gsv_SET_BULLET( pPacket->m_cli_SET_BULLET.m_wShotTYPE );
	}

_RETURN :
	Packet_ReleaseNUnlock( pCPacket );
	return bResult;
}


//-------------------------------------------------------------------------------------------------
/// ¾ÆÀÌÅÛ Á¦Á¶ °á°ú Åëº¸
bool classUSER::Send_gsv_CREATE_ITEM_REPLY (BYTE btResult, short nStepORInvIDX, float *pProcPOINT, tagITEM *pOutItem)
{
	classPACKET *pCPacket = Packet_AllocNLock ();

	pCPacket->m_HEADER.m_wType = GSV_CREATE_ITEM_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_CREATE_ITEM_REPLY );

	pCPacket->m_gsv_CREATE_ITEM_REPLY.m_btRESULT = btResult;
	pCPacket->m_gsv_CREATE_ITEM_REPLY.m_nStepORInvIDX = nStepORInvIDX;

	if ( NULL != pProcPOINT ) {
		for (short nI=0; nI<CREATE_ITEM_STEP; nI++)
			pCPacket->m_gsv_CREATE_ITEM_REPLY.m_nPRO_POINT[ nI ] = (short)pProcPOINT[ nI ];
	}
	if ( NULL != pOutItem ) {
		pCPacket->m_gsv_CREATE_ITEM_REPLY.m_CreatedITEM = *pOutItem;
	}

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}
/// ¾ÆÀÌÅÛ Á¦Á¶ ¿äÃ» ¹ÞÀ½
bool classUSER::Recv_cli_CREATE_ITEM_REQ( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;

	if ( pPacket->m_cli_CREATE_ITEM_REQ.m_btSkillSLOT >= MAX_LEARNED_SKILL_CNT )
		return false;
	if ( !tagITEM::IsValidITEM( pPacket->m_cli_CREATE_ITEM_REQ.m_cTargetItemTYPE, pPacket->m_cli_CREATE_ITEM_REQ.m_nTargetItemNO ) ) {
		// Hacking ???
		return true;
	}
	if ( this->m_nCreateItemEXP > 0 ) {
		// ¸ÞÅ©·Î³ª ÇØÅ·À¸·Î Á¦Á¶ ÆÐÅ¶ »ý¼ºÇØ¼­ º¸³¾¶§...
		return true;
	}
	this->m_nCreateItemEXP = 0;

	short nSkillIDX = this->m_Skills.m_nSkillINDEX[ pPacket->m_cli_CREATE_ITEM_REQ.m_btSkillSLOT ];

	// ½ºÅ³ÀÇ Á¦Á¶ ¹øÈ£¿Í ¸¸µé·Á´Â ¾ÆÀÌÅÛÀÇ Á¦Á¶¹øÈ£ ºñ±³...
	if ( SKILL_ITEM_MAKE_NUM( nSkillIDX ) != ITEM_MAKE_NUM( pPacket->m_cli_CREATE_ITEM_REQ.m_cTargetItemTYPE, pPacket->m_cli_CREATE_ITEM_REQ.m_nTargetItemNO ) ) {
		return true;
	}

	// ½ºÅ³ ·¹º§ ºñ±³...
	if ( SKILL_LEVEL( nSkillIDX ) < ITEM_SKILL_LEV( pPacket->m_cli_CREATE_ITEM_REQ.m_cTargetItemTYPE, pPacket->m_cli_CREATE_ITEM_REQ.m_nTargetItemNO ) ) {
		return IS_HACKING( this, "Recv_cli_CREATE_ITEM_REQ-2 :: Invalid SKILL_LEVEL");				// ERROR:: Å©°Å³ª °°¾Æ¾ß ÇÏ´Âµ¥ ???
	}

	// ½ºÅ³ »ç¿ë °¡´ÉÇÑ°¡ ???
	if ( !this->Skill_ActionCondition( nSkillIDX ) ) {
		return this->Send_gsv_CREATE_ITEM_REPLY( RESULT_CREATE_ITEM_INVALID_CONDITION, 0 );
	}

	// ½ÇÁ¦ ÇÊ¿ä ¼öÄ¡ ¼Ò¸ð Àû¿ë...
	this->Skill_UseAbilityValue( nSkillIDX );	// Recv_cli_CREATE_ITEM_REQ

	short nI, nProductIDX;
	// ¾ÆÀÌÅÛ Á¦Á¶¹øÈ£
	nProductIDX = ITEM_PRODUCT_IDX( pPacket->m_cli_CREATE_ITEM_REQ.m_cTargetItemTYPE, pPacket->m_cli_CREATE_ITEM_REQ.m_nTargetItemNO );

	short nInvIDX, nMAT_QUAL, nNUM_MAT, nPLUS;
	float fPRO_POINT[4], fSUC_POINT[4];
	tagITEM *pInvITEM, sOutITEM, sUsedITEM[4];

	// ¸¸µé·Á´Â ¾ÆÀÌÅÛÀÇ Á¦Á¶ ³­ÀÌµµ...
	short nITEM_DIF = ITEM_MAKE_DIFFICULT( pPacket->m_cli_CREATE_ITEM_REQ.m_cTargetItemTYPE, pPacket->m_cli_CREATE_ITEM_REQ.m_nTargetItemNO );
	// ¸¸µé·Á´Â ¾ÆÀÌÅÛÀÇ Ç°Áú ¼öÄ¡...
	short nITEM_QUAL = ITEM_QUALITY( pPacket->m_cli_CREATE_ITEM_REQ.m_cTargetItemTYPE, pPacket->m_cli_CREATE_ITEM_REQ.m_nTargetItemNO );
	// »ç¿ëµÇ´Â Àç·á °¹¼ö...
	for (nNUM_MAT=1, nI=1; nI<CREATE_ITEM_STEP; nI++) {
		if ( PRODUCT_NEED_ITEM_NO( nProductIDX, nI ) ) 
			nNUM_MAT ++;
	}

	// ÀçÁ¶ ½ÃÀÛÀ» ÁÖº¯¿¡ Åëº¸...
	this->Send_gsv_ITEM_RESULT_REPORT( REPORT_ITEM_CREATE_START,
									pPacket->m_cli_CREATE_ITEM_REQ.m_cTargetItemTYPE,
									pPacket->m_cli_CREATE_ITEM_REQ.m_nTargetItemNO );


	short nRand, nUsedCNT=0;
	// Check: ¸¸µé°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛ¿¡ ÇÊ¿äÇÑ Àç·áµéÀÇ Á¾·ù¿Í °¹¼ö°¡ ÇÕ´çÇÑ°¡ ÆÇ´Ü.
	for (nI=0; nI<CREATE_ITEM_STEP; nI++) {
		nInvIDX  = pPacket->m_cli_CREATE_ITEM_REQ.m_nUseItemINV[ nI ];
		pInvITEM = &this->m_Inventory.m_ItemLIST[ nInvIDX ];

		if ( 0 == PRODUCT_NEED_ITEM_NO( nProductIDX, nI ) ) {
			if ( 0 != nI ) {
				// ÇÊ¿äÇÑ Àç·á°¡ ¾ø´Ù.
				continue;
			}
			if ( 0 == pInvITEM->GetHEADER() ) {
				// ¸¶¿ì½º·Î ¸¶±¸ Å¬¸¯½Ã ¿©±â·Î µé¾î¿Ã¼ö ÀÖ´Ù..
				return this->Send_gsv_CREATE_ITEM_REPLY( RESULT_CREATE_ITEM_INVALID_ITEM, 0 );
			}

			// Ã¹¹øÂ° ¿ø·á Á¾·ù³ª, Àç·á ¾ÆÀÌÅÛ ºñ±³ ,,, µÑÁß ÇÏ³ª ..
			if ( PRODUCT_RAW_MATERIAL( nProductIDX ) ) {
				// ¿ø·á Á¾·ùÀÏ °æ¿ì...
				if ( PRODUCT_RAW_MATERIAL( nProductIDX ) != ITEM_TYPE( pInvITEM->GetTYPE(), pInvITEM->GetItemNO() ) ) {
					return this->Send_gsv_CREATE_ITEM_REPLY( RESULT_CREATE_ITEM_INVALID_ITEM, 0 );
				}
			} else {
				// LIST_PRODUCT.STB µ¥ÀÌÅ¸°¡ Àß¸ø ÀÔ·ÂµÇ¾î ÀÖ´Ù.
				return true;
			}
		} else {
			if ( 0 == pInvITEM->GetHEADER() ) {
				// Å¬¶óÀÌ¾ðÆ®¿¡¼­ ÀÌ°æ¿ì¿¡µµ ¿äÃ»ÇÑ´Ù.. ÀÌ¹Ì ¼Ò¸ðµÈ ¾ÆÀÌÅÛÀ» µÇµ¹¸±¼ö ¾ø´Â ¹®Á¦ !!!
				return this->Send_gsv_CREATE_ITEM_REPLY( RESULT_CREATE_ITEM_INVALID_ITEM, nI, fPRO_POINT );
			}
			sOutITEM.Init( PRODUCT_NEED_ITEM_NO( nProductIDX, nI ) );
			if ( pInvITEM->GetHEADER() != sOutITEM.GetHEADER() ) {
				return this->Send_gsv_CREATE_ITEM_REPLY( RESULT_CREATE_ITEM_INVALID_ITEM, nI, fPRO_POINT );
			}
		}

		// Àåºñ ¾ÆÀÌÅÛÀÌ ¾Æ´Ò °æ¿ì ÇÊ¿ä °¹¼ö ºñ±³...
		if ( pInvITEM->IsEnableDupCNT() ) {
			if ( pInvITEM->GetQuantity() < (WORD)PRODUCT_NEED_ITEM_CNT( nProductIDX, nI ) ) {
				return this->Send_gsv_CREATE_ITEM_REPLY( RESULT_CREATE_ITEM_NEED_ITEM, nI );
			}
		}

		nRand = RANDOM(100);
		switch( nI ) {
			case 0 :
			{
				// 1¹øÂ° ¿øÀç·á ¾ÆÀÌÅÛ Ç°Áú¿¡¼­ ¾òÀ½.
				nMAT_QUAL = ITEM_QUALITY( pInvITEM->GetTYPE(), pInvITEM->GetItemNO() );

				fSUC_POINT[0] = ( nITEM_DIF+35 ) * ( nITEM_QUAL+15 ) / 16.f;
				if( IsTAIWAN() )
				{
					// PRO_POINT1 = { (MAT_QUAL) * ((RAN(1~100)+70) * ((CON / A_LV)*100 + ITEM_DIF/2 + 430) * (WORLD_PROD) } / 800000 (2005.07.08 ´ë¸¸)
					fPRO_POINT[0] = ( nMAT_QUAL * (nRand+71) * ( (GetCur_CON() / GetCur_LEVEL())*100.f + nITEM_DIF/2.f + 430 ) * Get_WorldPROD() ) / 800000.f;
				}
				else
					fPRO_POINT[0] = ( nMAT_QUAL * (nRand+71) * ( 0.5f*this->GetCur_CON() + nITEM_DIF/2.f + 530 ) * Get_WorldPROD() ) / 800000.f;
				nPLUS = (short)( ( fPRO_POINT[0] - fSUC_POINT[0] ) * 30 / ( fPRO_POINT[0]+nITEM_QUAL ) );
				break;
			}
			case 1 :
			{
				fSUC_POINT[1] = ( nITEM_DIF+15 ) * ( nITEM_QUAL+140 ) / ( nNUM_MAT+3) / 4.f;
				if( IsTAIWAN() )
				{
					// PRO_POINT2 = { (MAT_QUAL + ITEM_DIF/2) * ((RAN(1~100)+100) * ((CON / A_LV)*100 + MAT_QUAL*2 + 600) } / (NUM_MAT+7) / 1600 (2005.07.08 ´ë¸¸)
					fPRO_POINT[1] = ( ( nMAT_QUAL + nITEM_DIF/2.f ) * (nRand+101) * ( (this->GetCur_CON() / this->GetCur_LEVEL())*100.f + nMAT_QUAL*2 + 600 ) ) / ( nNUM_MAT+7 ) / 1600.f;
				}
				else
					fPRO_POINT[1] = ( ( nMAT_QUAL + nITEM_DIF/2.f ) * (nRand+96) * ( 0.5f*this->GetCur_CON() + SKILL_LEVEL( nSkillIDX )*6 + nMAT_QUAL*2 + 770 ) ) / ( nNUM_MAT+7 ) / 1600.f;
				nPLUS += (short) ( ( fPRO_POINT[1] - fSUC_POINT[1] ) * 20 / ( fPRO_POINT[1] ) );
				break;
			}
			case 2 :
			{
				fSUC_POINT[2] = ( nITEM_DIF+90 ) * ( nITEM_QUAL+30 ) / ( nNUM_MAT+3) / 4.f;
				if( IsTAIWAN() )
				{
					// PRO_POINT3 = { (PRO_POINT1/6 + ITEM_QUAL) * (RAN(1~100)+80) * ((CON / A_LV)*100 + MAT_QUAL*2 + 500) } / (NUM_MAT+7) / 2000 (2005.07.08 ´ë¸¸)
					fPRO_POINT[2] = ( ( fPRO_POINT[0]/6.f + nITEM_QUAL ) * ( nRand+81 ) * ( (GetCur_CON() / GetCur_LEVEL()) * 100.f + nMAT_QUAL*2 + 500 ) ) / ( nNUM_MAT+7 ) / 2000.f;
				}
				else
					fPRO_POINT[2] = ( ( fPRO_POINT[0]/6.f + nITEM_QUAL ) * ( nRand+81 ) * ( 0.3f*this->GetCur_CON() + SKILL_LEVEL( nSkillIDX )*5 + nMAT_QUAL*2 + 600 ) ) / ( nNUM_MAT+7 ) / 2000.f;
				nPLUS += (short)( ( fPRO_POINT[2] - fSUC_POINT[2] ) * 10 / ( fPRO_POINT[2] ) );
				break;
			}
			case 3 :
			{
				fSUC_POINT[3] = ( nITEM_DIF+40 ) * ( nITEM_QUAL+60 ) / ( nNUM_MAT+2) / 6.f;
				fPRO_POINT[3] = ( ( fPRO_POINT[0] + fPRO_POINT[1] + 40 ) * (nRand+51) ) / 200.f;
				nPLUS += (short)( ( fPRO_POINT[3] - fSUC_POINT[3] ) * 10 / ( fPRO_POINT[3] ) );
				break;
			}
		}
#ifdef __INC_WORLD
		m_fPROPOINT[ 0 ] = fPRO_POINT[ 0 ] >= 0 ? fPRO_POINT[ 0 ] : 0;
		m_fPROPOINT[ 1 ] = fPRO_POINT[ 1 ] >= 0 ? fPRO_POINT[ 1 ] : 0;
		m_fPROPOINT[ 2 ] = fPRO_POINT[ 2 ] >= 0 ? fPRO_POINT[ 2 ] : 0;
		m_fPROPOINT[ 3 ] = fPRO_POINT[ 3 ] >= 0 ? fPRO_POINT[ 3 ] : 0;
		m_fSUCPOINT[ 0 ] = fSUC_POINT[ 0 ] >= 0 ? fSUC_POINT[ 0 ] : 0;
		m_fSUCPOINT[ 1 ] = fSUC_POINT[ 1 ] >= 0 ? fSUC_POINT[ 1 ] : 0;
		m_fSUCPOINT[ 2 ] = fSUC_POINT[ 2 ] >= 0 ? fSUC_POINT[ 2 ] : 0;
		m_fSUCPOINT[ 3 ] = fSUC_POINT[ 3 ] >= 0 ? fSUC_POINT[ 3 ] : 0;
#endif
		// LogString (LOG_DEBUG, "Step:%d, Rand: %d, Suc:%f, Pro:%f\n", nI, nRand, fSUC_POINT[nI], fPRO_POINT[nI] );

		// »ç¿ëµÈ ¿øÀç·á¸¦ ¼Ò¸ð ½ÃÅ´...
		nUsedCNT = nI;
		sUsedITEM[ nI ] = this->m_Inventory.m_ItemLIST[ nInvIDX ];
		if ( sUsedITEM[ nI ].IsEnableDupCNT() ) {
			// Áßº¹µÈ °¹¼ö¸¦ °®´Â ¾ÆÀÌÅÛÀÌ´Ù.
			sUsedITEM[ nI ].m_uiQuantity = PRODUCT_NEED_ITEM_CNT( nProductIDX, nI );
		}
		#ifndef	__NEW_LOG
			g_pThreadLOG->When_DestroyedITEM( this, &sUsedITEM[ nI ] );
		#endif
		this->Sub_ITEM( nInvIDX, sUsedITEM[ nI ] );

		if ( fPRO_POINT[nI] < fSUC_POINT[nI] ) {
			// ½ÇÆÐ...
			#ifdef	__NEW_LOG
				g_pThreadLOG->When_CreateOrDestroyITEM ( this, NULL, sUsedITEM, nUsedCNT, NEWLOG_CREATE, NEWLOG_FAILED );
			#endif

			if( IsTAIWAN() )
			{
				// Å¸ÀÌ¿Ï ¹öÁ¯¿¡¼­ °æÇèÄ¡´Â »©Áö ¾ÊÀ½..
			}
			else
				this->m_nCreateItemEXP = 1 + (short) ( ( this->Get_TRUE_LEVEL() + 50 ) * fPRO_POINT[0] * ( nNUM_MAT+4 ) / 1300.f );
			return this->Send_gsv_CREATE_ITEM_REPLY( RESULT_CREATE_ITEM_FAILED, nI, fPRO_POINT );
		}
	}
 
	sOutITEM.Clear ();

	sOutITEM.m_cType    = pPacket->m_cli_CREATE_ITEM_REQ.m_cTargetItemTYPE;
	sOutITEM.m_nItemNo  = pPacket->m_cli_CREATE_ITEM_REQ.m_nTargetItemNO;
	sOutITEM.m_bCreated = 1;

	if ( !sOutITEM.IsEnableDupCNT() ) {
		sOutITEM.m_nLife = MAX_ITEM_LIFE;

		// ³»±¸µµ
		int iTEMP = (int)( ( ITEM_DURABITY(sOutITEM.m_cType,sOutITEM.GetItemNO()) + 15 ) * 
									( nPLUS*1.3F + SKILL_LEVEL( nSkillIDX )*2 + 120 ) / (RANDOM(100)+81) * 0.6f );
		if ( iTEMP > 100 )
			sOutITEM.m_cDurability = 100;
		else
			sOutITEM.m_cDurability = iTEMP;

		// ¼Ò°¹ °¹¼ö °áÁ¤
		switch ( ITEM_RARE_TYPE(sOutITEM.GetTYPE(), sOutITEM.GetItemNO() ) ) {
			case 1 :	// ¹«Á¶°Ç
				sOutITEM.m_bHasSocket = 1;
				sOutITEM.m_bIsAppraisal = 1;
				break;
			case 2 :	// °è»ê
				if( IsTAIWAN() )
				{
					// ¼ÒÄÏ¼ö=[{(SEN/A_LV)*40+400)*(PLUS*1.8+¾ÆÀÌÅÛÇ°Áú*0.4+ 8) * 0.2}   / (RAND(1~100)+50)] ? 100 (2005.07.08 ´ë¸¸)
					iTEMP = (int)( ( (GetCur_SENSE()/GetCur_LEVEL())*40.f+400  ) * 
						( nPLUS*1.8f + ITEM_QUALITY(sOutITEM.GetTYPE(), sOutITEM.GetItemNO() )*0.4f + 8 ) * 0.2f / ( RANDOM(100)+51 ) - 100 );
				}
				else
					iTEMP = (int)( ( this->GetCur_SENSE()+400-this->Get_TRUE_LEVEL()*0.7f ) * 
						( nPLUS*1.8f + ITEM_QUALITY(sOutITEM.GetTYPE(), sOutITEM.GetItemNO() )*0.4f + 8 ) * 0.2f / ( RANDOM(100)+50 ) - 100 );
				if ( iTEMP >= 1 ) {
					sOutITEM.m_bHasSocket = 1;
					sOutITEM.m_bIsAppraisal = 1;
					break;
				}
			case 0 :
				{
					// ¾ÆÀÌÅÛ ¿É¼Ç
					iTEMP = 1 + RANDOM(100);
					int iITEM_OP = 0;
					if( IsTAIWAN() )
					{
						// ITEM_OP = [{((SEN/A_LV)*50 + 220) * (PLUS+20) * 0.4 + ITEM_DIF*35 ? 1600 - TEMP} / (TEMP+17)] ? 85 (2005.07.08 ´ë¸¸)
						iITEM_OP = (int)( ( ((GetCur_SENSE()/Get_TRUE_LEVEL()/2.f)*50.f + 220) * ( nPLUS+20 )*0.4f + nITEM_DIF*35 - 1600 - iTEMP ) / ( iTEMP+17 ) - 85 );
					}
					else
						iITEM_OP = (int)( ( ( this->GetCur_SENSE()+220-this->Get_TRUE_LEVEL()/2.f ) * ( nPLUS+20 )*0.4f + nITEM_DIF*35 - 1600 - iTEMP ) / ( iTEMP+17 ) - 85 );
					if ( iITEM_OP > 0 ) {
						sOutITEM.m_bIsAppraisal = 1;	// °¨Á¤ ¹ÞÀº°É·ç...
						int iMod = (int)( (ITEM_QUALITY(sOutITEM.GetTYPE(), sOutITEM.GetItemNO() ) + 12)*3.2f );
						if ( iMod < 300 )
							sOutITEM.m_nGEM_OP = iITEM_OP % iMod;
						else
							sOutITEM.m_nGEM_OP = iITEM_OP % 300;
					}
				}
				break;
		}

		this->Set_ItemSN( sOutITEM );	// ¾ÆÀÌÅÛ Á¦Á¶½Ã...
	} else {
		// ¹«Á¶°Ç 1°³ Á¦Á¶µÈ´Ù...
		sOutITEM.m_uiQuantity = 1;
	}

	#ifdef	__NEW_LOG
		g_pThreadLOG->When_CreateOrDestroyITEM ( this, &sOutITEM, sUsedITEM, nUsedCNT, NEWLOG_CREATE, NEWLOG_SUCCESS );
	#else
		g_pThreadLOG->When_CreatedITEM( this, &sOutITEM );
	#endif

	// sOutITEM ÀÎº¥Åä¸®¿¡ ³Ö°í ¼º°ø Àü¼Û.
	nI = this->Add_ITEM( sOutITEM );
	if ( nI<= 0 ) {
		this->Save_ItemToFILED( sOutITEM );	// Á¦Á¶½Ã ÀÎº¥Åä¸®°¡ Ãß°¡°¡ ºÒ°¡´ÉÇØ¼­ ¹Û¿¡ 30ºÐ°£ º¸°ü...
	}

	if( IsTAIWAN() )
	{
		// ´ë¸¸ ¹öÁ¯ÀÏ °æ¿ì.. °æÇèÄ¡ ÁÖÁö ¾ÊÀ½.
	}
	else
		this->m_nCreateItemEXP = 1 + (short)( ( this->Get_TRUE_LEVEL() + 35 ) * ( fPRO_POINT[0] + this->Get_TRUE_LEVEL() ) * ( nNUM_MAT+4 ) * (nITEM_DIF+20) / 23000.f );

	return this->Send_gsv_CREATE_ITEM_REPLY( RESULT_CREATE_ITEM_SUCCESS, nI, fPRO_POINT, &sOutITEM);
}

//-------------------------------------------------------------------------------------------------
/// Á¦Á¶, Á¦·Ã °á°ú ÀÀ´ä¿¡ ´ëÇÑ Åëº¸
bool classUSER::Send_gsv_ITEM_RESULT_REPORT( BYTE btReport, BYTE btItemType, short nItemNo)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_ITEM_RESULT_REPORT;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_ITEM_RESULT_REPORT );

	pCPacket->m_gsv_ITEM_RESULT_REPORT.m_wObjectIDX = this->Get_INDEX();
	pCPacket->m_gsv_ITEM_RESULT_REPORT.m_btREPORT	= btReport;
	pCPacket->m_gsv_ITEM_RESULT_REPORT.m_btItemType = btItemType;
	pCPacket->m_gsv_ITEM_RESULT_REPORT.m_nItemNO	= nItemNo;
	
    this->GetZONE()->SendPacketToSectors( this, pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
	return true;
}
/// Á¦Á¶, Á¦·Ã °á°ú ÀÀ´ä
bool classUSER::Recv_cli_ITEM_RESULT_REPORT( t_PACKET *pPacket )
{
	switch( pPacket->m_cli_ITEM_RESULT_REPORT.m_btREPORT ) {
		// case REPORT_ITEM_CREATE_START	
		case REPORT_ITEM_CREATE_SUCCESS	:
		case REPORT_ITEM_CREATE_FAILED	:
			if ( this->m_nCreateItemEXP > 0 ) {
				this->Add_EXP( this->m_nCreateItemEXP, true, 0 );
				this->m_nCreateItemEXP = 0;
			} 
			else 
				return true;	// false;

		// case REPORT_ITEM_UPGRADE_START	
		case REPORT_ITEM_UPGRADE_SUCCESS:	
		case REPORT_ITEM_UPGRADE_FAILED	:
			this->Send_gsv_ITEM_RESULT_REPORT( pPacket->m_cli_ITEM_RESULT_REPORT.m_btREPORT,
											   pPacket->m_cli_ITEM_RESULT_REPORT.m_btItemType,
											   pPacket->m_cli_ITEM_RESULT_REPORT.m_nItemNO );
			break;
	}

	return true;
}


//-------------------------------------------------------------------------------------------------
/// Æ¯Á¤ ÄÉ¸¯ÀÇ HP¸¦ ¿äÃ»
bool classUSER::Recv_cli_HP_REQ( t_PACKET *pPacket )
{
	CObjCHAR *pCHAR;
	pCHAR = g_pObjMGR->Get_CharOBJ( pPacket->m_cli_HP_REQ.m_wObjectIDX, true );

	if ( pCHAR ) {
		// TODO:: ¹®Á¦... »óÅÂ¸¸ º¸ÀÚ´Âµ¥ Å¸°ÙÀÌ ¹Ù²î³× ¤Ñ,.¤Ñ;
		// this->Set_TargetIDX( pPacket->m_cli_HP_REQ.m_wObjectIDX );

		return Send_gsv_HP_REPLY( pPacket->m_cli_HP_REQ.m_wObjectIDX, pCHAR->Get_HP() );
	}
	
	return Send_gsv_HP_REPLY( pPacket->m_cli_HP_REQ.m_wObjectIDX, 0 );
}

//-------------------------------------------------------------------------------------------------
/// ¼Ò¸ð ¾ÆÀÌÅÛ »ç¿ë¿äÃ»
bool classUSER::Recv_cli_USE_ITEM( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsSET( FLAG_ING_IGNORE_ALL ) ) return true;

    if ( pPacket->m_cli_USE_ITEM.m_nInventoryIndex < 0 || 
		 pPacket->m_cli_USE_ITEM.m_nInventoryIndex >= INVENTORY_TOTAL_SIZE ) {
        // ÆÐÅ¶ÀÌ Á¶ÀÛµÈ°ÍÀÎ°¡?
		return IS_HACKING( this, "Recv_cli_USE_ITEM-1" );
    }

	return this->Use_InventoryITEM( pPacket );
}

//-------------------------------------------------------------------------------------------------
/// ¾ÆÀÌÅÛÀ» ¹Ù´Û¿¡ ¶³±À
bool classUSER::Recv_cli_DROP_ITEM( t_PACKET *pPacket )
{
	// 1:1°Å·¡Áß ¾ÆÀÌÅÛ µå·Ó ±ÝÁö..
	if ( this->m_btTradeBIT & (BIT_TRADE_READY|BIT_TRADE_DONE) )	return true;
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() )			return true;

	tagITEM sDropITEM;

	// ÀåÂøÃ¢¿¡¼­ ÇÊµå·Î Á÷Á¢ µå·Ó ¾ÈµÇµµ·Ï...
    if ( pPacket->m_cli_DROP_ITEM.m_btInventoryIndex < MAX_EQUIP_IDX ||
		 pPacket->m_cli_DROP_ITEM.m_btInventoryIndex >= INVENTORY_TOTAL_SIZE ) {

		if ( 0 == pPacket->m_cli_DROP_ITEM.m_btInventoryIndex ) {
			// µ·...
			sDropITEM.m_cType  = ITEM_TYPE_MONEY;
			if ( pPacket->m_cli_DROP_ITEM.m_uiQuantity > this->GetCur_MONEY() ) {
				sDropITEM.m_uiMoney = this->GetCur_MONEY();
				this->SetCur_MONEY( 0 );
			} else {
				// <ZULY_HACK>
				if( pPacket->m_cli_DROP_ITEM.m_uiQuantity <= 0 )
					return IS_HACKING( this, "Recv_cli_DROP_ITEM: Player is attempting to drop negitive zuly." );

				this->Sub_CurMONEY( pPacket->m_cli_DROP_ITEM.m_uiQuantity );
				sDropITEM.m_uiMoney = pPacket->m_cli_DROP_ITEM.m_uiQuantity;
			}

			if ( sDropITEM.m_uiMoney <= 0 )
				return true;

			// ¹Ù²ï µ· Á¤º¸ Àü¼Û..
			this->Send_gsv_SET_MONEYONLY( GSV_SET_MONEY_ONLY );
		} else {
	        // ÆÐÅ¶ÀÌ Á¶ÀÛµÈ°ÍÀÎ°¡?
			return IS_HACKING( this, "Recv_cli_DROP_ITEM-2" );
		}
	} else {
		tagITEM *pITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_DROP_ITEM.m_btInventoryIndex ];
		if ( 0 == pITEM->GetHEADER() ) {
			// ºó ¾ÆÀÌÅÛ...
			return true;
		}
		if ( !pITEM->IsEnableDROP() ) {
			// µå·Ó ºÒ°¡ ¾ÆÀÌÅÛÀÌ´Ù.
			CStrVAR *pStrVAR = this->GetZONE()->GetStrVAR ();
			return IS_HACKING( this, "Recv_cli_DROP_ITEM-3 : cant drop item" );
		}

		sDropITEM = *pITEM;
		if ( pITEM->IsEnableDupCNT() ) {
			// ¸ÅÅ©·Îµî¿¡¼­ -°ªÀÌ ÀÔ·ÂµÉ°æ¿ì...
			if ( pPacket->m_cli_DROP_ITEM.m_uiQuantity < 0 ) {
				return IS_HACKING( this, "Recv_cli_DROP_ITEM-minus quantity" );
			}

			// °¹¼ö ¸¸Å­...
			if ( pPacket->m_cli_DROP_ITEM.m_uiQuantity >= pITEM->GetQuantity() ) {
				sDropITEM.m_uiQuantity = pITEM->GetQuantity();
				// Clear !!!
				m_Inventory.DeleteITEM( pPacket->m_cli_DROP_ITEM.m_btInventoryIndex );
			} else {
				pITEM->m_uiQuantity -= pPacket->m_cli_DROP_ITEM.m_uiQuantity;
				sDropITEM.m_uiQuantity = pPacket->m_cli_DROP_ITEM.m_uiQuantity;
			}

			// ¶³¾îÁø°Í °¹¼ö°¡ 0ÀÌ´Ù ...
			if ( sDropITEM.GetQuantity() <= 0 )
				return true;

			this->m_Battle.m_nWEIGHT -= ( ITEM_WEIGHT( sDropITEM.m_cType, sDropITEM.m_nItemNo ) * sDropITEM.GetQuantity() );
		} else {
			// ÅëÃ¤·Î :: ³»ºÎ¿¡¼­ ¹«°Ô »«´Ù...
			this->ClearITEM( pPacket->m_cli_DROP_ITEM.m_btInventoryIndex );
		}

		// ¹Ù²ï ÀÎº¥Åä¸® ±¸Á¶ Àü¼Û..
		this->Send_gsv_SET_INV_ONLY( pPacket->m_cli_DROP_ITEM.m_btInventoryIndex, pITEM );
	}

	// ¶³¾îÁø ¾ÆÀÌÅÛ Àü¼Û..
	if ( sDropITEM.GetHEADER() ) {
		CObjITEM *pObjITEM = new CObjITEM;
		if ( pObjITEM ) {
			//
			tPOINTF PosSET;
			PosSET.x = this->m_PosCUR.x + RANDOM( 201 ) - 100;
			PosSET.y = this->m_PosCUR.y + RANDOM( 201 ) - 100;

			pObjITEM->InitItemOBJ( this, PosSET, this->m_PosSECTOR, sDropITEM );	// »ç¿ëÀÚ µå·Ó.

			#ifdef	__NEW_LOG
				if ( ITEM_TYPE_MONEY == sDropITEM.m_cType )
					g_pThreadLOG->When_TagItemLOG( LIA_DROP, this, NULL, 0, sDropITEM.m_uiMoney );
				else
					g_pThreadLOG->When_TagItemLOG( LIA_DROP, this, &sDropITEM, 0, 0 );
			#else
				g_pThreadLOG->When_DropITEM( this, pObjITEM );
			#endif
			this->GetZONE()->Add_DIRECT( pObjITEM );	// µå·Ó ¾ÆÀÌÅÛ
		}
	}

	return true;
}


//-------------------------------------------------------------------------------------------------
bool classUSER::Check_ItemEventMSG( tagITEM &sITEM )
{
	//if ( sITEM.GetTYPE() >= ITEM_TYPE_JEWEL && sITEM.GetTYPE() < ITEM_TYPE_RIDE_PART ) {
	//	// ¼­¹ö °øÁö ÇÊ¿äÇÑ ÀÌº¥Æ® ¾ÆÀÌÅÛÀÌ³Ä ???
	//	int iMsgID = ITEM_ANNOUNCE_TYPE( sITEM.GetTYPE(), sITEM.GetItemNO() );
	//	if ( 0 == ::Get_ServerLangTYPE() && 1 == iMsgID ) {
	//		CStrVAR *pCStr = this->GetZONE()->GetStrVAR ();

	//		//pCStr->Printf ( "/na %d-Ã¤³Î ¼­¹öÀÇ %s´Ô²²¼­ %s ¾ÆÀÌÅÛÀ» È¹µæ ÇÏ¼Ì½À´Ï´Ù.", 
	//			// CLIB_GameSRV::GetInstance()->GetChannelNO(),
	//		pCStr->Printf ( "/na %s´Ô²²¼­ %s ¾ÆÀÌÅÛÀ» È¹µæ ÇÏ¼Ì½À´Ï´Ù.", 
	//			this->Get_NAME(),
	//			ITEM_NAME(sITEM.GetTYPE(), sITEM.GetItemNO() ) );
	//		g_pSockLSV->Send_gsv_CHEAT_REQ( this, 0, 0, pCStr->Get() );

	//		g_LOG.CS_ODS( 0xffff, pCStr->Get() );
	//		g_LOG.CS_ODS( 0xffff, "\n" );
	//		return true;
	//	}
	//}
	return false;
}


//-------------------------------------------------------------------------------------------------
/// ¹Ù´ÛÀÇ ¾ÆÀÌÅÛÀ» ½Àµæ...
bool classUSER::Pick_ITEM( CObjITEM *pITEM )
{
	// µ· ¾ÆÀÌÅÛÀÏ °æ¿ì... ITEM_TYPE( type )¿¡¼­ »¶~~~~
	if ( ITEM_TYPE_USE == pITEM->m_ITEM.GetTYPE() && 
		 USE_ITEM_VOLATILITY_ITEM == ITEM_TYPE( ITEM_TYPE_USE, pITEM->m_ITEM.GetItemNO() ) ) {
		// ½Àµæ½Ã ¹Ù·Î ¼Ò¸ðµÇ´Â ¾ÆÀÌÅÛ...
		this->Use_pITEM( &pITEM->m_ITEM );

		#ifdef	__NEW_LOG
			g_pThreadLOG->When_ObjItemLOG( LIA_PICK, this, pITEM );
		#else
			g_pThreadLOG->When_PickITEM( this, pITEM );
		#endif

		pITEM->m_iRemainTIME = -1;		// Pick_ITEM:: ½Àµæ½Ã µ· »èÁ¦µÇµµ·Ï...
		pITEM->m_bDropperIsUSER = false;

		return true;
	}

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_GET_FIELDITEM_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_GET_FIELDITEM_REPLY );
	pCPacket->m_gsv_GET_FIELDITEM_REPLY.m_wServerItemIDX = pITEM->Get_INDEX();//pPacket->m_cli_GET_FIELDITEM_REQ.m_wServerItemIDX;

	// °³ÀÎ ÀÎº¥Åä¸®¿¡ ³Ö´Â´Ù.
	pCPacket->m_gsv_GET_FIELDITEM_REPLY.m_nInventoryListNO = this->Add_ITEM( pITEM->m_ITEM );
	if ( pCPacket->m_gsv_GET_FIELDITEM_REPLY.m_nInventoryListNO >= 0 ) {

		pCPacket->m_gsv_GET_FIELDITEM_REPLY.m_btResult = REPLY_GET_FIELDITEM_REPLY_OK;
		pCPacket->m_gsv_GET_FIELDITEM_REPLY.m_ITEM	   = pITEM->m_ITEM;

		if ( ITEM_TYPE_MONEY != pITEM->m_ITEM.GetTYPE() ) {
			this->Set_ItemSN( this->m_Inventory.m_ItemLIST[ pCPacket->m_gsv_GET_FIELDITEM_REPLY.m_nInventoryListNO ] /* pITEM->m_ITEM */ );	// ÇÊµå¿¡¼­ ½Àµæ½Ã...
		}

		#ifdef	__NEW_LOG
			g_pThreadLOG->When_ObjItemLOG( LIA_PICK, this, pITEM );
		#else
			g_pThreadLOG->When_PickITEM( this, pITEM );
		#endif
		if ( !pITEM->m_bDropperIsUSER ) {
			// »ç¿ëÀÚ°¡ ¹ö¸° ¾ÆÀÌÅÛÀÌ ¾Æ´Ï¶ó¸é...
			this->Check_ItemEventMSG( pITEM->m_ITEM );
		}

		pITEM->m_iRemainTIME = -1;		// Pick_ITEM:: ½Àµæ½Ã »èÁ¦µÇµµ·Ï...
		pITEM->m_bDropperIsUSER = false;
		/*
		// Å×½ºÆ® ÀÌº¥Æ® ¾ÆÀÌÅÛ...
		if ( pITEM->m_ITEM.GetTYPE() == ITEM_TYPE_NATURAL && pITEM->m_ITEM.GetItemNO() == 245 ) {
			CStrVAR *pCStr = this->GetZONE()->GetStrVAR ();

			g_pZoneLIST->Send_gsv_ANNOUNCE_CHAT( "¡Ù¡Ú¡Ù¡Ú ÃàÇÏ ÇÕ´Ï´Ù ¡Ú¡Ù¡Ú¡Ù", NULL  );
			pCStr->Printf ( "%s´Ô²²¼­ %s Áö¿ª¿¡¼­ ÀÌº¥Æ® ¾ÆÀÌÅÛ %sÀ» È¹µæ ÇÏ¼Ì½À´Ï´Ù.", 
				this->Get_NAME(),
				this->GetZONE()->Get_NAME(),
				ITEM_NAME(ITEM_TYPE_NATURAL, pITEM->m_ITEM.GetItemNO() ) );
			g_pZoneLIST->Send_gsv_ANNOUNCE_CHAT( pCStr->Get(), NULL  );
			pCStr->Printf ( "%s´Ô²²¼­´Â È¹µæÇÑ ¾ÆÀÌÅÛÀ» ¸¶À»ÀÇ ÇÏÃ÷¼ÒÈ¯¼ú»ç¿¡°Ô °¡Á®´Ù ÁÖ¼¼¿ä.", this->Get_NAME() );
			g_pZoneLIST->Send_gsv_ANNOUNCE_CHAT( pCStr->Get(), NULL  );
		}
		*/
		// ZoneThread¿¡¼­ SetZone(NULL)ÀÌ µÇ¸é¼­ ... ¾ÆÀÌÅÛ »èÁ¦ ÆÐÅ¶ Àü¼Û.
	} else
		pCPacket->m_gsv_GET_FIELDITEM_REPLY.m_btResult = REPLY_GET_FIELDITEM_REPLY_TOO_MANY;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

/// ¹Ù´ÛÀÇ ¾ÆÀÌÅÛ ½Àµæ ÆÐÅ¶ ¿äÃ» ¹ÞÀ½
bool classUSER::Recv_cli_GET_FIELDITEM_REQ( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;

	// 실제 필요 수치 소모 적용...
	short nSkillIDX = pPacket->m_cli_GET_FIELDITEM_REQ.m_btSkillIDX;
	this->Skill_UseAbilityValue( nSkillIDX );

	BYTE btResult;

	CObjITEM *pITEM = g_pObjMGR->Get_ItemOBJ( pPacket->m_cli_GET_FIELDITEM_REQ.m_wServerItemIDX );
	if ( pITEM ) {
		if ( pITEM->Check_PartyRIGHT( this->GetPARTY() ) ) {
			// ÆÄÆ¼ ÇÃ·¹ÀÌÁßÀÌ°í, ¼ÒÀ¯±ÇÀÌ ³» ÆÄÆ¼¿¡ ÀÖ´Ù...
			if ( this->GetPARTY()->Is_ShareMODE() ) {
				// ¸ÕÀú ¸Ô´Â°Ô ÀÓÀÚ´Ù,, ´Ü µ·ÀÏ°æ¿ì °øÅë ºÐ¹è...
				if ( ITEM_TYPE_MONEY == pITEM->m_ITEM.GetTYPE() ) {
					// µ·ÀÌ°í.. ÆÄÆ¼ÁßÀÌ¸ç.. ¾ÆÀÌÅÛ ¼ÒÀ¯±ÇÀÌ ÆÄÆ¼¿¡ ÀÖ°í...
					// ¾ÆÀÌÅÛ ºÐ¹è ¹æ½ÄÀÌ µ· ÀÚµ¿ºÐ¹è.. ¾ÆÀÌÅÛ ¼ÒÀ¯±ÇÀÌ ÆÄÆ¼¿¡

					this->GetPARTY()->Share_MONEY( pITEM->m_ITEM.GetMoney() );

					pITEM->m_iRemainTIME = -1;		// ½ÀµæµÈ µ· »èÁ¦µÇµµ·Ï...
					pITEM->m_bDropperIsUSER = false;

					return true;
				}
				// ½ÃµµÇÑ³Ñ ÀÓÀÚ
				return this->Pick_ITEM( pITEM );
			}

			// ¼ø¼­ÀÎ »ç¿ëÀÚ¿¡°Ô ÀÚµ¿À¸·Î ÁØ´Ù,,, ¼ø¼­ÀÚ°¡ µýÁ¸¿¡ ÀÖÀ¸¸é ¸ÔÀº³ÑÀÌ Â÷Áö...
			return this->GetPARTY()->Give_Item2ORDER( this, pITEM );
		} else
		if ( pITEM->Is_FREE () ) {
			// ¼ÒÀ¯±Ç ¾ø´Â ¾ÆÀÌÅÛÀÌ´Ù... <<-- ÀÌ°Íµµ ÆÄÆ¼½Ã¿¡´Â ¼ø¼­ ºÐ¹è???
			return this->Pick_ITEM( pITEM );
		} else
		if ( pITEM->Check_PrivateRIGHT( this->Get_INDEX() ) )
			return this->Pick_ITEM( pITEM );
		
		// ±ÇÇÑ ¾ø´Ù..
		btResult = REPLY_GET_FIELDITEM_REPLY_NO_RIGHT;
	} else {
		btResult = REPLY_GET_FIELDITEM_REPLY_NONE;
	}

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_GET_FIELDITEM_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_GET_FIELDITEM_REPLY );
	pCPacket->m_gsv_GET_FIELDITEM_REPLY.m_wServerItemIDX = pPacket->m_cli_GET_FIELDITEM_REQ.m_wServerItemIDX;

	pCPacket->m_gsv_GET_FIELDITEM_REPLY.m_btResult = btResult;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

    return true;
}

//-------------------------------------------------------------------------------------------------
/// ÁÙ¸®¸¦ Ã¢°í<->ÀÎº¥ °£¿¡ ÀÌµ¿
bool classUSER::Recv_cli_MOVE_ZULY( t_PACKET *pPacket )
{
	if ( !(this->m_dwPayFLAG & PLAY_FLAG_STOCK_SPACE ) ) {
		return true;
	}
	if ( BANK_UNLOADED == this->m_btBankData )
		return false;
	if ( pPacket->m_cli_MOVE_ZULY.m_i64MoveZuly < 0 )
		return false;

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	bool bResult = true;
	pCPacket->m_HEADER.m_wType = GSV_MOVE_ZULY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_MOVE_ZULY );

	switch( pPacket->m_cli_MOVE_ZULY.m_btMoveTYPE ) {
		case MOVE_ZULY_TYPE_INV2BANK :	// º¸°ü
			if ( pPacket->m_cli_MOVE_ZULY.m_i64MoveZuly > this->GetCur_MONEY() ) {
				goto _RETURN;
			}
			this->m_Inventory.m_i64Money -= pPacket->m_cli_MOVE_ZULY.m_i64MoveZuly;
			this->m_Bank.m_i64ZULY += pPacket->m_cli_MOVE_ZULY.m_i64MoveZuly;

			break;
		case MOVE_ZULY_TYPE_BANK2INV :	// ²¨³¿
			if ( pPacket->m_cli_MOVE_ZULY.m_i64MoveZuly > this->m_Bank.m_i64ZULY ) {
				goto _RETURN;
			}
			this->m_Bank.m_i64ZULY -= pPacket->m_cli_MOVE_ZULY.m_i64MoveZuly;
			this->m_Inventory.m_i64Money += pPacket->m_cli_MOVE_ZULY.m_i64MoveZuly;
			break;

		default :
			bResult = false;
	}

	if ( bResult ) {
		pCPacket->m_gsv_MOVE_ZULY.m_i64InvZuly	= this->GetCur_MONEY();	
		pCPacket->m_gsv_MOVE_ZULY.m_i64BankZuly = this->m_Bank.m_i64ZULY;	

		this->SendPacket( pCPacket );

		this->m_btBankData = BANK_CHANGED;
	}

_RETURN :
	Packet_ReleaseNUnlock( pCPacket );
	return bResult;
}

/// ¾ÆÀÌÅÛÀ» Ã¢°í<->ÀÎº¥ °£¿¡ ÀÌµ¿ 
bool classUSER::Recv_cli_MOVE_ITEM( t_PACKET *pPacket )
{
	if ( !(this->m_dwPayFLAG & PLAY_FLAG_STOCK_SPACE ) ) {
		return true;
	}
	if ( BANK_UNLOADED == this->m_btBankData )
		return false;

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	bool bResult = true;
	pCPacket->m_HEADER.m_wType = GSV_MOVE_ITEM;

	short nToSlotIDX;
	tagITEM *pSourITEM;

	switch( pPacket->m_cli_MOVE_ITEM.m_btMoveTYPE ) {
		case MOVE_ITEM_TYPE_INV2BANK :
		{
			if ( !pPacket->m_cli_MOVE_ITEM.m_MoveITEM.IsEnableKEEPING() ) {
				IS_HACKING(this, "Invalid keeping item");
				bResult = false;
				goto _RETURN;
			}

			if ( pPacket->m_cli_MOVE_ITEM.m_btFromIDX < MAX_EQUIP_IDX || 
				 pPacket->m_cli_MOVE_ITEM.m_btFromIDX >= INVENTORY_TOTAL_SIZE ) {
				IS_HACKING(this, "Invalid keeping inventory index");
				bResult = false;
				goto _RETURN;
			}

			// ·ºÀÌ ¹ß»ýÇÏ¿© °°Àº ¾ÆÀÌÅÛÀ» µÎ¹ø ¿Å±æ°æ¿ì ÀÖÀ»¼ö ÀÖ´Ù.
			pSourITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_MOVE_ITEM.m_btFromIDX ];
			if ( pSourITEM->IsEmpty() ) {
				goto _RETURN;		// bResult = true±â¶§¹®¿¡ Â©¸®Áø ¾Ê´Â´Ù.;
			}

			// ¸Þ¸ð¸® Á¶ÀÛ¿¡ ÀÇÇØ ¹Ù²î´Â°Å ¹æÁö
			if ( pSourITEM->GetHEADER() != pPacket->m_cli_MOVE_ITEM.m_MoveITEM.GetHEADER() ) {
				IS_HACKING(this, "Difference keeping item");
				bResult = false;
				goto _RETURN;
			}

			tagITEM sOriITEM, sMoveITEM;
			sMoveITEM = pPacket->m_cli_MOVE_ITEM.m_MoveITEM;

			int iFee;
			UINT iDupCnt;
			if ( pSourITEM->IsEnableDupCNT() ) {
				sMoveITEM.m_nItemNo = pSourITEM->GetItemNO();	// ¸Þ¸ð¸® Á¶ÀÛÀ¸·Î ¾ÆÀÌÅÛ ¹øÈ£ ¹Ù²î´Â°Å Â÷´Ü !!!
				iDupCnt = pPacket->m_cli_MOVE_ITEM.m_MoveITEM.GetQuantity();
				// ¿Å±â·Á´Â °¹¼ö°¡ ´õ ¸¹À¸¸é...
				if ( iDupCnt > pSourITEM->GetQuantity() || iDupCnt > MAX_DUP_ITEM_QUANTITY ) {
					goto _RETURN;	// bResult = true±â¶§¹®¿¡ Â©¸®Áø ¾Ê´Â´Ù.;

				}
				if ( pSourITEM->GetQuantity() > MAX_DUP_ITEM_QUANTITY) {
					// ÇØÅ·¿¡ ÀÇÇØ 999°³ÀÌ»ó ¿Å°Ü ³õÀº ¾ÆÀÌÅÛÀÌ¶ó¸é »èÁ¦...
					g_pThreadLOG->When_ItemHACKING( this, pSourITEM, "ItemHACK" );
					this->m_Inventory.m_i64Money = 0;
					pSourITEM->Clear();
					goto _RETURN;	// bResult = true±â¶§¹®¿¡ Â©¸®Áø ¾Ê´Â´Ù.;
				}
#ifdef	__INC_PLATINUM
				if ( this->m_GrowAbility.IsBankFREE( this->GetCurAbsSEC() ) )
					iFee = 0;
				else
#endif
					iFee = CCal::Get_StorageFEE (
							ITEM_BASE_PRICE(sMoveITEM.GetTYPE(), sMoveITEM.GetItemNO() ),
							ITEM_PRICE_RATE(sMoveITEM.GetTYPE(), sMoveITEM.GetItemNO() ),
							iDupCnt );
			} else {
				sMoveITEM.m_dwBody = pSourITEM->m_dwBody;		// ¸Þ¸ð¸® Á¶ÀÛÀ¸·Î ¿É¼ÇÀÌ ¹Ù²î´Â°Å Â÷´Ü !!!
#ifdef	__INC_PLATINUM
				if ( this->m_GrowAbility.IsBankFREE( this->GetCurAbsSEC() ) )
					iFee = 0;
				else
#endif
					iFee = CCal::Get_StorageFEE (
								ITEM_BASE_PRICE(sMoveITEM.GetTYPE(), sMoveITEM.GetItemNO() ),
								ITEM_PRICE_RATE(sMoveITEM.GetTYPE(), sMoveITEM.GetItemNO() ),
								1 );
			}
			if ( this->GetCur_MONEY() < iFee ) {
				// º¸°ü·á ¾ø´Ù.
				goto _RETURN;		// bResult = true±â¶§¹®¿¡ Â©¸®Áø ¾Ê´Â´Ù.;
			}

			sMoveITEM.m_iSN = pSourITEM->m_iSN;
			sOriITEM = *pSourITEM;
			this->Sub_ITEM( pPacket->m_cli_MOVE_ITEM.m_btFromIDX, sMoveITEM );
			// ÇÃ·¹Æ¼³Ñ ¼­ºñ½º ÅÜ °¡´É...
			if ( IsJAPAN() ) {
				if ( this->m_dwPayFLAG & PLAY_FLAG_EXTRA_STOCK ) {
					// ÃÑ 120°³ »ç¿ë
					nToSlotIDX = this->m_Bank.Add_ITEM( sMoveITEM, 0, BANKSLOT_JPN_DEFAULT+BANKSLOT_JPN_EXTRA );
				} else {
					// ÃÑ 40°³ »ç¿ë
					nToSlotIDX = this->m_Bank.Add_ITEM( sMoveITEM, 0, BANKSLOT_JPN_DEFAULT );
				}
			} else {
	#ifndef	__INC_WORLD
				if ( ( this->m_dwPayFLAG & PLAY_FLAG_EXTRA_STOCK ) && pPacket->m_cli_MOVE_ITEM.m_btUseSpecialTAB == 1 ) {
	#else
				if ( pPacket->m_cli_MOVE_ITEM.m_btUseSpecialTAB == 1 ) {
	#endif
					nToSlotIDX = this->m_Bank.Add_ITEM( sMoveITEM, BANKSLOT_PLATINUM_0, BANKSLOT_PLATINUM_0+BANKSLOT_PLATINUM );
				} else 
				if ( this->m_GrowAbility.IsBankAddON( this->GetCurAbsSEC() ) ) {
					// Ã¢°í È®Àå½½·Ô °¡´É......
					nToSlotIDX = this->m_Bank.Add_ITEM( sMoveITEM, 0, BANKSLOT_DEFAULT+BANKSLOT_ADDON );
				} else {
					// ÀÏ¹Ý Ã¢°í....
					nToSlotIDX = this->m_Bank.Add_ITEM( sMoveITEM, 0, BANKSLOT_DEFAULT );
				}
			}

			if ( nToSlotIDX < 0 ) {
				// ½ÇÆÐ :: Add_ITEM ½º·Ô¹øÈ£ÀÇ ¾ÆÀÌÅÛÀ» ´ëÃ¼ ÇØ¹ö¸®±â¶§¹®¿¡...»ç¿ëÇÏ¸é ¾ÈµÊ !!!
				// this->Add_ITEM( pPacket->m_cli_MOVE_ITEM.m_btFromIDX, sMoveITEM );
				*pSourITEM = sOriITEM;
			} else {
				#ifdef	__NEW_LOG
					g_pThreadLOG->When_TagItemLOG( LIA_DEPOSIT, this, &sMoveITEM, 0);
				#else
					g_pThreadLOG->When_DEPOSIT( this, &sMoveITEM );
				#endif

				if ( pPacket->m_cli_MOVE_ITEM.m_MoveITEM.IsEnableDupCNT() ) {
					if ( iDupCnt != pPacket->m_cli_MOVE_ITEM.m_MoveITEM.GetQuantity() )
						iFee = CCal::Get_StorageFEE (
									ITEM_BASE_PRICE(sMoveITEM.GetTYPE(), sMoveITEM.GetItemNO() ),
									ITEM_PRICE_RATE(sMoveITEM.GetTYPE(), sMoveITEM.GetItemNO() ),
									pPacket->m_cli_MOVE_ITEM.m_MoveITEM.GetQuantity() );
				}

				this->Sub_CurMONEY( iFee );
			}

			pCPacket->m_HEADER.m_nSize = sizeof( gsv_MOVE_ITEM ) + sizeof(__int64);
			pCPacket->m_gsv_MOVE_ITEM.m_iCurMoney[ 0 ] = this->GetCur_MONEY();
			pCPacket->m_gsv_MOVE_ITEM.m_nInvIDX		   = pPacket->m_cli_MOVE_ITEM.m_btFromIDX;
			pCPacket->m_gsv_MOVE_ITEM.m_nBankIDX	   = nToSlotIDX;
			break;
		}

		case MOVE_ITEM_TYPE_BANK2INV :
			// ²¨³» °¡´Â°Ç °áÁ¦¿Í »ó°ü¾ø´Ù..
			if ( pPacket->m_cli_MOVE_ITEM.m_btFromIDX >= BANKSLOT_TOTAL ) {
				bResult = false;
				goto _RETURN;
			}
			pSourITEM = &this->m_Bank.m_ItemLIST[ pPacket->m_cli_MOVE_ITEM.m_btFromIDX ];

			// ·ºÀÌ ¹ß»ýÇÏ¿© °°Àº ¾ÆÀÌÅÛÀ» µÎ¹ø ¿Å±æ°æ¿ì ÀÖÀ»¼ö ÀÖ´Ù.
			if ( pSourITEM->IsEmpty() ) {
				goto _RETURN;
			}

			// ¸Þ¸ð¸® Á¶ÀÛ¿¡ ÀÇÇØ ¹Ù²î´Â°Å ¹æÁö
			if ( pSourITEM->GetHEADER() != pPacket->m_cli_MOVE_ITEM.m_MoveITEM.GetHEADER() ) {
				IS_HACKING(this, "Difference withdraw item");
				bResult = false;
				goto _RETURN;
			}

			tagITEM sOriITEM, sMoveITEM, sLogITEM;
			sMoveITEM = pPacket->m_cli_MOVE_ITEM.m_MoveITEM;

			if ( pSourITEM->IsEnableDupCNT() ) {
				if ( pSourITEM->GetQuantity() > MAX_DUP_ITEM_QUANTITY) {
					// ÇØÅ·¿¡ ÀÇÇØ 999°³ÀÌ»ó ¿Å°Ü ³õÀº ¾ÆÀÌÅÛÀÌ¶ó¸é »èÁ¦...
					g_pThreadLOG->When_ItemHACKING( this, pSourITEM, "BankHACK" );
					this->m_Inventory.m_i64Money = 0;
					pSourITEM->Clear();
					goto _RETURN;
				}

				// ¿Å±â·Á´Â °¹¼ö°¡ ´õ ¸¹À¸¸é...
				if ( pPacket->m_cli_MOVE_ITEM.m_MoveITEM.GetQuantity() > pSourITEM->GetQuantity() ||
					 pPacket->m_cli_MOVE_ITEM.m_MoveITEM.GetQuantity() > MAX_DUP_ITEM_QUANTITY ) {
					goto _RETURN;
				}
			} else {
				sMoveITEM.m_dwBody = pSourITEM->m_dwBody;		// ¸Þ¸ð¸® Á¶ÀÛÀ¸·Î ¿É¼Ç ¹Ù²î´Â°Å Â÷´Ü..
			}

			sOriITEM = *pSourITEM;
			sMoveITEM.m_iSN = pSourITEM->m_iSN;
			sLogITEM = sMoveITEM;

			this->m_Bank.Sub_ITEM( pPacket->m_cli_MOVE_ITEM.m_btFromIDX, sMoveITEM );
			nToSlotIDX = this->Add_ITEM( sMoveITEM );
			if ( nToSlotIDX < 0 ) {
				// ½ÇÆÐ :: Add_ITEM ½º·Ô¹øÈ£ÀÇ ¾ÆÀÌÅÛÀ» ´ëÃ¼ ÇØ¹ö¸®±â¶§¹®¿¡...»ç¿ëÇÏ¸é ¾ÈµÊ !!!
				// this->m_Bank.Add_ITEM( pPacket->m_cli_MOVE_ITEM.m_btFromIDX, sMoveITEM );
				*pSourITEM = sOriITEM;
			} else {
			#ifdef	__NEW_LOG
				g_pThreadLOG->When_TagItemLOG( LIA_WITHDRAW, this, &sLogITEM, 0, 0, 0, true);
			#else
				g_pThreadLOG->When_WithDRAW( this, &sMoveITEM );
			#endif
			}

			pCPacket->m_HEADER.m_nSize = sizeof( gsv_MOVE_ITEM );
			pCPacket->m_gsv_MOVE_ITEM.m_nBankIDX = pPacket->m_cli_MOVE_ITEM.m_btFromIDX;
			pCPacket->m_gsv_MOVE_ITEM.m_nInvIDX  = nToSlotIDX;
			break;

		default :
			bResult = false;
	}

	if ( bResult ) {
		if ( pCPacket->m_gsv_MOVE_ITEM.m_nInvIDX > 0 )
			pCPacket->m_gsv_MOVE_ITEM.m_InvItem  = this->m_Inventory.m_ItemLIST[ pCPacket->m_gsv_MOVE_ITEM.m_nInvIDX ];

		if ( pCPacket->m_gsv_MOVE_ITEM.m_nBankIDX >= 0 )
			pCPacket->m_gsv_MOVE_ITEM.m_BankITEM = this->m_Bank.m_ItemLIST[ pCPacket->m_gsv_MOVE_ITEM.m_nBankIDX ];

		this->SendPacket( pCPacket );

		this->m_btBankData = BANK_CHANGED;
	}

_RETURN :
	Packet_ReleaseNUnlock( pCPacket );

	return bResult;
}

//-------------------------------------------------------------------------------------------------
/// Ã¢°í¸®½ºÆ® ¿äÃ» ¹ÞÀ½
bool classUSER::Recv_cli_BANK_LIST_REQ( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;
	if ( BANK_UNLOADED == this->m_btBankData ) {
		// DB¿¡ ÀºÇà ¾ÆÀÌÅÛ ¿äÃ»...
		if ( BANK_REQ_OPEN == pPacket->m_cli_BANK_LIST_REQ.m_btREQ )
			return g_pThreadSQL->Add_SqlPacketWithACCOUNT (this, pPacket );
		// ÀºÇà µ¥ÀÌÅ¸ ¿­¸®Áöµµ ¾Ê¾Ò´Âµ¥ ºñ¹øÀ» ¹Ù²ã???
		return true;
	}

	if ( BANK_REQ_CHANGE_PASSWORD == pPacket->m_cli_BANK_LIST_REQ.m_btREQ ) {
		return g_pThreadSQL->Add_SqlPacketWithACCOUNT (this, pPacket );
	}

	if ( this->m_BankPASSWORD.Get() ) {
		if ( pPacket->m_HEADER.m_nSize > sizeof( cli_BANK_LIST_REQ ) ) {
			// ºñ¹ø Æ²¸®¸é...
			short nOffset=sizeof( cli_BANK_LIST_REQ );
			char *szPassWD = Packet_GetStringPtr( pPacket, nOffset );
			if ( !szPassWD || strcmp( this->m_BankPASSWORD.Get(), pPacket->m_cli_BANK_LIST_REQ.m_szPassword ) )
				return this->Send_gsv_BANK_LIST_REPLY( BANK_REPLY_INVALID_PASSWORD );
		} else {
			// ºñ¹ø³Ö°í ÆÐÅ¶Àü¼ÛÇÏ¶ó°í...
			return this->Send_gsv_BANK_LIST_REPLY( BANK_REPLY_NEED_PASSWORD );
		}
	}

	return this->Send_gsv_BANK_LIST_REPLY( BANK_REPLY_SHOW_DATA );
}

//-------------------------------------------------------------------------------------------------
/// ¿öÇÁ ¿äÃ» ÆÐÅ¶
#define	MAX_WARP_OBJECT_DISTANCE	( 1000 * 5 )
short classUSER::Recv_cli_TELEPORT_REQ( t_PACKET *pPacket )
{
	if ( !this->GetZONE() ) 
		return RET_FAILED;

    WORD wWarpIDX = (WORD)( pPacket->m_cli_TELEPORT_REQ.m_nWarpIDX );
	if ( wWarpIDX >= g_TblWARP.m_nDataCnt ) {
		g_LOG.CS_ODS( 0xffff, "[ %s:%s ] Invalied Warp INDEX: %d", this->m_IP.Get(), this->Get_NAME(), wWarpIDX );
		return IS_HACKING( this, "Recv_cli_TELEPORT_REQ-1" );
	}
	if ( TELEPORT_ZONE( wWarpIDX ) <= 0 || NULL == TELEPORT_EVENT_POS(wWarpIDX) ) {
		g_LOG.CS_ODS( 0xffff, "[ %s:%s ] Invalied Warp INDEX: %d", this->m_IP.Get(), this->Get_NAME(), wWarpIDX );
		return IS_HACKING( this, "Recv_cli_TELEPORT_REQ-11" );
	}

	if ( sizeof( cli_TELEPORT_REQ ) == pPacket->m_HEADER.m_nSize ) {
		// ½ºÇÙÃ¼Å©¿ë Å¬¶óÀÌ¾ðÆ® ÁÂÇ¥ ºñ±³
		int iDistance = distance ((int)m_PosCUR.x, (int)m_PosCUR.y, (int)pPacket->m_cli_TELEPORT_REQ.m_PosCUR.x, (int)pPacket->m_cli_TELEPORT_REQ.m_PosCUR.y);
		if ( iDistance > MAX_WARP_OBJECT_DISTANCE )	{ 
			// ¼­¹öÀÇ À§Ä¡·Î Å¬¶óÀÌ¾ðÆ® º¹±Í...
			return this->Send_gsv_ADJUST_POS( true );
		}
	} else 
	if ( IsTAIWAN() ) {
#ifndef	__INC_WORLD
		return RET_OK;	// IS_HACKING( this, "Recv_cli_TELEPORT_REQ-4" );
#endif
	}
	/*
	// Ã¼Å©ÇØ ¸»¾î ?
	if ( !g_pZoneLIST->IsValidZONE( TELEPORT_ZONE( nWarpIDX ) ) {
		g_LOG.CS_ODS( 0xffff, "ERROR:: Invalid Warp Zone ... WarpIDX: %d, ZoneNO: %d \n", nWarpIDX, TELEPORT_ZONE( nWarpIDX ) );
		return IS_HACKING( this, "Recv_cli_TELEPORT_REQ-2" );
	}
	*/

	// °°Àº ¼­¹ö¿¡¼­ Á¸ÀÌ ½ÇÇà µÇ°í ÀÖ´Â°¡ ???
	tagEVENTPOS *pEventPOS = g_pZoneLIST->Get_EventPOS( TELEPORT_ZONE( wWarpIDX ), TELEPORT_EVENT_POS(wWarpIDX) );
	if ( !pEventPOS ) {
		g_LOG.CS_ODS( 0xffff, "ERROR:: Invalid Warp Position... WarpIDX: %d \n", wWarpIDX );
		return IS_HACKING( this, "Recv_cli_TELEPORT_REQ-3" );
	}

	return this->Proc_TELEPORT( pEventPOS->m_nZoneNO, pEventPOS->m_Position );	// Recv_cli_TELEPORT_REQ
}


//-------------------------------------------------------------------------------------------------
/// ½ºÅÈ º¯°æ ¿äÃ» ¹ÞÀ½
bool classUSER::Recv_cli_USE_BPOINT_REQ( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsSET( FLAG_ING_IGNORE_ALL ) ) return true;

	if ( pPacket->m_cli_USE_BPOINT_REQ.m_btAbilityNO >= BA_MAX ) {
		return IS_HACKING (this, "cli_USE_POINTS-2" );
	}

	short nNeedPoints, nAbilityValue, nCurSpeed;

	// ±âº» ´É·ÂÄ¡ »ó½Â...
	nNeedPoints = this->Get_NeedPoint2AbilityUP( pPacket->m_cli_USE_BPOINT_REQ.m_btAbilityNO );
	if ( this->GetCur_BonusPOINT() < nNeedPoints ) {
		// Æ÷ÀÎÆ® ¸ðÀß¶õ´Ù..
		return true;
	}

	if ( this->m_BasicAbility.m_nBasicA[ pPacket->m_cli_USE_BPOINT_REQ.m_btAbilityNO ] >= MAX_BASIC_ABLITY ) {
		// ´õÀÌ»ó ¿Ã¸±¼ö ¾ø´Ù.
		return true;
	}

	this->m_BasicAbility.m_nBasicA[ pPacket->m_cli_USE_BPOINT_REQ.m_btAbilityNO ] ++;

	nCurSpeed = this->GetOri_RunSPEED();
	this->UpdateAbility ();		// use_bpoint

	nAbilityValue = this->m_BasicAbility.m_nBasicA[ pPacket->m_cli_USE_BPOINT_REQ.m_btAbilityNO ];

	this->SetCur_BonusPOINT( this->GetCur_BonusPOINT() - nNeedPoints );

	/* kchs-modify
	2. Ä³¸¯ÅÍ ½ºÅÈ
	1) ½ºÅÈ º¯µ¿
	¨ç º¯µ¿µÈ ½ºÅÈÀÇ Á¾·ù ¹× ¼Ò¸ð Æ÷ÀÎÆ®
	¨è ³¯Â¥/½Ã°£/ IP/ÁÂÇ¥*/
	g_pThreadLOG->When_ChangeABILITY ( this , pPacket->m_cli_USE_BPOINT_REQ.m_btAbilityNO, nNeedPoints );

    classPACKET *pCPacket = Packet_AllocNLock ();

    pCPacket->m_HEADER.m_wType = GSV_USE_BPOINT_REPLY;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_USE_BPOINT_REPLY );

    pCPacket->m_gsv_USE_BPOINT_REPLY.m_btAbilityNO	= pPacket->m_cli_USE_BPOINT_REQ.m_btAbilityNO;
    pCPacket->m_gsv_USE_BPOINT_REPLY.m_nAbilityValue = nAbilityValue;

	this->SendPacket( pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	if ( nCurSpeed != this->GetOri_RunSPEED() )  {
		// ÀÌµ¿ ¼Óµµ°¡ ¹Ù²î¹Ç·Î ÁÖº¯¿¡ ÀÌµ¿ ¼Óµµ Àü¼Û...
		this->Send_gsv_SPEED_CHANGED ();
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
/// 1:1 °Å·¡½Ã Á¤»óÀûÀÎ °Å·¡ »óÈ²ÀÎÁö Ã¼Å©...
bool classUSER::Check_TradeITEM ()
{
	// 1°³ÀÇ ¾ÆÀÌÅÛÀ» ÇØÅ·À¸·Î ¿©·¯ °Å·¡ ½½·Ô¿¡ ¿Ã·Á ³õ°í °Å·¡ ½ÃµµÇÒ¶§...
	//char cCheckedSlot[ INVENTORY_TOTAL_SIZE ];
	//::ZeroMemory( cCheckedSlot, INVENTORY_TOTAL_SIZE );

	for (short nI=0; nI<TRADE_MONEY_SLOT_NO; nI++) {
		if ( this->m_TradeITEM[ nI ].m_Item.GetTYPE() ) {
			if ( ITEM_TYPE_MONEY == m_TradeITEM[ nI ].m_Item.GetTYPE() )
				return false;

			if ( m_TradeITEM[ nI ].m_Item.GetHEADER() != m_Inventory.m_ItemLIST[ m_TradeITEM[ nI ].m_nInvIDX ].GetHEADER() ) {
				// Å¸ÀÔ Ã¼Å©...
				return false;
			}
			if ( m_Inventory.m_ItemLIST[ m_TradeITEM[ nI ].m_nInvIDX ].IsEnableDupCNT() ) {
				// Áßº¹ °¹¼ö ¾ÆÀÌÅÛ..
				if ( m_Inventory.m_ItemLIST[ m_TradeITEM[ nI ].m_nInvIDX ].GetQuantity() < m_TradeITEM[ nI ].m_Item.GetQuantity() ) {
					// ¼ö·® Ã¼Å©..
					return false;
				}
			} else 
			if ( m_TradeITEM[ nI ].m_Item.m_dwBody != m_Inventory.m_ItemLIST[ m_TradeITEM[ nI ].m_nInvIDX ].m_dwBody ) {
				// Àåºñ ¾ÆÀÌÅÛÀÏ °æ¿ì ¿É¼Ç/Á¦¹Ö/°­È­ Ã¼Å©...
				return false;
			}

			// if ( cCheckedSlot[ this->m_TradeITEM[ nI ].m_nInvIDX ] )
			//	return false;
			// cCheckedSlot[ this->m_TradeITEM[ nI ].m_nInvIDX ] = 1;
		}
	}

	if ( this->m_TradeITEM[ TRADE_MONEY_SLOT_NO ].m_Item.GetTYPE() ) {
		if ( this->GetCur_MONEY() < m_TradeITEM[ TRADE_MONEY_SLOT_NO ].m_Item.m_uiMoney )
			return false;
	}
	return true;
}

/// 1:1 °Å·¡ ¿Ï·á½Ã °Ç³×ÁØ ¾ÆÀÌÅÛÀ» ÀÎº¥Åä¸®¿¡¼­ Á¦°Å
void classUSER::RemoveTradeItemFromINV (classUSER *pTradeUSER, classPACKET *pCPacket)
{
	for (short nI=0; nI<TRADE_MONEY_SLOT_NO; nI++) {
		if ( this->m_TradeITEM[ nI ].m_Item.GetTYPE() ) {
			// ¾ÆÀÌÅÛ Áá´Ù.. »©ÀÚ !!!
			this->Sub_ITEM( m_TradeITEM[ nI ].m_nInvIDX, m_TradeITEM[ nI ].m_Item );
			#ifdef	__NEW_LOG
				g_pThreadLOG->When_TagItemLOG( LIA_GIVE, this, &m_TradeITEM[ nI ].m_Item, 0, 0, pTradeUSER);
			#else
				g_pThreadLOG->When_GiveITEM( this, &m_TradeITEM[ nI ].m_Item, pTradeUSER, nI);
			#endif

			// º¯°æµÈ ÀÎº¥Åä¸®
			pCPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_btInvIDX = (BYTE)m_TradeITEM[ nI ].m_nInvIDX;
			pCPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_ITEM	 = m_Inventory.m_ItemLIST[ m_TradeITEM[ nI ].m_nInvIDX ];
			pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ++;
		}
	}

	if ( this->m_TradeITEM[ TRADE_MONEY_SLOT_NO ].m_Item.GetTYPE() ) {
		// µ·Áá´Ù... »©ÀÚ !!!
		this->Sub_ITEM( m_TradeITEM[ TRADE_MONEY_SLOT_NO ].m_nInvIDX, m_TradeITEM[ TRADE_MONEY_SLOT_NO ].m_Item );

		#ifdef	__NEW_LOG
			g_pThreadLOG->When_TagItemLOG( LIA_GIVE, this, NULL, 0, m_TradeITEM[ TRADE_MONEY_SLOT_NO ].m_Item.GetMoney(), pTradeUSER);
		#else
			g_pThreadLOG->When_GiveITEM( this, &m_TradeITEM[ TRADE_MONEY_SLOT_NO ].m_Item, pTradeUSER, TRADE_MONEY_SLOT_NO );
		#endif
	}
}

/// 1:1°Å·¡ ¿Ï·á½Ã ¹ÞÀº ¾ÆÀÌÅÛÀ» ÀÎº¥Åä¸®¿¡ Ãß°¡
void classUSER::AddTradeItemToINV (classUSER *pTradeUSER/*tagTradeITEM *pTradeITEM*/, classPACKET *pCPacket)
{
	short nI, nInvIDX;

	for (nI=0; nI<TRADE_MONEY_SLOT_NO; nI++) {
		if ( pTradeUSER->m_TradeITEM[ nI ].m_Item.GetTYPE() ) {
			// ´õÇÏÀÚ !!!
			nInvIDX = this->Add_ITEM( pTradeUSER->m_TradeITEM[ nI ].m_Item );
			#ifdef	__NEW_LOG
				g_pThreadLOG->When_TagItemLOG( LIA_RECV, this, &pTradeUSER->m_TradeITEM[ nI ].m_Item, 0, 0, pTradeUSER);
			#else
				g_pThreadLOG->When_RecvITEM( this, &pTradeUSER->m_TradeITEM[ nI ].m_Item, pTradeUSER, nI );
			#endif
			// º¯°æµÈ ÀÎº¥Åä¸®
			if ( nInvIDX > 0 ) {
				pCPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_btInvIDX = (BYTE)nInvIDX;
				pCPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_ITEM	 = m_Inventory.m_ItemLIST[ nInvIDX ];
				pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ++;
			} else {
				this->Save_ItemToFILED( pTradeUSER->m_TradeITEM[ nI ].m_Item );	// 1:1 °Å·¡ÈÄ ¾ÆÀÌÅÛ ¹Ù´Û¿¡ µå·Ó....¼ÒÀ¯½Ã°£ ±æ°Ô~~~~
			}
		}
	}

	if ( pTradeUSER->m_TradeITEM[ TRADE_MONEY_SLOT_NO ].m_Item.GetTYPE() ) {
		// µ· ¹Þ¾ÒÀ½..
		this->Add_ITEM( pTradeUSER->m_TradeITEM[ TRADE_MONEY_SLOT_NO ].m_Item );
		#ifdef	__NEW_LOG
			g_pThreadLOG->When_TagItemLOG( LIA_RECV, this, NULL, 0, pTradeUSER->m_TradeITEM[ TRADE_MONEY_SLOT_NO ].m_Item.GetMoney(), pTradeUSER);
		#else
			g_pThreadLOG->When_RecvITEM( this, &pTradeUSER->m_TradeITEM[ TRADE_MONEY_SLOT_NO ].m_Item, pTradeUSER, TRADE_MONEY_SLOT_NO );
		#endif
	}
}

//-------------------------------------------------------------------------------------------------
/// °Å·¡ ¿äÃ» ÆÐÅ¶..
bool classUSER::Recv_cli_TRADE_P2P( t_PACKET *pPacket )
{
	if ( this->m_iIndex == pPacket->m_cli_TRADE_P2P.m_wObjectIDX ) {
		// ·ÎÁîÇØÄ¿ deebeeÀÇ ¸ÞÀÏ·Î ÀÎÇØ ¾Ë°ÔµÈ ÇØÅ·¹æ¹ý ::
		// 1:1°Å·¡½Ã ÇØÅ·À» ÅëÇØ¼­ ÀÚ½Å°ú °Å·¡¸¦ ÇÏ°Ô ÇÏ´Â ¹æ¹ýÀ¸·Î ¾ÆÀÌÅÛ º¹»ç...
		return IS_HACKING( this, "Recv_cli_TRADE_P2P :: Trade from self" );
	}

	classUSER *pUSER;

	pUSER = g_pObjMGR->Get_UserOBJ( pPacket->m_cli_TRADE_P2P.m_wObjectIDX );
	if ( pUSER ) {
		switch( pPacket->m_cli_TRADE_P2P.m_btRESULT ) {
			case RESULT_TRADE_REQUEST :		// °Å·¡ ¿äÃ»
			{
				if ( !(this->m_dwPayFLAG & PLAY_FLAG_TRADE) ) {
					return true;
				}
				if ( !(pUSER->m_dwPayFLAG & PLAY_FLAG_TRADE) ) {
					return this->Send_gsv_TRADE_P2P( pPacket->m_cli_TRADE_P2P.m_wObjectIDX, RESULT_TRADE_NO_CHARGE_TARGET );
				}
					 
				if ( pUSER->m_iTradeUserIDX || pUSER->m_IngSTATUS.IsIgnoreSTATUS() ) {
					// ´ë»óÀÌ ¹Ù»Ú´Ù..
					return this->Send_gsv_TRADE_P2P( pPacket->m_cli_TRADE_P2P.m_wObjectIDX, RESULT_TRADE_BUSY );
				}

				int iDistance = distance ((int)m_PosCUR.x, (int)m_PosCUR.y, (int)pUSER->m_PosCUR.x, (int)pUSER->m_PosCUR.y);
				if ( iDistance > MAX_TRADE_DISTANCE )	{ 
					// °Å·¡ ¿äÃ» pUSER¿ÍÀÇ °Å¸®´Â Ã¼Å©ÈÄ ÀÚ½Å¿¡°Ô °Å·¡Ãë¼Ò Àü¼Û...
					return this->Send_gsv_TRADE_P2P( pPacket->m_cli_TRADE_P2P.m_wObjectIDX, RESULT_TRADE_TOO_FAR );
				}

				this->m_btTradeBIT = 0;
				this->m_iTradeUserIDX = pPacket->m_cli_TRADE_P2P.m_wObjectIDX;
				break;
			}

			case RESULT_TRADE_ACCEPT :		// °Å·¡ ½ÂÀÎ Çß´Ù.
			{
				if ( pUSER->m_iTradeUserIDX != this->Get_INDEX() ) {
					return this->Send_gsv_TRADE_P2P( pPacket->m_cli_TRADE_P2P.m_wObjectIDX, RESULT_TRADE_CANCEL );
				}
				if ( this->m_IngSTATUS.IsIgnoreSTATUS() || pUSER->m_IngSTATUS.IsIgnoreSTATUS() ) {
					pUSER->Send_gsv_TRADE_P2P( pPacket->m_cli_TRADE_P2P.m_wObjectIDX, RESULT_TRADE_BUSY );
					this->Send_gsv_TRADE_P2P( pPacket->m_cli_TRADE_P2P.m_wObjectIDX, RESULT_TRADE_BUSY );
					return true;
				}

				this->m_btTradeBIT = pUSER->m_btTradeBIT = 0;
				this->m_iTradeUserIDX = pUSER->Get_INDEX();

				::ZeroMemory( pUSER->m_TradeITEM,	sizeof(tagTradeITEM) * MAX_TRADE_ITEM_SLOT );
				::ZeroMemory( this->m_TradeITEM,	sizeof(tagTradeITEM) * MAX_TRADE_ITEM_SLOT );
				break;
			}

			case RESULT_TRADE_REJECT :			// °Å·¡ °ÅºÎ.
			case RESULT_TRADE_CANCEL :			// °Å·¡ µµÁß Ãë¼Ò.
			{
				if ( pUSER->m_iTradeUserIDX == this->Get_INDEX() ) {
					pUSER->m_btTradeBIT = pUSER->m_iTradeUserIDX = 0;
				}
				this->m_btTradeBIT  = this->m_iTradeUserIDX  = 0;
				break;
			} 

			case RESULT_TRADE_DONE :			// °Å·¡ ½ÇÇà.
			{
				// ¼­·Î ´ë»ó¿¡ º¯È­°¡ ¾ø³ª ?
				if ( this->m_iTradeUserIDX != pUSER->Get_INDEX() || pUSER->m_iTradeUserIDX != this->Get_INDEX() ) {
					this->m_btTradeBIT = this->m_iTradeUserIDX = 0;
					pUSER->m_btTradeBIT = pUSER->m_iTradeUserIDX = 0;
					return this->Send_gsv_TRADE_P2P( pPacket->m_cli_TRADE_P2P.m_wObjectIDX, RESULT_TRADE_CANCEL );
				}

				// µÑ´Ù ÁØºñ°¡ ¿Ï·á µÆ³ª ?
				this->m_btTradeBIT |= BIT_TRADE_DONE;
				if ( pUSER->m_btTradeBIT != ( BIT_TRADE_READY | BIT_TRADE_DONE ) ||
					 this->m_btTradeBIT  != ( BIT_TRADE_READY | BIT_TRADE_DONE ) ) {
					return true;
				}

				// Á×°Å³ª °³ÀÎ»óÁ¡ ¸ðµå³Ä ?
				if ( this->m_IngSTATUS.IsIgnoreSTATUS() || 
					 pUSER->m_IngSTATUS.IsIgnoreSTATUS() ||
					!this->Check_TradeITEM () || 
					!pUSER->Check_TradeITEM () ) {
					this->m_btTradeBIT = this->m_iTradeUserIDX = 0;
					pUSER->m_btTradeBIT = pUSER->m_iTradeUserIDX = 0;
					this->Send_gsv_TRADE_P2P( pPacket->m_cli_TRADE_P2P.m_wObjectIDX, RESULT_TRADE_CANCEL );
					pUSER->Send_gsv_TRADE_P2P( pPacket->m_cli_TRADE_P2P.m_wObjectIDX, RESULT_TRADE_CANCEL );
					return true;
				}

				// ½ÇÁ¦ ¹°°Ç ±³È¯ ÇÒ°÷...
				classPACKET *pCPacket1 = Packet_AllocNLock ();
				classPACKET *pCPacket2 = Packet_AllocNLock ();

				pCPacket1->m_HEADER.m_wType = GSV_SET_MONEYnINV;
				pCPacket1->m_HEADER.m_nSize = sizeof( gsv_SET_MONEYnINV );
				pCPacket1->m_gsv_SET_MONEYnINV.m_btItemCNT = 0;

				pCPacket2->m_HEADER.m_wType = GSV_SET_MONEYnINV;
				pCPacket2->m_HEADER.m_nSize = sizeof( gsv_SET_MONEYnINV );
				pCPacket2->m_gsv_SET_MONEYnINV.m_btItemCNT = 0;

				this->RemoveTradeItemFromINV ( pUSER, pCPacket1 );
				pUSER->RemoveTradeItemFromINV ( this, pCPacket2 );

				this->AddTradeItemToINV( pUSER/*pUSER->m_TradeITEM*/, pCPacket1 );
				pUSER->AddTradeItemToINV( this/*this->m_TradeITEM*/, pCPacket2 );

				pCPacket1->m_HEADER.m_nSize += ( pCPacket1->m_gsv_SET_MONEYnINV.m_btItemCNT * sizeof( tag_SET_INVITEM ) );
				pCPacket1->m_gsv_SET_MONEYnINV.m_i64Money = this->GetCur_MONEY();

				pCPacket2->m_HEADER.m_nSize += ( pCPacket2->m_gsv_SET_MONEYnINV.m_btItemCNT * sizeof( tag_SET_INVITEM ) );
				pCPacket2->m_gsv_SET_MONEYnINV.m_i64Money = pUSER->GetCur_MONEY();

				this->SendPacket( pCPacket1 );
				pUSER->SendPacket( pCPacket2 );

				Packet_ReleaseNUnlock( pCPacket1 );
				Packet_ReleaseNUnlock( pCPacket2 );

				this->m_btTradeBIT = this->m_iTradeUserIDX  = 0;
				pUSER->m_btTradeBIT = pUSER->m_iTradeUserIDX = 0;

				pUSER->Send_gsv_TRADE_P2P( this->Get_INDEX(), RESULT_TRADE_DONE, 0 );
				this->Send_gsv_TRADE_P2P( pUSER->Get_INDEX(), RESULT_TRADE_DONE, 0 );
				return true;
			}
			case RESULT_TRADE_CHECK_READY	:	// °Å·¡ ÁØºñ ¿Ï·á.
				this->m_btTradeBIT |= BIT_TRADE_READY;
				break;
			case RESULT_TRADE_UNCHECK_READY :	// °Å·¡ ÁØºñ ÇØÁ¦
				this->m_btTradeBIT = 0;
				pUSER->m_btTradeBIT &= ~BIT_TRADE_DONE;
				break;
			case RESULT_TRADE_OUT_OF_INV :		// ÀÎº¥Åä¸®°¡ ²ËÂ÷¼­ ¹ÞÀ»¼ö ¾ø´Ù.
				if ( pPacket->m_cli_TRADE_P2P.m_cTradeSLOT < 0 ||
					 pPacket->m_cli_TRADE_P2P.m_cTradeSLOT >= MAX_TRADE_ITEM_SLOT ) {
					return IS_HACKING (this, "Recv_cli_TRADE_P2P" ); 
				}

				pUSER->m_TradeITEM[ pPacket->m_cli_TRADE_P2P.m_cTradeSLOT ].m_Item.m_cType = 0;
				break;
		}

		pUSER->Send_gsv_TRADE_P2P( this->Get_INDEX(), pPacket->m_cli_TRADE_P2P.m_btRESULT, pPacket->m_cli_TRADE_P2P.m_cTradeSLOT );
	} else {
		// ´ë»óÀÌ ¾ø´Ù.
		this->m_btTradeBIT = this->m_iTradeUserIDX = 0;
		this->Send_gsv_TRADE_P2P( this->m_iTradeUserIDX, RESULT_TRADE_NOT_TARGET );
	}

    return true;
}

/// °Å·¡ ¾ÆÀÌÅÛ µî·Ï..
bool classUSER::Recv_cli_TRADE_P2P_ITEM( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;
	if ( this->m_btTradeBIT & (BIT_TRADE_READY|BIT_TRADE_DONE) ) return true;

	if ( pPacket->m_cli_TRADE_P2P_ITEM.m_nInventoryIndex < 0 ||
		 pPacket->m_cli_TRADE_P2P_ITEM.m_nInventoryIndex >= INVENTORY_TOTAL_SIZE ) {
		return IS_HACKING (this, "Recv_cli_TRADE_P2P_ITEM-1 : Invalid inv idx" );
	}
	if ( pPacket->m_cli_TRADE_P2P_ITEM.m_cTradeSLOT < 0 ||
		 pPacket->m_cli_TRADE_P2P_ITEM.m_cTradeSLOT >= MAX_TRADE_ITEM_SLOT ) {
		return IS_HACKING (this, "Recv_cli_TRADE_P2P_ITEM-2 : Invalid trade slot" );
	}

	classUSER *pUSER;
	pUSER = g_pObjMGR->Get_UserOBJ( this->m_iTradeUserIDX );
	if ( NULL == pUSER ) {
		// °Å·¡ ´ë»óÀÌ ¾ø¾î Á³´Ù... °Å·¡ Ãë¼Ò ...
		this->Send_gsv_TRADE_P2P( this->m_iTradeUserIDX , RESULT_TRADE_NOT_TARGET );
		this->m_iTradeUserIDX = 0;
		return true;
	}

	if ( this->m_iTradeUserIDX != pUSER->Get_INDEX() || 
		 pUSER->m_iTradeUserIDX != this->Get_INDEX() ) {
		this->m_iTradeUserIDX = 0;
		return true;
	}

	tagITEM *pTradeITEM = &this->m_TradeITEM[ pPacket->m_cli_TRADE_P2P_ITEM.m_cTradeSLOT ].m_Item;
	if ( pPacket->m_cli_TRADE_P2P_ITEM.m_uiQuantity > 0 ) {
		// HACKING !!! :: 2004. 10. 11
		// pPacket->m_cli_TRADE_P2P_ITEM.m_nInventoryIndex == 0ÀÌ°í TRADE_MONEY_SLOT_NO != pPacket->m_cli_TRADE_P2P_ITEM.m_cTradeSLOT 
		// µµ·Ï Á¶ÀÛÇØ¼­ ÆÐÅ¶À» º¸³»´Â ³ÑÀÌ ÀÖÀ½. ÇìÅ·¿¡ ÀÇÇØ µ·ÀÌ µé¾î¿Â´Ù... ³ª»Û ½§~
		if ( TRADE_MONEY_SLOT_NO == pPacket->m_cli_TRADE_P2P_ITEM.m_cTradeSLOT ) {
			// µ·
			pPacket->m_cli_TRADE_P2P_ITEM.m_cTradeSLOT = TRADE_MONEY_SLOT_NO;
			pTradeITEM->m_cType = ITEM_TYPE_MONEY;

			if ( pPacket->m_cli_TRADE_P2P_ITEM.m_uiQuantity > this->GetCur_MONEY() ) {
				pPacket->m_cli_TRADE_P2P_ITEM.m_uiQuantity = (int)this->GetCur_MONEY();
			}
			pTradeITEM->m_uiMoney = pPacket->m_cli_TRADE_P2P_ITEM.m_uiQuantity;
		} else {
			// HACKING !!! :: 2005. 05. 30 °°Àº ÀÎº¥ÀÇ ¾ÆÀÌÅÛÀ» ´Ù¸¥ °Å·¡ ½½·Ô¿¡ µî·ÏÇÏ¿©...¾ÆÀÌÅÛ º¹»ç~
			for (short nT=0; nT<TRADE_MONEY_SLOT_NO; nT++) {
				if ( this->m_TradeITEM[ nT ].m_nInvIDX == pPacket->m_cli_TRADE_P2P_ITEM.m_nInventoryIndex ) {
					return IS_HACKING (this, "Recv_cli_TRADE_P2P_ITEM-3 : Duplicated inventory item" ); 
				}
			}

			// ÀÌ¹Ì °Å·¡¾ÆÀÌÅÛÀÌ ÀÖ´Â ½½·ÔÀÌ´Ù. ( ·ÎÁîÇØÄ¿ deebeeÀÇ ¸ÞÀÏ·Î ÀÎÇØ ¾Ë°ÔµÈ ÇØÅ·¹æ¹ý )
			if( this->m_TradeITEM[ pPacket->m_cli_TRADE_P2P_ITEM.m_cTradeSLOT ].m_Item.GetItemNO() != 0 ) 
				return IS_HACKING (this, "Recv_cli_TRADE_P2P_ITEM-4 : Invalid trade slot" ); 

			*pTradeITEM = this->m_Inventory.m_ItemLIST[ pPacket->m_cli_TRADE_P2P_ITEM.m_nInventoryIndex ];
			if ( !pTradeITEM->IsEnableDROP() ) {
				// 1:1 °Å·¡ ºÒ°¡ Ç°¸ñ..
				return true;
			}

			if ( 0 == pPacket->m_cli_TRADE_P2P_ITEM.m_nInventoryIndex || ITEM_TYPE_MONEY == pTradeITEM->GetTYPE() ) {
				// Hacking ... ÆÐÅ¶ Á¶ÀÛÀÎ°¡???
				IS_HACKING( this, "classUSER::Recv_cli_TRADE_P2P_ITEM( Hacking Money slot )" );
				this->SetCur_MONEY( 0 );
				return false;
			}

			if ( tagITEM::IsEnableDupCNT( pTradeITEM->GetTYPE() ) ) {
				if ( pPacket->m_cli_TRADE_P2P_ITEM.m_uiQuantity > pTradeITEM->GetQuantity() ) {
					// ¼ö·®ÀÌ ¸ðÀÚ¶ó´Ù.
					pPacket->m_cli_TRADE_P2P_ITEM.m_uiQuantity = pTradeITEM->GetQuantity();
				}

				pTradeITEM->m_uiQuantity = pPacket->m_cli_TRADE_P2P_ITEM.m_uiQuantity;
			}
		}
	} else {
		pTradeITEM->Clear();		// °Å·¡ Ç°¸ñ¿¡¼­ »¯´Ù...
	}

	this->m_TradeITEM[ pPacket->m_cli_TRADE_P2P_ITEM.m_cTradeSLOT ].m_nInvIDX = pPacket->m_cli_TRADE_P2P_ITEM.m_nInventoryIndex;

	pUSER->Send_gsv_TRADE_P2P_ITEM( pPacket->m_cli_TRADE_P2P_ITEM.m_cTradeSLOT, *pTradeITEM );

	return true;
}


//-------------------------------------------------------------------------------------------------
/// °³ÀÎ »óÁ¡ÀÌ ¿­·ÈÀ½À» Åëº¸
bool classUSER::Send_gsv_P_STORE_OPENED()
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_P_STORE_OPENED;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_P_STORE_OPENED );

	pCPacket->m_gsv_P_STORE_OPENED.m_wObjectIDX = this->Get_INDEX();

#ifdef	__INC_PLATINUM
	short nStoreSkin = this->m_GrowAbility.GetStoreSKIN( this->GetCurAbsSEC() );
	pCPacket->AppendData( &nStoreSkin, sizeof(short) );
#endif
	pCPacket->AppendString( this->m_szUserTITLE );

    this->GetZONE()->SendPacketToSectors( this, pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}


//-------------------------------------------------------------------------------------------------
/// ±¸ÀÔ Èñ¸Á ¾ÆÀÌÅÛÀ» µî·Ï
bool classUSER::Recv_cli_SET_WISHITEM( t_PACKET *pPacket )
{
	if ( tagITEM::IsValidITEM( &pPacket->m_cli_SET_WISHITEM.m_ITEM ) )
		return this->m_WishLIST.Set( pPacket->m_cli_SET_WISHITEM.m_btWishSLOT, pPacket->m_cli_SET_WISHITEM.m_ITEM );
	return true;
}


//-------------------------------------------------------------------------------------------------
/// °³ÀÎ »óÁ¡ ¿­±â ¿äÃ»À» ¹ÞÀ½
bool classUSER::Recv_cli_P_STORE_OPEN( t_PACKET *pPacket )
{
	if ( !(this->m_dwPayFLAG & PAY_FLAG_JP_TRADE) )
		return true;

	// PVPÁ¸¿¡¼­´Â °³ÀÎ»óÁ¡ ¿­¼ö ¾ø´Ù...
	if ( this->Get_ActiveSKILL() ) return true;	// ½ºÅ³ ÄÉ½ºÆÃ ÁßÀÏ¶© ¸ø¹Ù²ã
	if ( ZONE_PVP_STATE( this->m_nZoneNO ) && 11 != ZONE_PVP_STATE( this->m_nZoneNO ) ) return true;

	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;
	if ( this->Get_RideMODE() ) return true;

	if ( pPacket->m_cli_P_STORE_OPEN.m_btSellItemCNT >= MAX_P_STORE_ITEM_SLOT )
		return false;
	if ( pPacket->m_cli_P_STORE_OPEN.m_btWishItemCNT >= MAX_P_STORE_ITEM_SLOT )
		return false;

	if ( !this->SetCMD_STOP() || this->Get_RideMODE() )
		return true;

	// »óÁ¡ ¿­±â »óÅÂ·Î
	// Á¤Áö ¸í·É »óÅÂ·Î ?
	this->m_STORE.m_bActive = true;
	this->Set_COMMAND( CMD_STORE );

	this->m_IngSTATUS.ClearSubFLAG( FLAG_SUB_INTRO_CHAT );
	this->m_IngSTATUS.SetSubFLAG( FLAG_SUB_STORE_MODE );

	short nI, nJ, nK, nOffset;

	this->m_STORE.m_btSellCNT = 0;
	tagITEM *pInvITEM;
	for (nI=0; nI<pPacket->m_cli_P_STORE_OPEN.m_btSellItemCNT; nI++) {
		if ( pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nI ].m_btInvIDX >= INVENTORY_TOTAL_SIZE )
			continue;

		// ¹¹³Ä... ÇØÅ· ??
		if ( !tagITEM::IsValidITEM( &pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nI ].m_ITEM ) ) 
			continue;

		// °Å·¡ ÇÒ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÎ°¡ ???
		if ( !pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nI ].m_ITEM.IsEnableDROP() ) {
			// ¾ÆÀÌÅÛ °Å·¡ À¯ÇüÀÌ 2,3¹ø ÀÌ¸é °³ÀÎ »óÁ¡ °Å·¡ ºÒ°¡..
			this->m_STORE.m_SellITEM[ nI ].Clear();
			continue;
		}
		pInvITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nI ].m_btInvIDX ];
		if ( pInvITEM->GetHEADER() != pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nI ].m_ITEM.GetHEADER() ) {
			this->m_STORE.m_SellITEM[ nI ].Clear();
			continue;
		}

		if ( pInvITEM->IsEnableDupCNT() && pInvITEM->GetQuantity() > pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nI ].m_ITEM.GetQuantity() ) {
			// Áßº¹µÈ °¹¼ö¾ÆÀÌÅÛÀÌ°í ¼ÒÁö·®º¸´Ù Àû°Ô ÆÈ·Á°í ÇÏ¸é...
			this->m_STORE.m_SellITEM[ nI ] = pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nI ].m_ITEM;
		} else  {
			this->m_STORE.m_SellITEM[ nI ] = *pInvITEM;
		}

		this->m_STORE.m_nInvIDX			[ nI ] = pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nI ].m_btInvIDX;
		this->m_STORE.m_dwSellPricePerEA[ nI ] = pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nI ].m_dwPricePerEA;
		this->m_STORE.m_btSellCNT ++;
	}

	bool bValid;
	nJ = pPacket->m_cli_P_STORE_OPEN.m_btSellItemCNT;
	for (nI=0; nI<pPacket->m_cli_P_STORE_OPEN.m_btWishItemCNT; nI++, nJ++) {
		if ( pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nJ ].m_btWishIDX >= MAX_P_STORE_ITEM_SLOT ) {
			return IS_HACKING( this, "Recv_cli_P_STORE_OPEN :: IDX >= MAX_P_STORE_ITEM_SLOT" );
		}

		this->m_STORE.m_WishITEM[ nI ].Clear();

		// ¹¹³Ä... ÇØÅ· ??
		if ( !tagITEM::IsValidITEM( &pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nI ].m_ITEM ) ) 
			continue;

		if ( !pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nJ ].m_ITEM.IsEnableDROP() ) {
			// ¾ÆÀÌÅÛ °Å·¡ À¯ÇüÀÌ 2,3¹ø ÀÌ¸é °³ÀÎ »óÁ¡ °Å·¡ ºÒ°¡..
			continue;
		}

		bValid = true;
		// °°Àº ¹°°ÇÀ» ¼­·Î Æ²¸° °¡°Ý¿¡ ¿Ã·Á¼­...¾ÕÂÊÀÇ ½Ñ°¡°ÝÀ¸·Î ±¸¸ÅµÇ´Â ¹ö±×...
		for ( nK=0; nK<nI; nK++ ) {
			if ( this->m_STORE.m_WishITEM[ nK ].GetHEADER() == pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nJ ].m_ITEM.GetHEADER() && 
				 this->m_STORE.m_dwWishPricePerEA[ nK ]     != pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nJ ].m_dwPricePerEA ) {
				bValid = false;
				break;
			}
		}
		if ( bValid && pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nJ ].m_ITEM.IsValidITEM() ) {
			this->m_STORE.m_btWishIDX		[ nI ] = pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nJ ].m_btWishIDX;
			this->m_STORE.m_WishITEM		[ nI ] = pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nJ ].m_ITEM;
			this->m_STORE.m_dwWishPricePerEA[ nI ] = pPacket->m_cli_P_STORE_OPEN.m_ITEMs[ nJ ].m_dwPricePerEA;
			this->m_STORE.m_btWishIdx2StoreIDX[ m_STORE.m_btWishIDX[ nI ] ] = (BYTE)nI;
			if ( this->m_STORE.m_WishITEM[ nI ].IsEnableDupCNT() && this->m_STORE.m_WishITEM[ nI ].GetQuantity() > MAX_DUP_ITEM_QUANTITY ) {
				this->m_STORE.m_WishITEM[ nI ].m_uiQuantity = MAX_DUP_ITEM_QUANTITY;
			}
		}
	}
	this->m_STORE.m_btWishCNT = pPacket->m_cli_P_STORE_OPEN.m_btWishItemCNT;

	nOffset = sizeof( cli_P_STORE_OPEN ) + sizeof( tagPS_ITEM ) * ( pPacket->m_cli_P_STORE_OPEN.m_btSellItemCNT+pPacket->m_cli_P_STORE_OPEN.m_btWishItemCNT );

	char *szTitle = Packet_GetStringPtr( pPacket, nOffset );
	if ( strlen( szTitle ) >= MAX_USER_TITLE ) {
		strncpy( this->m_szUserTITLE, szTitle, MAX_USER_TITLE );
		this->m_szUserTITLE[ MAX_USER_TITLE ] = 0;
	} else {
		strcpy( this->m_szUserTITLE, szTitle );
	}

	return this->Send_gsv_P_STORE_OPENED ();
}


//-------------------------------------------------------------------------------------------------
/// °³ÀÎ »óÁ¡ Á¾·á...
bool classUSER::Recv_cli_P_STORE_CLOSE( t_PACKET *pPacket )
{
	if ( !this->m_STORE.m_bActive )
		return true;

    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_P_STORE_CLOSED;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_P_STORE_CLOSED );

	pCPacket->m_gsv_P_STORE_CLOSED.m_wObjectIDX = this->Get_INDEX();

    this->GetZONE()->SendPacketToSectors( this, pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	this->m_STORE.m_bActive = false;
	this->m_IngSTATUS.ClearSubFLAG( FLAG_SUB_STORE_MODE );

	// ÁÖº¯¿¡ Åëº¸ ¾øÀÌ Á¤Áö ¸í·É »óÅÂ·Î...
	this->CObjAI::SetCMD_STOP ();

	return true;
}


//-------------------------------------------------------------------------------------------------
/// °³ÀÎ »óÁ¡ ¹°Ç° ¸ñ·ÏÀ» ¿äÃ»
bool classUSER::Recv_cli_P_STORE_LIST_REQ( t_PACKET *pPacket )
{
	classUSER *pStoreOWNER = g_pObjMGR->Get_UserOBJ( pPacket->m_cli_P_STORE_LIST_REQ.m_wStoreObjectIDX );
	if ( pStoreOWNER ) {
		if ( !pStoreOWNER->m_STORE.m_bActive )
			return true;

		classPACKET *pCPacket = Packet_AllocNLock ();
		if ( !pCPacket )
			return false;

		BYTE btI, btItemCNT=0;
		for (btI=0; btI<pStoreOWNER->m_STORE.m_btSellCNT; btI++) {
			if ( !pStoreOWNER->m_STORE.m_SellITEM[ btI ].GetHEADER() )
				continue;
			
			pCPacket->m_gsv_P_STORE_LIST_REPLY.m_SlotITEMs[ btItemCNT ].m_btSLOT   = btI;
			pCPacket->m_gsv_P_STORE_LIST_REPLY.m_SlotITEMs[ btItemCNT ].m_SlotITEM = pStoreOWNER->m_STORE.m_SellITEM[ btI ];
			pCPacket->m_gsv_P_STORE_LIST_REPLY.m_SlotITEMs[ btItemCNT ].m_dwPRICE  = pStoreOWNER->m_STORE.m_dwSellPricePerEA[ btI ];

			btItemCNT ++;
		}
		pCPacket->m_gsv_P_STORE_LIST_REPLY.m_btSellItemCNT = btItemCNT;

		pCPacket->m_gsv_P_STORE_LIST_REPLY.m_btWishItemCNT = 0;
		for (btI=0; btI<pStoreOWNER->m_STORE.m_btWishCNT; btI++) {
			if ( !pStoreOWNER->m_STORE.m_WishITEM[ btI ].GetHEADER() ) 
				continue;

			if ( pStoreOWNER->GetCur_MONEY() >= pStoreOWNER->m_STORE.m_dwWishPricePerEA[ btI ] ) {
				pCPacket->m_gsv_P_STORE_LIST_REPLY.m_SlotITEMs[ btItemCNT ].m_btSLOT   = pStoreOWNER->m_STORE.m_btWishIDX[ btI ];
				pCPacket->m_gsv_P_STORE_LIST_REPLY.m_SlotITEMs[ btItemCNT ].m_SlotITEM = pStoreOWNER->m_STORE.m_WishITEM[ btI ];
				pCPacket->m_gsv_P_STORE_LIST_REPLY.m_SlotITEMs[ btItemCNT ].m_dwPRICE  = pStoreOWNER->m_STORE.m_dwWishPricePerEA[ btI ];

				btItemCNT ++;
				pCPacket->m_gsv_P_STORE_LIST_REPLY.m_btWishItemCNT ++;
			}
		}

		pCPacket->m_HEADER.m_wType = GSV_P_STORE_LIST_REPLY;
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_P_STORE_LIST_REPLY ) + sizeof(tagPS_SLOT_PRICE) * btItemCNT;

		this->SendPacket( pCPacket );

		Packet_ReleaseNUnlock( pCPacket );
	} else 
		return this->Send_gsv_P_STORE_RESULT( pPacket->m_cli_P_STORE_LIST_REQ.m_wStoreObjectIDX, RESULT_P_STORE_CANCLED );

	return true;
}


//-------------------------------------------------------------------------------------------------
/// °³ÀÎ »óÁ¡°Å·¡ ÆÐÅ¶ ÃÊ±âÈ­...
classPACKET *classUSER::Init_gsv_P_STORE_RESULT( WORD wObjectIDX )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( pCPacket ) {
		pCPacket->m_HEADER.m_wType = GSV_P_STORE_RESULT;
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_P_STORE_RESULT );

		pCPacket->m_gsv_P_STORE_RESULT.m_wStoreObjectIDX = wObjectIDX;
		pCPacket->m_gsv_P_STORE_RESULT.m_btItemCNT = 0;
	}

	return pCPacket;
}
/*
bool classUSER::Send_gsv_P_STORE_RESULT( classPACKET *pCPacket, BYTE btResult )
{
	if ( pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ) {
		// º¯°æµÈ ÀÎº¥Åä¸®°¡ ÀÖÀ»°æ¿ì¸¸ Àü¼ÛÇÑ´Ù.
		pCPacket->m_HEADER.m_nSize += ( pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT * sizeof( tagPS_SLOT_ITEM ) );
	}

	pCPacket->m_gsv_P_STORE_RESULT.m_btResult = btResult;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}
*/
/// °³ÀÎ »óÁ¡ °Å·¡ °á°ú ÆÐÅ¶ Åëº¸
bool classUSER::Send_gsv_P_STORE_RESULT( WORD wObjectIDX, BYTE btResult )
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_P_STORE_RESULT;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_P_STORE_RESULT );

	pCPacket->m_gsv_P_STORE_RESULT.m_wStoreObjectIDX = wObjectIDX;
	pCPacket->m_gsv_P_STORE_RESULT.m_btResult = btResult;

	this->SendPacket( pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}


//-------------------------------------------------------------------------------------------------
/// °³ÀÎ »óÁ¡¿¡¼­ ¹°Ç° ±¸ÀÔ Èñ¸Á...
bool classUSER::Recv_cli_P_STORE_BUY_REQ( t_PACKET *pPacket )
{
	if ( pPacket->m_cli_P_STORE_BUY_REQ.m_btItemCNT >= MAX_P_STORE_ITEM_SLOT ) {
		return IS_HACKING( this, "Recv_cli_P_STORE_BUY_REQ :: m_btItemCNT >= MAX_P_STORE_ITEM_SLOT" );
	}
	if ( this->m_iIndex == pPacket->m_cli_P_STORE_BUY_REQ.m_wStoreObjectIDX ) {
		return IS_HACKING( this, "Recv_cli_P_STORE_BUY_REQ :: Trade from self private store" );
	}

	// 1:1°Å·¡Áß »óÁ¡ °Å·¡Áß ±ÝÁö..
	if ( this->m_btTradeBIT & (BIT_TRADE_READY|BIT_TRADE_DONE) )	return true;

	classUSER *pStoreOWNER = g_pObjMGR->Get_UserOBJ( pPacket->m_cli_P_STORE_BUY_REQ.m_wStoreObjectIDX );
	if ( pStoreOWNER ) {
		if ( !pStoreOWNER->m_STORE.m_bActive )
			return true;

		classPACKET *pStorePacket  = Init_gsv_P_STORE_RESULT( pPacket->m_cli_P_STORE_BUY_REQ.m_wStoreObjectIDX );
		classPACKET *pBuyerPacket  = Init_gsv_SET_MONEYnINV ( GSV_P_STORE_MONEYnINV );
		classPACKET *pSellerPacket = Init_gsv_SET_MONEYnINV ( GSV_P_STORE_MONEYnINV );

		BYTE btStoreSLOT, btTradeRESULT;
		tagITEM *pInvSellITEM, *pSlotITEM, sBuyITEM, sSubITEM;
		__int64 biNeedMoney;	// 0xffffffff * °¹¼ö -> DWORD°ªÀÌ ¹üÀ§ ÃÊ°úÇØ¼­ ÀûÀº ±Ý¾×À¸·Î ¼±Á¤µÇ´ø ¹ö±× ¼öÁ¤..
		short nBuyerInvIDX;

		btTradeRESULT = RESULT_P_STORE_BOUGHT_ALL;
		for (short nI=0; nI<pPacket->m_cli_P_STORE_BUY_REQ.m_btItemCNT; nI++) {
			// °³ÀÎ »óÁ¡¿¡¼­ ¹°°ÇÀ» ±¸¸Å...
			btStoreSLOT	= pPacket->m_cli_P_STORE_BUY_REQ.m_BuyITEMs[ nI ].m_btSLOT;
			if ( btStoreSLOT >= MAX_P_STORE_ITEM_SLOT ) {
				return IS_HACKING( this, "Recv_cli_P_STORE_BUY_REQ :: btStoreSLOT >= MAX_P_STORE_ITEM_SLOT" );
			}

			pSlotITEM	= &pStoreOWNER->m_STORE.m_SellITEM[ btStoreSLOT ];
			if ( !pSlotITEM->GetTYPE() ) {
				// ´Ù ÆÈ·È´Ù.
				btTradeRESULT = RESULT_P_STORE_BOUGHT_PART;
				continue;
			}

			pInvSellITEM = &pStoreOWNER->m_Inventory.m_ItemLIST[ pStoreOWNER->m_STORE.m_nInvIDX[ btStoreSLOT ] ];
			sBuyITEM = pPacket->m_cli_P_STORE_BUY_REQ.m_BuyITEMs[ nI ].m_SlotITEM;
			if ( pInvSellITEM->GetHEADER() != sBuyITEM.GetHEADER() ) {
				// ¹¹³Ä ÀÌ°Ç ?? 
				continue;
			}

			if ( pSlotITEM->IsEnableDupCNT() ) {
				// Áßº¹ °¹¼ö ¾ÆÀÌÅÛÀÏ°æ¿ì...
				if ( pInvSellITEM->GetQuantity() < pSlotITEM->GetQuantity() ) {
					// ¼ÒÁöÇÑ°Å º¸´Ù ÆÇ¸ÅÇÏ·Á´Â°ÍÀÌ ¸¹´Ù¸é...
					pSlotITEM->m_uiQuantity = pInvSellITEM->GetQuantity();
				}
				if ( pSlotITEM->GetQuantity() < sBuyITEM.GetQuantity() ) {
					// °¹¼ö°¡ ºÎÁ·ÇÏ´Ù, °¹¼ö ¼öÁ¤..
					sBuyITEM.m_uiQuantity = pSlotITEM->GetQuantity();
					btTradeRESULT = RESULT_P_STORE_BOUGHT_PART;
				}

				biNeedMoney = ( (__int64)( pStoreOWNER->m_STORE.m_dwSellPricePerEA[ btStoreSLOT ] ) * sBuyITEM.GetQuantity() );
			} else {
				sBuyITEM.m_dwBody = pInvSellITEM->m_dwBody;		// ¸Þ¸ð¸® Á¶ÀÛÀ¸·Î ¿É¼ÇÀÌ ¹Ù²î´Â°É ¹æÁö
				// ÀåºñÀÏ °æ¿ì ¾ÆÀÌÅÛ ½Ã¸®¾ó...
				sBuyITEM.m_iSN = pInvSellITEM->m_iSN;
				biNeedMoney = (__int64)( pStoreOWNER->m_STORE.m_dwSellPricePerEA[ btStoreSLOT ] );
			}
			if ( this->GetCur_MONEY() < biNeedMoney ) {
				// ±¸¸Å ¿äÃ»Àü¿¡ µ· °è»êÇØ¼­ º¸³»¾ßÁö..
				// btTradeRESULT |= RESULT_P_STORE_NEED_MONEY;
				continue;
			}
			
			// ¾ÆÀÌÅÛ ÀÌµ¿...
			sSubITEM = sBuyITEM;
			nBuyerInvIDX = this->Add_ITEM( sBuyITEM );		// pBuyITEM = È£ÃâÈÄ ´õÇØÁø °á°úÀÇ ÀÎº¥Åä¸® ¾ÆÀÌÅÛ
			if ( nBuyerInvIDX > 0 ) {
				pInvSellITEM->SubtractOnly( sSubITEM );
				pSlotITEM->SubtractOnly( sSubITEM );

				this->m_Inventory.m_i64Money -= biNeedMoney;
				pStoreOWNER->m_Inventory.m_i64Money += biNeedMoney;

				// ±¸ÀÔÀÚ ÀÎº¥ °»½Å
				pBuyerPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pBuyerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_btInvIDX = (BYTE)nBuyerInvIDX;
				pBuyerPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pBuyerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_ITEM = sBuyITEM;
				pBuyerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ++;

				// ÆÇ¸ÅÀÚ ÀÎº¥ °»½Å
				pSellerPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pSellerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_btInvIDX = (BYTE)pStoreOWNER->m_STORE.m_nInvIDX[ btStoreSLOT ];
				pSellerPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pSellerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_ITEM = *pInvSellITEM;
				pSellerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ++;

				// »óÁ¡ ¹°Ç°ÀÇ °»½ÅµÈ ³»¿ª...
				pStorePacket->m_gsv_P_STORE_RESULT.m_UpdatedITEM[ pStorePacket->m_gsv_P_STORE_RESULT.m_btItemCNT ].m_btSLOT   = btStoreSLOT;
				pStorePacket->m_gsv_P_STORE_RESULT.m_UpdatedITEM[ pStorePacket->m_gsv_P_STORE_RESULT.m_btItemCNT ].m_SlotITEM = *pSlotITEM;
				pStorePacket->m_gsv_P_STORE_RESULT.m_btItemCNT ++;

				// °³ÀÎ »óÁ¡ °Å·¡ ·Î±×...
				#ifdef	__NEW_LOG
					g_pThreadLOG->When_TagItemLOG( LIA_P2PTRADE, pStoreOWNER, &sSubITEM, 0, biNeedMoney, this);
				#else
					g_pThreadLOG->When_P2PTRADE( pStoreOWNER, &sSubITEM, this, biNeedMoney );
				#endif
			} /* else {
				ÀÎº¥Åä¸® ¸ðÀÚ¶÷... ÀÌ ¾ÆÀÌÅÛ °Å·¡ Ãë¼Ò...
			} */
		}

		this->Send_gsv_SET_MONEYnINV ( pBuyerPacket );		// ±¸¸ÅÀÚ´Â ÀÎº¥Åä¸®,µ· ÆÐÅ¶À¸·Î º¯°æ Á¤º¸ Àü¼Û.
		pStoreOWNER->Send_gsv_SET_MONEYnINV ( pSellerPacket );

		// »óÁ¡ ¾ÆÀÌÅÛ ½½·Ô °»½Å
		pStorePacket->m_gsv_P_STORE_RESULT.m_btResult = btTradeRESULT;
		if ( pStorePacket->m_gsv_P_STORE_RESULT.m_btItemCNT ) {
			// °³·¡µÈ Ç°¸ñÀÌ ÀÖ´Ù¸é...
			pStorePacket->m_HEADER.m_nSize += ( pStorePacket->m_gsv_P_STORE_RESULT.m_btItemCNT * sizeof( tagPS_SLOT_ITEM ) );
			pStoreOWNER->SendPacket( pStorePacket );
		}
		this->SendPacket( pStorePacket );

		Packet_ReleaseNUnlock( pStorePacket );
	} else {
		return this->Send_gsv_P_STORE_RESULT( pPacket->m_cli_P_STORE_BUY_REQ.m_wStoreObjectIDX, RESULT_P_STORE_CANCLED );
	}

    return true;
}


//-------------------------------------------------------------------------------------------------
/// °³ÀÎ »óÁ¡¿¡ ¹°Ç° ÆÇ¸Å Èñ¸Á...
bool classUSER::Recv_cli_P_STORE_SELL_REQ( t_PACKET *pPacket )
{
	if ( pPacket->m_cli_P_STORE_SELL_REQ.m_btItemCNT >= MAX_P_STORE_ITEM_SLOT ) {
		return IS_HACKING( this, "Recv_cli_P_STORE_SELL_REQ :: m_btItemCNT >= MAX_P_STORE_ITEM_SLOT" );
	}
	if ( this->m_iIndex == pPacket->m_cli_P_STORE_SELL_REQ.m_wStoreObjectIDX ) {
		return IS_HACKING( this, "Recv_cli_P_STORE_SELL_REQ :: Trade from self private store" );
	}

	// 1:1°Å·¡Áß »óÁ¡ °Å·¡Áß ±ÝÁö..
	if ( this->m_btTradeBIT & (BIT_TRADE_READY|BIT_TRADE_DONE) )	return true;

	classUSER *pStoreOWNER = g_pObjMGR->Get_UserOBJ( pPacket->m_cli_P_STORE_SELL_REQ.m_wStoreObjectIDX );

	if ( pStoreOWNER ) {
		if ( !pStoreOWNER->m_STORE.m_bActive )
			return true;

		classPACKET *pStorePacket  = Init_gsv_P_STORE_RESULT( pPacket->m_cli_P_STORE_SELL_REQ.m_wStoreObjectIDX );
		classPACKET *pBuyerPacket  = Init_gsv_SET_MONEYnINV( GSV_P_STORE_MONEYnINV );
		classPACKET *pSellerPacket = Init_gsv_SET_MONEYnINV( GSV_P_STORE_MONEYnINV );

		BYTE btStoreSLOT, btTradeRESULT;
		tagITEM *pSellerITEM, *pWishITEM, sToSellITEM, sSubITEM;
		DWORD dwNeedMoney;
		short nInvIDX;

		btTradeRESULT = RESULT_P_STORE_SOLD_ALL;
		for (short nI=0; nI<pPacket->m_cli_P_STORE_SELL_REQ.m_btItemCNT; nI++) {
			if ( pPacket->m_cli_P_STORE_SELL_REQ.m_SellITEMs[ nI ].m_btWishSLOT >= MAX_P_STORE_ITEM_SLOT ) {
				return IS_HACKING( this, "Recv_cli_P_STORE_SELL_REQ :: m_btWishSLOT >= MAX_P_STORE_ITEM_SLOT" );
			}
			// °³ÀÎ »óÁ¡¿¡ ¹°°ÇÀ» ÆÈÀÚ ...
			btStoreSLOT = pStoreOWNER->m_STORE.m_btWishIdx2StoreIDX[ pPacket->m_cli_P_STORE_SELL_REQ.m_SellITEMs[ nI ].m_btWishSLOT ];

			pWishITEM   = &pStoreOWNER->m_STORE.m_WishITEM[ btStoreSLOT ];
			pSellerITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_P_STORE_SELL_REQ.m_SellITEMs[ nI ].m_btInvIDX ];
			if ( pWishITEM->GetHEADER() != pSellerITEM->GetHEADER() )
				continue;

			sToSellITEM = pPacket->m_cli_P_STORE_SELL_REQ.m_SellITEMs[ nI ].m_SellITEM;
			if ( pWishITEM->GetHEADER() != sToSellITEM.GetHEADER() )
				continue;

			if ( pWishITEM->IsEnableDupCNT() ) {
				// Áßº¹ °¹¼ö ¾ÆÀÌÅÛÀÏ°æ¿ì...
				if ( sToSellITEM.GetQuantity() > pWishITEM->GetQuantity() ) {
					// ÆÈ·Á´Â°ÍÀÌ ±¸¸ÅÈñ¸Á·® º¸´Ù ¸¹´Ù - ÆÇ¸Å°¹¼ö º¸Á¤.
					sToSellITEM.m_uiQuantity = pWishITEM->GetQuantity();
					btTradeRESULT = RESULT_P_STORE_SOLD_PART;
				}
				if ( sToSellITEM.GetQuantity() > pSellerITEM->GetQuantity() ) {
					// ÆÈ·Á´Â°ÍÀÌ ¼ÒÁö·® º¸´Ù ¸¹´Ù :: ¸ÞÅ©·Î¿¡ ÀÇÇÑ ¼ýÀÚ Á¶ÀÛÀ¸·Î ÇìÅ· ¹ß»ý...
					continue;
				}
				dwNeedMoney = pStoreOWNER->m_STORE.m_dwWishPricePerEA[ btStoreSLOT ] * sToSellITEM.GetQuantity();
			} else {
				// Àåºñ ¾ÆÀÌÅÛÀÏ °æ¿ì ¿É¼Ç±îÁö °°ÀºÁö Ã¼Å©...
				if ( 0 == pSellerITEM->GetLife() ) {
					// ¼ö¸íÀÌ 0ÀÎ°Ç ¸ø¾²´Â ¾ÆÀÌÅÛ¾ß...
					continue;
				}
				if ( pWishITEM->HasSocket() && pWishITEM->HasSocket() != pSellerITEM->HasSocket() ) {
					// ±¸¸ÅÀÚ°¡ ¼ÒÄÏ ÀÖ´Â ¾ÆÀÌÅÛÀ» ¿øÇÏ´Âµ¥... ¼ÒÄÏÀÌ ÀÏÄ¡ ÇÏÁö ¾Ê³×
					continue;
				}
				if ( pWishITEM->GetDurability() > pSellerITEM->GetDurability() ) {
					// ±¸¸ÅÀÚ°¡ ¿øÇÏ´Â ¾ÆÀÌÅÛÀÇ ³»±¸µµ º¸´Ù ³·À½ ¸øÆÈ¾Æ¿ä~~
					continue;
				}
				if ( pWishITEM->GetGrade() > pSellerITEM->GetGrade() ) {
					// ±¸¸ÅÀÚ°¡ ¿øÇÏ´Â ¾ÆÀÌÅÛÀÇ µî±Þº¸´Ù ³·À½ ¸øÆÈ¾Æ¿ä~~
					continue;
				}
				if ( pWishITEM->GetOption() && pWishITEM->GetOption() != pSellerITEM->GetOption() ) {
					// ±¸¸ÅÀÚ°¡ ¿øÇÏ´Â ¾ÆÀÌÅÛ¿¡ ¿É¼ÇÀÖ°í ¿É¼ÇÀÌ Æ²¸®¸é ¸øÆÈ¾Æ¿ä~
					continue;
				}
				sToSellITEM.m_dwBody = pSellerITEM->m_dwBody;		// ¸Þ¸ð¸® Á¶ÀÛÀ¸·Î ¿É¼ÇÀÌ ¹Ù²î´Â°É ¹æÁö
				// Àåºñ ¾ÆÀÌÅÛÀÏ °æ¿ì ½Ã¸®¾ó ¹øÈ£...
				sToSellITEM.m_iSN = pSellerITEM->m_iSN;
				dwNeedMoney = pStoreOWNER->m_STORE.m_dwWishPricePerEA[ btStoreSLOT ];
			}

			if ( pStoreOWNER->GetCur_MONEY() < dwNeedMoney ) {
				// ±¸¸Å »óÁ¡À» ¿­±â Àü¿¡ ¹Ýµå½Ã µ·ÀÌ ÀÖ´ÂÁö Ã¼Å©...
				#pragma COMPILE_TIME_MSG( "±¸ÀÔ ÇÏ·Á´Â ³ÑÀÌ µ·ÀÌ ¸ðÀÚ¶ó´Ù :: »óÁ¡ Á¢¾î ??.. Ç°¸ñ »èÁ¦?? " )
				// btTradeRESULT |= RESULT_P_STORE_NEED_MONEY;
				continue;
			}

			sSubITEM = sToSellITEM;
			nInvIDX  = pStoreOWNER->Add_ITEM( sToSellITEM );
			if ( nInvIDX > 0 ) {
				pStoreOWNER->m_Inventory.m_i64Money -= dwNeedMoney;
				this->m_Inventory.m_i64Money += dwNeedMoney;
				pSellerITEM->SubtractOnly( sSubITEM );		// ÆÈ¸° ¾ç¸¸Å­ Á¦°Å
				pWishITEM->SubtractOnly( sSubITEM );		// ±¸¸ÅÇÑ ¾ç¸¸Å­ Á¦°Å

				pSellerPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pSellerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_btInvIDX = pPacket->m_cli_P_STORE_SELL_REQ.m_SellITEMs[ nI ].m_btInvIDX;
				pSellerPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pSellerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_ITEM = *pSellerITEM;
				pSellerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ++;

				pBuyerPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pBuyerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_btInvIDX = (BYTE)nInvIDX;
				pBuyerPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ pBuyerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ].m_ITEM = pStoreOWNER->m_Inventory.m_ItemLIST[ nInvIDX ];
				pBuyerPacket->m_gsv_SET_MONEYnINV.m_btItemCNT ++;

				// »óÁ¡ ¹°Ç°ÀÇ °»½ÅµÈ ³»¿ª...
				pStorePacket->m_gsv_P_STORE_RESULT.m_UpdatedITEM[ pStorePacket->m_gsv_P_STORE_RESULT.m_btItemCNT ].m_btSLOT = pPacket->m_cli_P_STORE_SELL_REQ.m_SellITEMs[ nI ].m_btWishSLOT;
				pStorePacket->m_gsv_P_STORE_RESULT.m_UpdatedITEM[ pStorePacket->m_gsv_P_STORE_RESULT.m_btItemCNT ].m_SlotITEM = *pWishITEM;
				pStorePacket->m_gsv_P_STORE_RESULT.m_btItemCNT ++;

				// °³ÀÎ »óÁ¡ °Å·¡ ·Î±×...
				#ifdef	__NEW_LOG
					g_pThreadLOG->When_TagItemLOG( LIA_P2PTRADE, this, &sSubITEM, 0, dwNeedMoney, pStoreOWNER);
				#else
					g_pThreadLOG->When_P2PTRADE( this, &sSubITEM, pStoreOWNER, dwNeedMoney );
				#endif
			} /* else {
				±¸¸ÅÀÚ ÀÎº¥ ²ËÂü.. °Å·¡¿¡¼­ ÀÌ ¾ÆÀÌÅÛ Á¦¿Ü~~~
			} */
		}

		this->Send_gsv_SET_MONEYnINV( pSellerPacket );			// ÆÇ¸ÅÀÚ ÀÎº¥Åä¸®,µ· ÆÐÅ¶À¸·Î º¯°æ Á¤º¸ Àü¼Û.
		pStoreOWNER->Send_gsv_SET_MONEYnINV( pBuyerPacket );	// ±¸¸ÅÀÚ(»óÁ¡ÁÖÀÎ) Ãß°¡µÈ ¾ÆÀÌÅÛ ÀÎº¥Åä¸® Á¤º¸, ±¸¸ÅÀÚ °¨¼ÒµÈ µ· Á¤º¸ Àü¼Û

		// »óÁ¡ ¾ÆÀÌÅÛ ½½·Ô °»½Å
		pStorePacket->m_gsv_P_STORE_RESULT.m_btResult = btTradeRESULT;
		if ( pStorePacket->m_gsv_P_STORE_RESULT.m_btItemCNT ) {
			// °³·¡µÈ Ç°¸ñÀÌ ÀÖ´Ù¸é...
			pStorePacket->m_HEADER.m_nSize += ( pStorePacket->m_gsv_P_STORE_RESULT.m_btItemCNT * sizeof( tagPS_SLOT_ITEM ) );
			pStoreOWNER->SendPacket( pStorePacket );
		}
		this->SendPacket( pStorePacket );

		Packet_ReleaseNUnlock( pStorePacket );
	} else {
		return this->Send_gsv_P_STORE_RESULT( pPacket->m_cli_P_STORE_BUY_REQ.m_wStoreObjectIDX, RESULT_P_STORE_CANCLED );
	}

    return true;
}


//-------------------------------------------------------------------------------------------------
/// ½ºÅ³ÀÇ ·¾¾÷ ¿äÃ»
bool classUSER::Recv_cli_SKILL_LEVELUP_REQ ( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return true;
	if ( pPacket->m_cli_SKILL_LEVELUP_REQ.m_btSkillSLOT >= MAX_LEARNED_SKILL_CNT )
		return false;

    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_SKILL_LEVELUP_REPLY;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_SKILL_LEVELUP_REPLY );

	short nSkillIDX = this->m_Skills.m_nSkillINDEX[ pPacket->m_cli_SKILL_LEVELUP_REQ.m_btSkillSLOT ];

	BYTE btResult = this->Skill_LevelUpCondition( nSkillIDX, pPacket->m_cli_SKILL_LEVELUP_REQ.m_nNextLevelSkillIDX );
	if ( RESULT_SKILL_LEVELUP_SUCCESS == btResult ) {
		// 05.05.25 ½ºÅ³ ·¾¾÷½Ã ÁÙ¸® ¼Ò¸ð...
		int iNeedZuly = IsTAIWAN() ? SKILL_LEVELUP_NEED_ZULY( pPacket->m_cli_SKILL_LEVELUP_REQ.m_nNextLevelSkillIDX ) * 100 : 0;

		if ( this->GetCur_MONEY() < iNeedZuly ) {
			btResult = RESULT_SKILL_LEVELUP_OUTOFZULY;
		} else {
			this->SetCur_MONEY( this->GetCur_MONEY() - iNeedZuly );	// ¼Ò¸ð ºñ¿ë Àû¿ë

			#ifdef	__NEW_LOG
				g_pThreadLOG->When_LearnSKILL( this, pPacket->m_cli_SKILL_LEVELUP_REQ.m_nNextLevelSkillIDX );
			#else
				g_pThreadLOG->When_LevelUpSKILL( this, pPacket->m_cli_SKILL_LEVELUP_REQ.m_nNextLevelSkillIDX );
			#endif

			switch( this->Skill_LEARN( pPacket->m_cli_SKILL_LEVELUP_REQ.m_btSkillSLOT, pPacket->m_cli_SKILL_LEVELUP_REQ.m_nNextLevelSkillIDX ) ) {
				case 0x03 :
					this->Cal_BattleAbility ();
				case 0x01 :
					this->Update_SPEED ();
					this->Send_gsv_SPEED_CHANGED ();
			}

			pCPacket->m_gsv_SKILL_LEVELUP_REPLY.m_btSkillSLOT = pPacket->m_cli_SKILL_LEVELUP_REQ.m_btSkillSLOT;
			pCPacket->m_gsv_SKILL_LEVELUP_REPLY.m_nSkillINDEX = this->m_Skills.m_nSkillINDEX[ pPacket->m_cli_SKILL_LEVELUP_REQ.m_btSkillSLOT ];
		}
	}
	pCPacket->m_gsv_SKILL_LEVELUP_REPLY.m_btResult = btResult;
	pCPacket->m_gsv_SKILL_LEVELUP_REPLY.m_nSkillPOINT = this->GetCur_SkillPOINT ();

	this->SendPacket( pCPacket );

    Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ¼¿ÇÁ ½ºÅ³ÀÎÁö Ã¼Å©...
bool classUSER::Is_SelfSKILL( short nSkillIDX )
{
	switch ( SKILL_TYPE( nSkillIDX ) ) {
	//	case SKILL_TYPE_01 :	// Á¡ÇÁ, °øÁßºÎ¾ç.
		case SKILL_TYPE_08 :	// ´É·ÂÄ¡ÀÇ Áõ/°¨À» ÀÏÁ¤½Ã°£ Áö¼Ó(¼¿ÇÁ)
		case SKILL_TYPE_10 :	// ´É·ÂÄ¡ º¯°æÇü(¼¿ÇÁ)	:	ÇÑ¹æ¿¡ ¿Ã¸²
		case SKILL_TYPE_12 :	// »óÅÂ Áö¼ÓÇü(¼¿ÇÁ)	Áßµ¶, º¡¾î¸®, ±âÀý, Åõ¸í, ¹æÆÐµ¥¹ÌÁö, Ãß°¡ µ¥¹ÌÁö
		case SKILL_TYPE_14 :	// ¼ÒÈ¯...
		case SKILL_TYPE_17 :	// self & damage to scope...
			return true;
	} 
	return false;
}
/// Å¸°Ù ½ºÅ³ÀÎÁö Ã¼Å©...
bool classUSER::Is_TargetSKILL( short nSkillIDX )
{
	switch ( SKILL_TYPE( nSkillIDX ) ) {
		case SKILL_TYPE_03 :	// °ø°Ý µ¿ÀÛ º¯°æÇü
		case SKILL_TYPE_04 :	// ¹«±â »óÅÂ º¯°æÇü
		case SKILL_TYPE_05 :	// ÃÑ¾Ë º¯°æÇë °ø°ÝÇü

		case SKILL_TYPE_06 :	// ¹ß»ç ¸¶¹ýÇü
		case SKILL_TYPE_09 :	// ´É·ÂÄ¡ÀÇ Áõ/°¨À» ÀÏÁ¤½Ã°£ Áö¼Ó(Å¸°Ù)
		case SKILL_TYPE_11 :	// ´É·ÂÄ¡ º¯°æÇü(Å¸°Ù)	:	ÇÑ¹æ¿¡ ¿Ã¸²
		case SKILL_TYPE_13 :	// »óÅÂ Áö¼ÓÇü(Å¸°Ù)	Áßµ¶, º¡¾î¸®, ±âÀý, Åõ¸í, ¹æÆÐµ¥¹ÌÁö, Ãß°¡ µ¥¹ÌÁö
		case SKILL_TYPE_19 :	// (Å¸°Ù¿¡ µ¥¹ÌÁö°ø°ÝÀ» ÇÏ¸é¼­ HP,MP »¯¾î¿À±â °³³ä)
		case SKILL_TYPE_20 :	// ½ÃÃ¼ ºÎÈ°
			break;

/*
		case SKILL_TYPE_08 :	// ´É·ÂÄ¡ÀÇ Áõ/°¨À» ÀÏÁ¤½Ã°£ Áö¼Ó(¼¿ÇÁ)
		case SKILL_TYPE_10 :	// ´É·ÂÄ¡ º¯°æÇü(¼¿ÇÁ)	:	ÇÑ¹æ¿¡ ¿Ã¸²
		case SKILL_TYPE_12 :	// »óÅÂ Áö¼ÓÇü(¼¿ÇÁ)	Áßµ¶, º¡¾î¸®, ±âÀý, Åõ¸í, ¹æÆÐµ¥¹ÌÁö, Ãß°¡ µ¥¹ÌÁö
			return false;
		default :
			if ( SA_TARGET_STOP   == SKILL_ACTION_MODE( nSkillIDX ) ||
				 SA_TARGET_ATTACK == SKILL_ACTION_MODE( nSkillIDX ) )
				break;
*/
		default :
			return false;
	} 
	return true;
}

//-------------------------------------------------------------------------------------------------

#define	SKILL_DELAY_TIME	500	// 0.5 sec
/// °³ÀÎ ½ºÅ³ ½ÃÀÛ ¿äÃ» ¹ÞÀ½
bool classUSER::Recv_cli_SELF_SKILL( t_PACKET *pPacket )
{
	if ( !(this->m_dwPayFLAG & PLAY_FLAG_BATTLE) ) {
		// °ø°Ý ¸øÇØ... µ·³»¾ßÇÔ
		return true;
	}

#ifdef	__INC_WORLD
	char szTmp[ MAX_PATH ];
#endif

	DWORD dwCurTime = this->GetZONE()->GetCurrentTIME();
	if ( SKILL_DELAY_TIME > dwCurTime - this->m_dwLastSkillActiveTIME ) {
#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore Skill:: Delay:%d, Charged:%d", SKILL_DELAY_TIME, dwCurTime - this->m_dwLastSkillActiveTIME );
		this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
#endif
		return true;
	}

	if ( this->Get_ActiveSKILL() ) {
#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore Skill in casting motion" );
		this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
#endif
		return true;	// ½ºÅ³ ÄÉ½ºÆÃ ÁßÀÏ¶© ¸ø¹Ù²ã
	}
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() || this->m_IngSTATUS.IsSET( FLAG_ING_DUMB ) ) 
	{
#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore Skill:: not allowd status" );
		this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
#endif
		return true;
	}

	if ( pPacket->m_cli_SELF_SKILL.m_btSkillSLOT >= MAX_LEARNED_SKILL_CNT ) {
#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore Skill:: invalid skill slot %d / %d", pPacket->m_cli_SELF_SKILL.m_btSkillSLOT, MAX_LEARNED_SKILL_CNT);
		this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
#endif
		return false;
	}

	short nSkillIDX = this->m_Skills.m_nSkillINDEX[ pPacket->m_cli_SELF_SKILL.m_btSkillSLOT ];
	if ( IsTAIWAN() ) {
		if ( SKILL_AVAILBLE_STATUS(nSkillIDX) && !(m_btRideATTR & SKILL_AVAILBLE_STATUS(nSkillIDX)) )
			return true;

		if ( SKILL_RELOAD_TYPE(nSkillIDX) ) {
			// 05.05.30 ½ºÅ³ ¸®·Îµå ±×·ìÅ¸ÀÔÀ¸·Î Ã¼Å©...
			if ( SKILL_RELOAD_SECOND(nSkillIDX) > dwCurTime - this->m_dwLastSkillGroupSpeelTIME[ SKILL_RELOAD_TYPE(nSkillIDX) ] ) {
	#ifdef	__INC_WORLD
				sprintf( szTmp, "ignore SkillIdx:%d, :: Delay:%d > Charged:%d, LastStamp:%d \n", 
						nSkillIDX, 
						SKILL_RELOAD_SECOND(nSkillIDX), 
						dwCurTime - this->m_dwLastSkillGroupSpeelTIME[ pPacket->m_cli_SELF_SKILL.m_btSkillSLOT ], 
						this->m_dwLastSkillGroupSpeelTIME[ pPacket->m_cli_SELF_SKILL.m_btSkillSLOT ] );
				this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
	#endif
				return true;
			}
		}
	}

	if ( SKILL_RELOAD_SECOND(nSkillIDX) > dwCurTime - this->m_dwLastSkillSpellTIME[ pPacket->m_cli_SELF_SKILL.m_btSkillSLOT ] ) {
		// ´Ù½Ã »ç¿ë°¡´ÉÇÑ ½Ã°£ÀÎÁö Ã¼Å©...
#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore SkillIdx:%d, :: Delay:%d > Charged:%d, LastStamp:%d \n", 
				nSkillIDX, 
				SKILL_RELOAD_SECOND(nSkillIDX), 
				dwCurTime - this->m_dwLastSkillSpellTIME[ pPacket->m_cli_SELF_SKILL.m_btSkillSLOT ], 
				this->m_dwLastSkillSpellTIME[ pPacket->m_cli_SELF_SKILL.m_btSkillSLOT ] );
		this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
#endif
		return true;
	}

	if ( this->Get_WeightRATE() >= WEIGHT_RATE_CANT_ATK ) {
		// ¹«°Ì´Ù.. ¸í·É ºÒ°¡...
		if ( ( SKILL_TYPE( nSkillIDX ) >= 3 && SKILL_TYPE( nSkillIDX ) <= 13 ) ||
			   SKILL_TYPE( nSkillIDX ) == 17 ) {
#ifdef	__INC_WORLD
			sprintf( szTmp, "ignore Skill:: over weight ");
			this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
#endif
			return true;
		}
	}
#ifdef __KCHS_BATTLECART__
	if ( this->Get_RideMODE() ) {
		if( !this->CanDoPatSkill( nSkillIDX ) )
		{
	#ifdef	__INC_WORLD
			sprintf( szTmp, "ignore Skill:: in riding mode");
			this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
	#endif
			return true;
		}
	}
#else
	if ( this->Get_RideMODE() ) {
	#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore Skill:: in riding mode");
		this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
	#endif
		return true;
	}
#endif

	if ( !this->Is_SelfSKILL( nSkillIDX ) ) {
		return IS_HACKING( this, "Recv_cli_SELF_SKILL :: no self type skill" );
	} 

	if ( this->Do_SelfSKILL( nSkillIDX ) ) {
		// ½ÃÀÛ ¼º°ø...
		this->m_dwLastSkillActiveTIME = dwCurTime;
		this->m_dwLastSkillSpellTIME[ pPacket->m_cli_SELF_SKILL.m_btSkillSLOT ] = dwCurTime;
		this->m_dwLastSkillGroupSpeelTIME[ SKILL_RELOAD_TYPE(nSkillIDX) ] = dwCurTime;
	}

	return true;
}

/// Å¸°Ù ½ºÅ³ ½ÃÀÛ ¿äÃ» ¹ÞÀ½
bool classUSER::Recv_cli_TARGET_SKILL( t_PACKET *pPacket )
{
	if ( !(this->m_dwPayFLAG & PLAY_FLAG_BATTLE) ) {
		// °ø°Ý ¸øÇØ... µ·³»¾ßÇÔ
		return true;
	}

#ifdef	__INC_WORLD
	char szTmp[ MAX_PATH ];
#endif

	DWORD dwCurTime = this->GetZONE()->GetCurrentTIME();
	if ( SKILL_DELAY_TIME > dwCurTime - this->m_dwLastSkillActiveTIME ) {
#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore Skill:: Delay:%d, Charged:%d", SKILL_DELAY_TIME, dwCurTime - this->m_dwLastSkillActiveTIME );
		this->Send_gsv_WHISPER( "SELF_TARGET", szTmp );
#endif
		return true;
	}

	if ( this->Get_ActiveSKILL() ) {
#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore Skill in casting motion" );
		this->Send_gsv_WHISPER( "TARGET_SKILL", szTmp );
#endif
		return true;	// ½ºÅ³ ÄÉ½ºÆÃ ÁßÀÏ¶© ¸ø¹Ù²ã
	}
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() || this->m_IngSTATUS.IsSET( FLAG_ING_DUMB ) ) {
#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore Skill:: not allowd status" );
		this->Send_gsv_WHISPER( "TARGET_SKILL", szTmp );
#endif
		return true;
	}
	if ( pPacket->m_cli_TARGET_SKILL.m_btSkillSLOT >= MAX_LEARNED_SKILL_CNT ) {
#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore Skill:: invalid skill slot %d / %d", pPacket->m_cli_SELF_SKILL.m_btSkillSLOT, MAX_LEARNED_SKILL_CNT);
		this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
#endif
		return false;
	}

	short nSkillIDX = this->m_Skills.m_nSkillINDEX[ pPacket->m_cli_TARGET_SKILL.m_btSkillSLOT ];
	if ( IsTAIWAN() ) {
		if ( SKILL_AVAILBLE_STATUS(nSkillIDX) && !(m_btRideATTR & SKILL_AVAILBLE_STATUS(nSkillIDX)) )
			return true;

		if ( SKILL_RELOAD_TYPE(nSkillIDX) ) {
			// 05.05.30 ½ºÅ³ ¸®·Îµå ±×·ìÅ¸ÀÔÀ¸·Î Ã¼Å©...
			if ( SKILL_RELOAD_SECOND(nSkillIDX) > dwCurTime - this->m_dwLastSkillGroupSpeelTIME[ SKILL_RELOAD_TYPE(nSkillIDX) ] ) {
	#ifdef	__INC_WORLD
				sprintf( szTmp, "ignore SkillIdx:%d, :: Delay:%d > Charged:%d, LastStamp:%d \n", 
						nSkillIDX, 
						SKILL_RELOAD_SECOND(nSkillIDX), 
						dwCurTime - this->m_dwLastSkillGroupSpeelTIME[ pPacket->m_cli_SELF_SKILL.m_btSkillSLOT ], 
						this->m_dwLastSkillGroupSpeelTIME[ pPacket->m_cli_SELF_SKILL.m_btSkillSLOT ] );
				this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
	#endif
				return true;
			}
		}
	}

	if ( SKILL_RELOAD_SECOND(nSkillIDX) > dwCurTime - this->m_dwLastSkillSpellTIME[ pPacket->m_cli_TARGET_SKILL.m_btSkillSLOT ] ) {
		// ´Ù½Ã »ç¿ë°¡´ÉÇÑ ½Ã°£ÀÎÁö Ã¼Å©...
#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore SkillIdx:%d, :: Delay:%d > Charged:%d, LastStamp:%d \n", 
				nSkillIDX, 
				SKILL_RELOAD_SECOND(nSkillIDX), 
				dwCurTime - this->m_dwLastSkillSpellTIME[ pPacket->m_cli_TARGET_SKILL.m_btSkillSLOT ], 
				this->m_dwLastSkillSpellTIME[ pPacket->m_cli_TARGET_SKILL.m_btSkillSLOT ] );
		this->Send_gsv_WHISPER( "TARGET_SKILL", szTmp );
#endif
		return true;
	}

	if ( this->Get_WeightRATE() >= WEIGHT_RATE_CANT_ATK ) {
		// ¹«°Ì´Ù.. ¸í·É ºÒ°¡...
		if ( ( SKILL_TYPE( nSkillIDX ) >= 3 && SKILL_TYPE( nSkillIDX ) <= 13 ) ||
			   SKILL_TYPE( nSkillIDX ) == 17 ) {
#ifdef	__INC_WORLD
			sprintf( szTmp, "ignore Skill:: over weight ");
			this->Send_gsv_WHISPER( "TARGET_SKILL", szTmp );
#endif
			return true;
		}
	}

#ifdef __KCHS_BATTLECART__
	if ( this->Get_RideMODE() ) {
		if( !this->CanDoPatSkill( nSkillIDX ) )
		{
	#ifdef	__INC_WORLD
			sprintf( szTmp, "ignore Skill:: in riding mode");
			this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
	#endif
			return true;
		}
	}
#else
	if ( this->Get_RideMODE() ) {
	#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore Skill:: in riding mode");
		this->Send_gsv_WHISPER( "TARGET_SKILL", szTmp );
	#endif
		return true;
	}
#endif

	if ( !this->Is_TargetSKILL( nSkillIDX ) ) {
		// Â¥¸£Áö ¸»°í ¹«½Ã :: ¸î°Ç¾¿ µé¾î¿Â´Ù -_-;
		// return IS_HACKING( this, "Recv_cli_TARGET_SKILL :: no target type skill" );
#ifdef	__INC_WORLD
		sprintf( szTmp, "ignore Skill:: invalied self skill");
		this->Send_gsv_WHISPER( "TARGET_SKILL", szTmp );
#endif
		return true;
	}

	// Å¸°ÙÀÌ À¯È¿ÇÑÁö( »ì¾Æ ÀÖ´ÂÁö )... Ã¼Å©...
	if ( this->Do_TargetSKILL( pPacket->m_cli_TARGET_SKILL.m_wDestObjIDX, nSkillIDX ) ) {
		// ½ÃÀÛ ¼º°ø...
		this->m_dwLastSkillActiveTIME = dwCurTime;
		this->m_dwLastSkillSpellTIME[ pPacket->m_cli_TARGET_SKILL.m_btSkillSLOT ] = dwCurTime;
		this->m_dwLastSkillGroupSpeelTIME[ SKILL_RELOAD_TYPE(nSkillIDX) ] = dwCurTime;
	}

	return true;
}

/// Áö¿ª ½ºÅ³ ¿äÃ» ¹ÞÀ½ 
bool classUSER::Recv_cli_POSITION_SKILL( t_PACKET *pPacket )
{
	if ( !(this->m_dwPayFLAG & PLAY_FLAG_BATTLE) ) {
		// °ø°Ý ¸øÇØ... µ·³»¾ßÇÔ
		return true;
	}

//#ifndef	__INC_WORLD
//	if ( this->m_IngSTATUS.IsSubSET( FLAG_CHEAT_INVINCIBLE ) ) // ¹«Àû ¸ðµå½Ã °ø°Ý ºÒ°¡...
//		return true;
//#endif
	DWORD dwCurTime = this->GetZONE()->GetCurrentTIME();
	if ( SKILL_DELAY_TIME > dwCurTime - this->m_dwLastSkillActiveTIME ) {
#ifdef	__INC_WORLD
		char szTmp[ MAX_PATH ];
		sprintf( szTmp, "ignore Skill:: Delay:%d, Charged:%d", SKILL_DELAY_TIME, dwCurTime - this->m_dwLastSkillActiveTIME );
		this->Send_gsv_WHISPER( "SELF_TARGET", szTmp );
#endif
		return true;
	}

	if ( this->Get_ActiveSKILL() ) return true;	// ½ºÅ³ ÄÉ½ºÆÃ ÁßÀÏ¶© ¸ø¹Ù²ã
	if ( this->m_IngSTATUS.IsIgnoreSTATUS() || this->m_IngSTATUS.IsSET( FLAG_ING_DUMB ) ) return true;
	if ( pPacket->m_cli_POSITION_SKILL.m_btSkillSLOT >= MAX_LEARNED_SKILL_CNT ) 
		return false;

	short nSkillIDX = this->m_Skills.m_nSkillINDEX[ pPacket->m_cli_POSITION_SKILL.m_btSkillSLOT ];
	if ( IsTAIWAN() ) {
		if ( SKILL_AVAILBLE_STATUS(nSkillIDX) && !(m_btRideATTR & SKILL_AVAILBLE_STATUS(nSkillIDX)) )
			return true;

		if ( SKILL_RELOAD_TYPE(nSkillIDX) ) {
			// 05.05.30 ½ºÅ³ ¸®·Îµå ±×·ìÅ¸ÀÔÀ¸·Î Ã¼Å©...
			if ( SKILL_RELOAD_SECOND(nSkillIDX) > dwCurTime - this->m_dwLastSkillGroupSpeelTIME[ SKILL_RELOAD_TYPE(nSkillIDX) ] ) {
	#ifdef	__INC_WORLD
				char szTmp[ MAX_PATH ];
				sprintf( szTmp, "ignore SkillIdx:%d, :: Delay:%d > Charged:%d, LastStamp:%d \n", 
						nSkillIDX, 
						SKILL_RELOAD_SECOND(nSkillIDX), 
						dwCurTime - this->m_dwLastSkillGroupSpeelTIME[ pPacket->m_cli_SELF_SKILL.m_btSkillSLOT ], 
						this->m_dwLastSkillGroupSpeelTIME[ pPacket->m_cli_SELF_SKILL.m_btSkillSLOT ] );
				this->Send_gsv_WHISPER( "SELF_SKILL", szTmp );
	#endif
				return true;
			}
		}
	}

	if ( SKILL_RELOAD_SECOND(nSkillIDX) > dwCurTime - this->m_dwLastSkillSpellTIME[ pPacket->m_cli_POSITION_SKILL.m_btSkillSLOT ] ) {
		// ´Ù½Ã »ç¿ë°¡´ÉÇÑ ½Ã°£ÀÎÁö Ã¼Å©...
#ifdef	__INC_WORLD
		char szTmp[ MAX_PATH ];
		sprintf( szTmp, "ignore SkillIdx:%d, :: Delay:%d > Charged:%d, LastStamp:%d \n", 
				nSkillIDX, 
				SKILL_RELOAD_SECOND(nSkillIDX), 
				dwCurTime - this->m_dwLastSkillSpellTIME[ pPacket->m_cli_POSITION_SKILL.m_btSkillSLOT ], 
				this->m_dwLastSkillSpellTIME[ pPacket->m_cli_POSITION_SKILL.m_btSkillSLOT ] );
		this->Send_gsv_WHISPER( "POSITION_SKILL", szTmp );
#endif
		return true;
	}
	if ( this->Get_WeightRATE() >= WEIGHT_RATE_CANT_ATK ) {
		// ¹«°Ì´Ù.. ¸í·É ºÒ°¡...
		if ( ( SKILL_TYPE( nSkillIDX ) >= 3 && SKILL_TYPE( nSkillIDX ) <= 13 ) ||
			   SKILL_TYPE( nSkillIDX ) == 17 )
			return true;
	}
	if ( this->Get_RideMODE() )
	{
#ifdef __KCHS_BATTLECART__
		if( !this->CanDoPatSkill( nSkillIDX ) )
			return true;
#else
		return true;
#endif
	}

	switch ( SKILL_TYPE( nSkillIDX ) ) {
		case SKILL_TYPE_07 :					// Áö¿ª ¸¶¹ý °ø°ÝÇü
			if ( this->GetCur_RIDE_MODE() )		// Å¾½Â½Ã »ç¿ë ÇÒ¼ö ¾ø´Ù.
				return true;
			break;

		default :
			return IS_HACKING( this, "Recv_cli_POSITION_SKILL :: no position type skill" );
	} 

	if ( this->Skill_ActionCondition( nSkillIDX ) ) {
		// ½ÇÁ¦ ÇÊ¿ä ¼öÄ¡ ¼Ò¸ð Àû¿ë...
		// if ( this->SetCMD_Skill2POS( pPacket->m_cli_POSITION_SKILL.m_PosTARGET, nSkillIDX ) )
		//	this->Skill_UseAbilityValue( nSkillIDX );
		if ( this->SetCMD_Skill2POS( pPacket->m_cli_POSITION_SKILL.m_PosTARGET, nSkillIDX ) ) {
			// ½ÃÀÛ ¼º°ø...
			this->m_dwLastSkillActiveTIME = dwCurTime;
			this->m_dwLastSkillSpellTIME[ pPacket->m_cli_POSITION_SKILL.m_btSkillSLOT ] = dwCurTime;
			this->m_dwLastSkillGroupSpeelTIME[ SKILL_RELOAD_TYPE(nSkillIDX) ] = dwCurTime;
		}
	}

	return true;
}


//-------------------------------------------------------------------------------------------------
/// Äù½ºÆ® Ãß°¡/½ÇÇà¿¡ ´ëÇÑ °á°ú Åëº¸
bool classUSER::Send_gsv_QUEST_REPLY (BYTE btResult, BYTE btSlot, int iQuestID)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_QUEST_REPLY;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_QUEST_REPLY);

    pCPacket->m_gsv_QUEST_REPLY.m_btResult	  = btResult;
    pCPacket->m_gsv_QUEST_REPLY.m_btQuestSLOT = btSlot;
    pCPacket->m_gsv_QUEST_REPLY.m_iQuestID	  = iQuestID;

    this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}
/// Äù½ºÆ® ½ÇÇà...
bool classUSER::Do_QuestTRIGGER( t_HASHKEY HashTRIGGER, short nSelectReward )
{
	// ¼­¹ö¿¡¼­¸¸ Ã¼Å©ÇÏ°í Å¬¶óÀÌ¾ðÆ®¿¡¼­ Ã¼Å©ÇÏÁö ¾Ê´Â Á¶°ÇÀÏ °æ¿ì
	// ¼­¹ö¿¡¼­ Á¶°ÇÀÌ falseµÉ¼ö ÀÖ´Ù...¹«Á¶°Ç Á¢¼Ó ²÷À¸¸é ¾ÈµÊ... ½ÇÆÐ¸¦ Åëº¸ÇÒ²¨³Ä? °Á ¹«½Ã ÇÒ²¨³Ä???
	eQST_RESULT eResult = g_QuestList.CheckQUEST( this, HashTRIGGER, true, this->m_iLastEventNpcIDX, NULL, nSelectReward );
	this->m_iLastEventNpcIDX = 0;
	switch ( eResult ) {
		case QST_RESULT_SUCCESS :
			g_pThreadLOG->When_QuestLOG( this, HashTRIGGER, NEWLOG_QUEST_HASH_DONE );
			// g_pThreadLOG->When_DoneQUEST( this, HashTRIGGER );
			return this->Send_gsv_QUEST_REPLY(RESULT_QUEST_REPLY_TRIGGER_SUCCESS, 0, (int)HashTRIGGER );
		case QST_RESULT_FAILED :
			return this->Send_gsv_QUEST_REPLY(RESULT_QUEST_REPLY_TRIGGER_FAILED, 0, (int)HashTRIGGER );
		case QST_RESULT_STOPPED :
			return true;
		case QST_RESULT_INVALID :
			// ¹«½Ã...
			return true;
		default :
		{
			CQuestTRIGGER *pQuestTrigger = g_QuestList.GetQuest( HashTRIGGER );
			if ( pQuestTrigger ) {
				// ¿ì¼± Â©¸£Áö ¸»°í...
				g_LOG.CS_ODS( 0xffff, ">>> ERROR(%s,ZoneNO:%d):: QuestTrigger( %s:Result:%d )\n", 
							this->Get_NAME(), this->GetZONE()->Get_ZoneNO(), 
							pQuestTrigger->m_Name.Get(), eResult);//HashTRIGGER );
				return true;
			} else {
				g_LOG.CS_ODS( 0xffff, ">>> ERROR(%s,ZoneNO:%d):: QuestTrigger not found( Result:%d, Hash:%d )\n", 
							this->Get_NAME(), this->GetZONE()->Get_ZoneNO(), eResult, HashTRIGGER );
			}
		}
	} // switch ( eResult )


	// Å¬¶óÀÌ¾ðÆ®¿¡¼­ ÀÌ¹Ì ÇÑ¹ø °ËÁõÇÑÈÄ º¸³»±â ¶§¹®¿¡ ½ÇÆÐÇÒ °æ¿ì 
	// µ¥ÀÌÅ¸°¡ Æ²¸±¼ö ÀÖÀ¸¹Ç·Î Â©¸£°í ´Ù½Ã Á¢¼ÓÇÏµµ·Ï À¯µµ... !!!
	// return this->Send_gsv_QUEST_REPLY(RESULT_QUEST_REPLY_TRIGGER_FAILED, 0, (int)HashTRIGGER );
	return true;
}
/// Äù½ºÆ® »èÁ¦/¿Ï·áÃ¼Å© ¸¦ ¿äÃ» ¹ÞÀ½
bool classUSER::Recv_cli_QUEST_REQ( t_PACKET *pPacket )
{
	switch( pPacket->m_cli_QUEST_REQ.m_btTYPE ) {
		//case TYPE_QUEST_REQ_ADD :
		//	if ( this->Quest_Append( pPacket->m_cli_QUEST_REQ.m_btQuestSLOT, pPacket->m_cli_QUEST_REQ.m_iQuestID ) ) {
		//		// Äù½ºÆ® µî·Ï ·Î±×...
		//		#ifdef	__NEW_LOG
		//			g_pThreadLOG->When_QuestLOG( this, pPacket->m_cli_QUEST_REQ.m_iQuestID, NEWLOG_QUEST_RECV );
		//		#else
		//			g_pThreadLOG->When_RecvQUEST( this, pPacket->m_cli_QUEST_REQ.m_iQuestID );
		//		#endif
		//		return this->Send_gsv_QUEST_REPLY( RESULT_QUEST_REPLY_ADD_SUCCESS, pPacket->m_cli_QUEST_REQ.m_btQuestSLOT, pPacket->m_cli_QUEST_REQ.m_iQuestID );
		//	}
		//	break;

		case TYPE_QUEST_REQ_DEL :
			if ( this->Quest_Delete( pPacket->m_cli_QUEST_REQ.m_btQuestSLOT, pPacket->m_cli_QUEST_REQ.m_iQuestID ) ) {
				// Äù½ºÆ® Æ÷±â ·Î±×...
				#ifdef	__NEW_LOG
					g_pThreadLOG->When_QuestLOG( this, pPacket->m_cli_QUEST_REQ.m_iQuestID, NEWLOG_QUEST_GIVEUP );
				#else
					g_pThreadLOG->When_GiveUpQUEST( this, pPacket->m_cli_QUEST_REQ.m_iQuestID );
				#endif
				return this->Send_gsv_QUEST_REPLY( RESULT_QUEST_REPLY_DEL_SUCCESS, pPacket->m_cli_QUEST_REQ.m_btQuestSLOT, pPacket->m_cli_QUEST_REQ.m_iQuestID );
			}
			break;

		case TYPE_QUEST_REQ_DO_TRIGGER :
			return this->Do_QuestTRIGGER( pPacket->m_cli_QUEST_REQ.m_TriggerHash/*º¸»ó¼±ÅÃ ÀÎµ¦½º */ );
	}

	IS_HACKING( this, "classUSER::cli_QUEST_REQ( Script hacking.. )" );

	return false;
}

/*
bool classUSER::Recv_cli_ADD_QUEST( t_PACKET *pPacket )
{
	if ( pPacket->m_cli_ADD_QUEST.m_btQuestSLOT >= QUEST_PER_PLAYER )
		return false;

	this->m_Quests.m_QUEST[ pPacket->m_cli_ADD_QUEST.m_btQuestSLOT ].Init ();
	this->m_Quests.m_QUEST[ pPacket->m_cli_ADD_QUEST.m_btQuestSLOT ].SetID( pPacket->m_cli_ADD_QUEST.m_iQuestID );

	return true;
}

bool classUSER::Recv_cli_DEL_QUEST( t_PACKET *pPacket )
{
	if ( pPacket->m_cli_DEL_QUEST.m_btQuestSLOT >= QUEST_PER_PLAYER )
		return false;

	if ( pPacket->m_cli_DEL_QUEST.m_iQuestID == this->m_Quests.m_QUEST[ pPacket->m_cli_DEL_QUEST.m_btQuestSLOT ].GetID() ) {
		this->m_Quests.m_QUEST[ pPacket->m_cli_DEL_QUEST.m_btQuestSLOT ].Init ();
	}

	return true;
}

bool classUSER::Recv_cli_QUEST_DATA_REQ( t_PACKET *pPacket )
{
	if ( pPacket->m_cli_QUEST_DATA_REQ.m_btQuestSLOT >= QUEST_PER_PLAYER )
		return false;

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_gsv_QUEST_DATA_REPLY.m_btQuestSLOT = pPacket->m_cli_QUEST_DATA_REQ.m_btQuestSLOT;
	::CopyMemory( &pCPacket->m_gsv_QUEST_DATA_REPLY.m_Quest, &this->m_Quests.m_QUEST[ pPacket->m_cli_QUEST_DATA_REQ.m_btQuestSLOT ], sizeof( CQUEST ) );

	this->SendPacket( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}
*/

//-------------------------------------------------------------------------------------------------
/// ÆÄÆ¼ °ü·Ã ¿äÃ» ÆÐÅ¶
bool classUSER::Recv_cli_PARTY_REQ( t_PACKET *pPacket )
{
	switch( pPacket->m_cli_PARTY_REQ.m_btREQUEST ) {
		case PARTY_REQ_BAN :		// ÆÄÂ¯ ÀÌ¾î¾ß ÇÑ´Ù.
			if ( this->GetPARTY() && this == this->m_pPartyBUFF->GetPartyOWNER() ) {
				this->m_pPartyBUFF->Kick_MEMBER( pPacket->m_cli_PARTY_REQ.m_dwDestIDXorTAG );
			}
			return true;

		case PARTY_REQ_LEFT :		// ³»°¡ ³ª°£´Ù.
			if ( this->GetPARTY() ) {
				this->m_pPartyBUFF->Sub_PartyUSER( this->m_nPartyPOS );		// PARTY_REQ_LEFT
			}
			return true;
	} // switch( pPacket->m_cli_PARTY_REQ.m_btREQUEST )


	// ÀÚ½ÅÇÑÅ× ÆÄÆ¼°¡ ½ÅÃ»µÇ´Â È²´çÇÑ ÆÐÅ¶ÀÌ ¿Â´Ù°í...
	if ( this->Get_INDEX() == pPacket->m_cli_PARTY_REQ.m_dwDestIDXorTAG )
		return false;

	classUSER *pUSER = g_pObjMGR->Get_UserOBJ( pPacket->m_cli_PARTY_REQ.m_dwDestIDXorTAG );
	if ( NULL == pUSER ) {
		return this->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REQ.m_dwDestIDXorTAG, PARTY_REPLY_NOT_FOUND );
	}
	switch( pPacket->m_cli_PARTY_REQ.m_btREQUEST ) {
		case PARTY_REQ_MAKE :
			// °ú±Ý ¾ÈµÇ¾î ÀÖ´Ù¸é ÆÄÆ¼ ¸øÇÑ´Ù.
			if ( !(this->m_dwPayFLAG & PAY_FLAG_JP_BATTLE) )
				return true;
			if ( !(pUSER->m_dwPayFLAG & PAY_FLAG_JP_BATTLE) ) {
				return this->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REQ.m_dwDestIDXorTAG, PAATY_REPLY_NO_CHARGE_TARGET );
			}

			if ( this->GetPARTY() )
				return true;
			if ( pUSER->Get_HP() <= 0 || pUSER->GetPARTY() ) {
				// ÀÌ¹Ì ´Ù¸¥ ÆÄÆ¼¿¡ Âü°¡ ÁßÀÌ¸é..
				return this->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REQ.m_dwDestIDXorTAG, PARTY_REPLY_BUSY );
			}

			// Numenor : no level restriction if it is a shared party
			if ( !pPacket->m_cli_PARTY_REQ.m_bShareParty && !this->Check_PartyJoinLEVEL( pUSER->Get_TRUE_LEVEL(), this->Get_LEVEL(), 0 ) ) {
				return Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REQ.m_dwDestIDXorTAG, PARTY_REPLY_INVALID_LEVEL );
			}

			return pUSER->Send_gsv_PARTY_REQ( this->Get_INDEX(), PARTY_REQ_MAKE, pPacket->m_cli_PARTY_REQ.m_bShareParty );

		case PARTY_REQ_JOIN :
			// °ú±Ý ¾ÈµÇ¾î ÀÖ´Ù¸é ÆÄÆ¼ ¸øÇÑ´Ù.
			if ( !(this->m_dwPayFLAG & PAY_FLAG_JP_BATTLE) ) {
				return true;
			}
			if ( !(pUSER->m_dwPayFLAG & PAY_FLAG_JP_BATTLE) ) {
				return this->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REQ.m_dwDestIDXorTAG, PAATY_REPLY_NO_CHARGE_TARGET );
			}

			if ( !this->GetPARTY() )
				return true;
			if ( pUSER->Get_HP() <= 0 || pUSER->GetPARTY() ) {
				// ÀÌ¹Ì ´Ù¸¥ ÆÄÆ¼¿¡ Âü°¡ ÁßÀÌ¸é..
				return this->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REQ.m_dwDestIDXorTAG, PARTY_REPLY_BUSY );
			}

			// Numenor : no level restriction if it is shared party
			if ( !pPacket->m_cli_PARTY_REQ.m_bShareParty && !this->Check_PartyJoinLEVEL( pUSER->Get_TRUE_LEVEL(), this->m_pPartyBUFF->GetAverageLEV(), this->m_pPartyBUFF->GetPartyLEV() ) ) {
				return Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REQ.m_dwDestIDXorTAG, PARTY_REPLY_INVALID_LEVEL );
			}

			return pUSER->Send_gsv_PARTY_REQ( this->Get_INDEX(), PARTY_REQ_JOIN, pPacket->m_cli_PARTY_REQ.m_bShareParty );

		case PARTY_REQ_CHANGE_OWNER :
			if ( this->GetPARTY() && this->GetPARTY() == pUSER->GetPARTY() ) {
				this->m_pPartyBUFF->Change_PartyOWNER( pUSER );
			}
			break;

		default :
			return false;
	}

	return true;
}

/// ÆÄÆ¼ °ü·Ã ÀÀ´ä ÆÐÅ¶
bool classUSER::Recv_cli_PARTY_REPLY( t_PACKET *pPacket )
{
	classUSER *pUSER = g_pObjMGR->Get_UserOBJ( pPacket->m_cli_PARTY_REPLY.m_dwDestIDXorTAG );
	if ( NULL == pUSER ) {
		return this->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REPLY.m_dwDestIDXorTAG, PARTY_REPLY_NOT_FOUND );
	}

	if( this->Get_CharHASH() == pUSER->Get_CharHASH() )
	{
		return IS_HACKING( this, "Recv_cli_PARTY_REPLY" );
	}

	switch( pPacket->m_cli_PARTY_REPLY.m_btREPLY ) {
		case PARTY_REPLY_ACCEPT_MAKE :
		{
			if ( this->GetPARTY() ) {
				// ÆÄÆ¼ °á¼º ¿äÃ»À» Çã¶ôÇß´Âµ¥ ³»°¡ ÀÌ¹Ì ´Ù¸¥ ÆÄÆ¼¿øÀÌ¶ó¸é ???
				// ³»°¡ ÆÄÆ¼ °á¼ºÀ» ¿äÃ»ÇÑ »ç¶÷ »ç¶÷ÀÌ ¹Þ¾Æµé¿©Á® ÆÄÆ¼°¡ °á¼ºµÉ¶§...
				return true;
			}

			if ( NULL == pUSER->GetPARTY() ) {
				// ÆÄÆ¼ °á¼º ¿äÃ»ÀÚ°¡ ÆÄÆ¼¸¦ ¸¸µç´Ù.
				if ( !g_pPartyBUFF->CreatePARTY( pUSER, pPacket->m_cli_PARTY_REPLY.m_bShareParty ) ) {
					return this->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REPLY.m_dwDestIDXorTAG, PARTY_REPLY_DESTROY );
				}
			} else
			if ( pUSER != pUSER->m_pPartyBUFF->GetPartyOWNER() ) {
				// ÆÄÆ¼ °á¼º ¿äÃ»ÀÚ°¡ ÀÌ¹Ì ´Ù¸¥ ÆÄÆ¼¿¡ °¡ÀÔÇß´Ù.
				return this->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REPLY.m_dwDestIDXorTAG, PARTY_REPLY_DESTROY );
			}

			// ÆÄÂ¯ÀÌ µÈ À¯Àú¿¡°Ô Çã¶ô ÆÐÅ¶ Àü¼Û.
			pUSER->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REPLY.m_dwDestIDXorTAG, PARTY_REPLY_ACCEPT_MAKE ); //HUGO

			BYTE btFailed = pUSER->m_pPartyBUFF->Add_PartyUSER( this );
			if ( btFailed )
				this->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REPLY.m_dwDestIDXorTAG, btFailed );
		}

		case PARTY_REPLY_ACCEPT_JOIN :
		{
			if ( this->GetPARTY() ) {
				// ³»°¡ ÆÄÆ¼ Âü°¡¸¦ Çã¶ôÇÑ »óÅÂ¿¡¼­ ³»°¡ ÆÄÆ¼°¡ ÀÖ´Ù¸é ???
				// ³»°¡ ÆÄÆ¼ °á¼ºÀ» ¿äÃ»ÇÑ ´Ù¸¥ ÀÌ°¡ Çã·°ÇØ¼­ ³»°¡ ÆÄÆ¼¿¡ Á¶ÀÎÀÌ µÈ°æ¿ì...
				return true;
			}

			// ÆÄÆ¼°á¼º ¿äÃ»ÀÚ, °¡ÀÔ ¿äÃ»ÀÚ°¡ ÀÚ½ÅÀÇ ÆÄÆ¼ÀÇ Â¯ÀÎ°¡ ??
			if ( pUSER->GetPARTY() && pUSER == pUSER->m_pPartyBUFF->GetPartyOWNER() ) {
				BYTE btFailed = pUSER->m_pPartyBUFF->Add_PartyUSER( this );
				if ( btFailed )
					this->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REPLY.m_dwDestIDXorTAG, btFailed );
			} else
				this->Send_gsv_PARTY_REPLY( pPacket->m_cli_PARTY_REPLY.m_dwDestIDXorTAG, PARTY_REPLY_DESTROY );
			break;
		}

		case PARTY_REPLY_REJECT_JOIN :
			pUSER->Send_gsv_PARTY_REPLY( this->Get_INDEX(), PARTY_REPLY_REJECT_JOIN );
			break;

		default :
			pUSER->Send_gsv_PARTY_REPLY( this->Get_INDEX(), pPacket->m_cli_PARTY_REPLY.m_btREPLY ); //HUGO
			break;
	} // switch( pPacket->m_cli_PARTY_REPLY.m_btREPLY )


	return true;
}


//-------------------------------------------------------------------------------------------------
/// ¾ÆÀÌÅÛ ¿É¼Ç °ËÁõ ¿äÃ» ÆÐÅ¶
bool classUSER::Recv_cli_APPRAISAL_REQ( t_PACKET *pPacket )
{
	if ( pPacket->m_cli_APPRAISAL_REQ.m_wInventoryIndex < 1 ||
		 pPacket->m_cli_APPRAISAL_REQ.m_wInventoryIndex >= INVENTORY_TOTAL_SIZE ) {
		return IS_HACKING( this, "Recv_cli_APPRAISAL_REQ" );
	}

	tagITEM *pITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_APPRAISAL_REQ.m_wInventoryIndex ];
	if ( !pITEM->IsEquipITEM() || pITEM->IsAppraisal() )
		return true;
	int iNeedMoney = CCal::GetMoney_WhenAPPRAISAL( ITEM_BASE_PRICE(pITEM->GetTYPE(), pITEM->GetItemNO() ), pITEM->GetDurability() );

    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_APPRAISAL_REPLY;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_APPRAISAL_REPLY );
	pCPacket->m_gsv_APPRAISAL_REPLY.m_wInventoryIndex = pPacket->m_cli_APPRAISAL_REQ.m_wInventoryIndex;

	if ( iNeedMoney > this->Get_MONEY() ) {	// µ·¾ø´ç
		pCPacket->m_gsv_APPRAISAL_REPLY.m_btResult = RESULT_APPRAISAL_REPLY_FAILED;
	} else {
		this->Sub_CurMONEY( iNeedMoney );
		pITEM->m_bIsAppraisal = 1;
		pCPacket->m_gsv_APPRAISAL_REPLY.m_btResult = RESULT_APPRAISAL_REPLY_OK;
	}

    this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	if ( pPacket->m_cli_APPRAISAL_REQ.m_wInventoryIndex < MAX_EQUIP_IDX /* || pPacket->m_cli_APPRAISAL_REQ.m_wInventoryIndex >= INVENTORY_RIDE_ITEM0 */ ) {
		// TODO:: PAT¾ÆÀÌÅÛÀº ??
		// ÀåÂøµÈ ÀåºñÀÌ¹Ç·Î ´É·ÂÄ¡ º¯°æµÊ ÀÌ¼Ó, È¸º¹¼Óµµ º¯°æ µÇ¸é ÁÖº¯¿¡ Åëº¸ ÇÊ¿ä
		if ( this->GetPARTY() ) {
			BYTE btCurCON = this->GetCur_CON();
			BYTE btRecvHP = this->m_btRecoverHP;
			BYTE btRecvMP = this->m_btRecoverMP;

			this->UpdateAbility ();		// appraisal

			// º¯°æ¿¡ ÀÇÇØ ¿É¼ÇÀÌ ºÙ¾î È¸º¹ÀÌ ¹Ù²î¸é ÆÄÆ¼¿ø¿¡°Ô Àü¼Û.
			if ( btCurCON != this->GetCur_CON()  ||
				 btRecvHP != this->m_btRecoverHP ||
				 btRecvMP != this->m_btRecoverMP ) {
				this->m_pPartyBUFF->Change_ObjectIDX( this );
			}
		} else {
			this->UpdateAbility ();		// appraisal
		}
		this->Send_gsv_EQUIP_ITEM( pPacket->m_cli_APPRAISAL_REQ.m_wInventoryIndex, pITEM );
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ¼Ò¸ð ¾ÆÀÌÅÛÀ¸·Î ¾ÆÀÌÅÛ ¼ö¸® ¿äÃ»
bool classUSER::Recv_cli_USE_ITEM_TO_REPAIR( t_PACKET *pPacket )
{
	if ( this->m_IngSTATUS.IsSET( FLAG_ING_IGNORE_ALL ) ) return true;

    if ( pPacket->m_cli_USE_ITEM_TO_REPAIR.m_nUseItemInvIDX < 0 || 
		 pPacket->m_cli_USE_ITEM_TO_REPAIR.m_nUseItemInvIDX >= INVENTORY_TOTAL_SIZE ) {
        // ÆÐÅ¶ÀÌ Á¶ÀÛµÈ°ÍÀÎ°¡?
		return IS_HACKING( this, "Recv_cli_USE_ITEM_TO_REPLY-1" );
    }
    if ( pPacket->m_cli_USE_ITEM_TO_REPAIR.m_nRepairTargetInvIDX < 0 || 
		 pPacket->m_cli_USE_ITEM_TO_REPAIR.m_nRepairTargetInvIDX >= INVENTORY_TOTAL_SIZE ) {
        // ÆÐÅ¶ÀÌ Á¶ÀÛµÈ°ÍÀÎ°¡?
		return IS_HACKING( this, "Recv_cli_USE_ITEM_TO_REPLY-2" );
    }

	tagITEM *pUseITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_USE_ITEM_TO_REPAIR.m_nUseItemInvIDX ];
	if ( ITEM_TYPE_USE != pUseITEM->GetTYPE() || USE_ITEM_REPAIR_ITEM != ITEM_TYPE( pUseITEM->GetTYPE(), pUseITEM->GetItemNO() ) )
		return true;

	tagITEM *pTgtITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_USE_ITEM_TO_REPAIR.m_nRepairTargetInvIDX ];
	if ( !pTgtITEM->IsEquipITEM() ) {
		// Àåºñ°¡ ¾Æ´Ï´Ù.
		if ( ITEM_TYPE_RIDE_PART != pTgtITEM->GetTYPE() )
			return true;
		// ¿£ÁøÀ» ¼ö¸® ¸øÇÑ´Ù... ¿¬·á·Î º¸ÃæÀ» ÇØ¾ßÁã~~~
		if ( RIDE_PART_ENGINE == PAT_ITEM_PART_IDX( pTgtITEM->GetItemNO() ) )
			return true;
	}
	if ( pTgtITEM->GetDurability() <= 0 )	// ³»±¸µµ°¡ 0ÀÌ¸é ¼ö¸® ¸øÇÑ´Ù.
		return true;

	// ³»±¸µµ °¨¼Ò¾ø´Â ÆÛÆÑÆ® ¸ÁÄ¡³Ä ???
	if ( 0 == USEITEM_ADD_DATA_VALUE( pUseITEM->GetItemNO() ) ) {
		int iDec = (int)( ( 1400 - pTgtITEM->GetLife() ) * ( RANDOM(100)+11 ) / ( pTgtITEM->GetDurability()+40) / 400.f );
		if ( pTgtITEM->GetDurability() >= iDec )
			pTgtITEM->m_cDurability -= iDec;
		else
			pTgtITEM->m_cDurability = 0;
	}

	pTgtITEM->m_nLife = MAX_ITEM_LIFE;
	this->UpdateAbility ();		// repair weapon

	// ¼ö·® °¨¼Ò
	if ( --pUseITEM->m_uiQuantity <= 0 ) {
		// ´Ù ¼Ò¸ðÇß´Ù..
		m_Inventory.DeleteITEM( pPacket->m_cli_USE_ITEM_TO_REPAIR.m_nUseItemInvIDX );
	}
	
	this->Send_gsv_SET_TWO_INV_ONLY( GSV_USED_ITEM_TO_REPAIR, 
			(BYTE)pPacket->m_cli_USE_ITEM_TO_REPAIR.m_nUseItemInvIDX,		pUseITEM,
			(BYTE)pPacket->m_cli_USE_ITEM_TO_REPAIR.m_nRepairTargetInvIDX,	pTgtITEM);

	return true;
}
/// npc¸¦ ÅëÇØ ¾ÆÀÌÅÛ ¼ö¸® ¿äÃ»
bool classUSER::Recv_cli_REPAIR_FROM_NPC( t_PACKET *pPacket )
{
	CObjNPC *pCharNPC;
	pCharNPC = (CObjNPC*)g_pObjMGR->Get_GameOBJ( pPacket->m_cli_REPAIR_FROM_NPC.m_wNPCObjIDX, OBJ_NPC );
	if ( !pCharNPC ) {
		return false;
	}

	// »óÁ¡ npc¿ÍÀÇ °Å·¡ Ã¼Å©...
	int iDistance = distance ((int)m_PosCUR.x, (int)m_PosCUR.y, (int)pCharNPC->m_PosCUR.x, (int)pCharNPC->m_PosCUR.y);
	if ( iDistance > MAX_TRADE_DISTANCE )	{ 
		return true;
	}

	tagITEM *pTgtITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_REPAIR_FROM_NPC.m_nRepairTargetInvIDX ];
	if ( !pTgtITEM->IsEquipITEM() ) {
		// Àåºñ°¡ ¾Æ´Ï´Ù.
		if ( ITEM_TYPE_RIDE_PART != pTgtITEM->GetTYPE() )
			return true;
		// ¿£ÁøÀ» ¼ö¸® ¸øÇÑ´Ù... ¿¬·á·Î º¸ÃæÀ» ÇØ¾ßÁã~~~
		if ( RIDE_PART_ENGINE == PAT_ITEM_PART_IDX( pTgtITEM->GetItemNO() ) )
			return true;
	}
	if ( pTgtITEM->GetDurability() <= 0 )	// ³»±¸µµ°¡ 0ÀÌ¸é ¼ö¸® ¸øÇÑ´Ù.
		return true;

	int iFee = (int)( ( ITEM_BASE_PRICE(pTgtITEM->GetTYPE(), pTgtITEM->GetItemNO() ) + 1000 ) / 400000.f
					* ( pTgtITEM->GetDurability() + 10 )
					* ( 1100 - pTgtITEM->GetLife() ) );
	if ( this->Get_MONEY() < iFee ) {
		// µ· ¾ø´Ù.
		return true;
	}

	this->Sub_CurMONEY( iFee );
	pTgtITEM->m_nLife = MAX_ITEM_LIFE; 
	this->UpdateAbility ();		// repair weapon

	classPACKET *pCPacket = this->Init_gsv_SET_MONEYnINV( GSV_REPAIRED_FROM_NPC );
	if ( pCPacket ) {
		pCPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ 0 ].m_btInvIDX = (BYTE)pPacket->m_cli_REPAIR_FROM_NPC.m_nRepairTargetInvIDX;
		pCPacket->m_gsv_SET_MONEYnINV.m_sInvITEM[ 0 ].m_ITEM = *pTgtITEM;
		pCPacket->m_gsv_SET_MONEYnINV.m_btItemCNT = 1;
		this->Send_gsv_SET_MONEYnINV( pCPacket );
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ¾ÆÀÌÅÛÀÇ ¼ö¸íÀ» Åëº¸
bool classUSER::Send_gsv_SET_ITEM_LIFE (short nInvIDX, short nLife)
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = GSV_SET_ITEM_LIFE;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_SET_ITEM_LIFE );

    pCPacket->m_gsv_SET_ITEM_LIFE.m_nInventoryIDX = nInvIDX;
	pCPacket->m_gsv_SET_ITEM_LIFE.m_nLife = nLife;

	this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}


//-------------------------------------------------------------------------------------------------
/// ¸Þ½ÅÁ® :: °³ÀÎ¼· Å×½ºÆ®¿ëÇÔ¼ö
bool classUSER::Send_tag_MCMD_HEADER( BYTE btCMD, char *szStr )
{
    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

    pCPacket->m_HEADER.m_wType = WSV_MESSENGER;
    pCPacket->m_HEADER.m_nSize = sizeof( tag_MCMD_HEADER );
	pCPacket->m_tag_MCMD_HEADER.m_btCMD = btCMD;
	if ( szStr )
		pCPacket->AppendString( szStr );

	this->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}
/// ¸Þ½ÅÁ® :: °³ÀÎ¼· Å×½ºÆ®¿ëÇÔ¼ö
bool classUSER::Recv_cli_MCMD_APPEND_REQ( t_PACKET *pPacket )
{
	char *szName = pPacket->m_cli_MCMD_APPEND_REQ.m_szName;
	classUSER *pDestUSER = g_pUserLIST->Find_CHAR( pPacket->m_cli_MCMD_APPEND_REQ.m_szName );
	if ( NULL == pDestUSER )
		return this->Send_tag_MCMD_HEADER( MSGR_CMD_NOT_FOUND, szName );

    classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

#pragma COMPILE_TIME_MSG ( "Ä£±¸ Ãß°¡ °ÅºÎ »óÅÂ¸é..." )

    pCPacket->m_HEADER.m_wType = WSV_MESSENGER;
    pCPacket->m_HEADER.m_nSize = sizeof( wsv_MCMD_APPEND_REQ );

	pCPacket->m_wsv_MCMD_APPEND_REQ.m_btCMD = MSGR_CMD_APPEND_REQ;
	pCPacket->m_wsv_MCMD_APPEND_REQ.m_wUserIDX = this->m_iSocketIDX;
	pCPacket->AppendString( this->Get_NAME() );

	pDestUSER->SendPacket( pCPacket );
    Packet_ReleaseNUnlock( pCPacket );

	return true;
}
/// ¸Þ½ÅÁ® :: °³ÀÎ¼· Å×½ºÆ®¿ëÇÔ¼ö
bool classUSER::Recv_cli_MESSENGER( t_PACKET *pPacket )
{
#ifdef	__INC_WORLD
	switch( pPacket->m_tag_MCMD_HEADER.m_btCMD ) {
		case MSGR_CMD_APPEND_REQ	:	// »ó´ë¹æ¿¡ ¿äÃ»
			return this->Recv_cli_MCMD_APPEND_REQ( pPacket );

		case MSGR_CMD_APPEND_REJECT	:	// °ÅºÎ
		{
			classUSER *pDestUSER = (classUSER*)g_pUserLIST->GetSOCKET( pPacket->m_cli_MCMD_APPEND_REPLY.m_wUserIDX );
			if ( pDestUSER )
				pDestUSER->Send_tag_MCMD_HEADER( MSGR_CMD_APPEND_REJECT, pDestUSER->Get_NAME() );
			return true;
		}

		case MSGR_CMD_APPEND_ACCEPT :	// ½Ö¹æ¿¡ Ãß°¡..
			g_pThreadMSGR->Add_MessengerCMD( this->Get_NAME(), MSGR_CMD_APPEND_ACCEPT, pPacket, this->m_iSocketIDX );
			return true;

		default :
			g_pThreadMSGR->Add_MessengerCMD( this->Get_NAME(), pPacket->m_tag_MCMD_HEADER.m_btCMD, pPacket, this->m_iSocketIDX );
	} // switch( pPacket->m_tag_MCMD_HEADER.m_btCMD )
#endif

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ¸Þ½ÅÁ® :: °³ÀÎ¼· Å×½ºÆ®¿ëÇÔ¼ö
bool classUSER::Recv_cli_MESSENGER_CHAT( t_PACKET *pPacket )
{
#ifdef	__INC_WORLD
	if ( pPacket->m_HEADER.m_nSize <= 1+sizeof( cli_MESSENGER_CHAT ) )
		return true;

	g_pThreadMSGR->Add_MessengerCMD( this->Get_NAME(), 0x0ff, pPacket, this->m_iSocketIDX );
#endif
	return true;
}

//-------------------------------------------------------------------------------------------------
/// ¾ÆÀÌÅÛ Àç¹Ö/ºÐÇØ ½ÇÆÐ °á°ú Åëº¸
bool classUSER::Send_gsv_CRAFT_ITEM_RESULT (BYTE btRESULT)
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( pCPacket ) {
		pCPacket->m_HEADER.m_wType = GSV_CRAFT_ITEM_REPLY;
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_CRAFT_ITEM_REPLY ) - sizeof(BYTE);

		pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_btRESULT = btRESULT;

		this->SendPacket( pCPacket );
	    Packet_ReleaseNUnlock( pCPacket );

		return true;
	}

	return false;
}
/// ¾ÆÀÌÅÛ Àç¹Ö/ºÐÇØ °á°ú ÆÐÅ¶ ÃÊ±âÈ­
classPACKET *classUSER::Init_gsv_CRAFT_ITEM_REPLY ()
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( pCPacket ) {
		pCPacket->m_HEADER.m_wType = GSV_CRAFT_ITEM_REPLY;
	/*
		pCPacket->m_HEADER.m_nSize = sizeof( gsv_CRAFT_ITEM_REPLY );
		pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_btRESULT = btRESULT;
		pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_btOutCNT = 0;
	*/
	}

	return pCPacket;
}
/// ¾ÆÀÌÅÛ Àç¹Ö/ºÐÇØ ¼º°ø °á°ú Åëº¸
void classUSER::Send_gsv_CRAFT_ITEM_REPLY( classPACKET *pCPacket, BYTE btRESULT, BYTE btOutCNT )
{
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_CRAFT_ITEM_REPLY ) + btOutCNT * sizeof( tag_SET_INVITEM );
	pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_btRESULT = btRESULT;
	pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_btOutCNT = btOutCNT;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
}

/// Gemming request
bool classUSER::Proc_CRAFT_GEMMING_REQ( t_PACKET *pPacket )
{
	if ( pPacket->m_cli_CRAFT_GEMMING_REQ.m_btEquipInvIDX >= MAX_EQUIP_IDX )
		return true;	
	if ( pPacket->m_cli_CRAFT_GEMMING_REQ.m_btJemInvIDX < INVENTORY_ETC_ITEM0 ||
		 pPacket->m_cli_CRAFT_GEMMING_REQ.m_btJemInvIDX >= INVENTORY_SHOT_ITEM0 )
		 return false;

	tagITEM *pEquipITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_CRAFT_GEMMING_REQ.m_btEquipInvIDX ];
	if ( !pEquipITEM->IsEquipITEM() )
		return true;
	
	tagITEM *pJewelITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_CRAFT_GEMMING_REQ.m_btJemInvIDX ];
	
	if ( ITEM_TYPE_GEM != pJewelITEM->GetTYPE() || pJewelITEM->GetQuantity() < 1 )
		return true;
	if ( pJewelITEM->GetItemNO() >= g_TblGEMITEM.m_nDataCnt ) {
		return false;
	}
	if ( 411 != ITEM_TYPE( ITEM_TYPE_GEM, pJewelITEM->GetItemNO() ) ) {
		return false;
	}

	if ( !pEquipITEM->HasSocket() ) {
		return this->Send_gsv_CRAFT_ITEM_RESULT( CRAFT_GEMMING_NEED_SOCKET );
	}
	if ( pEquipITEM->GetGemNO() > 300 ) {
		return this->Send_gsv_CRAFT_ITEM_RESULT( CRAFT_GEMMING_USED_SOCKET );
	}
	// Gemming Log ..
	g_pThreadLOG->When_GemmingITEM( this, pEquipITEM, pJewelITEM, NEWLOG_GEMMING, NEWLOG_SUCCESS );
	
	// Gem stuck automatic verification ...
	pEquipITEM->m_nGEM_OP = pJewelITEM->GetItemNO();
	this->SetPartITEM( pPacket->m_cli_CRAFT_GEMMING_REQ.m_btEquipInvIDX );
	
	pJewelITEM->SubQuantity( 1 );
	
	classPACKET *pCPacket = this->Init_gsv_CRAFT_ITEM_REPLY ();
	if ( !pCPacket )
		return false;
	
	pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 0 ].m_btInvIDX = pPacket->m_cli_CRAFT_GEMMING_REQ.m_btEquipInvIDX;
	pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 0 ].m_ITEM		= *pEquipITEM;
	pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 1 ].m_btInvIDX = pPacket->m_cli_CRAFT_GEMMING_REQ.m_btJemInvIDX;
	pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 1 ].m_ITEM		= *pJewelITEM;

	this->Send_gsv_CRAFT_ITEM_REPLY( pCPacket, CRAFT_GEMMING_SUCCESS, 2 );
	
	// Because it is equipped equipment, you need to be notified around ...
	this->UpdateAbility ();		// gemming
	this->InitPassiveSkill ();
	this->Send_gsv_EQUIP_ITEM( pPacket->m_cli_CRAFT_GEMMING_REQ.m_btEquipInvIDX, pEquipITEM );
	
	return true;
}
/// ¾ÆÀÌÅÛ ºÐÇØ ¿äÃ»
bool classUSER::Proc_CRAFT_BREAKUP_REQ( t_PACKET *pPacket, bool bUseMP )
{
	if ( pPacket->m_cli_CRAFT_BREAKUP_REQ.m_btTargetInvIDX <  MAX_EQUIP_IDX ||
		 pPacket->m_cli_CRAFT_BREAKUP_REQ.m_btTargetInvIDX >= INVENTORY_SHOT_ITEM0 )
		return false;

	tagITEM *pInITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_CRAFT_BREAKUP_REQ.m_btTargetInvIDX ];
	if ( 0 == pInITEM->GetHEADER() ) 
		return true;

	if ( !pInITEM->IsEnableDupCNT() && pInITEM->HasSocket() && pInITEM->GetGemNO() > 300 ) {
		// º¸¼® ºÐ¸®
		short ORI_QUAL = ITEM_QUALITY( pInITEM->GetTYPE(), pInITEM->GetItemNO() );
		short GEM_QUAL = ITEM_QUALITY( ITEM_TYPE_GEM,	   pInITEM->GetGemNO() );

		short nNeed;//  = ( (ORI_QUAL/2) + GEM_QUAL );
		if ( bUseMP ) {
			nNeed = CCal::GetMP_WhenBreakupGEM( ORI_QUAL, GEM_QUAL );
			if ( this->Get_MP() < nNeed ) {
				// ¿¥ ¾ø´Ù
				return true;
			}
			this->Sub_MP( nNeed );
		} else {
			nNeed = CCal::GetMONEY_WhenBreakupGEM( ORI_QUAL, GEM_QUAL );
			if ( this->GetCur_MONEY() < nNeed ) {
				// µ· ¾ø´Ù.
				return true;
			}
			this->Sub_CurMONEY( nNeed );
		}

		classPACKET *pCPacket = this->Init_gsv_CRAFT_ITEM_REPLY ();
		if ( !pCPacket )
			return false;

		BYTE btResult;
		tagITEM sOutITEM;
		sOutITEM.Clear ();

		if ( RANDOM(100)+1 + GEM_QUAL/6 <= 30 ) {	// µî±Þ °¨¼Ò
			if ( GEM_QUAL <= 35 ) {					// º¸¼® »èÁ¦
				// ·Î±×:: ºÐÇØ½Ã º¸¼® »èÁ¦ µÆÀ½...
				#ifdef	__NEW_LOG
					g_pThreadLOG->When_GemmingITEM( this, pInITEM, NULL, NEWLOG_UNGEMMING, NEWLOG_FAILED );
				#else
					g_pThreadLOG->When_UngemmingITEM( this, pInITEM, NULL );
				#endif

				// Àåºñ ¾ÆÀÌÅÛ...
				pInITEM->m_nGEM_OP = 0;				// Àåºñ¿¡¼­ º¸¼® »èÁ¦
				pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 0 ].m_btInvIDX =  pPacket->m_cli_CRAFT_BREAKUP_REQ.m_btTargetInvIDX;
				pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 0 ].m_ITEM		= *pInITEM;
				this->Send_gsv_CRAFT_ITEM_REPLY( pCPacket, CRAFT_BREAKUP_CLEARED_GEM, 1 );
				return true;
			}

			// º¸¼® ¾ÆÀÌÅÛ »ý¼º...
			sOutITEM.m_nItemNo = pInITEM->GetGemNO() - 1;

			btResult = CRAFT_BREAKUP_DEGRADE_GEM;
		} else {
			sOutITEM.m_nItemNo = pInITEM->GetGemNO();
			btResult = CRAFT_BREAKUP_SUCCESS_GEM;
		}
		sOutITEM.m_cType = ITEM_TYPE_GEM;
		sOutITEM.m_uiQuantity = 1;

		#ifdef	__NEW_LOG
			g_pThreadLOG->When_GemmingITEM( this, pInITEM, &sOutITEM, NEWLOG_UNGEMMING, NEWLOG_SUCCESS );
		#else
			g_pThreadLOG->When_UngemmingITEM( this, pInITEM, &sOutITEM );
		#endif

		pInITEM->m_nGEM_OP = 0;				// Àåºñ¿¡¼­ º¸¼® »èÁ¦
		pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 0 ].m_btInvIDX =  pPacket->m_cli_CRAFT_BREAKUP_REQ.m_btTargetInvIDX;
		pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 0 ].m_ITEM		= *pInITEM;

		// º¸¼® ºÐ¸® µÆÀ½.
		short nInvIDX = this->Add_ITEM( sOutITEM );
		if ( nInvIDX > 0 ) {
			//pInITEM->m_nGEM_OP = 0;				// Àåºñ¿¡¼­ º¸¼® »èÁ¦
			//pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 0 ].m_btInvIDX =  pPacket->m_cli_CRAFT_BREAKUP_REQ.m_btTargetInvIDX;
			//pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 0 ].m_ITEM		= *pInITEM;

			pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 1 ].m_btInvIDX = (BYTE)nInvIDX;
			pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ 1 ].m_ITEM		= sOutITEM;
			
			this->Send_gsv_CRAFT_ITEM_REPLY( pCPacket, btResult, 2 );
		} else {
			this->Send_gsv_CRAFT_ITEM_REPLY( pCPacket, btResult, 1 );
			this->Save_ItemToFILED( sOutITEM );	// º¸¼® ºÐ¸®ÈÄ ÀÎº¥Åä¸® ¸ðÀÚ¶÷...
		}
		return true;
	} // if ( !pInITEM->IsEnableDupCNT() && pInITEM->HasSocket() && pInITEM->GetGemNO() > 300 ) 
	else {
		// ºÐÇØ.
		if ( 0 == ITEM_PRODUCT_IDX( pInITEM->GetTYPE(), pInITEM->GetItemNO() ) ) {
			// Àç·á ¹øÈ£ ¾ø´Â°Ç ºÐÇØ ¸ø½ÃÄÑ !!!
			return true;	// false
		}

		short ORI_QUAL		= ITEM_QUALITY	  ( pInITEM->GetTYPE(), pInITEM->GetItemNO() );
		short nProductIDX	= ITEM_PRODUCT_IDX( pInITEM->GetTYPE(), pInITEM->GetItemNO() );
		tagITEM sOutITEM;
		if ( PRODUCT_RAW_MATERIAL( nProductIDX ) ) {	// Àç·á Á¾·ù...
			short TEMP = ( ORI_QUAL - 20 ) / 12;			// nTemp = ORI_QUAL
			if ( TEMP < 1 ) TEMP = 1;
			else
			if ( TEMP > 10 ) TEMP = 10;

			sOutITEM.Clear();
			sOutITEM.m_cType     = ITEM_TYPE_NATURAL;
			sOutITEM.m_nItemNo   = ( PRODUCT_RAW_MATERIAL( nProductIDX ) - 421 ) * 10 + TEMP;
		} else {
			if ( 0 == PRODUCT_NEED_ITEM_NO(nProductIDX,0) ) {
				// ÀÔ·Â ¿À·ù~~~
				return true;
			}
			sOutITEM.Init( PRODUCT_NEED_ITEM_NO(nProductIDX,0), 1 );
		}

		int iTmpVAR;
		if ( bUseMP ) {
			iTmpVAR = CCal::GetMP_WhenBreakupMAT( ORI_QUAL );
			if ( this->Get_MP() < iTmpVAR ) {
				// ¿¥ ¾ø´Ù
				return true;
			}
			this->Sub_MP( iTmpVAR );
		} else {
			iTmpVAR = CCal::GetMONEY_WhenBreakupMAT( ORI_QUAL );
			if ( this->GetCur_MONEY() < iTmpVAR ) {
				// µ· ¾ø´Ù.
				return true;
			}
			this->Sub_CurMONEY( iTmpVAR );
		}


		classPACKET *pCPacket = this->Init_gsv_CRAFT_ITEM_REPLY ();
		if ( !pCPacket )
			return false;

		short DURATION, LIFE, MAT_NUM;
		if ( pInITEM->IsEquipITEM() ) {
			DURATION = pInITEM->GetDurability();
			LIFE     = pInITEM->GetLife();
		} else {
			DURATION = 0;
			LIFE	 = 100;
		}

		short nStep = 0,nOutLogCnt=0;
		BYTE  btOutCNT=0;
		tagITEM sOutLogITEM[ CREATE_ITEM_STEP ];
		while( true ) {
			//switch( ITEM_TYPE( sOutITEM.GetTYPE(), sOutITEM.GetItemNO() ) ) {
			//	case 427 :	// ¿¬±ÝÀç·á
			//	case 428 :	// È­ÇÐÇ°
			//		// ºÐÇØ ¾ÈµÇ°í »ç¶óÁø´Ù..
			//		break;
			//	default :
					MAT_NUM = PRODUCT_NEED_ITEM_CNT( nProductIDX, nStep );
					sOutITEM.m_uiQuantity = (ULONG)( ( MAT_NUM * (111+RANDOM(40)) * (DURATION/2+LIFE/10+100) ) / 60000 );

					if ( sOutITEM.GetQuantity() > 0 ) {
						sOutLogITEM[ nOutLogCnt ++ ] = sOutITEM;

						iTmpVAR = this->Add_ITEM( sOutITEM );
						if ( iTmpVAR > 0 ) {
							pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ btOutCNT ].m_btInvIDX = (BYTE)iTmpVAR;
							pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ btOutCNT ].m_ITEM	   = sOutITEM;
							btOutCNT ++;
						} else {
							this->Save_ItemToFILED( sOutITEM );	// ¾ÆÀÌÅÛ ºÐ¸®ÈÄ 
						}
					}
			// }

			if ( ++nStep >= CREATE_ITEM_STEP )
				break;
			if ( 0 == PRODUCT_NEED_ITEM_NO ( nProductIDX, nStep ) )
				break;

			sOutITEM.Init( PRODUCT_NEED_ITEM_NO ( nProductIDX, nStep ), 1 );
		}

		#ifdef	__NEW_LOG
			g_pThreadLOG->When_CreateOrDestroyITEM ( this, pInITEM, sOutLogITEM, nOutLogCnt, NEWLOG_BREAKUP, NEWLOG_SUCCESS );
		#else
			g_pThreadLOG->When_BreakupITEM( this, pInITEM );
		#endif
		
		// ºÐÇØµÈ ¾ÆÀÌÅÛ Á¦°Å...
		if ( pInITEM->IsEnableDupCNT() ) {
			pInITEM->SubQuantity ();		// °¹¼ö 1°³ Á¦°Å
		} else {
			pInITEM->Clear();
		}
		pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ btOutCNT ].m_btInvIDX = pPacket->m_cli_CRAFT_BREAKUP_REQ.m_btTargetInvIDX;
		pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ btOutCNT ].m_ITEM = *pInITEM;
		btOutCNT ++;

		this->Send_gsv_CRAFT_ITEM_REPLY( pCPacket, CRAFE_BREAKUP_SUCCESS, btOutCNT );
	}


	return true;
}
/// ¾ÆÀÌÅÛ Àç·Ã ¿äÃ»
bool classUSER::Proc_CRAFT_UPGRADE_REQ( t_PACKET *pPacket, bool bUseMP )
{
	if ( pPacket->m_cli_CRAFT_UPGRADE_REQ.m_btTargetInvIDX <  MAX_EQUIP_IDX ||
		 pPacket->m_cli_CRAFT_UPGRADE_REQ.m_btTargetInvIDX >= INVENTORY_SHOT_ITEM0 )
		return false;

	tagITEM *pInITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_CRAFT_UPGRADE_REQ.m_btTargetInvIDX ];
	if ( !pInITEM->IsEquipITEM() )
		return true;
	if ( pInITEM->GetGrade() >= 9 )		// ´õÀÌ»ó Àç·Ã ºÒ°¡..
		return true;

	short nProductIDX;
	if ( ITEM_TYPE_WEAPON == pInITEM->GetTYPE() )
		nProductIDX = pInITEM->GetGrade() + 1;
	else
		nProductIDX = pInITEM->GetGrade() + 11;

	if ( pPacket->m_cli_CRAFT_UPGRADE_REQ.m_btUseItemINV[ 0 ] <  INVENTORY_ETC_ITEM0 ||
		 pPacket->m_cli_CRAFT_UPGRADE_REQ.m_btUseItemINV[ 0 ] >= INVENTORY_ETC_ITEM0 + INVENTORY_PAGE_SIZE ) {
		return false;
	}

	tagITEM *pMatITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_CRAFT_UPGRADE_REQ.m_btUseItemINV[ 0 ] ];
	if ( PRODUCT_RAW_MATERIAL(nProductIDX) != ITEM_TYPE(pMatITEM->GetTYPE(), pMatITEM->GetItemNO() ) ) {
		return true;
	}
	short MAT_QUAL  = ITEM_QUALITY( pMatITEM->GetTYPE(), pMatITEM->GetItemNO() );
	short ITEM_QUAL = ITEM_QUALITY( pInITEM->GetTYPE(), pInITEM->GetItemNO() );
	short nStep;
	if ( bUseMP ) {
		nStep = CCal::GetMP_WhenUpgradeEQUIP( pInITEM->GetGrade(), ITEM_QUAL );
		if ( this->Get_MP() < nStep ) {
			// ¿¥ ¾ø´Ù
			return true;
		}
		this->Sub_MP( nStep );
	} else {
		nStep = CCal::GetMONEY_WhenUpgradeEQUIP( pInITEM->GetGrade(), ITEM_QUAL );
		if ( this->GetCur_MONEY() < nStep ) {
			// µ· ¾ø´Ù.
			return true;
		}
		this->Sub_CurMONEY( nStep );
	}

	classPACKET *pCPacket = this->Init_gsv_CRAFT_ITEM_REPLY ();
	if ( !pCPacket )
		return false;

	BYTE btOutCNT=0;
	tagITEM sNeedITEM;
	nStep = 0;
	while( true ) {
		// °¹¼ö¸¸Å­ ¼Ò¸ð...
		if ( !pMatITEM->SubQuantity( PRODUCT_NEED_ITEM_CNT( nProductIDX, nStep ) ) ) {
			return this->Send_gsv_CRAFT_ITEM_RESULT( CRAFT_UPGRADE_INVALID_MAT );
		}
		pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ btOutCNT ].m_btInvIDX = pPacket->m_cli_CRAFT_UPGRADE_REQ.m_btUseItemINV[ nStep ];
		pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ btOutCNT ].m_ITEM = *pMatITEM;
		btOutCNT ++;

		if ( ++nStep >= UPGRADE_ITEM_STEP )
			break;
		if ( 0 == PRODUCT_NEED_ITEM_NO(nProductIDX, nStep) )
			break;

		if ( pPacket->m_cli_CRAFT_UPGRADE_REQ.m_btUseItemINV[ nStep ] <  INVENTORY_ETC_ITEM0 ||
			 pPacket->m_cli_CRAFT_UPGRADE_REQ.m_btUseItemINV[ nStep ] >= INVENTORY_ETC_ITEM0 + INVENTORY_PAGE_SIZE ) {
			return false;
		}

		pMatITEM = &this->m_Inventory.m_ItemLIST[ pPacket->m_cli_CRAFT_UPGRADE_REQ.m_btUseItemINV[ nStep ] ];
		sNeedITEM.Init( PRODUCT_NEED_ITEM_NO( nProductIDX, nStep ) );
		if ( pMatITEM->GetHEADER() != sNeedITEM.GetHEADER() ) {
			return this->Send_gsv_CRAFT_ITEM_RESULT( CRAFT_UPGRADE_INVALID_MAT );
		}
	}

	// Àç·Ã ½ÃÀÛÀ» ÁÖº¯¿¡ Åëº¸...
	this->Send_gsv_ITEM_RESULT_REPORT( REPORT_ITEM_UPGRADE_START, static_cast<BYTE>( pInITEM->GetTYPE() ), pInITEM->GetItemNO() );

	short SUC;
	BYTE btResult, btBeforeGrade;
	btBeforeGrade = pInITEM->m_cGrade;
	// ¼º°ø·ü °è»ê
	SUC = ( ( (pInITEM->GetGrade()+2) * (pInITEM->GetGrade()+3) * ( pInITEM->GetGrade()*5 + ITEM_QUAL*3 + 250 )
		* ( 61+RANDOM(100) ) * 320 ) / ( MAT_QUAL * (pInITEM->GetDurability()+180) * ( Get_WorldPROD()+10 ) ) ) + 200;
	if ( SUC < 1000 ) {	// ¼º°ø
		SUC = ( 200+(MAT_QUAL+5)*10 + (1+RANDOM(100))*3 - ( pInITEM->GetGrade()+6)*80 ) / 40;
		if ( SUC > 0 ) {
			if ( pInITEM->m_cDurability + SUC > 120 )
				pInITEM->m_cDurability = 120;
			else
				pInITEM->m_cDurability += SUC;
		}

		btResult = CRAFT_UPGRADE_SUCCESS;
		pInITEM->m_cGrade ++;

		g_pThreadLOG->When_UpgradeITEM( this, pInITEM, btBeforeGrade, bUseMP ? NEWLOG_UPGRADE_SUC_WITH_SKILL : NEWLOG_UPGRADE_SUC_FROM_NPC );
	} else {			// ½ÇÆÐ ½Ã µî±Þ °¨¼Ò
		// ³»±¸µµ º¯È­.
		SUC = pInITEM->m_cDurability + ( 200+(MAT_QUAL+5)*10 + (1+RANDOM(100))*3 - ( pInITEM->GetGrade()+6)*80 ) / 40;
		if ( SUC > 120 )
			pInITEM->m_cDurability = 120;
		else
		if ( SUC < 0 )
			pInITEM->m_cDurability = 0;
		else
			pInITEM->m_cDurability = SUC;

		btResult = CRAFT_UPGRADE_FAILED;
		nStep = (short)( ( pInITEM->GetGrade()+1 ) * ( pInITEM->GetGrade()+10 ) / (41+RANDOM(100)) );
		if ( nStep > pInITEM->GetGrade() )
			pInITEM->m_cGrade = 0;
		else
			pInITEM->m_cGrade -= nStep;

		g_pThreadLOG->When_UpgradeITEM( this, pInITEM, btBeforeGrade, NEWLOG_UPGRADE_FAILED );
	}

	pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ btOutCNT ].m_btInvIDX	=  pPacket->m_cli_CRAFT_UPGRADE_REQ.m_btTargetInvIDX;
	pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ btOutCNT ].m_ITEM		= *pInITEM;
	btOutCNT ++;

	pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ btOutCNT ].m_btInvIDX	=  0;
	pCPacket->m_gsv_CRAFT_ITEM_REPLY.m_sInvITEM[ btOutCNT ].m_ITEM.m_uiQuantity = SUC;
	btOutCNT ++;

	this->Send_gsv_CRAFT_ITEM_REPLY( pCPacket, btResult, btOutCNT );

	//if ( Equipment being installed is not upgraded ...
	//	this->UpdateAbility ();		// upgrade...
	//	this->Send_gsv_EQUIP_ITEM( pPacket->m_cli_APPRAISAL_REQ.m_wInventoryIndex, pITEM );
	//}

	return true;
}

//-------------------------------------------------------------------------------------------------
/// ¾ÆÀÌÅÛ Àç¹Ö/ºÐÇØ/Àç·Ã °ü·Ã ¿äÃ»
bool classUSER::Recv_cli_CRAFT_ITEM_REQ( t_PACKET *pPacket )
{
	switch( pPacket->m_cli_CRAFT_ITEM_REQ.m_btTYPE ) {
		case CRAFT_GEMMING_REQ		:
			return Proc_CRAFT_GEMMING_REQ( pPacket );
		case CRAFT_BREAKUP_USE_SKILL:
		{
			short nSkillIDX = this->m_Skills.m_nSkillINDEX[ pPacket->m_cli_CRAFT_BREAKUP_REQ.m_nSkillSLOTorNpcIDX ];
			if ( 2 != SKILL_TYPE( nSkillIDX ) || 41 != SKILL_ITEM_MAKE_NUM( nSkillIDX ) )
				return false;

			return Proc_CRAFT_BREAKUP_REQ( pPacket, true );
		}
		case CRAFT_BREAKUP_FROM_NPC	:
		{
			CObjNPC *pCharNPC;
			pCharNPC = (CObjNPC*)g_pObjMGR->Get_GameOBJ( pPacket->m_cli_CRAFT_BREAKUP_REQ.m_nSkillSLOTorNpcIDX, OBJ_NPC );
			if ( !pCharNPC ) {	// »óÁ¡ ÁÖÀÎÀÌ ¾ø¾î???
				return false;
			}
			int iDistance = distance ((int)m_PosCUR.x, (int)m_PosCUR.y, (int)pCharNPC->m_PosCUR.x, (int)pCharNPC->m_PosCUR.y);
			if ( iDistance > MAX_TRADE_DISTANCE )	{	// »óÁ¡ npc¿ÍÀÇ °Å·¡ Ã¼Å©...
				return true;
			}

			return Proc_CRAFT_BREAKUP_REQ( pPacket );
		}
		case CRAFT_UPGRADE_USE_SKILL:
		{
			short nSkillIDX = this->m_Skills.m_nSkillINDEX[ pPacket->m_cli_CRAFT_UPGRADE_REQ.m_nSkillSLOTorNpcIDX ];
			if ( 2 != SKILL_TYPE( nSkillIDX ) || 42 != SKILL_ITEM_MAKE_NUM( nSkillIDX ) )
				return false;

			return Proc_CRAFT_UPGRADE_REQ( pPacket, true );
		}
		case CRAFT_UPGRADE_FROM_NPC	:
		{
			CObjNPC *pCharNPC;
			pCharNPC = (CObjNPC*)g_pObjMGR->Get_GameOBJ( pPacket->m_cli_CRAFT_UPGRADE_REQ.m_nSkillSLOTorNpcIDX, OBJ_NPC );
			if ( !pCharNPC ) {	// »óÁ¡ ÁÖÀÎÀÌ ¾ø¾î???
				return false;
			}
			int iDistance = distance ((int)m_PosCUR.x, (int)m_PosCUR.y, (int)pCharNPC->m_PosCUR.x, (int)pCharNPC->m_PosCUR.y);
			if ( iDistance > MAX_TRADE_DISTANCE ) {	// »óÁ¡ npc¿ÍÀÇ °Å·¡ Ã¼Å©...
				return true;
			}

			return Proc_CRAFT_UPGRADE_REQ( pPacket );
		}
	} // switch( pPacket->m_cli_CRAFT_ITEM_REQ.m_btTYPE )

	return false;
}

//-------------------------------------------------------------------------------------------------
/// ÆÄÆ¼ ·ê º¯°æ
bool classUSER::Recv_cli_PARTY_RULE( t_PACKET *pPacket )
{
	if ( this->GetPARTY() && this == this->GetPARTY()->GetPartyOWNER() ) {
		this->GetPARTY()->Set_PartyRULE( pPacket->m_cli_PARTY_RULE.m_btPartyRUEL );
	}
	return true;
}


//-------------------------------------------------------------------------------------------------
/// ¸ð´ÏÅÍ¸µ Åø¿¡ ¼­¹ö ¹öÁ¯,½ÃÀÛ½Ã°£ Àü¼Û
bool classUSER::Recv_mon_SERVER_LIST_REQ( t_PACKET *pPacket )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

//	g_pListSERVER->Make_srv_SERVER_LIST_REPLY( pCPacket );
	pCPacket->m_HEADER.m_wType = GSV_SERVER_LIST_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( srv_SERVER_LIST_REPLY );

	pCPacket->m_srv_SERVER_LIST_REPLY.m_dwBuildNO   = GetServerBuildNO();
	pCPacket->m_srv_SERVER_LIST_REPLY.m_dwStartTIME = GetServerStartTIME();
	pCPacket->m_srv_SERVER_LIST_REPLY.m_nServerCNT  = 0;

	this->Send_Start( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
	return true;
}
/// ¸ð´ÏÅÍ¸µ Åø¿¡ »ç¿ëÀÚ¼ö Àü¼Û
bool classUSER::Recv_mon_SERVER_STATUS_REQ( t_PACKET *pPacket )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = WLS_SERVER_STATUS_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( wls_SERVER_STATUS_REPLY );

	pCPacket->m_wls_SERVER_STATUS_REPLY.m_dwTIME = pPacket->m_wls_SERVER_STATUS_REPLY.m_dwTIME;
	pCPacket->m_wls_SERVER_STATUS_REPLY.m_nServerCNT = 0;//g_pZoneLIST->GetZoneCNT();
	pCPacket->m_wls_SERVER_STATUS_REPLY.m_iUserCNT = g_pUserLIST->Get_AccountCNT();

	this->Send_Start( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
	return true;
}
/// ¸ð´ÏÅÍ¸µÅø¿¡¼­ °øÁö »çÇ× ¹ÞÀ½
bool classUSER::Recv_mon_SERVER_ANNOUNCE( t_PACKET *pPacket )
{
	short nOffset = sizeof( t_PACKETHEADER );
	char *szMsg, *szID;

	szMsg = Packet_GetStringPtr( pPacket, nOffset );
	szID  = Packet_GetStringPtr( pPacket, nOffset );

	if ( szMsg )
		g_pZoneLIST->Send_gsv_ANNOUNCE_CHAT( szMsg, szID );

	return true;
}

//-------------------------------------------------------------------------------------------------
/**
 * Á¸ °øÁö
 */
bool classUSER::Recv_ost_SERVER_ZONEANNOUNCE( t_PACKET *pPacket )
{
	if( !pPacket )
		return true;
	short nOffset = sizeof( ost_SERVER_ZONEANNOUNCE );
	char * szMsg = Packet_GetStringPtr( pPacket, nOffset );
	char * szID  = Packet_GetStringPtr( pPacket, nOffset );
	if( !szMsg )
		return true;
	if( !g_pZoneLIST->IsValidZONE( pPacket->m_ost_SERVER_ZONEANNOUNCE.m_nZoneNO ) )
		return true;

	classPACKET *pCPacket = Packet_AllocNLock ();

	pCPacket->m_HEADER.m_wType = GSV_ANNOUNCE_CHAT;
	pCPacket->m_HEADER.m_nSize = sizeof( t_PACKETHEADER );

	Packet_AppendString( pCPacket, szMsg );
	if ( szID ) {
		g_LOG.CS_ODS( 0xffff, "[Zone Announce:%s] %s\n", szID, szMsg );
		Packet_AppendString( pPacket, szMsg );
	} else {
		g_LOG.CS_ODS( 0xffff, "[Zone Announce] %s\n", szMsg );
	}
	
	CZoneTHREAD * pAnnounceZONE = g_pZoneLIST->GetZONE( pPacket->m_ost_SERVER_ZONEANNOUNCE.m_nZoneNO );
	pAnnounceZONE->SendPacketToZONE( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/**
 * À¯Àú¸¦ °­Á¦·Î ·Î±×¾Æ¿ô ½ÃÅ´
 */
bool  classUSER::Recv_ost_SERVER_USERLOGOUT( t_PACKET* pPacket )
{
	short nOffset   = sizeof( ost_SERVER_USERLOGOUT );
	char *szAccount = Packet_GetStringPtr( pPacket, nOffset );
	if( !szAccount )
		return true;

	classUSER * pUserToLogOut = ::g_pUserLIST->Find_ACCOUNT( szAccount );
	if( !pUserToLogOut )
		return true;
	bool bLogOuted = g_pUserLIST->Kick_ACCOUNT( szAccount );

	Send_gsv_SERVER_USERLOGOUT_REPLY( szAccount, bLogOuted );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// °­Á¦ ·Î±×¾Æ¿ô ÀÀ´ä
bool  classUSER::Send_gsv_SERVER_USERLOGOUT_REPLY( const char * szAccount , bool bLogOuted )
{
	if( !szAccount )
		return true;

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;
	pCPacket->m_HEADER.m_wType = GSV_SERVER_USERLOGOUT_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SERVER_USERLOGOUT_REPLY );
	pCPacket->m_gsv_SERVER_USERLOGOUT_REPLY.m_btRESULT = (BYTE)bLogOuted;
	pCPacket->AppendString( (char *)szAccount );

	this->SendPacket( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
	return true;

}

//-------------------------------------------------------------------------------------------------
// Å¬¶óÀÌ¾ðÆ®·ÎºÎÅÍ À¯Àú Á¤º¸¸¦ ¿äÃ»À» ¹ÞÀ½
bool  classUSER::Recv_ost_SERVER_USERINFO( t_PACKET* pPacket )
{
	short nOffset   = sizeof( ost_SERVER_USERINFO );
	char *szAccount = Packet_GetStringPtr( pPacket, nOffset );
	if( !szAccount )
		return true;

	classUSER * pUSER = ::g_pUserLIST->Find_ACCOUNT( szAccount );
	Send_gsv_SERVER_USERINFO_REPLY( szAccount, pUSER );

	return true;
}

//-------------------------------------------------------------------------------------------------
// À¯Àú Á¤º¸¸¦ ¿äÃ»¿¡ ´ëÇÑ ÀÀ´ä
bool  classUSER::Send_gsv_SERVER_USERINFO_REPLY( const char * szACCOUNT, classUSER * pUSER )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
			return false;
	
	pCPacket->m_HEADER.m_wType = GSV_SERVER_USERINFO_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SERVER_USERINFO_REPLY );
	if( !pUSER || !pUSER->GetZONE() ) {
		pCPacket->m_gsv_SERVER_USERINFO_REPLY.m_btEXIST = 0;
		pCPacket->m_gsv_SERVER_USERINFO_REPLY.m_nZoneNO = 0;
		pCPacket->m_gsv_SERVER_USERINFO_REPLY.m_nX = 0;
		pCPacket->m_gsv_SERVER_USERINFO_REPLY.m_nY = 0;
		pCPacket->m_gsv_SERVER_USERINFO_REPLY.m_ulIP = 0;
		pCPacket->AppendString( (char *)szACCOUNT );
	} else {
		pCPacket->m_gsv_SERVER_USERINFO_REPLY.m_btEXIST = 1;
		pCPacket->m_gsv_SERVER_USERINFO_REPLY.m_nZoneNO = pUSER->GetZONE()->Get_ZoneNO();
		pCPacket->m_gsv_SERVER_USERINFO_REPLY.m_nX = (short)(pUSER->Get_CurXPOS() / 1000) ;
		pCPacket->m_gsv_SERVER_USERINFO_REPLY.m_nY = (short)(pUSER->Get_CurYPOS() / 1000) ;
		pCPacket->m_gsv_SERVER_USERINFO_REPLY.m_ulIP = inet_addr( pUSER->Get_IP() );
		pCPacket->AppendString( pUSER->Get_NAME() );
		pCPacket->AppendString( pUSER->Get_ACCOUNT() );
	}
	this->SendPacket( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// Æ¯Á¤ À¯Àú¿¡ ´ëÇØ °­Á¦·Î »óÅÂ¸¦ ¹Ù²Þ ¿äÃ»À» Å¬¶óÀÌ¾ðÆ®·ÎºÎÅÍ ¹ÞÀ½
bool  classUSER::Recv_ost_SERVER_CHGUSER( t_PACKET* pPacket )
{
	if(!pPacket)
		return true;

	short nOffSet = (short)sizeof( ost_SERVER_CHGUSER );
	char * szACCOUNT = Packet_GetStringPtr( pPacket, nOffSet );
	if( !szACCOUNT )
		return true;

	classUSER * pUSER = ::g_pUserLIST->Find_ACCOUNT( szACCOUNT );
	
	return Send_gsv_SERVER_CHGUSER_REPLY( pUSER, 
		pPacket->m_ost_SERVER_CHGUSER.m_dwSTATUS, 
		pPacket->m_ost_SERVER_CHGUSER.m_dwCMD);
}

#define OST_CHAT_BLOCK_TIME		30 // ±âº» Ã¤ÆÃºí·° Å¸ÀÓ
//-------------------------------------------------------------------------------------------------
/// Æ¯Á¤ À¯Àú¿¡ ´ëÇØ °­Á¦·Î »óÅÂ¸¦ ¹Ù²Þ.
bool  classUSER::Send_gsv_SERVER_CHGUSER_REPLY( classUSER * pUSER, DWORD dwSTATUS, DWORD dwCMD )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	

	DWORD dwResCMD = 0;
	pCPacket->m_HEADER.m_wType = GSV_SERVER_CHGUSER_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SERVER_CHGUSER_REPLY );
	pCPacket->m_gsv_SERVER_CHGUSER_REPLY.m_dwSTATUS = dwSTATUS;
	if( !pUSER ) {
		pCPacket->m_gsv_SERVER_CHGUSER_REPLY.m_dwCMD = 0;
	} else {
		DWORD dwBitMASK = 1;
		for( short nI = 0; nI < sizeof( DWORD ) * 8; nI++ ) {
			switch( dwSTATUS & dwBitMASK )
			{
			case OST_CHGUSER_BLOCKCHAT:
				if( dwCMD & OST_CHGUSER_BLOCKCHAT ) {
					WORD wBlockTIME = OST_CHAT_BLOCK_TIME;
					pUSER->Send_gsv_GM_COMMAND( pUSER->Get_NAME(), GM_CMD_SHUT, wBlockTIME );
					dwResCMD |= OST_CHGUSER_BLOCKCHAT;
				} else {
					pUSER->Send_gsv_GM_COMMAND( pUSER->Get_NAME(), GM_CMD_SHUT, 0 );
					dwResCMD &= ~OST_CHGUSER_BLOCKCHAT;
				}
				break;
			}
			dwBitMASK <<= 1;
		}
		pCPacket->AppendString( pUSER->Get_NAME() );
		pCPacket->AppendString( pUSER->Get_ACCOUNT() );
	}

	pCPacket->m_gsv_SERVER_CHGUSER_REPLY.m_dwCMD = dwResCMD;
	this->SendPacket( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// Á¸ Á¤º¸ 
bool  classUSER::Recv_ost_SERVER_ZONEINFO( t_PACKET* pPacket )
{
	Send_gsv_SERVER_ZONEINFO_REPLY();

	return true;
}

//-------------------------------------------------------------------------------------------------
// Á¸¿¡ ´ëÇÑ Á¤º¸¸¦ Á¶»ç
bool  classUSER::Send_gsv_SERVER_ZONEINFO_REPLY( void )
{
	short nZoneCNT = ::g_pZoneLIST->GetZoneCNT();
	if( nZoneCNT <= 0 )
		return true;

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_SERVER_ZONEINFO_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SERVER_ZONEINFO_REPLY );

	ost_ZoneINFO * pZoneINFO = new ost_ZoneINFO[ nZoneCNT ];
	int iZI = 0;
	for( short nI = 0; nI < g_TblZONE.m_nDataCnt; nI++ )
	{
		if( iZI >= nZoneCNT ) // ¿©±â¼­ °É¸° °Ç µµ´ëÃ¼ ÀÌÇØ¸¦ ÇÒ¼ö ¾øÀ½. Á¸ÀÌ °©ÀÚ±â Ãß°¡µÆ³ª..
			break;

		if( !ZONE_FILE(nI) || !g_pZoneLIST->IsValidZONE( nI ) )
			continue;

		pZoneINFO[ iZI ].m_nZoneNO = g_pZoneLIST->GetZONE( nI )->Get_ZoneNO();
		pZoneINFO[ iZI ].m_nUserCNT = g_pZoneLIST->GetZONE( nI )->Get_UserCNT();
		iZI++;
	}

	pCPacket->m_gsv_SERVER_ZONEINFO_REPLY.m_nZoneCOUNT = iZI;
	pCPacket->AppendData( pZoneINFO, (short)sizeof( ost_ZoneINFO ) * iZI );
	delete [] pZoneINFO; pZoneINFO = NULL;

	this->SendPacket( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
	return true;
}

//-------------------------------------------------------------------------------------------------
/**
 * ¿Â¶óÀÎÅø¿¡¼­ ¹ÞÀº Á¤º¸·Î Ä³¸¯ÅÍ¸¦ ÀÌµ¿½ÃÅ´
 */
bool classUSER::Recv_ost_SERVER_MOVECHR( t_PACKET* pPacket )
{
	short nOffset   = sizeof( ost_SERVER_MOVECHR );
	char *szAccount = Packet_GetStringPtr( pPacket, nOffset );
	if( !szAccount )
		return true;
	if( !g_pZoneLIST->IsValidZONE( pPacket->m_ost_SERVER_MOVECHR.m_nZoneNO ) )
		return true;

	classUSER * pUSER = g_pUserLIST->Find_ACCOUNT( szAccount );
	if( !pUSER )
		return true;

	Send_gsv_SERVER_MOVECHR_REPLY( pUSER, pPacket->m_ost_SERVER_MOVECHR.m_nZoneNO, 
		pPacket->m_ost_SERVER_MOVECHR.m_nX, pPacket->m_ost_SERVER_MOVECHR.m_nY );

	return true;
}

//-------------------------------------------------------------------------------------------------
/**
 * ¿Â¶óÀÎÅø¿¡¼­ ¹ÞÀº Á¤º¸·Î Ä³¸¯ÅÍ¸¦ ÀÌµ¿½ÃÅ²ÈÄ °á°ú¸¦ Åëº¸
 */
bool classUSER::Send_gsv_SERVER_MOVECHR_REPLY( classUSER * pUSER, short nZoneNO, short nX, short nY )
{
	if( !pUSER )
		return true;

	if ( pUSER->Is_CartGuest() || !g_pZoneLIST->IsValidZONE(nZoneNO) )
		return true;

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	tPOINTF PosWARP;
	PosWARP.x = nX * 1000.f ;
	PosWARP.y = nY * 1000.f ;

	POINTS  PosSEC;
	PosSEC.x = (short) ( PosWARP.x / g_pZoneLIST->GetSectorSIZE(nZoneNO) );
	PosSEC.y = (short) ( PosWARP.y / g_pZoneLIST->GetSectorSIZE(nZoneNO) );

	// yÃà Ã¼Å©´Â ¾ÈµÊ - ·ÎÄÃÁ¸ÀÌ ¾Æ´Ò°æ¿ì g_pZoneLIST->GetZONE(nZoneNO)->Get_SectorYCNT()¿¡¼­ »¶~~
	if ( PosSEC.x >= 0 && PosSEC.y >= 0 )
	{
		pUSER->Proc_TELEPORT( nZoneNO, PosWARP );
		pUSER->m_nZoneNO = nZoneNO;
		pUSER->m_PosCUR = PosWARP;
		pCPacket->m_gsv_SERVER_MOVECHR_REPLY.m_btMOVED = 1;
	}
	else
		pCPacket->m_gsv_SERVER_MOVECHR_REPLY.m_btMOVED = 0;
	
	pCPacket->m_HEADER.m_wType = GSV_SERVER_MOVECHR_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SERVER_MOVECHR_REPLY );
	pCPacket->AppendString( pUSER->Get_NAME() );
	pCPacket->AppendString( pUSER->Get_ACCOUNT() );

	SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/**
 * IP¸¦ ÅëÇØ À¯Àú °Ë»ö
 */
bool classUSER::Recv_ost_SERVER_IPSEARCH( t_PACKET* pPacket )
{
	if( !pPacket )
		return true;

	short nOffset   = sizeof( ost_SERVER_IPSEARCH );
	char *szIP = Packet_GetStringPtr( pPacket, nOffset );
	if( !szIP )
		return true;

	classUSER * pUSER = g_pUserLIST->Find_IP( szIP );
	
	Send_gsv_SERVER_IPSEARCH_REPLY( pUSER );

	return true;
}

//-------------------------------------------------------------------------------------------------
/**
 * IP¸¦ ÅëÇØ À¯Àú °Ë»ö¿¡ ´ëÇÑ ÀÀ´ä
 */
bool classUSER::Send_gsv_SERVER_IPSEARCH_REPLY( classUSER * pUSER )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_SERVER_IPSEARCH_REPLY;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SERVER_IPSEARCH_REPLY );
	if( !pUSER ) {
		pCPacket->m_gsv_SERVER_IPSEARCH_REPLY.m_nCOUNT = 0;
	}
	else {
		pCPacket->m_gsv_SERVER_IPSEARCH_REPLY.m_nCOUNT = 1;
		pCPacket->AppendString( pUSER->Get_NAME() );
		pCPacket->AppendString( pUSER->Get_ACCOUNT() );
	}

	SendPacket( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/**
 * ÇÊ¸®ÇÉÀÇ °æ¿ì n-Protect Àû¿ë
 *	#define	AUTH_MODULE_nPROTECT	0xf1
 *	struct srv_CHECK_AUTH : public t_PACKETHEADER {
 *		BYTE	m_btModuleTYPE;
 *		// °¢ ¸ðµâº° µ¥ÀÌÅ¸...
 *	} ;
 */
bool classUSER::Send_srv_CHECK_AUTH ()
{
#if defined(__N_PROTECT) && !defined(__NORTHUSA)
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = SRV_CHECK_AUTH;
	pCPacket->m_HEADER.m_nSize = sizeof( srv_CHECK_AUTH );

	//if ( this->m_dwCSARecvTime < this->m_dwCSASendTime ) {
	//	// ¸¶Áö¸·À¸·Î º¸³½ ÆÐÅ¶ÀÇ ÀÀ´äÀÌ ¾ø¾ú´Ù. :: Â©·¯~
	//	Packet_ReleaseNUnlock( pCPacket );
	//	return false;
	//}
	DWORD dwGGErrCode = this->m_CSA.GetAuthQuery();
	if ( ERROR_SUCCESS != dwGGErrCode ) {
		// Á¢¼Ó Á¾·á :: Å¸ÀÌ¸Ó ÁÖ±âµ¿¾È ÀÎÁõ°ªÀÌ µµÂøÇÏÁö ¾Ê¾Ò°Å³ª ±âÅ¸ ¿¡·¯ ¹ß»ý 
		LogString( 0xffff, "Send ERROR on m_CSA.GetAuthQuery() :: Return:0x%x, [ 0x%x, 0x%x, 0x%x, 0x%x ]\n",
				dwGGErrCode,
				this->m_CSA.m_AuthQuery.dwIndex,
				this->m_CSA.m_AuthQuery.dwValue1,
				this->m_CSA.m_AuthQuery.dwValue2,
				this->m_CSA.m_AuthQuery.dwValue3 );
		Packet_ReleaseNUnlock( pCPacket );

#ifdef	__INC_WORLD
		return true;		// °³ÀÎ¼·Àº Â¥¸£Áö ¸»ÀÚ~~
#endif
		return false;
	}
	this->m_dwCSASendTime = ::timeGetTime();		// ¸¶Áö¸·À¸·Î º¸³½ ½Ã°£..

	pCPacket->m_srv_CHECK_AUTH.m_btModuleTYPE = AUTH_MODULE_nPROTECT;
	pCPacket->AppendData( &this->m_CSA.m_AuthQuery, sizeof( GG_AUTH_DATA ) );

	//LogString( 0xffff, "SEND:: 0x%x, 0x%x, 0x%x, 0x%x\n",
	//		this->m_CSA.m_AuthQuery.dwIndex,
	//		this->m_CSA.m_AuthQuery.dwValue1,
	//		this->m_CSA.m_AuthQuery.dwValue2,
	//		this->m_CSA.m_AuthQuery.dwValue3 );

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
#else
	// pCPacket->m_srv_CHECK_AUTH.m_btModuleTYPE = 0;
#endif
	return true;
}
/// n-protectÀÇ Ã¼Å©¿äÃ» ÀÀ´ä ÆÐÅ¶
bool classUSER::Recv_cli_CHECK_AUTH( t_PACKET *pPacket )
{
#if defined(__N_PROTECT) && !defined(__NORTHUSA)
	// ÇÊ¸®ÇÉÀÌ¸é...
	this->m_dwCSARecvTime = ::timeGetTime();		// ¸¶Áö¸·À¸·Î ¹ÞÀº ½Ã°£..
	::CopyMemory( &this->m_CSA.m_AuthAnswer, &pPacket->m_pDATA[ sizeof(cli_CHECK_AUTH) ], sizeof(GG_AUTH_DATA) );

	//LogString( 0xffff, "RECV:: 0x%x, 0x%x, 0x%x, 0x%x\n",
	//		this->m_CSA.m_AuthAnswer.dwIndex,
	//		this->m_CSA.m_AuthAnswer.dwValue1,
	//		this->m_CSA.m_AuthAnswer.dwValue2,
	//		this->m_CSA.m_AuthAnswer.dwValue3 );

	DWORD dwGGErrCode = this->m_CSA.CheckAuthAnswer();
	if ( ERROR_SUCCESS != dwGGErrCode ) {
		LogString( 0xffff, "Recv ERROR on m_CSA.CheckAuthAnswer():: Return:0x%x, [ 0x%x, 0x%x, 0x%x, 0x%x ]\n",
				dwGGErrCode,
				this->m_CSA.m_AuthAnswer.dwIndex,
				this->m_CSA.m_AuthAnswer.dwValue1,
				this->m_CSA.m_AuthAnswer.dwValue2,
				this->m_CSA.m_AuthAnswer.dwValue3 );
#ifdef	__INC_WORLD
		return true;		// °³ÀÎ¼·Àº Â¥¸£Áö ¸»ÀÚ~~
#endif
		return false;
	}
#endif
	return true;
}

/// »ç¿ë¾ÈÇÔ...
bool classUSER::Send_srv_ERROR ( WORD wErrCODE )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = SRV_ERROR;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_ERROR );
	pCPacket->m_gsv_ERROR.m_wErrorCODE = 0;
	
	this->SendPacket( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
	return true;
}
/// ³×Æ®¿÷ ¿¬°á »óÅÂ È®ÀÎ 
bool classUSER::Recv_cli_ALIVE ()
{
	g_pSockASV->Send_cli_ALIVE ( this->Get_ACCOUNT() );
	return this->Send_srv_ERROR( 0 );
}

/// Å¬·£ °ü·Ã ¸í·É::: °³ÀÎ¼­¹ö Å×½ºÆ®¿ë
bool classUSER::Recv_cli_CLAN_COMMAND( t_PACKET *pPacket )
{
#ifdef	__INC_WORLD
	if (  GCMD_CREATE == pPacket->m_cli_CLAN_COMMAND.m_btCMD ) {
		if ( this->GetClanID() || !this->CheckClanCreateCondition( 0 ) ) {
			// Ã¢¼³ Á¶°Ç ¾ÈµÇ~~~
			this->Send_wsv_CLAN_COMMAND( RESULT_CLAN_CREATE_NO_CONDITION, NULL );
			return true;
		}
	}
	return g_pThreadGUILD->Add_ClanCMD( pPacket->m_cli_CLAN_COMMAND.m_btCMD, this->m_iSocketIDX, pPacket );
#else
	if (  GCMD_CREATE == pPacket->m_cli_CLAN_COMMAND.m_btCMD ) {
		if ( this->GetClanID() || !this->CheckClanCreateCondition( 0 ) ) {
			// Ã¢¼³ Á¶°Ç ¾ÈµÇ~~~
			this->Send_wsv_CLAN_COMMAND( RESULT_CLAN_CREATE_NO_CONDITION, NULL );
			return true;
		}

		return g_pThreadGUILD->Add_ClanCMD( pPacket->m_cli_CLAN_COMMAND.m_btCMD, this->m_iSocketIDX, pPacket );
	}
	return true;
#endif
}
bool classUSER::Recv_cli_CLAN_CHAT( t_PACKET *pPacket )
{
	if ( !(this->m_dwPayFLAG & PAY_FLAG_JP_BATTLE) )
		return true;

#ifdef	__INC_WORLD
    char *szMsg;
    short nOffset=sizeof( cli_CLAN_CHAT );

    szMsg = Packet_GetStringPtr( pPacket, nOffset );
    if ( szMsg ) {
		if ( szMsg[ 0 ] == '/' ) {
			if ( this->Check_CheatCODE( szMsg ) ) {
				return ( !this->GetZONE() ? RET_SKIP_PROC : RET_OK );
			}
		}
	}

	if ( this->GetClanID() ) {
		g_pThreadGUILD->Add_ClanCMD( GCMD_CHAT, this->GetClanID(), pPacket, this->Get_NAME() );
	}
#endif

	return true;
}

//-------------------------------------------------------------------------------------------------
bool classUSER::Send_wsv_CLAN_COMMAND( BYTE btCMD, ... )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = WSV_CLAN_COMMAND;
	pCPacket->m_HEADER.m_nSize = sizeof( wsv_CLAN_COMMAND );

	pCPacket->m_wsv_CLAN_COMMAND.m_btRESULT = btCMD;

	va_list va;
	va_start( va, btCMD );
	{
		short nDataLen;
		BYTE *pDataPtr;
		while( (nDataLen=va_arg(va, short)) != NULL ) {
			pDataPtr = va_arg(va, BYTE*);
			pCPacket->AppendData( pDataPtr, nDataLen );
		}
	}
	va_end(va);

	this->SendPacket( pCPacket );

//#ifdef	__INC_WORLD
//	if ( btCMD == RESULT_CLAN_MY_DATA ) {
//		::CopyMemor( &this->m_CLAN, pCPacket->m_wsv_CLAN_COMMAND.;
//		tag_MY_CLAN
//	}
//#endif

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
bool classUSER::Send_gsv_BILLING_MESSAGE( BYTE btMsgType, char *szMsg )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_BILLING_MESSAGE;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_BILLING_MESSAGE );

	pCPacket->m_gsv_BILLING_MESSAGE.m_btTYPE = btMsgType;
	pCPacket->AppendString( szMsg);

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	if ( ( btMsgType & 0x0ff ) < 0x0f ) {
		// °ú±Ý Å¸ÀÔÀÌ´Ù.
		this->m_dwPayFLAG = ( 0x01 << btMsgType );
	}

	// ÀÓ½Ã Å×½ºÆ®...
	//CStrVAR	tmpStr;
	//tmpStr.Alloc( 200 );
	//tmpStr.Printf("Type:0x%x : %s", btMsgType, szMsg ? szMsg : "<null>" );
	//this->Send_gsv_WHISPER( "GUMS", tmpStr.Get() );

	return true;
}
bool classUSER::Send_gsv_BILLING_MESSAGE_EXT( WORD wMsgType, DWORD dwPayType, DWORD *pPlayFlag, char *szMsg )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_BILLING_MESSAGE_EXT;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_BILLING_MESSAGE_EXT );

	pCPacket->m_gsv_BILLING_MESSAGE_EXT.m_wMsgTYPE	= wMsgType;
	pCPacket->m_gsv_BILLING_MESSAGE_EXT.m_dwPayType	= dwPayType;
	::CopyMemory( pCPacket->m_gsv_BILLING_MESSAGE_EXT.m_dwPlayingFlag, pPlayFlag, 4*sizeof(DWORD) );

	if ( szMsg ) {
		// NULLÀÌ ¿À´Â °æ¿ì ÀÖÀ½
		pCPacket->AppendString( szMsg);
	}

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}
bool classUSER::Send_gsv_BILLING_MESSAGE2( BYTE btType, char cFunctionType, DWORD dwPayFlag )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_BILLING_MESSAGE;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_BILLING_MESSAGE2 );

	pCPacket->m_gsv_BILLING_MESSAGE2.m_btTYPE	= btType;
	pCPacket->m_gsv_BILLING_MESSAGE2.m_cFunctionTYPE = cFunctionType;
	pCPacket->m_gsv_BILLING_MESSAGE2.m_dwPayFlag= dwPayFlag;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );
	
	return true;
}


//-------------------------------------------------------------------------------------------------
/// È¨ÆäÀÌÁö¿¡¼­ ±¸ÀÔÇÑ ¼îÇÎ¸ô ¾ÆÀÌÅÛ °ü·Ã ¿äÃ»...
bool classUSER::Recv_cli_MALL_ITEM_REQ( t_PACKET *pPacket )
{
	switch( pPacket->m_cli_MALL_ITEM_REQ.m_btReqTYPE ) {
		case REQ_MALL_ITEM_LIST		:
		{
			// ¸ô µðºñ¿¡ ¿äÃ»
			this->m_MALL.m_HashDestCHAR = 0;
			if ( g_pThreadMALL )
				g_pThreadMALL->Add_SqlPacketWithACCOUNT( this, pPacket );
			break;
		}
		case REQ_MALL_ITEM_BRING	:
		{
			if ( pPacket->m_HEADER.m_nSize != ( 1 + sizeof( cli_MALL_ITEM_REQ ) ) )
				return false;
			if ( pPacket->m_cli_MALL_ITEM_REQ.m_btInvIDX[0] >= MAX_MALL_ITEM_COUNT )
				return false;
			if ( 0 == this->m_MALL.m_ITEMS[ pPacket->m_cli_MALL_ITEM_REQ.m_btInvIDX[0] ].m_ui64ID || 
				!this->m_MALL.m_ITEMS[ pPacket->m_cli_MALL_ITEM_REQ.m_btInvIDX[0] ].m_ITEM.IsValidITEM() ) {
				return true;
			}
			if ( g_pThreadMALL )
				g_pThreadMALL->Add_SqlPacketWithACCOUNT( this, pPacket );
			break;
		}
		case REQ_MALL_ITEM_FIND_CHAR:
		{
			short nOffset = sizeof( cli_MALL_ITEM_REQ );
			char *szCharName = Packet_GetStringPtr( pPacket, nOffset );
			if ( szCharName && *szCharName ) {
				// ÄÉ¸¯ÀÌ¸§ È®ÀÎ ¿äÃ» :: ¸ÞÀÎ µðºñ¿¡ ¿äÃ»...
				g_pThreadSQL->Add_SqlPacketWithACCOUNT( this, pPacket );
			}
			break;
		}
		case REQ_MALL_ITEM_GIVE		:
		{
			if ( pPacket->m_HEADER.m_nSize < ( 2 + sizeof( cli_MALL_ITEM_REQ ) ) )
				return false;

			if ( this->m_MALL.m_HashDestCHAR ) {
				if ( pPacket->m_cli_MALL_ITEM_REQ.m_btInvIDX[0] >= MAX_MALL_ITEM_COUNT )
					return false;
				if ( 0 == this->m_MALL.m_ITEMS[ pPacket->m_cli_MALL_ITEM_REQ.m_btInvIDX[0] ].m_ui64ID || 
					!this->m_MALL.m_ITEMS[ pPacket->m_cli_MALL_ITEM_REQ.m_btInvIDX[0] ].m_ITEM.IsValidITEM() ) {
					return true;
				}

				short nOffset = 1 + sizeof( cli_MALL_ITEM_REQ );
				char *szCharName = Packet_GetStringPtr( pPacket, nOffset );
				if ( szCharName && *szCharName ) {
					// ÄÉ¸¯ÀÌ¸§ È®ÀÎ ¿äÃ» :: ¸ÞÀÎ µðºñ¿¡ ¿äÃ»...
					t_HASHKEY HashChar = ::StrToHashKey ( szCharName );
					if ( HashChar == this->m_MALL.m_HashDestCHAR ) {
						// ¸ô µðºñ¿¡ ¿äÃ»
						if ( g_pThreadMALL )
							g_pThreadMALL->Add_SqlPacketWithACCOUNT( this, pPacket );
					}
				}
			}
			break;
		}
	} // switch( pPacket->m_cli_MALL_ITEM_REQ.m_btReqTYPE )

	return true;
}

bool  classUSER::Recv_cli_SCREEN_SHOT_TIME(t_PACKET *pPacket)                  
{
	return Send_gsv_SCREEN_SHOT_TIME();
}

 // ½º¼¦¿¡ ÇÊ¿äÇÑ  ¼­¹ö½Ã°£ Àü¼Û         (ÇãÀç¿µ Ãß°¡ 2005.10.18)
bool  classUSER::Send_gsv_SCREEN_SHOT_TIME()         
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	pCPacket->m_HEADER.m_wType = GSV_SCREEN_SHOT_TIME;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_SCREEN_SHOT_TIME );

	pCPacket->m_gsv_SCREEN_SHOT_TIME.wYear = sysTime.wYear;
	pCPacket->m_gsv_SCREEN_SHOT_TIME.btMonth = sysTime.wMonth;
	pCPacket->m_gsv_SCREEN_SHOT_TIME.btDay = sysTime.wDay;
	pCPacket->m_gsv_SCREEN_SHOT_TIME.btHour = sysTime.wHour;
	pCPacket->m_gsv_SCREEN_SHOT_TIME.btMin = sysTime.wMinute;

	this->SendPacket( pCPacket );
	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
/// »ç¿ë¾ÈÇÔ...
bool classUSER::Recv_cli_SUMMON_CMD( t_PACKET *pPacket )
{
	this->m_btSummonCMD = pPacket->m_cli_SUMMON_CMD.m_btCMD;

	return true;
}


//-------------------------------------------------------------------------------------------------
/// ¿ùµå ¼­¹ö¿¡¼­ ¹ÞÀº ÆÐÅ¶ Ã³¸®...
bool classUSER::HandleWorldPACKET (void)
{
	CSLList< t_PACKET* >::tagNODE *pNode;
	m_csSrvRECV.Lock ();
		pNode = m_SrvRecvQ.GetHeadNode();
		while( pNode ) {
			if ( pNode->m_VALUE->m_zws_CLAN_PACKET.m_HashCHAR == this->m_HashCHAR ) {
				switch( pNode->m_VALUE->m_HEADER.m_wType ) {
					case ZWS_SET_USER_CLAN :
					{
						short nOffset = sizeof( zws_SET_USER_CLAN );
						char *pClanName = Packet_GetStringPtr( pNode->m_VALUE, nOffset );

						this->m_CLAN = pNode->m_VALUE->m_zws_SET_USER_CLAN.m_ClanINFO;
						this->SetClanNAME( pClanName );
						this->Send_wsv_RESULT_CLAN_SET( pClanName );
						break;
					}
					case ZWS_DEL_USER_CLAN :
						this->ClanINIT ();
						this->Send_wsv_RESULT_CLAN_SET ();
						// Å¬·£ÀÌ »èÁ¦µÆ´Ù... Å¬·£¾ÆÁöÆ®Á¸¿¡¼­ ÀÚµ¿ ¿öÇÁ ½ÃÅ²´Ù.
						break;
				}
			}
			delete[] pNode->m_VALUE;
			m_SrvRecvQ.DeleteHead ();
			pNode = m_SrvRecvQ.GetHeadNode();
		} // while( pNode )

	m_csSrvRECV.Unlock ();
	return true;
}


//-------------------------------------------------------------------------------------------------
bool classUSER::HandlePACKET( t_PACKETHEADER *pPacketHeader )
{
    t_PACKET *pPacket = (t_PACKET*) pPacketHeader;

//  LogString (0xffff, "        >> PacketHandle(%s:%d): Type: 0x%x, Length: %d \n", Get_NAME(), Get_INDEX(), pPacket->m_HEADER.m_wType, pPacket->m_HEADER.m_nSize);

	if ( !this->GetZONE() ) {
	    switch ( pPacketHeader->m_wType ) {
			case MON_SERVER_LIST_REQ :
				this->m_bVerified = true;
				return Recv_mon_SERVER_LIST_REQ( (t_PACKET*)pPacket );
			case MON_SERVER_STATUS_REQ :
				return Recv_mon_SERVER_STATUS_REQ( (t_PACKET*)pPacket );
			case MON_SERVER_ANNOUNCE   :
				return Recv_mon_SERVER_ANNOUNCE( (t_PACKET*)pPacket);

			case OST_SERVER_USERLOGOUT:
				return Recv_ost_SERVER_USERLOGOUT( (t_PACKET*)pPacket );

			case OST_SERVER_ZONEINFO :
				return Recv_ost_SERVER_ZONEINFO( (t_PACKET*)pPacket );
				break;

			case OST_SERVER_USERINFO :
				return Recv_ost_SERVER_USERINFO( (t_PACKET*)pPacket );
				break;

			case OST_SERVER_CHGUSER	:
				return Recv_ost_SERVER_CHGUSER( (t_PACKET*)pPacket );
				break;

			case OST_SERVER_MOVECHR :
				return Recv_ost_SERVER_MOVECHR( (t_PACKET*)pPacket );
				break;

			case OST_SERVER_ZONEANNOUNCE :
				return Recv_ost_SERVER_ZONEANNOUNCE( (t_PACKET*)pPacket );
				break;

			case OST_SERVER_IPSEARCH :
				return Recv_ost_SERVER_IPSEARCH( (t_PACKET*)pPacket );
				break;
			/*
			/////////////////////////////////////////////////////////////////////
			// Login Server Packet ...
			case CLI_LOGIN_REQ :
				// TODO:: init ...
				Init_DefaultValue ();
				return Recv_cli_LOGIN_REQ( pPacket );

			case CLI_SELECT_SERVER :
				return Recv_cli_SELECT_SERVER( pPacket );
			/////////////////////////////////////////////////////////////////////
			*/
			case CLI_LOGOUT_REQ :
				if ( this->m_pPartyBUFF ) {
					this->m_pPartyBUFF->Sub_PartyUSER( this->m_nPartyPOS );		// CLI_LOGOUT_REQ
				}
				// Â©¸®µµ·Ï..
				return false;

			//case CLI_STRESS_TEST :
			//	return Recv_cli_STRESS_TEST( pPacket );

			case CLI_JOIN_SERVER_REQ :
				this->m_bVerified = true;
				return Recv_cli_JOIN_SERVER_REQ( pPacket );

#ifdef	__INC_WORLD
			case CLI_CHAR_LIST :
				return Recv_cli_CHAR_LIST( pPacket );

			case CLI_CREATE_CHAR :
				return Recv_cli_CREATE_CHAR( pPacket );

			case CLI_DELETE_CHAR :
				return Recv_cli_DELETE_CHAR( pPacket );

			case CLI_SELECT_CHAR :
				return Recv_cli_SELECT_CHAR( pPacket, true, 0, 0 );

			case CLI_MESSENGER :
				return Recv_cli_MESSENGER( pPacket );

			case CLI_MESSENGER_CHAT :
				return Recv_cli_MESSENGER_CHAT( pPacket );

			case CLI_CLAN_COMMAND :
				return this->Recv_cli_CLAN_COMMAND( pPacket );
#endif
			case CLI_ALIVE :
				return this->Recv_cli_ALIVE ();

			case CLI_CHECK_AUTH :
				return this->Recv_cli_CHECK_AUTH( pPacket );

			case CLI_JOIN_ZONE :
				this->HandleWorldPACKET ();
				// ÀÌ ÆÐÅ¶À» ¹ÞÀº ÀÌÈÄ¿¡´Â °ÔÀÓ ÇÃ·¹ÀÌ¿ë ÆÐÅ¶ÀÌ´Ù...
				return Recv_cli_JOIN_ZONE ( pPacket );
		} // switch ( pPacketHeader->m_wType )

		LogString (0xffff, "** ERROR( ZONE==NULL ): Invalid packet type: 0x%x, Size: %d, [ %s:%s, iSocketIDX: %d ] \n", pPacket->m_HEADER.m_wType, pPacket->m_HEADER.m_nSize, this->Get_ACCOUNT(), this->Get_NAME(), this->m_iSocketIDX );

		if ( ( pPacket->m_HEADER.m_wType & 0x0f00 ) == 0x0700 ) {
			// °ÔÀÓ ÆÐÅ¶ÀÌ´Ï±î ÀÚ¸£Áø ¸»ÀÚ..
			return true;
		}
	} // if ( !this->GetZONE() )


	#pragma COMPILE_TIME_MSG( "ÆÐÅ¶ °É·¯¼­ ¼ÒÄÏ Á¾·áÇÒ°÷ :: Å¬¶óÀÌ¾ðÆ®°¡ º¸³»¸é ¾ÈµÇ´Â ÆÐÅ¶À» º¸³»°í ÀÖÀ½..." )
	if ( ( pPacket->m_HEADER.m_wType & 0x0f00 ) == 0x0700 ) {
		if ( CLI_MOUSECMD != pPacket->m_HEADER.m_wType )
			g_LOG.CS_ODS (0xffff, "** Zone == NULL :: Invalid Packet: Type: 0x%x, Length: %d \n", pPacket->m_HEADER.m_wType, pPacket->m_HEADER.m_nSize);
		return true;
	}

	g_LOG.CS_ODS (0xffff, "** ERROR( Zone must NULL ):: Invalid Packet: Type: 0x%x, Length: %d \n", pPacket->m_HEADER.m_wType, pPacket->m_HEADER.m_nSize);

	return IS_HACKING( this, "HandlePACKET" );
}

#define	LOGOUT_WAIT_SECOND	10
int classUSER::Proc_ZonePACKET( t_PACKET *pPacket )
{
    switch ( pPacket->m_HEADER.m_wType ) {
#ifdef	__INC_WORLD
		case CLI_CHATROOM :
			return g_pChatROOMs->Recv_cli_CHATROOM( this, pPacket );
		case CLI_CHATROOM_MSG :
			return g_pChatROOMs->Recv_cli_CHATROOM_MSG( this, pPacket );

		case CLI_MEMO	:			// ÂÊÁö º¸³»ÀÚ...
			return g_pThreadSQL->Add_SqlPacketWithAVATAR (this, pPacket );

		case CLI_MESSENGER :
			return Recv_cli_MESSENGER( pPacket );

		case CLI_MESSENGER_CHAT :
			return Recv_cli_MESSENGER_CHAT( pPacket );

		case CLI_CLAN_CHAT :
			return Recv_cli_CLAN_CHAT( pPacket );

		case CLI_CLANMARK_SET :
			return Recv_cli_CLANMARK_SET( pPacket );
		case CLI_CLANMARK_REQ :
			return Recv_cli_CLANMARK_REQ( pPacket );
		case CLI_CLANMARK_REG_TIME :
			return Recv_cli_CLANMARK_REG_TIME( pPacket );
#endif
		case CLI_CLAN_COMMAND :
			return this->Recv_cli_CLAN_COMMAND( pPacket );

		case CLI_ALIVE :
			return this->Recv_cli_ALIVE ();

		case CLI_CHECK_AUTH :
			return this->Recv_cli_CHECK_AUTH( pPacket );

		case CLI_CHAR_CHANGE :
		{
			m_btWishLogOutMODE = LOGOUT_MODE_CHARLIST;
			if ( this->GetZONE()->GetTimeGetTIME() - this->m_dwAttackTIME < LOGOUT_WAIT_SECOND * 1000 ) {
				m_dwTimeToLogOUT = this->GetZONE()->GetTimeGetTIME() + LOGOUT_WAIT_SECOND * 1000;
				this->Send_gsv_LOGOUT_REPLY( LOGOUT_WAIT_SECOND );
			} else {
				// ¹Ù·Î Á¢¼Ó Á¾·á °¡´É...
				m_dwTimeToLogOUT = this->GetZONE()->GetTimeGetTIME();
				this->Send_gsv_LOGOUT_REPLY( 0 );
			}
//#ifdef	__INC_WORLD
//			return false;
//#endif
			//if ( this->m_pPartyBUFF ) {
			//	this->m_pPartyBUFF->Sub_PartyUSER( this->m_nPartyPOS );		// CLI_CHAR_CHANGE
			//}

			//this->GetZONE()->Dec_UserCNT ();
			//this->GetZONE()->Sub_DIRECT( this );
			//// Â©¸®µµ·Ï..
			//g_pUserLIST->DeleteUSER( this, LOGOUT_MODE_CHARLIST );
			return RET_SKIP_PROC;
		}
        case CLI_LOGOUT_REQ :
			this->Recv_cli_STOP( NULL );
			m_btWishLogOutMODE = LOGOUT_MODE_LEFT;
			if ( this->GetZONE()->GetTimeGetTIME() - this->m_dwAttackTIME < LOGOUT_WAIT_SECOND * 1000 ) {
				m_dwTimeToLogOUT   = this->GetZONE()->GetTimeGetTIME() + LOGOUT_WAIT_SECOND * 1000;
				this->Send_gsv_LOGOUT_REPLY( LOGOUT_WAIT_SECOND );
			} else {
				// ¹Ù·Î Á¢¼Ó Á¾·á °¡´É...
				m_dwTimeToLogOUT = this->GetZONE()->GetTimeGetTIME();
				this->Send_gsv_LOGOUT_REPLY( 0 );
			}
			//if ( this->m_pPartyBUFF ) {
			//	this->m_pPartyBUFF->Sub_PartyUSER( this->m_nPartyPOS );		// CLI_LOGOUT_REQ
			//}
			//// Â©¸®µµ·Ï..
			//return RET_FAILED;
			return RET_SKIP_PROC;

		case CLI_REVIVE_REQ :
		{
			if ( pPacket->m_cli_REVIVE_REQ.m_btReviveTYPE >= REVIVE_TYPE_CURRENT_POS ) 
			{
				// Å¬¶óÀÌ¾ðÆ®´Â ÇöÀç À§Ä¡ ºÎÈ° ÇÒ¼ö ¾ø´Ù !!!
				return RET_FAILED;
			}
			/*if ( IsTAIWAN() ) {
				// ´ë¸¸ ¿ÀÇÂº£Å¸ ÀÌÈÄ¿¡´Â °°ÀºÁ¸ ºÎÈ° ¾ø¾Ø´Ù...
				switch( pPacket->m_cli_REVIVE_REQ.m_btReviveTYPE ) {
					case REVIVE_TYPE_START_POS	:
					case REVIVE_TYPE_REVIVE_POS :
						// pPacket->m_cli_REVIVE_REQ.m_btReviveTYPE = REVIVE_TYPE_SAVE_POS;
						return true;
				}
			}*/

			return Recv_cli_REVIVE_REQ( pPacket->m_cli_REVIVE_REQ.m_btReviveTYPE, true, true );
		}

		case CLI_SET_REVIVE_POS :
			return Recv_cli_SET_REVIVE_POS ();

		case CLI_SET_VAR_REQ :
			return Recv_cli_SET_VAR_REQ( pPacket );

		case CLI_SET_MOTION :
			return Recv_cli_SET_MOTION( pPacket );

		case CLI_TOGGLE :
			return Recv_cli_TOGGLE( pPacket );

		case CLI_MOUSECMD :
			if ( RIDE_MODE_GUEST == this->GetCur_RIDE_MODE() )	// Ä«Æ® ¾ò¾î ÅÀ´Âµ¥ ??
				return true;
			return Recv_cli_MOUSECMD( pPacket );

		/*
		case CLI_CHAR_INFO_REQ :
			return Recv_cli_CHAR_INFO_REQ( pPacket );
		*/
		case CLI_SET_WEIGHT_RATE :
			return Recv_cli_SET_WEIGHT_RATE( pPacket->m_cli_SET_WEIGHT_RATE.m_btWeightRate, true );		// ÁÖº¯¿¡ Àü¼Û

        case CLI_CHAT :
            return Recv_cli_CHAT( pPacket );
        case CLI_WHISPER :
            return Recv_cli_WHISPER( pPacket );
        case CLI_SHOUT :
            return Recv_cli_SHOUT( pPacket );
		case CLI_PARTY_CHAT :
			return Recv_cli_PARTY_CHAT( pPacket );

		case CLI_ALLIED_CHAT :
			return Recv_cli_ALLIED_CHAT( pPacket );
		case CLI_ALLIED_SHOUT :
			return Recv_cli_ALLIED_SHOUT( pPacket );

        case CLI_STOP :
			if ( RIDE_MODE_GUEST == this->GetCur_RIDE_MODE() )	// Ä«Æ® ¾ò¾î ÅÀ´Âµ¥ ??
				return true;
            return Recv_cli_STOP( pPacket );

        case CLI_ATTACK :
			if ( RIDE_MODE_GUEST == this->GetCur_RIDE_MODE() )	// Ä«Æ® ¾ò¾î ÅÀ´Âµ¥ ??
				return true;
			return Recv_cli_ATTACK( pPacket );
        case CLI_DAMAGE :
            return Recv_cli_DAMAGE( pPacket );

		case CLI_CANTMOVE :
			if ( RIDE_MODE_GUEST == this->GetCur_RIDE_MODE() )	// Ä«Æ® ¾ò¾î ÅÀ´Âµ¥ ??
				return true;
			return Recv_cli_CANTMOVE( pPacket );
		case CLI_SETPOS :
			if ( RIDE_MODE_GUEST == this->GetCur_RIDE_MODE() )	// Ä«Æ® ¾ò¾î ÅÀ´Âµ¥ ??
				return true;
			return Recv_cli_SETPOS( pPacket );

		//case CLI_CHANGE_SKIN :
		//	return Send_gsv_CHANGE_SKIL( pPacket->m_cli_CHANGE_SKIN.m_btBodyIDX, pPacket->m_cli_CHANGE_SKIN.m_nItemNO );
		//	return Recv_cli_CHANGE_SKIN( pPacket );

		case CLI_DROP_ITEM :
			return Recv_cli_DROP_ITEM( pPacket );

		case CLI_EQUIP_ITEM :
			return Recv_cli_EQUIP_ITEM( pPacket );

		case CLI_ASSEMBLE_RIDE_ITEM :
			return Recv_cli_ASSEMBLE_RIDE_ITEM( pPacket );

		case CLI_STORE_TRADE_REQ :
			return Recv_cli_STORE_TRADE_REQ( pPacket );

        case CLI_USE_ITEM :
            return Recv_cli_USE_ITEM( pPacket );

		case CLI_SET_HOTICON :
			return Recv_cli_SET_HOTICON( pPacket );

		case CLI_SET_BULLET :
			return Recv_cli_SET_BULLET( pPacket );

		case CLI_CREATE_ITEM_REQ :
			return Recv_cli_CREATE_ITEM_REQ( pPacket );

		//case CLI_CREATE_ITEM_EXP_REQ :
		//	return Recv_cli_CREATE_ITEM_EXP_REQ ();

		case CLI_ITEM_RESULT_REPORT :
			return Recv_cli_ITEM_RESULT_REPORT( pPacket );

		case CLI_HP_REQ :
			return Recv_cli_HP_REQ( pPacket );

        case CLI_GET_FIELDITEM_REQ :
            return Recv_cli_GET_FIELDITEM_REQ( pPacket );

		case CLI_MOVE_ITEM :
			return Recv_cli_MOVE_ITEM( pPacket );
		case CLI_MOVE_ZULY :
			return Recv_cli_MOVE_ZULY( pPacket );

		case CLI_BANK_LIST_REQ :
			return Recv_cli_BANK_LIST_REQ( pPacket );

        case CLI_TELEPORT_REQ :
			if ( RIDE_MODE_GUEST == this->GetCur_RIDE_MODE() )	// Ä«Æ® ¾ò¾î ÅÀ´Âµ¥ ??
				return true;
			// Á¸ÀÌ ¹Ù²î´Â°¡???
            return Recv_cli_TELEPORT_REQ( pPacket );

		case CLI_USE_BPOINT_REQ :
			return Recv_cli_USE_BPOINT_REQ( pPacket );
/*
        case CLI_SKILL_LEARN_REQ :
            return Recv_cli_SKILL_LEARN_REQ ( pPacket->m_cli_SKILL_LEARN_REQ.m_nSkillIDX  );
*/
		case CLI_SKILL_LEVELUP_REQ :
			return Recv_cli_SKILL_LEVELUP_REQ( pPacket );

		case CLI_SELF_SKILL :
			if ( RIDE_MODE_GUEST == this->GetCur_RIDE_MODE() )	// Ä«Æ® ¾ò¾î ÅÀ´Âµ¥ ??
				return true;
			return Recv_cli_SELF_SKILL( pPacket );
		case CLI_TARGET_SKILL :
			if ( RIDE_MODE_GUEST == this->GetCur_RIDE_MODE() )	// Ä«Æ® ¾ò¾î ÅÀ´Âµ¥ ??
				return true;
			return Recv_cli_TARGET_SKILL( pPacket );
		case CLI_POSITION_SKILL :
			if ( RIDE_MODE_GUEST == this->GetCur_RIDE_MODE() )	// Ä«Æ® ¾ò¾î ÅÀ´Âµ¥ ??
				return true;
			return Recv_cli_POSITION_SKILL( pPacket );


		case CLI_TRADE_P2P :
			return Recv_cli_TRADE_P2P ( pPacket );
		case CLI_TRADE_P2P_ITEM :
			return Recv_cli_TRADE_P2P_ITEM( pPacket );

		case CLI_SET_WISHITEM :
			return Recv_cli_SET_WISHITEM( pPacket );

        case CLI_P_STORE_OPEN :
            return Recv_cli_P_STORE_OPEN( pPacket );
        case CLI_P_STORE_CLOSE :
            return Recv_cli_P_STORE_CLOSE( pPacket );

        case CLI_P_STORE_LIST_REQ :
            return Recv_cli_P_STORE_LIST_REQ( pPacket );

        case CLI_P_STORE_BUY_REQ :
            return Recv_cli_P_STORE_BUY_REQ( pPacket );
		case CLI_P_STORE_SELL_REQ :
			return Recv_cli_P_STORE_SELL_REQ( pPacket );

		case CLI_QUEST_REQ :
			return Recv_cli_QUEST_REQ( pPacket );

		case CLI_SUMMON_CMD :
			return Recv_cli_SUMMON_CMD( pPacket );

			/*
		case CLI_ADD_QUEST :
			return Recv_cli_ADD_QUEST( pPacket );
		case CLI_DEL_QUEST :
			return Recv_cli_DEL_QUEST( pPacket );
		case CLI_QUEST_DATA_REQ :
			return Recv_cli_QUEST_DATA_REQ( pPacket );
		case CLI_SET_QUEST_VAR :
			return Recv_cli_SET_QUEST_VAR( pPacket );
		case CLI_SET_QUEST_SWITCH :
			return Recv_cli_SET_QUEST_SWITCH( pPacket );
		case CLI_SET_QUEST_FAMEnPROGRESS :
			return Recv_cli_SET_QUEST_FAMEnPROGRESS( pPacket );
		case CLI_COMPLETE_QUEST :
			return Recv_cli_COMPLETE_QUEST( pPacket );
			*/
		case CLI_PARTY_REQ :
			return Recv_cli_PARTY_REQ( pPacket );
		case CLI_PARTY_REPLY :
			return Recv_cli_PARTY_REPLY( pPacket );

		case CLI_RELAY_REPLY :
			return Recv_cli_RELAY_REPLY( pPacket );

		case CLI_APPRAISAL_REQ :
			return Recv_cli_APPRAISAL_REQ( pPacket );

		case CLI_USE_ITEM_TO_REPAIR	:
			return Recv_cli_USE_ITEM_TO_REPAIR( pPacket );
		case CLI_REPAIR_FROM_NPC :
			return Recv_cli_REPAIR_FROM_NPC( pPacket );

		case CLI_CRAFT_ITEM_REQ :
		#pragma message( "#### ÇÊ¸®ÇÉ ¹öÁ¯ÀÌ¸é ¿©±â ¸·¾Æ¾ß µÊ ####" )
		#if defined(__PHILIPPINES)
			return IS_HACKING( this, "Proc_ZonePACKET:CRAFT_ITEM" );
		#else
			return Recv_cli_CRAFT_ITEM_REQ( pPacket );
		#endif

		case CLI_PARTY_RULE :
			return Recv_cli_PARTY_RULE( pPacket );

		case CLI_MALL_ITEM_REQ :
			return Recv_cli_MALL_ITEM_REQ( pPacket );

		case CLI_CART_RIDE :
			return Recv_cli_CART_RIDE( pPacket );

		case CLI_SCREEN_SHOT_TIME :
			return Recv_cli_SCREEN_SHOT_TIME( pPacket );

		case PXY_UPDATE_NAME:
			return Recv_cli_UPDATE_NAME( pPacket );
			
		case PXY_SET_RIGHTS:
			return Recv_cli_SET_RIGHTS( pPacket );

        default :
            g_LOG.CS_ODS (0xffff, "** ERROR( ZONE!=NULL ) : Invalid packet type: 0x%x, Size: %d ", pPacket->m_HEADER.m_wType, pPacket->m_HEADER.m_nSize);
    } // switch ( pPacket->m_HEADER.m_wType )

#pragma COMPILE_TIME_MSG( "ÆÐÅ¶ °É·¯¼­ ¼ÒÄÏ Á¾·áÇÒ°÷ :: Å¬¶óÀÌ¾ðÆ®°¡ º¸³»¸é ¾ÈµÇ´Â ÆÐÅ¶À» º¸³»°í ÀÖÀ½..." )
	if ( ( pPacket->m_HEADER.m_wType & 0x0f00 ) == 0x0700 ) 
		return true;

	return IS_HACKING( this, "Proc_ZonePACKET" );
}

//-------------------------------------------------------------------------------------------------
bool classUSER::Add_SrvRecvPacket( t_PACKET *pPacket )
{
	CSLList< t_PACKET* >::tagNODE *pNode;
	pNode = new CSLList< t_PACKET* >::tagNODE;
	if ( NULL == pNode )
		return false;

	pNode->m_VALUE = (t_PACKET*)new BYTE[ pPacket->m_HEADER.m_nSize ];
	::CopyMemory( pNode->m_VALUE, pPacket, pPacket->m_HEADER.m_nSize );

	m_csSrvRECV.Lock ();
		m_SrvRecvQ.AppendNode( pNode );
	m_csSrvRECV.Unlock ();
	return true;
}

bool classUSER::Recv_cli_UPDATE_NAME( t_PACKET *pPacket )
{
	short nOffset = sizeof( pxy_UPDATE_NAME );
	char *szDesc;
	szDesc = Packet_GetStringPtr( pPacket, nOffset );

	this->Set_NAME(szDesc);

	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return true;

	pCPacket->m_HEADER.m_wType	= SRV_UPDATE_NAME;
	pCPacket->m_HEADER.m_nSize	= sizeof( gsv_UPDATE_NAME );
	pCPacket->m_gsv_UPDATE_NAME.m_nCharID = this->Get_INDEX();
	pCPacket->AppendString( this->Get_NAME() );

	this->GetZONE()->SendPacketToZONE( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
	return true;
}

bool classUSER::Recv_cli_SET_RIGHTS( t_PACKET *pPacket )
{
	this->m_dwRIGHT = pPacket->m_pxy_SET_RIGHTS.wRIGHT;
	return true;
}

//-------------------------------------------------------------------------------------------------
bool classUSER::Send_wsv_RESULT_CLAN_SET (char *szClanName)
{
	if ( this->GetZONE() ) {
		classPACKET *pCPacket = Packet_AllocNLock ();
		if ( !pCPacket )
			return false;

		pCPacket->m_HEADER.m_wType = WSV_CLAN_COMMAND;
		pCPacket->m_HEADER.m_nSize = sizeof( wsv_RESULT_CLAN_SET );

		pCPacket->m_wsv_RESULT_CLAN_SET.m_btRESULT = RESULT_CLAN_SET;
		pCPacket->m_wsv_RESULT_CLAN_SET.m_wObjIDX  = this->Get_INDEX();
		pCPacket->m_wsv_RESULT_CLAN_SET.m_ClanID   = this->m_CLAN;
		if ( szClanName )
			pCPacket->AppendString( szClanName );
		this->GetZONE()->SendPacketToSectors( this, pCPacket );

		Packet_ReleaseNUnlock( pCPacket );
		return true;
	}
	return false;
}

/**
 * \brief ´ÙÀÎ½Â Å¾½Â ÆÐÅ¶ Ã³¸®
 */
bool classUSER::Recv_cli_CART_RIDE( t_PACKET *pPacket )
{
	classUSER *pUSER;

	switch( pPacket->m_cli_CART_RIDE.m_btType ) {
		case CART_RIDE_REQ :
		{
			if ( RIDE_MODE_DRIVE != this->GetCur_RIDE_MODE() || this->m_iLinkedCartObjIDX ) {
				// Å¾½Â½Ã¿¡¸¸ & ÅÂ¿ì°í ÀÖÁö ¾ÊÀ» °æ¿ì¿¡¸¸...
				return true;
			} else {
#ifdef __KCHS_BATTLECART__
				tagITEM *pChair = &this->m_Inventory.m_ItemRIDE[ RIDE_PART_ABIL	];
				if ( !pChair->GetItemNO() || 1 != PAT_ABILITY_TYPE( pChair->GetItemNO() ) ) {
					// Å¾½Â½ÃÅ°·Á¸é ¾îºô¸®Æ¼¿¡ ÀÇÀÚ°¡ ºÙ¾î¾ßÇÔ.
					return true;
				}
#else
				return true;
#endif
			}

			pUSER = g_pObjMGR->Get_UserOBJ( pPacket->m_cli_CART_RIDE.m_wGuestObjIDX );
			if ( !pUSER ) {
				this->Send_gsv_CART_RIDE( CART_RIDE_GUEST_NOT_FOUND, 
						pPacket->m_cli_CART_RIDE.m_wOwnerObjIDX,
						pPacket->m_cli_CART_RIDE.m_wGuestObjIDX );
				return true;
			}
			if ( pUSER->GetCur_RIDE_MODE() ) {
				// Å¾½ÂÁßÀÎ ´ë»óÀº ¸øÅÂ¿ö~
				return true;
			}

			pUSER->Send_gsv_CART_RIDE( CART_RIDE_REQ, 
					pPacket->m_cli_CART_RIDE.m_wOwnerObjIDX,
					pPacket->m_cli_CART_RIDE.m_wGuestObjIDX );
			return true;
		}
		case CART_RIDE_ACCEPT :
		{
			pUSER = g_pObjMGR->Get_UserOBJ( pPacket->m_cli_CART_RIDE.m_wOwnerObjIDX );
			// Åºµ¥...
			if ( !pUSER ) {
				this->Send_gsv_CART_RIDE( CART_RIDE_OWNER_NOT_FOUND, 
						pPacket->m_cli_CART_RIDE.m_wOwnerObjIDX,
						pPacket->m_cli_CART_RIDE.m_wGuestObjIDX );
				return true;
			}

			if ( this->GetCur_RIDE_MODE() || !pUSER->GetCur_RIDE_MODE() ) {
				// Å¾½Â½Ã¿¡¸¸ ÅÂ¿ì±â °¡´É...
				return true;
			}
			if ( this->m_iLinkedCartObjIDX || pUSER->m_iLinkedCartObjIDX ) {
				return true;
			}

			/// Å¾½Â »óÅÂ º¯°æÀü¿¡ Á¤Áö ¸í·ÉÇÒ´ç
			this->SetCMD_STOP();

			pUSER->m_btRideMODE = RIDE_MODE_DRIVE;
			pUSER->m_iLinkedCartObjIDX = this->Get_INDEX();
			pUSER->m_iLinkedCartUsrIDX = this->m_iSocketIDX;

			this->m_btRideMODE = RIDE_MODE_GUEST;
			this->m_iLinkedCartObjIDX = pUSER->Get_INDEX();
			this->m_iLinkedCartUsrIDX = pUSER->m_iSocketIDX;

			pUSER->Send_gsv_CART_RIDE( CART_RIDE_ACCEPT, 
					pPacket->m_cli_CART_RIDE.m_wOwnerObjIDX,
					pPacket->m_cli_CART_RIDE.m_wGuestObjIDX, true );
			break;
		}
		case CART_RIDE_REFUSE :
		{
			pUSER = g_pObjMGR->Get_UserOBJ( pPacket->m_cli_CART_RIDE.m_wOwnerObjIDX );
			// ¾ÈÅºµ¥...
			if ( !pUSER ) {
				// °ÅºÎ ÆÐÅ¶ º¸³¾ ÄÉ¸¯ÀÌ ¾ø³× ?
				return true;
			}
			pUSER->Send_gsv_CART_RIDE( CART_RIDE_REFUSE, 
					pPacket->m_cli_CART_RIDE.m_wOwnerObjIDX,
					pPacket->m_cli_CART_RIDE.m_wGuestObjIDX, true );
			break;
		}

		default :
			return false;
	} // switch( pPacket->m_cli_CART_RIDE.m_btType )

	return true;
}

/**
 * 
 *
 */
bool classUSER::Send_gsv_CART_RIDE( BYTE btType, WORD wSourObjIdx, WORD wDestObjIdx, bool bSendToSector )
{
	/*classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_CART_RIDE;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_CART_RIDE );

	pCPacket->m_gsv_CART_RIDE.m_btType = btType;
	pCPacket->m_gsv_CART_RIDE.m_wOwnerObjIDX = wSourObjIdx;
	pCPacket->m_gsv_CART_RIDE.m_wGuestObjIDX = wDestObjIdx;

	if ( bSendToSector )
		this->GetZONE()->SendPacketToSectors( this, pCPacket );
	else
		this->SendPacket( pCPacket );

	Packet_ReleaseNUnlock( pCPacket );*/
	return true;
}

//-------------------------------------------------------------------------------------------------
/**
 *	³×Æ®¿÷ ÆÐÅ¶ÀÌ ¿Ï¼ºµÇ¾î µé¾î ¿ÔÀ»¶§ Å¥¿¡ µî·ÏÇÏ°Å³ª Ã³¸®ÇÏ´Â ÇÔ¼ö
 */
bool classUSER::Recv_Done (tagIO_DATA *pRecvDATA)
{
	// ¹Ù·Î Ã³¸®ÇÒ°Í°ú Å¥¿¡ ³Ö¾î¼­ Ä¡¸®ÇÒ°ÍÀ» ±¸ºÐÇØ¾ß ÇÑ´Ù...
	// ¹Ù·Î Ã³¸®ÇÒ°ÍÀº iocpSOCKET::Recv_Done( .. )¸¦ È£Ãâ...
	if ( !this->GetZONE() ) {
		return iocpSOCKET::Recv_Done( pRecvDATA );
	}

    m_csRecvQ.Lock ();
    {
        m_RecvList.AppendNode( pRecvDATA->m_pNODE );
    }
	m_csRecvQ.Unlock ();

    return true;
}


//-------------------------------------------------------------------------------------------------
/**
 *	»ç¿ëÀÚÀÇ ·Î±×¾Æ¿ô ¿äÃ»½Ã Ã³¸® ÇÏ´Â ÇÔ¼ö
 */
int  classUSER::ProcLogOUT ()
{
	classDLLNODE<tagIO_DATA> *pRecvNODE;
	t_PACKETHEADER *pPacket;
	short nTotalPacketLEN;

	// Á¢¼Ó Á¾·á ¿ä±¸Çß´Ù.
	if ( this->GetZONE()->GetTimeGetTIME() >= m_dwTimeToLogOUT ) {
		switch( this->m_btWishLogOutMODE ) {
			case LOGOUT_MODE_CHARLIST :
				if ( this->m_pPartyBUFF ) {
					this->m_pPartyBUFF->Sub_PartyUSER( this->m_nPartyPOS );
				}

				this->GetZONE()->Dec_UserCNT ();
				this->GetZONE()->Sub_DIRECT( this );
				// Â©¸®µµ·Ï..
				g_pUserLIST->DeleteUSER( this, LOGOUT_MODE_CHARLIST );
				return 1;

			case LOGOUT_MODE_LEFT	   :
				if ( this->m_pPartyBUFF ) {
					this->m_pPartyBUFF->Sub_PartyUSER( this->m_nPartyPOS );
				}
				break;
		} // switch( this->m_btWishLogOutMODE )

		return 0;
	} else 
	if ( LOGOUT_MODE_NET_ERROR == this->m_btWishLogOutMODE ) {
		return 1;
	}

	// Á¢¼Ó Á¾·á ´ë±âÁß¿¡µµ Ã³¸®ÇØÁà¾ß ÇÑµ¥...
	if ( this->Get_HP() > 0 )
		this->Check_PerFRAME ( this->GetZONE()->GetPassTIME() );

	// Ãë¼Ò ÆÐÅ¶ÀÌ ¿Ô´Â°¡ ????
	m_csRecvQ.Lock ();
	{
		pRecvNODE = m_RecvList.GetHeadNode ();

		while( pRecvNODE ) {
			pPacket = (t_PACKETHEADER*)pRecvNODE->DATA.m_pCPacket->m_pDATA;
			do {
				nTotalPacketLEN = this->D_RecvB( pPacket );
				if ( !nTotalPacketLEN ) {
					// ÆÐÅ¶ÀÌ º¯Á¶µÇ¾î ¿Ô´Ù.
					// ÇìÅ·ÀÎ°¡ ???
					m_csRecvQ.Unlock ();
					IS_HACKING( this, "classUSER::Proc( Decode_Recv ... )" );
					return 0;
				}
				
				if ( CLI_LOGOUT_CANCEL == pPacket->m_wType && !this->m_bKickOutFromGUMS ) {
					this->m_btWishLogOutMODE = 0;
					this->m_dwTimeToLogOUT = 0;

					m_RecvList.DeleteNode( pRecvNODE );
					this->Free_RecvIODATA( &pRecvNODE->DATA );
					m_csRecvQ.Unlock ();
					return 1;
				}

				pRecvNODE->DATA.m_dwIOBytes -= nTotalPacketLEN;
				pPacket = (t_PACKETHEADER*)( pPacket->m_pDATA + nTotalPacketLEN );
			} while ( pRecvNODE->DATA.m_dwIOBytes );

			m_RecvList.DeleteNode( pRecvNODE );
			this->Free_RecvIODATA( &pRecvNODE->DATA );
			pRecvNODE = m_RecvList.GetHeadNode ();
		} // while( pRecvNODE ) 
	}
	m_csRecvQ.Unlock ();

	return 1;
}

#define	DB_BACKUP_TIME	( 1000 * 60 * 5 )	// 5 min

//-------------------------------------------------------------------------------------------------
/**
 *	»ç¿ëÀÚ·Î ºÎÅÍ ¹ÞÀº ÆÐÅ¶ Ã³¸® & ÄÉ¸¯ÅÍ Ã³¸® 
 */
int	 classUSER::Proc (void)
{
	// Á¸¾È¿¡ ÀÖÀ»¶§ Ã³¸®µÇ´Â ÇÔ¼ö´Ù...
	if ( this->m_btLogOutMODE || INVALID_SOCKET == this->Get_SOCKET() ) {
		// Á¢¼ÓÀÌ ²÷°å°Å³ª ¿À·ù...
		if ( 0 == this->m_dwTimeToLogOUT ) {
			if ( this->m_pPartyBUFF ) {
				this->m_pPartyBUFF->Sub_PartyUSER( this->m_nPartyPOS );
			}

			this->m_btWishLogOutMODE = LOGOUT_MODE_NET_ERROR;

			if ( this->GetZONE()->GetTimeGetTIME() - this->m_dwAttackTIME < LOGOUT_WAIT_SECOND * 1000 ) {
				m_dwTimeToLogOUT = this->GetZONE()->GetTimeGetTIME() + LOGOUT_WAIT_SECOND * 500;
			} else {
				// ¹Ù·Î Á¢¼Ó Á¾·á °¡´É...
				// m_dwTimeToLogOUT = this->GetZONE()->GetTimeGetTIME();
				return 0;
			}
			return 1;
		}
		return this->ProcLogOUT ();
	}

	if ( this->m_btWishLogOutMODE ) {
		return this->ProcLogOUT ();
	}

	this->HandleWorldPACKET ();

	classDLLNODE<tagIO_DATA> *pRecvNODE;
	t_PACKETHEADER *pPacket;
	short nTotalPacketLEN;

	m_csRecvQ.Lock ();
	{
		pRecvNODE = m_RecvList.GetHeadNode ();

		while( pRecvNODE ) {
			pPacket = (t_PACKETHEADER*)pRecvNODE->DATA.m_pCPacket->m_pDATA;
			do {
				nTotalPacketLEN = this->D_RecvB( pPacket );
				if ( !nTotalPacketLEN ) {
					// ÆÐÅ¶ÀÌ º¯Á¶µÇ¾î ¿Ô´Ù.
					// ÇìÅ·ÀÎ°¡ ???
					m_csRecvQ.Unlock ();
					IS_HACKING( this, "classUSER::Proc( Decode_Recv ... )" );
					return 0;
				}

				switch ( this->Proc_ZonePACKET( (t_PACKET*)pPacket ) ) {
					case RET_SKIP_PROC :
					{
						pRecvNODE->DATA.m_dwIOBytes -= nTotalPacketLEN;

						if ( 0 == pRecvNODE->DATA.m_dwIOBytes ) {
							// ´ÙÃ³¸®µÈ ÆÐÅ¶...
							m_RecvList.DeleteNode( pRecvNODE );
							this->Free_RecvIODATA( &pRecvNODE->DATA );
						} else {
							pPacket = (t_PACKETHEADER*)( pPacket->m_pDATA + nTotalPacketLEN );
							// Ã³¸®ÇÏ°í ³²Àº ºÎºÐ ´ÙÀ½¿¡ Ã³¸® ÇÒ¼ö ÀÖµµ·Ï...
							for (WORD wI=0; wI<pRecvNODE->DATA.m_dwIOBytes; wI++) {
								pRecvNODE->DATA.m_pCPacket->m_pDATA[ wI ] = pPacket->m_pDATA[ wI ];
							}
						}
						m_csRecvQ.Unlock ();
						return 1;
					}
					case RET_FAILED :
					{
						// Â¥¸¦ ³Ñ...
						m_csRecvQ.Unlock ();
						return 0;
					}
				} // switch ( this->Proc_ZonePACKET( (t_PACKET*)pPacket ) )

				pRecvNODE->DATA.m_dwIOBytes -= nTotalPacketLEN;
				pPacket = (t_PACKETHEADER*)( pPacket->m_pDATA + nTotalPacketLEN );
			} while ( pRecvNODE->DATA.m_dwIOBytes );

			m_RecvList.DeleteNode( pRecvNODE );
			this->Free_RecvIODATA( &pRecvNODE->DATA );
			pRecvNODE = m_RecvList.GetHeadNode ();
		} // while( pRecvNODE )
	}

	m_csRecvQ.Unlock ();


#if defined(__N_PROTECT) && !defined(__NORTHUSA)
	if ( this->GetZONE()->GetTimeGetTIME() - this->m_dwCSASendTime >= 10000 /* 3 * 60 * 1000 */ ) {
		if ( !this->Send_srv_CHECK_AUTH() )
			return 0;
	}
#else
	// 5ºÐµ¿¾È º¸³½ ÆÐÅ¶ÀÌ ÀüÇô ¾ø³Ä ???
	if ( this->GetZONE()->GetTimeGetTIME() - SOCKET_KEEP_ALIVE_TIME >= this->Get_CheckTIME() ) {
		return 0;
	}
#endif

	if ( this->GetZONE()->GetCurrentTIME() - this->m_dwBackUpTIME >= DB_BACKUP_TIME ) {
		this->m_dwBackUpTIME = this->GetZONE()->GetCurrentTIME();
		g_pThreadSQL->Add_BackUpUSER( this );
	}

	if ( this->Get_HP() > 0 )
		this->Check_PerFRAME ( this->GetZONE()->GetPassTIME() );
	
	if ( this->Is_CartGuest() ) {
		if ( this->Get_HP() > 0 && this->m_IngSTATUS.GetFLAGs() ) {
			this->Proc_IngSTATUS( this->GetZONE()->GetPassTIME() );
		}

		if ( 0 == this->m_iLinkedCartObjIDX ) {
			// Ä«Æ® µå¶óÀÌ¹ö¿Í ÇÔ²² ¿öÇÁÇØ¼­ »õ·Î¿î Á¸¿¡ µé¾î ¿Ô´Ù
			classUSER *pDriver = (classUSER*)g_pUserLIST->GetSOCKET( this->m_iLinkedCartUsrIDX );
			if ( pDriver && RIDE_MODE_DRIVE == pDriver->GetCur_RIDE_MODE() ) {
				if ( NULL == pDriver->GetZONE() || NULL == pDriver->m_pGroupSECTOR ) {
					// ÄÄÅÍ°¡ ´À¸°°¡ º¸´Ù...¾ÆÁ÷ Á¸¿¡ ÀÔÀåÇÏÁö ¾Ê¾ÒÀ½
					return true;
				}

				if ( pDriver->GetZONE() == this->GetZONE() ) {
					this->m_iLinkedCartObjIDX = pDriver->Get_INDEX ();
					pDriver->m_iLinkedCartObjIDX = this->Get_INDEX ();

					this->SetCMD_STOP();
					// send ride on packet to around clients...
					return this->Send_gsv_CART_RIDE( CART_RIDE_ACCEPT, pDriver->Get_INDEX (), this->Get_INDEX(), true );
				} // else ÀÌ·±°æ¿ì´Â ¾øÀ»µí...
			}
			
			// 1. ¿îÀüÀÚÀÇ Á¢¼Ó Á¾·á
			// 2. ¿îÀüÀÚ¿Í ÇÔ²² ¿öÇÁÇÏ°í Á¸¿¡ ÀÔÀåÇØ º¸´Ï µå¶óÀÌ¹ö°¡ Ä«Æ®¿¡¼­ ³»·È´Ù ??
			// 3. ¿îÀüÀÚ¿Í Á¸ÀÌ Æ²¸®´Ù 
			this->m_btRideMODE = 0;
			this->m_btRideATTR = RIDE_ATTR_NORMAL;
			this->m_iLinkedCartObjIDX = 0;
			this->m_iLinkedCartUsrIDX = 0;

			// send ride off packet to around clients...
			return this->SetCMD_TOGGLE( TOGGLE_TYPE_DRIVE, true ); 
		}

		return true;
	}
	
	// ¿öÇÁ ¸í·É ÀÌÈÄ¿¡´Â ...
	// ZoneÀÌ NULLÀÌ µÇ¾î ¾Æ·¡ ÇÔ¼ö È£Ãâ½Ã »¶~~~~~~~
	return CObjCHAR::Proc ();
}

/**
 * \param dwFLAG	:: »óÅÂÁö¼Ó ºñÆ® ÇÃ·¡±×	
 */
bool  classUSER::Send_gsv_CHARSTATE_CHANGE( DWORD dwFLAG )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType = GSV_CHARSTATE_CHANGE;
	pCPacket->m_HEADER.m_nSize = sizeof( gsv_CHARSTATE_CHANGE );

	pCPacket->m_gsv_CHARSTATE_CHANGE.m_wObjectIDX = this->Get_INDEX();
	pCPacket->m_gsv_CHARSTATE_CHANGE.m_dwFLAG = dwFLAG;

	this->GetZONE()->SendPacketToSectors( this, pCPacket );

	Packet_ReleaseNUnlock( pCPacket );
	return true;
}

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Ã¤ÆÃÀ» ÆÄÀÏ¿¡ ·Î±×·Î ³²±è ( ´ë¸¸ÀÏ °æ¿ì¿¡¸¸ )
void classUSER::LogCHAT( const char * szMSG , const char * pDestCHAR , const char * szMsgTYPE )
{
	return;

	if( !IsTAIWAN() )
		return ;

	if( !szMSG )
		return;
	if( !IsTAIWAN() )
		return;

	char * szACCOUNT = this->Get_ACCOUNT();
	char * szNAME = this->Get_NAME();

	if( szMSG[ 0 ] == '/' )
		szMsgTYPE = "CHEAT";

	if( this->m_dwRIGHT >= RIGHT_TWG ) /// 128 º¸´Ù Å©¸é.
		::g_ChatGMLOG.QueueString( "[%s] %s\t%s\t%s\t%s\t%s\r\n", 
				szMsgTYPE,
				szACCOUNT ? szACCOUNT : "",
				szNAME ? szNAME : "",
				pDestCHAR ? pDestCHAR : "",
				this->Get_IP(), 
				szMSG  );
	else
		::g_ChatLOG.QueueString( "[%s] %s\t%s\t%s\t%s\t%s\r\n", 
				szMsgTYPE,
				szACCOUNT ? szACCOUNT : "",
				szNAME ? szNAME : "",
				pDestCHAR ? pDestCHAR : "",
				this->Get_IP(), 
				szMSG );
}
