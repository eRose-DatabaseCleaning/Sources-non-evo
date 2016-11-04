#ifndef _ITDRAW_
#define _ITDRAW_

/**
* Client�� ��ӹ޾� �����ؾ��� Draw Image Interface - TGameCtrl�� �� �������̽��� ���ؼ� Image�� Draw�Ѵ�.
*
* @Warning	Client::CTDrawImpl - ���� Ŭ���̾�Ʈ���� ������ Ŭ����
*
* @Author		2005/08/23 ������
*/
class ITDraw
{

public:
	virtual void Draw( int iX, int iY, int iModuleID, int iGraphicID ) = 0 ;
	virtual void Draw( int iX, int iY, int iWidth, int iModuleID, int iGraphicID ) = 0 /*{ _ASSERT( 0 && "ITDraw::Draw2");}*/;
	virtual void Draw( float iX, float iY, float iZ, int iModuleID, int iGraphicID ) = 0 /*{ _ASSERT( 0 && "ITDraw::Draw3");}*/;

	virtual void Draw( int iX, int iY, int iModuleID, int iGraphicID, float fScaleWidth , float fScaleHeight , D3DCOLOR color ) = 0;
	
	virtual void Draw( int iX, int iY, int iModuleID, int iGraphicID, int iWidth, float fScaleWidth , float fScaleHeight , D3DCOLOR color ) = 0;
	
	virtual void Draw( int iX, int iY, int iModuleID, int iGraphicID, D3DCOLOR color ) = 0;
	virtual void Draw( int iX, int iY, int iWidth, int iModuleID, int iGraphicID, D3DCOLOR color ) = 0;

	virtual void DrawFitW( int iX, int iY, int iModuleID, int iGraphicID, int iWidth, D3DCOLOR color ) = 0;
	virtual void DrawFitH( int iX, int iY, int iModuleID, int iGraphicID, int iHeight , D3DCOLOR color ) = 0;
	virtual void DrawFit( int iX, int iY, int iModuleID, int iGraphicID, int iWidth , int iHeight , D3DCOLOR color ) = 0;

};
#endif /// _ITDRAW_
