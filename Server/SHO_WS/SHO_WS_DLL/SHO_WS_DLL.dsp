# Microsoft Developer Studio Project File - Name="SHO_WS_DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SHO_WS_DLL - Win32 Debug2
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SHO_WS_DLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SHO_WS_DLL.mak" CFG="SHO_WS_DLL - Win32 Debug2"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SHO_WS_DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SHO_WS_DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SHO_WS_DLL - Win32 Compaq Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SHO_WS_DLL - Win32 Debug2" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SHO_WS_DLL", ZGAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SHO_WS_DLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHO_WS_DLL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../Client/Network" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /I "../SHO_WS_LIB" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHO_WS_DLL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"../../../LIB_Util/VC_LIB" /libpath:"../../../Client/Util" /libpath:"../../../LIB_Server/VC_LIB"

!ELSEIF  "$(CFG)" == "SHO_WS_DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHO_WS_DLL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Client/Network" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /I "../SHO_WS_LIB" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHO_WS_DLL_EXPORTS" /D "__SERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"libcd.lib" /out:"../SHO_WS.dll" /pdbtype:sept /libpath:"../../../LIB_Util/VC_LIB" /libpath:"../../../Client/Util" /libpath:"../../../LIB_Server/VC_LIB"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=make bcb lib
PostBuild_Cmds=..\dll2lib.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SHO_WS_DLL - Win32 Compaq Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SHO_WS_DLL___Win32_Compaq_Debug"
# PROP BASE Intermediate_Dir "SHO_WS_DLL___Win32_Compaq_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compaq_Debug"
# PROP Intermediate_Dir "Compaq_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Client/Network" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHO_WS_DLL_EXPORTS" /D "__SERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Client/Network" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /I "../SHO_WS_LIB" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHO_WS_DLL_EXPORTS" /D "__SERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"../SHO_WS.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"libcd.lib" /out:"q:/server/bin/SHO_WS.dll" /pdbtype:sept /libpath:"../../../LIB_Util/VC_LIB" /libpath:"../../../Client/Util" /libpath:"../../../LIB_Server/VC_LIB"

!ELSEIF  "$(CFG)" == "SHO_WS_DLL - Win32 Debug2"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SHO_WS_DLL___Win32_Debug2"
# PROP BASE Intermediate_Dir "SHO_WS_DLL___Win32_Debug2"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Debug2"
# PROP Intermediate_Dir "Debug2"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Client/Network" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /I "../SHO_WS_LIB" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHO_WS_DLL_EXPORTS" /D "__SERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Client/Network" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /I "../SHO_WS_LIB" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHO_WS_DLL_EXPORTS" /D "__SERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"libcd.lib" /out:"../SHO_WS.dll" /pdbtype:sept /libpath:"../../../LIB_Util/VC_LIB" /libpath:"../../../Client/Util" /libpath:"../../../LIB_Server/VC_LIB"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"libcd.lib" /out:"../Debug2/SHO_WS.dll" /pdbtype:sept /libpath:"../../../LIB_Util/VC_LIB" /libpath:"../../../Client/Util" /libpath:"../../../LIB_Server/VC_LIB"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=make bcb lib
PostBuild_Cmds=..\dll2lib.bat
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SHO_WS_DLL - Win32 Release"
# Name "SHO_WS_DLL - Win32 Debug"
# Name "SHO_WS_DLL - Win32 Compaq Debug"
# Name "SHO_WS_DLL - Win32 Debug2"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "stdAFX"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\SHO_WS_DLL.cpp
# End Source File
# Begin Source File

SOURCE=.\SHO_WS_DLL.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
