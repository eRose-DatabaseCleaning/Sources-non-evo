// TriggerDetectDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "TriggerInfo.h"
#include "afxcmn.h"

// CTriggerDetectDlg ��ȭ ����
class CTriggerDetectDlg : public CDialog
{
// ����
public:
	CTriggerDetectDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_TRIGGERDETECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����


// ����
protected:
	HICON m_hIcon;

	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedOk();
private:
	CComboBox m_cbMonitor;
	CComboBox m_cbResolution;
	CComboBox m_cbFreq;

	AdapterInformation * m_pAdptInfo;

	void ListAdapter (void);
	void ListResnColor (int iAdptIDX);
	void ListFreq (int iAdptIDX, int iResIDX);
public:
	afx_msg void OnClose();
	afx_msg void On_Adpt_SelChange();
	afx_msg void On_ResnColor_SelChange();
private:
	CSliderCtrl m_sldPerform;
	int m_rdFullScreen;
	int m_rdWindow;
public:
	afx_msg void OnBnHotItemChangeFullscreen(NMHDR *pNMHDR, LRESULT *pResult);

private:
	CComboBox m_cbWinMode;
	int CheckDefaultRes (void);
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CButton m_OkButton;
};

/**********************************************************************
 * ����Ʈ Resolution �� ��� ���ؼ�...
 */
struct DefRes
{
	int iWidth, iHeight, iColorDetph;
};