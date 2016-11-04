#ifndef __TRIGGERINFO_H__
#define __TRIGGERINFO_H__

#ifdef TRIGGERINFO_EXPORTS
#define TRIGGERINFO_API __declspec(dllexport)
#else
#define TRIGGERINFO_API __declspec(dllimport)
#endif

#include <string>

// system information �� �����ϴ� ����
#define SYSINFO_FILE		"sysinfo.dat"
#define SYSINFO_FILE_WEB	"sysinfow.dat"
// adapter information�� �����ϴ� ����
#define ADPTINFO_FILE "sysadpt.dat"

/********************************************************************
 * 
 * TI_GatherInfo - ������ ������
 * hWnd			: D3D �� �̿��ϱ� ���ؼ�
 *
 */
TRIGGERINFO_API const char * TI_GatherSysInfo (void);

/********************************************************************
 * 
 * TI_GetInfo - �˾� �� ������ ���ڿ� ���ۿ� �����Ѵ�
 * buff			: ���⿡ ī��
 * iLength		: buff�� ũ��
 *
 */
TRIGGERINFO_API const char * TI_GetSysInfo (void);

TRIGGERINFO_API const char * TI_GetSysInfoForWeb (void);

/********************************************************************
 * 
 * TI_SendHttpPostData - Post ������� �����͸� �����Ѵ�. �����Ͱ� Ŭ ���
 * szServerIP	: ������ IP		��) "218.55.179.114"
 * szPage		: ������		��) "/testaa/receive.php"
 * pData		: ������		��) "v1=kkkkk&v2=rrrrrddd&v3=abcdefgh.."
 *
 */


/******************************************************************
 * Dectect�Ǿ� �ִ� ������ �����ϰ� �ִ� ���Ͽ��� ������ �о�´�
 */
TRIGGERINFO_API const char * TI_ReadSysInfoFile (void);


TRIGGERINFO_API bool TI_SendHttpPostData (const char * szServerIP, const char * szPage, const char * pData);

/********************************************************************
 * 
 * TI_SendHttpQueryData - ���� ������� �����͸� �����Ѵ�. �����Ͱ� ���� ���
 * szURL	: ������ IP		��) "http:://218.55.179.114/aa.php?v1=kkkkk&v2=rrrrrddd&v3=abcdefgh.."
 *
 */
TRIGGERINFO_API bool TI_SendHttpQueryData (const char * szURL);

struct SystemResolution{
      
	int pixel_width;
	int pixel_height;
	int pixel_color;
    int frequency; 
	int	adaptor_number;
};

struct SystemAdapter{
    
	char Driver[ 100 ];
    char Description[ 100 ];
    char DeviceName[ 32 ];
    int  AdapterNumber;
	int	 nResolution;
    
	SystemResolution *pResolution;
};


struct AdapterInformation
{
  	  SystemAdapter *	pAdapter;
	  int				iAdapterCNT;
};


// Resolution ������ Release�Ѵ�
TRIGGERINFO_API void TI_ReleaseAdptInfo (void);

// ���Ͽ� ����Ǿ� �ִ� ������ ������ �´�.
TRIGGERINFO_API AdapterInformation * TI_ReadAdptInfoFile (void);

// ���� Dectect�Ǿ� �ִ� ������ ������ �´�.
TRIGGERINFO_API AdapterInformation * TI_GetAdptInfo (void);

// Resolution ���� ������ Dectect�Ѵ�
TRIGGERINFO_API AdapterInformation * TI_GatherAdptInfo (void);

TRIGGERINFO_API bool TI_DXInfo(void);


#endif
