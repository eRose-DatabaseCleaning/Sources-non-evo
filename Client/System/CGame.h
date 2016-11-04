#ifndef _CGAME_
#define _CGAME_

#include "../AppMsgQ.h"
#include "Net_Prototype.h"
#include "../GameProc/MouseTargetEffect.h"
#include "../interface/CInfo.h"
#include "../util/FileVersionInfo.h"

#include "TCommandQ.h"

#include "../Interface/ExternalUI/CSelectServer.h"

extern bool			g_bDontDeleteCapFile;

#define		WM_USER_CLOSE_MSGBOX		WM_USER + 1
#define		WM_USER_SERVER_DISCONNECTED WM_USER + 2
#define		WM_USER_WORLDSERVER_DISCONNECTED WM_USER + 3
#define		CHEAT_MM		32						// ����� ȸ��
#define		CHEAT_GM		256						// GM ȸ��
#define		CHEAT_DEV		1024					// ������ ȸ��

//Japan ����
enum
{
	BILL_FLAG_JP_BATTLE	= 0,
	BILL_FLAG_JP_COMMUNITY,
	BILL_FLAG_JP_TRADE,
	BILL_FLAG_JP_STOCK_SPACE,
	BILL_FLAG_JP_EXTRA_STOCK,
	BILL_FLAG_JP_STARSHIP_PA,
	BILL_FLAG_JP_DUNGEON_ADV,
	BILL_FLAG_JP_EXTRA_CHAR,
	PAY_FLAG_JP_SIZE,
};

class CGameState;
class CInfo;



/**
* Ŭ���̾�Ʈ�� ���� Ŭ����
*	- �� ����( �α���, �������� ���)�� ���� ���� �������� ���еǾ� �ִ�.
*
* @Author		������
* @Date			2005/9/15
*/
class CGame
{
	friend class CGameStateNull;
	friend class CGameStateTitle;
	friend class CGameStateLogin;
	friend class CGameStatePrepareSelectAvatar;
	friend class CGameStateSelectAvatar;
	friend class CGameStateCreateAvatar;
	friend class CGameStatePrepareMain;
	friend class CGameStateMain; 
	friend class CGameStateWarp;
	friend class CGameStateExitMain;
	friend class CGameStateSelectSvr;
private: 
	CGame(void);

public:
	~CGame(void);
	static CGame& GetInstance();

	void	ChangeState( int newState );
	void    GameLoop();
	bool	AddWndMsgQ( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	void    ProcWndMsg( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	void	ClearWndMsgQ(){ m_WndMsgQ.Clear(); }

	int		GetCurrStateID();

	void	ChangeScreenMode();

	enum eGAME_STATE{

		GS_NULL,
		GS_TITLE,
		GS_LOGIN,
		GS_WARNING,
		GS_SELECTSVR,
		GS_PREPARESELECTAVATAR,
		GS_SELECTAVATAR,
		GS_LOGINVIRTUAL,
		GS_CREATEAVATAR,
		GS_MOVEMAIN,		///�ƹ�Ÿ�� ������ ī�޶� ����� ���� ����
		GS_PREPAREMAIN,     ///�������� ������ DATA Loading
		GS_MAIN,
		GS_WARP,
		GS_EXITMAIN,
		GS_MOVEPLANET,		///�༺���̵�
		GS_RELOGIN,
		GS_MAX
	};

	enum eREPAIR_TYPE{
		REPAIR_NONE,
		REPAIR_NPC,		/// NPC�� ���� ����
		REPAIR_ITEM,	/// �����ۿ� ���� ����
	};

	enum{
		APPRAISAL_NONE,
		APPRAISAL_NPC
	};
	///���� Mouse��ġ�� ���ϴ� Method
	///���� MousePosition�� WM_MOUSEMOVE�� �Ͼ���� �������ִ°����� ����
	short	Get_XPos ();//					{ return (short)m_ptMouse.x; }
	short	Get_YPos ();//					{ return (short)m_ptMouse.y; }
	void	Get_MousePos(POINT &ptMouse){ ptMouse = m_ptCurrMouse; }
	void	Get_PrevMousePos( POINT &ptMouse) { ptMouse = m_ptPrevMouse; }
	
	///Mouse ���¹� Image ���濡 ���Ǵ� Method
//	void	ToggleUserMouseFlag(){ m_bUseUserMouse = !m_bUseUserMouse; };
//	bool	GetUseUserMouseFlag(){ return m_bUseUserMouse; }
	void	ResetCheckFrame(){ m_iCheckFrame = 0; }
//	void	RefreshCursor(){ SetUserCursor( m_iCurrentCursorType ); }
	void	IncreseCheckFrame(){ m_iCheckFrame++; }
//	int		GetCurrCursorType(){ return m_iCurrentCursorType; }
	int     GetCheckFrame(){ return m_iCheckFrame; }

	///*----------------------------------------------------------------------------------------
	/// �� ���º���� ���� ���¿� ���� ���°��� Data�̵��� ���� �ӽ� ���� ����&Method
	void    SetLoadingData( const gsv_TELEPORT_REPLY& Reply ){ m_gsv_TELEPORT_REPLY = Reply; }
	void	GetLoadingData( gsv_TELEPORT_REPLY& Reply ){ Reply = m_gsv_TELEPORT_REPLY; }


	void					CreateSelectedAvata();
	gsv_SELECT_CHAR			m_SelectedAvataInfo;
	gsv_INVENTORY_DATA		m_SelectedAvataINV;
	gsv_QUEST_DATA			m_QuestData;
	std::string				m_strAvataName;
	///*----------------------------------------------------------------------------------------
	
	void	GetRayOrig( D3DXVECTOR3& RayOrig ){ RayOrig = m_RayOrig; }
	void    GetRayDir( D3DXVECTOR3& RayDir ){ RayDir = m_RayDir; } 
	void    SetRayOrig( const D3DXVECTOR3& RayOrig ){ m_RayOrig = RayOrig; }
	void    SetRayDir( const D3DXVECTOR3& RayDir ){ m_RayDir = RayDir; } 

	bool	IJL_BMP2JPEG (const char *szBMPFile, char *szJPGFile);
	void	ScreenCAPTURE ();
	void    ScreenCAPTURE (SYSTEMTIME SystemTime);
	//CInfo&	GetMouseInfo(){ return m_MouseInfo; }







	/*void	SetUserCursor( int iCursorType , CInfo* MouseInfo = NULL );
	void	SetDisableUserCursor();*/


	void	BeginRepair( int iType );
	void	EndRepair();
	int		GetRepairMode();
	void	ResetRepairMode();
	
	void	BeginAppraisal();
	void	EndAppraisal();

	__int64 GetAppraisalCost();
	void	SetAppraisalCost( __int64 i64Cost );

	void	SetUsingRepairItemInvenIdx( int iItemInvenIdx );
	int		GetUsingRepairItemInvenIdx();
	
	void	SetRepairNpcSvrIdx( WORD wNpcSvrIdx );
	WORD    GetRepairNpcSvrIdx();

	void	SetAppraisalNpcSvrIdx( WORD wNpcSvrIdx );
	WORD	GetAppraisalNpcSvrIdx();
	//----------------------------------------------------------------------------------------------
	/// ���콺 Ÿ������ ����Ʈ
	//----------------------------------------------------------------------------------------------
	void	SetMouseTargetEffect( float x, float y, float z ){ m_MouseTargetEffect.Start( x, y, z ); }

	DWORD	GetRight();
	void	SetRight( DWORD dwRight );
	void	AcceptedConnectLoginSvr();
	void	UpdateCurrentState();
	void	SetJoinChannelNo( int iNo );
	int		GetJoinChannelNo();

	bool	AddTCommand( CTCommand* pCmd );


	enum E_PAY{
		PAY_FREE,
		PAY_PREMIUM,
		PAY_PLATINUM
	};

	void	SetPayType( WORD paytype );
	WORD	GetPayType();
	WORD	GetDetailPayType();
	void	SetExpireTime( const char* time );
	DWORD   GetSetExpireTime();
	const std::string& GetExpireTime();
	void	SetExpireTimeType( int type );
	int		GetExpireTimeType();

	//Ext Func
	void	SetExpireTimeExt( int type, const char* time );
	DWORD   GetSetExpireTimeExt( int type );
	const	std::string& GetExpireTimeExt( int type );
	void	SetExpireTimeTypeExt( int type, int time );
	int		GetExpireTimeTypeExt( int type );


	void	SetSelectedServerID( unsigned int id );
	unsigned int GetSelectedServerID();

	bool	IsAutoRun();
	void	ToggleAutoRun();
	void	ResetAutoRun();

	/// �Ϻ� ��Ʈ�ʻ� ������ ���� �޺��ڽ��� Ȱ��ȭ��ų���ΰ�?
	bool				IsActiveRouteComboBox();
	void				SetJapanPartnerString( const char* str );
	const std::string&	GetJapanPartnerString();

	/// Ŭ���̾�Ʈ ȭ�Ϲ���
	std::string&		GetClientFileVersion();	

	//
	void		SetServerInfo( ServerInfo & serverInfo_ ){	m_sServerInfo = serverInfo_;	}
	ServerInfo	& GetServerInfo(){	return m_sServerInfo;	}

private:
	int		Init();
	void	Exit();
	void	Load_NewVersionData();
	bool	Load_BasicDATA();
	bool	Load_BasicDATA2();//�ð��� ���� �ɸ��� Data Loading( CGameLoadSelectAvatarState���� Loading �Ѵ�.)
	bool	Load_DataNotUseThread();///�����带 ����ؼ� ������ �ȵǴ� Data�� Load�ϴ� Method: ������ ���ϸ����ؾ��Ѵ�.
	void	Free_BasicDATA ();
	void	MakeFont();

	//void	LoadUserCursor();


	void	ProcInput();///Mouse�� Keyboard�Է� ó��

	CGameState*		MakeState( int iState );
	
	void	MakeBadWordTable();
	void	ProcCommand();



private:
	CGameState*		m_pCurrState;
	CGameState*		m_GameStates[ GS_MAX ];

	CAppMsgQ		m_WndMsgQ;

	int				m_iCheckFrame;

	/*HCURSOR			m_hCursor[ CURSOR_MAX ];
	HNODE			m_hDxCursor[ CURSOR_MAX ];*/
	//int				m_iCurrentCursorType;
	//bool			m_bUseUserMouse;

	POINT			m_ptCurrMouse;///���� Frame���۽��� ���콺 ��ġ
	POINT			m_ptPrevMouse;///���� Frame�� ���콺 ��ġ
	//CInfo			m_MouseInfo;///���콺 ���¿� ���� �������� ��������

	//----------------------------------------------------------------------------------------------
	/// ���콺 Ÿ������ ����Ʈ
	//----------------------------------------------------------------------------------------------
	CMouseTargetEffect	m_MouseTargetEffect;

	
	gsv_TELEPORT_REPLY	m_gsv_TELEPORT_REPLY;

	D3DXVECTOR3	m_RayOrig;
	D3DXVECTOR3	m_RayDir;

	
	DWORD			m_dwDebugFlag;

	int				m_iRepairMode;
	int				m_iUsingRepairItemInvenIdx;///����������� ���� �����ص� ����
	WORD			m_wRepairNpc;///������ ���ִ� Npc�� ServerObject Index

	WORD			m_wAppraisalNpc;
	int				m_iAppraisalMode;

	DWORD			m_dwRight;///����
	__int64			m_i64AppraisalCost;


	bool			m_bLoadedBasicData2;
	int				m_iJoinChannelNo;


	CTCommandQ		m_CommandQ;

	WORD			m_paytype;
	std::string		m_expireTime;
	DWORD			m_time_setexpiretime;
	int				m_expiretime_type;

	//Japan Billing.
	std::string		m_expireTimeExt[PAY_FLAG_JP_SIZE];
	DWORD			m_time_setexpiretimeExt[PAY_FLAG_JP_SIZE];
	int				m_expiretime_typeExt[PAY_FLAG_JP_SIZE];


	unsigned int	m_uiSelectedServerID;
	bool			m_bAutoRun;

	std::string		m_JapanPartnerString;
	CFileVersionInfo m_ClientFileVersion;


	ServerInfo		m_sServerInfo;
	
};
#endif