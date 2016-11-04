#ifndef	__CWORLDVAR_H
#define	__CWORLDVAR_H
//-------------------------------------------------------------------------------------------------

#define	WORLD_VAR	"WORLD_VAR"

#define	MAX_WORLD_VAR_CNT		40

#define WORLD_VAR_YEAR			1
#define	WORLD_VAR_MONTH			2
#define	WORLD_VAR_DAY			3
#define	WORLD_VAR_TIME			4

#define	WORLD_VAR_PRICES		11		// ���� ����
#define	WORLD_VAR_EXP			12		// ���� ����ġ
#define	WORLD_VAR_DROP_I		13		// ���� ��� Ȯ��
#define	WORLD_VAR_DROP_M		14		// ���� ��� �Ӵ�
#define	WORLD_VAR_QST_REWARD	15		// ���� ����Ʈ ����
#define	WORLD_VAR_PRODUCT		16		// ���� ���� ������
#define	WORLD_VAR_STAMINA		17		// ���� ���׹̳�	

struct tagWorldVAR {
	union {
		struct {
			DWORD	m_dwAccTIME;
			short	m_nWorldVAR[ MAX_WORLD_VAR_CNT ];
		} ;
		BYTE	m_pVAR[ 1 ];
	} ;
} ;

class CWorldVAR : public tagWorldVAR
{ 
public :
	CWorldVAR ()
	{
		m_dwAccTIME = 0;
		::ZeroMemory( m_nWorldVAR,	sizeof(short)*MAX_WORLD_VAR_CNT );

		m_nWorldVAR[ WORLD_VAR_DROP_M		] = 100;
		m_nWorldVAR[ WORLD_VAR_DROP_I		] = 100;
		m_nWorldVAR[ WORLD_VAR_EXP			] = 100;
		m_nWorldVAR[ WORLD_VAR_PRODUCT		] = 100;
		m_nWorldVAR[ WORLD_VAR_PRICES		] = 100;
		m_nWorldVAR[ WORLD_VAR_QST_REWARD	] = 100;
		m_nWorldVAR[ WORLD_VAR_STAMINA		] = 100;
	}

	#define	TIME_PER_YEAR		103680
	#define	TIME_PER_MONTH		8640

	#define	MONTH_PER_YEAR		12
	#define DAY_PER_MONTH		54
	void Inc_WorldTIME (bool bWorldServer=false)
	{
		m_dwAccTIME ++;
		if ( ++m_nWorldVAR[ WORLD_VAR_TIME  ] > TIME_PER_MONTH ) {
			m_nWorldVAR[ WORLD_VAR_TIME  ] = 0;

			if ( ++m_nWorldVAR[ WORLD_VAR_MONTH ] > MONTH_PER_YEAR ) {
				m_nWorldVAR[ WORLD_VAR_MONTH ] = 0;
				m_nWorldVAR[ WORLD_VAR_YEAR ]++;
			}
		}

		m_nWorldVAR[ WORLD_VAR_DAY ] = (short)( m_nWorldVAR[ WORLD_VAR_TIME  ] / (TIME_PER_MONTH/DAY_PER_MONTH) ) + 1;

		if ( bWorldServer ) {
			if ( 9 == ( m_nWorldVAR[ WORLD_VAR_TIME  ] % 10 ) ) {
				this->Save_WorldVAR ();
			}
		}
	}

	void Reset_WorldVAR (BYTE *pVAR)
	{
		::CopyMemory( m_pVAR, pVAR, sizeof( tagWorldVAR ) );

		short nYear, nMonth, nDay;
		nYear  = (short)( m_dwAccTIME / TIME_PER_YEAR );
		nMonth = (short)( (m_dwAccTIME - (nYear*TIME_PER_YEAR) ) / TIME_PER_MONTH + 1 );
		nDay   = (short)( (m_dwAccTIME - (nYear*TIME_PER_YEAR) - TIME_PER_MONTH*(nMonth-1) ) / 160 + 1 );

		m_nWorldVAR[ WORLD_VAR_YEAR  ] = nYear;
		m_nWorldVAR[ WORLD_VAR_MONTH ] = nMonth;
		m_nWorldVAR[ WORLD_VAR_DAY   ] = nDay;
		m_nWorldVAR[ WORLD_VAR_TIME  ] = (short)( m_dwAccTIME % TIME_PER_MONTH );
	}

	int	 Get_WorldVAR (short nVarIDX)
	{
		return m_nWorldVAR[ nVarIDX ];
	}
	virtual void Set_WorldVAR (short nVarIDX, short nValue)
	{
		m_nWorldVAR[ nVarIDX ] = nValue;
	}

	// ���� ���������� ��ӹ���~~~
	virtual bool Save_WorldVAR ()	{	return true;	}
} ;

//-------------------------------------------------------------------------------------------------
#endif

