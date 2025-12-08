// DlgSelectMidiOut.cpp : implementation file
//

#include "pch.h"


// CDlgSelectMidiOut dialog

IMPLEMENT_DYNAMIC(CDlgSelectMidiOut, CDialog)

CDlgSelectMidiOut::CDlgSelectMidiOut(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_MIDIOUT, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	for (int i = 0; i < 16; ++i)m_Instruments[i] = 0;
	m_Selection = 0;
	m_aChecks = &m_Static_Check1;
	m_pcsNames = 0;
}

CDlgSelectMidiOut::~CDlgSelectMidiOut()
{
	delete[] m_pcsNames;
}

void CDlgSelectMidiOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MIDIOUT_SELECT, m_Combo_MidiOut);
	DDX_Control(pDX, IDC_STATIC_MOS_BM1, m_Static_Check1);
	DDX_Control(pDX, IDC_STATIC_MOS_BM2, m_Static_Check2);
	DDX_Control(pDX, IDC_STATIC_MOS_BM3, m_Static_Check3);
	DDX_Control(pDX, IDC_STATIC_MOS_BM4, m_Static_Check4);
	DDX_Control(pDX, IDC_STATIC_MOS_BM5, m_Static_Check5);
	DDX_Control(pDX, IDC_STATIC_MOS_BM6, m_Static_Check6);
	DDX_Control(pDX, IDC_STATIC_MOS_BM7, m_Static_Check7);
	DDX_Control(pDX, IDC_STATIC_MOS_BM8, m_Static_Check8);
	DDX_Control(pDX, IDC_STATIC_MOS_BM9, m_Static_Check9);
	DDX_Control(pDX, IDC_STATIC_MOS_BM10, m_Static_Check10);
	DDX_Control(pDX, IDC_STATIC_MOS_BM11, m_Static_Check11);
	DDX_Control(pDX, IDC_STATIC_MOS_BM12, m_Static_Check12);
	DDX_Control(pDX, IDC_STATIC_MOS_BM13, m_Static_Check13);
	DDX_Control(pDX, IDC_STATIC_MOS_BM14, m_Static_Check14);
	DDX_Control(pDX, IDC_STATIC_MOS_BM15, m_Static_Check15);
}


BEGIN_MESSAGE_MAP(CDlgSelectMidiOut, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_MIDIOUT_SELECT, &CDlgSelectMidiOut::OnCbnSelchangeComboMidioutSelect)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM1, &CDlgSelectMidiOut::OnClickedStaticMosBm1)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM2, &CDlgSelectMidiOut::OnClickedStaticMosBm2)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM3, &CDlgSelectMidiOut::OnClickedStaticMosBm3)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM4, &CDlgSelectMidiOut::OnClickedStaticMosBm4)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM5, &CDlgSelectMidiOut::OnClickedStaticMosBm5)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM6, &CDlgSelectMidiOut::OnClickedStaticMosBm6)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM7, &CDlgSelectMidiOut::OnClickedStaticMosBm7)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM8, &CDlgSelectMidiOut::OnClickedStaticMosBm8)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM9, &CDlgSelectMidiOut::OnClickedStaticMosBm9)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM10, &CDlgSelectMidiOut::OnClickedStaticMosBm10)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM11, &CDlgSelectMidiOut::OnClickedStaticMosBm11)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM12, &CDlgSelectMidiOut::OnClickedStaticMosBm12)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM13, &CDlgSelectMidiOut::OnClickedStaticMosBm13)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM14, &CDlgSelectMidiOut::OnClickedStaticMosBm14)
	ON_STN_CLICKED(IDC_STATIC_MOS_BM15, &CDlgSelectMidiOut::OnClickedStaticMosBm15)
END_MESSAGE_MAP()


// CDlgSelectMidiOut message handlers


BOOL CDlgSelectMidiOut::OnInitDialog()
{
	CDialog::OnInitDialog();

	UINT nMidiOutDevs = midiOutGetNumDevs();
	MIDIOUTCAPS MidiOut;

	m_pcsNames = new CString[nMidiOutDevs];
	UINT i, l = 0;
	for (i = 0; i < nMidiOutDevs; ++i)
	{
		midiOutGetDevCaps(i, &MidiOut, sizeof(MIDIOUTCAPS));
		m_pcsNames[i] = CString(MidiOut.szPname);
		m_Combo_MidiOut.InsertString(i, m_pcsNames[i]);

	}
	m_Combo_MidiOut.SetCurSel(0);
	for (i = 0; i < 15; ++i)
	{
		m_aChecks[i].SetTrackID(i + 1);
		m_aChecks[i].SetColor(GETMIDIINFO->GetTrack(i+1).GetColor());
	}
	return TRUE;  // return true unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return false
}

void CDlgSelectMidiOut::OnOK()
{
	CDialog::OnOK();
}

void CDlgSelectMidiOut::OnCbnSelchangeComboMidioutSelect()
{
	m_Selection = m_Combo_MidiOut.GetCurSel();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm1()
{
	m_Instruments[1] = m_Static_Check1.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm2()
{
	m_Instruments[2] = m_Static_Check2.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm3()
{
	m_Instruments[3] = m_Static_Check3.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm4()
{
	m_Instruments[4] = m_Static_Check4.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm5()
{
	m_Instruments[5] = m_Static_Check5.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm6()
{
	m_Instruments[6] = m_Static_Check6.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm7()
{
	m_Instruments[7] = m_Static_Check7.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm8()
{
	m_Instruments[8] = m_Static_Check8.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm9()
{
	m_Instruments[9] = m_Static_Check9.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm10()
{
	m_Instruments[10] = m_Static_Check10.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm11()
{
	m_Instruments[11] = m_Static_Check11.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm12()
{
	m_Instruments[12] = m_Static_Check12.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm13()
{
	m_Instruments[13] = m_Static_Check13.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm14()
{
	m_Instruments[14] = m_Static_Check14.GetCheck();
}


void CDlgSelectMidiOut::OnClickedStaticMosBm15()
{
	m_Instruments[15] = m_Static_Check15.GetCheck();
}
