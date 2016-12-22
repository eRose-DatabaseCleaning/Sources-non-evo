#ifndef	__CHOTICON_H
#define	__CHOTICON_H
#pragma warning (disable:4201)
//-------------------------------------------------------------------------------------------------

/// �ִ� ���� ������ ��� ���� ����
#ifdef _NEW_UI

#define	MAX_ICONS_PAGES			8	// Numenor :normal 4 + ext 4. /!\ Warning: you also have to change this variable in the server!
									// Numenor : look here SHO_GS_LIB\Common\CHotICON.h
#define	MAX_ICONS_PAGES_NORMAL	4
#define	MAX_ICONS_PAGES_EXT		4

#define	HOT_ICONS_PER_PAGE		8

#define	MAX_HOT_ICONS			64

#else

#define	MAX_ICONS_PAGES			4
#define	MAX_ICONS_PAGES_NORMAL	4

#define	HOT_ICONS_PER_PAGE		8

#define	MAX_HOT_ICONS			32

#endif


enum t_HotIconTYPE 
{
	INV_ICON = 1,
	COMMAND_ICON,
	SKILL_ICON,
	EMOTION_ICON,
	DIALOG_ICON,
	CLANSKILL_ICON,
} ;


#pragma pack (push, 1)
union tagHotICON {
	struct {
		unsigned short	m_cType			: 5;	// 0~31
		unsigned short	m_nSlotNo		: 11;	// 0~2047
	} ;
	WORD	m_wHotICON;
} ;

class CHotICONS {
public :
	union {
		tagHotICON	m_IconLIST[ MAX_HOT_ICONS ];
		tagHotICON	m_IconPAGE[ MAX_ICONS_PAGES ][ HOT_ICONS_PER_PAGE ];
	} ;
	void Init ();
	bool RegHotICON (BYTE btListIDX, tagHotICON sHotICON);
	void DelHotICON (BYTE btListIDX);

#ifndef	__SERVER
	void UpdateHotICON();
#endif
} ;
#pragma pack (pop)

//-------------------------------------------------------------------------------------------------
#pragma warning (default:4201)
#endif
