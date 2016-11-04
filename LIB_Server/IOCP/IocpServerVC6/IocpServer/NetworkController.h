#pragma once

#include "IocpHandler.h"
#include "def.h"
#include "MemPooler.h"
#include "Statistics.h"
#define  MAX_USER 3000

class NetworkController:public IIocpProcessThread
{
public:
	NetworkController(void);
	~NetworkController(void);

	// �ʱ�ȭ ó��
	BOOL Init(const int Port=15000);
    // RECV ��û
	BOOL RecvPost(PPerSocketContext pPerSocketCtx);
	// Send ��û
	BOOL SendPost(PPerSocketContext pPerSocketCtx);	
	// Accept �۾� ó��
	void AcceptProcess(void);
    // ���� �ߴ�
	void ServerClose(void);

private:	
    // Per Io Context �޸� �Ҵ�
	PPerIoContext AllocPerIoContextForRecv(void);
	PPerIoContext AllocPerIoContextForSend(void);
	// Per Socket Context �޸� �Ҵ�
	PPerSocketContext AllocPerSocketContext(SOCKET clientSocket);
	// �Ϸ� ��Ŷ ó�� �Լ�
	void ProcessingThread(void);
	// PerIoContext �޸� ����
	void DeallocPerIoContextForSend(PPerIoContext pPerIoCtx);
	void DeallocPerIoContextForRecv(PPerIoContext pPerIoCtx);
	// PerSocketContext �޸� ����
	void DeallocPerSocketContext(PPerSocketContext pPerSocketCtx);
	// ���ú� �̺�Ʈ ó�� �ڵ鷯 �Լ�
	BOOL RecvCompleteEvent(PPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred);
	// Send �Ϸ� ��Ŷ ó�� �ڵ鷯 �Լ�
	BOOL SendCompleteEvent(PPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred);
	// Recv, Send �Ϸ� ���� ���� ó�� �ڵ鷯 �Լ�
	BOOL OtherCompleteEvent(PPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred);
	// Ŭ���̾�Ʈ ���� ���ؽ�Ʈ �����ϰ� ���� ����
	void CloseClient(PPerSocketContext pPerSocketCtx,bool bGraceful);
private:
	// ���� ����
	SOCKET m_listenSocket;
	// IOCP �ڵ鷯 
	IocpHandler m_IocpHandler;

#ifdef STATISTICS
	// ��� ǥ�ø� ���� Ŭ���� �ν��Ͻ�
	Statistics m_state;
#endif

	// ����ȭ�� ���� ����( �̷��� �ϴ°� ���� �ٸ� ����� �� ���� �� ����)
	CRITICAL_SECTION m_cs;

    MemPooler<PerSocketContext> * m_pPerSocketCtxMemPool;
	MemPooler<PerIoContext> * m_pRecvMemPool;
	MemPooler<PerIoContext> * m_pSendMemPool;
	
};
