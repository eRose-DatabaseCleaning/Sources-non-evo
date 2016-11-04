/*
	$Header: /Client/Event/CEvent.h 22    05-01-07 2:20p Jeddli $
*/
#ifndef	__CEVENT_H
#define	__CEVENT_H
#include "..\util\classLUA.h"
//-------------------------------------------------------------------------------------------------

struct tagSCRIPTMSG {
	CStrVAR	m_CheckFunc;
	CStrVAR	m_ClickFunc;
} ;

//-------------------------------------------------------------------------------------------------
/// �ϳ��� ��� ���� Ÿ��, �ڽļ�Ʈ �ε���, üũ ���, Ŭ�� ���, �޼����� ����
/// 	m_iType					: ���� ��� ����� Ÿ��
/// 	m_lChildDataIDX			: �ڽ� ��Ʈ�� �ε���
/// 	CStrVAR	m_CheckFunc		: Ʈ���� �����ɶ� ����Ǵ� üũ ���
/// 	CStrVAR	m_ClickFunc		: �ش� ��簡 Ŭ���Ǿ����� ����Ǿ���� ���
/// 	CStrVAR m_Message		: ������ �������� �޼���
//-------------------------------------------------------------------------------------------------
struct tagSCRIPTITEM {
	int		m_iType;
	long	m_lChildDataIDX;

	CStrVAR	m_CheckFunc;
	CStrVAR	m_ClickFunc;

	CStrVAR m_Message;
} ;


//-------------------------------------------------------------------------------------------------
/// �ϳ��� Ʈ�� �θ� ��带 ������ �ǹ������� ����Ʈ
/// !!! Ư�� Ʈ������� �ڽĵ��� �ٷ� �����Ǵ°��� �ƴ϶� ��� Ʈ���� ������ �ڽ��� ������ �Բ� ��Ʈ�� ����
/// �θ� ���� �ܼ��� ���ϵ��弼Ʈ�� �ε����� �����ϰ� �ִ�.
///
///   ||A||    ||B||        ||C||    
/// 
///  ���� �׸����� A, B, C �� ������ ����Ʈ�� ������ ��Ʈ
///  A ����Ʈ�� � �� ��尡 B, C ��Ʈ�� �ε����� ������ �׳���� �ش����� �ڽ��� �Ǵ°��̴�.
///
//-------------------------------------------------------------------------------------------------
struct tagSCRIPTDATA {
	short			m_iScrItemCNT;
	tagSCRIPTITEM  *m_pScrITEM;

	tagSCRIPTDATA ()	{	m_pScrITEM = NULL;					}
	~tagSCRIPTDATA ()	{	SAFE_DELETE_ARRAY( m_pScrITEM );	}
} ;

//-------------------------------------------------------------------------------------------------
class CEvent;
struct tagEventITEM {
	CEvent			*m_pEVENT;
	tagSCRIPTITEM	*m_pScrITEM;
} ;

#define	MAX_CONV_STR_LEN	2048

class CEvent {
private:
	static char	    m_TempBuffer[ MAX_CONV_STR_LEN ];
	static char		m_MBCSBuffer[ MAX_CONV_STR_LEN ];

	int				m_iScrMsgCNT;
	tagSCRIPTMSG   *m_pScrMSG;

	int				m_iScrDataCNT;
	tagSCRIPTDATA  *m_pScrDATA;

	int				m_iLuaDataLEN;
	char		   *m_pLuaDATA;

	t_HASHKEY		m_HashKEY;
	classLUA	   *m_pLUA;
	classDLLIST< tagEventITEM >	m_EventITEM;	

	CStrVAR			m_EventFUNC[ 16 ];



	//-------------------------------------------------------------------------------------------------
	/// �����ۿ� ���� ������ �̺�Ʈ �ΰ�?
	/// 2005/1/6 ������ Dlg Ÿ������ ����
	//-------------------------------------------------------------------------------------------------
	///bool			m_bItemEvent;
	int				m_iEventDlgType;


	
	char *ParseMESSAGE (char *szMessage);
	classDLLNODE< tagEventITEM >* Add_ClickITEM (tagSCRIPTITEM *pScrITEM);
	void Del_ClickITEMS (void);
	bool End ();
	int  GetQueryType();

public :
	CEvent ( int iEventDlgType = 0 );
	~CEvent ();

	void			SetEventDlgType( int iEventDlgType ){ m_iEventDlgType = iEventDlgType; }

	int				m_iOwnerObjIDX;

	bool			Load (char *szFileName);
	bool			Start (short nEventIDX);
	short			Conversation (int iMenuIDX);

	static void		Click_ITEM( int iHandle );
	static void		Click_CLOSE( int iHandle );

	void			Decode( char *pData, int iSize, int iValue1, int iValue2 );
} ;

//-------------------------------------------------------------------------------------------------
#endif
