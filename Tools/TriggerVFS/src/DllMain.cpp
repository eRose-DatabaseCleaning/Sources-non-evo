#include "stdafx.h"
#include "DllMain.h"
#include "TriggerVFS.h"

/***********************************************************************************
 * Dll Main
 */
#ifdef __DLL
BOOL APIENTRY DllMain ( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#endif

