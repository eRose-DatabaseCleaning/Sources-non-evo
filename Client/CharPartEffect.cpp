#include "stdafx.h"
#include ".\charparteffect.h"

CCharPartEffect::CCharPartEffect(void)
{
}

CCharPartEffect::~CCharPartEffect(void)
{
}

bool CCharPartEffect::CreatePartEffect( int iPartIndex )
{
	//if( iPartIndex < 0 || iPartIndex >= MAX_BODY_PART )
	//	return false;

	//CMODEL<CCharPART> *pCharPART = m_pBodyPART[ nPartIdx ];
	//if( pCharPART == NULL )
	//	return false;

	///// �̹� � ����Ʈ�� �پ� �ִٸ� ������
	//if( m_ppEFFECTp iPartIndex ] )
	//	DeletePartEffect( iPartIndex );


	///// Effect �� �پ��ִٸ�..( ���� ���������Ͱ� �ְ�, �ٿ����Ѵٸ� )
	//if ( pCharPART->m_nDummyPointCNT ) 
	//{
	//	m_ppEFFECT[ iPartIndex ] = new CEffect*[ pCharPART->m_nDummyPointCNT ];
	//	for (nI=0; nI<pCharPART->m_nDummyPointCNT; nI++) 
	//	{
	//		m_ppEFFECT[ nI ] = NULL;
	//		/// POINT_LIGHT_CONTAINER �� �ƽ����� ���� ������ �Ҷ� ����Ʈ�� �����ϱ� ���� ������ �̴�.
	//		if( pCharPART->m_pDummyPoints[ nI ].m_nEffectType < POINT_LIGHT_CONTAINER )
	//		{
	//			m_ppEFFECT[ nI ] = g_pEffectLIST->Add_EFFECT( pCharPART->m_pDummyPoints[ nI ].m_uiEftKEY );

	//			if ( m_ppEFFECT[ nI ] ) 
	//			{
	//				m_ppEFFECT[ nI ]->Transform( pCharPART->m_pDummyPoints[ nI ].m_Transform );
	//				m_ppEFFECT[ nI ]->Rotation( pCharPART->m_pDummyPoints[ nI ].m_Rotate );
	//				m_ppEFFECT[ nI ]->Scale( pCharPART->m_pDummyPoints[ nI ].m_Scale );
	//				m_ppEFFECT[ nI ]->LinkNODE( m_pHNODES[ pCharPART->m_pDummyPoints[ nI ].m_nParent ] );

	//				/// ��,���� �¿� �޴� ����Ʈ��� ��,�� �Ŵ����� ���
	//				if( pCharPART->m_pDummyPoints[ nI ].m_nEffectType == POINT_EFFECT_DAYNNIGHT )
	//				{
	//					g_DayNNightProc.PushEffect( m_ppEFFECT[ nI ] );
	//					m_ppEFFECT[ nI ]->SetNightEffect( true );
	//				}
	//			}
	//		}
	//	}
	//}
	return true;
}

void CCharPartEffect::DeletePartEffect( int iPartIndex )
{
	/*CMODEL<CCharPART> *pCharPART = m_pBodyPART[ nPartIdx ];
	if( pCharPART == NULL )
		return false;

	if( m_ppEFFECT[ iPartIndex ] )
	{		
		for (short nI=0; nI<pCharPART->m_nDummyPointCNT; nI++) 
		{
			g_DayNNightProc.DeleteEffectFromList( m_ppEFFECT[ iPartIndex ][ nI ] );
			SAFE_DELETE( m_ppEFFECT[ iPartIndex ][ nI ] );				
		}
		SAFE_DELETE_ARRAY( m_ppEFFECT[ iPartIndex ] );		
	}*/
}

void CCharPartEffect::DeleteAllPartEffect()
{
	/*for( int i = 0; i < MAX_BODY_PART ;i++ )
	{
		DeletePartEffect( i );
	}*/	
}
