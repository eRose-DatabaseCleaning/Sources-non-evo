#ifndef _CTSCROLLBOXTYPE_
#define _CTSCROLLBOXTYPE_
#include <crtdbg.h>

/**
* CTScrollBar�� Type�� ���� Strategy Class( ����/���� )
*
* @Author	������
* @Date		2005/8/30
*/

class CTScrollBoxType
{
public:
	CTScrollBoxType(void);
	~CTScrollBoxType(void);
	virtual POINT GetMoveBoxPos( RECT MoveRange, int iValue, int iMax ,int iExtent ) = 0 { _ASSERT( 0 && "CTScrollBoxType::GetMoveBoxPos"); POINT pt; return pt; };
	virtual POINT GetMoveBoxPosByScreenPoint( SIZE sizeScrollBox, RECT MoveRange, POINT ptMove, int iMax ,int iExtent ,int& Value ) = 0 { _ASSERT( 0 && "CTScrollBoxType::GetMoveBoxPosByScreenPoint"); POINT pt; return pt;};

	void	SetOffset( POINT pt );
	void	SetTickMove( bool bTickMove );
protected:
	
	POINT	m_ptOffset;			///���콺�� LButtonDn�� ���콺�����Ϳ� ��ũ�ѹڽ��� Left-Top���� �Ÿ� ����
	bool	m_bTickMove;		///��ũ�ѹڽ� �̵��� ���� ����� Tick���� �̵��Ұ��ΰ�?
};

class CTScrollBoxTypeVertical : public CTScrollBoxType
{
public:
	CTScrollBoxTypeVertical(void){}
	~CTScrollBoxTypeVertical(void){}
	virtual POINT GetMoveBoxPos( RECT MoveRange, int iValue, int iMax ,int iExtent );
	virtual POINT GetMoveBoxPosByScreenPoint( SIZE sizeScrollBox, RECT MoveRange, POINT ptMove,int iMax ,int iExtent ,int& Value );

};

class CTScrollBoxTypeHorizontal : public CTScrollBoxType
{
public:
	CTScrollBoxTypeHorizontal(void){}
	~CTScrollBoxTypeHorizontal(void){}
	virtual POINT GetMoveBoxPos( RECT MoveRange, int iValue, int iMax ,int iExtent );
	virtual POINT GetMoveBoxPosByScreenPoint( SIZE sizeScrollBox,RECT MoveRange, POINT ptMove,int iMax ,int iExtent ,int& Value );

};

#endif