#include "stdafx.h"
#include ".\cavatarstate.h"
#include "CJustModelAVT.h"
CAvatarState::CAvatarState(void)
{
}

CAvatarState::~CAvatarState(void)
{

}

void CAvatarState::Update(CJustModelAVT* pAvt)
{
	assert( 0 && "call pure virtual function" );
}

void CAvatarState::Enter( CJustModelAVT* pAvt )
{
	assert( 0 && "call pure virtual function" );
}

void CAvatarState::Leave( CJustModelAVT* pAvt )
{
	assert( 0 && "call pure virtual function" );
}
///*--------------------------------------------------------------------------------------------*///
void CAvatarStateNormal::Enter( CJustModelAVT* pAvt )
{
	pAvt->SetMotion( 1 );	
	///��� �ٲ�
}
///*--------------------------------------------------------------------------------------------*///
void CAvatarStateSitdown::Enter( CJustModelAVT* pAvt )
{
	pAvt->SetMotion( 5 , 1);	
	///����� �ٲ�
}

void CAvatarStateSitdown::Update( CJustModelAVT* pAvt )
{
	if( getAnimatableState( pAvt->GetModelNode() ) == 0 )
		pAvt->SwitchState( CJustModelAVT::STATE_SIT );
}

///*--------------------------------------------------------------------------------------------*///
void CAvatarStateSit::Enter( CJustModelAVT* pAvt )
{
	pAvt->SetMotion( 6 );	
}
///*--------------------------------------------------------------------------------------------*///

void CAvatarStateStandup::Enter( CJustModelAVT* pAvt )
{
	pAvt->SetMotion( 7 , 1);	
}

void CAvatarStateStandup::Update(CJustModelAVT* pAvt)
{
	///����� ������ ���¸� normal�� ����
	if( getAnimatableState( pAvt->GetModelNode() ) == 0 )
		pAvt->SwitchState( CJustModelAVT::STATE_NORMAL );
}
