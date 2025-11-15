// DlgSelectMidiIn.cpp : implementation file
//

#include "pch.h"


// CDlgSelectMidiIn dialog

IMPLEMENT_DYNAMIC(CDlgSelectMidiIn, CDialogEx)

CDlgSelectMidiIn::CDlgSelectMidiIn(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SELECT_MIDI_IN, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CDlgSelectMidiIn::~CDlgSelectMidiIn()
{
}

void CDlgSelectMidiIn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SELECT_MIDI_IN, m_Combo_MidiInSelect);
}


BEGIN_MESSAGE_MAP(CDlgSelectMidiIn, CDialogEx)
	ON_COMMAND(ID_COMBO_SELECTOR, &CDlgSelectMidiIn::OnComboSelector)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT_MIDI_IN, &CDlgSelectMidiIn::OnSelchangeComboSelectMidiIn)
END_MESSAGE_MAP()


// CDlgSelectMidiIn message handlers


BOOL CDlgSelectMidiIn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UINT nMidiInDevs = midiInGetNumDevs();
	MIDIINCAPS MidiIn;

	m_ppNames = new char* [nMidiInDevs];
	UINT i, l = 0;
	for (i = 0; i < nMidiInDevs; ++i)
	{
		midiInGetDevCaps(i, &MidiIn, sizeof(MIDIINCAPS));
		l = wcslen(MidiIn.szPname) + 1;
		m_ppNames[i] = new char[l];
		WcharToChar(m_ppNames[i], MidiIn.szPname, l);
		m_Combo_MidiInSelect.InsertString(i, MidiIn.szPname);

	}
	m_Combo_MidiInSelect.SetCurSel(0);

	return true;  // return true unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return false
}


void CDlgSelectMidiIn::OnComboSelector()
{
}


void CDlgSelectMidiIn::OnSelchangeComboSelectMidiIn()
{
	m_Selection = m_Combo_MidiInSelect.GetCurSel();
}
