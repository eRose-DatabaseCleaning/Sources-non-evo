/*
	$Header: /Client/CObjUSER.cpp 176   05-09-14 11:44a Raning $
*/
#include "stdAFX.h"
#include "Game.h"
#include "CObjUSER.h"
#include "Game_FUNC.h"
#include "Network/CNetwork.h"
#include "interface/it_mgr.h"
#include "interface/dlgs/cpartydlg.h"
#include "gamedata/cbank.h"
#include "gamedata/cparty.h"
#include "system/cgame.h"
#include "gamecommon/item.h"
#include "common/io_pat.h"
#include "IO_Terrain.h"
#include "Country.h"
#include "CommandFilter.h"

//����ȣ
#include "./GameProc/UseItemDelay.h"


CObjUSER* CObjUSER::m_pInstance=NULL;

//-------------------------------------------------------------------------------------------------
CObjUSER::CObjUSER ()
{
	m_Inventory.Clear ();
	::ZeroMemory ( m_iAddValue,	sizeof(int) * AT_MAX );

	this->m_iTeamInfo = 2;
	m_nCurrWeight		= 0;
	m_iServerSavedMemoCount	= 0;

	m_iSummonMobCapacity	= 0;
	m_bWaitUpdateInventory	= false;
	m_dwLastBattleTime		= 0 ;

	m_AruaAddHp				= 0;
	m_AruaAddMp				= 0;
	m_AruaAddCritical		= 0;
	m_AruaAddAttackPower	= 0;
	m_AruaAddDefence		= 0;
	m_PatGuageMax			= 0;

}

CObjUSER::~CObjUSER ()
{
}

CObjUSER* CObjUSER::Instance ()
{
	if ( !m_pInstance  )
		m_pInstance = new CObjUSER;

	return m_pInstance; 
}
void CObjUSER::Destroy ()
{
	SAFE_DELETE( m_pInstance );
}

//-------------------------------------------------------------------------------------------------
void CObjUSER::Init_DefaultValue (void)
{
	::ZeroMemory ( &m_BasicINFO,	sizeof( tagBasicINFO	) );
	::ZeroMemory ( &m_BasicAbility,	sizeof( tagBasicAbility	) );
	::ZeroMemory ( &m_GrowAbility,	sizeof( tagGrowAbility	) );
	::ZeroMemory ( &m_Skills,		sizeof( tagSkillAbility	) );
	::ZeroMemory ( &m_Quests,		sizeof( tagQuestData	) );
	::ZeroMemory ( &m_Battle,		sizeof( tagBattleAbility) );
	m_Inventory.Clear ();

	Set_STR( 100 );		// �ٷ�
	Set_DEX( 100 );		// ��ø
	Set_INT( 10 );		// ����
	Set_CON( 10 );		// ����
	Set_CHARM( 10 );	// �ŷ�
	Set_SENSE( 10 );	// ����

	tagITEM sITEM;
	::ZeroMemory( &sITEM, sizeof(tagITEM) );

	sITEM.m_cType   = ITEM_TYPE_ARMOR;			// ����
	sITEM.m_nItemNo = 72;	
	Set_EquipITEM ( EQUIP_IDX_ARMOR,		sITEM );

	sITEM.m_cType   = ITEM_TYPE_KNAPSACK;		// �����
	sITEM.m_nItemNo = 0;	
	Set_EquipITEM ( EQUIP_IDX_KNAPSACK,		sITEM );	

	sITEM.m_cType   = ITEM_TYPE_GAUNTLET;		// �尩
	sITEM.m_nItemNo = 71;	
	Set_EquipITEM ( EQUIP_IDX_GAUNTLET,		sITEM );

	sITEM.m_cType   = ITEM_TYPE_BOOTS;			// �Ź�
	sITEM.m_nItemNo = 44;	
	Set_EquipITEM ( EQUIP_IDX_BOOTS,		sITEM );

//*
	sITEM.m_cType   = ITEM_TYPE_WEAPON;			// ������
	sITEM.m_nItemNo = 36;//231;//202;//26;
	Set_EquipITEM ( EQUIP_IDX_WEAPON_R,		sITEM );
//	117 ;	// �����
//	209 ;	// ���

	sITEM.m_cType   = ITEM_TYPE_SUBWPN;
	sITEM.m_nItemNo = 0;	// �޼�
	Set_EquipITEM ( EQUIP_IDX_WEAPON_L,		sITEM );
//*/

	this->SetPartITEM( BODY_PART_FACE,		1 );	// ��
	this->SetPartITEM( BODY_PART_HAIR,		0 );	// �⺻ �Ӹ���
	this->SetPartITEM( BODY_PART_HELMET,	51 );
	// TEST .......

	Set_MONEY( 900000 );

	short nA, nI, nJ;

	short	nDefWpnITEM[] = {
		5,	32,	105, 161, 232, 303, 0
	} ;
	for (nI=0; nDefWpnITEM[ nI ]; nI++) {
		sITEM.m_cType     = ITEM_TYPE_WEAPON;
		sITEM.m_nItemNo   = nDefWpnITEM[ nI ];
		Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI, sITEM );
	}

	short	nDefSubITEM[] = {
		2, 12, 13,	0
	} ;
	for (nJ=0; nDefSubITEM[ nJ ]; nJ++) {
		sITEM.m_cType   = ITEM_TYPE_SUBWPN;
		sITEM.m_nItemNo	= nDefSubITEM[ nJ ];
		Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ, sITEM );
	}

	short	nDefArmorITEM[] = {
		7,	31,	35, 0
	} ;
	for (nA=0; nDefArmorITEM[ nA ]; nA++) {
		sITEM.m_cType     = ITEM_TYPE_ARMOR;
		sITEM.m_nItemNo   = nDefArmorITEM[ nA ];
		Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ+nA, sITEM );
	}

	// ����
	sITEM.m_cType     = BODY_PART_HELMET;
	sITEM.m_nItemNo   = 55;
	Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ+nA, sITEM );

	sITEM.m_cType     = BODY_PART_HELMET;
	sITEM.m_nItemNo   = 35;
	Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ+nA + 1, sITEM );

	sITEM.m_cType     = BODY_PART_HELMET;
	sITEM.m_nItemNo   = 8;
	Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ+nA + 2, sITEM );

	//����
	sITEM.m_cType     = EQUIP_IDX_BOOTS;
	sITEM.m_nItemNo   = 44;
	Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ+nA+3, sITEM );	

	/*
	//�尩
	sITEM.m_cType     = ITEM_TYPE_GAUNTLET;
	sITEM.m_nItemNo   = 7;
	Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ+nA+4, sITEM );

	sITEM.m_cType     = ITEM_TYPE_GAUNTLET;
	sITEM.m_nItemNo   = 6;
	Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ+nA+5, sITEM );

	//����
	sITEM.m_cType     = ITEM_TYPE_KNAPSACK;
	sITEM.m_nItemNo   = 2;
	Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ+nA+6, sITEM );

	sITEM.m_cType     = ITEM_TYPE_KNAPSACK;
	sITEM.m_nItemNo   = 4;
	Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ+nA+7, sITEM );

	//��
	sITEM.m_cType     = ITEM_TYPE_FACE_ITEM;
	sITEM.m_nItemNo   = 5;
	Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ+nA+8, sITEM );

	sITEM.m_cType     = ITEM_TYPE_FACE_ITEM;
	sITEM.m_nItemNo   = 8;
	Add_ITEM( MAX_EQUIP_IDX+INV_WEAPON*INVENTORY_PAGE_SIZE + nI+nJ+nA+9, sITEM );
	*/


	struct tagDefITEM {
		short nItemNo;
		short nQuantity;
	} ;

	tagDefITEM sDefUseITEM[ ] = {
		{	3,	2	},	{	6,	2	},	{	9,	2	},
		{	13,	2	},	{	6,	2	},	{	19,	2	},

		{ 0, 0  }
	} ;
	
	// �Ҹ�
	for (nI=0; sDefUseITEM[ nI ].nItemNo; nI++) {
		sITEM.m_cType     = ITEM_TYPE_USE;
		sITEM.m_nItemNo   = sDefUseITEM[ nI ].nItemNo;
		sITEM.m_uiQuantity = sDefUseITEM[ nI ].nQuantity;
		Add_ITEM( MAX_EQUIP_IDX+INV_USE*INVENTORY_PAGE_SIZE + nI, sITEM );
	}

	// ��Ÿ
	tagDefITEM sDefEtcITEM[ ] = {
		/*
		{	1, 100	},	{ 3, 100 },	{ 6, 50 },	{ 8, 50 },	{ 23, 20 },
		{	31, 20  },	{ 54, 20 }, 
		*/
		{	1,	2	},	{	21,	2	},	{	31,	2	},	{	41,	2	},
		{	51,	2	},	{	61,	2	},	{	71,	2	},	{	122,	2	},

		{ 0, 0  }
	} ;
	for (nI=0; sDefEtcITEM[ nI ].nItemNo; nI++) {
		sITEM.m_cType     = ITEM_TYPE_NATURAL;
		sITEM.m_nItemNo   = sDefEtcITEM[ nI ].nItemNo;
		sITEM.m_uiQuantity = sDefEtcITEM[ nI ].nQuantity;
		Add_ITEM( MAX_EQUIP_IDX+INV_ETC*INVENTORY_PAGE_SIZE + nI, sITEM );
	}

	this->m_GrowAbility.m_nLevel = 1;
	this->LevelUP ();

	/// ���� �׼�
	AddNewSkill( 1, 0, 1 );
	AddNewSkill( 2, 1, 1 );
	AddNewSkill( 3, 2, 1 );

	/// ����- �̸��
	AddNewSkill( 11, 10, 1 );
	AddNewSkill( 12, 11, 1 );
	AddNewSkill( 13, 12, 1 );

	/// ���� - ����
	AddNewSkill( 21, MAX_LEARNED_SKILL_CNT/4 + 0, 1 );
	AddNewSkill( 22, MAX_LEARNED_SKILL_CNT/4 + 1, 1 );
	
	/// ���� - ���Ÿ� �߻�
	AddNewSkill( 31, MAX_LEARNED_SKILL_CNT/4 + 2, 1 );

	/// ���� - ����
	AddNewSkill( 41, MAX_LEARNED_SKILL_CNT/4 + 3, 1 );
}

/// ���ο� ��ų �߰�.
bool CObjUSER::AddNewSkill( int iSkillIDX, int iSkillSlotNO, int iSkillLevel ,bool bSubPOINT )
{
	if( iSkillSlotNO >= MAX_LEARNED_SKILL_CNT )
		return false;

	/// Learn skills if applicable
	if ( this->Skill_LEARN( iSkillSlotNO, iSkillIDX , bSubPOINT ) ) {
		/// If the result in the change in the speed of the passive skill.
		//PY: NOPE. This is now controlled by the server
		//this->Update_SPEED ();
	}

	m_SkillSlot.SetSkillSlot( iSkillSlotNO, iSkillIDX, iSkillLevel ); 

	return true;
}
///  ���� ��ų�� ������
bool CObjUSER::Skill_LevelUp( int iSkillSlotNo, int iSkillIndex )
{
	if( iSkillSlotNo >= MAX_LEARNED_SKILL_CNT )
		return false;

	m_SkillSlot.SkillLevelUp( iSkillSlotNo, iSkillIndex );
	return true;
}


void CObjUSER::Update_SPEED ()
{	
	m_nRunSPEED     = Cal_RunSPEED ();
	/*m_fRunAniSPEED  = Cal_RunAniSPEED( m_nRunSPEED );
	m_nAtkAniSPEED  = Cal_AtkAniSPEED( Get_R_WEAPON() );	*/
	if ( this->GetPetMode() < 0 )
	{
 		m_fRunAniSPEED = Cal_RunAniSPEED( m_nRunSPEED );
 		m_nAtkAniSPEED = Cal_AtkAniSPEED( this->GetPartITEM(BODY_PART_WEAPON_R) );
 	} 
	else 
	{
		/// ��ƲīƮ ���� ���� �߰� nAvy - 2005/7/28
#if defined(_GBC)
		m_fRunAniSPEED = ( m_nRunSPEED + 500 ) / 1000.f;
		m_nAtkAniSPEED = Cal_AtkAniSPEED( this->GetPartITEM(BODY_PART_WEAPON_R) );

		/*for( int i = 0 ; i < MAX_RIDING_PART; ++i )
			m_nAtkAniSPEED += PAT_ITEM_ATK_SPD( m_sPetPartItemIDX[ i ].m_nItemNo );*/
#else
 		m_fRunAniSPEED = ( m_nRunSPEED + 500 ) / 1000.f;
 		///<- 2005/7/25 īƮ �ý��� ���� �߰��� ���� : nAvy
		//m_nAtkAniSPEED = ( 1500.f / ( ( PAT_ITEM_ATK_SPD( m_sPetPartItemIDX[ RIDE_PART_ARMS ].m_nItemNo )) + 5 ) ) + this->m_iAddValue[ AT_ATK_SPD ];
		m_nAtkAniSPEED = ( 1500.f / ( ( PAT_ITEM_ATK_SPD( m_sPetPartItemIDX[ RIDE_PART_ARMS ].m_nItemNo )) + 5 ) ) + this->m_iAddValue[ AT_ATK_SPD ];
		///->
#endif
 	}


	LogString (LOG_DEBUG, "CmPerSEC: %f, AniSpeed: %f \n", m_nRunSPEED, m_fRunAniSPEED );
};

//-------------------------------------------------------------------------------------------------
void CObjUSER::Add_EXP (short nExp)
{	
	m_GrowAbility.m_lEXP += nExp;

	int  iNeedEXP = this->Get_NeedEXP( this->Get_LEVEL() );
	bool bLevelUp=false;

	while ( m_GrowAbility.m_lEXP >= iNeedEXP ) {
		// ���� ���� ???
		this->Set_LEVEL( this->Get_LEVEL() + 1 );

		g_itMGR.AppendChatMsg(CStr::Printf (">>> %s LEVEL UP to %d", Get_NAME(), Get_LEVEL() ), IT_MGR::CHAT_TYPE_SYSTEM );

		m_GrowAbility.m_lEXP -= iNeedEXP;

		iNeedEXP = this->Get_NeedEXP( this->Get_LEVEL() );
		bLevelUp = true;
	}

	/*if ( bLevelUp )
		ON_CharLevelUp( m_nIndex );*/
}

void CObjUSER::RideCartToggle( bool bRide )
{
	CObjAVT::RideCartToggle( bRide );

	this->UpdateAbility();
	Cal_PatAbility();

	/// Patž�� ���ο� ���� ��ų�� Ȱ��ȭ/��Ȱ��ȭ�� �ٲپ� �ش�.
	/// 
	m_SkillSlot.SetActiveSkillEnableByRideState( bRide );

}

//-------------------------------------------------------------------------------------------------
/// Ŭ���� ���ο� ������ �ƹ�Ÿ ������ �̿��ؼ�, ������Ʈ �Ѵ�.
void CObjUSER::Update (bool bUpdateBONE )
{
	CObjAVT::Update( bUpdateBONE );

 	// 7. Attack, the moving speed update.
	//PY: NOPE. Server does this now
	//this->Update_SPEED ();
}


//-------------------------------------------------------------------------------------------------
/// Add : 2004 / 2/ 12 : nAvy
bool CObjUSER::Quest_IsPartyLEADER()
{
	return CParty::GetInstance().IsPartyLeader();
}
int	 CObjUSER::Quest_GetPartyLEVEL()
{
	return CParty::GetInstance().GetLevel();
}

/*override*/int	CObjUSER::Quest_GetZoneNO()
{
	return g_pTerrain->GetZoneNO();
}

/*override*/int	CObjUSER::Quest_DistanceFrom(int iX, int iY, int iZ)
{
	int iDistance = CD3DUtil::distance ((int)m_PosCUR.x, (int)m_PosCUR.y, iX, iY );
	return iDistance;
}

/*override*/int	CObjUSER::Quest_PartyMemberCNT()
{
	return CParty::GetInstance().GetMemberCount();
}


//-------------------------------------------------------------------------------------------------
/*override*/int	CObjUSER::Get_BulletNO()
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

	if( iShotType < MAX_SHOT_TYPE && CUserDATA::m_Inventory.m_ItemSHOT[ iShotType ].GetQuantity() > 0 )
		iBulletNo = NATURAL_BULLET_NO( CUserDATA::m_Inventory.m_ItemSHOT[ iShotType ].GetItemNO() );
	
	return iBulletNo;
}

void CObjUSER::SubActiveBulletQuantity( )
{
	tagITEM sItem;
	sItem.m_cType   = ITEM_TYPE_WEAPON;
	sItem.m_nItemNo = Get_R_WEAPON();

	int iBulletNo = 0;
	int iShotType = sItem.GetShotTYPE();

	if( iShotType >= MAX_SHOT_TYPE )
		return;


	CItem* pItem = m_ItemSlot.GetItem( INVENTORY_SHOT_ITEM0 + iShotType );
	if( pItem == NULL )
	{
		assert( pItem && "Not Found Bullet @CObjUSER::SubActiveBulletQuantity() " );
		return;
	}

	pItem->SubQuantity( 1 );
	m_ItemSlot.ChangeItemStatus( INVENTORY_SHOT_ITEM0 + iShotType );
	
	///�κ��丮���� ����
	///gsv_SET_BULLET���� ���õȰ��� 0��� ( �� ����ߴٰ� �˷����ٸ� );
	if( CObjAVT::m_ShotData[ iShotType ].m_nItemNo == 0 )
	{
		ClearITEM( INVENTORY_SHOT_ITEM0 + iShotType );
		UpdateAbility();
		g_itMGR.AppendChatMsg( STR_WARN_BULLET_USEUP, IT_MGR::CHAT_TYPE_SYSTEM );
		D3DVECTOR PosSCR;
		g_pAVATAR->GetScreenPOS( PosSCR );	
		g_pNet->Send_cli_STOP( PosSCR );
	}
	UpdateInventory();
}

BYTE  CObjUSER::GetCur_MOVE_MODE ()
{
	return this->m_btMoveMODE;
}

/// ���� ���� ������ �ٵǾ� Ż�� �ִ°�?
bool CObjUSER::CanRidePET()
{
// ȫ�� :
#ifdef _GBC
	for( int i = 0; i < MAX_RIDING_PART - 2; i++ )
	{
		if( m_Inventory.m_ItemRIDE[ i ].GetTYPE() == 0 )
		{
			return false;
		}
	}
#else
	for( int i = 0; i < MAX_RIDING_PART - 1; i++ )
	{
		if( m_Inventory.m_ItemRIDE[ i ].GetTYPE() == 0 )
		{
			return false;
		}
	}
#endif	

	return true;
}


//2�ν� īƮ
bool	CObjUSER::IsRideItem(int iIndex_)
{
	if( iIndex_>=0 
		&& iIndex_<MAX_RIDING_PART
		&& m_Inventory.m_ItemRIDE[ iIndex_ ].GetTYPE() != 0 
		&& PAT_ITEM_ABILITY_TYPE( m_Inventory.m_ItemRIDE[ iIndex_ ].GetItemNO() ) )
	{
		return true;
	}
	return false;
}

void	CObjUSER::Set_HP (int iHP)
{	
	assert( iHP != 0 );
	SetCur_HP( iHP );				
}
void	CObjUSER::Set_MP (int iMP)
{	
	SetCur_MP( iMP );				
}
//--------------------------------------------------------------------------------
/// @brief  : CInventoryDlg�� VirtualInventory�� WeightRate�� �����Ѵ�.
//--------------------------------------------------------------------------------
void	CObjUSER::UpdateInventory()
{
	if( m_bWaitUpdateInventory )
		return;

	/// ���� �κ��丮 ����
	//g_itMGR.UpdateInventory();

	/// ���Ժ��� ����	
	int iMaxWeight  = Get_MaxWEIGHT();///�ִ� ������

	//assert( iMaxWeight > 0 );
	if( iMaxWeight <= 0 )
		return;

	int iCurrWeight = 0;///���� ������
	int iItemWeight = 0;///������ 1���� ����

	tagITEM*	pItem = NULL;
	for( int iIndex = 0; iIndex < INVENTORY_TOTAL_SIZE; ++iIndex )
	{
		pItem = &(m_Inventory.m_ItemLIST[ iIndex ]);
		assert( pItem );
		if( !pItem->IsEmpty() )
		{
			iItemWeight = ITEM_WEIGHT( pItem->GetTYPE(), pItem->GetItemNO() );

			if( pItem->IsEnableDupCNT() )
				iCurrWeight += iItemWeight * pItem->GetQuantity();
			else
				iCurrWeight += iItemWeight;
		}
	}

	SetCurrWeight( iCurrWeight );

	BYTE	btPrevWeightRate = GetWeightRate();
	BYTE	btCurrWeightRate = 0;
	

	int		iCurrWeightRate  = iCurrWeight * 100 / iMaxWeight ;

	if( iCurrWeightRate >= 250 )
		btCurrWeightRate = 250;
	else
		btCurrWeightRate = (BYTE)iCurrWeightRate;

	SetWeightRate( btCurrWeightRate );

	///��������( 80%, 100% )�� �Ѱų� ���������� �������� �˸���.
	int		iPrevWeightRateState = 0;
	if( btPrevWeightRate < 100 )
		iPrevWeightRateState = 0;
	else if( btPrevWeightRate < 110 )
		iPrevWeightRateState = 1;
	else
		iPrevWeightRateState = 2;

	int		iCurrWeightRateState = 0;
	if( btCurrWeightRate < 100 )
		iCurrWeightRateState = 0;
	else if( btCurrWeightRate < 110 )
		iCurrWeightRateState = 1;
	else
		iCurrWeightRateState = 2;


	if( iPrevWeightRateState != iCurrWeightRateState ) 
	{
		if( CGame::GetInstance().GetCurrStateID() == CGame::GS_MAIN ) 
			g_pNet->Send_cli_SET_WEIGHT_RATE( btCurrWeightRate );
		else
		{
			if( btCurrWeightRate >= 100 )		
				g_pAVATAR->SetCMD_TOGGLE( MOVE_MODE_WALK + TOGGLE_TYPE_DRIVE );
		}
	}
}

short	CObjUSER::Add_ITEM(short nListRealNO, tagITEM &sITEM)
{
	short nRet = CUserDATA::Add_ITEM( nListRealNO, sITEM );
	m_ItemSlot.AddItem( nListRealNO, sITEM );
	UpdateInventory();
	return nRet;
}

void	CObjUSER::Sub_ITEM(short nListRealNO, tagITEM &sITEM)
{
	CUserDATA::Sub_ITEM( nListRealNO, sITEM );
	m_ItemSlot.SubItem( nListRealNO, sITEM );
	UpdateInventory();
}

void	CObjUSER::ClearITEM(WORD wListRealNO)
{
	CUserDATA::ClearITEM( wListRealNO );
	m_ItemSlot.DelItem( wListRealNO );
	UpdateInventory();
}


short	CObjUSER::Add_CatchITEM (short nListRealNO, tagITEM &sITEM)
{
	short nRet = CUserDATA::Add_CatchITEM( nListRealNO, sITEM );
	if( sITEM.GetTYPE() != ITEM_TYPE_MONEY )
	{
		m_ItemSlot.AddItem( nListRealNO, sITEM );
		UpdateInventory();
	}
	///�κ��丮�� �������� ����
	g_pSoundLIST->IDX_PlaySound( SID_GETTING_ITEM );
	return nRet;
}

short	CObjUSER::Add_ITEM (tagITEM &sITEM)
{
	short nRet = CUserDATA::Add_ITEM( sITEM );
	m_ItemSlot.AddItem( sITEM );
	UpdateInventory();
	return nRet;
}

void	CObjUSER::Set_ITEM(short nListRealNO, tagITEM& sITEM)
{
	tagITEM* pItem = &(CUserDATA::m_Inventory.m_ItemLIST[ nListRealNO ] );

	///�������Կ� �������� �ְ� ... �ߺ������ϸ�, ���� �������ϰ��� Count�� �ø���.
	if( !pItem->IsEmpty() && !sITEM.IsEmpty() && sITEM.IsEnableDupCNT() && pItem->IsEqual ( sITEM.GetTYPE(), sITEM.GetItemNO() ) )
	{
		CUserDATA::m_Inventory.m_ItemLIST[ nListRealNO ] = sITEM;
		GetItemSlot()->ChangeItemStatus( nListRealNO );
	}
	else
	{
		CUserDATA::Set_ITEM( nListRealNO, sITEM );
		if( !sITEM.IsEmpty() )
		{
			if( (nListRealNO > 0 && nListRealNO < INVENTORY_ITEM_INDEX_0) || 
				(nListRealNO >= INVENTORY_SHOT_ITEM0 && nListRealNO <  INVENTORY_SHOT_ITEM0 + MAX_SHOT_TYPE ) )///������
				g_pSoundLIST->IDX_PlaySound( ITEM_EQUIP_SOUND( sITEM.GetTYPE(), sITEM.GetItemNO() )  );
		}
	}
	///m_ItemSlot�� Add,Sub,DelItem�� CUserDATA::Set_ITEM���� ȣ��ȴ�.
	//m_ItemSlot.SetItem( nListRealNO, sITEM );


	if( nListRealNO >= INVENTORY_RIDE_ITEM0 && nListRealNO < INVENTORY_RIDE_ITEM0 + MAX_RIDING_PART )
	{
		if( !sITEM.IsEmpty() )
		{
			int iItemIDX = sITEM.GetItemNO();
			
			if( nListRealNO == INVENTORY_RIDE_ITEM0 )
				g_pAVATAR->SetPetType( PAT_ITEM_PART_TYPE( iItemIDX ) );

			g_pAVATAR->SetPetParts( nListRealNO - INVENTORY_RIDE_ITEM0, iItemIDX );
		}
		else
		{
			g_pAVATAR->SetPetParts( nListRealNO - INVENTORY_RIDE_ITEM0, 0 );
		}
	}
	UpdateInventory();
}

void	CObjUSER::UpdateAbility()
{  

	Cal_BattleAbility(); 
	Calc_AruaAddAbility();
	
	// changing the position of UpdateInventory () and Update_SPEED ().
 	UpdateInventory();///The maximum amount of possession may have been changed.

	//PY: I just sent in the move speed from the server. Why would i want it to be calculated again so that the damn values are different?
	//just NO. Commenting this out
	//Update_SPEED();	
	

}		

void	CObjUSER::SetBankItem( int iIndex, tagITEM& Item )
{
	CUserDATA::m_Bank.Set_ITEM( iIndex, Item );


	CBank::GetInstance().SetItem( iIndex, Item );
}

void	CObjUSER::InitBank()
{
	CUserDATA::m_Bank.Init();
	CBank::GetInstance().ClearItems();
}


int	CObjUSER::Proc (void)
{
	m_SkillSlot.UpdateSkillSlot();

	return CObjAVT::Proc();
}

//-------------------------------------------------------------------------------------------------
/// ���� ����	
short CObjUSER::GetCur_WEIGHT()
{
	return m_nCurrWeight;
}

void CObjUSER::SetCurrWeight( short nWeight )
{
	m_nCurrWeight = nWeight;
}

//-------------------------------------------------------------------------------------------------
/// ���弭������ ����ũ�� 
void  CObjUSER::SetUniqueTag( DWORD dwUniqueTag )
{
	m_dwUniqueTag = dwUniqueTag;
}

DWORD CObjUSER::GetUniqueTag()
{
	return m_dwUniqueTag;
}

//-------------------------------------------------------------------------------------------------
/// ������ ����Ǿ� �ִ� ���� ���� ó���� ���� Method
void CObjUSER::SetServerSavedMemoCount( int iCount )
{
	m_iServerSavedMemoCount = iCount;
}

int	 CObjUSER::GetServerSavedMemoCount()
{
	return m_iServerSavedMemoCount;
}

void CObjUSER::DecreaseServerSavedMemoCount()
{
	--m_iServerSavedMemoCount;
	assert( m_iServerSavedMemoCount >= 0 );
	if( m_iServerSavedMemoCount < 0 )
		m_iServerSavedMemoCount;
}

/// ��ȯ���� �� ��..
int	CObjUSER::GetCur_SummonMaxCapacity()
{ 
	return 50 + GetPassiveSkillValue( AT_PSV_SUMMON_MOB_CNT ); 
}

void CObjUSER::AddSummonedMob( int iIndex, int iSkillIDX )
{ 
	/// �̹� �ִ³��̶��..
	std::list< SummonMobInfo >::iterator begin = m_SummonedMobList.begin();

	for( ; begin != m_SummonedMobList.end() ; ++begin )
	{
		SummonMobInfo& monInfo = *begin;

		if( monInfo.iIndex == iIndex )
			return;
	}
		
	///if( itor == m_SummonedMobList.end() )
	{
		SummonMobInfo summonMobInfo;
		summonMobInfo.iIndex	= iIndex;
		summonMobInfo.iSkillIDX = iSkillIDX;

		CObjCHAR* pObj = g_pObjMGR->Get_ClientCharOBJ( iIndex, false );
		if( pObj )
		{
			m_iSummonMobCapacity += NPC_NEED_SUMMON_CNT( pObj->Get_CharNO() );

			summonMobInfo.iCapacity = NPC_NEED_SUMMON_CNT( pObj->Get_CharNO() );
			m_SummonedMobList.push_back( summonMobInfo ); 
		}
	}
}

void CObjUSER::SubSummonedMob( int iIndex )
{ 
	/// �̹� �ִ³��̶��..
	std::list< SummonMobInfo >::iterator begin = m_SummonedMobList.begin();

	for( ; begin != m_SummonedMobList.end() ; ++begin )
	{
		SummonMobInfo& monInfo = *begin;

		if( monInfo.iIndex == iIndex )
		{	
			m_iSummonMobCapacity -= monInfo.iCapacity;			
			
			m_SummonedMobList.erase( begin );
			break;
		}
	}		
}

bool CObjUSER::GetSummonedMobInfo( int iIndex, SummonMobInfo& mobInfo )
{
	/// �̹� �ִ³��̶��..
	std::list< SummonMobInfo >::iterator begin = m_SummonedMobList.begin();

	for( ; begin != m_SummonedMobList.end() ; ++begin )
	{
		SummonMobInfo& monInfo = *begin;

		if( monInfo.iIndex == iIndex )
		{
			mobInfo = monInfo;
			return true;
		}
	}

	return false;
}

short CObjUSER::GetCur_STAMINA()
{
	return CUserDATA::m_GrowAbility.m_nSTAMINA;
}

void CObjUSER::SetCur_STAMINA( short nStamina )
{
	CUserDATA::m_GrowAbility.m_nSTAMINA = nStamina;
}
//*----------------------------------------------------------------------
/// @brief overriden CObjAVT::GetStamina() -- CObjAVT::Proc���� ȣ��� CObjUSER�� ��� ȣ��Ǿ� ����.
//*----------------------------------------------------------------------
short CObjUSER::GetStamina()
{
	return GetCur_STAMINA();
}

void CObjUSER::SetWaitUpdateInventory( bool bWait )
{
	m_bWaitUpdateInventory = bWait;
}

void CObjUSER::Set_JOB (int iValue)
{		
	SetCur_JOB (iValue);
	if( GetClanID() )
		g_pNet->Send_cli_CLAN_MEMBER_JOBnLEV( Get_LEVEL(), iValue ) ;

	//20050823 ȫ�� ġƮ�� ������ �������� ��쿡�� MAXHP ��ȭ ǥ�� ���ֱ� ����.
	UpdateAbility();
}


bool CObjUSER::IsInventoryFull( std::list< tagITEM >& appendItems, std::list< pair<int,tagITEM> >& removeItems  )
{
	CInventory TempInventory;
	memcpy( &TempInventory, &m_Inventory, sizeof( CInventory ) );

	std::list< tagITEM >::iterator iterAppend;
	std::list< pair< int, tagITEM> >::iterator iterRemove;
	short nInvenIndex = 0;
	short nWeight	  = 0;

	for( iterRemove = removeItems.begin(); iterRemove != removeItems.end(); ++iterRemove )
		TempInventory.SubtractITEM( iterRemove->first, iterRemove->second, nWeight );

	for( iterAppend = appendItems.begin(); iterAppend != appendItems.end(); ++iterAppend )
	{
		if( TempInventory.AppendITEM(*iterAppend , nWeight ) == -1 )
			return true;
	}
	return false;
}

bool CObjUSER::IsInventoryFull( std::list< tagITEM >& appendItems )
{
	CInventory TempInventory;
	memcpy( &TempInventory, &m_Inventory, sizeof( CInventory ) );

	std::list< tagITEM >::iterator iterAppend;
	short nInvenIndex = 0;
	short nWeight	  = 0;

	for( iterAppend = appendItems.begin(); iterAppend != appendItems.end(); ++iterAppend )
	{
		if( TempInventory.AppendITEM(*iterAppend , nWeight ) == -1 )
			return true;
	}
	return false;
}

void	CObjUSER::SetBattleTime( DWORD dwTime )
{
	m_dwLastBattleTime = dwTime;
	//LogString( LOG_NORMAL,"Set Battle Time\n" );
}

DWORD   CObjUSER::GetLastBattleTime()
{	
	return m_dwLastBattleTime;
}



void CObjUSER::ChangeModelInfo( short nType, int iValue )
{
	/// AT_SEX, AT_HAIR, AT_FACE
	switch( nType )
	{
		case AT_SEX:			
			{
			this->m_bIsFemale = iValue;
			this->m_btRace = iValue;
			this->m_CharMODEL.Set_SEX( m_bIsFemale );			
			this->Update( true );
			}
			break;
		case AT_HAIR:
			this->SetPartITEM( BODY_PART_HAIR, iValue );
			m_BasicINFO.m_cHairIDX = iValue;
			this->Update();
			break;
		case AT_FACE:			
			this->SetPartITEM( BODY_PART_FACE, iValue );
			m_BasicINFO.m_cFaceIDX = iValue;			
			this->Update();
			break;

		default:
			break;
	}
}

short CObjUSER::GetPsv_ATKSPEED (float fCurSpeed, short nRightWeaponItemNo)
{
	return this->GetPassiveSkillAttackSpeed( fCurSpeed, nRightWeaponItemNo ) + CUserDATA::m_iAddValue[ AT_ATK_SPD ];
}

/// If the function returns more haejyeoseo the value fixed by the Skill
/// arua Also adds value changed by the state.					2005/7/12 : nAvy
int CObjUSER::GetCur_MaxHP ()
{ 
	//return this->m_Battle.m_nMaxHP + m_EndurancePack.GetStateValue( ING_INC_MAX_HP ) + m_AruaAddHp; 
	return this->m_Battle.m_nMaxHP; //PY: removed the extra stuff so it will only return the value sent in from the server
};

/// ��ų�� ���� ������ ���� ������� �������� ������ �Լ�
/// arua���¿� ���ؼ� ����� ���� �����ش�.					2005/7/12 : nAvy
int	CObjUSER::GetCur_MaxMP ()
{ 
	return this->m_Battle.m_nMaxMP; // + m_EndurancePack.GetStateValue( ING_INC_MAX_MP ) + m_AruaAddMp; // PY: Nope. not adding any extra stuff. MaxMP is now set from the server only.    
};
//------------------------------------------------------------------------------------
/// @brief aura Values for the changed conditions by					2005/7/13 : nAvy
/// PY: Removing all the add on stuff so that it just compares the raw stats //Numenor: Edited it accordingly to our server
//------------------------------------------------------------------------------------
int CObjUSER::GetCur_CRITICAL()		
{ 
	return GetDef_CRITICAL(); // + m_EndurancePack.GetStateValue( ING_INC_CRITICAL ) - m_EndurancePack.GetStateValue( ING_DEC_CRITICAL ) + m_AruaAddCritical;
}

int CObjUSER::GetCur_ATK()
{ 
	return GetDef_ATK(); // + m_EndurancePack.GetStateValue( ING_INC_APOWER ) - m_EndurancePack.GetStateValue( ING_DEC_APOWER ) + m_AruaAddAttackPower;		
}

int CObjUSER::GetCur_DEF()
{ 
	return GetDef_DEF(); // + m_EndurancePack.GetStateValue( ING_INC_DPOWER ) - m_EndurancePack.GetStateValue( ING_DEC_DPOWER ) + m_AruaAddDefence;		
}

//------------------------------------------------------------------------------------
/// @brief virtual method from CObjAI					2005/7/13 : nAvy
//------------------------------------------------------------------------------------
int CObjUSER::GetOri_MaxHP()
{
	return m_Battle.m_nMaxHP;
}
//------------------------------------------------------------------------------------
/// @brief virtual method from CObjAI					2005/7/13 : nAvy
//------------------------------------------------------------------------------------
int CObjUSER::GetOri_MaxMP()
{
	return m_Battle.m_nMaxMP;
}
//------------------------------------------------------------------------------------
/// @brief Arua stat buff additional calculations for the jammed state 2005/7/13 : navy
//------------------------------------------------------------------------------------
void CObjUSER::Calc_AruaAddAbility()	//PY: adding the fairy. We need to disable this
{
	if( CCountry::GetSingleton().IsApplyNewVersion() )
	{
		if( m_IsAroa )
		{
			m_AruaAddHp			= GetDef_MaxHP(); //* 0.2;
			m_AruaAddMp			= GetDef_MaxMP(); // * 0.2;
			m_AruaAddCritical	= GetDef_CRITICAL(); // * 0.2;
			m_AruaAddAttackPower	= GetDef_ATK(); // * 0.2;
			m_AruaAddDefence		= GetDef_DEF(); // * 0.2;
		}
		else
		{
			m_AruaAddHp			= 0;
			m_AruaAddMp			= 0;
			m_AruaAddCritical	= 0;
			m_AruaAddAttackPower	= 0;
			m_AruaAddDefence		= 0; 

		}
		CObjCHAR::Calc_AruaAddAbility();
	}
}

//------------------------------------------------------------------------------------
bool CObjUSER::SetCMD_Skill2OBJ ( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget, short nSkillIDX)
{
	CObjSkill2ObjCommand* pObjCommand = (CObjSkill2ObjCommand*)g_CommandFilter.GetCommandObject( OBJECT_COMMAND_Skill2OBJ );
	pObjCommand->SetCMD_Skill2OBJ( wSrvDIST, PosTO, iServerTarget, nSkillIDX );
	g_CommandFilter.SetPrevCommand( pObjCommand );

	//SubPatGuagePerPatSkillExcute( nSkillIDX  );

	return CObjAI::SetCMD_Skill2OBJ ( wSrvDIST, PosTO, iServerTarget, nSkillIDX);
}

void CObjUSER::SetCMD_Skill2SELF (short nSkillIDX)
{
	CObjSkill2SelfCommand* pObjCommand = (CObjSkill2SelfCommand*)g_CommandFilter.GetCommandObject( OBJECT_COMMAND_Skill2SELF );
	pObjCommand->SetCMD_Skill2SELF( nSkillIDX );
	g_CommandFilter.SetPrevCommand( pObjCommand );

	//SubPatGuagePerPatSkillExcute( nSkillIDX  );

	CObjAI::SetCMD_Skill2SELF( nSkillIDX );
}

void CObjUSER::SetCMD_Skill2POS (const D3DVECTOR& PosGOTO, short nSkillIDX)
{
	CObjSkill2PosCommand* pObjCommand = (CObjSkill2PosCommand*)g_CommandFilter.GetCommandObject( OBJECT_COMMAND_Skill2POS );
	pObjCommand->SetCMD_Skill2POS( PosGOTO, nSkillIDX );
	g_CommandFilter.SetPrevCommand( pObjCommand );

	//SubPatGuagePerPatSkillExcute( nSkillIDX  );

	CObjAI::SetCMD_Skill2POS( PosGOTO, nSkillIDX );
}



//------------------------------------------------------------------------------------------------------
/// @brief īƮ�� �߰����� �ɷ�ġ�� �����ִ� �Լ�
///			1. īƮ ������
///		- �������� ���� �����ֱ� ������ �� �κ��� ���� 2005/7/27 - nAvy
//------------------------------------------------------------------------------------------------------
void CObjUSER::Cal_PatAbility()
{
#if defined(_GBC)
	m_PatGuageMax = 0;
	for( int i = 0 ; i < MAX_RIDING_PART; ++i )
	{
		if( !m_Inventory.m_ItemRIDE[ i ].IsEmpty() )
			m_PatGuageMax += PAT_ITEM_CART_GAUAGE( m_Inventory.m_ItemRIDE[ i ].GetItemNO() );
	}

	if( GetCur_PatHP() > m_PatGuageMax )
	{
		_RPTF0 ( _CRT_ASSERT,"m_GrowAbility.m_nPatHP > m_PatGuageMax\n" );		
		SetCur_PatHP(m_PatGuageMax);
	}
#endif
}

int CObjUSER::GetPatGuageMax()
{
	return m_PatGuageMax;
}

int CObjUSER::GetPatGuageCurrent()
{
	return GetCur_PatHP();	
}

void CObjUSER::SetPatGuageMax( int guage )
{
	m_PatGuageMax = guage;
}

void CObjUSER::SetPatGuageCurrent( int guage )
{
	SetCur_PatHP(guage);	
}

//------------------------------------------------------------------------------------------------------
///����ȣ::���� ������ ��Ÿ���� �����Ѵ�. 
//------------------------------------------------------------------------------------------------------
BOOL CObjUSER::SetCoolTimeUseItem(char* uName,short iItem)
{
 
	//��뱹������...
	if(!CCountry::GetSingleton().IsUseItemDelayNewVersion())
		return FALSE;

	//�ƹ�Ÿ���� üũ�Ѵ�. 
	if(lstrcmp(this->Get_NAME(),uName) != 0)
		return FALSE;

	int iDelayType	 = USEITME_DELAYTIME_TYPE( iItem );
	float iDelayTick  = (float)(USEITME_DELAYTIME_TICK(iItem)) * 1000;

	//0 �� �ƴϸ� �����̸� �����Ѵ�. 
	if( iDelayType )
	{
		//������ Ÿ���� �־ ���� Ÿ���� ������ ���̶�� ����Ҽ� ���ٴ� 
		//�޼����� ä��â�� ����Ѵ�. 
		if( g_UseItemDelay.GetUseItemDelay( iDelayType ) > 0.0f)
		{
			///Use item delay �� �����Ǿ� �ִٸ� �н�..
			g_itMGR.AppendChatMsg( STR_CANT_DOUBLE_USE, IT_MGR::CHAT_TYPE_SYSTEM );
			return false;
		}
		//ó���̶�� ������ Ÿ���� �����Ѵ�. 
		g_UseItemDelay.SetUseItemDelay( iDelayType, iDelayTick );
		g_CurUseItemDelayTick.SetUseItemDelay( iDelayType, iDelayTick );
	}
	//�����϶� �ܵ����� ó���Ѵ�. 
	else
	{
		//�ڱ� �ڽ��� ������ �� 
		if(g_SoloUseItemDelayTick.GetUseItemDelay( iItem ) > 0.0f)
		{
			g_itMGR.AppendChatMsg( STR_CANT_DOUBLE_USE, IT_MGR::CHAT_TYPE_SYSTEM );
			return false;
		}

		//�ڱ� �ڽ��� ������ �ε��� �� ������ ƽ�� �����Ѵ�. 
		g_SoloUseItemDelayTick.SetUseItemDelay( iItem, iDelayTick );
	}	
	return TRUE;

}


//------------------------------------------------------------------------------------------------------
///����ȣ::��ų ��Ÿ���� �����Ѵ�. 
//------------------------------------------------------------------------------------------------------
BOOL CObjUSER::SetCoolTimeSkill(char* uName,int iSkill)
{

	//��뱹������...
	if(!CCountry::GetSingleton().IsUseItemDelayNewVersion())
		return FALSE;

	//�ƹ�Ÿ���� üũ�Ѵ�. 
	if(lstrcmp(this->Get_NAME(),uName) != 0)
		return FALSE;


	int iDelayType = SKILL_RELOAD_TYPE( iSkill);
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
				
		g_UseSkillDelay.SetUseItemDelay( iDelayType, iDelayTick );
		g_CurSkillDelayTick.SetUseItemDelay( iDelayType, iDelayTick );
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

		//�ڱ� �ڽ��� ��ų�ε��� �� ������ ƽ�� �����Ѵ�. 
		g_SoloSkillDelayTick.SetUseItemDelay( iSkill, iDelayTick );
		
	} 

	return TRUE;

}

void CObjUSER::Set_Block_CartRide(bool ride)
{
	GetSkillSlot()->SetActiveSkillEnableByRideState_Aid( ride );
}

