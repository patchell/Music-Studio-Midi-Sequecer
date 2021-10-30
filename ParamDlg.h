#pragma once

// ParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParamDlg dialog

class CParamDlg : public CDialog
{
	CString m_csCaption;
	int m_Val;
	int m_Max;
	int m_Min;
	CStatic	m_Static_Param;
	CMyScrollBar	m_ScrollBar_Param;
	// Construction
public:
	CParamDlg(CWnd* pParent = NULL);   // standard constructor
	enum { IDD = IDD_DIALOG_PARAMETER };
	void SetCaption(CString Caption) { m_csCaption = Caption; }
	void SetMax(int max) { m_Max = max; }
	void SetMin(int min) { m_Min = min; }
	void SetValue(int val) { m_Val = val; }
	int GetValue() { return m_Val; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// Generated message map functions
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

