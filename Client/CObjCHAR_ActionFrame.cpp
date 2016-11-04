#include "stdAFX.h"
#include "Game.h"
#include "OBJECT.h"
#include "BULLET.h"
#include "IO_Terrain.h"
#include "IO_Event.h"
#include "CViewMSG.h"
#include "Network\CNetwork.h"
#include "common\Calculation.h"
#include "Game_FUNC.h"
#include "Interface/it_Mgr.h"
#include "Interface/CUIMediator.h"
#include "Interface/TypeResource.h"
#include "Interface/Dlgs/ChattingDlg.h"
#include "GameCommon/Skill.h"
#include "Event/Quest_FUNC.h"
#include "Common/IO_PAT.h"
#include "Country.h"



void CObjCHAR::FireBulletWithPetWeapon( CObjCHAR* pTarget )
{
	if( this->IsPET() )
	{

		int iBulletNO = PAT_BULLET( ((CObjCART*)this)->GetPetParts( RIDE_PART_ARMS ) );


		if ( pTarget && iBulletNO ) 
		{
			//����ȣ::�߻� �����͸� �����´�. 
			///<- 2005/7/25 īƮ �ý��� ���� �߰��� ���� : nAvy
			/// int	btPoint = PAT_BULLET_FIRE_POINT(((CObjCART*)this)->GetPetParts(RIDE_PART_ARMS),64);
			int btPoint = 0;

			btPoint = PAT_BULLET_FIRE_POINT(((CObjCART*)this)->GetPetParts(RIDE_PART_ARMS),64);

#ifdef _GBC
			g_pBltMGR->Add_BULLET( this, pTarget, iBulletNO, true, btPoint );
#else
			g_pBltMGR->Add_BULLET( this, pTarget, iBulletNO, true, 8 );
#endif			
		}
	}
}



//-------------------------------------------------------------------------------------------------
/// Ÿ��ȿ���� ���̰�, 
/// @todo Ÿ��ġ�� �����κ����� ���ۿ� �����Ѵ�.. �����
void CObjCHAR::ActionEVENT (int iActionIDX)
{
	// ����� �׼� �����ӿ� ����Ÿ�� ���� ��� 
	// ������ �־� �������� �׼��� �߻��Ѵ�. 
	if ( this->Get_HP() <= 0 )
		return;

	/// ���� �������� �̺�Ʈ ����..
	switch( iActionIDX ) 
	{
		/// Ư�� ������...
	case 1:
	case 2:
	case 3:

	case 91:
		{
			ActionSpecialFrame( iActionIDX );
		}
		break;

		/// ����		
	case 21:
	case 31:
	case 41:
		{
			// ���� !
			ActionInFighting( iActionIDX );
		}
		break;

		/// Ȱ		
	case 22:
	case 32:
	case 42:
		{
			// Ȱ���� !
			ActionBow( iActionIDX );
		}
		break;

		/// ��		
	case 23:
	case 33:
	case 43:
		{
			// �Ѱ��� !
			ActionGun( iActionIDX );
		}
		break;

		/// ��ų		
	case 24:
	case 34:	
		{
			// ��ų���� !
			ActionSkill( iActionIDX );
		}
		break;

		/// ��ų ������ Ÿ�� ��ų	
	case 25:
	case 35:	
	case 10:
	case 20:
	case 56:
	case 66:

		/// �޼� ���� �Ѿ˺���
	case 26:	
		{
			// ��ų���� !
			ActionImmediateSkill( iActionIDX );
		}
		break;

	case 44:
	case 64:
	case 74:	
	case 84:
		{
			ShowCastingEffect( iActionIDX );
		}

	default :
		//LogString (LOG_DEBUG, "Invalid Action event[ %d ] in %s \n", iActionIDX, Get_NAME() );
		break;
	}

	/*
	/// �ѹ� ������ ����
	if ( m_bAttackSTART )
	Attack_END( false );
	*/	
}

//////////////////////////////////////////////////////////////////////////
/// Ư�� �׼� ������
//////////////////////////////////////////////////////////////////////////
const int CHAR_STEP_SOLID = 653;
void CObjCHAR::ActionSpecialFrame( int iActionIDX )
{
	switch( iActionIDX )
	{
		/// ���ڱ� �Ҹ�
		case 1:
			{				
				if( this->IsA( OBJ_USER) )
				{
					D3DVECTOR vPos = g_pAVATAR->Get_CurPOS();

					CMAP_PATCH* pPatch = g_pTerrain->GetPatchFromWorld( vPos.x, vPos.y );
					if( pPatch )
					{
						int iTileNO = pPatch->GetUpTileNO();

						int iZoneNO = g_pTerrain->GetZoneNO();
						int iZoneType = ZONE_TYPE( iZoneNO );

						int iWalkSoundIDX = CHAR_STEP_SOLID;
						if( this->GetCollisionGroundNode() == NULL )
							iWalkSoundIDX = STEPSOUND( iZoneType, iTileNO );

						g_pSoundLIST->IDX_PlaySound3D( iWalkSoundIDX, this->Get_CurPOS() );				
					}

#ifdef _DEBUG
					if( g_GameDATA.m_bShowCurPos )
						g_pNet->Send_cli_DROP_ITEM( 0, (int)1 );
#endif
				}				
			}
			break;

		//----------------------------------------------------------------------------------------------------
		/// Castle Gear �� ���ڱ�.. �׸���.. ���ڱ� ����
		//----------------------------------------------------------------------------------------------------
		case 2:
		case 3:
			{
				if( this->IsPET() )
				{
					int iEffectIDX = PAT_MOVE_EFFECT( ((CObjCART*)this)->GetPetParts( RIDE_PART_LEG ) );
					int iDummyIDX = 0;
					switch( iActionIDX )
					{
						/// 1�� ���̿� ����
						case 2:
							{
								iDummyIDX = 1;
							}
							break;
						/// 2�� ���̿� ����
						case 3:
							{
								iDummyIDX = 2;
							}
							break;
					}
					CEffect *pEffect = g_pEffectLIST->Add_EffectWithIDX( iEffectIDX, true );
					if ( pEffect ) 
					{
						this->LinkDummy( pEffect->GetZNODE(), iDummyIDX );
						this->AddExternalEffect( pEffect );
						pEffect->SetParentCHAR( this );

						pEffect->InsertToScene();
					}
					

					int iSoundIDX  = PAT_MOVE_SOUND( ((CObjCART*)this)->GetPetParts( RIDE_PART_LEG ) );
					if ( iSoundIDX )
					{		
						/*CObjCHAR* pParent = NULL;
						if( pParent )*/
						{
							D3DXVECTOR3 vCur = Get_CurPOS();
							g_pSoundLIST->IDX_PlaySound3D( iSoundIDX, vCur );
							Log_String( LOG_NORMAL, "Play sound[ %d ][ %d ][ %.2f, %.2f, %.2f ]\n", iDummyIDX, iSoundIDX, vCur.x, vCur.y, vCur.z );  
						}/*else
						{
							assert( 0 && "The Parent of cart is invalid" );
						}*/
					}
				}
			}
			break;

		/// resurrection ��ų ���
		case 91:
			{
				CObjCHAR *pTarget = g_pObjMGR->Get_CharOBJ( this->m_iActiveObject, false );

				if( pTarget )
				{
					::setModelBlinkCloseMode( pTarget->GetZMODEL(), false );

					///��Ƽ���ϰ�� �������� ���ο� HP�� �´�. �̰�� �ٽ� ���½�Ű�� �ʱ� ���� ����
					///2005/3/22-nAvy
					if( pTarget->Get_HP() <= 0 )
						pTarget->Set_HP( 1 );

					pTarget->SetCMD_STOP();

					int iBulletIDX = SKILL_BULLET_NO( m_nDoingSkillIDX );
					if ( iBulletIDX ) 
					{
						short nEffectFileIDX = EFFECT_BULLET_NORMAL( iBulletIDX );
						/// �������ݸ����� �Ѿ��� ��°� �ƴ϶� Ÿ�ٿ� ����Ʈ�� ���δ�.
						CEffect *pHitEFT = g_pEffectLIST->Add_EffectWithIDX( nEffectFileIDX, true );
						if ( pHitEFT ) 
						{								
							pHitEFT->LinkNODE ( pTarget->GetZMODEL() );
							pHitEFT->UnlinkVisibleWorld ();
														
							pHitEFT->InsertToScene ();
						}
					}

					if( pTarget->IsA( OBJ_USER ) )
					{
						g_itMGR.CloseDialog( DLG_TYPE_RESTART );
						g_itMGR.ChangeState( IT_MGR::STATE_NORMAL );

						pTarget->Set_HP( ( g_pAVATAR->Get_MaxHP() * 3 ) / 10 );

						/// ����ġ ����
						int iSkillRecoverEXPConst = SKILL_POWER( m_nDoingSkillIDX );						
						g_pAVATAR->Cancel_PenalEXP( iSkillRecoverEXPConst );

						
					}
				}
			}
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
/// ���� ���� �׼�
//////////////////////////////////////////////////////////////////////////
void CObjCHAR::ActionInFighting( int iActionIDX )
{
	CObjCHAR *pTarget = g_pObjMGR->Get_CharOBJ( this->m_iActiveObject, true );

	if ( pTarget ) 
	{	
		switch( iActionIDX )
		{
			/// Ÿ��ġ
		case 21:
			{
				int iEffectIDX = 0;

				//----------------------------------------------------------------------------------------------------				
				/// @brief �ָ�忡 ���� ����Ʈ ����( ���� Arms �� ������ ���� ȿ���� �����´�. )
				//----------------------------------------------------------------------------------------------------
				if( this->IsPET() == false  )
				{
					/// ���Ⱑ �������..					
					iEffectIDX = WEAPON_DEFAULT_EFFECT( this->Get_R_WEAPON() );

					/// ���Ⱑ �������..
					if( !this->Get_R_WEAPON() )
					{	
						if( this->IsA( OBJ_MOB ) )
							iEffectIDX = NPC_HAND_HIT_EFFECT( this->Get_CharNO() );
					}
				

					pTarget->Hitted( this, iEffectIDX, this->m_nActiveSkillIDX );					

					/// ����
					if( pTarget->IsUSER() == false )
					{
						/// Ÿ�ݻ��� ���
						short nCharIdx = pTarget->Get_CharNO();

						short nHitSound = WEAPON_ATK_HIT_SOUND( this->Get_R_WEAPON() );			
						short nHittedSound = HIT_SOUND( nHitSound, NPC_HIT_MATERIAL_TYPE( nCharIdx ) );
						if ( nHittedSound )
						{
							g_pSoundLIST->IDX_PlaySound3D( nHittedSound, pTarget->Get_CurPOS() );
						}
					}else // �ڱ� �ƹ�Ÿ�� Ÿ���� ���
					{
						/// Ÿ�ݻ��� ���
						short nHitSound = WEAPON_ATK_HIT_SOUND( this->Get_R_WEAPON() );			
						short nHittedSound = HIT_SOUND( nHitSound, 1 );
						if ( nHittedSound )
						{
							//g_pSoundLIST->IDX_PlaySound( nHittedSound ); // ��3D �� ���							
							g_pSoundLIST->IDX_PlaySound3D( nHittedSound, Get_CurPOS() );
						}
					}
				}
				else
				{
			 		///<- 2005/7/25 īƮ �ý��� ���� �߰��� ���� : nAvy
					///��ó���� ���Ƿ� ������ �ٲ� ���� �ٽ� ���� 2005/8/31
					iEffectIDX = PAT_HIT_EFFECT( ((CObjCART*)this)->GetPetParts( RIDE_PART_ARMS ) );					

					pTarget->Hitted( this, iEffectIDX, 0 );

					int iSoundIDX = 0;
			 		///<- 2005/7/25 īƮ �ý��� ���� �߰��� ���� : nAvy
					///��ó���� ���Ƿ� ������ �ٲ� ���� �ٽ� ���� 2005/8/31 navye
					iSoundIDX  = PAT_HIT_SOUND( ((CObjCART*)this)->GetPetParts( RIDE_PART_ARMS ) );
					//if( CCountry::GetSingleton().IsApplyBattleCart() )
					//	iSoundIDX  = PAT_HIT_SOUND( ((CObjCART*)this)->GetPetParts( RIDE_PART_ARMS ) );
					//else
					//	iSoundIDX  = PAT_HIT_SOUND( ((CObjCART*)this)->GetPetParts( RIDE_PART_ABIL ) );
					///->

					if ( iSoundIDX )
						g_pSoundLIST->IDX_PlaySound3D( iSoundIDX, Get_CurPOS() );

				}
			}
			break;

			/// ���� ����
		case 31:
			{
				/// ���� ���� ����			
				short nHitStartSound = WEAPON_ATK_START_SOUND( this->Get_R_WEAPON() );	

				if( !( this->Get_R_WEAPON() ) && this->IsA( OBJ_MOB ) )
				{
					nHitStartSound = NPC_ATTACK_SOUND( this->m_nCharIdx );
				}

				if ( nHitStartSound )
				{
					if( this->IsUSER() ) // �ڱ� �ƹ�Ÿ�� �����ϴ� ��쿡�� ��3D �� ���
						//g_pSoundLIST->IDX_PlaySound( nHitStartSound );
						g_pSoundLIST->IDX_PlaySound3D( nHitStartSound, Get_CurPOS() );
					else 
						g_pSoundLIST->IDX_PlaySound3D( nHitStartSound, Get_CurPOS() );
				}	
			}
			break;			
		}
	}			
}

/// Ȱ ���� �׼�
void CObjCHAR::ActionBow( int iActionIDX )
{
	CObjCHAR *pTarget = g_pObjMGR->Get_CharOBJ( this->m_iActiveObject, true );

	if ( pTarget ) 
	{	
		switch( iActionIDX )
		{			
			/// �߻�, ����
		case 22:
			{

				//----------------------------------------------------------------------------------------------------
				/// Pet �� ��°Ŷ�� ���� ������ �Ѿ��� ���.
				//----------------------------------------------------------------------------------------------------
				if( this->IsPET() )
				{
					FireBulletWithPetWeapon( pTarget );
					return;
				}

				// �߻�
				// ���� ���۽� ������ ���� �߻�...				
				int iBulletIDX = Get_BulletNO ();
				if ( pTarget && iBulletIDX ) 
				{
					g_pBltMGR->Add_BULLET( this, pTarget, iBulletIDX );
					if( IsA( OBJ_USER ))
						g_pAVATAR->SubActiveBulletQuantity();


				}

				//if( pTarget->IsUSER() == false )
				{
					/// Ÿ�ݻ��� ���
					short nFireSound = WEAPON_ATK_FIRE_SOUND( this->Get_R_WEAPON() );								
					if ( nFireSound )
					{
						if ( IsUSER() ) {
							//g_pSoundLIST->IDX_PlaySound( nFireSound );
							g_pSoundLIST->IDX_PlaySound3D( nFireSound, Get_CurPOS() );
						}
						else {
							g_pSoundLIST->IDX_PlaySound3D( nFireSound, Get_CurPOS() );
						}
					}
				}
			}
			break;		

			/// ���� ����
		case 32:
			{
				/// ���� ���� ����			
				short nHitStartSound = WEAPON_ATK_START_SOUND( this->Get_R_WEAPON() );					
				if ( nHitStartSound )
				{
					if ( IsUSER() ) 
						//g_pSoundLIST->IDX_PlaySound( nHitStartSound );
						g_pSoundLIST->IDX_PlaySound3D( nHitStartSound, Get_CurPOS() );
					else
						g_pSoundLIST->IDX_PlaySound3D( nHitStartSound, Get_CurPOS() );
				}	
			}
			break;			
		}
	}
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �� ���� �׼�
//----------------------------------------------------------------------------------------------------

void CObjCHAR::ActionGun( int iActionIDX )
{
	CObjCHAR *pTarget = g_pObjMGR->Get_CharOBJ( this->m_iActiveObject, true );

	if ( pTarget ) 
	{	
		switch( iActionIDX )
		{

			/// �߻�, ����
		case 23:
			{
				//----------------------------------------------------------------------------------------------------
				/// Pet �� ��°Ŷ�� ���� ������ �Ѿ��� ���.
				//----------------------------------------------------------------------------------------------------
				if( this->IsPET() )
				{
					FireBulletWithPetWeapon( pTarget );
					return;
				}



				// �߻�
				// ���� ���۽� ������ ���� �߻�...				
				int iBulletIDX = Get_BulletNO ();
				if ( pTarget && iBulletIDX ) 
				{
					g_pBltMGR->Add_BULLET( this, pTarget, iBulletIDX );

					if( IsA( OBJ_USER ))
						g_pAVATAR->SubActiveBulletQuantity();

				}


				//if( pTarget->IsUSER() == false )
				{
					/// �߻���� ���
					short nFireSound = WEAPON_ATK_FIRE_SOUND( this->Get_R_WEAPON() );								
					if ( nFireSound )
					{
						if ( IsUSER() ) 
							//g_pSoundLIST->IDX_PlaySound( nFireSound );
							g_pSoundLIST->IDX_PlaySound3D( nFireSound, Get_CurPOS() );
						else
							g_pSoundLIST->IDX_PlaySound3D( nFireSound, Get_CurPOS() );
					}
				}
			}
			break;		

			/// ���� ����
		case 33:
			{
				/// ���� ���� ����			
				short nHitStartSound = WEAPON_ATK_START_SOUND( this->Get_R_WEAPON() );					
				if ( nHitStartSound )
				{
					D3DXVECTOR3 Velocity = pTarget->Get_CurPOS() - Get_CurPOS(); // 1�� ���ȿ� �̵��ϴ� ����

					if ( IsUSER() ) 
						//g_pSoundLIST->IDX_PlaySound( nHitStartSound );
						g_pSoundLIST->IDX_PlaySound3D( nHitStartSound, Get_CurPOS(), &Velocity );
					else
						g_pSoundLIST->IDX_PlaySound3D( nHitStartSound, Get_CurPOS(), &Velocity );
				}	
			}
			break;			
		}
	}
}

/// ��ų ���� �׼�
void CObjCHAR::ActionSkill( int iActionIDX )
{
	CObjCHAR *pTarget = 0;

	switch( iActionIDX )
	{		
		/// �߻�, ����
	case 24:
		{
			int iSkillIdx = ( this->m_nActiveSkillIDX )? this->m_nActiveSkillIDX : this->m_nDoingSkillIDX;
			if( iSkillIdx == 0 )
				break;

			switch( SKILL_TYPE( iSkillIdx ) )
			{
				///// self bound
			case SKILL_ACTION_SELF_BOUND_DURATION:
			case SKILL_ACTION_SELF_BOUND:
			case SKILL_ACTION_SELF_STATE_DURATION:
			case SKILL_ACTION_SELF_DAMAGE:
				{
					ProcEffectedSkill();
					CSkillManager::ActionSkill( iSkillIdx, this, this );		
				}						
				break;

				///// Target bound
			case SKILL_ACTION_TARGET_BOUND_DURATION:
			case SKILL_ACTION_TARGET_BOUND:
			case SKILL_ACTION_TARGET_STATE_DURATION:
				{
					CObjCHAR *pTarget = g_pObjMGR->Get_ClientCharOBJ( this->m_iServerTarget, true );

					/// Ÿ���� ���� �׾��.. �����ؾߵȴ�..
					ProcEffectedSkill();
					if( pTarget )
					{							
						CSkillManager::ActionSkill( iSkillIdx, this, pTarget );		
					}
				}						
				break;



				/// Ÿ�ٴ�� ����
			case SKILL_ACTION_FIRE_BULLET:
				{
					CObjCHAR *pTarget = g_pObjMGR->Get_ClientCharOBJ( this->m_iServerTarget, true );
					if ( pTarget ) 
					{
						_ASSERT( this->m_iServerTarget == g_pObjMGR->Get_ServerObjectIndex( pTarget->Get_INDEX() ) );
					}

					ProcEffectedSkill();
					CSkillManager::ActionSkill( iSkillIdx, this, pTarget );		
				}
				break;
				/// ������� ����
			case SKILL_ACTION_AREA_TARGET:
				{
					CObjCHAR *pTarget = g_pObjMGR->Get_ClientCharOBJ( this->m_iServerTarget, true );
					CSkillManager::ActionSkill( iSkillIdx, this, pTarget, this->m_PosGOTO, true );		
					ProcEffectedSkill();
				}
				break;

			case SKILL_ACTION_SUMMON_PET:
				{
					SummonMob();
				}
				break;

			default:
				break;

			}							



			/// �߻� ���� ����			
			short nFireSound = SKILL_BULLET_FIRE_SOUND( iSkillIdx );					
			if ( nFireSound )
			{
				if ( IsUSER() ) 
					//g_pSoundLIST->IDX_PlaySound( nFireSound );
					g_pSoundLIST->IDX_PlaySound3D( nFireSound, Get_CurPOS() );
				else
					g_pSoundLIST->IDX_PlaySound3D( nFireSound, Get_CurPOS() );
			}

		}
		break;		


		/// Ÿ��( �߻� ) ����
	case 34:
		{
			int iSkillIdx = ( this->m_nActiveSkillIDX )? this->m_nActiveSkillIDX : this->m_nDoingSkillIDX;
			if( iSkillIdx == 0 )
				break;

			/// ���� ���� ����			
			short nHitStartSound = SKILL_BULLET_FIRE_SOUND( this->m_nActiveSkillIDX );					
			if ( nHitStartSound )
			{
				if ( IsUSER() ) 
					//g_pSoundLIST->IDX_PlaySound( nHitStartSound );
					g_pSoundLIST->IDX_PlaySound3D( nHitStartSound, Get_CurPOS() );
				else
					g_pSoundLIST->IDX_PlaySound3D( nHitStartSound, Get_CurPOS() );
			}	
			//CObjCHAR *pTarget = g_pObjMGR->Get_ClientCharOBJ( this->m_iServerTarget, true );

			///// Ÿ���� �ְ� ������ �ƴҰ��...
			//if( pTarget && 
			//	( SKILL_TYPE( this->m_nActiveSkillIDX ) != SKILL_ACTION_SELF_BOUND_DURATION &&
			//	SKILL_TYPE( this->m_nActiveSkillIDX ) != SKILL_ACTION_SELF_BOUND &&
			//	SKILL_TYPE( this->m_nActiveSkillIDX ) != SKILL_ACTION_SELF_STATE_DURATION ) )
			//{
			//	if( this->m_nActiveSkillIDX == 0 )
			//		break;

			//	int iEffectIDX = SKILL_HIT_EFFECT( this->m_nActiveSkillIDX );
			//	if ( iEffectIDX ) 
			//	{
			//		CEffect *pCastingEFT = g_pEffectLIST->Add_EffectWithIDX( iEffectIDX, true );
			//		if ( pCastingEFT ) 
			//		{
			//			if( SKILL_HIT_EFFECT_LINKED_POINT( this->m_nActiveSkillIDX ) == INVALID_DUMMY_POINT_NUM )
			//			{
			//				pCastingEFT->LinkNODE ( pTarget->GetZMODEL() );
			//			}else
			//			{
			//				pTarget->LinkDummy( pCastingEFT->GetZNODE(), SKILL_HIT_EFFECT_LINKED_POINT( this->m_nActiveSkillIDX ) );
			//			}						

			//			pCastingEFT->SetParentCHAR( pTarget );
			//			pTarget->AddExternalEffect( pCastingEFT );

			//			//pCastingEFT->UnlinkVisibleWorld ();
			//			pCastingEFT->InsertToScene ();
			//		}
			//	}
			//}
		}
		break;
	}	
}


void CObjCHAR::ShowCastingEffect( int iActionIDX )
{
	switch( iActionIDX )
	{
		/// ��ų ����Ʈ �ÿ�.
		/// ĳ���ý� �̺�Ʈ�� m_nActiveSkillIDX �� �ε��� �̿�..
		case 44:				
			{
				ActionSkillCastingFrame( 0 );
			}
			break;		

			/// ĳ���ý� �̺�Ʈ�� m_nActiveSkillIDX �� �ε��� �̿�..
		case 64:		
			{
				ActionSkillCastingFrame( 1 );
			}	
			break;

			/// ĳ���ý� �̺�Ʈ�� m_nActiveSkillIDX �� �ε��� �̿�..
		case 74:
			{
				ActionSkillCastingFrame( 2 );
			}
			break;
		case 84:
			{
				ActionSkillCastingFrame( 3 );
			}
			break;
	}
}

/// 44,64,75, 45,65,75 ó��..
void CObjCHAR::ActionSkillCastingFrame( int iCastingOrder )
{
	int iSkillIDX = ( this->m_nActiveSkillIDX )? this->m_nActiveSkillIDX : this->m_nDoingSkillIDX;
	if( iSkillIDX == 0 )
		return;

	int iEffectIDX = SKILL_CASTING_EFFECT( iSkillIDX, iCastingOrder );
	if ( iEffectIDX ) 
	{
		CEffect *pCastingEFT = g_pEffectLIST->Add_EffectWithIDX( iEffectIDX, true );
		if ( pCastingEFT ) 
		{			
			
			if( SKILL_CASTING_EFFECT_POINT( iSkillIDX, iCastingOrder ) == INVALID_DUMMY_POINT_NUM )
			{
				pCastingEFT->LinkNODE ( this->GetZMODEL() );
			}else
			{
				LinkDummy( pCastingEFT->GetZNODE(), SKILL_CASTING_EFFECT_POINT( iSkillIDX, iCastingOrder ) );
			}					


			/// �� ���� ���� ������ Ÿ���̰ų� ���� ��ų �̶��..
			if(	SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_SELF_DAMAGE ||
				SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_AREA_TARGET )
			{
				/// ��ų ���� �׼� �������� ĳ���� ����Ʈ�� ��츸..
				if( iCastingOrder == 2 || iCastingOrder == 3 )
				{
					/// ��ũ ���� �ʴ´�.
					pCastingEFT->UnlinkVisibleWorld ();
				}
			}else
			{
				pCastingEFT->SetParentCHAR( this );
				AddExternalEffect( pCastingEFT );
			}

			//pCastingEFT->UnlinkVisibleWorld ();
			pCastingEFT->InsertToScene ();
		}
	}

	/// Ÿ�� ���� ����			
	int iHitSound = SKILL_HIT_DUMMY_SOUND( iSkillIDX, iCastingOrder );					
	if ( iHitSound )
	{
		if ( IsUSER() ) 
			//g_pSoundLIST->IDX_PlaySound( nFireSound );
			g_pSoundLIST->IDX_PlaySound3D( iHitSound, Get_CurPOS() );
		else
			g_pSoundLIST->IDX_PlaySound3D( iHitSound, Get_CurPOS() );
	}		
}

/// ��Ÿ���� ȿ��( ����.. )
void CObjCHAR::RepeatedlyHitEffect( int iHitOrder )
{
	int iSkillIdx = ( this->m_nActiveSkillIDX )? this->m_nActiveSkillIDX : this->m_nDoingSkillIDX;
	if( iSkillIdx == 0 )
		return;

	CObjCHAR *pTarget = g_pObjMGR->Get_ClientCharOBJ( this->m_iServerTarget, true );
	if ( pTarget ) 
	{
		_ASSERT( this->m_iServerTarget == g_pObjMGR->Get_ServerObjectIndex( pTarget->Get_INDEX() ) );

		int iEffectIDX = SKILL_HIT_DUMMY_EFFECT( iSkillIdx, iHitOrder );
		if ( iEffectIDX ) 
		{
			CEffect *pDummyEFT = g_pEffectLIST->Add_EffectWithIDX( iEffectIDX, true );
			if ( pDummyEFT ) 
			{
				pDummyEFT->LinkNODE ( pTarget->GetZMODEL() );

				pDummyEFT->SetParentCHAR( pTarget );
				pTarget->AddExternalEffect( pDummyEFT );

				pDummyEFT->InsertToScene ();
			}
		}	
	}		

	/// Ÿ�� ���� ����			
	int iHitSound = SKILL_HIT_DUMMY_SOUND( iSkillIdx, iHitOrder );					
	if ( iHitSound )
	{
		if ( IsUSER() ) 
			//g_pSoundLIST->IDX_PlaySound( nFireSound );
			g_pSoundLIST->IDX_PlaySound3D( iHitSound, Get_CurPOS() );
		else
			g_pSoundLIST->IDX_PlaySound3D( iHitSound, Get_CurPOS() );
	}
}

void CObjCHAR::FireEffectBullet( CObjCHAR *pTarget, int iBulletIDX, bool bDummy, int iSkillIDX )
{	
	if ( pTarget && iBulletIDX ) 
	{
		if( !bDummy )
			g_pBltMGR->Add_BULLET( this, pTarget, iBulletIDX, ( iSkillIDX == 0 ), 0, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), false, bDummy, iSkillIDX );		
		else
		{
			g_pBltMGR->Add_BULLET( this, pTarget, iBulletIDX, true, 0, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), false, bDummy, iSkillIDX );			
		}
	}
}

/// ���� Ÿ�� ��ų ���� �׼�
void CObjCHAR::ActionImmediateSkill( int iActionIDX )
{
	CObjCHAR *pTarget = g_pObjMGR->Get_CharOBJ( this->m_iActiveObject, true );

	if( pTarget )
	{
		switch( iActionIDX )
		{

			/// �޼� ���� �Ѿ˺���
			/// 56, 66 �� �Ѿ˿� �������� ���� ���� �Ѿ��̶�°� �ִ´�.
		case 26:
			{
				int iSkillIdx = ( this->m_nActiveSkillIDX )? this->m_nActiveSkillIDX : this->m_nDoingSkillIDX;
				if( iSkillIdx == 0 )
					return;

				if( IsA( OBJ_USER ))
					g_pAVATAR->SubActiveBulletQuantity();

				FireEffectBullet( pTarget, SKILL_BULLET_NO( iSkillIdx ), false, iSkillIdx );
			}
			break;
		case 56:
		case 66:
			{
				int iSkillIdx = ( this->m_nActiveSkillIDX )? this->m_nActiveSkillIDX : this->m_nDoingSkillIDX;
				if( iSkillIdx == 0 )
					return;

				FireEffectBullet( pTarget, SKILL_BULLET_NO( iSkillIdx ), true, iSkillIdx );
			}
			break;
		

			/// ��Ÿ ���� ����Ʈ
		case 10:
			{
				RepeatedlyHitEffect( 0 );
			}
			break;
		case 20:
			{
				RepeatedlyHitEffect( 1 );
			}
			break;

			///Ÿ��
		case 25:
			{
				int iSkillIdx = ( this->m_nActiveSkillIDX )? this->m_nActiveSkillIDX : this->m_nDoingSkillIDX;
				if( iSkillIdx == 0 )
					return;

				CObjCHAR *pTarget = g_pObjMGR->Get_ClientCharOBJ( this->m_iServerTarget, true );
				if ( pTarget ) {
					_ASSERT( this->m_iServerTarget == g_pObjMGR->Get_ServerObjectIndex( pTarget->Get_INDEX() ) );
				}

				ProcEffectedSkill();
				CSkillManager::ActionSkill( iSkillIdx, this, pTarget );	


				///
				/// Ÿ��
				///

				int iEffectIDX;

				if( pTarget )
				{
					iEffectIDX = SKILL_HIT_EFFECT( iSkillIdx );
					if( iEffectIDX == 0 )
					{
						iEffectIDX = WEAPON_DEFAULT_EFFECT( this->Get_R_WEAPON() );

						/// ���Ⱑ �������..
						if( !this->Get_R_WEAPON() )
						{	
							if( this->IsA( OBJ_MOB ) )
								iEffectIDX = NPC_HAND_HIT_EFFECT( this->Get_CharNO() );
						}

						pTarget->Hitted( this, iEffectIDX, 0, false, false, true );
					}else
					{
						pTarget->Hitted( this, iEffectIDX, this->m_nActiveSkillIDX, true, false, true );
					}
				}

				/// �߻� ���� ����			
				short nHittedSound = SKILL_HIT_SOUND( iSkillIdx );					
				if ( nHittedSound )
				{
					if ( IsUSER() ) 
						//g_pSoundLIST->IDX_PlaySound( nHittedSound );
						g_pSoundLIST->IDX_PlaySound3D( nHittedSound, Get_CurPOS() );
					else
						g_pSoundLIST->IDX_PlaySound3D( nHittedSound, Get_CurPOS() );
				}

			}
			break;

			/// ���� ���� ����
		case 35:
			{
			}
			break;
		}
	}
}