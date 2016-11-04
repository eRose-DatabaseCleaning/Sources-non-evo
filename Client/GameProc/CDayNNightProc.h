#ifndef _DAYNNIGHTPROC_
#define _DAYNNIGHTPROC_

#include <list>


class CEffect;

//----------------------------------------------------------------------------------------------------
/// @brief ���� ��,�� ���� Ÿ��
//----------------------------------------------------------------------------------------------------

enum enumDAYNNIGHT
{
	DN_DAY = 0,	
	DN_NIGHT,	
	DN_MAX,
};


//----------------------------------------------------------------------------------------------------
/// @brief ���� �ð�Ÿ��..
//----------------------------------------------------------------------------------------------------

enum enumTIME_DAYNNIGHT
{
	TIME_DN_MORNING = 0,
	TIME_DN_DAY,
	TIME_DN_EVENING,
	TIME_DN_NIGHT,	
	TIME_DN_MAX,
};


//----------------------------------------------------------------------------------------------------
/// @brief ����Ʈ Ÿ��
//----------------------------------------------------------------------------------------------------

enum enumCOLORTYPE
{
	BG_AMBIENT = 0,
	CHAR_AMBIENT,
	CHAR_DIFFUSE,
};


//----------------------------------------------------------------------------------------------------
/// @brief �÷� Ÿ��
//----------------------------------------------------------------------------------------------------

enum enumDAYCOLOR
{
	COLOR_MORNING = 0,
	COLOR_DAY,
	COLOR_EVENING,
	COLOR_NIGHT,
};



//----------------------------------------------------------------------------------------------------
/// @brief Managing day and night change..
//----------------------------------------------------------------------------------------------------

class CDayNNightProc
{
private:
	//----------------------------------------------------------------------------------------------------
	/// Effect list effected by changing day and night
	//----------------------------------------------------------------------------------------------------
	std::list< CEffect* >	m_EffectList;
	int a;


	//----------------------------------------------------------------------------------------------------
	/// ���� �ð�����..
	//----------------------------------------------------------------------------------------------------
	int						m_iZoneNO;
	DWORD					m_dwWorldTime;		/// 0 ~ 2880
	DWORD					m_dwZoneTime;		/// World time / zone time value
	int						m_iZoneTimeCycle;	/// World time per a day
	
	DWORD					m_dwStartWorldTime;
	DWORD					m_dwElapsedWorldTime;			/// ������� ����� ���� Ÿ��
		


	//----------------------------------------------------------------------------------------------------
	/// ���� �ð���ȭ ��ġ
	//----------------------------------------------------------------------------------------------------
	float					m_fTimeRateValue;


	int						m_iDAYENVIR_TICK;

	//----------------------------------------------------------------------------------------------------
	/// �㳷 ��ȭ ������ ���� �ð�������
	//----------------------------------------------------------------------------------------------------
	int						m_iChangeStep;					/// ��ȭ �ܰ�
	float					m_fChangeValue;					/// ���� �ܰ躰 ���밪	
	DWORD					m_dwStartTime;					/// ��ȭ ���� �ð�
	int						m_iCurrentState;				/// ���� �㳷�� ����( �ð� �� )
	int						m_iCurrentTimeState;			/// ���� �㳷�� ����( ���� ������ �ϴ� ����.. ��� ���� ���� )
	int						m_iCurrentSkyNO;				/// ���� sky �ε���
	DWORD					m_dwElapsedTime;				/// ����� �ð�

	float					m_fBlendFactor;					/// ���� ����

	DWORD					m_dwAddiditionalTime;			/// �ð� ��ȭ �߰��� ���� ���ӽ� ���� �帥�ð��� �����ش�.

public:
	CDayNNightProc();
	~CDayNNightProc();

	//----------------------------------------------------------------------------------------------------
	/// ������
	//----------------------------------------------------------------------------------------------------
	int						GetHeartsYear();
	int						GetHeartsMonth();
	int						GetHeartsDay();



	DWORD					GetWorldTime()			{ return m_dwWorldTime; }
	DWORD					GetElapsedWorldTime()	{ return m_dwElapsedWorldTime; }
	DWORD					GetZoneTime()			{ return m_dwZoneTime; }
	float					GetBlendFactor()		{ return m_fBlendFactor; }


	void					SetWorldTime( int iZoneNO, DWORD dwWorldTime );
	int						GetCurrentTimeState();
	bool					IsDay(){ if( m_iCurrentState == TIME_DN_MORNING || m_iCurrentState == TIME_DN_DAY ) return true; return false; }

	bool					CheckUpdateEnvironment();


	void					SetGlobalIllumination( float fBlendRatio );
	D3DXCOLOR				GetColorStringFromSTB( int iType, int iState );

	bool					PushEffect( CEffect* pEffect );
	void					DeleteEffectFromList( CEffect* pEffect );

	void					Start( int iCurrentSkyNO, int iCurrentState, int iChangeStep = 20 );
	void					Proc();

	void					ShowAllEffect( bool bShow );


	void					SetDayEnviTick( int iTick ){ m_iDAYENVIR_TICK = iTick; }
	void					ResetDayEnviTick();
};

extern CDayNNightProc		g_DayNNightProc;


extern DWORD Get_WorldPassTIME();
extern short Get_WorldTIME ();

#endif //_DAYNNIGHTPROC_
