#ifndef	_IMAGEIDMGR_
#define _IMAGEIDMGR_
#include "TGameCtrl.h"
#include <map>
#include <string>
#include <list>
using namespace std;

typedef		map< string, int >			TABLE_IMAGE_ID;
typedef		TABLE_IMAGE_ID::iterator	TABLE_IMAGE_ID_ITER;

typedef		map< int,TABLE_IMAGE_ID >	TABLE_MODULE;
typedef		TABLE_MODULE::iterator		TABLE_MODULE_ITER;
////////////////////////////////////////////////////////////////////////////////////////////
/// 2003 / 11 /28 / ������
/// ���ڿ��� �׷���ID�� ���� �׷���ID�� ���ν����ִ� Ŭ����
////////////////////////////////////////////////////////////////////////////////////////////
class TGAMECTRL_API CImageIDMgr
{
private:
	CImageIDMgr(void);
	virtual ~CImageIDMgr(void);
	static	CImageIDMgr*	s_pInstance;


public:
	static	CImageIDMgr*	GetInstance();
	void	Destroy();

	int GetImageNID( int iModuleID, const char* szSID );///���ڿ�ID�� ModuleID�� ����ID�� ã�´�.
	bool AddResource( char* szFileName, int iModuleID );///Client���� Table�� ����� ��û�Ҷ� ���Ǵ� operation


protected:
	void LoadResourceFile( char* szFileName, int iModuleID );
	

protected:
	TABLE_MODULE		m_ResourceTable;
};
#endif
