/**
 * \ingroup SHO_GS
 * \file	GS_ThreadMALL.h
 * \brief	Ȩ������ ���θ����� ������ �������� ����ִ� ���� ����,���
 */
#ifndef	__GS_THREAD_MALL_H
#define	__GS_THREAD_MALL_H
#include "CSqlTHREAD.h"
//-------------------------------------------------------------------------------------------------

/**
 * \ingroup SHO_GS_LIB
 * \class	GS_CThreadMALL
 * \author	wookSang.Jo
 * \brief	���θ� ���� ����Ǵ� ������ Ŭ����
 *			��񿬰�� ó�� Ŭ������ CSqlTHREAD�� ��� ����
 */
class GS_CThreadMALL : public CSqlTHREAD
{
private:
	bool		m_bWaiting;
	CStrVAR		m_QuerySTR;

	bool Send_gsv_MALL_ITEM_REPLAY(classUSER *pUSER, BYTE btType, BYTE btData=0);

	bool Give_MallITEM ( tagQueryDATA *pSqlPACKET, BYTE btMallInvIDX, short nDupCnt );
	bool Bring_MallITEM ( tagQueryDATA *pSqlPACKET, BYTE btMallInvIDX, short nDupCnt );
	bool List_MallITEM( int iScoketIDX, char *szAccount );

	bool Run_SqlPACKET( tagQueryDATA *pSqlPACKET );
	void Execute ();

public :
	GS_CThreadMALL ();
	virtual ~GS_CThreadMALL ();

	bool Add_SqlPacketWithACCOUNT (classUSER *pUSER, t_PACKET *pPacket);
	bool Add_SqlPacketWithAVATAR  (classUSER *pUSER, t_PACKET *pPacket);
} ;
extern GS_CThreadMALL *g_pThreadMALL;

//-------------------------------------------------------------------------------------------------
#endif