#ifndef _ITSOUND_
#define _ITSOUND_

#include "TGameCtrl.h"
#include <crtdbg.h>

//------------------------------------------------------------------------------------------------------------------------
///	Client�� ��ӹ޾� �����ؾ��� Play Sound Interface - TGameCtrl�� �� �������̽��� ���ؼ� Sound�� Play�Ѵ�.
///
/// @Author			������
/// @Date			2005/08/23
//------------------------------------------------------------------------------------------------------------------------
class TGAMECTRL_API ITSound
{
public:
	virtual void Playsound( int iIndex ) = 0;/* { _ASSERT( 0 && "ITSound::PlaySound" );};*/
};
#endif