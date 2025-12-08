#pragma once


// CDlgMidiInfo dialog

class CDlgMidiInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMidiInfo)
	int m_CurrentInstrument;

	CStaticBitmap m_Static_InstBitmap;
	CMyScrollBar m_SB_TrackSelection;

	CMyScrollBar m_SB_Patch_Selection;
	CStatic m_Static_Patch;
	CStatic m_Static_PatchValue;

	CMyScrollBar m_SB_RangeSelection;
	CStatic m_Staticf_Range;
	CStatic m_Static_RangeValue;

	CMyScrollBar m_SB__MidiChannel;
	CEdit m_Edit_ChannelName;
	CStatic m_Static_ChannelValue;

	CButton m_Check_GroupOne;
	CButton m_Check_GroupTwo;
	CButton m_Check_GroupThree;
	CButton m_Check_GroupFour;

	CComboBox m_Combo_MidiDevice;

	CEdit m_Edit_SongTitle;

public:
	CDlgMidiInfo(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgMidiInfo();
	virtual BOOL OnInitDialog();
	void UpdateControls(int InstID);
	virtual void OnOK();

	enum { IDD = IDD_DIALOG_INSTUMENT_MIDI };

protected:
	afx_msg LRESULT OnMyscrollbarMsg(WPARAM wParam, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnClickedStaticTrack();

	DECLARE_MESSAGE_MAP()
};
