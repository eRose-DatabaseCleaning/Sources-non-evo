#ifndef _IListCellRenderer_
#define _IListCellRenderer_

#include "TGameCtrl.h"
#include <crtdbg.h>

class CTObject;
class CWinCtrl;

//---------------------------------------------------------------------------------------------------
/// CJListBox�� items�� Drawing Class
///
/// *����Class	CJListBox
///
/// @Author		������
/// @Date		2005/8/23
//---------------------------------------------------------------------------------------------------

class TGAMECTRL_API IListCellRenderer
{
public:
	virtual CWinCtrl* GetListCellRendererComponent( const CTObject* pObject, bool selected, bool mouseover ) = 0/* { _ASSERT( 0 && "IListCellRenderer::GetListCellRendererComponent"); return NULL;}*/;
};
#endif