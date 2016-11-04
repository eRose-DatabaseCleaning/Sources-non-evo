#ifndef	__CALCULATION_H
#define	__CALCULATION_H
#include "CItem.h"

#ifdef	__SERVER
	class	CObjCHAR;
	typedef	CObjCHAR	CObjUSER;
#else
	class	CObjUSER;
	class	CObjCHAR;
#endif
class CObjMOB;

//-------------------------------------------------------------------------------------------------
class CUserDATA;
class CCal {
private:
	static int  Get_SuccessRATE(CObjCHAR *pATK, CObjCHAR *pDEF );
	static WORD Get_BasicDAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, WORD wHitCNT, int iSuc);
	static WORD Get_MagicDAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, WORD wHitCNT, int iSuc);

	static int  Get_WeaponSkillDAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, short nSkillIDX, int iSuccess);
	static int  Get_MagicSkillDAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, short nSkillIDX, int iSuccess);

public :
	static bool Get_DropITEM (int iLevelDiff, CObjMOB *pMobCHAR, tagITEM &sITEM, int iZoneNO, int iDropRate, int iCharm);
	static __int64	Get_EXP (CObjCHAR *pAtkCHAR, CObjCHAR *pDefCHAR, int iGiveDamage);

	static WORD Get_DAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, WORD wHitCNT);
	static WORD Get_SkillDAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, short nSkillIDX, WORD wHitCNT);
	static int	Get_StorageFEE (int iBasePrice, int iPriceRate, UINT uiDupCnt)
	{
		return (int)( ( iBasePrice * 5 / 1000 + 1 ) * ( iPriceRate+1 ) ) * uiDupCnt;
	}

	static int  Get_RewardVALUE( BYTE btEquation, int S_REWARD, CUserDATA *pUSER, short nDupCNT );

	static int  Get_SkillAdjustVALUE( CObjUSER *pCHAR, short nSkillIDX, short nCol, int iSpellerINT );

	static int	GetMoney_WhenAPPRAISAL ( int iBasePrice, int iCurDurability )
	{
		return (int)( ( iBasePrice+10000 ) * ( iCurDurability + 50 ) / 10000.f );
	}

	// ��񿡼� ���� �и��� �ʿ��� mp & money
	// param :
	//		short ORI_QUAL = ITEM_QUALITY( pInITEM->GetTYPE(), pInITEM->GetItemNO() );
	//		short GEM_QUAL = ITEM_QUALITY( ITEM_TYPE_GEM,	  pInITEM->GetGemNO() );
	static int  GetMP_WhenBreakupGEM ( short ORI_QUAL, short GEM_QUAL )
	{
		return ( (ORI_QUAL/2) + GEM_QUAL );
	}
	static int  GetMONEY_WhenBreakupGEM ( short ORI_QUAL, short GEM_QUAL )
	{
		return ( ORI_QUAL*5 + 50 );
	}

	// ��� ���� ���ؽ� �ʿ��� mp & money
	// param : ITEM_QUALITY( pInITEM->GetTYPE(), pInITEM->GetItemNO() );
	static int  GetMP_WhenBreakupMAT ( short ORI_QUAL )
	{
		return ( ORI_QUAL + 30 );
	}
	static int  GetMONEY_WhenBreakupMAT ( short ORI_QUAL )
	{
		return ( ORI_QUAL*10 + 20 );
	}

	// ����� ����� �ø��� �ʿ��� mp & money
	// param :
	//		pInITEM->GetGrade ()
	//		ITEM_QUALITY( pInITEM->GetTYPE(), pInITEM->GetItemNO() );
	static int  GetMP_WhenUpgradeEQUIP ( short nCurGrade, short ITEM_QUAL )
	{
		return (int)( (nCurGrade+4) * (ITEM_QUAL+20)*0.9f );
	}
	static int  GetMONEY_WhenUpgradeEQUIP( short nCurGrade, short ITEM_QUAL )
	{
		return (int)( (nCurGrade) * (nCurGrade+1) * ITEM_QUAL * (ITEM_QUAL+20) * 0.2f );
	}

	// ���� ��Ƽ �������� ���Ǳ� ���� ������ ����ġ��
	static int  Get_PartyLevelUpNeedEXP( int iCurPartyLEV )
	{
		return ( (iCurPartyLEV+7) * (iCurPartyLEV+10)*5 ) + 200;
	}

	static __int64 Get_NeedRawEXP (int iLevel);
} ;

//-------------------------------------------------------------------------------------------------
#endif
 