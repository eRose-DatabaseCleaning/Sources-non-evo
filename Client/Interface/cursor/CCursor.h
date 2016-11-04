#ifndef _CCursor_
#define _CCursor_
#include "../CInfo.h"
class CCursorState;
class CInfo;



/**
* ���ӳ� Ŀ�� Class
* - �׸��� ��Ŀ� ���� 3���� ���·� ���еǾ� ����.
*
* @Author		������
*
* @Date			2005/9/6
*/
class CCursor
{
	CCursor(void);
	~CCursor(void);

public:
	static CCursor& GetInstance();
	void Initialize();
	void Finalize();

	void	Draw( POINT ptMouse );
	void	ChangeNextState();			/// ���� ���·� �ٲ۴�.
	int		SetCursorType( int iType, CInfo* pMouseInfo = NULL );
	int		GetCurrCursorType();		/// ���� Ŀ�� State�� �����Ѵ�.
	int		RefreshCursor();	
	void	ReloadCursor();				/// 

	CInfo&	GetMouseInfo();

	enum{
		STATE_DX,						/// DirectX Cursor	-> ������ Ŀ���� �����Ͽ� ����Ѵ�( ����Ʈ )
		STATE_SPRITE,					/// Sprite Cursor(�������ӱ׸���)
		STATE_WINDOW,					/// Window Default Cursor
		STATE_MAX
	};

	enum eCURSOR_TYPE
	{
		CURSOR_DEFAULT = 0,
		CURSOR_ATTACK,
		CURSOR_ITEM_INVEN,
		CURSOR_ITEM_PICK,
		CURSOR_LEFT,
		CURSOR_RIGHT,
		CURSOR_NPC,
		CURSOR_USER,
		CURSOR_WHEEL,
		CURSOR_NOUI,
		CURSOR_REPAIR,
		CURSOR_APPRAISAL,
		CURSOR_MAX,
	};

private:
	CCursorState*	m_pCurrState;
	CCursorState*	m_pCursorState[ STATE_MAX ];
	CInfo			m_MouseInfo;
	int				m_iType;
};
#endif