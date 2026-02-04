#pragma once

// CSelectorDlg dialog

class CSelectorDlg : public CDialog
{
	CMyComboBox m_Combo_Selector;
	CMyBitmap** m_ppBitmaps;
	int m_nBitmaps;
	int m_nSelection;
	CString m_csCaption;
	DECLARE_DYNAMIC(CSelectorDlg)
public:
	CSelectorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectorDlg();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//------------------------------
	void SetCaption(CString csCap) { m_csCaption = csCap; }
	void SetBitmaps(CMyBitmap** ppBMs) { m_ppBitmaps = ppBMs; }
	void SetSelection(int n) { m_nSelection = n; }
	int GetSelection() { return m_nSelection; }
	void SetNumberOfBitmaps(int n) { m_nBitmaps = n; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELECTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
