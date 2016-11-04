#pragma once

#define MAX_BUFFER 4096

typedef struct tagPerIoContext
{

	WSAOVERLAPPED overlapped;
	WSABUF wsaBuf;
	char Buffer[MAX_BUFFER];

	//DWORD dwBytesTransferred; // �� IO ���۵� ����Ʈ �� 
	//DWORD dwTotalBytes;       // �� �󸶸�ŭ ���۵Ǿ����ĸ� �����ϴ� ����Ʈ �� 

} PerIoContext, *PPerIoContext;

typedef struct tagPerSocketContext
{

  SOCKET socket;
  PPerIoContext recvContext;
  PPerIoContext sendContext;

} PerSocketContext, *PPerSocketContext;