
#include <winsock2.H>
#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>
#include "LIB_Util.h"
#include "classLOG.h"
#include "classUTIL.h"
#include "classIOCP.h"
#include "iocpSOCKET.h"

//-------------------------------------------------------------------------------------------------
//
//  IOCP Client socket class
//
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
iocpSOCKET::iocpSOCKET () : m_csRecvQ( 4000 )
{
	this->Init_SCOKET ();
}
iocpSOCKET::~iocpSOCKET ()
{
	_ASSERT( 0 == this->m_iSocketIDX );
	// classIOCP::DelClientSocket( this->m_iSocketIDX );		// @~iocpSOCKET()
	this->Clear_LIST ();
}


void iocpSOCKET::CloseSocket (void)
{
    if ( m_Socket != INVALID_SOCKET ) {
		struct linger li = {0, 0};	// Default: SO_DONTLINGER

		::shutdown   (m_Socket, SD_BOTH);
		::setsockopt (m_Socket, SOL_SOCKET, SO_LINGER, (char *)&li, sizeof(li));
		::closesocket(m_Socket);

        m_Socket    = INVALID_SOCKET;
	    m_bWritable = false;
    }
}

//-------------------------------------------------------------------------------------------------
void iocpSOCKET::Clear_LIST (void)
{
    classDLLNODE<tagIO_DATA> *pNode;
    
    m_csRecvQ.Lock ();
    {
		pNode = m_RecvList.GetHeadNode();
        while( pNode ) {
            m_RecvList.DeleteNode( pNode );
			this->Free_RecvIODATA( &pNode->DATA );

			pNode = m_RecvList.GetHeadNode();
        }
    }
    m_csRecvQ.Unlock ();

    m_csSendQ.Lock ();
    {
		pNode = m_SendList.GetHeadNode();
        while( pNode ) {
            m_SendList.DeleteNode( pNode );
            this->Free_SendIODATA ( &pNode->DATA );

			pNode = m_SendList.GetHeadNode();
        }
    }
    m_csSendQ.Unlock ();
}

//-------------------------------------------------------------------------------------------------
// pRecvNode�� �̾� �ޱ�.
ePacketRECV iocpSOCKET::Recv_Continue (tagIO_DATA *pRecvDATA)
{
	_ASSERT( pRecvDATA->m_pCPacket->GetRefCnt() == 1 );

    if ( 0 == ::ReadFile( (HANDLE)m_Socket,										// HANDLE hFile,                // handle to file
                    &pRecvDATA->m_pCPacket->m_pDATA[ pRecvDATA->m_dwIOBytes ],  // LPVOID lpBuffer,             // data buffer
                    MAX_PACKET_SIZE-pRecvDATA->m_dwIOBytes,						// DWORD nNumberOfBytesToRead,  // number of bytes to read
                    NULL,														// LPDWORD lpNumberOfBytesRead, // number of bytes read
                    (LPOVERLAPPED)pRecvDATA ) ) 								// LPOVERLAPPED lpOverlapped    // overlapped buffer
    {
        // Function failed ..
        if ( ERROR_IO_PENDING != ::GetLastError () ) {
			/*
            DWORD dwCode = ::GetLastError ();
            g_LOG.CS_ODS(0xffff, "$$$$$$$  SocketIDX: %d : ERROR[ %d:%s ] ::: ReadFile \n", 
					this->m_iSocketIDX, dwCode, CUtil::GetLastErrorMsg( dwCode ) );
			*/
            // false �����ϸ� ���� ����...
			return eRESULT_PACKET_DISCONNECT;//false;
        } 
		/* 
		else 
			g_LOG.CS_ODS(0xfff, "    %d  ReadFile pending error ..\n", this->m_iSocketIDX);
		*/
    }

	return eRESULT_PACKET_OK;//true;
}


//-------------------------------------------------------------------------------------------------
// ���� �ޱ�.
ePacketRECV iocpSOCKET::Recv_Start (void)
{
    classDLLNODE<tagIO_DATA> *pRecvNODE;

    pRecvNODE = this->Alloc_RecvIODATA ();
    if ( NULL == pRecvNODE )
        return eRESULT_PACKET_DISCONNECT;//false;

	_ASSERT( pRecvNODE->DATA.m_IOmode    == ioREAD );
	_ASSERT( pRecvNODE->DATA.m_dwIOBytes == 0 );

    return this->Recv_Continue( &pRecvNODE->DATA );
}


//-------------------------------------------------------------------------------------------------
// CThreadWORKER::STATUS_ReturnTRUE () ������ ȣ��ȴ�.
ePacketRECV iocpSOCKET::Recv_Complete (tagIO_DATA *pRecvDATA)
{
	ePacketRECV eResult;

	this->LockSOCKET ();

    if ( pRecvDATA->m_dwIOBytes < sizeof(t_PACKETHEADER) ) {
		// ���ڵ� �ȵ� ��Ŷ�� ũ��� 0�̴� !!!
		_ASSERT( 0 == pRecvDATA->m_pCPacket->GetLength() );
		// �ּ� ũ���� ��Ŷ �ޱ�...
        eResult = this->Recv_Continue( pRecvDATA );	// �̾� �ޱ�.
		goto _JUMP_RETURN;
	}

	if ( 0 == pRecvDATA->m_pCPacket->GetLength() ) {
		// ���ڵ� �ȵǾ� �ִٸ�...
		pRecvDATA->m_pCPacket->SetLength( this->D_RecvH( &pRecvDATA->m_pCPacket->m_HEADER ) );
		if ( 0 == pRecvDATA->m_pCPacket->GetLength() ) {
			this->UnlockSOCKET ();
			this->Free_RecvIODATA( pRecvDATA );

			// �� ����Ʈ�� ip ���...
			g_LOG.CS_ODS( 0xffff, "*** ERROR: Decode recv packet header1, IP[ %s ]\n", this->m_IP.Get() );
			return eRESULT_PACKET_BLOCK;//false;
		}
	}

	_ASSERT( pRecvDATA->m_pCPacket->GetLength() >= sizeof(t_PACKETHEADER) );
	
	if ( (short)pRecvDATA->m_dwIOBytes < pRecvDATA->m_pCPacket->GetLength() ) {
        eResult = this->Recv_Continue( pRecvDATA );	// �̾� �ޱ�.
		goto _JUMP_RETURN;
    } else
    if ( (short)pRecvDATA->m_dwIOBytes == pRecvDATA->m_pCPacket->GetLength() ) {
		this->UnlockSOCKET ();
		if ( !this->Recv_Done( pRecvDATA ) )		// Free_RecvIODATA( pRecvDATA ); <-- Recv_done���� ȣ��Ǿ��
			return eRESULT_PACKET_DISCONNECT;//false;
        return this->Recv_Start ();	// RecvComplete
    }

    // ���Ŀ� ��Ŷ �и�.
    t_PACKETHEADER *pHEADER;
    short	nRemainBytes, nPacketSIZE;

	_ASSERT( pRecvDATA->m_dwIOBytes > pRecvDATA->m_pCPacket->GetLength() );

    nRemainBytes = (short)pRecvDATA->m_dwIOBytes - pRecvDATA->m_pCPacket->GetLength() ;
    pHEADER		 = (t_PACKETHEADER*) &pRecvDATA->m_pCPacket->m_pDATA[ pRecvDATA->m_pCPacket->GetLength() ];
	nPacketSIZE  = 0;
	while ( nRemainBytes >= sizeof(t_PACKETHEADER) ) {
		if ( 0 == nPacketSIZE ) {
			nPacketSIZE = this->D_RecvH( pHEADER );
			if ( 0 == nPacketSIZE ) {
				this->UnlockSOCKET ();
				this->Free_RecvIODATA( pRecvDATA );
				g_LOG.CS_ODS( 0xffff, "*** ERROR: Decode recv packet header2, IP[ %s ]\n", this->m_IP.Get() );

				// �� ����Ʈ�� ip ���...
				return eRESULT_PACKET_BLOCK;//false;
			}
		}

		if ( nRemainBytes == nPacketSIZE ) {
			this->UnlockSOCKET ();
			if ( !this->Recv_Done( pRecvDATA ) )		// �ϼ� ��Ŷ �߰�, Free_RecvIODATA( pRecvDATA );<-- Recv_done���� ȣ��Ǿ��
				return eRESULT_PACKET_DISCONNECT;//false;
			return this->Recv_Start ();					// ���� �ޱ�. :: RecvComplete
		}

		if ( nRemainBytes > nPacketSIZE ) {
			// �߰� ��Ŷ�� �ִ�.
			nRemainBytes -= nPacketSIZE;
			pHEADER     = (t_PACKETHEADER*) ( pHEADER->m_pDATA + nPacketSIZE );
			nPacketSIZE = 0;
		} else
			break;
	}

    // ���ڶ� �κ��� �ٽ� �о���� ����Ÿ ����.
    classDLLNODE<tagIO_DATA> *pNewNODE;
    pNewNODE = this->Alloc_RecvIODATA ();
	if ( pNewNODE ) {
		if ( nRemainBytes >= sizeof(t_PACKETHEADER) ) {
			// Header�� Decoding �Ǿ���..
			pNewNODE->DATA.m_pCPacket->SetLength( this->P_Length( pHEADER ) );
		}
		pNewNODE->DATA.m_dwIOBytes = nRemainBytes;
		::CopyMemory (pNewNODE->DATA.m_pCPacket->m_pDATA, pHEADER, nRemainBytes);

		// �պκ��� �ϼ� ��Ŷ���.
		pRecvDATA->m_dwIOBytes -= nRemainBytes;

		this->UnlockSOCKET ();
		if ( !this->Recv_Done( pRecvDATA ) ) {
			// 2003. 11. 04 �ؿ� �Լ� �߰�... ���Ծ �޸� ��Ⱦ���..
			this->Free_RecvIODATA( &pNewNODE->DATA );
			return eRESULT_PACKET_DISCONNECT;//false;
		}
		return this->Recv_Continue( &pNewNODE->DATA );	    			// �̾� �ޱ�.
	}
	
	eResult = eRESULT_PACKET_DISCONNECT;//false;

_JUMP_RETURN :
	this->UnlockSOCKET ();
	return eResult;
}


//-------------------------------------------------------------------------------------------------
// pSendNode�� �̾� ������.
bool iocpSOCKET::Send_Continue (tagIO_DATA *pSendDATA)
{
	// 2004. 10. 25 �Ʒ� assert �ɸ�...
	// _ASSERT( pSendDATA->m_pCPacket->GetLength() > pSendDATA->m_dwIOBytes );
	if ( pSendDATA->m_dwIOBytes >= pSendDATA->m_pCPacket->GetLength() ) {
		g_LOG.CS_ODS( 0xffff, ">>ERROR:: Sending packet: Len: %d, completed: %d, IP:%s\n", 
					pSendDATA->m_pCPacket->GetLength(), pSendDATA->m_dwIOBytes , this->Get_IP() );
		return false;
	}

	// 2003. 12. 16
	// �ѹ� ������ ���� �Ϸ� �Ǵ���, ������ �߻��Ǿ� ������ ������...
	this->m_bWritable = false;
    if ( 0 == ::WriteFile((HANDLE)m_Socket,											// HANDLE hFile,                    // handle to file
                    &pSendDATA->m_pCPacket->m_pDATA[ pSendDATA->m_dwIOBytes ],	    // LPCVOID lpBuffer,                // data buffer
                    pSendDATA->m_pCPacket->GetLength() - pSendDATA->m_dwIOBytes,		// DWORD nNumberOfBytesToWrite,     // number of bytes to write
                    NULL,															// LPDWORD lpNumberOfBytesWritten,  // number of bytes written
                    (LPOVERLAPPED)pSendDATA )) {									// LPOVERLAPPED lpOverlapped        // overlapped buffer
        // Function failed ..
        if ( ERROR_IO_PENDING != GetLastError () ) {
			/*
            DWORD dwCode = ::GetLastError ();
            g_LOG.CS_ODS(0xffff, "$$$$$$$  SocketIDX: %d : ERROR[ %d:%s ] ::: WriteFile \n", this->m_iSocketIDX, dwCode, CUtil::GetLastErrorMsg( dwCode ));
			*/
            // false �����ϸ� ���� ����...
			return false;
		} /* else 
			g_LOG.CS_ODS(0xfff, "    %d  WriteFile pending error ..\n", this->m_iSocketIDX); */
		/*
		else {
			// 2003. 12. 16
			// ���ÿ� ������ ���°͵� �ƴѵ� ???
			// �ƴϸ� �ٸ� ������ ��Ʈ�� ��뿡 ���� ERROR_IO_PENDING�� �ü��ִٸ�...
			// �̰�� ���� ��Ŷ�� ������ �߾��µ�...
			// #define WSA_IO_PENDING          (ERROR_IO_PENDING)
			// WSASend()��� WSA_IO_PENDING : An overlapped operation was successfully initiated and completion will be indicated at a later time.
		}
		*/
    }
	this->m_dwCheckTIME = ::timeGetTime ();	// ���������� ������ �õ��� �ð�...

    return true;
}


//-------------------------------------------------------------------------------------------------
bool iocpSOCKET::Send_Start (classPACKET *pCPacket)
{
    if ( m_Socket == INVALID_SOCKET )
        return false;

    classDLLNODE<tagIO_DATA> *pSendNODE;
    pSendNODE = this->Alloc_SendIODATA( pCPacket );
    if ( NULL == pSendNODE )
        return false;

	_ASSERT( pSendNODE->DATA.m_pCPacket->GetRefCnt() >= 1 );
	_ASSERT( pSendNODE->DATA.m_pCPacket == pCPacket );
	_ASSERT( pSendNODE->DATA.m_IOmode == ioWRITE );
	_ASSERT( pSendNODE->DATA.m_dwIOBytes == 0 );

	_ASSERT( pSendNODE->DATA.m_pCPacket->GetLength() >= sizeof( t_PACKETHEADER ) );
	_ASSERT( pSendNODE->DATA.m_pCPacket->GetLength() <= MAX_PACKET_SIZE );

    m_csSendQ.Lock ();
    {
        m_SendList.AppendNode( pSendNODE );

        if ( this->m_bWritable ) {
            pSendNODE = m_SendList.GetHeadNode ();
            if ( !this->Send_Continue( &pSendNODE->DATA ) ) {
			    m_csSendQ.Unlock ();
				this->CloseSocket();
				return false;
			}
		} else { 
			int iQedCnt = m_SendList.GetNodeCount();
			if ( iQedCnt > 100 ) {
				// ������ �õ��� �� �������� ���� ��Ŷ�� ������ ���ϰ� �ִ³�...
				// ��Ŷ�� �׳��� �ִٸ� ©���������...
				DWORD dwPassTime = ::timeGetTime() - this->m_dwCheckTIME; 
				if ( dwPassTime >= 60*1000 || iQedCnt > 2500 ) {
					g_LOG.CS_ODS( 0xffff, ">>Sending timeout: packet: %d, time: %d, IP:%s\n", iQedCnt, dwPassTime, this->Get_IP() );
					this->CloseSocket();
				}
			}
		}
    }
    m_csSendQ.Unlock ();
	return true;
}


//-------------------------------------------------------------------------------------------------
// CThreadWORKER::STATUS_ReturnTRUE() ������ ȣ��ȴ�.
bool iocpSOCKET::Send_Complete (tagIO_DATA *pSendDATA)
{
    m_csSendQ.Lock ();
	{
		if ( 0 == this->m_iSocketIDX ) {	// ����ƴ�.
			// 2004. 10. 3... 
			// _ASSERT( pHeadNODE == pSendDATA->m_pNODE ); ���� ���� �߻�...
			// m_SendList.GetNodeCount() == 0�λ��¿��� �̸��� ��� �ͼ� ��~~~
			// this->m_iSocketIDX = 0�̰�, pSendDATA->m_dwIOBytes = 0, pSendDATA->m_pCPacket->GetLength()=10 �̾�����
			// �Ʒ� pSendDATA->m_dwIOBytes == (WORD)pSendDATA->m_pCPacket->GetLength() ������ ����ߴ�.
			// 1. ���ÿ� ���� ��Ŀ �����忡�� IO�߻�
			// 2. �ٸ� ��Ŀ�����忡�� ���� ���� => m_SendList�ʱ�ȭ ��( ���� pSendDATA�� Pool�� �ݳ��� )
			// 3. ���� ��Ŀ�����忡�� �� �Լ��� ����pSendDATA->m_dwIOBytes == (WORD)pSendDATA->m_pCPacket->GetLength() �������.
			// 4. �ٸ� ���Ͽ����� pSendDATA�� �Ҵ�, �ֱ�ȭ��.
			// 5. ���� ��Ŀ������ ��~~~
			m_csSendQ.Unlock ();
			return false;
		}

        this->m_bWritable = true;
		if ( pSendDATA->m_dwIOBytes == (WORD)pSendDATA->m_pCPacket->GetLength() ) {		// ��ü ���� �Ϸ�..
            // ** �Ʒ� ���ο��� m_SendList���� pSendNode�� �����ϴ� ��������
            //    ������ ������ pUSER�� �̹� ��������Ǿ� ClearIOList() �Լ���
            //    �����Ͽ� m_SendList�� �̹� ����ֱ� �����̴�.
            classDLLNODE<tagIO_DATA> *pHeadNODE = m_SendList.GetHeadNode ();
			_ASSERT( pHeadNODE == pSendDATA->m_pNODE );

            classDLLNODE<tagIO_DATA> *pNextNODE = m_SendList.GetNextNode ( pHeadNODE );
			if ( pNextNODE ) {
				_ASSERT( pNextNODE->DATA.m_dwIOBytes == 0 );
			}

            m_SendList.DeleteNode( pSendDATA->m_pNODE );
            this->Free_SendIODATA( pSendDATA );

            classDLLNODE<tagIO_DATA> *pSendNODE = m_SendList.GetHeadNode ();
            if ( pSendNODE ) {

				_ASSERT( pSendNODE->DATA.m_dwIOBytes == 0 );

                if ( !this->Send_Continue( &pSendNODE->DATA ) ) {
				    m_csSendQ.Unlock ();
					return false;
				}
            }
		} else 
        if ( pSendDATA->m_dwIOBytes < pSendDATA->m_pCPacket->GetLength() ) {			// �κ� ���۵�..
            if ( !this->Send_Continue (pSendDATA) ) {
			    m_csSendQ.Unlock ();
				return false;
			}
        } else {
			_ASSERT( 0 );
        }
    }
    m_csSendQ.Unlock ();

	return true;
}


//-------------------------------------------------------------------------------------------------
bool iocpSOCKET::Recv_Done (tagIO_DATA *pRecvDATA)
{
	// �ٷ� ó���ϴ� �Լ�...
	short nTotalPacketLEN;

	t_PACKETHEADER *pPacket = (t_PACKETHEADER*)&pRecvDATA->m_pCPacket->m_pDATA;
    do {
		nTotalPacketLEN = this->D_RecvB( pPacket );
		if ( !nTotalPacketLEN ) {
			// ��Ŷ�� �����Ǿ� �Դ�.
			// ��ŷ�ΰ� ???
			g_LOG.CS_ODS( 0xffff, "*** ERROR: Decode recv packet body, IP[ %s ]\n", this->m_IP.Get() );
			this->Free_RecvIODATA( pRecvDATA );

			// �� ����Ʈ�� ip ���...

			return false;
		}

        if ( !this->HandlePACKET( pPacket ) ) {
			this->Free_RecvIODATA( pRecvDATA );
			return false;
        }

        pRecvDATA->m_dwIOBytes -= nTotalPacketLEN;
        pPacket = (t_PACKETHEADER*)( pPacket->m_pDATA + nTotalPacketLEN );
    } while ( pRecvDATA->m_dwIOBytes );

	this->Free_RecvIODATA( pRecvDATA );

    return true;
}

//-------------------------------------------------------------------------------------------------
