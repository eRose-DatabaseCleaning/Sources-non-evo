#include "stdAFX.h"
#ifndef	__SERVER
#include "CRegenAREA.h"
#include "Game.h"
#include "IO_Terrain.h"
	#if defined( _DEBUG )
	#include "Tgamectrl.h"
	#endif
#else
#include "GS_ThreadZONE.h"
#endif

//-------------------------------------------------------------------------------------------------
#ifndef	__SERVER
CRegenPOINT::CRegenPOINT (CMAP *pZONE, float fXPos, float fYPos)
#else
CRegenPOINT::CRegenPOINT (CZoneTHREAD *pZONE, float fXPos, float fYPos)
#endif
{
	m_pZONE = pZONE;
	m_pBasicMOB = NULL;
	m_pTacticsMOB = NULL;

	m_iLiveCNT = 0;
	m_fXPos = fXPos;
	m_fYPos = fYPos;
	m_dwCheckTIME = 0;
	m_iCurTactics = 1;
//#ifdef	_DEBUG
//	m_iLastCurTactics = m_iCurTactics-1;
//#endif
}
CRegenPOINT::~CRegenPOINT ()
{
#ifdef	__VIRTUAL_SERVER
	classDLLNODE< CObjCHAR*> *pCharNODE;
	
	pCharNODE = m_CharLIST.GetHeadNode ();
	while( pCharNODE ) {
		this->ClearCharacter ( pCharNODE->DATA );
		pCharNODE = m_CharLIST.GetHeadNode ();
	}
#endif

	SAFE_DELETE_ARRAY( m_pBasicMOB );
	SAFE_DELETE_ARRAY( m_pTacticsMOB );
}

//-------------------------------------------------------------------------------------------------
#ifdef	__SERVER
bool CRegenPOINT::Load (FILE *fp)
{
#else
bool CRegenPOINT::Load ( CFileSystem* pFileSystem )
{
	if( pFileSystem == NULL )
		return false;
#endif

	int iIndex, iCount;
	BYTE btStrLen;

	#ifdef	__SERVER
		fread( &btStrLen,		sizeof(char),	1,			fp);
		fseek(fp,	btStrLen,	SEEK_CUR);
		fread( &m_iBasicCNT,		sizeof(int),	1,			fp);
	#else
		pFileSystem->ReadByte( &btStrLen );
		pFileSystem->Seek( btStrLen, FILE_POS_CUR );
		pFileSystem->ReadInt32( &m_iBasicCNT );
	#endif
	if ( m_iBasicCNT > 0 ) 
	{
		m_pBasicMOB = new tagREGENMOB[ m_iBasicCNT ];
		::ZeroMemory( m_pBasicMOB, sizeof( tagREGENMOB ) * m_iBasicCNT );
	}

    int iM;
	for (iM=0; iM<m_iBasicCNT; iM++) 
	{
	#ifdef	__SERVER
		fread( &btStrLen,	sizeof(char),	1,			fp);
		fseek(fp,	btStrLen,	SEEK_CUR);
		fread( &iIndex,		sizeof(int),	1,			fp);
		fread( &iCount,		sizeof(int),	1,			fp);
	#else
		pFileSystem->ReadByte( &btStrLen );
		pFileSystem->Seek( btStrLen, FILE_POS_CUR );
		pFileSystem->ReadInt32( &iIndex );
		pFileSystem->ReadInt32( &iCount );
	#endif

		m_pBasicMOB[ iM ].m_iMobIDX = iIndex;
		m_pBasicMOB[ iM ].m_iMobCNT = iCount;
	}

	#ifdef	__SERVER
	fread( &m_iTacticsCNT, sizeof(int),	1,	fp);
	#else
	pFileSystem->ReadInt32( &m_iTacticsCNT );
	#endif
	if ( m_iTacticsCNT > 0 ) 
	{
		m_pTacticsMOB = new tagREGENMOB[ m_iTacticsCNT ];
		::ZeroMemory( m_pTacticsMOB, sizeof( tagREGENMOB ) * m_iTacticsCNT );
	}

	for (iM=0; iM<m_iTacticsCNT; iM++) 
	{
	#ifdef	__SERVER
		fread( &btStrLen,	sizeof(char),	1,			fp);
		fseek(fp,	btStrLen,	SEEK_CUR);
		fread( &iIndex,		sizeof(int),	1,			fp);
		fread( &iCount,		sizeof(int),	1,			fp);
	#else
		pFileSystem->ReadByte( &btStrLen );
		pFileSystem->Seek( btStrLen, FILE_POS_CUR );		
		pFileSystem->ReadInt32( &iIndex );
		pFileSystem->ReadInt32( &iCount );
	#endif
		m_pTacticsMOB[ iM ].m_iMobIDX = iIndex;
		m_pTacticsMOB[ iM ].m_iMobCNT = iCount;
	}

	#ifdef	__SERVER
		fread( &m_iInterval,		sizeof(int),	1,	fp);
		fread( &m_iLimitCNT,		sizeof(int),	1,	fp);
		fread( &m_iRange,			sizeof(int),	1,	fp);
		fread( &m_iTacticsPOINT,	sizeof(int),	1,	fp);
	#else
		pFileSystem->ReadInt32( &m_iInterval );
		pFileSystem->ReadInt32( &m_iLimitCNT );
		pFileSystem->ReadInt32( &m_iRange );
		pFileSystem->ReadInt32( &m_iTacticsPOINT );
	#endif
	m_iInterval *= 1000;
	m_iRange *= 100;		// m --> cm

	return true;
}


#ifndef	__SERVER
#include "../interface/it_mgr.h"
void CRegenPOINT::RegenCharacter (int iIndex, int iCount)
{
	if ( iIndex < 1 || iCount < 1 )
		return;

	float fXPos, fYPos;

	#if defined( _DEBUG )
	g_itMGR.AppendChatMsg( CStr::Printf ("      %s  %d���� ����", NPC_NAME( iIndex ), iCount ), IT_MGR::CHAT_TYPE_SYSTEM );
	#endif
	for (int iC=0; iC<iCount; iC++) {
		fXPos = m_fXPos - m_iRange + RANDOM( m_iRange*2 );
		fYPos = m_fYPos - m_iRange + RANDOM( m_iRange*2 );

#ifdef	__VIRTUAL_SERVER
		D3DVECTOR Position = { fXPos, fYPos, 0 };

		int iObject = g_pObjMGR->Add_MobCHAR( 0, iIndex, Position, 0, NPC_WALK_SPEED( iIndex ) );
		if ( iObject ) {
			CObjCHAR *pCharOBJ = g_pObjMGR->Get_CharOBJ( iObject, true );
			if( pCharOBJ )
			{
				pCharOBJ->m_pRegenPOINT    = this;
				pCharOBJ->m_pRegenListNODE = m_CharLIST.AllocNAppend( pCharOBJ );

				m_iLiveCNT ++;
			}
		}
#endif
	}
}
#endif

//-------------------------------------------------------------------------------------------------
void CRegenPOINT::ClearCharacter (CObjCHAR *pCharOBJ)
{
#ifdef	__VIRTUAL_SERVER
	m_CharLIST.DeleteNFree( pCharOBJ->m_pRegenListNODE );
	pCharOBJ->m_pRegenPOINT = NULL;
	m_iLiveCNT --;
#endif
}

enum {
	SLOT0 = 0,
	SLOT1,
	SLOT2,
	SLOT3,
	SLOT4,
	SLOT5,
	SLOT6,
	SLOT7
} ;

//-------------------------------------------------------------------------------------------------
void CRegenPOINT::Proc (DWORD dwCurTIME)
{
	/*
	DWORD dwCurTime = ::timeGetTime ();
	if ( dwCurTime - m_dwCheckTIME < m_iInterval )
		return;
	*/
	if ( dwCurTIME - m_dwCheckTIME < (DWORD)m_iInterval )
		return;

	m_dwCheckTIME = dwCurTIME;

	if ( m_iLiveCNT > m_iLimitCNT ) {
		// ���� ����Ʈ 1 ����
		if ( m_iCurTactics > 1 )
			m_iCurTactics--;
		return;
	}

	// �������� = { ( �Ѱ����*2 - ������� ) * ��������P * 50 } / { �Ѱ���� * ����P�ֱ� }
	int iVar = ( ( m_iLimitCNT*2 - m_iLiveCNT ) * m_iCurTactics * 50 ) / ( m_iLimitCNT *  m_iTacticsPOINT );
	if ( iVar <= 10 ) {
		// 0 ~ 10	1��ĭ�� ���͸� ���� ���� ��ŭ �߻�	��������Ʈ + 12
		m_iCurTactics += 12;
		if ( m_iBasicCNT > SLOT0 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT0 ].m_iMobIDX, m_pBasicMOB[ SLOT0 ].m_iMobCNT );
	} else
	if ( iVar <= 15 ) {
		// 11 ~ 15	1��ĭ�� ���͸� (���� ����-2) ��ŭ �߻��ϰ�, 
		///			2��ĭ�� ���͸� ���� ���� ��ŭ �߻�.	��������Ʈ + 15
		m_iCurTactics += 15;
		if ( m_iBasicCNT > SLOT0 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT0 ].m_iMobIDX, m_pBasicMOB[ SLOT0 ].m_iMobCNT-2 );
		if ( m_iBasicCNT > SLOT1 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT1 ].m_iMobIDX, m_pBasicMOB[ SLOT1 ].m_iMobCNT );
	} else
	if ( iVar <= 25 ) {
		// 16 ~ 25	3��ĭ�� ���͸� ���� ���� ��ŭ �߻�	��������Ʈ + 12
		m_iCurTactics += 12;
		if ( m_iBasicCNT > SLOT2 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT2 ].m_iMobIDX, m_pBasicMOB[ SLOT2 ].m_iMobCNT );
	} else
	if ( iVar <= 30 ) {
		// 26 ~ 30	1��ĭ�� ���͸� (���� ����-1) ��ŭ �߻��ϰ�, 
		//			3��ĭ�� ���͸� ���� ���� ��ŭ �߻�	��������Ʈ + 15
		m_iCurTactics += 15;
		if ( m_iBasicCNT > SLOT0 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT0 ].m_iMobIDX, m_pBasicMOB[ SLOT0 ].m_iMobCNT-1 );
		if ( m_iBasicCNT > SLOT2 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT2 ].m_iMobIDX, m_pBasicMOB[ SLOT2 ].m_iMobCNT );
	} else
	if ( iVar <= 40 ) {
		// 31 ~ 40	4��ĭ�� ���͸� ���� ���� ��ŭ �߻�	��������Ʈ + 12
		m_iCurTactics += 12;
		if ( m_iBasicCNT > SLOT3 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT3 ].m_iMobIDX, m_pBasicMOB[ SLOT3 ].m_iMobCNT );
	} else 
	if ( iVar <= 50 ) {
		// 41 ~ 50	2��ĭ�� ���͸� ���� ���� ��ŭ �߻��ϰ�, 
		//			3��ĭ�� ���͸� (���� ����-2) ��ŭ �߻�	��������Ʈ + 12
		m_iCurTactics += 12;
		if ( m_iBasicCNT > SLOT1 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT1 ].m_iMobIDX, m_pBasicMOB[ SLOT1 ].m_iMobCNT );
		if ( m_iBasicCNT > SLOT2 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT2 ].m_iMobIDX, m_pBasicMOB[ SLOT2 ].m_iMobCNT-2 );
	} else
	if ( iVar <= 65 ) {
		// 51 ~ 65	3��ĭ�� ���͸� ���� ���� ��ŭ �߻��ϰ�, 
		//			4��ĭ�� ���͸� (���� ����-2) ��ŭ �߻�	��������Ʈ + 20
		m_iCurTactics += 20;
		if ( m_iBasicCNT > SLOT2 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT2 ].m_iMobIDX, m_pBasicMOB[ SLOT2 ].m_iMobCNT );
		if ( m_iBasicCNT > SLOT3 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT3 ].m_iMobIDX, m_pBasicMOB[ SLOT3 ].m_iMobCNT-2 );
	} else
	if ( iVar <= 73 ) {
		// 66 ~ 73	4��ĭ�� ���͸� ���� ���� ��ŭ �߻��ϰ�, 
		//			5��ĭ�� ���͸� ���� ���� ��ŭ �߻�	��������Ʈ + 15
		m_iCurTactics += 15;
		if ( m_iBasicCNT > SLOT3 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT3 ].m_iMobIDX, m_pBasicMOB[ SLOT3 ].m_iMobCNT );
		if ( m_iBasicCNT > SLOT4 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT4 ].m_iMobIDX, m_pBasicMOB[ SLOT4 ].m_iMobCNT );
	} else 
	if ( iVar <= 85 ) {
		// 74 ~ 85	1��ĭ�� ���͸� ���� ���� ��ŭ �߻��ϰ�, 
		//			4��ĭ�� ���͸� (���� ����-1) ��ŭ �߻��ϰ�, 
		//			5��ĭ�� ���͸� (���� ����-2) ��ŭ �߻�	��������Ʈ + 15
		m_iCurTactics += 15;
		if ( m_iBasicCNT > SLOT0 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT0 ].m_iMobIDX, m_pBasicMOB[ SLOT0 ].m_iMobCNT );

		if ( m_iTacticsCNT > SLOT0 ) 
			this->RegenCharacter( m_pTacticsMOB[ SLOT0 ].m_iMobIDX, m_pTacticsMOB[ SLOT0 ].m_iMobCNT-1 );

		if ( m_iBasicCNT > SLOT4 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT4 ].m_iMobIDX, m_pBasicMOB[ SLOT4 ].m_iMobCNT-2 );
	} else 
	if ( iVar <= 92 ) {
		// 85 ~ 92	6��ĭ�� ���͸� ���� ���� ��ŭ �߻��ϰ�, 
		//			7��ĭ�� ���͸� ���� ���� ��ŭ �߻�	��������Ʈ�� 1�� ����
		m_iCurTactics = 1;

		if ( m_iBasicCNT > SLOT1 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT1 ].m_iMobIDX, m_pBasicMOB[ SLOT1 ].m_iMobCNT );

		if ( m_iTacticsCNT > SLOT0 ) 
			this->RegenCharacter( m_pTacticsMOB[ SLOT0 ].m_iMobIDX, m_pTacticsMOB[ SLOT0 ].m_iMobCNT );

		if ( m_iTacticsCNT > SLOT1 ) 
			this->RegenCharacter( m_pTacticsMOB[ SLOT1 ].m_iMobIDX, m_pTacticsMOB[ SLOT1 ].m_iMobCNT );
	} else {
		// 93 ~ 	2��ĭ�� ���͸� ���� ���� ��ŭ �߻��ϰ�, 
		//			6��ĭ�� ���͸� ���� ���� ��ŭ �߻��ϰ�, 
		//			7��ĭ�� ���͸� ���� ���� ��ŭ �߻�	��������Ʈ�� 7�� ����
		m_iCurTactics = 7;

		if ( m_iBasicCNT > SLOT4 ) 
			this->RegenCharacter( m_pBasicMOB[ SLOT4 ].m_iMobIDX, m_pBasicMOB[ SLOT4 ].m_iMobCNT );

		if ( m_iTacticsCNT > SLOT0 )
			this->RegenCharacter( m_pTacticsMOB[ SLOT0 ].m_iMobIDX, m_pTacticsMOB[ SLOT0 ].m_iMobCNT+1 );
		if ( m_iTacticsCNT > SLOT1 )
			this->RegenCharacter( m_pTacticsMOB[ SLOT1 ].m_iMobIDX, m_pTacticsMOB[ SLOT1 ].m_iMobCNT );
	}

	if ( m_iCurTactics > 500 )
		m_iCurTactics = 500;

//#ifdef	_DEBUG
//	if ( m_iLastCurTactics != m_iCurTactics ) {
//		//AddMsgToChatWND(CStr::Printf (">>>> ��������Ʈ ��ȭ: %d => %d", m_iLastCurTactics, m_iCurTactics ), g_dwBLUE );
//		m_iLastCurTactics = m_iCurTactics;
//	}
//#endif
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#ifndef	__SERVER
CRegenAREA::CRegenAREA ()
{
}
CRegenAREA::~CRegenAREA ()
{
	classDLLNODE< CRegenPOINT* > *pNode;

	pNode = m_LIST.GetHeadNode ();
	while( pNode ) {
		SAFE_DELETE( pNode->DATA );

		m_LIST.DeleteNFree( pNode );
		pNode = m_LIST.GetHeadNode ();
	} ;
}

//-------------------------------------------------------------------------------------------------
void CRegenAREA::Proc ()
{
	classDLLNODE< CRegenPOINT* > *pNode;

	DWORD dwCurTime = g_GameDATA.GetGameTime();

	pNode = m_LIST.GetHeadNode ();
	while( pNode ) {
		
		pNode->DATA->Proc (dwCurTime);

		pNode = m_LIST.GetNextNode( pNode );
	} ;
}
#endif

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

