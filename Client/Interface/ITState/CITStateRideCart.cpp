#include "stdafx.h"
#include ".\CITStateRideCart.h"
#include "../it_mgr.h"
#include "../../Object.h"

// īƮ��忡�� �� �� �ִ� ����//

// 1. �����ϱ�,
// 2. ���뽺ų ��� (�̸�Ƽ�ܵ�)
// 3. īƮ ���� ��ų ��� (���� ����.)
// 4. ä�� ���

CITStateRideCart::CITStateRideCart(void)
{
	m_iID = IT_MGR::STATE_RIDECART;
}


CITStateRideCart::~CITStateRideCart(void)
{

}


void CITStateRideCart::Enter()
{
	// 1, 2�� ����	
	g_pAVATAR->GetSkillSlot()->SetActiveSkillEnableByRideState_Aid( true );	
}

void CITStateRideCart::Leave()
{
	g_pAVATAR->GetSkillSlot()->SetActiveSkillEnableByRideState_Aid( false );
}

unsigned CITStateRideCart::Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam )
{
	if( uiMsg == WM_LBUTTONUP )
		CWinCtrl::SetMouseExclusiveCtrl( NULL );

	///ä��, ��ų, ����Ű ���̾�α׸� ó���Ѵ�.�������� �����Ѵ�.
	CTDialog* pDlg = NULL;

	// 4. ä�ø��
	if( pDlg = g_itMGR.FindDlg( DLG_TYPE_CHAT ) )
	{
		if( pDlg->Process( uiMsg, wParam, lParam ) )
		{
			return uiMsg;
		}
	}

	return 0;
}