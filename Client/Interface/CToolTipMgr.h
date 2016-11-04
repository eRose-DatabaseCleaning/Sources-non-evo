#ifndef _CToolTipMgr_
#define _CToolTipMgr_
#include "CInfo.h"


///<- ���� define�� ������ �ʴ°� ���� 2005/9/5 ������
#define INFO_STATUS_DEF			0x00000001			
#define INFO_STATUS_ATK			0x00000002
#define INFO_STATUS_ACC			0x00000004
#define INFO_STATUS_GEM			0x00000010
#define INFO_STATUS_USE			0x00000020
#define INFO_STATUS_ETC			0x00000040
//->

/**
* ���� �ý��ۿ����� ������ Class
*	- ���� ������ �������Ӵ� CInfo�� string�� ���� ����ϰ� Draw�ϴ� ��ȿ���� �����̴�. 
*
* @Author		������
* @Date			2005/9/5	
*/

class CToolTipMgr
{
	CToolTipMgr(void);
public:
	~CToolTipMgr(void);
	static CToolTipMgr& GetInstance();

	/// ���� ��� Methods
	void  RegUIInfo( const int x, const int y, const DWORD dwDlgType, const int iToolTipID );
	void  RegistInfo( const CInfo& Info );
	void  RegToolTip( int x, int y, const char* pszMsg );

	/// ��ϵ� ���� Draw
	void  Draw();

private:
	bool			m_bHasDrawInfo;
	CInfo			m_Info;

};
#endif