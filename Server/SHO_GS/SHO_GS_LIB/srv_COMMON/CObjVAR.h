#ifndef	__COBJVAR_H
#define	__COBJVAR_H
//---------------------------------------------------------------------------
#define	MAX_OBJ_VAR_CNT		20

struct tagObjVAR {
	union {
		struct {
			//t_HASHKEY	m_HashCurrentTrigger;
			t_HASHKEY	m_HashNextTrigger;
			int			m_iNextCheckTIME;
			int			m_iPassTIME;
			union {
				short	m_nEventSTATUS;
				short	m_nAI_VAR[ MAX_OBJ_VAR_CNT ];
			} ;
		} ;
		BYTE m_pVAR[ 1 ];
	} ;
} ;

/**
 * \ingroup SHO_GS_LIB
 * \class	CObjVAR
 * \author	wookSang.Jo
 * \brief	CObjNPC, CObjEVENT ��ü�� �⺻ ����Ÿ ������ ó�� Ŭ����
 */
class CObjVAR : public tagObjVAR
{
public :
	CObjVAR ()
	{
		this->InitObjVAR ();
	}
	void InitObjVAR ()
	{
		//m_HashCurrentTrigger = 0;
		m_HashNextTrigger = 0;
		m_iNextCheckTIME = 0;
		m_iPassTIME = 0; 
		::ZeroMemory( m_nAI_VAR, sizeof(short) * MAX_OBJ_VAR_CNT );
	}

	virtual bool  VSend_gsv_CHAT ( char* /* szMsg */)			{	return true;	}
	virtual bool  VSend_gsv_SHOUT( char* /* szMsg */)			{	return true;	}
	virtual void  VSend_gsv_ANNOUNCE_CHAT( char* /* szMsg */)	{	/* nop */		}
	virtual void  VSet_SHOW		 ( BYTE btShowMode )			{	/* nop */		}

	virtual short VGetCur_ZoneNO() = 0;
	virtual float VGetCur_XPOS()   = 0;
	virtual float VGetCur_YPOS()   = 0;

	virtual	int	  VGetCur_ZoneTIME() = 0;

	short Get_ObjVAR(BYTE btVarIDX)
	{	
		if ( btVarIDX < MAX_OBJ_VAR_CNT )
			return	m_nAI_VAR[ btVarIDX ];
		return 0;
	}
	virtual void  Set_ObjVAR(BYTE btVarIDX, short nValue )
	{
		// ��� �޾Ƽ� ���� ������ �����ؾ� ��...
		if ( btVarIDX < MAX_OBJ_VAR_CNT ) {
			m_nAI_VAR[ btVarIDX ] = nValue;
		}
	}

	virtual void Set_NextTRIGGER( int iRemainSec, t_HASHKEY HashNextTrigger )
	{
		// ��� �޾Ƽ� ���� ������ �����ؾ� ��...
		m_iPassTIME = 0;
		m_iNextCheckTIME = iRemainSec * 1000;
		m_HashNextTrigger = HashNextTrigger;
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