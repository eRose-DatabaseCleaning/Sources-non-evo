#include "stdafx.h"
#include "CSkillCommand.h"
#include "it_mgr.h"
#include "OBJECT.h"	
#include "Skill.h"	
#include "../Network/CNetwork.h"	
#include "../JCommandState.h"
#include "Dlgs/ChattingDlg.h"
#include "../NetWork/CNetWork.h"
#include "GameData/CParty.h"
#include "GameData/CExchange.h"
#include "GameData/CPrivateStore.h"
#include "io_terrain.h"
#include "../Country.h"
#include "../System/CGame.h"


CCommand* g_QuickCommandPool[ MAX_QUICK_COMMAND ];



//----------------------------------------------------------------------
/// Basic command class
//----------------------------------------------------------------------


CBasicCommand::CBasicCommand()
{
}

CBasicCommand::~CBasicCommand()
{
}

bool CBasicCommand::Execute()
{	
	/// get skill.
	CSkillSlot* pSkillSlot = g_pAVATAR->GetSkillSlot();
	CSkill* pSkill = pSkillSlot->GetSkill( m_iSlotNO );

	int iBasicCommandType = SKILL_DISTANCE( pSkill->GetSkillIndex() );

	//Numenor: check if command can be executed
	if(!g_pAVATAR->Skill_ActionCondition( pSkill->GetSkillIndex() )) return false;
	bool actionDone = false;
		
	/// �⺻���( �ɱ�, ����, ����ǥ���� )�� ��� �����Ÿ��� � ��������� �Ǵ��ϴµ� ���
	switch( iBasicCommandType )
	{		
	case COMMON_COMMAND_SELFTARGET:
		{
			g_UserInputSystem.SetTargetSelf();
			actionDone = true;
		}
		break;
	case COMMON_COMMAND_SIT:
		{
			/// ��ž�¸�忡��.. ����
			if( g_pAVATAR->GetPetMode() > 0 )
				break;

			if( ( g_pAVATAR->Get_STATE() != CS_STOP ) && ( g_pAVATAR->Get_STATE() != CS_SIT ) )
			{
				g_itMGR.AppendChatMsg( STR_ACTION_COMMAND_STOP_STATE_FAILED, IT_MGR::CHAT_TYPE_SYSTEM );
				return false;
			}

			g_pNet->Send_cli_TOGGLE( TOGGLE_TYPE_SIT );
			///I ask my server for my HP information when I toggle.
			g_pNet->Send_cli_HP_REQ( g_pAVATAR->Get_INDEX() );
			actionDone = true;
		}
		break;
	case COMMON_COMMAND_PICK_ITEM: //Numenor: this is when you use the "gather" skill. Not when you click on the object, just on the skill
		{
			/// ��ž�¸�忡��.. ����
			//if( g_pAVATAR->GetPetMode() > 0 )
			//	break;

			//-------------------------------------------------------------------------------
			/// �ɱ� ���¿��� ����..
			//-------------------------------------------------------------------------------				
			if( g_pAVATAR->Get_STATE() == CS_SIT )
				break;


			D3DVECTOR vecCenter = g_pAVATAR->Get_CurPOS();
			int iNodeCnt = ::collectBySphere( (float *)( &vecCenter ), 1000 );

			CGameOBJ*	pObj			= NULL;
			HNODE		hNode			= 0;
			HNODE		hUserData		= 0;
			D3DVECTOR	vecTarget;
			int			iItemDistance	= 100000;
			bool		bFindItem		= false;
			CObjITEM*	pFoundItem		= NULL;
			CObjITEM*	pTempItem		= NULL;

			for( int i = 0; i < iNodeCnt; i++ )
			{
				hNode = ::getCollectNode( i );
				hUserData = ::getUserData( hNode );

				if( hUserData )
				{
					pObj = reinterpret_cast< CGameOBJ* >( hUserData );
	    
					/// Item ���� �˻�..
					if( pObj->IsA( OBJ_ITEM ) )
					{		
						pTempItem = (CObjITEM*)pObj;
						DWORD dwPassTIME = g_GameDATA.GetGameTime() - pTempItem->m_dwCreatedTIME;
						if ( pTempItem->m_wOwnerServerObjIDX &&  (int)(pTempItem->m_wRemainTIME -  dwPassTIME) >  62 * 1000 ) 
						{
							// ȹ�� ������ �ִ��� ����...
							if ( pTempItem->m_wOwnerServerObjIDX != g_pObjMGR->Get_ServerObjectIndex(  g_pAVATAR->m_nIndex ) ) 
							{
								//g_itMGR.AppendChatMsg( STR_NOTIFY_02, g_dwBLUE );
								continue;
							}
						}

						vecTarget = pObj->Get_CurPOS();
						int iDistance = CD3DUtil::distance ((int)vecCenter.x, (int)vecCenter.y, (int)vecTarget.x, (int)vecTarget.y);
						if( iDistance < iItemDistance )
						{
							iItemDistance = iDistance;

							bFindItem = true;
							pFoundItem = (CObjITEM*)pObj;
						}							
					}
				}
			}

			if( bFindItem )
			{
				if( pFoundItem )
				{						
					g_pNet->Send_cli_MOUSECMD( pFoundItem->Get_INDEX(), pFoundItem->Get_CurPOS() );	
					g_UserInputSystem.ClearMouseState();						
					actionDone = true;
				}
			}
		}
		break;

	/// ���� ���¿��� ���� �Ұ�
	case COMMON_COMMAND_JUMP:
		{
			/// ��ž�¸�忡��.. ����
			if( g_pAVATAR->GetPetMode() > 0 )
				break;


			if( ( g_pAVATAR->Get_STATE() != CS_SIT ) ){
				g_pNet->Send_cli_SET_MOTION( SKILL_ANI_ACTION_TYPE( pSkill->GetSkillIndex() ) );			
			///else
			///	AddMsgToChatWND( STR_ACTION_COMMAND_STOP_STATE_FAILED, g_dwRED, CChatDLG::CHAT_TYPE_SYSTEM );
				actionDone = true;
			}
		}
		break;
	case COMMON_COMMAND_AIR_JUMP: 
		{
			/// ��ž�¸�忡��.. ����
			if( g_pAVATAR->GetPetMode() > 0 )
				break;

			if( ( g_pAVATAR->Get_STATE() != CS_SIT ) ){
				g_pNet->Send_cli_SET_MOTION( SKILL_ANI_ACTION_TYPE( pSkill->GetSkillIndex() ), 0, 0, pSkill->GetSkillIndex() );
			///else
			///	AddMsgToChatWND( STR_ACTION_COMMAND_STOP_STATE_FAILED, g_dwRED, CChatDLG::CHAT_TYPE_SYSTEM );
				actionDone = true;
			}
		}
		break;
	/// ���� Ÿ��..
	case COMMON_COMMAND_AUTO_TARGET:
		{
		}
		break;
	/// �Ϲ� ����
	case COMMON_COMMAND_ATTACK:
		{
			//-------------------------------------------------------------------------------
			/// �ɱ� ���¶��.. ���� ����� �����ش�.
			//-------------------------------------------------------------------------------
			if( g_pAVATAR->Get_STATE() == CS_SIT )
			{
				g_pNet->Send_cli_TOGGLE( TOGGLE_TYPE_SIT );
				break;
			}


			int iTargetObj = g_UserInputSystem.GetCurrentTarget();
			if( iTargetObj == 0 )
				break;

			int iServerTarget = g_pAVATAR->Get_TargetIDX();
			int iClientTarget = g_pObjMGR->Get_ClientObjectIndex( iServerTarget );
			/// ���� ���� �������γ��� �ٽ� ��Ŷ�� ������ �ʴ´�.
			if( g_pAVATAR->Get_COMMAND() == CMD_ATTACK && iClientTarget == iTargetObj )
				break;				

			CObjCHAR* pChar = g_pObjMGR->Get_CharOBJ( iTargetObj, true );				
			if( pChar && pChar->IsA( OBJ_MOB ) )
			{
				CObjMOB* pObjMOB = (CObjMOB*)pChar;
				if( pObjMOB != NULL )
				{
					/// ���ݺҰ� NPC �� ���ݸ���..
					if( CUserInputState::IsEnemy( pObjMOB ) && ( NPC_CAN_TARGET( pObjMOB->Get_CharNO() ) != 1 ) ) 
					{
						//----------------------------------------------------------------------------------------------------
						/// PAT�� Ÿ�� �ִ� ���߿��� �Ұ�
						/// �׷��� Castle Gear �� ����
						//----------------------------------------------------------------------------------------------------
						int iPetMode = g_pAVATAR->GetPetMode();
						if( iPetMode < 0 )
						{
							g_pNet->Send_cli_ATTACK( iTargetObj );						
						}else
						{
							/// Pet mode �� ��쿡��..
							if( g_pAVATAR->CanAttackPetMode() )
							{
								g_pNet->Send_cli_ATTACK( iTargetObj );
							}								
						}
						actionDone = true;
					}
				}
			}
		}
		break;
	case COMMON_COMMAND_DRIVE_CART:
		{
			if( g_pAVATAR->CanRidePET() == false )
			{
				g_itMGR.AppendChatMsg( STR_ERROR_NOT_ASSEMBLED, IT_MGR::CHAT_TYPE_SYSTEM );
				return false;
			}


			if( CPrivateStore::GetInstance().IsOpened()  )
			{
				g_itMGR.AppendChatMsg(STR_CANT_USE_DRIVESKILL, IT_MGR::CHAT_TYPE_SYSTEM );
				return false;
			}
			/// ���� ��ž�»��¿��� ���� ���¸� Ȯ��..
			if( g_pAVATAR->GetPetMode() >= 0 )
			{
				if( ( g_pAVATAR->GetPetState() != CS_STOP ) )
				{
					g_itMGR.AppendChatMsg( STR_ACTION_COMMAND_STOP_STATE_FAILED, IT_MGR::CHAT_TYPE_SYSTEM );
					return false;
				}
			}
			else
			{
				if( ( g_pAVATAR->Get_STATE() != CS_STOP ) )
				{
					g_itMGR.AppendChatMsg( STR_ACTION_COMMAND_STOP_STATE_FAILED, IT_MGR::CHAT_TYPE_SYSTEM );
					return false;
				}

				int iItemType = g_pAVATAR->m_Inventory.m_ItemRIDE[ 0 ].GetTYPE();
				int iItemNo   = g_pAVATAR->m_Inventory.m_ItemRIDE[ 0 ].GetItemNO();

				if( CCountry::GetSingleton().IsApplyNewVersion() )
				{
					switch( ZONE_RIDING_REFUSE_FLAG( g_pTerrain->GetZoneNO() ) )
					{
					case 1:
						if( (ITEM_TYPE( iItemType, iItemNo ) / 100 == 5) && (ITEM_TYPE( iItemType, iItemNo ) % 10 == 1 ) )
						{
							g_itMGR.AppendChatMsg( STR_CANT_USE_CART , IT_MGR::CHAT_TYPE_SYSTEM , D3DCOLOR_ARGB( 255, 206, 223, 136));				
							return false;
						}
						break;
					case 2:
						if( (ITEM_TYPE( iItemType, iItemNo ) / 100 == 5) && (ITEM_TYPE( iItemType, iItemNo ) % 10 == 2 ) )
						{
							g_itMGR.AppendChatMsg( STR_CANT_USE_CASTLEGEAR , IT_MGR::CHAT_TYPE_SYSTEM , D3DCOLOR_ARGB( 255, 206, 223, 136));				
							return false;
						}
						break;
					case 3:
						g_itMGR.AppendChatMsg(STR_CANT_USE_DRIVESKILL, IT_MGR::CHAT_TYPE_SYSTEM, D3DCOLOR_ARGB( 255, 206, 223, 136) );
						return false;
						break;
					default:
						break;
					}
				}
			}


			//20050818 ȫ�� īƮ �������� 0�϶� īƮ ����̺� ��ų ���� "������ ���� �ʾƼ� ����� �� �����ϴ�."
			if( g_pAVATAR->GetCur_PatHP() <= 0 )
			{
				g_itMGR.AppendChatMsg( STR_NOTIFY_06, IT_MGR::CHAT_TYPE_SYSTEM, D3DCOLOR_ARGB( 255, 206, 223, 136) );
			}			

			g_pNet->Send_cli_TOGGLE( TOGGLE_TYPE_DRIVE );
			actionDone = true;
		}
		break;

		//2�ν� īƮ
	case COMMON_COMMAND_BOARD_CART: //īƮ 2�ν¿� ������ �¿�� ������ ���ִ� ��ų.
		{
			//Exception Proc.//
			//�������� ĳ���Ͱ� �����ϸ� ����.
			if( g_pAVATAR->IsRideUser() || g_pAVATAR->GetRideUserIndex() )
			{
				g_pNet->Send_cli_TOGGLE( TOGGLE_TYPE_DRIVE );
				return false;
			}

			//�ƹ�Ÿ�� īƮ�� ž���ϰ� �־�� ��.
			if( g_pAVATAR->GetPetMode() < 0 )
			{
				g_itMGR.AppendChatMsg(STR_MUST_BOARD_CART_ABLE_TO_USE, IT_MGR::CHAT_TYPE_SYSTEM, D3DCOLOR_ARGB( 255, 206, 223, 136) );
				return false;
			}

#ifdef _GBC
			//�������� �����Ǿ� �־�� ��.
			if( g_pAVATAR->IsRideItem( RIDE_PART_ABIL ) == false )
			{
				g_itMGR.AppendChatMsg(STR_AFFIX_RUMBLE, IT_MGR::CHAT_TYPE_SYSTEM, D3DCOLOR_ARGB( 255, 206, 223, 136) );
				return false;
			}								
#endif

			//Find Index.
			int iTargetObjClientIndex = g_UserInputSystem.GetCurrentTarget();
			CObjAVT* pAVT = g_pObjMGR->Get_CharAVT( iTargetObjClientIndex, true );

			// �ɸ��͸� �����ؾ� �մϴ�.
			if( !pAVT )
			{
				g_itMGR.AppendChatMsg(STR_REQUEST_SELECT_CHARACTER, IT_MGR::CHAT_TYPE_SYSTEM, D3DCOLOR_ARGB( 255, 206, 223, 136) );				
				return false;
			}

			// �Ÿ��� �ʹ� �شϴ�. (200 ���� == 2����)
#ifdef _DEBUG
			CStr::DebugString( "Dist: %f \n", g_pAVATAR->Get_DISTANCE( pAVT ) );
			CStr::DebugString( "SKILL_SCOPE: %f \n", SKILL_SCOPE(pSkill->GetSkillIndex()) );			
#endif		

			if( g_pAVATAR->Get_DISTANCE( pAVT ) >= SKILL_SCOPE(pSkill->GetSkillIndex()) )
			{				
				g_itMGR.AppendChatMsg( STR_OUTOFRANGE, IT_MGR::CHAT_TYPE_SYSTEM, D3DCOLOR_ARGB( 255, 206, 223, 136) );							
				return false;
			}

			//īƮ�� �¿�� ���� ���� �Դϴ�. (������ īƮ ž���ϰ� ����.)
			if( pAVT->GetPetMode() >= 0 )
			{
				g_itMGR.AppendChatMsg(STR_CANT_BOARD_CONDITION, IT_MGR::CHAT_TYPE_SYSTEM, D3DCOLOR_ARGB( 255, 206, 223, 136) );
				return false;
			}			

			//�ʹ� ���ſ��� �¿� �� �����ϴ�.
			if( pAVT->GetWeightRate() >= 100 )
			{
				g_itMGR.AppendChatMsg(STR_CANT_TOO_HEAVY, IT_MGR::CHAT_TYPE_SYSTEM, D3DCOLOR_ARGB( 255, 206, 223, 136) );
				return false;
			}

			//Send Msg.
			if( iTargetObjClientIndex != g_pAVATAR->Get_INDEX() && pAVT )
			{
                g_pNet->Send_cli_CART_RIDE( CART_RIDE_REQ,
					g_pObjMGR->Get_ServerObjectIndex(g_pAVATAR->Get_INDEX()),
					g_pObjMGR->Get_ServerObjectIndex(iTargetObjClientIndex) );
				
#ifdef _DEBUG
				CStr::DebugString("Send_cli_CART_RIDE\n");
#endif				
			}	
			actionDone = true;
		}
		break;

	case COMMON_COMMAND_PARTY:
		{
			int iTargetObjClientIndex = g_UserInputSystem.GetCurrentTarget();
			CObjAVT* pAVT = g_pObjMGR->Get_CharAVT( iTargetObjClientIndex, true );

			if( iTargetObjClientIndex != g_pAVATAR->Get_INDEX() && pAVT )
			{
				if( !g_pTerrain->IsPVPZone() ||
					( g_pTerrain->IsPVPZone() && pAVT->Get_TeamNO() == g_pAVATAR->Get_TeamNO() )
				)
				{
					if( CParty::GetInstance().HasParty() )
					{
						///��Ƽ �ʴ�
						if( CParty::GetInstance().IsPartyLeader() )
						{
							///��Ƽ Ǯ
							if( CParty::GetInstance().IsPartyFull() ){
								g_itMGR.AppendChatMsg(STR_PARTY_IS_FULL, IT_MGR::CHAT_TYPE_SYSTEM );
							}
							else if( CParty::GetInstance().IsShareParty() ){
								g_itMGR.AppendChatMsg("You are in a scaled party. You cannot invite someone as in a normal party.", IT_MGR::CHAT_TYPE_SYSTEM );
							}
							else{
								g_pNet->Send_cli_PARTY_REQ(PARTY_REQ_JOIN,  g_pObjMGR->Get_ServerObjectIndex(iTargetObjClientIndex ));
								actionDone = true;
							}
						}
						else///Error
						{
							g_itMGR.AppendChatMsg(STR_CANT_INVITE_TO_PARTY, IT_MGR::CHAT_TYPE_SYSTEM );
						}
					}
					else
					{
						g_pNet->Send_cli_PARTY_REQ(PARTY_REQ_MAKE,  g_pObjMGR->Get_ServerObjectIndex(iTargetObjClientIndex));
						actionDone = true;
					}
				}
				else
				{
					g_itMGR.OpenMsgBox( STR_CANTPARTY_WITHOTHERTEAM_INPVPZONE );
				}
			}
		}
		break;

	case COMMON_COMMAND_EXCHANGE:
		{
			int iTargetObjClientIndex = g_UserInputSystem.GetCurrentTarget();

						
			if( iTargetObjClientIndex != g_pAVATAR->Get_INDEX()	)				// Ÿ���� �ڽ��� ��� �ȵ�				
			{
				CObjAVT* pObjAVT = g_pObjMGR->Get_CharAVT( iTargetObjClientIndex , true );
				if( pObjAVT && !pObjAVT->IsRideUser() ) //ȫ�� : �������� ź �ƹ�Ÿ���� �ŷ� ����.
				{
					if( CExchange::GetInstance().SendCliTradeReq( g_pObjMGR->Get_ServerObjectIndex(iTargetObjClientIndex) ) )
					{
						string Temp = pObjAVT->Get_NAME();
						Temp.append(STR_REQ_TRADE);
						g_itMGR.AppendChatMsg( Temp.c_str(), IT_MGR::CHAT_TYPE_SYSTEM);
						actionDone = true;
					}
				}
			}
		}
		break;

	case COMMON_COMMAND_PRIVATESTORE:
		{

			if(CCountry::GetSingleton().IsJapan())
			{


				if( !(CGame::GetInstance().GetPayType() & PLAY_FLAG_TRADE) )
				{
					g_itMGR.OpenMsgBox( STR_JP_BILL_CANT_OPEN_PRIVATESTORE );
					return false;
				}        
			}


			/// ���� 10���� �ȿ� NPC�� �ִٸ� ���� ���� ����
			g_itMGR.OpenDialog( DLG_TYPE_PRIVATESTORE );
			g_itMGR.OpenDialog( DLG_TYPE_ITEM, false );
			actionDone = true;
		}
		break;

	case COMMON_COMMAND_PICK_ITEM_FROM_DISTANCE: //Numenor: this is when you use the "gather" skill. Not when you click on the object, just on the skill
		{
			/// ��ž�¸�忡��.. ����
			//if( g_pAVATAR->GetPetMode() > 0 )
			//	break;

			//-------------------------------------------------------------------------------
			/// �ɱ� ���¿��� ����..
			//-------------------------------------------------------------------------------				
			if( g_pAVATAR->Get_STATE() == CS_SIT )
				break;


			D3DVECTOR vecCenter = g_pAVATAR->Get_CurPOS();
			int iNodeCnt = ::collectBySphere( (float *)( &vecCenter ), 1000 );

			CGameOBJ*	pObj			= NULL;
			HNODE		hNode			= 0;
			HNODE		hUserData		= 0;
			D3DVECTOR	vecTarget;
			int			iItemDistance	= 100000;
			bool		bFindItem		= false;
			CObjITEM*	pFoundItem		= NULL;
			CObjITEM*	pTempItem		= NULL;

			for( int i = 0; i < iNodeCnt; i++ )
			{
				hNode = ::getCollectNode( i );
				hUserData = ::getUserData( hNode );

				if( hUserData )
				{
					pObj = reinterpret_cast< CGameOBJ* >( hUserData );
	    
					/// Item ���� �˻�..
					if( pObj->IsA( OBJ_ITEM ) )
					{		
						pTempItem = (CObjITEM*)pObj;
						DWORD dwPassTIME = g_GameDATA.GetGameTime() - pTempItem->m_dwCreatedTIME;
						if ( pTempItem->m_wOwnerServerObjIDX &&  (int)(pTempItem->m_wRemainTIME -  dwPassTIME) >  62 * 1000 ) 
						{
							// ȹ�� ������ �ִ��� ����...
							if ( pTempItem->m_wOwnerServerObjIDX != g_pObjMGR->Get_ServerObjectIndex(  g_pAVATAR->m_nIndex ) ) 
							{
								//g_itMGR.AppendChatMsg( STR_NOTIFY_02, g_dwBLUE );
								continue;
							}
						}

						vecTarget = pObj->Get_CurPOS();
						int iDistance = CD3DUtil::distance ((int)vecCenter.x, (int)vecCenter.y, (int)vecTarget.x, (int)vecTarget.y);
						if( iDistance < iItemDistance )
						{
							iItemDistance = iDistance;

							bFindItem = true;
							pFoundItem = (CObjITEM*)pObj;
						}							
					}
				}
			}

			if( bFindItem )
			{
				if( pFoundItem )
				{						
					g_pNet->Send_cli_GET_FIELDITEM_REQ(g_pAVATAR ,g_pObjMGR->Get_ServerObjectIndex( pFoundItem->Get_INDEX() ), pSkill->GetSkillIndex());
					g_UserInputSystem.ClearMouseState();	
					actionDone = true;
				}
			}
		}
		break;
	
	case COMMON_COMMAND_PARTY_SCALE: //Numenor: new skill, party with scaling and exp sharing
		{
			int iTargetObjClientIndex = g_UserInputSystem.GetCurrentTarget();
			CObjAVT* pAVT = g_pObjMGR->Get_CharAVT( iTargetObjClientIndex, true );

			if( iTargetObjClientIndex != g_pAVATAR->Get_INDEX() && pAVT )
			{
				if( !g_pTerrain->IsPVPZone())
				{
					if( CParty::GetInstance().HasParty() )
					{
						///��Ƽ �ʴ�
						if( CParty::GetInstance().IsPartyLeader() )
						{
							///��Ƽ Ǯ
							if( CParty::GetInstance().IsPartyFull() ){
								g_itMGR.AppendChatMsg(STR_PARTY_IS_FULL, IT_MGR::CHAT_TYPE_SYSTEM );
							}
							else if( !CParty::GetInstance().IsShareParty() ){
								g_itMGR.AppendChatMsg("You are in a normal party. You cannot invite someone as in a scaled party.", IT_MGR::CHAT_TYPE_SYSTEM );
							}
							else{
								g_pNet->Send_cli_PARTY_REQ(PARTY_REQ_JOIN,  g_pObjMGR->Get_ServerObjectIndex(iTargetObjClientIndex ), true);
								actionDone = true;
							}
						}
						else///Error
						{
							g_itMGR.AppendChatMsg(STR_CANT_INVITE_TO_PARTY, IT_MGR::CHAT_TYPE_SYSTEM );
						}
					}
					else
					{
						g_pNet->Send_cli_PARTY_REQ(PARTY_REQ_MAKE,  g_pObjMGR->Get_ServerObjectIndex(iTargetObjClientIndex), true);
						actionDone = true;
					}
				}
				else
				{
					g_itMGR.OpenMsgBox( "You cannot use a scaled party in PvP!" );
				}
			}
		}
		break;

	} //End of switch

	if(actionDone) g_pAVATAR->Skill_UseAbilityValue(pSkill->GetSkillIndex()); //Numenor: use MP/HP/stamina etc... on the client side

	return true;
}



//----------------------------------------------------------------------
/// Emotion expression command
//----------------------------------------------------------------------

CEmotionCommand::CEmotionCommand()
{
}

CEmotionCommand::~CEmotionCommand()
{
}

bool CEmotionCommand::Execute()
{	
	/// �⺻��ɵ��� �������¿����� �����Ѵ�.
	if( g_pAVATAR->Get_STATE() != CS_STOP && g_pAVATAR->Get_STATE() != CS_SIT )
	{
		g_itMGR.AppendChatMsg( STR_ACTION_COMMAND_STOP_STATE_FAILED, IT_MGR::CHAT_TYPE_SYSTEM);
		return false;
	}

	/// get skill.
	CSkillSlot* pSkillSlot = g_pAVATAR->GetSkillSlot();
	CSkill* pSkill = pSkillSlot->GetSkill( m_iSlotNO );


	/// 
	/// !! 2005/01/10 ������ ��� ������ ��û�Ҷ����� ���Ÿ�Թ�ȣ�� ��û�Ѵ�.
	///


	// ������ ���� ����������...
	//short nWeaponTYPE = WEAPON_MOTION_TYPE( g_pAVATAR->Get_R_WEAPON() );
	int iActionType = SKILL_ANI_ACTION_TYPE( pSkill->GetSkillIndex() );

	///short nFileIDX = FILE_MOTION( nWeaponTYPE, iActionType );	

	if( iActionType != 0 )
		g_pNet->Send_cli_SET_MOTION( iActionType, 1 );

	return true;
}


//----------------------------------------------------------------------
/// Inventory slot command class
//----------------------------------------------------------------------

CInventoryCommand::CInventoryCommand()
{
}

CInventoryCommand::~CInventoryCommand()
{
}

bool CInventoryCommand::Execute()
{
/*	CInventoryDLG* pDlg = g_itMGR.GetAvtInventoryDLG();

	pDlg->UseItem( m_iSlotNO );
*/
	return true;
}


//----------------------------------------------------------------------
/// Normal command class
//----------------------------------------------------------------------

CNormalCommand::CNormalCommand()
{
}

CNormalCommand::~CNormalCommand()
{
}

bool CNormalCommand::Execute()
{

	return true;
}


//----------------------------------------------------------------------
/// Skill command class
//----------------------------------------------------------------------

CSkillCommand::CSkillCommand()
{
	m_iSlotNO = 0;
}

CSkillCommand::~CSkillCommand()
{
}

/// Ÿ���� �ʿ�ġ ���� ��ų��( ���� ���� )
bool CSkillCommand::Execute()
{	
	//-------------------------------------------------------------------------------
	/// �ɱ� ���¿��� ����..
	//-------------------------------------------------------------------------------				
	if( g_pAVATAR->Get_STATE() == CS_SIT )
		return false;

	int iTarget = g_UserInputSystem.GetCurrentTarget();
	CObjCHAR* pObjCHAR = g_pObjMGR->Get_CharOBJ( iTarget, true );
    
	D3DXVECTOR3 vPosTO( 0.0f, 0.0f, 0.0f );

	if( pObjCHAR )
	{
		vPosTO = pObjCHAR->Get_CurPOS();
	}
	// Ȱ�� ��ų �۵�...
	g_SkillManager.FireSkill( m_iSlotNO, g_UserInputSystem.GetCurrentTarget(), vPosTO );
	return true;
}
