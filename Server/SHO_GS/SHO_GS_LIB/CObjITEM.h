/**
 * \ingroup SHO_GS
 * \file	CObjITEM.h
 * \brief	�ʵ忡 ��ϵ� ������ ��ü
 */
#ifndef	__COBJITEM_H
#define	__COBJITEM_H
#include "CGameOBJ.h"
#include "CItem.h"
//-------------------------------------------------------------------------------------------------
class classUSER;
class CParty;

/**
 * \ingroup SHO_GS_LIB
 * \class	CObjITEM
 * \author	wookSang.Jo
 * \brief	�ٴۿ� ������ ������ ��ü ������ ó���ϴ� Ŭ����
 *			CGameOBJŬ������ ��ӹ���
 */
class CObjITEM : public CGameOBJ
{
private:
	CParty	   *m_pOwnerPARTY;
	DWORD		m_dwOwnerDBID;

public :
    tagITEM     m_ITEM;

	int			m_iOwnerObjIDX;			/// �������� �����Ҽ� �ִ� ����
	int			m_iRemainTIME;			/// �ͼӵǾ� �ִ� ���� �ð�

	bool		m_bDropperIsUSER;		/// ����ڰ� ���� �������̳�?
	CStrVAR		m_DropperACCOUNT;		/// ���� ������� ����
	CStrVAR		m_DropperNAME;			/// ���� �ɸ��� �̸�
	CStrVAR		m_DropperIP;			/// ���� ����� IP
	DWORD		m_dwDropperDBID;		/// db id
	DWORD		m_dwDropperClanID;

/*
	CObjITEM ();
*/
	virtual ~CObjITEM ();

    t_ObjTAG Get_TYPE ()						{	return OBJ_ITEM;	}

	void InitItemOBJ (classUSER *pDropper, tPOINTF &PosCUR, POINTS &PosSEC, tagITEM &sITEM, classUSER *pOwner=NULL, bool bActive=true, CParty *pOwnerPARTY=NULL);
	void SetACTIVE ();

	void Update_OwnerObjIDX ( classUSER *pUSER );
    bool Make_gsv_ADD_OBJECT( classPACKET *pCPacket );

	bool Is_FREE ()								{	return ( 0 == m_iOwnerObjIDX || m_iRemainTIME < ITEM_OBJ_FREE_TIME );	}

	bool Check_PrivateRIGHT(int iTryObjIDX)		{	return iTryObjIDX == m_iOwnerObjIDX;					}
	bool Check_PartyRIGHT( CParty *pTryPARTY )	{	return ( pTryPARTY && pTryPARTY == m_pOwnerPARTY );		}

	int	 Proc (void);
} ;

//-------------------------------------------------------------------------------------------------
#endif
