// DlgMidiInfo.cpp : implementation file
//

#include "pch.h"


// CDlgMidiInfo dialog

IMPLEMENT_DYNAMIC(CDlgMidiInfo, CDialogEx)

CDlgMidiInfo::CDlgMidiInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INSTUMENT_MIDI, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_CurrentInstrument = 1;
}

CDlgMidiInfo::~CDlgMidiInfo()
{
}

void CDlgMidiInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TRACK, m_Static_InstBitmap);
	DDX_Control(pDX, IDC_VSB_INST_SELECT, m_SB_TrackSelection);
	DDX_Control(pDX, IDC_HSB_PATCH, m_SB_Patch_Selection);
	DDX_Control(pDX, IDC_HSB_RANGE, m_SB_RangeSelection);
	DDX_Control(pDX, IDC_HSB_CHANNEL, m_SB__MidiChannel);
	DDX_Control(pDX, IDC_CHANNEL_NAME, m_Edit_ChannelName);
	DDX_Control(pDX, IDC_CHECK_GROUP_FOUR, m_Check_GroupFour);
	DDX_Control(pDX, IDC_CHECK_GROUP_ONE, m_Check_GroupOne);
	DDX_Control(pDX, IDC_CHECK_GROUP_THREE, m_Check_GroupThree);
	DDX_Control(pDX, IDC_CHECK_GROUP_TWO, m_Check_GroupTwo);
	DDX_Control(pDX, IDC_COMBO_MDIDEVICE, m_Combo_MidiDevice);
	DDX_Control(pDX, IDC_EDIT_TITLE, m_Edit_SongTitle);
	DDX_Control(pDX, IDC_STATIC_PATCH, m_Static_Patch);
	DDX_Control(pDX, IDC_STATIC_PATCHVALUE, m_Static_PatchValue);
	DDX_Control(pDX, IDC_STATIC_RANGE, m_Staticf_Range);
	DDX_Control(pDX, IDC_STATIC_RANGEVALUE, m_Static_RangeValue);
	DDX_Control(pDX, IDC_STATIC_CHANNEL_VALUE, m_Static_ChannelValue);
}


BEGIN_MESSAGE_MAP(CDlgMidiInfo, CDialogEx)
	ON_MESSAGE(WM_MYSCROLLBAR_MSG, &CDlgMidiInfo::OnMyscrollbarMsg)
	ON_STN_CLICKED(IDC_STATIC_TRACK, &CDlgMidiInfo::OnClickedStaticTrack)
END_MESSAGE_MAP()


// CDlgMidiInfo message handlers


BOOL CDlgMidiInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Static_InstBitmap.Init();

	m_SB_Patch_Selection.SetScrollRange(0, 127);
	m_SB_Patch_Selection.SetWindowMessage(this, WM_MYSCROLLBAR_MSG);

	m_SB_RangeSelection.SetScrollRange(1, 5);
	m_SB_RangeSelection.SetWindowMessage(this, WM_MYSCROLLBAR_MSG);

	m_SB__MidiChannel.SetScrollRange(1, 16);
	m_SB__MidiChannel.SetWindowMessage(this, WM_MYSCROLLBAR_MSG);

	m_SB_TrackSelection.SetScrollRange(1, 15);
	m_SB_TrackSelection.SetWindowMessage(this, WM_MYSCROLLBAR_MSG);
	m_SB_TrackSelection.SetScrollPos(1);

	int nMidiDevices = GETMIDIOUTTABLE.GetNumDevices();
	for (int i = 0; i < nMidiDevices; ++i)
	{
		m_Combo_MidiDevice.AddString(GETMIDIOUTDEVICE(i).csGetName());
	}
	m_Combo_MidiDevice.SetCurSel(0);

	m_Edit_SongTitle.SetWindowTextW(CString(GETMIDIINFO->GetTitle()));;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMidiInfo::UpdateControls(int InstID)
{
	char* s = new char[256];

	m_Static_InstBitmap.SetColor(GETMIDIINFO->GetInstColor(InstID));

	m_SB_Patch_Selection.SetScrollPos(GETMIDIINFO->GetPatch(InstID));
	m_SB_RangeSelection.SetScrollPos(GETMIDIINFO->GetRange(InstID));
	m_SB__MidiChannel.SetScrollPos(GETMIDIINFO->GetChannel(InstID) + 1);

	m_Combo_MidiDevice.SetCurSel(GETTRACKINFO(InstID).GetMidiOutDeviceID());
	delete[] s;
}


afx_msg LRESULT CDlgMidiInfo::OnMyscrollbarMsg(WPARAM ControlID, LPARAM Position)
{
	CString csString;

	switch (ControlID)
	{
	case IDC_VSB_INST_SELECT:
		csString.Format(_T("%d"), Position);
		m_Static_InstBitmap.SetTextString(csString);
		m_CurrentInstrument = Position;
		UpdateControls(m_CurrentInstrument);
		break;
	case IDC_HSB_PATCH:
		GETMIDIINFO->GetTrack(m_CurrentInstrument).SetPatch(Position);
		csString.Format(_T("%d"), Position);
		m_Static_PatchValue.SetWindowTextW(csString);
		break;
	case IDC_HSB_RANGE:
		GETMIDIINFO->GetTrack(m_CurrentInstrument).SetPitchRange(Position);
		csString.Format(_T("%d"), Position);
		m_Static_RangeValue.SetWindowTextW(csString);
		break;
	case IDC_HSB_CHANNEL:
		GETMIDIINFO->GetTrack(m_CurrentInstrument).SetChanel(Position - 1);
		csString.Format(_T("%d"), Position);
		m_Static_ChannelValue.SetWindowTextW(csString);
		break;
	}
	return 0;
}

void CDlgMidiInfo::OnClickedStaticTrack()
{
	CColorDialog dlg(GETMIDIINFO->GetInstColor(m_CurrentInstrument),0,this);

	if (IDOK == dlg.DoModal())
	{
		GETMIDIINFO->SetInstColor(m_CurrentInstrument, dlg.GetColor());
		m_Static_InstBitmap.SetColor(dlg.GetColor());
	}
}

void CDlgMidiInfo::OnOK()
{
	CString cs;
	char* s = new char[256];
	
	m_Edit_SongTitle.GetWindowTextW(cs);
	GETMIDIINFO->SetTitle(WcharToChar(s, cs.GetBuffer(), cs.GetLength()));
	CDialogEx::OnOK();
}
