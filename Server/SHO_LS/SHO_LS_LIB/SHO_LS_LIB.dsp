# Microsoft Developer Studio Project File - Name="SHO_LS_LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SHO_LS_LIB - Win32 Compaq Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SHO_LS_LIB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SHO_LS_LIB.mak" CFG="SHO_LS_LIB - Win32 Compaq Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SHO_LS_LIB - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SHO_LS_LIB - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SHO_LS_LIB - Win32 Compaq Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/OnlineRPG/Server/SHO_LS/SHO_LS_LIB", GUCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SHO_LS_LIB - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../Client/Network" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "__SERVER" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SHO_LS_LIB - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Client/Network" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "__SERVER" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SHO_LS_LIB - Win32 Compaq Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SHO_LS_LIB___Win32_Compaq_Debug"
# PROP BASE Intermediate_Dir "SHO_LS_LIB___Win32_Compaq_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compaq_Debug"
# PROP Intermediate_Dir "Compaq_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../Client/Network" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "__SERVER" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Client/Network" /I "../../LIB_Util" /I "../../LIB_Server" /I "../../Client/Util" /I "../../Common" /I "../../../LIB_Util" /I "../../../LIB_Server" /I "../../../Client/Network" /I "../../../Client/Util" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "__SERVER" /YX /FD /GZ /c
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

# Name "SHO_LS_LIB - Win32 Release"
# Name "SHO_LS_LIB - Win32 Debug"
# Name "SHO_LS_LIB - Win32 Compaq Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "CLS_Thread"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CLS_SQLThread.cpp
# End Source File
# Begin Source File

SOURCE=.\CLS_SQLThread.h
# End Source File
# End Group
# Begin Group "CLS_List"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CLS_Account.cpp
# End Source File
# Begin Source File

SOURCE=.\CLS_Account.h
# End Source File
# Begin Source File

SOURCE=.\CLS_Client.cpp
# End Source File
# Begin Source File

SOURCE=.\CLS_Client.h
# End Source File
# Begin Source File

SOURCE=.\CLS_Server.cpp
# End Source File
# Begin Source File

SOURCE=.\CLS_Server.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CLS_API.h
# End Source File
# Begin Source File

SOURCE=.\SHO_LS_LIB.CPP
# End Source File
# Begin Source File

SOURCE=.\SHO_LS_LIB.H
# End Source File
# Begin Source File

SOURCE=.\stdAFX.h
# End Source File
# End Group
# End Target
# End Project
