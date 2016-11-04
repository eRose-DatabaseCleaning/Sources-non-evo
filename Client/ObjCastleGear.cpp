#include "stdafx.h"
#include ".\objcastlegear.h"
#include "Common/IO_Pat.h"

CObjCastleGear::CObjCastleGear(void)
{
}

CObjCastleGear::~CObjCastleGear(void)
{
}

int CObjCastleGear::GetANI_Stop()	{	return PAT_RELATIVE_MOTION_POS( m_nPartItemIDX[ RIDE_PART_BODY ] ) +  CASTLE_GEAR_ANI_STOP1;}
int CObjCastleGear::GetANI_Move()	{	return PAT_RELATIVE_MOTION_POS( m_nPartItemIDX[ RIDE_PART_BODY ] ) +  CASTLE_GEAR_ANI_MOVE;	}
int	CObjCastleGear::GetANI_Attack()	{	return PAT_RELATIVE_MOTION_POS( m_nPartItemIDX[ RIDE_PART_BODY ] ) +  CASTLE_GEAR_ANI_ATTACK01 + RANDOM(3);	}
int	CObjCastleGear::GetANI_Die ()	{	return PAT_RELATIVE_MOTION_POS( m_nPartItemIDX[ RIDE_PART_BODY ] ) +  CASTLE_GEAR_ANI_DIE;	}
int	CObjCastleGear::GetANI_Hit()	{	return PAT_RELATIVE_MOTION_POS( m_nPartItemIDX[ RIDE_PART_BODY ] ) +  CASTLE_GEAR_ANI_STOP1;	}	
int	CObjCastleGear::GetANI_Casting(){	return 0;	}	
int	CObjCastleGear::GetANI_Skill()	{	return 0;	}	


bool CObjCastleGear::Create( CObjCHAR* pParent, int iCartType, D3DVECTOR &Position )
{
	if( CObjCART::Create( pParent, iCartType, Position ) )
	{
		///::setScale( this->GetZMODEL(), 1.2f, 1.2f, 1.2f );
		return true;
	}

	return false;
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief /*override*/virtual bool	SetCMD_ATTACK ( int iServerTarget ); �� ������ �ʿ����.
///			CObjAI::SetCMD_ATTACK �� ���������� ȣ���ϴ� �Լ��̴�.
//----------------------------------------------------------------------------------------------------

void CObjCastleGear::SetCMD_ATTACK (int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosGOTO)
{
	CObjCHAR::SetCMD_ATTACK( iServerTarget, wSrvDIST, PosGOTO );

	//----------------------------------------------------------------------------------------------------
	/// �� ž���� �� ĳ������ ����� ��ü�Ѵ�.
	//----------------------------------------------------------------------------------------------------	
	m_pObjParent->Set_MOTION( this->GetRideAniPos() + PETMODE_AVATAR_ANI_ATTACK01 + +RANDOM(3) );


	m_iOldCartState = m_iCurrentCartState;
	m_iCurrentCartState = CART_STATE_ATTACK;

	StopSound( m_iOldCartState );
	PlaySound( m_iCurrentCartState );
}


