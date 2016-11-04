#ifndef	__CLS_ACCOUNT_H
#define	__CLS_ACCOUNT_H
//-------------------------------------------------------------------------------------------------

// ���� ���� ����...
// ���� ���� ����...
// ���弭�� ���� ����...
class CLS_Server;

class CLS_Account
{
private:
public :

    CStrVAR         m_Account;
    CStrVAR         m_IP;
    DWORD           m_dwLoginTIME;
	DWORD			m_dwDisconnectTryTIME;

    t_HASHKEY       m_HashACCOUNT;
    DWORD           m_dwLSID;
    DWORD           m_dwRIGHT;

	DWORD			m_dwPayFLAG;

    DWORD           m_dwMD5Password[ 8 ];
	BYTE			m_btChannelNO;

    CLS_Server     *m_pWorldServer;

    classDLLNODE< CLS_Account* > *m_pListNODE;

    CLS_Account ();
    ~CLS_Account ();
	void Init ();
} ;


class CLS_AccountLIST// : public CDataPOOL< CLS_Account >
{
private:
	static CDataPOOL< CLS_Account >	*m_pAccountPOOL;
	static classTIME				 m_Timer;

	CStrVAR							 m_Name;
    classHASH< CLS_Account* >		*m_pHTAcnt;
    classHASH< CLS_Account* >		*m_pHTLSID;
	DWORD							 m_dwLastLSID;

    CCriticalSection	m_csListACCOUNT;
	
public :
    CLS_AccountLIST (char *szLisNAME, int iHashTableSIZE);
    ~CLS_AccountLIST ();

	CLS_Account *Mem_NEW ()						
	{	
		CLS_Account *pCAccount = m_pAccountPOOL->Pool_Alloc();
		if ( pCAccount )
			pCAccount->Init ();
		return pCAccount;
	}
	void	Mem_DEL( CLS_Account *pCAccount )	
	{	
		// LogString (LOG_NORMAL, "   $$$ [%s] deleted in %s UsedCnt: %d \n", pCAccount->m_Account.Get(), m_pAccountPOOL->GetPoolNAME(), m_pAccountPOOL->GetUsedCNT()-1 );
		m_pAccountPOOL->Pool_Free( pCAccount );	
	}

    int         GetLSIDCount ()     {   return m_pHTLSID->GetCount();    }
    int         GetAcntCount ()     {   return m_pHTAcnt->GetCount();    }

    bool		Insert_ACCOUNT (CLS_Account *pCAccount, CLS_Server *pWorldServer);
	CLS_Account*Remove_FromLIST(DWORD dwLSID);
	void		Delete_ACCOUNT (CLS_Account *pCAccount);

    CLS_Account*Search_ACCOUNT (DWORD dwLSID);
    CLS_Account*Search_ACCOUNT (char *szAccount);

	void        Delete_IdleACCOUNT (int iTimeOutSecond);

	DWORD GenerateLSID ()
	{
		DWORD dwLSID;
		m_csListACCOUNT.Lock();
			do {
				// classTIME::InitStartTime() ȣ���� ������� ����� 1/100 �ʸ� ���Ѵ�.
				// ���� !!! �α��� ���� ������ ���弭���� ����� ������ �浹���� �ִٳ�...
				dwLSID = m_Timer.GetPassAbsMilliSecond ();
			} while( m_dwLastLSID == dwLSID );
			m_dwLastLSID = dwLSID;
		m_csListACCOUNT.Unlock();
    
		return dwLSID;
	}
} ;

extern CLS_AccountLIST *g_pListJOIN;    // HASHKEY = CStr::GetHASH( szAccount )
extern CLS_AccountLIST *g_pListWAIT;    // HASHKEY = dwLoginServerSocketID
extern CLS_AccountLIST *g_pListBKDR;

//-------------------------------------------------------------------------------------------------
#endif