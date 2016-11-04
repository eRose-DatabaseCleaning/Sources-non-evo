#ifndef _CTCandidateWindow_
#define _CTCandidateWindow_

#include "winctrl.h"

//---------------------------------------------------------------------------------------------------------------------
/// IME������ Candidate Window Classes�� Base Class
/// -CTIme
///
/// @Author			������
/// @Date			2005/8/30
//---------------------------------------------------------------------------------------------------------------------
class CTCandidateWindow : public CWinCtrl
{
public:
	CTCandidateWindow(void);
	virtual ~CTCandidateWindow(void);
	virtual unsigned Process( unsigned uiMsg, WPARAM wParam, LPARAM lParam );

	virtual void Draw(){};
	virtual void AddCandidate( const char* pszCandidate ) = 0;
	virtual void Clear() = 0;
	virtual void SetSelection( short n ){}
};
#endif