#ifndef _CDragItem_
#define _CDragItem_

#include "interfacetype.h"
#include "../gamecommon/IObserver.h"
#include <map>
class CIcon;
class CTCommand;

/**
* �巡��N��� �ý��ۿ��� �巡�� ���۽� ��ϵǴ� Class
*	- ���� CIcon�� �巡��N����� �ȴ�
*	- Target �� �ش� Ÿ�ٿ� ����� ������ CTCommand�� ������ �ִ´�.
*		-- TagetType
*			0						: Ground, 
*			1 ~ DLG_TYPE_MAX - 1	: TDialog
*			DLG_TYPE_MAX			: TARGETALL - �׻� ó��
*		-- CTCommand�� Null�ϰ��  CIcon::ExecuteCommand�� ����ȴ�
*	
* @Author		������
* @Date			2005/9/5
*/
class CDragItem
{
public:
	CDragItem(void);
	~CDragItem(void);

	void AddTarget( int iTargetType , CTCommand* pCmd );		/// ����Ҽ� �ִ� Target�� Target�� ����� ������ CTCommand 
	void SetIcon( CIcon* pIcon );								/// �巡���� CIcon�� Clone
	
	CIcon*	GetIcon();
	const std::map< int , CTCommand* >& GetTargets();

	enum{
		TARGET_GROUND,
		TARGET_ALL			= DLG_TYPE_MAX
	};

protected:
	std::map< int , CTCommand* > m_Targets;
	CIcon*	m_pIcon;

};
#endif