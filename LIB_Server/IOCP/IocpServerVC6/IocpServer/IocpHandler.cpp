#include "StdAfx.h"
#include "iocphandler.h"
#include <assert.h>

typedef unsigned (__stdcall *PTHREAD_START) (void *);

#define BEGINTHREADEX(psa, cbStack, pfnStartAddr, \
   pvParam, fdwCreate, pdwThreadId)                 \
      ((HANDLE)_beginthreadex(                      \
         (void *)        (psa),                     \
         (unsigned)      (cbStack),                 \
         (PTHREAD_START) (pfnStartAddr),            \
         (void *)        (pvParam),                 \
         (unsigned)      (fdwCreate),               \
         (unsigned *)    (pdwThreadId)))

IocpHandler::IocpHandler(void)
{
	m_hIOCP=NULL;
}

IocpHandler::~IocpHandler(void)
{
	if(m_hIOCP)
		CloseHandle(m_hIOCP);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// IOCP ���� �� �ʱ�ȭ
// nMaxNumberOfConcurrentThreads: Concurrent Thread �� ����
// pnOutErrCode: ���� ��Ȳ �ÿ� �ܺη� ������ �����ڵ�, NULL �� ������ �����ڵ带 ������ �ʴ´�.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL IocpHandler::Create(int nMaxNumberOfConcurrentThreads, int* pnOutErrCode)
{
	m_hIOCP=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,nMaxNumberOfConcurrentThreads);
    if(m_hIOCP==NULL && pnOutErrCode!=NULL)
	{        
       *pnOutErrCode=GetLastError(); 
	}

	return (m_hIOCP!=NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ϰ� IOCP �� ����
// hSocket		 : IOCP �� ����� ����
// CompletionKey : ����Ǹ鼭 ������ Completion Key
// pnOutErrCode  : ���� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL IocpHandler::Associate(SOCKET hSocket, ULONG_PTR CompletionKey, int* pnOutErrCode)
{
	assert(hSocket!=INVALID_SOCKET);

	return Associate((HANDLE)hSocket,CompletionKey,pnOutErrCode);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���� �ڵ�� IOCP ���� ����
// hDevice		 : IOCP �� ����� ����̽�
// CompletionKey : ����Ǹ鼭 ������ Completion Key
// pnOutErrCode  : ���� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL IocpHandler::Associate(HANDLE hDevice, ULONG_PTR CompletionKey, int* pnOutErrCode)
{
	assert(hDevice!=INVALID_HANDLE_VALUE);
	assert(CompletionKey!=0);

	HANDLE retHandle=CreateIoCompletionPort(hDevice,m_hIOCP,CompletionKey,0);
    if(retHandle!=m_hIOCP && pnOutErrCode!=NULL)
	{
		*pnOutErrCode=GetLastError();     
	}

	return (retHandle==m_hIOCP);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// IO Completion Queue�� �� �Ϸ� ��Ŷ�� ����� ���Ƿ� �߰�
// CompletionKey      : ����� ���� �Ϸ� ��Ŷ�� �� Completion Key
// dwBytesTransferred : ����� ���� �Ϸ� ��Ŷ�� �� ����Ʈ��(0 ����Ʈ)
// pOverlapped        : ����� ���� �Ϸ� ��Ŷ�� ���� �������� ����ü(NULL ����Ʈ)
// pnOutErrCode       : ���� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL IocpHandler::PostCompletionStatus(ULONG_PTR CompletionKey, DWORD dwBytesTransferred, OVERLAPPED* pOverlapped, int* pnOutErrCode)
{
	BOOL ret=PostQueuedCompletionStatus(m_hIOCP,dwBytesTransferred,CompletionKey,pOverlapped);
	if(!ret && pnOutErrCode!=NULL)
	{
       *pnOutErrCode=GetLastError();	   
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// IO Completion Queue���� �Ϸ� ��Ŷ�� ���´�.
// pOutCompletionKey      : IO Completon ť���� ������ ���ڵ��� Completion Key
// pdwOutBytesTransferred : IO Completon ť���� ������ ���ڵ��� ����Ʈ��(0 ����Ʈ)
// pOutOverlapped         : IO Completon ť���� ������ ���ڵ��� �������� ����ü(NULL ����Ʈ)
//  pnOutErrCode          : ���� ��
//  dwWaitingTime         : �Ϸ� ��Ŷ�� ���� �ÿ� ��ٸ� �ð� (����Ʈ: ���Ѵ�)
//
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL IocpHandler::GetCompletionStatus(ULONG_PTR* pOutCompletionKey, LPDWORD pdwOutBytesTransferred, LPOVERLAPPED* pOutOverlapped, int* pnOutErrCode, DWORD dwWaitingTime)
{
	BOOL ret=GetQueuedCompletionStatus(m_hIOCP,pdwOutBytesTransferred,pOutCompletionKey,pOutOverlapped,dwWaitingTime);
	if(!ret && pnOutErrCode!=NULL)
	{
       *pnOutErrCode=GetLastError();	   
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// IOCP Worker Thread Ǯ�� ����
// piProcessThread: IIocpProcessThread �������̽��� ��ӹ��� Ŭ������ ���� ������ �� ������ Ǯ�� ��
//                  ������ �Լ��� ������ Ŭ������ ������
// nNumOfPooledThredad: Ǯ���� ������ ����, 0�̸� ����Ʈ ���� ���߾��� 
//
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL IocpHandler::CreateThreadPool(IIocpProcessThread* piProcessThread, int nNumOfPooledThread)
{
	assert(piProcessThread);
	assert(nNumOfPooledThread>=0);
	
	if(nNumOfPooledThread==0)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		// ����Ʈ ������ ���� 
		// 2 * ���μ����� + 2 �� ������ ������
		m_CreatedThreadNumber=si.dwNumberOfProcessors*2+2;
	}
	else
	{
		m_CreatedThreadNumber=nNumOfPooledThread;
	}

	for(int i=0;i<m_CreatedThreadNumber;i++)
	{
		DWORD dwThreadId=0;		
		CloseHandle(BEGINTHREADEX(NULL,0,IocpWorkerThreadStartingPoint,piProcessThread,0,&dwThreadId));
	}

	return TRUE;
}

// IOCP ��Ŀ ������ ���� ����Ʈ�μ����� ��Ȱ
DWORD __stdcall IocpHandler::IocpWorkerThreadStartingPoint(PVOID pvParam)
{
	IIocpProcessThread* piProcessThread=(IIocpProcessThread*)pvParam;
	piProcessThread->ProcessingThread();

	return 0;
}

// ���� ���� �ִ� ������ ��� ����
void IocpHandler::CloseAllThreads(void)
{
	for(int i=0;i<m_CreatedThreadNumber;i++)
	{
		PostCompletionStatus(THREAD_DIE);
	}
}
