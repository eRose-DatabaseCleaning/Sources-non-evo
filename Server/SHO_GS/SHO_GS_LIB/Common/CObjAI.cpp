/*
	** ���� ���� !!!
	common/*.cpp, *.h ���ϵ��� ������ �ҽ��� ����������
	Ŭ���̾�Ʈ�� �����ؼ� ��ġ�� ����� �ȵ� !!!! ***

	$Header: /7HeartsOnline/Server/SHO_GS/Sho_gs_lib/Common/CObjAI.cpp 44    05-10-06 10:21p Maior $
*/
#include "stdAFX.h"
#include "CObjCHAR.h"
#include "OBJECT.h"
#ifndef	__SERVER
#include "..\Network\CNetwork.h"
#include "Skill.h"
#include "it_mgr.h"
#endif

//-------------------------------------------------------------------------------------------------
/// Target Object �� ���Ѵ�.( HP üũ )
CAI_OBJ *CObjTARGET::Get_TargetOBJ(bool bCheckHP)
{	
	if ( m_iServerTarget ) {
		CObjCHAR *pTarget = g_pObjMGR->Get_ClientCharOBJ( m_iServerTarget, bCheckHP /*true*/ );
		if ( pTarget ) {
			if ( pTarget->GetZONE() == this->GetCur_ZONE() )
				return pTarget;
		}
		m_iServerTarget = 0;
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------------
CObjAI::CObjAI ()
{
	m_pCurMOTION = NULL;
	m_iCurMotionFRAME = 0, m_fAccMotionFRAME = 0.f;

    m_wState        = 0;
    m_wCommand      = 0;

	m_bAttackSTART  = false;
	m_iActiveObject = 0;

	m_bCastingSTART	= false;

	m_bRunMODE		= false;	// �⺻�� �ȱ� ���...

	m_fRunAniSPEED  = 1.0f;
	m_fCurMoveSpeed = 0;

	m_nToDoSkillIDX = 0;
	m_nActiveSkillIDX = 0;
	m_nDoingSkillIDX = 0;

	m_iWaitLoopCnt = 0;
}
CObjAI::~CObjAI ()
{
}

//-------------------------------------------------------------------------------------------------
/// ������ �ٲ��� ������ ���߱� ���� �̰����� �ű�... 2004. 2. 6
/// ���� ��ȣ�� ���ݼӵ��� ��� : ��/NPC���� : �ƹ�Ÿ/������ �ش�.
float CObjAI::Cal_AtkAniSPEED( short nRightWeaponItemNO )
{
	/// 100�� �������� ����.
	short nWeaponSpeed = WEAPON_ATTACK_SPEED( nRightWeaponItemNO ) + 5;
	float fSpeed = this->GetPsv_ATKSPEED() + ( 1500.f / nWeaponSpeed );
	return fSpeed;
}

//-------------------------------------------------------------------------------------------------
/// ���ο� ����� ���� ������� ����, ������ ����.
bool CObjAI::Set_CurMOTION( tagMOTION* pMotion )
{
	m_pCurMOTION = pMotion;
	m_iCurMotionFRAME = 0, m_fAccMotionFRAME = 0.f;

	return ( NULL != m_pCurMOTION );
}

//-------------------------------------------------------------------------------------------------
/// ���� ����� �ٲ۴�.( ���� ����̶� ������ �׳� �н� )
bool CObjAI::Chg_CurMOTION( tagMOTION* pMotion )
{
	if ( pMotion && m_pCurMOTION != pMotion ) {
		m_pCurMOTION = pMotion;
		m_iCurMotionFRAME = 0, m_fAccMotionFRAME = 0.f;
		return true;
	}

	return false;
}


//-------------------------------------------------------------------------------------------------
/// ���ݽ���.
void CObjAI::Start_ATTACK (CObjCHAR *pTarget)
{
	/// �Ϲ� ���ݽ��� ���� ����.
	m_wState = CS_ATTACK;
	m_iActiveObject = g_pObjMGR->Get_ClientObjectIndex( Get_TargetIDX() );

	// if ( Attack_START( pTarget ) )
	{
		// ���� �ִϸ��̼� & �ӵ� ����. :: ũ��Ƽ�� ������ ��� ����� ������ �������� �ʰ� ȿ���� ����Ѵ�.
		if ( this->Set_MOTION( this->GetANI_Attack(), 0, ( this->Get_nAttackSPEED() / 100.f ), true ) ) {
			this->Attack_START( pTarget );

			#if defined(_DEBUG) && !defined(__SERVER)
				if ( m_pCurMOTION->m_nActionPointCNT <= 0 ) {
					char *szMsg = CStr::Printf("%s need attack point frame!!!", Get_NAME());
					g_pCApp->ErrorBOX( szMsg, "ERROR" );
					LogString (LOG_NORMAL, szMsg);
				}
			#endif
		}
	}
}


//-------------------------------------------------------------------------------------------------
/// casting
char CObjAI::Do_SKILL (int iServerTarget, CObjCHAR *pTarget)
{
#ifdef	__INC_WORLD
	if ( this->IsUSER() ) {
		LogString( 0xffff, "%s( %.0f/%.0f ) DO_Skill: bCast:%d / ToDo:%d / Active%d, CMD:%x, STATE:%x \n", 
				this->Get_NAME(),
				this->m_PosCUR.x, this->m_PosCUR.y,
				m_bCastingSTART, m_nToDoSkillIDX, m_nActiveSkillIDX,
				m_wCommand, m_wState);
	}
#endif

	if ( !m_bCastingSTART && m_nToDoSkillIDX ) 
	{
		if ( !this->SkillChk_ACTIVE( m_nToDoSkillIDX ) ) 
		{
			// �ʿ� �ɷ�ġ ����...
			return 2;	// ��ų �Ϸ�Ȱɷ� ���� ���� ���� �����ϵ���...
		}

		// ���� �ʿ� ��ġ �Ҹ� ����...
		this->SkillUse_ABILITY( m_nToDoSkillIDX );

		Casting_START( NULL );

		m_iActiveObject   = g_pObjMGR->Get_ClientObjectIndex( iServerTarget );
		m_nActiveSkillIDX = m_nToDoSkillIDX;
		m_nToDoSkillIDX   = 0;

		if ( this->GetANI_Casting() ) {
			/// ��ų �ֹ� ���� !!!
			/// Casting ���� ���� ����.
			/// ��ų �����߿��� �´� ���۵� ���� �ȵȴ�.

			this->m_wState = CS_CASTING;
			this->Set_MOTION( this->GetANI_Casting(), 0, g_SkillList.Get_CastingAniSPEED( m_nActiveSkillIDX ) );
			return 1;
		}
	}

	if ( m_nActiveSkillIDX ) 
	{
		/// ĳ���� �Ϸ�� ��ų�� ���� ���� ���...
		Casting_END ();

		if ( 1 == Get_PatHP_MODE() ) {
			Set_PatHP_MODE( 2 );
		}

		/// ��ų �����߿��� �´� ���۵� ���� �ȵȴ�.
		this->Set_MOTION( this->GetANI_Skill(), 0, g_SkillList.Get_ActionAniSPEED( m_nActiveSkillIDX ), true );
		if ( this->Skill_START( pTarget ) ) {
			switch( SKILL_ACTION_MODE( m_nActiveSkillIDX ) ) {
				case SA_ATTACK  :	// ������ ��������( �� Ÿ���� �����ؾ�...)
					m_wCommand = CMD_ATTACK;
					m_wState   = CS_ATTACK;
					break;

				case SA_RESTORE :	// ���� ����� ��ų�̸� ������
					m_wState = CS_NEXT_STOP2;

					switch( m_wBeforeCMD ) {
						case CMD_SKILL2SELF	:
						case CMD_SKILL2OBJ	:
						case CMD_SKILL2POS	:
							m_wCommand = CMD_STOP;
							break;
						default :
							m_wCommand = m_wBeforeCMD;
					}
					break;

				default :			// ������ stop����...
					m_wCommand = CMD_STOP;
					m_wState   = CS_NEXT_STOP2;				
			}
			this->Del_ActiveSKILL ();
			return 1;
/*
			if ( SA_ATTACK == SKILL_ACTION_MODE( m_nActiveSkillIDX ) ) {
				// ���ݸ���� �̾�� ��ų... ������/ ���ݸ������ ����...
				m_wCommand = CMD_ATTACK;
				m_wState   = CS_ATTACK;

				Del_ActiveSKILL ();
				return 1;
			} else {
				m_wState = CS_NEXT_STOP2;
				Del_ActiveSKILL ();

				return 2;
			}
*/
		}
		
		Del_ActiveSKILL ();
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------
/// Move vector reset
/// 1. Normalize DirVEC, 2. DirVEC * Move Speed
/// ���� �߿��� �̵��ӵ� 0�ΰ� ����..
void CObjAI::Reset_MoveVEC ()
{
	tPOINTF DirVEC;

#ifndef	__SERVER
	this->Set_ModelDIR ( m_PosGOTO );
	this->Set_ModelSPEED( m_fCurMoveSpeed );
	DirVEC.x = m_PosGOTO.x - m_PosCUR.x;
	DirVEC.y = m_PosGOTO.y - m_PosCUR.y;
#else
	DirVEC  = m_PosGOTO - m_PosCUR;
#endif
	float fLength = DirVEC.Length () * 1000.f;
	if ( m_fCurMoveSpeed > 0 && fLength != 0.0f ) 
	{
		m_MoveVEC = ( DirVEC * m_fCurMoveSpeed ) / fLength; // Normalized dirvec * move speed
		assert( m_MoveVEC.x != 0 || m_MoveVEC.y != 0 );
	} else {
		// �̵��� �ʿ䰡 ���µ�...
		m_MoveVEC.x = 0;
		m_MoveVEC.y = 0;
	}
}

///
/// Start move
/// ���� �̵����̸� �Ÿ� ����ϰ�, MoveVEC �� ����.
/// �̵����� �ƴ϶�� ��� ����
void CObjAI::Start_MOVE( float fSpeed )
{
	m_fCurMoveSpeed = fSpeed;
	m_PosMoveSTART  = m_PosCUR;

	//LogString( LOG_NORMAL, "Start_MOVE( %s:%.0f ) , (%.0f,%.0f) => (%.0f,%.0f) \n", this->Get_NAME(), m_fCurMoveSpeed, m_PosCUR.x, m_PosCUR.y, m_PosGOTO.x, m_PosGOTO.y);

	m_iMoveDistance = distance ((int)m_PosCUR.x, (int)m_PosCUR.y, (int)m_PosGOTO.x, (int)m_PosGOTO.y);
	this->Reset_MoveVEC ();
	if ( Get_STATE() != CS_MOVE ) {
		// �̵����� �ƴϴ�.
		m_wState  = CS_MOVE;
		this->Set_MOTION( this->GetANI_Move(), m_fCurMoveSpeed, this->Get_MoveAniSPEED() );
	}
}

/// Restart move
/// ������ǥ�� �ٲ��� �ʾҴٸ� �״�� ��.
/// ������ǥ�� �����Ǿ��ٸ�, �ٽ� ��ŸƮ.
void CObjAI::Restart_MOVE (t_POSITION &PosGOTO)
{
	if ( Get_STATE() == CS_MOVE && PosGOTO.x == m_PosGOTO.x && PosGOTO.y == m_PosGOTO.y ) {
		// ���� ��ǥ�� �ٲ��� �ʾҴ�.
#ifdef	__INC_WORLD
		if ( this->IsUSER() ) {
			LogString( 0xffff, ">>SKIP Restart_MOVE ( %s ): Dist:%.0f   Pos( %.0f,%.0f )\n",
				this->Get_NAME(),
				::distance_square( m_PosCUR, m_PosGOTO ),
				this->m_PosGOTO.x, this->m_PosGOTO.y );
		}
#endif
		return;
	}

	m_PosGOTO = PosGOTO;

	this->Start_MOVE( this->Get_MoveSPEED() );
}


//-------------------------------------------------------------------------------------------------
bool CObjAI::SetCMD_SIT (void)
{
#ifdef	__BLOCK_WHEN_SKILL
	if ( m_nActiveSkillIDX ) {
		return false;
	}
#endif

	m_wState		= CS_SITTING;
	m_wCommand		= CMD_SIT;
	m_fCurMoveSpeed = 0;

	this->Set_MOTION( this->GetANI_Sitting() );
	return true;
}
bool CObjAI::SetCMD_STAND (void)
{
#ifdef	__BLOCK_WHEN_SKILL
	if ( m_nActiveSkillIDX ) {
		return false;
	}
#endif

	m_wState		= CS_STANDING;
	m_wCommand		= CMD_STOP;
	m_fCurMoveSpeed = 0;

	this->Set_MOTION( this->GetANI_Standing() );
	return true;
}

//-------------------------------------------------------------------------------------------------
/// Stop command ����.
/// I must set "m_iTargetObject" to 0??? if My state is STOP
bool CObjAI::SetCMD_STOP (void)
{
#ifdef	__BLOCK_WHEN_SKILL
	if ( m_nActiveSkillIDX ) {
		return false;
	}
#endif

	m_wState		= CS_STOP;
	m_wCommand		= CMD_STOP;
	m_fCurMoveSpeed = 0;

#ifdef	__SERVER
	this->Set_TargetIDX( 0 );
#endif

	this->Set_MOTION( this->GetANI_Stop() );
	return true;
}

//-------------------------------------------------------------------------------------------------
/// Move command ����.
/// SetCMD_MOVE (tPOINTF &PosCUR, tPOINTF &PosTO, int iTargetObjIDX) ȣ��
bool CObjAI::SetCMD_MOVE2D (float fPosX, float fPosY, BYTE btRunMODE)
{
#ifdef	__BLOCK_WHEN_SKILL
	if ( m_nActiveSkillIDX ) {
		return false;
	}
#endif
	tPOINTF PosGOTO;

	PosGOTO.x = fPosX;
	PosGOTO.y = fPosY;

	this->m_bRunMODE = ( btRunMODE != 0 );
	CObjAI::SetCMD_MOVE ((tPOINTF&)this->m_PosCUR, PosGOTO, 0);
	return true;
}

/// Move command ����.
bool CObjAI::SetCMD_MOVE (tPOINTF &PosFROM, tPOINTF &PosTO, int iServerTarget)
{
#ifdef	__BLOCK_WHEN_SKILL
	if ( m_nActiveSkillIDX ) {
		return false;
	}
#endif
	//if ( this->Get_WeightRATE() >= WEIGHT_RATE_STOP ) {
	//	// ���̴�.. ��� �Ұ�...
	//	return false;
	//}

	m_wCommand = CMD_MOVE;

	CGameOBJ *pDestOBJ = g_pObjMGR->Get_ClientOBJECT( iServerTarget );
	/// Ÿ�� ������Ʈ�� �ִٸ�. Ÿ�� ������Ʈ�� �̵�.
	if ( pDestOBJ ) 
	{
		/// ����� �������̶�� CMD_PICK_ITEM ���.
		if ( pDestOBJ->Get_TYPE() == OBJ_ITEM )
			m_wCommand = CMD_PICK_ITEM;

		m_PosGOTO = pDestOBJ->m_PosCUR;
		this->Set_TargetIDX( iServerTarget );
	} else {
		// m_pRecvPacket->m_gsv_MOUSECMD.m_PosCUR;
		m_PosGOTO.x = PosTO.x;
		m_PosGOTO.y = PosTO.y;
		this->Set_TargetIDX( 0 );
	}

	if ( CS_BIT_INT & this->Get_STATE() ) {
		if ( ( CS_BIT_MOV | CS_BIT_MOV2 ) & this->Get_STATE() )
			m_wState = CS_NEXT_STOP | CS_BIT_MOV2;
		else
			m_wState = CS_NEXT_STOP;
	} else {
		this->Start_MOVE( this->Get_MoveSPEED() );
	}
	return true;
}

//-------------------------------------------------------------------------------------------------
/// Attack command ����.
/// Ÿ���� ���� ����?( �׾���? )
bool CObjAI::SetCMD_ATTACK (int iServerTarget)
{
#ifdef	__BLOCK_WHEN_SKILL
	if ( m_nActiveSkillIDX ) {
		return false;
	}
#endif
	//if ( this->Get_WeightRATE() >= WEIGHT_RATE_STOP ) {
	//	// ���̴�.. ��� �Ұ�...
	//	return false;
	//}

	CObjCHAR *pDestCHAR = g_pObjMGR->Get_ClientCharOBJ( iServerTarget, true );
	if ( pDestCHAR ) {
		// ���� ���¸�...
		if ( pDestCHAR->m_IngSTATUS.IsSET( FLAG_ING_DISGUISE | FLAG_ING_TRANSPARENT ) )
			return false;

		if ( CMD_ATTACK == m_wCommand && this->Get_TargetIDX() == iServerTarget ) {
			// ��ȭ ����..
			return false;
		}

		m_wCommand = CMD_ATTACK;
		m_PosGOTO  = pDestCHAR->m_PosCUR;

		this->Set_TargetIDX( iServerTarget );

		if ( ( CS_BIT_MOV | CS_BIT_MOV2 ) & this->Get_STATE() ) {	// �̵���...
			m_wState = CS_BIT_MOV2 | ( ( CS_BIT_INT & this->Get_STATE() ) ? CS_NEXT_STOP : CS_STOP );
		} else {
			m_wState = ( CS_BIT_INT & this->Get_STATE() ) ? CS_NEXT_STOP : CS_STOP;
		}
	} else {
		this->Set_TargetIDX( 0 );

		if ( CMD_STOP == m_wCommand && 0 == this->Get_TargetIDX() ) {
			// ��ȭ ����..
			return false;
		}
		m_wCommand = CMD_STOP;
		m_wState =  ( CS_BIT_INT & this->Get_STATE() ) ? CS_NEXT_STOP : CS_STOP;
	}

	/// ���¿� ��ȭ�� �����.
	return true;
}

//-------------------------------------------------------------------------------------------------
/// Die command ����.
void CObjAI::SetCMD_DIE ()
{
	this->Set_TargetIDX( 0 );

	m_wState   = CS_DIE;
	m_wCommand = CMD_DIE;

	this->Del_ActiveSKILL ();
	this->Set_MOTION( this->GetANI_Die() );
}


//-------------------------------------------------------------------------------------------------
bool CObjAI::SetCMD_Skill2SELF (short nSkillIDX)
{
#ifdef	__BLOCK_WHEN_SKILL
	if ( m_nActiveSkillIDX ) {
		return false;
	}
#endif

	if ( CS_BIT_INT & this->m_wState )
		m_wState = CS_NEXT_STOP;
	else
		m_wState = CS_STOP;

	m_nToDoSkillIDX = nSkillIDX;
/*
	switch( m_wCommand ) {
		case CMD_SKILL2SELF	:
		case CMD_SKILL2OBJ	:
		case CMD_SKILL2POS	:
			m_wBeforeCMD = CMD_STOP;	// ��ų�� ��� ¯���� �ʴ´�.
			break;
		default :
			m_wBeforeCMD = m_wCommand;
	} 
*/
	m_wBeforeCMD = m_wCommand;
	m_wCommand = CMD_SKILL2SELF;

	return true;
}

bool CObjAI::SetCMD_Skill2OBJ (int iServerTarget, short nSkillIDX)
{
#ifdef	__BLOCK_WHEN_SKILL
	if ( m_nActiveSkillIDX ) {
		return false;
	}
#endif
	if ( nSkillIDX == m_nToDoSkillIDX && CMD_SKILL2OBJ == m_wCommand && iServerTarget == this->Get_TargetIDX() )
		return false;
	
	//if ( this->Get_WeightRATE() >= WEIGHT_RATE_STOP ) {
	//	// ���̴�.. ��� �Ұ�...
	//	return false;
	//}

	CObjCHAR *pDestCHAR = g_pObjMGR->Get_ClientCharOBJ( iServerTarget, false /*true*/ );
	if ( pDestCHAR ) {
		bool bCheckHP;
		// ���������� ���
		if ( SKILL_TARGET_FILTER_DEAD_USER == SKILL_CLASS_FILTER( nSkillIDX ) ) {
			// ������̰� ������.
			if ( !pDestCHAR->IsUSER() || pDestCHAR->Get_HP() > 0 )
				return false;
			bCheckHP = false;
		} else {
			if ( pDestCHAR->Get_HP() <= 0 )
				return false;

			// ���� ���¸�...
			if ( pDestCHAR->m_IngSTATUS.IsSET( FLAG_ING_DISGUISE | FLAG_ING_TRANSPARENT ) && !this->Is_ALLIED(pDestCHAR) )
				return false;

			bCheckHP = true;
		}

#ifdef	__INC_WORLD
		if ( this->IsUSER() ) {
			LogString( 0xffff, ">>>> SetCMD_Skill2OBJ %d :: %.0f, %.0f \n", nSkillIDX, m_PosCUR.x, m_PosCUR.y );
		}
#endif
		
		if ( ( CS_BIT_MOV | CS_BIT_MOV2 ) & this->Get_STATE() ) {
			m_wState = CS_BIT_MOV2 | ( ( CS_BIT_INT & this->Get_STATE() ) ? CS_NEXT_STOP: CS_STOP );
		} else {
			m_wState = ( CS_BIT_INT & this->Get_STATE() ) ? CS_NEXT_STOP : CS_STOP;
		}

		m_wBeforeCMD	= m_wCommand;
		m_wCommand		= CMD_SKILL2OBJ;
		m_nToDoSkillIDX = nSkillIDX;
		m_PosGOTO		= pDestCHAR->m_PosCUR;	// ?????

		this->Set_TargetIDX( iServerTarget, bCheckHP );

		return true;
	} 

	return false;
}

bool CObjAI::SetCMD_Skill2POS (tPOINTF &PosGOTO, short nSkillIDX)
{
#ifdef	__BLOCK_WHEN_SKILL
	if ( m_nActiveSkillIDX ) {
		return false;
	}
#endif

	//if ( this->Get_WeightRATE() >= WEIGHT_RATE_STOP ) {
	//	// ���̴�.. ��� �Ұ�... ��ų ���� �Ÿ��� ???
	//	float fDist = ::distance_square( this->m_PosCUR, PosGOTO );
	//	if ( fDist > SKILL_DISTANCE( nSkillIDX ) ) 
	//		return false;
	//}

	if ( ( CS_BIT_MOV | CS_BIT_MOV2 ) & this->Get_STATE() ) {
		m_wState = CS_BIT_MOV2 | ( ( CS_BIT_INT & this->Get_STATE() ) ? CS_NEXT_STOP : CS_STOP );
	} else {
		m_wState = ( CS_BIT_INT & this->Get_STATE() ) ? CS_NEXT_STOP : CS_STOP;
	}

	m_nToDoSkillIDX = nSkillIDX;

	m_wBeforeCMD	= m_wCommand;
	m_wCommand		= CMD_SKILL2POS;
	m_PosGOTO.x		= PosGOTO.x;
	m_PosGOTO.y		= PosGOTO.y;
	this->Set_TargetIDX( 0 );
	return true;
}

//-------------------------------------------------------------------------------------------------
/// �ִϸ��̼� ����..
/// @bug -->�ȿ��� AI Action�� ���� m_pCurMOTION�� �ٲ�� ���� ��찡 ���� ��~~~~~~~
// ���� ȣ��Ǿ��� �κп��� ����� �ð���ŭ ��� �������� ����.
bool CObjAI::ProcMotionFrame (void)
{
	int iFrame = this->GetCurrentFrame();
	if ( iFrame < 0 ) 
	{
		m_iCurMotionFRAME = 0, m_fAccMotionFRAME = 0.f;

		m_wState &= ~CS_BIT_INT;
		return false;
	}

#ifdef	__INC_WORLD
	assert( iFrame <= m_pCurMOTION->m_wTotalFrame );
	assert( m_iCurMotionFRAME >= 0 );
#endif

	/// ������������ üũ �ؾߵȴٸ�..
	if ( m_wState & CS_BIT_CHK ) 
	{
		/// �ѹ��� üũ
		if ( m_wState & CS_BIT_ONE ) 
		{
			/// iFrame < m_iCurMotionFRAME �ִϸ��̼��� �ѹٱ� ���� iFrame�� �����������
			if ( iFrame < m_iCurMotionFRAME || m_iCurMotionFRAME >= m_pCurMOTION->m_wTotalFrame ) 
			{
				// ����� �Ϸ� �ƴ�.
				m_iCurMotionFRAME = 0, m_fAccMotionFRAME = 0.f;
				m_wState &= ~CS_BIT_INT;
				return false;
			}
		}
	}

	/// @todo m_iCurMotionFRAME = iFrame;	  �̰� �־��µ� ���..
	/// ����� ������.
	/// iFrame < m_iCurMotionFRAME �ִϸ��̼��� �ѹٱ� ���� iFrame�� �����������
	if ( iFrame < m_iCurMotionFRAME || m_iCurMotionFRAME >= m_pCurMOTION->m_wTotalFrame ) 
	{
		m_iCurMotionFRAME = iFrame;	
		return false;
	}

	m_iCurMotionFRAME = iFrame;	

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CObjAI::Goto_TARGET( CObjCHAR *pTarget, int iRange )
{
/*
#ifdef	__INC_WORLD
	if ( this->IsUSER() ) {
		LogString( 0xffff, ">>Goto_TARGET( %s -> %s ): Dist:%.0f   Pos( %.0f,%.0f )=> ( %.0f,%.0f )\n",
			this->Get_NAME(), pTarget->Get_NAME(),
			::distance_square( m_PosCUR, pTarget->m_PosCUR ),
			this->m_PosCUR.x, this->m_PosCUR.y,
			pTarget->m_PosCUR.x, pTarget->m_PosCUR.y );
	}
#endif
*/

	if ( IsInRANGE( pTarget, iRange ) ) {
		/// �Ÿ��ȿ� ��� �Դ�...
		this->m_PosGOTO = this->m_PosCUR;
#ifndef	__SERVER
		this->Move_COMPLETED ();
#endif
		return true;
	}

	// ���� ��ġ�� �ٲ������ �̵� ���� �ٽ� ���...
	// �̵����� �ƴ� ���¸�...�̵����� �ʴ´�.
	this->Restart_MOVE( pTarget->m_PosCUR );

	return false;
}

bool CObjAI::Goto_POSITION (int iRange)
{
	if ( !(Get_STATE() & CS_BIT_MOV) )
	{
		// �̵����� �ƴϸ� �̵� ����...
		this->Start_MOVE( this->Get_MoveSPEED() );
	}

	int iDistance = distance ((int)m_PosCUR.x, (int)m_PosCUR.y, (int)m_PosMoveSTART.x, (int)m_PosMoveSTART.y);

/*
#ifdef	__INC_WORLD
	if ( this->IsUSER() ) {
		int iDist2 = distance ((int)m_PosMoveSTART.x, (int)m_PosMoveSTART.y, (int)m_PosGOTO.x, (int)m_PosGOTO.y);

		LogString( 0xffff, ">>Goto_POSITION( %s ): Dist:(%d+%d)%d / %d, Start( %.0f,%.0f ), Cur( %.0f,%.0f ), GoTo( %.0f,%.0f ),,, Start->Goto:%d\n",
			this->Get_NAME(),
			iDistance, iRange, iDistance + iRange, m_iMoveDistance,
			this->m_PosMoveSTART.x, this->m_PosMoveSTART.y,
			this->m_PosCUR.x, this->m_PosCUR.y,
			this->m_PosGOTO.x, this->m_PosGOTO.y,
			iDist2 );
	}
#endif
*/

	if ( iDistance+iRange >= m_iMoveDistance ) {
#ifdef	__INC_WORLD
		if ( this->IsUSER() ) {
			int iTemp=0;
		}
#endif
		/// ���� 
		/// this->Move_COMPLETED ();  iRange���� ���� ���� �ɰ�� ȣ��Ǿ� ��ǥ ������ �߻��߾���...
		return true;
	} 

	return false;
}

//-------------------------------------------------------------------------------------------------
int CObjAI::ProcCMD_SIT ()
{
	if ( this->Get_STATE() != CS_SIT ) {
		this->Set_STATE( CS_SIT );
		this->Set_MOTION( this->GetANI_Sit() );
	} 

	return 1;
}

//-------------------------------------------------------------------------------------------------
/// Stop command
/// ������ ������¿����� Do_StopAI ȣ���� ���ǹ� �ϴ�..
int CObjAI::ProcCMD_STOP ()
{
	if ( Get_STATE() != CS_STOP ) 
	{
		CObjAI::SetCMD_STOP ();
	} 

	this->Do_StopAI ();
	return 1;
}

//-------------------------------------------------------------------------------------------------
/// �̵� ���μ���..
/// �̵��� ����� NPC�̰ų�, ITEM �� ��� ó��..
int CObjAI::ProcCMD_MOVE ()
{
	CObjCHAR *pTarget = (CObjCHAR*)this->Get_TARGET();

#ifdef	__INC_WORLD
	if ( this->IsUSER() ) {
		int i=0;
	}
#endif

	if ( pTarget )
	{
		if( pTarget->Is_AVATAR() ) 
		{
			/// ����� ����ڶ��
			if ( Goto_TARGET( pTarget, AVT_CLICK_EVENT_RANGE ) ) 
			{
				CObjAI::SetCMD_STOP ();
	 		}

			return 1;
		} else 
		if ( pTarget->IsA( OBJ_NPC ) ) 
		{
			/// ����� NPC���
			if ( Goto_TARGET( pTarget, NPC_CLICK_EVENT_RANGE ) ) 
			{
				// �����ߴ�... ��ȭ !
				pTarget->Check_EVENT( this );
				CObjAI::SetCMD_STOP ();
			}

			return 1;
		}
	}

	/// m_PosGOTO�� �̵�..
	if ( this->Goto_POSITION () ) {
		CObjAI::SetCMD_STOP ();
	} 

	return 1;
}


//-------------------------------------------------------------------------------------------------
/// �̵��� �������� ����ϰ���� ó��..
/// ��� �������̶� �̸� �پ�����.. Command �� CMD_PICK_ITEM �� �ƴϸ� �Ϲ� ����.
int CObjAI::ProcCMD_PICK_ITEM ()
{
	/// m_PosGOTO�� �̵�..
	if ( this->Goto_POSITION( ITEM_CLICK_EVENT_RANGE ) ) 
	{
		CObjAI::SetCMD_STOP ();
	} 

	return 1;
}

//-------------------------------------------------------------------------------------------------
///
int CObjAI::ProcCMD_ATTACK ()
{	
	CObjCHAR *pTarget = (CObjCHAR*)this->Get_TARGET();
	if ( pTarget ) {
		if ( Get_STATE() & CS_BIT_ATTACK ) {
			/// �������̴�..
			if ( !(Get_STATE() & CS_BIT_INT) ) {			
				if ( !IsInRANGE( pTarget, this->Get_AttackRange( 0 ) ) ) {
					// ������ ���� ������ �̵�...
					m_wState  = CS_STOP;
					m_PosGOTO = pTarget->m_PosCUR;
					this->Start_MOVE( this->Get_MoveSPEED() );
					return 1;
				}

				// ������ ���� ���� !!!
				this->Start_ATTACK (pTarget);
			}

			return 1;
		} 

		/// Ÿ������ �̵�...
		if ( Goto_TARGET( pTarget, this->Get_AttackRange( 0 ) ) ) {
			// ���� ������ ���� ���� !!!
			this->Start_ATTACK (pTarget);
		} else {
			if ( !(Get_STATE() & CS_BIT_MOV) ) {
				this->Start_MOVE( this->Get_MoveSPEED() );
			} else
				this->Do_AttackMoveAI( pTarget );		/// MOB ���� �̵��� �ΰ����� ó��..
		}
	} else {
		/// Ÿ���� ���� :: ���� ������ ��ų���ٸ� ???
		if ( Get_ActiveSKILL() )
			Del_ActiveSKILL ();

		this->Set_TargetIDX( 0 );
		m_wCommand = CMD_STOP;
	}

	return 1;
}


//-------------------------------------------------------------------------------------------------
/// ���� ������ �ٷ� ó���� ��ų --> ���� ���·� �ȴ�.
int CObjAI::ProcCMD_Skill2SELF ()
{
	if ( 1 != this->Do_SKILL( 0 ) ) 
	{
		// casting: 1, cancel: 0, active: 2
		// m_wCommand = CMD_STOP;
		m_wCommand = m_wBeforeCMD;
	}
	return 1;
}

//-------------------------------------------------------------------------------------------------
/// �������� �̵��� ��ų ó�� --> ���� ���·� �ȴ�.
int CObjAI::ProcCMD_Skill2POSITION ()
{
	// ��ҷ� �̵�... & ��ų ����
	if ( !m_bCastingSTART ) {
		if ( !this->Goto_POSITION ( this->Get_AttackRange( m_nToDoSkillIDX ) ) ) 
			return 1;
	}

	/// ĳ���� �Ǵ� �������� ����...
	if ( 1 != this->Do_SKILL( 0 ) ) 
	{
		// casting: 1, cancel: 0, active: 2
		m_wCommand = CMD_STOP;
	}

	return 1;
}

//-------------------------------------------------------------------------------------------------
/// Ÿ������ �̵��� ��ų ó�� --> ���� ���·� �ȴ�.
int CObjAI::ProcCMD_Skill2OBJECT ()
{
	// Ÿ������ �̵�... & ��ų ����
	CObjCHAR *pTarget = (CObjCHAR*)this->Get_TARGET( false /* true */ );
	if ( pTarget ) 
	{
		if ( !m_bCastingSTART )
		{
			/// ĳ�������� �ƴϸ� Ÿ������ �̵�...
			if( !Goto_TARGET( pTarget, this->Get_AttackRange( m_nToDoSkillIDX ) ) ) {
				if ( !(Get_STATE() & CS_BIT_MOV) ) {
					// �̵����� �ƴϸ�..
					this->Start_MOVE( this->Get_MoveSPEED() );
				}
				return 1;
			}
			/// �̵��� �Ϸ� ������....
			/// Cool Time ���� ///������

		}

		/// ĳ���� �Ǵ� �������� ����...
		switch ( this->Do_SKILL( this->Get_TargetIDX(), pTarget ) ) {
			case 2 :	// �ɷ�ġ ����..
				this->Send_gsv_SKILL_CANCEL( SKILL_CANCEL_NEED_ABILITY );
			case 0 :
				// casting: 1, cancel: 0, active: 2
				m_wCommand = CMD_STOP;
				break;
		}
	} else {
		// Ÿ�� �����Ǿ� ��ų ���...
		if ( this->m_nActiveSkillIDX ) {
			// �ɽ��� �����ؼ� MP�� �Ҹ�ƴ�...
			this->Send_gsv_SKILL_CANCEL( SKILL_CANCEL_TARGET_NOT_FOUND );
		} else {
			this->Send_gsv_SKILL_CANCEL( SKILL_CANCEL_NEED_TARGET );
		}

		this->Set_TargetIDX( 0 );
		this->Del_ActiveSKILL ();
		m_wCommand = CMD_STOP;
	}

	return 1;
}

//-------------------------------------------------------------------------------------------------
