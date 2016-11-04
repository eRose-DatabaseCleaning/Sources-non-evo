// CQuestDlg.h: interface for the CQuestDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CQUESTDLG_H__98E8A820_E90F_4103_91BE_1D58E5862D0D__INCLUDED_)
#define AFX_CQUESTDLG_H__98E8A820_E90F_4103_91BE_1D58E5862D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDialog.h"
#include "../../Common/CUserData.h"
class CQuestListItem;
/**
* Quest ������ �����ִ� ���̾�α�
*
* @Warning		NPC ��ȭâ�� �����ִ� ���ȿ��� ���⸦ �Ҽ� ������ ���� ��ư�� Disable���Ѿ��Ѵ�.
* @Author		������
* @Date			2005/9/14
*/
class CQuestDlg : public CTDialog  
{
public:
	CQuestDlg();
	virtual ~CQuestDlg();

	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual	void	Update( POINT ptMouse );
	virtual void	Draw();
	virtual void    Show();
	virtual void    Hide();
	virtual void	MoveWindow( POINT pt );

	void UpdateQuestList();													/// ���� �������� ����Ʈ ��� ����

	enum{

		IID_BTN_ABANDON		= 50,											/// ����Ʈ ���� ��ư
		IID_BTN_CLOSE		= 10,
		IID_BTN_ICONIZE		= 11,
		IID_BTN_MINIMIZE	= 113,
		IID_BTN_MAXIMIZE	= 114,

		IID_ZLIST_QUEST		= 20,
		IID_ZLIST_SCROLLBAR = 21,
		IID_LIST_QUESTINFO	= 30,
		IID_ZLIST_ITEM		= 99,
		IID_PANE_QUESTLIST	= 100,
		IID_PANE_QUESTINFO	= 200

		//IID_LASTDRAWCTRL = 20,
	}; 
protected:

	bool On_LButtonUP( unsigned ID );  
	bool On_LButtonDN( unsigned ID );

	void DrawItems();														/// �ش� ����Ʈ�� ������ Draw
	void UpdateQuestDescription();											/// ���õ� ����Ʈ�� ���� ����
	int  GetSelectQuestIdx();												
	
	void Minimize();														/// �ּ�ȭ
	void Maximize();														/// �ִ�ȭ
	void ActiveListBoxMoveBegin();

	CWinCtrl* FindChildInPane( unsigned uiPaneID, unsigned uiChildID );
protected:
	///����Ʈ �ڽ��� ������ Quest Slot Idx�� �־�д�.: 
	///CUserDATA���� Quest Slot�� �տ� ���� �������� �� �ڿ� ���� �� ���Կ� �ִ´�.

	int					m_iList2QuestIdx[QUEST_PER_PLAYER];
	CQuestListItem*		m_pSelectedQuestItem;					/// ���õ� ����Ʈ�����ۿ� ���� ������

	int					m_iPrevSelectedQuestID;					/// ������ ���õǾ��� ����Ʈ �ε���
	bool				m_bMinimize;							/// �ּ� or �ִ�

	
};

#endif // !defined(AFX_CQUESTDLG_H__98E8A820_E90F_4103_91BE_1D58E5862D0D__INCLUDED_)
