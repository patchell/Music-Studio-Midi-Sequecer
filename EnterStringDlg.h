#pragma once


// CEnterStringDlg dialog

class CEnterStringDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnterStringDlg)
	CString m_S;
	CEdit m_Edit_String;
public:
	CEnterStringDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CEnterStringDlg();
	inline void SetString(CString& str) { m_S = str; }
	inline CString GetString(void) { return m_S; }
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
