#ifndef	__CLASSTIME_H
#define	__CLASSTIME_H
//-------------------------------------------------------------------------------------------------

class classTIME {
private:
	INT64	m_i64StartTIME;

public :
	void	InitStartTime ();
	// InitStartTime() ȣ���� ������� ����� 1/100 �ʸ� ���Ѵ�.
	// InitStartTime() ȣ���� �ִ� 497 Day�� ����ϸ� �ȵȴ�.
	DWORD	GetPassAbsMilliSecond (void);

	// st��¥�� ���� ��¥�� ���Ѵ�. 1601�� 1�� 1���� ����(0)���� ����� ��¥ ���� ���ش�.
	static DWORD	GetAbsDay (SYSTEMTIME &st);
	// ���� ��¥�� �ý��� Ÿ������ �ٲ۴�.
	static void		AbsDayToSystem (DWORD dwABS, SYSTEMTIME &st);

	// st ��¥�� ���� �ʸ� ���Ѵ�. 2000�� 1�� 1�� ��������
	static DWORD	GetAbsSecond (SYSTEMTIME &st);

	// �����ʸ� �ý��� Ÿ������ �ٲ۴�
	static void		AbsSecondToSystem (DWORD dwABS, SYSTEMTIME &st);

	static DWORD	GetCurrentAbsSecond (void);
	static DWORD	GetPassAbsSecond (SYSTEMTIME &st);

	static INT64	GetAbsMilliSecond (SYSTEMTIME &st);
	static INT64	GetCurrentAbsMilliSecond (void);
} ;

class CTimer {
protected:
	HWND		m_hWND;
	UINT_PTR	m_uiTimerID;
	UINT_PTR	m_uiEventID;
	UINT		m_uiElapse;
	TIMERPROC	m_lpTimerFunc;

public :
	CTimer (HWND hWND, UINT_PTR uiEventID, UINT uiElapse, TIMERPROC lpTimerFunc);
	virtual ~CTimer ();

	bool Start ();
	void Stop ();

	UINT_PTR GetEventID ()	{	return m_uiEventID;		}
} ;

//-------------------------------------------------------------------------------------------------
#endif
