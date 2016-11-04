#ifndef _SKILLDLG_
#define _SKILLDLG_


#include "../SlotContainer/CSkillSlot.h"
#include "../../gamecommon/IObserver.h"
#include "subclass/cslot.h"

#include "TDialog.h"

class CZListBox;



/**
* ���� ��� ��ų�� ���� ������ �����ִ� ���̾�α�
*
* @Warning		��ųƮ��â�� ���� ��ư�� �븸�ǿ����� ���ŵǾ� �ִ�( XML Data )
* @Author		������
* @Date			2005/9/14
*/
class CSkillDLG  : public CTDialog, public IObserver
{
private:

	int				m_iCurrentSkillTab;
public:
	CSkillDLG( int iType );
	virtual ~CSkillDLG();

	virtual void Draw( );
	virtual void Show( );
	virtual bool Create( const char* IDD );
	virtual bool IsVision();
	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );

	virtual void Update( CObservable* pObservable, CTObject* pObj );


	enum SKILL_TAB_TYPE
	{
		TAB_BASIC	= 0,
		TAB_ACTIVE	= 1,
		TAB_PASSIVE	= 2
	};

	enum{
		IID_BTN_CLOSE	= 10,
		IID_BTN_ICONIZE,
		IID_BTN_OPEN_SKILLTREE,
		IID_TABBEDPANE	= 20,
		IID_TAB_BASIC	= 2,
		IID_BTN_BASIC	= 25,
		IID_ZLISTBOX_BASIC	= 26,
		IID_TAB_ACTIVE	= 0,
		IID_BTN_ACTIVE	= 35,
		IID_ZLISTBOX_ACTIVE = 36,
		IID_TAB_PASSIVE	= 1,
		IID_BTN_PASSIVE = 45,
		IID_ZLISTBOX_PASSIVE = 46
	};

protected:

	bool	On_LButtonUP( unsigned iProcID, WPARAM wParam, LPARAM lParam );
	void	DrawTexts();														
	bool	IsValidLevelUp( int iSkillSlotIdx );								/// �������� ������ �����ΰ�?
	void	RemoveSkillListItem( CZListBox* pListBox, int iSkillSlotIndex );
	void	UpdateSkillListBySkillLevelUp( CZListBox* pListBox );			/// ��ų�� �������� ��ų�� ����ǥ�� ����
	void	UpdateSkillLists();												/// ��ų�� ���� �Ҹ� �ɷ�ġ�� ��Ÿ ������� ����
};


#endif //_SKILLDLG_