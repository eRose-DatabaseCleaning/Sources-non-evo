/*
pEVENT->m_pDestCHAR;
pEVENT->m_pFindCHAR;
pEVENT->m_pLocalNPC;
pEVENT->m_pNearCHAR;
pEVENT->m_pSourCHAR;
*/

#include "stdAFX.h"
#include "CAI_FILE.h"
#include "CAI_LIB.h"

#ifdef	__SERVER
#include "ZoneLIST.h"
#endif

//-------------------------------------------------------------------------------------------------
void F_AIACT00 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif
/*
	���� ��������..
*/

	AIACT00 *pAct = (AIACT00*)pActDATA;
	pEVENT->m_pSourCHAR->SetCMD_STOP ();
}

//-------------------------------------------------------------------------------------------------
void F_AIACT01 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
/*
struct		AIACT01
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	BYTE			cAction ;					// ���۹�ȣ
} ;

	cAction : ���� ǥ������.
*/
	AIACT01 *pAct = (AIACT01*)pActDATA;
	pEVENT->m_pSourCHAR->Set_EMOTION( pAct->cAction );
}

//-------------------------------------------------------------------------------------------------
void F_AIACT02 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
/*
struct		AIACT02
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	char			szCon[1] ;					// ��� ������
} ;
*/
//	AIACT02 *pAct = (AIACT02*)pActDATA;
	AIACT02_STR *pAct = (AIACT02_STR*)pActDATA;
	pEVENT->m_pSourCHAR->Say_MESSAGE( pAct->szCon );
}
//-------------------------------------------------------------------------------------------------
void F_AIACT03 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
// �ڽ��� ������ġ���� ���� �̵�...
struct		AIACT03
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	int				iDistance ;					// �Ÿ�
	BYTE			cSpeed ;					// õõ�� , ���� , ������
} ;
*/
	AIACT03 *pAct = (AIACT03*)pActDATA;

	float fPosX, fPosY;
	fPosX = pEVENT->m_pSourCHAR->Get_CurXPOS() + AI_SysRANDOM( pAct->iDistance*2 ) - pAct->iDistance;
	fPosY = pEVENT->m_pSourCHAR->Get_CurYPOS() + AI_SysRANDOM( pAct->iDistance*2 ) - pAct->iDistance;

	pEVENT->m_pSourCHAR->SetCMD_MOVE2D( fPosX, fPosY, pAct->cSpeed );
}
//-------------------------------------------------------------------------------------------------
void F_AIACT04 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
// �ڽ��� ������ġ���� ���� �̵�...
struct		AIACT04
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	int				iDistance ;					// �Ÿ�
	BYTE			cSpeed ;					// õõ�� , ���� , ������
} ;
*/
	AIACT04 *pAct = (AIACT04*)pActDATA;

	float fPosX, fPosY;
	fPosX = pEVENT->m_pSourCHAR->Get_BornXPOS() + AI_SysRANDOM( pAct->iDistance*2 ) - pAct->iDistance;
	fPosY = pEVENT->m_pSourCHAR->Get_BornYPOS() + AI_SysRANDOM( pAct->iDistance*2 ) - pAct->iDistance;

	pEVENT->m_pSourCHAR->SetCMD_MOVE2D( fPosX, fPosY, pAct->cSpeed );
}

//-------------------------------------------------------------------------------------------------
void F_AIACT05 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
struct		AIACT05
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	BYTE			cSpeed ;					// õõ�� , ���� , ������
	//BYTE			cAttackOrMove ;				// ���� , �̵�
} ;
*/
	if ( NULL == pEVENT->m_pFindCHAR )
		return;

	AIACT05 *pAct = (AIACT05*)pActDATA;

	float fPosX, fPosY;

	// 2003. 1. 2 �ֺ� 2m ���� ��ġ��..
	fPosX = pEVENT->m_pFindCHAR->Get_CurXPOS() + AI_SysRANDOM( 400 ) - 200;
	fPosY = pEVENT->m_pFindCHAR->Get_CurYPOS() + AI_SysRANDOM( 400 ) - 200;

	pEVENT->m_pSourCHAR->SetCMD_MOVE2D( fPosX, fPosY, pAct->cSpeed );
}
//-------------------------------------------------------------------------------------------------
void F_AIACT06 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
// �Ÿ����� �ƹ�Ÿ�� �ɷ�ġ�� ������ x�� ���� ����.
struct		AIACT06
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	int				iDistance ;					// �Ÿ�
	BYTE			cAbType ;					// �ɷ�ġ Ÿ��
	BYTE			cMoreLess ;					// ū�� , ������
} ;
*/
	AIACT06 *pAct = (AIACT06*)pActDATA;

	int iMinValue=9999999, iMaxValue=-9999999, iValue;

	CAI_OBJ *pMinCHAR=NULL, *pMaxCHAR=NULL;
	CAI_OBJ *pFindCHAR = pEVENT->m_pSourCHAR->AI_FindFirstOBJ( pAct->iDistance );
	int iFindCNT = 0;
//	short nMyTeamNO = pEVENT->m_pSourCHAR->Get_TeamNO();

	while( pFindCHAR ) {
		if ( ( pEVENT->m_pSourCHAR!=pFindCHAR ) && !pEVENT->m_pSourCHAR->Is_ALLIED( pFindCHAR ) ) {
			iValue = AI_Get_Ability( pFindCHAR, pAct->cAbType );
			if ( iValue < iMinValue ) {
				pMinCHAR = pFindCHAR;
				iMinValue = iValue;
			}
			if ( iValue > iMaxValue ) {
				pMaxCHAR = pFindCHAR;
				iMaxValue = iValue;
			}
		}
		pFindCHAR = pEVENT->m_pSourCHAR->AI_FindNextOBJ ();
	} 

	if ( pAct->cMoreLess ) {
		// ����
		if ( pMinCHAR )
			pEVENT->m_pSourCHAR->SetCMD_RUNnATTACK( pMinCHAR->Get_TAG() );
	} else {
		// ū
		if ( pMaxCHAR )
			pEVENT->m_pSourCHAR->SetCMD_RUNnATTACK( pMaxCHAR->Get_TAG() );
	}
}
//-------------------------------------------------------------------------------------------------
void F_AIACT07 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
struct		AIACT07
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
												// ������ �ʿ� ����
} ;
*/
	pEVENT->m_pSourCHAR->Special_ATTACK();
}
//-------------------------------------------------------------------------------------------------
void F_AIACT08 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
struct		AIACT08
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	int				iDistance ;					// �Ÿ�
	BYTE			cSpeed ;					// õõ�� , ���� , ������
} ;
*/
	AIACT08 *pAct = (AIACT08*)pActDATA;

	CAI_OBJ *pTarget = pEVENT->m_pSourCHAR->Get_TARGET ();
	if ( NULL == pTarget )
		return;

	float fPosX, fPosY;
	float fDistance, fX, fY;

	fX = pTarget->Get_CurXPOS() - pEVENT->m_pSourCHAR->Get_CurXPOS();
	fY = pTarget->Get_CurYPOS() - pEVENT->m_pSourCHAR->Get_CurYPOS();
	fDistance = pEVENT->m_pSourCHAR->Get_DISTANCE( pTarget );

	fPosX = pEVENT->m_pSourCHAR->Get_CurXPOS() - ( pAct->iDistance * fX / fDistance );
	fPosY = pEVENT->m_pSourCHAR->Get_CurYPOS() - ( pAct->iDistance * fY / fDistance );

	pEVENT->m_pSourCHAR->SetCMD_MOVE2D( fPosX, fPosY, pAct->cSpeed );
}
//-------------------------------------------------------------------------------------------------
void F_AIACT09 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif
/*
typedef struct		tagAIACT09
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	WORD			wMonster ;					// ���͹�ȣ
} AIACT09 , AIACT10 ;
*/
	AIACT09 *pAct = (AIACT09*)pActDATA;
	pEVENT->m_pSourCHAR->Change_CHAR( pAct->wMonster );
}
//-------------------------------------------------------------------------------------------------
void F_AIACT10 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif
/*
typedef struct		tagAIACT09
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	WORD			wMonster ;					// ���͹�ȣ
} AIACT09 , AIACT10 ;
*/
	AIACT10 *pAct = (AIACT10*)pActDATA;
	pEVENT->m_pSourCHAR->Create_PET ( 
			pAct->wMonster, 
			pEVENT->m_pSourCHAR->Get_CurXPOS(),
			pEVENT->m_pSourCHAR->Get_CurYPOS(), 150, 0, pEVENT->m_wPattern != AI_PATTERN_DEAD );
}
//-------------------------------------------------------------------------------------------------
void F_AIACT11 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
// idistance���� ���������� ���� numofmons ����ŭ �ڽ���
// ���� Ÿ���� �����϶�� ���.
struct		AIACT11
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	int				iDistance ;					// �Ÿ�
	int				iNumOfMonster;
} ;
*/
	AIACT11 *pAct = (AIACT11*)pActDATA;
	CAI_OBJ *pTarget = pEVENT->m_pSourCHAR->Get_TARGET ();
	if ( NULL == pTarget )
		return;

//	short nMyTeamNO = pEVENT->m_pSourCHAR->Get_TeamNO();

	CAI_OBJ *pFindCHAR = pEVENT->m_pSourCHAR->AI_FindFirstOBJ( pAct->iDistance );
	int iFindCNT = 0;
	while( pFindCHAR ) {
		if ( ( pEVENT->m_pSourCHAR!=pFindCHAR )		&&
			 pEVENT->m_pSourCHAR->Is_SameTEAM( pFindCHAR )	&&
			 pFindCHAR->Get_TARGET() == NULL ) {

			pFindCHAR->SetCMD_RUNnATTACK( pTarget->Get_TAG() );

			if ( ++iFindCNT >= pAct->iNumOfMonster )
				return;
		}
		pFindCHAR = pEVENT->m_pSourCHAR->AI_FindNextOBJ ();
	} 
}
//-------------------------------------------------------------------------------------------------
void F_AIACT12 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
// ���� ������ �ִ� ĳ���͸� ���� Ÿ������...
struct		AIACT12
{
		DWORD			dwSize ;		// �� ��Ʈ������ ũ��
		AITYPE			Type ;			// ���� Ÿ��
} ;
*/
	if ( pEVENT->m_pNearCHAR ) {
		pEVENT->m_pSourCHAR->SetCMD_RUNnATTACK( pEVENT->m_pNearCHAR->Get_TAG() );
	}
}
//-------------------------------------------------------------------------------------------------
void F_AIACT13 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
// ���ǿ��� üũ�� �ɸ��͸� ���� Ÿ������...
struct		AIACT13
{
		DWORD			dwSize ;		// �� ��Ʈ������ ũ��
		AITYPE			Type ;			// ���� Ÿ��
} ;
*/
	if ( pEVENT->m_pFindCHAR ) {
		pEVENT->m_pSourCHAR->SetCMD_RUNnATTACK( pEVENT->m_pFindCHAR->Get_TAG() );
	}
}
//-------------------------------------------------------------------------------------------------
void F_AIACT14 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
// ��ó distance���� ���� ������ ���ῡ�� �ڽ��� ���� Ÿ���� 
// �����϶�� ����Ѵ�.
struct		AIACT14
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
	int				iDistance ;					// �Ÿ�
} ;
*/
	CAI_OBJ *pTarget   = pEVENT->m_pSourCHAR->Get_TARGET ();
	if ( NULL == pTarget )
		return;

	AIACT14 *pAct = (AIACT14*)pActDATA;

//	short nMyTeamNO = pEVENT->m_pSourCHAR->Get_TeamNO ();
	short CharNO    = pEVENT->m_pSourCHAR->Get_CharNO ();

	CAI_OBJ *pFindCHAR = pEVENT->m_pSourCHAR->AI_FindFirstOBJ( pAct->iDistance );
	while( pFindCHAR ) {
		if ( ( pEVENT->m_pSourCHAR!=pFindCHAR )		&&
			 pFindCHAR->Get_CharNO() == CharNO		&&
			 pEVENT->m_pSourCHAR->Is_SameTEAM( pFindCHAR )	&&
			 pFindCHAR->Get_TARGET() == NULL ) {
			pFindCHAR->SetCMD_RUNnATTACK( pTarget->Get_TAG() );
		}
		pFindCHAR = pEVENT->m_pSourCHAR->AI_FindNextOBJ ();
	} 
}
//-------------------------------------------------------------------------------------------------
void F_AIACT15 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
// ���� Ÿ���� ���� �ڽ��� ������ �ɸ��ͷ� �ٲ۴�.
struct		AIACT15
{
	DWORD			dwSize ;					// �� ��Ʈ������ ũ��
	AITYPE			Type ;						// ���� Ÿ��
} ;
*/
	if ( pEVENT->m_pDestCHAR ) {
		pEVENT->m_pSourCHAR->SetCMD_RUNnATTACK( pEVENT->m_pDestCHAR->Get_TAG() );
	}
}

//-------------------------------------------------------------------------------------------------
void F_AIACT16 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
struct AIACT16
{
	DWORD	dwSize;		/// �� ��Ʈ������ ũ��
	AITYPE	Type;		/// Type = AIACT_16 . ����
	int		iDistance ;	/// ������ �߽����� �� ���� �̳��� ����
};
*/
	AIACT16 *pAct = (AIACT16*)pActDATA;

	pEVENT->m_pSourCHAR->Run_AWAY ( pAct->iDistance );
}

//-------------------------------------------------------------------------------------------------
void F_AIACT17 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
struct AIACT17
{
	DWORD	dwSize;		/// �� ��Ʈ������ ũ��
	AITYPE	Type;		/// Type = AIACT_16 . ����
	short	item0;		/// ������ ��ȣ 0  . ���� �����۹�ȣ�� ������ -1�� ���� �ִ´�
	short	item1;		/// ������ ��ȣ 1
	short	item2;		/// ������ ��ȣ 2
	short	item3;		/// ������ ��ȣ 3
	short	item4;		/// ������ ��ȣ 4 
};
*/
	AIACT17 *pAct = (AIACT17*)pActDATA;

	short nDropITEM = pAct->m_ITEMS[ pEVENT->m_pSourCHAR->Get_RANDOM(5) ];
	if ( nDropITEM > 0 )
		pEVENT->m_pSourCHAR->Drop_ITEM( nDropITEM, (BYTE)pAct->iToOwner );
}

//-------------------------------------------------------------------------------------------------
void F_AIACT18 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif

/*
 * Struct Name : AIACT18 - <<2004.1.29 �߰�>>
 * ������ ��ȣ�� ���������� ������ ����ȣ�� ���. (���͹�ȣ, �Ÿ�, ������) 
 * �߰� ���� : ������ �Ÿ��ȿ� ������ ��������ŭ�� ������ ��ȣ�� ���Ϳ��� ����ȣ�� ���
struct AIACT18
{
	DWORD		dwSize;			/// �� ��Ʈ������ ũ��
	AITYPE		Type;			/// Type���� AIACT_18

	WORD		cMonster ;		// ���͹�ȣ
	WORD		wHowMany ;		// ���
	int			iDistance ;		// �Ÿ�
};
*/
	CAI_OBJ *pTarget   = pEVENT->m_pSourCHAR->Get_TARGET ();
	if ( NULL == pTarget )
		return;

	AIACT18 *pAct = (AIACT18*)pActDATA;

	CAI_OBJ *pFindCHAR = pEVENT->m_pSourCHAR->AI_FindFirstOBJ( pAct->iDistance );

//	short nMyTeamNO = pEVENT->m_pSourCHAR->Get_TeamNO();

	WORD wHowMany = 0;
	while( pFindCHAR ) {
		if ( ( pEVENT->m_pSourCHAR!=pFindCHAR )				&&
			 pFindCHAR->Get_CharNO() == pAct->cMonster		&&
			 pEVENT->m_pSourCHAR->Is_SameTEAM( pFindCHAR )	&&
			 pFindCHAR->Get_TARGET() == NULL ) {

			pFindCHAR->SetCMD_RUNnATTACK( pTarget->Get_TAG() );

			if ( ++wHowMany >= pAct->wHowMany )
				break;
		}
		pFindCHAR = pEVENT->m_pSourCHAR->AI_FindNextOBJ ();
	} 
}

//-------------------------------------------------------------------------------------------------
void F_AIACT19 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif
/*
 * Struct Name : AIACT19 - <<2004.1.29 �߰�>>
 * ���� ������ �ִ� �ƹ�Ÿ�� ����Ÿ������ ���� 
struct AIACT19
{
	DWORD		dwSize;				/// �� ��Ʈ������ ũ��
	AITYPE		Type;				/// Type���� AIACT_19

	/// �߰� ���� �ʿ�
};
*/
	if ( pEVENT->m_pNearCHAR ) {
		pEVENT->m_pSourCHAR->SetCMD_RUNnATTACK( pEVENT->m_pNearCHAR->Get_TAG() );
	}
}

//-------------------------------------------------------------------------------------------------
void F_AIACT20 (stActHead *pActDATA, t_AIPARAM *pEVENT)
{
#if !defined( __SERVER ) && !defined( __VIRTUAL_SERVER )
	return;
#endif
/*
 * Struct Name : AIACT20 - <<2004.1.29 �߰�>>
 * ������ ���͸� ��ȯ��Ų��. �ڽ�/������(�ڽ��� ������ ĳ����)/����Ÿ��(�ڽ��� �����ϴ� ĳ����).
 * ��ó ?���� �̳��� ��ȯ 
 * �߰����� : �������� 1�����̴�.
struct AIACT20
{
	DWORD		dwSize;				/// �� ��Ʈ������ ũ��
	AITYPE		Type;				/// Type���� AIACT_20

	WORD		cMonster;			/// ���͹�ȣ
	BYTE		btPos;				/// ��ȯ��ġ : 0 = �ڽ� ����,  1 = ������ ����(�ڽ��� ������), 2 = ����Ÿ�� ����(�ڽ��� �����ϰ� �ִ�)
	int			iDistance ;			// �Ÿ� . �� �Ÿ� �̳��� ��ȯ
};
*/

	AIACT20 *pAct = (AIACT20*)pActDATA;
	switch( pAct->btPos ) {
		case 0 :	// �ڽ� ����...
		{
			pEVENT->m_pSourCHAR->Create_PET( pAct->cMonster, 
							pEVENT->m_pSourCHAR->Get_CurXPOS(),
							pEVENT->m_pSourCHAR->Get_CurYPOS(),
							pAct->iDistance, 0, pEVENT->m_wPattern != AI_PATTERN_DEAD );
			break;
		}

		case 1 :	// �ڽ��� ������...
		{
			if ( pEVENT->m_pDestCHAR ) {
				pEVENT->m_pSourCHAR->Create_PET( pAct->cMonster, 
								pEVENT->m_pDestCHAR->Get_CurXPOS(),
								pEVENT->m_pDestCHAR->Get_CurYPOS(),
								pAct->iDistance, 0, pEVENT->m_wPattern != AI_PATTERN_DEAD );
			}
			break;
		}
			
		case 2 :	// �ڽ��� ��������...
		{
			CAI_OBJ *pTarget = pEVENT->m_pSourCHAR->Get_TARGET ();
			if ( pTarget ) {
				pEVENT->m_pSourCHAR->Create_PET( pAct->cMonster, 
								pTarget->Get_CurXPOS(),
								pTarget->Get_CurYPOS(),
								pAct->iDistance, 0, pEVENT->m_wPattern != AI_PATTERN_DEAD );
			}
			break;
		}
	} ;
}

void F_AIACT21 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
{	
	// ��� ����.
}
void F_AIACT22 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
{	
	// ��� ����.
}
void F_AIACT23 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
{
/*
// �ڻ��Ų��
struct AIACT23
{
	DWORD	dwSize;
	AITYPE	Type;
};
*/
#ifdef	__SERVER
	pEVENT->m_pSourCHAR->Add_DAMAGE( pEVENT->m_pSourCHAR->Get_HP()+1 );
#endif
}
void F_AIACT24 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
{
/*
/// ��ų�� ������ ����� ���� ���
struct AIACT24
{
	DWORD		dwSize;			/// �� ��Ʈ������ ũ��
	AITYPE		Type;			/// Type

	BYTE		btTarget;		// 0=���ǿ� üũ�� ĳ����, 1=���� ����Ÿ��, 2=�ڱ��ڽ�(������ų)
	short		nSkill;			// ��ų��ȣ
	short		nMotion;		// ���۹�ȣ -1 = ���۾���
};
*/
#ifdef	__SERVER
	AIACT24 *pAct = (AIACT24*)pActDATA;
	switch( pAct->btTarget )
	{
		case 0 :
			if ( pEVENT->m_pFindCHAR ) {
				pEVENT->m_pSourCHAR->SetCMD_Skill2OBJ( pEVENT->m_pFindCHAR->Get_TAG(), pAct->nSkill, pAct->nMotion );
			}
			break;
		case 1 :
		{
			CAI_OBJ *pTarget   = pEVENT->m_pSourCHAR->Get_TARGET ();
			if ( pTarget ) {
				pEVENT->m_pSourCHAR->SetCMD_Skill2OBJ( pTarget->Get_TAG(), pAct->nSkill, pAct->nMotion );
			}
			break;
		}
		case 2 :
			pEVENT->m_pSourCHAR->SetCMD_Skill2SELF( pAct->nSkill, pAct->nMotion );
			break;
	}
#endif
}
void F_AIACT25 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
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
#ifdef	__SERVER
	if ( NULL == pEVENT->m_pLocalNPC )
		return;

	AIACT25 *pAct = (AIACT25*)pActDATA;

	int iValue  = pEVENT->m_pLocalNPC->Get_ObjVAR( (BYTE)pAct->nVarNo );
	int iResult = ::Result_AiOP( pAct->btOp, iValue, pAct->iValue );

	pEVENT->m_pLocalNPC->Set_ObjVAR( (BYTE)pAct->nVarNo, iResult );
#endif
}
void F_AIACT26 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
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
#ifdef	__SERVER
	AIACT26 *pAct = (AIACT26*)pActDATA;

	int iValue  = pEVENT->m_pSourCHAR->Get_WorldVAR( pAct->nVarNo );
	int iResult = ::Result_AiOP( pAct->btOp, iValue, pAct->iValue );

	pEVENT->m_pSourCHAR->Set_WorldVAR( pAct->nVarNo, iResult );
#endif
}
void F_AIACT27 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
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
#ifdef	__SERVER
	AIACT27 *pAct = (AIACT27*)pActDATA;

	int iValue  = pEVENT->m_pSourCHAR->Get_EconomyVAR( pAct->nVarNo );
	int iResult = ::Result_AiOP( pAct->btOp, iValue, pAct->iValue );
	
	pEVENT->m_pSourCHAR->Set_EconomyVAR( pAct->nVarNo, iResult );
#endif
}
void F_AIACT28 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
{	
/*
struct AIACT28
{
	DWORD		dwSize;			/// �� ��Ʈ������ ũ��
	AITYPE		Type;			/// Type

	BYTE		btMsgType;		/// 0 = �����ʵ�, 1 = ������, 2=��ü����
	short		nMsgLength;		/// ����� ���� NULL����
	char		szMsg[ 1 ];		/// ���
};
*/
//	AIACT28 *pAct = (AIACT28*)pActDATA;
	AIACT28_STR *pAct = (AIACT28_STR*)pActDATA;
	
#ifndef	__SERVER
	pEVENT->m_pSourCHAR->Say_MESSAGE( pAct->szMsg );
#else
	char *szMSG = pAct->szMsg;
	switch( pAct->btMsgType )
	{
		case 0 :
			pEVENT->m_pSourCHAR->Send_gsv_CHAT( szMSG  );
			break;
		case 1 :
			pEVENT->m_pSourCHAR->Send_gsv_SHOUT( szMSG );
			break;
		case 2 :
			pEVENT->m_pSourCHAR->Send_gsv_ANNOUNCE_CHAT( szMSG );
			break;
	}
#endif
}

void F_AIACT29 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
{	
#ifdef	__SERVER
	CAI_OBJ *pOwner = pEVENT->m_pSourCHAR->Get_CALLER ();
	if ( !pOwner )
		return;

	AIACT29 *pAct = (AIACT29*)pActDATA;

	float fPosX, fPosY;
	float fDistance;

	fDistance = pEVENT->m_pSourCHAR->Get_DISTANCE( pOwner );

	fPosX = pOwner->Get_CurXPOS() - ( 0.2f * fDistance );
	fPosY = pOwner->Get_CurYPOS() - ( 0.2f * fDistance );

	pEVENT->m_pSourCHAR->SetCMD_MOVE2D( fPosX, fPosY, 1 /* run mode */ );
#endif
}

void F_AIACT30 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
{	
#ifdef	__SERVER
	AIACT30 *pAct = (AIACT30*)pActDATA;

	t_HASHKEY*pHash = (t_HASHKEY*)( pAct->szTrigger );

	if ( OBJ_MOB == pEVENT->m_pSourCHAR->Get_CharObjTAG() ) {
		// ���� ������ Ʈ���Ÿ� �ߵ��Ҽ� ���� !!! ��??? ������ �������� �ʴ´�.
		if ( pEVENT->m_pLocalNPC  )
			pEVENT->m_pLocalNPC->Set_TRIGGER( *pHash );
		return;
	}

	pEVENT->m_pSourCHAR->Set_TRIGGER( *pHash );
#endif
}

void F_AIACT31 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
{	
/*
// ������ ������ �ִ� ����Ÿ�� ����
struct AIACT31
{
	DWORD		dwSize;
	AITYPE		Type;
};
*/
#ifdef	__SERVER
	AIACT31 *pAct = (AIACT31*)pActDATA;

	CAI_OBJ *pOwner = pEVENT->m_pSourCHAR->Get_CALLER ();
	if ( pOwner ) {
		CAI_OBJ *pTarget = pOwner->Get_TARGET();
		if ( pTarget ) {
			// ����
			if ( !pEVENT->m_pSourCHAR->Is_ALLIED( pTarget ) )
				pEVENT->m_pSourCHAR->SetCMD_RUNnATTACK( pTarget->Get_TAG() );
		}
	}
#endif
}

void F_AIACT32 (stActHead *pActDATA, t_AIPARAM *pEVENT)		
{	
/*
// PK ��� On/Off. NPC�� �����ϴ� ����.
struct AIACT32
{
	DWORD		dwSize;
	AITYPE		Type;

	short		nZoneNo;	// ����ȣ, ����� ������ 0
	BYTE		btOnOff;	// 0 = Off , 1 = On
};
*/	
#ifdef	__SERVER
	AIACT32 *pAct = (AIACT32*)pActDATA;

	if ( 0 == pAct->nZoneNo ) {
		// ���� ��...
		g_pZoneLIST->Set_PK_FLAG( pEVENT->m_pSourCHAR, pAct->btOnOff );
	} else {
		g_pZoneLIST->Set_PK_FLAG( pAct->nZoneNo, pAct->btOnOff );
	}
#endif
}

void F_AIACT33 (stActHead *pActDATA, t_AIPARAM *pEVENT )
{
/*
// ����/��� ���� ������ On/Off ��
struct AIACT33
{
	DWORD		dwSize;
	AITYPE		Type;

	short		nZonNo;	// 0 : ������, �̿�: ����ȣ
	BYTE		btOp;	// 0: Off, 1: On, 2: Toggle
};
*/
#ifdef	__SERVER
	AIACT33 *pAct = (AIACT33*)pActDATA;

	if ( 0 == pAct->nZonNo ) {
		// ������..
		if ( 2 == pAct->btOp ) {
			g_pZoneLIST->Toggle_RegenSYSTEM( pEVENT->m_pSourCHAR );
		} else {
			g_pZoneLIST->Set_RegenSYSTEM( pEVENT->m_pSourCHAR, pAct->btOp );
		}
	} else {
		if ( 2 == pAct->btOp ) {
			g_pZoneLIST->Toggle_RegenSYSTEM( pAct->nZonNo );
		} else {
			g_pZoneLIST->Set_RegenSYSTEM( pAct->nZonNo, pAct->btOp );
		}
	}
#endif
}

void F_AIACT34 (stActHead *pActDATA, t_AIPARAM *pEVENT )
{
/*
// ����ĳ���Ϳ��� ������ ����
struct AIACT34
{
	DWORD		dwSize;
	AITYPE		Type;

	short		nItemNo;
	short		nCnt;		// nItemNo �� nCnt ���� ��ŭ ����
};
*/
#ifdef	__SERVER
	AIACT34 *pAct = (AIACT34*)pActDATA;

	CAI_OBJ *pOwner = pEVENT->m_pSourCHAR->Get_CALLER ();
	if ( !pOwner )
		return;

	pOwner->Recv_ITEM( pAct->nItemNo, pAct->nCnt );
#endif
}

void F_AIACT35 (stActHead *pActDATA, t_AIPARAM *pEVENT )
{
/*
// ���� ���� �� ����
struct	AIACT35
{
	DWORD			dwSize;
	AITYPE			Type;

	short			nVarNo;	// ���� ��ȣ : 0 ~ 4
	int				iValue;	// ���� �����Ͱ�
	BYTE			btOp;	// (�׼���) 5 = ���ٲ�, 6 = ����, 7 = ����
};
*/
#ifdef	__SERVER
	AIACT35 *pAct = (AIACT35*)pActDATA;

	int iValue  = pEVENT->m_pSourCHAR->Get_AiVAR( pAct->nVarNo );
	int iResult = ::Result_AiOP( pAct->btOp, iValue, pAct->iValue );

	pEVENT->m_pSourCHAR->Set_AiVAR( pAct->nVarNo, iResult );
#endif
}


void F_AIACT36 (stActHead *pActDATA, t_AIPARAM *pEVENT )
{
/*
// �ڽ��� ��ó�� ?�� ���� ��ȯ ( ���� 10������ ���� ���� �߰� ) 
struct		AIACT36 
{
	DWORD		dwSize ;		// �� ��Ʈ������ ũ��
	AITYPE		Type ;			// ���� Ÿ��
	WORD		cMonster ;		// ���͹�ȣ

	BYTE		btMaster;	// 0: ������, 1: �ڽ�(ȣ����)�� �������� ����
} ;
*/
	AIACT36 *pAct = (AIACT36*)pActDATA;
	pEVENT->m_pSourCHAR->Create_PET ( 
			pAct->cMonster, 
			pEVENT->m_pSourCHAR->Get_CurXPOS(),
			pEVENT->m_pSourCHAR->Get_CurYPOS(), 150, pAct->btMaster, pEVENT->m_wPattern != AI_PATTERN_DEAD );
}

void F_AIACT37 (stActHead *pActDATA, t_AIPARAM *pEVENT )
{
/*
// ?�ֺ��� ?���� �̳��� ?�� ���� ��ȯ ( ���� 20������ ���� ���� �߰� )
struct AIACT37
{
	DWORD		dwSize;				/// �� ��Ʈ������ ũ��
	AITYPE		Type;				/// Type���� AIACT_20

	WORD		cMonster;			/// ���͹�ȣ
	BYTE		btPos;				/// ��ȯ��ġ : 0 = �ڽ� ����,  1 = ������ ����(�ڽ��� ������), 2 = ����Ÿ�� ����(�ڽ��� �����ϰ� �ִ�)
	int			iDistance;			// �Ÿ� . �� �Ÿ� �̳��� ��ȯ

	BYTE		btMaster;	// 0: ������, 1: �ڽ�(ȣ����)�� �������� ����
};
*/
	AIACT37 *pAct = (AIACT37*)pActDATA;

	switch( pAct->btPos ) {
		case 0 :	// �ڽ� ����...
		{
			pEVENT->m_pSourCHAR->Create_PET( pAct->cMonster, 
							pEVENT->m_pSourCHAR->Get_CurXPOS(),
							pEVENT->m_pSourCHAR->Get_CurYPOS(),
							pAct->iDistance, pAct->btMaster, pEVENT->m_wPattern != AI_PATTERN_DEAD );
			break;
		}

		case 1 :	// �ڽ��� ������...
		{
			if ( pEVENT->m_pDestCHAR ) {
				pEVENT->m_pSourCHAR->Create_PET( pAct->cMonster, 
								pEVENT->m_pDestCHAR->Get_CurXPOS(),
								pEVENT->m_pDestCHAR->Get_CurYPOS(),
								pAct->iDistance, pAct->btMaster, pEVENT->m_wPattern != AI_PATTERN_DEAD );
			}
			break;
		}
			
		case 2 :	// �ڽ��� ��������...
		{
			CAI_OBJ *pTarget = pEVENT->m_pSourCHAR->Get_TARGET ();
			if ( pTarget ) {
				pEVENT->m_pSourCHAR->Create_PET( pAct->cMonster, 
								pTarget->Get_CurXPOS(),
								pTarget->Get_CurYPOS(),
								pAct->iDistance, pAct->btMaster, pEVENT->m_wPattern != AI_PATTERN_DEAD );
			}
			break;
		}
	} ;
}

//-------------------------------------------------------------------------------------------------
void F_AIACT_NULL(stActHead *pActDATA, t_AIPARAM *pEVENT )
{
	;
}

tagActionFUNC g_FuncACTION[] = {
	{	NULL		},
	{	F_AIACT00	},
	{	F_AIACT01	},
	{	F_AIACT02	},
	{	F_AIACT03	},
	{	F_AIACT04	},
	{	F_AIACT05	},
	{	F_AIACT06	},
	{	F_AIACT07	},
	{	F_AIACT08	},
	{	F_AIACT09	},
	{	F_AIACT10	},
	{	F_AIACT11	},
	{	F_AIACT12	},
	{	F_AIACT13	},
	{	F_AIACT14	},
	{	F_AIACT15	},
	{	F_AIACT16	},
	{	F_AIACT17	},
	{	F_AIACT18	},
	{	F_AIACT19	},
	{	F_AIACT20	},

	{	F_AIACT21	},

	{	F_AIACT22	},
	{	F_AIACT23	},
	{	F_AIACT24	},
	{	F_AIACT25	},
	{	F_AIACT26	},
	{	F_AIACT27	},
	{	F_AIACT28	},

	{	F_AIACT29	},
	{	F_AIACT30	},

	{	F_AIACT31	},
	{	F_AIACT32	},

	{	F_AIACT33	},
	{	F_AIACT34	},
	{	F_AIACT35	},

	{	F_AIACT36	},
	{	F_AIACT37	},

	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
	{	F_AIACT_NULL	},
} ;

//-------------------------------------------------------------------------------------------------
