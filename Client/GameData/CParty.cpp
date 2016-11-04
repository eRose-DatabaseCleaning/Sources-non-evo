#include "stdafx.h"
#include ".\cparty.h"
#include "Object.h"
#include "../interface/interfacetype.h"
#include "../interface/it_mgr.h"
#include "../interface/dlgs/CMsgBox.h"
#include "../interface/Command/CTCmdHotExec.h"
#include "common/Calculation.h"
#include "../Network/CNetwork.h"
//*-------------------------------------------------------------------------------------
/// ���� - ���������� Object Server Index�� ��ġ�� ���
/// 1. UserTag�� Object Server Index�� �ٲپ������, �ٸ� Zone Server�� �ִ� �ƹ�Ÿ�鳢���� index�� ��ĥ�� �ִ�.
//*-------------------------------------------------------------------------------------
const int c_iAutoExpDecreaseTimeSpace = 10000;
CParty::CParty(void)
{
	m_bHasParty			= false;
	m_iLevel			= 1;
	m_iCurrExp			= 0;
	m_wLeaderObjSvrIdx	= 0;
}

CParty::~CParty(void)
{
}

CParty& CParty::GetInstance()
{
	static CParty s_Instance;
	return s_Instance;
}
//*----------------------------------------------------------------------
/// @param	memberinfo ������ Network���� ���Ǵ� struct
/// @param  pszName	�ƹ�Ÿ �̸�
/// @return �߰��� ������ true
/// @brief ��Ƽ�� ��Ƽ���� ���ö� ó���ϴ� Method
//*----------------------------------------------------------------------
bool CParty::JoinMember( tag_PARTY_MEMBER& memberinfo, const char* pszName )
{
	assert( pszName );
	if( pszName == NULL )
		return false;

	//if( IsPartyMember( memberinfo.m_wObjectIDX ) )
	//{
	//	assert( 0 && "�������Ʈ�� �ִ� �ƹ�Ÿ�Դϴ�.@CParty::JoinMember" );
	//	return false;
	//}

	if( memberinfo.m_wObjectIDX != g_pObjMGR->Get_ServerObjectIndex(g_pAVATAR->Get_INDEX() ))
	{
		CObjAVT* pObjAVT = g_pObjMGR->Get_CharAVT( g_pObjMGR->Get_ClientObjectIndex( memberinfo.m_wObjectIDX), false );

		if( pObjAVT )
		{
			
			pObjAVT->m_EndurancePack.DeleteEnduranceState( memberinfo.m_dwStatusFALG );
			//increase_maxhp = pObjAVT->m_EndurancePack.GetStateValue( ING_INC_MAX_HP );
			pObjAVT->Set_HP( memberinfo.m_nHP );
			pObjAVT->Set_MaxHP( memberinfo.m_nMaxHP );
			pObjAVT->Set_CON( memberinfo.m_nCON );
			pObjAVT->Set_AddRecoverHP( memberinfo.m_btRecoverHP );
			pObjAVT->Set_AddRecoverMP( memberinfo.m_btRecoverMP );
			pObjAVT->SetStamina( memberinfo.m_nSTAMINA );
		}
	}

	PartyMember member;

	member.m_strName = pszName;
	member.m_Info = memberinfo;

	m_Members.push_back( member );

	m_Event.SetID( CTEventParty::EID_JOIN_MEMBER );
	m_Event.SetObjSvrIdx( member.m_Info.m_wObjectIDX );
	m_Event.SetObjectTag( member.m_Info.m_dwUserTAG );
	m_Event.SetName( pszName );
	SetChanged();
	NotifyObservers( &m_Event );
	return true;
}
//*----------------------------------------------------------------------
/// @param	wObjSvrIdx ��Ƽ�� ������ �ƹ�Ÿ�� Object Server Index;
/// @return ��Ƽ������Ʈ���� ã�� ���ϸ� false;
/// @brief ��Ƽ�� ��Ƽ���� ������ Method
//*----------------------------------------------------------------------
bool CParty::LeaveMember(  DWORD dwObjectTAG )
{
	std::list< PartyMember >::iterator iter;
	for( iter = m_Members.begin(); iter != m_Members.end(); ++iter )
	{
		if( iter->m_Info.m_dwUserTAG == dwObjectTAG )
		{
			m_Event.SetID( CTEventParty::EID_LEAVE_MEMBER );
			m_Event.SetObjSvrIdx( iter->m_Info.m_wObjectIDX );
			SetChanged();
			NotifyObservers( &m_Event );
			m_Members.erase( iter );
			return true;
		}
	}
	assert( 0 && "Not Found Party Member @CParty::LeaveMember" );
	return false;
}

bool CParty::BanMember( DWORD dwObjectTag )
{
	///���� Ban���������
	if( dwObjectTag == g_pAVATAR->GetUniqueTag() )
	{
		g_itMGR.AppendChatMsg(STR_PARTY_BANNED, IT_MGR::CHAT_TYPE_SYSTEM );
		Leave();
		return true;
	}

	std::list< PartyMember >::iterator iter;
	for( iter = m_Members.begin(); iter != m_Members.end(); ++iter )
	{
		if( iter->m_Info.m_dwUserTAG == dwObjectTag )
		{
			m_Event.SetID( CTEventParty::EID_BAN_MEMBER );
			m_Event.SetObjectTag( dwObjectTag );
			SetChanged();
			NotifyObservers( &m_Event );
			m_Members.erase( iter );
			return true;
		}
	}
	assert( 0 && "Not Found Party Member @CParty::BanMember" );
	return false;
}
//*----------------------------------------------------------------------
/// @param			wObjSvrIdx	������ ���ϴ� �ƹ�Ÿ�� Object Server Index;
/// @param [out]	member		�ƹ�Ÿ�� ������ ���� Data Reference
/// @return			bool		��Ƽ������Ʈ���� ã�� ���ϸ� false;
///
/// @brief	��Ƽ�� ��Ƽ���� ������ Method
//*----------------------------------------------------------------------
bool CParty::GetMemberInfoByObjSvrIdx(WORD wObjSvrIdx, PartyMember& member )
{
	std::list< PartyMember >::iterator iter;
	for( iter = m_Members.begin(); iter != m_Members.end(); ++iter )
	{
		if( iter->m_Info.m_wObjectIDX == wObjSvrIdx )
		{
			member = *iter;
			return true;
		}
	}
	return false;
}	
bool CParty::GetMemberInfoByTag(DWORD dwObjectTAG, PartyMember& member )
{
	std::list< PartyMember >::iterator iter;
	for( iter = m_Members.begin(); iter != m_Members.end(); ++iter )
	{
		if( iter->m_Info.m_dwUserTAG == dwObjectTAG )
		{
			member = *iter;
			return true;
		}
	}
	return false;
}
//*----------------------------------------------------------------------
/// @brief ���� ��Ƽ���ΰ�?
//*----------------------------------------------------------------------
bool CParty::HasParty()
{
	return m_bHasParty;
}
//*----------------------------------------------------------------------
/// @brief ��Ƽ����� Ǯ�ΰ�?
//*----------------------------------------------------------------------
bool CParty::IsPartyFull()
{
	return false;
}
//*----------------------------------------------------------------------
/// @brief ���� ��Ƽ�ο��� 
//*----------------------------------------------------------------------
int CParty::GetMemberCount()
{
	return m_Members.size();
}
//*----------------------------------------------------------------------
/// @brief ���� ��Ƽ�� �����ΰ�?
//*----------------------------------------------------------------------
bool CParty::IsPartyLeader()				
{
	if( g_pAVATAR->GetUniqueTag() == m_dwLeaderObjectTAG )//g_pObjMGR->Get_ServerObjectIndex(g_pAVATAR->Get_INDEX()) == m_wLeaderObjSvrIdx )
		return true;

	return false;
}
//*----------------------------------------------------------------------
/// @param wObjSvrIdx �ƹ�Ÿ�� ���� ������Ʈ �ε���
/// @brief ��Ƽ ����ΰ�?
//*----------------------------------------------------------------------
bool CParty::IsPartyMember(WORD wObjSvrIdx)
{
	std::list< PartyMember >::iterator iter;
	for( iter = m_Members.begin(); iter != m_Members.end(); ++iter )
	{
		if( iter->m_Info.m_wObjectIDX == wObjSvrIdx )
			return true;
	}
	return false;
}
//*----------------------------------------------------------------------
/// @param iLevel ��Ƽ ����
/// @brief ������������ ���ؼ� �ʿ��� �� Exp
//*----------------------------------------------------------------------
int	 CParty::GetNeedExp4Levelup ( int iLevel )
{
	// return ( ( iLevel+3) * ( iLevel+5)*5 ) + 40;
	return CCal::Get_PartyLevelUpNeedEXP( iLevel );	// 2004. 7. 22 ������ �������� ����ϴ� �Լ��� ��ü
}
//*----------------------------------------------------------------------
/// @brief ��Ƽ������ ���Ѵ�.
//*----------------------------------------------------------------------
int CParty::GetLevel()
{
	return m_iLevel;
}
//*----------------------------------------------------------------------
/// @brief ���� ���������� ����ġ�� ���Ѵ�
//*----------------------------------------------------------------------
int CParty::GetCurrExp()
{
	return m_iCurrExp;
}
//*----------------------------------------------------------------------
/// @brief ���� ���������� �ִ����ġ�� ���Ѵ�( ������������ ���� �ʿ����ġ�̱⵵ �ϴ� )
//*----------------------------------------------------------------------
int CParty::GetMaxExp()
{
	return GetNeedExp4Levelup( m_iLevel );
}
//*----------------------------------------------------------------------
/// @brief ���� ��Ƽ�� �����Ҷ� ó���� Method
//*----------------------------------------------------------------------
void CParty::Join()
{
	m_bHasParty = true;
	m_Event.SetID( CTEventParty::EID_INIT );
	SetChanged();
	NotifyObservers( &m_Event );
	///�ڱ� �ڽ��� ����Ʈ�� �߰�.
	tag_PARTY_MEMBER memberinfo;
	memberinfo.m_wObjectIDX = g_pObjMGR->Get_ServerObjectIndex( g_pAVATAR->Get_INDEX() );
	memberinfo.m_dwUserTAG	= g_pAVATAR->GetUniqueTag();
	JoinMember( memberinfo, g_pAVATAR->Get_NAME() );
}

void CParty::Make()///���� ��Ƽ�� ����������
{
	m_bHasParty = true;
	m_Event.SetID( CTEventParty::EID_INIT );
	SetChanged();
	NotifyObservers( &m_Event );

	SetExp( 0 );
	SetLevel( 1 );
	tag_PARTY_MEMBER memberinfo;
	memberinfo.m_wObjectIDX = g_pObjMGR->Get_ServerObjectIndex( g_pAVATAR->Get_INDEX() );
	memberinfo.m_dwUserTAG	= g_pAVATAR->GetUniqueTag();
	JoinMember( memberinfo, g_pAVATAR->Get_NAME() );
	ChangeLeaderByObjSvrIdx( g_pObjMGR->Get_ServerObjectIndex( g_pAVATAR->Get_INDEX() ) );
}
//*----------------------------------------------------------------------
/// @brief ���� ��Ƽ�� ������ ó���� Method
//*----------------------------------------------------------------------
void CParty::Leave()
{
	m_bHasParty = false;
	Destory();
}
//*----------------------------------------------------------------------
/// @brief ��Ƽ�� ��� ������ �ػ�Ǿ���.
//*----------------------------------------------------------------------
void CParty::Destory()
{
	m_bHasParty			= false;
	m_iLevel			= 1;
	m_iCurrExp			= 0;
	m_wLeaderObjSvrIdx	= 0;
	m_dwLeaderObjectTAG = 0;

	std::list< PartyMember >::iterator iter;
	for( iter =	m_Members.begin(); iter != m_Members.end(); )
	{
		m_Event.SetID( CTEventParty::EID_CLEAR );
		SetChanged();
		NotifyObservers( &m_Event );


		iter = m_Members.erase( iter );
	}
}
//*----------------------------------------------------------------------
/// @brief ��Ƽ���� ����
//*----------------------------------------------------------------------

bool CParty::ChangeLeaderByObjSvrIdx( WORD wObjSvrIdx )
{
	PartyMember member;
	if( GetMemberInfoByObjSvrIdx( wObjSvrIdx , member ) )
	{
		m_wLeaderObjSvrIdx = wObjSvrIdx;
		m_dwLeaderObjectTAG = member.m_Info.m_dwUserTAG;

		m_Event.SetID( CTEventParty::EID_CHANGE_LEADER );
		m_Event.SetObjSvrIdx( wObjSvrIdx );
		SetChanged();
		NotifyObservers( &m_Event );
		return true;
	}
	assert( 0 && "Not Found Party Member @CParty::ChangeLeader" );
	return false;
}
bool CParty::ChangeLeaderByTag( DWORD dwObjectTag )
{
	PartyMember member;
	if( GetMemberInfoByTag( dwObjectTag, member ) )
	{
		m_wLeaderObjSvrIdx	= member.m_Info.m_wObjectIDX;
		m_dwLeaderObjectTAG = dwObjectTag;

		m_Event.SetID( CTEventParty::EID_CHANGE_LEADER );
		m_Event.SetObjSvrIdx( member.m_Info.m_wObjectIDX );

		SetChanged();
		NotifyObservers( &m_Event );

		return true;
	}
	assert( 0 && "Not Found Party Member @CParty::ChangeLeader" );
	return false;
}



//*----------------------------------------------------------------------
/// @brief MemberInfo.m_dwUserTag �� Member�� ã�Ƽ� �ش� ������ �ٲپ� �ش�.( �������� �̵��� ��� )
//*----------------------------------------------------------------------
bool CParty::ChangeMemberInfoByUserTag( tag_PARTY_MEMBER& MemberInfo )
{
	std::list< PartyMember >::iterator iter;
	for( iter = m_Members.begin(); iter != m_Members.end(); ++iter )
	{
		if( iter->m_Info.m_dwUserTAG == MemberInfo.m_dwUserTAG )
		{
			m_Event.SetID( CTEventParty::EID_CHANGE_OBJECT_SERVERINDEX );
			m_Event.SetObjSvrIdx( MemberInfo.m_wObjectIDX );
			m_Event.SetObjectTag( MemberInfo.m_dwUserTAG );

			iter->m_Info	= MemberInfo;

			SetChanged();
			NotifyObservers( &m_Event );
			return true;
		}
	}
	assert( 0 && "Not Found Party Member @CParty::ChangeMemberInfoByUserTag" );
	return false;
}
void CParty::SetLevel( int iLevel )
{
	m_iLevel = iLevel;
}
//*----------------------------------------------------------------------
/// @brief �������� ���� ����ġ�� �����ϰ� ���� Update �ð��� ���� �ð����� set�Ѵ�.
//*----------------------------------------------------------------------
void CParty::SetExp( int iExp )
{
	m_iCurrExp = iExp;
	m_dwExpLastUpdateTime = g_GameDATA.GetGameTime();
}

//*----------------------------------------------------------------------
/// @brief ��Ƽ�� �������� �Ǿ����� ó���� Method
/// @warn	1. ���ڽŵ� ����Ʈ�� ���ԵǾ��ִ�.MP�� ������ ���� �÷��ش�. ( �ٸ� ����� �˼��� ����. )
///			2. �׾�����쿡�� �ø��� �ʴ´�.
//*----------------------------------------------------------------------
void CParty::LevelUp()
{
	g_pAVATAR->Set_MP( g_pAVATAR->Get_MaxMP() );

//	g_pAVATAR->Set_HP( g_pAVATAR->Get_MaxHP() );

	CObjAVT* pObjAVT;
	std::list< PartyMember >::iterator iter;
	for( iter = m_Members.begin(); iter != 	m_Members.end(); ++iter )
	{
		pObjAVT = g_pObjMGR->Get_CharAVT( g_pObjMGR->Get_ClientObjectIndex( iter->m_Info.m_wObjectIDX ), true );
		if( pObjAVT )
			pObjAVT->Set_HP( pObjAVT->Get_MaxHP() );
	}
}
//*-----------------------------------------------------------------------------------//
/// @breif ��Ƽ���ϰ�� �������� Update �ϴ� Method
//*-----------------------------------------------------------------------------------//
void CParty::Update()
{
	if( !m_bHasParty ) return;

	DWORD dwCurrTime  = g_GameDATA.GetTime();
	DWORD dwTimeSpace =  dwCurrTime - m_dwExpLastUpdateTime;

	if( dwTimeSpace && (dwTimeSpace > c_iAutoExpDecreaseTimeSpace) )
		SetExp( m_iCurrExp -  ( GetAutoDecreaseExp( m_iLevel, GetMemberCount() ) * dwTimeSpace ) / c_iAutoExpDecreaseTimeSpace );

	/// ���� ���� ��Ƽ���ϰ�쿡�� ó���ϴ� �κ�
	if( !IsPartyLeader() ) 	return;
	std::list< PartyMember >::iterator iter;
	for( iter = m_Members.begin(); iter != m_Members.end(); ++iter )
	{
		if( iter->m_Info.m_wObjectIDX == 0 )
		{
			if( dwCurrTime - iter->m_dwDisconnectedTime > 5 * 60 * 1000 )
			{
				///�������� Ban�ϵ��� ��û�Ѵ�.
				if( iter->m_Info.m_dwUserTAG == g_pAVATAR->GetUniqueTag() )
					continue;

				CTCmdSendPacketPartyReq Command( PARTY_REQ_BAN, iter->m_Info.m_dwUserTAG );
				Command.Exec( NULL );
			}
		}
	}
}
//*-----------------------------------------------------------------------------------//
/// @breif �����ð����� �����ϴ� ��Ƽ ����ġ�� ���ϴ� Method
//*-----------------------------------------------------------------------------------//
int CParty::GetAutoDecreaseExp( int iPartyLv, int iCountPartyMember )
{
	if( iCountPartyMember <= 0 )
		return 0;

	return iPartyLv / 2 + iCountPartyMember;
}
//*-----------------------------------------------------------------------------------//
/// @breif ���� ���� ��Ƽ�� ���� �ִ°�?
//*-----------------------------------------------------------------------------------//
bool CParty::IsValidJoinParty()
{
	if( g_pAVATAR && g_pAVATAR->Get_HP() <= 0 )
		return false;

	if( g_itMGR.IsDlgOpened( DLG_TYPE_DEAL ) )
		return false;

	if( g_itMGR.IsDlgOpened( DLG_TYPE_EXCHANGE ) )
		return false;

	if( g_itMGR.IsDlgOpened( DLG_TYPE_DIALOG ))
		return false;

	if( g_itMGR.IsDlgOpened( DLG_TYPE_MAKE ))
		return false;

	if( HasParty() )
		return false;

	if( g_itMGR.FindMsgBox( CMsgBox::MSGTYPE_RECV_PARTY_REQ ) )
		return false;

	return true;
}

WORD CParty::GetLeaderSvrIndex()
{
	return 	m_wLeaderObjSvrIdx;
}

DWORD CParty::GetLeaderObjectTAG()
{
	return m_dwLeaderObjectTAG;
}

void CParty::SetLeaderObjSvrIdx( WORD wObjSvrIdx )
{
	m_wLeaderObjSvrIdx = wObjSvrIdx;
}

void CParty::SetLeaderObjectTag( DWORD dwObjectTag )
{
	m_dwLeaderObjectTAG = dwObjectTag;
}

//*-----------------------------------------------------------------------------------//
/// @breif ��Ƽ���� ������ ����Ǿ���
//*-----------------------------------------------------------------------------------//
void CParty::MemberDisconnected( DWORD dwUserTag )
{
	std::list< PartyMember >::iterator iter;
	for( iter = m_Members.begin(); iter != m_Members.end(); ++iter )
	{
		if( iter->m_Info.m_dwUserTAG == dwUserTag )
		{
			iter->m_Info.m_wObjectIDX = 0;
			iter->m_dwDisconnectedTime = g_GameDATA.GetTime();
			m_Event.SetID( CTEventParty::EID_MEMBER_DISCONNECTED );
			m_Event.SetObjectTag( dwUserTag );
			m_Event.SetObjSvrIdx(0);

			SetChanged();
			NotifyObservers( &m_Event );
			return;
		}
	}
	assert( 0 && "Not Found Party Member @CParty::MemberDisconnected" );
}

void CParty::SendChangePartyRule( BYTE btPartyRule )
{	
	if( m_btPartyRule != btPartyRule )
	{
		m_btPartyRule = btPartyRule;
		g_pNet->Send_cli_PARTY_RULE( btPartyRule );
	}
}

void CParty::RecvPartyRule( BYTE btPartyRule )
{
	m_btPartyRule = btPartyRule;

	m_Event.SetID( CTEventParty::EID_CHANGE_RULE );
	m_Event.SetRule( btPartyRule );

	SetChanged();
	NotifyObservers( &m_Event );

	///UI�鿡�� �˸���.
}