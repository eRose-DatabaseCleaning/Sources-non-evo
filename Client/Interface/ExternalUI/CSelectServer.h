#ifndef _SELECTSERVER_
#define _SELECTSERVER_

#include "CExternalUI.h"
#include "../../GameCommon/IObserver.h"
#include <string>
#include <vector>
#include <deque>


#include "TDialog.h"

struct ServerInfo
{
	std::string			strServerName;
	unsigned int		iServerID;
};

struct ChannelInfo
{
	int	m_btChannelNO;		// ������ ä�� ���� ��ȣ
	int	m_btLowAGE;			// ���� ������ ���� ���� 0 �̸� ���� ���� ���� ����
	int	m_btHighAGE;		// ���� ������ �ְ� ���� 0 �̸� �ְ� ���� ���� ����
};

struct t_PACKET;
struct tagCHANNEL_SRV;


/**
* ���弭��&ä�� ���� ���ý� ���Ǵ� �������̽�
* - Observable : GameData/CServerList
*
* @Warning		����(2005/9/6) �ѱ��� ��� ä�μ����� 1���̰� ä�� ����Ʈ�� �������� �ʴ´�.�� �� ������ ��� ä�μ����� 2�� �̻����� ������ �� �� �ִ�.
* @Author		������
* @Date			2005/9/6
*/
class CSelectServer : public CTDialog, public IObserver
{
private:	
	///
	///	Observer class
	///
	CExternalUIManager*				m_pEUIManager;

	std::vector< ServerInfo >		m_ServerInfoList;
	std::deque< ChannelInfo >		m_Channels;

	bool							m_bWaitServerReply;
public:
	CSelectServer();
	~CSelectServer();

	///
	/// set observer class
	///
	void SetEUIObserver( CExternalUIManager* pObserver );
	///
	/// Message procedure
	///
	virtual UINT Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	///
	/// Drawing method
	///
	virtual void Update( CObservable* pObservable, CTObject* pObj );

	virtual void    Hide();
	virtual void	Show();
	virtual void    MsgBoxProc(  unsigned int iMsgType, short int iButton );
	///
	///	All server list count
	///
	///
	///	Add server to server list
	///
	ServerInfo* GetServerInfo( int iIndex );

	///
	///	Send select server request
	///

	///
	///	Recv proc..
	///
	void	RecvSelectServer( t_PACKET* recvPacket );
	void	SendReqFirestServerChannelList();

	enum{
		IID_LISTBOX_SERVER	= 2,
		IID_LISTBOX_CHANNEL = 3,
		IID_BTN_OK		= 10,
		IID_BTN_CANCEL	= 11
	};
	enum{
		MSG_TYPE_EXIT,
		MSG_TYPE_NORMAL
	};
protected:

	void	OnLButtonUp( unsigned iProcID );
	void	OnLButtonDblClk( unsigned iProcID );
	void	OnLButtonDown( unsigned iProcID );
	
	unsigned int	FindSvrId( short ItemID );
	int				FindChannelNo(  int iIndex );

	void	SendSelectChannel();
	void	SendSelectServerReq();
};

#endif //_SELECTSERVER_