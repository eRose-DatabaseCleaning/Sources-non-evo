#ifndef	_RESOURCEMGR_
#define _RESOURCEMGR_

#include "TGameCtrl.h"
#include <map>
#include <string>
#include <list>
using namespace std;

typedef		map< string, int >			TABLE_IMAGE_ID;
typedef		TABLE_IMAGE_ID::iterator	TABLE_IMAGE_ID_ITER;

typedef		map< int,TABLE_IMAGE_ID >	TABLE_MODULE;
typedef		TABLE_MODULE::iterator		TABLE_MODULE_ITER;


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// ���ڿ��� �׷���ID�� ���� �׷���ID�� ���ν����ִ� Ŭ����
///		- ��������Ʈ �����Ϳ��� �Է��� ���ڿ� ID�� ������ ���� ID�� �����ϰ� Client������ �ش� Sprite���� ���ڿ� ID�� �̿��Ҽ� �ֵ��� �����ش�.
///		- Module���� ������ �ʿ� �����Ѵ�.
///		- ����ȭ�Ͻý���(TriggerVFS)�� ������� �ʱ� ������ �����ǿ��� ������ ����Ǿ� �ִ�.
///
/// @Author			������
/// @Date			2003/11/28
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TGAMECTRL_API CResourceMgr
{
private:
	CResourceMgr(void);
	virtual ~CResourceMgr(void);
	static	CResourceMgr*	s_pInstance;

public:
	static	CResourceMgr*	GetInstance();
	void	Destroy();

	int GetImageNID( int iModuleID, const char* szSID );				/// ���ڿ�ID�� ModuleID�� ����ID�� ã�´�.
	bool AddResource( char* szFileName, int iModuleID );				/// Client���� ��������Ʈ ���� �߰�
	void UnLoadResource( int iModuleID );								/// �ش� Module�� ���� ������ Unload


protected:
	void LoadResourceFile( char* szFileName, int iModuleID );
	

protected:
	TABLE_MODULE		m_ResourceTable;
};
#endif