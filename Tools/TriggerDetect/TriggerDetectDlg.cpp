// TriggerDetectDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "TriggerDetect.h"
#include "TriggerDetectDlg.h"
#include "TriggerInfo.h"
#include ".\triggerdetectdlg.h"
#include "RgnBitmap.h"


extern LPCSTR g_CmdLine;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define		TESTWND_WIDHT	300
#define		TESTWND_HIEGT	300
#define		TESTWND_NAME	"Trigger Detect Window"

DefRes g_defs[] = 
{
	{1024, 768, 32}, {1024, 768, 16}, {800, 600, 32}, {800, 600, 16}
};

// CTriggerDetectDlg ��ȭ ����

CTriggerDetectDlg::CTriggerDetectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTriggerDetectDlg::IDD, pParent)
	, m_rdFullScreen(0)
	, m_rdWindow(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAdptInfo = NULL;
}

void CTriggerDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCCOMBO_MONITOR, m_cbMonitor);
	DDX_Control(pDX, IDCCOMBO_RESOLUTION, m_cbResolution);
	DDX_Control(pDX, IDCCB_FREQ, m_cbFreq);
	DDX_Control(pDX, IDCSLD_PERMERMANCE, m_sldPerform);
	DDX_Control(pDX, IDC_COMBO2, m_cbWinMode);
	DDX_Control(pDX, IDOK, m_OkButton);
}

BEGIN_MESSAGE_MAP(CTriggerDetectDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDCCOMBO_MONITOR, On_Adpt_SelChange)
	ON_CBN_SELCHANGE(IDCCOMBO_RESOLUTION, On_ResnColor_SelChange)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CTriggerDetectDlg �޽��� ó����

BOOL CTriggerDetectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	LPCSTR strMyClass = AfxRegisterWndClass (CS_VREDRAW | CS_HREDRAW);

	// �ý��� ������ ���� ���Ϸ� ����
	TI_GatherSysInfo ();

	const char * pSysInfo = TI_GetSysInfo ();
	const char * pSysInfo2 = TI_ReadSysInfoFile ();

	// �ػ� ������ ���� ���Ϸ� ����
	TI_GatherAdptInfo ();

	// ���Ͽ� ����� �ػ� ������ �ٽ� ������ ��.
	m_pAdptInfo = TI_ReadAdptInfoFile ();

	ListAdapter ();

	m_sldPerform.SetRange (1, 5, true);
	m_sldPerform.SetPos (4);

	m_cbWinMode.ResetContent ();
	m_cbWinMode.AddString ("Full Screen");
	m_cbWinMode.AddString ("Window");
	m_cbWinMode.SetCurSel (0);

	//-----------------------------------------------------------------------------
	HBITMAP hBitmap = NULL;		
	hBitmap = (HBITMAP) LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE( IDB_BITMAP1 ) );
    if ( hBitmap == NULL)
         hBitmap = (HBITMAP) LoadBitmap( NULL, "TriggerInfoDetector.bmp" );	
	
	if( hBitmap )
	{
		HRGN hRgn = CRgnBitmap::BitmapToRegion( hBitmap );
		this->SetWindowRgn( hRgn, true );		
	}

	DeleteObject( hBitmap );

	if( strcmp( g_CmdLine, "" ) == 0 )
	m_OkButton.SetWindowText( "Apply" );
	else
    m_OkButton.SetWindowText("Play");

	return TRUE;
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CTriggerDetectDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CTriggerDetectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTriggerDetectDlg::OnBnClickedRadio1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CTriggerDetectDlg::ListAdapter (void)
{
	m_cbMonitor.ResetContent ();

	if(m_pAdptInfo)
	{
		for(int i = 0; i < m_pAdptInfo->iAdapterCNT; i++)
		{
			m_cbMonitor.AddString (m_pAdptInfo->pAdapter[ i ].Description);
		}

		m_cbMonitor.SetCurSel (0);
		ListResnColor (0);
	}
}

int CTriggerDetectDlg::CheckDefaultRes (void)
{
	
	for(int i = m_cbResolution.GetCount () - 1; i >= 0 ; i--)
	{
		SystemResolution * pRes = (SystemResolution *)m_cbResolution.GetItemData (i);

		for(int j = 0; j < sizeof (g_defs) / sizeof (DefRes); j++)
		{
			if(pRes->pixel_width	== g_defs[ j ].iWidth && 
				pRes->pixel_height	== g_defs[ j ].iHeight&&
				pRes->pixel_color	== g_defs[ j ].iColorDetph)
			{
				return i;
			}
		}
	}

	return -1;
}

void CTriggerDetectDlg::ListResnColor (int iAdptIDX)
{
	bool bSelDef = false;
	int iSelDef = -1;
	m_cbResolution.ResetContent ();

	if(iAdptIDX < m_pAdptInfo->iAdapterCNT)
	{
		SystemResolution *pResolution = m_pAdptInfo->pAdapter[ iAdptIDX ].pResolution;
		int iResCNT = m_pAdptInfo->pAdapter[ iAdptIDX ].nResolution;

		if(iResCNT > 0)
		{
			char szResnColor[ 255 ];
			int iWidth	= pResolution[ 0 ].pixel_width;
			int iHeight = pResolution[ 0 ].pixel_height;
			int iColor	= pResolution[ 0 ].pixel_color;

			sprintf (szResnColor, "%d * %d (%dbit Color)", iWidth, iHeight, iColor);
			m_cbResolution.AddString (szResnColor);
			m_cbResolution.SetItemData (0, (DWORD_PTR)&pResolution[ 0 ]);

			for(int i = 1; i < iResCNT; i++)
			{
				if(
					iWidth	!= pResolution[ i ].pixel_width ||
					iHeight != pResolution[ i ].pixel_height ||
					iColor	!= pResolution[ i ].pixel_color
				)
				{
					iWidth	= pResolution[ i ].pixel_width;
					iHeight = pResolution[ i ].pixel_height;
					iColor	= pResolution[ i ].pixel_color;

					sprintf (szResnColor, "%d x %d (%dbit Color)", iWidth, iHeight, iColor);
					int iItemIDX = m_cbResolution.AddString (szResnColor);
					m_cbResolution.SetItemData (iItemIDX, (DWORD_PTR)&pResolution[ i ]);
				}
			}

			iSelDef = CheckDefaultRes ();
			if(iSelDef >= 0)
			{
				m_cbResolution.SetCurSel (iSelDef);
				ListFreq (iAdptIDX, iSelDef);
			}
			else
			{
				m_cbResolution.SetCurSel (0);
				ListFreq (iAdptIDX, 0);
			}
		}
	}
}

void CTriggerDetectDlg::ListFreq (int iAdptIDX, int iResComboIDX)
{
	m_cbFreq.ResetContent ();
	m_cbFreq.EnableWindow (true);
	
	if(iAdptIDX < m_pAdptInfo->iAdapterCNT)
	{
		SystemResolution *pResolution = m_pAdptInfo->pAdapter[ iAdptIDX ].pResolution;
		int iResCNT = m_pAdptInfo->pAdapter[ iAdptIDX ].nResolution;

		if(iResComboIDX < iResCNT)
		{
			SystemResolution * pItemData = (SystemResolution *)m_cbResolution.GetItemData (iResComboIDX);

			int iWidth	= pItemData->pixel_width;
			int iHeight = pItemData->pixel_height;
			int iColor = pItemData->pixel_color;

			char szFreq[ 255 ];
			int iAddCNT = 0;
			for(int i = 0; i < iResCNT; i++)
			{
				if(
					iWidth	== pResolution[ i ].pixel_width &&
					iHeight == pResolution[ i ].pixel_height &&
					iColor	== pResolution[ i ].pixel_color
				)
				{
					if(pResolution[ i ].frequency > 0)
					{
						sprintf (szFreq, "%dMHz", pResolution[ i ].frequency);
						int iItemIDX = m_cbFreq.AddString (szFreq);
						m_cbFreq.SetItemData (iItemIDX, (DWORD_PTR)pResolution[ i ].frequency);

						iAddCNT++;
					}
				}
			}

			if(iAddCNT > 0)
				m_cbFreq.SetCurSel (0);
			else
				m_cbFreq.EnableWindow (false);
		}
	}
}


void CTriggerDetectDlg::OnClose()
{
	if(m_pAdptInfo)
	{
		TI_ReleaseAdptInfo ();
		m_pAdptInfo = NULL;
	}

	CDialog::OnClose();
}

void CTriggerDetectDlg::On_Adpt_SelChange()
{
	int iSel = m_cbMonitor.GetCurSel ();

	if(iSel >= 0)
		ListResnColor (iSel);
}

void CTriggerDetectDlg::On_ResnColor_SelChange()
{
	int iSelAdpt		= m_cbMonitor.GetCurSel ();
	int iSelResnColor	= m_cbResolution.GetCurSel ();

	if(iSelAdpt >= 0 && iSelResnColor >= 0)
	{
		ListFreq (iSelAdpt, iSelResnColor);
	}
}


/********************************************************************
 *
 * "Ȯ��" ��ư ������ ��
 *
 */
void CTriggerDetectDlg::OnBnClickedOk()
{
	int iResWidth	= 0;
	int iHeight		= 0;
	int iColorBits	= 0;
	int	iFreq		= 0;

	int iAdapterIDX = m_cbMonitor.GetCurSel ();
	int iResnColorIDX = m_cbResolution.GetCurSel ();
	int iFreqIDX = m_cbFreq.GetCurSel ();
	SystemResolution *pResolution = (SystemResolution *)m_cbResolution.GetItemData (iResnColorIDX);
	if(pResolution)
	{
		// �ػ� - ��
		iResWidth	= pResolution->pixel_width;
		// �ػ� - ����
		iHeight		= pResolution->pixel_height;
		// �ػ� - �÷���Ʈ��
		iColorBits	= pResolution->pixel_color;
	}

	if(iFreqIDX >= 0)
	{
		// ���ļ�
		iFreq = (int)m_cbFreq.GetItemData (iFreqIDX);
	}
	
	if(iFreq <= 0)
		iFreqIDX = 60;

	// ������ ��� : ��ü : 0, â��� : 1
	int iWinModeIDX = m_cbWinMode.GetCurSel ();
	iWinModeIDX = iWinModeIDX >= 0 ? iWinModeIDX : 0;
		
	// �����ս� ���� : 1 ~ 5
	int iPerformance = m_sldPerform.GetPos ();

	// ��ó�� Ŭ���̾�Ʈ���� �Ѿ���� Ŀ�ǵ� ������ �Ʒ��� ����Ǿ� ����.
	// g_CmdLine 

	// ���⼭ Ŭ���̾�Ʈ ���� ���� �ּ���. �� ��.
/// ���� nAvy �߰� 	2004/8/2
	char	szTemp[128];
	const   char	g_szIniFileName[] = "./SevenHearts.ini";

	///����
	itoa( iPerformance - 1, szTemp, 10 );
	WritePrivateProfileString("VIDEO","PERFORMANCE", szTemp,g_szIniFileName );

	///Ǯ��ũ��
	int iFullScreen = (iWinModeIDX == 0)? 1: 0;
	itoa( iFullScreen, szTemp, 10 );
	WritePrivateProfileString("VIDEO","FULLSCREEN", szTemp,g_szIniFileName );

	///�ػ�
	itoa( iResWidth, szTemp, 10 );
	WritePrivateProfileString("RESOLUTION", "WIDTH", szTemp, g_szIniFileName );

	itoa( iHeight, szTemp, 10 );
	WritePrivateProfileString("RESOLUTION", "HEIGHT", szTemp, g_szIniFileName );

	itoa( iColorBits, szTemp, 10 );
	WritePrivateProfileString("RESOLUTION", "DEPTH", szTemp, g_szIniFileName );

	///���ļ�
	itoa( iFreq, szTemp, 10 );
	WritePrivateProfileString( "RESOLUTION","FREQUENCY", szTemp, g_szIniFileName );

	///ADAPTER
	itoa( iAdapterIDX, szTemp, 10 );
	WritePrivateProfileString( "VIDEO", "ADAPTER", szTemp, g_szIniFileName );


	///
	itoa( 1, szTemp, 10 );
	WritePrivateProfileString( "TRIGGER","DETECTED", szTemp, g_szIniFileName );
	OnOK ();

	if(g_CmdLine && strlen (g_CmdLine) > 1 )
		ShellExecute( NULL, NULL, "TRose.exe", g_CmdLine, NULL, SW_SHOWNORMAL );
	else
	{
		ShellExecute( NULL, NULL, "ROSEonline.exe",NULL,NULL, SW_SHOWNORMAL );
		//MessageBox("Execute ROSEonline.exe.",NULL,MB_OK);
	
	}

}
LRESULT CTriggerDetectDlg::OnNcHitTest(CPoint point)
{
	

	return HTCAPTION;//CDialog::OnNcHitTest(point);
}
