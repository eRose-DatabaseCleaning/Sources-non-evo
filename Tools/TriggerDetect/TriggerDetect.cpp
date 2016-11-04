// TriggerDetect.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "TriggerDetect.h"
#include "TriggerDetectDlg.h"
#include "TriggerInfo.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTriggerDetectApp

BEGIN_MESSAGE_MAP(CTriggerDetectApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTriggerDetectApp ����

CTriggerDetectApp::CTriggerDetectApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CTriggerDetectApp ��ü�Դϴ�.

CTriggerDetectApp theApp;
LPCSTR g_CmdLine = NULL;

// CTriggerDetectApp �ʱ�ȭ

BOOL CTriggerDetectApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControls()�� �ʿ��մϴ�. 
	// InitCommonControls()�� ������� ������ â�� ���� �� �����ϴ�.

	///[TRIGGER] DETECTED�� 1�̸� �׳� CLIENT����
	
	bool bSysDataFile = _access (SYSINFO_FILE, 0) == 0 && _access (ADPTINFO_FILE, 0) == 0;

	g_CmdLine = theApp.m_lpCmdLine;
	
	if(!TI_DXInfo())
	{
		MessageBox (NULL,"DirectX 9.0b is required! ","Error",MB_OK);
		return FALSE;
	}

	if(bSysDataFile)
	{
		if( theApp.m_lpCmdLine && strlen( theApp.m_lpCmdLine ) > 1 
			&& GetPrivateProfileInt( "TRIGGER","DETECTED", 0, "./SevenHearts.ini" ) == 1)
		{	

			ShellExecute( NULL, NULL, "TRose.exe", theApp.m_lpCmdLine, NULL, SW_SHOWNORMAL );	
			return false;
		}
	}
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	CTriggerDetectDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ ��ȭ ���ڰ� Ȯ���� ���� �������� ��� ó����
		// �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ���⿡ ��ȭ ���ڰ� ��Ҹ� ���� �������� ��� ó����
		// �ڵ带 ��ġ�մϴ�.
	}

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�
	// ���� ���α׷��� ���� �� �ֵ��� FALSE�� ��ȯ�մϴ�.
	
	return FALSE;
}
