/*
	$Header: /Client/CObjCHAR.h 353   05-10-20 2:45p Gioend $
*/
#ifndef	__COBJCHAR_H
#define	__COBJCHAR_H
#include "IO_Model.h"
#include "CModelCHAR.h"
#include "Common\CObjAI.h"
#include "Common\IO_STB.h"
#include "common\IO_AI.h"
#include "NET_Prototype.h"	
#include "Interface/CEnduranceProperty.h"
#include "GameProc/CDayNNightProc.h"
#include "GameProc/Objvibration.h"

#include "ObjectCommand/ObjCommand.h"

#include <VECTOR>
#include <numeric>


#ifdef	__VIRTUAL_SERVER
#include "Common\\CRegenAREA.h"
#endif


#ifndef	__SERVER
#include "GameProc\changeactionmode.h"
#endif


#define	QUEST_EVENT_ON_DEAD		0

const int INVALID_DUMMY_POINT_NUM = 999;

//struct uniDAMAGE;

///
/// Damage class
///
struct tagDAMAGE {
	short	m_nTargetObjIDX;
	WORD	m_wDamage;
} ;


class CChangeVisibility;
class CObjCHAR_Collision;
class CObjFIXED;
class CClanMarkUserDefined;
//����ȣ 
class CObjCART;
class CObjAVT;
///
///	CObjCHAR class inherited from COjbAI
/// This is base class of AVATA, MOB class.
///

class CObjCHAR : public CObjAI
{

protected:
	int							m_iHP;
	int							m_iMP;
	int							m_iMaxHP;
	int							m_iMaxMP;


	CChangeVisibility*			m_pChangeVisibility;
	bool						m_bStopDead;			/// Stop ���·� ���..( �� �״� ��������� ��ȯ�� ����. )
    

public:
	float						m_fHeightOfGround;		// �⺻ �������� ĳ���� �߽��� ����( ��� ���� )
	bool						m_bDead;				/// �����κ��� Dead damage �� �޾Ҵ°�?
	/// �������� ��( ���������� �� )
	float						m_fAltitude;

protected :
	CObjCHAR_Collision			* m_pCollision; // �浹 ����
	float						m_fStature;				// ����(Ű)
	float						m_fScale;
	HNODE						m_hNodeGround; // ��� �ִ� ������Ʈ�� ���� �ڵ�

	bool						m_bUseResetPosZ; // Z���� ���� ������ �� ����.
	float 						m_fResetPosZ; // ���� ������ Z��. ���� ���� ��� ��, �� ������ ���ŵǾ�� �Ѵ�.
	float						m_fModelSPEED; // �� �̵� �ӵ�.

	/// ������..
	int							m_iTeamInfo;			// �� ����


	void						Add_EFFECT(CMODEL<CCharPART> *pCharPART, short nPartIDX, short nPointIDX, t_HASHKEY HashEffectFILE, bool bLinkNODE=true );
	void						LoadTRAIL   (CMODEL<CCharPART> *pCharPART, short nPartIDX, bool bLinkBONE, int iColorNO, int iDuration, int iBaseDummyIdx = 0 );
	void						UnloadTRAIL (short nPartIDX);

	CAI_OBJ*					AI_FindFirstOBJ( int iDistance );
	CAI_OBJ*					AI_FindNextOBJ ();

	bool						m_bFrameING;
	
protected:
	CCharMODEL					*m_pCharMODEL;

	CEffect						**m_ppBoneEFFECT;
	short						m_nEffectPointCNT[ MAX_BODY_PART ];
	CEffect						**m_pppEFFECT[ MAX_BODY_PART ];	// �⺻ ȿ��.

	int							m_iLastDummyIDX;

	HNODE						m_hNodeMODEL;
	/// Model nodes..
	
	union 
	{
		HNODE					*m_phPartVIS[ MAX_BODY_PART ];
		struct 
		{
			// ������ t_CharPART�� ����...
			HNODE				*m_phFaceVIS;
			HNODE				*m_phHairVIS;
			HNODE				*m_phHelmetVIS;

			HNODE				*m_phArmorVIS;
			HNODE				*m_phGauntletVIS;
			HNODE				*m_phBootsVIS;

			HNODE				*m_phGoggleVIS;
			HNODE				*m_phKnapsackVIS;

			HNODE				*m_phRWeaponVIS;
			HNODE				*m_phLWeaponVIS;
		} ;
	} ;

	/// Nodes of Trailes..
	union 
	{
		HNODE					m_hTRAIL[ 2 ];	// right=0, left=1
		struct 
		{
			HNODE				m_hR_TRAIL;
			HNODE				m_hL_TRAIL;
		} ;
	} ;


	union 
	{
		short					m_nCharIdx;		// NPC, MOB ��ȣ
		bool					m_bIsFemale;	// �ƹ�Ÿ: ����0, ���� 1
		BYTE					m_btRace;		// ����...
	} ;

	

	int							GetCurrentFrame ()						{	return ::getMotionFrame ( this->m_hNodeMODEL );		}
	void						ActionEVENT (int iActionIDX);
	
	
	DWORD						m_dwLastRecoveryUpdateTime;	///HP�� MP�� �ڵ� ȸ���Ǿ��� ���� �ð�	
	DWORD						m_dwFrameElapsedTime;		/// ���� ó������.. �帥�ð�..
	DWORD						m_dwElapsedTime;			/// ���� ó������.. �帥�ð�..

public:
	void						ClearTimer();

	CObjCHAR_Collision*			GetCollisionUtility(){ return m_pCollision; }
   
    
private:

	///
	///	Process action frame..
	///
	void						ActionSpecialFrame( int iActionIDX );

	void						ActionInFighting( int iActionIDX );
	void						ActionBow( int iActionIDX );
	void						ActionGun( int iActionIDX );
	void						ActionSkill( int iActionIDX );
	void						ActionImmediateSkill( int iActionIDX );

	void						FireEffectBullet( CObjCHAR *pTarget, int iBulletIDX, bool bDummy, int iSkillIDX );

	/// 44,64,75, 45,65,75 ó��..
	void						ShowCastingEffect( int iActionIDX );
	void						ActionSkillCastingFrame( int iCastingOrder );
	/// ��Ÿ���� ȿ��( ����.. )
	void						RepeatedlyHitEffect( int iHitOrder );


	enum	
	{
		LEFT_WEAPON = 0,
		RIGHT_WEAPON,
	};

public:
	void						Adjust_HEIGHT (void);
	void						ResetCUR_POS( D3DXVECTOR3& Pos );

	/// @brief �������� ���� ���̰����� ������ Adjust_Height()���� ���ŵǴ� ���̰� ����.
	/// @param bUseResetPosZ ���� ���̰��� ������� ����. ���̰��� ������ ������ true, �ʱ�ȭ�� ������ false�� �Է��Ѵ�.
	/// @param fPosZ ���� ���ŵ� ���̰�.
	/// @see GetResetPosZ()
	void						SetResetPosZ( bool bUseResetPosZ, float fPosZ )
	{
		if (m_bUseResetPosZ = bUseResetPosZ) { // = ��Ÿ�ƴ�.
			m_fResetPosZ = fPosZ;
		}
	}

	/// @brief �������� ���� ���̰��� �ִ����� ���� �� �� ���̰��� ��´�.
	/// @param fResetPosZ ���� ������ ���� ��
	/// @return ��ȿ�ϴٸ� true, ��ȿ���� �ʴٸ� false �� ����
	/// @see SetResetPosZ()
	bool						GetResetPosZ( float& fResetPosZ )
	{
		if (m_bUseResetPosZ) {
			fResetPosZ = m_fResetPosZ;
			return true;
		}
		return false;
	}

	bool						LoadModelNODE (char *szName);
	void						UnloadModelNODE ();

	void						Link_EFFECT (void);
	void						Unlink_EFFECT (void);

	void						New_EFFECT(short nPartIdx, short nItemNo, bool bLinkNODE=true);

	void						CreateSpecificPART( char *szName, int iPartIDX );
	void						DeleteSpecificPART( short nPartIdx, HNODE *pVIS );

	void						CreatePARTS (char *szName);
	void						DeletePARTS (bool bDelEFFECT=true);

	bool						CreateCHAR (char *szName, CCharMODEL *pMODEL, short nCharPartCNT, const D3DVECTOR &Position);
	void						DeleteCHAR (void);				// avatar, mob ����


	void						SetTeamInfo( int iTeamInfo ){ m_iTeamInfo = iTeamInfo; }

	
	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CGameObj virtual functions
	/// <

	/*override*/virtual bool	IsInViewFrustum()						{ return ( 0 != ::inViewfrustum( GetZMODEL() ) );	}	
	/*override*/bool			IsIntersect (float &fCurDistance)			{	return	CGameOBJ::IsIntersect(m_hNodeMODEL, fCurDistance);	}
	/*override*/void			InsertToScene (void);
	/*override*/void			RemoveFromScene (bool bIncludeEFFECT=true);

	/*override*/D3DXVECTOR3&	Get_CurPOS ();
	/// <
	/// < End
	//////////////////////////////////////////////////////////////////////////////////////////




	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CObjAI virtual functions
	/// <

	/*override*/ void			SetEffectedSkillFlag( bool bResult );
	/*override*/ bool			bCanActionActiveSkill();
	/*override*/ void			SetStartSkill( bool bResult ){ m_bCanStartSkill = bResult; }
	/*override*/ bool			bCanStartSkill(){ return m_bCanStartSkill; }					/// ������ ���� ĳ���� ��ŸƮ ��Ŷ�� �޾Ҵ°�?

	/*override*/ virtual int	Get_MaxHP ()					   {   return 1;			}
	/*override*/ virtual int	Get_MaxMP ()                       {   return 1;			}

	/*override*/ virtual int	Get_HP ()						{	return m_iHP;			}
	/*override*/ virtual int	Get_MP ()                       {   return m_iMP;			}

	/*override*/virtual void	Set_HP (int iHP);
	/*override*/virtual void	Set_MP (int iMP);

	/*override*/ bool			SetCMD_ATTACK ( int iServerTarget ){ return CObjAI::SetCMD_ATTACK( iServerTarget ); }

	/*override*/bool			IsInRANGE (CObjCHAR *pT, int iRange)		{	return CD3DUtil::distance ((int)m_PosCUR.x, (int)m_PosCUR.y, (int)pT->m_PosCUR.x, (int)pT->m_PosCUR.y ) <= iRange;	}

	/*override*/void			Adj_MoveSPEED ( WORD wSrvDIST, const D3DVECTOR& PosGOTO );
	/*override*/void			Adj_AniSPEED ( float fAniSpeed );
	/*override*/virtual int		Get_AttackRange ();
	/*override*/void			Set_MOTION (short nActionIdx, float fMoveSpeed=0, float fAniSpeed=1.0f, bool bAttackMotion=false, int iRepeatCnt = 0 );
	/*override*/void			Move_COMPLETED ();

	/*override*/void			Reset_Position ();

	///
	/// concerned command
	///
	/*override*/virtual void	SetCMD_ATTACK (int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosGOTO);	
	/*override*/bool			Attack_START (CObjCHAR *pTarget);
	/*override*/bool			Skill_START (CObjCHAR *pTarget);
	/*override*/bool			Casting_START( CObjCHAR *pTarget );
	/*override*/void			Casting_END();
	/*override*/void			Attack_END (bool bStopTrail=true);
	/*override*/void			MoveStart ();


	///*override*/void			SetCMD_SIT (void);
	///*override*/void			SetCMD_STAND (void);
	///*override*/void			SetCMD_MOVE (tPOINTF &PosFROM, tPOINTF &PosTO, int iServerTarget);
	///*override*/void			SetCMD_DIE ();
	///*override*/bool			SetCMD_TOGGLE (BYTE btTYPE);

	//
	///*override*/void			SetCMD_STOP ();
	///*override*/void			SetCMD_MOVE (float fPosX, float fPosY, BYTE btRunMODE);
	///*override*/void			SetCMD_RUNnATTACK(int iTargetObjTAG)

	///*override*/void			SetCMD_Skill2SELF (short nSkillIDX);
	///*override*/bool			SetCMD_Skill2OBJ (int iServerTarget, short nSkillIDX);
	///*override*/void			SetCMD_Skill2POS (tPOINTF &PosGOTO, short nSkillIDX);



	/// ���� Ÿ�� ��ġ�� �ٶ󺸵��� ȸ��.
	/*override*/void			Set_ModelDIR (t_POSITION &PosToView, bool bImmediate = false );
	/*override*/void			Set_ModelDIR ( float fAngleDegree, bool bImmediate = false );


	/// ���� �̵� �ӵ��� �����Ѵ�.
	/*override*/void			Set_ModelSPEED (float fMoveSpeed)
	{
		m_fModelSPEED = fMoveSpeed;
		::setModelMoveVelocity( m_hNodeMODEL, m_fModelSPEED );
	}
	/// ���� �̵� �ӵ��� ���´�.
	/*override*/float			Get_ModelSPEED ()
	{
		return m_fModelSPEED;
	}


	
	/*
	�� �������� ������Ʈ Ÿ�Ե��� Get_WalkSPEED(), GetOri_RunSPEED() �� ������ ���ؼ� �̵��ӵ��� ���Ѵ�.!!
	�ᱹ Get_WalkSPEED(), GetOri_RunSPEED() �� ��ӱ����� �Ű���..
	*/
	/*override*/ virtual float			Get_MoveSPEED ();
				 virtual float			Get_DefaultSPEED ();

	/*override*/ virtual short			Get_nAttackSPEED ();
	/*override*/ virtual float			Get_fAttackSPEED ();

	/*override*/ DWORD			GetIngDurationStateFLAG(){ return m_EndurancePack.GetStateFlag(); }; /// ���� ���� ���� �÷��װ��� ��´�.
	/*override*/ short			GetIngDurationStateSKILL( eING_TYPE eTYPE ){ return 0; };	/// ���� ���� ���¿� ����� ��ų��ȣ

	/// RECOVER_STATE_CHECK_TIME �������� Get_RecoverHP���� ���� HP, Get_ReocverMP���� ���� MP�� ���� �ش�.
	/*override*/ virtual void	RecoverHP( short nRecoverMODE );	
	/*override*/ virtual void	RecoverMP( short nRecoverMODE );	

	/*override*/virtual void	Do_DeadEvent	(CObjCHAR *pTarget){};	

	/*override*/virtual void	SetMotionRepeatCount( int iRepeatCount ) { ::setRepeatCount( GetZMODEL(), iRepeatCount ); };
	
	virtual int		 Cur_SKILL_DURATION(int iSkillNo)					{	return 0	;	}
	//Numenor: Virtualization. Return true duration from CObjUSER if "this" pointer is in fact a CObjUSER, i.e. if the current player does something. If we look at someone else, this function is called... and return therefore 0. Indeed we cannot acces other CHAR info without asking the server. Which is done thanks to packet.


protected:
	CChangeActionMode			m_ChangeActionMode;

public:
	/*override*/void			ChangeActionMode( int iActionMode ){ return m_ChangeActionMode.ChangeActionMode( iActionMode ); }


	/*override*/ virtual int	GetPetMode(){ return -1; }			

	/// <
	/// < End
	//////////////////////////////////////////////////////////////////////////////////////////




	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CAI_OBJ virtual functions
	/// <
	
	/*override*/int				Get_TeamNO ()						{	return m_iTeamInfo;}	/// �������� ���� ����ȣ
	/*override*/BYTE			Is_DAY()							{	return g_DayNNightProc.IsDay();}			/// �� �ð��� üũ ����ð����� ������?
	/*override*/int				Get_ZoneTIME()							{	return g_DayNNightProc.GetZoneTime();}			/// �� �ð��� üũ ����ð����� ������?
	

	/*override*/float			Get_CurXPOS ()						{	return m_PosCUR.x;	}
	/*override*/float			Get_CurYPOS ()						{	return m_PosCUR.y;	}
	/*override*/float			Get_CurZPOS ()						{	return m_PosCUR.z;}
	/*override*/float			Get_BornXPOS ()						{	return m_PosBORN.x;	}
	/*override*/float			Get_BornYPOS ()						{	return m_PosBORN.y;	}	


	/*override*/int				Get_RANDOM (int iMod)				{	return  (rand()%iMod);												}
	/*override*/float			Get_MoveDISTANCE ()					{	return	CD3DUtil::distance(m_PosBORN, m_PosCUR);					}
	/*override*/float			Get_DISTANCE (CAI_OBJ *pObj)		{	return	CD3DUtil::distance(m_PosCUR, ((CObjCHAR*)pObj)->m_PosCUR);	}	
	/*override*/void			Special_ATTACK ();
	/*override*/void			Set_EMOTION(short nEmotionIDX);

	
	/// <
	/// < End
	//////////////////////////////////////////////////////////////////////////////////////////







	/// ��� ������ ����Ʈ ����
	void							ClearAllEntityList();



	/////////////////////////////////////////////////////////////////////////////////
	/// �ܺο��� �ٿ��� ����Ʈ��..
	classDLLIST< CEffect* >				m_ExternalEffectLIST;

	classDLLIST< CEffect* >				m_WeatherEffectLIST;
public:
	void								AddExternalEffect( CEffect* pEffect );
	void								DeleteExternalEffect( CEffect* pEffect );

	void								ClearExternalEffect();


	/// ����
	void								AddWeatherEffect( CEffect* pEffect );
	void								DeleteWeatherEffect();

	/////////////////////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////////////////////
	/// Skill result effect ��ϸ���Ʈ
private:

	struct stEFFECT_OF_SKILL
	{
		DWORD				m_dwCreateTime;
		int					iSkillIDX;
		bool				bDamageOfSkill;
		int					iCasterINT;						/// �������� INT ���Ŀ� �ʿ�
		int					iCasterSKILL_DURATION;

		gsv_DAMAGE_OF_SKILL	EffectOfSkill;					/// damage_of_skill �� effect_of_skill �� ��ӹ޾����Ƿ� ���̸� ���ؼ�
	};

	std::vector< stEFFECT_OF_SKILL >	m_EffectedSkillList;	
	int									m_iEffectedSkillIndex;
	bool								m_bProcEffectedSkill;
	bool								m_bCanStartSkill;

public:	
	/*void								SetEffectedSkillIndex( int iEffectedSkillIdx )
	{ 
		m_iEffectedSkillIndex = iEffectedSkillIdx; 
		m_bProcEffectedSkill = true;
	}*/
	void								PushEffectedSkillToList( int iSkillIDX, gsv_DAMAGE_OF_SKILL EffectedSkill, int iCasterINT, int iCasterSKILL_DURATION, bool bDamageOfSkill = false );
	void								ProcTimeOutEffectedSkill();
	bool								ProcEffectedSkill();	
	void								ProcOneEffectedSkill( stEFFECT_OF_SKILL*	pEffectOfSkill );

	void								ProcEffectOfSkillInDamageOfSkill( int iSkillIDX, int iObjIDX, CObjCHAR* pChar, stEFFECT_OF_SKILL*	pEffectOfSkill );
	void								ApplyEffectOfSkill( int iSkillIDX, int iObjIDX, CObjCHAR* pChar, stEFFECT_OF_SKILL*	pEffectOfSkill, CObjCHAR* pCaster );
	/////////////////////////////////////////////////////////////////////////////////




	/////////////////////////////////////////////////////////////////////////////////
	/// Damage ��ϸ���Ʈ
private:
	struct stAPPLY_DAMAGE
	{
		DWORD			m_dwCreateTime;
		int				m_iAttacker;
		uniDAMAGE		m_Damage;
	};

	std::vector< stAPPLY_DAMAGE >		m_DamageList;

public:
	void								PushDamageToList( int iAttacker, uniDAMAGE wDamage );


	uniDAMAGE							PopCurrentAttackerDamage( int iAttacker );
	uniDAMAGE							PopTotalDamageFromList( int& iMaxDamage );	

	/// ��� ������ ó��( �װų�.. �� �׷� ��Ȳ����.. ���� )
	void								ClearAllDamage();

	/// �ð����� ó��
	void								ProcDamageTimeOut();

	/// Ÿ��ġ �ٷ� ǥ��
	void								CreateImmediateDigitEffect(  WORD wDamage  );

	/// Damage_of_Skill => �Ϲ� �������� ��ȯ����..
	void								ConvertDamageOfSkillToDamage( gsv_DAMAGE_OF_SKILL stDamageOfSkill );
	/////////////////////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////////////////////
	/// �ʵ������ ��� ��ϸ���Ʈ( Ȥ�� �ѹ��� ������ ����߸��� ���� ���ͷμ� )
private:
	std::vector< tag_DROPITEM >			m_FieldItemList;

public:
	void								PushFieldItemToList( tag_DROPITEM& wDamage ){ m_FieldItemList.push_back( wDamage ); }
	void								DropFieldItemFromList( );	
	/////////////////////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////////////////////
	/// ���Ӽ� Entity ���
public:
	CEndurancePack						m_EndurancePack;				/// �ʱ�ȭ ����Ʈ �̿� ��..

public:	
	bool								AddEnduranceEntity( int iEntityIDX, int iStatusSTBNO, int iEnduranceTime, int iEntityType )
	{
		return m_EndurancePack.AddEntity( iEntityIDX, iStatusSTBNO, iEnduranceTime, iEntityType );
	}

	int									GetEnduranceEntityCount(){ return m_EndurancePack.GetEntityCount(); }
	int									GetEnduranceStateSTBNO( int Index )
	{
		CEnduranceProperty* pEntity = m_EndurancePack.GetEntity( Index );
		if( pEntity == NULL )
			return 0;

		return pEntity->GetStateSTBNO();
	}		
	
	/// �������� ����, �Ҹ����� ���� ��..
	int									ProcFlushStateSkill( int iStateTableIndex ){ return m_EndurancePack.ProcFlushStateSkill( iStateTableIndex ); }
	/////////////////////////////////////////////////////////////////////////////////




	/////////////////////////////////////////////////////////////////////////////////
	/// �� ��ȯ ����
	/// �׼� �����ӿ� ���߰� ���� ������ �д�.
private:
	gsv_MOB_CHAR						m_SummonMob;
	bool								m_bHaveSummonedMob;				/// ��ȯ�ؾߵ� ���� �ִ°�?

public:
	void								SetSummonMobInfo( gsv_MOB_CHAR& MobInfo );
	void								SummonMob();

	/////////////////////////////////////////////////////////////////////////////////




	//////////////////////////////////////////////////////////////////////////////////////////////////
	/// queuing the command 
	//////////////////////////////////////////////////////////////////////////////////////////////////
	

	CObjCommandManager			m_CommandQueue;

	/// ���� ����� �����Ҽ� �ִ°�?
	/*override*/virtual bool	CanApplyCommand();
	void						ProcQueuedCommand();

	void						ClearAllCommand(){ m_CommandQueue.ClearCommand(); }


	/*override*/virtual void	PushCommandSit();
	/*override*/virtual void	PushCommandStand();
	/*override*/virtual void	PushCommandStop();
	/*override*/virtual void	PushCommandMove( const D3DVECTOR& PosTo, BYTE btRunMODE );
	/*override*/virtual void	PushCommandMove( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget );
	/*override*/virtual void	PushCommandAttack( int iServerTarget, WORD wSrvDIST, const D3DVECTOR &PosTO );
	/*override*/virtual void	PushCommandDie();
	/*override*/virtual void	PushCommandToggle( BYTE btTYPE );
	/*override*/virtual void	PushCommandSkill2Self( short nSkillIDX );
	/*override*/virtual void	PushCommandSkill2Obj( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget, short nSkillIDX );
	/*override*/virtual void	PushCommandSkill2Pos( const D3DVECTOR &PosGOTO, short nSkillIDX );


	//////////////////////////////////////////////////////////////////////////////////////////////////





	//////////////////////////////////////////////////////////////////////////////////////////////////
	/// Object Vibration
	//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	CObjvibration				m_ObjVibration;

public:
	void						StartVibration();

	//////////////////////////////////////////////////////////////////////////////////////////////////





#ifdef	__VIRTUAL_SERVER
	CRegenPOINT					*m_pRegenPOINT;
	classDLLNODE< CObjCHAR* >	*m_pRegenListNODE;
#endif
	
	
	//////////////////////////////////////////////////////////////////////////////////////////	
	/// > CObjCHAR virtual functions
	/// >

	// default npc skeleton
	virtual HNODE				Get_SKELETON ()	{	return m_pCharMODEL->GetZSKEL ();	}

		
	virtual bool				Check_EVENT (CGameOBJ *pSourOBJ, short nEventIDX=-1)		{	return false;	}

	virtual bool				IsFemale ()		=0;
	virtual short				IsMagicDAMAGE()	=0;

	virtual tagMOTION*			Get_MOTION (short nActionIdx)=0{ *(int*)0 = 10; };

	virtual int					Add_HP (int iAdd);
	virtual int					Add_MP (int iAdd)				{	return (m_iMP+=iAdd);	}

	virtual int					Sub_HP (int iSub);
	virtual int					Sub_MP (int iSub)				{	return (m_iMP-=iSub);	}



	virtual void				Add_EXP (short nExp)=0{ *(int*)0 = 10; };	
	virtual short				GetOri_WalkSPEED ()=0{ *(int*)0 = 10; };
	virtual short				GetOri_ATKSPEED()=0{ *(int*)0 = 10; };
	virtual short				GetOri_RunSPEED()=0{ *(int*)0 = 10; };


	/// �ƹ�Ÿ�� ��� ���� �Ҹ����� �Ѿ˿� ������ �޴´�.
	virtual int					Get_BulletNO ();

	/// Ư�� ��ǥ�� �̵��Ͽ� ���� ����. ���̴� ���� ��ǥ ���� ���� ������Ʈ �� ���� ���� ��ġ
	virtual void				DropFromSky ( float fX, float fY );

	virtual int					Get_INT(){ return 0; }						/// !!!Cal Ŭ�������� CObjCHAR �� �ɹ��� �����ϴºκ��� �־� �̰͸�..


	/// �������� �����ġ ������ ���ؼ� ���� ����Ǿ��ִ� �ɷ¼�ġ( �нú� ��ų ���� )
	virtual int					Get_DefaultAbilityValue( int iType ){ return 1; };



	///
	/// ������ ������ ������ ���� �� �÷����� ���¿����� ��� ����( STOP, MOVE... )
	///
	virtual void				SetUpdateMotionFlag( bool bUpdate ){}
	virtual bool				GetUpdateMotionFlag(){return false;} 


	///
	/// ���� ���������� ��ũ�� �Ǿ��°�?
	///
	virtual bool				IsChild(){ return false; }


	//----------------------------------------------------------------------------------------------------	
	/// @brief �� ������Ʈ Ÿ�Ը��� �߰� ó�����׵��� ���ݾ� �ִ�.
	//----------------------------------------------------------------------------------------------------
	virtual int					Proc (void);


	//----------------------------------------------------------------------------------------------------	
	/// @brief ĳ���� ���� ����Ʈ�� ���δ�.
	//----------------------------------------------------------------------------------------------------
	virtual void				ShowEffectOnCharByIndex( int iEffectIDX, int iSoundIDX = 0, bool bWeatherEffect = false );
	virtual void				ShowEffectOnCharByHash( int iHash, int iSoundIDX = 0, bool bWeatherEffect = false ); 


	/// >
	/// > End
	//////////////////////////////////////////////////////////////////////////////////////////

	

	/// ���� Ÿ�� ��ġ�� �ٶ󺸵��� ȸ��.	
	void						Add_ModelDIR( float fAngleDegree );
	
	D3DXVECTOR3					Get_GotoPOSITION ()					{	return m_PosGOTO;	}
	D3DXVECTOR3					Get_BornPOSITION ()					{	return m_PosBORN;	}

	float						Get_DISTANCE (t_POSITION PosBASE)	{	return	CD3DUtil::distance(m_PosCUR, PosBASE);						}

	

public:
	/// not overrided function
	int							Get_RWeapon(){ return Get_R_WEAPON(); }	

	CObjCHAR ();
	virtual ~CObjCHAR ();

	HNODE						GetZMODEL()									{	return m_hNodeMODEL;	}
	CMODEL<CCharPART> *			GetPartMODEL(short nPartIDX)				{	return m_pCharMODEL->GetCharPART( nPartIDX );	}
	int							Get_CharNO ()								{	return	m_nCharIdx;				}	

	bool						LinkDummy(HNODE hLinkNODE, short nDummyIDX);
	bool						Link2LastDummy(HNODE hLinkNODE);
	int							GetLastDummyIDX()							{	return m_iLastDummyIDX;	}

	

	bool						IsIntersect (HNODE hModel)					{	return ( ::intersectNode( m_hNodeMODEL, hModel)!=0 );		}



	
	void						SetSCALE (float fScale)						{	m_fScale=fScale, ::setScale(m_hNodeMODEL, fScale, fScale, fScale);					}
	float						Get_SCALE(){ return m_fScale; }

	void						Rotate (D3DXQUATERNION &Rotate)				{	::setQuaternion( m_hNodeMODEL, Rotate.w, Rotate.x, Rotate.y, Rotate.z );			}
	/// NPC�� ��� STB�� ������ ���̰� �� ������ �� ���̸� ����Ѵ�.
	virtual void				GetScreenPOS ( D3DVECTOR &PosSCR);

	bool						LinkEffectToPOINT( CEffect *pEffect, short nPartIDX, short nPointIDX);
	bool						LinkEffectToDUMMY( CEffect *pEffect, short nDummyIDX );

	void						Add_EFFECT(short nPartIDX, short nPointIDX, t_HASHKEY HashEffectFILE );
	void						Del_EFFECT(short nPartIDX);

	
	void						Warp_POSITION (float fX, float fY);

		
	void						Dead ();
	bool						ProcessSkillHit( CObjCHAR *pFromOBJ );
	bool						Hitted (CObjCHAR *pFromOBJ, int iEffectIDX, int iSkillIDX = 0, bool bIsSkillEffect = false, bool bIsBulletHit = false, bool bJustEffect = false );
	
	
	void						Set_UserMOITON ( t_HASHKEY HashMOTION, float fMoveSpeed, int iRepeatCnt = 0 );
	void						Set_UserMOITON (tagMOTION *pMotion, float fMoveSpeed, int iRepeatCnt = 0 );


	void						SetCollisionGroundNode ( HNODE hNodeGroundIn ) { m_hNodeGround = hNodeGroundIn; }
	HNODE						GetCollisionGroundNode () { return m_hNodeGround; } // �ٴڿ� ��ġ�� ������Ʈ�� ���� �ڵ��� ������. �����̳�, �� ���� ���ܵȴ�. ������ 0.

public:
	tagDAMAGE					m_DeadDAMAGE;
	long						m_lDeadTIME;
	void						Apply_DAMAGE (CObjCHAR *pAtkOBJ, WORD wDamage);


	//----------------------------------------------------------------------------------------------------
	/// @brief Ư�� �׼� �߰��� ��� ��ü������( Ư�� ���� ) �ٲ� ��� �´� ������� ��ü
	//----------------------------------------------------------------------------------------------------
	void						UpdateMotionByChangingEquip();


	//----------------------------------------------------------------------------------------------------
	/// @brief ���� ���� ����� ����߻�
	//----------------------------------------------------------------------------------------------------
	void						FireBulletWithPetWeapon( CObjCHAR* pTarget );


private:
	int							m_iLastCastingSkill;
public:
	void						SetLastCastingSkill( int iSkillIDX ){ m_iLastCastingSkill = iSkillIDX; }
	int							GetLastCastingSkill(){ return m_iLastCastingSkill; }



	//----------------------------------------------------------------------------------------------------
	/// @brief Ŭ���Ҽ� �ִ� ��ü�ΰ�?
	//----------------------------------------------------------------------------------------------------
	virtual bool				CanClickable(){ return true; }



private:
	WORD					m_dwClanID;
	WORD					m_wClanMarkBack;
	WORD					m_wClanMarkCenter;
	int						m_iClanLevel;
	std::string				m_strClanName;
	BYTE					m_btClanPos;

	CClanMarkUserDefined*	m_ClanMarkUserDefined;
public:
	void					SetClan( DWORD dwClanID, WORD wMarkBack, WORD wMarkCenter, const char* pszName , int iLevel , BYTE btClanPos );
	DWORD					GetClanID();
	WORD					GetClanMarkBack();
	WORD					GetClanMarkCenter();
	const char*				GetClanName();
	BYTE					GetClanPos();
	void					SetClanPos( BYTE btPos );
	void					ResetClan();
	void					SetClanLevel( int iLevel );
	int						GetClanLevel();
	void					SetClanMark( WORD wMarkBack, WORD wMarkCenter );

	CClanMarkUserDefined*	GetUserDefinedClanMark(){ return m_ClanMarkUserDefined; }
	void					SetUserDefinedClanMark( CClanMarkUserDefined* pUserDefinedClanMark );//{ m_ClanMarkUserDefined = pUserDefinedClanMark; }

	/// �������� HP/MP ���̸� �����ϱ� ���� ����/�Լ�
protected:
	int						m_ReviseHP;
	int						m_ReviseMP;
	int						m_AruaAddMoveSpeed;		/// �Ʒ�� ���Ż��� �ϰ�� �����Ǵ� �̵��ӵ�

public:
	void					SetReviseHP( int hp );
	void					SetReviseMP( int mp );

	
	void			SHOW()
	{	::setVisibilityRecursive(GetZMODEL(),TRUE); 	}
	void			HIDE()
	{	::setVisibilityRecursive(GetZMODEL(),FALSE); 	}
	
//-----------------------------------------------------------------------------------
	//������
	 bool            m_bDisguise;


//------------------------------------------------------------------------------------	
	//����ȣ
	//īƮ ������...
	CObjCART*			m_pObjCART;					//īƮ Ŭ���� 
	CObjCHAR*			m_pRideUser;				//2�ν� �÷��̾� 

	int					m_iPetType;
	short				m_skCartIDX;				//īƮ ��ų ����Ʈ (���� ��빫)
	WORD				m_iRideIDX;					//2�ν� ���� �ε��� 
	
	float				m_saveSpeed;				//����Ʈ ���� ���� 

	BOOL				m_bUseCartSkill;				
	BOOL				m_IsRideUser;				//2�ν� ž������ 

	//�Ʒξ� ���Ż��� 
	BOOL				m_IsAroa;					//�Ʒξ� ���� ���� 
	BOOL				m_IsCartVA;					//īƮ ���̺극�̼� (����)

	void ChangeHittedEffect(CObjCHAR* pObj,BOOL bA,int& hitIDX);
	void SetNewCommandAfterSkill_PET(int iSkillNO);
	void Stop_Cart(void);
	void Ride_Cansel_Motion(void);

	void ReleaseRideUser(void);
	void ReleaseRideUser(WORD irideIDX);
	void Set_SitMode(void);		

	int  ProcCMD_Skill2OBJECT_PET();
	

	BOOL SetRideUser(WORD irideIDX=0);


	short&		GetCartSKIDX(void)	
	{	return	m_skCartIDX;							}
	
	BOOL&		GetUseCartSkill(void)	
	{	return	m_bUseCartSkill;						}

	//���� ����� īƮ�� ž�� �ߴ��� üũ(TRUE: ž��, FALSE: �ƴ�)
	BOOL&		IsRideUser(void)
	{	return m_IsRideUser;							}

	//���� īƮ�� ������ ž�� ���״��� üũ(0: �ƴ�, ����: ž�� ��Ŵ)
	WORD		GetRideUserIndex(void)
	{	return m_iRideIDX;								}

	//�Ʒξ� ���� ���¼��� �Լ� 
	BOOL&			SetAroaState(void)
	{	return m_IsAroa;								}
	
	BOOL&			SetCartVA(void)
	{	return m_IsCartVA;								}

	float& SaveSpeed(void)
	{	return m_saveSpeed;								}
//------------------------------------------------------------------------------------

	/// ������ : �Ʒ�� ���� �����ϰ���� �߰� �ɷ�ġ ��� �Լ�
	virtual		void Calc_AruaAddAbility();
} ;




///
///	MOB class inherited from COjbCHAR
///

class CObjMOB : public CObjCHAR
{
private:
	short						m_nQuestIDX;
	int							m_iMobAniSkill;

public :
	static DWORD				m_dwCreateOrder;
	
	CObjMOB();
	virtual ~CObjMOB ();



	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CGameObj virtual functions
	/// <

	/*override*/virtual int		Get_TYPE()							{	return OBJ_MOB;					}
	/*override*/char*			Get_NAME ()							{	return (char*)NPC_NAME( m_nCharIdx );	}

	/// <
	/// < End
	//////////////////////////////////////////////////////////////////////////////////////////
	
	
	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CAI_OBJ virtual functions
	/// <

	/*override*/virtual void		Say_MESSAGE (char *szMessage);
	/*override*/virtual bool		Change_CHAR (int nCharIDX);
	/*override*/virtual bool		Create_PET  (int nCharIDX);
	/*override*/virtual void		Run_AWAY (int iDistance);

	/*override*/virtual int			Get_LEVEL ()						{	return NPC_LEVEL( m_nCharIdx );				}
	/*override*/virtual int			Get_ATK ()							{	return NPC_ATK  ( m_nCharIdx );				}
	/*override*/virtual int			Get_DEF ()							{	return NPC_DEF	( m_nCharIdx );				}
	/*override*/virtual int			Get_RES ()							{	return NPC_RES	( m_nCharIdx );				}
	/*override*/virtual int			Get_HIT ()							{	return NPC_HIT	( m_nCharIdx );				}	
	/*override*/virtual int			Get_CHARM ()						{	return 0;		}
	/*override*/virtual int			Get_AVOID ()						{	return NPC_AVOID( m_nCharIdx );				}
	/*override*/virtual int			Get_SENSE ()						{	return Get_LEVEL();							}	// LEVEL�� ��ü !
	/*override*/virtual int			Get_GiveEXP ()						{	return NPC_GIVE_EXP( m_nCharIdx );			}
	/*override*/virtual void		Add_EXP (short nExp)				{	;	/* nop */	}
	/*override*/virtual int			Get_CRITICAL()						{	return (int)(this->Get_LEVEL() * 0.8 );		}
	
	/// <
	/// < End
	//////////////////////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CObjAI virtual functions
	/// <

	/*override*/int				Def_AttackRange()					{	return NPC_ATK_RANGE( m_nCharIdx );			}

	/*override*/int				GetANI_Stop()						{	return MOB_ANI_STOP;	}
	/*override*/int				GetANI_Move()						{	return ( m_bRunMODE ) ? MOB_ANI_RUN : MOB_ANI_MOVE;	}
	/*override*/int				GetANI_Attack()						{	return MOB_ANI_ATTACK;	}
	/*override*/int				GetANI_Die ()
	{	
		if( m_bStopDead )
			return MOB_ANI_STOP;
		return  MOB_ANI_DIE;				
	}
	/*override*/int				GetANI_Hit()						{	return MOB_ANI_HIT;		}
	/*override*/int				GetANI_Casting()					{   return m_iMobAniSkill;	}		/// ĳ���õ���
	/*override*/int				GetANI_CastingRepeat()				{   return m_iMobAniSkill;	}		/// ���������� ĳ���õ������� ���..
	/*override*/int				GetANI_Skill()						{	return m_iMobAniSkill + 1;	}

	/*override*/int				Get_MP ()							{	return 32767;			}
	/*override*/int				Get_R_WEAPON ()						{	return NPC_R_WEAPON( m_nCharIdx );			}
	/*override*///int				Get_KNAPSACK ()						{	return	0;								}//Numenor: could be fun to implement this one day
	/*override*/int				Get_L_WEAPON ()						{	return 0;				}
	/*override*/ virtual void	RecoverHPnMP( short nRecoverMODE )	{};	


	/// �ִ� �����	
	void						Set_MaxHP( int iMaxHP )				{ m_iMaxHP = iMaxHP; }
	/*override*/int				Get_MaxHP()							{	return m_iMaxHP;			}
	/// �ִ� ����
	/*override*/int				Get_MaxMP()							{	return 100;								}


	/*override*/int				Get_AttackRange ();


	/*override*/void			Do_StopAI (void)					{	g_AI_LIST.AI_Stop( NPC_AI_TYPE(m_nCharIdx), this );							}
	/*override*/void			Do_AttackMoveAI (CObjCHAR *pTarget)	{	g_AI_LIST.AI_AttackMove( NPC_AI_TYPE(m_nCharIdx), this, pTarget );			}
	/*override*/void			Do_DamagedAI (CObjCHAR *pTarget, int iDamage )	
	{	
		if ( !( CMD_BIT_INT & this->Get_COMMAND() ) )
			g_AI_LIST.AI_Damaged( NPC_AI_TYPE(m_nCharIdx), this, pTarget, iDamage );
	}
	/*override*/void			Do_KillAI (CObjCHAR *pTarget, int iDamage)		{	g_AI_LIST.AI_Kill( NPC_AI_TYPE( m_nCharIdx ), this, pTarget, iDamage );		}
	/*override*/void			Do_DeadAI (CObjCHAR *pTarget, int iDamage)		{	g_AI_LIST.AI_Dead( NPC_AI_TYPE( m_nCharIdx ), this, pTarget, iDamage );		}

	/*override*/virtual void	Do_DeadEvent	(CObjCHAR *pAtk);	



	/*override*/virtual float			Get_DefaultSPEED ()		
				{	
					if ( !m_bRunMODE ) return GetOri_WalkSPEED();
					
					short nR = ( GetOri_RunSPEED() + m_EndurancePack.GetStateValue( ING_INC_MOV_SPD ) 
												- m_EndurancePack.GetStateValue( ING_DEC_MOV_SPD ) );
					return nR;
				}

	/*override*/ virtual short			Get_nAttackSPEED ()
				{	
					int iR = GetOri_ATKSPEED() + m_EndurancePack.GetStateValue( ING_INC_ATK_SPD )
												- m_EndurancePack.GetStateValue( ING_DEC_ATK_SPD );
					return iR;
				}
	/*override*/ virtual float			Get_fAttackSPEED ()
				{	
					int iR = GetOri_ATKSPEED() + m_EndurancePack.GetStateValue( ING_INC_ATK_SPD )
												- m_EndurancePack.GetStateValue( ING_DEC_ATK_SPD );
					return iR/100.0f;
				}


	/// <
	/// < End
	//////////////////////////////////////////////////////////////////////////////////////////




	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CObjCHAR virtual functions
	/// <

	/*override*/bool			Check_EVENT (CGameOBJ *pSourOBJ, short nEventIDX);

	/*override*/ bool			IsFemale ()			{	return false;	}
	/*override*/ short			IsMagicDAMAGE()		{	return NPC_IS_MAGIC_DAMAGE( m_nCharIdx );	}

	/*override*/tagMOTION *		Get_MOTION (short nActionIdx=MOB_ANI_STOP)	{	return m_pCharMODEL->GetMOTION( nActionIdx );	}

	

	/*override*/ short			GetOri_WalkSPEED()					{	return NPC_WALK_SPEED( m_nCharIdx );		}
	/*override*/ short			GetOri_RunSPEED()					{	return NPC_RUN_SPEED( m_nCharIdx );			}
	/*override*/ short			GetOri_ATKSPEED ()					{	return NPC_ATK_SPEED( m_nCharIdx );			}


	/// �������� �����ġ ������ ���ؼ� ���� ����Ǿ��ִ� �ɷ¼�ġ( �нú� ��ų ���� )
	virtual int					Get_DefaultAbilityValue( int iType );

	/// NPC�� ��� STB�� ������ ���̰� �� ������ �� ���̸� ����Ѵ�.
	virtual void				GetScreenPOS ( D3DVECTOR &PosSCR);

	/// <
	/// < End
	//////////////////////////////////////////////////////////////////////////////////////////



	bool	Create (short nCharIdx, const D3DVECTOR& Position, short nQuestIDX, bool bRunMODE);

	void	SetMobAniSkill( int iMobAniSkill = MOB_ANI_CASTION01 ){ m_iMobAniSkill = iMobAniSkill; }


	//------------------------------------------------------------------------------------------------
	/// ���� ������ ���� ���..
	//------------------------------------------------------------------------------------------------
	void	PlayStopSound();


	virtual int	Proc();
	
} ;



///
///	Npc class inherited from COjbMOB
///

class CObjNPC : public CObjMOB
{
private:
	int				m_iEventSTATUS;

public :

	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CGameObj virtual functions
	/// <

	CObjNPC();

	virtual int		Get_TYPE()							{	return OBJ_NPC;	}
	virtual int		Proc();
	/// <
	/// < end	
	//////////////////////////////////////////////////////////////////////////////////////////	


	void			SetEventValue( int iEventValue );
	int				GetEventValue();


	//-------------------------------------------------------------------------------------------------
	// 05.05.19 icarus:: �Ϳ� ��Ÿ���� NPC ����Ʈ ���� �۾�...
public :
	// �������� npc �߰� ��Ŷ�� �ް� ������ �⺻�� -1, 
	short			m_nQuestSignal;
//	int				Proc (void);

} ;



class CObjCART;
///
///	Avata class
///

class CObjAVT : public CObjCHAR
{
protected:
	CCharMODEL		m_CharMODEL;

	/// ĳ���� ǥ��
	int				m_iExpression;

	/// ĳ���� ����
	union {
		tagPartITEM	m_sPartItemIDX[ MAX_BODY_PART ];
		// short	m_nPartItemIDX[ MAX_BODY_PART ];
		struct {
			// ������ t_CharPART�� ����...
			tagPartITEM m_sFaceIDX;			//short m_nFaceIDX;
			tagPartITEM m_sHairIDX;			//short m_nHairIDX;
			tagPartITEM m_sHelmetIDX;		//short m_nHelmetIDX;
			
			tagPartITEM m_sArmorIDX;		//short m_nArmorIDX;
			tagPartITEM m_sGauntletIDX;		//short m_nGauntletIDX;
			tagPartITEM m_sBootsIDX;		//short m_nBootsIDX;
			
			tagPartITEM m_sGoggleIDX;		//short m_nGoggleIDX;
			tagPartITEM m_sKnapsackIDX;		//short m_nKnapsackIDX;
			
			tagPartITEM m_sRWeaponIDX;		//short m_nRWeaponIDX;
			tagPartITEM m_sLWeaponIDX;		//short m_nLWeaponIDX;
		} ;
	} ;

		
protected:
	std::string				m_Name;
	int						m_iLevel;


	DWORD					m_dwSitTIME;
	short					m_nPsvAtkSPEED;

	short					m_nRunSPEED;
	short					m_nAtkAniSPEED;


	/// ��ȯ�� ���Ⱑ ���õǸ�.. ����� �����Ŀ� �ٲ۴�..
	int						m_iDoChangeWeaponR;
	int						m_iDoChangeWeaponL;
	bool					m_bUpdateMotion;	

	/// ���� ������� �Ѿ� ������..
	tagShotDATA				m_ShotData[MAX_SHOT_TYPE];
	int						m_iCon;
	int						m_iRecoverHP;
	int						m_iRecoverMP;

	///������ ���� ���Ժ���: 
	BYTE					m_btWeightRate;

	/// ���׹̳�			
	short					m_nStamina;

	

public:
	CObjAVT ();
	virtual ~CObjAVT ();


	void					SetShotData( int i, int iItemNo );

	
	void					Set_CON( int iCon ){	m_iCon = iCon; }
	void					Set_AddRecoverHP( int iRecoverHP ){ m_iRecoverHP = iRecoverHP; }
	void					Set_AddRecoverMP( int iRecoverMP ){ m_iRecoverMP = iRecoverMP; }

public :
	
	/// ĳ���� ǥ��
	void					SetCharExpression( int iFaceNo ){ m_iExpression = iFaceNo; }
	int						GetCharExpression(){ return m_iExpression; }

	/// ��ȯ�� ���Ⱑ ���õǸ�.. ����� �����Ŀ� �ٲ۴�..
	void					SetChangeWeaponR( int iWeaponNO ){ m_iDoChangeWeaponR = iWeaponNO;}
	void					SetChangeWeaponL( int iWeaponNO ){ m_iDoChangeWeaponL = iWeaponNO;}
	int						GetChangeWeaponR( ){ return m_iDoChangeWeaponR;}
	int						GetChangeWeaponL( ){ return m_iDoChangeWeaponL;}


	/// ��� ��ü�� ��� �ɷ��ִ� �Ӽ� ����..
	void					ClearRWeaponSkillEffect();
	void					ClearLWeaponSkillEffect();


	///
	/// ������ ������ ������ ���� �� �÷����� ���¿����� ��� ����( STOP, MOVE... )
	///
	void						SetUpdateMotionFlag( bool bUpdate ){ m_bUpdateMotion = bUpdate; }
	bool						GetUpdateMotionFlag(){ return m_bUpdateMotion; } 






	/// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	/// > virtual function of CObjAVT
	/// >

	virtual void				Update_SPEED ()			
	{	
		m_nAtkAniSPEED = Cal_AtkAniSPEED( this->GetPartITEM(BODY_PART_WEAPON_R) );
		m_fRunAniSPEED = Cal_RunAniSPEED( m_nRunSPEED );
	}
	/// Virtual function
	virtual void				Update (bool bUpdateBONE=false);

	virtual void				Set_SEX ( BYTE btRace )		
	{	
		m_btRace = btRace;
		m_bIsFemale = btRace & 0x01;
		m_CharMODEL.Set_SEX( m_bIsFemale );	
	}

	/// >
	/// > end	
	/// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>







	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CGameOBJ virtual functions
	/// <

	/*override*/virtual int	Get_TYPE()					{	return OBJ_AVATAR;				}
	char*					Get_NAME ()					{	return (char*)m_Name.c_str();				}

	/// <
	/// < end	
	//////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CAI_OBJ virtual functions
	/// <

	/*override*/virtual bool	Is_SameTYPE(int ObjTYPE)	{	return ( ObjTYPE==OBJ_AVATAR ) || ( ObjTYPE==OBJ_USER );	}

	/*override*/virtual int		Get_LEVEL ()						{	return m_iLevel;}
	/*override*/virtual int		Get_ATK ()							{	return 1;		}
	/*override*/virtual int		Get_DEF ()							{	return 1;		}
	/*override*/virtual int		Get_RES ()							{	return 1;		}		
	/*override*/virtual int		Get_HIT ()							{	return 1;		}
	/*override*/virtual int		Get_CHARM ()						{	return 1;		}
	/*override*/virtual int		Get_AVOID ()						{	return 1;		}
	/*override*/virtual int		Get_SENSE ()						{	return 1;		}			
	/*override*/virtual int		Get_GiveEXP ()						{	return 1;		}
	/*override*/virtual void	Add_EXP (short nExp)				{	;	/* nop */	}
	/*override*/virtual int		Get_CRITICAL()						{	return 1;		}
	/// <
	/// < end	
	//////////////////////////////////////////////////////////////////////////////////////////




	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CObjAI virtual functions
	/// <

	/*override*/virtual int		Get_CON()				{	return m_iCon;	}	// ������ ��� return pAVATAR->GetCur_CON();
	/*override*/virtual int		GetAdd_RecoverHP()		{	return m_iRecoverHP;	}	// ������ ��� return pAVATAR->m_btRecoverHP;
	/*override*/virtual int		GetAdd_RecoverMP()		{	return m_iRecoverMP;	}	// ������ ��� return pAVATAR->m_btRecoverHP;

	/*override*/virtual short	GetPsv_ATKSPEED(float fCurSpeed, short nRightWeaponItemNo)		{	return m_nPsvAtkSPEED;				}
	/*override*/int				Def_AttackRange()		{	return WEAPON_ATTACK_RANGE( 0 );							}


	/*override*/int				GetANI_Stop();				
	/*override*/int				GetANI_Move();
	/*override*/int				GetANI_Attack();
	/*override*/int				GetANI_Die();			
	/*override*/int				GetANI_Hit();
	/*override*/int				GetANI_Casting ();
	/*override*/int				GetANI_CastingRepeat();
	/*override*/int				GetANI_Skill();
	/*override*/int				GetANI_Sitting();
	/*override*/int				GetANI_Standing();
	/*override*/int				GetANI_Sit();
	/*override*/int				GetANI_Ride();
	/*override*/int				GetANI_PickITEM();


//	/*override*/int				Get_R_WEAPON ()			{	return this->m_nRWeaponIDX;				}
//	/*override*/int				Get_L_WEAPON ()			{	return this->m_nLWeaponIDX;				}
	/*override*/int				Get_R_WEAPON ()			{	return this->m_sRWeaponIDX.m_nItemNo;	}
	/*override*/int				Get_L_WEAPON ()			{	return this->m_sLWeaponIDX.m_nItemNo;	}
	/*override*/int				Get_KNAPSACK ()			{	return this->m_sKnapsackIDX.m_nItemNo;	}
	
	/*override*/int				Get_MaxHP()				{ return m_iMaxHP + m_EndurancePack.GetStateValue( ING_INC_MAX_HP ); }	
	/*override*/int				Get_MaxMP()				{ return m_iMaxMP + m_EndurancePack.GetStateValue( ING_INC_MAX_MP ); }

	/*override*/bool			ToggleRunMODE ( float fAdjRate );
	/*override*/bool			ToggleSitMODE ();


	///
	/// Check PET MODE
	/// if mode < 0, this char is normal mode
	/*override*/ virtual int	GetPetMode();
	/*override*/ virtual bool	CanAttackPetMode();
	/*override*/ virtual void	SetCMD_PET_MOVE( const D3DVECTOR& PosTO, BYTE btRunMODE );	
	/*override*/ virtual void	SetCMD_PET_MOVE( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget);
	/*override*/ virtual void	SetCMD_PET_STOP( void );
	/*override*/ virtual void	SetCMD_PET_ATTACK (int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosTO);

	/*override*/ virtual void	RideCartToggle( bool bRide );		

	/// <
	/// < end	
	//////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////	
	/// < Inherited from CObjCHAR virtual functions
	/// <

private:
	/*override*/HNODE			Get_SKELETON ()		{	return g_DATA.Get_SKELETON( this->m_bIsFemale );	}

public:
	/*override*/bool			IsFemale ()				{	return m_bIsFemale;					}
	/*override*/short			IsMagicDAMAGE()			{	return WEAPON_IS_MAGIC_DAMAGE( Get_R_WEAPON() );			}

	/*override*/tagMOTION*		Get_MOTION (short nActionIdx=AVT_ANI_STOP1);




	/*override*/short			GetOri_ATKSPEED()					{	return m_nAtkAniSPEED;	}
	/*override*/short			GetOri_WalkSPEED()					{	return WALK_CmPerSec;			}
	/*override*/short			GetOri_RunSPEED()					{	return m_nRunSPEED;				}
	virtual		int				GetOri_MaxHP();

	/*override*/ virtual int	Get_BulletNO ();

	
	/// �������� �����ġ ������ ���ؼ� ���� ����Ǿ��ִ� �ɷ¼�ġ( �нú� ��ų ���� )
	/*override*/ virtual int	Get_DefaultAbilityValue( int iType );

	///
	/// ���� ���������� ��ũ�� �Ǿ��°�?
	///
	/*override*/ virtual bool	IsChild(){ return ( ( m_pObjCART == NULL )? false:true ); }

	//----------------------------------------------------------------------------------------------------
	/// CObjCHAR �� �ϴ��ϸ��� �߰������� �� ������ �ִ�.
	//----------------------------------------------------------------------------------------------------
	/*override*/int							Proc ();
		
	/// <
	/// < end	
	//////////////////////////////////////////////////////////////////////////////////////////



	
	void						SetOri_RunSPEED(short nRunSpeed)	{	m_nRunSPEED = nRunSpeed;		}
	void						SetPsv_AtkSPEED(short nPsvAtkSpeed)	{	m_nPsvAtkSPEED = nPsvAtkSpeed;	}
	
	
	/// �ִ� �����	
	void						Set_MaxHP( int iMaxHP )			{ m_iMaxHP = iMaxHP; }

	
	

	

	short						GetPartITEM(short nPartIdx)						{	return m_sPartItemIDX[ nPartIdx ].m_nItemNo;	}
	void						SetPartITEM(short nPartIdx, tagPartITEM &sITEM)	{	m_sPartItemIDX[ nPartIdx ] = sITEM;				}
	void						SetPartITEM(short nPartIdx, tagITEM &sITEM)		
	{	
		tagPartITEM sPartITEM;
		sPartITEM.m_nItemNo = sITEM.GetItemNO();
		sPartITEM.m_bHasSocket = sITEM.m_bHasSocket;
		sPartITEM.m_cGrade = sITEM.GetGrade();
		sPartITEM.m_nGEM_OP = sITEM.GetGemNO();
		m_sPartItemIDX[ nPartIdx ] = sPartITEM;
	}
	void						SetPartITEM(short nPartIdx, short nItemNo);
	void						SetAvataName (char *szName)						{	m_Name = szName;							}
	void						SetAllPARTS (tagPartITEM *pPartItemIDX)			{	::CopyMemory( m_sPartItemIDX, pPartItemIDX, sizeof(tagPartITEM)*MAX_BODY_PART);			}
	void						SetAllPetPARTS (tagPartITEM *pPetPartItemIDX)	{	::CopyMemory( m_sPetPartItemIDX, pPetPartItemIDX, sizeof(tagPartITEM)*MAX_RIDING_PART);	}

	void						SetAvtLevel( int iLevel ){ m_iLevel = iLevel; }

	bool						Create (const D3DVECTOR& Position, BYTE btCharRACE);	


	//----------------------------------------------------------------------------------------------------		
	/// @brief ���, ��ð��� ����Ʈ..
	//----------------------------------------------------------------------------------------------------
	void						CreateGemmingEffect();
	void						DeleteGemmingEffect();


//--------------------------------------------------------------------------------------------------------
	//2005. 06.15 �� ��ȣ 
	void						Set_RareITEM_Glow(void);

	void						CreateGradeEffect();
	void						DeleteGreadEffect();
	void						Process_JOIN_RIDEUSER(void);
//--------------------------------------------------------------------------------------------------------



	//----------------------------------------------------------------------------------------------------	
	/// ���� ��ǥ(m_PosCUR.x, m_PosCUR.y)�� ���̸� ����
	/// @pReachable ���� ������ ��ġ������ ����. ���� �������� ��ġ�κ��� ���ް������� �ʴٸ�, false ���� ���õ�.
	/// @return ���̰�. ���� �Ұ����� ��ġ���, ���� ���� ������Ʈ�� ��ġ�� ����.
	//----------------------------------------------------------------------------------------------------
	float						GetHeight (bool * pReachable = NULL);



	//////////////////////////////////////////////////////////////////////////////////////////
	///
	/// ���λ��� ���� ������
	///
	//////////////////////////////////////////////////////////////////////////////////////////
private:
	bool						m_bPersonalStoreMode;
	std::string					m_strPersonalStoreTitle;

public:
	void						SetPersonalStoreTitle( char* strTitle, int iPersonalStoreType = 0 );
	const char*					GetPersonalStoreTitle();
	bool						IsPersonalStoreMode();

	CObjFIXED*					m_pObjPersonalStore;
	


	//////////////////////////////////////////////////////////////////////////////////////////
	///
	/// �� �ý��� ���� ������..
	///
	//////////////////////////////////////////////////////////////////////////////////////////

public:

	friend CObjCART;
	/// �� ����..
	/// ����.. CObjPET ���ο� �� �����͵��� �����Ѵ�.. ���⼭�ϴ� ���Ǹ� ���� �������ش�.



//����ȣ::īƮ���� ���� 
#if defined(_GBC)
	union 
	{
		//short				m_nPetPartItemIDX[ MAX_RIDING_PART ];
		tagPartITEM		m_sPetPartItemIDX[ MAX_RIDING_PART ];
		struct 
		{
			// ������ t_CharPART�� ����...
			tagPartITEM	m_sEngineIDX;	// short			m_nEngineIDX;
			tagPartITEM	m_sBodyIDX;		// short			m_nBodyIDX;
			tagPartITEM	m_sLegIDX;		// short			m_nLegIDX;
			tagPartITEM	m_sAbilIDX;		// short			m_nAbilIDX;	
			tagPartITEM	m_sWeaponIDX;	// short			m_nWeaponIDX;	
		} ;
	} ;
#else 
	union 
	{
		//short				m_nPetPartItemIDX[ MAX_RIDING_PART ];
		tagPartITEM		m_sPetPartItemIDX[ MAX_RIDING_PART ];
		struct 
		{
			// ������ t_CharPART�� ����...
			tagPartITEM	m_sEngineIDX;	// short			m_nEngineIDX;
			tagPartITEM	m_sBodyIDX;		// short			m_nBodyIDX;
			tagPartITEM	m_sLegIDX;		// short			m_nLegIDX;
			tagPartITEM	m_sAbilIDX;		// short			m_nAbilIDX;	
		} ;
	} ;
#endif 




//	int						m_iPetType;
//	CObjCART*				m_pObjCART;

	CEffect*				m_pWeaponJemEffect;

public:		
	void					SetPetType( int iPetType ){ m_iPetType = iPetType; }
	WORD					GetPetState();

	bool					CreateCartFromMyData( bool bShowEffect = false );
	bool					CreateCart( unsigned int iPetType, int iEnginePart, int iBodyPart, int iLegPart, int iAbilIPart,int iWeaponPart=0 );
	void					DeleteCart( bool bShowEffect = false );

	void					SetPetParts( unsigned int iPetPartIDX, unsigned int iItemIDX, bool bJustInfo = true );
	void					UpdatePet();
	
	void					SetWeightRate( BYTE btWeightRate );
	BYTE					GetWeightRate();
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	//----------------------------------------------------------------------------------------------------	
	/// @brief �ָ�� �����϶��� ���� ��ǥ�� �����ϰ�, �Ϲ����϶��� �� ��ǥ�� ������
	//----------------------------------------------------------------------------------------------------
	const D3DVECTOR&		GetWorldPos();



	//----------------------------------------------------------------------------------------------------	
	/// @brief �𵨿� ��ϵ� renderUnit���ǰ͵鿡 ���� �浹����.. (�ƹ�Ÿ�ϰ�츸)
	//----------------------------------------------------------------------------------------------------
	bool					IsIntersectAccessory( float &fCurDistance );


	virtual short			GetStamina();
	void					SetStamina( short nStamina );


	//--------------------------------------------------------------------------------------
	/// �ƹ�Ÿ�� ������� ����
	//--------------------------------------------------------------------------------------
private:
	DWORD					m_dwSubFLAG;

public:
	DWORD					GetSpecialStateFLAG(){ return m_dwSubFLAG; }
	void					ChangeSpecialState( DWORD dwSubFLAG );

	//----------------------------------------------------------------------------------------------------
	/// @brief Ŭ���Ҽ� �ִ� ��ü�ΰ�?
	//----------------------------------------------------------------------------------------------------
	virtual bool			CanClickable();
	
} ;

//-------------------------------------------------------------------------------------------------





///////////////////////////////////////////////////////////////////////////////////////////	
// 2005. 6. 20	�� ��ȣ 
//
// ���ż�ȯ ���� Ŭ���� 
///////////////////////////////////////////////////////////////////////////////////////////	

using namespace std;

typedef enum 
{	
	GOD_NONE=0,GOD_APPEAR_PARTCLE, GOD_APPEAR_GODDESS, 
	GOD_SPRITE_EFFECT,GOD_END_EFFECT, MAX_GOD_EFFECT 

} GOD_STATE;



typedef struct _tagGODDESSSTR 
{

	BOOL		bEnable;		//GOD SetUp ���� 
	GOD_STATE	god_State;	 	//GOD �ൿ����
								
	HNODE		hGoddess;
	HNODE		hSprite;

	WORD		idxMaster;		//����� ĳ���� ������Ʈ 
	DWORD		sTick,eTick;	//��¿� ���� ƽ
	D3DXVECTOR3	cpos;			//��µ� ��ġ 

	CObjMOB		*pGODModel;
	CObjMOB		*pSprModel;
	
	CEffect*	pEffect[2];
	HNODE		hParticle[2];

	float		frame;			//������

	float		fviewGODD;		//������	
	float		fviewSpr;		
	

	void Init(void)
	{
		bEnable		= FALSE;
		god_State	= GOD_NONE; 
		
		pGODModel	= NULL;
		pSprModel	= NULL;
	
		hGoddess	= NULL;
		hSprite		= NULL;
		
		for(register int i=0;i<2;i++)
		{
			pEffect[i]		= NULL;
			hParticle[i]	= NULL;
		}

		idxMaster	= 0; 
		frame		= 0;
		fviewGODD	= 0;
		fviewSpr	= 0;

		cpos		= D3DXVECTOR3(0,0,0);

		sTick = eTick = 0;
		
	}
	
}GODDESSSTR,*LPGODDESSSTR;





typedef map<WORD,GODDESSSTR>::iterator GODLIST;
class CGoddessMgr
{

	BOOL		m_IsUse;
	DWORD		m_Count;			
	map<WORD,GODDESSSTR>	m_list;

	DWORD		m_dwAvata;				//�ƹ�Ÿ �ڽ��� On/Off
	BOOL		m_IsAvata;

public:
	CGoddessMgr();
	~CGoddessMgr();

	BOOL  Init(void);
	void  Release(void); 
	void  Release_List(GODDESSSTR* pGDS);
	BOOL  Permission_Country(void);

	BOOL Register_God_And_Sprite(void);
	BOOL Register_RenderObj(GODDESSSTR& gds);
	BOOL SetProcess(DWORD bonOff,WORD idx,BOOL bPrograss=FALSE);
	
	void Set_GDSData(GODDESSSTR& gds,BOOL bonOff,BOOL bPrograss);
	void Update(void);
	int  ProcessVisible(float& fv,float fseq);

	BOOL& IsAvataAroa(void)  	{ return m_IsAvata;	}
	BOOL& IsGetUse(void)		{ return m_IsUse;		}
	DWORD IsAvataState(void)	{ return m_dwAvata;	} 


};


extern CGoddessMgr goddessMgr;


#endif

