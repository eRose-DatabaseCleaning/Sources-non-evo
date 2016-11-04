//----------------------------------------------------------------------------
// File: systeminfo.h
// �̸��� ���� ������ sysinfo.h - DxDiagReport_2003 �ַ�� ����.
// Desc: 
//
// Copyright (c) Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef SYSTEMINFORMATION_H
#define SYSTEMINFORMATION_H


typedef struct SystemInformation
{
    DWORD       m_dwOSMajorVersion;         // OS major version
    DWORD       m_dwOSMinorVersion;         // OS minor version
    DWORD       m_dwOSBuildNumber;          // OS build number
    DWORD       m_dwOSPlatformID;           // OS platform id
    DWORD       m_dwDirectXVersionMajor;    // DirectX major version (ex. 8) -- info in m_szDirectXVersion*
    DWORD       m_dwDirectXVersionMinor;    // DirectX minor version (ex. 1) -- info in m_szDirectXVersion*
    TCHAR       m_szDirectXVersionLetter[2]; // DirectX letter version (ex. a) -- info in m_szDirectXVersion*
    BOOL        m_bDebug;                   // debug version of user.exe -- info in m_szOSEx*
    BOOL        m_bNECPC98;                 // info in m_szMachineName*
    DWORDLONG   m_ullPhysicalMemory;        // info in m_szPageFile*
    DWORDLONG   m_ullUsedPageFile;          // info in m_szPageFile*
    DWORDLONG   m_ullAvailPageFile;         // info in m_szPageFile*
    BOOL        m_bNetMeetingRunning;       // info in DX file notes

    BOOL        m_bIsD3D8DebugRuntimeAvailable;
    BOOL        m_bIsD3DDebugRuntime;
    BOOL        m_bIsDInput8DebugRuntimeAvailable;
    BOOL        m_bIsDInput8DebugRuntime;
    BOOL        m_bIsDMusicDebugRuntimeAvailable;
    BOOL        m_bIsDMusicDebugRuntime;
    BOOL        m_bIsDDrawDebugRuntime;
    BOOL        m_bIsDPlayDebugRuntime;
    BOOL        m_bIsDSoundDebugRuntime;

    LONG        m_nD3DDebugLevel;
    LONG        m_nDDrawDebugLevel;
    LONG        m_nDIDebugLevel;
    LONG        m_nDMusicDebugLevel;
    LONG        m_nDPlayDebugLevel;
    LONG        m_nDSoundDebugLevel;
    LONG        m_nDShowDebugLevel;

    // English only or un-localizable strings 
    TCHAR m_szWindowsDir[MAX_PATH];             // location of windows dir
    TCHAR m_szBuildLab[100];                    // Win2k build lab (not localizable)
    TCHAR m_szDxDiagVersion[100];               // DxDiag version (not localizable)
    TCHAR m_szSetupParamEnglish[100];           // setup params (English)
    TCHAR m_szProcessorEnglish[200];            // Processor name and speed (english)
    TCHAR m_szSystemManufacturerEnglish[200];   // System manufacturer (english)
    TCHAR m_szSystemModelEnglish[200];          // System model (english)
    TCHAR m_szBIOSEnglish[200];                 // BIOS (english)
    TCHAR m_szPhysicalMemoryEnglish[100];       // Formatted version of physical memory (english)
    TCHAR m_szCSDVersion[200];                  // OS version with CSD info (localized)
    TCHAR m_szDirectXVersionEnglish[100];       // DirectX version (english)
    TCHAR m_szDirectXVersionLongEnglish[100];   // long DirectX version (english)

    // strings localized to OS language 
    TCHAR m_szMachineNameLocalized[200];        // machine name 
    TCHAR m_szOSLocalized[100];                 // Formatted version of platform (localized)
    TCHAR m_szOSExLocalized[100];               // Formatted version of platform, version, build num (localized)
    TCHAR m_szOSExLongLocalized[300];           // Formatted version of platform, version, build num, patch, lab (localized)
    TCHAR m_szLanguagesLocalized[200];          // m_szLanguages, in local language (localized)
    TCHAR m_szPageFileLocalized[100];           // Formatted version of pagefile (localized)
    TCHAR m_szTimeLocalized[100];               // Date/time, localized for UI (localized)

    // strings localized to english 
    TCHAR m_szMachineNameEnglish[200];          // machine name 
    TCHAR m_szOSEnglish[100];                   // Formatted version of platform (english)
    TCHAR m_szOSExEnglish[100];                 // Formatted version of platform, version, build num (english)
    TCHAR m_szOSExLongEnglish[300];             // Formatted version of platform, version, build num, patch, lab (english)
    TCHAR m_szLanguagesEnglish[200];            // Formatted version of m_szLanguage, m_szLanguageRegional (english) 
    TCHAR m_szPageFileEnglish[100];             // Formatted version of pagefile (english)
    TCHAR m_szTimeEnglish[100];                 // Date/time, dd/mm/yyyy hh:mm:ss for saved report (english)

//    CPUExtendedFunctionBitmask m_ExtFuncBitmasks[16];  // 128-bit CPU Extended Function Bitmasks (array of 16-byte structs) 

    DWORD m_nElementCount;
}STINFO, *LPSTINFO;

#endif // SYSTEMINFORMATION_H