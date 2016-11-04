#ifndef _CTCMDNUMBERINPUT_
#define _CTCMDNUMBERINPUT_
#include "../../../tgamectrl/tcommand.h"
#include "../../Common/CItem.h"


/// Item�̵��� �����Է��� �䱸�ɼ� �ִ� ó���� ���� Command
class CTCmdNumberInput : public CTCommand
{
public:
	CTCmdNumberInput(void){};
	~CTCmdNumberInput(void){};
	virtual bool Exec( CTObject* pObj) { return true; }
	void SetNumber( __int64 iNumber ){ m_iNumber = iNumber; }

protected:
	__int64	m_iNumber;
};


/// �κ��丮���� �ٴ����� ������ ������
class CTCmdDropItem : public CTCmdNumberInput
{
public:
	CTCmdDropItem(void){}
	virtual ~CTCmdDropItem(void){}
	virtual bool	Exec( CTObject* pObj);
};

/// @breief �κ��丮���� �ٴ����� ������ ���� ������.
class CTCmdDropMoney : public CTCmdNumberInput
{
public:
	CTCmdDropMoney(void){}
	virtual ~CTCmdDropMoney(void){}
	virtual bool Exec( CTObject* pObj );
};

///1:1 �ŷ��� �κ��丮���� �ŷ�â������ ������ �̵�
class CTCmdAddMyItem2Exchange : public CTCmdNumberInput
{
public:
	CTCmdAddMyItem2Exchange(void){};
	virtual ~CTCmdAddMyItem2Exchange(void){};
	virtual bool	Exec( CTObject* pObj );
};

/// 1:1�ŷ��� �ŷ�â���� �κ��丮�� ������ �̵�
class CTCmdRemoveMyItemFromExchange : public CTCmdNumberInput
{
public:
	CTCmdRemoveMyItemFromExchange(void){};
	virtual ~CTCmdRemoveMyItemFromExchange(void){};
	virtual bool	Exec( CTObject* pObj );
};

/// 1:1 �ŷ��� �κ��丮���� �ŷ�â���� �� �̵�
class CTCmdAddMyMoney2Exchange : public CTCmdNumberInput
{
public:
	CTCmdAddMyMoney2Exchange(void){};
	~CTCmdAddMyMoney2Exchange(void){};
	virtual bool Exec( CTObject* pObj );
};

/// 1:1 �ŷ��� �ŷ�â���� �κ��丮�ε��� ����.
class CTCmdRemoveMyMoneyFromExchange : public CTCmdNumberInput
{
public:
	CTCmdRemoveMyMoneyFromExchange(void){};
	~CTCmdRemoveMyMoneyFromExchange(void){};

	virtual bool Exec( CTObject* pObj );
};


/// �κ��丮���� â��� ������ �̵�
class CTCmdMoveItemInv2Bank : public CTCmdNumberInput
{
public:
	CTCmdMoveItemInv2Bank(void){}
	virtual ~CTCmdMoveItemInv2Bank(void){}
	virtual bool Exec( CTObject* pObj );
};

/// â���� �κ��丮�� ������ �̵�
class CTCmdMoveItemBank2Inv : public CTCmdNumberInput
{
public:
	CTCmdMoveItemBank2Inv(){}
	virtual ~CTCmdMoveItemBank2Inv(void){}
	virtual bool Exec( CTObject* pObj );
};

/// NPC���� �ŷ��� �κ��丮���� �ŷ�â���� ������ �̵�
class CTCmdAddItem2DealFromInventory : public CTCmdNumberInput
{
public:
	CTCmdAddItem2DealFromInventory(void){}
	virtual ~CTCmdAddItem2DealFromInventory(void){}
	virtual bool Exec( CTObject* pObj );
};

/// NPC ���� �ŷ��� �������� �ŷ�â���� ������ �̵�
class CTCmdAddItem2DealFromStore : public CTCmdNumberInput
{
public:
	CTCmdAddItem2DealFromStore(void){}
	virtual ~CTCmdAddItem2DealFromStore(void){}
	virtual bool Exec(CTObject* pObj );
};

/// NPC�����ŷ��� �������� �������� ���� �巡��&����Ͽ� ������ ���Խ� ���Ǵ� Command
class CTCmdBuyItem : public CTCmdNumberInput
{
public:
	CTCmdBuyItem(void){}
	virtual ~CTCmdBuyItem(void){}
	virtual bool Exec(CTObject* pObj );
};

/// NPC�����ŷ��� �κ��丮���� �������� ���� �巡��&����Ͽ� ������ �ǸŽ� ���Ǵ� Command
class CTCmdSellItem : public CTCmdNumberInput
{
public:
	CTCmdSellItem(void){}
	virtual ~CTCmdSellItem(void){}
	virtual bool Exec(CTObject* pObj );
};

/// ���λ������� �������� ���.
class CTCmdBuyItemAtAvatarStore : public CTCmdNumberInput
{
public:
	CTCmdBuyItemAtAvatarStore(void){}
	virtual ~CTCmdBuyItemAtAvatarStore(void){}
	virtual bool Exec( CTObject* pObj );
};

/// ���λ����� �������� �Ǵ�.
class CTCmdSellItem2AvatarStore : public CTCmdNumberInput
{
public:
	CTCmdSellItem2AvatarStore(void){}
	virtual ~CTCmdSellItem2AvatarStore(void){}
	virtual bool Exec( CTObject* pObj );
};

/// ���ϸ��� â���� �������� �����´�.
class CTCmdBringItemFromMall : public CTCmdNumberInput
{
public:
	CTCmdBringItemFromMall(){}
	virtual ~CTCmdBringItemFromMall(){}
	virtual bool Exec( CTObject* pObj );
};

#endif