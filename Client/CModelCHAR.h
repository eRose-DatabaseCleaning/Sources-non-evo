/*
	$Header: /Client/CModelCHAR.h 34    04-09-02 3:16p Jeddli $
*/
#ifndef	__CMODELCHAR_H
#define	__CMODELCHAR_H
/*
#include "IO_Model.h"
*/
#include "IO_Basic.h"
#include "Common\IO_Motion.h"
#include <list>
//-------------------------------------------------------------------------------------------------

struct tagBONE_EFFECT
{
	char		m_cBoneIDX;
	t_HASHKEY	m_EffectFILE;
} ;

///
///	character Model class
///

class CCharMODEL 
{
private:
#ifdef	_DEBUG
	CStrVAR			m_Name;
#endif
	bool			m_bIsFemale;							///< ���ڳ�? ���ڳ�?
			
	t_HASHKEY		m_HashSkelFILE;							///< Skeleton
	t_HASHKEY		m_BoneAniFILE[ MAX_MOB_ANI ];			///< Animation file
	short			m_nBoneEFFECT;							///< ?? Bone effect.
	tagBONE_EFFECT *m_pBoneEFFECT;							///< ??

	short			m_nBodyPartCNT;
	union 
	{
		// �޸� �Ҵ��� ���� �ʰ� ���� ����Ÿ�� �����͸� ���´�.
		CMODEL<CCharPART>	  *m_pBodyPART[ MAX_BODY_PART ];
		struct 
		{
			// ������ t_CharPART�� ����...
			CMODEL<CCharPART> *m_pFaceMODEL;		///< loadvisible, linkBone ( head ), addRenderUnit : boneidx >= 0
			CMODEL<CCharPART> *m_pHairMODEL;		///< loadVisible, linkbone ( head ), addRenderUnit
			CMODEL<CCharPART> *m_pHelmetMODEL;		///< loadVisible, linkbone ( head ), addRenderUnit

			CMODEL<CCharPART> *m_pArmorMODEL;		///< skinning addRenderUnit
			CMODEL<CCharPART> *m_pGauntletMODEL;	///< skinning addRenderUnit
			CMODEL<CCharPART> *m_pBootsMODEL;		///< skinning addRenderUnit

			CMODEL<CCharPART> *m_pGoggleMODEL;
			CMODEL<CCharPART> *m_pKnapsackMODEL;
								
			CMODEL<CCharPART> *m_pRWeaponMODEL;		///< loadVisible, linkDummy, addRenderUnit : dummyidx >= 0
			CMODEL<CCharPART> *m_pLWeaponMODEL;		///< loadVisible, linkDummy, addRenderUnit
		} ;
	} ;

public:
	/// RenderUnit ���Լ��� ������..
	int						m_iRenderUnitOrder;
	std::list< int >		m_RenderUnitPart[ MAX_BODY_PART ];

	void					ClearRenderUnitParts();
	
public :
	CCharMODEL ();
	~CCharMODEL ();

	void  Set_SEX( bool bIsFemale )		{	m_bIsFemale=bIsFemale;	}
	short GetPartCNT ()					{	return m_nBodyPartCNT;	}
	bool  Load_MOBorNPC ( CFileSystem* pFileSystem, t_HASHKEY *pSkelKEY, t_HASHKEY *pAniKEY, t_HASHKEY *pEftKEY);

	bool  SetPartMODEL (short nPartIdx, CMODEL<CCharPART> *pPartMODEL);
	bool  SetPartMODEL (short nPartIdx, short nModelIdx);
	void  SetMeshMOTION (HNODE **pphPartVIS, short nAniIdx);

	HNODE			   GetZSKEL (void)				{	return g_DATA.KEY_GetSKELETON( m_HashSkelFILE );			}
	tagMOTION		  *GetMOTION (short nIndex);
	CMODEL<CCharPART> *GetCharPART(short nPartIdx)	{	return m_pBodyPART[ nPartIdx ];								}

	///
	/// .Txt ������ ������Ʈ �ε�
	///
	HNODE*CreatePART (char *szName, HNODE hModelNODE, short nPartIdx, short nLinkDummyIdx=-1);
	void  DeletePART (short nPartIdx, HNODE *pVIS);
	
	short		GetBoneEffectCNT ()					{	return m_nBoneEFFECT;	}
	CEffect**	CreateBoneEFFECT (HNODE hMODEL);
	void		DeleteBoneEFFECT (CEffect **ppEffect);
	void		LinkBoneEFFECT (HNODE hMODEL, CEffect **ppEffect);
	void		UnlinkBoneEFFECT (CEffect **ppEffect);
} ;


//-------------------------------------------------------------------------------------------------
///
/// Character model data( for npc list )
/// m_pMODELS is data( model, skeleton ) array for each npc 
///

class CCharModelDATA 
{
private:
	short		m_nModelCNT;
	CCharMODEL *m_pMODELS;

public :
	CCharModelDATA ();
	~CCharModelDATA ();

	CCharMODEL *GetMODEL(short nIndex) 
	{
		_ASSERT( nIndex>=0 && nIndex<m_nModelCNT );
		return &m_pMODELS[ nIndex ];
	}

	bool Load_MOBorNPC (char *szFileName);
	void Free (void);
} ;



extern CCharModelDATA	g_MOBandNPC;
//-------------------------------------------------------------------------------------------------

#endif
