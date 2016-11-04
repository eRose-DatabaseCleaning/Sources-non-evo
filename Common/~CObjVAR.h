#ifndef	__COBJVAR_H
#define	__COBJVAR_H
//---------------------------------------------------------------------------
#define	MAX_OBJ_VAR_CNT		20
class CObjVAR
{
public :
	int			m_iNextCheckTIME;
	t_HASHKEY	m_HashNextTrigger;
	int			m_iPassTIME;
	union {
		short	m_nEventSTATUS;
		short	m_nAI_VAR[ MAX_OBJ_VAR_CNT ];
	} ;

	CObjVAR ()
	{
		this->InitObjVAR ();
	}
	void InitObjVAR ()
	{
		m_HashNextTrigger = 0;
		m_iNextCheckTIME = 0;
		::ZeroMemory( m_nAI_VAR, sizeof(short) * MAX_OBJ_VAR_CNT );
	}

	virtual bool  VSend_gsv_CHAT ( char* /* szMsg */)			{	return true;	}
	virtual bool  VSend_gsv_SHOUT( char* /* szMsg */)			{	return true;	}
	virtual void  VSend_gsv_ANNOUNCE_CHAT( char* /* szMsg */)	{	/* nop */		}

	short Get_ObjVAR(BYTE btVarIDX)
	{	
		return	m_nAI_VAR[ btVarIDX ];
	}
	virtual void  Set_ObjVAR(BYTE btVarIDX, short nValue )
	{
		// ��� �޾Ƽ� ���� ������ �����ؾ� ��...
		m_nAI_VAR[ btVarIDX ] = nValue;
	}

	virtual void Set_NextTRIGGER( int iRemainSec, t_HASHKEY /* HashNextTrigger */)
	{
		// ��� �޾Ƽ� ���� ������ �����ؾ� ��...
		m_iPassTIME = 0;
		m_iNextCheckTIME = iRemainSec * 1000;
	}

	bool ProcVAR (DWORD dwPassTIME)
	{
		if ( m_iNextCheckTIME ) {
			m_iPassTIME += dwPassTIME;
			if ( m_iPassTIME >= m_iNextCheckTIME ) {
				m_iNextCheckTIME = 0;
				// true�� ���� Ʈ���� �̺�Ʈ ó��...
				return true;
			}
		}
		return false;
	}
} ;

//---------------------------------------------------------------------------
#endif