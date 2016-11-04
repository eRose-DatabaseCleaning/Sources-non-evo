#include "StdAfx.h"
#include "CharacterDLG.h"
#include "../it_mgr.h"
#include "../CTDrawImpl.h"
#include "../CToolTipMgr.h"
#include "../CDragItem.h"
#include "../IO_ImageRes.h"
#include "../TypeResource.h"

#include "../icon/ciconitem.h"

#include "../../System/CGame.h"
#include "../../OBJECT.h"
#include "../../Network/CNetWork.h"
#include "../../gamecommon/item.h"
#include "../../GameData/Event/CTEventItem.h"
#include "../../GameData/CClan.h"

#include "TabbedPane.h"
#include "TGuage.h"
#include "JContainer.h"
#include "ResourceMgr.h"

CCharacterDLG::CCharacterDLG( int iType )
{
	m_iGuageBlueGID = 0;
	m_iGuageYellowGID = 0;
	m_iGuageRedGID = 0;

	m_iTab = BASIC_INFO;
	SetDialogType( iType );
}

CCharacterDLG::~CCharacterDLG()
{
}

void CCharacterDLG::Draw()
{
	if(!IsVision()) return ;

	CTDialog::Draw();

	switch( m_iTab )
	{
	case BASIC_INFO:
		DrawBasicInfo();
		break;
	case ABILITY_INFO:
		DrawAbilityInfo();
		break;
	case UNION_INFO:
		DrawUnionInfo();
		break;
	default:
		break;
	}
}




unsigned int  CCharacterDLG::Process(UINT uiMsg,WPARAM wParam,LPARAM lParam)
{
	if( g_pAVATAR == NULL )
		return 0;


	if(!IsVision()) return 0;

	unsigned iProcID = 0;
	if( iProcID =  CTDialog::Process(uiMsg,wParam,lParam)) 
	{
		switch(uiMsg)
		{
		case WM_LBUTTONDOWN:
			On_LButtonDN( iProcID, wParam, lParam );
			break;
		case WM_LBUTTONUP:
			On_LButtonUP( iProcID, wParam, lParam );
			break;
		default:
			break;
		}
		return uiMsg;
	}
	return 0;
}


bool CCharacterDLG::On_LButtonUP( unsigned iProcID, WPARAM wParam, LPARAM lParam )
{
	switch( iProcID )
	{
	case IID_BTN_DIALOG2ICON:
		g_itMGR.AddDialogIcon( 	GetDialogType() );
		break;
	case IID_BTN_CLOSE:
		Hide();
		break;
	//case CHAR_BUTTON_PAT:
	//	g_itMGR.OpenDialog( DLG_TYPE_PAT );
	//	return true;
	//case CHAR_CLOSE:
	//	Hide();
	//	return true;
	default:
		return true;
	}
	return false;
}

bool CCharacterDLG::On_LButtonDN( unsigned iProcID, WPARAM wParam, LPARAM lParam )
{
	
	short nNeedPoint = 0;	
	switch( iProcID )
	{
	case IID_TAB_BASICINFO_BTN:
		m_iTab = BASIC_INFO;
		break;
	case IID_TAB_ABILITY_BTN:
		m_iTab = ABILITY_INFO;
		break;
	case IID_TAB_UNION_BTN:
		m_iTab = UNION_INFO;
		break;
	case IID_BTN_UP_STR:
		nNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_STR );
		if( ( g_pAVATAR->Get_BonusPOINT() - nNeedPoint ) >= 0 )
		{
//			g_pAVATAR->Set_BonusPOINT( g_pAVATAR->Get_BonusPOINT() - nNeedPoint );
			g_pNet->Send_cli_USE_BPOINT_REQ( BA_STR );
		}			
		break;
	case IID_BTN_UP_DEX:
		nNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_DEX );
		if( ( g_pAVATAR->Get_BonusPOINT() - nNeedPoint ) >= 0 )
		{
//			g_pAVATAR->Set_BonusPOINT( g_pAVATAR->Get_BonusPOINT() - nNeedPoint );
			g_pNet->Send_cli_USE_BPOINT_REQ( BA_DEX );
		}
		break;
	case IID_BTN_UP_INT:
		nNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_INT );
		if( ( g_pAVATAR->Get_BonusPOINT() - nNeedPoint ) >= 0 )
		{
//			g_pAVATAR->Set_BonusPOINT( g_pAVATAR->Get_BonusPOINT() - nNeedPoint );
			g_pNet->Send_cli_USE_BPOINT_REQ( BA_INT );
		}
		break;
	case IID_BTN_UP_CON:
		nNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_CON );
		if( ( g_pAVATAR->Get_BonusPOINT() - nNeedPoint ) >= 0 )
		{
//			g_pAVATAR->Set_BonusPOINT( g_pAVATAR->Get_BonusPOINT() - nNeedPoint );
			g_pNet->Send_cli_USE_BPOINT_REQ( BA_CON );
		}
		break;
	case IID_BTN_UP_CHARM:
		nNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_CHARM );
		if( ( g_pAVATAR->Get_BonusPOINT() - nNeedPoint ) >= 0 )
		{
//			g_pAVATAR->Set_BonusPOINT( g_pAVATAR->Get_BonusPOINT() - nNeedPoint );
			g_pNet->Send_cli_USE_BPOINT_REQ( BA_CHARM );
		}
		break;
	case IID_BTN_UP_SENSE:
		nNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_SENSE );
		if( ( g_pAVATAR->Get_BonusPOINT() - nNeedPoint ) >= 0 )
		{
//			g_pAVATAR->Set_BonusPOINT( g_pAVATAR->Get_BonusPOINT() - nNeedPoint );
			g_pNet->Send_cli_USE_BPOINT_REQ( BA_SENSE );
		}
		break;
	default:
		break;
	}
	return false;
}



void CCharacterDLG::Update( POINT ptMouse )
{
	if( !IsVision() ) return;

	CTDialog::Update( ptMouse );
	
	//std::map<int, CSlot>::iterator iterSlots;
	//for( iterSlots = m_Slots.begin(); iterSlots != m_Slots.end(); ++iterSlots )
	//	iterSlots->second.Update( ptMouse );


	/// ���ʽ� ����Ʈ�� ���� ���ͷ� Ȱ��ȭ..
	/*if( g_pAVATAR )
	{
		int iNeedPoint = 0;
		CWinCtrl* pCtrl = NULL;

		iNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_STR );
		pCtrl = Find( CHAR_BUTTON_UP_STR );
		if( ( g_pAVATAR->Get_BonusPOINT() - iNeedPoint ) >= 0 )
			pCtrl->SetEnable( true );
		else
			pCtrl->SetEnable( false );


		iNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_DEX );
		pCtrl = Find( CHAR_BUTTON_UP_DEX );
		if( ( g_pAVATAR->Get_BonusPOINT() - iNeedPoint ) >= 0 )
			pCtrl->SetEnable( true );
		else
			pCtrl->SetEnable( false );

		iNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_INT );
		pCtrl = Find( CHAR_BUTTON_UP_INT );
		if( ( g_pAVATAR->Get_BonusPOINT() - iNeedPoint ) >= 0 )
			pCtrl->SetEnable( true );
		else
			pCtrl->SetEnable( false );


		iNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_CON );
		pCtrl = Find( CHAR_BUTTON_UP_CON );
		if( ( g_pAVATAR->Get_BonusPOINT() - iNeedPoint ) >= 0 )
			pCtrl->SetEnable( true );
		else
			pCtrl->SetEnable( false );


		iNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_CHARM );
		pCtrl = Find( CHAR_BUTTON_UP_CHM );
		if( ( g_pAVATAR->Get_BonusPOINT() - iNeedPoint ) >= 0 )
			pCtrl->SetEnable( true );
		else
			pCtrl->SetEnable( false );

		iNeedPoint = g_pAVATAR->Get_NeedPoint2AbilityUP( BA_SENSE );
		pCtrl = Find( CHAR_BUTTON_UP_SEN );
		if( ( g_pAVATAR->Get_BonusPOINT() - iNeedPoint ) >= 0 )
			pCtrl->SetEnable( true );
		else
			pCtrl->SetEnable( false );

	}*/
	
	switch( m_iTab )
	{
	case BASIC_INFO:
		{
			CWinCtrl* pCtrl = Find( IID_TABBEDPANE );
			if( pCtrl && pCtrl->GetControlType() == CTRL_TABBEDPANE )
			{
				CTabbedPane* pPane = (CTabbedPane*)pCtrl;

				CJContainer* pContainer = pPane->GetTabContainer( IID_TABBASIC );
				assert( pContainer );
				if( pContainer )
				{
					pCtrl = pContainer->Find( IID_GUAGE_STAMINA );
					if( pCtrl && pCtrl->GetControlType() == CTRL_GUAGE )
					{
						CTGuage* pGuage = (CTGuage*)pCtrl;
						int iValue = g_pAVATAR->GetCur_STAMINA() * 100 / g_pAVATAR->GetCur_MaxSTAMINA();


						///if( iValue != pGuage->GetValue() )
						{
							pGuage->SetValue( iValue );
							pGuage->SetText( CStr::Printf("%d / %d", g_pAVATAR->GetCur_STAMINA(), g_pAVATAR->GetCur_MaxSTAMINA () ) );
							if( g_pAVATAR->GetCur_STAMINA() >= g_pAVATAR->GetCur_MaxSTAMINA () )
								pGuage->SetGuageImageID( m_iGuageBlueGID );
							else
								pGuage->SetGuageImageID( m_iGuageYellowGID );
						}
					}
				}
			}
			break;
		}
	case ABILITY_INFO:
		{
			///���� ���� ǥ��
			CTDialog* pDlg = CTDialog::GetProcessMouseOverDialog();
			if( pDlg && pDlg != this )
					return ;

			CWinCtrl* pCtrl = CWinCtrl::GetProcessMouseOverCtrl();
			if( pCtrl )
			{
				switch( pCtrl->GetControlID() )
				{
				case IID_BTN_UP_STR:
					CToolTipMgr::GetInstance().RegToolTip( ptMouse.x, ptMouse.y, 
						CStr::Printf("%s %d",STR_REQUIRE_POINT, g_pAVATAR->Get_NeedPoint2AbilityUP( BA_STR ) ));
					break;
				case IID_BTN_UP_DEX:
					CToolTipMgr::GetInstance().RegToolTip( ptMouse.x, ptMouse.y, 
						CStr::Printf("%s %d",STR_REQUIRE_POINT,g_pAVATAR->Get_NeedPoint2AbilityUP( BA_DEX ) ));
					break;
				case IID_BTN_UP_INT:
					CToolTipMgr::GetInstance().RegToolTip( ptMouse.x, ptMouse.y, 
						CStr::Printf("%s %d",STR_REQUIRE_POINT,g_pAVATAR->Get_NeedPoint2AbilityUP( BA_INT ) ));
					break;
				case IID_BTN_UP_CON:
					CToolTipMgr::GetInstance().RegToolTip( ptMouse.x, ptMouse.y, 
						CStr::Printf("%s %d",STR_REQUIRE_POINT,g_pAVATAR->Get_NeedPoint2AbilityUP( BA_CON ) ));
					break;
				case IID_BTN_UP_CHARM:
					CToolTipMgr::GetInstance().RegToolTip( ptMouse.x, ptMouse.y, 
						CStr::Printf("%s %d",STR_REQUIRE_POINT,g_pAVATAR->Get_NeedPoint2AbilityUP( BA_CHARM ) ));
					break;
				case IID_BTN_UP_SENSE:
					CToolTipMgr::GetInstance().RegToolTip( ptMouse.x, ptMouse.y, 
						CStr::Printf("%s %d",STR_REQUIRE_POINT, g_pAVATAR->Get_NeedPoint2AbilityUP( BA_SENSE ) ));
					break;
				default:
					break;
				}
			}
			break;
		}
	default:
		break;
	}
}

bool CCharacterDLG::IsVision()
{
	if( g_pAVATAR == NULL )
		return CTDialog::IsVision();
	else
		return( CTDialog::IsVision() && g_pAVATAR->Get_HP() > 0 );

	return false;
}

void CCharacterDLG::DrawBasicInfo()
{
	D3DXMATRIX mat;	
	D3DXMatrixTranslation( &mat, (float)m_sPosition.x, (float)m_sPosition.y,0.0f);
	::setTransformSprite( mat );

	///�̸�
	::drawFont( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 59,67, D3DCOLOR_XRGB( 0, 255, 42 ),  g_pAVATAR->Get_NAME() );		
	///Ŭ����(����)
	::drawFont( g_GameDATA.m_hFONT[ FONT_NORMAL ], true, 59,88, D3DCOLOR_XRGB( 0, 210, 255), CStringManager::GetSingleton().GetJobName( g_pAVATAR->Get_JOB() ) );		

	if( CClan::GetInstance().GetName() )
		::drawFont( g_GameDATA.m_hFONT[ FONT_NORMAL ], true, 59,109, g_dwWHITE, CClan::GetInstance().GetName() );		

	//����	
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 59,172, D3DCOLOR_XRGB( 255, 255, 0 ), "%d", g_pAVATAR->Get_LEVEL());		
	//����ġ	
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL ], true, 59,193, D3DCOLOR_XRGB( 0, 210, 255), "%d / %d", g_pAVATAR->Get_EXP(), g_pAVATAR->Get_NeedEXP(g_pAVATAR->Get_LEVEL()) );

	///214 - ���׹̳� ������
	//::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL ], true, 59,214, D3DCOLOR_XRGB( 0, 210, 255), "%d / %d", g_pAVATAR->GetCur_STAMINA(),c_iMaxStamina );
}

void CCharacterDLG::DrawAbilityInfo()
{
	D3DXMATRIX mat;	
	D3DXMatrixTranslation( &mat, (float)m_sPosition.x, (float)m_sPosition.y,0.0f);
	::setTransformSprite( mat );

	///��
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 58, 67, D3DCOLOR_XRGB( 0, 255, 42), "%d",g_pAVATAR->Get_STR());		
	///��ø
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 58, 88, D3DCOLOR_XRGB( 0, 255, 42), "%d",g_pAVATAR->Get_DEX());		
	///����
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 58,109, D3DCOLOR_XRGB( 0, 255, 42), "%d",g_pAVATAR->Get_INT());		
	///����
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 58,130, D3DCOLOR_XRGB( 0, 255, 42), "%d",g_pAVATAR->Get_CON());
	///�ŷ�
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 58,151, D3DCOLOR_XRGB( 0, 255, 42), "%d",g_pAVATAR->Get_CHARM());
	///����
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 58,172, D3DCOLOR_XRGB( 0, 255, 42), "%d",g_pAVATAR->Get_SENSE());


	///���ݷ�
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 171,  67, g_dwWHITE, "%d",g_pAVATAR->Get_ATK() );				
	///����
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 171,  88, g_dwWHITE, "%d",g_pAVATAR->Get_DEF() );
	///��������
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 171, 109, g_dwWHITE, "%d",g_pAVATAR->Get_RES() );
	///���߷�
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 171, 130, g_dwWHITE, "%d",g_pAVATAR->Get_HIT() );
	///ũ��Ƽ��
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 171, 151, g_dwWHITE, "%d",g_pAVATAR->Get_CRITICAL() );
	///ȸ�Ƿ�
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 171, 172, g_dwWHITE, "%d",g_pAVATAR->Get_AVOID() );
	///���ݼӵ�
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 171, 193, g_dwWHITE, "%d",g_pAVATAR->GetCur_ATK_SPD() );
	///�̵��ӵ�
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 171, 214, g_dwWHITE, "%.0f",g_pAVATAR->GetCur_MOVE_SPEED() );


	///����Ʈ
	::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 69,211, D3DCOLOR_XRGB( 0, 255, 42),"%d", g_pAVATAR->Get_BonusPOINT());			
}

void CCharacterDLG::DrawUnionInfo()
{
	D3DXMATRIX mat;	
	D3DXMatrixTranslation( &mat, (float)m_sPosition.x, (float)m_sPosition.y,0.0f);
	::setTransformSprite( mat );

	///Ŭ��
	::drawFont( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 90,67, g_dwCOLOR[ UNION_COLOR(g_pAVATAR->Get_UNION()) ],  UNION_NAME(g_pAVATAR->Get_UNION()) );

	int iAbility = AT_UNION_POINT1;
	POINT pt;
	for( int i = 0 ; i < 2; ++i )
	{
		pt.x = 50 + i * 113;
		for( int j = 0 ; j < 5; ++j )
		{
			pt.y = 130 + j * 21;
			::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, pt.x, pt.y, g_dwWHITE, "%d",g_pAVATAR->Get_AbilityValue( iAbility ) );		
			++iAbility;
		}
	}
}
bool CCharacterDLG::Create( const char* IDD )
{
	if( CTDialog::Create( IDD ) )
	{
			
		m_iGuageBlueGID		= CResourceMgr::GetInstance()->GetImageNID( IMAGE_RES_UI, "UI21_GUAGE_BLUE" );
		m_iGuageYellowGID	= CResourceMgr::GetInstance()->GetImageNID( IMAGE_RES_UI, "UI21_GUAGE_YELLOW" );
		m_iGuageRedGID		= CResourceMgr::GetInstance()->GetImageNID( IMAGE_RES_UI, "UI21_GUAGE_RED" );
		return true;
	}
	return false;
}