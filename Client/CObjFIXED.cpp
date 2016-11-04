/*
	$Header: /Client/CObjFIXED.cpp 44    05-06-08 5:05p Gioend $
*/
#include "stdAFX.h"
#include "CObjFIXED.h"
#include "GameProc/CDayNNightProc.h"
#include "../Common/IO_STB.H"
#include "CObjGROUND.h"

//UINT CObjFIXED::m_iLightMapTAG = 0;

//-------------------------------------------------------------------------------------------------
DWORD CObjFIXED::m_dwCreateOrder=0;

///
/// @bug ��Ʈ ��常 �������� ���� �ؾߵǴ°� �ƴѰ�?
///
void  CObjFIXED::SetPOSITION (D3DVECTOR &Position)
{
	HNODE hNode = GetRootZNODE();
	if( hNode == NULL )
		return;

	::setPosition  ( hNode, Position.x, Position.y, Position.z );
	/*
	for (short nP=0; nP<m_pMODEL->m_nPartCNT; nP++) {
		if ( m_pHNODES[ nP ] != NULL ) {
			::setPosition  ( m_pHNODES[ nP ], Position.x, Position.y, Position.z );
		}
	}
	*/
}

///
/// @bug �̰͵� �̻�.. ��ũ�� ���ٰ� �Ŵ°ž�?
///
void  CObjFIXED::LinkNODE (HNODE hTarget, CPointPART *pDummyPoint)
{
	for (short nP=0; nP<m_pMODEL->m_nPartCNT; nP++) 
	{
		if ( m_pHNODES[ nP ] != NULL ) 
		{
			::setPosition  ( m_pHNODES[ nP ], pDummyPoint->m_Transform.x, pDummyPoint->m_Transform.y, pDummyPoint->m_Transform.z );
			::setQuaternion( m_pHNODES[ nP ], pDummyPoint->m_Rotate.w, pDummyPoint->m_Rotate.x, pDummyPoint->m_Rotate.y, pDummyPoint->m_Rotate.z );
			::linkNode (hTarget, m_pHNODES[ nP ] );
		}
	}
}

//-------------------------------------------------------------------------------------------------
///
/// 
///
void  CObjFIXED::UnlinkVisibleWorld (void)
{
	for (short nP=0; nP<m_pMODEL->m_nPartCNT; nP++) {
		if ( m_pHNODES[ nP ] != NULL )
			::unlinkVisibleWorld( m_pHNODES[ nP ] );
	}
}

void CObjFIXED::LinkToModel( HNODE hModel )
{
	for (short nP=0; nP<m_pMODEL->m_nPartCNT; nP++) 
	{
		if ( m_pHNODES[ nP ] != NULL ) 
		{
			::linkNode ( hModel, m_pHNODES[ nP ] );
		}
	}
}

//--------------------------------------------------------------------------------
/// class : CObjFIXED
/// @param  D3DVECTOR &PosSCR ���� ��ũ����ǥ( �ƿ�ǲ )
/// @brief  : ���� ������Ʈ ��ġ�� ��ũ�� ��ǥ�� ����
//--------------------------------------------------------------------------------

void CObjFIXED::GetScreenPOS ( D3DVECTOR &PosSCR)
{	
	// ���� ��ǥ�� Ű�� ���� ��ġ�� �̸���� ��ġ�� ����
	::worldToScreen( m_PosCUR.x, m_PosCUR.y, getPositionZ(GetRootZNODE()), &PosSCR.x, &PosSCR.y, &PosSCR.z );
}

#include "IO_Basic.h"
void CObjFIXED::InsertToScene (void)
{
	if ( m_bIsVisible || !m_pMODEL )
		return;

	m_bIsVisible = true;

	HNODE hNode = GetRootZNODE();
	::insertToScene( hNode );

	/*float fMin[3] = { 0.0f, 0.0f, 0.0f };
	float fMax[3] = { 0.0f, 0.0f, 0.0f };

	CMODEL<CFixedPART>* pModel = NULL;

	switch( this->Get_TYPE() )
	{
		case OBJ_GROUND:
			{
				CObjTREE *pTree = (CObjTREE *)this;
				pModel = g_DATA.m_ModelDECO.GetMODEL( pTree->m_nTreeIdx );	
			}
			break;

		case OBJ_CNST:
			{
				CObjCNST *pCnst = (CObjCNST *)this;
				pModel = g_DATA.m_ModelCNST.GetMODEL( pCnst->m_nCnstIdx );	
			}
			break;
	}

	if( pModel )
	{
		fMin[ 0 ] = pModel->m_BBMin[ 0 ];
		fMin[ 1 ] = pModel->m_BBMin[ 1 ];
		fMin[ 2 ] = pModel->m_BBMin[ 2 ];

		fMax[ 0 ] = pModel->m_BBMax[ 0 ];
		fMax[ 1 ] = pModel->m_BBMax[ 1 ];
		fMax[ 2 ] = pModel->m_BBMax[ 2 ];
	}
	
	setObbox (	this->GetRootZNODE(),
					( fMax[0]+fMin[0] )/2,
					( fMax[1]+fMin[1] )/2,
					( fMax[2]+fMin[2] )/2,
					( fMax[0]-fMin[0] )/2,
					( fMax[1]-fMin[1] )/2,
					( fMax[2]-fMin[2] )/2
				);*/
	

	//----------------------------------------------------------------------------------------------------	
	/// @brief CHILD��InsertToScene����
	//----------------------------------------------------------------------------------------------------
	//for (short nP=0; nP<m_pMODEL->m_nPartCNT; nP++) {
	//	if ( m_pHNODES[ nP ] != NULL )
	//		::insertToScene( m_pHNODES[ nP ] );		// CObjGROUND::RegisterToEZIN
	//}

	/// ��ũ�� ����Ʈ�� InsertToScene�� ȣ������ �ʴ´�.
	/*for ( short nP=0; nP<m_pMODEL->m_nDummyPointCNT; nP++)
	{
		if ( m_ppEFFECT[ nP ] ) 
		{
			m_ppEFFECT[ nP ]->InsertToScene();
		}
	}*/
}

///
/// 
///
void CObjFIXED::RemoveFromScene (bool bIncludeEFFECT)
{
	if ( !m_bIsVisible )
		return;

	m_bIsVisible = false;

	HNODE hNode = GetRootZNODE();
	::removeFromScene( hNode );

	//----------------------------------------------------------------------------------------------------	
	/// @brief CHILD��removeFromScene����
	//----------------------------------------------------------------------------------------------------
	//for (short nP=0; nP<m_pMODEL->m_nPartCNT; nP++) {
	//	if ( m_pHNODES[ nP ] != NULL )
	//		::removeFromScene( m_pHNODES[ nP ] );	// CObjGROUND::UnregisterFromNZIN
	//}

	/// ��ũ�� ����Ʈ�� RemoveFromScene�� ȣ������ �ʴ´�.
	/*if ( bIncludeEFFECT && m_ppEFFECT ) 
	{
		for (short nP=0; nP<m_pMODEL->m_nDummyPointCNT; nP++) 
		{
			if ( m_ppEFFECT[ nP ] ) 
			{
				m_ppEFFECT[ nP ]->RemoveFromScene();
			}
		}
	}*/
}

//-------------------------------------------------------------------------------------------------
void CObjFIXED::Scale (D3DVECTOR &Scale)
{
	if ( m_pMODEL->m_nRootPART >= 0 ) {
		::setScale( m_pHNODES[ m_pMODEL->m_nRootPART ] , Scale.x, Scale.y, Scale.z );
	}
}

//-------------------------------------------------------------------------------------------------
void CObjFIXED::Rotate (D3DXQUATERNION &Rotate)
{
	if ( m_pMODEL->m_nRootPART >= 0 ) {
		::setQuaternion( m_pHNODES[ m_pMODEL->m_nRootPART ] , Rotate.w, Rotate.x, Rotate.y, Rotate.z );
	} else {
		// ��ũ�� ���ϵ�� ���� �ʿ� ����.
		for (short nP=0; nP<m_pMODEL->m_nPartCNT; nP++) {
			::setQuaternion( m_pHNODES[ nP ], Rotate.w, Rotate.x, Rotate.y, Rotate.z );
		}
	}
}

//-------------------------------------------------------------------------------------------------
bool CObjFIXED::IsIntersect (float &fCurDistance)
{
	float fDistance = fCurDistance;
	bool  bCollision = false;

	for (short nP=0; nP<m_pMODEL->m_nPartCNT; nP++) 
	{
		if ( m_pHNODES[ nP ] ) 
		{
			if ( CGameOBJ::IsIntersect(m_pHNODES[ nP ], fDistance) )
			{
				bCollision = true;
				if( fDistance < fCurDistance )
				{
					fCurDistance = fDistance;
				}
			}
		}
	}

	return bCollision;
}

//-------------------------------------------------------------------------------------------------
bool CObjFIXED::IsIntersectForCamera(float &fCurDistance )
{
	float fDistance = fCurDistance;
	bool  bCollision = false;

	for (short nP=0; nP<m_pMODEL->m_nPartCNT; nP++) 
	{
		if ( m_pHNODES[ nP ] && !::getVisibleSeethruMode( m_pHNODES[ nP ] ) ) 
		{

			if ( CGameOBJ::IsIntersect(m_pHNODES[ nP ], fDistance) )
			{
				bCollision = true;
				if( fDistance < fCurDistance )
				{
					fCurDistance = fDistance;
				}
			}
		}
	}

	return bCollision;
}

bool CObjFIXED::IsInViewFrustum()
{
	for (short nP=0; nP<m_pMODEL->m_nPartCNT; nP++) 
	{
		if ( m_pHNODES[ nP ] && ::inViewfrustum( m_pHNODES[ nP ] ) ) 
		{			
			return true;
		}
	}

	return false;
}

//-------------------------------------------------------------------------------------------------
///
/// ..txt ���Ͽ��� �θ�, ���ϵ�. .. ��.. ���ĵ� ����Ʈ�� �����Ϳ� ���� ������ �԰��Ͽ�, ���� ��Ʈ�� ���Ѵ�.
///
short CObjFIXED::GetPartIndex (short nPartSeq)
{
	short nP, nSeq;

	if ( nPartSeq == 0 ) 
	{
		// parent part index
		for (nP=0; nP<m_pMODEL->m_nPartCNT; nP++) {
			if ( m_pMODEL->m_pParts[ nP ].m_nParent < 0 )
				return nP;
		}
	} 
	else
	{
		for (nP=0, nSeq=1; nP<m_pMODEL->m_nPartCNT; nP++) 
		{
			if ( m_pMODEL->m_pParts[ nP ].m_nParent < 0 )
				continue;
			
			if ( nPartSeq == nSeq++ )
				return nP;
		}
	}

	return nPartSeq;
}

//-------------------------------------------------------------------------------------------------
///
/// ..txt ������ ���� ������Ʈ�� �����.
/// Create �Լ��� ���� ȣ��.. �ܺ� ������ �ǳ�? 
///
bool CObjFIXED::CreatePart( int iCreateOrder, D3DVECTOR &Position, short nPartIdx )
{
	if ( m_pHNODES[ nPartIdx ] != NULL )
		return true;

	HNODE hParent=NULL;
	/// �θ� �ִٸ�..
	if ( m_pMODEL->m_pParts[ nPartIdx ].m_nParent >= 0 ) 
	{
		hParent = m_pHNODES[ m_pMODEL->m_pParts[ nPartIdx ].m_nParent ];
		if ( hParent == NULL ) 
		{
			/// �θ� �ִµ� �������� �������, ����.
			if ( this->CreatePart(iCreateOrder, Position, m_pMODEL->m_pParts[ nPartIdx ].m_nParent) )
				hParent = m_pHNODES[ m_pMODEL->m_pParts[ nPartIdx ].m_nParent ];
			else
				return false;
		}
	}

	/// �ش� ��Ʈ�� �ε��Ѵ�.
	char *pName = Make_ZNAME( iCreateOrder, nPartIdx);
	m_pHNODES[ nPartIdx ] = m_pMODEL->m_pParts[ nPartIdx ].LoadVisible( pName, Position, hParent);
	if ( NULL == m_pHNODES[ nPartIdx ] ) {
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
///
/// FIXED ������Ʈ�� �����Ѵ�.
/// @param pMODEl ������ �𵨵�����. �̰��� �̿��ؼ� FIXED ������Ʈ ����.
/// @bCreateEffect ����Ʈ�� �ִٸ� ���δ�. 
/// @bug ����Ʈ�� ��Ʈ�� ���ϰ� �ƴ϶�.. ���� �����Ϳ� ���̴°�.. ���� �Ȥ�����?
///
bool CObjFIXED::Create (CMODEL<CFixedPART> *pMODEL, D3DVECTOR &Position, bool bCreateEffect)
{
	if ( pMODEL == NULL )
		return false;
	if ( pMODEL->m_nPartCNT <= 0 ) 
		return false;


	m_bIsVisible = false;

	m_PosCUR  = Position;
	m_pMODEL  = pMODEL;
	m_pHNODES = new HNODE[ pMODEL->m_nPartCNT ];
		
	::ZeroMemory (m_pHNODES, sizeof(HNODE)*pMODEL->m_nPartCNT);
	for (short nI=0; nI<pMODEL->m_nPartCNT; nI++) 
	{
		/// �� ��Ʈ ����.
		if ( !this->CreatePart (this->m_dwCreateOrder++, Position, nI) ) 
		{
			this->Delete ();
			return false;
		}

		if( pMODEL->m_pParts[ nI ].m_nRangeSet )
		{
			::setVisibleRangeFromCamera( m_pHNODES[ nI ], LOD_APPEAR_MIN( pMODEL->m_pParts[ nI ].m_nRangeSet ), LOD_APPEAR_MAX( pMODEL->m_pParts[ nI ].m_nRangeSet ) );
		}
	}

	/// Effect �� �پ��ִٸ�..( ���� ���������Ͱ� �ְ�, �ٿ����Ѵٸ� )
	if ( bCreateEffect && pMODEL->m_nDummyPointCNT ) 
	{
		m_ppEFFECT = new CEffect*[ pMODEL->m_nDummyPointCNT ];
		for (short nI=0; nI<pMODEL->m_nDummyPointCNT; nI++) 
		{
			m_ppEFFECT[ nI ] = NULL;
			/// POINT_LIGHT_CONTAINER �� �ƽ����� ���� ������ �Ҷ� ����Ʈ�� �����ϱ� ���� ������ �̴�.
			if( pMODEL->m_pDummyPoints[ nI ].m_nEffectType < POINT_LIGHT_CONTAINER )
			{
				m_ppEFFECT[ nI ] = g_pEffectLIST->Add_EFFECT( pMODEL->m_pDummyPoints[ nI ].m_uiEftKEY );

				if ( m_ppEFFECT[ nI ] ) 
				{
					m_ppEFFECT[ nI ]->Transform( pMODEL->m_pDummyPoints[ nI ].m_Transform );
					m_ppEFFECT[ nI ]->Rotation( pMODEL->m_pDummyPoints[ nI ].m_Rotate );
					m_ppEFFECT[ nI ]->Scale( pMODEL->m_pDummyPoints[ nI ].m_Scale );
					m_ppEFFECT[ nI ]->LinkNODE( m_pHNODES[ pMODEL->m_pDummyPoints[ nI ].m_nParent ] );

					/// ��,���� �¿� �޴� ����Ʈ��� ��,�� �Ŵ����� ���
					if( pMODEL->m_pDummyPoints[ nI ].m_nEffectType == POINT_EFFECT_DAYNNIGHT )
					{
						g_DayNNightProc.PushEffect( m_ppEFFECT[ nI ] );
						m_ppEFFECT[ nI ]->SetNightEffect( true );
					}
				}
			}
		}
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
void CObjFIXED::Delete (void)
{
	/// Attached �� ����Ʈ ����
	if ( m_pMODEL ) {
		if ( m_ppEFFECT ) {
			for (short nI=0; nI<m_pMODEL->m_nDummyPointCNT; nI++) 
			{
				g_DayNNightProc.DeleteEffectFromList( m_ppEFFECT[ nI ] );
				SAFE_DELETE( m_ppEFFECT[ nI ] );				
			}
			SAFE_DELETE_ARRAY( m_ppEFFECT );
		}
	}

	if ( m_pHNODES ) {
		this->RemoveFromScene ();

		for (short nI=0; nI<m_pMODEL->m_nPartCNT; nI++) {
			if ( m_pHNODES[ nI ] ) {
				// delete created material with light map...
				if ( NULL != m_pLightMapMaterial )
					// delete immediately ? 
					g_MatFILE.Sub_DATAUseKEY( m_pLightMapMaterial[ nI ] );

				m_pMODEL->m_pParts[ nI ].UnloadVisible( m_pHNODES[ nI ] );
				m_pHNODES[ nI ] = NULL;
			}
		}

	}

	SAFE_DELETE_ARRAY( m_pHNODES );
	SAFE_DELETE_ARRAY( m_pLightMapMaterial );
}

//-------------------------------------------------------------------------------------------------
void CObjFIXED::SetLightMap (short nPartSEQ, char *szLightMapFile, int iXPos, int iYPos, int iWidth, int iHeight)
{
	/*
		����Ʈ���� ����Ǹ� ������ Material�� ��� unique�ؾ� �Ѵ�.
	*/
	short nPartIdx = this->GetPartIndex( nPartSEQ );
	if ( NULL == m_pHNODES[ nPartIdx ] )
		return;
	
	tagFileDATA< tagMATERIAL* > *pMAT;
	pMAT = g_MatFILE.KEY_Find_DATA( m_pMODEL->m_pParts[ nPartIdx ].m_uiMatKEY );
	if ( NULL == pMAT )
		return;

	if ( NULL == m_pLightMapMaterial ) {
		m_pLightMapMaterial = new t_HASHKEY [ m_pMODEL->m_nPartCNT ];
		::ZeroMemory (m_pLightMapMaterial, sizeof(t_HASHKEY)*m_pMODEL->m_nPartCNT );
	}

	char *szName = CStr::Printf ("@P%dx%d@%s", iXPos, iYPos, szLightMapFile);
	LogString (LOG_DEBUG, "SET LIGHT MAP map Obj: %d, Part: %d->%d, %s /  %s \n", m_nIndex, nPartSEQ, nPartIdx, szLightMapFile, szName);

	// �⺻ ��Ʈ������ ������ ���ο� ��Ʈ�̾��� �����Ѵ�.
	m_pLightMapMaterial[ nPartIdx ] = g_MatFILE.Add_MATERIAL (
					pMAT->m_DATA->m_RealFileName.Get(),	// char *szFileName, 
					0,									// short nIsSkin, 
					pMAT->m_DATA->m_nAlpha,				// short nIsAlpha, 
					pMAT->m_DATA->m_nTwoSide,			// short nIs2Side, 
					false,								// bool bTerrainMAT,
					szName,								// szLightMapMatName
					pMAT->m_DATA->m_nAlphaTest,					// short nAlphaTest,
					pMAT->m_DATA->m_nAlphaRef,					// short nAlphaRef,
					pMAT->m_DATA->m_nZTest,						// short nZTest,
					pMAT->m_DATA->m_nZWrite,					// short nZWrite,
					pMAT->m_DATA->m_nBlendType,					// short nBlendType,
					pMAT->m_DATA->m_nSpecular,				// short nSpecular
					pMAT->m_DATA->m_fAlphaValue,			// float fAlphaValue
					pMAT->m_DATA->m_nGlowType,				// short nGlowType
					pMAT->m_DATA->m_fGlowColor				// float fGlowColor[3]
					);					

	if ( m_pLightMapMaterial[ nPartIdx ] ) {
		HNODE hLightMapMAT = g_MatFILE.KEY_HNODE( m_pLightMapMaterial[ nPartIdx ] );
		if ( hLightMapMAT ) {
			::setMaterialLightmap( hLightMapMAT,
									szLightMapFile,
									iXPos,
									iYPos,
									iWidth,
									iHeight);
			::setVisibleMaterial( m_pHNODES[ nPartIdx ], 0, hLightMapMAT );
			::setReceiveShadow( m_pHNODES[ nPartIdx ], 1 ); 
		}
	}
}

//-------------------------------------------------------------------------------------------------
