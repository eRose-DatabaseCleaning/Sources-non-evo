#include "stdAFX.h"
#include "CAI_FILE.h"
#include "CAI_LIB.h"

#ifdef	__SERVER
	extern short Get_ServerChannelNO ();
#endif

//-------------------------------------------------------------------------------------------------
bool F_AICOND_00 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
	// ������.
	return false;
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_01 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
/*
struct		AICOND01
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	int				iDamage;					// ��������
	BYTE			cRecvOrGive ;				// 0=�޴� , 1=�ִ�
} ;
*/
	AICOND01 *pCond = (AICOND01*)pConDATA;

	if ( pCond->cRecvOrGive )
		return false;

	if ( pAIParam->m_iDamage >= pCond->iDamage )
		return true;

	return false;
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_02 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return false;
#endif

	AICOND02 *pCond = (AICOND02*)pConDATA;

	bool bAllied = ( 0 != pCond->btIsAllied );
	// int CharTYPE = ( pCond->cChrType ) ? OBJ_MOB : OBJ_AVATAR;

//	short nMyTeamNO = pAIParam->m_pSourCHAR->Get_TeamNO();
	CAI_OBJ *pFindCHAR = pAIParam->m_pSourCHAR->AI_FindFirstOBJ( pCond->iDistance );
	int iFindCNT = 0, iNearDist=999999999, iCurDist;

	while( pFindCHAR ) {
		if ( ( pAIParam->m_pSourCHAR!=pFindCHAR ) && (bAllied==pAIParam->m_pSourCHAR->Is_ALLIED( pFindCHAR ) ) ) {
			if ( pAIParam->m_pSourCHAR->Get_LEVEL() - pFindCHAR->Get_LEVEL() >= pCond->nLevelDiff &&
				 pAIParam->m_pSourCHAR->Get_LEVEL() - pFindCHAR->Get_LEVEL() <= pCond->nLevelDiff2 ) {
				iFindCNT ++;

				iCurDist = (int)( pAIParam->m_pSourCHAR->Get_DISTANCE( pFindCHAR ) );
				if ( iCurDist < iNearDist ) {
					iNearDist = iCurDist;
					// ���� ����� ��...
					pAIParam->m_pNearCHAR = pFindCHAR;
				}

				if ( iFindCNT >= pCond->wChrNum ) {
					// ���� �������� ã����
					pAIParam->m_pFindCHAR = pFindCHAR;
					return true;
				}
			}
		}
		pFindCHAR = pAIParam->m_pSourCHAR->AI_FindNextOBJ ();
	} 
	
	return false;
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_03 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return false;
#endif
/*
struct		AICOND03
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	int				iDistance ;					// �Ÿ�
} ;
*/
	AICOND03 *pCond = (AICOND03*)pConDATA;
	int iDistance = (int)( pAIParam->m_pSourCHAR->Get_MoveDISTANCE () );
	if ( iDistance >= pCond->iDistance )
		return true;

	return false;
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_04 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
/*
struct		AICOND04
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	int				iDistance ;					// �Ÿ�
	BYTE			cMoreLess ;					// �̻� ����
} ;
*/
	AICOND04 *pCond = (AICOND04*)pConDATA;
	CAI_OBJ *pTarget = pAIParam->m_pSourCHAR->Get_TARGET ();
	if ( NULL == pTarget )
		return false;

	int iDistance = (int)( pAIParam->m_pSourCHAR->Get_DISTANCE( pTarget ) );
	if ( pCond->cMoreLess ) {
		// ����
		if ( iDistance <= pCond->iDistance )
			return true;
	} else {
		// �̻�
		if ( iDistance >= pCond->iDistance )
			return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_05 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
/*
struct		AICOND05
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	int				cAbType ;					// �ɷ�ġ Ÿ��
	int				iDiff ;
	BYTE			cMoreLess ;					// ū�� , ������
} ;
	cAbType 
		0 ����
		1 ���ݷ�
		2 ����
		3 �׸���
		4 HP
		5 �ŷ�
*/
	AICOND05 *pCond = (AICOND05*)pConDATA;

	// Ÿ����...
	CAI_OBJ *pTarget = pAIParam->m_pSourCHAR->Get_TARGET ();
	if ( NULL == pTarget )
		return false;

	int iValue = AI_Get_Ability ( pTarget, pCond->cAbType );
	if ( pCond->cMoreLess ) {
		// ����
		if ( iValue <= pCond->iDiff )
			return true;
	} else {
		// �̻�
		if ( iValue >= pCond->iDiff )
			return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_06 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
/*
struct		AICOND06
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	DWORD			wHP ;						// HP ��ġ
	BYTE			cMoreLess ;					// ū�� , ������
} ;
*/
	AICOND06 *pCond = (AICOND06*)pConDATA;

	if ( pCond->cMoreLess ) {
		// ����
		if ( pAIParam->m_pSourCHAR->Get_PercentHP() <= (int)pCond->wHP )
			return true;
	} else {
		// �̻�
		if ( pAIParam->m_pSourCHAR->Get_PercentHP() >= (int)pCond->wHP )
			return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_07 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
/*
struct		AICOND07
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	BYTE			cPercent ;					// Ȯ��
} ;
*/
	AICOND07 *pCond = (AICOND07*)pConDATA;

	if ( pAIParam->m_pSourCHAR->Get_RANDOM( 100 ) < pCond->cPercent )
		return true;

	return false;
}

//-------------------------------------------------------------------------------------------------
bool F_AICOND_08 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return false;
#endif

/*
// �ֺ� distance���� �ڽŰ� �������� �ִ� �ƹ�Ÿ/���Ͱ� ������
// Ÿ������ ����.
struct		AICOND08
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	int				iDistance ;					// �Ÿ�
	char			cLevelDiff ;				// ������ (-128~127 ) �ڽ��� �������� cLevelDiff�̻���
	BYTE			cChrType ;					// ĳ���� Ÿ�� (�ƹ�Ÿ=0,����=1)
} ;
*/
	AICOND08 *pCond = (AICOND08*)pConDATA;

	bool bAllied = ( 0 != pCond->btIsAllied );

	CAI_OBJ *pFindCHAR = pAIParam->m_pSourCHAR->AI_FindFirstOBJ( pCond->iDistance );
//	short nMyTeamNO = pAIParam->m_pSourCHAR->Get_TeamNO();
	while ( pFindCHAR ) {
		if ( ( pAIParam->m_pSourCHAR!=pFindCHAR ) && (bAllied==pAIParam->m_pSourCHAR->Is_ALLIED( pFindCHAR ) ) ) {
			if ( pAIParam->m_pSourCHAR->Get_LEVEL() - pFindCHAR->Get_LEVEL() >= pCond->nLevelDiff &&
				 pAIParam->m_pSourCHAR->Get_LEVEL() - pFindCHAR->Get_LEVEL() <= pCond->nLevelDiff2 ) {
				pAIParam->m_pFindCHAR = pFindCHAR;
				pAIParam->m_pNearCHAR = pFindCHAR;
				return true;
			}
		}
		pFindCHAR = pAIParam->m_pSourCHAR->AI_FindNextOBJ ();
	} 

	return false;
}

//-------------------------------------------------------------------------------------------------
bool F_AICOND_09 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
/*
// �ڽ��� ������ �ɸ��Ϳ� ���� Ÿ���� �ٸ� �ɸ��� �ΰ�?
struct		AICOND09
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
} ;
*/
	AICOND09 *pCond = (AICOND09*)pConDATA;
	if ( pAIParam->m_pSourCHAR->Get_TARGET() != pAIParam->m_pDestCHAR )
		return true;

	return false;
}

//-------------------------------------------------------------------------------------------------
bool F_AICOND_10 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
/*
// �ڽ��� ������ �ɸ����� �ɷ�ġ�� ���� Ÿ�� �ɸ��ͺ��� ū��/������?
struct		AICOND10
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	BYTE			cAbType ;					// �ɷ�ġ Ÿ��
	BYTE			cMoreLess ;					// ū=0 , ����=1
} ;
*/
	AICOND10 *pCond = (AICOND10*)pConDATA;

	// �ݵ�� Ÿ���� �����ؾ� �Ѵ�.
	if ( NULL == pAIParam->m_pSourCHAR->Get_TARGET()  || 
		 NULL == pAIParam->m_pDestCHAR )
		return false;

	int iV1, iV2;
	iV1 = AI_Get_Ability (pAIParam->m_pDestCHAR, pCond->cAbType );
	iV2 = AI_Get_Ability (pAIParam->m_pSourCHAR->Get_TARGET(), pCond->cAbType );
	if ( pCond->cMoreLess ) {
		// ����
		if ( iV1 < iV2 )
			return true;
	} else {
		// ū
		if ( iV1 > iV2 )
			return true;
	}

	return false;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
bool F_AICOND_11 (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
/*
 * Explain : �ڽ��� ������ ĳ������ A�ɷ�ġ�� B������ ũ�ų�/���� ���
struct		AICOND11
{
	DWORD			dwSize ;		// �� ��Ʈ������ ũ��
	AITYPE			Type ;			// ���� Ÿ�� = AICOND_11
	BYTE			cAbType ;		// �ɷ�ġ (����=0, ���ݷ�=1, ����=2, �׸���=3, HP=4, �ŷ�=5)
	int				iValue;			// �񱳰�. �̰����� ū��/������. �̷�������
	BYTE			cMoreLess ;		// ū=0 , ����=1
} ;
*/
	AICOND11 *pCond = (AICOND11*)pConDATA;

	// �ݵ�� Ÿ���� ����� �Ѵ�.
	if ( pAIParam->m_pSourCHAR->Get_TARGET() )
		return false;

	if ( NULL == pAIParam->m_pDestCHAR )
		return false;

	int iValue = AI_Get_Ability (pAIParam->m_pDestCHAR, pCond->cAbType );

	if ( pCond->cMoreLess ) {
		// ����
		if ( iValue <= pCond->iValue )
			return true;
	} else {
		// �̻�
		if ( iValue >= pCond->iValue )
			return true;
	}

	return false;
}

//-------------------------------------------------------------------------------------------------
bool F_AICOND_12 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{	
/*
/// ���� ���� ���� ���ΰ� / ���ΰ� ?
struct		AICOND12
{
	DWORD			dwSize ;		// �� ��Ʈ������ ũ��
	AITYPE			Type ;			// ���� Ÿ�� = AICOND_12
	BYTE			cWhen;			// 0 = ������ üũ, 1 = ������ üũ
};
*/
	int iZoneTIME = pAIParam->m_pSourCHAR->Get_ZoneTIME();

	return ( ((AICOND12*)pConDATA)->cWhen != pAIParam->m_pSourCHAR->Is_DAY() );
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_13 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{	
/*
/// �ڽ�/Ÿ�ٿ��� �ɷ��ִ� ���� üũ. 
struct		AICOND13
{
	DWORD			dwSize;
	AITYPE			Type;

	BYTE			btCheckTarget;	// 0 = �ڱ��ڽ�, 1 = ����Ÿ��
	BYTE			btStatusType;	// 0 = �طο�, 1 = ������, 2 = �طο�/������ �ƹ��ų�
	BYTE			btHave;			// 0 = ���°� ? 1 = �ִ°� ?
};
*/
#ifndef	__SERVER
	return false;
#else
	AICOND13 *pCond = (AICOND13*)pConDATA;

	CAI_OBJ *pTarget;
	if ( pCond->btCheckTarget ) {
		// ���� Ÿ�� ...
		pTarget = pAIParam->m_pSourCHAR->Get_TARGET ();
		if ( NULL == pTarget )
			return false;
	} else {
		// �ڽ�..
		pTarget = pAIParam->m_pSourCHAR;
	}

	DWORD dwStatus;
	switch( pCond->btStatusType ) {
		case 0 :	// �طο�
			dwStatus = FLAG_ING_BAD  & pTarget->Get_MagicSTATUS();
			break;
		case 1 :	// ������
			dwStatus = FLAG_ING_GOOD & pTarget->Get_MagicSTATUS();
			break;
		case 2 :	// �ϰų�..
			dwStatus = pTarget->Get_MagicSTATUS();
			break;
	}

	if ( dwStatus ) {
		// �����ִ�..
		return ( pCond->btHave ) ? true : false;
	}

	// ���¾���.
	return ( 0 == pCond->btHave ) ? true : false;
#endif
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_14 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
/*
// NPC ���� üũ	- AICOND14
typedef struct	tagValue
{
	DWORD			dwSize;
	AITYPE			Type;

	short			nVarNo;	// ���� ��ȣ : 0 ~ , ���� ���������� ����
	int				iValue;	// ���� �����Ͱ� (iType�� �ɷ�ġ Op iValue)
	BYTE			btOp;	// iValue���� ���� �񱳹��. 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
							// (�׼���) 5 = ���ٲ�, 6 = ����, 7 = ����
} AICOND14, AICOND15, AICOND16, AIACT25, AIACT26, AIACT27;
*/
#ifndef	__SERVER
	return true;
#else
	if ( NULL == pAIParam->m_pLocalNPC )
		return false;

	tagValueAI *pVALUE = (tagValueAI*)pConDATA;

	int iValue = pAIParam->m_pLocalNPC->Get_ObjVAR( (BYTE)pVALUE->nVarNo );
	return ::Check_AiOP( pVALUE->btOp, iValue, pVALUE->iValue );
#endif
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_15 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{	
/*
// ���庯�� üũ	- AICOND15
typedef struct	tagValue
{
	DWORD			dwSize;
	AITYPE			Type;

	short			nVarNo;	// ���� ��ȣ : 0 ~ , ���� ���������� ����
	int				iValue;	// ���� �����Ͱ� (iType�� �ɷ�ġ Op iValue)
	BYTE			btOp;	// iValue���� ���� �񱳹��. 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
							// (�׼���) 5 = ���ٲ�, 6 = ����, 7 = ����
} AICOND14, AICOND15, AICOND16, AIACT25, AIACT26, AIACT27;
*/
#ifndef	__SERVER
	return false;
#else
	tagValueAI *pVALUE = (tagValueAI*)pConDATA;

	int iValue = pAIParam->m_pSourCHAR->Get_WorldVAR( pVALUE->nVarNo );
	return ::Check_AiOP( pVALUE->btOp, iValue, pVALUE->iValue );
#endif
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_16 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{	
/*
// ���� ������ üũ - AICOND16
typedef struct	tagValue
{
	DWORD			dwSize;
	AITYPE			Type;

	short			nVarNo;	// ���� ��ȣ : 0 ~ , ���� ���������� ����
	int				iValue;	// ���� �����Ͱ� (iType�� �ɷ�ġ Op iValue)
	BYTE			btOp;	// iValue���� ���� �񱳹��. 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.
							// (�׼���) 5 = ���ٲ�, 6 = ����, 7 = ����
} AICOND14, AICOND15, AICOND16, AIACT25, AIACT26, AIACT27;
*/
#ifndef	__SERVER
	return false;
#else
	tagValueAI *pVALUE = (tagValueAI*)pConDATA;

	int iValue = pAIParam->m_pSourCHAR->Get_EconomyVAR( pVALUE->nVarNo );
	return ::Check_AiOP( pVALUE->btOp, iValue, pVALUE->iValue );
#endif
}
//-------------------------------------------------------------------------------------------------
bool F_AICOND_17 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{	
/*
/// NPC ����
struct AICOND17
{
	DWORD			dwSize;
	AITYPE			Type;

	int				iNpcNo; /// ������ NPC ��ȣ
};
*/
#ifndef	__SERVER
	return false;
#else
	AICOND17 *pCond = (AICOND17*)pConDATA;

	pAIParam->m_pLocalNPC = (CObjNPC*)pAIParam->m_pSourCHAR->Find_LocalNPC( pCond->iNpcNo );

	return ( NULL != pAIParam->m_pLocalNPC );
#endif
}

bool F_AICOND_18 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
#ifndef	__SERVER
	return false;
#else
	CAI_OBJ *pOwner = pAIParam->m_pSourCHAR->Get_CALLER ();
	if ( NULL == pOwner ) 
		return false;

	AICOND18 *pCond = (AICOND18*)pConDATA;
	int iDistance = static_cast<int>( pAIParam->m_pSourCHAR->Get_DISTANCE( pOwner ) );

	return ::Check_AiOP( pCond->btOp, iDistance, pCond->iDistance );
#endif
}

bool F_AICOND_19 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
#ifndef	__SERVER
	return false;
#else
	AICOND19 *pCond = (AICOND19*)pConDATA;

	if ( (UINT)pAIParam->m_pSourCHAR->Get_ZoneTIME() >= pCond->ulTime &&
		 (UINT)pAIParam->m_pSourCHAR->Get_ZoneTIME() <= pCond->ulEndTime )
		return true;

	return false;
#endif
}


bool F_AICOND_20 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
	AICOND20 *pCond = (AICOND20*)pConDATA;
/*
// �ڽ�(����)�� �ɷ�ġ/��Ÿ/����ĳ���� ���� ������ üũ
struct AICOND20
{
	DWORD	dwSize;
	AITYPE	Type;

	BYTE	btAbType ;		// �ɷ�ġ (����=0, ���ݷ�=1, ����=2, �׸���=3, HP=4, �ŷ�=5)

	int		iValue;	// ���� �����Ͱ� (iType�� �ɷ�ġ Op iValue)
	BYTE	btOp;	// iData���� ���� �񱳹��. 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.					
};
*/
#ifndef	__SERVER
	return false;
#else
	int iValue;

	iValue = AI_Get_Ability (pAIParam->m_pSourCHAR, pCond->btAbType );
	return ::Check_AiOP( pCond->btOp, iValue, pCond->iValue );
#endif
}

bool F_AICOND_21 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
#ifndef	__SERVER
	return false;
#else
	AICOND21 *pCond = (AICOND21*)pConDATA;
	/// ����ĳ���Ͱ� ���� ���� Ȯ��

	return ( NULL == pAIParam->m_pSourCHAR->Get_CALLER() );
#endif
}

bool F_AICOND_22 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
#ifndef	__SERVER
	return false;
#else
	AICOND22 *pCond = (AICOND22*)pConDATA;
	/// ����ĳ���Ͱ� ���� ����Ÿ���� ������ �ִ°� ?

	CAI_OBJ *pOwner = pAIParam->m_pSourCHAR->Get_CALLER ();
	if ( !pOwner )
		return false;
	
	return ( NULL != pOwner->Get_TARGET() );
#endif
}

bool F_AICOND_23 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
	/// ����ð� üũ
	AICOND23 *pCond = (AICOND23*)pConDATA;

	if ( (UINT)pAIParam->m_pSourCHAR->Get_WorldTIME() >= pCond->ulTime &&
		 (UINT)pAIParam->m_pSourCHAR->Get_WorldTIME() <= pCond->ulEndTime )
		return true;

	return false;
}

bool F_AICOND_24 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
/*
/// ��¥ + �ð� üũ
struct AICOND24
{
	DWORD			dwSize;
	AITYPE			Type;

	BYTE			btDate;		// ��¥. (1 ~ 31)

	BYTE			btHour1;	// �� (1 ~ 24)
	BYTE			btMin1;		// �� (1 ~ 60)
	BYTE			btHour2;	// ��
	BYTE			btMin2;		// ��
	//  ������ ��¥�� btDate �̰�, btHour1�� btMin1��  <= ���� �ð� <= btHour1�� btMin1�� �̸� true
};
*/
#ifdef	__SERVER
	AICOND24 *pCond = (AICOND24*)pConDATA;
	SYSTEMTIME sTIME;

	::GetLocalTime( &sTIME );
	if ( pCond->btDate && sTIME.wDay != pCond->btDate )
		return false;
	
	WORD wMin = sTIME.wHour * 60 + sTIME.wMinute;
	WORD wFrom, wTo;

	wFrom = pCond->btHour1 * 60 + pCond->btMin1;
	wTo   = pCond->btHour2 * 60 + pCond->btMin2;
	if ( wMin >= wFrom && wMin <= wTo )
		return true;
#endif

	return false;
}

bool F_AICOND_25 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
/*
/// ���� + �ð� üũ
struct AICOND25
{
	DWORD			dwSize;
	AITYPE			Type;

	BYTE			btWeekDay;	// ���� (0 ~ 6)

	BYTE			btHour1;	// ��
	BYTE			btMin1;		// ��
	BYTE			btHour2;	
	BYTE			btMin2;		
	//  ������ ������ btWeekDay �̰�, btHour1�� btMin1��  <= ���� �ð� <= btHour1�� btMin1�� �̸� true
};
*/
#ifdef	__SERVER
	AICOND25 *pCond = (AICOND25*)pConDATA;
	SYSTEMTIME sTIME;

	::GetLocalTime( &sTIME );
	if ( sTIME.wDayOfWeek != pCond->btWeekDay )
		return false;

	WORD wMin = sTIME.wHour * 60 + sTIME.wMinute;
	WORD wFrom, wTo;

	wFrom = pCond->btHour1 * 60 + pCond->btMin1;
	wTo   = pCond->btHour2 * 60 + pCond->btMin2;
	if ( wMin >= wFrom && wMin <= wTo )
		return true;
#endif
	return false;
}

bool F_AICOND_26 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
	AICOND26 *pCond = (AICOND26*)pConDATA;
#ifdef	__SERVER
	short nChannelNo = ::Get_ServerChannelNO ();
	return ( pCond->nX >= nChannelNo && pCond->nY <= nChannelNo );
#else
	return false;
#endif
}

bool F_AICOND_27 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
/*
 * Struct Name : AICOND_27
 * Check the surrounding character situation
 * Explain : If the character type is cChrType within the check range iDistance, the level difference is greater than cLevelDiff, and the number of characters is wChrNum or more /
  Struct AICOND27 // same condition twice and ...
{
	DWORD		dwSize ;		
	AITYPE		Type ;			
	int			iDistance ;		// üũ�Ÿ�
	BYTE		cChrType ;		// ĳ���� Ÿ�� (�ƹ�Ÿ=0,����=1)
	short		nLevelDiff ;	// 
	short		nLevelDiff2 ;	// nLevelDiff <= XXXX <= nLevelDiff2

	WORD		wChrNum ;		// ĳ���� ����
	BYTE		btOp;			// ĳ���� ���ڿ� ���� OP (0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.)
} ;
*/
#ifdef	__SERVER
	AICOND27 *pCond = (AICOND27*)pConDATA;

	bool bAllied = ( 0 != pCond->btIsAllied );

	CAI_OBJ *pFindCHAR = pAIParam->m_pSourCHAR->AI_FindFirstOBJ( pCond->iDistance );
	int iFindCNT = 0, iNearDist=999999999, iCurDist;

	while( pFindCHAR ) {
		if ( ( pAIParam->m_pSourCHAR!=pFindCHAR ) && (bAllied==pAIParam->m_pSourCHAR->Is_ALLIED( pFindCHAR ) ) ) {
			if ( pAIParam->m_pSourCHAR->Get_LEVEL() - pFindCHAR->Get_LEVEL() >= pCond->nLevelDiff &&
				 pAIParam->m_pSourCHAR->Get_LEVEL() - pFindCHAR->Get_LEVEL() <= pCond->nLevelDiff2 ) {
				iFindCNT ++;

				iCurDist = (int)( pAIParam->m_pSourCHAR->Get_DISTANCE( pFindCHAR ) );
				if ( iCurDist < iNearDist ) {
					iNearDist = iCurDist;
					// ���� ����� ��...
					pAIParam->m_pNearCHAR = pFindCHAR;
				}

				//if ( iFindCNT >= pCond->wChrNum ) {
					// ���� �������� ã����
				//	pAIParam->m_pFindCHAR = pFindCHAR;
				//	return true;
				//}
			}
		}
		pFindCHAR = pAIParam->m_pSourCHAR->AI_FindNextOBJ ();
	} 
	pAIParam->m_pFindCHAR = pAIParam->m_pNearCHAR;

	return ::Check_AiOP( pCond->btOp, iFindCNT, pCond->wChrNum );
#endif
	return false;
}

bool F_AICOND_28 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
/*
// ���� ���� üũ
struct	AICOND28
{
	DWORD			dwSize;
	AITYPE			Type;

	short			nVarNo;	// ���� ��ȣ : 0 ~ 4
	int				iValue;	// ���� �����Ͱ�
	BYTE			btOp;	// iValue���� ���� �񱳹��. 0 = ����, 1 = ũ��, 2 = ũ�ų� ����. 3=�۴�, 4=�۰ų� ����.							
} ;
*/
#ifdef	__SERVER
	tagValueAI *pVALUE = (tagValueAI*)pConDATA;

	int iValue = pAIParam->m_pSourCHAR->Get_AiVAR( (BYTE)pVALUE->nVarNo );
	return ::Check_AiOP( pVALUE->btOp, iValue, pVALUE->iValue );
#endif
}


// ������ / ����Ÿ���� Ŭ���������ΰ� ?
bool F_AICOND_29 (stCondHead *pConDATA, t_AIPARAM *pAIParam)	
{
	AICOND29 *pCond = (AICOND29*)pConDATA;

	switch( pCond->btTargetType ) {
		case 0 :	// 0 : ������
			if ( NULL == pAIParam->m_pDestCHAR )
				return false;
			return pAIParam->m_pDestCHAR->Is_ClanMASTER ();

		case 1 :	// 1 : ����Ÿ��
		{
			CAI_OBJ *pTarget = pAIParam->m_pSourCHAR->Get_TARGET ();
			if ( NULL == pTarget )
				return false;

			return pTarget->Is_ClanMASTER ();
		}
	}
	return false;
}

//-------------------------------------------------------------------------------------------------
bool F_AICOND_NULL (stCondHead *pConDATA, t_AIPARAM *pAIParam)
{
	return false;
}

tagConditionFUNC g_FuncCOND[] = {
	{	NULL			},
	{	F_AICOND_00		},	// 0x000000001
	{	F_AICOND_01		},	// 0x000000002
	{	F_AICOND_02		},	// 0x000000003
	{	F_AICOND_03		},	// 0x000000004
	{	F_AICOND_04		},	// 0x000000005
	{	F_AICOND_05		},	// 0x000000006
	{	F_AICOND_06		},	// 0x000000007
	{	F_AICOND_07		},  // 0x000000008
	{	F_AICOND_08		},	// 0x000000009
	{	F_AICOND_09		},	// 0x00000000A
	{	F_AICOND_10		},	// 0x00000000B

	{	F_AICOND_11		},	// 0x00000000c

	{	F_AICOND_12		},	
	{	F_AICOND_13		},	
	{	F_AICOND_14		},	
	{	F_AICOND_15		},	
	{	F_AICOND_16		},	
	{	F_AICOND_17		},

	{	F_AICOND_18		},
	{	F_AICOND_19		},

	{	F_AICOND_20		},
	{	F_AICOND_21		},
	{	F_AICOND_22		},
	{	F_AICOND_23		},

	{	F_AICOND_24		},
	{	F_AICOND_25		},

	{	F_AICOND_26		},

	{	F_AICOND_27		},
	{	F_AICOND_28		},

	{	F_AICOND_29		},

	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
	{	F_AICOND_NULL	},
} ;

//-------------------------------------------------------------------------------------------------
