// TriggerDetect.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CTriggerDetectApp:
// �� Ŭ������ ������ ���ؼ��� TriggerDetect.cpp�� �����Ͻʽÿ�.
//

class CTriggerDetectApp : public CWinApp
{
public:
	CTriggerDetectApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CTriggerDetectApp theApp;
