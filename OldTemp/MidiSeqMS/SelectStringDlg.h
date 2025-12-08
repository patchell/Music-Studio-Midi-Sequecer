#pragma once


// CSelectStringDlg dialog

class CSelectStringDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectStringDlg)
	CComboBox m_Combo_Selector;
	CString* m_pcsStrings;
	int m_nStrings;
	int m_nSelection;
	CString m_csCaptionString;
public:
	CSelectStringDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectStringDlg();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	int GetSelection() { return m_nSelection; }
	void SetNumStrings(int n) { 
		m_nStrings = n; 
		m_pcsStrings = new CString[n];
	}
	void SetCaptionString(CString csCap) { m_csCaptionString = csCap; }
	void SetSelectionString(int index, CString &csStr) {
		m_pcsStrings[index] = csStr;
	}
	CString& GetSelectionString(int index) { return m_pcsStrings[index]; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STRINGSELECTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
