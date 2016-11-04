#include "stdafx.h"
#include "CDigitEffect.h"
#include "..\\Game.h"

/*
const char* _digit_Texture[ MAX_DIGIT_EFFECT ] = { "3DData\\Effect\\Special\\DigitWorkTex01.tga",
													"3DData\\Effect\\Special\\DigitWorkTex02.tga",
													"3DData\\Effect\\Special\\DigitWorkTex03.tga",
													"3DData\\Effect\\Special\\DigitWorkTex04.tga",
													"3DData\\Effect\\Special\\DigitWorkTex05.tga",
													"3DData\\Effect\\Special\\DigitWorkTex06.tga",
													"3DData\\Effect\\Special\\DigitWorkTex07.tga",
													"3DData\\Effect\\Special\\DigitWorkTex08.tga",
													"3DData\\Effect\\Special\\DigitWorkTex09.tga",
													"3DData\\Effect\\Special\\DigitWorkTex10.tga", 
												};

*/

const char* _digit_Mesh[ MAX_DIGIT_EFFECT ] = { "3DData\\Effect\\Special\\DigitWork01.zms",
													"3DData\\Effect\\Special\\DigitWork01.zms",
													"3DData\\Effect\\Special\\DigitWork01.zms",
													"3DData\\Effect\\Special\\DigitWork01.zms",
													"3DData\\Effect\\Special\\DigitWork01.zms",
													"3DData\\Effect\\Special\\DigitWork01.zms",
													"3DData\\Effect\\Special\\DigitWork01.zms",
													"3DData\\Effect\\Special\\DigitWork01.zms",
													"3DData\\Effect\\Special\\DigitWork01.zms",
													"3DData\\Effect\\Special\\DigitWork01.zms",
												};



#define DIGIT_WIDTH			32
#define DIGIT_HEIGHT		32
#define DIGIT_COUNT			4


CDigitEffect::CDigitEffect()
{
	int i = 0;
	for( i = 0; i < MAX_DIGIT_EFFECT ; i++ )
	{
		m_DigitNode[ i ].m_hAnimatable = NULL;
		m_DigitNode[ i ].m_hMesh = NULL;
		m_DigitNode[ i ].m_hMat = NULL;		
	}

	m_hMotion = NULL;

	m_DigitTex [ DIGIT_NORMAL ] = NULL;
	m_DigitTex [ DIGIT_ATTACKED ] = NULL;

	m_DigitClearTex = NULL;
	m_DigitMissTex = NULL;
}


CDigitEffect::~CDigitEffect()
{
	/// ���� �ı��Ŀ� ȣ��Ǳ⶧����..
	//ClearAll();
}

/// �۾� ��ü�� �ʱ�ȭ..
bool CDigitEffect::Init()
{
	ClearAll();

	int i = 0;

	HNODE hDummy = NULL;	
	HNODE hAnimatable = NULL;	
	HNODE hMesh = NULL;
	HNODE hMat = NULL;

	int ZZ_INTERP_SQUAD = 3, ZZ_INTERP_CATMULLROM = 4;
	m_hMotion = ::loadMotion( "DigitEffectMotion", "3DData\\Effect\\Special\\hit_figure_01.ZMO", 1, ZZ_INTERP_CATMULLROM, ZZ_INTERP_SQUAD, 1, 1 );
	if( m_hMotion == NULL )
	{
		ClearAll();
		return false;
	}

	for( i = 0; i < MAX_DIGIT_EFFECT ; i++ )
	{		
		hMat = ::loadNullColormapMaterial( CStr::Printf ("DigitEffect_%d", i ), g_GameDATA.m_hShader_nolit, DIGIT_WIDTH * DIGIT_COUNT, DIGIT_HEIGHT);
		//hMat = ::loadColormapMaterial( CStr::Printf ("DigitEffect_%d", i ), g_GameDATA.m_hShader_nolit, "3DData\\Effect\\Special\\DigitNumber01.tga" );
		if( hMat == NULL )
		{
			ClearAll();
			return false;
		}

		hMesh = ::loadMesh ( CStr::Printf ("DigitMesh_%d", i ), "3DData\\Effect\\Special\\DigitWork01.zms" );
		if( hMesh == NULL )
		{
			ClearAll();
			return false;
		}		
		
		hAnimatable = ::loadAnimatable( CStr::Printf ("DigitNode_%d", i ), hMesh, m_hMotion, hMat, g_GameDATA.m_hShader_nolit );
		if( hAnimatable == NULL )
		{
			ClearAll();
			return false;
		}

		hDummy = ::loadVisible( CStr::Printf ("DigitDummy_%d", i ), 0, 0, 0 );
		if( hDummy == NULL )
		{
			ClearAll();
			return false;
		}

		m_DigitNode[ i ].m_hPositionDummy = hDummy;		
		m_DigitNode[ i ].m_hAnimatable = hAnimatable;		
		m_DigitNode[ i ].m_hMesh = hMesh;
		m_DigitNode[ i ].m_hMat = hMat;

		/// Material attribute setting

		::setMaterialBlendTypeCustom( hMat, D3DBLEND_SRCALPHA,
											D3DBLEND_INVSRCALPHA,
											D3DBLENDOP_ADD
											);


		::setMaterialUseAlpha( hMat, 1 );
		::setMaterialUseLight( hMat, 0 ); // ������ ���� ����.
		
		//::setMaterialUseAlphaTest( hMat, 1 );
		//::setMaterialZWrite( hMat, 1 );
		//::setMaterialZTest( hMat, 1 );


		/// 
		::setBillboard( m_DigitNode[ i ].m_hAnimatable, 1 );
		::controlAnimatable( m_DigitNode[ i ].m_hAnimatable, 0 );

		::setRepeatCount( m_DigitNode[ i ].m_hAnimatable, 1 );

		::linkNode( hDummy, hAnimatable ); 

		
		//::insertToScene( hDummy );
		//----------------------------------------------------------------------------------------------------	
		/// @brief CHILD��InsertToScene����
		//----------------------------------------------------------------------------------------------------
		//::insertToScene( hAnimatable );
	}

		
	/// Load digit number
	m_DigitTex[ DIGIT_NORMAL ] = loadTextureWithPool ( "3DData\\Effect\\Special\\DigitNumber01.dds", 
														"3DData\\Effect\\Special\\DigitNumber01.dds", 
														1, 
														0,
														2 );
	if( m_DigitTex[ DIGIT_NORMAL ] == NULL )
	{
			g_pCApp->ErrorBOX( "Chatbox load failed !!", "File open error" );
			//������ ������ �����ش�..
			ClearAll();
			return false;
	}

	/// Load digit number
	m_DigitTex[ DIGIT_ATTACKED ] = loadTextureWithPool ( "3DData\\Effect\\Special\\DigitNumber02.dds", 
														"3DData\\Effect\\Special\\DigitNumber02.dds", 
														1, 
														0,
														2 );
	if( m_DigitTex[ DIGIT_ATTACKED ] == NULL )
	{
		g_pCApp->ErrorBOX( "Chatbox load failed !!", "File open error" );
		//������ ������ �����ش�..
		ClearAll();
		return false;
	}




	/// Load digit clear number
	m_DigitClearTex = loadTextureWithPool ( "3DData\\Effect\\Special\\DigitWorkClear.dds", 
										"3DData\\Effect\\Special\\DigitWorkClear.dds", 
										1, 
										0,
										2 );
	if( m_DigitClearTex == NULL )
	{
			g_pCApp->ErrorBOX( "m_DigitClearTex load failed !!", "File open error" );
			//������ ������ �����ش�..
			ClearAll();
			return false;
	}

	
	/// Load digit Miss number
	m_DigitMissTex = loadTextureWithPool ( "3DData\\Effect\\Special\\DigitNumberMiss.dds", 
											"3DData\\Effect\\Special\\DigitNumberMiss.dds", 
											1, 
											0,
											2 );
	if( m_DigitMissTex == NULL )
	{
		g_pCApp->ErrorBOX( "m_DigitMissTex load failed !!", "File open error" );
		//������ ������ �����ش�..
		ClearAll();
		return false;
	}

	return true;
}

void CDigitEffect::ClearAll()
{
	HNODE hVisible = NULL;
	HNODE hMesh = NULL;
	HNODE hMat = NULL;

	for( int i = 0; i < MAX_DIGIT_EFFECT ; i++ )
	{		
		if( m_DigitNode[ i ].m_hPositionDummy != NULL )
		{
			::removeFromScene( m_DigitNode[ i ].m_hPositionDummy );
			::unloadVisible( m_DigitNode[ i ].m_hPositionDummy );
			m_DigitNode[ i ].m_hPositionDummy = NULL;
		}
		
		if( m_DigitNode[ i ].m_hAnimatable != NULL )
		{
			//----------------------------------------------------------------------------------------------------	
			/// @brief CHILD��RemoveToScene����
			//----------------------------------------------------------------------------------------------------
			//::removeFromScene( m_DigitNode[ i ].m_hAnimatable );
			::unloadAnimatable( m_DigitNode[ i ].m_hAnimatable );
			m_DigitNode[ i ].m_hAnimatable = NULL;
		}

		if( m_DigitNode[ i ].m_hMesh != NULL )
		{
			::unloadMesh( m_DigitNode[ i ].m_hMesh );
			m_DigitNode[ i ].m_hMesh = NULL;
		}

		if( m_DigitNode[ i ].m_hMat != NULL )
		{
			::unloadMaterial( m_DigitNode[ i ].m_hMat );		
			m_DigitNode[ i ].m_hMat = NULL;
		}

	}

	if( m_hMotion != NULL )
	{
		::unloadMotion( m_hMotion );
		m_hMotion = NULL;
	}
	
	
	if( m_DigitTex[ DIGIT_NORMAL ] != NULL )
	{
		::unloadTexture( m_DigitTex[ DIGIT_NORMAL ] );
		m_DigitTex[ DIGIT_NORMAL ] = NULL;
	}
	if( m_DigitTex[ DIGIT_ATTACKED ] != NULL )
	{
		::unloadTexture( m_DigitTex[ DIGIT_ATTACKED ] );
		m_DigitTex[ DIGIT_ATTACKED ] = NULL;
	}

	if( m_DigitClearTex != NULL )
	{
		::unloadTexture( m_DigitClearTex );
		m_DigitClearTex = NULL;
	}

	if( m_DigitMissTex != NULL )
	{
		::unloadTexture( m_DigitMissTex );
		m_DigitMissTex = NULL;
	}
}

/// Animation �� ��������� �Ⱥ��̰� �Ѵ�.
void CDigitEffect::Refresh()
{
	for( int i = 0; i < MAX_DIGIT_EFFECT ; i++ )
	{
		if( ( m_DigitNode[ i ].m_hAnimatable != NULL ) && m_DigitNode[ i ].m_bVisible )
		{
			short int iState = ::getAnimatableState( m_DigitNode[ i ].m_hAnimatable );

			if( iState == 0 )
			{
				//----------------------------------------------------------------------------------------------------	
				/// @brief Visibility �� �����ϴ°� �ƴ϶� �ƿ� Scene���� ����.
				//----------------------------------------------------------------------------------------------------
				::removeFromScene( m_DigitNode[ i ].m_hPositionDummy );
				m_DigitNode[ i ].m_bVisible = false;
				/*::setVisibility( m_DigitNode[ i ].m_hAnimatable, 0 );
				::setVisibility( m_DigitNode[ i ].m_hPositionDummy, 0 );*/
			}
		}
	}
}

/// ���� ��������� ���� ��带 ���Ѵ�.
int CDigitEffect::GetEmptyNode()
{
	for( int i = 0; i < MAX_DIGIT_EFFECT ; i++ )
	{
		/*if( m_DigitNode[ i ].m_hMesh == NULL )
		{
			return i;
		}*/

		if( ( m_DigitNode[ i ].m_bVisible == false ) && ( m_DigitNode[ i ].m_hAnimatable != NULL ) )
		{
			short int iState = ::getAnimatableState( m_DigitNode[ i ].m_hAnimatable );

			if( iState == 0 )
			{
				return i;
			}
		}
	}

	return INVALID_EFFECTNODE;
}

/// �ִ� 4�ڸ� Ÿ��ġ�� ǥ���Ѵ�.. ( 32 * 32 * 4 )
void CDigitEffect::CreateDigitEffect(int iPoint,float x,float y,float z, bool bIsUSER )
{
	int iEmptyNode = GetEmptyNode();

	if( iEmptyNode == INVALID_EFFECTNODE )
		return;



	/// �� �ڸ����� ���Ѵ�..
	int iDigit[ 4 ];
	iPoint = iPoint % 10000;
	iDigit[ 0 ] = iPoint / 1000;
	iDigit[ 1 ] = ( iPoint % 1000 ) / 100;
	iDigit[ 2 ] = ( iPoint % 100 ) / 10;
	iDigit[ 3 ] = ( iPoint % 10 );

	/// ��� ���ڸ� ǥ���ؾ��ϴ°�?
	int iDigitCount = 4;
	for( int i = 0; i < 4 ; i++ )
	{
		if( iDigit[ i ] == 0 )
			iDigitCount--;
		else
			break;
	}	
	
	/// ����� ���ڰ� ����..
	if( iDigitCount == 0 )
	{
		LPD3DTEXTURE		workTexture = (LPD3DTEXTURE) ::getTexture( m_DigitNode[ iEmptyNode ].m_hMat, 0 );
		LPD3DTEXTURE		MissTexture = (LPD3DTEXTURE) ::getTexturePointer( m_DigitMissTex );

		LPDIRECT3DSURFACE9 psurfWork = NULL;		
		LPDIRECT3DSURFACE9 psurfMiss = NULL;

		if (!workTexture || !MissTexture ) return; // �ؽ��İ� ���� �ε����� ���� ���¶�� �ǳʶ�.

		HRESULT hr = S_OK;

		/// �۾� �ؽ���..
		hr = workTexture->GetSurfaceLevel(0, &psurfWork);
		if( FAILED( hr ) )
			return;		

		/// Miss �ؽ���..
		hr = MissTexture->GetSurfaceLevel(0, &psurfMiss);
		if( FAILED( hr ) )
			return;

		RECT SourceRect = { 0, 0, DIGIT_WIDTH*DIGIT_COUNT, DIGIT_HEIGHT };
		POINT DestinationPoint = { 0, 0 };

		LPD3DDEVICE pDevice = (LPD3DDEVICE)::getDevice();
		hr = pDevice->UpdateSurface(psurfMiss,
									&SourceRect,
									psurfWork,
									&DestinationPoint );

		if( FAILED( hr ) )
			return;


		::setPosition( m_DigitNode[ iEmptyNode ].m_hPositionDummy, x, y, z );
		::controlAnimatable( m_DigitNode[ iEmptyNode ].m_hAnimatable, 1 );

		//----------------------------------------------------------------------------------------------------	
		/// @brief Visibility �� �����ϴ°� �ƴ϶� �ƿ� Scene���� �ְ� ����...
		//----------------------------------------------------------------------------------------------------
		::insertToScene( m_DigitNode[ iEmptyNode ].m_hPositionDummy );
		m_DigitNode[ iEmptyNode ].m_bVisible = true;
		///::setVisibility( m_DigitNode[ iEmptyNode ].m_hAnimatable, 1 );

		SAFE_RELEASE(psurfWork);		
		SAFE_RELEASE(psurfMiss);		
	}
	else
	{

		if( m_DigitNode[ iEmptyNode ].m_hMat == NULL )
			return;

		LPD3DTEXTURE		workTexture = (LPD3DTEXTURE) ::getTexture( m_DigitNode[ iEmptyNode ].m_hMat, 0 );

		/// ���� ���� ����������, �ƹ�Ÿ�� ���� �����Ŷ��� Ʋ���� ����.
		LPD3DTEXTURE		DigitTexture = NULL;
		if( bIsUSER )
			DigitTexture = (LPD3DTEXTURE) ::getTexturePointer( m_DigitTex[ DIGIT_ATTACKED ] );
		else
			DigitTexture = (LPD3DTEXTURE) ::getTexturePointer( m_DigitTex[ DIGIT_NORMAL ] );

		LPD3DTEXTURE		ClearTexture = (LPD3DTEXTURE) ::getTexturePointer( m_DigitClearTex );

		LPDIRECT3DSURFACE9 psurfWork = NULL;
		LPDIRECT3DSURFACE9 psurfDigit = NULL;
		LPDIRECT3DSURFACE9 psurfClear = NULL;
		
		if (!workTexture || !DigitTexture || !ClearTexture) return; // �ؽ��İ� ���� �ε����� ���� ���¶�� �ǳʶ�.

		HRESULT hr = S_OK;

		/// �۾� �ؽ���..
		hr = workTexture->GetSurfaceLevel(0, &psurfWork);
		if( FAILED( hr ) )
			return;

		/// ���� �ؽ���..
		hr = DigitTexture->GetSurfaceLevel(0, &psurfDigit);
		if( FAILED( hr ) )
			return;

		/// Clear �ؽ���..
		hr = ClearTexture->GetSurfaceLevel(0, &psurfClear);
		if( FAILED( hr ) )
			return;
		

		RECT SourceRect = { 0, 0, DIGIT_WIDTH*DIGIT_COUNT, DIGIT_HEIGHT };
		POINT DestinationPoint = { 0, 0 };


		LPD3DDEVICE pDevice = (LPD3DDEVICE)::getDevice();
		hr = pDevice->UpdateSurface(psurfClear,
									&SourceRect,
									psurfWork,
									&DestinationPoint );	

		/// ��Ÿ�� ��ġ
		int iStartPos = ( ( DIGIT_COUNT - iDigitCount ) * DIGIT_WIDTH ) / 2;

		for( int i = 0; i < iDigitCount ; i++ )
		{	
			SetRect( &SourceRect, (DIGIT_WIDTH * iDigit[ DIGIT_COUNT - iDigitCount + i ] ), 0, (DIGIT_WIDTH * iDigit[DIGIT_COUNT - iDigitCount + i]) + DIGIT_WIDTH, DIGIT_HEIGHT ); 
			DestinationPoint.x = iStartPos + i*DIGIT_WIDTH;
			DestinationPoint.y = 0;
			
			hr = pDevice->UpdateSurface(psurfDigit,
										&SourceRect,
										psurfWork,
										&DestinationPoint );
			

			if( FAILED( hr ) )
			{
				return;
			}
		}
		
		::setPosition( m_DigitNode[ iEmptyNode ].m_hPositionDummy, x, y, z );
		::controlAnimatable( m_DigitNode[ iEmptyNode ].m_hAnimatable, 1 );

		//----------------------------------------------------------------------------------------------------	
		/// @brief Visibility �� �����ϴ°� �ƴ϶� �ƿ� Scene���� �ְ� ����...
		//----------------------------------------------------------------------------------------------------
		::insertToScene( m_DigitNode[ iEmptyNode ].m_hPositionDummy );
		m_DigitNode[ iEmptyNode ].m_bVisible = true;
		///::setVisibility( m_DigitNode[ iEmptyNode ].m_hAnimatable, 1 );

		SAFE_RELEASE(psurfWork);
		SAFE_RELEASE(psurfDigit);
		SAFE_RELEASE(psurfClear);
	}
}



