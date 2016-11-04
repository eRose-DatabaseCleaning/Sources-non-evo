# Microsoft Developer Studio Project File - Name="SHO_WS_LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SHO_WS_LIB - Win32 Debug2
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SHO_WS_LIB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SHO_WS_LIB.mak" CFG="SHO_WS_LIB - Win32 Debug2"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SHO_WS_LIB - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SHO_WS_LIB - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SHO_WS_LIB - Win32 Compaq Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SHO_WS_LIB - Win32 Debug2" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SHO_WS/SHO_WS_LIB", KQAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SHO_WS_LIB - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../Client/Network" /I "../../../Client/Common" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SHO_WS_LIB - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../Client/Common" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SHO_WS_LIB - Win32 Compaq Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SHO_WS_LIB___Win32_Compaq_Debug"
# PROP BASE Intermediate_Dir "SHO_WS_LIB___Win32_Compaq_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compaq_Debug"
# PROP Intermediate_Dir "Compaq_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../Client/Common" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../Client/Common" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SHO_WS_LIB - Win32 Debug2"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SHO_WS_LIB___Win32_Debug2"
# PROP BASE Intermediate_Dir "SHO_WS_LIB___Win32_Debug2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug2"
# PROP Intermediate_Dir "Debug2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../Client/Common" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../Client/Common" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "SHO_WS_LIB - Win32 Release"
# Name "SHO_WS_LIB - Win32 Debug"
# Name "SHO_WS_LIB - Win32 Compaq Debug"
# Name "SHO_WS_LIB - Win32 Debug2"
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
# Begin Group "Zone"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WS_ZoneLIST.cpp
# End Source File
# Begin Source File

SOURCE=.\WS_ZoneLIST.h
# End Source File
# End Group
# Begin Group "CWS_Thread"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WS_ThreadSQL.cpp
# End Source File
# Begin Source File

SOURCE=.\WS_ThreadSQL.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Client\Common\CInventory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Client\Common\CInventory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Client\Common\CItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Client\Common\CItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Client\Common\IO_STB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Client\Common\IO_STB.h
# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Common\CDB_Socket.cpp
# End Source File
# Begin Source File

SOURCE=.\CWS_Client.cpp
# End Source File
# Begin Source File

SOURCE=.\CWS_Client.h
# End Source File
# Begin Source File

SOURCE=.\CWS_Server.cpp
# End Source File
# Begin Source File

SOURCE=.\CWS_Server.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Client\Network\NET_Prototype.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\SRV_Prototype.h
# End Source File
# Begin Source File

SOURCE=.\WS_SocketLOG.cpp
# End Source File
# Begin Source File

SOURCE=.\WS_SocketLOG.h
# End Source File
# Begin Source File

SOURCE=.\WS_SocketLSV.cpp
# End Source File
# Begin Source File

SOURCE=.\WS_SocketLSV.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CWS_API.h
# End Source File
# Begin Source File

SOURCE=..\..\SHO_GS\SHO_GS_LIB\ETC_Math.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SHO_GS\SHO_GS_LIB\ETC_Math.h
# End Source File
# Begin Source File

SOURCE=.\SHO_WS_LIB.CPP
# End Source File
# Begin Source File

SOURCE=.\SHO_WS_LIB.H
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
