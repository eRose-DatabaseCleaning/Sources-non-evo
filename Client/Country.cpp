#include "stdafx.h"
#include ".\country.h"


unsigned long ReadRegistryKeyValue (HKEY hRootKey , const char * Path, const char * KeyName )
{
	HKEY hKey;
	LPBYTE lpbVALUE ;
	DWORD buflen, ret;
	char szZeroEndedPath[ 1024 ];

	sprintf ( szZeroEndedPath, "%s", Path );

    if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, szZeroEndedPath, 0, KEY_READ, &hKey)) 
	{
        lpbVALUE = 0;
        buflen = sizeof( lpbVALUE );
        ret = RegQueryValueEx(hKey, KeyName, NULL, NULL, (LPBYTE) &lpbVALUE, &buflen);
		
		RegCloseKey ( hKey );
		if (ret == ERROR_SUCCESS) 
			return (unsigned long)lpbVALUE;
    }

	return 0;
}

CCountry _country;

CCountry::CCountry(void)
{
	m_dwCountryCode = 0;
}

CCountry::~CCountry(void)
{
}

bool CCountry::CheckCountry()
{
	char * Path =  "SOFTWARE\\TriggerSoft\\Rose Online";
	unsigned long dwValue = ReadRegistryKeyValue ( 	HKEY_LOCAL_MACHINE, Path, "Language" );


	memset( m_bIsThatCountry, 0, sizeof( m_bIsThatCountry ) );	
	m_dwCountryCode = 0;

	if( dwValue < COUNTRY_MAX )
	{
		m_dwCountryCode				= dwValue;
		m_bIsThatCountry[ dwValue ] = true;
		return true;
	}

	return false;
}
//*-----------------------------------------------------------------------------------------/
/// ���ο� �ý����� ������Ѿ� �ϴ°�?
//*-----------------------------------------------------------------------------------------/
bool CCountry::IsApplyNewVersion()
{
//	if( this->IsTAIWAN() || this->IsPhilipplines() || this->IsUSA() || this->IsEU() ) 
//		return true; 

	if( this->IsKorea() || this->IsJapan() )
	{
		return false;
	}

	return false;
	//return true;
}

//*-----------------------------------------------------------------------------------------/
///UseItem Delay�� ���Ӱ� ����� ����üũ 
//*-----------------------------------------------------------------------------------------/
bool CCountry::IsUseItemDelayNewVersion()
{
	return IsApplyNewVersion();
}

DWORD CCountry::GetCountryCode()
{ 
	return m_dwCountryCode; 
}
bool CCountry::IsTAIWAN()
{ 
	//Numenor: We want to avoid Taiwan, let's put it to false for the moment.
	/*
	return m_bIsThatCountry[ COUNTRY_CHA_TW ]; 
	*/
	return false;
}
bool CCountry::IsROSE() 
{ 
	return m_bIsThatCountry[ COUNTRY_IROSE ]; 
}
bool CCountry::IsPhilipplines()
{ 
	return m_bIsThatCountry[ COUNTRY_PH ]; 
}
bool CCountry::IsJapan()
{
	return m_bIsThatCountry[ COUNTRY_JPN ];
}
bool CCountry::IsUSA()
{
	return m_bIsThatCountry[ COUNTRY_USA ];
}
bool CCountry::IsKorea()
{
	return m_bIsThatCountry[ COUNTRY_KOR ];
}
bool CCountry::IsEU()
{
	return m_bIsThatCountry[ COUNTRY_EU ];
}

//---------------------------------------------------------------------------------------
/// @brief Ŭ���̾�Ʈ ����� �ε�� ������Ʈ������ ���ڷ� �Ѿ�� �����ڵ尡 ������ �Ǵ��Ѵ�.
//---------------------------------------------------------------------------------------
bool CCountry::IsCountry( int country )
{
	/// �迭 ���� üũ
	assert( country >= 0 && country < COUNTRY_MAX );
	if( country < 0 && country >= COUNTRY_MAX ) return false;

	return m_bIsThatCountry[ country ];
}

//--------------------------------------------------------------------------------------
bool IsTAIWAN()
{	
	//Numenor: We don't want to work with Taiwan!
	/*
	return true;//( CCountry::GetSingleton().IsTAIWAN() || CCountry::GetSingleton().IsPhilipplines() || CCountry::GetSingleton().IsUSA() );
	*/
	return false;
}


