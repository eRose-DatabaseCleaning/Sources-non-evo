#ifndef _CCommDlg_
#define _CCommDlg_
#include "TDialog.h"
class CZListBox;
class CFriendListItem;

/**
* Community �� ���� ���̾�α�
*
* @Author		������
* @Date			2005/9/12
*/
class CCommDlg : public CTDialog
{
public:
	CCommDlg( int iDlgType );
	virtual ~CCommDlg(void);
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual void Show();

	///ģ����� ����
	void AddFriend( DWORD dwUserTag, BYTE btStatus, const char* pszName );
	void ClearFriendList();
	void ChangeFriendStatus( DWORD dwUserTag, BYTE btStatus );

	void RemoveFriend( DWORD dwUserTag );

	CFriendListItem* FindFriend( DWORD dwUserTag );
	CFriendListItem* FindFriendByName( const char* pszName );

	///�����Ը�� ����
	void ClearMemoList();
	void AddMemo( int iRowid, const char* pszFromName, const char* pszMemo , DWORD dwRecvTime );
	void DeleteMemo( int iIndex );

	/// ��ȭ���� ����
	void ClearChatRoomList();
	void AddChatRoom( const char* pszTitle, BYTE btRoomType, short nRoomID, BYTE btUserCount );


	/// Ŭ���̾�Ʈ�� ����Ǿ� �ִ� ������ �������� ���Ͽ� �ʿ��� Method(  Sqlite ����� )
	static int callback(void *NotUsed, int argc, char **argv, char **azColName);

private:
	void		OnLButtonUp( unsigned uiProcID, WPARAM wParam, LPARAM lParam );
	CZListBox*	GetZListBox( int iTab, int iListID );

	/// Ŭ���̾�Ʈ�� ����Ǿ� �ִ� ���� ��������( Sqlite ����� )
	void		LoadMemoFromDB();
private:

	enum{
		IID_BTN_CLOSE			= 10,
		IID_BTN_ICONIZE			= 11,
		IID_TABBEDPANE			= 20,
		IID_TABBUTTON_FRIEND	= 25,
		IID_ZLIST_FRIEND		= 26,
		IID_BTN_REMOVEFRIEND	= 24,
		IID_BTN_ADDFRIEND		= 29,
		IID_TABBUTTON_CHATROOM	= 35,
		IID_ZLIST_CHATROOM		= 36,
		IID_BTN_CREATECHATROOM	= 39,
		IID_TABBUTTON_MAILBOXM	= 45,
		IID_ZLIST_MAILBOX		= 46,
	};

	enum{
		TAB_FRIEND   = 2,
		TAB_CHATROOM = 1,
		TAB_MAILBOX  = 0
	};
};
#endif