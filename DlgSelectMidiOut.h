#pragma once


// CDlgSelectMidiOut dialog

class CDlgSelectMidiOut : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectMidiOut)
	CComboBox m_Combo_MidiOut;
	CString* m_pcsNames;
	int m_Selection;
	bool m_Instruments[16];
	CStaticCheckBitmap m_Static_Check1;
	CStaticCheckBitmap m_Static_Check2;
	CStaticCheckBitmap m_Static_Check3;
	CStaticCheckBitmap m_Static_Check4;
	CStaticCheckBitmap m_Static_Check5;
	CStaticCheckBitmap m_Static_Check6;
	CStaticCheckBitmap m_Static_Check7;
	CStaticCheckBitmap m_Static_Check8;
	CStaticCheckBitmap m_Static_Check9;
	CStaticCheckBitmap m_Static_Check10;
	CStaticCheckBitmap m_Static_Check11;
	CStaticCheckBitmap m_Static_Check12;
	CStaticCheckBitmap m_Static_Check13;
	CStaticCheckBitmap m_Static_Check14;
	CStaticCheckBitmap m_Static_Check15;
	CStaticCheckBitmap* m_aChecks;
public:
	CDlgSelectMidiOut(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSelectMidiOut();
	virtual BOOL OnInitDialog();
	void OnOK();
	int GetSelection() { return m_Selection; }
	CString& GetNameString() { return m_pcsNames[m_Selection]; }
	bool GetInstrumentSel(int i) {
		return m_Instruments[i];
	}
	enum { IDD = IDD_DIALOG_MIDIOUT };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboMidioutSelect();
	afx_msg void OnClickedStaticMosBm1();
	afx_msg void OnClickedStaticMosBm2();
	afx_msg void OnClickedStaticMosBm3();
	afx_msg void OnClickedStaticMosBm4();
	afx_msg void OnClickedStaticMosBm5();
	afx_msg void OnClickedStaticMosBm6();
	afx_msg void OnClickedStaticMosBm7();
	afx_msg void OnClickedStaticMosBm8();
	afx_msg void OnClickedStaticMosBm9();
	afx_msg void OnClickedStaticMosBm10();
	afx_msg void OnClickedStaticMosBm11();
	afx_msg void OnClickedStaticMosBm12();
	afx_msg void OnClickedStaticMosBm13();
	afx_msg void OnClickedStaticMosBm14();
	afx_msg void OnClickedStaticMosBm15();
};
