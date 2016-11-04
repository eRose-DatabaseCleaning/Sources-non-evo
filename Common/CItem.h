#ifndef	__CITEM_H
#define	__CITEM_H
#pragma warning (disable:4201)
//-------------------------------------------------------------------------------------------------
/*
��(MONEY)	������ȣ : 40	(0 ~ 999,999)
������ ����(ITEM_CLASS)		(1 ~ 20)		: 5  bit  0~31		
�ƾ��� ��ȣ(ITEM_ID)		(0 ~ 999)		: 10 bit  0~1023	

��� ��ȣ1(JAMMING1)		(0~120)			: 7  bit  0~127
��� ��ȣ2(JAMMING2)		(0~120)         : 7  bit  0~127
��� ��ȣ3(JAMMING3)		(0~120)         : 7  bit  0~127

��ȭ ���(RESMELT)			(0~9)			: 4  bit  0~15		��� �������� ��츸..
ǰ��(QUALITY)				(0~120)			: 7  bit  0~127		��� �������� ��츸..
����(QUANTITY)				(1~999)			: 10 bit  0~1023	�Ҹ�, ��Ÿ �������� ���

  ��� : 5 + 10 + 21 + 11 ==> 15+33 : 48   6 bytes
  ��Ÿ : 5 + 10 + 10      ==> 15+10
  ��   : 5 + 10 + xx
*/


#define	MAX_ITEM_LIFE			1000
#define __ITEM_MAX

#define	MAX_DUP_ITEM_QUANTITY	999
#pragma pack (push, 1)
#ifdef __ITEM_MAX

struct tagPartITEM {
	unsigned int	m_nItemNo		: 26;	// 0~1023	�ƾ��� ��ȣ(ITEM_ID)		(0 ~ 999)
	unsigned int	m_nGEM_OP		: 9;	// 0~512	������ȣ(m_bHasSocket==1) �Ǵ� �ɼ� ��ȣ(m_bHasSocket==0)
	unsigned int	m_bHasSocket	: 1;	// 0~1		���� ���� ����
	unsigned int	m_cGrade	    : 4;	// 0~15		���						(0~9)
} ;

#else

struct tagPartITEM {
	unsigned int	m_nItemNo		: 10;	// 0~1023	�ƾ��� ��ȣ(ITEM_ID)		(0 ~ 999)
	unsigned int	m_nGEM_OP		: 9;	// 0~512	������ȣ(m_bHasSocket==1) �Ǵ� �ɼ� ��ȣ(m_bHasSocket==0)
	unsigned int	m_bHasSocket	: 1;	// 0~1		���� ���� ����
	unsigned int	m_cGrade	    : 4;	// 0~15		���						(0~9)
} ;

#endif

#ifndef	__SERVER
#define	tagITEM		tagBaseITEM
#endif

int getItemNo(int iFullItemNo);
int getItemType(int iFullItemNo);
int setItemFullNo( int iItemType, int iItemNo );

// �� 64 bits, 8 bytes
struct tagBaseITEM {
#ifdef __ITEM_MAX

	union {
		// ��� ������ ����
		struct {	
			// LSB ::
			// �Ʒ� ���� �ϳ��� ��Ʈ �÷��� ��.
			unsigned int	m_cType			: 5;	// 0~31		������ ����(ITEM_CLASS)		(1 ~ 20)			
			unsigned int	m_nItemNo		: 26;	// 0~1023	�ƾ��� ��ȣ(ITEM_ID)		(0 ~ 999999)
			unsigned int	m_bCreated		: 1;	// 0~1		������ �������ΰ� ?

			unsigned int	m_nGEM_OP		: 9;	// 0~512	������ȣ(m_bHasSocket==1) �Ǵ� �ɼ� ��ȣ(m_bHasSocket==0)
			unsigned int	m_cDurability	: 7;	// 0~127	������

			unsigned int	m_nLife			: 10;	// 0~1023	����
			unsigned int	m_bHasSocket	: 1;	// 0~1		���� ���� ����
			unsigned int	m_bIsAppraisal	: 1;	// 0~1		�ɼ� ���� ����
			unsigned int	m_cGrade	    : 4;	// 0~15		���						(0~9)

			// 32 + 16 + 16 => 64
			// MSB ::
		} ;

		// �Ҹ�, ��Ÿ ������ ����
		struct {
			unsigned int	m_cType_1		: 5;	// 0~31		������ ����(ITEM_CLASS)		(1 ~ 20)
			unsigned int	m_nItemNo_1		: 26;	// 0~1023	�ƾ��� ��ȣ(ITEM_ID)		(0 ~ 999)
			unsigned int    __dummy_0       : 1;

			unsigned int	m_uiQuantity	: 32;	// ����(��)
		} ;

		// �� ������ ����
		struct {
			unsigned int	m_cType_2		: 5;	// 0~31
			unsigned int	m_nReserved1	: 26;
			unsigned int    __dummy_0       : 1;

			unsigned int	m_uiMoney		: 32;
		} ;

		struct {
			unsigned int	m_wHeader		: 32;	//������ m_dwHeader�ε� ��ĥ���� ���Ƽ� �̸��� �������� �ʾҴ�.
			unsigned int	m_dwBody		: 32;
		} ;

		struct {
			unsigned int	m_dwLSB;
			unsigned int	m_dwMSB;
		} ;		

		unsigned int	m_dwITEM;
	} ;


#ifdef __ITEM_TIME_LIMMIT

	struct {
		DWORD dwPickOutTime;
		WORD  wPeriod;
	};	

	// 1970�� 1�� 1�� 0�� 0�� 0�ʺ��� 2007�� 01�� 01�� 0�� 0�� 0�ʱ����� �ð��� �ʴ�����
	time_t tagBaseITEM::get_basicTime()
	{
		time_t basic_t;
		struct tm t;

		// 2007�� 01�� 01�� 0�� 0�� 0�� ����
		t.tm_year = 2007 - 1900; t.tm_mon = 1 - 1; t.tm_mday = 1; t.tm_hour = 0; t.tm_min = 0; t.tm_sec = 0;
		basic_t = mktime( &t);	

		return basic_t;
	}

	//����ð�
	tm tagBaseITEM::get_startTime()
	{		
		time_t long_time = (dwPickOutTime) + get_basicTime();
		struct tm l = *localtime( &long_time );
		l.tm_year += 1900;
		l.tm_mon += 1;
		return l;
	}
	
	//�Ҹ�ð�
	tm tagBaseITEM::get_endTime()
	{		
		time_t long_time = (dwPickOutTime) + get_basicTime() + (wPeriod) * 3600 ;
		
		struct tm l = *localtime( &long_time );
		//struct tm l = *gmtime( &long_time );
		
		l.tm_year += 1900;
		l.tm_mon += 1;
		if( l.tm_min )
		{
            l.tm_hour += 1;            
			l.tm_min = 0;
			l.tm_sec = 0;
		}		
		return l;
	}

#endif


#else

	union {
		// ��� ������ ����
		struct {	
			// LSB ::
			// �Ʒ� ���� �ϳ��� ��Ʈ �÷��� ��.
			unsigned short	m_cType			: 5;	// 0~31		������ ����(ITEM_CLASS)		(1 ~ 20)			
			unsigned short	m_nItemNo		: 10;	// 0~1023	�ƾ��� ��ȣ(ITEM_ID)		(0 ~ 999)
			unsigned short	m_bCreated		: 1;	// 0~1		������ �������ΰ� ?

			unsigned int	m_nGEM_OP		: 9;	// 0~512	������ȣ(m_bHasSocket==1) �Ǵ� �ɼ� ��ȣ(m_bHasSocket==0)
			unsigned int	m_cDurability	: 7;	// 0~127	������

			unsigned int	m_nLife			: 10;	// 0~1023	����
			unsigned int	m_bHasSocket	: 1;	// 0~1		���� ���� ����
			unsigned int	m_bIsAppraisal	: 1;	// 0~1		�ɼ� ���� ����
			unsigned int	m_cGrade	    : 4;	// 0~15		���						(0~9)

			// 16 + 16 + 16 => 48
			// MSB ::
		} ;

		// �Ҹ�, ��Ÿ ������ ����
		struct {
			unsigned short	m_cType_1		: 5;	// 0~31		������ ����(ITEM_CLASS)		(1 ~ 20)
			unsigned short	m_nItemNo_1		: 10;	// 0~1023	�ƾ��� ��ȣ(ITEM_ID)		(0 ~ 999)

			unsigned int	m_uiQuantity	: 32;	// ����(��)
		} ;

		// �� ������ ����
		struct {
			unsigned short	m_cType_2		: 5;	// 0~31
			unsigned short	m_nReserved1	: 11;

			unsigned int	m_uiMoney		: 32;
		} ;

		struct {
			unsigned short	m_wHeader		: 16;
			unsigned int	m_dwBody		: 32;
		} ;

		struct {
			DWORD	m_dwLSB;
			WORD	m_wMSB;
		} ;

		struct {
			WORD	m_wLSB;
			DWORD	m_dwMSB;
		} ;

		DWORD	m_dwITEM;
	} ;
#endif

	void		   Init(int iItem, unsigned int uiQuantity=1);

#ifdef __ITEM_MAX
	void			Clear ()				
	{
		m_dwLSB=m_dwMSB=0;
#ifdef __ITEM_TIME_LIMMIT
		dwPickOutTime = wPeriod = 0;
#endif
	}
	unsigned int	GetItemNO ()			{	return	m_nItemNo;				}
	bool			IsEmpty ()				{	return (0==m_wHeader);			}
	unsigned int	GetHEADER ()			{	return (m_wHeader & 0x7fffffff);	}	// m_bCreated :: ��� �񱳽� ������Ʈ ����...
#else
	void			Clear ()				{	m_dwLSB=m_wMSB=0;				}
	unsigned short	GetItemNO ()			{	return	m_nItemNo;				}
	bool			IsEmpty ()				{	return (0==m_wHeader);			}
	unsigned short	GetHEADER ()			{	return (m_wHeader & 0x7fff);	}	// m_bCreated :: ��� �񱳽� ������Ʈ ����...

#endif

	unsigned short GetTYPE ()			{	return	m_cType;				}

	unsigned short GetGrade ()			{	return	m_cGrade;		}
	unsigned short GetOption ()			{	return  m_nGEM_OP;		}
	unsigned short GetGemNO ()			{	return  m_nGEM_OP;		}

	unsigned short GetLife()			{	return  m_nLife;		}
	unsigned short GetDurability()		{	return  m_cDurability;	}
	unsigned int   GetMoney ()			{	return	m_uiMoney;		}


	bool IsAppraisal()					{	return (0!=m_bIsAppraisal);		}
	bool HasSocket()					{	return (0!=m_bHasSocket);		}

	bool IsEnableDROP ();					// �����Ⱑ ������ �������ΰ� ?
	bool IsEnableSELL ();					// �ȱⰡ ������ �������ΰ� ?
	bool IsEnableKEEPING ();				// ���࿡ ���� ������ �������ΰ� ?

#ifdef __SERVER
	static bool IsValidITEM (DWORD wType, DWORD wItemNO);
	static bool IsValidITEM (tagBaseITEM *pItem)		{	return tagBaseITEM::IsValidITEM( pItem->GetTYPE(), pItem->GetItemNO() );	}
	bool IsValidITEM ()					{	return IsValidITEM( this->GetTYPE(), this->GetItemNO() );	}
#else
	bool IsValidITEM ();
	bool IsValidITEM (DWORD wType, DWORD wItemNO);
#endif

	static bool IsEnableDupCNT( unsigned short cType )		
	{	
		// �ߺ� �������� �������̳�???
		return (cType>=ITEM_TYPE_USE && cType<ITEM_TYPE_RIDE_PART);
	}
	bool IsEnableDupCNT()				{	return IsEnableDupCNT(m_cType);	}
	bool IsCreated()					{	return (1==m_bCreated);			}
	bool IsEquipITEM()					{	return ( m_cType && m_cType < ITEM_TYPE_USE );					}	// ���� �������ΰ�?
	bool IsEtcITEM()					{	return ( m_cType>ITEM_TYPE_USE && m_cType<ITEM_TYPE_QUEST);		}

	bool IsTwoHands ();

	t_eSHOT			GetShotTYPE();
	t_EquipINDEX	GetEquipPOS();
#ifdef	__SERVER
	unsigned int	GetQuantity ()		{	return	m_uiQuantity;			}
#else
	unsigned int	GetQuantity ();
	short			Subtract( tagITEM &sITEM );		// �־��� ������ ��ŭ ���� ���� ��������� sITEM�� ��� �ִ�.
	void			SubtractOnly (tagITEM &sITEM);	// �־��� ������ ��ŭ ���� ����.

	bool			IsEnableAppraisal ();					///���������� �������ΰ�?
	bool			IsEnableExchange ();					// �����Ⱑ ������ �������ΰ� ?
	bool			IsEnableSeparate ();
	bool			IsEnableUpgrade ();
	bool			HasLife();
	bool			HasDurability();
	unsigned short	GetModelINDEX ();
	char*			GettingMESSAGE (int iInventoryListNo);
	char*			GettingMESSAGE ();
	char*			GettingMESSAGE_Party (const char * partyName_);
	char*			GettingQuestMESSAGE();
	char*			SubtractQuestMESSAGE();
	///�Ҹ�ź�������� ShotType�� ���
	static t_eSHOT GetNaturalBulletType( int iItemNo );
	t_eSHOT GetBulletType();
	///���߷�
	int				GetHitRate();
	int				GetAvoidRate();
	bool			IsEqual( int iType, int iItemNo );
	int				GetUpgradeCost();
	int				GetSeparateCost();
	int				GetAppraisalCost();
	const	char*	GetName();
#endif
} ;


//-------------------------------------------------------------------------------------------------
#ifdef	__SERVER
struct tagITEM : public tagBaseITEM {
	union {
		DWORD	m_dwSN[2];
		__int64	m_iSN;
	} ;

	// ���� �����ۿ��� �־��� ������ ��ŭ ����, ���� ���Ը� �����Ѵ�.
	short	Subtract( tagITEM &sITEM );		// �־��� ������ ��ŭ ���� ���� ��������� sITEM�� ��� �ִ�.
	void	SubtractOnly (tagITEM &sITEM);	// �־��� ������ ��ŭ ���� ����.

	bool SubQuantity ()		{	
		if ( GetQuantity() > 0 ) {
			--m_uiQuantity;
			return true; 
		}
		return false;
	}
	bool SubQuantity(unsigned int uiCnt)
	{
		if ( GetQuantity() > uiCnt ) {
			m_uiQuantity -= uiCnt;
			return true;
		} else
		if ( m_uiQuantity == uiCnt ) {
			this->Clear ();
			return true;
		}
		return false;
	}

	void 	Init(int iItem, unsigned int uiQuantity=1)
	{
		tagBaseITEM::Init(iItem, uiQuantity);
		m_iSN = 0;
	}
	void	Clear ()
	{	
		tagBaseITEM::Clear();
		m_iSN = 0;
	}

#ifndef	__BORLANDC__
#ifdef __ITEM_MAX
	void operator=(tagBaseITEM &rBASE)
	{
		m_dwLSB  = rBASE.m_dwLSB;
		m_dwMSB = rBASE.m_dwMSB;
		m_iSN   = 0;
	}
#else
	void operator=(tagBaseITEM &rBASE)
	{
		m_wLSB  = rBASE.m_wLSB;
		m_dwMSB = rBASE.m_dwMSB;
		m_iSN   = 0;
	}
#endif
#endif
} ;
#endif
#pragma pack (pop)

//-------------------------------------------------------------------------------------------------
#pragma warning (default:4201)
#endif