#include "stdafx.h"
#include <windows.h>

/*********************************************************************
 *
 * GetRegistryValue - Registry���� ���� ������ �´�
 * hKey		: HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_USERS
 *				Windows Me/98/95:  HKEY_DYN_DATA
 * KeyPath	: Key Path �� ) "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"
 *
 */

DWORD GetRegistryValue (HKEY hKey, const char * KeyPath)
{
	HKEY hResKey;

	LONG lRet = RegOpenKeyEx (hKey
							, KeyPath
							, 0
							, KEY_QUERY_VALUE
							, &hResKey );

	RegCloseKey( hKey );

	return 0;
}