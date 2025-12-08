// SelDurDlg.cpp : implementation file
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelDurDlg dialog


CSelDurDlg::CSelDurDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelDurDlg::IDD, pParent)
	, m_nDuration(0)
{
	//{{AFX_DATA_INIT(CSelDurDlg)
	m_Radio_Sel = -1;
	//}}AFX_DATA_INIT
	m_pPrompt = 0;
	m_nDurSel = 2;
}


void CSelDurDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelDurDlg)
	DDX_Control(pDX, IDC_STATIC_PROMT_DUR, m_Static_Promt);
	DDX_Control(pDX, IDC_COMBO_SEL_DURATUION, m_Combo_SelDur);
	DDX_Radio(pDX, IDC_RADIO_NONE, m_Radio_Sel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelDurDlg, CDialog)
	//{{AFX_MSG_MAP(CSelDurDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SEL_DURATUION, OnSelchangeComboSelDuratuion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelDurDlg message handlers

BOOL CSelDurDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nDuration &= 0x1f;	//just in case not initialized.
	m_BmWhole.LoadBitmap(IDB_NOTE_WHOLE);
	m_Combo_SelDur.AddBitmap(&m_BmWhole, CString(""));
	m_BmHalf.LoadBitmap(IDB_NOTE_HALF);
	m_Combo_SelDur.AddBitmap(&m_BmHalf, CString(""));
	m_BmQuarter.LoadBitmap(IDB_NOTE_QUARTER);
	m_Combo_SelDur.AddBitmap(&m_BmQuarter, CString(""));
	m_BmEight.LoadBitmap(IDB_NOTE_EIGTH);
	m_Combo_SelDur.AddBitmap(&m_BmEight, CString(""));
	m_BmSixteenth.LoadBitmap(IDB_NOTE_SIXTEENTH);
	m_Combo_SelDur.AddBitmap(&m_BmSixteenth, CString(""));
	m_BmThirtySec.LoadBitmap(IDB_NOTE_THIRTYSECOND);
	m_Combo_SelDur.AddBitmap(&m_BmThirtySec, CString(""));
	m_nDurSel = CMsNote::GetDurationTable()[m_nDuration].NoteShapeIndex;
	m_Combo_SelDur.SetCurSel(m_nDurSel);
	if (CMsNote::GetDurationTable()[m_nDuration].Dotted)
		m_Radio_Sel = 1;
	else if (CMsNote::GetDurationTable()[m_nDuration].Triplet)
		m_Radio_Sel = 2;
	else
		m_Radio_Sel = 0;
	UpdateData(0);
	if(m_pPrompt) m_Static_Promt.SetWindowText(CString(m_pPrompt));
	return TRUE;  // return true unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return false
}

void CSelDurDlg::OnSelchangeComboSelDuratuion() 
{
	
}

void CSelDurDlg::OnOK() 
{
	UpdateData();
	m_nDurSel = m_Combo_SelDur.GetCurSel();
	m_nDuration = CMsNote::NoteDurLut[m_nDurSel];
	if (m_Radio_Sel == 1) m_nDuration += 2;	//dotted
	else if(m_Radio_Sel == 2) m_nDuration -= 2;	//tripplet
	CDialog::OnOK();
}
