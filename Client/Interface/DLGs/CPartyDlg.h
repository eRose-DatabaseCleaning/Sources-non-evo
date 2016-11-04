#ifndef _CPARTYDLG_
#define	_CPARTYDLG_

#include "../../../TGameCtrl/TDialog.h"
#include "Net_Prototype.h"

class CPartyMember;

#include "../../GameCommon/IObserver.h"
#include <list>

typedef	list< CPartyMember* >			LIST_PARTY_MEMBER;
typedef LIST_PARTY_MEMBER::iterator		LIST_PARTY_MEMBER_ITER;


/**
* ��Ƽ�� ��Ƽ������ ��Ƽ���� �����ִ� ���̾�α�
*	- ��Ƽ���� �׻� ����Ʈ�� �Ǿ�Node�� 
*	- Observable : CParty
*
* @Todo		�����ð����� ����ġ�� ��� �κ� �߰� �ʿ�
* @Author	������
* @Date		2005/9/12
*/
class CPartyDlg : public CTDialog, public IObserver
{

public:
	CPartyDlg(void);
	virtual ~CPartyDlg(void);
	virtual void Draw();
	virtual void Show();
	virtual void Hide();
	virtual void Update(POINT ptMouse);
	virtual void MoveWindow( POINT pt );
	virtual unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);

	virtual void Update( CObservable* pObservable, CTObject* pObj );
	
	void			ClearMemberList();

protected:

	void			OnLButtonUp( unsigned iProcID );

	WORD			GetSelectedMemberSvrIdx();
	DWORD			GetSelectedMemberUserTag();


	void			ResetMemberPositions();										/// ��Ƽ���� �Ѹ� ���������� ǥ�� ��ġ���� �� ������ش�.

	CPartyMember*	FindMember( WORD wObjSvrIdx );
	CPartyMember*	GetSelectedMember();

protected:

	enum{
		IID_BTN_CLOSE		= 10,	/// ��Ƽâ �ݱ� == IID_BTN_LEAVE�� ���� ó��
		IID_BTN_ENTRUST		= 11,	/// ��Ƽ������ ����
		IID_BTN_BAN			= 12,	/// ��Ƽ���� �߹�
		IID_BTN_LEAVE		= 13,	/// ��Ƽ ������
		IID_BTN_OPTION		= 14,	/// ��Ƽ �ɼ�â ����
	};


	LIST_PARTY_MEMBER		m_listPartyMember;							/// ��Ƽ���� ���� �����̳�


	int						m_iGuagePartyExpIdx;						/// ��Ƽ ������ ������ �׷��� ID
	int						m_iGuagePartyExpBGIdx;						/// ��Ƽ ����ġ ������ ��� �׷��� ID
};
#endif