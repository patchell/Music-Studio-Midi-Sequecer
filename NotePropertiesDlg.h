#pragma once

// CNotePropertiesDlg dialog

class CNotePropertiesDlg : public CDialog
{

	inline static int PitchToNote[12] = {
		0,0,1,1,2,3,3,4,4,5,5,6
	};

	inline static int NoteToPitch[7] = {
		0,2,4,5,7,9,11
	};
	//---------------------------------
	// Controls
	//---------------------------------
	CButton m_Check_Accent;
	CButton m_Check_Rest;
	CMyScrollBar m_SB_Octave;
	CMyScrollBar m_SB_Pitch;
	CStatic m_Static_Octave;
	CStatic m_Static_Pitch;
	CMyComboBox m_Combo_Duration;
	CMyComboBox m_Combo_Instrument;
	//------------ Data -------------------
	int m_Radio_None;
	int m_Radio_Accedental;

	CMsNote* m_pNote;

	DECLARE_DYNAMIC(CNotePropertiesDlg)
public:
	//---------------------------------
	// Construction/Destruction
	//---------------------------------
	CNotePropertiesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNotePropertiesDlg();
	CMsTrack* GetTrackInfo(int Track);
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
	virtual void OnOK();
	void SetNoteToEdit(CMsNote* pN) { m_pNote = pN; }
};
