#ifndef _UIMEDIATOR_
#define _UIMEDIATOR_

#include "CChatBox.h"
#include "CNameBox.h"
#include "CDigitEffect.h"
#include "PersonalStoreManager.h"

#include "..\\CObjCHAR.h"

#include <vector>
#include <string>


/**
* �ƹ�Ÿ�� ����(Z��)���� �׷����� �������̽����� ǥ���ϴ� Class
* 
* @Author		�̵���,������
*
* @Date			2005/9/5
*/
class CUIMediator
{
private:
	CChatBoxManager					m_ChatBoxMgr;				/// �ƹ�Ÿ �Ӹ��� ä�ñ� ǥ�� �ڽ�
	
	CNameBox						m_NameBox;					/// �ƹ�Ÿ �̸��� ������, HP��������..

	CDigitEffect					m_DigitEffect;				/// ������, ũ��Ƽ��, �̽��� ����Ʈ
	
	CPersonalStoreManager			m_PersonalStoreManager;		/// ���λ��� ���� ������

	HNODE							m_LogoTex;					/// ���ӷΰ�
	//std::string					m_strZoneName;

public:
	CUIMediator();
	~CUIMediator();


	bool	InitMediator();	
	void	FreeResource();

	void	Update();
	void    Draw();
	void	DrawAvataName( CObjCHAR* pCharOBJ );


	void	AddChatMsg( int iCharIndex, const char* pMsg, DWORD Color = D3DCOLOR_ARGB( 255, 0, 0, 0) );
	void	DrawChatBox(){ m_ChatBoxMgr.Draw(); };	
	void	CreateDamageDigit( int iPoint, float x, float y, float z, bool bIsUSER );


	//void	SetZoneName( const char* pszName );
	//void	DrawZoneName();


	//----------------------------------------------------------------------------------------------------	
	/// ���λ��� ����
	//----------------------------------------------------------------------------------------------------
	void	AddPersonalStoreIndex( int iObjIDX );
	void	SubPersonalStoreIndex( int iObjIDX );
	void	ResetPersonalStore();


protected:

	int		m_iLogoWidth;
	int		m_iLogoHeight;
};

extern CUIMediator	g_UIMed;

#endif //_UIMEDIATOR_