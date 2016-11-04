#include "stdafx.h"
#include ".\cobjcart.h"
#include "Common/IO_Pat.h"
#include "Interface/it_mgr.h"
#include "CObjCHAR_Collision.h"


CObjCART::CObjCART(void)
{
	m_pObjParent	= NULL;
	m_iCartType		= 0;

	m_iCurrentCartState	= CART_STATE_STOP;
	m_iOldCartState		= CART_STATE_STOP;

//----------------------------------------------------------------
//����ȣ
	m_pRideUser = NULL;

//----------------------------------------------------------------

}

CObjCART::~CObjCART(void)
{
	if( m_pObjParent != NULL )
	{
		D3DXVECTOR3 pos = Get_CurPOS();
		::unlinkNode( m_pObjParent->GetZMODEL() );
		m_pObjParent->ResetCUR_POS( pos );
		///::savePrevPosition( m_pObjParent->GetZMODEL() );

		CopyCollisionInformation( false );
		
		((CObjAVT*)m_pObjParent)->m_pObjCART = NULL;
		m_pObjParent = NULL;
	}

	this->StopSound( m_iOldCartState );

//-----------------------------------------------------------------
//����ȣ
	m_pRideUser = NULL;
//-----------------------------------------------------------------

}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ������ ��ũ�� �ֵ��� ��� �и�
//----------------------------------------------------------------------------------------------------
void CObjCART::UnLinkChild(int iStart)
{

#if defined(_GBC)
//----------------------------------------------------------------
//����ȣ::īƮ���� ž���� ������ ��ũ���� �Ѵ�. (����)
	D3DXVECTOR3 pos = Get_CurPOS();		

	CObjCHAR* pClass[] = { m_pObjParent, m_pRideUser };

	int si=0;
	
	if(iStart)
	{	si = 1; } 

	for(register int i=si;i<2;i++)
	{
		if(pClass[i] == NULL)
			continue;
		
		::unlinkVisibleWorld( pClass[i]->GetZMODEL() );
						
		if(i == 0)
		{
			pClass[i]->ReleaseRideUser();
			pClass[i]->ResetCUR_POS( pos );
			pClass[i]->SetResetPosZ( true, pos.z );
		}
		//�޿� ž���� ������ ���� �ڿ� ��ġ�� ž���Ѵ�. 
		else
		{
			D3DXVECTOR3 ruDir;
			D3DXVECTOR3 ruPos = pos;

			::getModelDirectionVector(this->GetZMODEL(),&ruDir.x);
			
			ruPos+=((ruDir * -1.0f) * 200.0f);
			
			pClass[i]->ResetCUR_POS( ruPos );
			//pClass[i]->DropFromSky(ruPos.x,ruPos.y);
			pClass[i]->SetResetPosZ( true, pos.z + 200);
			
			m_pRideUser = NULL;
		}

		pClass[i]->Set_STATE(CS_STOP);
		pClass[i]->SetCMD_STOP();


		CopyCollisionInformation( false );

		///((CObjAVT*)m_pObjParent)->m_pObjCART = NULL;
		pClass[i] = NULL;

		this->StopSound( m_iCurrentCartState );
	}
#else	
	if( m_pObjParent != NULL )
	{
		D3DXVECTOR3 pos = Get_CurPOS();		
	
		::unlinkVisibleWorld( m_pObjParent->GetZMODEL() );
		m_pObjParent->ResetCUR_POS( pos );
		m_pObjParent->SetResetPosZ( true, pos.z );
		///::savePrevPosition( m_pObjParent->GetZMODEL() );
		CopyCollisionInformation( false );

		///((CObjAVT*)m_pObjParent)->m_pObjCART = NULL;
		m_pObjParent = NULL;

		this->StopSound( m_iCurrentCartState );
	}
#endif 

}


void CObjCART::SetPetParts( unsigned int iPartIDX, unsigned int iItemIDX )
{
	if( iPartIDX >= MAX_RIDING_PART )
		return;

	m_nPartItemIDX[ iPartIDX ] = iItemIDX;
}

bool CObjCART::Create ( CObjCHAR* pParent, int iCartType, D3DVECTOR &Position )
{
	if( pParent == NULL )
		return false;

	float fDir = ::getModelDirection( pParent->GetZMODEL() );

	/// CObjCHAR�� �޽�� ����� ���ؼ�( InserToScene ���� )
	m_pCharMODEL = &m_CharMODEL;

	m_pObjParent = pParent;
	m_iCartType  = iCartType;


	char *szName  = CStr::Printf ( "%s_Cart", pParent->Get_NAME() );

	CMODEL<CCharPART> *pPartMODEL = NULL;
	for ( short nI = 0; nI < MAX_RIDING_PART; nI++ ) 
	{		
		pPartMODEL = g_DATA.m_pMD_CartPARTS->GetMODEL( m_nPartItemIDX[ nI ] );
		m_CharMODEL.SetPartMODEL( nI, pPartMODEL );	
	}	

	if ( CObjCHAR::CreateCHAR ( szName, &m_CharMODEL, MAX_RIDING_PART, Position) ) 
	{		
		this->m_iHP = pParent->Get_HP();		
		HNODE hChild = m_pObjParent->GetZMODEL();
		
		this->LinkDummy( hChild, 0 );					
		this->Set_ModelDIR( fDir, 1 );  

		/// CObjCHAR::CreateCHAR ���ο��� DropFromSky �Լ��� ȣ���ϱ� ������ �ٽ� ������ ����ߵȴ�.
		/// �̰Ͷ����� Ÿ�� ������ �ٸ��Ʒ����� Ÿ�� ������ ������ �־���. �Ʒ����� ���µ� �ٸ� ���� �ö� ������..
		this->ResetCUR_POS( D3DXVECTOR3( Position.x, Position.y, Position.z ) );

		::setUserData( this->GetZMODEL(), reinterpret_cast< HNODE >( this ) );
		///::setScale( this->GetZMODEL(), 2.0f, 2.0f, 2.0f );

		SetEffect();

		CopyCollisionInformation( true );


		return true;
	}	

	LogString (LOG_DEBUG, "Cart create failed .. %d: %s \n", szName );

	/// īƮ �� ĳ����� ũ�� 1.2 �� Ȯ��..
	::setScale( this->GetZMODEL(), 1.2f, 1.2f, 1.2f );

	return false;
}



//----------------------------------------------------------------------------------------------------
/// @����ȣ
/// @����ȣ::���õ� ������ īƮ�� �¿��.
//----------------------------------------------------------------------------------------------------
bool CObjCART::Create( CObjCHAR* pTarget)
{
	
	if(m_pObjParent == NULL)
		return false;

	m_pRideUser = pTarget;

	//��ġ �ʱ�ȭ 
	pTarget->Set_CurPOS(D3DXVECTOR3(0,0,0));
	::setPosition(pTarget->GetZMODEL(),0,0,0);

	HNODE hChild = pTarget->GetZMODEL();
		
	this->LinkDummy(hChild,10);					
	
	//ȸ�� �� ��� ���� 
	m_pRideUser->Set_ModelDIR(0.0f,1);  
	m_pRideUser->Set_MOTION( this->GetRideAniPos() + PETMODE_AVATAR_ANI_STOP1 );
	
	return true;

}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief Cart �� ���� ���⿡ ���� ����� �ƴ϶� ARMS ���̺��� ��� Ÿ�Կ� �����Ѵ�.
//----------------------------------------------------------------------------------------------------
tagMOTION *CObjCART::Get_MOTION (short nActionIdx)
{	

//����ȣ::
#if defined(_GBC)
	short nFileIDX = FILE_MOTION( PAT_RELATIVE_MOTION_POS( this->m_nWeaponIDX ), nActionIdx );
#else
	short nFileIDX = FILE_MOTION( PAT_RELATIVE_MOTION_POS( this->m_nAbilIDX ), nActionIdx );
#endif 

	if ( 0 == nFileIDX ) 
	{
		assert( 0 && "PET Motion is Invalid" );
		g_itMGR.OpenMsgBox( "INVALID PET MOTION" );
		
#if defined(_GBC)
		nFileIDX = FILE_MOTION( PAT_RELATIVE_MOTION_POS( this->m_nWeaponIDX ), 0 );
#else 
		nFileIDX = FILE_MOTION( PAT_RELATIVE_MOTION_POS( this->m_nAbilIDX ), 0 );
#endif 


	}

	tagMOTION *pMOTION = g_MotionFILE.IDX_GetMOTION( nFileIDX, false );
	if ( pMOTION ) 
	{
		pMOTION->m_nActionIdx = nActionIdx;
	}

	return pMOTION;
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief īƮ�� ����Ʈ ����
///		   �Ϲ� ��ǰ�� ȿ���� �׻� ������, ���� ��ǰȿ���� �̵��ÿ��� ���´�.
//----------------------------------------------------------------------------------------------------

void CObjCART::SetEffect()
{
	for ( short nI = 0; nI < MAX_RIDING_PART; nI++ ) 
	{
		if( nI != RIDE_PART_LEG )
			SetPartEffect( nI );		
	}
	//SetPartEffect( RIDE_PART_ENGINE );
}

#define MAX_CART_EFFECT_DUMMIES		8
void CObjCART::SetPartEffect( int iPart )
{
	int iEffectNO = 0;
	for( int i = 0; i < MAX_CART_EFFECT_DUMMIES; i++ )
	{
		iEffectNO = PAT_ITEM_ATTACHED_EFFECT( m_nPartItemIDX[ iPart ], i );

		/// ������ ����Ʈ�� �������..
		if( iEffectNO )
		{
			CEffect *pEffect = g_pEffectLIST->Add_EffectWithIDX( iEffectNO, true );
			if ( pEffect ) 
			{
				this->LinkDummy( pEffect->GetZNODE(), i );
				this->AddExternalEffect( pEffect );
				pEffect->SetParentCHAR( this );

				pEffect->InsertToScene();
			}		
		}
	}
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �̵��߿��� ������ ����Ʈ. Ȥ�� �� �ݴ�.
//----------------------------------------------------------------------------------------------------
void CObjCART::SetEffectByMoveState( bool bShow )
{
}

///
/// īƮ�� ���� ����
///
void CObjCART::PlaySound( int iCurrentState )
{
	for ( short nI = 0; nI < MAX_RIDING_PART; nI++ ) 
	{			
		PlayPartSound( nI, iCurrentState );		
	}
}

void CObjCART::PlayPartSound( int iPart, int iCurrentState )
{
	switch( iCurrentState )
	{
		case CART_STATE_STOP:
			{
				int iSoundIDX = PAT_STOP_SOUND( m_nPartItemIDX[ iPart ] );
				if( iSoundIDX )
				{
					D3DXVECTOR3 vecWorld = this->Get_CurPOS();
					g_pSoundLIST->IDX_PlaySound3DLoop( iSoundIDX, vecWorld, NULL ); 
				}
			}
			break;

		case CART_STATE_MOVE:
			{
				int iSoundIDX = PAT_MOVE_SOUND( m_nPartItemIDX[ iPart ] );
				if( iSoundIDX )
				{
					D3DXVECTOR3 TargetPos;
					TargetPos.x = this->m_PosGOTO.x;
					TargetPos.y = this->m_PosGOTO.y;
					TargetPos.z = 100.0f;

					D3DXVECTOR3 Velocity = TargetPos - Get_CurPOS(); // 1�� ���ȿ� �̵��ϴ� ����

					D3DXVECTOR3 vecWorld = this->Get_CurPOS();
					g_pSoundLIST->IDX_PlaySound3DLoop( iSoundIDX, vecWorld, &Velocity ); 
				}
			}
			break;	
	}
}

void CObjCART::StopSound( int iCurrentState )
{
	for ( short nI = 0; nI < MAX_RIDING_PART; nI++ ) 
	{			
		StopPartSound( nI, iCurrentState );		
	}
}

void CObjCART::StopPartSound( int iPart, int iCurrentState )
{
	switch( iCurrentState )
	{
		case CART_STATE_STOP:
			{
				int iSoundIDX = PAT_STOP_SOUND( m_nPartItemIDX[ iPart ] );
				if( iSoundIDX )
				{					
					g_pSoundLIST->IDX_StopSound3DLoop( iSoundIDX ); 
				}
			}
			break;

		case CART_STATE_MOVE:
			{				
				int iSoundIDX = PAT_MOVE_SOUND( m_nPartItemIDX[ iPart ] );
				if( iSoundIDX )
				{					
					g_pSoundLIST->IDX_StopSound3DLoop( iSoundIDX ); 
				}
			}
			break;
		case CART_STATE_ATTACK:
			{
				int iSoundIDX = PAT_STOP_SOUND( m_nPartItemIDX[ iPart ] );
				if( iSoundIDX )
				{					
					g_pSoundLIST->IDX_StopSound3DLoop( iSoundIDX ); 
				}
			}
			break;
	}
}


int	CObjCART::GetRideAniPos()	{ return PAT_RELATIVE_MOTION_POS_AVATAR( m_nPartItemIDX[ RIDE_PART_BODY ] ); }


int	CObjCART::GetANI_Stop()		{	return PAT_RELATIVE_MOTION_POS( m_nPartItemIDX[ RIDE_PART_BODY ] ) +  CART_ANI_STOP1;	}
int	CObjCART::GetANI_Move()		
{	
	return PAT_RELATIVE_MOTION_POS( m_nPartItemIDX[ RIDE_PART_BODY ] ) +  CART_ANI_MOVE;	
}
int	CObjCART::GetANI_Attack()	{	return PAT_RELATIVE_MOTION_POS( m_nPartItemIDX[ RIDE_PART_BODY ] ) +  CART_ANI_ATTACK01; /*+ RANDOM(3);*/	}
int	CObjCART::GetANI_Die ()		{	return PAT_RELATIVE_MOTION_POS( m_nPartItemIDX[ RIDE_PART_BODY ] ) +  CART_ANI_DIE;	}
int	CObjCART::GetANI_Hit()		{	return PAT_RELATIVE_MOTION_POS( m_nPartItemIDX[ RIDE_PART_BODY ] ) +  CART_ANI_STOP1;	}	

//����ȣ::��ų �ִϸ��̼� 
int	CObjCART::GetANI_Casting()	{	return SKILL_ANI_CASTING(m_nActiveSkillIDX);							}	
int	CObjCART::GetANI_CastingRepeat()	{	return  SKILL_ANI_CASTING_REPEAT(m_nActiveSkillIDX);			}	
int	CObjCART::GetANI_Skill()	{	return 	SKILL_ANI_ACTION_TYPE(m_nActiveSkillIDX);						}	



//----------------------------------------------------------------------------------------------------
/// @param
/// @brief
//----------------------------------------------------------------------------------------------------

bool CObjCART::SetCMD_MOVE( const D3DVECTOR& PosTO, BYTE btRunMODE )
{
	CObjAI::SetCMD_MOVE( PosTO, btRunMODE );

	//----------------------------------------------------------------------------------------------------
	/// �� ž���� �� ĳ������ ����� ��ü�Ѵ�.
	//----------------------------------------------------------------------------------------------------		
	m_pObjParent->Set_MOTION( this->GetRideAniPos() + PETMODE_AVATAR_ANI_MOVE );
	
	//----------------------------------------------------------------------------------------------------
#if defined(_GBC)
	//����ȣ::
	if(m_pRideUser)
		m_pRideUser->Set_MOTION( this->GetRideAniPos() + PETMODE_AVATAR_ANI_MOVE );
#endif
	//----------------------------------------------------------------------------------------------------


	m_iOldCartState = m_iCurrentCartState;
	m_iCurrentCartState = CART_STATE_MOVE;

	StopSound( m_iOldCartState );
	PlaySound( m_iCurrentCartState );
	return true;
}

void CObjCART::SetCMD_MOVE( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget)
{
	CObjAI::SetCMD_MOVE( wSrvDIST, PosTO, iServerTarget );

	//----------------------------------------------------------------------------------------------------
	/// �� ž���� �� ĳ������ ����� ��ü�Ѵ�.
	//----------------------------------------------------------------------------------------------------		
	m_pObjParent->Set_MOTION( this->GetRideAniPos() + PETMODE_AVATAR_ANI_MOVE );
	
	//----------------------------------------------------------------------------------------------------
	//����ȣ::
#if defined(_GBC)
	if(m_pRideUser)
		m_pRideUser->Set_MOTION( this->GetRideAniPos() + PETMODE_AVATAR_ANI_MOVE );
#endif 
	//----------------------------------------------------------------------------------------------------
		


	m_iOldCartState = m_iCurrentCartState;
	m_iCurrentCartState = CART_STATE_MOVE;

	StopSound( m_iOldCartState );
	PlaySound( m_iCurrentCartState );
}

bool CObjCART::SetCMD_STOP( void )
{
	CObjAI::SetCMD_STOP();

	//----------------------------------------------------------------------------------------------------
	/// �� ž���� �� ĳ������ ����� ��ü�Ѵ�.
	//----------------------------------------------------------------------------------------------------		
	m_pObjParent->Set_MOTION( this->GetRideAniPos() + PETMODE_AVATAR_ANI_STOP1 );
	
	//----------------------------------------------------------------------------------------------------
#if defined(_GBC)
	//����ȣ::
	if(m_pRideUser)
		m_pRideUser->Set_MOTION( this->GetRideAniPos() + PETMODE_AVATAR_ANI_STOP1 );
#endif 
	//----------------------------------------------------------------------------------------------------


	m_iOldCartState = m_iCurrentCartState;
	m_iCurrentCartState = CART_STATE_STOP;

	StopSound( m_iOldCartState );
	PlaySound( m_iCurrentCartState );	
	return true;
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief /*override*/virtual bool	SetCMD_ATTACK ( int iServerTarget ); �� ������ �ʿ����.
///			CObjAI::SetCMD_ATTACK �� ���������� ȣ���ϴ� �Լ��̴�.
//----------------------------------------------------------------------------------------------------

void CObjCART::SetCMD_ATTACK (int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosGOTO)
{
	CObjCHAR::SetCMD_ATTACK( iServerTarget, wSrvDIST, PosGOTO );

	//----------------------------------------------------------------------------------------------------
	/// �� ž���� �� ĳ������ ����� ��ü�Ѵ�.
	//----------------------------------------------------------------------------------------------------	
	
	//----------------------------------------------------------------------------------------------------
#if defined(_GBC)
	m_pObjParent->Set_MOTION( this->GetRideAniPos() + PETMODE_AVATAR_ANI_STOP1 );
	//����ȣ::
	if(m_pRideUser)
		m_pRideUser->Set_MOTION( this->GetRideAniPos() + PETMODE_AVATAR_ANI_STOP1 );
#else 
	m_pObjParent->Set_MOTION( this->GetRideAniPos() + PETMODE_AVATAR_ANI_ATTACK01 + RANDOM(3) );
#endif 
	//----------------------------------------------------------------------------------------------------


	m_iOldCartState = m_iCurrentCartState;
	m_iCurrentCartState = CART_STATE_ATTACK;

	StopSound( m_iOldCartState );
	PlaySound( m_iCurrentCartState );
}




//----------------------------------------------------------------------------------------------------	
/// @brief īƮ�迭�� LIST_PAT ���� ���ݰ��� �����ʹ� ���´�.
///        Move Speed ���� ���� ſ�ٴ� �����Ͽ� ����� �θ𿡰� ����� �ȴ�. �ᱹ �θ��� ���ǵ带 �̿��ϸ�ȴ�.
///			�̴� �θ��� SpeedUpdate �� �Ҷ� �� ž�¿��ΰ� ����Ǿ ���Ǳ� �����̴�.
///			�׷��� Attack Speed ������ Ʋ����. �̴� ���̺��� ���;��Ѵ�.( �ƹ�Ÿ�� ���⿡ �����ϰ� ��Ÿ���..
//----------------------------------------------------------------------------------------------------

float CObjCART::Get_MoveSPEED ()
{	
	return m_fAdjustSPEED;		
}

float CObjCART::Get_DefaultSPEED ()		
{	
	return ( ( m_pObjParent )? m_pObjParent->GetOri_RunSPEED(): 10 );
}

short CObjCART::Get_nAttackSPEED ()	
{ 
	return ( m_pObjParent )? m_pObjParent->GetOri_ATKSPEED() : 10; 
}

float CObjCART::Get_fAttackSPEED ()	
{ 
	return (float)Get_nAttackSPEED() / 100.0f; 
}
int	 CObjCART::Get_AttackRange ()	
{

#if defined(_GBC)
	return (int) ( PAT_ITEM_ATK_RANGE( m_nWeaponIDX ) + ( Get_SCALE() * 120 ) );	
#else
	return (int) ( PAT_ITEM_ATK_RANGE( m_nAbilIDX ) + ( Get_SCALE() * 120 ) );	
#endif 

}


/// �浹�� �ʿ��� ��ġ������ �����Ѵ�.
void CObjCART::CopyCollisionInformation( bool bRiding )
{
	/// Parent �� ������� �θ� �����δ� �ƹ�Ÿ�� īƮ�� ��ũ �Ǿ� �ִ�.
	/// ���⼭ Parent �� �ƹ�Ÿ
	CObjCHAR_Collision* pCollision = this->GetCollisionUtility();
	CObjCHAR_Collision* pParentCollision = m_pObjParent->GetCollisionUtility();
 
	/// Ż���� �ƹ�Ÿ�� ������ īƮ�� ����  
	if( bRiding )
	{
		if( m_pObjParent )
		{
			if( pCollision && pParentCollision )
			{
				pCollision->m_vPrevious = pParentCollision->m_vPrevious;
				pCollision->m_vCurrent	= pParentCollision->m_vCurrent;
				pCollision->m_vNext		= pParentCollision->m_vNext;
			}
		}
	}else
	{
		/// �������� īƮ�� ������ �ƹ�Ÿ�� ����
		if( m_pObjParent )
		{
			if( pCollision && pParentCollision )
			{
				pParentCollision->m_vPrevious	= pCollision->m_vPrevious;
				pParentCollision->m_vPrevious.z	= pCollision->m_vPrevious.z + 500.0f;
				pParentCollision->m_vCurrent	= pCollision->m_vCurrent;				
				pParentCollision->m_vNext		= pCollision->m_vNext;
				
			}
		}
	}
}


///����ȣ::īƮ�� �� ��Ʈ�� �������� �����Ѵ�. 
void CObjCART::SetCartPartVisible(float fv)
{

	CMODEL<CCharPART> *pCharPART = NULL;
	short iPart = 0;

	for(register int i=0;i<MAX_RIDING_PART;i++)
	{
		iPart = m_nPartItemIDX[i];

		if(!iPart)
			continue;

		pCharPART = m_pCharMODEL->GetCharPART(iPart);
		
		if(pCharPART == NULL)
			continue;
		 
		for(register int j=0;j<pCharPART->m_nPartCNT;j++)
		{ ::setVisibilityRecursive(m_phPartVIS[iPart][j],fv); }
		
	}

}




