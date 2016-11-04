#ifndef	__IO_SKILL_H
#define	__IO_SKILL_H

#include "IO_STB.h"
#ifndef __SERVER
	#include "../GameCommon/StringManager.h"
#endif
//-------------------------------------------------------------------------------------------------

enum {
	SKILL_TYPE_01 = 1,
	SKILL_TYPE_02 ,
	SKILL_TYPE_03 ,
	SKILL_TYPE_04 ,
	SKILL_TYPE_05 ,
	SKILL_TYPE_06 ,
	SKILL_TYPE_07 ,
	SKILL_TYPE_08 ,
	SKILL_TYPE_09 ,
	SKILL_TYPE_10 ,
	SKILL_TYPE_11 ,
	SKILL_TYPE_12 ,
	SKILL_TYPE_13 ,
	SKILL_TYPE_14 ,
	SKILL_TYPE_15 ,							// �нú� ��ų
	SKILL_TYPE_PASSIVE = SKILL_TYPE_15,
	SKILL_TYPE_16 ,							// Emotion
	SKILL_TYPE_17 ,							// Self & damage
	SKILL_TYPE_18 ,							/// �߰� 2004.4.8 : warp skill
	SKILL_TYPE_19,							/* �߰� 2004.6.16 :
											�ɷ�ġ ����(10������) & ����������(3������)
													������ 3����ų �������� Ÿ�ٿ��� �������� �ְ�, �ڽſ��Դ�
													������ �ɷ�ġ(21��,24�� Į��)�� ��ġ(22,25��Į��)�� �����ȴ�.
													��ų���������� �����Ͽ� �����ÿ��� �ɷ�ġ ������ �����Ų��.
													(Ÿ�ٿ� ������������ �ϸ鼭 HP,MP ������� ����)
											*/
	SKILL_TYPE_20,
} ;


enum enumSKILL_TAGER_FILTER
{
	SKILL_TARGET_FILTER_SELF = 0,			///< �ڱ� �ڽ�
	SKILL_TARGET_FILTER_GROUP = 1,			///< �׷�(��Ƽ)��
	SKILL_TARGET_FILTER_GUILD = 2,			///< ����
	SKILL_TARGET_FILTER_FRIEND_ALL = 3,		///< �Ʊ� (�ƹ�Ÿ, ��ȯ��)
	SKILL_TARGET_FILTER_MOB = 4,			///< ��� ���� (��ȯ��,������)
	SKILL_TARGET_FILTER_ENEMY_ALL = 5,		///< �� PC + ���� 	
	SKILL_TARGET_FILTER_ENEMY_PC = 6,		///< �� PC( �� ���� ���� )
	SKILL_TARGET_FILTER_ALL_PC = 7,			///< ��� PC
	SKILL_TARGET_FILTER_ALL_CHAR = 8,		///< ��� ĳ����( ����, �÷��̾� )	
	SKILL_TARGET_FILTER_DEAD_USER = 9,		/// ���� �����
	SKILL_TARGET_FILTER_ENEMY_MOB = 10,		/// �� ����
	SKILL_TARGET_FILTER_MAX,
};

//-------------------------------------------------------------------------------------------------

#ifdef __SERVER
	#define	SKILL_NAME(I)						g_SkillList.m_SkillDATA.m_ppNAME[ I ]			// ��ų �̸�
	#define SKILL_DESC(I)						g_SkillList.m_SkillDATA.m_ppDESC[ I ]			// ��ų ����
#else
	#define	SKILL_NAME(I)						CStringManager::GetSingleton().GetSkillName( I )
	#define SKILL_DESC(I)						CStringManager::GetSingleton().GetSkillDesc( I )
#endif

#define	SKILL_1LEV_INDEX(S)					g_SkillList.m_SkillDATA.m_ppDATA[ S ][  1 ]
#define	SKILL_LEVEL(S)						g_SkillList.m_SkillDATA.m_ppDATA[ S ][  2 ]
#define	SKILL_NEED_LEVELUPPOINT(S)			g_SkillList.m_SkillDATA.m_ppDATA[ S ][  3 ]
#define	SKILL_TAB_TYPE(S)					g_SkillList.m_SkillDATA.m_ppDATA[ S ][  4 ]
#define SKILL_TYPE( I )						g_SkillList.m_SkillDATA.m_ppDATA[ I ][  5 ]

#define SKILL_DISTANCE( I )					g_SkillList.m_SkillDATA.m_ppDATA[ I ][  6 ]		//	Ÿ�ٰ��� �Ÿ�
#define SKILL_WARP_PLANET_NO( I )			g_SkillList.m_SkillDATA.m_ppDATA[ I ][  6 ]		//	���� ��ų�� ��� ���� ��ġ�� �༺ ��ȣ

#define SKILL_CLASS_FILTER( I )				g_SkillList.m_SkillDATA.m_ppDATA[ I ][  7 ]	
#define	SKILL_SCOPE(S)						g_SkillList.m_SkillDATA.m_ppDATA[ S ][  8 ]		// �������
#define	SKILL_POWER(S)						g_SkillList.m_SkillDATA.m_ppDATA[ S ][  9 ]
#define	SKILL_ITEM_MAKE_NUM		SKILL_POWER	// ������ȣ
#define SKILL_HARM( I )						g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 10 ]	

#define SKILL_STATE_STB( I,C )				g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 11+C ]	//	���� ��ȣ
#define SKILL_STATE_STB1( I )				g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 11 ]		//	���� ��ȣ
#define SKILL_STATE_STB2( I )				g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 12 ]		//	���� ��ȣ
#define SKILL_SUCCESS_RATIO( I )			g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 13 ]		//	������
#define SKILL_DURATION( I )					g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 14 ]		//	���� �ð�
#define	SKILL_DAMAGE_TYPE(S)				g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 15 ]		// ����� ���� ����


#define	SKILL_USE_PROPERTY_CNT				2
#define	SKILL_USE_PROPERTY(S,T)				g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 16+(T)*2 ]
#define	SKILL_USE_VALUE(S,T)				g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 17+(T)*2 ]


#define	SKILL_RELOAD_TIME(S)				g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 20 ]

// �нú�(���Ӽ�) ��ų�� ��� �����ð� ��½�ų �ɷ�ġ...
#define	SKILL_INCREASE_ABILITY_CNT			2
#define	SKILL_INCREASE_ABILITY(S,T)			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 21+(T)*3 ]
#define	SKILL_INCREASE_ABILITY_VALUE(S,T)	g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 22+(T)*3 ]
#define	SKILL_CHANGE_ABILITY_RATE(S,T)		g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 23+(T)*3 ]

#define	SKILL_WARP_ZONE_NO( S )				g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 21 ]		// ���� ��ų�� ��� ������ �� ��ȣ
#define	SKILL_WARP_ZONE_XPOS( S )			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 22 ]		// ���� ��ų�� ��� ������ �� ��ġ
#define	SKILL_WARP_ZONE_YPOS( S )			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 23 ]

#define	SKILL_RELOAD_TYPE(S)				g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 27 ]		// ��ų����

#define	SKILL_SUMMON_PET(S)					g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 28 ]		//	��ȯ�� ��ȣ

#define	SKILL_ACTION_MODE(S)				g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 29 ]		// ��ų���� ����
// ���� �����ϰ� �־���ϴ� ���.  T = 0 ~ 3
#define	SKILL_NEED_WEAPON_CNT				5		// <--- 2004. 2. 26 4->5�� �ø��� ���� �÷� ��ȣ+1 by icarus
#define	SKILL_NEED_WEAPON(S,T)				g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 30+(T) ]

/*
// ���� ��ﶧ �ʿ��� ����.	T = 0 ~ 3
#define	SKILL_AVAILBLE_CLASS_CNT			4
#define	SKILL_AVAILBLE_CLASS(S,T)			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 1+34+(T) ]
*/
// LIST_CLASS.stb ���� ��ȣ...
#define	SKILL_AVAILBLE_CLASS_SET(S)			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 35 ]

// ���� ��ﶧ �Ҽ� ���� üũ...
#define	SKILL_AVAILBLE_UNION_CNT			3
#define	SKILL_AVAILBLE_UNION(S,T)			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 36+(T) ]


// ���ʷ� ��ﶧ �̸� �����ϰ� �־���ϴ� ��ų. T = 0 ~ 2
#define	SKILL_NEED_SKILL_CNT				3
#define	SKILL_NEED_SKILL_INDEX(S,T)			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 39+(T)*2 ]
#define	SKILL_NEDD_SKILL_LEVEL(S,T)			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 40+(T)*2 ]

// ���ʷ� ���ų� �������� �ʿ��� ���� �ɷ�ġ. T = 0 ~ 2
#define	SKILL_NEED_ABILITY_TYPE_CNT			2
#define	SKILL_NEED_ABILITY_TYPE(S,T)		g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 45+(T)*2 ]	// ���� AT_DEX~AT_SENSE �� ����
#define	SKILL_NEED_ABILITY_VALUE(S,T)		g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 46+(T)*2 ]

#define SKILL_SCRIPT1( I )					g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 49 ]		

#define SKILL_RESERVE_02( I )				g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 50 ]		
#define SKILL_ICON_NO( I )					g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 51 ]		

// ĳ���� ����
#define	SKILL_ANI_CASTING(S)				g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 52 ]
#define	SKILL_ANI_CASTING_SPEED(S)			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 53 ]

/// ĳ���� ���� ����
#define	SKILL_ANI_CASTING_REPEAT(S)			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 54 ]
#define	SKILL_ANI_CASTING_REPEAT_CNT(S)		g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 55 ]

/// ĳ���� ����Ʈ
#define SKILL_CASTING_EFFECT_CNT			4
#define SKILL_CASTING_EFFECT( I,T )			g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 56 + (T * 3) ]
#define SKILL_CASTING_EFFECT_POINT( I,T )	g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 57 + (T * 3) ]
#define SKILL_CASTING_SOUND( I,T )			g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 58 + (T * 3) ]

// ���� ����
#define	SKILL_ANI_ACTION_TYPE(S)			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 68 ]
#define	SKILL_ANI_ACTION_SPEED(S)			g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 69 ]
#define	SKILL_ANI_HIT_COUNT(S)				g_SkillList.m_SkillDATA.m_ppDATA[ S ][ 70 ]

/// �߻� �Ѿ� ����Ʈ
#define SKILL_BULLET_NO( I )				g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 71 ]
#define SKILL_BULLET_LINKED_POINT( I )		g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 72 ]
#define SKILL_BULLET_FIRE_SOUND( I )		g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 73 ]

/// Ÿ�� ����Ʈ
#define SKILL_HIT_EFFECT( I )				g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 74 ]			//	Ÿ�� ȿ��
#define SKILL_HIT_EFFECT_LINKED_POINT( I )	g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 75 ]			//	Ÿ�� ȿ��
#define SKILL_HIT_SOUND( I )				g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 76 ]			//	Ÿ�� ȿ����

/// ��Ÿ ���� ����Ʈ
#define SKILL_HIT_DUMMY_EFFECT_CNT			2
#define SKILL_HIT_DUMMY_EFFECT( I, T )					g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 77 + 3*T ]			//	���� Ÿ�� ȿ��
#define SKILL_HIT_DUMMY_EFFECT_LINKED_POINT( I, T )		g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 78 + 3*T ]			//	���� Ÿ�� ȿ��
#define SKILL_HIT_DUMMY_SOUND( I, T )					g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 79 + 3*T ]			//	���� Ÿ�� ȿ����

#define SKILL_AREA_HIT_EFFECT( I )			g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 83 ]								//	���� Ÿ�� ȿ��
#define SKILL_AREA_HIT_SOUND( I )			g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 84 ]								//	���� Ÿ�� ȿ����

#define	SKILL_LEVELUP_NEED_ZULY( I )		g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 85 ]				


#define	SKILL_ATTRIBUTE( I )				g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 86 ]///1 : �ƹ�Ÿ ��ų , 2: PAT��ų , 3: ���� ��ų

#define SKILL_ATTRIBUTE_AVATAR				1
#define SKILL_ATTRIBUTE_CART				2
#define SKILL_ATTRIBUTE_CASTLEGEAR			4

//	��ų �������� �Ҹ� ��� / 100
//#define SKILL_DURATION_EFFECT( I )			g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 66 ]		//	���Ӹ��� ȿ��
//#define SKILL_DURATION_SOUND( I )			g_SkillList.m_SkillDATA.m_ppDATA[ I ][ 67 ]		//	���Ӹ��� ȿ����


/*
#define	SA_SELF_STOP							0		// ��ų ������ ����
#define	SA_TARGET_STOP							1		// Ÿ�� ����, ��ų ������ ���� 
#define	SA_TARGET_ATTACK						2		// Ÿ�� ����, ��ų ������ Ÿ���� ����
#define	SA_POSITION_STOP						3		// ������ ��ų ������ ����
*/
#define	SA_STOP						0	// ��ų ������ ����
#define	SA_ATTACK					1	// ��ų ������ Ÿ���� ����
#define	SA_RESTORE					2	// ��ų ����� ���� ��� ����

#ifdef	__SERVER
	#define	SKILL_RELOAD_SECOND(S)			g_SkillList.m_pReloadTIME[ S ]
#endif

class CSkillLIST 
{
private:
	int			m_iSkillCount;	

	float	   *m_pCastingAniSPEED;		/// �̸� ���Ǿ��� ��ų �ɽ��� ���� �ӵ�
	float	   *m_pActionAniSPEED;		/// �̸� ���Ǿ��� ��ų ���� ���� �ӵ�.

public :
	STBDATA		m_SkillDATA;
#ifdef	__SERVER
	DWORD	  *m_pReloadTIME;
#else
	float	  *m_pReloadTIME;
#endif

	CSkillLIST ();
	~CSkillLIST ();
	void	Free ();

	int		Get_SkillCNT()							{	return m_iSkillCount;					}
	float	Get_CastingAniSPEED(short nSkillIDX)	{	return m_pCastingAniSPEED[ nSkillIDX ];	}
	float	Get_ActionAniSPEED(short nSkillIDX)		{	return m_pActionAniSPEED[ nSkillIDX ];	}

	bool	LoadSkillTable( const char* pFileName );
	float  GetDelayTickCount(DWORD i);
} ;

extern CSkillLIST g_SkillList;

//-------------------------------------------------------------------------------------------------
#endif
