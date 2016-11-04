#include "stdAFX.h"
#include "IO_Skill.h"
#ifndef	__SERVER
	#include "..\util\CGameSTB.h"
#else
	#include "..\util\classSTB.h"
#include "..\GameProc\UseItemDelay.h"
#endif


CSkillLIST g_SkillList;

//-------------------------------------------------------------------------------------------------
CSkillLIST::CSkillLIST ()
{
	m_iSkillCount = 0;

	m_pCastingAniSPEED = NULL;
	m_pActionAniSPEED = NULL;

	m_pReloadTIME = NULL;
}

CSkillLIST::~CSkillLIST ()
{
	this->Free ();
}

void CSkillLIST::Free ()
{
	m_SkillDATA.Free ();
	SAFE_DELETE_ARRAY( m_pCastingAniSPEED );
	SAFE_DELETE_ARRAY( m_pActionAniSPEED );

	SAFE_DELETE_ARRAY( m_pReloadTIME );
}

bool CSkillLIST::LoadSkillTable(const char* pFileName)
{
	if ( !m_SkillDATA.Load( (char*)pFileName, true, true) )
		return false;

	m_pCastingAniSPEED = new float[ m_SkillDATA.m_nDataCnt ];
	m_pActionAniSPEED = new float[ m_SkillDATA.m_nDataCnt ];

#ifdef	__SERVER
	m_pReloadTIME = new DWORD[ m_SkillDATA.m_nDataCnt ];
	::ZeroMemory( m_pReloadTIME, sizeof(DWORD) * m_SkillDATA.m_nDataCnt );
#else
	m_pReloadTIME = new float[ m_SkillDATA.m_nDataCnt ];
	::ZeroMemory( m_pReloadTIME, sizeof(float) * m_SkillDATA.m_nDataCnt );
#endif

	// 0�� ��ų ������...
	for (short nI=1; nI<m_SkillDATA.m_nDataCnt; nI++) {
		if ( SKILL_ANI_CASTING_REPEAT_CNT(nI) ) {
			if ( SKILL_ANI_CASTING_REPEAT(nI ) < 1 ) {
				// �ݺ� ���� Ƚ���� �ִ� �ݺ� ������ ���� !!!
				#ifdef	__INC_WORLD
				char *pMsg = CStr::Printf ("��ų �ݺ� ��� ��ȣ �Է� �ʿ� : %d ����", nI );
					::MessageBox( NULL, pMsg, "��ų stb ����Ÿ �Է� ����", MB_OK );
				#endif

				SKILL_ANI_CASTING_REPEAT_CNT(nI) = 0;
			}
		} // else
		//if ( SKILL_ANI_CASTING_REPEAT(nI ) )	// �ݺ� ������ �ִµ� �ݺ� Ƚ���� 0�̸� 1�� ����.
		//	SKILL_ANI_CASTING_REPEAT_CNT(nI) = 1;

#ifdef	__SERVER
		// ���������� �ݺ� ����� ������� �ɽ��� ������ �ִϼӵ��� �ݺ� ��ǰ��� �����Ͽ� ����...
		// ���� :: �ɽ��� ����, �ɽ��� �ݺ� ������ ������ ���⿡ ������� �׻� ���� ����� ����Ѵ� !!
		// �ݺ� �ӵ� ���� �⺻�� 1.0f, 2.0f�� �ɰ�� ����� 2�� ���� ����, 0.5f�� �Ǹ� 2�� ������.
		if ( SKILL_ANI_CASTING_REPEAT_CNT(nI) ) {
			tagMOTION *pMOTION = NULL;
			float fSpeed;
			DWORD dwTotTime;
			WORD wTotFrame;
			short nMotionTYPE=0;
			// �ݺ� ���
			for (short nM=0; nM<g_TblAniTYPE.m_nColCnt; nM++) {
				if ( FILE_MOTION( nM, SKILL_ANI_CASTING_REPEAT(nI) ) ) {
					nMotionTYPE = nM;
					break;
				}
			}

			pMOTION = g_MotionFILE.IDX_GetMOTION( FILE_MOTION( nMotionTYPE, SKILL_ANI_CASTING_REPEAT(nI) ), 0 );
			wTotFrame = pMOTION->Get_TotalFRAME () * SKILL_ANI_CASTING_REPEAT_CNT(nI);

			// ��� �ӵ�
			fSpeed = SKILL_ANI_CASTING_SPEED(nI) / 100.f;

			// �� �ݺ��� �ҿ�� �ð�...
			dwTotTime = pMOTION->Get_NeedTIME( wTotFrame, fSpeed );

			// �ɽ��� ���
			pMOTION = g_MotionFILE.IDX_GetMOTION( FILE_MOTION( nMotionTYPE, SKILL_ANI_CASTING(nI) ) , 0 );
			wTotFrame = pMOTION->Get_TotalFRAME ();

			// ��ü �ҿ�� �ð�..
			dwTotTime += pMOTION->Get_NeedTIME( wTotFrame, fSpeed );

			m_pCastingAniSPEED[ nI ] = ( 1000.f * wTotFrame ) / ( dwTotTime * pMOTION->m_wFPS );
		} else {
			m_pCastingAniSPEED[ nI ] = SKILL_ANI_CASTING_SPEED(nI) / 100.f;
		}

		// ��ų �ݺ����� ������ �ð�( ���� 1�� 0.2�� )
		m_pReloadTIME[ nI ] = SKILL_RELOAD_TIME(nI) * 200 - 100;	// 0.2f * 1000 :: 0.1�� ����~;
#else
		
		m_pCastingAniSPEED[ nI ] = SKILL_ANI_CASTING_SPEED(nI) / 100.f;

//---------------------------------------------------------------------------------------------
//2005. 5. 30. �� ��ȣ 
		//Ŭ���̾�Ʈ���� �����ð��� 1/5 �� �ð����� �ٲ۴�.	
		m_pReloadTIME[ nI ] = ((float)SKILL_RELOAD_TIME(nI) / 5.0f) * 1000.0f;
		 if ( m_pReloadTIME[ nI ] <= 0.f )
				m_pReloadTIME[ nI ] = 1.0f;	
//---------------------------------------------------------------------------------------------				 
#endif

		if ( m_pCastingAniSPEED[ nI ] <= 0.f ) {
			#ifndef	__SERVER
			// _ASSERT( m_pCastingAniSPEED[ nI ] > 0.f );
			#endif
			m_pCastingAniSPEED[ nI ] = 1.0f;		// ������� ����� ����...
		}

		m_pActionAniSPEED[ nI ] = SKILL_ANI_ACTION_SPEED(nI) / 100.f;
		if ( m_pActionAniSPEED[ nI ] <= 0.f ) {
			#ifndef	__SERVER
			// _ASSERT( m_pActionAniSPEED[ nI ] > 0.f );
			#endif
			m_pActionAniSPEED[ nI ] = 1.0f;
		}
	}

	m_iSkillCount = m_SkillDATA.m_nDataCnt;

	return true;
}

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// 2005. 5. 30 �� ��ȣ 
float CSkillLIST::GetDelayTickCount(DWORD i)
{

	if((i < 0) || (i >= m_SkillDATA.m_nDataCnt))
		return 1.0f;

	return m_pReloadTIME[ i ];

}

