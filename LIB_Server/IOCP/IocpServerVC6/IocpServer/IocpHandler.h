#pragma once

#include "IocpProcessThreadDef.h"

class IocpHandler
{
public:
	IocpHandler(void);
	~IocpHandler(void);

	// IOCP ���� �� �ʱ�ȭ
	BOOL Create(int nMaxNumberOfConcurrentThreads=0, int* pnOutErrCode=NULL);
	// ���ϰ� IOCP �� ����
	BOOL Associate(SOCKET hSocket, ULONG_PTR CompletionKey, int* pnOutErrCode=NULL);
	// ���� �ڵ�� IOCP ���� ����
	BOOL Associate(HANDLE hDevice, ULONG_PTR CompletionKey, int* pnOutErrCode=NULL);
	// IO Completion Queue�� �� �Ϸ� ���ڵ带 ����� ���Ƿ� �߰�
	BOOL PostCompletionStatus(ULONG_PTR CompleitonKey, DWORD dwBytesTransferred=0, OVERLAPPED* pOverlapped=NULL, int* pnOutErrCode=NULL);
	// IO Completion Queue���� �Ϸ� ��Ŷ�� ���´�.
	BOOL GetCompletionStatus(ULONG_PTR* pOutCompletionKey, LPDWORD pdwOutBytesTransferred, LPOVERLAPPED* pOutOverlapped, int* pnOutErrCode=NULL, DWORD dwWaitingTime=INFINITE);
	
	// IOCP Worker Thread Ǯ�� ����
	BOOL CreateThreadPool(IIocpProcessThread* piProcessThread, int nNumOfPooledThread=0);

private:
	// IOCP Ŀ�� ��ü�� ���� �ڵ�
	HANDLE m_hIOCP;
	// ������� Worker Thread ��
	int m_CreatedThreadNumber;
	
private:
	// IOCP ��Ŀ ������ ���� ����Ʈ�μ����� ��Ȱ
	static DWORD __stdcall IocpWorkerThreadStartingPoint(PVOID pvParam);
public:
	// ���� ���� �ִ� ������ ��� ����
	void CloseAllThreads(void);
};
