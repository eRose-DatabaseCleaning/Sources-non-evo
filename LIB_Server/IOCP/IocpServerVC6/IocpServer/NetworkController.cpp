#include "StdAfx.h"
#include "networkcontroller.h"
#include <assert.h>

#define TRACE    TraceDebug

// ����� ���
void TraceDebug(LPCTSTR lpszFormat, ...)
{
    static char szBuf[256];
    
    va_list argList;
    va_start(argList, lpszFormat);
    vsprintf(szBuf, lpszFormat, argList);
    OutputDebugString(szBuf);
    va_end(argList);
}

NetworkController::NetworkController(void)
{
	m_listenSocket=NULL;
	
	InitializeCriticalSection(&m_cs);
}

NetworkController::~NetworkController(void)
{
	if(m_listenSocket!=NULL)
	{
		closesocket(m_listenSocket);
		m_listenSocket=NULL;
	}

	WSACleanup();

	DeleteCriticalSection(&m_cs);
}

// �ʱ�ȭ ó��
BOOL NetworkController::Init(const int Port)
{
	WSADATA wsd;
	SOCKADDR_IN serverSockAddr;

	// ���� �ʱ�ȭ ó�� 
	if(WSAStartup(MAKEWORD(2,2), &wsd)!=0)
	{
		TRACE("WSAStartup Failed: %d\n",WSAGetLastError());
		return FALSE;
	}
    
	m_listenSocket=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(m_listenSocket==INVALID_SOCKET)
	{
		TRACE("Listen Socket Creation Failed: %d\n",WSAGetLastError());
		return FALSE;
	}
    
    serverSockAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverSockAddr.sin_family=AF_INET;
	serverSockAddr.sin_port=htons((short)Port);
	if(bind(m_listenSocket,(LPSOCKADDR)&serverSockAddr,sizeof(serverSockAddr)))
	{
		TRACE("Bind Failed: %d\n",WSAGetLastError());
		closesocket(m_listenSocket);
		m_listenSocket=NULL;
		return FALSE;
	}

	if(SOCKET_ERROR==listen(m_listenSocket, SOMAXCONN))
	{
		TRACE("Change to Listen Mode Error: %d\n",WSAGetLastError());
		closesocket(m_listenSocket);
		m_listenSocket=NULL;
		return FALSE;
	}

	TRACE("\nSocket Initiation Success\n");

	// IOCP �ʱ�ȭ ó�� 
	int ErrCode;
	if(!m_IocpHandler.Create(0,&ErrCode))
	{
		TRACE("Create IO Completion Port Error: %d\n",ErrCode);
		return FALSE;
	}

	if(!m_IocpHandler.CreateThreadPool(this))
	{
		TRACE("Create Thread Pool Failed\n");
		return FALSE;
	}
	
	TRACE("IOCP Initiation Success\n");
  
	m_pPerSocketCtxMemPool=new MemPooler<PerSocketContext>(MAX_USER);
	m_pRecvMemPool=new MemPooler<PerIoContext>(MAX_USER*2);
	m_pSendMemPool=new MemPooler<PerIoContext>(MAX_USER*2);
	if(!m_pPerSocketCtxMemPool || !m_pRecvMemPool || !m_pSendMemPool)
	{
		return FALSE;

	}

	TRACE("Memory Pool Create Success\n");

	return TRUE;
}

// Accept �۾� ó��
void NetworkController::AcceptProcess(void)
{
	int ErrCode=0;
	int sockaddr_size=sizeof(SOCKADDR_IN);    
	SOCKADDR_IN clientsockaddr;

	SOCKET clientsocket=INVALID_SOCKET;
	PPerSocketContext pPerSocketCtx=NULL;
	
	TRACE("Accept Process Start\n");

	#ifdef STATISTICS
	   m_state.SetTime();
	#endif

	while(TRUE)
	{
		__try
		{
			clientsocket=accept(m_listenSocket,(LPSOCKADDR)&clientsockaddr, &sockaddr_size);
			if(clientsocket==INVALID_SOCKET)
			{
				// ���� ������ Ŭ���� �ϸ� �� ������ �����Ƿ�
				// �� �����ÿ� Accept ������ ����������.
				if(WSAGetLastError()==WSAEINTR)
				{
					return ;
				}
				
				TRACE("Accepting Client Error: %d\n ",WSAGetLastError());

				__leave;			
			}

			TRACE("Client( IP: %s, Port: %d ) Connected\n",inet_ntoa(clientsockaddr.sin_addr),ntohs(clientsockaddr.sin_port));
            
			int nZero=0;
			if(SOCKET_ERROR==setsockopt(clientsocket,SOL_SOCKET,SO_RCVBUF,(const char*)&nZero,sizeof(int)))
			{
				TRACE("Change Buffer Size Error, %d\n",WSAGetLastError());
				continue;
			}
			nZero=0;
            if(SOCKET_ERROR==setsockopt(clientsocket,SOL_SOCKET,SO_SNDBUF,(const char*)&nZero,sizeof(int)))
			{
				TRACE("Change Buffer Size Error, %d\n",WSAGetLastError());
				continue;;
			}
			
			// ���� ���ؽ�Ʈ �Ҵ� -> Completion Key
			pPerSocketCtx=AllocPerSocketContext(clientsocket);
            if(pPerSocketCtx==NULL)
			{
				TRACE("Socket Context Allocation Error\n");
				continue;
			}

			// IOCP Ŀ�� ��ü�� ����
			if(!m_IocpHandler.Associate(clientsocket,reinterpret_cast<ULONG_PTR>(pPerSocketCtx),&ErrCode))
			{
				TRACE("Associating Error: %d\n",ErrCode);
				continue;
			}

			// �ʱ� Recv ��û
			BOOL bRet=RecvPost(pPerSocketCtx);
			if(bRet==FALSE)
			{
				TRACE("Init RecvPost Error\n");
				continue;
			}

			#ifdef STATISTICS
			   m_state.IncreaseConnection();
			#endif

		}
		__finally
		{	
			if(AbnormalTermination())
			{
				#ifdef STATISTICS
                   m_state.DecreaseConnection();
				#endif

			   CloseClient(pPerSocketCtx,false);
			}			
		}


	}
}


// �Ϸ� ��Ŷ ó�� �Լ�
void NetworkController::ProcessingThread(void)
{
	PPerSocketContext pPerSocketCtx=NULL;
	PPerIoContext pPerIoCtx=NULL;
	DWORD dwBytesTransferred=0;
	int ErrCode=0;

	while(TRUE)
	{
		// IO Completion Packet ���´�.
		BOOL bRet=m_IocpHandler.GetCompletionStatus(reinterpret_cast<ULONG_PTR*>(&pPerSocketCtx),
			                                        &dwBytesTransferred,
													reinterpret_cast<LPOVERLAPPED*>(&pPerIoCtx),
													&ErrCode);
		if(bRet)
		{
			if(((int)pPerSocketCtx)==THREAD_DIE)
			{
				break;
			}
                 
			if(NULL==pPerIoCtx)
			{
				TRACE("Getting Completion Packet Failed %d\n",ErrCode);
				continue;
			}
		}
		else
		{
			if(NULL==pPerIoCtx)
			{
				TRACE("Getting Completion Packet Failed %d\n",ErrCode);				
			}
			else
			{
				// ����� ���� ������ 64�� ���ɼ��� ����.
				// �� ������ ��Ʈ��ũ �̸��� ����� �� �����ϴ�. �̴�.
				// �� �̷��� ���� ��������� ���� OK�̴�.
                TRACE("Client Connection Close, Socket will Close.\n");
				#ifdef STATISTICS
					m_state.DecreaseConnection();
				#endif

			    CloseClient(pPerSocketCtx,false);
				
			}

			continue;

		}

		

		try
		{
			// Ŭ���̾�Ʈ�� ���� ���� 
			if(dwBytesTransferred==0)
			{
				TRACE("Client(%d) Connection Closed.\n",pPerSocketCtx->socket);
								
				throw "dwBytestransferred==0\n";
				
				 
			}

			// IO ���ݿ� ���� �׿� ���� ó��
			if(pPerIoCtx==pPerSocketCtx->recvContext)
			{
				// RECV Operation 
			    if(!RecvCompleteEvent(pPerSocketCtx,dwBytesTransferred))	
				{
					throw "RecvCompleteEvent Error\n";					
				}				

			}
			else if(pPerIoCtx==pPerSocketCtx->sendContext)
			{
				// SEND Operation
                if(!SendCompleteEvent(pPerSocketCtx,dwBytesTransferred))	
				{
					throw "SendCompleteEvent Error\n";				
				}			

			}
			else
			{				
				if(!OtherCompleteEvent(pPerSocketCtx,dwBytesTransferred))	
				{
					throw "OtherCompleteEvent Error\n";					
				}	
			}
			
		}
		catch(char* errText)
		{			
			    TRACE(errText);

				// continue ���� ���� Abnormal Termination �� �ȴ�.
				#ifdef STATISTICS
				 m_state.DecreaseConnection();
				#endif

				CloseClient(pPerSocketCtx,false);
				
		}

	}
}

// Ŭ���̾�Ʈ ���� ���ؽ�Ʈ �����ϰ� ���� ����
void NetworkController::CloseClient(PPerSocketContext pPerSocketCtx,bool bGraceful)
{
	EnterCriticalSection(&m_cs);
	
	//
	if(pPerSocketCtx!=NULL && pPerSocketCtx->socket!=INVALID_SOCKET)
	{
		if(!bGraceful)
		{
			LINGER LingerStruct;
			LingerStruct.l_onoff=1;
			LingerStruct.l_linger=0;
			assert(SOCKET_ERROR!=setsockopt(pPerSocketCtx->socket,SOL_SOCKET,SO_LINGER,(char*)&LingerStruct,sizeof(LingerStruct)));

		}
		                     
		closesocket(pPerSocketCtx->socket);
		pPerSocketCtx->socket=INVALID_SOCKET;
		
		DeallocPerSocketContext(pPerSocketCtx);
		pPerSocketCtx=NULL;
	}
	//
	
	LeaveCriticalSection(&m_cs);
}

// Per Io Context �޸� �Ҵ�
PPerIoContext NetworkController::AllocPerIoContextForSend(void)
{
	PPerIoContext pPerIoCtx=NULL;

	pPerIoCtx=m_pSendMemPool->Alloc();
	pPerIoCtx->wsaBuf.buf=pPerIoCtx->Buffer;
	pPerIoCtx->wsaBuf.len=MAX_BUFFER;

	return pPerIoCtx;
}

PPerIoContext NetworkController::AllocPerIoContextForRecv(void)
{
	PPerIoContext pPerIoCtx=NULL;
	
	pPerIoCtx=m_pRecvMemPool->Alloc();
	pPerIoCtx->wsaBuf.buf=pPerIoCtx->Buffer;
	pPerIoCtx->wsaBuf.len=MAX_BUFFER;

	return pPerIoCtx;
}

// Per Socket Context �޸� �Ҵ�
PPerSocketContext NetworkController::AllocPerSocketContext(SOCKET clientSocket)
{
	PPerSocketContext pPerSocketCtx=NULL;

	pPerSocketCtx=m_pPerSocketCtxMemPool->Alloc();
	
	pPerSocketCtx->socket=clientSocket;
	pPerSocketCtx->recvContext=AllocPerIoContextForRecv();
	pPerSocketCtx->sendContext=AllocPerIoContextForSend();
    	
	return pPerSocketCtx;
}

// PerIoContext �޸� ����
void NetworkController::DeallocPerIoContextForRecv(PPerIoContext pPerIoCtx)
{
	assert(pPerIoCtx);
	BOOL bRet=m_pRecvMemPool->Free(pPerIoCtx);
	assert(bRet);
	
}

void NetworkController::DeallocPerIoContextForSend(PPerIoContext pPerIoCtx)
{
	assert(pPerIoCtx);
	BOOL bRet=m_pSendMemPool->Free(pPerIoCtx);
	assert(bRet);
	
}

// PerSocketContext �޸� ����
void NetworkController::DeallocPerSocketContext(PPerSocketContext pPerSocketCtx)
{
	assert(pPerSocketCtx);

	// IO Context �Ҵ��� �� ����
	DeallocPerIoContextForRecv(pPerSocketCtx->recvContext);
	DeallocPerIoContextForSend(pPerSocketCtx->sendContext);
        	
	// ���� ���ؽ�Ʈ ���� 
	BOOL bRet=m_pPerSocketCtxMemPool->Free(pPerSocketCtx);
	assert(bRet);
	
}

// RECV ��û
BOOL NetworkController::RecvPost(PPerSocketContext pPerSocketCtx)
{
	DWORD dwRecvBytes=0;
	DWORD dwFlags=0;
	
	ZeroMemory(&pPerSocketCtx->recvContext->overlapped,sizeof(WSAOVERLAPPED));
    
	int ret=WSARecv(pPerSocketCtx->socket,&(pPerSocketCtx->recvContext->wsaBuf),1,
		    &dwRecvBytes,&dwFlags,&(pPerSocketCtx->recvContext->overlapped),NULL);

	if(SOCKET_ERROR==ret)
	{
		int ErrCode=WSAGetLastError();
		if(ErrCode!=WSA_IO_PENDING)
		{			
			TRACE("[%d] Recv Request Error(WSASend Function Failed): %d\n",GetTickCount(),ErrCode);;
			TRACE("Client will Close.\n"); 			
			return FALSE;
		}
	}

	return TRUE;
}

// Send ��û
BOOL NetworkController::SendPost(PPerSocketContext pPerSocketCtx)
{
	DWORD dwSendBytes=0;
	DWORD dwFlags=0;
	
	ZeroMemory(&pPerSocketCtx->sendContext->overlapped,sizeof(WSAOVERLAPPED));
    
	int ret=WSASend(pPerSocketCtx->socket,&(pPerSocketCtx->sendContext->wsaBuf),1,
		    &dwSendBytes,dwFlags,&(pPerSocketCtx->sendContext->overlapped),NULL);

	if(SOCKET_ERROR==ret)
	{
		int ErrCode=WSAGetLastError();
		if(ErrCode!=WSA_IO_PENDING)
		{
			TRACE("[%d] Send Request Error(WSASend Function Failed): %d\n",GetTickCount(),ErrCode);;
			TRACE("Client will Close.\n"); 			
			return FALSE;
		}
	}

	return TRUE;
}

// ���ú� �̺�Ʈ ó�� �ڵ鷯 �Լ�
// return ��: TRUE  -> ���� ���� ���������� ó����
//            FALSE -> �Ϸ� ��Ŷ ó�� ���� �� ���� �߻�
BOOL NetworkController::RecvCompleteEvent(PPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred)
{
	// Echo ���� �̹Ƿ� ���� ������ �״�� ����������.
	//TRACE("GET THIS: %d Bytes -> Return To Client \n",dwBytesTransferred);
	
    #ifdef STATISTICS
	  m_state.IncreaseBytesRead(dwBytesTransferred);
    #endif
	
	ZeroMemory(pPerSocketCtx->sendContext->Buffer,sizeof(char)*MAX_BUFFER);
	CopyMemory(pPerSocketCtx->sendContext->Buffer,pPerSocketCtx->recvContext->Buffer,dwBytesTransferred);
	pPerSocketCtx->sendContext->wsaBuf.len=dwBytesTransferred;
	
	ZeroMemory(pPerSocketCtx->recvContext->Buffer,sizeof(char)*MAX_BUFFER);
	

	BOOL bRet=SendPost(pPerSocketCtx);
	if(!bRet)
		return FALSE;
    
	bRet=RecvPost(pPerSocketCtx);
	if(!bRet)
		return FALSE;

	return TRUE;
}

// Send �Ϸ� ��Ŷ ó�� �ڵ鷯 �Լ�
// return ��: TRUE  -> ���� ���� ���������� ó����
//            FALSE -> �Ϸ� ��Ŷ ó�� ���� �� ���� �߻�
BOOL NetworkController::SendCompleteEvent(PPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred)
{
	// Echo ���� �̹Ƿ� Send �Ϸ�ô� �� ���� ����.
	
    #ifdef STATISTICS
	   m_state.IncreaseBytesSent(dwBytesTransferred);
    #endif

	return TRUE;
}

// Recv, Send �Ϸ� ���� ���� ó�� �ڵ鷯 �Լ�
// return ��: TRUE  -> ���� ���� ���������� ó����
//            FALSE -> �Ϸ� ��Ŷ ó�� ���� �� ���� �߻�
BOOL NetworkController::OtherCompleteEvent(PPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred)
{
	TRACE("Critical Error. Invalid Operation. Client Close!\n");

	// ���� ����� ���� Recv , Send �̿��� �̻��� ������ ����Ŵ ���� ���������.
    closesocket(pPerSocketCtx->socket);
	pPerSocketCtx->socket=INVALID_SOCKET;

	return FALSE; // ������ ����Ŵ
}

// ���� �ߴ�
void NetworkController::ServerClose(void)
{
	m_IocpHandler.CloseAllThreads();
	
	closesocket(m_listenSocket);
	m_listenSocket=INVALID_SOCKET;
	
	TRACE("Server Close!\n");;
}

