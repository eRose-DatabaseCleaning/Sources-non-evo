
#include <windows.h>
#include <assert.h>
#include "classTIME.h"

//-------------------------------------------------------------------------------------------------
void classTIME::InitStartTime ()
{
	m_i64StartTIME = GetCurrentAbsMilliSecond ();
}

//-------------------------------------------------------------------------------------------------
DWORD classTIME::GetPassAbsMilliSecond (void)
{
	INT64 i64 = GetCurrentAbsMilliSecond();

	return (DWORD)( i64-m_i64StartTIME );
}

//-------------------------------------------------------------------------------------------------
// st��¥�� ���� ��¥�� ���Ѵ�. 1601�� 1�� 1���� ����(0)���� ����� ��¥ ���� ���ش�.
DWORD classTIME::GetAbsDay (SYSTEMTIME &st)
{
	INT64 i64;
	FILETIME fst;

	st.wHour=st.wMinute=st.wSecond=st.wMilliseconds=st.wDayOfWeek=0;
	::SystemTimeToFileTime (&st, &fst);
	i64 = (((INT64)fst.dwHighDateTime) << 32 ) + fst.dwLowDateTime;
	i64 = i64 / ( 864000000000 );
	return (DWORD)i64;
}
// ���� ��¥�� �ý��� Ÿ������ �ٲ۴�.
void classTIME::AbsDayToSystem (DWORD dwABS, SYSTEMTIME &st)
{
	INT64 i64;
	FILETIME fst;

	i64 = dwABS * 864000000000;
	fst.dwHighDateTime = (DWORD)( i64 >> 32 );
	fst.dwLowDateTime  = (DWORD)( i64 & 0xffffffff );
	::FileTimeToSystemTime (&fst, &st);
}


//-------------------------------------------------------------------------------------------------
/*
	32��Ʈ ������ ���� �ʸ� ǥ���ϱ� ������ ǥ�� ������ ����� 
	136�� �ۿ� ���� �ʾ� �ε��� ���� ��¥�� FILETIME�� 1601������
	���� ���ϰ� 2000�� 1�� 1�Ϸ� ��Ҵ�. 
	2136�� ������ �� ������� �ִ�...
*/
// 1day = 60sec * 60min * 24hour = 86400 sec
// st ��¥�� ���� �ʸ� ���Ѵ�. 2000�� 1�� 1�� ��������
DWORD classTIME::GetAbsSecond (SYSTEMTIME &st)
{
	INT64 i64;
	FILETIME fst;

	::SystemTimeToFileTime (&st, &fst);
	i64 = (((INT64)fst.dwHighDateTime) << 32 ) + fst.dwLowDateTime;
	// �ʴ����� ȯ�� 2000�� 1�� 1�� ���� �������� �ٲ۴�.
	i64 = i64 / 10000000 - (INT64)145731 * 86400;

	return (DWORD)i64;
}

// �����ʸ� �ý��� Ÿ������ �ٲ۴�
void classTIME::AbsSecondToSystem (DWORD dwABS, SYSTEMTIME &st)
{
	INT64 i64;
	FILETIME fst;

	i64 = (dwABS + (INT64)145731 * 86400) * 10000000;
	fst.dwHighDateTime = (DWORD)( i64 >> 32 );
	fst.dwLowDateTime  = (DWORD)( i64 & 0xffffffff );

	::FileTimeToSystemTime (&fst, &st);
}

//-------------------------------------------------------------------------------------------------
DWORD classTIME::GetCurrentAbsSecond (void)
{
	SYSTEMTIME st;

	::GetLocalTime( &st );
	return GetAbsSecond( st );
}

//-------------------------------------------------------------------------------------------------
DWORD classTIME::GetPassAbsSecond (SYSTEMTIME &st)
{
	return ( GetCurrentAbsSecond() - GetAbsSecond( st ) );
}


//-------------------------------------------------------------------------------------------------
// 1day = 60sec * 60min * 24hour = 86400 sec
// st ��¥�� ���� �ʸ� ���Ѵ�. 1601�� 1�� 1���� ����(0)���� ����� 
// 1/100�ʸ� ���Ѵ�.
INT64 classTIME::GetAbsMilliSecond (SYSTEMTIME &st)
{
	INT64 i64;
	FILETIME fst;

	::SystemTimeToFileTime (&st, &fst);
	i64 = (((INT64)fst.dwHighDateTime) << 32 ) + fst.dwLowDateTime;
	// 1/100�ʴ����� ȯ�� 2000�� 1�� 1�� ���� �������� �ٲ۴�.

//	i64 = i64 / 10000000 - (INT64)145731 * 86400;
	i64 = i64 / 100000;

//	printf (" %f \n", (float)i64);

	return i64;
}
INT64 classTIME::GetCurrentAbsMilliSecond (void)
{
	SYSTEMTIME st;

	::GetLocalTime( &st );
	return GetAbsMilliSecond( st );
}

//-------------------------------------------------------------------------------------------------
CTimer::CTimer (HWND hWND, UINT_PTR uiEventID, UINT uiElapse, TIMERPROC lpTimerFunc)
{
	assert( uiEventID );

	m_hWND = hWND;
	m_uiTimerID = 0;
	m_uiEventID   = uiEventID  ;
	m_uiElapse    = uiElapse   ;
	m_lpTimerFunc = lpTimerFunc;
}
CTimer::~CTimer ()
{
	this->Stop ();
}

//-------------------------------------------------------------------------------------------------
bool CTimer::Start ()
{
	if ( !m_uiTimerID ) {
		m_uiTimerID = ::SetTimer( m_hWND, m_uiEventID, m_uiElapse, m_lpTimerFunc );
	}

	return ( m_uiTimerID != 0 );
}

void CTimer::Stop ()
{
	if ( m_uiTimerID ) {
		::KillTimer( m_hWND, m_uiEventID /* m_uiTimerID */ );
		m_uiTimerID = NULL;
	}
}

//-------------------------------------------------------------------------------------------------
