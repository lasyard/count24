
// count24Dlg.h : header file
//

#pragma once
#include "afxwin.h"

#define NUM_NUMBER 4
#define MIN_NUMBER 1
#define MAX_NUMBER 13

#define TARGET_NUMBER 24

// Ccount24Dlg dialog
class Ccount24Dlg : public CDialog
{
// Construction
public:
	Ccount24Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_COUNT24_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	HICON m_hIconNums[MAX_NUMBER-MIN_NUMBER+1];

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int num1;
	int num2;
	int num3;
	int num4;
	int result;
	CButton btnCard1;
	CButton btnCard2;
	CButton btnCard3;
	CButton btnCard4;
	CListBox lstSolution;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnCard1();
	afx_msg void OnBnClickedBtnCard2();
	afx_msg void OnBnClickedBtnCard3();
	afx_msg void OnBnClickedBtnCard4();
};
