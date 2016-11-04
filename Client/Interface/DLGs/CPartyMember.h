#ifndef _CPARTYMEMBER_
#define _CPARTYMEMBER_

#include "Net_Prototype.h"
#include "../Icon/CIcon.h"
#include <list>

/**
* ��Ƽâ( CPartyDlg)���� �� ��Ƽ������ ���¸� �����ϰ� �׸��� Class
*
* @Author		������
* @Date			2005/9/12
*/
class CPartyMember 
{
public:
	CPartyMember(void);
	CPartyMember( DWORD dwObjectTag, WORD wObjSvrIdx, const char* pszName );
	virtual ~CPartyMember(void);


	void		Draw();
	unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);
	void		Update( POINT ptMouse );


	void		SetSelected( bool b ){ m_bSelected = b; }
	bool		IsSelected(){ return m_bSelected; }
	void		SetPosition( POINT pt );
	int			GetHeight();

	WORD		GetObjSvrIdx();
	void		SetObjSvrIdx( WORD wObjSvrIdx );

	DWORD		GetObjectTag();

	const char* GetName(){ return m_stName.c_str(); }


protected:
	bool	IsInside(POINT pt);
	bool	AddStatusIcon( DWORD dwFlag );					/// ���� ������ �߰�
	void	ResetStatusIconsPosition();						/// ���� �����ܵ��� ��ġ�� �������Ѵ�.

protected:
	/// Member�� ����
	WORD				m_wObjSvrIdx;						/// ��Ƽ���� ���� �ε���
	DWORD				m_dwObjectTag;						/// ��Ƽ���� ���� �±�( �������� ������ ID , �����ε����� �������� ������ �ִ� )

	std::string			m_stName;							/// �̸�
	bool				m_bLeader;							/// ��Ƽ���ΰ�?

	RECT				m_rcThis;							/// �������̽� ����
	int					m_iWidth;
	int					m_iHeight;
	bool				m_bSelected;						/// ���õǾ� ���°�?

	std::list<CIconStatus>	m_listStatusIcon;				/// ���� ������ ����Ʈ

	int					m_iGuageIdx;						/// ü�� ������ �׷��� ID
	int					m_iGuageBGIdx;						/// ü�� ������ ��� �׷��� ID

};
#endif