/**
 * \ingroup SHO_GS
 * \file	CObjNPC.h
 * \brief	mob/npc �ɸ��� ó��
 */
#ifndef	__COBJNPC_H
#define	__COBJNPC_H
#include "CObjCHAR.H"
//-------------------------------------------------------------------------------------------------

// #define	MAX_SAVED_DAMAGED	7
struct tagSavedDAMAGE {
	DWORD	m_dwInsertTIME;
	DWORD	m_dwUpdateTIME;
	int		m_iObjectIDX;
	int		m_iDamage;
} ;

//-------------------------------------------------------------------------------------------------
#define	MAX_MOB_VAR_CNT		5

/**
 * \ingroup SHO_GS_LIB
 * \class	CObjMOB
 * \author	wookSang.Jo
 * \brief	mob �ɸ��� ��ü�� ����Ÿ ������ ó���ϴ� Ŭ����
 *			CObjCHARŬ������ ��ӹ���
 */
class CObjMOB : public CObjCHAR
{
private:
	char			m_cSkillMotionIDX;
public :	
	// virtual function inherit from CGameOBJ -----------------------------------------------------
	t_ObjTAG		Get_TYPE ()						{	return OBJ_MOB;					}
	char		   *Get_NAME ()	                	{	return NPC_NAME( m_nCharIdx );	}
    virtual bool	Make_gsv_ADD_OBJECT( classPACKET *pCPacket );

public :
	// virtual function inherit from CObjAI -------------------------------------------------------
	CAI_OBJ*Get_CALLER();
	bool	SetCMD_Skill2SELF (short nSkillIDX, short nMotion);
	bool	SetCMD_Skill2OBJ (int iTargetObjIDX, short nSkillIDX, short nMotion);

	void	Run_AWAY (int iDistance);
	void	Drop_ITEM (short nDropITEM, BYTE btToOwner);
	bool	Change_CHAR (int iCharIDX);
    void    Set_EMOTION (short nEmotionIDX);

	int		GetANI_Stop()		{	return MOB_ANI_STOP;		}
	int		GetANI_Move()		{	return ( m_bRunMODE ) ? MOB_ANI_RUN : MOB_ANI_MOVE;	}
	int		GetANI_Attack()		{	return MOB_ANI_ATTACK;		}
	int		GetANI_Die ()		{	return MOB_ANI_DIE;			}
	int		GetANI_Hit ()		{	return MOB_ANI_HIT;			}
	int		GetANI_Skill ()		{	return m_cSkillMotionIDX+1;	}
	int		GetANI_Etc ()		{	return MOB_ANI_ETC;			}
	int		GetANI_Casting()	{	return m_cSkillMotionIDX;	}

	int		Get_R_WEAPON ()		{	return NPC_R_WEAPON( m_nCharIdx );		}
	int		Def_AttackRange()	{	return NPC_ATK_RANGE( m_nCharIdx );		}

public :	
	// virtual function inherit from CObjCHAR -----------------------------------------------------
	int		m_iCallerMobObjIDX;
	int		GetCallerObjIDX ()	{	return m_iCallerMobObjIDX;		}

	bool	SendPacketToTARGET( CObjCHAR *pAtkCHAR, classPACKET *pCPacket );

	int		Get_AttackRange (short nSkillIDX);
    bool	IsFemale ()										{	return false;										}
    short	IsMagicDAMAGE()									{	return NPC_IS_MAGIC_DAMAGE( m_nCharIdx );			}

	tagMOTION *Get_MOTION (short nActionIdx=MOB_ANI_STOP)	{	return m_pCharMODEL->GetMOTION( (BYTE)nActionIdx );	}
	classUSER *Give_EXP();

	int		Save_Damage( int iAttackerIDX, int iDamage);
	bool	Dead (CObjCHAR *pKiller);

//    short	Get_ItemQUALITY (short nEquipIDX)	{	return 60;			    }	// ������ 60 !
    short	Get_WEIGHT ()			{	return 0;			            	}
    int		Get_LEVEL ()			{	return NPC_LEVEL( m_nCharIdx );		}
    int		Get_GiveEXP ()			{	return NPC_GIVE_EXP( m_nCharIdx );	}
	int		Get_AbilityValue (WORD wType);

	int		Get_INT ()				{	return Get_LEVEL();					}
    int		Get_CHARM ()			{	return 0;							}
    int		Get_SENSE ()			{	return Get_LEVEL();					}

	BYTE	Get_MoveMODE ()			{	return m_bRunMODE;					}
	BYTE	Get_RideMODE ()			{	return 0;							}

	short	GetOri_WalkSPEED()		{	return NPC_WALK_SPEED( m_nCharIdx );}
	short	GetOri_RunSPEED()		{	return NPC_RUN_SPEED( m_nCharIdx );	}
	short	GetOri_ATKSPEED ()		{	return NPC_ATK_SPEED( m_nCharIdx );	}

	int		GetOri_ATK ()			{	return NPC_ATK  ( m_nCharIdx );		}
    int		GetOri_DEF ()			{	return NPC_DEF	( m_nCharIdx );		}
    int		GetOri_RES ()			{	return NPC_RES	( m_nCharIdx );		}
    int		GetOri_HIT ()			{	return NPC_HIT	( m_nCharIdx );		}
    int		GetOri_AVOID ()			{	return NPC_AVOID( m_nCharIdx );		}
	int		GetOri_CRITICAL()		{	return m_nCritical;					}

	int		GetOri_MaxHP()			{	return m_iOriMaxHP;					}
	int		GetOri_MaxMP()			{	return 100;							}

	bool	Make_gsv_SUB_OBJECT( classPACKET *pCPacket );

	void	Add_ToTargetLIST( CObjAVT *pAVTChar );
	void	Sub_FromTargetLIST( CObjAVT *pAVTChar );

	WORD	Get_ATTRIBUTE ()		{	return NPC_ATTRIBUTE( m_nCharIdx );	}		// �Ӽ�


private:
	// CObjMOB ------------------------------------------------------------------------------------
	CCharDATA  *m_pCharMODEL;
	ULONG		m_ulAICheckTIME[ 2 ];
	short		m_nCritical;
	int			m_iAiVAR[ MAX_MOB_VAR_CNT ];

	classDLLIST< CObjAVT* >		m_TargetLIST;
#ifdef	__CS_TARGET_LIST
	CCriticalSection			m_csTargetLIST;
#endif

public :
	CObjMOB ()
	{
		m_SavedDAMAGED = NULL;
	}
	virtual ~CObjMOB ()
	{
		SAFE_DELETE_ARRAY( m_SavedDAMAGED );
	}
	short	m_nQuestIDX;
	int		m_iOriMaxHP;
	
	short	m_nSavedDamageCNT;
	tagSavedDAMAGE *m_SavedDAMAGED;
	CRegenPOINT *m_pRegenPOINT;

    bool Init (CZoneTHREAD *pZONE, short nCharIdx, float fXPos, float fYPos, int iTeamNO, CRegenPOINT *pRegenPOINT=NULL, short nQuestIDX=0);
    int  Proc ();

	// CAI_OBJ :: virtual �Լ� ������
	int		 Get_AiVAR(short nVarIdx)				{	return m_iAiVAR[ nVarIdx ];		}
	void	 Set_AiVAR(short nVarIdx, int iValue )	
	{	
		m_iAiVAR[ nVarIdx ] = iValue;
		if ( m_iAiVAR[ nVarIdx ] < 0 ) 
			m_iAiVAR[ nVarIdx ] = 0;
		else
		if ( m_iAiVAR[ nVarIdx ] > 500 )
			m_iAiVAR[ nVarIdx ] = 500;
	}


#ifdef	__CS_TARGET_LIST
	CStrVAR	m_StrLOC;
	void	LockTargetLIST ();
	void	UnlockTargetLIST ();
#else
	void	LockTargetLIST ()		{	/* nop */	}
	void	UnlockTargetLIST ()		{	/* nop */	}
#endif

	inline void Do_CreatedAI (void)								{	g_AI_LIST.AI_Created( NPC_AI_TYPE(m_nCharIdx), this );						}
	inline void Do_StopAI (void)								
	{	
		if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return;		// ����, ���� ���¸� ���..
		g_AI_LIST.AI_Stop( NPC_AI_TYPE(m_nCharIdx), this );							
	}
	inline void Do_AttackMoveAI (CObjCHAR *pTarget)				
	{	
		if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return;		// ����, ���� ���¸� ���..
		g_AI_LIST.AI_AttackMove( NPC_AI_TYPE(m_nCharIdx), this, pTarget );			
	}
	inline void Do_DamagedAI (CObjCHAR *pTarget, int iDamage )	
	{	
		if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return;		// ����, ���� ���¸� ���..

		// �������� �Ʊ��� �ƴϰ� Ŭ��ŷ ���°� �ƴϸ�...
		if ( !this->Is_ALLIED( pTarget ) /* && 0 == pTarget->m_IngSTATUS.IsSET( FLAG_ING_DISGUISE | FLAG_ING_TRANSPARENT) */ ) {
			// ����ڿ� ���� ��ȯ�� �ɸ��� �Ʊ��� �������� ���ϵ���...
			if ( !( CMD_BIT_INT & this->Get_COMMAND() ) )
				g_AI_LIST.AI_Damaged( NPC_AI_TYPE(m_nCharIdx), this, pTarget, iDamage );
		}
	}
	inline void Do_KillAI (CObjCHAR *pTarget, int iDamage)		
	{	
		if ( this->m_IngSTATUS.IsIgnoreSTATUS() ) return;		// ����, ���� ���¸� ���..
		g_AI_LIST.AI_Kill( NPC_AI_TYPE( m_nCharIdx ), this, pTarget, iDamage );		
	}
	inline void Do_DeadAI (CObjCHAR *pTarget, int iDamage)		{	g_AI_LIST.AI_Dead( NPC_AI_TYPE( m_nCharIdx ), this, pTarget, iDamage );		}

	unsigned long	Get_AICheckTIME(int iIDX)							{	return m_ulAICheckTIME[iIDX];		}
	void			Set_AICheckTIME(int iIDX, unsigned long lCheckTIME)	{	m_ulAICheckTIME[iIDX] = lCheckTIME;	}
} ;

//-------------------------------------------------------------------------------------------------
/**
 * \ingroup SHO_GS_LIB
 * \class	CObjSUMMON
 * \author	wookSang.Jo
 * \brief	���� ��ȯ�� �ɸ��� ��ü�� ����Ÿ ������ ó���ϴ� Ŭ����
 *			CObjMOBŬ������ ��ӹ���
 */
class CObjSUMMON : public CObjMOB
{
private:
	int		m_iLevel;
	short	m_nSummonedSkillIDX;

	int		m_iOriATK;	
	int		m_iOriHIT;	
	int		m_iOriDEF;	
	int		m_iOriRES;	
	int		m_iOriAVOID;	

public :
	CObjSUMMON ()
	{
		m_iCallerUserObjIDX = 0;
	}
    int		Proc ();
	BYTE		m_btSummonCMD;

	int			m_iCallerUserObjIDX;	// ��ȯ ��Ų ����
	t_HASHKEY	m_HashCALLER;			// ���� �ɸ� �ؽ�

	// ��ȯ ��Ų ���� ����. 2005-06-30(kchs)�߰�����: ��ȯ�� �ɷ�ġ ������ ���� ���� �ƹ�Ÿ ������ �ʿ�..
	void SetCallerOBJ( int iCallerOBJ, t_HASHKEY HashCallerOBJ, int iCallerLEVEL, short nSkillIDX , CObjCHAR * pOwnerAVT);

	int			GetCallerUsrIDX ()			{	return m_iCallerUserObjIDX;		}
	int			GetCallerObjIDX ()			{	return m_iCallerUserObjIDX;		}
	t_HASHKEY	GetCallerHASH ()			{	return m_HashCALLER;		}
	short		GetSummonedSkillIDX ()		{	return m_nSummonedSkillIDX;	}

	int		Get_LEVEL ()					{	return m_iLevel;		}

	int		GetOri_ATK ()					{	return m_iOriATK;		}
	int		GetOri_HIT ()					{	return m_iOriHIT;		}
	int		GetOri_DEF ()					{	return m_iOriDEF;		}
	int		GetOri_RES ()					{	return m_iOriRES;		}
	int		GetOri_AVOID ()					{	return m_iOriAVOID;		}
} ;

//-------------------------------------------------------------------------------------------------
/**
 * \ingroup SHO_GS_LIB
 * \class	CObjSUMMON
 * \author	wookSang.Jo
 * \brief	npc �ɸ��� ��ü�� ����Ÿ ������ ó���ϴ� Ŭ����
 *			CObjVAR, CObjMOB Ŭ������ ���� ��ӹ���
 */
// CObjVAR�� ���� ���...
class CObjNPC : public CObjVAR, public CObjMOB
{
public :
	bool		Send_gsv_SET_EVENT_STATUS ();

	// virtual function inherit from CGameOBJ -----------------------------------------------------
	t_ObjTAG	Get_TYPE ()		{	return OBJ_NPC;					}
	bool		Make_gsv_ADD_OBJECT( classPACKET *pCPacket );

	// virtual function inherit from CObjVAR -----------------------------------------------------
	bool  VSend_gsv_CHAT ( char *szMsg )				{	return Send_gsv_CHAT(szMsg);	}
	bool  VSend_gsv_SHOUT( char *szMsg )				{	return Send_gsv_SHOUT(szMsg);	}
	void  VSend_gsv_ANNOUNCE_CHAT( char *szMsg )		{	Send_gsv_ANNOUNCE_CHAT(szMsg);	}

	// 0 = ����� , 1 = ���̱�, 2 = ����ϱ�
	void  VSet_SHOW		 ( BYTE btShowMode );

	short VGetCur_ZoneNO();
	float VGetCur_XPOS()								{	return	this->m_PosCUR.x;		}
	float VGetCur_YPOS()								{	return	this->m_PosCUR.y;		}	

	int	  VGetCur_ZoneTIME()							{	return	GetCur_ZoneTIME();		}

	void  Set_ObjVAR(BYTE btVarIDX, short nValue )
	{
		// ���⼭ ���� ? �ƴ� �������� �޾Ƽ� ���� ???
		if ( 0 == btVarIDX ) {
			short nBefore = this->Get_ObjVAR( 0 );
			CObjVAR::Set_ObjVAR( 0, nValue );
			if ( nBefore != this->Get_ObjVAR( 0 ) ) {
				// ���� �ٲ����.
				this->Send_gsv_SET_EVENT_STATUS ();
			}
		} else
			CObjVAR::Set_ObjVAR( btVarIDX, nValue );
	}

	void Set_NextTRIGGER( int iRemainSec, t_HASHKEY HashNextTrigger )
	{
		// ��� �޾Ƽ� ���� ������ �����ؾ� ��...
		CObjVAR::Set_NextTRIGGER( iRemainSec, HashNextTrigger );
	}
	void Set_TRIGGER (t_HASHKEY HashNextTrigger)
	{	
		this->Set_NextTRIGGER( 1, HashNextTrigger );
	}


private:
	// CObjNPC ------------------------------------------------------------------------------------
	float	m_fModelDIR;
	bool	m_bShow;

public :
	CObjNPC ();
	~CObjNPC ();

	void	Do_CreatedAI	(void)								{	/* nop */	}
//	void	Do_StopAI		(void)								{	/* nop */	}
	void	Do_AttackMoveAI (CObjCHAR *pTarget)					{	/* nop */	}
//	void	Do_DamagedAI	(CObjCHAR *pTarget, int iDamage)	{	/* nop */	}
	void	Do_KillAI		(CObjCHAR *pTarget, int iDamage)	{	/* nop */	}
	void	Do_DeadAI		(CObjCHAR *pTarget, int iDamage)	{	/* nop */	}	
	void	Do_DeadEvent	(CObjCHAR *pTarget)					{	/* nop */	}	

	void	Set_DIR (float fDIR)						{	m_fModelDIR = fDIR;				}
	bool	Get_SellITEM( short nSellTAB, short nSellCOL, tagITEM &OutITEM );
    int		Proc ();
} ;

//-------------------------------------------------------------------------------------------------
#endif