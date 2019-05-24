
#include "stdAFX.h"
#include "CObjAVT.h"
#include "GS_ThreadZONE.h"
#include "GS_ThreadLOG.h"

//-------------------------------------------------------------------------------------------------

CObjAVT::CObjAVT ()
{
	m_pTargetNODE = new classDLLNODE< CObjAVT* >;
	m_pTargetNODE->DATA = this;
}
CObjAVT::~CObjAVT ()
{
	SAFE_DELETE( m_pTargetNODE );
}

void CObjAVT::Update_SPEED ()
{	
	float fSpeed = this->Cal_RunSPEED ();
	this->m_nRunSPEED = floor_int( fSpeed );

	short nWeaponSpeed;
	if ( this->GetCur_MOVE_MODE() <= MOVE_MODE_RUN ) {
		this->m_fRunAniSPEED = Cal_RunAniSPEED( m_nRunSPEED );

		// fSpeed = Cal_AtkAniSPEED( this->GetPartITEM(BODY_PART_WEAPON_R) );
		// this->m_nAtkAniSPEED = floor_int( fSpeed );

		//nWeaponSpeed = WEAPON_ATTACK_SPEED( GetPartITEM(BODY_PART_WEAPON_R) ) + 5;
		//fSpeed = 1500.f / nWeaponSpeed;
		//this->m_nAtkAniSPEED = floor_int( fSpeed );
		//this->m_nAtkAniSPEED += this->GetPsv_ATKSPEED();	// this->m_iAddValue[ AT_ATK_SPD ] ���ԵǾ� ����

		short nWeaponItemNo = GetPartITEM( BODY_PART_WEAPON_R );
		nWeaponSpeed = WEAPON_ATTACK_SPEED( nWeaponItemNo ) + 5;

		fSpeed = 1500.f / nWeaponSpeed + m_iAddValue[ AT_ATK_SPD ];
		this->m_nPassiveAttackSpeed = this->GetPassiveSkillAttackSpeed( fSpeed, nWeaponItemNo );
		this->m_nAtkAniSPEED = (short)( fSpeed + this->m_nPassiveAttackSpeed );
	} else if( this->GetCur_MOVE_MODE() > MOVE_MODE_RUN && IsTAIWAN() ) { // Ÿ�̿��̰� ž�¸���̸�..
		this->m_fRunAniSPEED = Cal_RunAniSPEED( m_nRunSPEED );

		short nWeaponItemNo = GetPartITEM( BODY_PART_WEAPON_R );
		nWeaponSpeed = WEAPON_ATTACK_SPEED( nWeaponItemNo ) + 5;

		fSpeed = 1500.f / nWeaponSpeed + m_iAddValue[ AT_ATK_SPD ];
		this->m_nPassiveAttackSpeed = this->GetPassiveSkillAttackSpeed( fSpeed, nWeaponItemNo );
		this->m_nAtkAniSPEED = (short)( fSpeed + this->m_nPassiveAttackSpeed );
		this->m_nAtkAniSPEED += m_iAddValue[ AT_ATK_SPD ];
		// ���������� ĳ���� �� �׸��� �ֲ� �Ʒ����� ����
		for (short nI=0; nI<MAX_RIDING_PART; nI++) {
			tagITEM sITEM = this->m_Inventory.m_ItemRIDE[ nI ];
			this->m_nAtkAniSPEED += PAT_ITEM_ATK_SPD( sITEM.GetItemNO() );
		}
	} else {
		m_fRunAniSPEED = ( m_nRunSPEED + 500 ) / 1000.f;

		nWeaponSpeed = PAT_ITEM_ATK_SPD( m_RideITEM[ RIDE_PART_ARMS ].m_nItemNo ) + 5;
		fSpeed = 1500.f / nWeaponSpeed;
		m_nAtkAniSPEED = floor_int( fSpeed );;
		m_nAtkAniSPEED += this->m_iAddValue[ AT_ATK_SPD ];
	}

	Cal_AruaAtkSPD ();
}

//-------------------------------------------------------------------------------------------------
int CObjAVT::Get_AttackRange( short nSkillIDX )
{
	/// ��ų�� ���� �Ÿ��� �ԷµǾ� �ִٸ� ��ų�Ÿ� �ƴϸ� ���� �Ÿ�..
	if ( nSkillIDX && SKILL_DISTANCE( nSkillIDX ) ) 
	{
		return SKILL_DISTANCE( nSkillIDX );
	}

	if ( this->GetCur_MOVE_MODE() <= MOVE_MODE_RUN ) {
		/// ���� �������� ���Ѵ�. ������ ���Ⱑ ������ �� ������ ������ ����.
		if ( Get_R_WEAPON() )
		{
			return (int) ( WEAPON_ATTACK_RANGE( this->Get_R_WEAPON() ) + ( Get_SCALE() * 120 ) );
		}
	} else {
		//if ( this->m_nRideItemIDX[ RIDE_PART_ARMS ] ) {
		if ( this->m_RideITEM[ RIDE_PART_ARMS ].m_nItemNo ) {
			// ������ ���Ⱑ �ִ�.
			//return (int) ( PAT_ITEM_ATK_RANGE( this->m_nRideItemIDX[RIDE_PART_ARMS] ) + ( Get_SCALE() * 120 ) );
			return (int) ( PAT_ITEM_ATK_RANGE( this->m_RideITEM[RIDE_PART_ARMS].m_nItemNo ) + ( Get_SCALE() * 120 ) );
		}
	}

	// Default attack range...
	return (int) ( Def_AttackRange () + ( this->Get_SCALE() * 120 ) );
}


bool CObjAVT::Make_gsv_ADD_OBJECT( classPACKET *pCPacket )
{
	this->Init_ADD_CHAR( pCPacket );

    pCPacket->m_HEADER.m_wType = GSV_AVT_CHAR;
    pCPacket->m_HEADER.m_nSize = sizeof( gsv_AVT_CHAR );

    pCPacket->m_gsv_AVT_CHAR.m_btCharRACE   = (BYTE)this->m_nCharRACE;
	pCPacket->m_gsv_AVT_CHAR.m_nRunSpeed    = this->GetOri_RunSPEED();			// �нú꿡���ؼ��� ����/ ���Ӻ��� ���ܵȰ�.
	pCPacket->m_gsv_AVT_CHAR.m_nPsvAtkSpeed	= this->GetPsv_ATKSPEED ();
	pCPacket->m_gsv_AVT_CHAR.m_btWeightRate = this->m_btWeightRate;
	pCPacket->m_gsv_AVT_CHAR.m_dwSubFLAG    = this->m_IngSTATUS.m_dwSubStatusFLAG;

    ::CopyMemory( pCPacket->m_gsv_AVT_CHAR.m_PartITEM,		this->m_PartITEM,	sizeof(tagPartITEM)*MAX_BODY_PART );

	pCPacket->m_gsv_AVT_CHAR.m_sShotItem[ SHOT_TYPE_ARROW ].m_wShotITEM = this->m_Inventory.m_ItemSHOT[ SHOT_TYPE_ARROW ].GetHEADER();
	pCPacket->m_gsv_AVT_CHAR.m_sShotItem[ SHOT_TYPE_BULLET].m_wShotITEM = this->m_Inventory.m_ItemSHOT[ SHOT_TYPE_BULLET].GetHEADER();
	pCPacket->m_gsv_AVT_CHAR.m_sShotItem[ SHOT_TYPE_THROW ].m_wShotITEM = this->m_Inventory.m_ItemSHOT[ SHOT_TYPE_THROW ].GetHEADER();

	::CopyMemory( pCPacket->m_gsv_AVT_CHAR.m_RidingITEM,	this->m_RideITEM,	sizeof(tagPartITEM)*MAX_RIDING_PART );

	pCPacket->m_gsv_AVT_CHAR.m_nJOB		= this->GetCur_JOB();
	pCPacket->m_gsv_AVT_CHAR.m_btLEVEL	= this->GetCur_LEVEL();

	/*
	if ( this->m_btRideMODE )
		pCPacket->m_tag_ADD_CHAR.m_btMoveMODE = this->m_btRideMODE;
	*/
    pCPacket->AppendString( this->Get_RNAME() );

	this->Add_ADJ_STATUS( pCPacket );

	pCPacket->AppendString( this->Get_NAME() );

	// ���� ���� ���¸� ���� Ÿ��Ʋ...
	if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_STORE_MODE ) ) {
#ifdef	__INC_PLATINUM
		short nStoreSkin = this->m_GrowAbility.GetStoreSKIN( this->GetCurAbsSEC() );
		pCPacket->AppendData( &nStoreSkin, sizeof(short) );
#endif
		pCPacket->AppendString( this->Get_StoreTITLE() );
	} else
	if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_INTRO_CHAT ) ) {
		pCPacket->AppendString( this->Get_StoreTITLE() );
	}

	if ( this->GetClanID() ) {
		pCPacket->AppendData ( &this->m_CLAN, sizeof( tag_CLAN_ID ) );
		pCPacket->AppendString( this->GetClanNAME() );
	}

    return true;
}

//-------------------------------------------------------------------------------------------------
tagMOTION *CObjAVT::Get_MOTION (short nActionIdx)
{
#ifdef	_DEBUG
	if ( nActionIdx >= g_TblAniTYPE.m_nDataCnt ) {
		assert( 0 && "nActionIdx >= g_TblAniTYPE.m_nDataCnt" );
		return NULL;
	}
#endif

	short nFileIDX, nType;
	tagMOTION *pMOTION;

	if ( IsTAIWAN() || !this->m_btRideMODE ) {
		// ������ ���� ����������...
		//nFileIDX = FILE_MOTION( WEAPON_MOTION_TYPE(this->m_nRWeaponIDX), nActionIdx );
		nFileIDX = FILE_MOTION( WEAPON_MOTION_TYPE(this->m_sRWeaponIDX.m_nItemNo), nActionIdx );
		nType = this->IsFemale();
	} else {
		nFileIDX = FILE_MOTION( this->m_nPatAniCOL, nActionIdx );
		nType = 0;
	}

	if ( 0 == nFileIDX ) {
		// ����� ������ �Ǽ� ������� ��ü ??? :: 0�� ���� ���µ� ??..
		// nFileIDX = FILE_MOTION( 0, nActionIdx );
		return NULL;
	}

	// TODO:: ���� ���ۿ� ���ؼ��� ��/�� ��� �������� ����.
	// ������ Ʋ���� ???
	pMOTION = g_MotionFILE.IDX_GetMOTION( nFileIDX, nType );
	if ( pMOTION ) {
		pMOTION->m_nActionIdx = nActionIdx;
	}
	return pMOTION;
}

//-------------------------------------------------------------------------------------------------
static short s_nEquipIdxToPartNO [] = {
	-1,						// 0
	BODY_PART_FACE_ITEM,	// EQUIP_IDX_FACE_ITEM = 1,
	BODY_PART_HELMET,		// EQUIP_IDX_HELMET,
	BODY_PART_ARMOR,		// EQUIP_IDX_ARMOR,
	BODY_PART_KNAPSACK,		// EQUIP_IDX_KNAPSACK,
	BODY_PART_GAUNTLET,		// EQUIP_IDX_GAUNTLET,
	BODY_PART_BOOTS,		// EQUIP_IDX_BOOTS,	// 5
	BODY_PART_WEAPON_R,		// EQUIP_IDX_WEAPON_R,
	BODY_PART_WEAPON_L,		// EQUIP_IDX_WEAPON_L,	
	-1,						// EQUIP_IDX_NECKLACE,
	-1,						// EQUIP_IDX_RING,
} ;
void CObjAVT::SetPartITEM (short nEquipInvIDX)
{
	COMPILE_TIME_ASSERT( EQUIP_IDX_FACE_ITEM	== 1 );
	COMPILE_TIME_ASSERT( EQUIP_IDX_HELMET		== 2 );
	COMPILE_TIME_ASSERT( EQUIP_IDX_ARMOR		== 3 );
	COMPILE_TIME_ASSERT( EQUIP_IDX_KNAPSACK		== 4 );
	COMPILE_TIME_ASSERT( EQUIP_IDX_GAUNTLET		== 5 );
	COMPILE_TIME_ASSERT( EQUIP_IDX_BOOTS		== 6 );
	COMPILE_TIME_ASSERT( EQUIP_IDX_WEAPON_R		== 7 );
	COMPILE_TIME_ASSERT( EQUIP_IDX_WEAPON_L		== 8 );
	COMPILE_TIME_ASSERT( EQUIP_IDX_NECKLACE		== 9 );
	COMPILE_TIME_ASSERT( EQUIP_IDX_RING			== 10 );

	short nPartNO = s_nEquipIdxToPartNO[ nEquipInvIDX ];
	if ( nPartNO > 0 ) {
		this->SetPartITEM( nPartNO, m_Inventory.m_ItemLIST[ nEquipInvIDX ] );
	}

	if ( this->GetCur_MOVE_MODE() <= MOVE_MODE_RUN )
		this->UpdateAbility ();		// ��Ʈ ������ ����
}
void CObjAVT::SetRideITEM (short nRideInvIDX)
{
//	m_nRideItemIDX[ nRideInvIDX ] = this->m_Inventory.m_ItemRIDE[ nRideInvIDX ].GetItemNO();

	m_RideITEM[ nRideInvIDX ].m_nItemNo	  = this->m_Inventory.m_ItemRIDE[ nRideInvIDX ].GetItemNO();
	m_RideITEM[ nRideInvIDX ].m_nGEM_OP	  = this->m_Inventory.m_ItemRIDE[ nRideInvIDX ].GetGemNO();
	m_RideITEM[ nRideInvIDX ].m_cGrade	  = this->m_Inventory.m_ItemRIDE[ nRideInvIDX ].GetGrade();
	m_RideITEM[ nRideInvIDX ].m_bHasSocket= this->m_Inventory.m_ItemRIDE[ nRideInvIDX ].m_bHasSocket;

	if ( this->GetCur_MOVE_MODE() == MOVE_MODE_DRIVE )
		this->UpdateAbility ();		// ž�� ������ ����
}

//-------------------------------------------------------------------------------------------------
void CObjAVT::Set_TargetIDX (int iTargetIndex, bool bCheckHP)
{
	if ( Get_TargetIDX() == iTargetIndex )
		return;

	CObjCHAR *pTarget;
	if ( Get_TargetIDX() ) {
		pTarget = (CObjCHAR*) this->Get_TargetOBJ (true);	// The CObjCHAR that inherits the HP checked CAI_OBJ is passed.
		if ( pTarget ) {
			pTarget->Sub_FromTargetLIST( this );
		}
	}

	this->CObjTARGET::Set_TargetIDX( iTargetIndex );

	if ( Get_TargetIDX() ) {
		pTarget = (CObjCHAR*) this->Get_TargetOBJ (bCheckHP);		// The CObjCHAR that inherits the HP checked CAI_OBJ is passed.
		if ( pTarget ) {
			pTarget->Add_ToTargetLIST( this );
			this->Send_gsv_HP_REPLY( Get_TargetIDX(), pTarget->Get_HP() );
		} else {
			// ĳ���Ͱ� �ƴϰų� �׾���...
			CObjTARGET::Set_TargetIDX( 0 );
		}
	}
}


//-------------------------------------------------------------------------------------------------
bool CObjAVT::SetCMD_TOGGLE (BYTE btTYPE, bool bForce)
{
#ifdef	__BLOCK_WHEN_SKILL
	// 2005.08.03 : icarus :: ���� �˻����� ���� ��� ��ų�� �ֵ��� ����...
	if ( !bForce && Get_ActiveSKILL() ) {
		return true;
	}
#endif

	switch( btTYPE ) {
		case TOGGLE_TYPE_RUN :
		{
			if ( this->m_btRideMODE ) {
				// ž���ϰ� ??
				return true;
			}
			if ( this->m_btWeightRate >= WEIGHT_RATE_WALK ) {
				// �۷��� ?
				return true;
			}

			this->m_bRunMODE = !this->m_bRunMODE;

			if ( Get_STATE() == CS_MOVE ) {
				this->m_fCurMoveSpeed = this->Get_MoveSPEED ();
				this->Reset_MoveVEC ();
			}
			btTYPE = TOGGLE_TYPE_DRIVE + this->Get_MoveMODE ();
			break;
		}

		case TOGGLE_TYPE_SIT :
		{
			if ( this->m_btRideMODE ) {
				// ž���ϰ� ??
				return true;
			}

			switch( this->Get_COMMAND() ) {
				case CMD_STOP :
					CObjAI::SetCMD_SIT ();
					break;

				case CMD_SIT :
					CObjAI::SetCMD_STAND ();
					this->Send_gsv_SET_HPnMP( 0x03 );	// both hp and mp : NB: 3 in binary is 11 and is compatible with 1 (01) and 2 (10) so 3 is doing both hp and mp!
					break;

				default :
					// If you can not sit ...
					return true;
			}
			break;
		}

		case TOGGLE_TYPE_DRIVE :
		{	
			if ( this->m_btRideMODE ) {
				// ����..
				if ( this->m_iLinkedCartObjIDX ) {
					classUSER *pUSER = g_pObjMGR->Get_UserOBJ( this->m_iLinkedCartObjIDX );
					if ( RIDE_MODE_GUEST == pUSER->GetCur_RIDE_MODE() ) {
						// �¿��� �մԵ� ������...
						pUSER->m_btRideMODE = 0;
						pUSER->m_btRideATTR = RIDE_ATTR_NORMAL;
					}
					pUSER->m_iLinkedCartObjIDX = 0;
				}

				this->m_btRideMODE = 0;
				this->m_btRideATTR = RIDE_ATTR_NORMAL;
				this->m_iLinkedCartObjIDX = 0;

				if ( IsTAIWAN() ) {
					// 2005.08.03 : icarus :: ������ ��� ���� ���� ����...
					this->m_IngSTATUS.ClearAllGOOD ();
				}
			} else {
				// ����, ���� üũ, 
				for (short nI=0; nI<MAX_RIDING_PART-2; nI++) {
					if ( ITEM_TYPE_RIDE_PART != this->m_Inventory.m_ItemRIDE[ nI ].GetTYPE() ) {
						return true;
					}
				}

				if ( IsTAIWAN() ) {
					if ( ZONE_RIDING_REFUSE_FLAG( this->GetZONE()->Get_ZoneNO() ) ) {
						// 0x01 : īƮ �Ұ�, 0x02 : ĳ����� �Ұ�
						//TUNING_PART_BODY_CART				= 511,	// Ʃ�׺�ǰ - īƮ �ٵ�
						//TUNING_PART_BODY_CASTLEGEAR		= 512,	// Ʃ�׺�ǰ - �ɽ���� �ٵ�
						int iType = ITEM_TYPE( ITEM_TYPE_RIDE_PART, this->m_Inventory.m_ItemRIDE[ RIDE_PART_BODY ].GetItemNO() );
						if ( (iType%3) & ZONE_RIDING_REFUSE_FLAG( this->GetZONE()->Get_ZoneNO() ) ) {
							// ž�� ����~
							return true;
						}
					}
				}

#ifdef	__KCHS_BATTLECART__
				if( this->GetCur_PatHP() <= 0 ) // īƮü���� 0�̸� īƮ�� ��ȯ�� �� ����
					return true;
#endif

				this->Dec_EngineLife ();	// ������ ���� ����
				this->m_btRideMODE = RIDE_MODE_DRIVE;

				if ( PET_TYPE_CASTLE_GEAR01 == PAT_ITEM_PART_TYPE( this->m_Inventory.m_ItemRIDE[ RIDE_PART_BODY ].GetItemNO() ) )
					this->m_btRideATTR = RIDE_ATTR_CASTLE;
#ifdef FRAROSE
				else if( PET_TYPE_MOUNT01 >= PAT_ITEM_PART_TYPE( this->m_Inventory.m_ItemRIDE[ RIDE_PART_BODY ].GetItemNO() ) && PET_TYPE_MAX <= PAT_ITEM_PART_TYPE( this->m_Inventory.m_ItemRIDE[ RIDE_PART_BODY ].GetItemNO() ) )
				{
					switch (PAT_ITEM_PART_TYPE( this->m_Inventory.m_ItemRIDE[ RIDE_PART_BODY ].GetItemNO() ))
					{
						case PET_TYPE_MOUNT01:
							this->m_btRideATTR = RIDE_ATTR_MOUNT01;
							break;
						case PET_TYPE_MOUNT02:
							this->m_btRideATTR = RIDE_ATTR_MOUNT02;
							break;
						case PET_TYPE_MOUNT03:
							this->m_btRideATTR = RIDE_ATTR_MOUNT03;
							break;
						case PET_TYPE_MOUNT04:
							this->m_btRideATTR = RIDE_ATTR_MOUNT04;
							break;
						case PET_TYPE_MOUNT05:
							this->m_btRideATTR = RIDE_ATTR_MOUNT05;
							break;
						case PET_TYPE_MOUNT06:
							this->m_btRideATTR = RIDE_ATTR_MOUNT06;
							break;
						case PET_TYPE_MOUNT07:
							this->m_btRideATTR = RIDE_ATTR_MOUNT07;
							break;
						case PET_TYPE_MOUNT08:
							this->m_btRideATTR = RIDE_ATTR_MOUNT08;
							break;
						case PET_TYPE_MOUNT09:
							this->m_btRideATTR = RIDE_ATTR_MOUNT09;
							break;
						case PET_TYPE_MOUNT10:
							this->m_btRideATTR = RIDE_ATTR_MOUNT10;
							break;
						case PET_TYPE_MOUNT11:
							this->m_btRideATTR = RIDE_ATTR_MOUNT11;
							break;
						case PET_TYPE_MOUNT12:
							this->m_btRideATTR = RIDE_ATTR_MOUNT12;
							break;
						case PET_TYPE_MOUNT13:
							this->m_btRideATTR = RIDE_ATTR_MOUNT13;
							break;
						case PET_TYPE_MOUNT14:
							this->m_btRideATTR = RIDE_ATTR_MOUNT14;
							break;
						case PET_TYPE_MOUNT15:
							this->m_btRideATTR = RIDE_ATTR_MOUNT15;
							break;
						case PET_TYPE_MOUNT16:
							this->m_btRideATTR = RIDE_ATTR_MOUNT16;
							break;
						case PET_TYPE_MOUNT17:
							this->m_btRideATTR = RIDE_ATTR_MOUNT17;
							break;
						case PET_TYPE_MOUNT18:
							this->m_btRideATTR = RIDE_ATTR_MOUNT18;
							break;
						case PET_TYPE_MOUNT19:
							this->m_btRideATTR = RIDE_ATTR_MOUNT19;
							break;
						case PET_TYPE_MOUNT20:
							this->m_btRideATTR = RIDE_ATTR_MOUNT20;
							break;
					}
				}
#endif
				else
					this->m_btRideATTR = RIDE_ATTR_CART;

				// ž�½� ��� ���� ���� ����...
				this->m_IngSTATUS.ClearAllGOOD ();
			}
			this->m_dwRecoverTIME = 0;

			this->UpdateAbility ();		// ž�� ���...
			btTYPE = TOGGLE_TYPE_DRIVE + this->Get_MoveMODE ();
			return this->Send_gsv_TOGGLE( btTYPE, true );
		}
	}

	// except TOGGLE_TYPE_DRIVE
	this->Send_gsv_TOGGLE( btTYPE, false );

	return true;
}

//-------------------------------------------------------------------------------------------------
void CObjAVT::SetCur_SEX( char cValue )
{
	this->m_nCharRACE &= 0x0fffe;			// ������Ʈ ������~
	this->m_nCharRACE |= ( cValue & 0x01 );	// ������Ʈ �����ϰ�~
}

int	 CObjAVT::GetCur_FUEL ()
{
	tagITEM *pEngine = &m_Inventory.m_ItemRIDE[ RIDE_PART_ENGINE ];
	if ( ITEM_TYPE_RIDE_PART != pEngine->GetTYPE() )
		return 0;
	return pEngine->m_nLife;
}
void CObjAVT::SubCur_FUEL (short nValue)
{
	tagITEM *pEngine = &m_Inventory.m_ItemRIDE[ RIDE_PART_ENGINE ];
	if ( ITEM_TYPE_RIDE_PART != pEngine->GetTYPE() )
		return;

	if ( pEngine->m_nLife > nValue )
		pEngine->m_nLife -= nValue;
	else {
		// ���� 0�ƴ�...
		pEngine->m_nLife = 0;
		this->Send_gsv_SET_ITEM_LIFE( INVENTORY_RIDE_ITEM0+RIDE_PART_ENGINE, 0 );
		// this->SetCMD_STOP ();
	}
}

int CObjAVT::Get_AbilityValue (WORD wType)
{
	switch( wType ) {
		// case AT_BIRTHSTONE :	break;
		case AT_SEX		:				return ( GetCur_RACE() & 0x01 );
		case AT_RACE	:				return ( GetCur_RACE() / 0x02 );
		case AT_CLASS	:				return GetCur_JOB ();
		case AT_UNION	:				return GetCur_JOHAP ();
		case AT_RANK	:				return GetCur_RANK ();
		case AT_FAME	:				return GetCur_FAME ();

		case AT_FACE	:				return GetCur_FACE ();
		case AT_HAIR	:				return GetCur_HAIR ();

		case AT_STR		:				return GetCur_STR ();
		case AT_DEX		:				return GetCur_DEX ();
		case AT_INT		:				return GetCur_INT ();
		case AT_CON		:				return GetCur_CON ();
		case AT_CHARM	:				return GetCur_CHARM ();
		case AT_SENSE	:				return GetCur_SENSE ();
		case AT_HP		:				return GetCur_HP (); 
		case AT_MP		:				return GetCur_MP ();
		case AT_ATK		:				return GetCur_ATK ();
		case AT_DEF		:				return GetCur_DEF ();
		case AT_HIT		:				return GetCur_HIT ();
		case AT_RES		:				return GetCur_RES ();
		case AT_AVOID	:				return GetCur_AVOID ();
		case AT_SPEED	:				return (int)GetCur_MOVE_SPEED ();
		case AT_ATK_SPD :				return GetCur_ATK_SPD ();
		case AT_WEIGHT	:				return GetCur_WEIGHT ();
		case AT_CRITICAL   :			return Get_CRITICAL ();

		case AT_SAVE_MP	:				return GetCur_SaveMP();
		case AT_DROP_RATE :				return GetCur_DropRATE ();

		case AT_FAME_G	:				return GetCur_FameG ();
		case AT_FAME_B	:				return GetCur_FameB ();

		case AT_STAMINA :				return GetCur_STAMINA ();

		case AT_GUILD_NO	:			return GetClanID ();
		case AT_GUILD_POS	:			return GetClanPOS ();
		case AT_GUILD_SCORE	:			return GetClanSCORE ();

		// case AT_RECOVER_HP :	
		// case AT_RECOVER_MP :	break;

		case AT_EXP			:			return GetCur_EXP ();
		case AT_LEVEL		:			return GetCur_LEVEL ();
		case AT_BONUSPOINT	:			return GetCur_BonusPOINT ();
		case AT_SKILLPOINT	:			return GetCur_SkillPOINT ();

		case AT_PK_FLAG		:			return GetCur_PK_FLAG();
		case AT_TEAM_NO		:			return Get_TeamNO();

		case AT_HEAD_SIZE :				return GetCur_HeadSIZE ();
		case AT_BODY_SIZE :				return GetCur_BodySIZE ();

		case AT_MONEY	:	
			if ( GetCur_MONEY() > MAX_INT )
				return MAX_INT;
			return (int)GetCur_MONEY();

		case AT_MAX_HP :				return GetCur_MaxHP ();
		case AT_MAX_MP :				return GetCur_MaxMP ();

		case AT_CURRENT_PLANET :
			if ( this->GetZONE() )
				return ZONE_PLANET_NO( this->GetZONE()->Get_ZoneNO() );
			return -1;

		default :
			if ( wType >= AT_UNION_POINT1 && wType <= AT_UNION_POINT10 )
				return GetCur_JoHapPOINT( wType-AT_UNION_POINT1 + 1 );

	}

	return 0;
}


//-------------------------------------------------------------------------------------------------
void CObjAVT::Stamp_AttackTIME ()
{
	this->m_dwAttackTIME = this->GetZONE()->GetTimeGetTIME();
}

int	CObjAVT::Save_Damage( int iAttackerIDX, int iDamage)
{
	this->Stamp_AttackTIME ();

	// �� ���� ����
	int iRand = 1 + RANDOM(400);
	if ( iRand >= 101 )
		return 0;

	tagITEM *pItem;
	short nInvIDX;

	if ( this->Get_MoveMODE() == MOVE_MODE_DRIVE ) {
		if ( iRand >= 51 )	nInvIDX =  INVENTORY_RIDE_ITEM0 + RIDE_PART_BODY;	// �ٵ�
		else
		if ( iRand >= 21 )	nInvIDX =  INVENTORY_RIDE_ITEM0 + RIDE_PART_LEG;	// ����
		else
		// if ( iRand >= 16 )	
		{
			nInvIDX =  INVENTORY_RIDE_ITEM0 + RIDE_PART_ARMS;	// ����
		}
		// else				nInvIDX =  INVENTORY_RIDE_ITEM0 + RIDE_PART_ENGINE;	// ����
	} else {
		pItem = &m_Inventory.m_ItemLIST[ EQUIP_IDX_WEAPON_L ];
		if ( pItem->GetLife() && WEAPON_ITEM_SHIELD == ITEM_TYPE( pItem->GetTYPE(), pItem->GetItemNO() ) ) {
			// ���� ����
			if ( iRand >  60 )	nInvIDX = EQUIP_IDX_WEAPON_L;	// ����, �޼չ���
			else				
			if ( iRand >  30 )	nInvIDX = EQUIP_IDX_ARMOR;		// ����
			else
			if ( iRand >  22 )	nInvIDX = EQUIP_IDX_BOOTS;		// �Ź�
			else
			if ( iRand >  16 )	nInvIDX = EQUIP_IDX_HELMET;		// �Ӹ�
			else
			if ( iRand >  9 )	nInvIDX = EQUIP_IDX_KNAPSACK;	// ��
			else
			if ( iRand >  5 )	nInvIDX = EQUIP_IDX_GAUNTLET;	// �尩
			else				nInvIDX = EQUIP_IDX_FACE_ITEM;	// ��(�Ǽ��縮)
		} else {
			// ���� ����
			if ( iRand > 64 )	nInvIDX = EQUIP_IDX_ARMOR;		// ����
			else
			if ( iRand > 46 )	nInvIDX = EQUIP_IDX_BOOTS;		// �Ź�
			else				
			if ( iRand > 31 )	nInvIDX = EQUIP_IDX_GAUNTLET;	// �尩
			else				
			if ( iRand > 15 )	nInvIDX = EQUIP_IDX_KNAPSACK;	// ��
			else 
			if ( iRand > 7 )	nInvIDX = EQUIP_IDX_HELMET;		// �Ӹ�
			else				nInvIDX = EQUIP_IDX_FACE_ITEM;	// ��(�Ǽ��縮)
		}
	}

	pItem = &m_Inventory.m_ItemLIST[ nInvIDX ];
	if ( 0 == pItem->GetHEADER() || pItem->GetLife() <= 0 )
		return 0;

	iRand = 1+RANDOM(120) - ( pItem->GetDurability() + 10 - (int)( iDamage*0.1f ) );
	if ( iRand >= 0 ) {
		// ������ 1 ����, 10���� ��ȭ�� ������ Ŭ���̾�Ʈ�� ����
		if ( pItem->GetLife() > 1 ) {
			pItem->m_nLife --;
			if ( pItem->GetLife() % 10 == 0 ) {
				// ����� ���� ����...
				this->Send_gsv_SET_ITEM_LIFE( nInvIDX, pItem->GetLife () );
			}
		} else {
			pItem->m_nLife = 0;
			this->Send_gsv_SET_ITEM_LIFE( nInvIDX, 0 );

			short nCurSpeed = this->GetOri_RunSPEED();
			this->UpdateAbility ();		// �� ���� = 0
			if ( nCurSpeed != this->GetOri_RunSPEED() )
				this->Send_gsv_SPEED_CHANGED( false );
		}
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------
void CObjAVT::Dec_EngineLife()
{
	tagITEM *pEngine = &m_Inventory.m_ItemRIDE[ RIDE_PART_ENGINE ];
	if ( ITEM_TYPE_RIDE_PART != pEngine->GetTYPE() )
		return;

	int iUseValue = PAT_ITEM_USE_FUEL_RATE( pEngine->GetItemNO() );
	if ( pEngine->m_nLife > iUseValue ) {
		int iDiv10 = pEngine->m_nLife / 10;
		pEngine->m_nLife -= iUseValue;
		if ( iDiv10 != (pEngine->m_nLife / 10) )
			this->Send_gsv_SET_ITEM_LIFE( INVENTORY_RIDE_ITEM0+RIDE_PART_ENGINE, pEngine->GetLife () );
	} else {
		// ���� 0�ƴ�...
		pEngine->m_nLife = 0;
		this->Send_gsv_SET_ITEM_LIFE( INVENTORY_RIDE_ITEM0+RIDE_PART_ENGINE, 0 );
		this->SetCMD_STOP ();
	}
}

//-------------------------------------------------------------------------------------------------
void CObjAVT::Dec_WeaponLife()
{
	tagITEM *pWeapon;
	short nInvIDX;

	if ( this->GetCur_MOVE_MODE() == MOVE_MODE_DRIVE ) {
		this->Dec_EngineLife ();		// ���ݽ� ���� ���� ���� ����...

		// PAT
		nInvIDX = INVENTORY_RIDE_ITEM0 + RIDE_PART_ARMS;
	} else {
		nInvIDX = EQUIP_IDX_WEAPON_R;
	}

	pWeapon = &m_Inventory.m_ItemLIST[ nInvIDX ];
	if ( 0 == pWeapon->GetHEADER() || pWeapon->GetLife() <= 0 )
		return;
		
	if ( RANDOM( 710 ) + 1 - ( pWeapon->GetDurability()+600 ) >= 0 ) {
		// ���� 1 ����
		if ( pWeapon->GetLife() > 1 ) {
			pWeapon->m_nLife --;
			if ( pWeapon->GetLife() % 10 == 0 ) {
				// ����� ���� ����...
				this->Send_gsv_SET_ITEM_LIFE( nInvIDX, pWeapon->GetLife () );
			}
		} else {
			pWeapon->m_nLife = 0;
			this->Send_gsv_SET_ITEM_LIFE( nInvIDX, 0 );

			short nCurSpeed = this->GetOri_RunSPEED();
			this->UpdateAbility ();			// ���� ���� = 0
			if ( nCurSpeed != this->GetOri_RunSPEED() )
				this->Send_gsv_SPEED_CHANGED( false );
		}
	}
}


//-------------------------------------------------------------------------------------------------
bool CObjAVT::Send_gsv_GODDNESS_MODE( BYTE btOnOff )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType	= GSV_GODDNESS_MODE;
	pCPacket->m_HEADER.m_nSize	= sizeof( gsv_GODDNESS_MODE);
	pCPacket->m_gsv_GODDNESS_MODE.m_btOnOff = btOnOff;
	pCPacket->m_gsv_GODDNESS_MODE.m_wObjectIDX = this->Get_INDEX();

	this->GetZONE()->SendPacketToSectors( this, pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	return true;
}

//-------------------------------------------------------------------------------------------------
//#define	ARUA_RATE_ATK	0.1f
//#define	ARUA_RATE_HIT	0.1f
//#define	ARUA_RATE_AVD	0.1f

#define	ARUA_RATE_ATK_SPD	0.2f
#define	ARUA_RATE_RUN_SPD	0.2f
#define	ARUA_RATE_CRI		0.2f
#define	ARUA_RATE_MAX_HP	0.2f
#define	ARUA_RATE_MAX_MP	0.2f
#define	ARUA_RATE_RCV_HP	0.5f
#define	ARUA_RATE_RCV_MP	0.5f
#define	ARUA_RATE_ATK		0.2f
#define	ARUA_RATE_RES		0.2f


void CObjAVT::Cal_AruaAtkSPD (void)
{
	//if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) {
	//	this->m_IngSTATUS.m_nAruaAtkSPD = (short)( this->GetOri_ATKSPEED() * ARUA_RATE_ATK_SPD );
	//} else
	//	this->m_IngSTATUS.m_nAruaAtkSPD = 0;
}
void CObjAVT::Cal_AruaRunSPD (void)
{
	if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) {
		this->m_IngSTATUS.m_nAruaRunSPD = (short)( this->GetOri_RunSPEED() * ARUA_RATE_RUN_SPD ); // 0.2f
	} else
		this->m_IngSTATUS.m_nAruaRunSPD = 0;
}

void CObjAVT::Cal_AruaMaxHP (void)
{
	if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) {
		this->m_IngSTATUS.m_nAruaMaxHP = (short)( this->GetOri_MaxHP() * ARUA_RATE_MAX_HP ); // 0.2f
	} else
		this->m_IngSTATUS.m_nAruaMaxHP = 0;
}
void CObjAVT::Cal_AruaMaxMP (void)
{
	if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) {
		this->m_IngSTATUS.m_nAruaMaxMP = (short)( this->GetOri_MaxMP() * ARUA_RATE_MAX_MP ); // 0.2f
	} else
		this->m_IngSTATUS.m_nAruaMaxMP = 0;
}


void CObjAVT::Cal_AruaATTACK (void)
{
	if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) {
		this->m_IngSTATUS.m_nAruaATK = (short)( this->GetOri_ATK() * ARUA_RATE_ATK );
	} else
		this->m_IngSTATUS.m_nAruaATK = 0;
}
void CObjAVT::Cal_AruaHIT (void)
{
	//if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) {
	//	this->m_IngSTATUS.m_nAruaHIT = (short)( GetOri_HIT() * ARUA_RATE_HIT );
	//} else
	//	this->m_IngSTATUS.m_nAruaHIT = 0;
}
void CObjAVT::Cal_AruaAVOID (void)
{
	//if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) {
	//	this->m_IngSTATUS.m_nAruaAVOID = (short)( GetOri_AVOID() * ARUA_RATE_AVD );
	//} else
	//	this->m_IngSTATUS.m_nAruaAVOID = 0;
}
void CObjAVT::Cal_AruaCRITICAL (void)
{
	if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) {
		this->m_IngSTATUS.m_nAruaCRITICAL = (short)( GetOri_CRITICAL() * ARUA_RATE_CRI );
	} else
		this->m_IngSTATUS.m_nAruaCRITICAL = 0;
}

void CObjAVT::Cal_AruaRES( void )
{
	if ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) {
		this->m_IngSTATUS.m_nAruaRES = (short)( this->GetOri_DEF() * ARUA_RATE_RES );
	} else
		this->m_IngSTATUS.m_nAruaRES = 0;
}

bool CObjAVT::Add_Goddess (void)
{
	this->m_IngSTATUS.SetSubFLAG( FLAG_SUB_ARUA_FAIRY );

	Cal_AruaMaxHP ();
	Cal_AruaMaxMP ();
	
	Cal_AruaRunSPD ();
	Cal_AruaAtkSPD ();

	Cal_AruaATTACK ();
	Cal_AruaHIT ();
	Cal_AruaAVOID ();

	Cal_AruaCRITICAL ();
	Cal_AruaRES();

	return this->Send_gsv_GODDNESS_MODE( 1 );
}
bool CObjAVT::Del_Goddess (void)
{
	DWORD dwIsSet = this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY );

	this->m_IngSTATUS.DelArua ();
	if ( this->GetCur_HP() > this->Get_MaxHP () )
		this->SetCur_HP( Get_MaxHP() );
	if ( this->GetCur_MP() > this->Get_MaxMP () )
		this->SetCur_MP( Get_MaxMP() );

	return dwIsSet ? this->Send_gsv_GODDNESS_MODE( 0 ) : true;
}


//-------------------------------------------------------------------------------------------------
#define	CHECK_GODDNESS_CALL_TIME			( 60 * 60 * 1000 ) 

#define	CHECK_PAT_COOL_TIME					( 1000 )
#define	PAT_DECREASE_COOLTIME_PER_SEC		(1 * 1000)		// 1�ʽ� ���ҽ��Ѽ�..

void CObjAVT::Check_PerFRAME (DWORD dwPassTIME)
{
	m_dwGoddnessTIME += dwPassTIME;
	m_dwRecoverTIME  += dwPassTIME;

	/*if ( IsTAIWAN() ) {
		if ( m_dwGoddnessTIME >= CHECK_GODDNESS_CALL_TIME ) {
			m_dwGoddnessTIME -= CHECK_GODDNESS_CALL_TIME;
			// ���� ��ȯ ����
			this->Del_Goddess ();
			if ( this->GetCur_LEVEL() >= 50 ) {
				if ( RANDOM(100)+1 <= 3 ) {
					this->Add_Goddess ();	// ���� ��ȯ
				}
			} else 
			if ( RANDOM(100)+1 <= 5 ) {
				this->Add_Goddess ();		// ���� ��ȯ
			}
		}
	}*/

	switch( this->Get_MoveMODE() ) {
		case MOVE_MODE_RIDEON	:
			break;
		case MOVE_MODE_DRIVE	:			
			if ( m_dwRecoverTIME >= USE_FUEL_CHEC_TIME ) {
				m_dwRecoverTIME -= USE_FUEL_CHEC_TIME;
				if( IsTAIWAN() )
				{
					if( !Get_STATE() && (Get_STATE() & CS_MOVE) ) // 
						this->Dec_EngineLife ();	// ž�� ���, ���� �Ҹ�	
				}
				else
					this->Dec_EngineLife ();	// ž�� ���, ���� �Ҹ�	
			}
			break;
		default :							// HP / MP ȸ��
		{
			DWORD dwCheckTime = IsTAIWAN() ? RECOVER_STATE_CHECK_TIME : 4000;
			if ( m_dwRecoverTIME >=	dwCheckTime ) {
				m_dwRecoverTIME -= dwCheckTime;

				// if ( this->GetCur_STAMINA() >= YELLOW_STAMINA ) 
				{
					// ���׹̳� üũ...
					int iAdd;
					if ( IsTAIWAN() ) {
						int iArua;
						if ( CMD_SIT == Get_COMMAND() ) {
							//[�ɱ� ����]
							// RECOVER_HP= (MAX_HP)/12+1+ ITEM_RECOVER_HP (1���� ����) �븸 2005.6.16 kchs
							iAdd = (int)( this->GetOri_MaxHP()/12.f + 1 + this->GetAdd_RecoverHP() );
							iArua = ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) ? iAdd >> 1 : 0;
							this->Add_HP( iAdd+iArua );
#ifdef	__INC_WORLD
							g_LOG.CS_ODS( 0xffff, "ADD HP:: %d = %d + %d \n", iAdd+iArua, iAdd, iArua );
#endif
							// RECOVER_MP= (MAX_MP)/12+1+ ITEM_RECOVER_MP (1���� ����) �븸 2005.6.19 kchs
							iAdd = (int)( this->GetOri_MaxMP()/12.f + 1 + this->GetAdd_RecoverMP() );
							iArua = ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) ? iAdd >> 1 : 0;
							this->Add_MP( iAdd+iArua );
//#ifdef	__INC_WORLD
//							g_LOG.CS_ODS( 0xffff, "ADD MP:: %d = %d + %d \n", iAdd+iArua, iAdd, iArua );
//#endif
						} else {
							//[�Ϲ� ����]
							// RECOVER_HP= (MAX_HP)/50+1+ ITEM_RECOVER_HP (1���� ����) (�븸 2005.06.19) kchs
							iAdd = (int)( this->GetOri_MaxHP()/50.f + 1 + this->GetAdd_RecoverHP() );
							iArua = ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) ? iAdd >> 1 : 0;
							this->Add_HP( iAdd+iArua );
//#ifdef	__INC_WORLD
//							g_LOG.CS_ODS( 0xffff, ">> HP:: %d \n", Get_HP() );
//							g_LOG.CS_ODS( 0xffff, "ADD HP:: %d = %d + %d \n", iAdd+iArua, iAdd, iArua );
//#endif

							// RECOVER_MP= (MAX_MP)/35+1+ ITEM_RECOVER_MP (1���� ����) �븸 2005.6.19 kchs
							iAdd = (int)( this->GetOri_MaxMP()/35.f + 1 + this->GetAdd_RecoverMP() );
							iArua = ( this->m_IngSTATUS.IsSubSET( FLAG_SUB_ARUA_FAIRY ) ) ? iAdd >> 1 : 0;
							this->Add_MP( iAdd+iArua );
//#ifdef	__INC_WORLD
//							g_LOG.CS_ODS( 0xffff, "ADD MP:: %d = %d + %d \n", iAdd+iArua, iAdd, iArua );
//#endif
						}
					} else {
						if ( CMD_SIT == Get_COMMAND() ) {
							iAdd = this->Get_RecoverHP( RECOVER_STATE_SIT_ON_GROUND );
							this->Add_HP( iAdd );

							iAdd = this->Get_RecoverMP( RECOVER_STATE_SIT_ON_GROUND );
							this->Add_MP( iAdd );
			#ifdef	__INC_WORLD
							LogString(0xffff, "Recover:: %d / %d :: %d\n", m_dwRecoverTIME, dwPassTIME, (this->GetZONE())->GetCurrentTIME() );
			#endif
						} else {
							iAdd = (int)( ( this->GetAdd_RecoverHP() + ( this->Get_CON()+40 ) / 6.f) / 6.f);
							this->Add_HP( iAdd );
						}
					}
				}
			}
		}
	}

#ifdef __KCHS_BATTLECART__
	if( GetCur_PatHP() <= 0 && GetCur_PatCoolTIME() <= 0 ) // ���� �����̰� PAT HP�� 0�̰� ��Ÿ���� �۵����� �ƴϸ�.. ��Ÿ�� ����
	{
		if( 3 == Get_PatHP_MODE() )
		{
			SetCur_PatCoolTIME( PAT_COOLTIME );
			Send_gsv_PATSTATE_CHAGE( 0, PAT_COOLTIME ); 
		}
		else if( Get_PatHP_MODE() == 0 && GetCur_PatMaxHP() > 0)
		{
			Set_PatHP_MODE( 2 );
		}
	}

	m_dwPatTIME += dwPassTIME;
	if( m_dwPatTIME > CHECK_PAT_COOL_TIME )
	{
		m_dwPatTIME = 0; // m_dwPatTIME�� �ʱ�ȭ�� �� �Ǳ� ������ 0���� ����� �Ѵ�.. �ƴϸ�.. ��û���� ū������ ���.
		if( GetCur_PatHP() <= 0 && GetCur_PatCoolTIME() > 0 )	// ���� īƮ ü���� 0�̸� ��Ÿ�� ���� �ִ� ���̴�.
		{
			#pragma message( "====> ���� �������� īƮ ü�� �˻��� �� if( GetCur_PatMaxHP() > 0 ) ��Ÿ�� �� ����.. " )
			if( SubCur_PatCoolTIME( PAT_DECREASE_COOLTIME_PER_SEC ) <= 0)
			{
				SetCur_PatHP( GetCur_PatMaxHP() );
				Send_gsv_PATSTATE_CHAGE( 1, 0 );
			}
		}
	}
#endif

}


//-------------------------------------------------------------------------------------------------
void CObjAVT::Resurrection (short nSkillIDX)
{
	this->m_IngSTATUS.Reset( false );
	this->m_dwRecoverTIME = 0;

	this->Del_ActiveSKILL ();
	this->Clear_SummonCNT ();			// ��Ȱ��...
	this->CObjAI::SetCMD_STOP ();

	// ������ ���ҵ� ����ġ ����
	this->Cancel_PenalEXP( SKILL_POWER(nSkillIDX) );
	// �������� ��ų�� ���� ��ġ �������� Ʋ������ ���� ��ġ ���� :: 2005. 03. 25
	this->Send_gsv_SETEXP( 0 );

	this->Set_HP( 3*this->GetCur_MaxHP() / 10 );

	// ��Ƽ�� ��� ��Ƽ������ �� ����...
	if ( this->GetPARTY() ) {
		this->GetPARTY()->Change_ObjectIDX( (classUSER*)this );
	}
}


//-------------------------------------------------------------------------------------------------
bool CObjAVT::Is_ALLIED( CAI_OBJ *pDestOBJ )
{
	if ( 0 == ( this->GetZONE()->GetGlobalFLAG() & ZONE_FLAG_PK_ALLOWED ) ) {
		if ( pDestOBJ->Get_ObjTYPE() >= OBJ_AVATAR ) {
			// PK�÷��װ� ���� �ȵ��� ���... ����� ����ڸ� ������ �Ʊ����� ����..
			return true;
		}
	}
	return CAI_OBJ::Is_ALLIED( pDestOBJ );
}


void CObjAVT::SetCur_UNION (char cValue)
{	
	if ( cValue >= 0 && cValue < MAX_UNION_COUNT )	{
		g_pThreadLOG->When_ChangeUNION( (classUSER*)this, this->GetCur_JOHAP(), cValue );
		// ���� ����
		this->m_BasicINFO.m_cUnion=cValue;	                                                                                                                                            
	}
}

//-------------------------------------------------------------------------------------------------
void CObjAVT::SetCur_JoHapPOINT( BYTE btUnionIDX, short nValue )
{	
	if ( btUnionIDX >=1 ) {
		g_pThreadLOG->When_PointUNION( (classUSER*)this, btUnionIDX, this->m_GrowAbility.m_nJoHapPOINT[ btUnionIDX-1 ], nValue );
		this->m_GrowAbility.m_nJoHapPOINT[ btUnionIDX-1 ] = nValue;	
	}
}

//-------------------------------------------------------------------------------------------------
void CObjAVT::UpdateCartGuest ()
{
	if ( Is_CartDriver () ) {
		classUSER *pGuest = g_pObjMGR->Get_UserOBJ( this->m_iLinkedCartObjIDX );
		if ( !pGuest || this->GetZONE() != pGuest->GetZONE() ) {
			this->m_iLinkedCartObjIDX = 0;
			this->m_iLinkedCartUsrIDX = 0;
			return;
		}

		// �մ� ��ġ ����~
		pGuest->m_PosCUR = this->m_PosCUR;
		this->GetZONE()->UpdateSECTOR( pGuest );
	}
}

//-------------------------------------------------------------------------------------------------
#ifdef __KCHS_BATTLECART__
bool CObjAVT::CanDoPatSkill( short nSkillIDX  )
{
	if( !this->Get_RideMODE() )
		return false;

	if( SKILL_AVAILBLE_STATUS( nSkillIDX ) < 2 ) // 2���� ũ�� īƮ �ƴϸ� ���� ��ų
		return false;

	if ( m_RideITEM[ RIDE_PART_ARMS ].m_nItemNo <= 0 ) // ARMS ��Ʈ�� ���Ⱑ �����Ǿ� �־�� �Ѵ�.
		return false;

	return true;
}

//-------------------------------------------------------------------------------------------------
// btOnOff : 0 : īƮ��ȯ �Ұ���(��Ÿ�Ӱ� ����) , 1 : īƮ ��ȯ ����
bool CObjAVT::Send_gsv_PATSTATE_CHAGE( BYTE btOnOff, DWORD dwCoolTIME )
{
	classPACKET *pCPacket = Packet_AllocNLock ();
	if ( !pCPacket )
		return false;

	pCPacket->m_HEADER.m_wType	= GSV_PATSTATE_CHANGE;
	pCPacket->m_HEADER.m_nSize	= sizeof( gsv_PATSTATE_CHANGE);
	pCPacket->m_gsv_PATSTATE_CHANGE.m_btOnOff = btOnOff;
	pCPacket->m_gsv_PATSTATE_CHANGE.m_dwCoolTIME = btOnOff ? 0 : 30 * 60 * 1000 /*PAT_COOLTIME*/;
	pCPacket->m_gsv_PATSTATE_CHANGE.m_nMaxPatHP = btOnOff ? GetCur_PatMaxHP() : 0;
	pCPacket->m_gsv_PATSTATE_CHANGE.m_wObjectIDX = this->Get_INDEX();

	this->GetZONE()->SendPacketToSectors(this, pCPacket );

	Packet_ReleaseNUnlock( pCPacket );

	if( !btOnOff )
	{
		this->m_btRideATTR = RIDE_ATTR_NORMAL;
		this->m_btRideMODE = 0; // ���� ��������..
		UpdateAbility();
		this->Send_gsv_TOGGLE( TOGGLE_TYPE_DRIVE + this->Get_MoveMODE(), true );	// Ŭ�󿡼��� �̰� �ʿ��ϴ�.. �̰� ������ Ŭ�󿡼� �ӵ� �� �����.

		this->SetCMD_STOP ();	// 2005.08.03 : icarus :: �����������
	}

	return true;
}
#endif

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

