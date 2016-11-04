/*
	$Header: /Client/CSkyDOME.cpp 23    04-10-11 6:15p Jeddli $
*/
#include "stdAFX.h"
#include "common\\IO_STB.h"
#include "CSkyDOME.h"

CSkyDOME* CSkyDOME::m_pInstance = NULL;

//-------------------------------------------------------------------------------------------------
CSkyDOME::CSkyDOME (HNODE hShader, HNODE hLight, short nSkyIDX) :
	m_nSkyIDX(nSkyIDX),
	m_hSKY(NULL),
	m_hMESH(NULL),
	m_hMAT(NULL)
{
	char *pZmsName, *pMatName, *pTexName1=NULL, *pTexName2=NULL;
	
	pZmsName = CStr::Printf ("__XXX_sky_me");
	if ( (CVFSManager::GetSingleton()).IsExistFile( SKY_MESH( nSkyIDX ) ) == false ) 
	{
		pZmsName = CStr::Printf ("[ %s ] file open error ...", SKY_MESH( nSkyIDX ) );
		g_pCApp->ErrorBOX( pZmsName, "ERROR" );
	}

	pMatName = CStr::Printf ("__XXX_sky_ma");
	if ( (CVFSManager::GetSingleton()).IsExistFile( SKY_TEXTURE( nSkyIDX, 0 ) ) ) 		
	{
		pTexName1 = SKY_TEXTURE( nSkyIDX, 0 );
	}
	else {
		pTexName1 = CStr::Printf ("[ %s ] file open error ...", SKY_TEXTURE( nSkyIDX, 0 ) );
		g_pCApp->ErrorBOX( pTexName1, "ERROR", MB_OK);
		return;
	}

	if ( (CVFSManager::GetSingleton()).IsExistFile( SKY_TEXTURE( nSkyIDX, 1 ) )  ) 		
	{
		pTexName2 = SKY_TEXTURE( nSkyIDX, 1 );
	}

	m_hMESH = ::loadMesh ( pZmsName, SKY_MESH( nSkyIDX ) );
	if ( m_hMESH )
	{
		m_hMAT = ::loadSkyMaterial( pMatName, hShader, pTexName1, pTexName2 );
	}
	
	::setSkyMaterialBlendRatio( m_hMAT, 1.0f );

	// --- ù��° �ϴ� �޽��� ���� ����. (���� �հŸ��� �ִ� �ͺ��� ����)
	// --- �ϴ� ������Ʈ ����(loadVisible() �� ���� ����)
	::setMaterialUseAlpha( m_hMAT, 0 );
	
	m_hSKY = ::loadSky( "__XXX_sky", m_hMESH, m_hMAT, hLight );
	::setScale( m_hSKY, 1.0, 1.0, 1.0 );

	// --- �ι�° �ϴ� �޽��� ���� ����
	::setMaterialUseAlpha( m_hMAT, 1 );
	// --- �޽��� ���� ���� �� �ϴ� ������Ʈ�� ��Ͻ�Ŵ
	::addRenderUnit( m_hSKY, m_hMESH, m_hMAT, hLight );

	::setMaterialUseAlphaTest( m_hMAT, 0);

	// --- �ϴ� ������Ʈ�� ������ ������ ���� ���ƾ� �ϹǷ�, �Ʒ��� ���� ����
	::setReceiveFog( m_hSKY, 0 );

	// --- �� �������ֵ��� ȸ�� �ӵ� ����
	// --- ȸ�� ���� ������ z ��.
	// --- �ι�° ����: �������� ��ȣ(0���� ����)
	// --- ����° ����: ȸ�� ���� �� �ӵ�. ���� ����. �ʴ� ȸ�� ������ ����
	/*
	::setSkyRotationSpeed( m_hSKY, 0, 2 );
	::setSkyRotationSpeed( m_hSKY, 1, 1 );
	::setSkyRotationSpeed( m_hSKY, 2, 0.5 );
	*/
}

//-------------------------------------------------------------------------------------------------
CSkyDOME::~CSkyDOME ()
{
	if ( m_hMAT )	::unloadMaterial( m_hMAT );
	if ( m_hMESH )	::unloadMesh    ( m_hMESH );

	if ( m_hSKY ) ::unloadSky( m_hSKY );
}

//-------------------------------------------------------------------------------------------------
bool CSkyDOME::Init (HNODE hShader, HNODE hLight, short nSkyIDX)
{
	if ( m_pInstance ) {
		if ( m_pInstance->GetSkyIDX() == nSkyIDX )
			return true;

		SAFE_DELETE( m_pInstance );
	}

	m_pInstance = new CSkyDOME( hShader, hLight, nSkyIDX );
	return ( m_pInstance != NULL );
}

//-------------------------------------------------------------------------------------------------
