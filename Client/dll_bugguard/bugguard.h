#ifndef	__SN_DEBUG_H
#define	__SN_DEBUG_H
//-------------------------------------------------------------------------------------------------

#ifdef	DLL_BUGGUARD_EXPORTS
	#define DLL_BUGGUARD	__declspec(dllexport)
#else
	#define DLL_BUGGUARD	__declspec(dllimport)
#endif


/*
	1.	LONG __stdcall UnhandledExceptionFilter ( EXCEPTION_POINTERS* pExPtrs);
		�Լ��� �ۼ�. ���ϰ��� MSDN ���� !!!
		�Լ� ���ο��� �Ʒ��� �Լ��� ȣ���Ͽ� ����� ��´�.

		LPCSTR	GetExceptionReason	( EXCEPTION_POINTERS* pExPtrs );
		LPCSTR	GetRegisters		( EXCEPTION_POINTERS* pExPtrs );
		LPCSTR	GetFirstCallStack	( EXCEPTION_POINTERS* pExPtrs );
		LPCSTR	GetNextCallStack	( EXCEPTION_POINTERS* pExPtrs );

	2.	SetUnhandledExceptionFilter (UnhandledExceptionFilter); ȣ��.

	3.	��.


	Ex)
	LONG __stdcall UnhandledExceptionFilter( EXCEPTION_POINTERS* pExPtrs)
	{
		// Program Version:		���� �˾Ƽ�...
		// Computer Name:		"
		// Time:				"

		OutputDebugString ( GetExceptionReason( pExPtrs ) );
		OutputDebugString ( GetRegisters	  ( pExPtrs ) );

		LPCSTR lpCStr;

		lpCStr = GetFirstCallStack ( pExPtrs );
		if ( lpCStr ) {
			OutputDebugString ( lpCStr );

			while ( lpCStr = GetNextCallStack (pExPtrs ) )
				OutputDebugString ( lpCStr );
		}

		return EXCEPTION_EXECUTE_HANDLER;
	}
*/
///*
LPCSTR DLL_BUGGUARD __stdcall GetExceptionReason( EXCEPTION_POINTERS* pExPtrs );
LPCSTR DLL_BUGGUARD __stdcall GetRegisters		( EXCEPTION_POINTERS* pExPtrs );
LPCSTR DLL_BUGGUARD __stdcall GetFirstCallStack ( EXCEPTION_POINTERS* pExPtrs );
LPCSTR DLL_BUGGUARD __stdcall GetNextCallStack	( EXCEPTION_POINTERS* pExPtrs );
//*/
/*
LPCSTR DLL_BUGGUARD __stdcall _		( EXCEPTION_POINTERS* pExPtrs );	// GetExceptionReason
LPCSTR DLL_BUGGUARD __stdcall __	( EXCEPTION_POINTERS* pExPtrs );	// GetRegisters
LPCSTR DLL_BUGGUARD __stdcall ___	( EXCEPTION_POINTERS* pExPtrs );	// GetFirstCallStack
LPCSTR DLL_BUGGUARD __stdcall ____	( EXCEPTION_POINTERS* pExPtrs );	// GetNextCallStack


#define	GetExceptionReason		_
#define GetRegisters			__
#define GetFirstCallStack		___
#define GetNextCallStack		____
*/

//-------------------------------------------------------------------------------------------------
#endif

