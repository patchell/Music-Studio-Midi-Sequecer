#pragma once


// CDlgMidiInDispatchEditor dialog

#define DISPATCHEDIT__NUM_MIDI_CMDS		23

extern MIDI_STATUS MidiStatusCommands[DISPATCHEDIT__NUM_MIDI_CMDS];

class CDlgMidiInDispatchEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgMidiInDispatchEditor)
	CMidiInDevice* m_MID;
	CMidiInMessageDispatcher* m_pMIMD;
	int m_DeviceID;
	//--------------- Controls -------------------------
	CComboBox m_Combo_DestinationWindow;
	CComboBox m_Combo_WM_DispatchCommand;
	CComboBox m_Combo_MidiChannel;
	CComboBox m_Combo_MidiStatusByte;
	CEdit m_Edit_Data1_High;
	CEdit m_Edit_Data1_Low;
	CEdit m_Edit_Data2_High;
	CEdit m_Edit_Data2_Low;
	CStatic m_Static_Data1;
	CStatic m_Static_Data2;
	CStatic m_static_Channel;
	CStatic m_static_Command;
	CStatic m_static_HighValue;
	CStatic m_Static_LowValue;
	CStatic m_static_StatusByte;
	CEdit m_Edit_Name;
	CButton m_Button_Prev;
	CButton m_Button_Next;
	CButton m_Button_Delete;

	CChildViewBase** m_ppChildViews;
public:
	CDlgMidiInDispatchEditor(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgMidiInDispatchEditor();
	virtual BOOL OnInitDialog();
	void InitializeMidiChannelComboBox();
	void InitializeMidiStatusByteComboBox();
	void SetDeviceID(int Id) { m_DeviceID = Id; }
	void UpdateDialogFields();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIDI_IN_DISPATCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPrevDispatch();
	afx_msg void OnBnClickedButtonDeletedispatcher();
	afx_msg void OnBnClickedButtonNewdispatcher();
	afx_msg void OnBnClickedButtonNextDispatch();
	afx_msg void OnCbnSelchangeComboDestWindow();
	afx_msg void OnCbnSelchangeComboDispatchCommand();
	afx_msg void OnCbnSelchangeComboDispatchStatus();
	afx_msg void OnCbnSelchangeComboDispatchMiduchannel();
	virtual void OnOK();
	afx_msg void OnKillfocusEditDispatchDataoneHigh();
	afx_msg void OnKillfocusEditDispatchDataoneLow();
	afx_msg void OnKillfocusEditDispatchDatatwoHigh();
	afx_msg void OnKillfocusEditDispatchDatatwoLow();
	afx_msg void OnKillfocusEditDispatchName();
	void UpdateDataEditBoxes(int statusID);
	void EnableCommand(int enable);
	void EnableChannel(int enable);
	void EnableStatusByte(int enable);
	afx_msg void OnCbnKillfocusComboDestWindow();
	afx_msg void OnCbnKillfocusComboDispatchCommand();
	afx_msg void OnCbnKillfocusComboDispatchStatus();
	afx_msg void OnCbnKillfocusComboDispatchMiduchannel();
	void CreateANewDispatcher();
};
