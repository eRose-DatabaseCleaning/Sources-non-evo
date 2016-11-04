#pragma once

#include <windows.h>
#include <assert.h>

/*
Title     : IOCP�� ������ ť ���ø� Ŭ����
Class Name: IocpQueue
Date      : 2002.8.30                                              
Programmer: snaiper- �̱�Ź (snaiper80@korea.com) 
*/                                           

template<typename Type, int QueueSize>
class IocpQueue
{
// Public Method
public:
   // Constructor and Destructor
   IocpQueue():m_hIocp(NULL),m_hSemaphore(NULL)
   {
	   Create();
   }
   ~IocpQueue()
   {
       Destroy();	   
   }

   // Push ���� - ť�� �� �ڸ��� ������ �ڸ� ���涧���� ��ŷ 
   void Blocking_Push(Type PushingData)
   {
	   Push(PushingData,INFINITE);
   }

   // Pop ���� - ť�� �������� ������ ��ŷ
   Type Blocking_Pop()
   {
	   Type PopedData;
	   Pop(PopedData,INFINITE);
	   return PopedData;
   }

   // Push ���� - ť�� �� �ڸ��� ������ Ÿ�Ӿƿ� �ð����� ��ŷ, �ڸ� ����� �ְ� ����
   BOOL Blocking_Push_Timeout(Type PushingData,DWORD dwTimeout)
   {
	  return Push(PushingData,dwTimeout);
   }

   // Pop ���� - ť�� �������� ������ Ÿ�Ӿƿ� �ð����� ��ŷ, ������ ������ ����
   BOOL Blocking_Pop_Timeout(Type& PopingData, DWORD dwTimeout)
   {
	  return Pop(PopingData,dwTimeout);
   }

   // Push ���� - ť�� �ڸ��� ������ �ְ� �ڸ� ������ �ٷ� FALSE��� ����� �Բ� ����
   BOOL NonBlocking_Push(Type PushingData)
   {	   
	   return Push(PushingData,0);
   }
   
   // Pop ���� - ť�� �������� ������ ������ ���� TRUE ����, ������ ������ �ٷ� FALSE �� �Բ� ���� 
   BOOL NonBlocking_Pop(Type& PopingData)
   {
	   return Pop(PopingData,0);
   }

// Private Method   
private:
	// �ʱ�ȭ, IOCP ����, �������� �ʱ�ȭ
	void Create()   
	{
		m_hIocp=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);
		assert(m_hIocp!=NULL);

		m_hSemaphore=CreateSemaphore(NULL,QueueSize,QueueSize,NULL);
		assert(m_hSemaphore!=NULL);

	}
	// �ڿ�����, IOCP ��ü �� �������� ����
	void Destroy()  
	{
	   if(m_hIocp!=NULL)
	   {
		   CloseHandle(m_hIocp);
           m_hIocp=NULL;
	   }

	   if(m_hSemaphore!=NULL)
	   {
		   CloseHandle(m_hSemaphore);
		   m_hSemaphore=NULL;
	   }
	}

	// Push Operation
	BOOL Push(Type& data, DWORD dwTimeout)
	{
	   DWORD ret=WaitForSingleObject(m_hSemaphore,dwTimeout);
	   if(ret==WAIT_OBJECT_0)
	   {
		   BOOL ret=PostQueuedCompletionStatus(m_hIocp,0,(ULONG_PTR)data,NULL);
		   assert(ret);
		   return TRUE;
	   }
	   else 
	   { 
		   assert(ret==WAIT_TIMEOUT);
		   return FALSE;
	   }
	}

	// Pop Operation
	BOOL Pop(Type& retData,DWORD dwTimeout)
	{
	   Type PopingItem;
	   DWORD dwTemp=0;	   
       LPOVERLAPPED pTempOverlapped=NULL;
	   
	   BOOL ret=GetQueuedCompletionStatus(m_hIocp,&dwTemp,((PULONG_PTR)(&PopingItem)),&pTempOverlapped,dwTimeout);
	   if(ret)
	   {       
	      assert(ReleaseSemaphore(m_hSemaphore,1,NULL));
		  retData=PopingItem;
		  return TRUE;
	   }
	   else
	   {
		  assert(WAIT_TIMEOUT==GetLastError());
	      return FALSE;
	   }
	}

// Private Member
private:
	HANDLE m_hIocp;
	HANDLE m_hSemaphore;
};