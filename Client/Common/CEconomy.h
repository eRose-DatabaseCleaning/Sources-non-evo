#ifndef	__CECONOMY_H
#define	__CECONOMY_H
#include "CItem.h"
//-------------------------------------------------------------------------------------------------
struct tagECONOMY {
	// �Է� ������...
	union {
		struct {
#if	defined( __SERVER ) || defined( __VIRTUAL_SERVER )
			DWORD	m_dwTown_COUNTER;					// ī���� 1�п� 1�� ����.		50~100

			short	m_nTown_POP_BASE;					// ���� �α���.					100~8000
			short	m_nTown_DEV_BASE;					// ���� ������					10~100
			short	m_nTown_CONSUM[ MAX_PRICE_TYPE ];	// �����ۺ� �Һ�

			short	m_nTownDEV;							// ���� ������
			int		m_iTownPOP;							// ���� �α�.
			int		m_iTownITEM[ MAX_PRICE_TYPE ];		// �����ۺ� ������				1000~100000

			DWORD	m_dwCheckTIME;
			union {
				struct {
					BYTE	m_btSave_TOWN_RATE;
					BYTE	m_btSave_ItemRATE[ MAX_PRICE_TYPE ];
					short	m_nSave_WorldPROD;
					short	m_nSave_WorldRATE;
				} ;
				BYTE	m_btSave_DATA[ MAX_PRICE_TYPE+sizeof(BYTE)+sizeof(short)*2 ];
			} ;
#endif
			DWORD	m_dwUpdateTIME;
			union {
				struct {
					BYTE	m_btTOWN_RATE;						// ���� ����					80~140
					BYTE	m_btItemRATE[ MAX_PRICE_TYPE ];		// �����ۺ� ����				1~127
					short	m_nCur_WorldPROD;
					short	m_nCur_WorldRATE;
				} ;
				BYTE	m_btCur_DATA[ MAX_PRICE_TYPE+sizeof(BYTE)+sizeof(short)*2 ];
			} ;
		} ;

		BYTE	m_pEconomy[ 1 ];
	} ;
} ;

class CEconomy : public tagECONOMY
{
private:
	int m_iTownCounter;
public :
	// ���� ����...
	static bool IsEssentialGoods (int iItemTYPE);

	CEconomy ();
	~CEconomy ();

	void	Init (void);

	void	SetTownRATE (int iValue);
	void	SetItemRATE(int iPriceType, int iValue);

	BYTE	Get_TownRATE (void)				{ return m_btTOWN_RATE; }
	int		Get_ItemRATE (short nItemTYPE)	{ return nItemTYPE < MAX_PRICE_TYPE ? m_btItemRATE[ nItemTYPE ] : -1; }

	int		Get_ItemBuyPRICE  (short nItemTYPE, short nItemNO, short nBuySkillVALUE);
	int		Get_ItemSellPRICE (tagITEM &sITEM, short nSellSkillVALUE);

	int		Get_EconomyVAR(short nVarIDX)
	{
		switch( nVarIDX ) {
			case 1 :	return Get_TownRATE();
#if	defined( __SERVER ) || defined( __VIRTUAL_SERVER )
			case 2 :	return m_iTownPOP;
			case 3 :	return m_nTownDEV;
#endif
		} 

#if	defined( __SERVER ) || defined( __VIRTUAL_SERVER )
		if ( nVarIDX >= 11 && nVarIDX <= 30 ) {
			if ( nVarIDX <= 20 ) {
				// ������
				return m_iTownITEM [ MIN_PRICE_TYPE + nVarIDX - 11 ];
			} else {
				// ����..
				return m_btItemRATE[ MIN_PRICE_TYPE + nVarIDX - 21 ];
			}
		}
#endif
		return 0;
	}

#if	defined( __SERVER ) || defined( __VIRTUAL_SERVER )
	#ifdef	__SERVER
	bool	Load (FILE *fp);
	#else
	bool	Load ( CFileSystem* pFileSystem );
	#endif

	void	BuyITEM  (tagITEM &sITEM);
	void	SellITEM (tagITEM &sITEM, int iQuantity);

	int		Get_TownITEM (short nItemTYPE)	{ return nItemTYPE < MAX_PRICE_TYPE ? m_iTownITEM[ nItemTYPE ] : -1; }

	bool	Proc (DWORD dwCurTIME);
#endif
} ;

extern short Get_WorldRATE ();
extern void  Set_WorldRATE (short nWorldRate);

extern short Get_WorldPROD ();					// ������ ���Ǵ� WORLD_PRODUCT
extern void  Set_WorldPROD (short nWorldProd);

//-------------------------------------------------------------------------------------------------
#endif
