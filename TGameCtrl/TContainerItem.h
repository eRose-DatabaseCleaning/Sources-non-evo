#ifndef _CTContainerItem_
#define _CTContainerItem_
#include "tobject.h"
#include <string>


/**
* TObject�� Item���� ������ Container�� Item���� �ֱ� ���� Class
*	- ǥ���ϰ��� �ϴ� ���� ���� ���� std::string���� ������ �ִ�
*
* @Author	������
*
* @Date		2005/8/30
*/
class TGAMECTRL_API CTContainerItem :	public CTObject
{
	std::string		m_strIdentify;///ǥ�ÿ� ���Ǵ� string
	std::string		m_strValue;///���� �����ϴ� string
public:
	CTContainerItem(void);
	virtual ~CTContainerItem(void);

	const char*		GetIndetify() const;
	const char*		GetValue() const;
	void			SetValue( const char* pszValue );
	void			SetIdentify( const char* pszIndentify );
};
#endif