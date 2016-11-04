/*
	$Header: /7HeartsOnline/Server/SHO_GS/Sho_gs_lib/Common/CObjAI.h 30    05-09-21 4:29p Icarus $
*/
#ifndef	__CCOMMAND_H
#define	__CCOMMAND_H
#include "CGameOBJ.h"
#include "IO_Motion.h"
#include "IO_Skill.h"
#include "CAI_LIB.h"
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
#define	CS_BIT_MOV2		0x0400					// ���°� �ٲ������ ���� �̵��� ��ġ��ŭ�� ����..

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

#define	CMD_STORE		0x000b

/// ����׼ǵ�
enum
{
	COMMON_COMMAND_SIT			= 1,
	COMMON_COMMAND_PICK_ITEM	= 2,
	COMMON_COMMAND_JUMP			= 3,
	COMMON_COMMAND_AIR_JUMP		= 4,
	COMMON_COMMAND_AUTO_TARGET	= 5,
	COMMON_COMMAND_ATTACK		= 6,
};


#define	WALK_CmPerSec			200

#define	WEIGHT_RATE_WALK		100
#define WEIGHT_RATE_CANT_ATK	110


//-------------------------------------------------------------------------------------------------

class CObjCHAR;
class CObjAI;
class CZoneTHREAD;


///
///	\class CObjTARGET
/// \brief class for object that has target.
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
	CAI_OBJ *				Get_TargetOBJ (bool bCheckHP);

public  :
			int				Get_TargetIDX ()								{	return m_iServerTarget;			}
	virtual void			Set_TargetIDX (int iTargetIndex, bool bHP=true)	{	m_iServerTarget=iTargetIndex;	}
	virtual CZoneTHREAD*	GetCur_ZONE () = 0;
	CObjTARGET ()	{		m_iServerTarget = 0;	}
} ;


#define	RECOVER_JUST_CHECK_TIME			0		/// �ϰ͵� ���ϰ� �ð��� ����.
#define	RECOVER_STATE_CHECK_TIME		8000	/// 4�ʴ� ȸ��..
#define	USE_FUEL_CHEC_TIME				10000	/// 10�ʴ� �Ҹ�..
#define	RECOVER_STATE_SIT_ON_CHAIR		30		/// ���� �ɱ�		:: nRecoverMODE = 30
#define	RECOVER_STATE_SIT_ON_GROUND		20		/// �ٴ� �ɱ�       :: nRecoverMODE = 20
#define RECOVER_STATE_STOP_OR_WALK		8		/// ����,�ȱ� ����	:: nRecoverMODE = 8

/**
 * \ingroup SHO_GS_LIB
 * \class	CObjAI
 * \author	wookSang.Jo
 * \brief	�ɸ��� �ΰ����� ó�� Ŭ����
 *			�ΰ����� ����ŸŬ����(CAI_OBJ)�� ��� ����
 */
class CObjAI : public CGameOBJ, public CAI_OBJ, public CObjTARGET 
{
private	  :
	WORD				m_wState;
	WORD				m_wCommand;
	WORD				m_wBeforeCMD;			/// self skill ���� ���� ��� ����..			

	CZoneTHREAD*		GetCur_ZONE ()	{	return this->GetZONE();		}

protected :

	//////////////////////////////////////////////////////////////////////////////////////////
	///>
	///> CObjAI virtual functions
	///>

	virtual bool		IsInRANGE (CObjCHAR *pT, int iRange)=0;
	virtual void		ActionEVENT (int iActionIDX)=0;
	virtual void		Adjust_HEIGHT () = 0;
	virtual int			GetCurrentFrame () = 0;
	virtual bool		Set_MOTION (short nActionIdx, float fMoveSpeed=0, float fAniSpeed=1.0f, bool bAttackMotion=false)=0;
//	virtual void		Move_COMPLETED ()=0;

	virtual int			GetANI_Stop()=0;
	virtual int			GetANI_Die()=0;
	virtual int			GetANI_Hit()=0;
	virtual int			GetANI_Move()=0;
	virtual int			GetANI_Attack()=0;
	virtual int			GetANI_Casting()=0;
	virtual int			GetANI_Skill()=0;
	virtual int			GetANI_Sitting()	{	return GetANI_Stop();	}
	virtual int			GetANI_Standing()	{	return GetANI_Stop();	}
	virtual int			GetANI_Sit()		{	return GetANI_Stop();	}

	virtual int			Def_AttackRange()=0;
	virtual int			Get_R_WEAPON ()=0;
	virtual bool		Attack_START (CObjCHAR *pTarget)=0;
	virtual bool		Skill_START (CObjCHAR *pTarget)=0;
	virtual bool		Casting_START( CObjCHAR* pTarget )=0;
	virtual void		Casting_END ()=0;

	virtual bool		ToggleRunMODE ()				{	return true;	}
	virtual bool		ToggleSitMODE ()				{	return true;	}

#ifndef	__SERVER
	virtual void		Attack_END (bool bStopTrail=true) = 0;
	virtual int			Get_L_WEAPON ()=0;

	/// ���� ��ų������ �Ҽ� �ִ°�?( ������ ���� ���� ����� �޾Ҵ°�? )
	virtual bool		bCanActionActiveSkill() = 0;
	virtual void		SetEffectedSkillFlag( bool bResult )= 0;

	virtual void		Set_ModelDIR (t_POSITION &PosToView)=0;
	virtual void		Set_ModelDIR( float fAngleDegree ) = 0;
	virtual void		Set_ModelSPEED (float fSpeed)=0;
#endif


	/// �Ʒ� 3���� �Լ��� �ڽ��ϰ�� ��ü ���� �� / ��Ƽ���ϰ�� �������� ���� �� ����
	virtual int			Get_CON()						{	return 0;	}	// ������ ��� return pAVATAR->GetCur_CON();
	virtual int			GetAdd_RecoverHP()				{	return 0;	}	// ������ ��� return pAVATAR->m_btRecoverHP;
	virtual int			GetAdd_RecoverMP()				{	return 0;	}	// ������ ��� return pAVATAR->m_btRecoverHP;
	
	
#ifndef	__SERVER
public:
			float		m_fAdjustSPEED;
	virtual void		Adj_MoveSPEED (tPOINTF PosCUR, tPOINTF PosGOTO)=0;
	virtual float		Get_DefaultSPEED ()=0;
#endif


protected:
	virtual short		GetPsv_ATKSPEED ()				{	return 0;		}
	virtual short		Get_nAttackSPEED()=0;			/// �⺻�ӵ� 100�� ���� :: �⺻ ���� �ӵ�( m_fAtkAniSPEED ) + ��ų�� ���� ������ �� ...
	virtual float		Get_MoveSPEED ()=0;
	virtual int			Get_AttackRange(short /*nSKillIDX*/)=0;
	virtual float		Get_SCALE() = 0;

public:
	virtual int			Get_HP ()	= 0;
	virtual int			Get_MP ()	= 0;
	virtual int			Get_MaxHP() = 0;
	virtual int			Get_MaxMP() = 0;
	virtual short		Get_WeightRATE()				{	return 0;		}

protected:
	virtual DWORD		GetIngDurationStateFLAG() = 0;						/// ���� ���� ���� �÷��װ��� ��´�.
	virtual short		GetIngDurationStateSKILL( eING_TYPE eTYPE ) = 0;	/// ���� ���� ���¿� ����� ��ų��ȣ

public:
	virtual void		Set_HP( int iHP )									{	;	};
	virtual void		Set_MP( int iMP )									{	;	};

protected :
	virtual bool		SetCMD_ATTACK (int iServerTarget);

public    :
	virtual bool		SkillChk_ACTIVE ( short /*nSkillIDX*/ )				{	return true;	}
	virtual void		SkillUse_ABILITY( short /*nSkillIDX*/ )				{	;				}

	virtual bool		SetCMD_SIT (void);
	virtual bool		SetCMD_STAND (void);
	virtual bool		SetCMD_MOVE (tPOINTF &PosFROM, tPOINTF &PosTO, int iServerTarget);
	virtual void		SetCMD_DIE ();
//	virtual bool		SetCMD_TOGGLE (BYTE btTYPE);

	virtual bool		SetCMD_Skill2SELF (short nSkillIDX);
	virtual bool		SetCMD_Skill2OBJ (int iServerTarget, short nSkillIDX);
	virtual bool		SetCMD_Skill2POS (tPOINTF &PosGOTO, short nSkillIDX);

	virtual void		Do_CreatedAI	(void)								{	/* nop */	}
	virtual void		Do_StopAI		(void)								{	/* nop */	}
	virtual void		Do_AttackMoveAI (CObjCHAR *pTarget)					{	/* nop */	}
	virtual void		Do_DamagedAI	(CObjCHAR *pTarget, int iDamage)	{	/* nop */	}
	virtual void		Do_KillAI		(CObjCHAR *pTarget, int iDamage)	{	/* nop */	}
	virtual void		Do_DeadAI		(CObjCHAR *pTarget, int iDamage)	{	/* nop */	}	
	virtual void		Do_DeadEvent	(CObjCHAR *pTarget)	{	/* nop */	}	

	virtual bool		Is_SameTYPE(int iType)			{	return (iType==this->Get_TYPE());	}
	virtual bool		Is_TauntSTATUS(int iTargetObject)=0;

	virtual bool		SetCMD_STOP ();
	virtual bool		SetCMD_MOVE2D (float fPosX, float fPosY, BYTE btRunMODE);
	virtual void		SetCMD_RUNnATTACK(int iTargetObjTAG)
	{	
		// �ΰ����ɿ��� ȣ��Ǵ� ���� ��� !!!
		if ( !Is_TauntSTATUS( iTargetObjTAG ) ) {
			if ( this->SetCMD_ATTACK (iTargetObjTAG) )		// MOB AI SetCMD_RUNnATTACK()... 
				this->m_bRunMODE = true;
		}
	}
	virtual bool		Send_gsv_SKILL_CANCEL( BYTE btWhy )=0;

	virtual short		Get_PatHP_MODE()			{	return 0;	}
	virtual void		Set_PatHP_MODE(short nV)	{	/* nop */	}
	/// > End
	//////////////////////////////////////////////////////////////////////////////////////////



	tagMOTION*			m_pCurMOTION;
	int					m_iCurMotionFRAME;
	float				m_fAccMotionFRAME;

	bool				Set_CurMOTION( tagMOTION* pMotion );
	bool				Chg_CurMOTION( tagMOTION* pMotion );

	#ifndef	__SERVER
		HNODE			Get_ZMOTION()			{	return m_pCurMOTION->m_hMotion;		}
		short			Get_ActionIDX()			{	return m_pCurMOTION->m_nActionIdx;	}
	#endif

	void				Start_MOVE (float fSpeed);
	void				Restart_MOVE (t_POSITION &PosGOTO);

	void				Start_ATTACK (CObjCHAR *pTarget);

private:
	char				Do_SKILL (int iServerTarget, CObjCHAR *pTarget=NULL);

public :
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

	
	short				Get_RecoverHP( short nRecoverMODE )	{	return ( this->GetAdd_RecoverHP() + ( this->Get_CON()+30 ) / 8 * (nRecoverMODE+3) / 10 );	}
	short				Get_RecoverMP( short nRecoverMODE )	{	return ( this->GetAdd_RecoverMP() + ( this->Get_CON()+20 ) / 10 * nRecoverMODE / 7 );		}

	
	

public    :
	bool				m_bAttackSTART;
	bool				m_bCastingSTART;

	int					m_iWaitLoopCnt;			/// �����κ����� ����� ��ٸ���..

	short				m_nToDoSkillIDX;		/// �����(������) ��ų

private :
	short				m_nActiveSkillIDX;		/// ���� ���ۿ� ����� (ĳ������) ��ų

public :
	short				Get_ActiveSKILL()		{	return m_nActiveSkillIDX;					}
	void				Del_ActiveSKILL()		{	m_bCastingSTART=false, m_nActiveSkillIDX=0;	}

	int					m_iActiveObject;		/// ���� ���ۿ� ����� Ÿ�� :: Ŭ���̾�Ʈ ��ü ��ȣ 
	short				m_nDoingSkillIDX;		/// ���� �������� ��ų( �����ð��� �ٰŸ� Ÿ�ݽ�ų�� ���ؼ� )

	float				m_fRunAniSPEED;

	float				m_fCurMoveSpeed;

	int					m_iMoveDistance;
	t_POSITION			m_PosBORN;
	t_POSITION			m_PosMoveSTART;
	t_POSITION			m_PosGOTO;
	tPOINTF				m_MoveVEC;		// 1/1000 �� �̵���
    BYTE				m_bRunMODE;



public:
	CObjAI ();
	virtual ~CObjAI ();

	bool				ProcMotionFrame (void);

	void				Set_STATE( WORD wState )				{	m_wState = wState;								}	/// ���� ����
	WORD				Get_STATE ()							{	return m_wState;								}
	WORD				Get_COMMAND ()							{	return m_wCommand;								}
	void				Set_COMMAND (WORD wCommand)			{	m_wCommand = wCommand;							}
	float				Get_MoveAniSPEED()						{	return ( m_bRunMODE ) ? m_fRunAniSPEED : 1.0f;	}

	void				Reset_MoveVEC ();
	float				Cal_AtkAniSPEED( short nRightWeaponItemNO );
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
		assert( this->Get_MaxHP() );
		return ( this->Get_HP()*100 / this->Get_MaxHP() );
	}

	CAI_OBJ *			Get_TARGET (bool bCheckHP=true)		{	return this->Get_TargetOBJ(bCheckHP);	}	
	void				Set_TargetObjIDX (int iTargetOBJ)	{	this->Set_TargetIDX(iTargetOBJ);		}	
} ;


#define	__BLOCK_WHEN_SKILL


//-------------------------------------------------------------------------------------------------
#endif
