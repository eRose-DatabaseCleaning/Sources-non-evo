/*
	$Header: /Client/CCamera.cpp 37    04-10-11 8:21p Navy $
*/
#include "stdAFX.h"
#include "CCamera.h"
#include "Game.h"
#include "IO_Terrain.h"


#define	MIN_DISTANCE	300.f
#ifndef _DEBUG
#define	MAX_DISTANCE	( 1800.f * ((g_GameDATA.m_bFilmingMode)? 2.0f : 1.0f) )
#else
#define	MAX_DISTANCE	( 230000 )
#endif


CCamera* CCamera::m_pInstance = NULL;


CCamera::CCamera ()
{
	m_hNODE		= NULL;
	m_hMotion	= NULL;
}
CCamera::~CCamera ()
{
	;
}

CCamera *CCamera::Instance ()
{
	if ( !m_pInstance )
		m_pInstance = new CCamera;

	return m_pInstance;
}

///*------------------------------------------------------------
/// 2004 / 3/ 31 :���� nAvy
/// 1. unloadCamera�ÿ� Error�߻�
/// =>Instance�� ����� ��� �������� ������ ���� ���� �ȴ�.
/// =>Instance������ �� ������ �̵��ϰų� Destructor���� ��������� ������.
void CCamera::Destroy ()
{
	if( m_hNODE != NULL )
	{
		::unloadCamera( m_hNODE );
		m_hNODE = NULL;
	}

	if( m_hMotion != NULL )
	{
		::unloadMotion( m_hMotion );
		m_hMotion = NULL;
	}

	SAFE_DELETE( m_pInstance );
}

//-------------------------------------------------------------------------------------------------
void CCamera::Init (HNODE hNODE)
{
	m_hNODE = hNODE;

	setCameraDefault( hNODE );

	m_Angle.x = 25.f;
	m_Angle.y = 180.f;
	m_Angle.z = 0.f;

	m_fDistance = 1000.f;

	// -- ī�޶� �ʱⰪ ����
	m_fYAW   = 0;			// [-180, 180]
	m_fPITCH = 0.5;			// [0, 1]
	m_bFollowMode = false;	// 1 : �ڿ��� ���󰡴� ���, 0 : 3��Ī ���

	if( m_hMotion != NULL )
	{
		::unloadMotion( m_hMotion );
		m_hMotion = NULL;
	}
}



void CCamera::SetSightInfo( int iInfoIdx )
{

}

void CCamera::ResetToAvatarCamera()
{
	if( m_hNODE != NULL )
	{
		HNODE hMotion = findNode("SelectAvatarCameraMotion");
		if( hMotion )
		{
			attachMotion(m_hNODE, 0 );
			unloadMotion ( hMotion );
		}

		::unloadCamera( m_hNODE );
		m_hNODE = NULL;
	}

	m_hNODE = ::findNode( "avatar_camera" );

	if( m_hNODE == NULL )
	{
		LogString( LOG_DEBUG, "Load camera failed[ cameras/camera01.zca ]"  );
		return;
	}

	setCameraDefault( m_hNODE );

	::setPosition( m_hNODE, m_PosAT.x, m_PosAT.y, m_PosAT.z );
}

/// set motion to camera

void CCamera::SetMotion( const char* strMotion )
{
	if( m_hNODE == NULL )
		return;

	if( m_hMotion != NULL )
	{
		::unloadMotion( m_hMotion );
		m_hMotion = NULL;
	}

	m_hMotion = ::findNode( "CameraMotion" );
	if( m_hMotion != NULL )
	{
		::unloadMotion( m_hMotion );
		m_hMotion = NULL;
	}


	int ZZ_INTERP_SQUAD = 3, ZZ_INTERP_CATMULLROM = 4;
	m_hMotion = ::loadMotion( "CameraMotion", strMotion, 1, ZZ_INTERP_CATMULLROM, ZZ_INTERP_SQUAD, 1, 1 );

	if( m_hMotion == NULL )
	{
		LogString( LOG_DEBUG, "Camera motion loading failed[ %s ]", strMotion );
		return;
	}

	::attachMotion( m_hNODE, m_hMotion );

	::controlAnimatable( m_hNODE, 1 );
}




bool CCamera::Toggle_FloowMODE ()
{
	m_bFollowMode = !m_bFollowMode;

	::setCameraFollowMode ( m_hNODE, m_bFollowMode );

	return m_bFollowMode;
}

void CCamera::Add_PITCH (short nMovement)
{
	m_fPITCH += ((float)nMovement / g_pCApp->GetHEIGHT() );

    if (m_fPITCH < 0.2) m_fPITCH = 0.2f;
	else
    if (m_fPITCH > 1.0) m_fPITCH = 1.0f;

    setCameraFollowPitch ( m_hNODE, m_fPITCH );
}

void CCamera::Add_YAW (short nMovement)
{
	const float fMaxAngle = 180.f;

	// -- ���� ����� �� ������(setCameraFollowYaw()�� ���� ������ ����� ���� �ٸ� �� ����)
	m_fYAW = getCameraFollowYaw(m_hNODE);

	// -- ���콺�� x�� ������ ȸ���� ����(�������� ������Ŵ)
	m_fYAW = m_fYAW - 480.f * nMovement / g_pCApp->GetWIDTH();

	// -- �ִ밢�� �ּҰ����� ����
/*
	if ( m_fYAW < -fMaxAngle ) m_fYAW = -fMaxAngle;
	else
	if ( m_fYAW >  fMaxAngle ) m_fYAW = fMaxAngle;
*/
	// -- ī�޶� ȸ���� ���� (���� ������ -180 ������ 180 ������)
	setCameraFollowYaw ( m_hNODE, m_fYAW );
}

void CCamera::Add_Distance (float fDistance)
{	
	m_fDistance += fDistance;
	if ( m_fDistance < MIN_DISTANCE )
		m_fDistance = MIN_DISTANCE;
	else
	if ( m_fDistance > MAX_DISTANCE )
		m_fDistance = MAX_DISTANCE;    

	::setCameraFollowDistance( m_hNODE, m_fDistance );
}


void CCamera::Attach (HNODE hModel)
{	
	::cameraAttachTarget   (m_hNODE, hModel);
	::setCameraTargetHeight( m_hNODE, 170 );
	::setCameraFollowDistance ( m_hNODE, m_fDistance );
	::setCameraFollowYaw      ( m_hNODE, m_fYAW );
	::setCameraFollowPitch    ( m_hNODE, m_fPITCH );
	::setCameraFollowMode     ( m_hNODE, m_bFollowMode );

	::setCameraFollowDistanceRange( m_hNODE, MIN_DISTANCE, MAX_DISTANCE * 2.0f );
}

//#ifdef	_DEBUG
D3DVECTOR CCamera::Get_Position (void)
{		
	::getCameraEye( m_hNODE, (float*)&m_PosAT );

	return m_PosAT;
}
//#endif
//-------------------------------------------------------------------------------------------------
void CCamera::Set_Position (float fX, float fY, float fZ)
{
	m_PosAT.x=fX;
	m_PosAT.y=fY;
	m_PosAT.z=fZ;

	::setPosition( m_hNODE, fX, fY, fZ);
	//::setPosition( m_hNODE, 0.0f, 0.0f, 0.0f );
}


void CCamera::LookAt( float eyeX, float eyeY, float eyeZ,
						float centerX, float centerY, float centerZ,
						float upX, float upY, float upZ )
{
	::lookAt( m_hNODE, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ );
}

