#ifndef _OBJFIXEDEVENT_
#define _OBJFIXEDEVENT_

#include "cobjGROUND.h"


enum EVENT_OBJ_STATE
{
	EVENT_OBJ_INIT_STATE = 0,
	EVENT_OBJ_CLOSE_STATE = 1,
	EVENT_OBJ_OPEN_STATE = 1,
};


//----------------------------------------------------------------------------------------------------
/// class CObjFixedEvent
/// �̺�Ʈ�� ���� ���� ������Ʈ
//----------------------------------------------------------------------------------------------------

class CObjFixedEvent :	public CObjFIXED
{
private:
	/// ExecEventScript ȣ��� ���ÿ� ���õ�.
	int				m_iCurrentState;


	int				m_iUniqueEventID;
	std::string		m_strTriggerName;
	std::string		m_strScriptName;

	/// ����� �����ϱ����� ���� ����..
	HNODE			m_PositionDummy;
	HNODE			m_AnimatibleDummy;


	/// �ѹ� �浹�ؼ� �̺�Ʈ�� �߻���Ų ������Ʈ�� �ٽ� �浹���� ���� -> �浹���� �Ǳ������� �̺�Ʈ�� �ٽ� �߻���Ű�� �ʴ´�.
	DWORD			m_dwCollisionCheckFrame;

	bool			m_bCanCheckCollision;


public:
	CObjFixedEvent(void);
	~CObjFixedEvent(void);

	int				GetCurrentState(){ return m_iCurrentState; }




	//////////////////////////////////////////////////////////////////////////////////////////
	/// < Inherited from CGameObj 

	/*override*/ int					Get_TYPE()		{	return OBJ_EVENTOBJECT;	}	
	
	/// < 
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	/// < Inherited from CObjFIXED 

	/*override*/char*				Make_ZNAME(int iCreateOrder, short nPartIdx)	{	return CStr::Printf (NAME_TREEOBJ, iCreateOrder, nPartIdx);	}	
	
	/// < End 
	//////////////////////////////////////////////////////////////////////////////////////////	


	void			SetUniqueEventID( int iID ){ m_iUniqueEventID = iID; }
	void			SetTriggerName( char* pFileName ){ m_strTriggerName = pFileName; }
	void			SetScriptName( char* pFileName ){ m_strScriptName = pFileName; }

	int				GetUniqueEventID(){ return m_iUniqueEventID; }
	const char*		GetTriggerName(){ return m_strTriggerName.c_str(); }			/// Ʈ���� �̸��� ��.
	const char*		GetScriptName(){ return m_strScriptName.c_str(); }					/// ������ ��ũ���� �̸��� ����.


	//-------------------------------------------------------------------------------------------------
	/// �̺�Ʈ ������Ʈ�� ����� ���̰ų� ���� ���ؼ� �߰����� ��尡 �ʿ��ϴ�.	
	//-------------------------------------------------------------------------------------------------
	HNODE			GetPositionNode(){ return m_PositionDummy; }
	HNODE			GetAnimatibleNode(){ return m_AnimatibleDummy; }

	/*override*/	void UnlinkVisibleWorld (void);	
	/*override*/	void LinkToModel( HNODE hModel );	
	/*override*/	void InsertToScene (void);
	/*override*/	void RemoveFromScene (bool bIncludeEFFECT);

	/*override*/	void SetPOSITION (D3DVECTOR &Position);
	/*override*/	bool Create (CMODEL<CFixedPART> *pMODEL, D3DVECTOR &Position, bool bCreateEffect=false);
	/*override*/	void Delete ();


	//-------------------------------------------------------------------------------------------------
	/// ���¿� ���� �̺�Ʈ �ߵ�
	//------------------------------------------------------------------------------------------------
	void			ExecEventScript( int iCurrentState, bool bJustEnd = false, bool bCollisionEvent = false, bool bInsertToScene = false );

	void			ExecTrigger();


	/// ���� �浹�� üũ�Ҽ� �ִ� �����ΰ�?
	/// �ѹ� �浹�� Ư���� ��Ȳ��( �����Ÿ��� �������ٴ��� �ƴϸ� ��Ÿ�� ) �Ͼ�� ������ disable �ȴ�.
	bool			CanCheckCollision(){ return m_bCanCheckCollision; }
	void			SetCheckCollisionFlag( bool bCanCheck ) { m_bCanCheckCollision = bCanCheck; }
	void			SetCollisionCheckFrame( DWORD dwCheckFrame ){ m_dwCollisionCheckFrame = dwCheckFrame; }
	DWORD			GetCollisionCheckFrame(){ return m_dwCollisionCheckFrame; }
};


#endif //_OBJFIXEDEVENT_