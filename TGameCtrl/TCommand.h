#ifndef _TCOMMAND_
#define _TCOMMAND_

#include "TGameCtrl.h"
class CTObject;


/**
* �������̽� ���������� ���Ǵ� Command Pattern���� ���Ǵ� Command�� Base Class
* - ����Class				CTMacroCommand, CTCommandQ
*
* @Author	������
*
* @Date		2005/8/30
*/
class TGAMECTRL_API CTCommand
{
public:
	CTCommand(void);
	virtual ~CTCommand(void);
	
	virtual	bool Exec(CTObject*) = 0;
	bool	IsWaitExecSucceed(){ return m_bWaitExec; }

protected:
	/// Execute���нÿ� ���� TCommand�� ť�� �ǵڷ� �̵��Ѵ�.
	/// �׷��� m_bWaitExec�� true�ϰ�쿡�� �������� �ʰ� ť�� �� �տ� �׷��� ����Ͽ� ������ �����Ҷ� ���� �ݺ��ϰ� �Ѵ�.
	/// Default�� false;
	/// => �������� Command���� �켱������ �ΰ� �켱���� ť�� �����ؾ� �Ѵ�.
	bool	m_bWaitExec;
};


#include <list>
//----------------------------------------------------------------------------------------------------------
/// @Brief					CTCommand�� ������ �����ϱ� ���� Multi Command Class
///
/// @Warning				MacroCommand�� Exec���ڷ� �׻� NULL�� ���ϱ� �����Ҳ�
///
/// @Author					2005/8/30 ������
//----------------------------------------------------------------------------------------------------------
class TGAMECTRL_API CTMacroCommand : public CTCommand
{
public:
	CTMacroCommand( void ){};
	~CTMacroCommand( void );
	virtual bool Exec( CTObject* );
	void	AddSubCommand( CTCommand* pCmd );
protected:
	std::list<CTCommand*>	m_listCommand;
};
#endif