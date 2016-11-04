#ifndef	__IO_QUEST_H
#define	__IO_QUEST_H
#include "../util/classSTR.h"
#include "../util/classHASH.h"
#include "IO_STB.h"
#include "CUserDATA.h"///�߰� 2004 / 2 /19 : �߰� nAvy



#pragma warning( disable : 4200 )
//-------------------------------------------------------------------------------------------------

#define	QST_VARTYPE_VAR			0x0000
#define	QST_VARTYPE_SWITCH		0x0100
#define	QST_VARTYPE_TIMER		0x0200
#define	QST_VARTYPE_EPISODE		0x0300
#define	QST_VARTYPE_JOB			0x0400
#define	QST_VARTYPE_PLANET		0x0500
#define	QST_VARTYPE_UNION		0x0600

/// STR_COND_001, STR_REWD_002�� �ʿ��� Structure. Quest������ ���� üũ�� ����
struct STR_QUEST_DATA
{
	union {
		int		iType;	/// 0x00000000~0x00000010 ==> QuestVar0 ~ QuestVar9, 
						/// 0x01000000~0x0100001F ==> switch0 ~ switch32
						/// 0x02000000 ==> ����Ʈ Ÿ�̸Ӹ� �ǹ���
						/// 0x03000000 ~ 0x03000004 ==> ���Ǽҵ� ���ຯ�� 5��
						/// 0x04000000 ~ 0x04000002 ==> �������ຯ�� 3��
						/// 0x05000000 ~ 0x05000006 ==> �༺�� ���ຯ�� 7��
						/// 0x06000000 ~ 0x06000009 ==> ���պ� ���ຯ�� 10��
		struct {
			WORD	m_wVarNO;
			WORD	m_wVarTYPE;
		} ;
	} ;

	// �񱳴� :: ������ op nValue
	short	nValue;	/// ���� �����Ͱ�. 0~255, ����ġ�� ��� 0, 1 (iType�� �ش��ϴ� �� Op btValue)
	BYTE	btOp;	/// iData���� ���� �񱳹��. 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����, 
					/// (���� �׼���) 5 = ���ٲ�, 6 = ����(�־��� ��ŭ), 7 = ����, 8 = ����ġ Off, 9 = ����ġ On
};


/// STR_COND_003, STR_REWD_004 �� �ʿ��� Structure. �ɷ�ġ üũ�� ���Ǵ� ������
struct STR_ABIL_DATA
{
	int		iType;	/// enum t_AbilityINDEX�� ����

	int		iValue;	/// ���� �����Ͱ� (iType�� �ɷ�ġ Op iValue)
	BYTE	btOp;	/// iData���� ���� �񱳹��. 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
					/// (�׼���) 5 = ���ٲ�, 6 = ����, 7 = ����
};


/// STR_COND_004�� �ʿ��� Structure. �Ѱ� Itemüũ ������
struct STR_ITEM_DATA
{
	unsigned int uiItemSN;	/// ������ ��ȣ (��5�ڸ� = ��2�ڸ�(�����ۺз�) + ��3�ڸ�(�����۹�ȣ)
							/// ����Ʈ �������� �����ۺз��� 13
	int		iWhere;			/// ������ ������ġ (enum t_EquipINDEX�� ����)
							/// EQUIP_IDX_NULL �̸� �κ��丮�� �ִ����� üũ
	int		iRequestCnt;	/// üũ�� ����. (������������ ���� 1) (iTemSN�ǰ��� Op iRequestCnt)
	BYTE	btOp;			/// iRequestCnt�� ���� OP.  0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
};


//-------------------------------------------------------------------------------------------------

struct StrHeader 
{
	unsigned int	uiSize;
	int				iType;
} ;

/****************************************************************************
 * ���� ����Ʈ�� �������ΰ�
 */
struct STR_COND_000
{
	unsigned int uiSize;	/// �� ��Ʈ������ ũ��
	int			 iType;		/// ����� Ÿ��

	int			 iQuestSN;	/// �˻��� ����Ʈ ��ȣ
} ;


/****************************************************************************
 * ����Ʈ ������ üũ
 */
struct STR_COND_001
{
	unsigned int	uiSize;			/// �� ��Ʈ������ ũ��
	int				iType;			/// ����� Ÿ��

	int				iDataCnt;		/// �ڿ� ��� �����Ͱ� �ִ���
	STR_QUEST_DATA	CheckData[ 1 ]; /// ������ �������̳�, ����ġ�� ���� �˻��ϴ� ������
} ;


/******************************************************************************
 * ����Ʈ ���� ���� üũ
 */ 
struct STR_COND_002
{
	unsigned int		uiSize;
	int					iType;

	int					iDataCnt;
	STR_QUEST_DATA		CheckData[ 1 ]; /// üũ�� ����Ʈ���� �����Ͱ� ���⿡ �迭��
};


/****************************************************************************
 * �ɷ�ġ üũ
 */
struct STR_COND_003
{
	unsigned int	uiSize;			/// �� ��Ʈ������ ũ��
	int				iType;			/// ����� Ÿ��

	int				iDataCnt;
	STR_ABIL_DATA	CheckData[ 1 ];
} ;


/****************************************************************************
 * ����/���� ������ üũ
 */
struct STR_COND_004
{
	unsigned int	uiSize;
	int				iType;

	int				iDataCnt;
	STR_ITEM_DATA	CheckData[ 1 ];
} ;


/****************************************************************************
 * ��Ƽ üũ
 */
struct STR_COND_005
{
	unsigned int	uiSize;
	int				iType;

	BYTE			btIsLeader;		/// ��Ƽ�����ΰ� ? 0 = �����ʿ� ����. 1 = ��Ƽ�������� üũ
	int				iLevel;			/// ��Ƽ���� ���̻� (�̻� ������)
	BYTE			btReversed;		/// ���ĸ� ���ؼ� �׳� �߰�. 2004.02.09����� ������ 0
};


/****************************************************************************
 * ��ġ üũ
 */
struct STR_COND_006
{
	unsigned int	uiSize;
	int				iType;

	int				iZoneSN;		/// ����ȣ
	int				iX;				/// X��ǥ
	int				iY;				/// Y��ǥ
	int				iZ;				/// ������ ���ؼ�. 2004.02.09����� ������ 0
	int				iRadius;		/// ������. (X,Y)�� �������� �Ͽ� ������ ���̳���..
};


/****************************************************************************
 * ���� �ð� üũ
 */
struct STR_COND_007
{
	unsigned int	uiSize;
	int				iType;

	unsigned long	ulTime;			/// 
	unsigned long	ulEndTime;		/// ���� 0�ø� �������� ����ð��� ulTime�̻� ulEndTime�����ΰ� ?
};


/****************************************************************************
 * �������Ʈ ���� �ܿ��ð� üũ
 */
struct STR_COND_008
{
	unsigned int	uiSize;
	int				iType;

	unsigned long	ulTime;			/// üũ�� �� (ulTime Op ����Ʈ��Ͻð�)
	BYTE			btOp;			/// ulTime�� ���� �񱳹��. 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
};


/// ��ų�� �����ϰ� �ִ��� üũ
struct STR_COND_009
{
	unsigned int	uiSize;
	int				iType;

	int				iSkillSN1;
	int				iSkillSN2;	/// iSkillSN1 ~ iSkillSN2 �� ��ų�� ������ �ִ°�/���°� ?
	BYTE			btOp;		/// 0 = ������ üũ, 1 = �ִ��� üũ
} ;


/// Ȯ��
struct STR_COND_010
{
	unsigned int	uiSize;
	int				iType;

	BYTE			btLowPcnt;	///
	BYTE			btHighPcnt; /// btLowPcnt <= Ȯ�� AND  Ȯ�� <= btHighPcnt
};

/// NPC ���� �� üũ
typedef struct	tagValueQST
{
	unsigned int	uiSize;
	int				Type;

	BYTE			btWho;	// 0:NPC, 1:EventOBJ

	short			nVarNo;	// ���� ��ȣ : 0 ~ , ���� ���������� ����
	int				iValue;	// ���� �����Ͱ� (iType�� �ɷ�ġ Op iValue)
	BYTE			btOp;	// iValue���� ���� �񱳹��. 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
							// (�׼���) 5 = ���ٲ�, 6 = ����, 7 = ����
} STR_COND_011, STR_REWD_011;

/// �̺�Ʈ ����
struct STR_COND_012
{
	unsigned int	uiSize;
	int				iType;

	short			iZone;	/// 04.03.40 �߰�...

	int				iX;
	int				iY;
	union {
		int			iEventID;
		t_HASHKEY	m_HashEventOBJ;
	} ;
};


/// NPC ����
struct STR_COND_013
{
	unsigned int	uiSize;
	int				iType;

//	short			nZoneNo;	/// 04.03.30 �߰�...
	int				iNpcNo; /// ������ NPC ��ȣ
};


/// Switch üũ
struct STR_COND_014
{
	unsigned int	uiSize;
	int				iType;

	short			nSN; // 0 ~ 255
	BYTE			btOp; // 0 = Off���� üũ, 1 = On���� üũ
};


/// ��Ƽ�ο� üũ. 
struct STR_COND_015
{
	unsigned int	uiSize;
	int				iType;

	short			nNumber1;
	short			nNumber2; // nNumber1 <= ��Ƽ���� <= nNumber2 �̸� true
							  // ����Ƽüũ�� 0 <= ��Ƽ���� <= 0 �� true �ΰ����� �Ǵ�
};

/// ���ð� üũ
struct STR_COND_016
{
	unsigned int	uiSize;
	int				iType;

	BYTE			btWho;		/// 0=���� ���õ� NPC, 1=���ǿ��� ������ Event, 2=�����

	unsigned long	ulTime;		//
	unsigned long	ulEndTime;	// ulTime <= ���� ���ð� <= ulEndTime �̸� true
};


struct STR_NPCVAR
{
	int				iNpcNo;
	short			nVarNo;
};

/// NPC ���� ��. (NPC1�� A���� OP NPC2�� C����)
struct STR_COND_017
{
	unsigned int	uiSize;
	int				iType;

	STR_NPCVAR		NpcVar1;			
	STR_NPCVAR		NpcVar2;			
	BYTE			btOp; // ���� .  <NpcVar1.nVarNo�� ��> btOp <NpcVar2.nVarNo�� ��>
};


/// ��¥ + �ð� üũ
struct STR_COND_018
{
	unsigned int	uiSize;
	int				iType;

	BYTE			btDate;		// ��¥. (1 ~ 31)

	BYTE			btHour1;	// �� (1 ~ 24)
	BYTE			btMin1;		// �� (1 ~ 60)
	BYTE			btHour2;	// ��
	BYTE			btMin2;		// ��
	//  ������ ��¥�� btDate �̰�, btHour1�� btMin1��  <= ���� �ð� <= btHour1�� btMin1�� �̸� true
};

/// ���� + �ð� üũ
struct STR_COND_019
{
	unsigned int	uiSize;
	int				iType;

	BYTE			btWeekDay;	// ���� (0 ~ 6)

	BYTE			btHour1;	// ��
	BYTE			btMin1;		// ��
	BYTE			btHour2;	
	BYTE			btMin2;		
	//  ������ ������ btWeekDay �̰�, btHour1�� btMin1��  <= ���� �ð� <= btHour1�� btMin1�� �̸� true
};

struct STR_COND_020
{
	unsigned int	uiSize;
	int				iType;
	int				iNo1;
	int				iNo2; // nNo1 <= ����ȣ <= nNo2 �̸� true
};

/// NPC�� �ŷ� üũ...
struct STR_COND_021
{
	unsigned int	uiSize;
	int				iType;
	BYTE			btSelObjType;	/// �Ÿ� ���� ������ ��ü...
	int				iRadius;		/// ������. (X,Y)�� �������� �Ͽ� ������ x ���ͳ���..
};

/// ä�� ���� üũ
struct STR_COND_022
{
	unsigned int	uiSize;
	int				iType;
	unsigned short	nX;
	unsigned short	nY;
};

//
// Ŭ�� ����
//

// Ŭ���� ���� üũ - �߰� ������ ����
struct STR_COND_023
{
	unsigned int	uiSize;
	int				iType;

	BYTE			btReg;	// 0:�������̸� true, 1:�������̸� true
};

// Ŭ�� ���� üũ
struct STR_COND_024
{
	unsigned int	uiSize;
	int				iType;

	short			nPOS;	// ���� ��ȣ
	BYTE			btOP;	// 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
};

// Ŭ�� ���� �⿩�� üũ
struct STR_COND_025
{
	unsigned int	uiSize;
	int				iType;

	short			nCONT;	// �⿩��
	BYTE			btOP;	// 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
};

// ��� üũ
struct STR_COND_026
{
	unsigned int	uiSize;
	int				iType;

	short			nGRD;	// Ŭ�� ���
	BYTE			btOP;	// 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
};

// ���� üũ
struct STR_COND_027
{
	unsigned int	uiSize;
	int				iType;

	short			nPOINT;	// Ŭ�� ����
	BYTE			btOP;	// 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
};

// �Ӵ� üũ
struct STR_COND_028
{
	unsigned int	uiSize;
	int				iType;

	int				iMONEY;	// Ŭ�� �Ӵ�
	BYTE			btOP;	// 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
};

// �ο� üũ
struct STR_COND_029
{
	unsigned int	uiSize;
	int				iType;

	short			nMemberCNT;	// Ŭ�� �ο�
	BYTE			btOP;	// 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
};

// ��ų üũ
struct STR_COND_030
{
	unsigned int	uiSize;
	int				iType;

	short			nSkill1;
	short			nSkill2; // nSkill1 ~ nSkill2 �� ��ų�� �ִ���/������ üũ
	BYTE			btOP;	// 0: ���°� ?  1: �ִ°� ?
};


//-------------------------------------------------------------------------------------------------
///
///
///
//-------------------------------------------------------------------------------------------------

/// ����Ʈ ���/���� ��û
struct STR_REWD_000
{
	unsigned int	uiSize;
	int				iType;

	int				iQuestSN;	/// Quest ��ȣ. �����ô� ���� ����Ʈ�� �����ϴ� �Ŵϱ�, �ʿ� ����.
	BYTE			btOp;		/// 0 = ����, 1 = ���, 2 = ����(������ ����), 3 = ����/�ֱ�ȭ 
} ;


/// ����Ʈ���� ������ �ֱ�/���� (�Ϲ� �����۵� �̰� ����ص� ��)
/// ����� Quest Inventory����. �ִ°Ŵ� �Ϲ�, ����Ʈ ������ ��� ����
struct STR_REWD_001
{
	unsigned int	uiSize;
	int				iType;

	unsigned int	uiItemSN;
	BYTE			btOp;		/// 0 = ���ѱ�, 1 = �ֱ�
	short			nDupCNT;	/// �ߺ� ����
	BYTE			btPartyOpt;	/// 0 = ��Ƽ�� ���� ����, 1 = ��Ƽ�� ����
} ;


/// ����Ʈ ������/����ġ�� ����
struct STR_REWD_002
{
	unsigned int	uiSize;
	int				iType;

	int				iDataCnt;		/// �ڿ� ��� �����Ͱ� �ִ���
	STR_QUEST_DATA	CheckData[ 1 ]; /// ������ �������̳�, ����ġ�� ���� �˻��ϴ� ������
} ;


/// ĳ���� �ɷ�ġ ���� - (����)
struct STR_REWD_003
{
	unsigned int	uiSize;
	int				iType;

	int				iDataCnt;		/// �ڿ� ��� �����Ͱ� �ִ���
	STR_ABIL_DATA	CheckData[ 1 ]; /// �����ų �ɷ�ġ
	BYTE			btPartyOpt;	/// 0 = ��Ƽ�� ���� ����, 1 = ��Ƽ�� ����
} ;


/// ĳ���� ���ຯ���� ����
struct STR_REWD_004
{
	unsigned int	uiSize;
	int				iType;

	int				iDataCnt;		/// �ڿ� ��� �����Ͱ� �ִ���
	STR_QUEST_DATA	CheckData[ 1 ];
} ;


/// ����
struct STR_REWD_005
{
	unsigned int	uiSize;
	int				iType;

	BYTE			btTarget;	/// 0 = ����ġ, 1 = �� , 2 = ������
	BYTE			btEquation;	/// ������ ��ӵ� ����
	int				iValue;		/// ���ذ�
	int				iItemSN;	/// btTarget = 2 �� ��� �����۹�ȣ�� �̰ŷ� ����
	BYTE			btPartyOpt;	/// 0 = ��Ƽ�� ���� ����, 1 = ��Ƽ�� ����
	// 2004-06-03 ����
	short			nItemOpt;	// ������ ����. list_jemitem.stb�� ���ι�ȣ. (btTarget=2�� ��츸 ��ȿ)
} ;


/// ĳ���� Hp, Mp ȸ��
struct STR_REWD_006
{
	unsigned int	uiSize;
	int				iType;

	int				iPercentOfHP;	/// ��� ȸ���̸� 100% (����� ������ 100)
	int				iPercentOfMP;	/// ��� ȸ���̸� 100% (����� ������ 100)
	BYTE			btPartyOpt;	/// 0 = ��Ƽ�� ���� ����, 1 = ��Ƽ�� ����
} ;


/// ����
struct STR_REWD_007
{
	unsigned int	uiSize;
	int				iType;

	int				iZoneSN;		/// ����ȣ
	int				iX;				/// �ش����� �ű� X��ǥ
	int				iY;				/// �ش����� �ű� Y��ǥ
	BYTE			btPartyOpt;		/// 0 = ��Ƽ�� ���� ����, 1 = ��Ƽ�� ����
} ;


/// ������
struct STR_REWD_008
{
	unsigned int	uiSize;
	int				iType;

	int				iMonsterSN;		/// ��ȯ�� ���� ��ȣ
	int				iHowMany;		/// ��ȯ�� ������

	/// 0 = ����(�ƹ�Ÿ)�ֺ�, 1 = NPC�ֺ�, 2 = �̺�Ʈ�ֺ�, 3 = �ش���� ����(��ǥ����)
	BYTE			btWho;		

	/// btWhere == 2 �� ��� �Ʒ� ����
	int				iZoneSN;		/// ����ȣ
	int				iX;				/// X
	int				iY;				/// Y

	/// ���������� ����
	int				iRange;			/// ���� �̳���
	int				iTeamNo;
} ;


/// *** ��� üũ�� �̵� ==> ������ ����, shNameLen �߰�
struct STR_REWD_009
{
	unsigned int	uiSize;
	int				iType;

	short			shNameLen;
	char			szNextTriggerSN[ 1 ];	/// ���� ���Ͽ��� iNextTriggerSN(O���̽�)��° �̵��ؼ� Ʈ����üũ
};

/// ���� �ʱ�ȭ !!!
struct STR_REWD_010
{
	unsigned int	uiSize;
	int				iType;
};


struct STR_REWD_012
{
	unsigned int	uiSize;
	int				iType;

	BYTE		btMsgType;		/// 0 = �����ʵ�, 1 = ������, 2=��ü����
//	short		nMsgLength;		/// ����� ���� NULL����
//	char		szMsg[ 1 ];		/// ���
	int			iStrID;
};


/// �p�� �Ŀ� � Ʈ���� ����
struct STR_REWD_013
{
	unsigned int	uiSize;
	int				iType;

	BYTE			btWho;	// 0:NPC, 1:EventOBJ

	int				iSec;	/// �����Ŀ�.
	short			nNameLength;
	char			szTriggerName[ 1 ]; /// Ʈ���Ÿ� NULL����

	t_HASHKEY		m_HashNextTrigger;	/// �ε��� Ʈ���Ÿ��� �콬������ ���� ��Ŵ...
};


/// ��ų�� �ְų� �����Ѵ�
struct STR_REWD_014
{
	unsigned int	uiSize;
	int				iType;

	BYTE			btOp;		// 0 - ���´� , 1 - �ش�
	int				iSkillNo;	// ��ų��ȣ
};


/// Switch Off On
//#define		TYPE_REWD_015
struct STR_REWD_015
{
	unsigned int	uiSize;
	int				iType;

	short			nSN;	// ����ġ ��ȣ 0 ~ 512
	BYTE			btOp;	// 0 = Off , 1 = On
};

/// �׷캰 Switch Clear
//#define		TYPE_REWD_016
struct STR_REWD_016
{
	unsigned int	uiSize;
	int				iType;

	short			nGroupSN;	// ����ġ ��ȣ 0 ~ 512 / 32
};

/// ��ü Switch Clear
//#define		TYPE_REWD_017
struct STR_REWD_017
{
	unsigned int	uiSize;
	int				iType;

	// ������ �ʿ� ����.
};



/// NPC ���� ���
struct STR_REWD_018
{
	unsigned int	uiSize;
	int				iType;

//	short			nFormatLength;
	int				iStrID;
	short			nCnt;
	BYTE			Data[ 1 ];
	/* Data �ȿ��� STR_NPCVAR NpcVars[ 1 ]
		Data ũ�� =  nCnt * sizeof (STR_NPCVAR) */
	/* printf�� ���˹��ڿ��� ����.
		"xxxx %d xxxx %d xxx "���� %d�� nCnt��ŭ ����. */
};


/// Ư�� ����ȣ�� ���� �ƹ�Ÿ���� Ʈ���� ����
struct STR_REWD_019
{
	unsigned int	uiSize;
	int				iType;

	short			nZoneNo;

	short			nTeamNo;
	short			nTriggerLength;
	char			TriggerName[ 1 ];	// char Trigger[ nTriggerLength ], NULL ����

	t_HASHKEY		m_HashTrigger;		/// �ε��� Ʈ���Ÿ��� �콬������ ���� ��Ŵ...
};

/// �������� ��Ȱ ��ġ ���� (PVP ��) 
struct STR_REWD_021
{
	unsigned int	uiSize;
	int				iType;
	short			nX;			// Ŭ���̾�Ʈ�� �����ʿ� ���̴� ��ǥ ������ǥ / 100
	short			nY;
};
/// ����ȣ �ο� (PVP ��) - Ʈ���Ÿ� �ߵ���Ų �в� ����ȣ �ο�
struct STR_REWD_020
{
	unsigned int	uiSize;
	int				iType;
	unsigned char	btNoType;	// 0:����ũ ����ȣ �ο�, 1:Ŭ����ȣ�� �ο�, 2:��Ƽ��ȣ�� �ο�
};

/// ��ũ��Ʈ ����. ( Ŭ���̾�Ʈ���� ���� )
struct STR_REWD_029
{
	unsigned int	uiSize;
	int				iType;

	short	nLength; // ��ũ��Ʈ���� ���� ( NULL ���� )
	char	szSrciptName[ 1 ]; // ��ũ��Ʈ��
} ;

/// ��ų �ʱ�ȭ
struct STR_REWD_030
{
	unsigned int	uiSize;
	int				iType;
};

/// ���� ��� ���� ����
struct STR_REWD_031
{
	unsigned int	uiSize;
	int				iType;

	int				iMonsterSN;		/// ��ȯ�� ���� ��ȣ
	int				iCompareValue;	// �� ��ġ
	STR_QUEST_DATA	Var;			// ���� ����
};



/// ����Ʈ ������ ȹ�� ����
struct STR_REWD_032
{
	unsigned int	uiSize;
	int				iType;

	unsigned int	uiItemSN;
	int				iCompareValue;
	BYTE			btPartyOpt;		// 0 = ��Ƽ ���� �� ��, ��Ƽ �����
};


/// ���ú��� Ʈ���� ����
struct STR_REWD_033
{
	unsigned int	uiSize;
	int				iType;

	short			nNextRewardSplitter;	// -1 �̸� ����Ʈ������ ����
};







//-------------------------------------------------------------------------------------------------
union uniQstENTITY {
	struct {
		unsigned int	uiSize;
		int				iType;
	} ;
	BYTE				m_pDATA[0];

	STR_COND_000		m_Cond000;
	STR_COND_001		m_Cond001;
	STR_COND_002		m_Cond002; 
	STR_COND_003		m_Cond003; 
	STR_COND_004		m_Cond004; 
	STR_COND_005		m_Cond005; 
	STR_COND_006		m_Cond006; 
	STR_COND_007		m_Cond007; 
	STR_COND_008		m_Cond008; 
	STR_COND_009		m_Cond009;
	STR_COND_010		m_Cond010;
	STR_COND_011		m_Cond011;
	STR_COND_012		m_Cond012;
	STR_COND_013		m_Cond013;
	STR_COND_014		m_Cond014;
	STR_COND_015		m_Cond015;
	STR_COND_016		m_Cond016;
	STR_COND_017		m_Cond017;
	STR_COND_018		m_Cond018;
	STR_COND_019		m_Cond019;
	STR_COND_020		m_Cond020;
	STR_COND_021		m_Cond021;
	STR_COND_022		m_Cond022;

	STR_COND_023		m_Cond023;
	STR_COND_024		m_Cond024;
	STR_COND_025		m_Cond025;
	STR_COND_026		m_Cond026;
	STR_COND_027		m_Cond027;
	STR_COND_028		m_Cond028;
	STR_COND_029		m_Cond029;
	STR_COND_030		m_Cond030;





	STR_REWD_000		m_Rewd000;
	STR_REWD_001		m_Rewd001;
	STR_REWD_002		m_Rewd002; 
	STR_REWD_003		m_Rewd003; 
	STR_REWD_004		m_Rewd004; 
	STR_REWD_005		m_Rewd005; 
	STR_REWD_006		m_Rewd006; 
	STR_REWD_007		m_Rewd007; 
	STR_REWD_008		m_Rewd008; 
	STR_REWD_009		m_Rewd009;
	STR_REWD_010		m_Rewd010;
	STR_REWD_011		m_Rewd011;
	STR_REWD_012		m_Rewd012;
	STR_REWD_013		m_Rewd013;
	STR_REWD_014		m_Rewd014;

	STR_REWD_015		m_Rewd015;
	STR_REWD_016		m_Rewd016;
	STR_REWD_017		m_Rewd017;

	STR_REWD_018		m_Rewd018;
	STR_REWD_019		m_Rewd019;
	STR_REWD_020		m_Rewd020;
	STR_REWD_021		m_Rewd021;
	STR_REWD_029		m_Rewd029;

	STR_REWD_031		m_Rewd031;
	STR_REWD_032		m_Rewd032;
	STR_REWD_033		m_Rewd033;
} ;



//-------------------------------------------------------------------------------------------------
/*
enum eQE_CODE {		// Quest error code
	QSTERR_QUEST_OK = 0x0000,
	QSTERR_QUEST_NOT_REGISTERED,	// ���� �������� ����Ʈ�� �ƴϴ�
} ;
*/
class CUserDATA;
class CObjUSER;
class CQUEST;
class CQuestTRIGGER;
class CObjNPC;
class CObjEVENT;

#ifdef	__SERVER
class classUSER;
#endif

struct tQST_PARAM {
#ifndef	__SERVER
	CQuestTRIGGER*m_pCurrentTRIGGER;
	CUserDATA	*m_pOWNER;				/// ����Ʈ �����
#else
	CObjVAR		*m_pEventVAR;			/// ���� ���õ� Event Object
	CObjVAR		*m_pNpcVAR;				/// ���� ���õ� NPC
	classUSER	*m_pOWNER;				/// ����Ʈ �����
#endif

	short		 m_nCurZoneNO;			/// ����Ʈ �߻��ڰ� �ִ� �� ��ȣ

	BYTE		 m_btQuestSLOT;
	CQUEST		*m_pQUEST;				/// ���� ���õ� ����Ʈ 
	t_HASHKEY	 m_HashNextTRIGGER;		/// ������ ������ ��� �߰� �˻��� ����Ʈ Ʈ����

	bool		 m_bServerFUNC;	
	short		 m_nErrSTEP;

	void Init ( CUserDATA *pOWNER, short nZoneNO=0)
	{
		m_btQuestSLOT = QUEST_PER_PLAYER;
		m_pQUEST = NULL;
		m_HashNextTRIGGER = 0;
		m_nErrSTEP	= -1;
		m_bServerFUNC = false;
		m_nCurZoneNO = nZoneNO;
#ifdef	__SERVER
		m_pNpcVAR	= NULL;
		m_pEventVAR = NULL;
		m_pOWNER = (classUSER*)pOWNER;
#else
		m_pOWNER = pOWNER;
#endif
	}
} ;

//-------------------------------------------------------------------------------------------------
class CQuestTRIGGER {
private:
	BYTE			m_btCheckNEXT;
	unsigned int	m_uiCondCNT;
	unsigned int	m_uiRewdCNT;

	uniQstENTITY **m_ppCondition;
	uniQstENTITY **m_ppReward;

	void Init_COND( uniQstENTITY *pCOND );
	void Init_REWD( uniQstENTITY *pREWD );

public :
	CStrVAR			m_Name;
	CQuestTRIGGER  *m_pNextTrigger;

	CQuestTRIGGER ()	{	m_pNextTrigger = NULL;	}
	~CQuestTRIGGER ()	{	this->Free();			}

	BYTE GetCheckNext()	{	return m_btCheckNEXT;	}

#ifdef	__SERVER
	bool Load (FILE *fpIN);
#else
	bool Client_Load( CFileSystem* pFileSystem );
#endif
	void Free ();

	bool Proc (tQST_PARAM *pPARAM, bool bDoReward);
} ;


//-------------------------------------------------------------------------------------------------
// QUEST 
#ifdef __SERVER
	#define QUEST_NAME(I)				g_QuestList.m_STB.m_ppNAME[ I ]
	#define QUEST_DESC(I)				g_QuestList.m_STB.m_ppDESC[ I ]
#else
	#include "../GameCommon/StringManager.h"
	#define QUEST_NAME(I)				CStringManager::GetSingleton().GetQuestName( I )
	#define QUEST_DESC(I)				CStringManager::GetSingleton().GetQuestDesc( I )
	#define QUEST_START_MSG(I)			CStringManager::GetSingleton().GetQuestStartMsg( I )
	#define QUEST_END_MSG(I)			CStringManager::GetSingleton().GetQuestEndMsg( I )
#endif

#define	QUEST_TIME_LIMIT(I)			g_QuestList.m_STB.m_ppDATA[ I ][ 1 ]
#define	QUEST_OWNER_TYPE(I)			g_QuestList.m_STB.m_ppDATA[ I ][ 2 ]
#define QUEST_ICON(I)				g_QuestList.m_STB.m_ppDATA[ I ][ 3 ]

enum eQST_RESULT {
	QST_RESULT_INVALID = 0,	// �������� ©����.
	QST_RESULT_SUCCESS,
	QST_RESULT_FAILED,		// �������� ���� �뺸 �ȴ�.
	QST_RESULT_STOPPED,		// ���� �������̴�.
} ;

class CQuestDATA {
private:
	classHASH< CQuestTRIGGER* >	m_HashQUEST;
	CStrVAR		m_QuestListSTB;
	CStrVAR		m_QuestFILE;
	CStrVAR		m_BaseDIR;
	
	bool		m_bEnable;
	long		m_lRefCnt;

	#ifdef	__SERVER
		void	LoadQuestTrigger (FILE *fpIN, unsigned int uiTriggerCNT, char *szFileName);
		bool	LoadDATA (char *szFileName);
	#else
		void	LoadQuestTrigger (CFileSystem* pFileSystem, unsigned int uiTriggerCNT, char *szFileName);
		bool	Client_LoadDATA (char *szFileName);
	#endif

public :
	STBDATA		m_STB;

	bool LoadQuestTable ();
	bool LoadQuestTable (char *szQuestFile, char *szQuestListFile, char *szBaseDIR=NULL);
	void Free ();
	CQuestDATA ()		{	m_bEnable=false, m_lRefCnt=0;	}
	~CQuestDATA ()		{	this->Free();		}

	eQST_RESULT CheckQUEST( CUserDATA *pUSER, t_HASHKEY HashQuest, bool bDoReward=false );
	CQuestTRIGGER *GetQuest( t_HASHKEY HashQuest ) 
	{
		tagHASH< CQuestTRIGGER* > *pHashNode;

		pHashNode = m_HashQUEST.Search( HashQuest );
		return ( pHashNode ) ? pHashNode->m_DATA : NULL;
	}

	bool IsEnable ()	
	{
		return m_bEnable;
	}
	#ifdef	__SERVER
	void ToggleEnable ()
	{
		m_bEnable = !m_bEnable;
	}
	long GetRefCNT ()	{	return m_lRefCnt;	}
	#endif
} ;
extern CQuestDATA g_QuestList;


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#pragma warning( default : 4200 )
#endif