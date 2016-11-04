#ifndef _CIconItemDelivery_
#define _CIconItemDelivery_
#include "ciconitem.h"
/**
* ���ϸ��� ������ â���� ���Ǵ� Item Icon
*
* @Author		������
* @Date			2005/9/6
*/
class CIconItemDelivery : public CIconItem
{
public:
	CIconItemDelivery(void);
	CIconItemDelivery( tagITEM* pItem , const char* pszFrom );
	virtual ~CIconItemDelivery(void);


	virtual unsigned	Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );
	virtual void		Draw();
	virtual void		ExecuteCommand(){}
	virtual void		GetToolTip( CInfo& strToolTip,  DWORD dwDialogType, DWORD dwType );


	static int s_selected_icon;			/// ���õ� �����ܿ� ���� �ε����� static���� ������ �ִ´�

protected:
	int				m_border_grahicid;
	std::string		m_from;
};
#endif