#ifndef _CICON_
#define _CICON_


#include "../../Common/CItem.h"
#include "../../../TGameCtrl/TObject.h"
#include "../CInfo.h"
//----------------------------------------------------------------------------------------------------
/// 
///			CSkillSlot				CItemSlot 
///				��						��
///			  CSkill				  CItem
///			   ���					   ���
///			CSkillIcon �� CIcon ��	CItemIcon
///				��						��
///				��--------------------------------> CSlot
///
//----------------------------------------------------------------------------------------------------


class CTCommand;
class CTDialog;
class CSlot;

/**
* ���ӳ��� ��� ������Ÿ���� �߻� Ŭ����
* �⺻������ �������޽��� ��� ���� �޽��, �׸��� ������Ʈ �޽�带 ������.
*
* @warning virtual Method�� �׻� CIconQuick���� ������ ���ٰ�
* @Author		������
* @Date			2005/9/6
*/
class CIcon : public CTObject
{
public:
	CIcon(void);
	virtual ~CIcon(void);


	//----------------------------------------------------------------------------------------------------
	/// Virtual methodes...
	//----------------------------------------------------------------------------------------------------
	virtual void	Draw();
	virtual void	ExecuteCommand();
	virtual CIcon*	Clone();
	virtual void	Clear();
	virtual void	GetToolTip( CInfo& ToolTip,  DWORD dwDialogType, DWORD dwType );
	virtual void	SetPosition( POINT pt ){ m_ptPosition = pt; }
	virtual bool	IsEnable();
	virtual const char*	GetName();
	virtual void	 Update( POINT ptMouse );
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual int		 GetIndex();

	POINT	GetPosition(){ return m_ptPosition; }

	void	SetEnable(){ m_bEnable = true; }
	void	SetDisable(){ m_bEnable = false; }
	bool	IsInside( POINT pt );
	//void	SetParent( CTDialog* pDialog ){ m_pParentDlg = pDialog; }

	void	SetGraphicID( int iID ){ m_iIconGraphicID = iID; }
	void	SetGraphicModuleID( int iID ){ m_iModuleID = iID; }

	int		GetWidth(){ return m_iWidth; }
	int		GetHeight(){ return m_iHeight; }
	void	SetToolTip(CInfo& Info){ m_ToolTip = Info; }
	void	SetSlot( CSlot* pSlot );
	CSlot*	GetSlot();

	void	SetCommand( CTCommand* pCommand );
	void    SetDrawColor( DWORD color );

protected:
	POINT m_ptPosition;

	///Graphic
	int			m_iWidth;
	int			m_iHeight;
	int			m_iIconGraphicID;																/// �̹��� �׷��� ID
	int			m_iModuleID;																	/// �̹��� ��� ID
	bool		m_bEnable;

	///Interface
	//CTDialog*	m_pParentDlg;
	CInfo		m_ToolTip;																		
	CSlot*		m_pSlot;																		/// Icon�� Slot�� Attach�ɰ�� �� ������ Pointer ����
	CTCommand*	m_pCommand;																		/// Icon�� �⺻ ���� Command( �����͸� ������ �ִ´� )
	DWORD		m_dwIconDrawColor;																/// Icon�� Draw�Ҷ� Texture�� ������ ����

};


/**
* �ƹ�Ÿ�� ���¸� ǥ���� ������
*	- ���� ��Ƽâ���� ��Ƽ���� ����ǥ�ÿ��� ���ȴ�.
*
* @Author		������
* @Date			2005/9/6
*/
class CIconStatus : public CIcon
{
public:
	CIconStatus();
	CIconStatus( int iModuleID, int iGraphicID );
	~CIconStatus(void){}

	virtual	void	Draw();
	virtual int		GetIndex();

	void SetGraphicID( int iModuleID, int iGraphicID );
};
#endif