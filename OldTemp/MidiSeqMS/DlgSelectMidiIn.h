#pragma once


// CDlgSelectMidiIn dialog

class CDlgSelectMidiIn : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSelectMidiIn)
	char** m_ppNames;
	CComboBox m_Combo_MidiInSelect;
	int m_Selection;
public:
	CDlgSelectMidiIn(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSelectMidiIn();
	virtual BOOL OnInitDialog();
	int GetSelection() { return m_Selection; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELECT_MIDI_IN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnComboSelector();
	afx_msg void OnSelchangeComboSelectMidiIn();
};
