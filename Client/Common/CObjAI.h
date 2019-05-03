/*
	$Header: /Client/Common/CObjAI.h 132   05-10-27 12:27p Gioend $
*/
#ifndef	__CCOMMAND_H
#define	__CCOMMAND_H
#include "CGameOBJ.h"
#include "IO_Motion.h"
#include "IO_Skill.h"
#include "..\AI_LIB\CAI_LIB.h"


//-------------------------------------------------------------------------------------------------
//
// Character state
//
#define	CS_BIT_INT		0x1000					// ������ ���ͷ�Ʈ �ɸ��� �ȵǴ°�..
#define CS_BIT_ADJ		0x2000					// ������ ��ǥ �����Ұ�.
#define	CS_BIT_CHK		0x4000					// ���� ������ üũ..
#define CS_BIT_ONE	  (	0x8000 | CS_BIT_CHK )	// ������ 1�� ���� ���� ������.
#define CS_BIT_MOV		0x0100					// �̵����̴�.
#define	CS_BIT_INT2	  (	0x0200 | CS_BIT_INT )	// ��Ʈ ���������� ����ȵȴ�..

#define	CS_BIT_ATTACK	0x0002

#define	CS_STOP		  (	0x0000									  )
#define	CS_MOVE		  (	0x0001			| CS_BIT_ADJ | CS_BIT_MOV )
#define	CS_ATTACK	  (	CS_BIT_ATTACK	| CS_BIT_INT | CS_BIT_ONE )
#define CS_HIT		  ( 0x0003			| CS_BIT_INT | CS_BIT_ONE )
#define CS_FALL		  ( 0x0004			| CS_BIT_INT | CS_BIT_ONE )
#define CS_RAISE	  ( 0x0005			| CS_BIT_INT | CS_BIT_ONE )
#define CS_SITTING	  ( 0x0005			| CS_BIT_INT | CS_BIT_ONE )
#define CS_SIT		  ( 0x0006			| CS_BIT_INT			  )		// ���� �Ǵ� ����
#define CS_STANDING	  ( 0x0007			| CS_BIT_INT | CS_BIT_ONE )
#define CS_NEXT_STOP  ( 0x0008			| CS_BIT_INT | CS_BIT_ONE )
#define CS_NEXT_STOP2 ( 0x0009			| CS_BIT_INT2| CS_BIT_ONE )
#define	CS_DIE		  (	0x0010			| CS_BIT_INT | CS_BIT_ONE )
#define CS_CASTING	  ( 0x0011			| CS_BIT_INT2| CS_BIT_ONE )

#define	CMD_BIT_INT		0x8000					// ����� ���ͷ�Ʈ �ɸ��� �ȵǴ°�..

#define	CMD_STOP		0x0000
#define	CMD_MOVE		0x0001
#define	CMD_ATTACK		0x0002
#define	CMD_DIE			0x0003
#define	CMD_PICK_ITEM	0x0004
#define CMD_SKILL2SELF	0x0006
#define	CMD_SKILL2OBJ	0x0007
#define	CMD_SKILL2POS	0x0008
#define	CMD_RUNAWAY	  ( 0x0009			| CMD_BIT_INT )
#define	CMD_SIT			0x000a

/// ����׼ǵ�
enum
{	//Numenor: this has to be the number on column 6 ("range") in the STB
	COMMON_COMMAND_SIT			= 1,
	COMMON_COMMAND_PICK_ITEM	= 2,
	COMMON_COMMAND_JUMP			= 3,
	COMMON_COMMAND_AIR_JUMP		= 4,
	COMMON_COMMAND_AUTO_TARGET	= 5,
	COMMON_COMMAND_ATTACK		= 6,
	COMMON_COMMAND_DRIVE_CART	= 7,
	COMMON_COMMAND_ADD_FRIEND	= 8,
	COMMON_COMMAND_PARTY		= 9,	///��Ƽ�Ἲ, ��Ƽ���ν�û
	COMMON_COMMAND_EXCHANGE		= 10,	///�ŷ���û
	COMMON_COMMAND_PRIVATESTORE = 11,
	COMMON_COMMAND_SELFTARGET   = 12,
	COMMON_COMMAND_BOARD_CART	= 13,	// īƮ ž�� ��ų.
	COMMON_COMMAND_PICK_ITEM_FROM_DISTANCE	= 14,
	COMMON_COMMAND_PARTY_SCALE	= 15, //Numenor: new skill, party with scaling and exp sharing
};


#define	WALK_CmPerSec	200

//-------------------------------------------------------------------------------------------------

class CObjCHAR;
class CObjAI;


///
///	class for object that has target.
///

class CObjTARGET 
{
#ifdef	__SERVER
private :
#else
public  :
#endif
	int						m_iServerTarget;		/// ������ ���� ���� ��ȣ

protected :
	CAI_OBJ *				Get_TargetOBJ ();

public  :
			int				Get_TargetIDX ()						{	return m_iServerTarget;			}
	virtual void			Set_TargetIDX (int iTargetIndex)		{	m_iServerTarget=iTargetIndex;	}

	CObjTARGET ()	{		m_iServerTarget = 0;	}
} ;



#define	RECOVER_JUST_CHECK_TIME			0		/// �ϰ͵� ���ϰ� �ð��� ����.
#define	RECOVER_STATE_CHECK_TIME_OLD	4000	/// 4�ʴ� ȸ��..
#define RECOVER_STATE_CHECK_TIME_NEW	8000	/// 8�ʴ� ȸ��

#define	RECOVER_STATE_SIT_ON_CHAIR		30		/// ���� �ɱ�		:: nRecoverMODE = 30 : ���� ���� ����( 2005/5/20 )

#define	RECOVER_STATE_SIT_ON_GROUND_OLD	20		/// �ٴ� �ɱ�       :: nRecoverMODE = 20
#define	RECOVER_STATE_SIT_ON_GROUND_NEW	25		/// �ٴ� �ɱ�       :: nRecoverMODE = 25//�븸, �ʸ��� ����( 2005/5/19 )

#define RECOVER_STATE_STOP_OR_WALK_OLD	8		/// ����,�ȱ� ����	:: nRecoverMODE = 8
#define RECOVER_STATE_STOP_OR_WALK_NEW	5		/// ����,�ȱ� ����	:: nRecoverMODE = 5//�븸, �ʸ��� ����( 2005/5/19 )
///
///	CObjAI class 
///
class CObjAI : public CGameOBJ, public CAI_OBJ, public CObjTARGET 
{
private	  :
	WORD				m_wState;
	WORD				m_wCommand;
	WORD				m_wBeforeCMD;			/// self skill ���� ���� ��� ����..			



protected :

	//////////////////////////////////////////////////////////////////////////////////////////
	///>
	///> CObjAI virtual functions
	///>

	virtual bool		IsInRANGE (CObjCHAR *pT, int iRange)=0{ *(int*)0 = 10; };
	virtual void		ActionEVENT (int iActionIDX)=0{ *(int*)0 = 10; };
	virtual void		Adjust_HEIGHT () = 0{ *(int*)0 = 10; };
	virtual int			GetCurrentFrame () = 0{ *(int*)0 = 10; };
	virtual void		Set_MOTION (short nActionIdx, float fMoveSpeed=0, float fAniSpeed=1.0f, bool bAttackMotion=false, int iRepeatCnt = 0 )=0{ *(int*)0 = 10; };
	virtual void		Move_COMPLETED ()=0{ *(int*)0 = 10; };

	virtual int			GetANI_Stop()=0{ *(int*)0 = 10; };
	virtual int			GetANI_Die()=0{ *(int*)0 = 10; };
	virtual int			GetANI_Hit()=0{ *(int*)0 = 10; };
	virtual int			GetANI_Move()=0{ *(int*)0 = 10; };
	virtual int			GetANI_Attack()=0{ *(int*)0 = 10; };
	virtual int			GetANI_Casting()=0{ *(int*)0 = 10; };
	virtual int			GetANI_CastingRepeat()=0{ *(int*)0 = 10; };
	virtual int			GetANI_Skill()=0{ *(int*)0 = 10; };
	virtual int			GetANI_Sitting()	{	return GetANI_Stop();	}
	virtual int			GetANI_Standing()	{	return GetANI_Stop();	}
	virtual int			GetANI_Sit()		{	return GetANI_Stop();	}
	virtual int			GetANI_Ride()		{   return 0; }
	virtual int			GetANI_PickITEM()	{   return GetANI_Stop();	}	

	virtual int			Def_AttackRange()=0{ *(int*)0 = 10; };
	virtual int			Get_R_WEAPON ()=0{ *(int*)0 = 10; };
	//virtual int			Get_KNAPSACK ()=0{	*(int*)0 = 10; };
	virtual bool		Attack_START (CObjCHAR *pTarget)=0{ *(int*)0 = 10; };
	virtual bool		Skill_START (CObjCHAR *pTarget)=0{ *(int*)0 = 10; };
	virtual bool		Casting_START( CObjCHAR* pTarget )=0{ *(int*)0 = 10; };
	virtual void		Casting_END ()=0{ *(int*)0 = 10; };
	virtual void		MoveStart (){ *(int*)0 = 10; };

	virtual bool		ToggleRunMODE ( float fAdjRate )				{	return true;	}
	virtual bool		ToggleSitMODE ()				{	return true;	}

#ifndef __SERVER
	virtual void		RideCartToggle( bool bRide ){};
#endif


#ifndef	__SERVER
	virtual void		Attack_END (bool bStopTrail=true) = 0{ *(int*)0 = 10; };
	virtual int			Get_L_WEAPON ()=0{ *(int*)0 = 10; };	
	virtual void		SetMotionRepeatCount( int iRepeatCount ) = 0{ *(int*)0 = 10; };

	/// ���� ��ų������ �Ҽ� �ִ°�?( ������ ���� ���� ����� �޾Ҵ°�? )
	virtual bool		bCanActionActiveSkill() = 0{ *(int*)0 = 10; };
	virtual void		SetEffectedSkillFlag( bool bResult )= 0{ *(int*)0 = 10; };
	virtual bool		bCanStartSkill() = 0{ *(int*)0 = 10; };							/// ������ ���� ĳ���� ��ŸƮ ��Ŷ�� �޾Ҵ°�?
	virtual void		SetStartSkill( bool bResult ) = 0{ *(int*)0 = 10; };

	virtual void		Set_ModelDIR (t_POSITION &PosToView, bool bImmediate = false )=0{ *(int*)0 = 10; };
	virtual void		Set_ModelDIR( float fAngleDegree, bool bImmediate = false ) = 0{ *(int*)0 = 10; };
	virtual void		Set_ModelSPEED (float fSpeed)=0{ *(int*)0 = 10; };
	virtual float		Get_ModelSPEED () = 0{ *(int*)0 = 10; };
#endif


	/// �Ʒ� 3���� �Լ��� �ڽ��ϰ�� ��ü ���� �� / ��Ƽ���ϰ�� �������� ���� �� ����
	virtual int			Get_CON()					{	return 0;	}	// ������ ��� return pAVATAR->GetCur_CON();
	virtual int			GetAdd_RecoverHP()			{	return 0;	}	// ������ ��� return pAVATAR->m_btRecoverHP;
	virtual int			GetAdd_RecoverMP()			{	return 0;	}	// ������ ��� return pAVATAR->m_btRecoverHP;
	
	
	/// RECOVER_STATE_CHECK_TIME �������� Get_RecoverHP���� ���� HP, Get_ReocverMP���� ���� MP�� ���� �ش�.	
	virtual void		RecoverHP( short nRecoverMODE )		{	/* nop */	}	
	virtual void		RecoverMP( short nRecoverMODE )		{	/* nop */	}

	

#ifndef	__SERVER
public:
			float		m_fAdjustSPEED;
	virtual void		Adj_MoveSPEED ( WORD wSrvDIST, const D3DVECTOR& PosGOTO)=0{ *(int*)0 = 10; };
	virtual void		Adj_AniSPEED ( float fAniSpeed )=0{ *(int*)0 = 10; };;
	virtual float		Get_DefaultSPEED ()=0{ *(int*)0 = 10; };
	
	virtual void		Reset_Position () = 0{ *(int*)0 = 10; }; /// �̵� ���۽�, ���� ��ġ�� ������
#endif


protected:
	virtual short		GetPsv_ATKSPEED (float fCurSpeed, short nRightWeaponItemNo)				{	return 0;		}
	virtual short		GetPsv_ATKSPEED()	{ return 0; }
	virtual float		Get_fAttackSPEED()=0{ *(int*)0 = 10; };			/// �⺻�ӵ� 1.0�� ���� :: �⺻ ���� �ӵ�( m_fAtkAniSPEED ) + ��ų�� ���� ������ �� ...
	virtual float		Get_MoveSPEED ()=0{ *(int*)0 = 10; };
	virtual int			Get_AttackRange () = 0{ *(int*)0 = 10; };
	virtual float		Get_SCALE() = 0{ *(int*)0 = 10; };

public:
	virtual int			Get_HP ()	= 0{ *(int*)0 = 10; };
	virtual int			Get_MP ()	= 0{ *(int*)0 = 10; };
	virtual int			Get_MaxHP() = 0{ *(int*)0 = 10; };
	virtual int			Get_MaxMP() = 0{ *(int*)0 = 10; };



protected:
	virtual DWORD		GetIngDurationStateFLAG() = 0{ *(int*)0 = 10; };						/// ���� ���� ���� �÷��װ��� ��´�.
	virtual short		GetIngDurationStateSKILL( eING_TYPE eTYPE ) = 0{ *(int*)0 = 10; };	/// ���� ���� ���¿� ����� ��ų��ȣ

public:
	virtual void		Set_HP( int iHP ){};
	virtual void		Set_MP( int iMP ){};

protected :
	virtual bool		SetCMD_ATTACK (int iServerTarget);

public    :
	virtual void		SetCMD_SIT (void);
	virtual void		SetCMD_STAND (void);
	virtual void		SetCMD_MOVE ( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget);
	virtual void		SetCMD_DIE ();
	virtual bool		SetCMD_TOGGLE (BYTE btTYPE);
	
	virtual bool		SetCMD_STOP ();
	virtual bool		SetCMD_MOVE2D ( float fPosX, float fPosY, BYTE btRunMODE );
	virtual bool		SetCMD_MOVE ( const D3DVECTOR& vPosTo, BYTE btRunMODE );
	
	virtual void		SetCMD_Skill2SELF (short nSkillIDX);
	virtual bool		SetCMD_Skill2OBJ ( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget, short nSkillIDX);
	virtual void		SetCMD_Skill2POS ( const D3DVECTOR& PosGOTO, short nSkillIDX);

	virtual void		SetCMD_RUNnATTACK(int iTargetObjTAG)
	{	
		if ( this->SetCMD_ATTACK (iTargetObjTAG) )		// MOB AI SetCMD_RUNnATTACK()... 
			this->m_bRunMODE = true;
	}

	virtual void		Do_CreatedAI	(void)								{	/* nop */	}
	virtual void		Do_StopAI		(void)								{	/* nop */	}
	virtual void		Do_AttackMoveAI (CObjCHAR *pTarget)					{	/* nop */	}
	virtual void		Do_DamagedAI	(CObjCHAR *pTarget, int iDamage)	{	/* nop */	}
	virtual void		Do_KillAI		(CObjCHAR *pTarget, int iDamage)	{	/* nop */	}
	virtual void		Do_DeadAI		(CObjCHAR *pTarget, int iDamage)	{	/* nop */	}	
	virtual void		Do_DeadEvent	(CObjCHAR *pTarget)	{	/* nop */	}	


	virtual bool		Is_SameTYPE(int iType)			{	return (iType==this->Get_TYPE());	}

	



#ifndef	__SERVER
	///
	/// Managing avt's action mode
	///
	virtual void		ChangeActionMode( int iActionMode ){};	

	///
	/// Check PET MODE
	/// Just avt have to override this virtual methodes.
	/// if mode < 0, this char is normal mode
	virtual int			GetPetMode(){ return -1; }
	virtual bool		CanAttackPetMode(){ return false; }
	
	virtual void		SetCMD_PET_MOVE( const D3DVECTOR& PosTO, BYTE btRunMODE ){};
	virtual void		SetCMD_PET_MOVE( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget){};
	virtual void		SetCMD_PET_STOP(){};
	virtual void		SetCMD_PET_ATTACK ( int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosTO ){ };
	virtual int		Get_WorldTIME(void);

//-----------------------------------------------------------------------------------------------
	//����ȣ:: �����Լ� �缱�� 
	//īƮ Ÿ�� ��ų�� ����ϱ� ���ؼ� ������ 
	virtual int		ProcCMD_Skill2OBJECT_PET() { return 0; };
	//īƮ ��ų ����� ���� �ൿ�� ���� �ϱ� ���� ������ .
	virtual void		SetNewCommandAfterSkill_PET( int iSkillNO ) {  }	
//-----------------------------------------------------------------------------------------------


#endif


	/// > End
	//////////////////////////////////////////////////////////////////////////////////////////








	tagMOTION*			m_pCurMOTION;
	int					m_iCurMotionFRAME;

	bool				Set_CurMOTION( tagMOTION* pMotion );
	bool				Chg_CurMOTION( tagMOTION* pMotion );

	#ifndef	__SERVER
		HNODE			Get_ZMOTION()			{	return m_pCurMOTION->m_hMotion;		}
		short			Get_ActionIDX()			{	return m_pCurMOTION->m_nActionIdx;	}
	#endif

	void				Start_MOVE (float fSpeed);
	void				Restart_MOVE (t_POSITION &PosGOTO);
    void                Restart_MOVE_AL (t_POSITION &PosGOTO);

	void				Start_ATTACK (CObjCHAR *pTarget);
	char				Do_SKILL (int iServerTarget, CObjCHAR *pTarget=NULL);
	void				SetSkillDelay( int iSkillIdx );

	void				SetNewCommandAfterSkill( int iSkillNO );
	int					ProcSkillCastingAction( int iServerTarget, CObjCHAR* pTarget );
	int					ProcSkillCastingLoop( CObjCHAR *pTarget );
	int					ProcSkillAction( CObjCHAR *pTarget );					///< ���� ��ų �׼ǵ����� �Ѵ�.


	bool				Goto_TARGET( CObjCHAR *pTarget, int iRange );
	bool				Goto_POSITION (int iRange=0);


	int					ProcCMD_SIT ();
	int					ProcCMD_STOP ();
	int					ProcCMD_MOVE ();
	int					ProcCMD_PICK_ITEM ();
	int					ProcCMD_ATTACK ();
	int					ProcCMD_Skill2SELF ();
	int					ProcCMD_Skill2POSITION ();
	int					ProcCMD_Skill2OBJECT ();

	
	short				Get_RecoverHP( short nRecoverMODE );
	short				Get_RecoverMP( short nRecoverMODE );

	virtual int			GetOri_MaxHP();
	virtual int			GetOri_MaxMP();



public    :
	bool				m_bAttackSTART;
	bool				m_bCastingSTART;
	int					m_iCastingStartTime;	/// ������� ���� ����...
		

	int					m_iWaitLoopCnt;			/// �����κ����� ����� ��ٸ���..

	int					m_SkillActionState;		///< ���� ��ų�� ���ۻ���( ĳ�������ΰ�? ��� ���� ���ΰ�? ���� �׼� ���ΰ�? )
	short				m_nToDoSkillIDX;		/// �����(������) ��ų

	short				m_nActiveSkillIDX;		/// ���� ���ۿ� ����� (ĳ������) ��ų
	int					m_iActiveObject;		/// ���� ���ۿ� ����� Ÿ�� :: Ŭ���̾�Ʈ ��ü ��ȣ 
	short				m_nDoingSkillIDX;		/// ���� �׼� �������� ��ų��ȣ. �׼ǵ��� ���۽� �����ϰ� �� ��� ����� ����..
	

	float				m_fRunAniSPEED;

	float				m_fCurMoveSpeed;

	int					m_iMoveDistance;
	t_POSITION			m_PosBORN;
	t_POSITION			m_PosMoveSTART;
	t_POSITION			m_PosGOTO;
	tPOINTF				m_MoveVEC;		// 1/1000 �� �̵���
    BYTE				m_bRunMODE;
	BYTE				m_btMoveMODE;	// 0:�ȱ�, 1:�ٱ�, 2:Ÿ��



public:
	CObjAI ();
	virtual ~CObjAI ();

	void				SetCastingState( bool bStart );
	bool				ProcOneActionFrame( int iIndex );
	bool				ProcMotionFrame (void);

	void				Set_STATE( WORD wState )				{	m_wState = wState;								}	/// ���� ����
	WORD				Get_STATE ()							{	return m_wState;								}
	WORD				Get_COMMAND ()							{	return m_wCommand;								}
	void				Set_COMMAND (WORD wCommand)				{	m_wCommand = wCommand;							}
	
//--------------------------------------------------------------------------------------------------------------------
	//����ȣ:: ���� ����� ��Ʈ�� �ϱ����� ������.
	WORD				Get_BECOMMAND()							{	return m_wBeforeCMD;							}
	void				Set_BECOMMAND(WORD wCommand)			{	m_wBeforeCMD = wCommand;						}
//--------------------------------------------------------------------------------------------------------------------

	float				Get_MoveAniSPEED()						{	return ( m_bRunMODE ) ? m_fRunAniSPEED : 1.0f;	}

	void				Reset_MoveVEC ();
	int					Cal_AtkAniSPEED( short nRightWeaponItemNO );
	float				Cal_RunAniSPEED( short nCmPerSec )
	{
		// �̵� ���� ������ = ( �̵��ӵ�+180) /600
		return ( (nCmPerSec+180.f) / 600.f );
	}

	

public :	

	int					Get_TAG ()							{	return this->Get_INDEX();			}
	int					Get_ObjTYPE()						{	return this->Get_TYPE();			}
	bool				Is_AVATAR()							{	return this->IsUSER();				}
	int					Get_PercentHP()
	{
		_ASSERT( this->Get_MaxHP() );
		return ( this->Get_HP()*100 / this->Get_MaxHP() );
	}

	CAI_OBJ *			Get_TARGET ()						{	return this->Get_TargetOBJ();		}	
	void				Set_TargetObjIDX (int iTargetOBJ)	{	this->Set_TargetIDX(iTargetOBJ);	}	





#ifndef	__SERVER
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/// queuing the command 
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	/// ���� ����� �����Ҽ� �ִ°�?
	virtual bool		CanApplyCommand() = 0{ *(int*)0 = 10; };

	virtual void		PushCommandSit() = 0{ *(int*)0 = 10; };
	virtual void		PushCommandStand() = 0{ *(int*)0 = 10; };
	virtual void		PushCommandStop() = 0{ *(int*)0 = 10; };
	virtual void		PushCommandMove( const D3DVECTOR& PosTo, BYTE btRunMODE ) = 0{ *(int*)0 = 10; };
	virtual void		PushCommandMove( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget ) = 0{ *(int*)0 = 10; };
	virtual void		PushCommandAttack( int iServerTarget, WORD wSrvDIST, const D3DVECTOR& PosTO ) = 0{ *(int*)0 = 10; };
	virtual void		PushCommandDie() = 0{ *(int*)0 = 10; };
	virtual void		PushCommandToggle( BYTE btTYPE ) = 0{ *(int*)0 = 10; };
	virtual void		PushCommandSkill2Self( short nSkillIDX ) = 0{ *(int*)0 = 10; };
	virtual void		PushCommandSkill2Obj( WORD wSrvDIST, const D3DVECTOR& PosTO, int iServerTarget, short nSkillIDX ) = 0{ *(int*)0 = 10; };
	virtual void		PushCommandSkill2Pos( const D3DVECTOR& PosGOTO, short nSkillIDX ) = 0{ *(int*)0 = 10; };


#endif




} ;

//-------------------------------------------------------------------------------------------------
#endif
