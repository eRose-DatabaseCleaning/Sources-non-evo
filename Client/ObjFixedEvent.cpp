#include "stdafx.h"
#include ".\objfixedevent.h"
#include "System/SystemProcScript.h"


CObjFixedEvent::CObjFixedEvent(void)
{
	m_iCurrentState		= EVENT_OBJ_INIT_STATE;

	m_iUniqueEventID	= 0;	

	m_PositionDummy		= 0;
	m_AnimatibleDummy	= 0;

	m_dwCollisionCheckFrame = 0;

	m_bCanCheckCollision = false;
}

CObjFixedEvent::~CObjFixedEvent(void)
{
}

//-------------------------------------------------------------------------------------------------
/// �̺�Ʈ ������Ʈ�� �߰� ���̶����� ���� ó��
//-------------------------------------------------------------------------------------------------
void  CObjFixedEvent::UnlinkVisibleWorld (void)
{
	assert( 0 && "CObjFixedEvent::UnlinkVisibleWorld" );
}

//-------------------------------------------------------------------------------------------------
/// �̺�Ʈ ������Ʈ�� �߰� ���̶����� ���� ó��
//-------------------------------------------------------------------------------------------------
void CObjFixedEvent::LinkToModel( HNODE hModel )
{
	assert( 0 && "CObjFixedEvent::LinkToModel" );
}

//-------------------------------------------------------------------------------------------------
/// �̺�Ʈ ������Ʈ�� �߰� ���̶����� ���� ó��
//-------------------------------------------------------------------------------------------------
void CObjFixedEvent::InsertToScene (void)
{
	if ( m_bIsVisible || !m_pMODEL || !m_PositionDummy )
		return;

	m_bIsVisible = true;
	
	::insertToScene( m_PositionDummy );		

	ExecEventScript( this->m_iCurrentState, 1, false, true );
}

//-------------------------------------------------------------------------------------------------
/// �̺�Ʈ ������Ʈ�� �߰� ���̶����� ���� ó��
//-------------------------------------------------------------------------------------------------
void CObjFixedEvent::RemoveFromScene (bool bIncludeEFFECT)
{
	if ( !m_bIsVisible || !m_PositionDummy )
		return;

	m_bIsVisible = false;
		
	::removeFromScene( m_PositionDummy );		
}

//------------------------------------------------------------------------------------------
/// �߰����� ���鶧���� �ణ�� ������ �ʿ�
//------------------------------------------------------------------------------------------
void CObjFixedEvent::SetPOSITION (D3DVECTOR &Position)
{	
	if( m_PositionDummy == NULL )
		return;


	::setPosition  ( m_PositionDummy, Position.x, Position.y, Position.z );
	m_PosCUR  = Position;
}

//------------------------------------------------------------------------------------------
/// �̺�Ʈ ������Ʈ�� ����� ���̰ų� ���� ���ؼ� �߰����� ��尡 �ʿ��ϴ�.
//------------------------------------------------------------------------------------------
bool CObjFixedEvent::Create (CMODEL<CFixedPART> *pMODEL, D3DVECTOR &Position, bool bCreateEffect )
{
	/// ���� �������� �ֱ⶧���� ������Ʈ ��ü�� �������� �����ϰ�
	/// ���� ��ǥ�̵��� ���������� ������Ʈ�� �����Ѵ�. ( SetPOSITION ���� ���� ��ġ( m_PosCUR ) �� ���ʿ��� ��� )
	D3DVECTOR vPos = { 0.0f, 0.0f, 0.0f};
	if( !CObjFIXED::Create( pMODEL, vPos, bCreateEffect ) )
		return false;

	if( m_PositionDummy )
		::unloadVisible( m_PositionDummy );

	std::string strName( "FixedEvent_" );
	char *pName = Make_ZNAME( this->m_dwCreateOrder, 100 );
	std::string strTempName( strName + std::string( pName ) );
	/// for position
	std::string strPosName = strTempName + "PosDummy";
	m_PositionDummy = ::loadVisible ( strPosName.c_str(), NULL, NULL, NULL ); 
	if( !m_PositionDummy )
	{
		assert( 0 && "CObjFixedEvent::Create[ loadVisible failed ]" );
		return false;
	}
	::setCollisionLevel( m_PositionDummy, 0 );

	if( m_AnimatibleDummy )
		::unloadVisible( m_AnimatibleDummy );

	std::string strRotName = strTempName + "AniDummy";
	/// for animation
	m_AnimatibleDummy = ::loadAnimatable ( strRotName.c_str(), 0, 0, 0, 0 ); 
	if( !m_AnimatibleDummy )
	{
		assert( 0 && "CObjFixedEvent::Create[ loadAnimatable failed ]" );
		return false;
	}

	::linkNode( m_AnimatibleDummy, this->GetRootZNODE() );
	::linkNode( m_PositionDummy, m_AnimatibleDummy );

	SetPOSITION( Position );

	return true;
}

void CObjFixedEvent::Delete ()
{
	CObjFIXED::Delete();

	if( m_PositionDummy )
		::unloadVisible( m_PositionDummy );

	if( m_AnimatibleDummy )
		::unloadVisible( m_AnimatibleDummy );
}

//-------------------------------------------------------------------------------------------------
/// ���¿� ���� �̺�Ʈ �ߵ�
//------------------------------------------------------------------------------------------------
void CObjFixedEvent::ExecEventScript( int iCurrentState, bool bJustEnd, bool bCollisionEvent, bool bInsertToScene )
{
	/// �浹�� ���� ��ũ��Ʈ �����̶��
	if( bCollisionEvent )
	{
		/// ���°� ������� �ʾҴٸ� �׳� ����..
		if( m_iCurrentState == iCurrentState )
			return;
	}

	m_iCurrentState = iCurrentState;

	if( m_strScriptName.c_str() != NULL && m_strScriptName != "EMPTY" )
	{
		CSystemProcScript::GetSingleton().CallLuaFunction( m_strScriptName.c_str(), 
															ZZ_PARAM_INT, m_iUniqueEventID, 
															ZZ_PARAM_INT, m_iCurrentState, 
															ZZ_PARAM_INT, (int)bJustEnd,
															ZZ_PARAM_INT, (int)bInsertToScene,
															ZZ_PARAM_END );
	}
}

//-------------------------------------------------------------------------------------------------
/// Ʈ���� �ߵ�
//------------------------------------------------------------------------------------------------
void CObjFixedEvent::ExecTrigger()
{

}