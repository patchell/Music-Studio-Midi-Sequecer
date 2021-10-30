// SelectorDlg.cpp : implementation file
//

#include "pch.h"


// CSelectorDlg dialog

IMPLEMENT_DYNAMIC(CSelectorDlg, CDialog)

CSelectorDlg::CSelectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_SELECTOR, pParent)
{
	m_pBitmaps = 0;
	m_nBitmaps = 0;
	m_nSelection = 0;
}

CSelectorDlg::~CSelectorDlg()
{
}

void CSelectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_COMBO_SELECTOR, m_Combo_Selector);
}


BEGIN_MESSAGE_MAP(CSelectorDlg, CDialog)
END_MESSAGE_MAP()


// CSelectorDlg message handlers


BOOL CSelectorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < m_nBitmaps; ++i)
		m_Combo_Selector.AddBitmap(m_pBitmaps++, CString(""));
	m_Combo_Selector.SetCurSel(m_nSelection);
	SetWindowText(m_csCaption);
	return TRUE;  
}


void CSelectorDlg::OnOK()
{
	m_nSelection = m_Combo_Selector.GetCurSel();
	CDialog::OnOK();
}
