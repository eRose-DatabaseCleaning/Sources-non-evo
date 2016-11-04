/*
*
* Revision 1.2	2004/5/12	nAvy
*	�þ߰Ÿ��� ���� NPC�� ��� ���� �ڵ����ؼ� �׸��� �ָ��������� Map���� Load�� ���� ���
*
* Revision 1.1	2004/5/11	nAvy
*	npc �̹��� �׸��� ��� ����, map load�� �߰��� Info�� �׸���.���� ��ƾ�� Ȥ�� ������ �ʿ��Ҽ��� ������ ���Ƽ� ���ܵд�( �����̴� NPC �� ��� map���� �ε�� ��ġ�����θ� ǥ���Ҽ� ���� );
*
* Revision 1.0	2004/4/xx	
*	1. ���� �̴ϸ��� �߾����� ǥ�õǴ� World ��ǥ�κ��� �׷��� Object���� �Ÿ��� ���Ѵ�.
*	2. �̴ϸ��� �߾��� �׷����� Screen ��ǥ�� ���Ѵ�.
*	3. 1,2���� ���� Scale�� World/Screen ���,�̹�������� ���� �׷��� Left-Top Screen��ǥ�� ���Ѵ�.
*
*/
#include "stdafx.h"
#include "CMinimapDLG.h"
#include "../IT_MGR.h"
#include "../CTDrawImpl.h"
#include "../CToolTipMgr.h"
#include "../CTDrawImpl.h"
#include "../IO_ImageRes.h"
#include "../../IO_Terrain.h"
#include "../../System/CGame.h"
#include "../../../engine/include/zz_interface.h"
#include "../../GameData/CParty.h"

#include "TPane.h"
#include "TCaption.h"
#include "ResourceMgr.h"
#include "TAnimation.h"
#include "TFrame.h"
#include "TImage.h"

#include "AvatarInfoDlg.h"

#define AVATA_POS_INFO_X			110
#define AVATA_POS_INFO_Y			198

#define MINIMAP_CURSOR_WIDTH		16
#define MINIMAP_CURSOR_HEIGHT		16


#define AVATAR_POINT_SIZE			4
#define NPC_POINT_SIZE					21
#define	MINIMAP_RESOLUTION_PER_MAP		64



const int	c_iCaptionHeight					= 21;
const int	c_iMargin							= 1;
	  int	c_iPosFix							= 80;
const RECT  c_draw_zonename_rect_expand	 = { 5, 5, 5 + 170, 5 + 16 };
const RECT  c_draw_zonename_rect_normal	 = { 5, 5, 5 + 100, 5 + 16 };


CMinimapDLG::CMinimapDLG()
{
	m_fCurrentScale = 1;

	m_bShowMiniMap = true;
	m_bExploreMode = false;

	m_fMinimapCenterWorldPosX = 0;
	m_fMinimapCenterWorldPosY = 0;			

	m_fMinMinimapWorldPosX = 0;		// �̴ϸ��� ������ǥ��  ���� ��ǥ( ���� )
	m_fMaxMinimapWorldPosY = 0;		// �̴ϸ��� ������ǥ��  ���� ��ǥ( ���� )

	m_fMaxMinimapWorldPosX = 0;		/// �̴ϸ� Texture�� Size�� �̵������� �ִ� ���� X��ǥ�� ���� ��
	m_fMinMinimapWorldPosY = 0;		/// �̴ϸ� Texture�� Size�� �̵������� �ּ� ���� Y��ǥ�� ���� ��


	m_hMiniMap	= NULL;
	m_hArrow	= NULL;

	m_dwMinimapStartX = 0;

	m_fPrevUserPosX = 0;			
	m_fPrevUserPosY = 0;
	m_bDragStart    = false;

	m_iAvatarImageID = 0;
	m_iPartyImageID	 = 0;

	m_bExpand		= false;		///���� ũ�� ���̴°�?
	m_bMinimize		= false;	///���� ĸ�Ǹ� ���̴°�?

	
	m_indicator		= new CTAnimation;

	CTImage* image	= new CTImage;
	image->Create( 0,0, 20,20, 4,IMAGE_RES_STATE_ICON );

	CTFrame* frame  = new CTFrame;
	frame->set_image( image );
	frame->set_time( 300 );
	m_indicator->add_frame( frame );
	m_indicator->set_curr_frame( 1 );
	m_indicator->set_loop( false );
	m_indicator->Show();
}

CMinimapDLG::~CMinimapDLG()
{
	FreeMinimap();

	if( m_hArrow )
	{
		::unloadTexture( m_hArrow );
		m_hArrow   = NULL;
	}
	SAFE_DELETE( m_indicator );
}


///
///	�� ������ ���缭 �ش� �̴ϸ� �ؽ��ĸ� �а� �� ������ǥ���� �����Ѵ�..
///
///	iMapStartX : ���� ������ �̴ϸ��� �����ϴ� ���� ��ġ
///
/// Arrow�� �����ϰ� Minimap�� �����ϰ� �ٽ� Load�Ѵ�.

bool CMinimapDLG::SetMinimap( const char* pFname, const char* pCursorName, DWORD dwMapStartX, DWORD dwMapStartY )
{
	if( pFname == NULL )
		return false;

	FreeMinimap();
	setDelayedLoad(0);
	m_hMiniMap = loadTexture( pFname , pFname, 1, 0 );
	

	if( m_hArrow == NULL )
		m_hArrow = loadTexture( pCursorName , pCursorName, 1, 0);

	m_dwMinimapStartX = dwMapStartX;
	m_dwMinimapStartY = dwMapStartY;	
	
	CalculateDisplayPos();


	///ĳ������ Type������ �׷����� �̹��� ID��  Load;
	m_iAvatarImageID = CResourceMgr::GetInstance()->GetImageNID( IMAGE_RES_UI,"ID_OTHER_AVATAR");
	m_iPartyImageID  = CResourceMgr::GetInstance()->GetImageNID( IMAGE_RES_UI,"ID_MINIMAP_PARTYMEMBER");

	setDelayedLoad(1);


	m_zonename.set_string( 
		ZONE_NAME( g_pTerrain->GetZoneNO() ), 
		m_bExpand?c_draw_zonename_rect_expand : c_draw_zonename_rect_normal,
		FONT_NORMAL
		);

	return true;
}


void CMinimapDLG::FreeMinimap()
{
	if( m_hMiniMap )
	{
		::unloadTexture( m_hMiniMap );
		m_hMiniMap = NULL;
	}
}

void CMinimapDLG::CalculateDisplayPos()
{
	m_fMinMinimapWorldPosX = (float)( PATCH_COUNT_PER_MAP_AXIS * m_dwMinimapStartX ) * ( (float)GRID_COUNT_PER_PATCH_AXIS * nGRID_SIZE );		// �̴ϸ��� ���� ���� ��ǥ( ���� )
	m_fMaxMinimapWorldPosY = (float)( PATCH_COUNT_PER_MAP_AXIS * ( MAP_COUNT_PER_ZONE_AXIS - m_dwMinimapStartY + 1 ) ) 
								* ( (float)GRID_COUNT_PER_PATCH_AXIS * nGRID_SIZE );		// �̴ϸ��� ���� ���� ��ǥ( ���� )		

	int iTextureHeight = 0;
	int iTextureWidth  = 0;

	getTextureSize( m_hMiniMap, iTextureWidth, iTextureHeight );///setDelayedLoad(1)�� ��� Size�� ����� ��ȯ �ȵɼ� �ִ�.

	int iMapRowCount = iTextureHeight / MINIMAP_RESOLUTION_PER_MAP - 2;
	int iMapColCount = iTextureWidth  / MINIMAP_RESOLUTION_PER_MAP - 2;

	m_fMaxMinimapWorldPosX = m_fMinMinimapWorldPosX + iMapColCount * PATCH_COUNT_PER_MAP_AXIS * GRID_COUNT_PER_PATCH_AXIS * nGRID_SIZE;
	m_fMinMinimapWorldPosY = m_fMaxMinimapWorldPosY - iMapRowCount * PATCH_COUNT_PER_MAP_AXIS * GRID_COUNT_PER_PATCH_AXIS * nGRID_SIZE;
}

void CMinimapDLG::Draw()
{
	if(!IsVision()) 
		return ;

	CTDialog::Draw();

	D3DXMATRIX mat;	
	D3DXMatrixTranslation( &mat, (float)m_sPosition.x, (float)m_sPosition.y, 0.0f);
	::setTransformSprite( mat);

	m_zonename.draw();

	if( m_bMinimize == false && m_hMiniMap && m_hArrow )
	{
		if( m_bExploreMode )
		{
			///Explore����ϰ�� �� �ƹ�Ÿ�� ����������� NormalMode�� ��ȯ�ȴ�.		
			if( m_fPrevUserPosX != g_pAVATAR->Get_CurPOS().x || m_fPrevUserPosY != g_pAVATAR->Get_CurPOS().y )
				m_bExploreMode = false;
		}

		if( !m_bExploreMode )
		{
			m_fMinimapCenterWorldPosX = g_pAVATAR->Get_CurPOS().x;
			m_fMinimapCenterWorldPosY = g_pAVATAR->Get_CurPOS().y;
		}


		//------------------------------------------------------------------------------------------------
		// �̴ϸ� ���..	
		//------------------------------------------------------------------------------------------------
		/// 1. �׷��� Sprite ������ ������ ���Ѵ�.
		float fSpriteCenterX = (m_fMinimapCenterWorldPosX - m_fMinMinimapWorldPosX) / fGetWorldDistancePerPixel() + MINIMAP_RESOLUTION_PER_MAP;
		float fSpriteCenterY = (m_fMaxMinimapWorldPosY - m_fMinimapCenterWorldPosY) / fGetWorldDistancePerPixel() + MINIMAP_RESOLUTION_PER_MAP;

		/// 2. ���� ���� ��ŭ Rect�� ���Ѵ�.
		RECT rcMinimap;
		rcMinimap.left	 = (long)(fSpriteCenterX - m_iWidth / (2 * m_fCurrentScale)) + c_iMargin;
		rcMinimap.top	 = ((long)(fSpriteCenterY - ( m_iHeight - c_iCaptionHeight ) / (2 * m_fCurrentScale) ));
		rcMinimap.right  = ((long)(rcMinimap.left + m_iWidth / m_fCurrentScale) - c_iMargin * 2) + + c_iPosFix;
		rcMinimap.bottom = (long)(rcMinimap.top  + ( m_iHeight - c_iCaptionHeight ) / m_fCurrentScale) - c_iMargin;

		///3. Zoom����
		D3DXMatrixScaling( &mat, m_fCurrentScale, m_fCurrentScale, 0.0f );


		///4. �׸���
		::setTransformSprite( mat );

		D3DCOLOR color = D3DCOLOR_RGBA( 255, 255, 255, 255 );

		::drawSprite( m_hMiniMap, 
						&rcMinimap,
						NULL,
						&D3DXVECTOR3( (m_sPosition.x + c_iMargin)/ m_fCurrentScale,(m_sPosition.y + c_iCaptionHeight) / m_fCurrentScale,0.0f),
						color );

		////-----------------------------------------------------------------------------------------
		/////�ٸ� �ƹ�Ÿ���� ��ġ ǥ��
		DrawAVTPosition();
		DrawCoordinatesIndicator();
		///-----------------------------------------------------------------------------------------
		////�̴ϸ� Ŀ���� ����Ѵ�.	
		///1. �ƹ�Ÿ�� ������ ���Ѵ�.
		float fRadian = D3DXToRadian( g_pAVATAR->GetDirection() );

		///2.  �̴ϸ��� �߽����κ����� �ƹ�Ÿ�� ����Ÿ��� ���Ѵ�.
		float fDistanceX  = m_fMinimapCenterWorldPosX - g_pAVATAR->Get_CurXPOS();
		float fDistanceY  = g_pAVATAR->Get_CurYPOS() - m_fMinimapCenterWorldPosY;

		///3. ȭ�鿡�� �̴ϸ��� ���� ȭ����ǥ�� ���Ѵ�.
		float fScreenCenterX = (float)(m_sPosition.x + GetWidth() / 2);
		float fScreenCenterY = (float)(m_sPosition.y + c_iCaptionHeight +( GetHeight() - c_iCaptionHeight) / 2 );

		///4. �̴ϸ��� �������� ������ ȭ��Ÿ��� ���Ѵ�.
		float fDrawPosX = fScreenCenterX - m_fCurrentScale * fDistanceX / fGetWorldDistancePerPixel();
		float fDrawPosY = fScreenCenterY - m_fCurrentScale * fDistanceY / fGetWorldDistancePerPixel(); 


		if( IsInsideMinimap( fDrawPosX - MINIMAP_CURSOR_WIDTH / 2, fDrawPosY - MINIMAP_CURSOR_HEIGHT / 2, MINIMAP_CURSOR_WIDTH, MINIMAP_CURSOR_HEIGHT ) )
		{
			D3DXMatrixRotationZ( &mat, -fRadian );
			D3DXMATRIX matPos;
			D3DXMatrixTranslation( &matPos, (float)fDrawPosX, (float)fDrawPosY, 0.0f);
			D3DXMatrixMultiply( &mat, &mat, &matPos );
			::setTransformSprite( mat);

			::drawSprite( m_hArrow,
						NULL,
  						&D3DXVECTOR3( (float)(MINIMAP_CURSOR_WIDTH / 2), (float)(MINIMAP_CURSOR_HEIGHT / 2), 0.0f ),// rotation center,
						NULL,
						D3DCOLOR_RGBA( 255, 255, 255, 255 ));							 // color
		}

		/////2004 /  2/ 2 : nAvy �߰� - ��ǥ���׸��� ����̹��� �׸���
		int iImageIndex =  CResourceMgr::GetInstance()->GetImageNID( IMAGE_RES_UI,"ID_BLACK_PANEL");
		g_DrawImpl.Draw( m_sPosition.x + c_iMargin, 
						 m_sPosition.y + m_iHeight - 18,
						 m_iWidth - (c_iMargin) * 2, 
						 IMAGE_RES_UI, iImageIndex ,D3DCOLOR_ARGB(128,255,255,255));

		::drawFontf( g_GameDATA.m_hFONT[ FONT_NORMAL_BOLD ], true, 
						0,
						2,
						g_dwWHITE,"%d,%d",
						(int)g_pAVATAR->Get_CurPOS().x / 100, (int)g_pAVATAR->Get_CurPOS().y / 100 );
		
	}


}
void CMinimapDLG::DrawCoordinatesIndicator()
{
	float fDistanceX,fDistanceY;
	float fScreenCenterX, fScreenCenterY;
	float fDrawPosX, fDrawPosY;

	std::map< int ,S_IndicatorCoordinates >::iterator iter;

	for( iter = m_indicators_coordinates.begin(); iter != m_indicators_coordinates.end(); ++iter )
	{
		if( iter->second.m_zoneno == g_pTerrain->GetZoneNO() )
		{
			fDistanceX  = m_fMinimapCenterWorldPosX - iter->second.m_x;
			fDistanceY  = iter->second.m_y - m_fMinimapCenterWorldPosY;

			fScreenCenterX = (float)(m_sPosition.x + GetWidth() / 2);
			fScreenCenterY = (float)(m_sPosition.y + c_iCaptionHeight +( GetHeight() - c_iCaptionHeight) / 2 );

			fDrawPosX = fScreenCenterX - m_fCurrentScale * fDistanceX / fGetWorldDistancePerPixel();
			fDrawPosY = fScreenCenterY - m_fCurrentScale * fDistanceY / fGetWorldDistancePerPixel(); 

			if( IsInsideMinimap( fDrawPosX - 10 , fDrawPosY - 10, 20, 20 ) )
			{
				m_indicator->SetPosition( fDrawPosX - 10, (int)fDrawPosY - 10 );
				m_indicator->Draw();
			}
		}
	}
}

void CMinimapDLG::Update( POINT ptMouse )
{
	if(!IsVision()) return;	

	CTDialog::Update( ptMouse );

	m_indicator->Update( ptMouse );
	/// ���� �̴ϸ��� ���̰� �ְ� �巡�� ���϶� �̴ϸ� �߽��� �������ش�.( �巡�׷� �̴ϸ� �̵� )
	if( m_bShowMiniMap && m_bDragStart )
	{
		if( IsInsideMinimap( (float)ptMouse.x, (float)ptMouse.y , 0 , 0)  )
		{
			///���� ���콺��ġ�� ���� ���콺��ġ���� �Ÿ��� ���Ѵ�.
			float fDistanceX = (float)(m_ptLButtonDown.x - ptMouse.x);
			float fDistanceY = (float)(m_ptLButtonDown.y - ptMouse.y);

			/// ���� �Ÿ��̻��϶��� ����
			if( abs(fDistanceX) > 5 || abs(fDistanceY ) > 5 )
			{
				/// ȭ��Ÿ����� ���� �Ÿ��� ������ �̵����������ϰ�쿡�� �����Ѵ�.
				float fTempX = m_fMinimapCenterWorldPosX + fDistanceX * fGetWorldDistancePerPixel() / m_fCurrentScale;
				float fTempY = m_fMinimapCenterWorldPosY - fDistanceY * fGetWorldDistancePerPixel() / m_fCurrentScale;
				if( fTempX > m_fMinMinimapWorldPosX && fTempX < m_fMaxMinimapWorldPosX &&
					fTempY < m_fMaxMinimapWorldPosY && fTempY > m_fMinMinimapWorldPosY )
				{
					m_fMinimapCenterWorldPosX = fTempX;
					m_fMinimapCenterWorldPosY = fTempY;
					
					m_ptLButtonDown = ptMouse;
				}
			}
		}
		else
		{
			m_bDragStart = false;
		}
	}

	///���� ���� ǥ��
	CTDialog* pDlg = CTDialog::GetProcessMouseOverDialog();
	if( pDlg && pDlg != this )
			return ;

	if( m_zonename.is_ellipsis() )
	{
		POINT ptPoint = { ptMouse.x - m_sPosition.x , ptMouse.y - m_sPosition.y };
		const RECT& rect_zonename = m_zonename.get_rect();
		if( PtInRect( &rect_zonename, ptPoint ) )
		{
			CToolTipMgr::GetInstance().RegToolTip( m_sPosition.x - 20, m_sPosition.y - 20, m_zonename.get_string() );
			return;
		}
	}

	WINCTRL_LIST_ITOR iter;
	CWinCtrl*	pCtrl;
	
	for( iter = m_listChild.begin(); iter != m_listChild.end(); ++iter)
	{
		pCtrl = *iter;
		if( pCtrl->GetControlType() == CTRL_IMAGE )
			continue;

		if( pCtrl->IsInside(ptMouse.x, ptMouse.y ) )
		{
			CToolTipMgr::GetInstance().RegUIInfo( ptMouse.x, ptMouse.y, GetDialogType(), pCtrl->GetControlID());
			break;
		}
	}




}
unsigned int CMinimapDLG::Process(UINT uiMsg,WPARAM wParam,LPARAM lParam)
{
	if(!IsVision()) return 0;	

	unsigned iProcID = 0;
	if( iProcID = CTDialog::Process(uiMsg,wParam,lParam)) {
		switch(uiMsg)
		{
		case WM_LBUTTONUP:
			if(On_LButtonUP( iProcID , wParam ,lParam ))
				return uiMsg;
			break;
		case WM_LBUTTONDOWN:
			if(On_LButtonDN( iProcID , wParam, lParam ))
				return uiMsg;
			else
				return 0;
			break;
		}
		return uiMsg;
	}
	return 0;
}

bool CMinimapDLG::On_LButtonUP( unsigned iProcID, WPARAM wParam, LPARAM lParam )
{
	m_bDragStart   = false;
	CWinCtrl* pCtrl = NULL;
	switch( iProcID )
	{
	case IID_BTN_EXPAND:	///ū������� ����
		{
			Expand();
			return true;
		}
	case IID_BTN_NORMAL:	///���� ������� ����
		{
			Reduct();
			return true;
		}
	case IID_BTN_MINIMIZE_SMALL:
		{
			ToggleShowMinimapSmall();
			return true;
		}
	case IID_BTN_MINIMIZE_BIG:		///ĸ�Ǹ� ���̱�
		{
			ToggleShowMinimapBig();
			return true;
		}
	default:
		return false;
	}
	return false;
}

bool CMinimapDLG::On_LButtonDN( unsigned iProcID, WPARAM wParam, LPARAM lParam )
{
	m_ptLButtonDown.x = LOWORD( lParam );
	m_ptLButtonDown.y = HIWORD( lParam );

	if( IsInside( m_ptLButtonDown.x, m_ptLButtonDown.y ) )
	{
		if(IsInsideMinimap( (float)m_ptLButtonDown.x, (float)m_ptLButtonDown.y ,0 ,0 ))
		{
			m_bExploreMode = true;
			m_bDragStart   = true;
			m_fPrevUserPosX = g_pAVATAR->Get_CurPOS().x;
			m_fPrevUserPosY = g_pAVATAR->Get_CurPOS().y;
		}
		return true;
	}
	return false;
}

void CMinimapDLG::ToggleShowMinimap()
{
	if( m_bExpand )
		ToggleShowMinimapBig();
	else
		ToggleShowMinimapSmall();

}

void CMinimapDLG::ToggleZoomMinimap()
{
	if( m_bExpand )
		Reduct();
	else
		Expand();
}

void CMinimapDLG::DrawAVTPosition()
{
	int	  iImageID		 = m_iAvatarImageID;

	CTDrawImpl* pDrawObj = g_itMGR.GetDrawImplPtr();
	CObjCHAR *pCharOBJ = NULL;

	short nRegObjCnt = g_pObjMGR->GetViewObjectCnt();

	float fDistanceX  = 0;
	float fDistanceY  = 0;

	float fScreenCenterX = 0;
	float fScreenCenterY = 0;

	float fDrawPosX = 0;
	float fDrawPosY = 0;
	
	//m_listNpcMarkInfo.clear();
	m_listPartyMemberMarkInfo.clear();
	PartyMemberMarkInfo partyMemberMarkInfo;
	NpcMarkInfo	npcMarkInfo;
	//CPartyDlg* pDlg = (CPartyDlg*)g_itMGR.FindDlg( DLG_TYPE_PARTY );

	for (short nI=0; nI< nRegObjCnt ; nI++) 
	{
		pCharOBJ = g_pObjMGR->Get_CharOBJ( g_pObjMGR->GetViewObjectIndex( nI ) , false );
		if( pCharOBJ == NULL )
			continue;

	
		///�̴ϸ��� �߽����κ����� ���� �Ÿ��� ���Ѵ�.
		fDistanceX  = m_fMinimapCenterWorldPosX - pCharOBJ->Get_CurXPOS();
		fDistanceY  = pCharOBJ->Get_CurYPOS() - m_fMinimapCenterWorldPosY;

		///�̴ϸ��� �߽��� �׷��� Screen ��ǥ�� ���Ѵ�.
		fScreenCenterX = (float)(m_sPosition.x + GetWidth() / 2);
		fScreenCenterY = (float)(m_sPosition.y + c_iCaptionHeight + ( GetHeight() - c_iCaptionHeight) / 2 );

		
		switch( pCharOBJ->Get_TYPE() )
		{
		case OBJ_NPC:
			{	///�������� �����ִ� ������ �ϴ� �����Ѵ�.
				///�̹����� �׷��� Screen Left-top �� ���Ѵ�.
				//fDrawPosX = fScreenCenterX - m_fCurrentScale * fDistanceX / fGetWorldDistancePerPixel() - NPC_POINT_SIZE / 2;
				//fDrawPosY = fScreenCenterY - m_fCurrentScale * fDistanceY / fGetWorldDistancePerPixel() - NPC_POINT_SIZE / 2; 
				//
				//if( IsInsideMinimap( fDrawPosX, fDrawPosY, NPC_POINT_SIZE, NPC_POINT_SIZE ) )
				//{
				//	if( GetIndicatorNpc( pCharOBJ->Get_CharNO() ) )
				//		npcMarkInfo.iIconNo = 0;
				//	else
				//		npcMarkInfo.iIconNo = NPC_MARK_NO( pCharOBJ->Get_CharNO() );

				//	npcMarkInfo.rcDraw.left		= (long)fDrawPosX;
				//	npcMarkInfo.rcDraw.top		= (long)fDrawPosY;
				//	npcMarkInfo.rcDraw.right	= (long)(npcMarkInfo.rcDraw.left + NPC_POINT_SIZE);
				//	npcMarkInfo.rcDraw.bottom	= (long)(npcMarkInfo.rcDraw.top + NPC_POINT_SIZE);
				//	npcMarkInfo.strName			= pCharOBJ->Get_NAME();

				//	m_listNpcMarkInfo.insert( std::map<int, NpcMarkInfo>::value_type( pCharOBJ->Get_CharNO(), npcMarkInfo ));
				//}
				break;
			}
		case OBJ_AVATAR:
			{
				fDrawPosX = fScreenCenterX - m_fCurrentScale * fDistanceX / fGetWorldDistancePerPixel() - AVATAR_POINT_SIZE / 2;
				fDrawPosY = fScreenCenterY - m_fCurrentScale * fDistanceY / fGetWorldDistancePerPixel() - AVATAR_POINT_SIZE / 2; 
				if( IsInsideMinimap( fDrawPosX, fDrawPosY , AVATAR_POINT_SIZE, AVATAR_POINT_SIZE ) )
				{

					if( pCharOBJ->Get_TeamNO() != g_pAVATAR->Get_TeamNO() )
					{
						pDrawObj->Draw( (int)fDrawPosX - 6, (int)fDrawPosY - 6, IMAGE_RES_STATE_ICON, 73 );
					}
					else if( CParty::GetInstance().IsPartyMember(g_pObjMGR->Get_ServerObjectIndex( pCharOBJ->Get_INDEX() )) )
					{
						partyMemberMarkInfo.iIconNo = m_iPartyImageID;
						partyMemberMarkInfo.rcDraw.left = (long)fDrawPosX;
						partyMemberMarkInfo.rcDraw.top  = (long)fDrawPosY;
						partyMemberMarkInfo.rcDraw.right  = partyMemberMarkInfo.rcDraw.left + AVATAR_POINT_SIZE;
						partyMemberMarkInfo.rcDraw.bottom = partyMemberMarkInfo.rcDraw.top + AVATAR_POINT_SIZE;
						m_listPartyMemberMarkInfo.push_back( partyMemberMarkInfo );
					}
					else
					{
						pDrawObj->Draw( (int)fDrawPosX , (int)fDrawPosY , IMAGE_RES_UI, m_iAvatarImageID );
					}
				}
				break;
			}
		default:
			break;
		}

	}

	DrawPartyMember();
	//DrawNpcFromServer();
	DrawNpcFromMap();
}
void	CMinimapDLG::DrawPartyMember()
{
	CTDrawImpl* pDrawObj = g_itMGR.GetDrawImplPtr();
	///PartyMember
	std::list<PartyMemberMarkInfo>::iterator iterPartyMember;
	for( iterPartyMember = m_listPartyMemberMarkInfo.begin() ; iterPartyMember != m_listPartyMemberMarkInfo.end(); ++iterPartyMember )
		pDrawObj->Draw( (int)iterPartyMember->rcDraw.left , (int)iterPartyMember->rcDraw.top , IMAGE_RES_UI, iterPartyMember->iIconNo );

}
void	CMinimapDLG::DrawNpcFromMap()
{
	CTDrawImpl* pDrawObj = g_itMGR.GetDrawImplPtr();

	float fDistanceX  = 0;
	float fDistanceY  = 0;

	float fScreenCenterX = 0;
	float fScreenCenterY = 0;

	float fDrawPosX = 0;
	float fDrawPosY = 0;
	RECT  rcHitTest;
	

	POINT ptMouse;
	CGame::GetInstance().Get_MousePos( ptMouse );

	///NPC - ��� NPC: m_listNpcMarkInfo�� �ִ� NPC�ϰ��� �׸��� �ʴ´�.
	int iImageID  = 0;

	const std::list< NpcInfoInMap >& listNpcInfo  = g_pObjMGR->GetNpcInfoList();
	std::list< NpcInfoInMap >::const_iterator iterNpcAll;
	std::map<int,NpcMarkInfo>::iterator iterNpc;
	std::string strNpcName;
	for( iterNpcAll = listNpcInfo.begin(); iterNpcAll != listNpcInfo.end(); ++iterNpcAll )
	{
		//iterNpc = m_listNpcMarkInfo.find( iterNpcAll->m_iNpcID );
		//if( iterNpc == m_listNpcMarkInfo.end() )
		//{
			///�̴ϸ��� �߽����κ����� ���� �Ÿ��� ���Ѵ�.
			fDistanceX  = m_fMinimapCenterWorldPosX - iterNpcAll->m_Position.x;
			fDistanceY  = iterNpcAll->m_Position.y - m_fMinimapCenterWorldPosY;

			///���� �Ÿ����� ��ũ�� �Ÿ��� ���Ѵ�.
			fScreenCenterX = (float)(m_sPosition.x + GetWidth() / 2);
			fScreenCenterY = (float)(m_sPosition.y + c_iCaptionHeight + ( GetHeight() - c_iCaptionHeight) / 2 );

	


			//fScreenCenterX = (float)(m_sPosition.x + c_iMargin + GetWidth() / 2);
			//fScreenCenterY = (float)(m_sPosition.y + c_iMargin + GetHeight() / 2 + c_iCaptionHeight);

			fDrawPosX = fScreenCenterX - m_fCurrentScale * fDistanceX / fGetWorldDistancePerPixel() - NPC_POINT_SIZE / 2;
			fDrawPosY = fScreenCenterY - m_fCurrentScale * fDistanceY / fGetWorldDistancePerPixel() - NPC_POINT_SIZE / 2; 
			if( IsInsideMinimap( fDrawPosX, fDrawPosY , 20 , 20 ) )
			{

				strNpcName = NPC_NAME( iterNpcAll->m_iNpcID );
				///�̸��� ���� ��� NPC��޾��Ѵ�.
				if( !strNpcName.empty() )
				{

					if( GetIndicatorNpc( iterNpcAll->m_iNpcID ) )
					{
						m_indicator->SetPosition( (int)fDrawPosX, (int)fDrawPosY );
						m_indicator->Draw();
					}
					else
					{
						pDrawObj->Draw( (int)fDrawPosX , (int)fDrawPosY , IMAGE_RES_STATE_ICON, NPC_MARK_NO(iterNpcAll->m_iNpcID) );
					}

					///���� �̹��� ������ 21/21 ���� 8/8�̴�.
					SetRect( &rcHitTest, (int)fDrawPosX + 6, (int)fDrawPosY + 6,(int)fDrawPosX + 14, (int)fDrawPosY + 14 );

					if( PtInRect( &rcHitTest, ptMouse ))
					{
						if( iterNpcAll->m_iNpcID > 0 && iterNpcAll->m_iNpcID < g_TblNPC.m_nDataCnt )
						{
							///���� ���� ǥ��
							CTDialog* pDlg = CTDialog::GetProcessMouseOverDialog();
							if( !(pDlg && pDlg != this) )
								CToolTipMgr::GetInstance().RegToolTip( ptMouse.x, ptMouse.y ,strNpcName.c_str() );

						}
					}
				}
			}
//		}
	}
}


float CMinimapDLG::fGetWorldDistancePerPixel()
{
	return  (float)(PATCH_COUNT_PER_MAP_AXIS * GRID_COUNT_PER_PATCH_AXIS * nGRID_SIZE / ( MINIMAP_RESOLUTION_PER_MAP));
}

bool  CMinimapDLG::IsInsideMinimap( float x, float y ,int width, int height )
{
	if( !m_bShowMiniMap )
		return false;

	if( x > m_sPosition.x + c_iMargin && 
		(x + width) < m_sPosition.x + m_iWidth - c_iMargin && 
		y > m_sPosition.y + c_iCaptionHeight  && 
		(y + height) < m_sPosition.y + m_iHeight )
		return true;

	return false;
}

///�׻� ���̰� ���ʿ� �ѹ� ȣ��Ǿ�� �Ѵ�.
void CMinimapDLG::Show()
{
	CTDialog::Show();

	HideChild( IID_PANE_BIG );
}

CWinCtrl* CMinimapDLG::FindPaneChild( unsigned uiPaneID, unsigned uiChildID )
{
	CWinCtrl* pCtrl = Find( uiPaneID );

	assert( pCtrl );
	if( pCtrl == NULL ) return NULL;
	
	assert( pCtrl->GetControlType() == CTRL_PANE );
	if( pCtrl->GetControlType() != CTRL_PANE ) return NULL;
		
	CTPane* pPane = (CTPane*)pCtrl;
	return pPane->FindChild( uiChildID );
}

void CMinimapDLG::Expand()
{
	CWinCtrl* pCtrl = NULL;
	m_bExpand = true;
	
	HideChild( IID_PANE_SMALL );
	ShowChild( IID_PANE_BIG );

	int iOldWidth = m_iWidth;

	if( m_bMinimize )
	{
		if( pCtrl = FindPaneChild(IID_PANE_BIG, IID_CAPTION_BIG ) )
		{
			SetWidth( pCtrl->GetWidth() );
			SetHeight( pCtrl->GetHeight() );
		}

		if( pCtrl = FindPaneChild( IID_PANE_BIG, IID_PANE_BIG_CHILDPANE ) )
			pCtrl->Hide();

	}
	else
	{
		if( pCtrl = Find( IID_PANE_BIG ) )
		{
			SetWidth( pCtrl->GetWidth() );
			SetHeight( pCtrl->GetHeight() );
		}
		c_iPosFix = 110;
	}

	POINT ptNewPos = { getScreenWidth() - m_iWidth, m_sPosition.y };	
	MoveWindow( ptNewPos );

	m_zonename.set_rect( c_draw_zonename_rect_expand );
}

void CMinimapDLG::Reduct()
{
	m_bExpand = false;

	ShowChild( IID_PANE_SMALL );
	HideChild( IID_PANE_BIG );

	int iOldWidth = m_iWidth;
	CWinCtrl* pCtrl = NULL;
	if( m_bMinimize )
	{
		if( pCtrl = FindPaneChild( IID_PANE_SMALL, IID_CAPTION_SMALL ) )
		{
			SetWidth( pCtrl->GetWidth() );
			SetHeight( pCtrl->GetHeight() );
		}

		if( pCtrl = FindPaneChild( IID_PANE_SMALL, IID_PANE_SMALL_CHILDPANE ) )
			pCtrl->Hide();

	}
	else
	{
		if( pCtrl = Find( IID_PANE_SMALL ) )
		{
			SetWidth( pCtrl->GetWidth() );
			SetHeight( pCtrl->GetHeight() );
		}
		c_iPosFix = 80;
	}

	POINT ptNewPos = { getScreenWidth() - m_iWidth, m_sPosition.y };	

	MoveWindow( ptNewPos );

	m_zonename.set_rect( c_draw_zonename_rect_normal );
}


void CMinimapDLG::ToggleShowMinimapSmall()
{
	CWinCtrl* pCtrl;
	if( m_bMinimize )
	{
		m_bMinimize		= false;
		if( pCtrl = Find( IID_PANE_SMALL ) )
		{
			SetWidth( pCtrl->GetWidth() );
			SetHeight( pCtrl->GetHeight() );
		}
		c_iPosFix = 80;

		if( pCtrl = FindPaneChild( IID_PANE_SMALL, IID_PANE_SMALL_CHILDPANE ) )
			pCtrl->Show();
	}
	else
	{
		m_bMinimize = true;
		if( CTCaption* pCaption = GetCaption() )
			SetHeight( pCaption->GetHeight() );

		if( pCtrl = FindPaneChild( IID_PANE_SMALL, IID_PANE_SMALL_CHILDPANE ) )
			pCtrl->Hide();

	}
}

void CMinimapDLG::ToggleShowMinimapBig()
{
	CWinCtrl* pCtrl;
	if( m_bMinimize )
	{
		m_bMinimize	= false;
		if( pCtrl = Find( IID_PANE_BIG ) )
		{
			SetWidth( pCtrl->GetWidth() );
			SetHeight( pCtrl->GetHeight() );
		}
		c_iPosFix = 110;

		if( pCtrl = FindPaneChild( IID_PANE_BIG, IID_PANE_BIG_CHILDPANE ) )
			pCtrl->Show();
	}
	else
	{
		m_bMinimize = true;
		if( CTCaption* pCaption = GetCaption() )
			SetHeight( pCaption->GetHeight() );

		if( pCtrl = FindPaneChild( IID_PANE_BIG, IID_PANE_BIG_CHILDPANE ) )
			pCtrl->Hide();

	}
}

void CMinimapDLG::AddIndicatorNpc( int npcno, int auto_delete )
{
	S_IndicatorNpc newitem;
	newitem.m_iNpcNo      = npcno;
	newitem.m_auto_remove = auto_delete;
	m_indicators_npc.insert( make_pair( npcno, newitem ) );
}

void CMinimapDLG::AddIndicatorCoordinates( int index, int zoneno, float x, float y )
{
	S_IndicatorCoordinates newitem;
	newitem.m_zoneno = zoneno;
	newitem.m_x = x;
	newitem.m_y = y;
	
	std::map< int ,S_IndicatorCoordinates >::iterator iter;
	iter = m_indicators_coordinates.find( index );
	if( iter != m_indicators_coordinates.end() )
	{
		assert( 0 && "�ߺ��� �ε��� �Դϴ�:��ǥ�ε������� �Է¿���");
		return;
	}
	else
	{
		m_indicators_coordinates.insert( make_pair( index, newitem) );
	}
}

void CMinimapDLG::RemoveIndicatorNpc( int npcno )
{
	std::multimap< int ,S_IndicatorNpc >::iterator iter;
	iter = m_indicators_npc.find( npcno );
	if( iter != m_indicators_npc.end() )
		m_indicators_npc.erase( iter );
}

void CMinimapDLG::RemoveAutodeleteIndicatorNpc( int npcno )
{
	std::multimap< int ,S_IndicatorNpc >::iterator iter;
	for( iter = m_indicators_npc.begin(); iter != m_indicators_npc.end(); )
	{
		if( iter->second.m_iNpcNo == npcno && iter->second.m_auto_remove )
			iter = m_indicators_npc.erase( iter );
		else
			iter++;
	}
}

void CMinimapDLG::RemoveIndicatorCoordinates( int index )
{
	std::map< int ,S_IndicatorCoordinates >::iterator iter;
	iter = m_indicators_coordinates.find( index );
	if( iter != m_indicators_coordinates.end() )
		m_indicators_coordinates.erase( iter );
}

int CMinimapDLG::GetIndicatorNpc( int npcno )
{
	std::multimap< int ,S_IndicatorNpc >::iterator iter;
	iter = m_indicators_npc.find( npcno );
	if( iter != m_indicators_npc.end() )
		return 1;

	return 0;
}

void CMinimapDLG::SetAvataInfoPos(int y)
{
#ifdef _NEWUI	
	POINT ptNewPos = { m_sPosition.x, y };
	MoveWindow( ptNewPos );	
#endif
}

void CMinimapDLG::RefreshDlg()
{
	POINT ptNewPos = { getScreenWidth() - m_iWidth, m_sPosition.y };	
	MoveWindow( ptNewPos );
}

void CMinimapDLG::SetInterfacePos_After()
{
	RefreshDlg();	
}