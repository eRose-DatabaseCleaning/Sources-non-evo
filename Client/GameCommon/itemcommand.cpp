#include "stdafx.h"
#include "itemcommand.h"
#include "Item.h"
#include "../object.h"
#include "../network/cnetwork.h"
#include "../System/CGame.h"
#include "../interface/it_mgr.h"
#include "../interface/dlgs/chattingdlg.h"
#include "../interface/dlgs/makedlg.h"
#include "../interface/command/ctcmdhotexec.h"
#include "../interface/command/uicommand.h"
#include "../JCommandState.h"
#include "skill.h"
#include "../GameProc/UseItemDelay.h"
#include "../Common/CItem.h"
#include "../IO_Event.h"
#include "../io_terrain.h"
#include "../Country.h"


//Item base command���� �ʿ��� �۷ι� �Լ���
bool IsUnequip( tagBaseITEM* item, int* curr_abilities, const int ability_count )
{
	assert( item );
	assert( curr_abilities );

	if(	item->GetLife() < 1 )/// ������ ���� �������� ����
		return false;

	int iAddType = 0;
	for( short nI = 0; nI < ITEM_NEED_DATA_CNT; ++nI )
	{
		if ( iAddType = ITEM_NEED_DATA_TYPE ( item->m_cType, item->m_nItemNo, nI) ) 
		{
			_ASSERT( iAddType < ability_count );		
			if ( curr_abilities[iAddType] < ITEM_NEED_DATA_VALUE( item->m_cType, item->m_nItemNo , nI) )
				return true;
		}
	}
	return false;
}

void SubAbilities( tagBaseITEM* item, int* curr_abilities, const int ability_count )
{
	assert( item );
	assert( curr_abilities );

	if ( item->GetItemNO() < 1 || item->GetLife() < 1 )		/// �������� ���ų� ������ ���Ѱ��� ���~
		return;

	short nI, nC, nType, nValue;

	// �ɼ�/���� ������ ���ؼ�...
	if ( item->GetGemNO() && ( item->IsAppraisal() || item->HasSocket() ) ) 
	{
		for (nI=0; nI<2; nI++)
		{
			nC = item->GetGemNO();
			nType  = GEMITEM_ADD_DATA_TYPE ( nC, nI );
			nValue = GEMITEM_ADD_DATA_VALUE( nC, nI );
			_ASSERT( nType < ability_count );
			curr_abilities[ nType ] -= nValue;
		}
	}


	for (nI=0; nI<2; nI++) 
	{
		nType = ITEM_NEED_UNION( item->GetTYPE(), item->m_nItemNo, nI);
		if ( nType && ( nType != curr_abilities[AT_UNION] ) )
			continue;

		nType = ITEM_ADD_DATA_TYPE	( item->GetTYPE(), item->m_nItemNo, nI);
		nValue= ITEM_ADD_DATA_VALUE	( item->GetTYPE(), item->m_nItemNo, nI);
		_ASSERT( nType < ability_count );
		curr_abilities[ nType ] -= nValue;
	}
}

void AddAbilities( tagBaseITEM* item, int* curr_abilities, const int ability_count )
{
	assert( item );
	assert( curr_abilities );

	if ( item->GetItemNO() < 1 || item->GetLife() < 1 )		/// �������� ���ų� ������ ���Ѱ��� ���~
		return;

	short nI, nC, nType, nValue;

	// �ɼ�/���� ������ ���ؼ�...
	if ( item->GetGemNO() && ( item->IsAppraisal() || item->HasSocket() ) ) 
	{
		for (nI=0; nI<2; nI++)
		{
			nC = item->GetGemNO();
			nType  = GEMITEM_ADD_DATA_TYPE ( nC, nI );
			nValue = GEMITEM_ADD_DATA_VALUE( nC, nI );
			_ASSERT( nType < ability_count );
			curr_abilities[ nType ] += nValue;
		}
	}


	for (nI=0; nI<2; nI++) 
	{
		nType = ITEM_NEED_UNION( item->GetTYPE(), item->m_nItemNo, nI);
		if ( nType && ( nType != curr_abilities[AT_UNION] ) )
			continue;

		nType = ITEM_ADD_DATA_TYPE	( item->GetTYPE(), item->m_nItemNo, nI);
		nValue= ITEM_ADD_DATA_VALUE	( item->GetTYPE(), item->m_nItemNo, nI);
		_ASSERT( nType < ability_count );
		curr_abilities[ nType ] += nValue;
	}
}
//-------------------------------------------------------------------------------------------------------------------

bool CTCmdItemEquipInInventory::Exec( CTObject* pObj )
{
	if( g_pAVATAR->GetPetMode() >= 0 )///����̺� ��ų ������̶��
	{
		g_itMGR.AppendChatMsg(STR_CANT_EQUIP_USING_DRIVESKILL, IT_MGR::CHAT_TYPE_SYSTEM );
		return true;
	}

	if( pObj == NULL )
	{
		assert( pObj && "Arg is Null @CTCmdItemEquipInventory::Exec");
		return true;
	}

	CItem* pItem = (CItem*)pObj;
	tagITEM& Item = pItem->GetItem();

	if( Item.IsEmpty() )
	{
		assert( 0 && "Item is empty @CTCmdItemEquipInInventory::Exec" );
		return true;
	}

	short nEquipIDX = 0;

	switch( Item.GetTYPE() )
	{
		case ITEM_TYPE_FACE_ITEM:
			nEquipIDX = EQUIP_IDX_FACE_ITEM;
			break;
		case ITEM_TYPE_HELMET:
			nEquipIDX = EQUIP_IDX_HELMET;
			break;
		case ITEM_TYPE_ARMOR:
			nEquipIDX = EQUIP_IDX_ARMOR;
			break;
		case ITEM_TYPE_KNAPSACK:
			nEquipIDX = EQUIP_IDX_KNAPSACK;
			break;
		case ITEM_TYPE_GAUNTLET:
			nEquipIDX = EQUIP_IDX_GAUNTLET;
			break;
		case ITEM_TYPE_BOOTS:
			nEquipIDX = EQUIP_IDX_BOOTS;
			break;
		case ITEM_TYPE_WEAPON:
			nEquipIDX = EQUIP_IDX_WEAPON_R;	
			break;
		case ITEM_TYPE_SUBWPN:
			nEquipIDX = EQUIP_IDX_WEAPON_L;
			break;
		case ITEM_TYPE_JEWEL:
			{
				switch( ITEM_TYPE(ITEM_TYPE_JEWEL, Item.GetItemNO() ))
				{
				case 171:///����
					nEquipIDX = EQUIP_IDX_RING;
					break;
				case 172:///�����
					nEquipIDX = EQUIP_IDX_NECKLACE;
					break;
				case 173:///�Ͱ���
					nEquipIDX = EQUIP_IDX_EARRING;
					break;
				default:
					assert( 0 && " Unknown Item Type @CItemEquip::Excute" );
					break;
				}
			}
			break;

		/// ��� �������� �ƴϴ�.
		default:
			return true;		
	}

	///���� ���� üũ 
	if( g_pAVATAR->Check_EquipCondition ( Item ) )
	{
		CInventory* pInven = g_pAVATAR->GetInventory();

		bool b = Item.IsTwoHands();
		int i  = pInven->GetEmptyInventory( 0 );

		int j  = g_pAVATAR->Get_R_WEAPON();
		int k  = g_pAVATAR->Get_L_WEAPON();
		
		tagITEM equiped_item = g_pAVATAR->Get_EquipITEM( EQUIP_IDX_WEAPON_R );

		if( Item.IsTwoHands() &&  pInven->GetEmptyInventory( 0 ) == - 1 && g_pAVATAR->Get_L_WEAPON() && g_pAVATAR->Get_R_WEAPON() )
			g_itMGR.AppendChatMsg( STR_NOT_ENOUGH_EQUIP_SPACE, IT_MGR::CHAT_TYPE_SYSTEM );
		else if( !equiped_item.IsEmpty() && equiped_item.IsTwoHands() && Item.GetTYPE() == ITEM_TYPE_SUBWPN )
			g_itMGR.AppendChatMsg( STR_EQUIPED_TWO_HAND_WEAPON, IT_MGR::CHAT_TYPE_SYSTEM );
		else
		{
			tagITEM& Item = pItem->GetItem();
			if( !Item.IsEmpty() )
			{
				int			curr_abilities[ AT_MAX ];			///���� �ƹ�Ÿ�� �ɷ�ġ �ӽ� ���� ����
				tagITEM		equiped_items[ MAX_EQUIP_IDX ];		///���� �ƹ�Ÿ�� ������� ���� ���� ����
				std::list< int > removeitem_indexlist;			///������ �������� ��û�� ���������� �κ��丮 �ε��� ����Ʈ

				
				for( int index = 0; index < AT_MAX; ++index )
					curr_abilities[ index ] = g_pAVATAR->GetCur_AbilityValue( index );
				
				
				for( int index = 1; index < MAX_EQUIP_IDX; ++index )
					equiped_items[index] = g_pAVATAR->m_Inventory.m_ItemEQUIP[ index ];
				
				//equiped_items[ nEquipIDX ] = Item;

				if( !equiped_items[ nEquipIDX ].IsEmpty() )
					SubAbilities( &(equiped_items[ nEquipIDX ]), curr_abilities, AT_MAX );

				AddAbilities( &Item, curr_abilities, AT_MAX );
				equiped_items[ nEquipIDX ] = Item;

				for( int index = 1; index < MAX_EQUIP_IDX; ++index )
				{
					if( !equiped_items[index].IsEmpty() )
					{
						if( IsUnequip( &equiped_items[index], curr_abilities, AT_MAX ) )
						{
							SubAbilities( &equiped_items[index], curr_abilities, AT_MAX );
							removeitem_indexlist.push_back( index );
							ZeroMemory( &equiped_items[ index ], sizeof( tagITEM ) );
							index = 0;
						}
					}
				}
				
				if( g_pAVATAR->m_Inventory.GetEmptyInvenSlotCount(INV_WEAPON) < (int)removeitem_indexlist.size() )
				{
					g_itMGR.OpenMsgBox( STR_NOT_ENOUGH_INVENTORY_SPACE );
				}
				else
				{
					g_pNet->Send_cli_EQUIP_ITEM( nEquipIDX, pItem->GetIndex() );

					std::list< int >::iterator iter; 
					for( iter = removeitem_indexlist.begin(); iter != removeitem_indexlist.end(); ++iter )
						g_pNet->Send_cli_EQUIP_ITEM( *iter , 0 );		
				}
			}
		}
	}
	else
		g_itMGR.AppendChatMsg( STR_NOTIFY_06, IT_MGR::CHAT_TYPE_SYSTEM );


	return true;
}

bool CTCmdItemUseInInventory::Exec( CTObject* pObj )
{
	if( pObj == NULL )
	{
		assert( pObj && "Arg is Null @CTCmdItemUseInInventory::Exec");
		return true;
	}

	CItem* pItem = (CItem*)pObj;
	
	tagITEM& Item = pItem->GetItem();


	if( Item.IsEmpty() )
	{
		assert( 0 && "Item is Empty @CTCmdItemUseInInventory::Exec");	
		return true;
	}



	DWORD dwNowTime = timeGetTime();
	///�����ð����� ��� ����
//	m_LastUseTime = dwNowTime;

	int iType = USEITEM_NEED_DATA_TYPE( Item.GetItemNO() );
	if( iType == AT_CURRENT_PLANET )///�༺ �����ϰ��
	{
		int iPlanetNo = USEITEM_NEED_DATA_VALUE(  Item.GetItemNO() );
		if( iPlanetNo != ZONE_PLANET_NO( g_pTerrain->GetZoneNO() ) )
		{
			g_itMGR.AppendChatMsg( STR_CANTUSE_WARPSCROLL_CURRENTZONE, IT_MGR::CHAT_TYPE_SYSTEM );
			return true;
		}
	}


	switch( ITEM_TYPE( Item.GetTYPE(), Item.GetItemNO()  ) )
	{
	///������ ������
	case USE_ITEM_REPAIR_ITEM:
		CGame::GetInstance().BeginRepair( CGame::REPAIR_ITEM );
		CGame::GetInstance().SetUsingRepairItemInvenIdx( pItem->GetIndex() );
		return true;
		break;
	/// ����å
	case USE_ITEM_SKILL_LEARN:
		{
			/// 2004 / 2 / 11 : nAvy - ���������̶�� ���ǾƷ�
			BYTE btT = g_pAVATAR->Skill_LearnCondition( USEITEM_SCROLL_LEARN_SKILL( Item.GetItemNO()) );						

			if( btT != RESULT_SKILL_LEARN_SUCCESS )
			{
				switch( btT )
				{
				case RESULT_SKILL_LEARN_FAILED :		// ���� ����.	
					g_itMGR.AppendChatMsg( STR_LEARN_SKILL_FAILED, IT_MGR::CHAT_TYPE_SYSTEM );
					break;
				case RESULT_SKILL_LEARN_NEED_JOB : 		// ������ ��ġ���� �ʴ´�.		
					g_itMGR.AppendChatMsg( STR_LEARN_SKILL_JOB_FAILED, IT_MGR::CHAT_TYPE_SYSTEM );
					break;
				case RESULT_SKILL_LEARN_NEED_SKILL :	// ������ ��ų�� �ʿ��Ѵ�.		
					g_itMGR.AppendChatMsg( STR_LEARN_SKILL_NEED_PRESKILL_FAILED, IT_MGR::CHAT_TYPE_SYSTEM );
					break;
				case RESULT_SKILL_LEARN_NEED_ABILITY :	// �ɷ�ġ�� �����ϴ�		
					g_itMGR.AppendChatMsg( STR_LEARN_SKILL_NEED_ABILITY_FAILED, IT_MGR::CHAT_TYPE_SYSTEM );
					break;
				case RESULT_SKILL_LEARN_OUTOFSLOT :		// ���̻� ��ų�� ���� ����.		
					g_itMGR.AppendChatMsg( STR_LEARN_SKILL_SLOT_FULL_FAILED, IT_MGR::CHAT_TYPE_SYSTEM );
					break;
				case RESULT_SKILL_LEARN_INVALID_SKILL :	// �߸��� ��ų��ȣ �Դϴ�.		
					g_itMGR.AppendChatMsg( STR_LEARN_SKILL_INVALID_SKILL_FAILED, IT_MGR::CHAT_TYPE_SYSTEM );
					break;
				case RESULT_SKILL_LEARN_OUTOFPOINT ://	0x07   // ��ų ����Ʈ ����
					g_itMGR.AppendChatMsg( STR_NOT_ENOUGH_SKILLPOINT, IT_MGR::CHAT_TYPE_SYSTEM );
					break;
				default:
					break;
				}
				return true;
			}
			break;
		}
	/// ��ũ�� ����
	case USE_ITEM_SKILL_DOING:
		{	
			if( g_pAVATAR->GetPetMode() > 0 )
			{
				g_itMGR.AppendChatMsg( STR_CANT_USESKILL_USING_DRIVESKILL,  IT_MGR::CHAT_TYPE_SYSTEM);
				return false;
			}

			int iSkillIDX		= USEITEM_SCROLL_USE_SKILL( Item.GetItemNO() );
			int iCurrentTarget	= g_UserInputSystem.GetCurrentTarget();				
	
			if( SKILL_TYPE( iSkillIDX ) == SKILL_TYPE_18 )///warp�� scroll�ϰ��
			{
				if( SKILL_WARP_PLANET_NO( iSkillIDX ) != ZONE_PLANET_NO( g_pTerrain->GetZoneNO() ) )
				{
					g_itMGR.AppendChatMsg( STR_CANTUSE_WARPSCROLL_CURRENTZONE, IT_MGR::CHAT_TYPE_SYSTEM );
					return true;
				}

				///<-MP üũ 
				int	iRequireMp = (int)(g_pAVATAR->GetCur_WEIGHT() * 0.05);
				if( g_pAVATAR->Get_AbilityValue( AT_MP ) < iRequireMp )
				{
					g_itMGR.AppendChatMsg( STR_NOT_ENOUGH_CONDITION, IT_MGR::CHAT_TYPE_SYSTEM );
					return true;
				}
			}

			//----------------------------------------------------------------------------------------------
			/// Emotion skill �ϰ��
			/// �ؾ��� ������� �����ؼ� ������ ��û�Ѵ�.
			if( SKILL_TYPE( iSkillIDX ) == SKILL_TYPE_16 )
			{
				/// �⺻��ɵ��� �������¿����� �����Ѵ�.
				if( g_pAVATAR->Get_STATE() != CS_STOP && g_pAVATAR->Get_STATE() != CS_SIT )
				{
					g_itMGR.AppendChatMsg( STR_ACTION_COMMAND_STOP_STATE_FAILED, IT_MGR::CHAT_TYPE_SYSTEM);
					return false;
				}

				int iActionType = SKILL_ANI_ACTION_TYPE( iSkillIDX );
				if( iActionType != 0 )
				{
					g_pNet->Send_cli_SET_MOTION( iActionType, 1 );					
				}
			}
			//----------------------------------------------------------------------------------------------

			
			if( CSkillManager::CheckConditionForFireSkill( iSkillIDX, iCurrentTarget ) == false )
			{
				g_itMGR.AppendChatMsg( STR_NOT_ENOUGH_CONDITION, IT_MGR::CHAT_TYPE_SYSTEM );
				return true;
			}

			/// ��ũ�ѷ� ����ϴ� ��ų�� �����Ÿ� üũ
			int iTargetDistance = SKILL_DISTANCE( iSkillIDX );
			CObjCHAR* pTarget = g_pObjMGR->Get_CharOBJ( iCurrentTarget, true );

			if( SKILL_TYPE( iSkillIDX ) != SKILL_TYPE_18 )
			{
				/// iTargetDistance �� 0 �̸� üũ���� �ʴ´�.
				if( iTargetDistance && pTarget && !g_pAVATAR->IsInRANGE( pTarget, iTargetDistance ) )
				{
					g_itMGR.AppendChatMsg( STR_NOT_ENOUGH_CONDITION, IT_MGR::CHAT_TYPE_SYSTEM );
					return true;
				}
			}

		}
		break;			

	case USE_ITEM_EVENT_ITEM:
		{
			g_pEventLIST->Run_EVENT( g_pAVATAR->Get_INDEX(), USEITEM_CONFILE_IDX( Item.GetItemNO() ), -1, true );
		}
		break;
	}

	//----------------------------------------------------------------------------------------------------
	/// Use Item Delay ���� ��
	/// Use Item Delay ����
	//----------------------------------------------------------------------------------------------------
	
	// ���� �������� New Delay type�� ���� �Ҽ� ����. 
	// ���� �������� ������ Ÿ���� �����´�. 
	if(CCountry::GetSingleton().IsUseItemDelayNewVersion())
	{
		short iItem = Item.GetItemNO();
       int iDelayType	 = USEITME_DELAYTIME_TYPE( iItem );
		float iDelayTick  = (float)(USEITME_DELAYTIME_TICK(iItem)) * 1000;

		//0 �� �ƴϸ� �����̸� �����Ѵ�. 
		if( iDelayType )
		{
			//������ Ÿ���� �־ ���� Ÿ���� ������ ���̶�� ����Ҽ� ���ٴ� 
			//�޼����� ä��â�� ����Ѵ�. 
			if( g_UseItemDelay.GetUseItemDelay( iDelayType ) > 0.0f)
			{
				///Use item delay �� �����Ǿ� �ִٸ� �н�..
				g_itMGR.AppendChatMsg( STR_CANT_DOUBLE_USE, IT_MGR::CHAT_TYPE_SYSTEM );
				return false;
			}
		}
		//�����϶� �ܵ����� ó���Ѵ�. 
		else
		{
			//�ڱ� �ڽ��� ������ �� 
			if(g_SoloUseItemDelayTick.GetUseItemDelay( iItem ) > 0.0f)
			{
				g_itMGR.AppendChatMsg( STR_CANT_DOUBLE_USE, IT_MGR::CHAT_TYPE_SYSTEM );
				return false;
			}
		}
	}
	else
	{
		// ���� ��ȣ�� �����Ǿ� �ִٸ�..
		if( USEITME_STATUS_STB( Item.GetItemNO() ) )
		{
			switch( USEITME_STATUS_STB( Item.GetItemNO() ) )
			{
			case 1:
			case 2:
			case 3:
				if( g_UseItemDelay.GetUseItemDelay( USE_ITEM_HP ) )
				{
					///Use item delay �� �����Ǿ� �ִٸ� �н�..
					g_itMGR.AppendChatMsg( STR_CANT_DOUBLE_USE, IT_MGR::CHAT_TYPE_SYSTEM );
					return false;
				}
				g_UseItemDelay.SetUseItemDelay( USE_ITEM_HP, DEFAULT_HP_ITEM_DELAY );
				break;
			case 4:
			case 5:
			case 6:
				if( g_UseItemDelay.GetUseItemDelay( USE_ITEM_MP ) )
				{
					///Use item delay �� �����Ǿ� �ִٸ� �н�..
					g_itMGR.AppendChatMsg( STR_CANT_DOUBLE_USE, IT_MGR::CHAT_TYPE_SYSTEM );
					return false;
				}

				g_UseItemDelay.SetUseItemDelay( USE_ITEM_MP, DEFAULT_MP_ITEM_DELAY );
				break;
			default:
				{
					if( g_UseItemDelay.GetUseItemDelay( USE_ITEM_OTHERS ) )
					{
						///Use item delay �� �����Ǿ� �ִٸ� �н�..
						g_itMGR.AppendChatMsg( STR_CANT_DOUBLE_USE, IT_MGR::CHAT_TYPE_SYSTEM );
						return false;
					}
					g_UseItemDelay.SetUseItemDelay( USE_ITEM_OTHERS, DEFAULT_OTHER_ITEM_DELAY );
				}
				break;
			}
		}
		else
		{
			/// ��ũ�� ��ų�ϰ��
			if( ITEM_TYPE( Item.GetTYPE(), Item.GetItemNO()  ) == USE_ITEM_SKILL_DOING )
			{

				if( g_UseItemDelay.GetUseItemDelay( USE_ITEM_SCROLL ) )
				{
					///Use item delay �� �����Ǿ� �ִٸ� �н�..
					g_itMGR.AppendChatMsg( STR_CANT_DOUBLE_USE, IT_MGR::CHAT_TYPE_SYSTEM );
					return false;
				}

				g_UseItemDelay.SetUseItemDelay( USE_ITEM_SCROLL, DEFAULT_USE_SCROLL_DELAY );

			}else
			{
				// �ٸ� ��� ������ Use Item

				if( g_UseItemDelay.GetUseItemDelay( USE_ITEM_OTHERS ) )
				{
					///Use item delay �� �����Ǿ� �ִٸ� �н�..
					g_itMGR.AppendChatMsg( STR_CANT_DOUBLE_USE, IT_MGR::CHAT_TYPE_SYSTEM );
					return false;
				}

				g_UseItemDelay.SetUseItemDelay( USE_ITEM_OTHERS, DEFAULT_OTHER_ITEM_DELAY );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------


	//���?

	//--------------------------------------------------------------------------------------------------
	/// Target skill �ϰ��� ��Ŷ�ڿ� Ÿ�� ��ȣ�� ���δ�.
	//--------------------------------------------------------------------------------------------------
	bool bTargetSkill = false;
	if( ITEM_TYPE( Item.GetTYPE(), Item.GetItemNO()  ) == USE_ITEM_SKILL_DOING )
	{
		int iSkillIDX		= USEITEM_SCROLL_USE_SKILL( Item.GetItemNO() );
		if( SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_FIRE_BULLET ||
			SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_TARGET_BOUND_DURATION ||
			SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_TARGET_BOUND ||
			SKILL_TYPE( iSkillIDX ) == SKILL_ACTION_TARGET_STATE_DURATION )
			bTargetSkill = true;
	}

	if ( g_pNet->Send_cli_USE_ITEM( pItem->GetIndex(), bTargetSkill, g_UserInputSystem.GetCurrentTarget() ) ) 
		g_pSoundLIST->IDX_PlaySound( USEITEM_USE_SOUND( Item.m_nItemNo_1) );
	
	return true;
}

bool CTCmdItemEtcInInventory::Exec( CTObject* pObj )
{

	if( pObj == NULL )
	{
		assert( pObj && "Arg is Null @CTCmdItemUseInInventory::Exec");
		return true;
	}



	CItem* pItem = (CItem*)pObj;
	tagITEM& Item = pItem->GetItem();

	if( Item.IsEmpty() )
	{
		assert( 0 && "Item is Empty @CTCmdTemEtcInInventory::Exec");
		return true;
	}

	switch( Item.GetTYPE() )
	{
		case ITEM_TYPE_NATURAL:
			{
				CMakeDLG* pDlg = (CMakeDLG*)g_itMGR.FindDlg( DLG_TYPE_MAKE );
				if( pDlg && pDlg->IsVision() )///����â�� ���� �ִµ��ȿ��� �Ҹ�ź �����Ұ�
				{
					//CTCmdTakeInItem2MakeDlg* pCmd = new CTCmdTakeInItem2MakeDlg( pItem->GetIndex() );
					//g_itMGR.AddTCommand( DLG_TYPE_MAKE, pCmd );
				}
				else if( NATURAL_BULLET_NO(Item.GetItemNO() ) )
				{
					if( g_pAVATAR->GetPetMode() >= 0 )///����̺� ��ų ������̶��
					{
						g_itMGR.AppendChatMsg(STR_CANT_EQUIP_USING_DRIVESKILL, IT_MGR::CHAT_TYPE_SYSTEM );
						return true;
					}
					t_eSHOT type = tagITEM::GetNaturalBulletType( Item.GetItemNO() );
				
					if( type != MAX_SHOT_TYPE )
						g_pNet->Send_cli_SET_BULLET( type, pItem->GetIndex() );
				}
			}break;
		case ITEM_TYPE_GEM:
			{

			}
			break;
	}
	return true;
}


bool CTCmdItemEquiped::Exec( CTObject* pObj )
{
	if( g_pAVATAR->GetPetMode() >= 0 )///����̺� ��ų ������̶��
	{
		g_itMGR.AppendChatMsg(STR_CANT_UNEQUIP_USING_DRIVESKILL, IT_MGR::CHAT_TYPE_SYSTEM );
		return true;
	}

	if( pObj == NULL )
	{
		assert( pObj && "Arg is Null @CTCmdItemEquiped::Exec");
		return true;
	}

	CItem* pItem = ( CItem*)pObj;
	///����
	tagITEM& Item = pItem->GetItem();
	if( !Item.IsEmpty() )
	{
		
		int			curr_abilities[ AT_MAX ];			///���� �ƹ�Ÿ�� �ɷ�ġ �ӽ� ���� ����
		tagITEM		equiped_items[ MAX_EQUIP_IDX ];		///���� �ƹ�Ÿ�� ������� ���� ���� ����
		std::list< int > removeitem_indexlist;			///������ �������� ��û�� ���������� �κ��丮 �ε��� ����Ʈ

		
		for( int index = 0; index < AT_MAX; ++index )
			curr_abilities[ index ] = g_pAVATAR->GetCur_AbilityValue( index );
		
		
		for( int index = 1; index < MAX_EQUIP_IDX; ++index )
			equiped_items[index] = g_pAVATAR->m_Inventory.m_ItemEQUIP[ index ];
		
		
		SubAbilities( &(pItem->GetItem()), curr_abilities, AT_MAX );
		removeitem_indexlist.push_back( pItem->GetIndex() );
		ZeroMemory( &equiped_items[ pItem->GetIndex() ], sizeof( tagITEM ) );


		for( int index = 1; index < MAX_EQUIP_IDX; ++index )
		{
			if( !equiped_items[index].IsEmpty() )
			{
				if( IsUnequip( &equiped_items[index], curr_abilities, AT_MAX ) )
				{
					SubAbilities( &equiped_items[index], curr_abilities, AT_MAX );
					removeitem_indexlist.push_back( index );
					ZeroMemory( &equiped_items[ index ], sizeof( tagITEM ) );
					index = 0;
				}
			}
		}
		
		if( g_pAVATAR->m_Inventory.GetEmptyInvenSlotCount(INV_WEAPON) < (int)removeitem_indexlist.size() )
		{
			g_itMGR.OpenMsgBox( STR_NOT_ENOUGH_INVENTORY_SPACE );
		}
		else
		{
			std::list< int >::iterator iter; 
			for( iter = removeitem_indexlist.begin(); iter != removeitem_indexlist.end(); ++iter )
				g_pNet->Send_cli_EQUIP_ITEM( *iter , 0 );		
		}
	}

	return true;
}



bool CTCmdItemBullet::Exec( CTObject* pObj )
{
	if( pObj == NULL )
	{
		assert( 0 && "Arg is Null @CTCmdItemBullet::Exec");
		return true;
	}

	CItem* pItem = ( CItem*)pObj;
	tagITEM& Item = pItem->GetItem();
	if( !Item.IsEmpty() )
	{
		if( g_pAVATAR->GetPetMode() >= 0 )///����̺� ��ų ������̶��
		{
			g_itMGR.AppendChatMsg(STR_CANT_UNEQUIP_BULLET_USING_DRIVESKILL, IT_MGR::CHAT_TYPE_SYSTEM );
			return true;
		}

		t_eSHOT ShotType = Item.GetBulletType();
		if( ShotType != MAX_SHOT_TYPE )
			g_pNet->Send_cli_SET_BULLET( ShotType, 0 );
	}
	return true;
}
