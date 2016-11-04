
#include "stdAFX.h"
#include "CThreadLOG.h"
#include "CThreadGUILD.h"

/*
enum e_LogTYPE {
	GSLOG_LOGIN=0,
	GSLOG_LOGOUT,

	GSLOG_DEADBY,		// ����

	GSLOG_PICK,			// �������� �ݱ�
	GSLOG_DROP,			// �������� �ٴڿ� ����
	GSLOG_DELETED,		// �ʵ� �������� �����

	GSLOG_CREATED,		// ������ �����ؼ� ������.
	GSLOG_DESTROYED,	// ������ ���۽� �����ؼ� �Ҹ��.

	GSLOG_SELL,			// �������� �Ǹ�
	GSLOG_USE,			// �������� ����ؼ� �Ҹ��

	GSLOG_GIVE,			// �������� �ǳ���
	GSLOG_RECV,			// �������� ����

	GSLOG_DEPOSIT,		// â�� ����
	GSLOG_WITHDRAW,		// â���� ã��
} ;
*/

//-------------------------------------------------------------------------------------------------
bool CThreadLOG::When_SysERR ( char *szFile, int iLine, char *szDesc )
{
	/*
	txtIP
	txtACCOUNT
	txtCHAR
	txtFILE
	intLINE
	txtDESC
	*/
	m_csSQL.Lock ();
	{
		// �ð�, �ɸ�, ����, ��ġ, IP
		this->m_QuerySTR.Printf("INSERT tblGS_ERROR ( dateREG, txtIP, txtACCOUNT, txtCHAR, txtFILE, intLINE, txtDESC ) VALUES( \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d,\'%s\' );",
				this->GetCurDateTimeSTR(),
				"SysERR",	// pUSER->Get_IP(),
				"SysERR",	// pUSER->Get_ACCOUNT(),
				"SysERR",	// pUSER->Get_NAME() ? pUSER->Get_NAME() : "????",

				szFile,
				iLine,
				szDesc 
				);

		this->Send_gsv_LOG_SQL();
	}
	m_csSQL.Unlock ();

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CThreadLOG::When_ERROR ( classUSER *pUSER, char *szFile, int iLine, char *szDesc )
{
	/*
	txtIP
	txtACCOUNT
	txtCHAR
	txtFILE
	intLINE
	txtDESC
	*/
	m_csSQL.Lock ();
	{
		// �ð�, �ɸ�, ����, ��ġ, IP
		this->m_QuerySTR.Printf("INSERT tblGS_ERROR ( dateREG, txtIP, txtACCOUNT, txtCHAR, txtFILE, intLINE, txtDESC ) VALUES( \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d,\'%s\' );",
				this->GetCurDateTimeSTR(),
				pUSER->Get_IP(),
				pUSER->Get_ACCOUNT(),
				pUSER->Get_NAME() ? pUSER->Get_NAME() : "????",

				szFile,
				iLine,
				szDesc 
				);

		this->Send_gsv_LOG_SQL();
	}
	m_csSQL.Unlock ();

	return true;
}

//-------------------------------------------------------------------------------------------------
/*
bool CThreadLOG::When_LogIN ( classUSER *pUSER )
{
	m_csSQL.Lock ();
	{
		// �ð�, �ɸ�, ����, ��ġ, IP
		this->m_QuerySTR.Printf("INSERT tblGS_LOG ( dateREG, txtSUBJECT, txtOBJECT, txtSBJIP, txtACTION, txtLOC ) VALUES( \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\' );",
				this->GetCurDateTimeSTR(),
				pUSER->Get_NAME(),
				pUSER->Get_ACCOUNT(),
				pUSER->Get_IP(),

				"LogIN",
				this->GetLocationSTR( pUSER->m_nZoneNO, pUSER->m_PosCUR )
				);

		this->Send_gsv_LOG_SQL();
	}
	m_csSQL.Unlock ();

	return true;
}

//-------------------------------------------------------------------------------------------------
bool CThreadLOG::When_LogOUT ( classUSER *pUSER )
{
	m_csSQL.Lock ();
	{
		SYSTEMTIME stIn;

		classTIME::AbsSecondToSystem( pUSER->m_dwLoginTIME, stIn );

		this->m_ItemSTR.Printf("LogIn Time[%d-%d-%d,%d:%d:%d]", stIn.wYear, stIn.wMonth, stIn.wDay, stIn.wHour, stIn.wMinute, stIn.wSecond );

		if ( 0 == pUSER->m_nZoneNO ) {
			m_PosSTR.Printf( "lobby :: Account: %s", pUSER->Get_ACCOUNT() );
			this->m_QuerySTR.Printf("INSERT tblGS_LOG ( dateREG, txtSUBJECT, txtOBJECT, txtSBJIP, txtACTION, txtLOC, txtITEM ) VALUES( \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\' );",
					this->GetCurDateTimeSTR(),
					"not selected",
					pUSER->Get_ACCOUNT(),
					pUSER->Get_IP(),

					"LogOUT",
					m_PosSTR.Get(),
					this->m_ItemSTR.Get()
					);
		} else {
			this->m_QuerySTR.Printf("INSERT tblGS_LOG ( dateREG, txtSUBJECT, txtOBJECT, txtSBJIP, txtACTION, txtLOC, txtITEM ) VALUES( \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\' );",
					this->GetCurDateTimeSTR(),
					pUSER->Get_NAME(),
					pUSER->Get_ACCOUNT(),
					pUSER->Get_IP(),

					"LogOUT",
					this->GetLocationSTR( pUSER->m_nZoneNO, pUSER->m_PosCUR ),
					this->m_ItemSTR.Get()
					);
		}

		this->Send_gsv_LOG_SQL();
	}
	m_csSQL.Unlock ();

	return true;
}
*/

//-------------------------------------------------------------------------------------------------
#ifdef	__NEW_LOG
bool CThreadLOG::When_CharacterLOG ( classUSER *pSourAVT, char *szCharNAME, BYTE btDelOrNew )
{
/*
	[AddCharacterLog]
		@AccountName nvarchar(20),
		@CharName nvarchar(30),
		@DelAdd tinyint
	#define	SP_AddCharacterLog	"{call AddCharacterLog(\'%s\',\'%s\',%d)}"
*/
	m_csSQL.Lock ();
	{
		this->m_QuerySTR.Printf( SP_AddCharacterLog,
			pSourAVT->Get_ACCOUNT(),
			szCharNAME,
			btDelOrNew );

		this->Send_gsv_LOG_SQL();
	}
	m_csSQL.Unlock ();

	return true;
}
#else
bool CThreadLOG::When_CreateCHAR ( classUSER *pSourAVT, char *szCharNAME )
{
	m_csSQL.Lock ();
	{
		this->m_QuerySTR.Printf("INSERT tblGS_LOG ( dateREG, txtSUBJECT, txtSBJIP, txtACTION, txtITEM ) VALUES( \'%s\',\'%s\',\'%s\',\'%s\',\'%s\' );",
				this->GetCurDateTimeSTR(),
				szCharNAME,
				pSourAVT->Get_IP(),

				"AddCHAR",
				pSourAVT->Get_ACCOUNT()
				);

		this->Send_gsv_LOG_SQL();
	}
	m_csSQL.Unlock ();

	return true;
}
bool CThreadLOG::When_DeleteCHAR ( classUSER *pSourAVT, char *szCharNAME )
{
	m_csSQL.Lock ();
	{
		this->m_QuerySTR.Printf("INSERT tblGS_LOG ( dateREG, txtSUBJECT, txtSBJIP, txtACTION, txtITEM ) VALUES( \'%s\',\'%s\',\'%s\',\'%s\',\'%s\' );",
				this->GetCurDateTimeSTR(),
				szCharNAME,
				pSourAVT->Get_IP(),

				"DelCHAR",
				pSourAVT->Get_ACCOUNT()
				);
		this->Send_gsv_LOG_SQL();
	}
	m_csSQL.Unlock ();

	return true;
}
#endif


//-------------------------------------------------------------------------------------------------
bool CThreadLOG::When_ws_CLAN ( char *szName, char *szIP, char *szLogType, CClan *pClan, BYTE btSucType, int iSkillNo, int iNewPos )
{
	m_csSQL.Lock ();
	{
#ifdef	__NEW_LOG
		this->m_QuerySTR.Printf( SP_AddClanLog,	// [AddClanLog]
				szName,					// @CharName nvarchar(32),
				pClan->m_Name.Get(),	// @ClanName nvarchar(20),
				pClan->m_nClanLEVEL,	// @ClanLevel smallint,
				pClan->m_iClanSCORE,	// @Point int,
				btSucType,				// @Success tinyint,
				szLogType,// "unknown(ws)",			// @Location varchar(24),
				iSkillNo,				// @LocX int,
				iNewPos );				// @LocY int
#endif
		this->Send_gsv_LOG_SQL();
	}
	m_csSQL.Unlock ();
	return true;
}

//-------------------------------------------------------------------------------------------------
//bool CThreadLOG::When_CheatCODE ( classUSER *pSourAVT, char *szCode )
//{
//	m_csSQL.Lock ();
//	{
//		this->m_ItemSTR.Printf( "Account:%s[ %s ]", pSourAVT->Get_ACCOUNT(), szCode );
//
//		this->m_QuerySTR.Printf("INSERT tblGS_LOG ( dateREG, txtSUBJECT, txtSBJIP, txtACTION, txtLOC, txtITEM ) VALUES( \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\' );",
//				this->GetCurDateTimeSTR(),
//				pSourAVT->Get_NAME(),
//				pSourAVT->Get_IP(),
//
//				"CheatCODE",
//				this->GetLocationSTR( pSourAVT->m_nZoneNO, pSourAVT->m_PosCUR ),
//				this->m_ItemSTR.Get()
//				);
//
//		this->Send_gsv_LOG_SQL();
//	}
//	m_csSQL.Unlock ();
//
//	return true;
//}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
CThreadLOG::CThreadLOG () : CSqlTHREAD( true ), m_PosSTR(128), m_ItemSTR(256), m_QuerySTR( 1024 ), m_DescSTR( 256 )
{
	m_bWaiting = false;
}
CThreadLOG::~CThreadLOG ()
{
	;
}

//-------------------------------------------------------------------------------------------------
void CThreadLOG::Send_gsv_LOG_SQL()
{
	this->Add_QueryString( this->m_QuerySTR.Get() );
}

//-------------------------------------------------------------------------------------------------
void CThreadLOG::Execute ()
{
	g_LOG.CS_ODS( 0xffff, ">  > >> CThreadLOG::Execute() ThreadID: %d(0x%x)\n", this->ThreadID, this->ThreadID );

    while( TRUE ) {
		if ( !this->Terminated ) {
			m_bWaiting = true;
			m_pEVENT->WaitFor( INFINITE );
			m_bWaiting = false;
		} else {
			int iCnt1;
			this->m_CS.Lock ();
				iCnt1 = m_AddPACKET.GetNodeCount();
			this->m_CS.Unlock ();
			if ( iCnt1 <= 0 ) {
				m_bWaiting = true;
				break;
			}
		}

		// ���� �α� ����..
		this->Proc_QuerySTRING ();
	}

	g_LOG.CS_ODS( 0xffff, "<  < << CThreadLOG::Execute() ThreadID: %d(0x%x)\n", this->ThreadID, this->ThreadID );
}

//-------------------------------------------------------------------------------------------------
