#pragma once


// CEnterStringDlg dialog

class CEnterStringDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnterStringDlg)
	CString m_csS;		//String to Edit
	CString m_csTitle;	//Dialog Title
	CEdit m_Edit_String;
public:
	CEnterStringDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CEnterStringDlg();
	void SetTitle(CString& str) { m_csTitle = str; }
	void SetString(CString& str) { m_csS = str; }
	CString GetString(void) { return m_csS; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ENTERSTRING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
