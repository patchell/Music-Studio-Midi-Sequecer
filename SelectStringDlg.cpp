// SelectStringDlg.cpp : implementation file
//

#include "pch.h"


// CSelectStringDlg dialog

IMPLEMENT_DYNAMIC(CSelectStringDlg, CDialog)

CSelectStringDlg::CSelectStringDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_STRINGSELECTOR, pParent)
{

	m_nStrings = 0;
	m_nSelection = 0;
}

CSelectStringDlg::~CSelectStringDlg()
{
	if (m_pcsStrings)
	{
		delete[] m_pcsStrings;
	}
}

void CSelectStringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SELECTOR, m_Combo_Selector);
}


BEGIN_MESSAGE_MAP(CSelectStringDlg, CDialog)
END_MESSAGE_MAP()


// CSelectStringDlg message handlers


BOOL CSelectStringDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < m_nStrings; ++i)
		m_Combo_Selector.InsertString(i, m_pcsStrings[i]);
	m_Combo_Selector.SetCurSel(m_nSelection);
	SetWindowText(m_csCaptionString);
	return true;  // return true unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return false
}


void CSelectStringDlg::OnOK()
{
	m_nSelection = m_Combo_Selector.GetCurSel();
	CDialog::OnOK();
}
