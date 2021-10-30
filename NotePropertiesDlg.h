#pragma once

// CNotePropertiesDlg dialog

class CNotePropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CNotePropertiesDlg)

public:
	//---------------------------------
	// Controls
	//---------------------------------
	CButton m_Check_Accent;
	CButton m_Check_Rest;
	CMyScrollBar m_SB_Octave;
	CMyScrollBar m_SB_Pitch;
	CStatic m_Static_Octave;
	CStatic m_Static_Pitch;
	//---------------------------------
	// Construction/Destruction
	//---------------------------------
	CNotePropertiesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNotePropertiesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NOTEPROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioNone();
//	CButton m_Radio_None;
	int m_Radio_None;
	virtual void OnOK();
	CMsNote *m_pNote;
	int m_Radio_Accedental;
	CMyComboBox m_Combo_Duration;
	CMyComboBox m_Combo_Instrument;
};
