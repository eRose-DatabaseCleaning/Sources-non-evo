#ifndef _CENDURANCEPROPERTY_
#define _CENDURANCEPROPERTY_

#include <list>
#include <map>
#include <algorithm>
#include "../Common/IO_STB.h"

class CObjCHAR;
class CEffect;

enum ENDURANCE_ENTITY_TYPE
{
	ENDURANCE_TYPE_SKILL = 0,
	ENDURANCE_TYPE_USEITEM,
};



#define MAX_ENDURAMCE_ENTITY	5

class CEnduranceProperty;


typedef std::vector< CEnduranceProperty* >					ENDURANCE_LIST;
typedef std::vector< CEnduranceProperty* >::iterator		ENDURANCE_LIST_ITOR;
///
/// Endurance entity pack.
/// ���������� ��ġ�� ���ϴ°͵��� �� Ŭ������ proc ���� ó���ϰ�
/// ��Ÿ ȭ�� ��¿��̳� ǥ�ÿ��� ���� ��ƼƼ ����Ʈ�� �̿��Ѵ�.

class CEndurancePack
{
private:
	/// ���� ������ ���� �÷���..
	DWORD									m_dwStateFlag;
	/// ���� ���� �߰��Ǿ���� ������ ����..( key �� ���°� ��. )
	int										m_CurrentStateValue[ ING_MAX_STATUS ];

	DWORD									m_dwUpdateCheckTime;
	DWORD									m_dwElapsedUpdateTime;


	/// ���º��� Ÿ�Ը�ŭ �����Ѵ�.
	CEnduranceProperty*						m_EntityList[ ING_MAX_STATUS ];	
	CObjCHAR*								m_pObjCHAR;



public:
	/// ���� �÷��װ����� ����Ÿ���� ã�Ƴ��� ���̺�
	static std::map< int , int >			m_StateFlagTable;	
	static std::map< int , int >			m_StateSTBIDXTable;	

	void									MakeStateFlagTable();
	void									MakeStateSTBIDXTable();


private:
	CEndurancePack();


public:	
	CEndurancePack( CObjCHAR* pObjCHAR );
	~CEndurancePack();


	void					ResetTimer();



	void					SetObjCHAR( CObjCHAR* pObjCHAR ){ m_pObjCHAR = pObjCHAR; }

	int						GetEntityCount(){ return ING_MAX_STATUS; }
	CEnduranceProperty*		GetEntity( int Index );
	CEnduranceProperty*		GetEntityByStateType( int iStateType );


	/// add new endurance entity.
	bool					AddEntity( int iEntityIDX, int iStateNO, int iEnduranceTime, int iEntityType );
    void					ClearEntityPack();

	void					Update();
	bool					Proc( CEnduranceProperty* pEntity );
	void					DeleteExpiredEntity();
	void					Draw();

	/// �ش� ���¿� ���� ��ƼƼ�� �����Ѵ�.
	void					DeleteEntityByStateType( int iStateType );

	/// ��밪 �����Լ�
	void					SetStateFlag( DWORD dwState ){ m_dwStateFlag = dwState; }
	void					AddStateFlag( DWORD dwState ){ m_dwStateFlag |= dwState; }
	DWORD					GetStateFlag(){ return m_dwStateFlag; }
	void					SetStateValue( int iType, int iValue );//{ m_CurrentStateValueMap[ iType ] = iValue; }
	int						GetStateValue( int iType )
	{
		/*if( std::find( m_CurrentStateValueMap.begin(), m_CurrentStateValueMap.end(), iType ) == m_CurrentStateValueMap.end() )
			return 0;*/

		return m_CurrentStateValue[ iType ]; 
	}
	void					ClearState(){ m_dwStateFlag = 0; memset( m_CurrentStateValue, 0, sizeof( m_CurrentStateValue ) ); }

	/// ���� ����
	void					DeleteEnduranceState( DWORD dwNewState );

	/// �������� ����, �Ҹ����� ���� ��..
	int						ProcFlushStateSkill( int iStateTableIndex );
	void					ProcFlushStateSkillByIngState( int iStateType );

	
	//--------------------------------------------------------------------------------------
	/// Ư�� ��Ȳ���� �ڵ����� Ǯ���� �ϴ°͵�
	//--------------------------------------------------------------------------------------

	/// īƮŽ�½� Ǯ���� �� ��ų
	void					ClearStateByDriveCart();

	/// ���ü�� �ش� ��� �ɷ��ִ� ���� ����..
	void					ClearRWeaponSkillEffect();
	void					ClearLWeaponSkillEffect();
	
	/// �¾����� �ڵ����� Ǯ�����ϴ� �͵�
	void					ClearStateByHitted();

	/// ���ݽ� �ڵ����� Ǯ�����ϴ� ��ų��
	void					ClearStateByAttack();

	/// �̵��� �ڵ����� Ǯ�����ϴ� ��ų��
	void					ClearStateByMove();



	/// Check visibility
	/// visibility ���� ��ų�� �ɷ������.. ������Ʈ �ؾߵɽñ⿡��( ���⸦ ��ü�Ѵٴ��� ) ��������Ƽ ������Ʈ
	void					UpdateVisibility();


public:
	/// ��� ������ ����..
	static int								m_iDrawIconCnt;

};





///
/// Base class of endurance property entity
///

class CEnduranceProperty
{
protected:
	CObjCHAR*		m_pObjCHAR;
	CEffect*		m_pEffect;

	int				m_iEndityIdx;
	int				m_iEnduranceTime;
	int				m_iEnduranceEndityType;
	bool			m_bExpired;

	DWORD			m_iStartTime;

	/// ��������..
	int				m_iStatusSTBNO;
	
public:
	CEnduranceProperty();
	virtual ~CEnduranceProperty();

	virtual bool	CreateEnduranceEntity( CObjCHAR* pObjCHAR, int iEntityIdx, int iStatusSTBNO, int iEnduranceTime ) = 0;

	virtual void	SetExpire()			{ m_iEnduranceTime = 0; m_bExpired = true; }
	virtual bool	IsExpired()			{ return m_bExpired; }
		
	virtual int		GetEntityIDX()		{ return m_iEndityIdx; };
	virtual int		GetEnduranceTime()	{ return m_iEnduranceTime; }
	virtual int		GeteEntityType()	{ return m_iEnduranceEndityType; };
	virtual int		GetStartTime()		{ return m_iStartTime; }
	virtual int		GetStateSTBNO()		{ return m_iStatusSTBNO; }

	virtual bool	CreateEffect( CObjCHAR* pObjCHAR, int iEffectIDX ) = 0{ *(int*)0 = 10; };
	virtual void	DeleteEffect() = 0{ *(int*)0 = 10; };

	virtual int		GetIconNO() = 0{ *(int*)0 = 10; };

	/// if return value is false, this endurance entity is expired.
	virtual bool	Proc( CObjCHAR* pObjCHAR ) = 0{ *(int*)0 = 10; };
};


///
/// Endurance skill class
///

class CEnduranceSkill : public CEnduranceProperty
{
private:
		
public:	
	CEnduranceSkill();
	//CEnduranceSkill( CObjCHAR* pObjCHAR, int iSkillIdx, int iEnduranceTime );
	~CEnduranceSkill();	
	
	virtual bool	CreateEnduranceEntity( CObjCHAR* pObjCHAR, int iSkillIdx, int iStateNO, int iEnduranceTime );
	virtual bool	CreateEffect( CObjCHAR* pObjCHAR, int iEffectIDX );
	virtual void	DeleteEffect();

	virtual int		GetIconNO();

	bool			Proc( CObjCHAR* pObjCHAR );
};


///
/// Endurance item class
///

class CEnduranceItem : public CEnduranceProperty
{
private:	
	/// ������� ����� ��ġ..
	int				m_iTotalAppliedProperty[ STATE_APPLY_ABILITY_CNT ];
	
public:
	CEnduranceItem();
	//CEnduranceItem( CObjCHAR* pObjCHAT, int iUseItemIdx, int iEnduranceTime );
	~CEnduranceItem();

	virtual bool	CreateEnduranceEntity( CObjCHAR* pObjCHAR, int iUseItemIdx, int iStateNO, int iEnduranceTime );
	virtual bool	CreateEffect( CObjCHAR* pObjCHAR, int iEffectIDX );
	virtual void	DeleteEffect();

	virtual int		GetIconNO();

	bool			Proc( CObjCHAR* pObjCHAR );
};



#endif // _CENDURANCEPROPERTY_	