#ifndef _CTEvent_
#define _CTEvent_

#include "../../tgamectrl/tobject.h"

/**
* Observable�� Observer���� �޼��� ���޿� ���Ǵ� CTEvent�� Base Class
*	
* @Warning	CTEvent�� ������ ��Ʈ������ �ϰ� �ִ�. �̶� ���� ���̸� ������� �̻����Ҷ��� �ִ�
* @Author	������
* @Date		2005/9/15
*/
class CTEvent :	public CTObject
{
public:
	CTEvent(void);
	virtual ~CTEvent(void);

	void	SetID( int iID );
	int		GetID();

protected:
	int		m_iID;
};
#endif