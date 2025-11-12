// NotePropertiesDlg.cpp : implementation file
//

#include "pch.h"

const char *NoteLut[8] = {
	"C",
	"D",
	"E",
	"F",
	"G",
	"A",
	"B"
};

static void PitchFormat(void *p, char *s, int l, int val, int aux)
{
	sprintf_s(s, l, "%s", NoteLut[val]);
}

// CNotePropertiesDlg dialog

IMPLEMENT_DYNAMIC(CNotePropertiesDlg, CDialog)

CNotePropertiesDlg::CNotePropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_NOTEPROPERTIES, pParent)
	, m_Radio_None(0)
	, m_Radio_Accedental(0)
	, m_pNote(0)
{

}

CNotePropertiesDlg::~CNotePropertiesDlg()
{
}

void CNotePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ACCENT, m_Check_Accent);
	DDX_Control(pDX, IDC_CHECK_REST, m_Check_Rest);
	DDX_Control(pDX, IDC_SCROLLBAR_OCTAVE, m_SB_Octave);
	DDX_Control(pDX, IDC_SCROLLBAR_PTICH, m_SB_Pitch);
	DDX_Control(pDX, IDC_STATIC_OCTAVE, m_Static_Octave);
	DDX_Control(pDX, IDC_STATIC_PITCH, m_Static_Pitch);
	//  DDX_Control(pDX, IDC_RADIO_NONE, m_Radio_None);
	DDX_Radio(pDX, IDC_RADIO_NONE, m_Radio_None);
	DDV_MinMaxInt(pDX, m_Radio_None, 0, 2);
	DDX_Radio(pDX, IDC_RADIO_ACCEDENTAL, m_Radio_Accedental);
	DDX_Control(pDX, IDC_COMBO_DURATION, m_Combo_Duration);
	DDX_Control(pDX, IDC_COMBO_DLG_INSTRUMENT, m_Combo_Instrument);
}


BEGIN_MESSAGE_MAP(CNotePropertiesDlg, CDialog)
	ON_COMMAND(IDC_RADIO_NONE, &CNotePropertiesDlg::OnRadioNone)
END_MESSAGE_MAP()


// CNotePropertiesDlg message handlers

int PitchToNote[12] = {
	0,0,1,1,2,3,3,4,4,5,5,6
};

int NoteToPitch[7] = {
	0,2,4,5,7,9,11
};

#define PROP_NONE	0
#define PROP_DOTTED	1
#define PROP_TRIPLET	2

BOOL CNotePropertiesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SB_Pitch.SetScrollRange(0, 7);
	m_SB_Pitch.SetDisplayFormatFunction(PitchFormat);
	m_SB_Pitch.SetDisplayText(&m_Static_Pitch);
	m_SB_Pitch.SetScrollPos(PitchToNote[m_pNote->GetPitch() % 12]);

	m_SB_Octave.SetScrollRange(3, 8);
	m_SB_Octave.SetDisplayText(&m_Static_Octave);
	m_SB_Octave.SetScrollPos(m_pNote->GetPitch() / 12);

	if (m_pNote->GetAccent())m_Check_Accent.SetCheck(1);
	if (m_pNote->IsRest())m_Check_Rest.SetCheck(1);
	if (m_pNote->IsTriplet()) m_Radio_None = PROP_TRIPLET;
	else if (m_pNote->IsDotted()) m_Radio_None = PROP_DOTTED;
	else
		m_Radio_None = PROP_NONE;
	m_Radio_Accedental = m_pNote->GetAccidental();
	UpdateData(false);	//Initialize Dialog Items

	for (int i = 1; i < 16; ++i)
		m_Combo_Instrument.AddBitmap(GETMIDIINFO->GetTrack(i).GetBitmap(), CString(""));
	m_Combo_Instrument.SetCurSel(m_pNote->GetTrack() - 1);

	m_Combo_Duration.AddBitmap(GETAPP->bmGetNoteType(BM_NOTE_INDEX_WHOLE), CString(""));
	m_Combo_Duration.AddBitmap(GETAPP->bmGetNoteType(BM_NOTE_INDEX_HALF), CString(""));
	m_Combo_Duration.AddBitmap(GETAPP->bmGetNoteType(BM_NOTE_INDEX_QUARTER), CString(""));
	m_Combo_Duration.AddBitmap(GETAPP->bmGetNoteType(BM_NOTE_INDEX_EIGTH), CString(""));
	m_Combo_Duration.AddBitmap(GETAPP->bmGetNoteType(BM_NOTE_INDEX_SIXTEENTH), CString(""));
	m_Combo_Duration.AddBitmap(GETAPP->bmGetNoteType(BM_NOTE_INDEX_THIRTYSECOND), CString(""));
	m_Combo_Duration.SetCurSel(DurTab[m_pNote->GetDuration()].NoteShapIndex);

	CString csCaption;
	CString csNote;

	m_pNote->ObjectToString(csNote);
	csCaption.Format(_T("Note Properties %lS ID=%d"), csNote.GetString(), m_pNote->GetObjectID());
	SetWindowText(csCaption);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CNotePropertiesDlg::OnRadioNone()
{
}

void CNotePropertiesDlg::OnOK()
{
	UpdateData(true);
	m_pNote->SetRest(m_Check_Rest.GetCheck());
	m_pNote->SetAccent(m_Check_Accent.GetCheck());
	int d = CMsNote::NoteDurLut[m_Combo_Duration.GetCurSel()];
	if (m_Radio_None == PROP_DOTTED && !m_Check_Rest.GetCheck())
		d += 2;
	else if (m_Radio_None == PROP_TRIPLET && !m_Check_Rest.GetCheck())
		d -= 2;
	m_pNote->SetDuration(d);
	m_pNote->SetTrack(m_Combo_Instrument.GetCurSel() + 1);
	int p = m_SB_Pitch.GetScrollPos();
	p = NoteToPitch[p];
	p += m_SB_Octave.GetScrollPos() * 12;
	m_pNote->SetPitch(p);
	m_pNote->SetAccidental(m_Radio_Accedental);
	CDialog::OnOK();
}
