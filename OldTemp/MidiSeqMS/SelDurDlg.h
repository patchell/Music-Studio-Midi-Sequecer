#pragma once

// SelDurDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSelDurDlg dialog

class CSelDurDlg : public CDialog
{
	CMyBitmap m_BmWhole;
	CMyBitmap m_BmHalf;
	CMyBitmap m_BmQuarter;
	CMyBitmap m_BmEight;
	CMyBitmap m_BmSixteenth;
	CMyBitmap m_BmThirtySec;
// Construction
public:
	int m_nDurSel;
	const char * m_pPrompt;
	CSelDurDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelDurDlg)
	enum { IDD = IDD_DIALOG_SELECT_DURATION };
	CStatic	m_Static_Promt;
	CMyComboBox	m_Combo_SelDur;
	int		m_Radio_Sel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelDurDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelDurDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboSelDuratuion();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_nDuration;
};

