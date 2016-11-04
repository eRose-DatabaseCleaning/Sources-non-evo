/*
	$Header: /Client/GameCommon/Skill.h 21    05-07-29 5:06p Navy $
*/
#ifndef	__SKILL_H
#define	__SKILL_H


#include <list>
#include "Bullet.h"
#include "Common\\IO_Skill.h"
#include "../../TGameCtrl/TObject.h"

///
///	Skill Apply type
///
enum enumSKILL_APPLY_TYPE
{
	SKILL_APPLY_ALL = 0,
	SKILL_APPLY_PASSIVE,
	SKILL_APPLY_ACTIVE,
	SKILL_APPLY_DULATION,
};



///
/// Skill Target type
///

enum enumSKILL_TARGET_TYPE
{
	SKILL_TARGET_NONE = 0,					///< Ÿ�� ������
	SKILL_TARGET_OBJECT,					///< ������Ʈ Ÿ�� ����
	SKILL_TARGET_GROUND,					///< �׶��� Ÿ��( ���콺 �ٴ�Ŭ�� )
	SKILL_TARGET_PB,						///< �ڱ��ڽ� ��� or.. ������ȭ?
	SKILL_TARGET_SCREEN,					///< ��ũ����ü
	SKILL_TARGET_ZONE,						///< ����ü.
	SKILL_TARGET_SERVER,					///< ������ü
	SKILL_TARGET_MAX,
};


///
/// Skill Action type
///

enum enumSKILL_ACTION_TYPE
{
	SKILL_BASE_ACTION = 1,					///< �⺻ ���( �ɱ�, ����, �ݱ�... ), ����ǥ��
	SKILL_CREATE_WINDOW = 2,				///< â����( ����, ��Ƽ, ���λ���... )	
	SKILL_ACTION_IMMEDIATE = 3,				///< ���� ��� �ߵ�.( ��� ��� ��ü )
	SKILL_ACTION_ENFORCE_WEAPON = 4,		///< ������� ����( ��ȭ, ȿ�� ����( ����ź? ) )
	SKILL_ACTION_ENFORCE_BULLET = 5,		///< ��ȭ�Ѿ� ���� �߻�. ( ���̽� �ַο�.. ���� ȭ���� ���ϴ�.. )
	SKILL_ACTION_FIRE_BULLET = 6,			///< �߻�.( ���̾ )
	SKILL_ACTION_AREA_TARGET = 7,			///< ���� ���ݸ���( ��� ����.. )
	SKILL_ACTION_SELF_BOUND_DURATION = 8,	///< �ڽſ��� �ߵ� ���� ����.( ĳ���� �� ) �ɷ�ġ 
	SKILL_ACTION_TARGET_BOUND_DURATION = 9,	///< Ÿ�ٿ��� �ߵ� ���� ����.( ĳ���� �� ) �ɷ�ġ
	SKILL_ACTION_SELF_BOUND = 10,			///< �ڽſ��� �ߵ� �ٷ� �� ����.( ĳ���� �� ) �ɷ�ġ
	SKILL_ACTION_TARGET_BOUND = 11,			///< Ÿ�ٿ��� �ߵ� �ٷ� �� ����.( ĳ���� �� ) �ɷ�ġ
	SKILL_ACTION_SELF_STATE_DURATION = 12,		///< �ڽſ��� �ߵ� ���� ����.( ĳ���� �� ) ���°���
	SKILL_ACTION_TARGET_STATE_DURATION = 13,	///< ��뿡�� �ߵ� ���� ����.( ĳ���� �� ) ���°���
	SKILL_ACTION_SUMMON_PET = 14,				///< �� ��ȯ ��ų
	SKILL_ACTION_PASSIVE = 15,				///< �нú�..
	SKILL_EMOTION_ACTION = 16,				///< ���� ǥ��
	SKILL_ACTION_SELF_DAMAGE = 17,			///< �ڽ������� �������� �ִ� ��ų..( �̰� ������ �ȵǳ�? ) ���� ����..
	SKILL_ACTION_WARP = 18,					///< ���� ��ų..( �������� ��� )
	SKILL_ACTION_SELF_AND_TARGET = 19,		///< Ÿ�ٿ��� ������ �ִµ��ÿ� ������ ������ ��
	SKILL_ACTION_RESURRECTION	= 20,		///< ��Ȱ..
};




class CObjCHAR;
class CCommand;

enum SKILL_SLOT_TYPE
{
	SKILL_SLOT_MAIN = 0,
	SKILL_SLOT_CLAN,
};

//////////////////////////////////////////////////////////////////////////////////////
/// CSkill abstraction class for Skill system
//////////////////////////////////////////////////////////////////////////////////////
class CSkill : public CTObject
{
protected:
	bool							m_bEnable;						///Ȱ��ȭ / ��Ȱ��ȭ
	short							m_nClanSkillIndex;				///Ŭ����ų�ϰ�� SkillIndex, ���� ��ų�ϰ�쿡�� CSkillSlot���� �����´�.
	int								m_iSkillLevel;
	int								m_iSkillDelayTimer;			///< �����ð�..

	int								m_iSkillSlotType;
	int								m_iSkillSlot;
	DWORD							m_dwExpiredTime;			

	/// Skill Creation type
	BYTE							m_btSkillTargetType;
	BYTE							m_btSkillActionType;
	BYTE							m_btSkillFilterType;

	

	//----------------------------------------------------------------------------------------------------	
	/// @brief Skill Command object
	//----------------------------------------------------------------------------------------------------
	CCommand*						m_pCommand;



	//----------------------------------------------------------------------------------------------------
	/// For skill Time
	//----------------------------------------------------------------------------------------------------
	int								m_iElapsedTime;

public:
	CSkill();
	~CSkill();


	//----------------------------------------------------------------------------------------------------
	/// @param
	/// @brief Skill Action interface
	//----------------------------------------------------------------------------------------------------
	virtual void	Action( CObjCHAR* pAttacker, CObjCHAR* pTarget ) = 0{ *(int*)0 = 10; };
	virtual void	DrawIcon( int iX, int iY );
	virtual int		Process();
	virtual bool	Execute();
	virtual int		SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) ) = 0{ *(int*)0 = 10; };	

	

	//----------------------------------------------------------------------------------------------------
	/// @param
	/// @brief get/set Skill slot
	//----------------------------------------------------------------------------------------------------
	void			SetSkillSlot( int iSlot );	
	int				GetSkillSlot( ){ return m_iSkillSlot; }


	//----------------------------------------------------------------------------------------------------
	/// @param
	/// @brief Skill index method
	//----------------------------------------------------------------------------------------------------
	short			GetSkillIndex();

	//----------------------------------------------------------------------------------------------------
	/// @param
	/// @brief Skill Level.. ����� ��ų�� ������ �ٲ�°���� ������� �ʴ´�.
	//----------------------------------------------------------------------------------------------------
	short			GetSkillLevel();

	
	//----------------------------------------------------------------------------------------------------
	/// @param
	/// @brief Skill timer method
	//----------------------------------------------------------------------------------------------------
	void			ResetSkillTimer(){ m_iSkillDelayTimer = 0;}
	void			SetSkillDelayTime( int iTime );	
	int				GetSkillDelayTime(){ return m_iSkillDelayTimer; }


	//----------------------------------------------------------------------------------------------------
	/// @param
	/// @brief ��ųŸ���� ( target type, action type, filter type �� �������� ���� )
	//----------------------------------------------------------------------------------------------------
	bool			Create( int iSkillType, short nSkillIdx, short nSkillType, short nSkillLevel = 0 );


	//----------------------------------------------------------------------------------------------------
	/// @brief Ŭ�� ��ų�����ؼ� ���Ѱɸ� ��ų�� ���� Methods
	//----------------------------------------------------------------------------------------------------
	bool			HasExpiredTime();
	DWORD			GetExpiredTime();
	void			SetExpiredTime( DWORD dwAbsSec );
	//	bool			HasLimitUseCount();
//	bool			HasLimitClass();


//	int				GetLimitUseCount();
//	int				GetLimitClass();

	void			SetEnable();
	void			SetDisable();
	bool			IsEnable();
};







//////////////////////////////////////////////////////////////////////////////////////
/// Base Action Skill
//////////////////////////////////////////////////////////////////////////////////////
class CBaseActionSkill : public CSkill
{
private:
	

public:
	CBaseActionSkill();
	~CBaseActionSkill();

	/*
	*	Skill Action interface
	*/
	virtual void Action( CObjCHAR* pAttacker, CObjCHAR* pTarget );
	virtual void DrawIcon( int iX, int iY );
	virtual int Process();
	virtual int SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
};






//////////////////////////////////////////////////////////////////////////////////////
/// Action create window Skill
//////////////////////////////////////////////////////////////////////////////////////
class CCreateWindowActionSkill : public CSkill
{
private:

public:
	CCreateWindowActionSkill();
	~CCreateWindowActionSkill();

	/*
	*	Skill Action interface
	*/
	virtual void Action( CObjCHAR* pAttacker, CObjCHAR* pTarget );
	virtual void DrawIcon( int iX, int iY );
	virtual int Process();
	virtual int SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
};







//////////////////////////////////////////////////////////////////////////////////////
/// Action immediate Skill
//////////////////////////////////////////////////////////////////////////////////////
class CImmediateActionSkill : public CSkill
{
private:
	
public:
	CImmediateActionSkill();
	~CImmediateActionSkill();

	/*
	*	Skill Action interface
	*/
	virtual void Action( CObjCHAR* pAttacker, CObjCHAR* pTarget );
	virtual void DrawIcon( int iX, int iY );
	virtual int Process();
	virtual int SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
};






//////////////////////////////////////////////////////////////////////////////////////
/// Fire Skill
//////////////////////////////////////////////////////////////////////////////////////
class CFireSkill : public CSkill
{
private:
	/// Bullet list
	/// ��¥�� Bullet List �� ��� �ɲ��⿡ ���� �ʿ� ������ ����..
	std::list< CBullet* >			m_BulletList;

public:
	CFireSkill();
	~CFireSkill();

	/*
	*	Skill Action interface
	*/
	virtual void Action( CObjCHAR* pAttacker, CObjCHAR* pTarget );
	virtual void DrawIcon( int iX, int iY );
	virtual int Process();
	virtual int SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
};





//////////////////////////////////////////////////////////////////////////////////////
/// Fire to position Skill
//////////////////////////////////////////////////////////////////////////////////////
class CFireToPositionSkill : public CSkill
{
private:
	/// Bullet list
	/// ��¥�� Bullet List �� ��� �ɲ��⿡ ���� �ʿ� ������ ����..
	std::list< CBullet* >			m_BulletList;

public:
	CFireToPositionSkill();
	~CFireToPositionSkill();

	/*
	*	Skill Action interface
	*/
	virtual void Action( CObjCHAR* pAttacker, CObjCHAR* pTarget );
	virtual void DrawIcon( int iX, int iY );
	virtual int Process();
	virtual int SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
};







//////////////////////////////////////////////////////////////////////////////////////
/// Self Bound Skill
/// SKILL_ACTION_SELF_BOUND_DURATION = 8,	///< �ڽſ��� �ߵ� ���� ����.( ĳ���� �� ) �ɷ�ġ 
/// SKILL_ACTION_SELF_BOUND = 10,			///< �ڽſ��� �ߵ� �ٷ� �� ����.( ĳ���� �� ) �ɷ�ġ
/// SKILL_ACTION_SELF_STATE_DURATION = 12,		///< �ڽſ��� �ߵ� ���� ����.( ĳ���� �� ) ���°���
//////////////////////////////////////////////////////////////////////////////////////
class CSelfBoundSkill : public CSkill
{
private:

public:
	CSelfBoundSkill();
	~CSelfBoundSkill();

	/*
	*	Skill Action interface
	*/
	virtual void Action( CObjCHAR* pAttacker, CObjCHAR* pTarget );
	virtual void DrawIcon( int iX, int iY );
	virtual int Process();
	virtual int SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
};



//////////////////////////////////////////////////////////////////////////////////////
/// Self Bound Skill
/// SKILL_ACTION_TARGET_BOUND_DURATION = 9,	///< Ÿ�ٿ��� �ߵ� ���� ����.( ĳ���� �� ) �ɷ�ġ
/// SKILL_ACTION_TARGET_BOUND = 11,			///< Ÿ�ٿ��� �ߵ� �ٷ� �� ����.( ĳ���� �� ) �ɷ�ġ
/// SKILL_ACTION_TARGET_STATE_DURATION = 13,	///< ��뿡�� �ߵ� ���� ����.( ĳ���� �� ) ���°���
//////////////////////////////////////////////////////////////////////////////////////
class CTargetBoundSkill : public CSkill
{
private:

public:
	CTargetBoundSkill();
	~CTargetBoundSkill();

	/*
	*	Skill Action interface
	*/
	virtual void Action( CObjCHAR* pAttacker, CObjCHAR* pTarget );
	virtual void DrawIcon( int iX, int iY );
	virtual int Process();
	virtual int SendSkillActionReq( int iSkillSlot, int iTarget, D3DXVECTOR3 &PosTO = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
};







//////////////////////////////////////////////////////////////////////////////////////
///
/// Skill manager
///
//////////////////////////////////////////////////////////////////////////////////////
class CSkillManager
{
private:
	std::list< CSkill* >			m_SkillList;


public:
	CSkillManager();
	~CSkillManager();


	//---------------------------------------------------------------------------------
	/// static function

	static int	GetSkillTargetType( int iSkillIndex );
	static int	GetSkillActionType( int iSkillIndex ){ return SKILL_TYPE( iSkillIndex ); }
	static int	GetSkillFilterType( int iSkillIndex ){ return SKILL_CLASS_FILTER( iSkillIndex ); }

	
	/// ��ųŸ���� ( target type, action type, filter type �� �������� ���� )
	CSkill*		CreateNewSkill( int iSkillSlotType, short nSkillIdx, char cSkillLevel = 0 );


	/// �ϴ� ����.. XP �� �����϶�.. !!!
	static bool		ActionSkill( short nSkillIdx, CObjCHAR* pSrc, CObjCHAR* pTarget, D3DXVECTOR3 PosTo = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), bool bTargetPos = false );

	
	/// ��ų �ߵ� ���� üũ..
	static bool		CheckConditionForFireSkill( int iSkillIDX, int iCurrentTarget );
	/// ��ų �ߵ�..
	static bool		FireSkill( int iSkillSlotIDX, int iTargetObj, D3DXVECTOR3 &PosTO = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	/// ���� Ȱ��ȭ�� ��ų �ߵ�..
	static bool		ActionActiveSkill( int iTargetObj, D3DXVECTOR3 &PosTO = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );



	/// �Ҹ�� �Ӽ�
	static void UpdateUseProperty( CObjAI* pObjAI, int iSkillIdx );	


	///
	///	��ų ��밡�� ���� üũ..
	///
	
	static bool	CheckCastingCondition( int iSkillIDX, CObjCHAR* pCaster, int iTargetObjIDX );

	/// Ÿ�̸� üũ( ���� ��뿩�ΰ��� )..
	static bool CheckSkillTimer( int iSkillIDX );
	/// Ÿ���� ��ȿ�� �˻�..
	static bool CheckCastingTarget( int iSkillIDX, CObjCHAR* pCaster, int iTargetObjIDX );

	/// Ÿ�� ���͸� �˻�
	static bool CheckCastingTargetFilter( int iSkillIDX, CObjCHAR* pCaster, int iTargetObjIDX );

	/// ��ȯ�� capacity �˻�
	static bool CheckSummonCapacity( int iSkillIDX, CObjCHAR* pCaster, int iTargetObjIDX );

	/// �ʿ� �ɷ�ġ üũ..
	static bool CheckNeedProperty( int iSkillIDX, CObjCHAR* pCaster );
	/// �ʿ���� üũ..
	static bool CheckNeedWeapon( int iSkillIDX, CObjCHAR* pCaster );
	/// �ߺ����� ���� üũ...
	static bool CheckDuplicatable( int iSkillIDX, CObjCHAR* pCaster, int iTargetObjIDX );
	
	
	///
	///	��ų ���氡�� ���� üũ..
	///

	/// ��ų�� ���⿡ ��ȿ�� �����ΰ�?
	static bool CheckJobForStudy( int iSkillIDX );
	/// ��ų�� ���� ���� ���� ��ų�� �����ϴ°�?
	static bool CheckProSkillForStudy( int iSkillIDX );
	/// ��ų�� ���� ���� �⺻ �ɷ�ġ�� �����ϴ°�?
	static bool CheckPropertyForStudy( int iSkillIDX );


	///
	///	��ų ����� ���� Ÿ�� ���ϱ�
	/// �Ϲ� ��ų�ϰ�� hpüũ�� ������ �������ǰ��� ��ų�� ��� hp üũ�� ���� �ʴ´�.
	///
	static CObjCHAR* GetSkillTarget( int iServerObjIDX, int iSkillIDX );

};

extern CSkillManager	g_SkillManager;

#endif
