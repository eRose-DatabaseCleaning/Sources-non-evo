/*
	$Header: /Client/GameCommon/Skill.cpp 71    05-10-26 2:56p Gioend $
*/
#include "stdAFX.h"
#include "Skill.h"
#include "JCommandState.h"	
#include "Network\CNetWork.h"	
#include "../Interface/SlotContainer/CSkillSlot.h"	
#include "../Interface/it_Mgr.h"	
#include "../Interface/IO_ImageRes.h"
#include "../Interface/Dlgs/ChattingDlg.h"
#include "../Interface/Dlgs/CPartyDlg.h"
#include "../Interface/CTDrawImpl.h"
#include "CSkillCommand.h"
#include "ReloadProcess.h"
#include "GameData/CParty.h"
#include "IO_Terrain.h"
#include "GameProc/SkillCommandDelay.h"

#include "../Country.h"
#include "../GameProc/UseItemDelay.h"

CSkillManager	g_SkillManager;




CSkill::CSkill()
{
	m_iSkillLevel		= 0;
	m_iSkillDelayTimer	= 0;

	m_iSkillSlot		= 0;
	m_pCommand			= NULL;

	m_iElapsedTime		= 0;

	m_dwExpiredTime		= 0;

	m_bEnable			= true;
}

CSkill::~CSkill()
{
	// Destruct
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief  ��ų �������ÿ��� UserData �� Skill ������ ������Ʈ �ϱ⶧����
///			SkillSlot �� ��ų �����鵵 ���ο� �������� �ʰ� ���� UserData �� Skill ������ �����Ѵ�.
//----------------------------------------------------------------------------------------------------

short CSkill::GetSkillIndex()
{
	if( m_iSkillSlot >= MAX_LEARNED_SKILL_CNT 
		|| m_iSkillSlot < 0  )
		return 0;

	short nSkillIDX = 0;
	
	switch( m_iSkillSlotType )
	{
	case SKILL_SLOT_MAIN:
		nSkillIDX = g_pAVATAR->m_Skills.m_nSkillINDEX[ m_iSkillSlot ];
		break;
	case SKILL_SLOT_CLAN:
		nSkillIDX = m_nClanSkillIndex;
		break;
	default:
		break;
	}	

	return nSkillIDX;
}
//----------------------------------------------------------------------------------------------------
/// @modify ���� STB���� �������°����� ���� nAvy 2004/6/30;
//----------------------------------------------------------------------------------------------------
short CSkill::GetSkillLevel()
{ 
	short nSkillIDX = GetSkillIndex();
	assert( nSkillIDX );
	return SKILL_LEVEL( nSkillIDX );
};

bool CSkill::Create( int iSkillType, short nSkillIdx, short nSkillType, short nSkillLevel )
{	
	m_iSkillSlotType	= iSkillType;

	m_nClanSkillIndex		= nSkillIdx;
	m_iSkillLevel		= nSkillLevel;
	m_iSkillDelayTimer	= 0;
	
	
	m_btSkillActionType = SKILL_TYPE( nSkillIdx );
	m_btSkillFilterType = SKILL_CLASS_FILTER( nSkillIdx );

	
	switch( m_btSkillActionType )
	{
		/// �⺻���( �ɱ�, ����, ����ǥ���� )
		case SKILL_BASE_ACTION:
			{
				m_pCommand = (CCommand*) g_QuickCommandPool[ COMMAND_COMMAND ];
			}
			break;
		case SKILL_EMOTION_ACTION:
			{
				m_pCommand = (CCommand*) g_QuickCommandPool[ EMOTION_COMMAND ];
			}
			break;		
		default:
			{				
				m_pCommand = (CCommand*) g_QuickCommandPool[ SKILL_COMMAND ];
			}
			break;
	}

	return true;
}

bool CSkill::Execute()
{
	if( m_pCommand == NULL )
		return false;

	m_pCommand->SetMember( m_iSkillSlot );
	return m_pCommand->Execute();
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief �� �뵵�� ��ų ������ Ÿ���� �����̴�.
//----------------------------------------------------------------------------------------------------

int CSkill::Process()
{	
	if( this->m_iSkillDelayTimer > 0 )
	{
		this->m_iSkillDelayTimer -= g_GameDATA.GetElapsedFrameTime();

		if( this->m_iSkillDelayTimer < 0 )
			this->m_iSkillDelayTimer = 0;
	}

	return 1;
}


void CSkill::SetSkillDelayTime( int iTime )
{ 
	m_iSkillDelayTimer = iTime; 
	m_iElapsedTime = 0;
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief draw icon..
//----------------------------------------------------------------------------------------------------
void CSkill::DrawIcon( int iX, int iY )
{
	
 	CTDrawImpl* pDrawObj = g_itMGR.GetDrawImplPtr();
	short nIconNo = SKILL_ICON_NO( GetSkillIndex() );

//-----------------------------------------------------------------------------------------------------
/////////2005. 5. 30.  �� ��ȣ 
	if(CCountry::GetSingleton().IsUseItemDelayNewVersion())
	{
		//��ų ������ Ÿ���� �����´�. 
		int iDelayType = SKILL_RELOAD_TYPE( GetSkillIndex() );
			
		float srcTick = 0.0f;
		float desTick = 0.0f;

		//�׷�Ÿ���̶�� ó�� ���� �Ǿ��� Ÿ���� ������ ƽ�� �����´�. 
		if(iDelayType)
		{
		   srcTick = g_CurSkillDelayTick.GetUseItemDelay(iDelayType);
		   desTick = g_UseSkillDelay.GetUseItemDelay( iDelayType );
		}
		//�ܵ����� �����Ǵ� ��ų�̶�� �ڽ��� ������ ƽ�� �����Ѵ�. 
		else
		{
			srcTick = g_SkillList.GetDelayTickCount( GetSkillIndex() );
		   desTick = g_SoloSkillDelayTick.GetUseItemDelay( GetSkillIndex() );
		}

		//���� ������ ������ ���̶��...
		if(desTick > 0.0f)	
		{
			float fProcessRate =  (desTick / srcTick);
				
			CReloadProcess* pReloadProcess = g_itMGR.GetReloadProcess();
			pReloadProcess->Draw( iX , iY, IMAGE_RES_SKILL_ICON, nIconNo, fProcessRate );	
		}
		else
		{
			if( IsEnable() )		
				pDrawObj->Draw( iX,	iY,	IMAGE_RES_SKILL_ICON, nIconNo ); 
			else
				pDrawObj->Draw( iX,	iY,	IMAGE_RES_SKILL_ICON, nIconNo ,D3DCOLOR_RGBA( 128, 128, 128, 255 )); 
		}
	}
//-------------------------------------------------------------------------------------------------------
	else
	{
		if( this->m_iSkillDelayTimer > 0 )
		{
			float fProcessRate = (float)m_iSkillDelayTimer / (float)( SKILL_RELOAD_TIME( this->GetSkillIndex() ) * 200 );

			CReloadProcess* pReloadProcess = g_itMGR.GetReloadProcess();
			pReloadProcess->Draw( iX , iY, IMAGE_RES_SKILL_ICON, nIconNo, fProcessRate );		
		}else
		{	pDrawObj->Draw( iX,	iY,	IMAGE_RES_SKILL_ICON, nIconNo ); 	}

		CSkillCommandDelay::GetSingleton().Draw( iX, iY );
	}
	
	
}	


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief get/set Skill slot
//----------------------------------------------------------------------------------------------------
void CSkill::SetSkillSlot( int iSlot )
{ 
	m_iSkillSlot = iSlot; 

	/// ��� Skill ��ü�� �ĸǵ� ��ü�� �����°��� �ƴ϶� �����Ѵ�.. �� ȣ��ø��� �ɹ� �����ض�..
	////if( m_pCommand )
	////	m_pCommand->SetMember( iSlot );
	////else
	////	assert( 0 && "CSkill has null pointer to command" );
}

bool CSkill::HasExpiredTime()
{
	short nSkillIndex = GetSkillIndex();
	if( SKILL_TAB_TYPE( nSkillIndex ) == 3 && SKILL_NEED_LEVELUPPOINT( nSkillIndex ) >= 1 )
		return true;

	return false;
}

//bool CSkill::HasLimitUseCount()
//{
//}
//
//bool CSkill::HasLimitClass()
//{
//}
DWORD CSkill::GetExpiredTime()
{
	return m_dwExpiredTime;
}

void CSkill::SetExpiredTime( DWORD dwAbsSec )
{
	m_dwExpiredTime   = dwAbsSec;
}
//int CSkill::GetLimitUseCount()
//{
//}
//
//int CSkill::GetLimitClass()
//{
//}
void CSkill::SetEnable()
{
	m_bEnable = true;
}
void CSkill::SetDisable()
{
	m_bEnable = false;
}
bool CSkill::IsEnable()
{
	return m_bEnable;
}


//////////////////////////////////////////////////////////////////////////////////////
/// Base action Skill
//////////////////////////////////////////////////////////////////////////////////////

CBaseActionSkill::CBaseActionSkill()
{
	// Construct
}

CBaseActionSkill::~CBaseActionSkill()
{
	// Destruct
}

/*
*	Skill Action interface
*/
void CBaseActionSkill::Action( CObjCHAR* pAttacker, CObjCHAR* pTarget )
{
	
}

void CBaseActionSkill::DrawIcon( int iX, int iY )
{
	CSkill::DrawIcon( iX, iY );
}

int CBaseActionSkill::Process()
{
	return CSkill::Process();
}

int CBaseActionSkill::SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO )
{
	return 0;
}











//////////////////////////////////////////////////////////////////////////////////////
/// Action create window Skill
//////////////////////////////////////////////////////////////////////////////////////

CCreateWindowActionSkill::CCreateWindowActionSkill()
{
	// Construct
}

CCreateWindowActionSkill::~CCreateWindowActionSkill()
{
	// Destruct
}

/*
*	Skill Action interface
*/
void CCreateWindowActionSkill::Action( CObjCHAR* pAttacker, CObjCHAR* pTarget )
{

}

void CCreateWindowActionSkill::DrawIcon( int iX, int iY )
{
	CSkill::DrawIcon( iX, iY );
}

int CCreateWindowActionSkill::Process()
{
	return CSkill::Process();
}

int CCreateWindowActionSkill::SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO )
{
	//g_itMGR.OpenMakeDlg( iSkillSlot );


	int iWindowType = SKILL_POWER(GetSkillIndex() );
	if( iWindowType >= 11 && iWindowType <= 39 )
		g_itMGR.OpenDialogBySkill( iSkillSlot, DLG_TYPE_MAKE );
	else if( iWindowType == 41 )
		g_itMGR.OpenDialogBySkill( iSkillSlot, DLG_TYPE_SEPARATE );
	else if( iWindowType == 42 )
		g_itMGR.OpenDialogBySkill( iSkillSlot, DLG_TYPE_UPGRADE );

	return 0;
}











//////////////////////////////////////////////////////////////////////////////////////
/// CImmediateActionSkill Skill
//////////////////////////////////////////////////////////////////////////////////////

CImmediateActionSkill::CImmediateActionSkill()
{
	// Construct
}

CImmediateActionSkill::~CImmediateActionSkill()
{
	// Destruct
}

/*
*	Skill Action interface
*/
void CImmediateActionSkill::Action( CObjCHAR* pAttacker, CObjCHAR* pTarget )
{

}

void CImmediateActionSkill::DrawIcon( int iX, int iY )
{
	CSkill::DrawIcon( iX, iY );
}

int CImmediateActionSkill::Process()
{

	return CSkill::Process();
}

int CImmediateActionSkill::SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO )
{
	g_pNet->Send_cli_TARGET_SKILL( iTarget, iSkillSlot );
	return 0;
}









//////////////////////////////////////////////////////////////////////////////////////
/// Fire Skill
//////////////////////////////////////////////////////////////////////////////////////

CFireSkill::CFireSkill()
{
	// Construct
}

CFireSkill::~CFireSkill()
{
	// Destruct
}

/*
*	Skill Action interface
*/
void CFireSkill::Action( CObjCHAR* pAttacker, CObjCHAR* pTarget )
{
	int iBulletIDX = SKILL_BULLET_NO( GetSkillIndex() );
	if ( pTarget && iBulletIDX ) 
	{
		g_pBltMGR->Add_BULLET( pAttacker, pTarget, iBulletIDX );

		/// Ÿ�ݻ��� ���

	}
}

void CFireSkill::DrawIcon( int iX, int iY )
{
	CSkill::DrawIcon( iX, iY );
}

int CFireSkill::Process()
{
	return CSkill::Process();
}

int CFireSkill::SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO )
{
	g_pNet->Send_cli_TARGET_SKILL( iTarget, iSkillSlot );

	return 0;
}



//////////////////////////////////////////////////////////////////////////////////////
/// Fire to position Skill
//////////////////////////////////////////////////////////////////////////////////////

CFireToPositionSkill::CFireToPositionSkill()
{
	// Construct
}

CFireToPositionSkill::~CFireToPositionSkill()
{
	// Destruct
}

/*
*	Skill Action interface
*/
void CFireToPositionSkill::Action( CObjCHAR* pAttacker, CObjCHAR* pTarget )
{
	int iBulletIDX = SKILL_BULLET_NO( GetSkillIndex() );
	if ( pTarget && iBulletIDX ) 
	{
		g_pBltMGR->Add_BULLET( pAttacker, pTarget, iBulletIDX );

		/// Ÿ�ݻ��� ���

	}
}

void CFireToPositionSkill::DrawIcon( int iX, int iY )
{
	CSkill::DrawIcon( iX, iY );
}

int CFireToPositionSkill::Process()
{
	return CSkill::Process();
}

int CFireToPositionSkill::SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO )
{
	g_pNet->Send_cli_POSITION_SKILL( PosTO, iSkillSlot );

	return 0;
}









//////////////////////////////////////////////////////////////////////////////////////
/// Self Bound Skill
//////////////////////////////////////////////////////////////////////////////////////

CSelfBoundSkill::CSelfBoundSkill()
{
	// Construct
}

CSelfBoundSkill::~CSelfBoundSkill()
{
	// Destruct
}

/*
*	Skill Action interface
*/
void CSelfBoundSkill::Action( CObjCHAR* pAttacker, CObjCHAR* pTarget )
{
}

void CSelfBoundSkill::DrawIcon( int iX, int iY )
{
	CSkill::DrawIcon( iX, iY );
}

int CSelfBoundSkill::Process()
{
	return CSkill::Process();
}

int CSelfBoundSkill::SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO )
{
	g_pNet->Send_cli_SELF_SKILL ( iSkillSlot );
	return 0;
}




//////////////////////////////////////////////////////////////////////////////////////
/// Target Bound Skill
//////////////////////////////////////////////////////////////////////////////////////

CTargetBoundSkill::CTargetBoundSkill()
{
	// Construct
}

CTargetBoundSkill::~CTargetBoundSkill()
{
	// Destruct
}

/*
*	Skill Action interface
*/
void CTargetBoundSkill::Action( CObjCHAR* pAttacker, CObjCHAR* pTarget )
{
}

void CTargetBoundSkill::DrawIcon( int iX, int iY )
{
	CSkill::DrawIcon( iX, iY );
}

int CTargetBoundSkill::Process()
{
	return CSkill::Process();
}

int CTargetBoundSkill::SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO )
{
	g_pNet->Send_cli_TARGET_SKILL ( iTarget, iSkillSlot );
	return 0;
}












//////////////////////////////////////////////////////////////////////////////////////
/// Skill Manager
//////////////////////////////////////////////////////////////////////////////////////

CSkillManager::CSkillManager()
{
	// Construct


	g_QuickCommandPool[ COMMAND_COMMAND ] = (CCommand*)new CBasicCommand();
	g_QuickCommandPool[ EMOTION_COMMAND ] = (CCommand*)new CEmotionCommand();
	g_QuickCommandPool[ SKILL_COMMAND ] = (CCommand*)new CSkillCommand();	

}

CSkillManager::~CSkillManager()
{
	// Destruct

	for( int i = 0; i < MAX_QUICK_COMMAND; i++ )
	{
		if( g_QuickCommandPool[ i ] != NULL )
		{
			delete g_QuickCommandPool[ i ];
			g_QuickCommandPool[ i ] = NULL;
		}
	}
}



CSkill* CSkillManager::CreateNewSkill( int iSkillSlotType, short nSkillIdx, char cSkillLevel )
{	
	CSkill* pSkill = NULL;
	int iSkillType = SKILL_TYPE( nSkillIdx );

	/// �Ŀ� ������ ���丮�� ��ü����..
	switch( iSkillType )
	{
		case SKILL_EMOTION_ACTION:
		case SKILL_BASE_ACTION:					///< �⺻ ���( �ɱ�, ����, �ݱ�... )
			{
				pSkill = new CBaseActionSkill();
				pSkill->Create( iSkillSlotType, nSkillIdx, iSkillType, cSkillLevel );
			}
			break;		
		case SKILL_CREATE_WINDOW:
			{
				pSkill = new CCreateWindowActionSkill();
				pSkill->Create( iSkillSlotType, nSkillIdx, iSkillType, cSkillLevel );
			}
			break;


		case SKILL_ACTION_IMMEDIATE:			///< ���� ��� �ߵ�.( ��� ��� ��ü )
			{
				pSkill = new CImmediateActionSkill();
				pSkill->Create( iSkillSlotType, nSkillIdx, iSkillType, cSkillLevel );
			}
			break;

		case SKILL_ACTION_ENFORCE_BULLET:
		case SKILL_ACTION_FIRE_BULLET:			///< �߻�.
			{
				pSkill = new CFireSkill();
				pSkill->Create( iSkillSlotType, nSkillIdx, iSkillType, cSkillLevel );
			}
			break;

		case SKILL_ACTION_AREA_TARGET:
			{
				pSkill = new CFireToPositionSkill();
				pSkill->Create( iSkillSlotType, nSkillIdx, iSkillType, cSkillLevel );
			}
			break;

		/// Self bound
		case SKILL_ACTION_SELF_BOUND_DURATION:	///< �ڽſ��� �ߵ� ���� ����.( ĳ���� �� ) �ɷ�ġ 
		case SKILL_ACTION_SELF_BOUND:			///< �ڽſ��� �ߵ�.( ĳ���� �� )
		case SKILL_ACTION_SELF_STATE_DURATION:	///< �ڽſ��� �ߵ� ���� ����.( ĳ���� �� ) ���°���
		case SKILL_ACTION_SELF_DAMAGE:
		case SKILL_ACTION_SUMMON_PET:
			{
				pSkill = new CSelfBoundSkill();
				pSkill->Create( iSkillSlotType, nSkillIdx, iSkillType, cSkillLevel );
			}
			break;

		/// Target bound
		case SKILL_ACTION_TARGET_BOUND_DURATION:	///< ��뿡�� �ߵ� ���� ����.( ĳ���� �� ) �ɷ�ġ 
		case SKILL_ACTION_TARGET_BOUND:				///< ��뿡�� �ߵ�.( ĳ���� �� )
		case SKILL_ACTION_TARGET_STATE_DURATION:	///< ��뿡�� �ߵ� ���� ����.( ĳ���� �� ) ���°���
		case SKILL_ACTION_SELF_AND_TARGET:			///< Ÿ�ٿ��� ������ �ִµ��ÿ� ������ ������ ��
		case SKILL_ACTION_RESURRECTION:
			{
				pSkill = new CTargetBoundSkill();
				pSkill->Create( iSkillSlotType, nSkillIdx, iSkillType, cSkillLevel );
			}
			break;
		
		case SKILL_ACTION_PASSIVE:	
			{
				pSkill = new CBaseActionSkill();
				pSkill->Create( iSkillSlotType, nSkillIdx, iSkillType, cSkillLevel );
			}			
			break;
	}

	return pSkill;
}


/// ��ų�� �׼� Ÿ�Կ� ���� Ÿ�� Ÿ���� ���Ѵ�.
/*static*/ int	CSkillManager::GetSkillTargetType( int iSkillIndex )
{
	switch( SKILL_TYPE( iSkillIndex ) )
	{
		case SKILL_BASE_ACTION:						///< �⺻ ���( �ɱ�, ����, �ݱ�... ), ����ǥ��		
		case SKILL_CREATE_WINDOW:					///< â����( ����, ��Ƽ, ���λ���... )	
		case SKILL_ACTION_SELF_BOUND_DURATION:		///< �ڽſ��� �ߵ� ���� ����.( ĳ���� �� ) �ɷ�ġ 
		case SKILL_ACTION_SELF_BOUND:				///< �ڽſ��� �ߵ� �ٷ� �� ����.( ĳ���� �� ) �ɷ�ġ
		case SKILL_ACTION_SELF_STATE_DURATION:		///< �ڽſ��� �ߵ� ���� ����.( ĳ���� �� ) ���°���
		case SKILL_ACTION_SUMMON_PET:				///< �� ��ȯ ��ų
		case SKILL_ACTION_PASSIVE:					///< �нú�..
		case SKILL_ACTION_SELF_DAMAGE:				///< �ڽ������� �������� �ִ� ��ų..( �̰� ������ �ȵǳ�? ) ���� ����..
		
			/// Ÿ���� �ʿ���� �͵��̴�.
			return SKILL_TARGET_NONE;

		case SKILL_ACTION_ENFORCE_WEAPON:			///< ������� ����( ��ȭ, ȿ�� ����( ����ź? ) )
			/// �̰��� ��ȣ�ϱ�..
			return SKILL_TARGET_OBJECT;

		case SKILL_ACTION_IMMEDIATE:				///< ���� ��� �ߵ�.( ��� ��� ��ü )				
			return SKILL_TARGET_OBJECT;

		case SKILL_ACTION_ENFORCE_BULLET:			///< ��ȭ�Ѿ� ���� �߻�. ( ���̽� �ַο�.. ���� ȭ���� ���ϴ�.. )		
		case SKILL_ACTION_FIRE_BULLET:				///< �߻�.( ���̾ )		
		case SKILL_ACTION_TARGET_BOUND_DURATION:	///< Ÿ�ٿ��� �ߵ� ���� ����.( ĳ���� �� ) �ɷ�ġ
		case SKILL_ACTION_TARGET_BOUND:				///< Ÿ�ٿ��� �ߵ� �ٷ� �� ����.( ĳ���� �� ) �ɷ�ġ
		case SKILL_ACTION_TARGET_STATE_DURATION:	///< �ڽſ��� �ߵ� ���� ����.( ĳ���� �� ) ���°���
		case SKILL_ACTION_RESURRECTION:
			return SKILL_TARGET_OBJECT;
		
		case SKILL_ACTION_AREA_TARGET:				///< ���� ���ݸ���( ��� ����.. )!!! �㳪 ������ UI������ Ÿ�� ������Ʈ�� ��ġ�� ����..
			return SKILL_TARGET_OBJECT;
	}

	return SKILL_TARGET_NONE;
}

///
/// @todo ������ �޼����� ������ ������.. <= ����� �������� ����� �ϴ޹���.. �׷��� ���� �ʿ����..
/// ���� ���� �׼��� �� ��ų Ŭ���� ��ü����..
bool CSkillManager::ActionSkill( short nSkillIdx, CObjCHAR* pSrc, CObjCHAR* pTarget, D3DXVECTOR3 PosTo, bool bTargetPos )
{
	/// ���� ��Ŀ� ����...
	int iSkillActionType = GetSkillActionType( nSkillIdx );

	switch( iSkillActionType )
	{
		case SKILL_BASE_ACTION:					///< �⺻ ���( �ɱ�, ����, �ݱ�... )
			break;

		case SKILL_CREATE_WINDOW:			
			break;

		case SKILL_ACTION_IMMEDIATE:
			break;

		case SKILL_ACTION_FIRE_BULLET:
			{
				int iBulletIDX = SKILL_BULLET_NO( nSkillIdx );
				if ( pTarget && iBulletIDX ) 
				{
					CBullet* pBullet = g_pBltMGR->Add_BULLET( pSrc, pTarget, iBulletIDX, true, SKILL_BULLET_LINKED_POINT( nSkillIdx ) );
					if( pBullet )
						pBullet->SetSkillIDX( nSkillIdx );
					
					/// Ÿ�ݻ��� ���

				}
			}
			break;

		/// �������� ����
		case SKILL_ACTION_AREA_TARGET:
			{
				if( bTargetPos )
				{
					int iBulletIDX = SKILL_BULLET_NO( nSkillIdx );
					if ( iBulletIDX ) 
					{
						short nEffectFileIDX = EFFECT_BULLET_NORMAL( iBulletIDX );
						/// �������ݸ����� �Ѿ��� ��°� �ƴ϶� Ÿ�ٿ� ����Ʈ�� ���δ�.
						CEffect *pHitEFT = g_pEffectLIST->Add_EffectWithIDX( nEffectFileIDX, true );
						if ( pHitEFT ) 
						{	
							/*if( pTarget )
							{
								pHitEFT->LinkNODE ( pTarget->GetZMODEL() );
								pHitEFT->UnlinkVisibleWorld ();
							}else*/
							{
								pHitEFT->SetPOSITION( PosTo );
							}
							
							pHitEFT->InsertToScene ();
						}
					}
				}
			}
			break;

		///< �ڽſ��� �ߵ�.( ĳ���� �� )
		case SKILL_ACTION_SELF_BOUND_DURATION:
		case SKILL_ACTION_SELF_BOUND:
		case SKILL_ACTION_SELF_STATE_DURATION:
		case SKILL_ACTION_SELF_DAMAGE:
			{
				/// Self ��ų�� ��쿡�� �Ѿ�ȿ��, Ÿ��ȿ���� ��� ������ �ٿ��ش�.

				/// �Ѿ� ȿ��..
				int iShotEffectIDX = SKILL_BULLET_NO( nSkillIdx );
				int iEffectIDX = EFFECT_BULLET_NORMAL( iShotEffectIDX );

				CEffect *pEffect = g_pEffectLIST->Add_EffectWithIDX( iEffectIDX, true );				
				if( pEffect )
				{
					pSrc->LinkDummy( pEffect->GetZNODE(), SKILL_BULLET_LINKED_POINT( nSkillIdx) );

					pEffect->SetParentCHAR( pSrc );
					pSrc->AddExternalEffect( pEffect );

					pEffect->InsertToScene ();
				}

				/// Ÿ�� ȿ��..				
				int iHitEffectIDX = SKILL_HIT_EFFECT( nSkillIdx );

				pEffect = g_pEffectLIST->Add_EffectWithIDX( iHitEffectIDX, true );				
				if( pEffect )
				{
					pSrc->LinkDummy( pEffect->GetZNODE(), SKILL_HIT_EFFECT_LINKED_POINT( nSkillIdx) );

					pEffect->SetParentCHAR( pSrc );
					pSrc->AddExternalEffect( pEffect );

					pEffect->InsertToScene ();
				}
			}
			break;	

			///< �ڽſ��� �ߵ�.( ĳ���� �� )
		case SKILL_ACTION_TARGET_BOUND_DURATION:
		case SKILL_ACTION_TARGET_BOUND:
		case SKILL_ACTION_TARGET_STATE_DURATION:
		case SKILL_ACTION_RESURRECTION:
			{
				int iBulletIDX = SKILL_BULLET_NO( nSkillIdx );
				if ( pTarget && iBulletIDX ) 
				{					
					CBullet* pBullet = g_pBltMGR->Add_BULLET( pSrc, pTarget, iBulletIDX, true, SKILL_BULLET_LINKED_POINT( nSkillIdx ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), false, true, nSkillIdx );
					if( pBullet )
						pBullet->SetSkillIDX( nSkillIdx );
					
					/// Ÿ�ݻ��� ���

				}

				/*int iShotEffectIDX = SKILL_BULLET_NO( nSkillIdx );
				int iEffectIDX = EFFECT_HITTED_NORMAL( iShotEffectIDX );

				CEffect *pEffect = g_pEffectLIST->Add_EffectWithIDX( iEffectIDX, true );
				if( pEffect )
				{
					pSrc->LinkDummy( pEffect->GetZNODE(), SKILL_BULLET_LINKED_POINT( nSkillIdx) );

					pEffect->SetParentCHAR( pSrc );
					pSrc->AddExternalEffect( pEffect );

					pEffect->InsertToScene ();
				}*/
			}
			break;
	}

	return true;
}

/// ��ų �ߵ� ���� üũ..
bool CSkillManager::CheckConditionForFireSkill( int iSkillIDX, int iCurrentTarget )
{
	/// ��ų ��� ����( Ÿ���� ) üũ..
	if( CSkillManager::CheckCastingTarget( iSkillIDX, g_pAVATAR, iCurrentTarget ) == false )
	{
		g_itMGR.AppendChatMsg( CStr::Printf("%s[ %d ]", STR_NOTIFY_05, CSkillManager::GetSkillTargetType( iSkillIDX ) ), IT_MGR::CHAT_TYPE_SYSTEM );
		return false;
	}

	/// Ÿ�� ���͸� �˻�	
	if( CSkillManager::CheckCastingTargetFilter( iSkillIDX, g_pAVATAR, iCurrentTarget ) == false )
	{
		g_itMGR.AppendChatMsg( CStr::Printf("%s[ %d ]", STR_NOTIFY_05, CSkillManager::GetSkillTargetType( iSkillIDX ) ), IT_MGR::CHAT_TYPE_SYSTEM );
		return false;
	}

	/// ��ȯ�� ���� üũ
	if( CSkillManager::CheckSummonCapacity( iSkillIDX, g_pAVATAR, iCurrentTarget ) == false )
	{	
		g_itMGR.AppendChatMsg( STR_CANT_SUMMON_NPC, IT_MGR::CHAT_TYPE_SYSTEM);
		return false;	
	}


	/// ��ų ��� ���� üũ..
	if( CSkillManager::CheckCastingCondition( iSkillIDX, g_pAVATAR, iCurrentTarget ) == false )
	{
		g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_NOTIFY_06 ),IT_MGR::CHAT_TYPE_SYSTEM);
		return false;
	}

	
	/// �ߺ� ��ų ���� ���� üũ..
	if( CSkillManager::CheckDuplicatable( iSkillIDX, g_pAVATAR, iCurrentTarget ) == false )
	{
		g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_NOTIFY_CANT_DUPLICATE ), IT_MGR::CHAT_TYPE_SYSTEM);
		return false;
	}
	
	return true;
}



///----------------------------------------------------------------------------------------------------
/// @param
/// @brief ��ų �ߵ�.. CSkill::Exec���� ȣ���Ѵ�. ���� ��ų �ߵ��� ������ ��û�ϰ� �ߵ��� ���� üũ���� �Ѵ�.
///----------------------------------------------------------------------------------------------------
bool CSkillManager::FireSkill( int iSkillSlotIDX, int iTargetObj, D3DXVECTOR3 &PosTO )
{

	/// ���� ��� �����̸� ���Ұ�
	if( g_pAVATAR->m_EndurancePack.GetStateValue( ING_DUMB ) )
	{
		///AddMsgToChatWND( STR_DOING_SKILL_ACTION,  g_dwRED ,CChatDLG::CHAT_TYPE_SYSTEM);
		return false;
	}

	/// ���� ��ų �õ� ���϶��� ��ų ���Ұ�..	
	if( g_pAVATAR->CanApplyCommand() == false )
	{
		g_itMGR.AppendChatMsg( STR_DOING_SKILL_ACTION,  IT_MGR::CHAT_TYPE_SYSTEM);
		return false;
	}

	if( iSkillSlotIDX < 0 )
		return false;

	/// get skill.
	CSkillSlot* pSkillSlot = g_pAVATAR->GetSkillSlot();
	CSkill* pSkill = pSkillSlot->GetSkill( iSkillSlotIDX );

	if( pSkill == NULL )
		return false;
	
	//----------------------------------------------------------------------------------------------------
	/// ���� ����Ϸ��� ��ų�� ��û�� ��ų�� �������̶�� ����..( ��Ÿ.. ���� )
	//----------------------------------------------------------------------------------------------------	
	if( g_pAVATAR->m_nToDoSkillIDX == pSkill->GetSkillIndex() )
	{
		g_itMGR.AppendChatMsg( STR_DOING_SKILL_ACTION,  IT_MGR::CHAT_TYPE_SYSTEM);
	}

	//----------------------------------------------------------------------------------------------------
	/// PVP �������̶�� ��ȯ�� ��ȯ����
	/// 204/12/6   
	if( ZONE_PVP_STATE( g_pTerrain->GetZoneNO() ) == 2 )
	{
		/// ��ȯ�� ��ȣ�� ���õ� ��ų�� ��� ����
		if( SKILL_SUMMON_PET( pSkill->GetSkillIndex() ) )
		{
			g_itMGR.AppendChatMsg( STR_CANT_SUMMON_IN_PVPZONE,  IT_MGR::CHAT_TYPE_SYSTEM);
			return false;
		}
	}
	//----------------------------------------------------------------------------------------------------

	int iSkillType = CSkillManager::GetSkillActionType( pSkill->GetSkillIndex() );
	//----------------------------------------------------------------------------------------------------
	/// ����Ʈ��� ���ѵ� ���..
	/// 204/12/6   
	if( ZONE_PVP_STATE( g_pTerrain->GetZoneNO() ) == 11 )
	{
		if( iSkillType != 1 && iSkillType != 2 && iSkillType != 16 )
		{
			g_itMGR.AppendChatMsg( STR_CANT_SKILL_IN_AGIT,  IT_MGR::CHAT_TYPE_SYSTEM);
			return false;
		}
	}
	//----------------------------------------------------------------------------------------------------



	//----------------------------------------------------------------------------------------------------
	/// pet ž�½ÿ���..
	/// ����̺� ��ų(17����ų, �� Ÿ��/������), �¿��(25�� ��ų) ���� �ְ��� ��ų ������
	//----------------------------------------------------------------------------------------------------		
	
	//-----------------------------------------------------------------------------------------------------
	///����ȣ::�븸 ������ �ƴ϶��...
#ifndef _GBC
		if( g_pAVATAR->GetPetMode() >= 0 )
		{		
			switch( iSkillType )
			{
				case SKILL_BASE_ACTION:
					{
						int iSkillIdx = pSkill->GetSkillIndex();
						if( iSkillIdx == 17 ||
							iSkillIdx == 25 )
							break;
					}
					return false;

				default:
					g_itMGR.AppendChatMsg( STR_CANT_USESKILL_USING_DRIVESKILL,  IT_MGR::CHAT_TYPE_SYSTEM);
					return false;				
			}
		}
#endif
	//-----------------------------------------------------------------------------------------------------


	if( CSkillManager::GetSkillTargetType( iSkillType ) != SKILL_TARGET_NONE )
	{
		if ( iTargetObj ) 
		{			
			if( g_pObjMGR->m_pOBJECTS[ iTargetObj ] == NULL )
			{
				assert( 0 && "Fire skill[ target is invalid ]" );
				return false;
			}

			switch ( g_pObjMGR->m_pOBJECTS[ iTargetObj ]->Get_TYPE() ) 
			{
			case OBJ_MOB :
				g_UserInputSystem.SetCurrentTarget( iTargetObj );			
				g_pAVATAR->m_iServerTarget = g_pObjMGR->Get_ServerObjectIndex( iTargetObj );
				break;
			case OBJ_AVATAR:
				{
					//if( g_GameDATA.m_iPvPState == PVP_CANT )
					//{
					//	/// ������踦 �����ϴ� ��ų�� �ƹ�Ÿ���� �ɼ� ����.
					//	if( SKILL_HARM( pSkill->GetSkillIndex() ) != 0 )
					//		return false;
					//}				
				}			
				break;
			}
		}
	}
	
	int iCurrentTarget = g_UserInputSystem.GetCurrentTarget();		
	
	//Ÿ�Ͽ� ���� �Ǻ�
	if( CSkillManager::CheckConditionForFireSkill( pSkill->GetSkillIndex(), iCurrentTarget ) == false )
			return false;


	//------------------------------------------------------------------------------------------------
//2005 5. 30 �� ��ȣ
	//��ų ������ Ÿ�� üũ �� �缳�� ���� ������ ���̶�� ���� 
 	if(CCountry::GetSingleton().IsUseItemDelayNewVersion())
	{
		int iDelayType = SKILL_RELOAD_TYPE( pSkill->GetSkillIndex() );
		int iSkill	   = pSkill->GetSkillIndex();
		float iDelayTick = g_SkillList.GetDelayTickCount( iSkill );

		////0 �� �ƴϸ� �����̸� �����Ѵ�. 
		if( iDelayType )
		{
			//������ Ÿ���� �־ ���� Ÿ���� ������ ���̶�� ����Ҽ� ���ٴ� 
			//�޼����� ä��â�� ����Ѵ�. 
			if( g_UseSkillDelay.GetUseItemDelay( iDelayType ) > 0.0f)
			{
				///Use item delay �� �����Ǿ� �ִٸ� �н�..
				g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_NOTIFY_04 ), IT_MGR::CHAT_TYPE_SYSTEM );
				return false;
			}
		}
		//�����϶� �ܵ����� �����Ѵ�. 
		else
		{	
			//�ڱ� �ڽ��� ���� ���̶��....
			if( g_SoloSkillDelayTick.GetUseItemDelay( iSkill ) > 0.0f)
			{	
				g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_NOTIFY_04 ), IT_MGR::CHAT_TYPE_SYSTEM );
				return false;
			}
		} 
		
	} 
//------------------------------------------------------------------------------------------------
	else
	{
		/// ��ų Ÿ�̸Ӱ� ���� �ߴ°�? (�������� �ʾҴٸ� �޼��� â�� ���)
		if( CSkillManager::CheckSkillTimer( iSkillSlotIDX ) == false )
		{
			g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_NOTIFY_04 ), IT_MGR::CHAT_TYPE_SYSTEM );
			return false;
		}
		//if( CSkillManager::CheckConditionForFireSkill( pSkill->GetSkillIndex(), iCurrentTarget ) == false )
		//	return false;

		// �ϴ� ���⼭ ����..�Ҹ�ġ
		// ���⼭ ������ �ȵȴ�.. �������� �ź��Ұ��� ������ �ִ�..
		//CSkillManager::UpdateUseProperty( pSkill->GetSkillIndex() );

		//-----------------------------------------------------------------------------------------------
		/// �� ��ų�� �����̰� �ƴ϶� ��� ��ų ��ɵ鿡 ���� �����̰� ����ȴ�.
		/// ���⼭ ���� ������ ��ų ������� ������ �����̰� �������� üũ
		//-----------------------------------------------------------------------------------------------	
		if( CSkillCommandDelay::GetSingleton().CanCastSkill() == false )
		{
			g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_NOTIFY_04 ), IT_MGR::CHAT_TYPE_SYSTEM );
			return false;
		}else
		{
			/// ��ų �Է� �����̰� �������� ���� ����
			CSkillCommandDelay::GetSingleton().StartSkill();
		}
	}
 
	/// request skill action	
	pSkill->SendSkillActionReq( iSkillSlotIDX, iCurrentTarget, PosTO );
	

	return true;
}

/// ���� Ȱ��ȭ�� ��ų �ߵ�..
bool CSkillManager::ActionActiveSkill( int iTargetObj, D3DXVECTOR3 &PosTO )
{
	int iCurrentActiveSkillSlot = g_UserInputSystem.GetCurrentActiveSkillSlot();

	if( iCurrentActiveSkillSlot < 0 )
		return false;

	return FireSkill( iCurrentActiveSkillSlot, iTargetObj, PosTO );
}


/*static*/ void CSkillManager::UpdateUseProperty( CObjAI* pObjAI, int iSkillIdx )
{	
	/// ���� ��Ŀ� ����...
	int iSkillActionType = GetSkillActionType( iSkillIdx );

	/// â���� ��ų�� ���� ������ �ȱ�´�..
	if( iSkillActionType == SKILL_CREATE_WINDOW )
		return;


	/// �����ϰ���..
	if( pObjAI->IsA( OBJ_USER ) )
	{
		g_pAVATAR->Skill_UseAbilityValue( iSkillIdx );
		return;
	}


	short nCurValue = 0;
	for ( short nI=0; nI<SKILL_USE_PROPERTY_CNT; nI++) 
	{
		if ( 0 == SKILL_USE_PROPERTY( iSkillIdx, nI ) )
			break;

		//nCurValue = g_pAVATAR->Skill_GetAbilityValue( SKILL_USE_PROPERTY( iSkillIdx, nI ) );
		switch( SKILL_USE_PROPERTY( iSkillIdx, nI ) )
		{
			case AT_HP :
				pObjAI->Set_HP( pObjAI->Get_HP() - SKILL_USE_VALUE( iSkillIdx, nI ) );
				break;
			case AT_MP :
				pObjAI->Set_MP( pObjAI->Get_MP() - SKILL_USE_VALUE( iSkillIdx, nI ) );
				break;
		}
	}	
}


///
///	Skill condition check function ( static function )
///
/* static */ bool CSkillManager::CheckCastingCondition(int iSkillIDX,CObjCHAR* pCaster,int iTargetObjIDX)
{

	//����ȣ::��ų�� �´� ���� üũ ����
	if(!g_pAVATAR->Skill_ActionCondition( iSkillIDX ))
		return FALSE;

	/// ���� ��� ���� üũ( ��ų ��밡���� ��� �����ϰ� �ִ°�?
	if(CSkillManager::CheckNeedWeapon( iSkillIDX, pCaster ) == false )
	{
		g_itMGR.AppendChatMsg( STR_WEAPON_MISMATCH, g_dwRED );
		return false;
	}

	/// �����ʿ��� ������ �Լ��� ����Ѵ�.
	//return g_pAVATAR->Skill_ActionCondition( iSkillIDX );

	/*
	/// �Ҹ� ����, HP üũ..
	if( CSkillManager::CheckNeedProperty( iSkillIDX, pCaster ) == false )
	{
		g_itMGR.AppendChatMsg( STR_NOT_ENOUGH_MANA, g_dwRED );
		return false;
	}

	/// �ش� ��ų�� ���� Ÿ�� ��ȿ���� �����Ѵ�.
	if( CSkillManager::CheckCastingTarget( iSkillIDX, pCaster, iTargetObjIDX ) == false )
	{
		g_itMGR.AppendChatMsg( STR_INVALID_TARGET, g_dwRED );
		return false;
	}

	/// ���� ��� ���� üũ( ��ų ��밡���� ��� �����ϰ� �ִ°�?
	if( CSkillManager::CheckNeedWeapon( iSkillIDX, pCaster ) == false )
	{
		g_itMGR.AppendChatMsg( STR_WEAPON_MISMATCH, g_dwRED );
		return false;
	}
	*/

	return true;
}

/// Ÿ�̸� üũ( ���� ��뿩�ΰ��� )..
/// ������ üũ���� ���ǰ� �����Ͼ���???
/*static*/ bool CSkillManager::CheckSkillTimer( int iActiveSkillSlot )
{
	CSkillSlot* pSkillSlot = g_pAVATAR->GetSkillSlot();

	if( pSkillSlot->GetSkillDelayTime( iActiveSkillSlot ) != 0 )
		return false;

	return true;
}

/// @todo Ÿ���� HP�� üũ�ؾ��ϴ°�?
/// �翬�� ���������� ����� ������ �ؾߵɵ�
/*static*/ bool CSkillManager::CheckCastingTarget( int iSkillIDX, CObjCHAR* pCaster, int iTargetObjIDX )
{
	if( pCaster == NULL )
		return false;

	CObjCHAR *pTargetCHAR = g_pObjMGR->Get_CharOBJ( iTargetObjIDX, true );

	int iSkillTargetType = CSkillManager::GetSkillTargetType( iSkillIDX );

	switch( iSkillTargetType )
	{
	case SKILL_TARGET_NONE:			
		return true;

	case SKILL_TARGET_OBJECT:	
		{
			/// ��ų�� ���������̶�� HPüũ���� Ÿ���� ����.
			if( SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_RESURRECTION )
			{
				pTargetCHAR = g_pObjMGR->Get_CharOBJ( iTargetObjIDX, false );
			}

			if( pTargetCHAR == NULL )
				return false;
		}		
		return true;

	case SKILL_TARGET_GROUND:			
		return true;

	case SKILL_TARGET_PB:			
		return true;

	case SKILL_TARGET_SCREEN:			
		return true;

	case SKILL_TARGET_ZONE:			
		return true;

	case SKILL_TARGET_SERVER:						
		return true;

	default:
		break;
	}

	return false;
}

/// Ÿ�� ���͸� �˻�
bool CSkillManager::CheckCastingTargetFilter( int iSkillIDX, CObjCHAR* pCaster, int iTargetObjIDX )
{
	if( pCaster == NULL )
		return false;

	int iSkillTargetType = CSkillManager::GetSkillTargetType( iSkillIDX );

	switch( iSkillTargetType )
	{
		case SKILL_TARGET_NONE:			
			return true;
	}


	CObjCHAR *pTargetCHAR = g_pObjMGR->Get_CharOBJ( iTargetObjIDX, false );

	int iTargetFilterType = SKILL_CLASS_FILTER( iSkillIDX );

	switch( iTargetFilterType )
	{
		case SKILL_TARGET_FILTER_SELF:
			return true;

		case SKILL_TARGET_FILTER_GROUP:
			{	
				/// Ÿ���� �������.. Ÿ���� �츮���� �ƴϸ�.. false
				if( pTargetCHAR != NULL )
				{
					/// ��Ƽ���� �ƴϴ�.
					if( !CParty::GetInstance().IsPartyMember( g_pObjMGR->Get_ServerObjectIndex( iTargetObjIDX ) ))
					{				
						g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_SKILL_TARGET_FILTER_NOTIFY_01 ), IT_MGR::CHAT_TYPE_SYSTEM );
						return false;
					}
				}
			}
			break;
		case SKILL_TARGET_FILTER_GUILD:
			{
				//AddMsgToChatWND ( CStr::Printf("%s", STR_SKILL_TARGET_FILTER_NOTIFY_02, g_dwRED,CChatDLG::CHAT_TYPE_SYSTEM );
			}
			break;
		case SKILL_TARGET_FILTER_FRIEND_ALL:
			{
				/// Ÿ���� ������� ���� ������ �ƴҰ�� false
				if( pTargetCHAR != NULL )
				{
					if( CUserInputState::IsEnemy( pTargetCHAR ) )
					{
						g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_SKILL_TARGET_FILTER_NOTIFY_03 ), IT_MGR::CHAT_TYPE_SYSTEM );
						return false;
					}
				}
			}
			break;
		case SKILL_TARGET_FILTER_MOB:
			{
				if( pTargetCHAR == NULL )
					return false;

				if( !pTargetCHAR->IsA( OBJ_MOB ) )
				{
					g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_SKILL_TARGET_FILTER_NOTIFY_04 ), IT_MGR::CHAT_TYPE_SYSTEM );
					return false;
				}
			}
			break;
		case SKILL_TARGET_FILTER_ENEMY_ALL:
			{
				if( pTargetCHAR == NULL )
					return false;

				if( CUserInputState::IsEnemy( pTargetCHAR ) == false )
				{
					g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_SKILL_TARGET_FILTER_NOTIFY_05 ), IT_MGR::CHAT_TYPE_SYSTEM );
					return false;
				}
			}
			break;
		case SKILL_TARGET_FILTER_ENEMY_PC:
			{
				if( pTargetCHAR == NULL )
					return false;

				/// �����̰ų�.. �ƹ�Ÿ�� �ƴҰ���..
				if( CUserInputState::IsEnemy( pTargetCHAR ) == false || !pTargetCHAR->IsA( OBJ_AVATAR ) )
				{
					g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_SKILL_TARGET_FILTER_NOTIFY_05 ), IT_MGR::CHAT_TYPE_SYSTEM );
					return false;
				}
			}
			break;

		case SKILL_TARGET_FILTER_ALL_PC:
			{
				if( pTargetCHAR == NULL )
					return false;

				if( !pTargetCHAR->IsUSER( ) )
				{
					g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_SKILL_TARGET_FILTER_NOTIFY_06 ), IT_MGR::CHAT_TYPE_SYSTEM );
					return false;
				}
			}			
			break;
		case SKILL_TARGET_FILTER_ALL_CHAR:
			{
				if( pTargetCHAR == NULL )
					return false;

				if( !( pTargetCHAR->IsUSER() || pTargetCHAR->IsA( OBJ_MOB ) ) )
				{
					g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_SKILL_TARGET_FILTER_NOTIFY_07 ), IT_MGR::CHAT_TYPE_SYSTEM );
					return false;
				}
			}
			break;

		/// ���� ������ ���� �Ʊ���
		case SKILL_TARGET_FILTER_DEAD_USER:
			{
				/// Ÿ���� ������� ���� ������ �ƴҰ�� false
				if( pTargetCHAR != NULL )
				{
					if( CUserInputState::IsEnemy( pTargetCHAR ) )
					{
						g_itMGR.AppendChatMsg( CStr::Printf("%s", STR_SKILL_TARGET_FILTER_NOTIFY_03 ), IT_MGR::CHAT_TYPE_SYSTEM );
						return false;
					}
				}
			}
			break;

		default:
			break;
	}

	return true;
}

/// ��ȯ�� capacity �˻�
bool CSkillManager::CheckSummonCapacity( int iSkillIDX, CObjCHAR* pCaster, int iTargetObjIDX )
{
	if( SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_SUMMON_PET )
	{
		int iMobNO = SKILL_SUMMON_PET( iSkillIDX );
		if( iMobNO )
		{
			/// ��ȯ�� �ʰ�
			if( ( NPC_NEED_SUMMON_CNT( iMobNO ) + g_pAVATAR->GetCur_SummonUsedCapacity() ) > g_pAVATAR->GetCur_SummonMaxCapacity() )
				return false;
		}
	}

	return true;
}

/// �ʿ� �ɷ�ġ üũ..
bool CSkillManager::CheckNeedProperty( int iSkillIDX, CObjCHAR* pCaster )
{
	int iCurrentHP = pCaster->Get_HP();	
	int iCurrentMP = pCaster->Get_MP();

	for( int i = 0; i < SKILL_USE_PROPERTY_CNT; i++ )
	{
		switch( SKILL_USE_PROPERTY( iSkillIDX, i ) )
		{
			case AT_HP:
				{
					if( iCurrentHP < SKILL_USE_VALUE( iSkillIDX, i ) )
						return false;
				}
				break;

			case AT_MP:
				{
					if( iCurrentMP < SKILL_USE_VALUE( iSkillIDX, i ) )
						return false;
				}
				break;
		}		
	}

	return true;
}

/// �ʿ���� üũ..
/*static*/ bool CSkillManager::CheckNeedWeapon( int iSkillIDX, CObjCHAR* pCaster )
{
	bool bResult = true;

	int iCurrentRWeaponNO = pCaster->Get_RWeapon();	

	for( int i = 0; i < SKILL_NEED_WEAPON_CNT; i++ )
	{
		/// ���� ���Ⱑ �ִٸ� �ϴ�,false �� ����� �����ϸ� �ٷ� ����..
		if( SKILL_NEED_WEAPON( iSkillIDX, i ) )
		{
			bResult = false;
			if( SKILL_NEED_WEAPON( iSkillIDX, i ) == WEAPON_TYPE( iCurrentRWeaponNO ) )
				return true;
		}
	}

	return bResult;
}

/// �ߺ����� ���� üũ...
/*static*/ bool CSkillManager::CheckDuplicatable( int iSkillIDX, CObjCHAR* pCaster, int iTargetObjIDX )
{
	if( pCaster == NULL )
		return false;	

	/// ������ ��ų�� ���.. �ߺ����θ� üũ�Ѵ�.
	if( SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_SELF_BOUND_DURATION ||
		SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_TARGET_BOUND_DURATION ||
		SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_SELF_STATE_DURATION ||
		SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_TARGET_STATE_DURATION )
	{
		int		iStateSTB		= 0;
		int		iSkillState		= 0;
		DWORD	dwSkillStateFlag = 0;
		DWORD	dwState 		 = 0;

		int		iBoundStateSTB = 0;

		CObjCHAR *pTargetCHAR = NULL;
		
		/// ������ �Ŵ°Ŷ��..
		if( SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_SELF_BOUND_DURATION ||
			SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_SELF_STATE_DURATION )
		{
			pTargetCHAR = pCaster;	
		}else
		{
			pTargetCHAR = g_pObjMGR->Get_CharOBJ( iTargetObjIDX, false );	
		}

		if( pTargetCHAR == NULL )
			return false;

		for( int i = 0; i < 2; i++ )
		{			
			iStateSTB = SKILL_STATE_STB( iSkillIDX, i );
			if( iStateSTB == 0 )
				continue;

			iSkillState = STATE_TYPE( iStateSTB );
			dwSkillStateFlag = c_dwIngFLAG[ iSkillState ];

			/// ���� ���� ��ų�� �ߺ����¸� üũ���� ����	
			if( ( iSkillState == ING_CLEAR_GOOD ) ||
				( iSkillState == ING_CLEAR_BAD ) ||
				( iSkillState == ING_CLEAR_ALL ) )
			{
				continue;
			}

			dwState = pTargetCHAR->m_EndurancePack.GetStateFlag();			

			/// ���� �ɷ��� ��ųŸ���� �̹� �ɷ��ִ�.
			if( dwState & dwSkillStateFlag )
			{
				/// �ߺ��Ұ�..
				if( STATE_CAN_DUPLICATED( iStateSTB ) == 0 )
				{
					return false;
				}

				CEnduranceProperty* pEntity = pTargetCHAR->m_EndurancePack.GetEntityByStateType( iSkillState );

				/// ���� �ߵ��� ��ų�� ���� �ɸ� ��ų�� LIST_STATUS.STB ���� ��ȣ�� �������� �н�..
				if( pEntity && iStateSTB < pEntity->GetStateSTBNO() )
				{
					return false;
				}
			}
		}		
	}

	return true;
}


///
///	��ų ���氡�� ���� üũ..
///

/// ��ȿ�� �����ΰ�?
/// ��ﶧ �ʿ��ϴ�..
/*static*/ bool CSkillManager::CheckJobForStudy( int iSkillIDX )
{
	
	return false;
}

/// ��ų�� ���� ���� ���� ��ų�� �����ϴ°�?
/*static*/ bool CSkillManager::CheckProSkillForStudy( int iSkillIDX )
{
	bool bResult = true;	

	/*
	for( int i = 0; i < SKILL_NEED_SKILL_CNT; i++ )
	{
		if( SKILL_NEED_SKILL_INDEX( iSkillIDX, i ) )
		{
			if( SKILL_NEED_SKILL_INDEX( iSkillIDX, i ) );
				
		}
	}
	*/

	return bResult;	
}

/// ��ų�� ���� ���� �⺻ �ɷ�ġ�� �����ϴ°�?
/*static*/ bool CSkillManager::CheckPropertyForStudy( int iSkillIDX )
{
	bool bResult = true;

	for( int i = 0; i < SKILL_NEED_ABILITY_TYPE_CNT; i++ )
	{
		if( SKILL_NEED_ABILITY_TYPE( iSkillIDX, i ) )
		{
			bResult = false;

			switch( SKILL_NEED_ABILITY_TYPE( iSkillIDX, i ) )
			{
				case AT_LEVEL:
					{
						if( SKILL_NEED_ABILITY_VALUE( iSkillIDX, i ) <= g_pAVATAR->Get_LEVEL() )
							return true;
					}
					break;
			}
		}
	}

	return bResult;
}


///
///	��ų ����� ���� Ÿ�� ���ϱ�
/// �Ϲ� ��ų�ϰ�� hpüũ�� ������ �������ǰ��� ��ų�� ��� hp üũ�� ���� �ʴ´�.
///
/*static*/ CObjCHAR* CSkillManager::GetSkillTarget( int iServerObjIDX, int iSkillIDX )
{
	if( iSkillIDX == 0 )
		return g_pObjMGR->Get_ClientCharOBJ( iServerObjIDX, true );

	if( iSkillIDX >= g_SkillList.Get_SkillCNT() )
	{
		assert( 0 && "Skill index is greater than max skill count" );
		return g_pObjMGR->Get_ClientCharOBJ( iServerObjIDX, true );
	}

	if( SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_RESURRECTION )
		return g_pObjMGR->Get_ClientCharOBJ( iServerObjIDX, false );

	return g_pObjMGR->Get_ClientCharOBJ( iServerObjIDX, true );
}