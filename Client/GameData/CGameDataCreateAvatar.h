#ifndef _CGAMEDATACREATEAVATAR_
#define _CGAMEDATACREATEAVATAR_
#include <map>
#include <vector>
#include "../../TGameCtrl/TContainerItem.h"

class CJustModelAVT;

/**
* ĳ���� �����ϴ� �κп��� ����ϴ� Data Class + �ణ�� ���� ��� �߰�
*	- CJustModelAVT list�� Interface�� Data���� �����Ѵ�.
*
* @Author		������
* @Date			2005/9/15
*/
class CGameDataCreateAvatar
{
	CGameDataCreateAvatar(void);

public:
	static CGameDataCreateAvatar& GetInstance();
	~CGameDataCreateAvatar(void);
	void Clear();

	///�𵨰���
	void	AddCreateAvatar( CJustModelAVT* pAvatar );
	void	RemoveCreateAvatar( const char* pszName );

	HNODE	GetModelNode( const char* pszName );

	CJustModelAVT*			FindAvatar( const char* pszName );
	const char*				Pick_AVATAR( long x, long y );
	
	const char*				GetSelectedAvatarName();
	int						GetSelectedAvatarIndex();
	CJustModelAVT*			GetSelectedAvatar();
	void					ClearSelectedAvatar();
	void					SelectAvatar( const char* pszName );
	void					SelectAvatar( int iIndex );
	void					Update();
	int						GetAvatarCount();
private:
	std::string m_strSelectedAvatarName;///���õǾ��� �ִ� �ƹ�Ÿ�� �̸�

	std::multimap< int , CJustModelAVT* > m_avatars_premium;
	std::multimap< int , CJustModelAVT* > m_avatars_platinum;

};
#endif