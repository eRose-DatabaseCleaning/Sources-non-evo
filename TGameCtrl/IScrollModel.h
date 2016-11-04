#ifndef _ISCROLLMODEL_
#define _ISCROLLMODEL_

#include <crtdbg.h>

//-------------------------------------------------------------------------------------------
/// Scroll�� ���̱� ���ϴ� Ŭ������ Implement�ؾ��ϴ� Inteface
///		- Extent : �� ȭ�鿡�� ������ Item�� ����
///
///
/// @Author				������
/// @Date				2003/11/26 
//-------------------------------------------------------------------------------------------

class TGAMECTRL_API IScrollModel
{
public:

	/// ���� ȭ�鿡�� ���� ���� �׷��� Item�� �ε��� ����
	virtual int GetValue() = 0; /*{ _ASSERT(0 && "IScrollModel::GetValue")		; return 0; }*/;
	
	/// ȭ�鿡 �ѹ��� ������ Item�� ���� ����
	virtual int GetExtent() = 0; /*{ _ASSERT(0 && "IScrollModel::GetExtent")	; return 0; }*/;

	/// �ǹ̻����δ� �ִ�Item�� ���������� ���������δ� ���� Item�� ���� �����ϰ� �ִ�.
	virtual int GetMaximum() = 0; /*{ _ASSERT(0 && "IScrollModel::GetMaximum")	; return 0; };*/

	/// CTree�̿ܿ��� ��� return 0;
	virtual int GetMinimum() = 0; /*{ _ASSERT(0 && "IScrollModel::GetMinimum")	; return 0; };*/
	




	/// ���� ȭ�鿡�� ���� ���� �׷��� Item ����
	virtual void SetValue( int ) = 0; /*{ _ASSERT(0 && "IScrollModel::SetValue");	};*/

	/// Set Extent
	virtual void SetExtent( int ) = 0; /*{ _ASSERT(0 && "IScrollModel::SetExtent");	};*/

	/// ���� �������� �ʰ� �ִ�.
	virtual void SetMaximum( int ) = 0; /*{ _ASSERT(0 && "IScrollModel::SetMaximum");};*/

	/// ������
	virtual void SetMinimum( int ) = 0;/* { _ASSERT(0 && "IScrollModel::SetMinimum");};*/

	/// ScrollBox���� �ٹ� ����� ó���� �ϱ� ���Ͽ� Model�� ȭ��� ������ �ʿ��ϴ�
	virtual RECT GetWindowRect() = 0; /*{ _ASSERT( 0 && "IScrollModel::GetWindowRect"); RECT rc = {0,0,0,0}; return rc; };*/
	

	/// ������ �������� �׷��� ���� �ִ°�?
	virtual bool IsLastItemDrawn() { return true; }
};
#endif