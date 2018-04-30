#ifndef _CTSCROLLBARTYPE_
#define _CTSCROLLBARTYPE_
#include <crtdbg.h>
/**
* CTScrollBar�� Type�� ���� Strategy Class( ����/���� )
*		- ���� Strategy Pattern�� �����ʾƵ� ���������� ������� �����԰� ������ �ǽð����� ��ũ�ѹٰ�
*		- Ÿ���� �ٲ��쵵 �����ؼ� �����.
*
* @Author		������
*
* @Date			2005/8/30
*/
class CTScrollBarType
{
public:
	CTScrollBarType(void);
	virtual ~CTScrollBarType(void);
	///��ũ�ѹ� Ÿ��(TScrollBar Type )

	virtual POINT GetBackImageOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE BackImageSize ) = 0 { _ASSERT( 0 && "CTScrollBarType::GetPrevButtonOffset"); POINT pt; return pt;};
	virtual POINT GetPrevButtonOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE PrevButtonSize ) = 0 { _ASSERT( 0 && "CTScrollBarType::GetPrevButtonOffset"); POINT pt; return pt;};
	virtual POINT GetNextButtonOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE NextButtonSize ) = 0 { _ASSERT( 0 && "CTScrollBarType::GetNextButtonOffset"); POINT pt; return pt;};
	virtual POINT GetScrollBoxOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE PrevButtonSize ,SIZE ScrollBoxSize ) = 0 { _ASSERT( 0 && "CTScrollBarType::GetScrollBoxOffset"); POINT pt; return pt;};
	virtual RECT  GetThumbMoveRange( POINT ptScrollBarPos,POINT ptNextButtonPos,SIZE ScrollBarSize, SIZE PrevButtonSize, SIZE ScrollBoxSize ) = 0 { _ASSERT( 0 && "CTScrollBarType::GetThumbMoveRange"); RECT rc; return rc; };

	virtual int		Get_SB_Type()	{	return m_iTSBT;	}
	enum{
		TSBT_VERTICAL,
		TSBT_HORIZONTAL
	};
	int m_iTSBT;

};

class CTScrollBarTypeVertical : public CTScrollBarType
{
public:
	CTScrollBarTypeVertical(void){	m_iTSBT = TSBT_VERTICAL;	};
	virtual ~CTScrollBarTypeVertical(void){		};

	virtual POINT GetBackImageOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE BackImageSize );
	virtual POINT GetPrevButtonOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE PrevButtonSize );
	virtual POINT GetNextButtonOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE NextButtonSize );
	virtual POINT GetScrollBoxOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE PrevButtonSize ,SIZE ScrollBoxSize);
	virtual RECT  GetThumbMoveRange( POINT ptScrollBarPos,POINT ptNextButtonPos,SIZE ScrollBarSize, SIZE PrevButtonSize, SIZE ScrollBoxSize );

};

class CTScrollBarTypeHorizontal : public CTScrollBarType
{
public:
	CTScrollBarTypeHorizontal(void){	m_iTSBT = TSBT_HORIZONTAL;	};
	virtual ~CTScrollBarTypeHorizontal(void){};

	virtual POINT GetBackImageOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE BackImageSize );
	virtual POINT GetPrevButtonOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE PrevButtonSize );
	virtual POINT GetNextButtonOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE NextButtonSize );
	virtual POINT GetScrollBoxOffset( POINT ptScrollBarOffset, SIZE ScrollBarSize, SIZE PrevButtonSize ,SIZE ScrollBoxSize);
	virtual RECT  GetThumbMoveRange( POINT ptScrollBarPos,POINT ptNextButtonPos,SIZE ScrollBarSize, SIZE PrevButtonSize, SIZE ScrollBoxSize );

};

#endif