/*
	$Header: /Client/OBJECT.h 32    05-08-30 5:17p Gioend $
*/
#ifndef	__OBJECT_H
#define	__OBJECT_H
#include "CModelCHAR.h"
#include "CObjGROUND.h"
#include "CObjUSER.h"
#include "Game.h"
#include <vector>
#include <list>


#define SPECIAL_WARP_OBJECT				1
#define SPECIAL_COLLISION_OBJECT		2
#define WARP_OBJECT_INDEX_OFFSET		1000000

class CMAP;
class CObjFixedEvent;


///CMap::Load���� ���� NPC����

struct	NpcInfoInMap{
	CMAP*		m_pMap;///npc�� �ε�� CMAP�� �޸� ������
	int			m_iNpcID;///stb No
	D3DVECTOR	m_Position;
};

/// 
/// Object manager class
///
class CObjectMANAGER {
private:
	classDLLNODE< int > *m_pFindNODE;

	int		m_iEmptySlotNo;								/// < ���� ����ִ� ���� ���� ��ȣ.			
	short	m_nServer2ClientOBJ[ MAX_SERVER_OBJECTS ];	/// < �������� �� �ε����� ������Ʈ�� Ŭ���̾�Ʈ�� � �����ΰ�?
	WORD	m_wClient2ServerOBJ[ MAX_CLIENT_OBJECTS ];	/// < ������ ���� ���� �ε��� ��ȣ ����.
	
	int		m_iObjectCNT[ OBJ_MAX_TYPE ];				/// < ������Ʈ Ÿ�Դ� ������Ʈ ����.
	int		m_iTotalObjCNT;								/// < ��ü ������Ʈ ����

	DWORD	m_dwCurTIME;
	DWORD	m_dwPassTIME;


	//----------------------------------------------------------------------------------------------------
	/// �̴ϸ��� ǥ�ø� ���� ����Ʈ..
	std::vector< int >				m_ViewObjectList;
	///MAP���� �ε�� NPC�� �̴ϸʿ� ǥ���ϱ� ���Ͽ� �ӽ� �����ϴ� List
	std::list< NpcInfoInMap >		m_listNpcInfo;
	//----------------------------------------------------------------------------------------------------


	static	CObjectMANAGER* m_pInstance;
	CObjectMANAGER ();
	~CObjectMANAGER ();

public :
	CGameOBJ	*m_pOBJECTS[ MAX_CLIENT_OBJECTS ];		///< ������Ʈ �迭.
	classDLLIST< int >	m_CharLIST;						///< ĳ���� ������Ʈ ��ȣ( ������Ʈ �迭���� �ε���) ����Ʈ
	classDLLIST< int >	m_CnstLIST;						///< �ǹ�, ����, ��Ÿ ������Ʈ ��ȣ ����Ʈ
	classDLLIST< int >	m_ItemLIST;						///< ������ ������Ʈ ��ȣ ����Ʈ

	std::map< int, CObjFixedEvent* >  m_EventObjectList;			///< �̺�Ʈ ������Ʈ ����Ʈ.

	static	CObjectMANAGER* Instance ();
	static  void	Destroy ();


	void ResetTime(){ m_dwCurTIME = g_GameDATA.GetGameTime(); m_dwPassTIME = 0; }


	/// �̴ϸ��� ǥ�ø� ���� ����Ʈ..
	void	ReserverViewObjectList( int iReserveCnt = 100 ){ m_ViewObjectList.reserve( 100 ); }
	void	ClearViewObjectList(){ /*m_ViewObjectList.clear()*/ m_ViewObjectList.erase( m_ViewObjectList.begin(), m_ViewObjectList.end() ); }
	void	AddViewObject( int Index ){ m_ViewObjectList.push_back( Index ); }
	int		GetViewObjectCnt(){ return m_ViewObjectList.size(); }
	int		GetViewObjectIndex( int Index ){ return m_ViewObjectList[ Index ]; }


	/// ������Ʈ�� �ű׷����� �ִ´�.
	void	InsertToScene ();							
	/// �ű׷����κ��� ������Ʈ ����.
	void	RemoveFromScene ();							


	/// �� ���� �Ҵ�( ������ �Ҵ� ���� ���ʺ��� �˻� )
	int		Get_EmptySlot ();							
	/// �ش� ���Կ� ������Ʈ �Ҵ�
	/// @param wServerObjectIndex ���������� ������Ʈ �ε���..
	bool	Set_EmptySlot (short nSlotNO, WORD wServerObjectIndex, CGameOBJ *pObject); 


	/// ��� �ɹ� ����Ʈ Ŭ����.
	void	Clear (short nExceptObjIndex);
	/// �ش� �ε����� ������Ʈ ����.
	void	Del_Object (short nIndex);
	/// �ش� ������Ʈ ����.
	void	Del_Object (CGameOBJ *pObject);



	/// ���� �߰�.
	int		Add_GndTREE( short nObjID, D3DVECTOR &Position, D3DXQUATERNION &Rotate, D3DVECTOR &Scale );
	/// �ǹ� �߰�.
	int		Add_GndCNST( short nObjID, D3DVECTOR &Position, D3DXQUATERNION &Rotate, D3DVECTOR &Scale );
	/// ������ �߰�
	int		Add_GndITEM( WORD wServerObjectIndex, tagITEM &sITEM, tPOINTF &Position, float fRefZPos = 0.0f, bool bUseRefZPos = false );
	/// ���� �߰�
	int		Add_MORPHER( short nObjID, D3DVECTOR &Position, D3DXQUATERNION &Rotate, D3DVECTOR &Scale );
	/// Warp object �߰�
	int		Add_WARP( short nObjID, D3DVECTOR &Position, D3DXQUATERNION &Rotate, D3DVECTOR &Scale );
	/// Collision object �߰�
	int	    Add_CollisionBox( int iReserved, D3DVECTOR &Position, D3DXQUATERNION &Rotate, D3DVECTOR &Scale );

	/// �� �߰�
	int		Add_MobCHAR( WORD wServerObjectIndex, short nMobIdx, const D3DVECTOR& Position, short nQuestIDX, bool bRunMODE);
	/// NPC ĳ���� �߰�
	int		Add_NpcCHAR( WORD wServerObjectIndex, short nNpcIdx, const D3DVECTOR& Position, short nQuestIDX, float fModelDIR );
	/// �ƹ�Ÿ ĳ���� �߰�.
	bool	Add_AvtCHAR(CObjAVT *pAvtCHAR, D3DVECTOR &Position, BYTE btCharRACE);

	/// Cart �߰�
	CObjCART*	Add_CartCHAR( int iPetType, CObjAVT* pOwner, WORD wServerObjectIndex );


	/// Event object �߰�
	int		Add_FixedEvent( int iEventID, short nObjID, 
							const char* pTriggerFileName,
							const char* pConFileName,
							D3DVECTOR &Position, D3DXQUATERNION &Rotate, D3DVECTOR &Scale );


	/// ���ο� ���� ĳ���� �߰�.
	CObjUSER  *New_UserCHAR (WORD wServerObjectIndex, char *szName);
	/// ���ο� �ƹ�Ÿ �߰�.
	CObjAVT   *New_AvtCHAR  (WORD wServerObjectIndex, char *szName);


	/// �����ε����� Ŭ���̾�Ʈ �ε��� ���
	void	Set_ServerObjectIndex (short nClientObjectIndex, WORD wServerObjectIndex);
	/// ���� �ε����� Ŭ���̾�Ʈ �ε����� ����.
	short	Get_ClientObjectIndex (WORD  wServerObjectIndex)	{	return m_nServer2ClientOBJ[ wServerObjectIndex ];	}
	/// Ŭ���̾�Ʈ �ε����� ���� �ε����� ����.
	WORD	Get_ServerObjectIndex (short nClientObjectIndex)	{	return m_wClient2ServerOBJ[ nClientObjectIndex ];	}

	/// ���� �ε����� Ŭ���̾�Ʈ ������Ʈ�� ��´�.
	inline CGameOBJ *Get_ClientOBJECT  (WORD wServerObjectIndex, t_ObjTAG ObjTYPE=OBJ_NULL)
	{
		return this->Get_OBJECT( this->Get_ClientObjectIndex(wServerObjectIndex), ObjTYPE);
	}
	/// ���� �ε����� ĳ���� ������Ʈ�� ��´�.
	inline CObjCHAR*Get_ClientCharOBJ (WORD wServerObjectIndex, bool bCheckHP)
	{
		return this->Get_CharOBJ( this->Get_ClientObjectIndex(wServerObjectIndex), bCheckHP );
	}

	///ĳ�����̸����� ĳ���� ������Ʈ�� ��´�.
	CObjCHAR*	Get_ClientCharOBJByName( const char* szName );

	/// ���� �ε����� �ƹ��� ������Ʈ�� ��´�.
	inline CObjAVT*Get_ClientCharAVT (WORD wServerObjectIndex, bool bCheckHP)
	{
		return this->Get_CharAVT( this->Get_ClientObjectIndex(wServerObjectIndex), bCheckHP );
	}
	/// ���� �ε����� ������ ������Ʈ�� ��´�.
	inline CObjITEM*Get_ClientItemOBJ (WORD wServerObjectIndex)
	{
		return this->Get_ItemOBJ( this->Get_ClientObjectIndex(wServerObjectIndex) );
	}


	CGameOBJ*	Get_OBJECT  (int iObjectIndex, t_ObjTAG ObjTYPE=OBJ_NULL);
	CObjCHAR*	Get_CharOBJ (int iObjectIndex, bool bCheckHP);
	CObjAVT*	Get_CharAVT (int iObjectIndex, bool bCheckHP);
	CObjITEM*	Get_ItemOBJ (int iObjectIndex);
	
	CObjCHAR*Get_FirstCHAR ();	// ai test function ...
	CObjCHAR*Get_NextCHAR ();	// ai test function ...


	int		 Get_ObjectCount(t_ObjTAG Type)	{	return m_iObjectCNT[ Type ];	}
	int		 Get_ObjectCount ()				{	return m_iTotalObjCNT;			}

	char	*Get_NAME (int iTargetObjectIndex);
	DWORD	 Get_PassTIME ()				{	return m_dwPassTIME;			}
	DWORD	 Get_CurTIME ()					{	return m_dwCurTIME;				}

	void	 ProcOBJECT ();


	void		AddNpcInfo( CMAP* pMap, int iNpcNo, D3DVECTOR Position );
	void		RemoveNpcInfo( CMAP* pMap );
	void		ClearNpcInfoList();
	const std::list< NpcInfoInMap >& GetNpcInfoList(); 


	//-------------------------------------------------------------------------------------------------
	/// �̺�Ʈ ������Ʈ ����
	CObjFixedEvent*	GetEventObject( int iEventID );

	void		ResetClanMarkInfo( DWORD dwClanID, WORD crc16 );

} ;
extern CObjectMANAGER *g_pObjMGR;
extern CObjUSER *g_pAVATAR;

//-------------------------------------------------------------------------------------------------
#endif
