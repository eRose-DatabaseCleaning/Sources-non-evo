#ifndef _TYPERESOURCE_
#define _TYPERESOURCE_

#include <map>
#include <string>

#include "..\\Common\\IO_STB.h"
#include "../GameCommon/StringManager.h"

// Resource String
#define RES_MINIMAP_ARROW			"3DData\\Control\\Res\\MiniMap_Arrow.tga"





#define LIST_STRING(I) CStringManager::GetSingleton().GetString( I )

#define STR_RESULT_SELECT_SERVER_FAILED					CStringManager::GetSingleton().GetString( 5 )
#define	STR_RESULT_SELECT_SERVER_FULL					CStringManager::GetSingleton().GetString( 6 )
#define	STR_RESULT_SELECT_SERVER_INVALID_CHANNEL		CStringManager::GetSingleton().GetString( 7 )
#define	STR_RESULT_SELECT_SERVER_CHANNEL_NOT_ACTIVE		CStringManager::GetSingleton().GetString( 8 )
#define	STR_RESUTL_SELECT_SERVER_INVALID_AGE			CStringManager::GetSingleton().GetString( 9 )
#define STR_CANT_ADDITEM2BUYLIST						CStringManager::GetSingleton().GetString( 10 )
#define STR_FILTER_BADWORD								CStringManager::GetSingleton().GetString( 11 )
#define STR_MSG_IDENTIFY_COST							CStringManager::GetSingleton().GetString( 12 )
#define STR_NOTENOUGH_IDENTIFYCOST						CStringManager::GetSingleton().GetString( 13 )
#define STR_TIMEOUT										CStringManager::GetSingleton().GetString( 14 )
#define STR_MINUTE										CStringManager::GetSingleton().GetString( 15 )
#define STR_CANT_SEPARATE								CStringManager::GetSingleton().GetString( 16 )
#define STR_NOTSELECT_MAKEITEM							CStringManager::GetSingleton().GetString( 17 )
#define STR_NEED_ABILITY								CStringManager::GetSingleton().GetString( 18 )
#define STR_CRAFT_UPGRADE_SUCCESS						CStringManager::GetSingleton().GetString( 19 )
#define STR_CRAFT_UPGRADE_FAILED						CStringManager::GetSingleton().GetString( 20 )
#define STR_ITEM										CStringManager::GetSingleton().GetString( 21 )
#define STR_AVATARINFO									CStringManager::GetSingleton().GetString( 22 )
#define STR_SKILL										CStringManager::GetSingleton().GetString( 23 )
#define STR_QUEST										CStringManager::GetSingleton().GetString( 24 )
#define STR_COMMUNITY									CStringManager::GetSingleton().GetString( 25 )
#define STR_OPTION										CStringManager::GetSingleton().GetString( 26 )
#define STR_PARTY										CStringManager::GetSingleton().GetString( 27 )
#define STR_FAIL_IDENTIFY								CStringManager::GetSingleton().GetString( 28 )
#define STR_CANT_STORE_MONEY2BANK						CStringManager::GetSingleton().GetString( 29 )
#define STR_HOUR										CStringManager::GetSingleton().GetString( 30 )
#define STR_MONEY										CStringManager::GetSingleton().GetString( 31 )
#define STR_QUERY_SELLITEM2PRIVATESTORE					CStringManager::GetSingleton().GetString( 32 )
#define STR_QUERY_BUYITEMFROMPRIVATESTORE				CStringManager::GetSingleton().GetString( 33 )
#define STR_REQUIRE_SUMMONQUANTITY						CStringManager::GetSingleton().GetString( 34 )
#define STR_TOTAL_TRADEMONEY_PRIVATESTORE				CStringManager::GetSingleton().GetString( 35 )
#define STR_CLAN										CStringManager::GetSingleton().GetString( 36 )







#define STR_TOO_NEAR_NPC								CStringManager::GetSingleton().GetString( 40 )
#define STR_RESULT_LOGIN_REPLY_NO_REAL_NAME				CStringManager::GetSingleton().GetString( 41 )
#define	STR_NOT_ENOUGH_MONEY_PRIVATESTORE				CStringManager::GetSingleton().GetString( 42 )
#define	STR_CANTUSE_WARPSCROLL_CURRENTZONE				CStringManager::GetSingleton().GetString( 43 )


#define STR_CLAN_NAME									CStringManager::GetSingleton().GetString( 44 )
#define STR_CLAN_LEV									CStringManager::GetSingleton().GetString( 45 )
#define STR_CLAN_POINT									CStringManager::GetSingleton().GetString( 46 )
#define STR_CLAN_SLOGAN									CStringManager::GetSingleton().GetString( 47 )
#define STR_CLAN_MONEY									CStringManager::GetSingleton().GetString( 48 )
#define STR_CLAN_ALLYED									CStringManager::GetSingleton().GetString( 49 )
#define STR_CLAN_MEMBER_CLASS							CStringManager::GetSingleton().GetString( 50 )
#define STR_CLAN_MEMBER_CONTRIB							CStringManager::GetSingleton().GetString( 51 )
#define STR_CHANGE_TRADEITEM_IN_MYREADY					CStringManager::GetSingleton().GetString( 52 )///1:1 �ŷ��� ���� Ȯ�� ��ư�� ���� ���¿��� ������ ��ǰ�� ����Ǿ���.
#define STR_CLAN_MEMBER_COUNT							CStringManager::GetSingleton().GetString( 53 )
#define STR_CLAN_CANT_PROMOTE							CStringManager::GetSingleton().GetString( 54 )
#define STR_CLAN_ORGANIZED								CStringManager::GetSingleton().GetString( 55 )	
#define STR_CLAN_RESULT_CLAN_JOIN_OK					CStringManager::GetSingleton().GetString( 56 )	
#define STR_CLAN_RESULT_CLAN_DESTROY_OK					CStringManager::GetSingleton().GetString( 57 )	
#define STR_CLAN_RESULT_CLAN_CREATE_FAILED				CStringManager::GetSingleton().GetString( 58 )	
#define STR_CLAN_RESULT_CLAN_CREATE_DUP_NAME			CStringManager::GetSingleton().GetString( 59 )	
#define STR_CLAN_RESULT_CLAN_CREATE_NO_RIGHT			CStringManager::GetSingleton().GetString( 60 )	
#define STR_CLAN_RESULT_CLAN_DESTROYED					CStringManager::GetSingleton().GetString( 61 )	
#define STR_CLAN_RESULT_CLAN_DESTROY_FAILED				CStringManager::GetSingleton().GetString( 62 )	
#define STR_CLAN_RESULT_CLAN_DESTROY_NO_RIGHT			CStringManager::GetSingleton().GetString( 63 )	
#define STR_CLAN_RESULT_CLAN_JOIN						CStringManager::GetSingleton().GetString( 64 )	
#define STR_CLAN_RESULT_CLAN_JOIN_FAILED				CStringManager::GetSingleton().GetString( 65 )	
#define STR_CLAN_RESULT_CLAN_JOIN_NO_RIGHT				CStringManager::GetSingleton().GetString( 66 )	
#define STR_CLAN_RESULT_CLAN_JOIN_HAS_CLAN				CStringManager::GetSingleton().GetString( 67 )	
#define STR_CLAN_RESULT_CLAN_KICK						CStringManager::GetSingleton().GetString( 68 )	
#define STR_CLAN_RESULT_CLAN_KICK_ME					CStringManager::GetSingleton().GetString( 69 )	
#define STR_CLAN_RESULT_CLAN_QUIT						CStringManager::GetSingleton().GetString( 70 )	
#define STR_CLAN_GCMD_INVITE_REQ						CStringManager::GetSingleton().GetString( 71 )	
#define STR_CLAN_GCMD_INVITE_REPLY_NO					CStringManager::GetSingleton().GetString( 72 )	
#define STR_CLAN_RESULT_CLAN_POSITION					CStringManager::GetSingleton().GetString( 73 )	
#define STR_CLAN_CANT_DELETE_CLANMASTER					CStringManager::GetSingleton().GetString( 74 )	
#define STR_CLAN_CANT_QUIT_CLANMASTER					CStringManager::GetSingleton().GetString( 75 )	
#define STR_CLAN_NO_RIGHT								CStringManager::GetSingleton().GetString( 76 )	
#define STR_CLAN_RESULT_CLAN_CREATE_NO_CONDITION		CStringManager::GetSingleton().GetString( 77 )	
#define STR_CLAN_INPUT_SLOGAN							CStringManager::GetSingleton().GetString( 78 )	

#define STR_SKILLPOWER_EFFECT_0							CStringManager::GetSingleton().GetString( 80 )	///���Ŀ� ���� ��ų�Ŀ��� ������ ��ġ�� ��
#define STR_SKILLPOWER_EFFECT_1							CStringManager::GetSingleton().GetString( 81 )	
#define STR_SKILLPOWER_EFFECT_2							CStringManager::GetSingleton().GetString( 82 )	
#define STR_SKILLPOWER_EFFECT_3							CStringManager::GetSingleton().GetString( 83 )	

#define STR_CLAN_CANT_ENTRUST_TO_LOW_CLASS				CStringManager::GetSingleton().GetString( 90 )	
#define STR_CLAN_QUERY_ENTRUST							CStringManager::GetSingleton().GetString( 91 )	
#define STR_CLAN_QUERY_BAN								CStringManager::GetSingleton().GetString( 92 )	
#define STR_CLAN_QUERY_PROMOTE							CStringManager::GetSingleton().GetString( 93 )	
#define STR_CLAN_QUERY_DEMOTE							CStringManager::GetSingleton().GetString( 94 )	
#define STR_CLAN_QUERY_DISORGANIZE						CStringManager::GetSingleton().GetString( 95 )	
#define STR_CLAN_CANT_MORE_JOINMEMBER					CStringManager::GetSingleton().GetString( 96 )	
#define STR_CLAN_CANT_DISORGANIZE_MEMBERCOUNT			CStringManager::GetSingleton().GetString( 97 )	
#define STR_CLAN_CREATE_CONDITION						CStringManager::GetSingleton().GetString( 98 )	

////////////////////////////////////////////////////////////////////////////////////
///  ���� ��Ʈ�� ������
////////////////////////////////////////////////////////////////////////////////////
#define	STR_ITEM_TYPE						CStringManager::GetSingleton().GetString( 106 )
#define STR_WEIGHT							CStringManager::GetSingleton().GetString( 107 )			//����
#define STR_ADD_ABILITY						CStringManager::GetSingleton().GetString( 108 )			//�߰��ɷ�
#define STR_EQUIP_CONDITION					CStringManager::GetSingleton().GetString( 109 )			//��������
#define STR_ATTACK_RANGE					CStringManager::GetSingleton().GetString( 110 )			//���ݰŸ�
#define STR_ATTACH							CStringManager::GetSingleton().GetString( 111 )			//����	
#define STR_DEAL_AT_STORE					CStringManager::GetSingleton().GetString( 112 )			//�����ŷ�..		
#define STR_BUY_CATALOG						CStringManager::GetSingleton().GetString( 113 )			//���Ը��
#define STR_SELL_CATALOG					CStringManager::GetSingleton().GetString( 114 )			//�ǸŸ��
#define STR_TRADE_PVP						CStringManager::GetSingleton().GetString( 115 )			//1:1�ŷ�
#define STR_STATUS_DLG						CStringManager::GetSingleton().GetString( 116 )			//����â
#define STR_INVENTORY_DLG					CStringManager::GetSingleton().GetString( 117 )			//�κ��丮â
#define STR_EQUIP_DLG						CStringManager::GetSingleton().GetString( 118 )			//���â
#define STR_SKILL_DLG						CStringManager::GetSingleton().GetString( 119 )			//���â
#define STR_QUEST_DLG						CStringManager::GetSingleton().GetString( 120 )			//����Ʈâ
#define STR_MOVE_QUICKSLOT_DLG				CStringManager::GetSingleton().GetString( 121 )			//���ٷ� ������
#define STR_MOVE_INVENTORY_DLG				CStringManager::GetSingleton().GetString( 122 )			//�κ��丮â���� ������
#define STR_MOVE_EQUIP_DLG					CStringManager::GetSingleton().GetString( 123 )			//���â���� ������
#define STR_STORE							CStringManager::GetSingleton().GetString( 124 )			//����
#define STR_QUALITY							CStringManager::GetSingleton().GetString( 125 )			//ǰ��
#define STR_HPRECOVERY						CStringManager::GetSingleton().GetString( 126 )			//HPȸ��
#define STR_NOT_ENOUGH_MANA					CStringManager::GetSingleton().GetString( 127 )			// ������ �����մϴ�.
#define STR_INVALID_TARGET					CStringManager::GetSingleton().GetString( 128 )			// Ÿ���� ��ȿ���� �ʽ��ϴ�.
#define STR_WEAPON_MISMATCH					CStringManager::GetSingleton().GetString( 129 )			// �ʿ��� ���Ⱑ ��� �Ǿ� ���� �ʽ��ϴ�.
#define STR_GET_EXP							CStringManager::GetSingleton().GetString( 130 )			// ����ġ %d�� ������ϴ�.
#define STR_NOTIFY_01						CStringManager::GetSingleton().GetString( 131 )			// ���̻� �������� ������ �����ϴ�.
#define STR_NOTIFY_02						CStringManager::GetSingleton().GetString( 132 )			// �������� �����ϴ�. ����� ȹ���ϼ���.
#define STR_NOTIFY_03						CStringManager::GetSingleton().GetString( 133 )			// <<���� ����>>
#define STR_NOTIFY_04						CStringManager::GetSingleton().GetString( 134 )			// ���� ��� ������Դϴ�.
#define STR_NOTIFY_05						CStringManager::GetSingleton().GetString( 135 )			// ��ȿ���� ���� Ÿ���Դϴ�.
#define STR_NOTIFY_06						CStringManager::GetSingleton().GetString( 136 )			// ������ ���� �ʾ� ����Ҽ� �����ϴ�.
#define ID_STR_DROP_MONEY					CStringManager::GetSingleton().GetString( 137 )			// ���� �����ðڽ��ϱ�?
#define	ID_STR_DROP_ITEM					CStringManager::GetSingleton().GetString( 138 )			// �������� �����ðڽ��ϱ�?
#define STR_USE_REQUIRE						CStringManager::GetSingleton().GetString( 169 )			// �������
#define STR_EQUIP_REQUIRE_JOB				CStringManager::GetSingleton().GetString( 170 )			// ��������
#define STR_MOVE_SPEED						CStringManager::GetSingleton().GetString( 171 )			// �̵��ӵ�
#define STR_CARRY_WEIGHT					CStringManager::GetSingleton().GetString( 172 )			// �޴빫��


#define FORMAT_STR_CANCEL_BLOCK_CHAT				CStringManager::GetSingleton().GetString( 173 )
#define	STR_CANCEL_BLOCK_CHAT						CStringManager::GetSingleton().GetString( 174 )
#define	FORMAT_STR_BLOCK_CHAT						CStringManager::GetSingleton().GetString( 175 )
#define STR_GM_BAN_USER								CStringManager::GetSingleton().GetString( 176 )
#define	FORMAT_STR_GM_BAN_USER						CStringManager::GetSingleton().GetString( 177 )
#define FORMAT_STR_RECEIVE_TRADE_REQ				CStringManager::GetSingleton().GetString( 178 ) 
#define STR_OTHER_CANCEL_TRADE						CStringManager::GetSingleton().GetString( 179 )
#define STR_NOT_ENOUGH_OTHER_INVENTORY_SPACE		CStringManager::GetSingleton().GetString( 180 )
#define STR_OTHER_CANT_RECEIVE_TRADE_REQ			CStringManager::GetSingleton().GetString( 181 )
#define STR_OTHER_REJECT_TRADE_REQ					CStringManager::GetSingleton().GetString( 182 )
#define STR_OTHER_IS_TOO_FAR_TO_TRADE				CStringManager::GetSingleton().GetString( 183 )
#define STR_NOT_FOUND_TRADE_OTHER					CStringManager::GetSingleton().GetString( 184 )
#define STR_STATE_BLOCK_CHAT						CStringManager::GetSingleton().GetString( 185 )
#define STR_REQ_TRADE								CStringManager::GetSingleton().GetString( 186 )
#define STR_NOT_ENOUGH_INVENTORY_SPACE				CStringManager::GetSingleton().GetString( 187 )
#define FORMAT_STR_FAIL_WHISPER						CStringManager::GetSingleton().GetString( 188 )
#define FORMAT_STR_WHISPER							CStringManager::GetSingleton().GetString( 189 )
///*-------------------------------------------------------------------------------*/
/// �α��� ���� 
#define STR_WAIT_LOGIN								CStringManager::GetSingleton().GetString( 190 )
#define STR_COMMON_ERROR							CStringManager::GetSingleton().GetString( 191 )
#define STR_NOT_FOUND_ACCOUNT						CStringManager::GetSingleton().GetString( 192 )
#define	STR_INVALID_PASSWORD						CStringManager::GetSingleton().GetString( 193 )
#define STR_ALREADY_LOGGEDIN						CStringManager::GetSingleton().GetString( 194 )
#define	STR_REFUSED_ACCOUNT							CStringManager::GetSingleton().GetString( 195 )
#define	STR_LOGIN_FAIL								CStringManager::GetSingleton().GetString( 196 )
#define STR_INSPECT_ALL_SEVER						CStringManager::GetSingleton().GetString( 197 )
#define	STR_WAIT_CONNECT_LOGIN_SERVER				CStringManager::GetSingleton().GetString( 198 )
#define	STR_NOT_ENOUGH_EQUIP_SPACE					CStringManager::GetSingleton().GetString( 199 )
#define	STR_EQUIPED_TWO_HAND_WEAPON					CStringManager::GetSingleton().GetString( 200 )

///������ ��� ���ѿ� ���� ��Ʈ�� 
#define STR_DONT_SELL_ITEM							CStringManager::GetSingleton().GetString( 201 )	/// �ȼ� ���� �������Դϴ�.
#define STR_DONT_DROP_ITEM							CStringManager::GetSingleton().GetString( 202 ) /// ������ ���� �������Դϴ�.
#define STR_DONT_EXCHANGE_ITEM						CStringManager::GetSingleton().GetString( 203 ) /// ��ȯ�Ҽ� ���� �������Դϴ�.

/// ��ų ���濡 ������ ��Ʈ��
#define STR_LEARN_SKILL_FAILED						CStringManager::GetSingleton().GetString( 204 ) /// ��ų ���� ����.
#define STR_LEARN_SKILL_JOB_FAILED					CStringManager::GetSingleton().GetString( 205 ) /// ������ ���� �ʽ��ϴ�.
#define STR_LEARN_SKILL_NEED_PRESKILL_FAILED		CStringManager::GetSingleton().GetString( 206 ) /// �ʿ� ��ų�� �����ϴ�.
#define STR_LEARN_SKILL_NEED_ABILITY_FAILED			CStringManager::GetSingleton().GetString( 207 ) /// �ɷ�ġ�� �����մϴ�.
#define STR_LEARN_SKILL_SLOT_FULL_FAILED			CStringManager::GetSingleton().GetString( 208 ) /// ���̻� ��ų�� ���� �����ϴ�.
#define STR_LEARN_SKILL_INVALID_SKILL_FAILED		CStringManager::GetSingleton().GetString( 209 ) /// �߸��� ��ų��ȣ �Դϴ�.

/// ���, ��ų ����
#define STR_ACTION_COMMAND_STOP_STATE_FAILED		CStringManager::GetSingleton().GetString( 210 ) /// �������� ������ ��밡���մϴ�.





///��Ƽ ���� 
#define FORMAT_STR_PARTY_MAKE_REQ					CStringManager::GetSingleton().GetString( 211 ) ///%s���� ��Ƽ �Ἲ�� ��û�߽��ϴ�
#define FORMAT_STR_PARTY_INVITE						CStringManager::GetSingleton().GetString( 212 ) ///%s���� ��Ƽ�� �ʴ��Ͽ����ϴ�
#define FORMAT_STR_PARTY_CHANGE_LEADER				CStringManager::GetSingleton().GetString( 213 ) ///%s���� ��Ƽ�� ������ �Ǿ����ϴ�
#define	STR_PARTY_ENTRUSTED_LEADER					CStringManager::GetSingleton().GetString( 214 ) ///��Ƽ�� ������ ���� �޾ҽ��ϴ�
#define STR_PARTY_BANNED							CStringManager::GetSingleton().GetString( 215 ) ///��Ƽ���� ���� ���� ���߽��ϴ�
#define FORMAT_STR_PARTY_BANNED						CStringManager::GetSingleton().GetString( 216 ) ///%s���� ��Ƽ���� ���� ���� ���߽��ϴ�
#define STR_PARTY_NOTFOUND_DEST						CStringManager::GetSingleton().GetString( 217 ) ///����� ã���� �����ϴ�
#define FORMAT_STR_PARTY_BUSY						CStringManager::GetSingleton().GetString( 218 ) ///%s���� ���� ��û�� ���� �� ���� �����Դϴ�
#define STR_PARTY_MADE								CStringManager::GetSingleton().GetString( 219 ) ///���ο� ��Ƽ�� �Ἲ�Ǿ����ϴ�
#define FORMAT_STR_PARTY_REJECT_INVITE				CStringManager::GetSingleton().GetString( 220 ) ///%s���� ��Ƽ ��û�� �ź��߽��ϴ�
#define FORMAT_STR_PARTY_LEAVE						CStringManager::GetSingleton().GetString( 221 ) ///%s���� ��Ƽ���� Ż���߽��ϴ�
#define FORMAT_STR_PARTY_JOIN						CStringManager::GetSingleton().GetString( 222 ) ///%s���� ��Ƽ�� �߰��Ǿ����ϴ�

#define FORMAT_STR_PARTY_QUERY_ENTRUST				CStringManager::GetSingleton().GetString( 223 ) ///%s���� ��Ƽ������ �����Ͻðڽ��ϱ�?
#define STR_PARTY_QUERY_LEAVE						CStringManager::GetSingleton().GetString( 224 ) ///��Ƽ���� Ż���ϰڽ��ϱ�?
#define FORMAT_STR_PARTY_QUERY_BAN					CStringManager::GetSingleton().GetString( 225 ) ///%s���� ������ ���� ��Ű�ڽ��ϱ�?
#define FORMAT_STR_PARTY_LEVEL						CStringManager::GetSingleton().GetString( 226 ) ///��Ƽ���� %d

#define STR_SKILL_APPLY_FAILED						CStringManager::GetSingleton().GetString( 227 ) ///��ų ���뿡 ���� �߽��ϴ�.
#define STR_UNKNOWN_STATE_FLAG						CStringManager::GetSingleton().GetString( 228 ) ///�˼� ���� ���� �÷���.

#define STR_SEND									CStringManager::GetSingleton().GetString( 230 ) 
#define STR_DELETE									CStringManager::GetSingleton().GetString( 231 ) 

#define STR_CANT_CASTING_STATE						CStringManager::GetSingleton().GetString( 245 ) ///��ų�� ����Ҽ� ���� �����Դϴ�..
#define STR_NOT_ENOUGH_PROPERTY						CStringManager::GetSingleton().GetString( 246 ) ///�ʿ��� �������� �����մϴ�.
#define STR_MISMATCH_CASTING_NEED_EQUIP				CStringManager::GetSingleton().GetString( 247 ) ///�ʿ� ��� �����մϴ�.

#define STR_DOING_SKILL_ACTION						CStringManager::GetSingleton().GetString( 248 ) ///�ʿ� ��� �����մϴ�.
#define STR_NOTIFY_CANT_DUPLICATE					CStringManager::GetSingleton().GetString( 249 ) ///�ߺ� ���� �Ұ� ��ų

#define STR_CANT_OPEN_UNIONSTORE					CStringManager::GetSingleton().GetString( 250 ) ///
#define STR_UNIONPOINT								CStringManager::GetSingleton().GetString( 251 ) ///��������Ʈ
#define STR_WANT_UNIONPOINT							CStringManager::GetSingleton().GetString( 252 ) ///��������Ʈ�� �����ϴ�.
#define STR_ALLIED									CStringManager::GetSingleton().GetString( 253 ) ///��������Ʈ�� �����ϴ�.


#define STR_CANT_EXITGAME							CStringManager::GetSingleton().GetString( 260 ) 
#define STR_WAITTIME_EXITGAME						CStringManager::GetSingleton().GetString( 261 ) 
#define STR_AVATARSTORE_CANT_SELL_OPTION			CStringManager::GetSingleton().GetString( 262 ) 
#define STR_AVATARSTORE_CANT_SELL_UPGRADE			CStringManager::GetSingleton().GetString( 263 ) 
#define STR_AVATARSTORE_CANT_SELL_DURABILITY		CStringManager::GetSingleton().GetString( 264 ) 
#define STR_AVATARSTORE_CANT_SELL_SOCKET			CStringManager::GetSingleton().GetString( 265 ) 
#define STR_AVATARSTORE_CANT_SELL_LIFE				CStringManager::GetSingleton().GetString( 266 ) 

#define STR_FUEL									CStringManager::GetSingleton().GetString( 270 ) 
#define STR_WASTE_FUEL								CStringManager::GetSingleton().GetString( 271 ) 
#define STR_RECOVERY_EXP							CStringManager::GetSingleton().GetString( 272 ) 
#define STR_SUMMON_CAPACITY							CStringManager::GetSingleton().GetString( 273 )


#define STR_NPC_TRADE_PRICE							CStringManager::GetSingleton().GetString( 275 )

#define STR_TARGET									CStringManager::GetSingleton().GetString( 307 )
#define STR_SUMMON_MOB								CStringManager::GetSingleton().GetString( 308 ) ///��ȯ��
#define STR_SHOOT_RANGE								CStringManager::GetSingleton().GetString( 309 ) ///�����Ÿ�
#define STR_APPLY_RANGE								CStringManager::GetSingleton().GetString( 310 ) ///�������
#define STR_RADIUS									CStringManager::GetSingleton().GetString( 311 ) ///�ݰ�
#define STR_EFFECT									CStringManager::GetSingleton().GetString( 312 ) ///ȿ��
#define STR_LEVEL									CStringManager::GetSingleton().GetString( 313 ) ///����
#define STR_CONTINUE_TIME							CStringManager::GetSingleton().GetString( 314 ) ///���ӽð�
#define STR_SECOND									CStringManager::GetSingleton().GetString( 315 ) ///��
#define STR_NEXT_LEVEL								CStringManager::GetSingleton().GetString( 316 ) ///��������
#define STR_SKILL_POWER								CStringManager::GetSingleton().GetString( 317 ) ///��ų�Ŀ�
#define STR_SUCCESS_RATE							CStringManager::GetSingleton().GetString( 318 ) ///������
#define STR_CONSUME_ABILITY							CStringManager::GetSingleton().GetString( 319 ) ///�Ҹ�ɷ�
#define STR_REQUIRE_EQUIP							CStringManager::GetSingleton().GetString( 320 ) ///�ʿ����
#define STR_REQUIRE_JOB								CStringManager::GetSingleton().GetString( 321 ) ///�������
#define STR_REQUIRE_SKILL							CStringManager::GetSingleton().GetString( 322 ) ///�ʼ���ų
#define STR_REQUIRE_ABILITY							CStringManager::GetSingleton().GetString( 323 ) ///���Ǵɷ�


#define	STR_START_MAKE_ITEM							CStringManager::GetSingleton().GetString( 324 ) ///������ ���� ����
#define STR_NOT_ENOUGH_MATERIAL						CStringManager::GetSingleton().GetString( 325 ) ///��ᰡ �����մϴ�
#define STR_NOT_EXIST_MATERIAL						CStringManager::GetSingleton().GetString( 326 ) ///��ᰡ �����ϴ�.
#define	STR_SUCCESS_MAKE_ITEM						CStringManager::GetSingleton().GetString( 327 ) ///������ ������ �����߽��ϴ�
#define STR_FAIL_MAKE_ITEM							CStringManager::GetSingleton().GetString( 328 ) ///������ ������ �����߽��ϴ�
#define STR_NOT_ENOUGH_MAKE_SKILL_LEVEL				CStringManager::GetSingleton().GetString( 329 ) ///���� ��ų ������ �����մϴ�



///<- 2004 / 3 / 4 : �߰� nAvy
#define STR_BANK_NOT_ENOUGH_MONEY					CStringManager::GetSingleton().GetString( 330 ) ///�����ᰡ �����ؼ� �� �̻� ������ �� �����ϴ�
#define STR_BANK_CANT_STORE_ITEM					CStringManager::GetSingleton().GetString( 331 ) ///������ �� ���� �������Դϴ�
#define STR_BANK_NOT_ENOUGH_SPACE					CStringManager::GetSingleton().GetString( 332 ) ///������ ���� ������ �����ϴ�

#define STR_BANK_STORAGE_FEE						CStringManager::GetSingleton().GetString( 333 ) ///������
#define STR_BANK_CANT_STORE							CStringManager::GetSingleton().GetString( 334 ) ///�����Ұ�
#define STR_CANT_SELL								CStringManager::GetSingleton().GetString( 335 ) ///�ǸźҰ�
#define STR_PRICE									CStringManager::GetSingleton().GetString( 336 ) ///����
#define STR_SELL_PRICE								CStringManager::GetSingleton().GetString( 337 ) ///�ǸŰ���
#define STR_REQUIRE_POINT							CStringManager::GetSingleton().GetString( 338 ) ///�ʿ�����Ʈ
#define STR_USING_MAKESKILL							CStringManager::GetSingleton().GetString( 339 ) ///������ų�� ������Դϴ�.
#define STR_CHANGE_PRICES							CStringManager::GetSingleton().GetString( 340 ) ///������ �����Ǿ����ϴ�.
#define STR_QUERY_STOP_TRADE						CStringManager::GetSingleton().GetString( 341 ) ///�ŷ��� �ߴ��Ͻðڽ��ϱ�?
#define STR_WHISPER									CStringManager::GetSingleton().GetString( 342 ) ///�ӼӸ�
#define STR_CANT_JOIN_PARTY_REASON_LEVEL			CStringManager::GetSingleton().GetString( 343 ) ///���� ������ ���� �ʾ� ��Ƽ�� ���� �� �����ϴ�
#define STR_BANK									CStringManager::GetSingleton().GetString( 344 ) ///â��
#define STR_NOT_ENOUGH_MONEY						CStringManager::GetSingleton().GetString( 345 ) ///���� �����մϴ�.
#define STR_ITEM_IN_TRADE_LIST						CStringManager::GetSingleton().GetString( 346 ) ///�ǸŸ�Ͽ� �÷��� �������Դϴ�
#define STR_CANT_USEITEM_REASON_SYNCOPE				CStringManager::GetSingleton().GetString( 347 ) ///������ ���¿����� ����Ҽ� �����ϴ�
#define STR_INVALID_CHARACTER_NAME					CStringManager::GetSingleton().GetString( 348 ) ///�̸��� ������ �ʴ� ���ڰ� �ֽ��ϴ�
#define STR_FAIL_MAKE_NEW_CHARACTER					CStringManager::GetSingleton().GetString( 349 ) ///ĳ���� ���� ����
#define STR_REQUEST_SELECT_CHARACTER				CStringManager::GetSingleton().GetString( 350 ) ///ĳ���͸� ������ �ּ���
#define STR_QUERY_DELETE_CHARACTER					CStringManager::GetSingleton().GetString( 351 ) ///ĳ���͸� ����ðڽ��ϱ�?
#define STR_RECEIVE_CHARACTER_LIST					CStringManager::GetSingleton().GetString( 352 ) ///�ƹ�Ÿ ����Ʈ�� �޽��ϴ�
#define STR_SHOUT									CStringManager::GetSingleton().GetString( 353 ) ///��ħ
#define STR_FAIL_SKILL_LEVELUP						CStringManager::GetSingleton().GetString( 354 ) ///��ų �������� �����߽��ϴ�
#define STR_NOT_ENOUGH_SKILLPOINT					CStringManager::GetSingleton().GetString( 355 ) ///��ų����Ʈ�� ���ڶ��ϴ�


#define TT_PARTY_ENTRUST							CStringManager::GetSingleton().GetString( 357 ) ///��Ƽ�� ����
#define TT_PARTY_LEAVE								CStringManager::GetSingleton().GetString( 358 ) ///��Ƽ Ż��
#define TT_PARTY_BAN								CStringManager::GetSingleton().GetString( 359 ) ///����

#define TT_AVATAR_MENU_BTN_EXCHANGE					CStringManager::GetSingleton().GetString( 360 ) ///"��ȯ ��û" ��ư
#define TT_AVATAR_MENU_BTN_PARTY					CStringManager::GetSingleton().GetString( 361 ) ///"��Ƽ ��û" ��ư
///->


#define TT_MINIMAP_BTN_ZOOM_MAX						CStringManager::GetSingleton().GetString( 139 )
#define TT_MINIMAP_BTN_ZOOM_MIN						CStringManager::GetSingleton().GetString( 140 )
#define TT_MINIMAP_BTN_SHOW_SHOW					CStringManager::GetSingleton().GetString( 141 )
#define TT_MINIMAP_BTN_SHOW_HIDE					CStringManager::GetSingleton().GetString( 142 )
#define TT_BASE_BTN_CHAR							CStringManager::GetSingleton().GetString( 143 )
#define TT_BASE_BTN_INVENTORY						CStringManager::GetSingleton().GetString( 144 )
#define TT_BASE_BTN_SKILL							CStringManager::GetSingleton().GetString( 145 )
#define TT_BASE_BTN_QUEST							CStringManager::GetSingleton().GetString( 146 )
#define TT_BASE_BTN_COMMUNITY						CStringManager::GetSingleton().GetString( 147 )
#define TT_BASE_BTN_OPTION							CStringManager::GetSingleton().GetString( 148 )
#define TT_BASE_BTN_SYSTEM							CStringManager::GetSingleton().GetString( 149 )
#define TT_CHAT_BTN_MAXIMIZE						CStringManager::GetSingleton().GetString( 150 )
#define TT_CHAT_BTN_MINIMIZE						CStringManager::GetSingleton().GetString( 151 )
#define TT_CHAT_BTN_FILTER							CStringManager::GetSingleton().GetString( 152 )
#define TT_QUICKBAR_BTN_NEXT						CStringManager::GetSingleton().GetString( 153 )
#define TT_QUICKBAR_BTN_ROTATE						CStringManager::GetSingleton().GetString( 154 )
#define TT_SKILL_BTN_LEVELUP						CStringManager::GetSingleton().GetString( 155 )
#define TT_QUEST_BTN_ABANDON						CStringManager::GetSingleton().GetString( 156 )
#define TT_ACTIVE_SKILL								CStringManager::GetSingleton().GetString( 157 )
#define TT_SLOT_HELMET								CStringManager::GetSingleton().GetString( 158 )
#define TT_SLOT_FACEITEM							CStringManager::GetSingleton().GetString( 159 )
#define TT_SLOT_ARMOR								CStringManager::GetSingleton().GetString( 160 )
#define TT_SLOT_GAUNTLET							CStringManager::GetSingleton().GetString( 161 )
#define TT_SLOT_BOOTS								CStringManager::GetSingleton().GetString( 162 )
#define TT_SLOT_KNAPSACK							CStringManager::GetSingleton().GetString( 163 )
#define TT_SLOT_WEAPON_R							CStringManager::GetSingleton().GetString( 164 )
#define TT_SLOT_WEAPON_L							CStringManager::GetSingleton().GetString( 165 )
#define TT_SLOT_RING								CStringManager::GetSingleton().GetString( 166 )
#define TT_SLOT_NECKLACE							CStringManager::GetSingleton().GetString( 167 )
#define TT_SLOT_EARING								CStringManager::GetSingleton().GetString( 168 )



#define STR_SKILL_TARGET_FILTER_NOTIFY_01			CStringManager::GetSingleton().GetString( 363 )			/// ���� �׷���� �ƴմϴ�.
#define STR_SKILL_TARGET_FILTER_NOTIFY_02			CStringManager::GetSingleton().GetString( 364 )			/// ���� ������ �ƴմϴ�.
#define STR_SKILL_TARGET_FILTER_NOTIFY_03			CStringManager::GetSingleton().GetString( 365 )			/// ��ȣ���� �÷��̾ �ƴմϴ�.
#define STR_SKILL_TARGET_FILTER_NOTIFY_04			CStringManager::GetSingleton().GetString( 366 )			/// ���Ͱ� �ƴմϴ�.
#define STR_SKILL_TARGET_FILTER_NOTIFY_05			CStringManager::GetSingleton().GetString( 367 )			/// ��������� �÷��̾ �ƴմϴ�.
#define STR_SKILL_TARGET_FILTER_NOTIFY_06			CStringManager::GetSingleton().GetString( 368 )			/// �÷��̾  �ƴմϴ�.
#define STR_SKILL_TARGET_FILTER_NOTIFY_07			CStringManager::GetSingleton().GetString( 369 )			/// �����Ҽ� �ִ� ����� �ƴմϴ�.

#define STR_NOT_ENOUGH_CONDITION					CStringManager::GetSingleton().GetString( 370 )			/// ������ ���� �ʾ� ����Ҽ� �����ϴ�.
#define STR_SO_FAR_DISTANCE							CStringManager::GetSingleton().GetString( 371 )			/// �ʹ� �ָ� ������ �ֽ��ϴ�.
#define STR_CANT_SKILL_IN_AGIT						CStringManager::GetSingleton().GetString( 372 )			/// ����Ʈ���� ����Ҽ� �����ϴ�.


/*
371~380 ������ Ÿ�� �߰� �� ����
*/


///
/// PAT
///
#define STR_ERROR_NOT_ASSEMBLED						CStringManager::GetSingleton().GetString( 381 )			/// ������ �Ǿ����� �ʽ��ϴ�.
#define STR_PAT_ERROR_NOT_EQUIP_BODY				CStringManager::GetSingleton().GetString( 382 )			/// �ٵ��ǰ�� ���� �����ϼ���
#define STR_PAT_ERROR_NOT_EQUAL_CLASS				CStringManager::GetSingleton().GetString( 383 )			/// �ٵ�� ��ǰ������ Ʋ���ϴ�
#define STR_PAT_ERROR_NOT_VERSION					CStringManager::GetSingleton().GetString( 384 )			/// ��ǰ�� ������ġ�� �ٵ𺸴� �����ϴ�
#define STR_CLAN_LEVELUP							CStringManager::GetSingleton().GetString( 385 )	


///#define STR_CANT_CASTING_STATE_CART					CStringManager::GetSingleton().GetString( 456 )			/// ž���߿��� �����Ҽ� �����ϴ�.


///386 ~ 407 : �ɷ¼Ӽ��̸� 



/// �߰� ��� �޼�����
#define STR_CANT_SUMMON_NPC							CStringManager::GetSingleton().GetString( 411 )
#define STR_CANT_DOUBLE_USE							CStringManager::GetSingleton().GetString( 412 )


/// ��� ����
#define STR_GEMMING_SUCCESS							CStringManager::GetSingleton().GetString( 413 )
#define STR_GEMMING_NEED_SOCKET						CStringManager::GetSingleton().GetString( 414 )

#define F_STR_SUMMON_TERMINATED						CStringManager::GetSingleton().GetString( 415 )//��ȯ�� %s�Ҹ�
#define F_STR_GETTING_MONEY							CStringManager::GetSingleton().GetString( 416 )//�� %���� ȹ���Ͽ����ϴ�
#define F_STR_GETTING_ITEMS							CStringManager::GetSingleton().GetString( 417 )//%s %d�� ȹ���Ͽ����ϴ�
#define F_STR_GETTING_ITEM							CStringManager::GetSingleton().GetString( 418 )//%s ȹ���Ͽ����ϴ�
#define F_STR_QUEST_GETTING_MONEY					CStringManager::GetSingleton().GetString( 419 )//�� %d���� ������ϴ�
#define F_STR_QUEST_GETTING_ITEMS					CStringManager::GetSingleton().GetString( 420 )//%s %d���� ������ϴ�
#define F_STR_QUEST_GETTING_ITEM					CStringManager::GetSingleton().GetString( 421 )//%s�� ������ϴ�
#define F_STR_QUEST_SUBTRACT_MONEY					CStringManager::GetSingleton().GetString( 422 )//�� %d���� ���������ϴ�
#define F_STR_QUEST_SUBTRACT_ITEMS					CStringManager::GetSingleton().GetString( 423 )//%s %d���� ���������ϴ�
#define F_STR_QUEST_SUBTRACT_ITEM					CStringManager::GetSingleton().GetString( 424 )//%s�� ���������ϴ�
#define F_STR_SET_ABILITY							CStringManager::GetSingleton().GetString( 425 )//%s �ɷ�ġ�� %d���� ����Ǿ����ϴ�
#define F_STR_INCREASE_ABILITY						CStringManager::GetSingleton().GetString( 426 )//%s �ɷ�ġ�� %d ����߽��ϴ�
#define STR_CANT_USE_DRIVESKILL						CStringManager::GetSingleton().GetString( 427 )//����̺� īƮ ��ų�� ��� �� �� �����ϴ�
#define STR_PARTY_IS_FULL							CStringManager::GetSingleton().GetString( 428 )//�� �̻� �ο��� �߰��� �� �����ϴ�
#define STR_CANT_INVITE_TO_PARTY					CStringManager::GetSingleton().GetString( 429 )//��Ƽ�ʴ�� ��Ƽ�������� �����մϴ�
#define STR_REQUIRE_IDENTIFY						CStringManager::GetSingleton().GetString( 430 )//[�����۰��� �ʿ�)

#define STR_UNIT_PRICE								CStringManager::GetSingleton().GetString( 431 )//�ܰ�
#define STR_REPAIR_PRICE							CStringManager::GetSingleton().GetString( 432 )//������
#define STR_LIFE									CStringManager::GetSingleton().GetString( 433 )//����
#define STR_DURABILITY								CStringManager::GetSingleton().GetString( 434 )//������
#define STR_NORMAL									CStringManager::GetSingleton().GetString( 435 )//����
#define STR_FASTER									CStringManager::GetSingleton().GetString( 436 )//����
#define STR_SLOWER									CStringManager::GetSingleton().GetString( 437 )//����
#define STR_CANT_EQUIP_USING_DRIVESKILL				CStringManager::GetSingleton().GetString( 438 )//PAT���ÿ��� �������� �����Ҽ� �����ϴ�
#define STR_CANT_UNEQUIP_USING_DRIVESKILL			CStringManager::GetSingleton().GetString( 439 )//PAT���ÿ��� �������� Ż���Ҽ� �����ϴ�
#define STR_CANT_UNEQUIP_BULLET_USING_DRIVESKILL	CStringManager::GetSingleton().GetString( 440 )//PAT���ÿ��� �Ҹ�ź�� Ż���Ҽ� �����ϴ�
#define STR_CANT_SUMMON_IN_PVPZONE					CStringManager::GetSingleton().GetString( 441 )//PVP �������� ��ȯ���� ��ȯ�Ҽ� �����ϴ�
#define STR_CANT_USESKILL_USING_DRIVESKILL			CStringManager::GetSingleton().GetString( 442 )//ž���߿��� �����Ҽ� �����ϴ�
#define STR_REMAIN_REMOVE_WAITTIME					CStringManager::GetSingleton().GetString( 443 )//���� �������ð�
#define STR_DAY										CStringManager::GetSingleton().GetString( 444 )//��
#define STR_CANT_OPEN_PRIVATESTORE					CStringManager::GetSingleton().GetString( 445 )//���λ����� ���� �� �� �����ϴ�
#define STR_PRIVATESTORE							CStringManager::GetSingleton().GetString( 446 )//���λ���
#define STR_NONAME									CStringManager::GetSingleton().GetString( 447 )//�̸�����
#define STR_ITEM_CANT_SEPARATED						CStringManager::GetSingleton().GetString( 448 )//�и�/���� �� �� ���� �������Դϴ�
#define STR_ITEM_CANT_UPGRADED						CStringManager::GetSingleton().GetString( 449 )//���� �� �� ���� �������Դϴ�


/// 450 ~ 480 : ���� �̸�

#define F_STR_QUERY_DROP_ITEM						CStringManager::GetSingleton().GetString( 481 )//%s�� �����ðڽ��ϱ�?
#define TT_SELFTARGETING							CStringManager::GetSingleton().GetString( 482 )//Ŭ���� ���� Ÿ����
#define STR_DUPLICATED_FRIENDNAME					CStringManager::GetSingleton().GetString( 483 )//ģ����Ͽ� �̹� �ִ� �̸��Դϴ�
#define F_STR_BANK_TITLE							CStringManager::GetSingleton().GetString( 484 )//%s'�� â��
#define F_STR_QUERY_DELETE_FRIEND					CStringManager::GetSingleton().GetString( 485 )//%s�� ģ����Ͽ��� �����Ͻðڽ��ϱ�?
#define F_STR_RESTRICT_SHOUT						CStringManager::GetSingleton().GetString( 486 )//��ġ��� %d�ʿ� �ѹ��� ����Ҽ� �ֽ��ϴ�
#define STR_HELP									CStringManager::GetSingleton().GetString( 487 )//����
#define STR_CHANGED_PARTY_CONFIG					CStringManager::GetSingleton().GetString( 488 )//��Ƽ ������ ����Ǿ����ϴ�
#define STR_ITEM_PICKUP_RIGHT						CStringManager::GetSingleton().GetString( 489 )//�����ۿ켱��
#define STR_PARTY_ITEM_PICKUP_SEQENCE				CStringManager::GetSingleton().GetString( 490 )//������� ȹ��
#define STR_PARTY_ITEM_PICKUP_RIGHT_ALL				CStringManager::GetSingleton().GetString( 491 )//��Ƽ�� ���� �ο�
#define STR_PARTY_EXP_LEVELRATE						CStringManager::GetSingleton().GetString( 492 )//������� �й�
#define STR_PARTY_EXP_EQUALITY						CStringManager::GetSingleton().GetString( 493 )//�յ��ϰ� �й�
#define STR_MYSTATE_IN_HUNT							CStringManager::GetSingleton().GetString( 494 )//�����
#define STR_MYSTATE_IN_TRADE						CStringManager::GetSingleton().GetString( 495 )//�����
#define STR_MYSTATE_IN_QUEST						CStringManager::GetSingleton().GetString( 496 )//����Ʈ��
#define STR_MYSTATE_IN_EAT							CStringManager::GetSingleton().GetString( 497 )//�Ļ���
#define STR_MYSTATE_IN_BREAK						CStringManager::GetSingleton().GetString( 498 )//�޽���
#define STR_NOT_FOUND_FRIEND						CStringManager::GetSingleton().GetString( 499 )//������ ģ����Ͽ��� ã���� �����ϴ�
#define STR_DEFAULT_PRIVATESTORE_TITLE				CStringManager::GetSingleton().GetString( 500 )//%s' ���λ���

#define F_STR_QUERY_ABANDON_QUEST					CStringManager::GetSingleton().GetString( 501 )//%s ����Ʈ�� �����ϰڽ��ϱ�?
#define STR_SEPARATE_EMPTY_TARGETITEM				CStringManager::GetSingleton().GetString( 502 )//�и�/������ �������� �����ϴ�
#define STR_NOTENOUGH_EMPTY_INVENTORYSLOT			CStringManager::GetSingleton().GetString( 503 )//�κ��丮�� ���������� �����ϴ�
#define F_STR_FRIEND_DELETED						CStringManager::GetSingleton().GetString( 504 )//%s�� ���� ģ�� ��Ͽ��� ���ܽ��׽��ϴ�
#define F_STR_MESSANGER_BLOCKED						CStringManager::GetSingleton().GetString( 505 )//%s�� ��ȭ�� �����Ͽ����ϴ�
#define STR_REQUIRE_SKILLPOINT						CStringManager::GetSingleton().GetString( 506 )//�ʿ� ��ų����Ʈ
#define STR_CANT_LEVELUP							CStringManager::GetSingleton().GetString( 507 )//���̻� �������� ���� �ʽ��ϴ�
#define STR_CANT_REPAIR_DURABILITY_IS_ZERO			CStringManager::GetSingleton().GetString( 508 )//�������� ��� ������ �Ұ����մϴ�
//#define STR_INVALID_CHARACTER_NAME					CStringManager::GetSingleton().GetString( 509 )//�̸��� ������ �ʴ� ���ڰ� �ֽ��ϴ�
#define STR_LOGIN_REPLY_TOO_MANY_USER				CStringManager::GetSingleton().GetString( 510 )//�ִ� �����ο��� �Ѿ����ϴ�. ����Ŀ� �ٽ� �̿����ֽñ� �ٶ��ϴ�
#define STR_LOGIN_REPLY_NO_RIGHT_TO_CONNECT			CStringManager::GetSingleton().GetString( 511 )//����¶����� 9���� ���� ��Ÿ �׽�Ʈ�� ������ �������� ����� �����ϽǼ� �����ϴ�
#define STR_QUERY_REVIVE_CHARACTER					CStringManager::GetSingleton().GetString( 512 )//ĳ���͸� ��Ȱ��Ű�ڽ��ϱ�?
#define STR_CANT_CREATE_MORE_CHARACTER				CStringManager::GetSingleton().GetString( 513 )//ĳ���͸� �� �̻� ���� �� �� �����ϴ�
#define STR_ABSORPTION								CStringManager::GetSingleton().GetString( 514 )//���
#define STR_CHANGE_ABILITY							CStringManager::GetSingleton().GetString( 515 )//����ɷ�ġ
#define STR_STATE									CStringManager::GetSingleton().GetString( 516 )//����
#define F_STR2_CHAT_BLOCKED_BY_GM					CStringManager::GetSingleton().GetString( 517 )//GM[%s)�� %d�а� ä�ñ��� ���׽��ϴ�
#define STR_OTHER_WHISPER_REJECT_STATE				CStringManager::GetSingleton().GetString( 518 )//������ ���� �ӼӸ� �źλ����Դϴ�
#define STR_WEIGHT_RATE								CStringManager::GetSingleton().GetString( 519 )//���Ժ���
#define STR_NO_PENALTY								CStringManager::GetSingleton().GetString( 520 )//���Ѿ���

#define STR_CANT_ATTACK								CStringManager::GetSingleton().GetString( 521 )//���� �Ұ�
#define STR_CANT_RUN								CStringManager::GetSingleton().GetString( 522 )//�ٱ� ����
#define F_STR_SUCCESS_HUNT							CStringManager::GetSingleton().GetString( 523 )//%s ��ɿ� �����߽��ϴ�
#define F_STR_WIN_PVP								CStringManager::GetSingleton().GetString( 524 )//%s���� �������� �̰���ϴ�
#define STR_SHIELD_COUNTERATTACK					CStringManager::GetSingleton().GetString( 525 )//���� �ݰ�
#define STR_DROP_ITEM								CStringManager::GetSingleton().GetString( 526 )//������ ���
#define STR_DROP_MONEY								CStringManager::GetSingleton().GetString( 527 )//�� ���
#define F_STR_SKILL_LEARNED							CStringManager::GetSingleton().GetString( 528 )//��ų %s�� ������ϴ�
#define STR_STORE_IS_CLOSED							CStringManager::GetSingleton().GetString( 529 )//������ �������ϴ�
#define STR_TRADE_IS_CANCEL							CStringManager::GetSingleton().GetString( 530 )//�ŷ��� ��ҵǾ����ϴ�
#define STR_ITEM_IS_SOLD							CStringManager::GetSingleton().GetString( 531 )//�̹� �ȸ� �������Դϴ�
#define STR_ITEM_IS_BUY_ALL							CStringManager::GetSingleton().GetString( 532 )//���̻� ���� �ʴ� �������Դϴ�

#define STR_BANK_REPLY_NEED_PASSWORD				CStringManager::GetSingleton().GetString( 533 )//â�� ��й�ȣ�� �Է��ϼ���
#define STR_BANK_REPLY_CHANGED_PASSWORD				CStringManager::GetSingleton().GetString( 534 )//â�� ��й�ȣ�� ����Ǿ����ϴ�
#define STR_BANK_REPLY_INVALID_PASSWORD				CStringManager::GetSingleton().GetString( 535 )//â�� ��й�ȣ�� Ʋ�Ƚ��ϴ�
#define STR_BANK_REPLY_CLEARED_PASSWORD				CStringManager::GetSingleton().GetString( 536 )//â�� ��й�ȣ�� �����Ǿ����ϴ�
#define STR_BANK_MOVEITEM_FAILED					CStringManager::GetSingleton().GetString( 537 )//������ �̵��� �����߽��ϴ�
#define STR_CHAT_REPLY_MAKE_FAILED					CStringManager::GetSingleton().GetString( 538 )//�游��⿡ �����߽��ϴ�
#define STR_CHAT_REPLY_INVALIED_PASS				CStringManager::GetSingleton().GetString( 539 )//��й�ȣ�� Ʋ���ϴ�
#define STR_CHAT_REPLY_ROOM_NOT_FOUND				CStringManager::GetSingleton().GetString( 540 )//������ ä�ù��Դϴ�


#define STR_CHAT_REPLY_FULL_USERS					CStringManager::GetSingleton().GetString( 541 )//���̻� �����Ҽ� �����ϴ�
#define STR_CHAT_REPLY_KICKED						CStringManager::GetSingleton().GetString( 542 )//ä�ù濡�� ���� ���߽��ϴ�
#define F_STR_QUERY_APPEND_FRIEND_REQ				CStringManager::GetSingleton().GetString( 543 )//%s�� ģ���� ����Ϸ��� �մϴ�. �³��Ͻðڽ��ϱ�?
#define F_STR_REJECT_APPEND_FRIEND_REQ				CStringManager::GetSingleton().GetString( 544 )//%s�� ģ�� �߰� ��û�� �����߽��ϴ�
#define F_STR_NOT_FOUNDORCONNECT_FRIEND				CStringManager::GetSingleton().GetString( 545 )//%s�� ã���� ���ų� �������� �ƴմϴ�
#define STR_OTHERSTATE_IS_REJCTMESSANGER			CStringManager::GetSingleton().GetString( 546 )//������ ���� �޽��� �źλ����Դϴ�
#define STR_MEMO_REPLY_SEND_NOT_EXISTORINVALID		CStringManager::GetSingleton().GetString( 547 )//����� ã���� �����ϴ�
#define STR_MEMO_REPLY_SEND_REFUSED					CStringManager::GetSingleton().GetString( 548 )//���� ���� �ź�
#define STR_MEMO_REPLY_SEND_FULL_MEMO				CStringManager::GetSingleton().GetString( 549 )//������ �������� ������ ���̻� ������ �����ϴ�
#define STR_MEMO_REPLY_SEND_INVALID_CONTENT			CStringManager::GetSingleton().GetString( 550 )//�������

#define STR_CRAFT_BREAKUP_SUCCESS_GEM				CStringManager::GetSingleton().GetString( 551 )//������ �и��� �����߽��ϴ�
#define STR_CRAFT_BREAKUP_DEGRADE_GEM				CStringManager::GetSingleton().GetString( 552 )//������ �и��� �����߽��ϴ�. �и� �������� ������ ��ް��Ұ� �߻��߽��ϴ�
#define STR_CRAFT_BREAKUP_CLEARED_GEM				CStringManager::GetSingleton().GetString( 553 )//������ �и��� �����߽��ϴ�. �и� �������� ������ �սǵǾ����ϴ�
#define STR_CRAFE_BREAKUP_SUCCESS					CStringManager::GetSingleton().GetString( 554 )//������ ���ذ� �����߽��ϴ�
#define STR_CRAFT_UPGRADE_INVALID_MAT				CStringManager::GetSingleton().GetString( 555 )//��� �������� �߸�����ϴ�

#define F_STR_SET_MONEY_INC							CStringManager::GetSingleton().GetString( 556 )//���� %d�����Ͽ����ϴ�
#define F_STR_SET_MONEY_DEC							CStringManager::GetSingleton().GetString( 557 )//���� %d�����Ͽ����ϴ�
#define F_STR_REWARD_MONEY_INC						CStringManager::GetSingleton().GetString( 558 )//�� %d�� �޾ҽ��ϴ�
#define F_STR_REWARD_MONEY_DEC						CStringManager::GetSingleton().GetString( 559 )//�� %d�� ���������ϴ�
#define F_STR_REWARD_ITEMS							CStringManager::GetSingleton().GetString( 560 )//%s %d���� ���� �޾ҽ��ϴ�
#define F_STR_REWARD_ITEM							CStringManager::GetSingleton().GetString( 561 )//%s�� ���� �޾ҽ��ϴ�

/// 562 ~ 570 : ������ Ÿ�� //Numenor: seems like the ammunition type when looking at the STL file.

#define STR_RIDE_PART_VER							CStringManager::GetSingleton().GetString( 571 )//��ǰ��ȣ
#define STR_RESULT_CREATE_CHAR_DUP_NAME				CStringManager::GetSingleton().GetString( 572 )//��ǰ��ȣ
#define STR_APPRAISAL_COST							CStringManager::GetSingleton().GetString( 573 )//��ǰ��ȣ
#define STR_COST									CStringManager::GetSingleton().GetString( 574 )//��ǰ��ȣ

#define STR_SERVERSTATE_SMOOTH						CStringManager::GetSingleton().GetString( 575 )
#define STR_SERVERSTATE_NORMAL						CStringManager::GetSingleton().GetString( 576 )
#define STR_SERVERSTATE_CONFUSION					CStringManager::GetSingleton().GetString( 577 )
#define STR_SERVERSTATE_EXCESS						CStringManager::GetSingleton().GetString( 578 )


#define STR_SKILLTREE								CStringManager::GetSingleton().GetString( 580 )




#define STR_RECEIVER								CStringManager::GetSingleton().GetString( 581 )
#define STR_SENDING_ITEM							CStringManager::GetSingleton().GetString( 582 )
#define STR_REPLY_MALL_ITEM_CHECK_CHAR_NONE			CStringManager::GetSingleton().GetString( 583 )
#define STR_REPLY_MALL_ITEM_CHECK_CHAR_INVALID		CStringManager::GetSingleton().GetString( 584 )
#define STR_REPLY_MALL_ITME_GIVE_SUCCESS			CStringManager::GetSingleton().GetString( 585 )
#define STR_REPLY_MALL_ITEM_GIVE_FAILED				CStringManager::GetSingleton().GetString( 586 )
#define STR_REPLY_MALL_ITEM_BRING_FAILED			CStringManager::GetSingleton().GetString( 587 )
#define STR_SENDER									CStringManager::GetSingleton().GetString( 588 )
#define STR_GIFT_ITEM								CStringManager::GetSingleton().GetString( 589 )
#define STR_PLATINUM								CStringManager::GetSingleton().GetString( 590 )

#define STR_CANTPARTY_WITHOTHERTEAM_INPVPZONE		CStringManager::GetSingleton().GetString( 591 )

#define	STR_BILLING_MSG_FREE_USER				CStringManager::GetSingleton().GetString( 592 )//
#define	STR_BILLING_MSG_FEE_TYPE1				CStringManager::GetSingleton().GetString( 593 )//  /���: ���ϴ� ���׿���� ����� �Դϴ�. (2004��06��01��)
#define	STR_BILLING_MSG_FEE_TYPE2				CStringManager::GetSingleton().GetString( 594 )//         ���ϴ� ��������� ����� �Դϴ�. (18�ð�20��20��)
#define	STR_BILLING_MSG_TIME_ALERT				CStringManager::GetSingleton().GetString( 595 )	// ���� �ð� �뺸
#define	STR_BILLING_MSG_TIME_EXPIRED			CStringManager::GetSingleton().GetString( 596 )	// <����> ���� �ð��� �ٵǾ� ������ �����մϴ�.

#define	STR_BILLING_MSG_KICKOUT_CHEAT_DETECT	CStringManager::GetSingleton().GetString( 597 )	// ���ٵ� ġƮ �߰ߵǼ� ����
#define	STR_BILLING_MSG_KICKOUT_ALREADY_LOGIN	CStringManager::GetSingleton().GetString( 598 ) // �̹� ���ӵ� �����̶� ���� ����
#define	STR_BILLING_MSG_KICKOUT_TIME_EXPIRED	CStringManager::GetSingleton().GetString( 599 )	// ���� �ð� ����
#define	STR_BILLING_MSG_KICKOUT_DUP_LOGIN		CStringManager::GetSingleton().GetString( 600 )	// �ٸ� ���� �����ؼ� ���� ����
#define STR_BILLING_MSG_KICKOUT_NO_RIGHT		CStringManager::GetSingleton().GetString( 601 )	// ������ ������ ����...
#define	STR_BILLING_MSG_KICKOUT_OUT_OF_IP		CStringManager::GetSingleton().GetString( 602 )	// pc�濡�� �����Ҽ� �ִ� ip ������ �ʰ�
#define	STR_BILLING_MSG_KICKOUT_EXIST_ACCOUNT	CStringManager::GetSingleton().GetString( 603 )	// ���ӵǾ� �ִ� ������ ©����.

#define	STR_BILLING_MSG_TYPE_SET_DATE			CStringManager::GetSingleton().GetString( 604 )	// ���� ���� ����	szMsg = 200309101025
#define	STR_BILLING_MSG_TYPE_SET_TIME			CStringManager::GetSingleton().GetString( 605 )	// ���� �ð� ����	szMsg = ������
#define	STR_BILLING_MSG_TYPE_TIME_ALERT			CStringManager::GetSingleton().GetString( 606 )	// ���� �ð� ���	szMsg = ������

#define STR_RESULT_SELECT_SERVER_NEED_CHARGE	CStringManager::GetSingleton().GetString( 607 ) //���ð��� ����Ǿ� �� �̻� �����ϽǼ� �����ϴ�
#define STR_ONLY_PLATINUM_TAB_BANK				CStringManager::GetSingleton().GetString( 608 )
#define STR_ONLY_PLATINUM_SELECT_CHAR			CStringManager::GetSingleton().GetString( 609 )
#define STR_SHOW_PARTYMEMBER_HPGUAGE			CStringManager::GetSingleton().GetString( 610 )


#define STR_PARTYLEVEL							CStringManager::GetSingleton().GetString( 611 )
#define STR_PARTYEXP_DISTRIBUTE_FORM			CStringManager::GetSingleton().GetString( 612 )
#define STR_OK									CStringManager::GetSingleton().GetString( 613 )
#define STR_DELIVERY_STORE						CStringManager::GetSingleton().GetString( 614 )

#define STR_OUTOFRANGE							CStringManager::GetSingleton().GetString( 615 )

#define FSTR_FRIEND_LOGIN						CStringManager::GetSingleton().GetString( 616 )
#define FSTR_FRIEND_LOGOUT						CStringManager::GetSingleton().GetString( 617 )
#define STR_EXPIRED_TIME						CStringManager::GetSingleton().GetString( 618 )
#define STR_ERROR_EQUIP_PAT_BODY_VER			CStringManager::GetSingleton().GetString( 619 )
#define STR_PREVIEW								CStringManager::GetSingleton().GetString( 620 )
#define STR_REGIST_CLANMARK						CStringManager::GetSingleton().GetString( 621 )
#define STR_WARN_BULLET_USEUP					CStringManager::GetSingleton().GetString( 622 )
#define STR_CLANMARK_HELP_HOMEPAGE				CStringManager::GetSingleton().GetString( 623 )
#define STR_CLANMARK_FILENAME					CStringManager::GetSingleton().GetString( 624 )
#define STR_CLANMARK_INVALID_SIZE				CStringManager::GetSingleton().GetString( 625 )
#define STR_CLANMARK_FILE_NOTFOUND				CStringManager::GetSingleton().GetString( 626 )
#define STR_CLANMARK_INVALID_FORMAT				CStringManager::GetSingleton().GetString( 627 )
#define STR_CLANMARK_FILE_NOTFOUNT_REGISTER		CStringManager::GetSingleton().GetString( 628 )
#define STR_CLANMARK_INVALID_COLOR				CStringManager::GetSingleton().GetString( 629 )
#define STR_CLANMARK_REGISTER_SUCCESS			CStringManager::GetSingleton().GetString( 630 )

#define STR_SUCCESS_EXCHANGE					CStringManager::GetSingleton().GetString( 631 )
#define STR_CLANMARK_UPDATE_ERROR				CStringManager::GetSingleton().GetString( 632 )
#define STR_QUERY_CHANGE_CLANMARK				CStringManager::GetSingleton().GetString( 633 )
#define STR_CLANMARK_REGISTER_TIME				CStringManager::GetSingleton().GetString( 634 )

#define STR_CHAT_RESTRICT						CStringManager::GetSingleton().GetString( 635 )
#define STR_SAVED_CHAT							CStringManager::GetSingleton().GetString( 636 )
#define STR_CRASH_MSG							CStringManager::GetSingleton().GetString( 637 )


#define STR_BANK_WITHDRAW_FAIL					CStringManager::GetSingleton().GetString( 638 )
#define STR_BANK_SAVE_FAIL						CStringManager::GetSingleton().GetString( 639 )
#define STR_QUERY_BANK_SAVE						CStringManager::GetSingleton().GetString( 640 )
#define STR_QUERY_BANK_WITHDRAW					CStringManager::GetSingleton().GetString( 641 )

#define STR_SAVE								CStringManager::GetSingleton().GetString( 642 )
#define STR_WITHDRAW							CStringManager::GetSingleton().GetString( 643 )
#define STR_QUERY_REGIST_WISHLIST				CStringManager::GetSingleton().GetString( 644 )

#define STR_ENTERCHATTING_MODE					CStringManager::GetSingleton().GetString( 645 )
#define STR_NORMALCHATTING_MODE					CStringManager::GetSingleton().GetString( 646 )
#define STR_REQUIRE_SP							CStringManager::GetSingleton().GetString( 647 )
#define STR_REQUIRE_JULY						CStringManager::GetSingleton().GetString( 648 )
#define STR_REQUIRE_LEVEL						CStringManager::GetSingleton().GetString( 649 )
#define STR_CONSUME_JULY						CStringManager::GetSingleton().GetString( 650 )
#define STR_CONSUME_SP							CStringManager::GetSingleton().GetString( 651 )
#define STR_EARN_SP								CStringManager::GetSingleton().GetString( 652 )
#define STR_SERVER_EXAMINE						CStringManager::GetSingleton().GetString( 653 )
#define STR_SERVER_INFO							CStringManager::GetSingleton().GetString( 654 )



#define STR_JP_PAY_FREEPLAY						CStringManager::GetSingleton().GetString( 655 )
#define STR_JP_PAY_ENTRY						CStringManager::GetSingleton().GetString( 656 )
#define STR_JP_PAY_COMMUNITY					CStringManager::GetSingleton().GetString( 657 )
#define STR_JP_PAY_TRADE						CStringManager::GetSingleton().GetString( 658 )
#define STR_JP_PAY_STOCKSPACE					CStringManager::GetSingleton().GetString( 659 )
#define STR_JP_PAY_EXTRASTOCK					CStringManager::GetSingleton().GetString( 660 )
#define STR_JP_PAY_STARSHIPPASS					CStringManager::GetSingleton().GetString( 661 )
#define STR_JP_PAY_DUNGEONADVENTURE				CStringManager::GetSingleton().GetString( 662 )
#define STR_JP_PAY_EXTRACHARACTER				CStringManager::GetSingleton().GetString( 663 )

#define STR_JP_BILL_CANT_ATTACK					CStringManager::GetSingleton().GetString( 664 )
#define STR_JP_BILL_CANT_INVITE_PARTY			CStringManager::GetSingleton().GetString( 665 )
#define STR_JP_BILL_CANT_INVITED_PARTY			CStringManager::GetSingleton().GetString( 666 )
#define STR_JP_BILL_CANT_USE_CLANAGIT			CStringManager::GetSingleton().GetString( 667 )
#define STR_JP_BILL_CANT_USE_CLANSKILL			CStringManager::GetSingleton().GetString( 668 )
#define STR_JP_BILL_CANT_USE_CLANCHAT			CStringManager::GetSingleton().GetString( 669 )
#define STR_JP_BILL_CANT_JOIN_CLAN				CStringManager::GetSingleton().GetString( 670 )
#define STR_JP_BILL_CANT_MOVE_PLANET			CStringManager::GetSingleton().GetString( 671 )
#define STR_JP_BILL_CANT_USE_FREINDLIST			CStringManager::GetSingleton().GetString( 672 )
#define STR_JP_BILL_CANT_SEND_MSG				CStringManager::GetSingleton().GetString( 673 )
#define STR_JP_BILL_CANT_USE_CHATROOM			CStringManager::GetSingleton().GetString( 674 )
#define STR_JP_BILL_CANT_TRADE					CStringManager::GetSingleton().GetString( 675 )
#define STR_JP_BILL_CANT_TRADE_DONT_PAY_TARGET	CStringManager::GetSingleton().GetString( 676 )
#define STR_JP_BILL_CANT_OPEN_PRIVATESTORE		CStringManager::GetSingleton().GetString( 677 )
#define STR_JP_BILL_CANT_USE_BANK				CStringManager::GetSingleton().GetString( 678 )
#define STR_JP_BILL_CANT_USE_BANK_MONEY			CStringManager::GetSingleton().GetString( 679 )
#define STR_JP_BILL_CANT_USE_BANK_EXTRASPACE	CStringManager::GetSingleton().GetString( 680 )
#define STR_JP_BILL_CANT_MOVE_PLANET2			CStringManager::GetSingleton().GetString( 681 )
#define STR_JP_BILL_CANT_ENTER_DUNGEON			CStringManager::GetSingleton().GetString( 682 )
#define STR_JP_BILL_CANT_SELECT_CHARACTER		CStringManager::GetSingleton().GetString( 683 )

#define STR_JP_PAY_TYPE							CStringManager::GetSingleton().GetString( 684 )
#define STR_JP_PAY_EXPIRE_DATE					CStringManager::GetSingleton().GetString( 685 )
#define STR_JP_PAY_REMAIN_TIME					CStringManager::GetSingleton().GetString( 686 )
#define STR_JP_PAY_EXPIRE_ALERT					CStringManager::GetSingleton().GetString( 687 )
#define STR_JP_PAY_REMAIN_TIME_MESSAGE			CStringManager::GetSingleton().GetString( 688 )
#define STR_JP_PAY_EXPIRE						CStringManager::GetSingleton().GetString( 689 )

#define STR_CANT_USE_CART						CStringManager::GetSingleton().GetString( 690 )
#define STR_CANT_USE_CASTLEGEAR					CStringManager::GetSingleton().GetString( 691 )
#define STR_CHANGED_STOREDTOWN_POS				CStringManager::GetSingleton().GetString( 692 )
#define STR_RECOVERD_STOREDTOWN_POS				CStringManager::GetSingleton().GetString( 693 )


//TAIWAN BILLING MSG
#define STR_TW_BILL_TYPE_1HOUR					CStringManager::GetSingleton().GetString( 695 )
#define STR_TW_BILL_TYPE_MONTH					CStringManager::GetSingleton().GetString( 696 )
#define STR_TW_BILL_TYPE_12HOUR					CStringManager::GetSingleton().GetString( 697 )
#define STR_TW_BILL_TYPE_3MONTH					CStringManager::GetSingleton().GetString( 698 )
					
#define STR_TW_BILL_TYPE_HONGKONG_MONTH			CStringManager::GetSingleton().GetString( 699 )
#define STR_TW_BILL_TYPE_HONGKONG_3MONTH		CStringManager::GetSingleton().GetString( 600 )
#define STR_TW_BILL_TYPE_HONGKONG_HOUR			CStringManager::GetSingleton().GetString( 701 )
#define STR_TW_BILL_TYPE_HONGKONG_12HOUR		CStringManager::GetSingleton().GetString( 702 )

#define STR_TW_BILL_TYPE_MINUTE					CStringManager::GetSingleton().GetString( 703 )
#define STR_TW_BILL_TYPE_2HOUR					CStringManager::GetSingleton().GetString( 704 )
#define STR_TW_BILL_TYPE_10DAY					CStringManager::GetSingleton().GetString( 705 )
#define STR_TW_BILL_TYPE_FREE					CStringManager::GetSingleton().GetString( 706 )

#define STR_TW_BILL_TYPE_BLOCK_3DAY				CStringManager::GetSingleton().GetString( 707 )
#define STR_TW_BILL_TYPE_BLOCK_7DAY				CStringManager::GetSingleton().GetString( 708 )
#define STR_TW_BILL_TYPE_BLOCK_10DAY			CStringManager::GetSingleton().GetString( 709 )

//Cart coupe(^^?) msg
#define STR_MUST_BOARD_CART_ABLE_TO_USE			CStringManager::GetSingleton().GetString( 710 )
#define STR_AFFIX_RUMBLE						CStringManager::GetSingleton().GetString( 711 )
#define STR_CANT_BOARD_CONDITION				CStringManager::GetSingleton().GetString( 712 )
#define STR_CANT_TOO_HEAVY						CStringManager::GetSingleton().GetString( 713 )
#define STR_WANT_BOARDS							CStringManager::GetSingleton().GetString( 714 )
#define STR_REFUSE_BOARD						CStringManager::GetSingleton().GetString( 715 )
#define STR_BOARDING_CANT_USE					CStringManager::GetSingleton().GetString( 716 )
#define STR_CANT_BOARD							CStringManager::GetSingleton().GetString( 717 )


//kor, ph billing msg
#define STR_FREE_ACCOUNT_USER					CStringManager::GetSingleton().GetString( 718 )
#define STR_PAID_DAYBASED_PLAN					CStringManager::GetSingleton().GetString( 719 )
#define STR_PAID_DAYBASED_PLAN_PLATINUM			CStringManager::GetSingleton().GetString( 720 )

#define STR_PAID_TIMEBASED_PLAN					CStringManager::GetSingleton().GetString( 721 )
#define STR_PAID_INTERNET_CAFE					CStringManager::GetSingleton().GetString( 722 )
#define STR_PAID_USER_PLAN_IC_TIME_END			CStringManager::GetSingleton().GetString( 723 )
#define STR_PAID_USER_PLAN_IC_IP_FULL			CStringManager::GetSingleton().GetString( 724 )

#define STR_GAME_TIME_LEFT_MINS					CStringManager::GetSingleton().GetString( 725 )

#define STR_BILL_AT_ROSEONLINE_HOMEPAGE			CStringManager::GetSingleton().GetString( 726 )
#define STR_INSUFFICIENCY_IP					CStringManager::GetSingleton().GetString( 727 )

#define STR_GETTING_ITEM_PARTY					CStringManager::GetSingleton().GetString( 728 ) //%s���� %s �������� ȹ���Ͽ����ϴ�
#define STR_GETTING_ITEMS_PARTY					CStringManager::GetSingleton().GetString( 729 ) //%s���� %s %d�� ȹ���Ͽ����ϴ�
#define STR_GETTING_MONEY_PARTY					CStringManager::GetSingleton().GetString( 730 ) //%s���� �� %���� ȹ���Ͽ����ϴ�

#define STR_CANT_REFAIR_PAT_ENGINE				CStringManager::GetSingleton().GetString( 731 ) //�� ������ ���� �� �� �����ϴ�. ���� ���Ḧ �����ؾ� �մϴ�.


///////////////////////////////////////////////////////////////////////////////////////
/// sound
///////////////////////////////////////////////////////////////////////////////////////

#define SOUND_EQUIP_ITEM							401
#define SOUND_EQUIP_ITEM_LEATHER					402
#define SOUND_EQUIP_ITEM_METAL						403

#define SOUND_WALK_AVT01							651






#define SOUND_LEVEL_UP								16
#define SOUND_BUTTON_CLICK01						1

#endif // _TYPERESOURCE_