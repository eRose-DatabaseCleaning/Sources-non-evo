#ifndef _CMINIMAPDLG_
#define _CMINIMAPDLG_


#include "TDialog.h"
#include "singlelinestring.h"
class CTAnimation;

/**
* �̴ϸ� ǥ�ø� ���� ���̾�α�
*
* @Author		������
* @Date			2005/9/12
*/
class CMinimapDLG : public CTDialog
{


private:

	/// �̴ϸ��� ���� ���° �ʺ��� �����ϴ°�? �׷��߸� ���� �����ġ�� �����Ҽ� �ִ�..
	DWORD				m_dwMinimapStartX;			/// �̴ϸ��� LEFT-TOP�� ��ġ��ų ���� ��ȣ
	DWORD				m_dwMinimapStartY;

	float				m_fMinMinimapWorldPosX;		/// �̴ϸ��� ������ǥ��  ���� ��ǥ( ���� )
	float				m_fMaxMinimapWorldPosY;		/// �̴ϸ��� ������ǥ��  ���� ��ǥ( ���� )
	
	float				m_fMaxMinimapWorldPosX;		/// �̴ϸ� Texture�� Size�� �̵������� �ִ� ���� X��ǥ�� ���� ��( ���� �̵����� ���� ��ǥ�� Ʋ���� �ִ�:�̴ϸ��� �߸� �� ���)
	float				m_fMinMinimapWorldPosY;		/// �̴ϸ� Texture�� Size�� �̵������� �ּ� ���� Y��ǥ�� ���� ��

	float				m_fCurrentScale;			/// Zoom�ÿ� Ȯ���� ����

	float				m_fMinimapCenterWorldPosX;	/// ���� �������� �̴ϸ��� �߰��� ���� ���� ��ǥ
	float				m_fMinimapCenterWorldPosY;			

	bool				m_bExploreMode;				/// ���콺 �巡�׿� ���� �̴ϸ��� �߽��� ����Ǵ� Mode(Draw���� false���� üũ)
	bool				m_bShowMiniMap;				/// Expand( ���� �̴ϸ��� �����ִ°�? Dialog�� �׻� �������� �����̴� )

	HNODE				m_hMiniMap;
	HNODE				m_hArrow;

	float				m_fPrevUserPosX;			/// Explore Mode�� �����ϱ� ���Ͽ� �ӽ������� �� �ƹ�Ÿ�� ��ġ�� �����صδ� ����
	float				m_fPrevUserPosY;
	POINT				m_ptLButtonDown;			/// LButtonDown�� ���콺 ��ġ
	bool				m_bDragStart;				/// �̴ϸ������� �巡�װ� ���۵Ǿ��°�?




	int					m_iAvatarImageID;
	int					m_iPartyImageID;


	struct  PartyMemberMarkInfo{
		RECT rcDraw;
		int	 iIconNo;
	};
	struct	NpcMarkInfo{
		RECT		rcDraw;
		std::string	strName;
		int			iIconNo;
	};

	struct S_IndicatorCoordinates{
		int		m_zoneno;
		float	m_x;
		float	m_y;
	};

	struct S_IndicatorNpc{
		int	m_iNpcNo;
		int m_auto_remove;
	};



	/**�̴ϸʿ��� npc&PartyMember��ũ�� �׸������Ͽ� �ӽ÷� �����صδ� ����Ʈ
	*	ObjMGR���� ������ ����Ʈ���� �ϴ� �ƹ�Ÿ���� �׸��� NPC���� �ӽ� �����صξ��ٰ�
	*	������ �׸��� ���Ͽ� ��� �����صд�.
	*	std::map<int, NpcMarkInfo>			
	*		-m_listNpcMarkInfo; - �ߺ��� NPC ��ȣ�� ���� NPC�� ��� �ȱ׷����� ��Ȳ �߻�
	*		-��� NPC�� �ʿ��� �ε��ؼ� �׸��°����� �Ѵ�.
	*/
	std::list<PartyMemberMarkInfo>		m_listPartyMemberMarkInfo;


	bool	m_bExpand;															///���� ũ�� ���̴°�?
	bool	m_bMinimize;														///���� ĸ�Ǹ� ���̴°�?

	CSinglelineString	m_zonename;												/// ������ ��Ʈ��

	/// ��ũ��Ʈ�� �̴ϸ����� Ư���� ǥ�ø� �߰��� ���� �����̳�
	std::multimap< int ,S_IndicatorNpc >			m_indicators_npc;			/// NPC( �ߺ� �����Ҽ� �ִ� )
	std::map< int ,S_IndicatorCoordinates >			m_indicators_coordinates;	/// ��ǥ

	CTAnimation*	m_indicator;												///ǥ�ÿ� ���� CTAnimation
public:
	CMinimapDLG();
	virtual ~CMinimapDLG();

	virtual unsigned int	Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);
	virtual void			Draw();
	virtual void			Update( POINT ptMouse );
	virtual void			Show();



	bool	SetMinimap( const char* pFname, const char* pCursorName, DWORD dwMapStartX, DWORD dwMapStartY );
	void	FreeMinimap();
	
	void	AddIndicatorNpc( int npcno , int auto_remove );
	void	AddIndicatorCoordinates( int index, int zoneno, float x, float y );
	void	RemoveIndicatorNpc( int npcno );
	void	RemoveIndicatorCoordinates( int index );
	int		GetIndicatorNpc( int npcno );
	void	RemoveAutodeleteIndicatorNpc( int npcno );
	void	CalculateDisplayPos();												///���ӿ��� ���Ǵ� ���� ������ġ�� �̴ϸ��� ������ġ�� �����.

	void	ToggleShowMinimap();
	void	ToggleZoomMinimap();

	bool	IsShowMinimap(){ return m_bShowMiniMap; }
	bool	IsZoomMinimap(){ return((m_fCurrentScale == 1)? false: true); }

	void	SetAvataInfoPos(int y);													//  �ƹ�Ÿ����â Ȯ��,��ҽ� ��ġ ����

	void	RefreshDlg();
	void	SetInterfacePos_After();

	enum{
		IID_PANE_BIG				= 50,
		IID_CAPTION_BIG				= 51,
		IID_BTN_NORMAL				= 52,
		IID_BTN_MINIMIZE_BIG		= 53,
		IID_PANE_BIG_CHILDPANE		= 60,

		IID_PANE_SMALL				= 100,
		IID_CAPTION_SMALL			= 101,
		IID_BTN_EXPAND				= 102,
		IID_BTN_MINIMIZE_SMALL		= 103,
		IID_PANE_SMALL_CHILDPANE	= 110
	};

protected:
	bool	On_LButtonUP( unsigned iProcID, WPARAM wParam, LPARAM lParam );
	bool	On_LButtonDN( unsigned iProcID, WPARAM wParam, LPARAM lParam );

	
	float	fGetWorldDistancePerPixel();										/// �̴ϸʰ� ���� �Ÿ����� ����

	void	DrawAVTPosition();													/// �ٸ� �ƹ�Ÿ,NPC���� ǥ��
	bool	IsInsideMinimap( float x, float y , int width, int height );		/// �̴ϸ�Dialog������ ���� �̴ϸ����ΰ�?

	void	DrawPartyMember();													/// ��Ƽ ��� ���
	void	DrawNpcFromMap();													/// �ʿ� ������ NPC�� ���( �������� ���� CObjNPC�� �ƴ� �ʿ��� ���� �ε��� )

	void	DrawCoordinatesIndicator();											/// Ư���� ǥ�ð� �߰��� ��ǥ Draw

	void	Expand();															/// ��ü ���̱�
	void	Reduct();															/// ĸ�Ǹ� ���̱�

	void	ToggleShowMinimapSmall();											/// �ּ�ȭ
	void	ToggleShowMinimapBig();												/// �ִ�ȭ

	

	CWinCtrl* FindPaneChild( unsigned uiPaneID, unsigned uiChildID );


};



#endif //_CMINIMAPDLG_