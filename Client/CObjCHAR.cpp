/*
	$Header: /Client/CObjCHAR.cpp 652   05-10-20 2:46p Gioend $  
*/
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
#include "Interface/CClanMarkUserDefined.h"
#include "GameCommon/Skill.h"
#include "Event/Quest_FUNC.h"
#include "CCamera.h"
#include "Game_FUNC.h"

#include "GameProc/ChangeVisibility.h"   
#include "CObjCART.h"
#include "CObjCHAR_Collision.h"

#include "GameCommon/Item.h"
#include "GameData/CParty.h"
#include "Common/IO_Pat.h"
#include "Misc/GameUtil.h"
#include "CommandFilter.h"
#include "../GameProc/DelayedExp.h"
#include "../Country.h"
#include "../common/CInventory.h"
#include "system/System_Func.h"


extern CCamera * g_pCamera;


extern CAI_OBJ *AI_FindFirstOBJ( CAI_OBJ *pBaseOBJ, int iDistance );
extern CAI_OBJ *AI_FindNextOBJ ();


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param int iDistance �Ÿ�
/// @brief  : AI ������ ���� ù���� ������Ʈ�� ã�´�..
//--------------------------------------------------------------------------------

CAI_OBJ* CObjCHAR::AI_FindFirstOBJ( int iDistance )
{
	return ::AI_FindFirstOBJ( this, iDistance );
}

//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param 
/// @brief  :AI_FindFirstOBJ ȣ���� ȣ���Ͽ� ����Ʈ���� ���� ������Ʈ�� ������ ��
//--------------------------------------------------------------------------------

CAI_OBJ* CObjCHAR::AI_FindNextOBJ ()
{
	return ::AI_FindNextOBJ ();
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param wSrvDIST �������� ���Ǿ� ���ƿ� ��ǥ��ġ�� ������ġ���� 2������ �Ÿ�. ���� cm
/// @param PosGOTO �������� ���ƿ� �̵��� ��ǥ��ġ. ����μ� Z ���� �ǹ̾���.
/// @brief  : �̵� �ӵ� ����. �� �����Ӹ��� ���ŵ�. ������ Ŭ���̾�Ʈ�� �ӵ����� ���� ��쿡,
///           ������ �ӵ��� ������Ų��. Ȥ�� �ʹ� ���� ���̰� �� ����, ������ �̵���Ų��.
///           
//--------------------------------------------------------------------------------

void CObjCHAR::Adj_MoveSPEED ( WORD wSrvDIST, const D3DVECTOR& PosGOTO )
{
	int iClientDIST; // The difference between the current-on the client position. Unit CM
	float fCurSpeed, fNewSpeed, fNeedTime;

	fCurSpeed = this->Get_DefaultSPEED(); // The current default rate calculated by the expression

	if ( 0 == fCurSpeed ) // If the velocity is zero, the same as the current location and the PosGOTO is?
	{ 
		m_fAdjustSPEED = 0;

		//assert(m_PosCUR.x == PosGOTO.x);
		//assert(m_PosCUR.y == PosGOTO.y);

		return;
	}

	fNeedTime = float(wSrvDIST) / fCurSpeed;

	// The client calculates the distance of the current goals.
	iClientDIST = CD3DUtil::distance ((int)m_PosCUR.x, (int)m_PosCUR.y, (int)PosGOTO.x, (int)PosGOTO.y);

	//Numenor: I kept the assert, not like PY. We'll see what it beco,es
	assert(iClientDIST >= 0);		//PY: Can't have these damn asserts breaking things all the time
	//if(iClientDIST < 0)				//this is probably a bad idea in the long run as it will cause discrepencies between server and client but it will do for now
	//	iClientDIST = 0;

	if ( 0 == iClientDIST ) { // If you have already reached the target area,
		// The client does not need to be moved.
		m_fAdjustSPEED = 0;
		return;
	}
	/// To go the distance to go from the client on the server is greater than the distance
	else if( iClientDIST <= wSrvDIST )
	{

		/// To keep the current rate.
		/// Because if you wait for a client to go ahead.
		m_fAdjustSPEED = fCurSpeed;
		return;

	}
	else // wSrvDIST < iClientDIST. The distance of the server from the client if, rather than distance from the rate increase.
	{
		fNewSpeed = float(iClientDIST) / fNeedTime; // Calculate the new speed to reach in time

		/// If the subject or just run sucked. (Without jumping)
		if( this->IsA( OBJ_USER ) == false )
		{
			int iDiffDistance = iClientDIST - wSrvDIST; // The server calculates the distance and the client street car. Unit: cm

			float fNeedTimeDiff = float(iDiffDistance) / fNewSpeed; // It takes time to overcome the distance car

			if (fNeedTimeDiff > 1.0f)
			{
				// Street car to overcome delays over one second (too slow).
				fNewSpeed = fCurSpeed; // The force that moves the displacement

				D3DXVECTOR3 vDir = (D3DXVECTOR3)PosGOTO - m_PosCUR;
				float fRatio = (float)iDiffDistance / (float)iClientDIST;

				D3DXVECTOR3 vPosCur = m_PosCUR + ( vDir * fRatio );
				vPosCur.z = m_PosCUR.z;

				this->ResetCUR_POS( vPosCur );
			}
		}
	}
	m_fAdjustSPEED = fCurSpeed;
}

//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ���ݼӵ� ����..
//--------------------------------------------------------------------------------

void CObjCHAR::Adj_AniSPEED ( float fAniSpeed )
{
	::setAnimatableSpeed( this->m_hNodeMODEL, fAniSpeed  );		
}

//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param int iServerTarget ������Ʈ �����ε���
/// @param tPOINTF &PosFROM 
/// @param tPOINTF &PosGOTO
/// @brief  : ���ݸ�� ����
//--------------------------------------------------------------------------------

void CObjCHAR::SetCMD_ATTACK (int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosGOTO)
{
	if( this->IsA( OBJ_USER ) )
	{
		CObjAttackCommand* pObjCommand = (CObjAttackCommand*)g_CommandFilter.GetCommandObject( OBJECT_COMMAND_ATTACK );
		pObjCommand->SetCMD_ATTACK( iServerTarget, wSrvDIST, PosGOTO );

		g_CommandFilter.SetPrevCommand( pObjCommand );
	}

	/// ���� ����� ���� �ֳ�?
	if( this->CanApplyCommand() == false )
	{
		this->PushCommandAttack( iServerTarget, wSrvDIST, PosGOTO );
		return;
	}

	//--------------------------------------------------------------------------------
	/// If this char is under pet mode, pass the command to pet.
	int iPetMode = this->GetPetMode();
	if( iPetMode >= 0 )
	{	
		/// Pet mode �� ��쿡��..
		if( this->CanAttackPetMode() )
		{
			SetCMD_PET_ATTACK( iServerTarget, wSrvDIST, PosGOTO );
		}
//����ȣ::������ ��� �ƹ�Ÿ�� ���ݼӼ��� ���� �ϵ��� �Ѵ�. 
#ifndef _GBC
		return;
#endif 
	}
	
	/// �������� ���� ���� ��Ŷ ó��...
	this->Adj_MoveSPEED( wSrvDIST, PosGOTO );	

	this->m_PosGOTO = PosGOTO;

	CObjAI::SetCMD_ATTACK( iServerTarget );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param 
/// @brief  : Constructor
//--------------------------------------------------------------------------------

CObjCHAR::CObjCHAR () : m_EndurancePack( this ), m_ChangeActionMode( this ), m_ObjVibration( this, 200 )
{
#ifdef	__VIRTUAL_SERVER
	m_pRegenPOINT = NULL;
	m_pRegenListNODE = NULL;
#endif

	m_fScale = 1.0f;

	m_fHeightOfGround = 0.0f;
	m_hNodeMODEL = NULL;

	m_ppBoneEFFECT = NULL;
	m_hR_TRAIL = m_hL_TRAIL = NULL;

	for (short nP=0; nP<MAX_BODY_PART; nP++) {
		m_phPartVIS[ nP ] = NULL;
		m_pppEFFECT[ nP ] = NULL;
		m_nEffectPointCNT[ nP ] = 0;
	}	

	::ZeroMemory( &m_DeadDAMAGE, sizeof( tagDAMAGE ) );
	m_lDeadTIME = 0;

	m_DamageList.reserve( 10 );

	m_bProcEffectedSkill = false;
	m_iEffectedSkillIndex = 0;
	m_EffectedSkillList.clear();

	m_fAltitude = 0.0;

	m_iHP	 = 1;
	m_iMP	 = 1;
	m_iMaxHP = 1;
	m_iMaxMP = 1;

	m_dwLastRecoveryUpdateTime	= g_GameDATA.GetGameTime();
	m_dwElapsedTime				= 0;
	m_dwFrameElapsedTime		= 0;
	
	m_bDead = false;

	m_pChangeVisibility			= NULL;
	m_bStopDead					= false;


	m_bIsVisible				= true;

	m_hNodeGround				= NULL;

	m_pCollision				= new CObjCHAR_Collision;				/// will be deleted in DeletCHAR()

	memset( &m_SummonMob, 0 , sizeof( gsv_MOB_CHAR ) );
	m_bHaveSummonedMob			= false;							/// ��ȯ�ؾߵ� ���� �ִ°�?

	m_bUseResetPosZ				= false;
	m_fResetPosZ				= 0;
	m_fModelSPEED				= 0;

	m_iLastCastingSkill			= 0;

	m_dwClanID					= 0;
	m_wClanMarkBack				= 0;
	m_wClanMarkCenter			= 0;

	m_bFrameING					= 0;
	m_ClanMarkUserDefined		= 0;


	m_ReviseHP					= 0;
	m_ReviseMP					= 0; 

//-------------------------------------------------------------------------------
	//������
	m_bDisguise					= false;


//--------------------------------------------------------------------------------
	///����ȣ
	//īƮ ������ �ʱ�ȭ
	m_iPetType					= -1;
	m_pObjCART					= NULL; 
	m_pRideUser					= NULL;

	m_bUseCartSkill				= FALSE;
	m_IsRideUser				= FALSE;
	
	m_saveSpeed					= 0.0f;

	m_iPetType					= -1;
	m_iRideIDX					= 0;
	m_skCartIDX					= 0;
		
	//�Ʒξ� ���º��� �ʱ�ȭ
	m_IsAroa					= 0;
	m_IsCartVA					= 0;
//--------------------------------------------------------------------------------
	m_AruaAddMoveSpeed			= 0;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param 
/// @brief  : Destructor
//--------------------------------------------------------------------------------

CObjCHAR::~CObjCHAR ()
{
	// ������ ��ϵ� HNODE�� ����.
	this->DeleteCHAR ();

	ClearExternalEffect();
	m_EndurancePack.ClearEntityPack();
	SAFE_DELETE( m_pCollision );
}

//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param 
/// @brief  : ȸ���� ���� Ÿ�̸� ����
//--------------------------------------------------------------------------------
void CObjCHAR::ClearTimer()
{ 
	m_dwLastRecoveryUpdateTime	= g_GameDATA.GetGameTime();
	m_dwElapsedTime				= 0;
	m_dwFrameElapsedTime		= 0;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param 
/// @brief  : �װų�, �ı��ÿ� ��� ������ ����Ʈ ����
//--------------------------------------------------------------------------------

void CObjCHAR::ClearAllEntityList()
{
	/// proc all effect of skill
	ProcEffectedSkill();

	/// clear all damage list
	ClearAllDamage();

	/// drop field item list
	DropFieldItemFromList();

	/// clear all command
	ClearAllCommand();
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param 
/// @brief  : �ܺο��� ��Ͻ�Ų ����Ʈ ����Ʈ�� ����
//--------------------------------------------------------------------------------

void CObjCHAR::ClearExternalEffect()
{
	classDLLNODE< CEffect* > *pNode;
	pNode = m_ExternalEffectLIST.GetHeadNode ();
	while( pNode ) 
	{			
		/// ����Ʈ�� ����� ����Ʈ�� �θ�ó���� ���Ѵ�. �ֳ�? ���� �θ�ϱ�..
		g_pEffectLIST->Del_EFFECT( pNode->DATA, false );

		m_ExternalEffectLIST.DeleteNFree( pNode );
		pNode = m_ExternalEffectLIST.GetHeadNode();
	}

	m_ExternalEffectLIST.ClearList();

	DeleteWeatherEffect();
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param CEffect* pEffect ���� ����Ʈ
/// @brief  : �ܺ� ����Ʈ ���
//--------------------------------------------------------------------------------

void CObjCHAR::AddExternalEffect(CEffect* pEffect)
{
	m_ExternalEffectLIST.AllocNAppend( pEffect );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  CEffect* pEffect ���� ����Ʈ
/// @brief  : �ܺο��� ��ϵ� ����Ʈ ����
//--------------------------------------------------------------------------------

void CObjCHAR::DeleteExternalEffect( CEffect* pEffect )
{
	if( pEffect == NULL )
		return;

	classDLLNODE< CEffect* > *pNode;
	pNode = m_ExternalEffectLIST.GetHeadNode ();
	while( pNode ) 
	{		
		if ( pNode->DATA == pEffect )
		{
			m_ExternalEffectLIST.DeleteNFree( pNode );
			return;
		}
		pNode = m_ExternalEffectLIST.GetNextNode( pNode );
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  D3DVECTOR &PosSCR ���� ��ũ����ǥ( �ƿ�ǲ )
/// @brief  : ���� ĳ���� ��ġ�� ��ũ�� ��ǥ�� ����
//--------------------------------------------------------------------------------

void CObjCHAR::GetScreenPOS ( D3DVECTOR &PosSCR)
{	
	// ���� ��ǥ�� Ű�� ���� ��ġ�� �̸���� ��ġ�� ����
	::worldToScreen( m_PosCUR.x, m_PosCUR.y, getPositionZ(m_hNodeMODEL) + m_fStature, &PosSCR.x, &PosSCR.y, &PosSCR.z );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  HNODE hLinkNODE ��ũ�� ����� ��� �ڵ�
/// @param  short nDummyIDX ��ũ�� ���� �ε���
/// @brief  : �Է¹��� ��带 �Է¹��� �����ε����� �ش��ϴ� ���̿� ��ũ��Ų��.
//--------------------------------------------------------------------------------

bool CObjCHAR::LinkDummy ( HNODE hLinkNODE, short nDummyIDX )
{	
	int iDummyCnt = ::getNumDummies( m_hNodeMODEL );
	if ( iDummyCnt >= nDummyIDX )
		return ( 0 != ::linkDummy( m_hNodeMODEL, hLinkNODE, nDummyIDX) );

	return false;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  HNODE hLinkNODE ��ũ�� ����� ��� �ڵ�
/// @brief  : �Է¹��� ��带 ������ ���̿� ��ũ ��Ŵ
//--------------------------------------------------------------------------------

bool CObjCHAR::Link2LastDummy(HNODE hLinkNODE)
{
/*
	int iDummyCnt = ::getNumDummies( m_hNodeMODEL );
	// ������ ���̿�...
	if ( iDummyCnt > 0 )
		m_iLastDummyIDX = iDummyCnt - 1;
*/
	return ( 0 != ::linkDummy( m_hNodeMODEL, hLinkNODE, m_iLastDummyIDX) );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  
/// @brief  : ���� ĳ���͸� ���� �ִ´�. ( ��� ���� ������Ʈ ���� )
//--------------------------------------------------------------------------------

void CObjCHAR::InsertToScene (void)
{
	if ( m_bIsVisible || !m_hNodeMODEL || !m_pCharMODEL )
		return;

	m_bIsVisible = true;

	::insertToScene( m_hNodeMODEL );		// CObjCHAR::InsertToScene

	//----------------------------------------------------------------------------------------------------	
	/// @brief CHILD��InsertToScene����
	//----------------------------------------------------------------------------------------------------
	//for (short nP=0; nP<MAX_BODY_PART; nP++) 
	//{
	//	CMODEL<CCharPART> *pCharPART = m_pCharMODEL->GetCharPART( nP );

	//	if ( pCharPART  ) 
	//	{
	//		short nI;

	//		if ( m_pppEFFECT[ nP ] ) 
	//		{
	//			for (nI=0; nI<pCharPART->m_nDummyPointCNT; nI++)
	//			{
	//				if ( m_pppEFFECT[ nP ][ nI ] ) 
	//				{
	//					m_pppEFFECT[ nP ][ nI ]->InsertToScene ();
	//				}
	//			}
	//		}

	//		for (nI=0; nI<pCharPART->m_nPartCNT; nI++) 
	//		{
	//			if ( m_phPartVIS[ nP ][ nI ] ) 
	//			{
	//				::insertToScene( m_phPartVIS[ nP ][ nI ] );		// CObjCHAR::InsertToScene
	//			}
	//		}
	//	}
	//}

	//
	//// ���� ȿ��.
	//if ( m_ppBoneEFFECT ) 
	//{
 	//	for (nP=0; nP<m_pCharMODEL->GetBoneEffectCNT(); nP++)
	//		m_ppBoneEFFECT[ nP ]->InsertToScene ();
	//}

	//// ���ܻ� ȿ��.
	//for (nP=0; nP<2; nP++) 
	//{
	//	if ( m_hTRAIL[ nP ] )
	//		::insertToScene( m_hTRAIL[ nP ] );
	//}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  bool bIncludeEFFECT ����Ʈ�� ���ԵǾ��°�?
/// @brief  : ������ �����Ѵ�. ( ���� ������Ʈ ���� )
//--------------------------------------------------------------------------------

void CObjCHAR::RemoveFromScene (bool bIncludeEFFECT)
{
	if ( !m_bIsVisible )
		return;

	m_bIsVisible = false;
	::removeFromScene ( m_hNodeMODEL );								// CObjCHAR::RemoveFromScene

	//----------------------------------------------------------------------------------------------------	
	/// @brief CHILD��removeFromScene����
	//----------------------------------------------------------------------------------------------------
	//for (short nP=0; nP<MAX_BODY_PART; nP++) 
	//{
	//	CMODEL<CCharPART> *pCharPART = m_pCharMODEL->GetCharPART( nP );

	//	if ( pCharPART  ) 
	//	{
	//		short nI;
	//		if ( bIncludeEFFECT && m_pppEFFECT[ nP ] ) 
	//		{
	//			for (nI=0; nI<pCharPART->m_nDummyPointCNT; nI++)
	//				if ( m_pppEFFECT[ nP ][ nI ] ) {
	//					m_pppEFFECT[ nP ][ nI ]->RemoveFromScene ();
	//				}
	//		}

	//		for (nI=0; nI<pCharPART->m_nPartCNT; nI++) {
	//			if ( m_phPartVIS[ nP ][ nI ] ) {
	//				::removeFromScene( m_phPartVIS[ nP ][ nI ] );	// CObjCHAR::RemoveFromScene
	//			}
	//		}
	//	}
	//}

	//if ( bIncludeEFFECT ) 
	//{
	//	// ���� ȿ��.
	//	if ( m_ppBoneEFFECT ) 
	//	{
	//		for (nP=0; nP<m_pCharMODEL->GetBoneEffectCNT(); nP++)
	//		{
	//			if( m_ppBoneEFFECT[ nP ] )
	//				m_ppBoneEFFECT[ nP ]->RemoveFromScene ();
	//		}
	//	}

	//	for (nP=0; nP<2; nP++) {
	//		if ( m_hTRAIL[ nP ] )
	//			::removeFromScene( m_hTRAIL[ nP ] );
	//	}
	//}	
}

/*override*/D3DXVECTOR3& CObjCHAR::Get_CurPOS ()
{
	m_PosCUR.z = ::getPositionZ(m_hNodeMODEL);

	return m_PosCUR;
}

/*override*/ void CObjCHAR::SetEffectedSkillFlag( bool bResult )
{ 
	m_bProcEffectedSkill = bResult; 
	///Log_String( LOG_NORMAL, "SetEffectedSkillFlag[ %d ]\n", bResult );
}

/*override*/ bool CObjCHAR::bCanActionActiveSkill()
{ 
	///Log_String( LOG_NORMAL, "bCanActionActiveSkill[ %d ]\n", m_bProcEffectedSkill );
	return m_bProcEffectedSkill; 
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  CEffect *pEffect ��ũ�� ����Ʈ
/// @param  short nPartIDX	 ��ũ�� ĳ���� ��Ʈ
/// @param  short nPointIDX  ��ũ�� ĳ���� ��Ʈ�� ������ �ε���
/// @brief  : ����Ʈ�� ĳ������ Ư�� ��Ʈ�� ���� Ư�� �����Ϳ�����Ʈ�� ��ũ�Ѵ�.
//--------------------------------------------------------------------------------

bool CObjCHAR::LinkEffectToPOINT (CEffect *pEffect, short nPartIDX, short nPointIDX)
{
	CMODEL<CCharPART> *pCharPART;
	pCharPART = m_pCharMODEL->GetCharPART( nPartIDX );
	
	if ( pCharPART && 
		( !pCharPART->m_pDummyPoints || nPointIDX >= pCharPART->m_nDummyPointCNT ) ) 
	{		
		_ASSERT( FALSE ); 

		return false;
	}

	pEffect->Transform( pCharPART->m_pDummyPoints[ nPointIDX ].m_Transform );
	pEffect->LinkNODE ( m_phPartVIS[ nPartIDX ][ pCharPART->m_pDummyPoints[ nPointIDX ].m_nParent ] );

	return true;
}

//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  CEffect *pEffect ��ũ�� ����Ʈ
/// @param  int iPointNO  ��ũ�� ���� ������ �ε���
/// @brief  : ����Ʈ�� ĳ������ Ư�� ���̿� ��ũ
//--------------------------------------------------------------------------------

bool CObjCHAR::LinkEffectToDUMMY (CEffect *pEffect, short nDummyIDX )
{
	int iDummyCnt = ::getNumDummies( m_hNodeMODEL );
	if ( iDummyCnt < nDummyIDX )
		return false;	
	
	pEffect->LinkDUMMY ( this->GetZMODEL(), nDummyIDX );

	return true;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  CMODEL<CCharPART> *pCharPART ĳ������ Ư����Ʈ( �������̳�, �޼� ���Ⱑ �ǰ���? )
/// @param  nPartIDX �ٵ� ��Ʈ �ε���( ������ ����, �޼չ��� �� �ε����� ���;ߵȴ� )
/// @param  bool bLinkBONE		���� ��ũ �Ұ��ΰ�?
/// @param  int iColorNO		���ܻ� Į�� ��ȣ
/// @param  int iDuration		���ӽð�?
/// @param  int iBaseDummyIDX	������, �޼� ������ ���� ���̽� �� �ε���
/// @brief  : ��տ� ���⸦ ��� �����Ƿ� �ΰ����� �����ϴ�.
//--------------------------------------------------------------------------------

void CObjCHAR::LoadTRAIL (CMODEL<CCharPART> *pCharPART, short nPartIDX, bool bLinkBONE, int iColorNO, int iDuration, int iBaseDummyIDX )
{
	if ( pCharPART->m_nDummyPointCNT < 3 )
		return;

	short nTrailIDX = nPartIDX - BODY_PART_WEAPON_R;
	_ASSERT( nTrailIDX >= 0 && nTrailIDX < 2 );

	m_hTRAIL[ nTrailIDX ] = ::loadTrail ( NULL,	// ZSTRING pName, 
		 10,									// float fDistancePerPoint,
		 iDuration,								// int iDurationInMilliSecond,
		 1,										// int bUseCurve,
		 g_GameDATA.m_TrailTexture.Get(),		// ZSTRING pTexturePath, 
		 g_dwCOLOR[ iColorNO ],					// D3DCOLOR,
		 //0, 0, 0,								// float fSP_X, float fSP_Y, float fSP_Z,
		 pCharPART->m_pDummyPoints[ 0 + iBaseDummyIDX ].m_Transform,
		 //0, 0, 100 );							// float fEP_X, float fEP_Y, float fEP_Z );
		 pCharPART->m_pDummyPoints[ 1 + iBaseDummyIDX ].m_Transform );

	if ( m_hTRAIL[ nTrailIDX ] ) 
	{
		::controlTrail ( m_hTRAIL[ nTrailIDX ], 0 );	// stop !!

		//----------------------------------------------------------------------------------------------------	
		/// @brief CHILD��InsertToScene����
		//----------------------------------------------------------------------------------------------------
		//if ( m_bIsVisible )		// ���� ���̸�..
		//	::insertToScene( m_hTRAIL[ nTrailIDX ] );
	}

	if ( bLinkBONE ) 
	{
		::linkDummy ( m_hNodeMODEL, m_hTRAIL[ nTrailIDX ], nTrailIDX+DUMMY_IDX_R_HAND );
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  nPartIDX �ٵ� ��Ʈ �ε���( ������ ����, �޼չ��� �� �ε����� ���;ߵȴ� )
/// @brief  : ���ܻ� ����
//--------------------------------------------------------------------------------

void CObjCHAR::UnloadTRAIL (short nPartIDX)
{
	short nTrailIDX = nPartIDX - BODY_PART_WEAPON_R;

	_ASSERT( nTrailIDX >= 0 && nTrailIDX < 2 );

	if ( m_hTRAIL[ nTrailIDX ] ) {
		::controlTrail( m_hTRAIL[ nTrailIDX ], 0 );	// stop !!
		::unlinkNode  ( m_hTRAIL[ nTrailIDX ] );
		::unloadTrail ( m_hTRAIL[ nTrailIDX ] );
		m_hTRAIL[ nTrailIDX ] = NULL;
	}
}

//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  
/// @brief  : ĳ���Ϳ� �ٴ� ȿ��( �����ۻ� ĳ���� ������ ���۵�)�� ��ũ
//--------------------------------------------------------------------------------

void CObjCHAR::Link_EFFECT (void)
{
	short nP;

	for (nP=0; nP<MAX_BODY_PART; nP++) 
	{
		if ( NULL == m_pppEFFECT[ nP ] )
			continue;

		CMODEL<CCharPART> *pCharPART = m_pCharMODEL->GetCharPART( nP );
		_ASSERT( pCharPART ) ;

		for (short nI=0; nI<pCharPART->m_nDummyPointCNT; nI++) 
		{
			if ( NULL != m_pppEFFECT[ nP ][ nI ] )
				m_pppEFFECT[ nP ][ nI ]->LinkNODE( m_phPartVIS[ nP ][ pCharPART->m_pDummyPoints[ nI ].m_nParent ] );
		}
	}

	for (nP=0; nP<2; nP++) 
	{
		if ( m_hTRAIL[ nP ] ) 
		{
			::linkDummy ( m_hNodeMODEL, m_hTRAIL[ nP ], nP+DUMMY_IDX_R_HAND );
		}
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  
/// @brief  : ĳ���Ϳ� �ٴ� ȿ��( �����ۻ� ĳ���� ������ ���۵�)�� �� ��ũ
//--------------------------------------------------------------------------------

void CObjCHAR::Unlink_EFFECT (void)
{
	short nP;

	for (nP=0; nP<2; nP++) {
		if ( m_hTRAIL[ nP ] ) {
			::unlinkNode ( m_hTRAIL[ nP ] );
		}
	}
	for (nP=0; nP<MAX_BODY_PART; nP++) 
	{
		if ( NULL == m_pppEFFECT[ nP ] )
			continue;

		// �������� �ٲ�鼭 ����Ʈ�� ���� �����ȴ�.
		// set part model�� ȣ��Ǳ����� pCharPART�� �ٲ� ����������
		// �������� �ʾ� ��~
		for (short nI=0; nI<m_nEffectPointCNT[ nI ]; nI++) 
		{
			if ( NULL != m_pppEFFECT[ nP ][ nI ] )
				m_pppEFFECT[ nP ][ nI ]->UnlinkNODE ();
		}
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  CMODEL<CCharPART> *pCharPART ����Ʈ�� �߰��� ĳ������ ��Ʈ
/// @param  short nPartIDX	   ��Ʈ �ε���
/// @param  short nPointIDX	   ��Ʈ�� ������ �ε���
/// @param  t_HASHKEY HashEffectFILE  ����Ʈ������ �ؽ�Ű
/// @param  bool bLinkNODE			  ��ũ�Ұ��ΰ�?
/// @brief  : ĳ���Ϳ� �ٴ� ȿ��( �����ۻ� ĳ���� ������ ���۵�)�� �߰�
///	@bug    : 04/4/28 g_pEffectLIST->Add_EFFECT(.., false ) ���µ�.. g_pEffectLIST->Del_EFFECT �� ������ �Ƿ��ߴ�..
/// @bug    : 04/4/28 ��ũ�Ҷ� Rotation�� ���ϳ�?
//--------------------------------------------------------------------------------

void CObjCHAR::Add_EFFECT( CMODEL<CCharPART> *pCharPART, short nPartIDX, short nPointIDX, t_HASHKEY HashEffectFILE, bool bLinkNODE )
{
	if ( NULL == pCharPART )
		return;

	if ( nPointIDX >= pCharPART->m_nDummyPointCNT )
		return;

	if ( m_pppEFFECT[ nPartIDX ] ) 
	{
		// ���� ����Ʈ�� ȿ�� ����...
		///g_pEffectLIST->Del_EFFECT( m_pppEFFECT[ nPartIDX ][ nPointIDX ] );
		SAFE_DELETE( m_pppEFFECT[ nPartIDX ][ nPointIDX ] );
		m_pppEFFECT[ nPartIDX ][ nPointIDX ] = NULL;
	} else 
	{
		m_nEffectPointCNT[ nPartIDX ] = pCharPART->m_nDummyPointCNT;
		m_pppEFFECT[ nPartIDX ] = new CEffect*[ pCharPART->m_nDummyPointCNT ];
		for (short nI=0; nI<pCharPART->m_nDummyPointCNT; nI++)
			m_pppEFFECT[ nPartIDX ][ nI ] = NULL;
	}

	m_pppEFFECT[ nPartIDX ][ nPointIDX ] = g_pEffectLIST->Add_EFFECT( HashEffectFILE );
	if ( m_pppEFFECT[ nPartIDX ][ nPointIDX ] ) 
	{
#ifdef	_DEBUG
		if ( !pCharPART->m_pDummyPoints ||
		      nPointIDX >= pCharPART->m_nDummyPointCNT ) 
		{
			_ASSERT( FALSE );
		}
#endif

		m_pppEFFECT[ nPartIDX ][ nPointIDX ]->Transform( pCharPART->m_pDummyPoints[ nPointIDX ].m_Transform );

		if ( m_bIsVisible )
			m_pppEFFECT[ nPartIDX ][ nPointIDX ]->InsertToScene ();

		if ( bLinkNODE && m_phPartVIS[ nPartIDX ] )
			m_pppEFFECT[ nPartIDX ][ nPointIDX ]->LinkNODE( m_phPartVIS[ nPartIDX ][ pCharPART->m_pDummyPoints[ nPointIDX ].m_nParent ] );
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  short nPartIDX	   ��Ʈ �ε���
/// @param  short nPointIDX	   ��Ʈ�� ������ �ε���
/// @param  t_HASHKEY HashEffectFILE  ����Ʈ������ �ؽ�Ű
/// @brief  : ĳ���Ϳ� �ٴ� ȿ��( �����ۻ� ĳ���� ������ ���۵�)�� �߰�
///	@bug    : ���ο��� Add_EFFECT( CMODEL<CCharPART> *pCharPART, short nPartIDX, short nPointIDX, t_HASHKEY HashEffectFILE, bool bLinkNODE ) �Լ� ȣ��
//--------------------------------------------------------------------------------

void CObjCHAR::Add_EFFECT(short nPartIDX, short nPointIDX, t_HASHKEY HashEffectFILE)
{
	CMODEL<CCharPART> *pCharPART;
	pCharPART = m_pCharMODEL->GetCharPART( nPartIDX );
	this->Add_EFFECT (pCharPART, nPartIDX, nPointIDX, HashEffectFILE);
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  short nPartIDX	   ��Ʈ �ε���
/// @brief  : ĳ���Ϳ� �ٴ� ȿ��( �����ۻ� ĳ���� ������ ���۵�)�� ����
///	@bug    : /// @Bug AddEffect( .. , false ) ����..
//--------------------------------------------------------------------------------

void CObjCHAR::Del_EFFECT( short nPartIDX )
{
	CMODEL<CCharPART> *pCharPART;
	pCharPART = m_pCharMODEL->GetCharPART( nPartIDX );
	if ( pCharPART ) {
		if ( m_pppEFFECT[ nPartIDX ] ) 
		{
			for (short nT=0; nT<pCharPART->m_nDummyPointCNT; nT++) 
			{
				///g_pEffectLIST->Del_EFFECT( m_pppEFFECT[ nPartIDX ][ nT ] );
				if( m_pppEFFECT[ nPartIDX ][ nT ] )
				{
					delete m_pppEFFECT[ nPartIDX ][ nT ];
					m_pppEFFECT[ nPartIDX ][ nT ] = NULL;
				}
				m_pppEFFECT[ nPartIDX ][ nT ] = NULL;
			}
		}
	}

	SAFE_DELETE_ARRAY( m_pppEFFECT[ nPartIDX ] );

	switch( nPartIDX ) {
		case BODY_PART_WEAPON_R :
		case BODY_PART_WEAPON_L :
			this->UnloadTRAIL( nPartIDX );
			break;
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  short nPartIDX	   ��Ʈ �ε���
/// @param  short nItemNo	   ��Ʈ�� ������ �ε���
/// @param  bool bLinkNODE	   ��ũ�Ҳ���?
/// @brief  : ĳ���Ϳ� �ٴ� ȿ���� ����
///				������ �ܻ�, ���� Ư���� �ɼǿ� ���� ����Ǵ� Ư�� ������ ȿ���� �ٿ����Ҷ�..
//--------------------------------------------------------------------------------

void CObjCHAR::New_EFFECT (short nPartIdx, short nItemNo, bool bLinkNODE)
{
	// �����ۿ� ���� �⺻ ȿ�� ����.
	this->Del_EFFECT( nPartIdx );

	CMODEL<CCharPART> *pCharPART = g_DATA.Get_CharPartMODEL( nPartIdx, nItemNo, this->IsFemale() );	//m_pMD_CharPART[ nPartIdx ]->GetMODEL( nItemNo );
	if ( NULL == pCharPART )
		return;

	short nEffectIDX=0;
	switch( nPartIdx ) 
	{
		case BODY_PART_WEAPON_R :
			{
				nEffectIDX = WEAPON_DEFAULT_EFFECT( nItemNo );

				if ( EFFECT_TRAIL_NORMAL( nEffectIDX ) ) 
				{
					this->LoadTRAIL( pCharPART, BODY_PART_WEAPON_R, bLinkNODE, EFFECT_TRAIL_NORMAL( nEffectIDX ), EFFECT_TRAIL_DURATION( nEffectIDX ) );

					/// ��� �̵��� �����ϰ��..
					if( ( pCharPART->m_nDummyPointCNT > 2 ) && 
						( ( WEAPON_TYPE( nItemNo ) == 251 ) || ( WEAPON_TYPE( nItemNo ) == 252 ) ) )
						this->LoadTRAIL( pCharPART, BODY_PART_WEAPON_L, bLinkNODE, EFFECT_TRAIL_NORMAL( nEffectIDX ), EFFECT_TRAIL_DURATION( nEffectIDX ), 2 );
				}				
			}
			break;
		case BODY_PART_WEAPON_L :
			// nEffectIDX = SUBWPN_DEFAULT_EFFECT( nItemNo );
			if ( EFFECT_TRAIL_NORMAL( nEffectIDX ) ) {
				this->LoadTRAIL( pCharPART, BODY_PART_WEAPON_L, bLinkNODE, EFFECT_TRAIL_NORMAL( nEffectIDX ), EFFECT_TRAIL_DURATION( nEffectIDX ) );
			}
			break;
		default :
			return;
	}

	if ( nEffectIDX ) 
	{
		t_HASHKEY EffectHASH;
		for (short nP=0; nP<EFFECT_POINT_CNT; nP++) 
		{
			EffectHASH = g_pEffectLIST->Get_IndexHashKEY( EFFECT_POINT( nEffectIDX, nP ) );
			if ( EffectHASH )
				this->Add_EFFECT( pCharPART, nPartIdx, nP, EffectHASH, bLinkNODE );
		}
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  char *szName �̸�( ���μ��� ���� ������ȣ�� �ٿ� ������ �̸��� �����Ѵ�. )
/// @param  int iPartIDX ��Ʈ �ε���
/// @brief  : Ư�� ���� ����
//--------------------------------------------------------------------------------

void CObjCHAR::CreateSpecificPART( char *szName, int iPartIDX )
{
	m_phPartVIS[ iPartIDX ] = m_pCharMODEL->CreatePART( szName, m_hNodeMODEL, iPartIDX );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  int iPartIDX ��Ʈ �ε���
/// @param  HNODE *pVIS  ��Ʈ�� �����ϴ� �κе��� visiable ��� �迭
/// @brief  : Ư�� ���� ����
//--------------------------------------------------------------------------------

void CObjCHAR::DeleteSpecificPART( short nPartIdx, HNODE *pVIS )
{
	m_pCharMODEL->DeletePART( nPartIdx, pVIS );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  char *szName �̸�
/// @brief  : ĳ���� ����Ʈ ����, ���ο��� ����Ʈ���� CreateSpecificPART ����
//--------------------------------------------------------------------------------

void CObjCHAR::CreatePARTS (char *szName)
{
	m_pCharMODEL->ClearRenderUnitParts();

	// npc ���ⶫ�� MAX_BODY_PART���� ...
	for (short nP=0; nP<MAX_BODY_PART; nP++) 
	{
		CreateSpecificPART( szName, nP );
		//m_phPartVIS[ nP ] = m_pCharMODEL->CreatePART( szName, m_hNodeMODEL, nP );
	}	

	// �ɸ��� ����.
	m_fStature = ::getModelHeight (this->m_hNodeMODEL);
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  bool bDelEFFECT ����Ʈ�� �����?
/// @brief  : ĳ���� �� ��Ʈ ����
//--------------------------------------------------------------------------------

void CObjCHAR::DeletePARTS (bool bDelEFFECT)
{
	this->RemoveFromScene (bDelEFFECT);

	if ( this->m_hNodeMODEL ) 
	{
		// addRenderUnit�Ȱ͵� ����.
		::clearRenderUnit( this->m_hNodeMODEL );
	}

	// loadVisible�Ȱ͵� ����.
	for (short nP=0; nP<MAX_BODY_PART; nP++) 
	{
		DeleteSpecificPART( nP, m_phPartVIS[ nP ] );
		//m_pCharMODEL->DeletePART( nP, m_phPartVIS[ nP ] );
		m_phPartVIS[ nP ] = NULL;
	}	
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  char *szName �� �̸�
/// @brief  : ĳ������ �����𵨳�� ����.
//--------------------------------------------------------------------------------

bool CObjCHAR::LoadModelNODE (char *szName)
{
	HNODE hSkel  = this->Get_SKELETON ();
	if ( hSkel == NULL ) {
		LogString (LOG_DEBUG, "failed creat char.. SKEL == NULL !! \n");
		return false;
	}

	if ( !this->Set_CurMOTION( this->Get_MOTION(0) ) ) {		// default stop motion ..
		LogString (LOG_DEBUG, "failed creat char.. MOTION == NULL !! \n");
		return false;
	}

	m_hNodeMODEL = ::loadModel (szName, hSkel, this->Get_ZMOTION(), 1.0f);
	if ( m_hNodeMODEL ) 
	{
		::setCollisionLevel( m_hNodeMODEL, 4 );

		// �⺻ �������� ĳ���� �߽��� ����
		m_fHeightOfGround = ::getPositionZ (this->m_hNodeMODEL);
		m_iLastDummyIDX   = ::getNumDummies( m_hNodeMODEL ) - 1;

		::setScale   (m_hNodeMODEL,  m_fScale, m_fScale, m_fScale);
		::setPosition( m_hNodeMODEL, m_PosCUR.x, m_PosCUR.y, m_PosCUR.z );

		return true;
	}

	return false;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ĳ������ �����𵨳�� ����. �ܺο��� ��ϵ� ��� ����Ʈ �鵵 ����
//--------------------------------------------------------------------------------

void CObjCHAR::UnloadModelNODE ()
{
	/// �ܺο��� ��ϵ� ����Ʈ�鵵 �� ����..	
	ClearExternalEffect();
	

	if ( m_hNodeMODEL ) 
	{	
		::unloadModel (m_hNodeMODEL);
		m_hNodeMODEL = NULL;
	}	
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param char *szName			�̸�
/// @param CCharMODEL *pMODEL	ĳ���� �� ������ ������.. ��Ŭ����
/// @param short nCharPartCNT	ĳ���� ��Ʈ ī��Ʈ
/// @param D3DVECTOR &Position	������ ĳ���� ��ġ
/// @brief  : ĳ���� ����
///				1. ĳ���� �𵨳�� ����
///				2. ĳ���� ��Ʈ�� ����
///				3. �� ����Ʈ( ĳ���� ������ ������ ) ����
//--------------------------------------------------------------------------------

bool CObjCHAR::CreateCHAR (char *szName, CCharMODEL *pMODEL, short nCharPartCNT, const D3DVECTOR &Position)
{
	m_bIsVisible = false;

	m_PosBORN	 = Position;
	m_PosCUR     = Position;
	m_pCharMODEL = pMODEL;

	if ( pMODEL == NULL || nCharPartCNT <= 0 )	return false;
	
	if ( this->LoadModelNODE (szName) ) 
	{
		this->CreatePARTS( szName );

		m_ppBoneEFFECT = m_pCharMODEL->CreateBoneEFFECT( m_hNodeMODEL );
		this->InsertToScene ();

		this->SetCMD_STOP();

		DropFromSky( Position.x, Position.y );
	 	return true;
	}
	
	return false;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ĳ���� ����
///				1. �� ����Ʈ( ĳ���� ������ ������ ) ����
///				2. ĳ���� ��Ʈ�� ����
///				3. ĳ���� �𵨳�� ����
///				4. ���� ��ü ����( visiable )
//--------------------------------------------------------------------------------

void CObjCHAR::DeleteCHAR (void)
{
	this->RemoveFromScene ();

	m_pCharMODEL->DeleteBoneEFFECT( m_ppBoneEFFECT );

	short nP;
	for (nP=0; nP<MAX_BODY_PART; nP++)
		this->Del_EFFECT( nP );

	this->DeletePARTS ();
	this->UnloadModelNODE ();

	for (short nI=0; nI<MAX_BODY_PART; nI++) 
	{
		SAFE_DELETE_ARRAY( m_phPartVIS[ nI ] );
	}

	if( m_ClanMarkUserDefined )
	{
		m_ClanMarkUserDefined->Release();
		m_ClanMarkUserDefined = NULL;
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  t_HASHKEY HashMOTION ����� �ؽ�Ű
/// @param  float fMoveSpeed	 �̵� �ӵ�
/// @param  int iRepeatCnt		 �ݺ�ī��Ʈ
/// @brief  : ������� ����
//--------------------------------------------------------------------------------

void CObjCHAR::Set_UserMOITON (t_HASHKEY HashMOTION, float fMoveSpeed, int iRepeatCnt )
{
	tagMOTION *pMotion = g_MotionFILE.KEY_GetMOTION( HashMOTION );

	Set_UserMOITON( pMotion, fMoveSpeed, iRepeatCnt );
}

//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  tagMOTION* pMotion   ���.
/// @param  float fMoveSpeed	 �̵� �ӵ�
/// @param  int iRepeatCnt		 �ݺ�ī��Ʈ
/// @brief  : ������� ����
//--------------------------------------------------------------------------------

void CObjCHAR::Set_UserMOITON (tagMOTION *pMotion, float fMoveSpeed, int iRepeatCnt )
{	
	// ���� �������� ��ǰ� ������� ����� ������Ʈ ���� �����Ƿ� 
	// �̵� �ӵ� ������ Chg_CurMOTION������ ����..
	this->Set_ModelSPEED( fMoveSpeed );

	if ( this->Chg_CurMOTION( pMotion ) ) 
	{

#ifndef __VIRTUAL_SERVER
		_ASSERT( fMoveSpeed >= 0.f && fMoveSpeed < 2000.f );
#endif

		::attachMotion		( this->m_hNodeMODEL, this->Get_ZMOTION()  );
		::setAnimatableSpeed( this->m_hNodeMODEL, 1.0f  );
		::setRepeatCount( this->GetZMODEL(), iRepeatCnt );		

		// �� �ִϰ� ���� ��쿡�� �޽� �ִϰ� ������ �ֳ�????
		this->m_pCharMODEL->SetMeshMOTION( m_phPartVIS, this->Get_ActionIDX() );
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  short nActionIdx ����� �ؽ�Ű
/// @param  float fMoveSpeed	 �̵� �ӵ�
/// @param  float fAniSpeed		 �ݺ�ī��Ʈ
/// @param  float bool bAttackMotion	 �ݺ�ī��Ʈ
/// @brief  : ��� ����
/// @bug ÷�� ���⿡ ���ݽ��� ���� �־��ٰ�.. �����ӿ� ������ �ְ� ��Ȯ�� �����ӿ��� �����ϰ� �Ű��.
//--------------------------------------------------------------------------------

void CObjCHAR::Set_MOTION (short nActionIdx, float fMoveSpeed, float fAniSpeed, bool bAttackMotion, int iRepeatCnt )
{	

	if ( !bAttackMotion && m_bAttackSTART )	{
		Attack_END ();
	}


	if ( this->Chg_CurMOTION( this->Get_MOTION (nActionIdx) ) ) 
	{

#ifndef __VIRTUAL_SERVER
//		_ASSERT( fMoveSpeed >= 0.f && fMoveSpeed < 2000.f );
#endif
	
		this->Set_ModelSPEED( fMoveSpeed );

		::attachMotion		( this->m_hNodeMODEL, this->Get_ZMOTION()  );
		::setAnimatableSpeed( this->m_hNodeMODEL, fAniSpeed  );		
		::setRepeatCount	( this->m_hNodeMODEL, iRepeatCnt  );		

		// �� �ִϰ� ���� ��쿡�� �޽� �ִϰ� ������ �ֳ�????
		this->m_pCharMODEL->SetMeshMOTION( m_phPartVIS, this->Get_ActionIDX() );
	}
	
	::controlAnimatable ( this->m_hNodeMODEL, 0 );
	::controlAnimatable ( this->m_hNodeMODEL, 1 );
	
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  D3DXVECTOR3& Pos ������ ��ġ
/// @brief  : ������ġ ����
//--------------------------------------------------------------------------------

void CObjCHAR::ResetCUR_POS( D3DXVECTOR3& Pos )
{
	CGameOBJ::Set_CurPOS( Pos );
	::setPositionVec3( m_hNodeMODEL, m_PosCUR );		
}

//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  CObjCHAR *pAtkOBJ ������
/// @param  WORD wDamage      ������
/// @brief  : ������ ���� ó��
//--------------------------------------------------------------------------------

void CObjCHAR::Apply_DAMAGE (CObjCHAR *pAtkOBJ, WORD wDamage)
{
	if ( this->Get_HP() <= DEAD_HP )
		return;

	if( IsA( OBJ_USER ) )///���� �������� �Ծ��ٸ�
		g_pAVATAR->SetBattleTime( g_GameDATA.GetGameTime() );

	if( pAtkOBJ && pAtkOBJ->IsA( OBJ_USER ) )///���� �����ߴٸ�
		g_pAVATAR->SetBattleTime( g_GameDATA.GetGameTime() );


	uniDAMAGE Damage;
	Damage.m_wDamage = wDamage;


	if ( Damage.m_wACTION & DMG_ACT_DEAD ) 
	{
		/// �ڽ��� �׾���.
		this->Do_DeadAI (pAtkOBJ, Damage.m_wVALUE );

		/// �����ڿ� ���� ���� ���̸�.. ��ȯ���� ������ó��.. �� �׷�.. �״� ����� �ϸ� �ȵȴ�.
		/// Ȥ���� �����ϰ�� ��������Ƽ�� ���õǸ� ������ ����� ���������� ���°�..�ϴ� User�ϰ��� �н�. -04/5/25
		m_bStopDead = ( pAtkOBJ == this );
		this->Dead ();
		if( m_bStopDead && ( this->IsA( OBJ_USER ) == false ) )
		{
			m_pChangeVisibility = new CChangeVisibility( this, 5000, true );			
			m_bStopDead = false;			
		}

#ifdef _DEBUG
		if( ( pAtkOBJ == this ) && (this->IsA( OBJ_USER )) )
			assert( 0 && "�� �����ڿ� ���� ������ �� ����? �� ������ �����̱�" );
#endif

		
		if ( pAtkOBJ ) 
		{
			//---------------------------------------------------------------------------------------
			/// �������� ��û�ÿ��� üũ�ϰ� ����..
			//---------------------------------------------------------------------------------------
			//if( this->IsA( OBJ_MOB) ) 
			//{
			//	if ( NPC_QUEST_TYPE( this->m_nCharIdx ) == 1 ) 
			//	{
			//		/// ��Ƽ ����Ʈ�� ?
			//		if ( CParty::GetInstance().IsPartyMember( g_pObjMGR->Get_ServerObjectIndex(pAtkOBJ->Get_INDEX()) ) ||
			//			( pAtkOBJ->IsA( OBJ_USER ) && g_itMGR.IsDlgOpened( DLG_TYPE_PARTY ) ) )
			//		{
			//			/// ���� ��Ƽ ����ΰ�, ���� �ɸ��� �ڽ��̰� �ڽ��� ��Ƽ���̳� ???
			//			Do_DeadEvent(pAtkOBJ);
			//		}
			//	} else
			//	if ( pAtkOBJ->IsA( OBJ_USER ) ) {
			//		/// ���� ����Ʈ��...
			//		Do_DeadEvent(pAtkOBJ);	
			//	}
			//}
			if( g_pAVATAR )
			{
				if( g_pAVATAR->Get_INDEX() == pAtkOBJ->Get_INDEX() )
				{				
					switch( this->Get_TYPE() )
					{
						case OBJ_MOB:
							sprintf( g_MsgBuf, F_STR_SUCCESS_HUNT, this->Get_NAME() );
							break;

						case OBJ_AVATAR:
							sprintf( g_MsgBuf, F_STR_WIN_PVP, this->Get_NAME() );
							break;
					}				

					g_itMGR.AppendChatMsg( g_MsgBuf, IT_MGR::CHAT_TYPE_SYSTEM );
				}
			}

			if ( pAtkOBJ->m_iServerTarget == g_pObjMGR->Get_ServerObjectIndex( this->Get_INDEX() ) )	// Ÿ�� ����.
				pAtkOBJ->m_iServerTarget = 0;

			/// �׿����� �ΰ����� ����...
			pAtkOBJ->Do_KillAI( this, Damage.m_wVALUE );

//---------------------------------------------------------------------------------------------
#if defined(_GBC)			
			//����ȣ::�����϶��� ȸ���� ���� �ʴ´�.
			if(GetPetMode() < 0)
				Set_ModelDIR( pAtkOBJ->Get_CurPOS() );
#else 
	Set_ModelDIR( pAtkOBJ->Get_CurPOS() );
#endif 
//---------------------------------------------------------------------------------------------
 


			//----------------------------------------------------------------------------------
			/// �г�Ƽ ����ġ ����
			//----------------------------------------------------------------------------------
			/// ���� �������
			if( this->IsA( OBJ_USER ) == true )
			{
				int iPanalEXP = 0;

				/// ���Ͷ��
				if( pAtkOBJ->IsA( OBJ_MOB ) ) 
				{
					/// ���� ���Ͷ�� ����ġ ���� ����
					if( pAtkOBJ->m_EndurancePack.GetStateFlag() & ING_DEC_LIFE_TIME )
					{
						iPanalEXP = 0;
					}else
					{
						g_pAVATAR->Set_PenalEXP( 3 );
					}					
				}
			}			
		}

		//------------------------------------------------------------------------------------
		/// delayed exp 
		//------------------------------------------------------------------------------------
		CDelayedExp::GetSingleton().GetExp( this->Get_INDEX() );

		
		/*
		������ �̺�Ʈ�� CObjCHAR::Dead �� �̵�..
		*/
		
		if( !m_FieldItemList.empty() )
			DropFieldItemFromList();

		this->m_lDeadTIME = 0;

	} else {
		this->Do_DamagedAI( pAtkOBJ, Damage.m_wVALUE );

		if ( this->Sub_HP( Damage.m_wVALUE ) <= 0 ) 
		{
			this->Set_HP( 1 );
		}

	}

	if ( pAtkOBJ ) 
	{
		if ( Damage.m_wACTION & DMG_ACT_CRITICAL ) 
		{
			// ũ��Ƽ�� ������ !!!
			// AddMsgToChatWND( CStr::Printf ("Critical:: %s�� %s���� ���� %d ����", pAtkOBJ->Get_NAME(), this->Get_NAME (), Damage.m_wVALUE ), g_dwRED );
		} else {
			// �Ϲ� ������ ...
			// AddMsgToChatWND( CStr::Printf ("%s�� %s���� ���� %d ����", pAtkOBJ->Get_NAME(), this->Get_NAME (), Damage.m_wVALUE ), g_dwRED );
		}

		if ( Damage.m_wDamage & DMG_BIT_HITTED ) 
		{
			// �´³� �´� �������� ���� �����ϸ�...
			/// ��ź �����̸� �´� ������ ���� �ʴ´�.( 2..4/12/10 �´������� �Ұ�� ������ �ϴٰ� ĳ���� �����ȴ�. )
			if( this->GetPetMode() > 0 )
			{
				
			}else
			{
				if ( !(this->Get_STATE() & CS_BIT_INT2) ) 
				{
					this->Set_MOTION( this->GetANI_Hit() );
					this->Set_STATE( CS_HIT );
				}
			}
		}
	}
	
//#ifdef	_DEBUG
//	if ( this->IsA( OBJ_USER ) ) {
//		if ( pAtkOBJ ) {
//			int iDist = CD3DUtil::distance ((int)pAtkOBJ->m_PosCUR.x, (int)pAtkOBJ->m_PosCUR.y, 
//											(int)this->m_PosCUR.x, (int)this->m_PosCUR.y);
//
//			AddMsgToChatWND( CStr::Printf ("�Ÿ� %d [%.0f, %.0f] ~ %s[%d] [ %.0f, %.0f ]", 
//					iDist,
//					this->m_PosCUR.x, this->m_PosCUR.y,
//					pAtkOBJ->Get_NAME(),
//					g_pObjMGR->Get_ServerObjectIndex( pAtkOBJ->Get_INDEX() ),
//					pAtkOBJ->m_PosCUR.x, pAtkOBJ->m_PosCUR.y), g_dwGREEN );
//		} else {
//			;
//		}
//	}
//#endif
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  CObjCHAR *pTarget Ÿ�� ������Ʈ
/// @brief  : ��ų ��� ����. ���⿡ ���ܻ��� ���� �Ѵٴ���..( ���ܻ��� Attack_Start ������ �ٿ����� .. ������ )
//--------------------------------------------------------------------------------

bool CObjCHAR::Skill_START (CObjCHAR *pTarget)
{
	if ( m_nActiveSkillIDX ) 
	{
		/// ���� ��� �ߵ� ��ų�� ���ܻ� �ߵ� )
		if( SKILL_TYPE( m_nActiveSkillIDX ) == SKILL_ACTION_IMMEDIATE )
		{
			// ���ܻ� ����.
			for (short nI=0; nI<2; nI++) 
			{
				if ( m_hTRAIL[ nI ] ) 
				{
					::controlTrail ( m_hTRAIL[ nI ], 0 );	// stop & clear
					::controlTrail ( m_hTRAIL[ nI ], 1 );	// start
				}
			}

		}

		return true;
	}

	return false;	
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  CObjCHAR *pTarget Ÿ�� ������Ʈ
/// @brief  : ���� ����.. ���ܻ� �ִϸ��̼� ����..
//--------------------------------------------------------------------------------

bool CObjCHAR::Attack_START (CObjCHAR *pTarget)
{
	/*
	// ���� ���̱� / ���ܻ�...
	CObjCHAR *pTarget = g_pObjMGR->Get_CharOBJ( this->m_iTargetObject );
	int iBulletIDX = Get_BulletNO ();
	if ( pTarget && iBulletIDX ) {
		g_pBltMGR->Add_BULLET( this, pTarget, iBulletIDX );
	}
	*/

	// ���ܻ� ����.
	for (short nI=0; nI<2; nI++) 
	{
		if ( m_hTRAIL[ nI ] ) 
		{
			::controlTrail ( m_hTRAIL[ nI ], 0 );	// stop & clear
			::controlTrail ( m_hTRAIL[ nI ], 1 );	// start
		}
	}

	m_bAttackSTART  = true;
	m_iActiveObject = g_pObjMGR->Get_ClientObjectIndex( m_iServerTarget );

	_ASSERT( m_iServerTarget == g_pObjMGR->Get_ServerObjectIndex( m_iActiveObject ) );


	/// ���� �Ӽ��� ���ݽ����ϸ� Ǯ���� �Ǵ°͵�
	m_EndurancePack.ClearStateByAttack();


	return true;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  bool bStopTrail ���ܻ� ����?
/// @brief  : ���� ��.
//--------------------------------------------------------------------------------

void CObjCHAR::Attack_END (bool bStopTrail)
{
	// �⺻ �ִϸ��̼� �ӵ� ����.
	::setAnimatableSpeed ( GetZMODEL(), 1.0 );

	// ���ܻ� ȿ�� �������..
	if ( bStopTrail ) 
	{
		for (short nI=0; nI<2; nI++) 
		{
			if ( m_hTRAIL[ nI ] ) 
			{
				::controlTrail ( m_hTRAIL[ nI ], 3 );	// no spawn
			}
		}
	}

	m_bAttackSTART = false;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  CObjCHAR *pTarget Ÿ�� ������Ʈ
/// @brief  : ĳ���� ���۽���..( ��ų�� ������ ǥ���Ѵ�. ) ���� �߿�../
///				�����ϰ��� Ÿ�̸Ӹ� �����Ѵ�. ��ų�� �����̰� �ֱ⶧����.
//--------------------------------------------------------------------------------

bool CObjCHAR::Casting_START (CObjCHAR *pTarget)
{
	SetCastingState( true );
	/// m_bProcEffectedSkill = false;

	//--------------------------------------------------------------------------------
	/// �����ϰ��� Ÿ�̸Ӹ� �����Ѵ�.
	//--------------------------------------------------------------------------------
	//if( this->IsA( OBJ_USER ) )
	//{
	//	/// Ÿ�̸� ����
	//	CSkillSlot* pSkillSlot = g_pAVATAR->GetSkillSlot();
	//	CSkill* pSkill = pSkillSlot->GetSkillBySkillIDX( m_nToDoSkillIDX );
	//	if( pSkill )
	//	{
	//		pSkill->SetSkillDelayTime( SKILL_RELOAD_TIME( m_nToDoSkillIDX ) * 200 );
	//	}
	//}

	/// ���� �Ӽ��� ���ݽ����ϸ� Ǯ���� �Ǵ°͵�
	if( SKILL_TYPE( m_nToDoSkillIDX ) == SKILL_ACTION_IMMEDIATE ||
		SKILL_TYPE( m_nToDoSkillIDX ) == SKILL_ACTION_FIRE_BULLET ||
		SKILL_TYPE( m_nToDoSkillIDX ) == SKILL_ACTION_SELF_DAMAGE )
	{
		m_EndurancePack.ClearStateByAttack();
	}

	return true;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ĳ���� ���۳�..( ��ų�� ���� ǥ���Ѵ�. ) ���� �߿�../
//--------------------------------------------------------------------------------

void CObjCHAR::Casting_END ()
{
	/// ���� Ȱ��ȭ�� ��ų�� �ִٸ�.. ĳ���� ���� ����..	
	SetCastingState( false );
	m_nActiveSkillIDX = 0;
	m_nToDoSkillIDX = 0;	
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ���� ����
//--------------------------------------------------------------------------------
void CObjCHAR::MoveStart ()
{
	/// �̵��� ���ÿ� �ڵ����� Ǯ���� �ϴ°͵�..
	m_EndurancePack.ClearStateByMove();
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : �Ѿ˹�ȣ�� ���Ѵ�.
//--------------------------------------------------------------------------------

int CObjCHAR::Get_BulletNO ()
{
	short nWeaponItem = Get_R_WEAPON();
	if( nWeaponItem )
		return WEAPON_BULLET_EFFECT( nWeaponItem );

	return 0;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : �״� ó��..( ������ ĳ������ ������ ��ƾ�� ����.. �̰͵� ����� �߿� )
///				m_nActiveSkillIDX �� ����.. �׾������.. �� �÷��װ� ���õǾ� ������. Casting_End �� ��ȿ������.
//--------------------------------------------------------------------------------

void CObjCHAR::Dead ()
{
	if ( Get_HP() <= DEAD_HP )
	{
		SetCastingState( false );
		return;
	}


	//----------------------------------------------------------------------------------------------------
	/// �ƹ�Ÿ�� �������
	//----------------------------------------------------------------------------------------------------	
	if( this->Is_AVATAR() )
	{
		CObjAVT* pAVT = ( CObjAVT* )this;
		/// Pet ���¶�� ������.
		if( GetPetMode() >= 0 )
		{			
			pAVT->DeleteCart( false );

			//----------------------------------------------------------------------------------------------------
			/// ������ ���� �ı� ����Ʈ ���..
			//----------------------------------------------------------------------------------------------------	
			if( pAVT->IsVisible() )
			{
				int iEffectNO	= PAT_DEAD_EFFECT( pAVT->m_sBodyIDX.m_nItemNo );
				int iSoundNO	= PAT_DEAD_SOUND( pAVT->m_sBodyIDX.m_nItemNo );
				
				this->ShowEffectOnCharByIndex( iEffectNO, iSoundNO );				
			}

			m_btMoveMODE = MOVE_MODE_RUN;
			pAVT->Update_SPEED();					
		}
		else
		{
			//----------------------------------------------------------------------------------------------------
			/// ĳ���� �������� ����Ʈ ���
			//----------------------------------------------------------------------------------------------------	
			SE_CharDie( this->Get_INDEX() );
		}

		/// ���λ��� ���� ����..
		if( pAVT->IsPersonalStoreMode() )
		{
			pAVT->SetPersonalStoreTitle( NULL );
			g_UIMed.SubPersonalStoreIndex( this->Get_INDEX() );
		}


		/// ���� ���� ���� �Ŷ��..
		if( this->IsA( OBJ_USER ) )
		{
			(( CObjUSER*)pAVT)->ClearSummonedMob();
			g_itMGR.ChangeState( IT_MGR::STATE_DEAD );
		}

	}
		
	//----------------------------------------------------------------------------------------------------
	/// ���� ������� ����Ʈ ���
	//----------------------------------------------------------------------------------------------------	
	if( this->IsA( OBJ_MOB ) )
	{
		///SE_MobDie( this->Get_INDEX() );
		int iMobDeadEffect = NPC_DEAD_EFFECT( this->Get_CharNO() );
		int iSoundIDX = NPC_DIE_SOUND( this->Get_CharNO() );		
		this->ShowEffectOnCharByIndex( iMobDeadEffect, iSoundIDX );		

	}

	Set_HP( DEAD_HP );
	SetCMD_DIE ();


	m_nActiveSkillIDX = 0;
	SetCastingState( false );
	
	m_EndurancePack.ClearEntityPack();
	ClearAllEntityList();

	// ��������.
	::setRepeatCount( m_hNodeMODEL, 1 );
	// �������� Ŭ�� �ȵǰ�...

	if( this->Is_AVATAR() == false )
	{
		/// 2004-11-26 ������ Ŭ���ǰ�...
		::setCollisionLevel( m_hNodeMODEL, 0 );
	}


	::setModelBlinkCloseMode( this->GetZMODEL(), true );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param int iSkillIDX ��ų �ε���
/// @param gsv_DAMAGE_OF_SKILL EffectedSkill ��ų ��� ����
/// @param bool bDamageOfSkill	������ ����ΰ�?( ���������� ��ų�� ���.. )
/// @brief  : ��ų����� �׼������ӿ� �����ϱ� ���� ¯�ھ� �д�.
//--------------------------------------------------------------------------------

void CObjCHAR::PushEffectedSkillToList( int iSkillIDX, gsv_DAMAGE_OF_SKILL EffectedSkill, int iCasterINT, bool bDamageOfSkill )
{
	stEFFECT_OF_SKILL steffectOfSkill;
	
	steffectOfSkill.m_dwCreateTime	= g_GameDATA.GetGameTime();
	steffectOfSkill.iSkillIDX		= iSkillIDX;
	steffectOfSkill.bDamageOfSkill	= bDamageOfSkill;
	steffectOfSkill.iCasterINT		= iCasterINT;

	steffectOfSkill.EffectOfSkill = EffectedSkill;
	m_EffectedSkillList.push_back( steffectOfSkill ); 
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : Ÿ�Ӿƿ� �ð��� ���� ��ų����� ó���Ѵ�.
//--------------------------------------------------------------------------------
const int SKILL_PROC_LIMIT = 1000 * 10;
void CObjCHAR::ProcTimeOutEffectedSkill()
{
	stEFFECT_OF_SKILL*	pEffectOfSkill	= NULL;	
	DWORD dwElapsedTime = 0;

	std::vector< stEFFECT_OF_SKILL >::iterator begin = m_EffectedSkillList.begin();
	for( ; begin != m_EffectedSkillList.end(); )
	{		
		pEffectOfSkill	= &( *begin );
     
		dwElapsedTime = g_GameDATA.GetGameTime() - pEffectOfSkill->m_dwCreateTime;
		if( dwElapsedTime > SKILL_PROC_LIMIT )
		{
			ProcOneEffectedSkill( pEffectOfSkill );
			begin = m_EffectedSkillList.erase( begin );
#ifdef _DEBUG
			
			sprintf( g_MsgBuf, "ProcTimeOutEffectedSkill [ ��� : %s ] ", this->Get_NAME() );
			///assert( 0 && Buf );
			MessageBox( NULL, g_MsgBuf, "WARNING", MB_OK );
#endif //_DEBUG
		}else
			++begin;
	}
}

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief /// Damage of skill �� �Ƿ����� ��ų������ �ݿ�..
//----------------------------------------------------------------------------------------------------

void CObjCHAR::ProcEffectOfSkillInDamageOfSkill( int iSkillIDX, int iObjIDX, CObjCHAR* pChar, stEFFECT_OF_SKILL*	pEffectOfSkill )
{
	int iSkillType = SKILL_TYPE( iSkillIDX );

	switch( iSkillType )
	{
		case SKILL_ACTION_SELF_AND_TARGET:
			{
				if( this->IsA( OBJ_USER ) )
				{
					for( int i = 0; i < SKILL_INCREASE_ABILITY_CNT; i++ )
					{
						if( ( 0x01 << i ) & pEffectOfSkill->EffectOfSkill.m_btSuccessBITS )
						{
							int iAbilityType = SKILL_INCREASE_ABILITY( iSkillIDX, i );
							switch( iAbilityType )
							{
								case AT_HP:
									g_pAVATAR->Add_HP( SKILL_INCREASE_ABILITY_VALUE( iSkillIDX, i ) );
									break;
								case AT_MP:
									g_pAVATAR->Add_MP( SKILL_INCREASE_ABILITY_VALUE( iSkillIDX, i ) );
									break;

								case AT_STAMINA:
									g_pAVATAR->AddCur_STAMINA( SKILL_INCREASE_ABILITY_VALUE( iSkillIDX, i ) );
									break;

							}
						}
					}
				}
			}
			break;

		default:
			ApplyEffectOfSkill( iSkillIDX, iObjIDX, pChar, pEffectOfSkill );
			break;
	}	
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief ������ ĳ���Ϳ� ��ų ����..
//----------------------------------------------------------------------------------------------------

void CObjCHAR::ApplyEffectOfSkill( int iSkillIDX, int iObjIDX, CObjCHAR* pEffectedChar, stEFFECT_OF_SKILL*	pEffectOfSkill )
{
	if( pEffectOfSkill->EffectOfSkill.m_btSuccessBITS == 0 )/// ���� ȿ���� �ٷ� ����..�� ��ų ���� ���д�
	{
		/// ���� ������ ��ų�ϰ�쿡�� ��ų ���� ���� �޽����� ǥ���Ѵ�.
		int iClientObjIndex = g_pObjMGR->Get_ClientObjectIndex( pEffectOfSkill->EffectOfSkill.m_wSpellObjIDX );
		if( iClientObjIndex == g_pAVATAR->Get_INDEX() )
		{
			///AddMsgToChatWND( STR_SKILL_APPLY_FAILED, g_dwRED ,CChatDLG::CHAT_TYPE_SYSTEM);
			;
		}		
	}else
	{
		///��ų�� �������� ��� �ִ� 2���� ���±��� �ٲ�� �����Ƿ�
		for( int i = 0 ; i < 2 ; i++ ) 
		{			
			if ( ( 0x01 << i ) & pEffectOfSkill->EffectOfSkill.m_btSuccessBITS ) 
			{
				int iStateIndex = 0;

				/// �������� �ƴ� �ܼ� �ɷ�ġ �����..
				/*if( SKILL_TYPE( iSkillIDX ) != SKILL_ACTION_SELF_BOUND &&
					SKILL_TYPE( iSkillIDX ) != SKILL_ACTION_TARGET_BOUND )*/
				{
					iStateIndex = SKILL_STATE_STB( iSkillIDX, i );

					/// �������� ����, �Ҹ����� ���� ���� ���� ���� ��ų��
					if( iStateIndex && STATE_TYPE( iStateIndex ) > ING_CHECK_END )
					{
						pEffectedChar->ProcFlushStateSkill( iStateIndex );
						continue;
					}
				}
				
				if( iStateIndex != 0 )
				/// ������ ��ų�̶��..
				/*if( SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_SELF_BOUND_DURATION ||
					SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_TARGET_BOUND_DURATION ||
					SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_SELF_STATE_DURATION ||
					SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_TARGET_STATE_DURATION )*/
				{
					/// �ϴ� �����ϰ�츸 �Ӽ���ü �߰�..
					//if( pChar->IsA( OBJ_USER ) )
					pEffectedChar->AddEnduranceEntity( iSkillIDX,	iStateIndex, SKILL_DURATION( iSkillIDX ), ENDURANCE_TYPE_SKILL ) ;

					/// ���� Ÿ��..
					int iStateType = STATE_TYPE( iStateIndex );
					/// ���� ��ȣ�� 1,2 ���ΰ�쿡�� LIST_STATUS.STB �� ���� �����ϰ�
					if( iStateType == ING_INC_HP || iStateType == ING_INC_MP || iStateType == ING_POISONED )
						pEffectedChar->m_EndurancePack.SetStateValue( iStateType, STATE_APPLY_ABILITY_VALUE( iStateIndex, i ) );
					else
					{
						int iIncValue = 0;					
						/// 04/4/24
						if( pEffectedChar->IsA( OBJ_USER ) )
						{
							iIncValue = CCal::Get_SkillAdjustVALUE( ( CObjUSER* )pEffectedChar, iSkillIDX, i, pEffectOfSkill->iCasterINT );
						}
						else							
						{
							iIncValue = 1;

							/// ������ �ƴҰ�� �˼��� ����. �����ϰ�쿡�� ����, �̼Ӹ� ����غ���.
							if( pEffectedChar->IsA( OBJ_MOB ) )
							{
								int iAbilityValue = 0;
								switch( SKILL_INCREASE_ABILITY( iSkillIDX, i ) )
								{
									case AT_SPEED:
										iAbilityValue = pEffectedChar->GetOri_RunSPEED();
										break;
									case AT_ATK_SPD:
										iAbilityValue = pEffectedChar->GetOri_ATKSPEED();
										break;
								}


								iIncValue = (short)( iAbilityValue * SKILL_CHANGE_ABILITY_RATE( iSkillIDX, i ) / 100.f + SKILL_INCREASE_ABILITY_VALUE( iSkillIDX, i ) );
							}	

							//--------------------------------------------------------------------------------------------
							/// �ٸ� �ƹ�Ÿ�ϰ��.. MAX_HP�� ����� �Ѵ�..
							if( pEffectedChar->IsA( OBJ_AVATAR ) )
							{
								int iAbilityValue = 0;
								switch( SKILL_INCREASE_ABILITY( iSkillIDX, i ) )
								{
									case AT_MAX_HP:
										iAbilityValue = pEffectedChar->Get_MaxHP();
										break;								
								}

								iIncValue = (short)( iAbilityValue * SKILL_CHANGE_ABILITY_RATE( iSkillIDX, i) / 100.f + SKILL_INCREASE_ABILITY_VALUE( iSkillIDX, i) * ( pEffectOfSkill->iCasterINT + 300 ) / 315.f );								
							}
							//--------------------------------------------------------------------------------------------

						}
						

						pEffectedChar->m_EndurancePack.SetStateValue( iStateType, iIncValue );

						/// ���½�ų�� �ɸ���� AVATAR �� ���� �ɷ�ġ ������Ʈ..
						if( pEffectedChar->IsA( OBJ_USER ) )
						{
							(( CObjUSER* )pEffectedChar)->UpdateAbility();
						}
					}


				}else if( SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_SELF_BOUND ||
							SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_TARGET_BOUND )
				{
					/// hp �� ���� ����..
					int iIncValue = CCal::Get_SkillAdjustVALUE( ( CObjUSER* )pEffectedChar, iSkillIDX, i, pEffectOfSkill->iCasterINT );

					switch( SKILL_INCREASE_ABILITY( iSkillIDX, i ) )
					{
						case AT_HP:
							pEffectedChar->Add_HP( iIncValue );
							break;
						case AT_MP:						
							pEffectedChar->Add_MP( iIncValue );
							break;
						case AT_STAMINA:
							/// ���½�ų�� �ɸ���� AVATAR �� ���� �ɷ�ġ ������Ʈ..
							if( pEffectedChar->IsA( OBJ_USER ) )
							{
								(( CObjUSER* )pEffectedChar)->AddCur_STAMINA( SKILL_INCREASE_ABILITY_VALUE( iSkillIDX, i ) );
							}	
							break;
						default:
							g_itMGR.AppendChatMsg( "�����°ų� �߰��ض�..", IT_MGR::CHAT_TYPE_SYSTEM);
							break;
					}
				}			
			} 
		}
	}
}

//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param stEFFECT_OF_SKILL*	pEffectOfSkill ��ų ��� �ϳ� ó��..
/// @brief  : ��ų ��� ó��
//--------------------------------------------------------------------------------

void CObjCHAR::ProcOneEffectedSkill( stEFFECT_OF_SKILL*	pEffectOfSkill )
{
	int iSkillIDX		= pEffectOfSkill->iSkillIDX;
	int iObjIDX			= pEffectOfSkill->EffectOfSkill.m_wObjectIDX;

	CObjCHAR *pChar = g_pObjMGR->Get_ClientCharOBJ( iObjIDX, true );

	if( pChar == NULL )
		return;

			/*
	*	���� ������ ��� m_nResultVALUE �� ������ �̴�.
	*/
	if( pEffectOfSkill->bDamageOfSkill )
	{
		D3DXVECTOR3	pos = pChar->Get_CurPOS();

		pChar->Apply_DAMAGE( this, pEffectOfSkill->EffectOfSkill.m_Damage.m_wDamage );	

		/// Damage of skill �� �Ƿ����� ��ų������ �ݿ�..
		ProcEffectOfSkillInDamageOfSkill( iSkillIDX, iObjIDX, pChar, pEffectOfSkill );

		/// ��ų Ÿ�� ����Ʈ
		// Ÿ�� ȿ��.
		if ( pChar->IsVisible() ) 
		{
			g_UIMed.CreateDamageDigit( pEffectOfSkill->EffectOfSkill.m_Damage.m_wVALUE, pos.x, pos.y, pos.z + pChar->m_fStature, pChar->IsA( OBJ_USER ) );

			int iEffectIDX = SKILL_HIT_EFFECT( iSkillIDX );
			if ( iEffectIDX  ) 
			{					
				CEffect *pHitEFT = g_pEffectLIST->Add_EffectWithIDX( iEffectIDX, true );
				if ( pHitEFT ) 
				{
					if( SKILL_HIT_EFFECT_LINKED_POINT( iSkillIDX ) == INVALID_DUMMY_POINT_NUM )
					{
						pHitEFT->LinkNODE ( pChar->GetZMODEL() );
					}else
					{
						pChar->LinkDummy( pHitEFT->GetZNODE(), SKILL_HIT_EFFECT_LINKED_POINT( iSkillIDX ) );
					}							

					pHitEFT->SetParentCHAR( pChar );
					pChar->AddExternalEffect( pHitEFT );					
	
					pHitEFT->UnlinkVisibleWorld ();
					pHitEFT->InsertToScene ();
				}					

				if ( SKILL_HIT_SOUND( iSkillIDX ) ) 
				{
					if ( pChar->IsUSER() ) 
					{ // �ڱ� �ƹ�Ÿ�� ��쿡�� ��3D ���� ���.
						g_pSoundLIST->IDX_PlaySound( SKILL_HIT_SOUND( iSkillIDX ) );
					}
					else 
					{
						g_pSoundLIST->IDX_PlaySound3D( SKILL_HIT_SOUND( iSkillIDX ), pChar->Get_CurPOS() );
					}						
				}			
			}
		}

	}else /// ���Ӽ��̰ų�.. ���¸� �ٲٴ� ��ų..
	{		
		ApplyEffectOfSkill( iSkillIDX, iObjIDX, pChar, pEffectOfSkill );
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ������ ��� ��ų������� �̺�Ʈ �����ӿ� ó���Ѵ�.
//--------------------------------------------------------------------------------

bool CObjCHAR::ProcEffectedSkill()
{
	bool bResult = false;

	stEFFECT_OF_SKILL*	pEffectOfSkill	= NULL;	


	for( int i = 0; i < m_EffectedSkillList.size(); i++ )
	{		
		pEffectOfSkill	= &( m_EffectedSkillList[ i ] );
        
		ProcOneEffectedSkill( pEffectOfSkill );

		bResult = true;
	}

	/// ó���� ��� ������ ��ų��� Ŭ����	
    m_EffectedSkillList.clear();
	
	SetEffectedSkillFlag( false );
	Log_String( LOG_NORMAL, "ProcEffectedSkill\n" );

	return bResult;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param int iattacker ������
/// @param uniDAMAGE wDamage	���� ������
/// @brief  : �������� ����Ʈ�� �߰���..
//--------------------------------------------------------------------------------

void CObjCHAR::PushDamageToList( int iAttacker, uniDAMAGE wDamage )
{ 
	stAPPLY_DAMAGE applyDamage;
	applyDamage.m_Damage			= wDamage;
	applyDamage.m_iAttacker			= iAttacker;
	applyDamage.m_dwCreateTime		= g_GameDATA.GetGameTime();

	m_DamageList.push_back( applyDamage ); 
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param int iattacker ������
/// @brief  : ���� ������ �������� ������ ����Ʈ���� ������.( �����ڰ� ���� �������� ���� )
///				�׼��� �������� ������...^^; �׼ǵ� ������Ű�� �ٶ��� ���װ� �����..
//--------------------------------------------------------------------------------

uniDAMAGE CObjCHAR::PopCurrentAttackerDamage( int iAttacker )
{
	uniDAMAGE wDamage;
	wDamage.m_wDamage = 0;	

	std::vector< stAPPLY_DAMAGE >::iterator begin = m_DamageList.begin();

	for( ; begin != m_DamageList.end() ; )
	{	
		if( (*(begin)).m_iAttacker == iAttacker )
		{
			wDamage.m_wVALUE += (*(begin)).m_Damage.m_wVALUE;
			wDamage.m_wACTION |= (*(begin)).m_Damage.m_wACTION;

			begin = m_DamageList.erase( begin );
		}else
		{
			 ++begin;
		}
	}	

	return wDamage;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param int& iMaxDamage
/// @brief  : ���� ����Ʈ�� ��� �������� ���ؼ� ����..
//--------------------------------------------------------------------------------

uniDAMAGE CObjCHAR::PopTotalDamageFromList( int& iMaxDamage )
{
	uniDAMAGE wDamage;
	wDamage.m_wDamage = 0;
	iMaxDamage = 0;

	for( int i =0; i < m_DamageList.size() ; i++ )
	{
		/// �ϴ� Ÿ��ġ ����� ���� ū��..
		if( m_DamageList[ i ].m_Damage.m_wVALUE > iMaxDamage )
			iMaxDamage = m_DamageList[ i ].m_Damage.m_wVALUE;

		wDamage.m_wVALUE += m_DamageList[ i ].m_Damage.m_wVALUE;
		wDamage.m_wACTION |= m_DamageList[ i ].m_Damage.m_wACTION;
	}
	m_DamageList.clear();

	return wDamage;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param 
/// @brief  : ��� ������ ó��( �װų�.. �� �׷� ��Ȳ����.. ���� )
//--------------------------------------------------------------------------------

void CObjCHAR::ClearAllDamage()
{
	uniDAMAGE wDamage;
	
	DWORD dwCurrentTime = g_GameDATA.GetGameTime();

	std::vector< stAPPLY_DAMAGE >::iterator begin = m_DamageList.begin();

	for( ; begin != m_DamageList.end() ; )
	{	
		begin = m_DamageList.erase( begin );		
	}	
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param 
/// @brief  : �������� �ð����� ó��
//--------------------------------------------------------------------------------

void CObjCHAR::ProcDamageTimeOut()
{
	uniDAMAGE wDamage;
	
	DWORD dwCurrentTime = g_GameDATA.GetGameTime();

	std::vector< stAPPLY_DAMAGE >::iterator begin = m_DamageList.begin();

	for( ; begin != m_DamageList.end() ; )
	{	
		/// 5�ʰ� �����ٸ�
		if( ( dwCurrentTime - (*begin).m_dwCreateTime ) > 5000 )
		{
			this->Apply_DAMAGE( g_pObjMGR->Get_CharOBJ( (*begin).m_iAttacker, true ), (*begin).m_Damage.m_wDamage );
			begin = m_DamageList.erase( begin );
		}else
		{
			++begin;		
		}
	}		
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  WORD wDamage ���� ������
/// @brief  : Ÿ��ġ �ٷ� ǥ��
//--------------------------------------------------------------------------------

void CObjCHAR::CreateImmediateDigitEffect( WORD wDamage )
{
	/// Ÿ�ݼ�ġ �ٷ�����.. ������ ��Ұ� ���� �ϴ� ���⿡..
	uniDAMAGE Damage;
	Damage.m_wDamage = wDamage;
	D3DXVECTOR3	pos = this->Get_CurPOS();
	g_UIMed.CreateDamageDigit( Damage.m_wVALUE, pos.x, pos.y, pos.z + this->m_fStature, this->IsA( OBJ_USER ) );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  gsv_DAMAGE_OF_SKILL stDamageOfSkill damage of kill ����
/// @brief  : Damage_of_Skill => �Ϲ� �������� ��ȯ����..
//--------------------------------------------------------------------------------

void CObjCHAR::ConvertDamageOfSkillToDamage( gsv_DAMAGE_OF_SKILL stDamageOfSkill )
{
	uniDAMAGE		Damage;

	CObjCHAR *pAtkOBJ;	
	
	pAtkOBJ = g_pObjMGR->Get_ClientCharOBJ( stDamageOfSkill.m_wSpellObjIDX, false );
	
	if ( pAtkOBJ ) 
	{		
		/// Damage ���..
		PushDamageToList( g_pObjMGR->Get_ClientObjectIndex( stDamageOfSkill.m_wSpellObjIDX ), 
							stDamageOfSkill.m_Damage );	
		return;
	}	
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  
/// @brief  : ������ ���..
//--------------------------------------------------------------------------------

void CObjCHAR::DropFieldItemFromList()
{
	for( int i = 0; i < m_FieldItemList.size(); i++ )
	{
		//gsv_ADD_FIELDITEM& ItemInfo = m_FieldItemList[ i ];
		
		int iItemOBJ = g_pObjMGR->Add_GndITEM( 	m_FieldItemList[ i ].m_wServerItemIDX, 
												m_FieldItemList[ i ].m_ITEM, 
												m_FieldItemList[ i ].m_PosCUR, this->m_PosCUR.z, true );

		if ( iItemOBJ ) 
		{
			CObjITEM *pITEM = (CObjITEM *)g_pObjMGR->Get_OBJECT( iItemOBJ );
			pITEM->m_wOwnerServerObjIDX = m_FieldItemList[ i ].m_wOwnerObjIDX;
			pITEM->m_wRemainTIME		= ITEM_OBJ_LIVE_TIME;

			pITEM->InsertToScene ();

			// m_pRecvPacket->m_gsv_ADD_FIELDITEM.m_wOwnerObjIDX;
			// m_pRecvPacket->m_gsv_ADD_FIELDITEM.m_wRemainTIME;

			tagITEM sITEM = m_FieldItemList[ i ].m_ITEM;

			/*if ( sITEM.m_cType != ITEM_TYPE_MONEY )
				AddMsgToChatWND(CStr::Printf ("������ %s ��� Type: %d, NO: %d ", 
														ITEM_NAME( sITEM.m_cType, sITEM.m_nItemNo ),
														sITEM.m_cType, sITEM.m_nItemNo ), g_dwBLUE ,CChatDLG::CHAT_TYPE_SYSTEM);
			else 
				AddMsgToChatWND(CStr::Printf ("�� ��� %d ", sITEM.m_nItemNo), g_dwBLUE ,CChatDLG::CHAT_TYPE_SYSTEM);*/
		}
	}

	m_FieldItemList.clear();
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  
/// @brief  : �Ѿ˿� �¾����� ��ųó��..
//--------------------------------------------------------------------------------

bool CObjCHAR::ProcessSkillHit( CObjCHAR *pFromOBJ )
{
	if( pFromOBJ == NULL )
		return false;

	return pFromOBJ->ProcEffectedSkill();
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  
/// @brief  : �� ��ȯ.. ( ����� ��ȯ���� �����ӿ� �ʸ��߰� �ٷ� ����.. ������ ��� ����ȭ ����
///				���������� ������ ���� ���ݸ���� ������ �ִµ� Ŭ���̾�Ʈ������ �������� ���� �ʾ����Ƿ�..
//--------------------------------------------------------------------------------

void CObjCHAR::SetSummonMobInfo( gsv_MOB_CHAR& MobInfo )
{
	memcpy( &m_SummonMob, &MobInfo, sizeof( gsv_MOB_CHAR ) );
	m_bHaveSummonedMob = true;

	SetEffectedSkillFlag( true );
}

//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  
/// @brief  : �� ��ȯ.. ( ����� ��ȯ���� �����ӿ� �ʸ��߰� �ٷ� ����.. ������ ��� ����ȭ ����
///				���������� ������ ���� ���ݸ���� ������ �ִµ� Ŭ���̾�Ʈ������ �������� ���� �ʾ����Ƿ�..
//--------------------------------------------------------------------------------

void CObjCHAR::SummonMob()
{
	if( m_bHaveSummonedMob )
	{
		D3DVECTOR PosCUR;

		PosCUR.x = m_SummonMob.m_PosCUR.x;
		PosCUR.y = m_SummonMob.m_PosCUR.y;
		PosCUR.z = 0.0f;


		short nCObj;

		if ( NPC_TYPE( m_SummonMob.m_nCharIdx ) != 999 ) {
			nCObj = g_pObjMGR->Add_MobCHAR( m_SummonMob.m_wObjectIDX, 
											m_SummonMob.m_nCharIdx, 
											PosCUR, 
											m_SummonMob.m_nQuestIDX, 
											m_SummonMob.m_btMoveMODE);
			if ( g_pNet->Recv_tag_ADD_CHAR( nCObj, &m_SummonMob ) ) 
			{
				short nOffset = sizeof( gsv_MOB_CHAR );
				g_pNet->Recv_tag_ADJ_STATUS( nOffset, &m_SummonMob );
			}
		}
	}

	m_bHaveSummonedMob = false;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @param  CObjCHAR *pFromOBJ ������
/// @param  int iEffectIDX		�Ѿ˹�ȣ
/// @param  int iSkillIDX		��ų�ϰ��.. ��ų��ȣ
/// @param  bool bIsSkillEffect	�̰��� ��ų�� ���� �����ΰ�?
/// @param  bool bIsBulletHit   
/// @brief  : �¾������� �ൿ ó��.. �������� �شٴ���. ����Ʈ�� ����Ѵٴ���.
/// @todo Reg_DAMAGE�� �˻��ؼ� Ÿ���� ������ �Ǵ��ؾ�...
//--------------------------------------------------------------------------------

bool CObjCHAR::Hitted (CObjCHAR *pFromOBJ, int iEffectIDX, int iSkillIDX, bool bIsSkillEffect, bool bIsBulletHit, bool bJustEffect )
{	
	if ( NULL == pFromOBJ )
		return true;

//-------------------------------------------------------------------------
///����ȣ::�����ڰ� �������ڸ� �Ǵ��Ѵ�.
#define HIT_AROA_EFF 1613

	BOOL IsAcceptAroa = FALSE;
   if(CCountry::GetSingleton().IsApplyNewVersion())
   {
	   if(pFromOBJ->IsUSER() || pFromOBJ->IsPET())
			IsAcceptAroa = TRUE;
   }
//-------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	/// �¾����� Ǯ�����ϴ� ȿ����..
	//--------------------------------------------------------------------------------
	m_EndurancePack.ClearStateByHitted();


	D3DXVECTOR3	pos = this->Get_CurPOS();

	//--------------------------------------------------------------------------------
	/// ȿ���� �����ų����.. ApplyDamage ������ �����ʴ´�.���� Ÿ��ġ��..
	//--------------------------------------------------------------------------------
	if( !bJustEffect )
	{			
		if ( this->m_lDeadTIME ) 
		{
			/// 2003. 12. 26 ������ �ѹ� ó�� ����...
			/// pTarget�� �̹��� �׾�� �Ѵ�.
			/// ���γ��� �������� �������� ? �ƴ� �ƹ��� ������ ��������???
			// if ( pTarget->m_DeadDAMAGE.m_nTargetObjIDX == this->Get_INDEX() ) 
			{
				// CObjCHAR *pAttacker = g_pObjMGR->Get_CharOBJ( this->m_DeadDAMAGE.m_nTargetObjIDX, true );
				this->Apply_DAMAGE( pFromOBJ, this->m_DeadDAMAGE.m_wDamage );
				this->m_lDeadTIME = 0;

				uniDAMAGE wDamage;
				wDamage.m_wDamage = this->m_DeadDAMAGE.m_wDamage;
				g_UIMed.CreateDamageDigit( wDamage.m_wVALUE, pos.x, pos.y, pos.z + m_fStature, this->IsA( OBJ_USER ) );			

				if( wDamage.m_wACTION & DMG_ACT_CRITICAL )
				{
					if( iEffectIDX )
					{
						int iCriticalEffect = EFFECT_HITTED_CRITICAL( iEffectIDX );

						//�Ʒξ� ����Ʈ 
						ChangeHittedEffect(pFromOBJ,IsAcceptAroa,iCriticalEffect);

						CEffect *pHitEFT = g_pEffectLIST->Add_EffectWithIDX( iCriticalEffect, true );
						if ( pHitEFT ) 
						{
							///D3DXVECTOR3 vec( pos.x, pos.y, pos.z + m_fStature );
							///pHitEFT->SetPOSITION( vec );

							pHitEFT->SetPOSITION( this->Get_CurPOS() );

							pHitEFT->InsertToScene();
						}
					}
				}							
			}
		}else
		{
			int iTotalDmage = 0;
			uniDAMAGE stDmage;

			/// ��ų�� ���� �������� Damage of skill �� ����´�.
			if( !bIsSkillEffect )
			{
				/// �����κ��� ���۹��� ������ ó��	
				

				///
				/// �ᱹ �´³��� ���̰ų�
				/// ���� �����³��� �¾Ұų�..
				/// ���� ���� �����°��( �ᱹ ���������� ) .. �� ó��..
				///

				/// ���ų�, ���������� ���̶�� ������ �����Ѵ�.
				int iTargetObject = g_pObjMGR->Get_ClientObjectIndex( g_pAVATAR->Get_TargetIDX() );
				//--------------------------------------------------------------------------------
				/// Avatar �� ���� ���� Ÿ�� ������ ���� Ÿ�ٵ� �˻�
				int iPetTargetObject = 0;
				if( g_pAVATAR->GetPetMode() > 0 )
					 iPetTargetObject = g_pObjMGR->Get_ClientObjectIndex( g_pAVATAR->m_pObjCART->Get_TargetIDX() );
				//--------------------------------------------------------------------------------
				if( this->IsA( OBJ_USER ) || 
					( this->Get_INDEX() == iTargetObject ) || 
					( this->Get_INDEX() == iPetTargetObject ) )
				{	
					//stDmage = PopTotalDamageFromList( iTotalDmage );
					stDmage = PopCurrentAttackerDamage( pFromOBJ->Get_INDEX() );
					this->Apply_DAMAGE( pFromOBJ, stDmage.m_wDamage );	

					g_UIMed.CreateDamageDigit( stDmage.m_wVALUE, pos.x, pos.y, pos.z + m_fStature, this->IsA( OBJ_USER ) );

					/// Ÿ�ݽ� ��鸲..
					if( stDmage.m_wVALUE )
					{
						if( this->GetPetMode() < 0 )
							this->StartVibration();
						else
						{
							//�� ��ȣ::�� ��忡�� ��鸲 ���� 
							m_IsCartVA = GetPetMode() ? TRUE : FALSE;	
						}
					}
	
					if( stDmage.m_wACTION & DMG_ACT_CRITICAL )
					{
						/// ī�޶� ����..
						if ( IsA( OBJ_USER ) ) 
						{
							D3DXVECTOR3 vMin(-10, -10, -10), vMax(10, 10, 50);
							::shakeCamera( g_pCamera->GetZHANDLE(), vMin, vMax, 200 ); // ī�޶� ��鸲.
						}

						if( iEffectIDX )
						{
							int iCriticalEffect = EFFECT_HITTED_CRITICAL( iEffectIDX );
							
							///�Ʒξ� ����Ʈ 
							ChangeHittedEffect(pFromOBJ,IsAcceptAroa,iCriticalEffect);
						
							CEffect *pHitEFT = g_pEffectLIST->Add_EffectWithIDX( iCriticalEffect, true );
							if ( pHitEFT ) 
							{
								///D3DXVECTOR3 vec( pos.x, pos.y, pos.z + m_fStature );
								///pHitEFT->SetPOSITION( vec );

								pHitEFT->SetPOSITION( this->Get_CurPOS() );

								pHitEFT->InsertToScene();
							}
						}
					}

				}else	/// Ŭ���̾�Ʈ���� ������ ó���ؾߵ� ������..
				{
					///
					/// ����.. Ư���� ��Ȳ���� ����, ���� �����ϴ� ���� �ƴѻ�Ȳ������ �����κ��� �������� �޾Ҵ�.
					///
					if( !m_DamageList.empty() )
					{
						stDmage = PopTotalDamageFromList( iTotalDmage );
						this->Apply_DAMAGE( pFromOBJ, stDmage.m_wDamage );								

						g_UIMed.CreateDamageDigit( stDmage.m_wVALUE, pos.x, pos.y, pos.z + m_fStature, this->IsA( OBJ_USER ) );

						/// Ÿ�ݽ� ��鸲
						if( stDmage.m_wVALUE )
						{
							if( this->GetPetMode() < 0 )
								this->StartVibration();
						}
					}else
					/// �����ϰ� Ŭ���̾�Ʈ���� ó���Ѵ�.
					{
						if( iSkillIDX )			
							stDmage.m_wDamage = CCal::Get_SkillDAMAGE ( pFromOBJ, this, iSkillIDX, 1 );
						else
							stDmage.m_wDamage = CCal::Get_DAMAGE ( pFromOBJ, this, 1 );					
					}

					iTotalDmage = stDmage.m_wVALUE;
				}
			}else
			{
				/// �Ѿ��� �¾������.. bIsSkillEffect�� ���õǾ� �Ʒ��� ó�����ش�.
								
				ProcessSkillHit( pFromOBJ );

				{
					//----------------------------------------------------------------------------------------------------				
					/// @brief ������Ʈ���� ��Ÿ� �Ѿ� ��ų�� ��� �������� ����´�.
					//----------------------------------------------------------------------------------------------------
					if( SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_FIRE_BULLET )
					{
						/// ���ų�, ���������� ���̶�� ������ �����Ѵ�.
						int iTargetObject = g_pObjMGR->Get_ClientObjectIndex( g_pAVATAR->Get_TargetIDX() );
						if( this->IsA( OBJ_USER ) || ( this->Get_INDEX() == iTargetObject ) )
						{	
							//stDmage = PopTotalDamageFromList( iTotalDmage );
							stDmage = PopCurrentAttackerDamage( pFromOBJ->Get_INDEX() );
							this->Apply_DAMAGE( pFromOBJ, stDmage.m_wDamage );	

							g_UIMed.CreateDamageDigit( stDmage.m_wVALUE, pos.x, pos.y, pos.z + m_fStature, this->IsA( OBJ_USER ) );
							/// Ÿ�ݽ� ��鸲
							if( stDmage.m_wVALUE )
							{
								if( this->GetPetMode() < 0 )
									this->StartVibration();
							}
						}
					}
				}
			}
		}		
	}


	///
	///	Miss �ϰ��� ���� �ʴ´�.
	///

	//if( stDmage.m_wVALUE <= 0 )
	//	return true;
	
	// Ÿ�� ȿ��.
	if ( this->IsVisible() ) 
	{
		// short nEffectIDX = WEAPON_DEFAULT_EFFECT( pFromOBJ->Get_R_WEAPON() );
		if ( iEffectIDX  ) 
		{
			int iHitEffect = iEffectIDX;

			/// skill �κ����� Ÿ���� ���� file_effect ����
			/// �Ϲݰ����� �������̺�.. �� List_Effect ����..
			if( !bIsSkillEffect )
				iHitEffect = EFFECT_HITTED_NORMAL( iEffectIDX ); 
			

			
			/// �Ѿ˾� �¾Ƽ� �������....
			if( bIsSkillEffect && bIsBulletHit )
			{
				iHitEffect = SKILL_HIT_EFFECT( iSkillIDX ); 

				if ( iHitEffect ) 
				{
					///�Ʒξ� ����Ʈ 
					ChangeHittedEffect(pFromOBJ,IsAcceptAroa,iHitEffect);

					CEffect *pHitEFT = g_pEffectLIST->Add_EffectWithIDX( iHitEffect, true );
					if ( pHitEFT ) 
					{	
						if( SKILL_HIT_EFFECT_LINKED_POINT( iSkillIDX ) == INVALID_DUMMY_POINT_NUM )
							pHitEFT->LinkNODE ( this->GetZMODEL() );
						else
							this->LinkDummy( pHitEFT->GetZNODE(), SKILL_HIT_EFFECT_LINKED_POINT( iSkillIDX ) );						
							

						///pHitEFT->SetParentCHAR( this );
						///AddExternalEffect( pHitEFT );						

						pHitEFT->UnlinkVisibleWorld ();
						pHitEFT->InsertToScene ();
					}
				}
			}else
			// �Ϲ� ���ΰ��..
			{
				if ( iHitEffect ) 
				{
					///�Ʒξ� ����Ʈ 
					ChangeHittedEffect(pFromOBJ,IsAcceptAroa,iHitEffect);

					CEffect *pHitEFT = g_pEffectLIST->Add_EffectWithIDX( iHitEffect, true );
					if ( pHitEFT ) 
					{					
						pHitEFT->LinkNODE ( this->GetZMODEL() );

						///pHitEFT->SetParentCHAR( this );
						///AddExternalEffect( pHitEFT );						

						pHitEFT->UnlinkVisibleWorld ();
						pHitEFT->InsertToScene ();
					}
				}
			}


			//----------------------------------------------------------------------------------------------------			
			/// @brief Hit sound ���
			//----------------------------------------------------------------------------------------------------
			int iHitSound = 0;
			if( !bIsSkillEffect )
			{
				iHitSound = EFFFCT_HIT_SND_IDX( iEffectIDX );
			}else
			{
				iHitSound = SKILL_HIT_SOUND( iSkillIDX );
			}

			if ( iHitSound ) 
			{
				g_pSoundLIST->IDX_PlaySound3D( iHitSound, Get_CurPOS() );				
			}
			//----------------------------------------------------------------------------------------------------


		}
	}


#ifdef	__VIRTUAL_SERVER		
	g_pNet->Send_cli_DAMAGE( pFromOBJ, this, stDmage.m_wDamage );
#endif
	return true;
}


//--------------------------------------------------------------------------------
/// class : ChangeHittedEffect
/// ����ȣ: �Ʒξ� �� īƮ ��ų ����Ʈ�� �����Ѵ�. 
//--------------------------------------------------------------------------------
void CObjCHAR::ChangeHittedEffect(CObjCHAR* pAttackObj,BOOL bA,int& hitIDX)
{

/*
	//īƮ ��ų ����Ʈ 
	if(pAttackObj->GetUseCartSkill())
	{
		hitIDX = SKILL_HIT_EFFECT(pAttackObj->GetCartSKIDX());
		pAttackObj->GetUseCartSkill() = FALSE;
	}
*/
	//�Ʒξ� ����Ʈ 
	if(bA)
	{
		CObjCHAR* pTg = pAttackObj;
		
		//���̶��...
		if(pAttackObj->IsPET())
		{	
			CObjCART* pCart = (CObjCART*)pAttackObj;
			 if(pCart)
				 pTg = pCart->GetParent();
		}

		if(pTg && bA && pTg->SetAroaState())
			hitIDX = HIT_AROA_EFF;
	}

}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ���� �Ÿ��� ���Ѵ�. ��ų�� ���Ѱ����̶�� ��ų���̺��� ���⿡ ���� �Ϲ� �����̶�� �������̺���
//--------------------------------------------------------------------------------

int CObjCHAR::Get_AttackRange()
{	
	/// ��ų�� ���� �Ÿ��� �ԷµǾ� �ִٸ� ��ų�Ÿ� �ƴϸ� ���� �Ÿ�..
	if ( this->m_nToDoSkillIDX > 0 ) 
	{
		if( this->m_nToDoSkillIDX < g_SkillList.Get_SkillCNT() )
		{
			if(	SKILL_DISTANCE( this->m_nToDoSkillIDX ) ) 
			{
				return SKILL_DISTANCE( this->m_nToDoSkillIDX );
			}
		}else
			assert( 0 && "Get_AttackRange Failed[ ToDoSkillIDX is invalid ]" );
	}

	if ( Get_R_WEAPON() )
	{
		return WEAPON_ATTACK_RANGE( Get_R_WEAPON() ) + ( Get_SCALE() * 120 );
	}

	// Default attack range...
	return ( Def_AttackRange () + ( Get_SCALE() * 120 ) );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

void CObjCHAR::Set_EMOTION (short nEmotionIDX)
{	
	;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

void CObjCHAR::Special_ATTACK ()
{
	// CObjCHAR *pTarget = Get_TARGET ();

	;
	;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ����.. ���� ��ġ�� �����ϰ� ����.. ��������..
//--------------------------------------------------------------------------------

void CObjCHAR::Warp_POSITION (float fX, float fY)
{
	SetCMD_STOP ();

	m_PosCUR.x = fX;
	m_PosCUR.y = fY;

	g_pTerrain->SetCenterPosition( fX, fY );

	DropFromSky( fX, fY );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : �ϴð� �浹.. ����ġ��..
//--------------------------------------------------------------------------------

void CObjCHAR::DropFromSky ( float fX, float fY )
{
	m_PosCUR.x = fX;
	m_PosCUR.y = fY;
	m_PosCUR.z = ::g_pTerrain->GetHeightTop(fX, fY);

	::setPosition ( m_hNodeMODEL, m_PosCUR.x, m_PosCUR.y, m_PosCUR.z );
	::savePrevPosition( m_hNodeMODEL );

}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ���� Ư���������� ȸ��.
//--------------------------------------------------------------------------------

void CObjCHAR::Set_ModelDIR (t_POSITION &PosToView, bool bImmediate )
{
	::setModelDirectionByPosition ( this->m_hNodeMODEL, PosToView.x, PosToView.y );	
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ���� Ư���������� ȸ��.
//--------------------------------------------------------------------------------

void CObjCHAR::Set_ModelDIR( float fAngleDegree, bool bImmediate )
{
	::setModelDirection( this->m_hNodeMODEL, fAngleDegree, bImmediate );
}

//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ���� Ư���������� ȸ��.
//--------------------------------------------------------------------------------

void CObjCHAR::Add_ModelDIR( float fAngleDegree )
{
	float fDir = ::getModelDirection( this->m_hNodeMODEL );

	::setModelDirection( this->m_hNodeMODEL, fAngleDegree + fDir, true );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : �̵� ������..
//--------------------------------------------------------------------------------

void CObjCHAR::Move_COMPLETED ()
{
	/*
	�ɸ����� �̵� �ӵ��� Ŭ���̾�Ʈ�� �����ϸ� �ȵ�...
	if ( !this->IsUSER() ) 
	{
		this->m_fMoveCmPerSec = this->Get_DefaultSPEED();
	}
	*/
	/// x-y ��ġ�� ���� �̵�. �ȱ׷��� ����� ���ɼ� ����.
	/// z ��ġ�� �켱 ������ġ�� �����ϰ� ����.
	m_PosGOTO.z = m_PosCUR.z;
	m_PosCUR = m_PosGOTO;	
	
	::setPosition (this->m_hNodeMODEL, m_PosCUR.x, m_PosCUR.y, m_PosCUR.z);

	/// 2003. 12.16 :: ��Ŷ ������ �����Ǿ��� �̵� �ӵ��� ���� ������ �ӵ��� �ٲ۴�.
	m_fAdjustSPEED = this->Get_DefaultSPEED();
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief : �̵��� ���� ��ġ�� ���̰��� �������� �޾ƿ� ������ �����Ͽ� ���̸� �����.
//--------------------------------------------------------------------------------

void CObjCHAR::Reset_Position ()
{
	return;

	///m_PosCUR.z = m_PosGOTO.z; // ��ǥ ������ z ���� ���� ���� ��ġ�� ���� ���̴�.
	///::setPositionVec3( this->m_hNodeMODEL, m_PosCUR );
}


//----------------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : RECOVER_STATE_CHECK_TIME �������� Get_RecoverHP���� ���� HP�� ���� �ش�.
///			- ���������� HP�� �޾������� Ŭ���̾�Ʈ������ ���� �����縸ŭ�� ���⼭ �����ش�.
///			- arua �����ϰ�� 50% ���� �����ش� : 2005/07/12 - nAvy
//----------------------------------------------------------------------------------------

void CObjCHAR::RecoverHP( short nRecoverMODE )
{
#ifdef _NoRecover
	Add_HP( m_ReviseHP );
#else
	int iRecoverHP = Get_RecoverHP( nRecoverMODE );	
	int iAruaAddHP = (m_IsAroa)?iRecoverHP >> 1:0;

	Add_HP( iRecoverHP + iAruaAddHP );
	
//	_RPT2( _CRT_WARN,"RecoverHP:%d, AruaAddHP:%d\n", iRecoverHP, iAruaAddHP );

	int iReviseConstHP = iRecoverHP;//Get_MaxHP() / 30;///��ƽ���� �������� ���̸� ���̱� ���� ������
	if( iReviseConstHP < 10 )
		iReviseConstHP = 10;


	if( m_ReviseHP > 0 )
	{
		if( m_ReviseHP > iReviseConstHP )
		{
			Add_HP( iReviseConstHP );
			_RPT1( _CRT_WARN,"Add Revise HP %d\n", iReviseConstHP );
			m_ReviseHP -= iReviseConstHP;
		}
		else
		{
			Add_HP( m_ReviseHP );
			_RPT1( _CRT_WARN,"Add Revise HP %d\n", m_ReviseHP );
			m_ReviseHP = 0;
		}
	}
	else if( m_ReviseHP < 0 )
	{
		if( abs(m_ReviseHP) > iReviseConstHP )
		{
			Add_HP( -iReviseConstHP );
			_RPT1( _CRT_WARN,"Add Revise HP %d\n", -iReviseConstHP );
			m_ReviseHP += iReviseConstHP;
		}
		else
		{
			Add_HP( m_ReviseHP );
			_RPT1( _CRT_WARN,"Add Revise HP %d\n", m_ReviseHP );
			m_ReviseHP = 0;
		}
	}
#endif
	int iMaxHP = Get_MaxHP();
	if( Get_HP() > Get_MaxHP() )
		Set_HP( Get_MaxHP() );					
}

//----------------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : RECOVER_STATE_CHECK_TIME �������� Get_ReocverMP���� ���� MP�� ���� �ش�.
///			- ���������� MP�� �޾������� Ŭ���̾�Ʈ������ ���� �����縸ŭ�� ���⼭ �����ش�.
///			- arua �����ϰ�� 50% ���� �����ش� : 2005/07/12 - nAvy
//----------------------------------------------------------------------------------------

void CObjCHAR::RecoverMP( short nRecoverMODE )
{
#ifdef _NoRecover
	Add_MP(m_ReviseMP);

#else
	int iRecoverMP = Get_RecoverMP( nRecoverMODE );
	int iAruaAddMP = (m_IsAroa)?iRecoverMP >> 1:0;

	Add_MP( iRecoverMP + iAruaAddMP );

	int iReviseConstMP = iRecoverMP;//Get_MaxMP() / 30;///��ƽ���� �������� ���̸� ���̱� ���� ������
	if( iReviseConstMP < 10 )
		iReviseConstMP = 10;

	if( m_ReviseMP > 0 )
	{
		if( m_ReviseMP > iReviseConstMP )
		{
			Add_MP( iReviseConstMP );
			m_ReviseMP -= iReviseConstMP;
		}
		else
		{
			Add_MP( m_ReviseMP );
			m_ReviseMP = 0;
		}
	}
	else if( m_ReviseMP < 0 )
	{
		if( abs(m_ReviseMP) > iReviseConstMP )
		{
			Add_MP( -iReviseConstMP );
			m_ReviseMP += iReviseConstMP;
		}
		else
		{
			Add_MP( m_ReviseMP );
			m_ReviseMP = 0;
		}
	}
#endif
	if( Get_MP() > Get_MaxMP() )
		Set_MP( Get_MaxMP() );			
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ĳ���� ���� ����. ���� �浹�� ���⼭ ó���ȴ�.
//--------------------------------------------------------------------------------

void CObjCHAR::Adjust_HEIGHT ()
{
	m_pCollision->UpdateHeight( this );
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief Ư�� �׼� �߰��� ��� ��ü������( Ư�� ���� ) �ٲ� ��� �´� ������� ��ü
//----------------------------------------------------------------------------------------------------

void CObjCHAR::UpdateMotionByChangingEquip()
{
	if( this->IsA( OBJ_AVATAR ) || this->IsA( OBJ_USER ) )
	{
		CObjAVT* pAvt = (CObjAVT*)this;
		/// ���⸦ �ٲܲ� �ִٸ�..
		if( pAvt->GetUpdateMotionFlag() || pAvt->GetChangeWeaponR() || pAvt->GetChangeWeaponL() )
		{
			/// ���ο� ��� ������ ���� ���ο� ��� ����
			switch( pAvt->Get_COMMAND() )
			{
				case CMD_STOP:						
					{
						pAvt->SetCMD_STOP( );
					}
					break;
				case CMD_MOVE:
					{
						//pAvt->SetCMD_MOVE( pAvt->m_PosGOTO.x, pAvt->m_PosGOTO.y, pAvt->m_bRunMODE );
						pAvt->Set_MOTION( pAvt->GetANI_Move(), pAvt->m_fCurMoveSpeed, pAvt->m_fRunAniSPEED );
					}
					break;
				case CMD_ATTACK:
					{
						pAvt->SetCMD_ATTACK( pAvt->m_iServerTarget );
						//pAvt->Set_MOTION( pAvt->GetANI_Attack(), pAvt->m_fCurMoveSpeed, pAvt->m_fRunAniSPEED );
					}
					break;
				case CMD_DIE:
					break;
				case CMD_PICK_ITEM:
					break;
				case CMD_SKILL2SELF:
					break;
				case CMD_SKILL2OBJ:
					break;
				case CMD_SKILL2POS:
					break;
				case CMD_RUNAWAY:
					break;
			}

			//pAvt->Update ();		

			/// if pCHAR is my avatar, update ability
			if( pAvt->IsA( OBJ_USER ) )
			{
				((CObjUSER*)pAvt)->UpdateAbility();
			}

			pAvt->SetChangeWeaponR( 0 );
			pAvt->SetChangeWeaponL( 0 );

			pAvt->SetUpdateMotionFlag( false );
		}
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : ĳ���� ó���Լ�
/// @todo �׽�Ʈ �ڵ� ����...//if (this->Get_TYPE() != OBJ_MOB ) 
/// @todo View list ���Ķ�..
//--------------------------------------------------------------------------------

int CObjCHAR::Proc (void)
{
	/*
	*
	* for debugging...
	*
	*/

	//if( m_iCastingStartTime != NULL )
	//{
	//	if( ( g_GameDATA.GetGameTime() - m_iCastingStartTime ) > SKILL_PROC_LIMIT )
	//	{
	//		///Casting_END();
	//		assert( 0 && "Casting time expired" );
	//	}	
	//}

#if defined(_GBC)	
//--------------------------------------------------------------------------------
		//����ȣ::īƮ ���ڼ��� ������ ž�� ���״ٸ� īƮ��ġ�� 2�ν� �������� �������� �Ѵ�. 
		if(this->GetPetMode() > -1)
		{
			if(m_pObjCART->m_pRideUser)
			{	
				float Pos1[3] = { 0 };

				//2�ν� ���� ��ġ���� �����´�. 
				::getDummyPosition(m_pObjCART->GetZMODEL(),10,Pos1);
				//2�ν� ���� ��ġ �������� 
				m_pObjCART->m_pRideUser->Set_CurPOS(D3DXVECTOR3(Pos1[0],Pos1[1],Pos1[2])); 	
			}
		}
//--------------------------------------------------------------------------------
#endif 


	//--------------------------------------------------------------------------------
	/// ������Ʈ ���̺극�̼� ������Ʈ.~~ �����~~
	//--------------------------------------------------------------------------------
    m_ObjVibration.Proc();



	//--------------------------------------------------------------------------------
	/// �ܺ� ���ڷ��̼ǵ� ������Ʈ	
	//--------------------------------------------------------------------------------
	m_EndurancePack.Update();

	
	//--------------------------------------------------------------------------------
	/// visibility ��ȭ
	/// ĳ���� ����� �ƿ� ����� ������Ʈ �ȵǴ°�찡 �ִ�.. �̰��� �Ʒ� �ڵ�� ������ �ִ°�? -04/5/25
	//--------------------------------------------------------------------------------
	if( m_pChangeVisibility != NULL )
	{
		/// if expired?
		if( m_pChangeVisibility->ProcessVisibility( g_GameDATA.GetElapsedFrameTime() ) == false )
		{
			delete m_pChangeVisibility;
			m_pChangeVisibility = NULL;

			this->m_lDeadTIME = 0;
			return 0;
		}

		assert( !(this->IsA( OBJ_USER )) && " ���� ������ �� ������Ƽ? " );

		return 1;
	}


	//--------------------------------------------------------------------------------
	/// Update check frame
	//--------------------------------------------------------------------------------
	DWORD dwCurrentTime = g_GameDATA.GetGameTime();
	m_dwFrameElapsedTime = dwCurrentTime - m_dwLastRecoveryUpdateTime;		/// ���������ӿ��� ���� ������ ���̿� �帥�ð��� �����ش�.
	m_dwLastRecoveryUpdateTime = dwCurrentTime;




	//--------------------------------------------------------------------------------
	/// ��Ŀ������ ó��	
	/// 04/5/28 CObjAVT::Proc ���̵�..
	//--------------------------------------------------------------------------------
	//if( this->Is_AVATAR() )
	//{
	//	switch ( Get_COMMAND() ) 
	//	{
	//		case CMD_SIT :
	//			this->RecoverHPnMP( RECOVER_STATE_SIT_ON_GROUND );
	//			break;
	//		default:
	//			/// ����� sit ���� �ٲ�ų�.. �������� sit ó���� �ƴ϶�� �ð� ����..
	//			m_dwElapsedTime = 0;
	//			break;	
	//	}

	//	//--------------------------------------------------------------------------------
	//	/// �ð��� ���� �׼� ��� ó��..
	//	/// if �� ���̱� ���ؼ� �������� �̵�.. 04/5/28
	//	//--------------------------------------------------------------------------------
	//	///if( this->IsA( OBJ_AVATAR ) || this->IsA( OBJ_USER ) )
	//	{
	//		m_ChangeActionMode.Proc();
	//	}		
	//}	
	
#ifdef _DEBUG
	/*
	if( ::getUserData( GetZMODEL() ) )
	{
		_ASSERT( ::getUserData( GetZMODEL() ) );

		const char* pStrName = ::getName( GetZMODEL() );
		D3DVECTOR vec;
		::getPosition( GetZMODEL(), (float*)( &vec ) );
	}
	*/
#endif

		
	//--------------------------------------------------------------------------------
	/// �ϴ��� ��� ������Ʈ ĳ���͸� �� ����Ʈ��..
	/// �̴ϸ� ����� ���� ����Ʈ
	//--------------------------------------------------------------------------------
	g_pObjMGR->AddViewObject( m_nIndex );


	//--------------------------------------------------------------------------------
	/// �Ÿ��� ���� ������Ʈ���� ����( ������ ��� Ȥ�� ����.. )
	/// 2004/3/17 Ŭ���̾�Ʈ �������� �Ÿ��� ���� ���� ������ ���ʿ��ϴ�. �������� ����°� �� �������ش�.
	/// �ʿ��Ұ�� LOD ������ �ݿ��ȴ�.
	/// 2004/7/8 InsertToScene/removeFromScene ���� �ʿ���� �������� �޴µ��� �߰��ϰ� �����϶�..
	//--------------------------------------------------------------------------------
	bool bInViewfrustum = false;
	///int iDistance = CD3DUtil::distance ((int)g_GameDATA.m_PosCENTER.x, (int)g_GameDATA.m_PosCENTER.y, (int)m_PosCUR.x, (int)m_PosCUR.y);
	///if ( iDistance < 8*4*nGRID_SIZE ) 
	{
		///this->InsertToScene ();

		// char name & chatting message 
		if ( ::inViewfrustum( this->GetZMODEL() ) ) // ���������� �ȿ� ������ ��, ������ ����
		{ 
			bInViewfrustum = true;
			if (this->Get_TYPE() != OBJ_MOB ) 
			{
				g_pViewMSG->AddObjIDX( m_nIndex );
			}
		}		
	} /*else
	{
		this->RemoveFromScene ();
	}*/



	//--------------------------------------------------------------------------------
	/// ���������� �ȿ� �ִ� ������Ʈ�鸸 ���̰� ����(���� ���� �Ա� ����)
	//--------------------------------------------------------------------------------
	if ( bInViewfrustum ) // ���������� �ȿ� �ִ� ������Ʈ�鸸 ���̰� ����(���� ���� �Ա� ����)
	{ 
		/// ���� ���������� ��ũ�Ǿ��ִٸ�.. �浹ó�� ����..
		//������ ĳ���� �����Ҷ�...
		//if(m_bDisguise)
		//{
		//	::setScale(this->m_hNodeMODEL, 1.0f, 1.0f, 1.0f);
		//}
		
		


		if( this->IsChild() == false )
			Adjust_HEIGHT ();
	
	    
		//������ ĳ���� �����Ҷ�...
		//if(m_bDisguise)
		//{
		//	::setLightingRecursive(this->GetZMODEL(), g_GameDATA.m_hCharacterLight2);
		//	::setVisibilityRecursive(this->GetZMODEL(), 0.15f);
		//	::setScale(this->GetZMODEL(), 0.7f, 0.7f, 0.7f);
		//	::setVisibleGlowRecursive( this->GetZMODEL(), 2, 0.2f, 0.5f, 0.945f );
		//}
	
	}
	else 
	{

#if (1) /// ����, �̵� ���۽� ���� ������ �Ǳ� ������, ���� ���� �ʿ� ����.	
		::getPosition( this->m_hNodeMODEL, (float*)m_PosCUR );
		//m_PosCUR.z = g_pAVATAR->Get_CurPOS().z; // �Ⱥ��̴� ĳ������ ���̴� ���ΰ� �ƹ�Ÿ�� ���̿� ����.
		
		/// �ƹ�Ÿ �̰ų�( īƮ�� ź���°� �ƴ�), ���� ī���ϰ�쿡��..
		///if ( ( this->Is_AVATAR() && ( this->GetPetMode() < 0 ) ) || 
		///	this->IsPET() ) 
		{
			D3DXVECTOR3 vAvatarPos	= g_pAVATAR->Get_CurPOS();
			float fDistanceSquare2D = (vAvatarPos.x - m_PosCUR.x)*(vAvatarPos.x - m_PosCUR.x) + (vAvatarPos.y - m_PosCUR.y)*(vAvatarPos.y - m_PosCUR.y);
			const float fMinDistanceSquare2D = 4000.0f*4000.0f;
			// ���� ����� �Ÿ��� ���� ��쿡�� ���� ����
			if ( fDistanceSquare2D < fMinDistanceSquare2D ) 
			{
				Adjust_HEIGHT();
			//	// �̵� ���� �Ŷ�� ������ �����Ѵٸ�, ���� ��ġ�� �ٸ� ��ġ�̴�.
			//	//m_PosCUR.z = g_pTerrain->GetHeightTop(m_PosCUR.x, m_PosCUR.y);
			//	::setPosition (m_hNodeMODEL, m_PosCUR.x, m_PosCUR.y, m_PosCUR.z);
			}
		}
#endif

	}


	//--------------------------------------------------------------------------------
	/// ��� �������� �����°�?
	//--------------------------------------------------------------------------------	
	m_bFrameING = this->ProcMotionFrame ();	

	//--------------------------------------------------------------------------------
	// ��� ������ ������.
	//--------------------------------------------------------------------------------
	if ( !m_bFrameING ) 
	{	
		//--------------------------------------------------------------------------------
		/// �׻� ActionSkillIDX �� ��ų �׼� ���۽� DoingSkill �� ��Ͻ�Ű�� ����� ������ ����..( ��������ӿ��� ��ų��ȣ�� �����ؾ��ϱ� ������ )
		//--------------------------------------------------------------------------------
		m_nDoingSkillIDX = 0;

//����ȣ::īƮ ���ݽ� ĳ���͸� ������. 
#if defined(_GBC)
		Set_SitMode();
#endif
		// { added by zho 2003-12-17
		// ���ܻ� ����
		for (short nI=0; nI<2; nI++) 
		{
			if ( m_hTRAIL[ nI ] ) 
			{
				::controlTrail ( m_hTRAIL[ nI ], 3 );	// ���� �����.
			}
		}				

		//--------------------------------------------------------------------------------
		/// ���ü�� ���� ������ ��� ��ü..
		//--------------------------------------------------------------------------------
		UpdateMotionByChangingEquip();

	} else if ( Get_STATE() & CS_BIT_INT )	// �̵����߿��� �ϰŵ� �Ҽ� ����.
	{
		return 1;
	}

	//--------------------------------------------------------------------------------
	/// ť�� ���� ��� ����...
	/// CanApplyCommand �� false �϶��� ������ �Ұ����ϴ�.
	/// ����� m_bCastingSTART = true �϶����̴�.
	//--------------------------------------------------------------------------------
	ProcQueuedCommand();


	//--------------------------------------------------------------------------------
	/// ĳ������ ���� Ʋ���� ó��
	/// Skill_START ���� m_bCastingSTART = true �� �����.
	/// �׷��� ������ ��ų�׼��� ���� ������ �˼� �����Ƿ�.. �ϴ�
	/// �̰Ϳ� �����Ѵ�.
	/// 04/5/28 - Skill ����( DO_SKILL �Լ� ) �� �����丵 �ϸ鼭 ������.( �ʿ䰡 ���� �Ǿ���. )
	//--------------------------------------------------------------------------------
	///Casting_END ();




	//--------------------------------------------------------------------------------
	/// ������ Ÿ�Ӿƿ� üũ
	//--------------------------------------------------------------------------------
	this->ProcDamageTimeOut();

	//--------------------------------------------------------------------------------
	/// Effect_Of_Skill Ÿ�Ӿƿ� üũ
	//--------------------------------------------------------------------------------
	ProcTimeOutEffectedSkill();

	//--------------------------------------------------------------------------------
	/// ������ ���� ������ �뺸 �޾Ҵ�...
	//--------------------------------------------------------------------------------
	if ( this->m_lDeadTIME ) 
	{
		CObjCHAR *pAttacker = g_pObjMGR->Get_CharOBJ( this->m_DeadDAMAGE.m_nTargetObjIDX, true );

		/// 2003. 12. 26 ������ �ѹ� ó�� ����...
		/// �� ������ 10�ʰ� �����µ�, �ƹ��� �� ������ �ʴ� ���...
		if ( g_GameDATA.GetGameTime() - this->m_lDeadTIME >= 1000 * 5 
				|| pAttacker == NULL  
				|| pAttacker->m_iServerTarget != g_pObjMGR->Get_ServerObjectIndex( this->Get_INDEX() ) ) 
		{
			this->Apply_DAMAGE( pAttacker, this->m_DeadDAMAGE.m_wDamage );

			/// �ڻ��ϰ��.. ������ ǥ������ ����..
			if( pAttacker != this )
			{
				uniDAMAGE wDamage;
				wDamage.m_wDamage = this->m_DeadDAMAGE.m_wDamage;
				
				D3DXVECTOR3	pos = this->Get_CurPOS();
				g_UIMed.CreateDamageDigit( wDamage.m_wVALUE, pos.x, pos.y, pos.z + m_fStature, this->IsA( OBJ_USER ) );				

				this->m_lDeadTIME = 0;				
			}else
			{
				this->m_lDeadTIME = 100;				
			}			
			return 1;
		}
	}




	
	/// if current hp <= 0 do CMD die
	if( Get_HP() == 0 )
	{
		_ASSERT( 0 && "/// if current hp <= 0 do CMD die" );
		this->SetCMD_DIE();
	}

	

	switch( Get_COMMAND() ) 
	{
		case CMD_DIE	: 
			return this->IsUSER ();

		case CMD_SIT	:
			return ProcCMD_SIT ();

		case CMD_STOP   :
			return ProcCMD_STOP ();

		case CMD_MOVE	   :
			return ProcCMD_MOVE ();

		case CMD_PICK_ITEM :
			return ProcCMD_PICK_ITEM ();

		case CMD_ATTACK :
			return	ProcCMD_ATTACK ();			
				
		case CMD_SKILL2SELF :
			return ProcCMD_Skill2SELF ();

		case CMD_SKILL2POS :
			return ProcCMD_Skill2POSITION ();

		case CMD_SKILL2OBJ :
			return ProcCMD_Skill2OBJECT ();
	}

	return 1;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// ����ȣ: Set_SitMode() 
///       : īƮ�� �����Ҷ� �ƹ�Ÿ�� ��ģ��		
///				
//--------------------------------------------------------------------------------
void CObjCHAR::Set_SitMode(void)
{
	
	if(this->GetPetMode() > 0)
	{
//		if(this->m_pObjCART->Get_COMMAND() == CMD_ATTACK)
			this->Set_MOTION(this->m_pObjCART->GetRideAniPos());
	}

}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// ����ȣ: ProcCMD_Skill2OBJECT_PET() 
///       : �� ������ Ÿ�Ͻ�ų�� �����Ѵ�. 				
///				
//--------------------------------------------------------------------------------
int CObjCHAR::ProcCMD_Skill2OBJECT_PET()
{
	
	CObjCHAR *pTarget = CSkillManager::GetSkillTarget( m_iServerTarget, (this->m_nToDoSkillIDX)? this->m_nToDoSkillIDX : this->m_nActiveSkillIDX);
		
	if ( pTarget ) 
	{
		//-----------------------------------------------------------------------------------------
		/// ���� ��ų ĳ������ �������� �ʾҴٸ�..
		//-----------------------------------------------------------------------------------------	
		if ( !m_bCastingSTART )
		{
			int iAttackRange = this->Get_AttackRange();
			
			//Ÿ�� ������ �����Ѵ�. 
			m_pObjCART->m_PosGOTO       = pTarget->Get_CurPOS();
			m_pObjCART->Set_TargetIDX(m_iServerTarget);

			if( m_pObjCART->Goto_TARGET( pTarget, iAttackRange ) == false )
			{	m_pObjCART->SetCMD_MOVE(pTarget->Get_CurPOS(),TRUE);	return 1;	}
			else
			{
				m_pObjCART->Set_STATE(CS_STOP);
				m_pObjCART->m_fCurMoveSpeed = 0;
				
				Set_STATE(CS_STOP);
				m_fCurMoveSpeed = 0; 
			}

			if( !bCanStartSkill() ) 
			{
				//Pet
				m_pObjCART->Set_STATE(CS_STOP);
				m_pObjCART->m_fCurMoveSpeed = 0;

				//�ƹ�Ÿ 
				Set_STATE(CS_STOP);
				m_fCurMoveSpeed = 0; 
				///this->Set_MOTION( this->GetANI_Casting() );
				return 1;
			}			

			m_pObjCART->Set_ModelDIR( pTarget->m_PosCUR );
			
		}	

		m_pObjCART->Set_ModelDIR( pTarget->m_PosCUR );
	
		//-----------------------------------------------------------------------------------------
		/// Ÿ���� �׾���ȴ�...
		//-----------------------------------------------------------------------------------------			
		if( pTarget->m_bDead )
		{
			SetEffectedSkillFlag( true );
		}

		//��ų Ÿ���� �����ͼ� īƮ�� ��ų�� �ߵ�����  üũ�Ѵ�. 
		int sType = SKILL_TYPE(this->m_nToDoSkillIDX);

		///īƮ::ĳ���� / �������� ����...
		if(sType == -1)
		{
			m_pObjCART->m_nToDoSkillIDX	= this->m_nToDoSkillIDX;
			
			if(m_pObjCART->m_SkillActionState == 0)
				m_pObjCART->m_SkillActionState = 1; 

			m_pObjCART->Do_SKILL(this->Get_TargetIDX(),pTarget);
		}
		///�ƹ�Ÿ::ĳ���� / �������� ����...
		else
		{ 	this->Do_SKILL(this->Get_TargetIDX(),pTarget);		}	

	} 

	else
	{
		SetEffectedSkillFlag( true );
		m_nActiveSkillIDX = 0;
		Casting_END();		
	}

	return 1;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// ����ȣ: SetNewCommandAfterSkill_PET() 
///       : �� ������ Ÿ�Ͻ�ų�� �����Ѵ�. 				
///				
//--------------------------------------------------------------------------------
void CObjCHAR::SetNewCommandAfterSkill_PET(int iSkillNO)
{
	
	switch( SKILL_ACTION_MODE( iSkillNO ) )
	{
		case SA_STOP:
			{
				m_pObjCART->Set_COMMAND(CMD_STOP);
				this->Set_COMMAND(CMD_STOP);
				
				break;
			}
			
		case SA_ATTACK:
			{
				CObjCHAR* pTarget = (CObjCHAR*)(this->Get_TargetOBJ());
				//g_pObjMGR->Get_ClientCharOBJ(this->m_iServerTarget,false);
				
				if(pTarget)
				{
					/// ���ϰ�� PVP���� �ƴ������� �������ݸ���� ����Ѵ�.
					if(this->IsA( OBJ_USER ) && pTarget->IsUSER() )
					{
						if( !g_pTerrain->IsPVPZone() || g_GameDATA.m_iPvPState == PVP_CANT )
						{ m_pObjCART->Set_COMMAND(CMD_STOP); this->Set_COMMAND(CMD_STOP); }
					}
					else
					{
						// ������ ���� ���� !!!
						m_pObjCART->Start_ATTACK(pTarget);
						m_pObjCART->Set_COMMAND(CMD_ATTACK);
						this->Set_COMMAND(CMD_ATTACK);
					}
				}
				else
				{
					m_pObjCART->Set_COMMAND(CMD_STOP);
					this->Set_COMMAND(CMD_STOP);
				}
				
				break;
			}
		
		case SA_RESTORE:
			{
				//���� �������� ����
				this->Set_COMMAND(this->Get_BECOMMAND());
				this->Set_BECOMMAND(CMD_STOP);
				
				if(m_pObjCART->Get_COMMAND() == CMD_ATTACK)
				{
					CObjCHAR* pTarget = (CObjCHAR*)(this->Get_TargetOBJ());
					//g_pObjMGR->Get_ClientCharOBJ(this->m_iServerTarget,false);

					if(pTarget)
					{
						/// ���ϰ�� PVP���� �ƴ������� �������ݸ���� ����Ѵ�.
						if(this->IsA( OBJ_USER ) && pTarget->IsUSER() )
						{
							if( !g_pTerrain->IsPVPZone() || g_GameDATA.m_iPvPState == PVP_CANT )
							{ m_pObjCART->Set_COMMAND(CMD_STOP); this->Set_COMMAND(CMD_STOP); }
						}
						// �ƴϸ� ������ �õ��Ѵ�. 
						else
						{
							m_pObjCART->Start_ATTACK(pTarget);
							m_pObjCART->Set_COMMAND(CMD_ATTACK);
							this->Set_COMMAND(CMD_ATTACK);
						}
					}
				}
			}

	}

}


///--------------------------------------------------------------------------------
/// class : CObjCHAR
/// ����ȣ: SetRideUser() 
///       : 2�ν��� �Ѵ�. 			
///				
//--------------------------------------------------------------------------------
BOOL CObjCHAR::SetRideUser(WORD irideIDX)
{
	//����...
	if(this->GetPetMode() < 0)
		return FALSE;

	//īƮ����...
	if(this->m_pObjCART == NULL)
		return FALSE;

	m_iRideIDX = g_pObjMGR->Get_ClientObjectIndex(irideIDX);

	CObjAVT* pTarget = g_pObjMGR->Get_CharAVT(m_iRideIDX,false);
		if(pTarget == NULL)
			return FALSE;

	//�� ��� ���� 
	pTarget->SetPetType(this->GetPetMode());
	pTarget->m_pObjCART = this->m_pObjCART;
	pTarget->m_IsRideUser = TRUE;

	//īƮ�� ž���� �Ѵ�. 
	this->m_pObjCART->Create(pTarget);

	pTarget->Set_COMMAND(CMD_STOP);
	pTarget->Set_STATE(CS_STOP);


	//�ƹ�Ÿ ���� �� ������ �Ⱥ��̵��� ���� 
	int iVisibilityPart[3] = {BODY_PART_KNAPSACK, BODY_PART_WEAPON_R, BODY_PART_WEAPON_L};

	for(register int i=0;i<3;i++)
	{
		CMODEL<CCharPART> *pCharPART = pTarget->m_pCharMODEL->GetCharPART(iVisibilityPart[i]);
		if (pCharPART) 
		{
			short nI;			

			for (nI=0; nI<pCharPART->m_nPartCNT; nI++) 
			{
				if (pTarget->m_phPartVIS[iVisibilityPart[ i ] ][ nI ] ) 
				{
					::setVisibilityRecursive( pTarget->m_phPartVIS[ iVisibilityPart[ i ] ][ nI ], 0.0f );
				}
			}
		}
	}

	return TRUE;

}


///--------------------------------------------------------------------------------
/// class : CObjCHAR
/// ����ȣ: ReleaseRideUser() 
///       : 2�ν��� �����Ѵ�.  			
///				
//--------------------------------------------------------------------------------
void CObjCHAR::ReleaseRideUser(void)
{

	if(m_iRideIDX == 0)
		return;

	CObjAVT* pTarget = g_pObjMGR->Get_CharAVT(m_iRideIDX,false);
	if(pTarget == NULL)
		return;

	m_iRideIDX = 0;

	//�� ��� ���� 
	pTarget->SetPetType(-1);
	pTarget->m_pObjCART = NULL;
	pTarget->m_IsRideUser = FALSE;
	
	pTarget->Set_STATE(CS_STOP);
	pTarget->SetCMD_STOP();


	//�ƹ�Ÿ ���� �� ������ ���̵��� ���� 
	int iVisibilityPart[3] = {BODY_PART_KNAPSACK, BODY_PART_WEAPON_R, BODY_PART_WEAPON_L};

	for(register int i=0;i<3;i++)
	{
		CMODEL<CCharPART> *pCharPART = pTarget->m_pCharMODEL->GetCharPART(iVisibilityPart[i]);
		if (pCharPART) 
		{
			short nI;			

			for (nI=0; nI<pCharPART->m_nPartCNT; nI++) 
			{
				if (pTarget->m_phPartVIS[iVisibilityPart[ i ] ][ nI ] ) 
				{
					::setVisibilityRecursive( pTarget->m_phPartVIS[ iVisibilityPart[ i ] ][ nI ], 1.0f );
				}
			}
		}
	}	

}


///--------------------------------------------------------------------------------
/// class : CObjCHAR
/// ����ȣ: ReleaseRideUser() 
///       : īƮ ������ ž�� �ߴٸ� �ڽ��� �پ� ������. 			
///				
//--------------------------------------------------------------------------------
void CObjCHAR::ReleaseRideUser(WORD irideIDX)
{
	
	//m_iRideIDX = g_pObjMGR->Get_ClientObjectIndex(irideIDX);
	CObjAVT* pTarget = g_pObjMGR->Get_CharAVT(irideIDX,false);
		if(pTarget == NULL)
			return;

	pTarget->m_pObjCART->UnLinkChild(1);

	//�θ��� �������� 
	pTarget->m_pObjCART->GetParent()->m_iRideIDX	= 0;
	pTarget->m_pObjCART->GetParent()->m_pRideUser	= NULL;
	//�� ��� ���� 
	pTarget->SetPetType(-1);
	pTarget->m_pObjCART = NULL; 
	pTarget->m_IsRideUser = FALSE;
	
	pTarget->Set_STATE(CS_STOP);
	//pTarget->SetCMD_STOP();

	//�پ� ������ ����� �����Ѵ�. 
	pTarget->Set_MOTION(SKILL_ANI_ACTION_TYPE(27),0,1,false,1);
	
	
	//�ƹ�Ÿ ���� �� ������ ���̵��� ���� 
	int iVisibilityPart[3] = {BODY_PART_KNAPSACK, BODY_PART_WEAPON_R, BODY_PART_WEAPON_L};

	for(register int i=0;i<3;i++)
	{
		CMODEL<CCharPART> *pCharPART = pTarget->m_pCharMODEL->GetCharPART(iVisibilityPart[i]);
		if (pCharPART) 
		{
			short nI;			

			for (nI=0; nI<pCharPART->m_nPartCNT; nI++) 
			{
				if (pTarget->m_phPartVIS[iVisibilityPart[ i ] ][ nI ] ) 
				{
					::setVisibilityRecursive( pTarget->m_phPartVIS[ iVisibilityPart[ i ] ][ nI ], 1.0f );
				}
			}
		}
	}	

}


///--------------------------------------------------------------------------------
/// class : CObjCHAR
/// ����ȣ: Ride_Cansel_Motion() 
///       : ž�� �ź� ����� �����Ѵ�. 
///				
//--------------------------------------------------------------------------------
void CObjCHAR::Ride_Cansel_Motion(void)
{
	
	this->Set_COMMAND(CMD_STOP);
	this->SetCMD_STOP();
	
	this->Set_MOTION(453,0,1,false,1);

}


///--------------------------------------------------------------------------------
/// class : CObjCHAR
/// class : Stop_Cart
/// ����ȣ: īƮ�� ���� ��Ų��.  			
///				
//--------------------------------------------------------------------------------
void CObjCHAR::Stop_Cart(void)
{

	if(!m_pObjCART)
		return;
	
	m_pObjCART->Set_COMMAND(CMD_STOP);
	m_pObjCART->SetCMD_STOP();

}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : queuing the command 
///				���� ����� �����Ҽ� �ִ°�?
///				������ ����� �����Ҽ� ���� ���¸� �ľ��϶�..
//--------------------------------------------------------------------------------

bool CObjCHAR::CanApplyCommand()
{
	/// ĳ������ ���۵Ǿ��ٸ� �����Ҽ� ����..
	/// �� result_of_skill �� �޾Ҵٸ�.. �ᱹ ���� ������ ��ų�� ������ ������ ��� ����� ť���ִ´�.
	/// �̹� �������� ����� ����Ȱű⶧���� Ŭ���̾�Ʈ�� ������ ��ų�� �����ؾߵȴ�.
	if( this->m_bCastingSTART && bCanActionActiveSkill() )
	{
		return false;
	}

	/// ���� ����Ǿ���� ���ť�� ������� �ʴٸ� ���� ť�� ����� �����ؾ��ϹǷ�..
	if( this->m_CommandQueue.IsEmpty() == false )
	{
		return false;
	}

	return true;
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : �׿��� ��ɵ��� �����Ѵ�.
///			  �������� �����ؾ��ϴ°�?
//--------------------------------------------------------------------------------

void CObjCHAR::ProcQueuedCommand()
{
	if( m_CommandQueue.IsEmpty() )
		return;

	/// ���� ��ų���� ����� �������̰ų� �����ؾߵ� ��ų�� ���õǾ� �ִٸ� ��ٷ���..
    if( this->m_bCastingSTART && bCanActionActiveSkill() )
	{
		return;
	}

	/// �ϴ� ���� ���������� ����
	bool bSkillCommand = false;
	CObjCommand* pCommand = m_CommandQueue.PopLastCommand( bSkillCommand );
		
	if( pCommand )
	{
		pCommand->Execute( this );
		Log_String( LOG_NORMAL, "!!ť�������� ���� ����!!\n" ); 

		/// Skill ����̰� �̹� Result �� ���� ����̶��..
		if( bSkillCommand )
		{
			if( pCommand->bGetResultOfSkil() )
			{
				SetEffectedSkillFlag( true );
				Log_String( LOG_NORMAL, "!!ť�������� ���� effectedskillflag ����!!\n" ); 
			}
		}
	}
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

/*override*/void CObjCHAR::PushCommandSit()
{
	CObjCommand* pCommand = m_CommandQueue.GetObjCommand( OBJECT_COMMAND_SIT );

	if( pCommand )
	{
		((CObjSitCommand*)pCommand)->SetCMD_SIT( );

		m_CommandQueue.PushCommand( pCommand );
	}else
		assert( 0 && " GetObjCommand failed " );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

/*override*/void CObjCHAR::PushCommandStand()
{
	CObjCommand* pCommand = m_CommandQueue.GetObjCommand( OBJECT_COMMAND_STAND );

	if( pCommand )
	{
		((CObjStandCommand*)pCommand)->SetCMD_STAND( );

		m_CommandQueue.PushCommand( pCommand );
	}else
		assert( 0 && " GetObjCommand failed " );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

/*override*/void CObjCHAR::PushCommandStop()
{
	CObjCommand* pCommand = m_CommandQueue.GetObjCommand( OBJECT_COMMAND_STOP );

	if( pCommand )
	{
		((CObjStopCommand*)pCommand)->SetCMD_STOP( );

		m_CommandQueue.PushCommand( pCommand );
	}else
		assert( 0 && " GetObjCommand failed " );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

/*override*/void CObjCHAR::PushCommandMove( const D3DVECTOR& PosTO, BYTE btRunMODE )
{
	CObjCommand* pCommand = m_CommandQueue.GetObjCommand( OBJECT_COMMAND_MOVE );

	if( pCommand )
	{
		((CObjMoveCommand*)pCommand)->SetCMD_MOVE( PosTO, btRunMODE );

		m_CommandQueue.PushCommand( pCommand );
	}else
		assert( 0 && " GetObjCommand failed " );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

/*override*/void CObjCHAR::PushCommandMove( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget )
{
	CObjCommand* pCommand = m_CommandQueue.GetObjCommand( OBJECT_COMMAND_MOVE );

	if( pCommand )
	{
		((CObjMoveCommand*)pCommand)->SetCMD_MOVE( wSrvDIST, PosTO, iServerTarget );

		m_CommandQueue.PushCommand( pCommand );
	}else
		assert( 0 && " GetObjCommand failed " );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

/*override*/void CObjCHAR::PushCommandAttack( int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosTO )
{
	CObjCommand* pCommand = m_CommandQueue.GetObjCommand( OBJECT_COMMAND_ATTACK );

	if( pCommand )
	{
		((CObjAttackCommand*)pCommand)->SetCMD_ATTACK( iServerTarget, wSrvDIST, PosTO );

		m_CommandQueue.PushCommand( pCommand );
	}else
		assert( 0 && " GetObjCommand failed " );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

/*override*/void CObjCHAR::PushCommandDie()
{
	CObjCommand* pCommand = m_CommandQueue.GetObjCommand( OBJECT_COMMAND_DIE );

	if( pCommand )
	{
		((CObjDieCommand*)pCommand)->SetCMD_DIE( );

		m_CommandQueue.PushCommand( pCommand );
	}else
		assert( 0 && " GetObjCommand failed " );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

/*override*/void CObjCHAR::PushCommandToggle( BYTE btTYPE )
{
	CObjCommand* pCommand = m_CommandQueue.GetObjCommand( OBJECT_COMMAND_TOGGLE );

	if( pCommand )
	{
		((CObjToggleCommand*)pCommand)->SetCMD_TOGGLE( btTYPE );

		m_CommandQueue.PushCommand( pCommand );
	}else
		assert( 0 && " GetObjCommand failed " );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

/*override*/void CObjCHAR::PushCommandSkill2Self( short nSkillIDX )
{
	CObjCommand* pCommand = m_CommandQueue.GetObjCommand( OBJECT_COMMAND_Skill2SELF );

	if( pCommand )
	{
		((CObjSkill2SelfCommand*)pCommand)->SetCMD_Skill2SELF( nSkillIDX );

		m_CommandQueue.PushCommand( pCommand );
	}else
		assert( 0 && " GetObjCommand failed " );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

/*override*/void CObjCHAR::PushCommandSkill2Obj( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget, short nSkillIDX )
{
	CObjCommand* pCommand = m_CommandQueue.GetObjCommand( OBJECT_COMMAND_Skill2OBJ );

	if( pCommand )
	{
		((CObjSkill2ObjCommand*)pCommand)->SetCMD_Skill2OBJ( wSrvDIST, PosTO, iServerTarget, nSkillIDX );

		m_CommandQueue.PushCommand( pCommand );
	}else
		assert( 0 && " GetObjCommand failed " );
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

/*override*/void CObjCHAR::PushCommandSkill2Pos( const D3DVECTOR& PosGOTO, short nSkillIDX )
{
	CObjCommand* pCommand = m_CommandQueue.GetObjCommand( OBJECT_COMMAND_Skill2POS );

	if( pCommand )
	{
		((CObjSkill2PosCommand*)pCommand)->SetCMD_Skill2POS( PosGOTO, nSkillIDX );

		m_CommandQueue.PushCommand( pCommand );
	}else
		assert( 0 && " GetObjCommand failed " );
}


//----------------------------------------------------------------------------------------------------
/// @param
/// @brief Start vibration
//----------------------------------------------------------------------------------------------------

void CObjCHAR::StartVibration()
{
	m_ObjVibration.StartVibration();
}


//--------------------------------------------------------------------------------
/// class : CObjCHAR
/// @brief  : 
//--------------------------------------------------------------------------------

void	CObjCHAR::Set_HP (int iHP)
{	
	if( iHP == DEAD_HP )
		m_iHP = iHP;
	else
	{
		if( iHP <= 0 )
			m_iHP = 1;
		else
			m_iHP = iHP;			
	}
}
void	CObjCHAR::Set_MP (int iMP)
{	
	m_iMP = iMP;			
}

int	CObjCHAR::Add_HP (int iAdd)
{	
	m_iHP += iAdd;

	if( m_iHP <= 0 )
	{		
		LogString( LOG_DEBUG,"Caution : HP < 0 @CObjCHAR::Add_HP" );
		m_iHP = 1;
	}

	if( m_iHP > Get_MaxHP() )
		m_iHP = Get_MaxHP();
	return m_iHP;	
}

int	CObjCHAR::Sub_HP (int iSub)
{	
	m_iHP -= iSub;
	if( m_iHP <= 0 )
		m_iHP = 1;

	return m_iHP;	
}


//----------------------------------------------------------------------------------------------------	
/// @brief ĳ���� ���� ����Ʈ�� ���δ�.
//----------------------------------------------------------------------------------------------------
void CObjCHAR::ShowEffectOnCharByIndex( int iEffectIDX, int iSountIDX, bool bWeatherEffect )
{
	if( iEffectIDX )
	{
		if ( this->IsVisible() ) 
		{
			CEffect *pEffect = g_pEffectLIST->Add_EffectWithIDX( iEffectIDX, true );
			if( pEffect )
			{
				pEffect->LinkNODE ( this->GetZMODEL() );
				
				pEffect->SetParentCHAR( this );
				

				if( !bWeatherEffect )
					this->AddExternalEffect( pEffect );		
				else
					this->AddWeatherEffect( pEffect );

				
				pEffect->InsertToScene ();
			}
		}
	}

	if ( iSountIDX )
	{	
		g_pSoundLIST->IDX_PlaySound3D( iSountIDX, Get_CurPOS() );		
	}
}

void CObjCHAR::ShowEffectOnCharByHash( int iEffectHash, int iSoundIDX, bool bWeatherEffect )
{		
	if( iEffectHash )
	{
		if ( this->IsVisible() ) 
		{
			CEffect *pEffect = g_pEffectLIST->Add_EFFECT( (t_HASHKEY)iEffectHash, true );
			pEffect->LinkNODE ( this->GetZMODEL() );
			
			pEffect->SetParentCHAR( this );

			if( !bWeatherEffect )
				this->AddExternalEffect( pEffect );		
			else
				this->AddWeatherEffect( pEffect );
			
			pEffect->InsertToScene ();
		}
	}

	if ( iSoundIDX )
	{	
		g_pSoundLIST->IDX_PlaySound3D( iSoundIDX, Get_CurPOS() );		
	}
}

void CObjCHAR::AddWeatherEffect( CEffect* pEffect )
{
	m_WeatherEffectLIST.AllocNAppend( pEffect );
}

void CObjCHAR::DeleteWeatherEffect()
{
	classDLLNODE< CEffect* > *pNode;
	pNode = m_WeatherEffectLIST.GetHeadNode ();
	while( pNode ) 
	{		
		/// ����Ʈ�� ����� ����Ʈ�� �θ�ó���� ���Ѵ�. �ֳ�? ���� �θ�ϱ�..
		g_pEffectLIST->Del_EFFECT( pNode->DATA, false );

		m_WeatherEffectLIST.DeleteNFree( pNode );
		pNode = m_WeatherEffectLIST.GetHeadNode( );
	}

	m_WeatherEffectLIST.ClearList();
}


void CObjCHAR::SetClanMark( WORD wMarkBack, WORD wMarkCenter )
{
	m_wClanMarkBack		= wMarkBack;
	m_wClanMarkCenter   = wMarkCenter;
	if( m_ClanMarkUserDefined )
	{
		m_ClanMarkUserDefined->Release();
		m_ClanMarkUserDefined = NULL;
	}
}

void CObjCHAR::SetClan( DWORD dwClanID, WORD wMarkBack, WORD wMarkCenter, const char* pszName , int iLevel, BYTE btClanPos )
{
	//assert( pszName && dwClanID );
	if( pszName && dwClanID)
	{

		m_dwClanID			= dwClanID;
		m_wClanMarkBack		= wMarkBack;
		m_wClanMarkCenter   = wMarkCenter;
		m_iClanLevel		= iLevel;
		m_strClanName		= pszName;
		m_strClanName.erase( remove( m_strClanName.begin(), m_strClanName.end(),' ' ) , m_strClanName.end() );
		m_btClanPos			= btClanPos;
	}
}

BYTE  CObjCHAR::GetClanPos()
{
	return m_btClanPos;
}

void CObjCHAR::SetClanPos( BYTE btPos )
{
	m_btClanPos = btPos;
}

DWORD CObjCHAR::GetClanID()
{
	return m_dwClanID;
}

WORD CObjCHAR::GetClanMarkBack()
{
	return m_wClanMarkBack;
}

WORD CObjCHAR::GetClanMarkCenter()
{
	return m_wClanMarkCenter;
}

const char* CObjCHAR::GetClanName()
{
	return m_strClanName.c_str();
}

void CObjCHAR::ResetClan()
{
	m_dwClanID					= 0;
	m_btClanPos					= 0;
	m_wClanMarkBack				= 0;
	m_wClanMarkCenter			= 0;

	if( m_ClanMarkUserDefined )
	{
		m_ClanMarkUserDefined->Release();
		m_ClanMarkUserDefined = NULL;
	}
}
void CObjCHAR::SetClanLevel( int iLevel )
{
	m_iClanLevel = iLevel;
}
int CObjCHAR::GetClanLevel()
{
	return m_iClanLevel;
}

void CObjCHAR::SetUserDefinedClanMark( CClanMarkUserDefined* pUserDefinedClanMark )
{ 
	assert( pUserDefinedClanMark );
	m_ClanMarkUserDefined = pUserDefinedClanMark; 
	pUserDefinedClanMark->AddRef();
}

///���� ������ Ŭ���̾�Ʈ�� HP���� Ʋ����� �� ���� �����Ѵ�.
void CObjCHAR::SetReviseHP( int hp )
{
	m_ReviseHP = hp;
}

///���� ������ Ŭ���̾�Ʈ�� MP���� Ʋ����� �� ���� �����Ѵ�.
void CObjCHAR::SetReviseMP( int mp )
{
	m_ReviseMP = mp;
}
//-----------------------------------------------------------------------------
/// @brief �Ʒ�� �����ϰ�� �߰� �ɷ�ġ ������: 2005/7/13 - nAvy
//-----------------------------------------------------------------------------
void CObjCHAR::Calc_AruaAddAbility()
{
	if( CCountry::GetSingleton().IsApplyNewVersion() )
	{
		if( m_IsAroa )
			m_AruaAddMoveSpeed   = GetOri_RunSPEED() * 0.2;
		else
			m_AruaAddMoveSpeed   = 0;
	}
}
//-----------------------------------------------------------------------------
/// @brief ���ݼӵ�
//-----------------------------------------------------------------------------
short CObjCHAR::Get_nAttackSPEED ()
{	
	int iR = GetOri_ATKSPEED() + m_EndurancePack.GetStateValue( ING_INC_ATK_SPD )
								- m_EndurancePack.GetStateValue( ING_DEC_ATK_SPD );

	return ( iR > 30 ) ? ( iR ) : 30;
}
//-----------------------------------------------------------------------------
/// @brief ������ ���ݼӵ������ �̿�Ǵ� �޽��			: 2005/7/13 - nAvy
//-----------------------------------------------------------------------------
float CObjCHAR::Get_fAttackSPEED ()
{	
	int iR = GetOri_ATKSPEED() + m_EndurancePack.GetStateValue( ING_INC_ATK_SPD )
								- m_EndurancePack.GetStateValue( ING_DEC_ATK_SPD );

	return ( iR > 30 ) ? ( iR/100.f ) : 0.3f;
}
//---------------------------------------------------------------------------------------------------------------
/// @brief ������ ���ݼӵ������ �̿�Ǵ� �޽�� : �Ʒ�� �����ϰ�� �߰� �ӵ� �����ϵ��� ���� : 2005/7/13 - nAvy
//---------------------------------------------------------------------------------------------------------------
float CObjCHAR::Get_MoveSPEED ()			
{	
	return m_fAdjustSPEED;		
}	

//-----------------------------------------------------------------------------
/// @brief �Ʒ�� �����ϰ�� �߰� �̵��ӵ� �����ϵ��� ���� : 2005/7/13 - nAvy
//-----------------------------------------------------------------------------
float CObjCHAR::Get_DefaultSPEED ()		
{	
	if ( !m_bRunMODE && ( m_btMoveMODE <= MOVE_MODE_RUN ) ) 
		return GetOri_WalkSPEED();
	
	short nR = ( GetOri_RunSPEED() + m_EndurancePack.GetStateValue( ING_INC_MOV_SPD ) 
									- m_EndurancePack.GetStateValue( ING_DEC_MOV_SPD ) );

	nR += m_AruaAddMoveSpeed;

	return ( nR > 200.0f ) ? nR : 200.0f;
}
//---------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @brief  : �� ���� ������ .. static
//--------------------------------------------------------------------------------

DWORD CObjMOB::m_dwCreateOrder=0;

//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @brief  : constructor
//--------------------------------------------------------------------------------

CObjMOB::CObjMOB()
{
	m_nQuestIDX = 0;
	

	m_iMobAniSkill = MOB_ANI_CASTION01;	
}


//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @brief  : destructor
//--------------------------------------------------------------------------------

CObjMOB::~CObjMOB ()
{
	/*
	if ( m_nQuestIDX ) {
		g_pEventLIST->Del_EVENT (xxx);
	}
	*/
//	LogString (LOG_DEBUG, "   CObjMOB::~CObjMOB ( charNo:%d, %s ), Obj:%d  \n", m_nCharIdx, Get_NAME(), m_nIndex );
}


//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @param CGameOBJ *pSourOBJ 
/// @param short nEventIDX
/// @brief  : ���� ������ �̺�Ʈ ó��..( NPC�� ��ȭ �̺�Ʈ �� )
//--------------------------------------------------------------------------------

bool CObjMOB::Check_EVENT (CGameOBJ *pSourOBJ, short nEventIDX)
{
	CGameOBJ* pGameObj = pSourOBJ;
	/// Pet ��� �θ� üũ
	if( pSourOBJ->IsPET() )
	{
		pGameObj = (CGameOBJ*)((CObjCART*)pSourOBJ)->GetParent();
	}


	if ( pGameObj->IsA( OBJ_USER ) && 
		 m_nQuestIDX ) 
	{
		return g_pEventLIST->Run_EVENT( this->Get_INDEX(), m_nQuestIDX, nEventIDX );
	}

	return false;
}


//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @param  : CObjCHAR *pAtk
/// @brief  : ����� �߻� �̺�Ʈ..
//--------------------------------------------------------------------------------

void CObjMOB::Do_DeadEvent	(CObjCHAR *pAtk)
{
	QF_doQuestTrigger( NPC_DESC( m_nCharIdx ) );
}


//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @param  : 
/// @brief  : �� ����
//--------------------------------------------------------------------------------

bool CObjMOB::Create (short nCharIdx, const D3DVECTOR& Position, short nQuestIDX, bool bRunMODE)
{
	char *szName  = CStr::Printf (NAME_MOB_MODEL, NPC_NAME( nCharIdx ), this->m_dwCreateOrder++);

	CCharMODEL *pMODEL = g_MOBandNPC.GetMODEL( nCharIdx );

	
	m_nCharIdx = nCharIdx;
	m_fScale = NPC_SCALE( nCharIdx ) / 100.f;
	if ( CObjCHAR::CreateCHAR (szName, pMODEL, pMODEL->GetPartCNT(), Position) ) 
	{
		this->m_iHP = NPC_HP( m_nCharIdx );
		this->m_iMaxHP = NPC_HP( m_nCharIdx ) * NPC_LEVEL( m_nCharIdx );

		if ( this->m_iHP < 0 )
			g_pCApp->ErrorBOX ("ERROR:: HP <= 0 !!!", (char*)NPC_NAME( nCharIdx ) );

		this->m_bRunMODE      = bRunMODE;
		this->m_fRunAniSPEED  = 1.0f;
//		this->m_fAtkAniSPEED  = (NPC_ATK_SPEED( m_nCharIdx ) / 100.f);

		//this->Adjust_HEIGHT ();

		// EVENT ...
		m_nQuestIDX = nQuestIDX;

		this->New_EFFECT( BODY_PART_WEAPON_R, NPC_R_WEAPON( m_nCharIdx ) );
		this->New_EFFECT( BODY_PART_WEAPON_L, NPC_L_WEAPON( m_nCharIdx ) );


		//----------------------------------------------------------------------------------------------------
		/// Glow effect
		//----------------------------------------------------------------------------------------------------
		unsigned int iColor = NPC_GLOW_COLOR( nCharIdx );
		if( iColor )
		{
			D3DXCOLOR color = CGameUtil::GetRGBFromString( iColor );
			::setVisibleGlowRecursive( this->GetZMODEL(), 2, color.r, color.g, color.b );
		}

		return true;
	}

	LogString (LOG_DEBUG, "MOB Char create failed .. %d: %s \n", nCharIdx, NPC_NAME( nCharIdx ) );

	return false;
}


//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @param  : 
/// @brief  : ����..
//--------------------------------------------------------------------------------

void CObjMOB::Run_AWAY (int iDistance)
{
	D3DVECTOR pos;

	pos.x = RANDOM( iDistance*2 ) - iDistance;
	pos.y = RANDOM( iDistance*2 ) - iDistance;
	pos.x += m_PosBORN.x;
	pos.y += m_PosBORN.y;

	this->SetCMD_MOVE (pos, true);
	this->Set_COMMAND( CMD_RUNAWAY );
}


//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @param  : 
/// @brief  : ĳ���� ����..( �׷��� �𵨳��� �����쳪? )
//--------------------------------------------------------------------------------

bool CObjMOB::Change_CHAR (int nCharIDX)
{
	this->DeleteCHAR ();

	D3DVECTOR PosBORN = Get_BornPOSITION ();

	if ( !this->Create (nCharIDX, m_PosCUR, m_nQuestIDX, m_bRunMODE) ) {
		return false;
	}

	m_PosBORN = PosBORN;
	return true;
}


//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @param  : 
/// @brief  : �������� ����
//--------------------------------------------------------------------------------

bool CObjMOB::Create_PET (int nCharIDX)
{
#ifdef	__VIRTUAL_SERVER
	return ( g_pObjMGR->Add_MobCHAR( 0, nCharIDX, m_PosCUR, 0, true ) ? true : false );
#else
	return false;
#endif
}


//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @param  : 
/// @brief  : �ΰ����� �޼���..
//--------------------------------------------------------------------------------

void CObjMOB::Say_MESSAGE (char *szMessage)
{
	//AddMsgToChatWND (CStr::Printf("%s> %s", this->Get_NAME (), szMessage ), g_dwBLACK );
	g_UIMed.AddChatMsg( g_pObjMGR->Get_ServerObjectIndex( this->Get_INDEX() ), szMessage, g_dwRED );
}


//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @param  : 
/// @brief  : �������� �����ġ ������ ���ؼ� ���� ����Ǿ��ִ� �ɷ¼�ġ( �нú� ��ų ���� )
//--------------------------------------------------------------------------------

int	CObjMOB::Get_DefaultAbilityValue( int iType )
{
	switch( iType )
	{
		case AT_SPEED:		
		case AT_ATK_SPD:
			return 1;			

		case AT_MAX_HP:
			{
				return this->m_iMaxHP;
			}
			break;

		case AT_MAX_MP:
			{
				return this->m_iMaxMP;
			}
			break;

		default:
			return 1;
	}

	return 1;
}

/// NPC�� ��� STB�� ������ ���̰� �� ������ �� ���̸� ����Ѵ�.
void CObjMOB::GetScreenPOS ( D3DVECTOR &PosSCR)
{
	float fStature = NPC_HEIGHT( this->m_nCharIdx );

	if( fStature != 0 )
	{
		::worldToScreen( m_PosCUR.x, m_PosCUR.y, getPositionZ(m_hNodeMODEL) + fStature, &PosSCR.x, &PosSCR.y, &PosSCR.z );
		return;
	}

	// ���� ��ǥ�� Ű�� ���� ��ġ�� �̸���� ��ġ�� ����
	::worldToScreen( m_PosCUR.x, m_PosCUR.y, getPositionZ(m_hNodeMODEL) + m_fStature, &PosSCR.x, &PosSCR.y, &PosSCR.z );
}

//--------------------------------------------------------------------------------
/// class : CObjMOB
/// @param  : 
/// @brief  : ���� �Ÿ�.. ��ų������̸� ��ų���̺��� �ƴϸ� NPC ���̺��̰���..
//--------------------------------------------------------------------------------

int CObjMOB::Get_AttackRange()
{
	/// ��ų�� ���� �Ÿ��� �ԷµǾ� �ִٸ� ��ų�Ÿ� �ƴϸ� ���� �Ÿ�..
	if ( this->m_nToDoSkillIDX > 0 ) 
	{
		if( this->m_nToDoSkillIDX < g_SkillList.Get_SkillCNT() )
		{
			if(	SKILL_DISTANCE( this->m_nToDoSkillIDX ) ) 
			{
				return SKILL_DISTANCE( this->m_nToDoSkillIDX );
			}
		}else
			assert( 0 && "Get_AttackRange Failed[ ToDoSkillIDX is invalid ]" );
	}

	/// ��ų�� ���� �Ÿ��� �ԷµǾ� �ִٸ� ��ų�Ÿ� �ƴϸ� ���� �Ÿ�..
	//if ( this->m_nToDoSkillIDX && SKILL_DISTANCE( this->m_nToDoSkillIDX ) ) 
	//{
	//	return SKILL_DISTANCE( this->m_nToDoSkillIDX );
	//}
	
	// Default attack range...
	return ( Def_AttackRange () + ( Get_SCALE() * 120 ) );
}

//------------------------------------------------------------------------------------------------
/// ���� ������ ���� ���..
//------------------------------------------------------------------------------------------------
void CObjMOB::PlayStopSound()
{
	int iIndex = NPC_NORMAL_EFFECT_SOUND( this->Get_CharNO() );
	g_pSoundLIST->IDX_PlaySound3D( iIndex, this->Get_CurPOS() );				
}

/*override*/ int CObjMOB::Proc()
{
	int iResult = CObjCHAR::Proc();

	//--------------------------------------------------------------------------------
	// ��� ������ ������.
	//--------------------------------------------------------------------------------
	if( this->Get_COMMAND() == CMD_STOP )	
	{			
		if ( !m_bFrameING ) 
		{
			if( RANDOM( 100 ) < 20 )
				PlayStopSound();
		}
	}

	return iResult;
}



//--------------------------------------------------------------------------------
// 05.05.19 icarus:: WOW����� ����Ʈ ���� �ý��� ������ ���� �߰�..
CObjNPC::CObjNPC()
{
	m_nQuestSignal = -1;
}

void CObjNPC::SetEventValue( int iEventValue )
{ 
	m_iEventSTATUS = iEventValue; 
}

int CObjNPC::GetEventValue()
{ 
	return m_iEventSTATUS; 
}


int CObjNPC::Proc ()
{
	if( CCountry::GetSingleton().IsApplyNewVersion() && ( this->m_nQuestSignal < 0 || g_pAVATAR->m_bQuestUpdate ) )
	{
		// �� �ƹ�Ÿ�� ����Ʈ ���°� ���ŵǾ��ų� ������ ����Ʈ ���¸� �Ǵ������� ���ٸ�...
		this->m_nQuestSignal = SC_QF_GetNpcQuestStatus( this->m_nCharIdx );
		_RPT1( _CRT_WARN,"SC_QF_GetNpcQuestStatus :%d \n", m_nQuestSignal );
	}

	return CObjMOB::Proc ();
}



//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : constructor
//--------------------------------------------------------------------------------

CObjAVT::CObjAVT()
{
//	m_fScale  = 0.5;
	m_dwSitTIME			= 0;
	m_bIsFemale			= false;
	m_bRunMODE			= true;	// �ƹ�Ÿ �⺻�� �޸���..
	m_nRunSPEED			= WALK_CmPerSec;

	m_pCharMODEL		= &m_CharMODEL;	// Init ..

	m_iExpression		= 0;

	m_iDoChangeWeaponR	= 0;
	m_iDoChangeWeaponL	= 0;
	m_iCon				= 1;
	m_iRecoverHP		= 1;
	m_iRecoverMP		= 1;


	m_iLevel = 1;

	::ZeroMemory( m_sPartItemIDX, sizeof(tagPartITEM)*MAX_BODY_PART );

	///
	/// Personal store
	///
	m_bPersonalStoreMode	= false;
	m_pObjPersonalStore		= NULL;
    
	///
	/// pet
	///
	::ZeroMemory( m_sPetPartItemIDX, sizeof(tagPartITEM)*MAX_RIDING_PART );
	//m_iPetType			= 0;
	//m_pObjCART			= NULL;
	m_btWeightRate		= 0;

	m_pWeaponJemEffect	= 0;
	m_nStamina			= 0;///�ּҰ����� ����
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : destructor
//--------------------------------------------------------------------------------

CObjAVT::~CObjAVT ()
{	
	DeleteCart();

	//--------------------------------------------------------------------------------
	/// ���,��ð��� ȿ�� �����.
	//--------------------------------------------------------------------------------
	DeleteGemmingEffect();
	DeleteGreadEffect();

	if( m_pObjPersonalStore )
	{
		delete m_pObjPersonalStore;
		m_pObjPersonalStore = NULL;
	}
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : ��Ʈ ���� ����( ���ø� )
//--------------------------------------------------------------------------------

void CObjAVT::SetPartITEM (short nPartIdx, short nItemNo)
{
	this->New_EFFECT( nPartIdx, nItemNo, false );

	m_sPartItemIDX[ nPartIdx ].m_nItemNo = nItemNo;

	/// �������� ������°Ŷ�� ��� ������ Ŭ����.
	if( nItemNo == 0 )	
	{
		m_sPartItemIDX[ nPartIdx ].m_bHasSocket = false;
		m_sPartItemIDX[ nPartIdx ].m_cGrade = 0;
		m_sPartItemIDX[ nPartIdx ].m_nGEM_OP = 0;
	}

	//g_pSoundLIST->IDX_PlaySound( SOUND_EQUIP_ITEM  );
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : ** ���� ��ǥ�� �Ѿ�;� �Ѵ�.
///				�ƹ��� ����
//--------------------------------------------------------------------------------

bool CObjAVT::Create (const D3DVECTOR &Position, BYTE btCharRACE)
{
	m_btRace = btCharRACE;
	m_bIsFemale = btCharRACE & 0x01;
	m_CharMODEL.Set_SEX( m_bIsFemale );

	for (short nItemIDX, nI=0; nI<MAX_BODY_PART; nI++) 
	{
		nItemIDX = m_sPartItemIDX[ nI ].m_nItemNo;

		/// �Ӹ�ī���̶�� ���ڿ� ��︮�� �Ӹ�ī������ ��ü.
		if ( nI == BODY_PART_HAIR ) 
		{
			nItemIDX += HELMET_HAIR_TYPE( m_sPartItemIDX[ BODY_PART_HELMET ].m_nItemNo );
		}

		/// ���̶�� ������ ǥ�� �󱼷� ��ü..
		if( nI == BODY_PART_FACE )
		{
			nItemIDX += GetCharExpression();
		}

		this->m_CharMODEL.SetPartMODEL( nI, nItemIDX );
		//this->New_EFFECT( nI, nItemIDX );
	}

	D3DVECTOR charPos = Position;
	charPos.z = g_pTerrain->GetHeightTop(Position.x, Position.y);

	if ( CObjCHAR::CreateCHAR ( (char*)m_Name.c_str(), &m_CharMODEL, MAX_BODY_PART, charPos) ) {
		m_nAtkAniSPEED = Cal_AtkAniSPEED( this->GetPartITEM(BODY_PART_WEAPON_R) );

		m_iHP = 100;

		// SetPartITEM���� ������ ȿ�� link
		this->Link_EFFECT ();

		// �ɸ��� ����.
		m_fStature = ::getModelHeight (this->m_hNodeMODEL);

		this->New_EFFECT( BODY_PART_WEAPON_R, this->GetPartITEM(BODY_PART_WEAPON_R));
		this->New_EFFECT( BODY_PART_WEAPON_L, this->GetPartITEM(BODY_PART_WEAPON_L));		


		CreateGemmingEffect();
		CreateGradeEffect();

		return true;
	}

	return false;
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : Ŭ���� ���ο� ������ �ƹ�Ÿ ������ �̿��ؼ�, ������Ʈ �Ѵ�.
//--------------------------------------------------------------------------------

void CObjAVT::Update (bool bUpdateBONE )
{
	// 0. ȿ�� unlink  :: DeletePARTS���� �����Ǹ鼭 �������� �ڵ����� unlink ??
	this->Unlink_EFFECT ();

	// 1. �����
	// addRenderUnit �Ȱ͵��� clearRenderUnit ( HNODE hVisible ); �� ����
	// loadVisible �Ȱ͵��� unloadVisible�� ����...
	this->DeletePARTS (false);	

	//--------------------------------------------------------------------------------
	/// ���,��ð��� ȿ�� �����.
	//--------------------------------------------------------------------------------
	DeleteGemmingEffect();
	DeleteGreadEffect();


	if ( bUpdateBONE ) {
		m_pCharMODEL->UnlinkBoneEFFECT( m_ppBoneEFFECT );

		this->UnloadModelNODE ();
		this->LoadModelNODE ( (char*)m_Name.c_str() );
		if ( m_bIsVisible ) {
			::insertToScene( m_hNodeMODEL );
			::setPosition (this->m_hNodeMODEL, m_PosCUR.x, m_PosCUR.y, m_PosCUR.z);			
		}

		m_pCharMODEL->LinkBoneEFFECT( m_hNodeMODEL, m_ppBoneEFFECT );		
	}

	// 2. ����Ÿ ����.
	for (short nItemIDX, nI=0; nI<MAX_BODY_PART; nI++) 
	{
		nItemIDX = m_sPartItemIDX[ nI ].m_nItemNo;

		if ( nI == BODY_PART_HAIR ) 
		{
			nItemIDX += HELMET_HAIR_TYPE( m_sPartItemIDX[ BODY_PART_HELMET ].m_nItemNo );
		}

		
		/// ���̶�� ������ ǥ�� �󱼷� ��ü..
		if( nI == BODY_PART_FACE )
		{
			nItemIDX += GetCharExpression();
		}
		

		this->m_CharMODEL.SetPartMODEL( nI, nItemIDX );		
	}


	// 4. �� ����.
	this->CreatePARTS ( (char*)m_Name.c_str() );

	this->InsertToScene ();

	// 5. ȿ�� relink
	this->Link_EFFECT ();


	//----------------------------------------------------------------------------------------------------		
	/// @brief ���, ��ð��� ����Ʈ ����
	//----------------------------------------------------------------------------------------------------
	
	//2005. 06. 15 �� ��ȣ
	Set_RareITEM_Glow();

	CreateGemmingEffect();
	CreateGradeEffect();


	// 6. ���Ⱑ �ٲ�� ���� ������ ����� Ʋ�� �����...
	#pragma message ("TODO:: change motion ..." __FILE__)



	// �ɸ��� ����. // ���� �̻��� ���ܻ��̳� ��Ÿ ������Ʈ�� ���̵� ����.. InsertToScene ������ �ű���..
	m_fStature = ::getModelHeight (this->m_hNodeMODEL);


	/// īƮ�� Ÿ�� �ִٸ�..
	if( GetPetMode() >= 0 )
	{
		this->UpdatePet();
	}

	/// CheckVisibiliey
	m_EndurancePack.UpdateVisibility();
}


//----------------------------------------------------------------------------------------------------		
/// @brief ���, ��ð��� ����Ʈ..
//----------------------------------------------------------------------------------------------------
const int iWeaponAttachedEffectDummyNO = 2;
void CObjAVT::CreateGemmingEffect()
{
	//----------------------------------------------------------------------------------------------------		
	/// @brief ����̳� ��ð���.. �پ��ٸ� ȿ���� �ٿ���..
	//----------------------------------------------------------------------------------------------------
	
	for( int i = BODY_PART_WEAPON_R; i <= BODY_PART_WEAPON_L ; i++ )
	{
		int iPartIdx = i;
		int iItemIDX = m_sPartItemIDX[ iPartIdx ].m_nItemNo;
		if( iItemIDX == 0 )
			continue;

		if( m_sPartItemIDX[ iPartIdx ].m_bHasSocket && m_sPartItemIDX[ iPartIdx ].m_nGEM_OP > 300 )
		{			
			int iEffectIDX = 0;
			
			iEffectIDX = GEMITEM_ATTACK_EFFECT( m_sPartItemIDX[ iPartIdx ].m_nGEM_OP );			
			if( iEffectIDX )
			{
				/// List_Effect ���� 4������ ȿ���� ������ ������.. �ϳ���..
				int iStartPointNO = 0;
				switch( iPartIdx )
				{
					case BODY_PART_WEAPON_R:
						iStartPointNO = WEAPON_GEMMING_POSITION( iItemIDX );
						break;

					case BODY_PART_WEAPON_L:
						iStartPointNO = SUBWPN_GEMMING_POSITION( iItemIDX );
						break;
				}

				for( int i = 0; i < 1 ; i++ )
				{
					int iEffect = EFFECT_POINT( iEffectIDX, i );
					
					m_pWeaponJemEffect = g_pEffectLIST->Add_EffectWithIDX( iEffect );
					if ( m_pWeaponJemEffect ) 
					{
						CMODEL<CCharPART> *pCharPART = g_DATA.Get_CharPartMODEL( iPartIdx, iItemIDX, this->IsFemale() );

						if( pCharPART && 
							m_phPartVIS[ iPartIdx ] &&
							iStartPointNO < pCharPART->m_nDummyPointCNT &&
							m_phPartVIS[ iPartIdx ][ pCharPART->m_pDummyPoints[ iStartPointNO + i ].m_nParent ] )
						{								
							m_pWeaponJemEffect->Rotation( pCharPART->m_pDummyPoints[ iStartPointNO + i ].m_Rotate );
							m_pWeaponJemEffect->Transform( pCharPART->m_pDummyPoints[ iStartPointNO + i ].m_Transform );						
							m_pWeaponJemEffect->LinkNODE( m_phPartVIS[ iPartIdx ][ pCharPART->m_pDummyPoints[ iStartPointNO + i ].m_nParent ] );
						}else
						{
							g_pEffectLIST->Del_EFFECT( m_pWeaponJemEffect );
						}
					}
				}

				/*m_pWeaponJemEffect = g_pEffectLIST->Add_EffectWithIDX( iEffectIDX );
				if ( m_pWeaponJemEffect ) 
				{
					CMODEL<CCharPART> *pCharPART = g_DATA.Get_CharPartMODEL( iPartIdx, iItemIDX, this->IsFemale() );

					if( iWeaponAttachedEffectDummyNO < pCharPART->m_nDummyPointCNT )
					{				
						m_pWeaponJemEffect->Transform( pCharPART->m_pDummyPoints[ iWeaponAttachedEffectDummyNO ].m_Transform );
						m_pWeaponJemEffect->Rotation( pCharPART->m_pDummyPoints[ iWeaponAttachedEffectDummyNO ].m_Rotate );
						m_pWeaponJemEffect->LinkNODE( m_phPartVIS[ iPartIdx ][ pCharPART->m_pDummyPoints[ iWeaponAttachedEffectDummyNO ].m_nParent ] );
					}else
					{
						g_pEffectLIST->Del_EFFECT( m_pWeaponJemEffect );
					}
				}*/
			}
		}	
	}
}

//----------------------------------------------------------------------------------------------------		
/// @brief ���, ��ð��� ����Ʈ..
//----------------------------------------------------------------------------------------------------
void CObjAVT::DeleteGemmingEffect()
{
	if( m_pWeaponJemEffect )
	{
		m_pWeaponJemEffect->UnlinkNODE();

		g_pEffectLIST->Del_EFFECT( m_pWeaponJemEffect );
		m_pWeaponJemEffect = NULL;
	}
}


//2005.06. 15 ����ȣ 
//----------------------------------------------------------------------------------------------------		
/// �븸�� ��� ���� �������϶� glow ȿ���� 3�ܰ� �����Ѵ�. 
//----------------------------------------------------------------------------------------------------
void CObjAVT::Set_RareITEM_Glow(void)
{
	
	//�븸�� �ƴϸ� �����Ѵ�.
//	if(!CCountry::GetSingleton().IsTAIWAN()) 
//	  return;

	//�븸�� �ʸ��ɿ� ����˴ϴ�. ���� 2005.6.20 nAvy
	if( !CCountry::GetSingleton().IsApplyNewVersion() )
		return;

	CInventory	m_Inventory;				
	short		m_iType = 0;
	

	for( int i = 1; i < MAX_BODY_PART ; i++ )
	{
		int iItemIDX = m_sPartItemIDX[ i ].m_nItemNo;
		
		if( iItemIDX == 0 )
			continue;
		
		//���� ���������� üũ�Ѵ�. 
		m_iType = m_Inventory.GetBodyPartToItemType(i);	
			if(m_iType == MAX_BODY_PART)
				continue;
		

		if(ITEM_RARE_TYPE(m_iType,m_sPartItemIDX[i].m_nItemNo) != 3)
			continue;

		//3�ܰ� Glow ȿ�� ���� 
		m_sPartItemIDX[i].m_cGrade = 3;
	}

}


//----------------------------------------------------------------------------------------------------		
/// @brief ���, ��ð��� ����Ʈ..
//----------------------------------------------------------------------------------------------------
void CObjAVT::CreateGradeEffect()
{
	CMODEL<CCharPART> *pCharPART = NULL;

	for( int i = 0; i < MAX_BODY_PART ; i++ )
	{
		int iItemIDX = m_sPartItemIDX[ i ].m_nItemNo;
		if( iItemIDX == 0 )
			continue;

		if( m_sPartItemIDX[ i ].m_cGrade != 0 )
		{
			//----------------------------------------------------------------------------------------------------
			/// Glow effect
			//----------------------------------------------------------------------------------------------------
			unsigned int iColor = ITEMGRADE_GLOW_COLOR( m_sPartItemIDX[ i ].m_cGrade );
			if( iColor )
			{
				D3DXCOLOR color = CGameUtil::GetRGBFromString( iColor );
				
				/// Skinning �Ǵ� ������Ʈ�� �ƴ϶��..
				if( m_pCharMODEL )
				{
					if( m_pCharMODEL->m_RenderUnitPart[ i ].empty() )
					{
						pCharPART = g_DATA.Get_CharPartMODEL( i, iItemIDX, this->IsFemale() );
						if( pCharPART )
						{
							for( int j = 0; j < pCharPART->m_nPartCNT; j++ )
							{
								if( m_phPartVIS[ i ] && m_phPartVIS[ i ][ j ] )
									::setVisibleGlow( m_phPartVIS[ i ][ j ], ZZ_GLOW_TEXTURE, color.r, color.g, color.b );
							}
						}
					}else
					{
						/// ��Ű�� �Ǵ� ������Ʈ�� ���..
						std::list< int >::iterator	begin = m_pCharMODEL->m_RenderUnitPart[ i ].begin();
						for( ; begin != m_pCharMODEL->m_RenderUnitPart[ i ].end() ; ++begin )
						{
							int iRenderUnitIndex = *begin;
							::setVisibleRenderUnitGlow( this->GetZMODEL(), iRenderUnitIndex, ZZ_GLOW_TEXTURE, color.r, color.g, color.b );
							//::setVisibleGlow( this->GetZMODEL(), ZZ_GLOW_SIMPLE, 1.0f, 1.0f, 1.0f );
						}
					}
				}
			}	
		}
	}
}

//----------------------------------------------------------------------------------------------------		
/// @brief ���, ��ð��� ����Ʈ..
//----------------------------------------------------------------------------------------------------
void CObjAVT::DeleteGreadEffect()
{
}



//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : �Ѿ� ��ȣ�� ���Ѵ�. ��Ÿ� ������ ���.. ���� ���õ� �Ѿ˿� ���� �Ǵ�
//--------------------------------------------------------------------------------

/*override*/int	CObjAVT::Get_BulletNO ()
{	
	tagITEM sItem;
	sItem.m_cType   = ITEM_TYPE_WEAPON;
	sItem.m_nItemNo = Get_R_WEAPON();

	int iBulletNo = 0;
	int iShotType = sItem.GetShotTYPE();

	/// �Ѿ˼Ҹ� ���ϴ� ���� ����
	if( iShotType >= MAX_SHOT_TYPE )
	{
		switch( WEAPON_TYPE( sItem.m_nItemNo ) ) 
		{
			case WEAPON_ITEM_NOT_USE_BULLET:
				{
					iBulletNo = WEAPON_BULLET_EFFECT( sItem.m_nItemNo );
					return iBulletNo;
				}
				break;		
		}		
	}

	if( iShotType < MAX_SHOT_TYPE && m_ShotData[iShotType].m_nItemNo )
		iBulletNo = NATURAL_BULLET_NO( m_ShotData[iShotType].m_nItemNo );

	return iBulletNo;
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : ���� �׼ǰ� ���⿡ �´� ����� ���Ѵ�.
//--------------------------------------------------------------------------------

tagMOTION *CObjAVT::Get_MOTION (short nActionIdx)
{
	int iActionIdx = m_ChangeActionMode.GetAdjustedActionIndex( nActionIdx );

	// ������ ���� ����������...
	short nWeaponTYPE = WEAPON_MOTION_TYPE( this->m_sRWeaponIDX.m_nItemNo );

	short nFileIDX = FILE_MOTION( nWeaponTYPE, iActionIdx );

	if ( 0 == nFileIDX ) {
		// ����� ������ �Ǽ� ������� ��ü..
		nFileIDX = FILE_MOTION( 0, nActionIdx );
	}

	tagMOTION *pMOTION = g_MotionFILE.IDX_GetMOTION( nFileIDX, m_bIsFemale );
	if ( pMOTION ) {
		pMOTION->m_nActionIdx = nActionIdx;
	}
	return pMOTION;
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : �ɱ� ���⺸�� ���
//--------------------------------------------------------------------------------

bool CObjAVT::ToggleSitMODE ()
{
	if ( this->Get_COMMAND() == CMD_SIT ) {
		this->SetCMD_STAND ();
	} else {
		// Ŭ���̾�Ʈ�� ������ ������.
		m_dwSitTIME = 0;
		this->SetCMD_SIT ();
	}
	::setRepeatCount( m_hNodeMODEL, 1 );

	return true;
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : float fAdjRate : ���� ������ �̵��ӵ��� ����Ʈ �ӵ��� ���� ����..
/// @brief  : �ٱ� �ȱ� ��� ���
///				m_bRunMODE ������ ���¿� ���� �ӵ� ���� default speed�� Ʋ������.
///             �̰� �������� �Լ� ���ο��� ���������� ������ �ۿ���..
//--------------------------------------------------------------------------------

bool CObjAVT::ToggleRunMODE ( float fAdjRate )
{	
	///float fAdjRate = this->Get_MoveSPEED() / this->Get_DefaultSPEED ();
	//m_bRunMODE = !m_bRunMODE;
	if ( Get_STATE() == CS_MOVE ) 
	{
		m_fAdjustSPEED = this->Get_DefaultSPEED () * fAdjRate;

		this->Set_CurMOTION( this->Get_MOTION( this->GetANI_Move() ) );
#ifdef	_DEBUG
		float fMoveSpeed = this->Get_MoveSPEED();
		_ASSERT( fMoveSpeed >= 0.f && fMoveSpeed < 2000.f );
#endif

		// �̵� �ӵ� ��� ���ǵ� ����...
		this->Set_ModelSPEED( this->Get_MoveSPEED() );

		::attachMotion		( this->m_hNodeMODEL, this->Get_ZMOTION()  );
		::setAnimatableSpeed( this->m_hNodeMODEL, ( m_bRunMODE )?this->m_fRunAniSPEED:1.0f );

		return true;
	}

	return false;
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : ���� ����
//--------------------------------------------------------------------------------

int	CObjAVT::GetPetMode()			
{
	if( m_pObjCART )
		return m_pObjCART->GetCartType();
	return -1; 
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : ���� ����
//--------------------------------------------------------------------------------

bool CObjAVT::CanAttackPetMode()
{
	int iPetMode = this->GetPetMode();
	if( iPetMode > 0 )
	{
		/// Pet mode �� ��쿡��..
		/*switch( iPetMode )
		{
			case PET_TYPE_CART01:
				return false;
			case PET_TYPE_CASTLE_GEAR01:
				return true;
		}*/
		//----------------------------------------------------------------------------------------------------
		/// @brief Pet �� Ÿ���� �ƴ϶� ���ݰŸ��� �ֳ� ���ķ� ���ݰ��ɿ��� ����..
		//----------------------------------------------------------------------------------------------------

//����ȣ::
#if defined(_GBC)
		if( PAT_ITEM_ATK_RANGE( this->m_sWeaponIDX.m_nItemNo ) <= 0 )
#else
		if( PAT_ITEM_ATK_RANGE( this->m_sAbilIDX.m_nItemNo ) <= 0 )
#endif 		
			return false;	

	}

	return true;
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : 
//--------------------------------------------------------------------------------

void CObjAVT::SetCMD_PET_MOVE( const D3DVECTOR& PosTO, BYTE btRunMODE )
{ 
	if( m_pObjCART )
	{
		m_pObjCART->SetCMD_MOVE( PosTO, btRunMODE ); 
	}
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : 
//--------------------------------------------------------------------------------

void CObjAVT::SetCMD_PET_MOVE( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget)
{ 
	if( m_pObjCART )
	{
		m_pObjCART->SetCMD_MOVE( wSrvDIST, PosTO, iServerTarget ); 
	}
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : 
//--------------------------------------------------------------------------------

void CObjAVT::SetCMD_PET_STOP( void )
{ 
	if( m_pObjCART )
	{
		m_pObjCART->SetCMD_STOP(); 
	}
}

//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : 
//--------------------------------------------------------------------------------

void CObjAVT::SetCMD_PET_ATTACK( int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosTO )
{ 
	if( m_pObjCART )
	{
		m_pObjCART->SetCMD_ATTACK( iServerTarget, wSrvDIST, PosTO ); 		
	}
}



//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : 
//--------------------------------------------------------------------------------

void CObjAVT::RideCartToggle( bool bRide )
{
	if( bRide == false )
	{
		if( GetPetMode() >= 0 )
		{
			/// ������
			DeleteCart( true );
		}else
		{
			/// Ÿ��
			CreateCartFromMyData( true );
		}

	}else
	{
		/// ���� īƮ�� ź���°� �ƴҰ�쿡�� Ÿ��
		if( GetPetMode() < 0 )			
			CreateCartFromMyData( true );
	}

	//���� ����.
	this->m_EndurancePack.ClearStateByDriveCart();

}


//-------------------------------------------------------------------------------------------------
#define	CHECK_TIME	1500

//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : HP��MP������ CObjAI::RecoverHPnMP() ���� ó���ǹǷ� ���� �ڵ��� �̰��� �����.	- 2004/02/11 : nAvy 
///           Aroa�����ϰ�� 50% �߰��� ȸ���ӵ�����											- 2005/07/12 : nAvy
//--------------------------------------------------------------------------------

int CObjAVT::Proc ()
{
	m_dwElapsedTime += m_dwFrameElapsedTime;

	//--------------------------------------------------------------------------------
	/// ��Ŀ������ ó��	
	//--------------------------------------------------------------------------------
	///if( GetStamina() > 3000 )
	{
		int	iRecoverStateCheckTime = RECOVER_STATE_CHECK_TIME_OLD;
		if( CCountry::GetSingleton().IsApplyNewVersion() )
			iRecoverStateCheckTime = RECOVER_STATE_CHECK_TIME_NEW;

		int iRecoverStateSitOnGround = RECOVER_STATE_SIT_ON_GROUND_OLD;
		if( CCountry::GetSingleton().IsApplyNewVersion() )
			iRecoverStateSitOnGround = RECOVER_STATE_SIT_ON_GROUND_NEW;


		int iRecoverStateStopWalk    = RECOVER_STATE_STOP_OR_WALK_OLD;
		if( CCountry::GetSingleton().IsApplyNewVersion() )
			iRecoverStateStopWalk = RECOVER_STATE_STOP_OR_WALK_NEW;



		if ( m_dwElapsedTime > iRecoverStateCheckTime ) 
		{
			//_RPT3( _CRT_WARN,"Update Recover HP/MP ElapsedTime:%d, CheckTime:%d, CurrTime:%d \n", m_dwElapsedTime, iRecoverStateCheckTime, timeGetTime() );
			switch ( Get_COMMAND() ) 
			{
			case CMD_SIT :
				this->RecoverHP( iRecoverStateSitOnGround );
				this->RecoverMP( iRecoverStateSitOnGround );							
				break;
			case CMD_DIE:
				break;
			default:
				/// ĳ����� ž�����϶��� ȸ�� ����
				if( this->GetPetMode() < 0 )
				{
					/// �ɱⰡ �ƴѵ��ۿ����� HP�� ȸ��
					this->RecoverHP( iRecoverStateStopWalk );			
					this->RecoverMP( iRecoverStateStopWalk );
				}
				break;	
			}	
			m_dwElapsedTime -= iRecoverStateCheckTime;
		}
	}/*else
	{
		m_dwElapsedTime = 0;
	}*/

	//--------------------------------------------------------------------------------
	// ����ȣ: �� ���̺극�̼� 
	// ������: ���ǹ��� m_pObjCART�߰�(������ īƮ���� ������ ���� �߻� ) 2005/7/31
	//--------------------------------------------------------------------------------
	if(GetPetMode() && m_pObjCART && SetCartVA())
	{	
		m_pObjCART->m_ObjVibration.StartVibration();
		SetCartVA() = FALSE;
	}

	if(GetPetMode() && m_pObjCART)
		m_pObjCART->m_ObjVibration.Proc();

	//--------------------------------------------------------------------------------
	/// �ð��� ���� �׼� ��� ó��..
	/// if �� ���̱� ���ؼ� �������� �̵�.. 04/5/28
	//--------------------------------------------------------------------------------
	///if( this->IsA( OBJ_AVATAR ) || this->IsA( OBJ_USER ) )
	{
		m_ChangeActionMode.Proc();
	}		
	
	return CObjCHAR::Proc ();
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : �Ѿ� ������ ����
//--------------------------------------------------------------------------------

void CObjAVT::SetShotData( int i, int iItemNo )
{ 
	if( i < 0 || i >= MAX_SHOT_TYPE )
		return;

	m_ShotData[i].m_nItemNo = iItemNo; 
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : �������� �����ġ ������ ���ؼ� ���� ����Ǿ��ִ� �ɷ¼�ġ( �нú� ��ų ���� )
//--------------------------------------------------------------------------------

int	CObjAVT::Get_DefaultAbilityValue( int iType )
{
	switch( iType )
	{
		case AT_SPEED:
			{
				return this->GetOri_WalkSPEED();
			}
			break;

		case AT_ATK_SPD:
			{
				return this->GetOri_ATKSPEED();
			}
			break;

		case AT_MAX_HP:
			{
				return this->m_iMaxHP;
			}
			break;

		case AT_MAX_MP:
			{
				return this->m_iMaxMP;
			}
			break;

		default:
			return 1;
	}

	return 1;
}




/*override*/int CObjAVT::GetANI_Stop()			
	{
		if( m_pObjCART )
			return GetANI_Ride();
		return  AVT_ANI_STOP1;				
	}
/*override*/int	CObjAVT::GetANI_Move()		{	return  m_bRunMODE ? AVT_ANI_RUN : AVT_ANI_WALK;	}
/*override*/int	CObjAVT::GetANI_Attack()	{	return( AVT_ANI_ATTACK+RANDOM(3) );	}
/*override*/int	CObjAVT::GetANI_Die ()			
{	
	if( m_bStopDead )
		return AVT_ANI_STOP1;
	return  AVT_ANI_DIE;				
}
/*override*/int	CObjAVT::GetANI_Hit()		{	return  AVT_ANI_HIT;				}
/*override*/int	CObjAVT::GetANI_Casting ()	{	return	SKILL_ANI_CASTING(m_nActiveSkillIDX);	}
/*override*/int	CObjAVT::GetANI_CastingRepeat()	{   return SKILL_ANI_CASTING_REPEAT(m_nActiveSkillIDX);	}		/// ���������� ĳ���õ������� ���..
/*override*/int	CObjAVT::GetANI_Skill ()	{	return	SKILL_ANI_ACTION_TYPE(m_nActiveSkillIDX);	}
/*override*/int	CObjAVT::GetANI_Sitting()	{	return	AVT_ANI_SITTING;			}
/*override*/int	CObjAVT::GetANI_Standing()	{	return	AVT_ANI_STANDUP;			}
/*override*/int	CObjAVT::GetANI_Sit()		{	return	AVT_ANI_SIT;				}
/*override*/int	CObjAVT::GetANI_Ride()		{   return  ( m_pObjCART )? m_pObjCART->GetRideAniPos():0; }
/*override*/int	CObjAVT::GetANI_PickITEM()	{	return  AVT_ANI_PICKITEM; }




//----------------------------------------------------------------------------------------------------
/// @param
/// @brief
//----------------------------------------------------------------------------------------------------
WORD CObjAVT::GetPetState()
{
	if( m_pObjCART )
		return m_pObjCART->Get_STATE();

	return 0;
}

//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : ���� ���ο� ������ �����ͷ� īƮ ����
//--------------------------------------------------------------------------------

bool CObjAVT::CreateCartFromMyData( bool bShowEffect )
{	

	//----------------------------------------------------------------------------------------------------
	/// Pet type ����
	/// ������ ��ü�ÿ��� Ÿ���� ���������� �����κ��� �������¿��� �˼�����.
	//----------------------------------------------------------------------------------------------------
	/*CItemSlot* pItemSlot = g_pAVATAR->GetItemSlot();
	CItem* pBodyItem = pItemSlot->GetItem( INVENTORY_RIDE_ITEM0 );*/

	if( m_sBodyIDX.m_nItemNo )
	{
		this->SetPetType( PAT_ITEM_PART_TYPE( m_sBodyIDX.m_nItemNo ) );
	}else
		return false; /// �ٵ������ ������ ��Ž


#ifdef _GBC
	bool bResult = CreateCart( m_iPetType, m_sEngineIDX.m_nItemNo, m_sBodyIDX.m_nItemNo,
		m_sLegIDX.m_nItemNo, m_sAbilIDX.m_nItemNo, m_sWeaponIDX.m_nItemNo );
#else
	bool bResult = CreateCart( m_iPetType, m_sEngineIDX.m_nItemNo, m_sBodyIDX.m_nItemNo,
		m_sLegIDX.m_nItemNo, m_sAbilIDX.m_nItemNo );
#endif
	
	
	/// ��� �������� �������� ���� 2005/7/30 - nAvy
	/// 
	///this->m_EndurancePack.ClearStateByDriveCart();

	//----------------------------------------------------------------------------------------------------
	/// īƮ�� Ż���� ����, ������ �Ⱥ��̰�
	//----------------------------------------------------------------------------------------------------		
	/*if( m_phPartVIS[ BODY_PART_KNAPSACK ] )
		::setVisibilityRecursive( *m_phPartVIS[ BODY_PART_KNAPSACK ], 0.0f );
	if( m_phPartVIS[ BODY_PART_WEAPON_R ] )
		::setVisibility( *m_phPartVIS[ BODY_PART_WEAPON_R ], 0.0f );
	if( m_phPartVIS[ BODY_PART_WEAPON_L ] )
		::setVisibility( *m_phPartVIS[ BODY_PART_WEAPON_L ], 0.0f );*/

	int iVisibilityPart[3] = { BODY_PART_KNAPSACK, BODY_PART_WEAPON_R, BODY_PART_WEAPON_L };
	for( int i = 0; i < 3; i++ )
	{
		CMODEL<CCharPART> *pCharPART = m_pCharMODEL->GetCharPART( iVisibilityPart[ i ] );
		if ( pCharPART  ) 
		{
			short nI;			

			for (nI=0; nI<pCharPART->m_nPartCNT; nI++) 
			{
				if ( m_phPartVIS[ iVisibilityPart[ i ] ][ nI ] ) 
				{
					::setVisibilityRecursive( m_phPartVIS[ iVisibilityPart[ i ] ][ nI ], 0.0f );
				}
			}
		}
	}
	

	//----------------------------------------------------------------------------------------------------
	/// Ż���� ȿ�� ���
	//----------------------------------------------------------------------------------------------------		
	if( bShowEffect )
	{
		int iEffectNO	= PAT_RIDE_EFFECT( m_sBodyIDX.m_nItemNo );
		int iSoundNO	= PAT_RIDE_SOUND( m_sBodyIDX.m_nItemNo );
	
		this->ShowEffectOnCharByIndex( iEffectNO, iSoundNO );				
	}
	
	return bResult;
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : ī�� ����. ���ڷ� �� ��Ʈ ������ �޴´�.
//--------------------------------------------------------------------------------
bool CObjAVT::CreateCart( unsigned int iPetType, int iEnginePart, int iBodyPart, int iLegPart, int iAbilIPart,int iWeaponPart )
{
	if( iBodyPart == 0 || iEnginePart == 0 || iLegPart == 0 )
	{
		return false;
	}

	m_ObjVibration.EndVibration();

	m_iPetType = iPetType;

	if( m_pObjCART != NULL )
	{
		delete m_pObjCART;
		m_pObjCART = NULL;
	}

	m_pObjCART = g_pObjMGR->Add_CartCHAR( iPetType, this, 0 );
	if( m_pObjCART == NULL )
	{
		assert( 0 && "Create cart failed" );
		///g_itMGR.OpenMsgBox( "Failed to create cart" );
		return false;
	}

	SetPetParts( RIDE_PART_BODY,	iEnginePart, false );
	SetPetParts( RIDE_PART_ENGINE,	iBodyPart, false );
	SetPetParts( RIDE_PART_LEG,		iLegPart, false );

	///<- 2005/7/25 īƮ �ý��� ���� �߰��� ���� : nAvy
	//SetPetParts( RIDE_PART_ARMS,	iArmsIDX, false );
#ifdef _GBC	
	SetPetParts( RIDE_PART_ABIL,	iAbilIPart, false );
	SetPetParts( RIDE_PART_ARMS,	iWeaponPart, false );
#else
	SetPetParts( RIDE_PART_ARMS,	iAbilIPart, false );
#endif
	
	///->

	if( m_pObjCART->Create( this, m_iPetType, this->Get_CurPOS() ) == false )
	{
		g_pObjMGR->Del_Object( m_pObjCART );
		return false;
	}

	D3DXVECTOR3 pos( 0.0f, 0.0f, 0.0f );
	ResetCUR_POS( pos );
	::savePrevPosition( m_hNodeMODEL );

	this->Set_ModelDIR( 0.0f );

	SetCMD_STOP();

	this->m_btMoveMODE = MOVE_MODE_DRIVE;

	return true;
}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : īƮ ����
//--------------------------------------------------------------------------------

void CObjAVT::DeleteCart( bool bShowEffect )
{
	if( m_pObjCART )
	{
		float fDir = ::getModelDirection( m_pObjCART->GetZMODEL() );
		this->Set_ModelDIR( fDir );

//����ȣ
#if defined(_GBC)
		//������ (2�ν� ���̶��)
		if(GetRideUserIndex())
		{	
			//20050901 ȫ�� 2�ν� īƮ ������ ž���ڰ� ������ ��� State�� Normal�� �ٲ��ش�.
			CObjAVT *oSrc = g_pObjMGR->Get_ClientCharAVT( g_pObjMGR->Get_ServerObjectIndex(GetRideUserIndex()), true ); 
			if(oSrc)
			{				
				if( !strcmp( oSrc->Get_NAME() , g_pAVATAR->Get_NAME()) )
				{
					g_pAVATAR->Set_Block_CartRide( false );
				}							
			}		

			m_pObjCART->UnLinkChild();		
			
			//���� ������Ʈ �Ŵ��� ����Ʈ���� īƮ ��ü�� ������.
			if(m_pObjCART)
			{	g_pObjMGR->Del_Object( m_pObjCART );	m_pObjCART = NULL;	}
			
			SetCMD_STOP();			
			
			goto CHAR_VISIBLE; 
		}
       //2�ν� ž���� 
		else if(IsRideUser())
		{
			if(m_pObjCART)
			{	
				ReleaseRideUser(m_pObjCART->GetParent()->GetRideUserIndex());	

				//20050901 ȫ�� 2�ν� īƮ ������ ž���ڰ� ������ ��� State�� Normal�� �ٲ��ش�.
				if( !strcmp( this->Get_NAME(), g_pAVATAR->Get_NAME()) )
				{
					g_pAVATAR->Set_Block_CartRide( false );
				}				

				return;
			}
		}
		//ȥ�� Ÿ�� �ִٸ�...
		else
		{	m_pObjCART->UnLinkChild();		}
#else	
		m_pObjCART->UnLinkChild();	
#endif 

		if(m_pObjCART)
		{
			g_pObjMGR->Del_Object( m_pObjCART );
			m_pObjCART = NULL;				
		}

		SetCMD_STOP();

		this->m_btMoveMODE = this->m_bRunMODE;


		//----------------------------------------------------------------------------------------------------
		/// īƮ�� Ż���� ����, ������ �Ⱥ��̰�
		//----------------------------------------------------------------------------------------------------		
		/*if( m_phPartVIS[ BODY_PART_KNAPSACK ] )
			::setVisibility( *m_phPartVIS[ BODY_PART_KNAPSACK ], 1.0f );
		if( m_phPartVIS[ BODY_PART_WEAPON_R ] )
			::setVisibility( *m_phPartVIS[ BODY_PART_WEAPON_R ], 1.0f );
		if( m_phPartVIS[ BODY_PART_WEAPON_L ] )
			::setVisibility( *m_phPartVIS[ BODY_PART_WEAPON_L ], 1.0f );*/
CHAR_VISIBLE:
		int iVisibilityPart[3] = { BODY_PART_KNAPSACK, BODY_PART_WEAPON_R, BODY_PART_WEAPON_L };
		for( int i = 0; i < 3; i++ )
		{
			CMODEL<CCharPART> *pCharPART = m_pCharMODEL->GetCharPART( iVisibilityPart[ i ] );
			if ( pCharPART  ) 
			{
				short nI;			

				for (nI=0; nI<pCharPART->m_nPartCNT; nI++) 
				{
					if ( m_phPartVIS[ iVisibilityPart[ i ] ][ nI ] ) 
					{
						::setVisibilityRecursive( m_phPartVIS[ iVisibilityPart[ i ] ][ nI ], 1.0f );
					}
				}
			}
		}

		//----------------------------------------------------------------------------------------------------
		/// �������� ȿ�����..
		//----------------------------------------------------------------------------------------------------	
		if( bShowEffect )
		{
			int iEffectNO	= PAT_GETOFF_EFFECT( m_sBodyIDX.m_nItemNo );
			int iSoundNO	= PAT_GETOFF_SOUND( m_sBodyIDX.m_nItemNo );
		
			this->ShowEffectOnCharByIndex( iEffectNO, iSoundNO );				
		}
	}	
}


///--------------------------------------------------------------------------------
/// class : CObjCHAR
/// ����ȣ: Process_JOIN_RIDEUSER() 
///       : �����ڰ� �� ������ 2�ν� ž���ڸ� �����Ѵ�.  
///				
//--------------------------------------------------------------------------------
void CObjAVT::Process_JOIN_RIDEUSER(void)
{

	if(this->GetPetMode() < 0)
		return;
	
	DeleteCart(true);

}


//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : īƮ ��Ʈ��������
//--------------------------------------------------------------------------------

void CObjAVT::SetPetParts( unsigned int iPetPartIDX, unsigned int iItemIDX, bool bJustInfo )
{
	if( iPetPartIDX >= MAX_RIDING_PART )
		return;

	m_sPetPartItemIDX[ iPetPartIDX ].m_nItemNo = iItemIDX;

	if( bJustInfo == false )
	{
		if( m_pObjCART == NULL )
			return;

		m_pObjCART->SetPetParts( iPetPartIDX, iItemIDX );
	}
}

//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : 
/// @brief  : īƮ ��Ʈ ����� �߻��� īƮ ������Ʈ
//--------------------------------------------------------------------------------

void CObjAVT::UpdatePet()
{
	if( m_pObjCART )
	{
		DeleteCart();
		CreateCartFromMyData();	
	}
}

//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @param  : ���ο� ���� ������ ���Ժ���
//--------------------------------------------------------------------------------

void CObjAVT::SetWeightRate( BYTE btWeightRate )
{
	m_btWeightRate = btWeightRate;
}

//--------------------------------------------------------------------------------
/// class : CObjAVT
/// @return : ���� ���� ������ ���� 
//--------------------------------------------------------------------------------

BYTE CObjAVT::GetWeightRate()
{
	return m_btWeightRate;
}

//-------------------------------------------------------------------------------------------
/// @brief ��� ���λ����� ���� �ݴ� ������..
/// ���λ��� ���� ���� �����ֱ����� �Ϸ��� �۾����� �Ѵ�.
//-------------------------------------------------------------------------------------------
void CObjAVT::SetPersonalStoreTitle( char* strTitle, int iPersonalStoreType )
{ 
	if( strTitle == NULL )
	{
		if( m_phPartVIS[ 0 ] == NULL )
			m_pCharMODEL->ClearRenderUnitParts();

		// loadVisible�Ȱ͵� ����.
		for (short nP=0; nP<MAX_BODY_PART; nP++) 
		{
			if( m_phPartVIS[ nP ] == NULL )
				CreateSpecificPART( (char*)m_Name.c_str(), nP );			
		}

		this->CreateGemmingEffect();
		this->CreateGradeEffect();

		m_bPersonalStoreMode = false;

		if( m_pObjPersonalStore )
		{			
			m_pObjPersonalStore->UnlinkVisibleWorld();
			///m_pObjPersonalStore->RemoveFromScene();
			delete m_pObjPersonalStore;
			m_pObjPersonalStore = NULL;			
		}
		return;
	}

	m_pObjPersonalStore = new CObjTREE();
		
	///D3DXVECTOR3 vPos = this->Get_CurPOS();	
	D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f );
	const int iPersonalStoreNO = 260;
	if( m_pObjPersonalStore->Create( g_DATA.m_ModelFieldITEM.GetMODEL( iPersonalStoreNO + iPersonalStoreType ), vPos ) )
	{
		//-------------------------------------------------------------------------------------------
		/// ��� ĳ���� ��Ʈ�� ����
		//-------------------------------------------------------------------------------------------
		::clearRenderUnit( this->m_hNodeMODEL );
		for (short nP=0; nP<MAX_BODY_PART; nP++) 
		{
			DeleteSpecificPART( nP, m_phPartVIS[ nP ] );
			//m_pCharMODEL->DeletePART( nP, m_phPartVIS[ nP ] );
			m_phPartVIS[ nP ] = NULL;
		}	

		m_pObjPersonalStore->LinkToModel( this->GetZMODEL() );		
	}else
	{
		delete m_pObjPersonalStore;
		m_pObjPersonalStore = NULL;
	}
	
	m_bPersonalStoreMode = true;
	m_strPersonalStoreTitle = std::string( strTitle ); 
}

const char*	CObjAVT::GetPersonalStoreTitle()
{ 
	return m_strPersonalStoreTitle.c_str(); 
}

bool CObjAVT::IsPersonalStoreMode()
{ 
	return m_bPersonalStoreMode; 
}

/// ��� ��ü�� ��� �ɷ��ִ� �Ӽ� ����..
void CObjAVT::ClearRWeaponSkillEffect()
{
	this->m_EndurancePack.ClearRWeaponSkillEffect();
}

void CObjAVT::ClearLWeaponSkillEffect()
{
	this->m_EndurancePack.ClearLWeaponSkillEffect();
}

//----------------------------------------------------------------------------------------------------	
/// @brief �ָ�� �����϶��� ���� ��ǥ�� �����ϰ�, �Ϲ����϶��� �� ��ǥ�� ������
//----------------------------------------------------------------------------------------------------
const D3DVECTOR& CObjAVT::GetWorldPos()
{
	if( GetPetMode() > 0 )
	{
		if( m_pObjCART )
		{
			return m_pObjCART->Get_CurPOS();
		}
	}

	return Get_CurPOS();
}


//----------------------------------------------------------------------------------------------------	
/// @brief �𵨿� ��ϵ� renderUnit���ǰ͵鿡 ���� �浹����.. (�ƹ�Ÿ�ϰ�츸)
//----------------------------------------------------------------------------------------------------
bool CObjAVT::IsIntersectAccessory( float &fCurDistance )
{
	/// ���λ��� ��ǥ��
	if( this->m_pObjPersonalStore )
	{
		return m_pObjPersonalStore->IsIntersectForCamera( fCurDistance );
	}
	return false;
}
//----------------------------------------------------------------------------------------------------	
/// @brief ��Ƽ����� �ڵ� HPȸ���� ���� Stamina ���� Method
///		   CObjUSER�� ��� CUserDATA::GetCur_STAMINA�� ȣ���Ѵ�. �����Ұ�
//----------------------------------------------------------------------------------------------------
short CObjAVT::GetStamina()
{
	return m_nStamina;
}
void CObjAVT::SetStamina( short nStamina )
{
	m_nStamina = nStamina;
}

//----------------------------------------------------------------------------------------------------
/// @brief m_dwSubFLAG �÷��׿� ���� Ư���� ���·��� ��ȯ
//----------------------------------------------------------------------------------------------------

void CObjAVT::ChangeSpecialState( DWORD dwSubFLAG )
{
	m_dwSubFLAG = dwSubFLAG;

	if( m_dwSubFLAG & FLAG_SUB_HIDE )
	{
		::setVisibilityRecursive( this->GetZMODEL(), 0.0f ); 
	}
	//��ȫ��::����
	else if( !(m_dwSubFLAG & FLAG_SUB_HIDE) )
	{
		::setVisibilityRecursive( this->GetZMODEL(), 1.0f ); 
	}
}

//----------------------------------------------------------------------------------------------------
/// @brief Ŭ���Ҽ� �ִ� ��ü�ΰ�?
//----------------------------------------------------------------------------------------------------
bool CObjAVT::CanClickable()
{
	if( m_dwSubFLAG & FLAG_SUB_HIDE )
		return false;

	if( ::getVisibility( this->GetZMODEL() ) < 0.1 )
		return false;

	return true;
}

//----------------------------------------------------------------------------------------------------
/// @brief virtual From CObjAI   : �߰�  2005/7/13 - nAvy
//----------------------------------------------------------------------------------------------------
int CObjAVT::GetOri_MaxHP()
{
	return m_iMaxHP;
}


///////////////////////////////////////////////////////////////////////////////////////////	
// 2005. 6. 20	�� ��ȣ 
//
// ���ż�ȯ ���� Ŭ���� 
///////////////////////////////////////////////////////////////////////////////////////////	


CGoddessMgr goddessMgr;
//������ 
CGoddessMgr::CGoddessMgr()
{
	
	Init();

}


//�Ҹ���
CGoddessMgr::~CGoddessMgr()
{

//	Release();
	
}


//�ʱ�ȭ 
BOOL CGoddessMgr::Init(void)
{

	m_IsUse		= FALSE;
	m_IsAvata	= FALSE;
	
	m_dwAvata	= 0;
	m_Count		= 0;


	return TRUE;

}


//�޸� ���� 
void CGoddessMgr::Release(void)
{

//����Ʈ ���� 
//-----------------------------------------------------------------------------------
	GODDESSSTR* pGds = NULL;
	for(GODLIST itr = m_list.begin(); itr != m_list.end();itr++)
	{
		pGds = &(itr->second);
		 if(pGds == NULL)
			continue;

		 Release_List(pGds);
	}

	m_list.clear();	
	m_Count = 0;
//------------------------------------------------------------------------------------
 	
	Init();
}


//�ϳ��� ����Ʈ�� �����Ѵ�. 
void CGoddessMgr::Release_List(GODDESSSTR* pGDS)
{

	delete pGDS->pGODModel;
	delete pGDS->pSprModel;
	
	for(register int i=0;i<2;i++)
	{
		delete  pGDS->pEffect[i];
		pGDS->pEffect[i]	= NULL;
		pGDS->hParticle[i]  = NULL;
	}

	pGDS->pGODModel		= NULL;
	pGDS->pSprModel		= NULL;
	
	pGDS->hGoddess		= NULL;
	pGDS->hSprite		= NULL;

}


//��뱹�� üũ 
BOOL CGoddessMgr::Permission_Country(void)
{

	//����  üũ �� ����� �����Ѵ�. 
	if( CCountry::GetSingleton().IsApplyNewVersion())
		return TRUE;


	return FALSE;

}


//����,���� ��ü�� �ε��Ѵ�. 
BOOL CGoddessMgr::Register_God_And_Sprite(void)
{
	
	if(m_IsUse)
		return TRUE;

	return (m_IsUse = TRUE);

}


//������ ������Ʈ ��ü�� ����Ѵ�. 
BOOL CGoddessMgr::Register_RenderObj(GODDESSSTR& gds)
{

	//�ƹ�Ÿ ��ü�� �����´�. 
	CObjAVT *pCHAR = g_pObjMGR->Get_ClientCharAVT(gds.idxMaster,TRUE);
		if(pCHAR == NULL)
			return FALSE;

	//�ƹ�Ÿ ���� ���� 
	pCHAR->SetAroaState() = gds.bEnable;

	//
	pCHAR->Calc_AruaAddAbility();

	//�ڽ��̶�� �ƹ�Ÿ ������ �����Ѵ�. 
	m_IsAvata = (lstrcmp(pCHAR->Get_NAME(),g_pAVATAR->Get_NAME()) ? FALSE : TRUE);
		
		
	char* ParticlName[] = 
	{ "3Ddata\\Effect\\_arua_ghost01.eft", "3Ddata\\Effect\\end_effect_02.eft" };

	for(register int i=0;i<2;i++)
	{
		if(gds.pEffect[i]!= NULL)
		{ delete gds.pEffect[i]; gds.pEffect[i] = NULL; }

		DWORD hKey = CStr::GetHASH(ParticlName[i]);
		
		gds.pEffect[i] = g_pEffectLIST->Add_EFFECT(hKey);
		if(	gds.pEffect[i] == NULL)
		{
			_ASSERT(gds.pEffect[i]);
			return FALSE;
		}
		
		gds.pEffect[i]->LinkNODE(pCHAR->GetZMODEL());
		gds.hParticle[i] = gds.pEffect[i]->GetZNODE();
	}

	
	//���� �ε� 
	if(gds.pGODModel == NULL)
	{
		gds.pGODModel = new CObjMOB;
		if (gds.pGODModel == NULL)
		{
			_ASSERT(gds.pGODModel);
			return FALSE;
		}

		D3DXVECTOR3 cpos = pCHAR->Get_CurPOS();
		if (!gds.pGODModel->Create(939,cpos,0,TRUE)) 
		{
			LogString (LOG_DEBUG, "Create Goddess Model Data\n");
			delete gds.pGODModel; gds.pGODModel = NULL;
		}
		
		gds.hGoddess = gds.pGODModel->GetZMODEL();
		::setRepeatCount(gds.hGoddess,1);
		::setAnimatableFrame(gds.hGoddess,0);
		::controlAnimatable(gds.hGoddess,0);
	}


	//���� �ε� 
	if(gds.pSprModel == NULL)
	{
		gds.pSprModel = new CObjMOB;
		if (gds.pSprModel == NULL)
		{
			_ASSERT(gds.pSprModel);
			return FALSE;
		}

		D3DXVECTOR3 cpos = pCHAR->Get_CurPOS();
	
		if (!gds.pSprModel->Create(940,cpos,0,FALSE)) 
		{
			LogString (LOG_DEBUG, "Create Sprite Model Data\n");
			delete gds.pSprModel; gds.pSprModel = NULL;
		}
	
		gds.hSprite = gds.pSprModel->GetZMODEL();
	}
   
	::controlAnimatable(gds.hGoddess,0);
	::setVisibilityRecursive(gds.hGoddess,0.0f);
	::setVisibilityRecursive(gds.hSprite,0.0f);

	return TRUE;

}


void CGoddessMgr::Set_GDSData(GODDESSSTR& gds,BOOL bonOff,BOOL bPrograss)
{

	if(bPrograss)
	{
		if(bonOff)
			gds.god_State = GOD_SPRITE_EFFECT;	
		else
			gds.god_State = GOD_NONE;
	}
	else
	{
		if(bonOff)
			gds.god_State = GOD_APPEAR_PARTCLE;	
		else
			gds.god_State = GOD_END_EFFECT;
	}

}


//���� ��ȯ on/off �� ó���Ѵ�. 
BOOL CGoddessMgr::SetProcess(DWORD bonOff,WORD idx,BOOL bPrograss)
{
	
  //���� �ڵ�  üũ 
	if(!Permission_Country())
		return TRUE;


	GODDESSSTR Gds,*pGds = NULL;

	BOOL bOnOff = bonOff ? 1 : 0;
	

//Old list
//-------------------------------------------------------------------
	GODLIST itr = m_list.find(idx);
	
	if(itr != m_list.end())
	{
		itr->second.bEnable		= bOnOff;
		itr->second.sTick		= GetTickCount();
				
		Set_GDSData(itr->second,bOnOff,bPrograss);
		
		if(!Register_RenderObj(itr->second))
			return FALSE;

		pGds = &(itr->second);

		goto SET_EFF;
	}  
//--------------------------------------------------------------------
	

//New list
//--------------------------------------------------------------------
	//ó���� OFF ��� ����Ʈ�� ������ �ʿ䰡 ����.
	if(bOnOff == FALSE)
		return TRUE;
		

	Gds.Init();

	Gds.bEnable		= bOnOff;
	Gds.sTick		= GetTickCount();
	Gds.idxMaster	= idx;
		
	Set_GDSData(Gds,bOnOff,bPrograss);

	if(!Register_RenderObj(Gds))
		return FALSE;
	
	m_list[idx] = Gds;
	m_Count = m_list.size();

	pGds = &Gds;
//--------------------------------------------------------------------

//����Ʈ ������ ���� 
SET_EFF:

	if(m_IsAvata)
		m_dwAvata = bonOff;


	//���� ����� ���̰� �����. 
	if(pGds->god_State == GOD_SPRITE_EFFECT)
	{
		pGds->fviewSpr = 1.0f;
		::setVisibilityRecursive(pGds->hSprite,1.0f);
	}


	//��ƼŬ �ִϸ��̼Ǹ� ���� ���� 
	if(pGds->god_State == GOD_APPEAR_PARTCLE)
	{
		pGds->pEffect[0]->StartEffect();	
		pGds->fviewGODD = 0.0f;
	}
	else
		pGds->pEffect[0]->StopEffect();	
	
	if(pGds->god_State == GOD_END_EFFECT)
	{	
		pGds->pEffect[1]->StartEffect();	
		pGds->fviewSpr = 1.0f;
	}
	else
		pGds->pEffect[1]->StopEffect();	


	return TRUE;

}


void CGoddessMgr::Update(void)
{
	
	//���� �ڵ�  üũ 
	if(!Permission_Country())
		return;


#define TIME_GOD_ACCEPT				9100
#define TIME_APPEAR_GODDESS			3000


	if(m_Count == 0)
		return;


	CObjAVT *pCHAR = NULL;
	DWORD curTick=0,tempTick=0; 
	D3DXVECTOR3 tPos = D3DXVECTOR3(0,0,0);
	
	BOOL IsBeginList = FALSE;
	GODLIST preNode; 
	
	float fRot[4] = { 0 };
	int t1=0,t2=0;



	for(GODLIST itr=m_list.begin();itr!=m_list.end();++itr)
	{

		GODDESSSTR* gds = &(itr->second);
			if(gds == NULL)
				continue;
		

	//	if(gds->god_State == GOD_NONE) 
	//		continue;


		//�ƹ�Ÿ ��ü�� �����´�. 
		pCHAR = g_pObjMGR->Get_ClientCharAVT(gds->idxMaster,TRUE);
			if((pCHAR == NULL) || (gds->god_State == GOD_NONE))
			{
				Release_List(&(itr->second));
				if(itr != m_list.begin())
				{
					preNode = --itr;	
					++itr;
					IsBeginList = FALSE;
				}
				else
					IsBeginList = TRUE;


				m_list.erase(itr);

				if(!(m_Count = m_list.size()))
					break;
				
				//����Ʈ�� ������ �ƴ϶�� Begin ����Ʈ �̴�
				if(IsBeginList)
					itr = m_list.begin();
				else
					itr = preNode;
	
				continue;
			}


		//�ƹ�Ÿ�� ��ǥ
		D3DXVECTOR3 cPos = tPos = pCHAR->Get_CurPOS();
			
		//�ƹ�Ÿ ȸ��
		::getRotationQuad(pCHAR->GetZMODEL(),fRot);
	

		//step1 :  ��ƼŬ ����Ʈ ��� 
		if(gds->god_State == GOD_APPEAR_PARTCLE)
		{
			curTick  = GetTickCount();
			tempTick = (curTick - gds->sTick);

			//���� ĳ���� ��� ���� 
			if(tempTick >= TIME_APPEAR_GODDESS)
			{
				if(gds->fviewGODD == 0)
				{
					::setRepeatCount(gds->hGoddess,1);
					::setAnimatableFrame(gds->hGoddess,0);
					::controlAnimatable(gds->hGoddess,1);
				}

				//���� ĳ���͸� ��Ÿ���� �Ѵ�. 
				if(ProcessVisible(gds->fviewGODD,0.0008f) == 1)
					gds->god_State = GOD_APPEAR_GODDESS;
				
				::setRotationQuat(gds->hGoddess,fRot);
				::setPosition(gds->hGoddess,tPos.x,tPos.y,tPos.z);	
				::setVisibilityRecursive(gds->hGoddess,gds->fviewGODD);
			}
		}

	
		//step2 : ��ƼŬ & ���� ����Ʈ ��� 
		if(gds->god_State == GOD_APPEAR_GODDESS)
		{
			curTick  = GetTickCount();
			tempTick = (curTick - gds->sTick);
		

			//������ ��������Ʈ�� ������ ���� Ʈ��ŷ���� ��ȯ�Ѵ�. 
			if(tempTick >= TIME_GOD_ACCEPT)
			{	
				//���� ĳ���͸� ��Ÿ���� �Ѵ�. 
				t1 = ProcessVisible(gds->fviewGODD,-0.0009f);
				t2 = ProcessVisible(gds->fviewSpr,0.001f);
			
				if((t1 == 0) && (t2 ==    1))
				{	
					gds->god_State = GOD_SPRITE_EFFECT;		
					::controlAnimatable(gds->hGoddess,0);
				}
				
				//���� 
				::setRotationQuat(gds->hSprite,fRot);
				::setPosition(gds->hSprite,tPos.x,tPos.y,tPos.z);	
				::setVisibilityRecursive(gds->hSprite,gds->fviewSpr);
			}
			            			
			//���� 
			::setRotationQuat(gds->hGoddess,fRot);
			::setPosition(gds->hGoddess,tPos.x,tPos.y,tPos.z);	
			::setVisibilityRecursive(gds->hGoddess,gds->fviewGODD);
		}

		//step3 : ���� Ʈ��ŷ ��� 
		if(gds->god_State == GOD_SPRITE_EFFECT)
		{
			::setRotationQuat(gds->hSprite,fRot);
			::setPosition(gds->hSprite,tPos.x,tPos.y,tPos.z);	
					
		}

		//step4 ; ������ ������� �Ѵ�. 
		if(gds->god_State == GOD_END_EFFECT)
		{
	
			if(ProcessVisible(gds->fviewSpr,-0.001f) == 0)
				gds->god_State = GOD_NONE;	

			::setRotationQuat(gds->hSprite,fRot);
			::setPosition(gds->hSprite,tPos.x,tPos.y,tPos.z);	
			::setVisibilityRecursive(gds->hSprite,gds->fviewSpr);
		}
		
	}

}


//Visible ó���� �Ѵ�. 
int CGoddessMgr::ProcessVisible(float& fv,float fseq)
{
	
	float fdelta =  g_GameDATA.GetElapsedFrameTime();
	
	fv+=(fseq * fdelta);

	if(fv > 1.0f)
	{	fv = 1.0f; return 1; }

	if(fv < 0.0f)
	{	fv = 0.0f; return 0; }


	return -1;

}
