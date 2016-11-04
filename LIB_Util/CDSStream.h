#ifndef	__DLL_DSSTREAM_H
#define	__DLL_DSSTREAM_H
#include <MMSystem.h>
#include <DSound.h>
#include "classMP3.h"
#include "classWAV.h"
//-------------------------------------------------------------------------------------------------

#ifdef	DLL_ICARUS_EXPORTS
	#define DLL_DSSTREAM	__declspec(dllexport)
#else
	#define DLL_DSSTREAM	//__declspec(dllimport)
#endif

//-------------------------------------------------------------------------------------------------

// #define NUM_PLAY_NOTIFICATIONS		32

class DLL_DSSTREAM classDSOUNDSTREAM {
private :
	bool					m_bMP3Stream;
	WORD					m_wBitsPerSample;
	DWORD					m_dwReadData;
	short					m_nNumPlayNotifications;		// ��ü ���۸� ����� ������ �о� ���ϰ��ΰ�?

	classMP3			   *m_pClassMP3;
	classWAV			   *m_pClassWAV;

    LPDIRECTSOUNDBUFFER		m_pDSBuffer;
	LPDIRECTSOUNDNOTIFY		m_pDSNotify;

//	DSBPOSITIONNOTIFY		m_sDSBPosNotify[ NUM_PLAY_NOTIFICATIONS + 1 ];  
	DSBPOSITIONNOTIFY	   *m_pDSBPosNotify;

	DWORD					m_dwBufferSize;
	DWORD					m_dwNotifySize;
	DWORD					m_dwNextWriteOffset;
	DWORD					m_dwProgress;
	DWORD					m_dwLastPos;
	bool					m_bFoundEnd;
	bool					m_bLooped;

	HRESULT CreateStreamBuffer (LPDIRECTSOUND pDirectSound, WAVEFORMATEX *pWFX, int iBufferingSec=8, short nNumPlayNotifications=32);
	HRESULT RestoreBuffers	( void );
	HRESULT FillBuffer		( void );
	HRESULT WriteToBuffer	( void* pbBuffer, DWORD dwBufferLength );
	HRESULT UpdateProgress	( void );

public :
	HANDLE					m_hNotificationEvents[ 2 ];

	classDSOUNDSTREAM ();
	~classDSOUNDSTREAM ();

	void	_Init (void);
	void	_Free (void);

	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// iBufferingSec			:: �Ҵ��� ������ ����� ����.. �Ҵ��� �޸��� ���� ����.
	// nNumPlayNotifications	:: ��ü ���۸� ����� ��ó�� �о���� ���ΰ� .. ��ũ �׼��� Ƚ���� ����.
	//
	bool	OpenMP3StreamFile (char *szMP3File, LPDIRECTSOUND pDirectSound, int iBufferingSec, short nNumPlayNotifications, bool bAlreadyCoInitialized);
	bool	OpenWAVStreamFile (char *szWAVFile, LPDIRECTSOUND pDirectSound, int iBufferingSec, short nNumPlayNotifications, long lFilePtr);

	void	SetVolume (long lVolume);
	HRESULT PlayStreamBuffer	( bool bLooped );
	void	StopStreamBuffer	( void );
	HRESULT HandleNotification	( void );
} ;


//-------------------------------------------------------------------------------------------------
#endif


