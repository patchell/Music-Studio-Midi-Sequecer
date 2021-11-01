#pragma once


// CEnterStringDlg dialog

class CEnterStringDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnterStringDlg)
	CString m_csS;
	CEdit m_Edit_String;
public:
	CEnterStringDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CEnterStringDlg();
	inline void SetString(CString& str) { m_csS = str; }
	inline CString GetString(void) { return m_csS; }
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
