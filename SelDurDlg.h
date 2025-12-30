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
	CStatic	m_Static_Promt;
	CMyComboBox	m_Combo_SelDur;
public:
	int	m_Radio_Sel;
	CMsNote::NoteShape m_nDurSel;
	int m_nDuration;
	const char* m_pPrompt;
public:
	enum { IDD = IDD_DIALOG_SELECT_DURATION };
	CSelDurDlg(CWnd* pParent = NULL);   // standard constructor
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboSelDuratuion();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

