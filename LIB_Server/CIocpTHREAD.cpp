
#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>
#include "classLOG.h"
#include "CIocpTHREAD.h"

//-------------------------------------------------------------------------------------------------
CIocpTHREAD::CIocpTHREAD (bool bCreateSuspended) : classTHREAD( bCreateSuspended )
{
	;
}

//-------------------------------------------------------------------------------------------------
void CIocpTHREAD::Execute ()
{
	DWORD		 dwBytesIO;
    DWORD        dwCompletionKey;
    LPOVERLAPPED lpOverlapped;

	g_LOG.CS_ODS(0xffff, ">  > >> CIocpTHREAD::Execute() ThreadID: %d(0x%x),  WorkerNO: %d \n", this->ThreadID, this->ThreadID, m_iThreadNO);

	while ( !this->Terminated )
	{
		if ( 0 == ::GetQueuedCompletionStatus( m_hIOCP, &dwBytesIO, &dwCompletionKey, (LPOVERLAPPED *)&lpOverlapped, INFINITE) ) {
			/*
			If *lpOverlapped is NULL and the function does not dequeue a completion packet from the completion port, 
			the RETURN VALUE IS ZERO.
			The function does not store information in the variables pointed to by 
			the lpNumberOfBytesTransferred and lpCompletionKey parameters.

			If *lpOverlapped is not NULL and the function dequeues a completion packet 
			for a failed I/O operation from the completion port, the RETURN VALUSE IS ZERO.
			return value is zero.
			The function stores information in the variables pointed to by 
			lpNumberOfBytesTransferred, lpCompletionKey, and lpOverlapped. 
			*/
			if ( NULL != lpOverlapped ) {
				// ���� ����
				DWORD dwErrCODE = ::GetLastError();
				
				//
				// ErrorCode == 64 :: ������ ��Ʈ��ũ �̸��� ���̻� ����Ҽ� �����ϴ� == Ŭ���̾�Ʈ ������ ����.
				//
				/* if ( dwErrCODE != 64 ) {
					LPVOID lpMsgBuf;
					if ( ::FormatMessage( 
						FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						dwErrCODE,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL ))
					{
						// Display the string.
						// MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
						g_LOG.CS_ODS( 0xffff, "$$$$ ERROR[%d]:: socketIdx: %d, %s\n", dwErrCODE, dwCompletionKey, lpMsgBuf );
						::LocalFree( lpMsgBuf );
					}
				} */
				
				STATUS_ReturnFALSE( lpOverlapped, dwCompletionKey );
				continue;
			}

			// �ý��� ���� !!!
            g_LOG.CS_ODS(0xffff, "* ERROR:: WorkerThread ID: %d(0x%x),  WorkerNO: %d : lpOverlapped == NULL \n", this->ThreadID, this->ThreadID, m_iThreadNO);
			break;
		}

		if ( 0 == dwBytesIO ) {
			STATUS_ReturnFALSE( lpOverlapped, dwCompletionKey );
			continue;
		}
        if ( dwBytesIO == -1 ) {
			// 0 == dwCompletionKey
			// IOCP is freed ?
			// g_LOG.CS_ODS(0xffff, "* Break:: WorkerThread ID: %d(0x%x),  WorkerNO: %d : dwBytesIO == -1 \n", this->ThreadID, this->ThreadID, m_iThreadNO);
            break;
        } 
		
		STATUS_ReturnTRUE( lpOverlapped, dwCompletionKey, dwBytesIO );
	}

	g_LOG.CS_ODS(0xffff, "<  < << CIocpTHREAD::Execute() ThreadID: %d(0x%x),  WorkerNO: %d \n", this->ThreadID, this->ThreadID, m_iThreadNO);
}


//-------------------------------------------------------------------------------------------------
bool CIocpTHREAD::Init (HANDLE hIOCP, int iThreadNO)
{
    m_hIOCP = hIOCP;
    m_iThreadNO = iThreadNO;

    return true;
}
void CIocpTHREAD::Free (void)
{
    this->Terminate ();

	g_LOG.CS_ODS(0xffff, ">>> CIocpTHREAD::Free ThreadID: %d(0x%x),  WorkerNO: %d \n", this->ThreadID, this->ThreadID, m_iThreadNO);
	do {
        if ( 0 == ::PostQueuedCompletionStatus( m_hIOCP, -1, 0, NULL) ) {
            g_LOG.CS_ODS(0xffff, "ThreadWORKER::Free() PostQueuedCompletionStatus() return 0, LastERROR: %d(0x%x)", GetLastError(), GetLastError());
			break;
        }
	} while( !this->IsFinished() ) ;
	g_LOG.CS_ODS(0xffff, "<<< CIocpTHREAD::Free ThreadID: %d(0x%x),  WorkerNO: %d \n", this->ThreadID, this->ThreadID, m_iThreadNO);

/*    
    // ** ���� !!!
    // m_hIOCP�� �����ϴ� �ٸ� ��ũ�����尡 ::GetQueuedCompletionStatus���� ����
    // ��ȣ�� �޾Ƽ� ó���Ǿ� �����ִ�.
    // �� �Լ��� ȣ��Ǿ� ������ this �����尡 Terminate���� �ʴ´�..
    if ( 0 == ::PostQueuedCompletionStatus( m_hIOCP, -1, 0, NULL) )
    {
        Log_String( 0xffff, "ThreadWORKER::Free() PostQueuedCompletionStatus() return 0, LastERROR: %d(0x%x)", GetLastError(), GetLastError());
    }
*/
/*
    this->WaitFor ();
    m_hIOCP = NULL;
*/
}


//-------------------------------------------------------------------------------------------------
/*
//void ThreadWORKER::STATUS_ReturnTRUE  (LPOVERLAPPED lpOverlapped, DWORD dwCompletionKey, DWORD dwBytesIO)
//{
//    if ( dwBytesIO == 0 ) {
//        // ���� ����
//        Log_String ("WorkerThread %d: [ %s ] dwBytesIO == 0, Close Socket () :: IONode->PoolIndex: %d \n", pParam->wThreadNO , (lpOverlapped->m_IOmode == ioREAD) ? "ioREAD" : "ioWRITE", pParam->wThreadNO, lpOverlapped->m_pPoolNode->DATA.m_iPoolIndex);
//
//        // ioWRITE �ϰ�쿡�� pUSER->m_SendList�� ��尡 �̹� ��ϵǾ� �־� SubUser()���� Ǯ������ ioREAD�� ����...
//        if ( lpOverlapped->m_IOmode == ioREAD ) {
//            g_pIO_DATA->ReleaseIONode ( lpOverlapped->m_pPoolNode );
//        }
//
//        pUSER->LockUSER ();
//        if ( pUSER->m_Socket != INVALID_SOCKET ) {
//            pParam->pIOCP->_CloseSocket ( pUSER->m_Socket );
//            pUSER->m_Socket = INVALID_SOCKET;
//            g_CUserLIST.SubUser ( pUSER->m_pPoolNode );			// WorkerThreadFunc
//        }
//        pUSER->UnlockUSER ();
//    } else
//    if ( pUSER->m_Socket != INVALID_SOCKET ) {
//        // ** pUSER�� ������ ����� ���¿����� �̰����� �´� �Ѥ�;
//
//        // ���� ó�� !!!
//        lpOverlapped->m_dwIOBytes += dwBytesIO;
//
//        switch ( lpOverlapped->m_IOmode ) {
//            case ioREAD :
//                pUSER->Recv_Complete ( lpOverlapped->m_pPoolNode );
//                break;
//            case ioWRITE :	// ������ �Ϸ� !!!
//                pUSER->Send_Complete ( lpOverlapped->m_pPoolNode );
//                break;
//        }
//    }
//}
//-------------------------------------------------------------------------------------------------
//void ThreadWORKER::STATUS_ReturnFALSE (LPOVERLAPPED lpOverlapped, DWORD dwCompletionKey)
//{
//    Log_String (0xffff, "WorkerThread %d: [ %s ] bResult == FALSE, Close Socket () :: IONode->PoolIndex: %d \n", m_iThreadNO, (lpOverlapped->m_IOmode == ioREAD) ? "ioREAD" : "ioWRITE", lpOverlapped->m_pPoolNode->DATA.m_iPoolIndex);
//
//    // ioWRITE �ϰ�쿡�� pUSER->m_SendList�� ��尡 �̹� ��ϵǾ� �־� SubUser()���� Ǯ������ ioREAD�� ����...
//    if ( lpOverlapped->m_IOmode == ioREAD ) {
//        g_pIO_DATA->ReleaseIONode ( lpOverlapped->m_pPoolNode );
//    }
//
//    pUSER->LockUSER ();
//    if ( pUSER->m_Socket != INVALID_SOCKET ) {
//        pParam->pIOCP->_CloseSocket ( pUSER->m_Socket );
//        pUSER->m_Socket = INVALID_SOCKET;
//        g_CUserLIST.SubUser ( pUSER->m_pPoolNode );		// WorkerThreadFunc
//    }
//    pUSER->UnlockUSER ();
//}
*/

//-------------------------------------------------------------------------------------------------
