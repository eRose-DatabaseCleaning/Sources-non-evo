#ifndef	__STDAFX_H
#define	__STDAFX_H
//-------------------------------------------------------------------------------------------------

#define	_WIN32_WINNT 0x0500
//#define	__SERVER


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define	__SHO_LS_LIB
// #define	__TAIWAN
// #define	__PHILIPPINE

#define	USE_ORACLE_DB	1	// �系���� ������...
#define	USE_MSSQL			// �ѱ��� ������ �ٸ� ����...


#include <Winsock2.h>
	
#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>
#include <assert.h>
#include <stdio.h>

#include "LIB_Util.h"
#include "classHASH.h"
#include "classTIME.h"
#include "classLOG.h"
#include "classSTR.h"

#include "classSYNCOBJ.h"
#include "CDataPOOL.h"
#include "DLLIST.h"
#include "CDLList.h"
#include "CVector.h"
#include "Net_Prototype.h"
#include "SRV_Prototype.h"

#include "IOCPSocketSERVER.h"
#include "ioDataPOOL.h"
#include "CRandom.h"


//-------------------------------------------------------------------------------------------------
#endif