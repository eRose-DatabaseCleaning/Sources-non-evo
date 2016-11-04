#ifndef _IT_MGR_H
#define _IT_MGR_H

#include "InterfaceType.h"



#include "dlgs/CMsgBox.h"
#include "command/CTCmdOpenDlg.h"

#include "../common/citem.h"
#include "../Game.h"


#include "TGameCtrl.h"
#include "TCommandQ.h"

#include <list>

struct t_PACKET;



class CBasePAN;
class CChatDLG;
class CMakeDLG;
class CDialogDlg;
class CInventoryDLG;
class CInventoryDLG;
class CDealDLG;
class CQuickBAR;
class CCharacterDLG;
class CMinimapDLG;
class CTargetObjectDLG;
class CRestartDLG;
class CSystemDLG;
class CSkillDLG;
class CExchangeDLG;
class CTDialog;
class CTDrawImpl;
class CTCommand;
class CTCmdNumberInput;
class CInventory;
class CSkillSlot;
class CAvatarInfoDlg;
class CPrivateChatDlg;
class CTCmdInputName;
class CITState;
class CTEditBox;
class CTDrawImpl;

typedef std::list< CTDialog* >			list_dlgs;
typedef list_dlgs::iterator				list_dlgs_itor;
typedef list_dlgs::reverse_iterator		list_dlgs_ritor;

class CReloadProcess;
class CIconDialog;
class CNotifyButtonDlg;

/**
* ĳ���� ������ ������ �����ų� ĳ���� ����â���� �ٽ� �̵��ϱ� �������� �������̽� ���� ������
*  - WM_LBUTTONDOWN�� �Ͼ�ų� Show�� ��Dlg�� ����Ʈ�� �ǵڷ� �Űܼ� �׸���� �� ���������� �ϰ� ó���� ���� ���� �ϰ� �Ѵ�.
*
*
* @Author		������
*
* @Date			2005/9/5
*
*/
class IT_MGR
{
	/// IT_MGR�� ����� ������ �ʿ��� Ŭ�������� friend�� ����
	friend class CTCmdCreateMsgBox;				/// �޼���â�� ���� ���� TCommand
	friend class CTCmdDeleteMsgBox;				/// �޼���â�� �ݴ� TCommand
	friend class CTCmdClosePrivateChatDlg;		/// 1:1��ȭâ�� �ݴ� TCommand
	friend class CTCmdOpenPrivateChatDlg;		/// 1:1��ȭâ�� ���� TCommand

	/// IT_MGR�� State��
	friend class CITStateNormal;				
	friend class CITStateRepair;
	friend class CITStateAppraisal;
	friend class CITStateDead;
public:
	IT_MGR();
	~IT_MGR();

	enum{											/// ä�� �Է� ��� Ÿ��
		INPUTTYPE_NORMAL,							/// ä�ýø��� Enter�� �Է��ϰ� �ٽ� ���͸� �Է½� ä�ø��� ����ȴ�.
		INPUTTYPE_AUTOENTER,						/// Show�ÿ� �ش� Dialog�� EditBox�� �ִٸ� �ڵ� �Էµȴ�.Enter�� �ڵ����� �ٽ� Focus�� ������.
	};

	enum{											/// ä�ñ� Ÿ��	
		CHAT_TYPE_ALL,
		CHAT_TYPE_SHOUT,
		CHAT_TYPE_PARTY,
		CHAT_TYPE_WHISPER,
		CHAT_TYPE_NOTICE,
		CHAT_TYPE_SYSTEM,
		CHAT_TYPE_ERROR,
		CHAT_TYPE_QUEST,
		CHAT_TYPE_QUESTREWARD,
		CHAT_TYPE_CLAN,
		CHAT_TYPE_TRADE,
		CHAT_TYPE_ALLIED,
		CHAT_TYPE_FRIEND,
		CHAT_TYPE_MAX,
	};

	enum {											/// ���� Ÿ��
		STATE_NORMAL,
		STATE_REPAIR,
		STATE_APPRAISAL,
		STATE_DEAD,
		STATE_WAITDISCONNECT,
		STATE_NPCDIALOG,
		STATE_RIDECART,
		STATE_MAX,
	};

	enum{
		QUERYTYPE_NPC,								/// NPC ��ȭ
		QUERYTYPE_ITEM,								/// ������ ���� ���Ǵ� ��ȭâ
		QUERYTYPE_SELECT,							/// ���ù����� �����Ǿ� ������ �����ϴ� ��ȭâ( �糪������ ��������Ʈ )
	};

	/// ���ӳ� ��� ��Ʈ�� ���տ� ���۷� ���..
	char m_MessageBuf[ 1024 ];


	bool Init();
	void Free();
	UINT MsgProc(UINT uiMsg,WPARAM wParam,LPARAM lParam);
	void Update();
	
	
	///�ڵ��ÿ��� �̰����� �ϰ��� ����ȭ�Ǹ� CRecvPacket���� �̵���Ű��
	///bool PacketProc( t_PACKET * pRecvPacket );
	//void RegToolTip( const CInfo& Info );
	// *--------------------------------------------------------------* //
	void InitDLG();
	void FreeDLG();
	void DrawDLGs(POINT ptMouse);


	CChatDLG*			GetChatDLG();
	CDialogDlg*			GetQueryDLG();
	CQuickBAR*			GetQuickBAR();
#ifdef _NEW_UI
	CQuickBAR*			GetQuickBAR_EXT(); // 2nd skillbar
#endif
	CMinimapDLG*		GetMinimapDLG();
	CExchangeDLG*		GetExchangeDLG();
	CPrivateChatDlg*	GetPrivateChatDlg( DWORD dwUserTag );

	
	void				OpenPrivateChatDlg( DWORD dwUserTag, BYTE btStatus, const char* pszName );			/// 1:1 ��ȭâ ����


	// *------------------------------------------------------------------------------------------------------------------------------* //
	/// NPC ��ȭâ ����
	bool	OpenQueryDLG(short nSX,short nSY,char* szQuery,short nMaxCharLen,int iEventID,void (*fpCloseProc)(int iEventID), int iEventOwner, int iEventDlgType );

	void	CloseQueryDlg();																				/// NPC��ȭâ�� �ݴ´�
	
	bool	QueryDLG_AppendExam(char* szExam,int iEventID,void (*fpExamEvent)(int iEventID), int iType );	/// NPC ��ȭâ���� ���ù��� �߰��Ѵ�.
	// *------------------------------------------------------------------------------------------------------------------------------* //

	void	UpdateQuickSlot();																				/// CHotIconSlot ������ �������� QuickDlg �� ���Ե��� ������Ʈ
	
	short	GetItemInventoryTAB(tagITEM sItem);																/// ������â���� �������� �� ���� ���´� 
	
	void	Close_store();																					/// NPC������ �ݴ´� 



	void	ShowNumberInputDlg( CTCmdNumberInput* pCmd , CTObject* pCmdParam ,__int64 iMaxNumber = -1);		/// ���� �Է�â ����
	
	bool	OpenMakeDlg( int iSkillSlotIdx );																/// ����â ����

	void	OpenDialogBySkill( int iSkillSlotIdx  , int iTargetDialogID );									/// ��ų�� ���̾�α׸� ���� ���Ǵ� Method

	bool	OpenInputNameDlg( CTCmdInputName* pCmd );														/// �̸� �Է�â ����

	/*-----------------------------------------------------------------------------*/
	/// �޼��� �ڽ� ���� methods
	void OpenMsgBox( const char* szMsg, 
		short int iButtonType = CMsgBox::BT_OK , bool	bModal = true,
		unsigned int iInvokerDlgID =  0, CTCommand* pCmdOk = NULL, CTCommand* pCmdCancel = NULL ,
		int iMsgType = 0
		);
	
	void	OpenMsgBox_CartRide( const char* szMsg, 
		short int iButtonType = CMsgBox::BT_OK , bool	bModal = true,
		unsigned int iInvokerDlgID =  0, CTCommand* pCmdOk = NULL, CTCommand* pCmdCancel = NULL ,
		int iMsgType = 0, WORD parm1 = 0, WORD parm2 = 0
		);

	void	OpenMsgBox2(CreateMsgBoxData& Data);

	/*---------------------------------------------------------------------------*/
	void		PostMsg2Dlg( int dialog_id, unsigned msg_id, unsigned param1, unsigned param2 );
	void		AppendChatMsg( const char* pszMsg, int iType ,DWORD forceapply_color = 0 );


	void		OpenDialog( int iDlgType, bool bToggle = true , int iPosX = -1, int iPosY = -1 );	/// ���̾�α׸� ����
	void		CloseDialog( int iDlgType );														/// ���̾�α׸� �ݴ´�.

	CTDialog*	FindDlg( DWORD dwDlgType );												/// �ش� ID�� ���̾�α׸� ã�Ƽ� �����͸� ����
	
	CMsgBox*	FindMsgBox( int iMsgType );												/// �ش� Ÿ���� �޼��� �ڽ��� �����͸� ����

	int			IsDlgOpened( int iDlgID );												/// �ش� ID�� ���̾�αװ� ���� �ִ°�?
	bool		IsOpenedMsgBox( int iType );											/// �ش� Ÿ���� �޼��� �ڽ��� 1�� �̻� ���� �ִ°�?

	bool		AddTCommand( int iDlgID, CTCommand* pCmd );								/// Command�� Queue�� �߰�

	/*---------------------------------------------------------------------------*/
	void		InitInterfacePos();														/// �������̽� �ʱ�ȭ, �ػ󵵰� �ٲ�ų� �������̽��� ���� ��ġ�� �ű�� ������ ����ϴ� Method
	void		SetInterfacePosBySavedData();											///	������ִ� Dialog Position�� ������� �����ͼ� Dialog�� ��ġ���� �����Ѵ�.
	void		SetInterfacePos_After();												/// ���� �������� ������ Pos ����

	void		ServerDisconnected();													/// �������� ������ ����������� ó���ϴ� method
	
	
	bool		IsMouseOnInterface();													/// ���� ���콺�� �������̽����� �ִ°�?

	/*---------------------------------------------------------------------------*/
	void		AddDialogIcon( int iDialogType );
	void		DelDialogIcon( int iDialogType );
	CIconDialog* FindDialogIcon( int iDialogType );

	void		SetDialogIconFromMenu( CIconDialog* pIcon );
	void		DelDialogiconFromMenu();
	void		SetDialogIconPosition( int iDialogType, POINT pt );

	/*---------------------------------------------------------------------------*/
	/// ���� ���濡 ���� �������̽� ���� methods
	void		BeginRepair();															/// ���� ����
	void		BeginAppraisal();														/// �����۰���  ����
	void		EndRepair();															/// �������� ��
	void		EndAppraisal();															/// �����۰��� ��

	//void	BeginRepairItem();

	/*---------------------------------------------------------------------------*/
	int			GetState();																/// ���� ���¸� ���ϴ� method
	void		ChangeState( int iIndex );												/// ���� ���� method

	/*---------------------------------------------------------------------------*/
	void		CloseDialogAllExceptDefaultView();										/// �⺻������ ���̴� ���̾�α׸� ������ ��� ���̾�α׸� �ݴ´�.

	/*---------------------------------------------------------------------------*/
	void		ShowQuestStartMessage( int index );										/// ����Ʈ ���۽� ä��â�� ǥ���ϴ� method

	/*---------------------------------------------------------------------------*/
	void		AddNotifybutton( int iEventIndex );										/// �˸���ư�� �߰��Ѵ�.
	void		ClearNotifyButtons();													/// ��� �˸���ư�� �ݴ´�.
	
	/*---------------------------------------------------------------------------*/
	CTEditBox*	GetFocusEditBox();														/// ���� �Է� ��Ŀ���� ���� TEditBox�� �����͸� ���Ѵ�.

	/*---------------------------------------------------------------------------*/
	/// ������ Ȥ�� �������ÿ� �������� ���� ���Ḧ �����ϱ� ���� Methods
	void		SetWaitDisconnectTime( int iSec );
	int			GetWaitDisconnectTime();

	void		SetWaitDisconnectType( int iType );
	int			GetWaitDisconnectType();
	/*---------------------------------------------------------------------------*/
	CReloadProcess*				GetReloadProcess();
	CTDrawImpl*					GetDrawImplPtr();

	void		RefreshDlg();															//  �ػ� ���濡 ���� ��ġ ������.

private:
	void		ProcCommand();															/// Command Queue�� ����Ǿ� �ִ� TCommand�� �����Ѵ�.

	void		AppendDlg(short nDlgType,CTDialog* pDLG, int iID = 0);					/// Dialog���� �����ϴ� �����̳ʿ� �߰�
	void		InsertDlg(short nDlgType, CTDialog* pDlg,  int iNextID , int iID = 0 );	/// �����̳ʿ��� Ư�� Dialog�ڿ� �ش� ���̾�α׸� �߰��Ѵ�.

	void		MoveDlg2ListEnd( CTDialog* pDlg );										/// Ư�� Dialog�� ����Ʈ�� �ǵڷ� �̵� ( ȭ�鿡�� �������� ���Եȴ�)
	void		MoveDlgIcon2ListEnd( CIconDialog* pIcon );								/// Ư�� Dialog Icon�� ����Ʈ�� �ǵڷ� �̵� ( ȭ�鿡�� �������� ���Եȴ�)

	void		ProcessEscKeyDown();													/// ESC Ű�� �������� ó���ϴ� Method

private:
	list_dlgs		m_Dlgs;																/// ���ӳ� ���Ǵ� �������̽� ���̾�α׵��� �����ϴ� �����̳�
	CTCommandQ		m_CommandQ;															/// Command Queue( ���̾�α׿� �������� Command�� �� ���̾�αװ� ������ �ִ´�)

	/*---------------------------------------------------------------------------*/
	/// ����ڰ� EscŰ�� �����ִ� â���� ���¸� ����(�������¸�)�Ҽ� �ִ� �ý����� ���� Member��	
	std::list< int >				m_listSavedDialog;									///Set�� �����ִ� Dialog���� ID
	std::list< std::list< int > >	m_StackDialogSet;									///Set�� ����Ǿ��� Stack;
	/*---------------------------------------------------------------------------*/

	std::list< CIconDialog* >		m_Icons;											/// ���� ���̾�α׵��� ȭ��� ������ȭ �Ǿ����� ����Ǵ� �����̳�
	std::map< int, POINT >			m_ptIcons;											/// ������ȭ�� ���̾�α��� ��ġ�� �����صδ� �����̳�

	CIconDialog*					m_pDialogIconFromMenu;								/// �޴�â���� ��ư�� �巡���Ҷ� �ʿ��� �⺻ �������� �����Ѵ�.
	CNotifyButtonDlg*				m_pNotifyButtonDlg;									/// �˸���ư�� �����ص� �����̳� ���̾�α�

	/*---------------------------------------------------------------------------*/
	//States
	CITState*	m_pCurrState;
	CITState*	m_pStates[ STATE_MAX ];
	/*---------------------------------------------------------------------------*/


	int			m_iWaitDisconnectTime;
	int			m_iWaitDisconnectType;													///0. Disconnect 1. Goto Select Avatar

	CReloadProcess*				m_pReloadProcess;										/// �����۹� ��ų�� ��Ÿ���� ���������� ǥ���ϱ� ���� ���


};

extern IT_MGR			g_itMGR;

#define g_MsgBuf g_itMGR.m_MessageBuf


#endif