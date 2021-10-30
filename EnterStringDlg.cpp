// EnterStringDlg.cpp : implementation file
//

#include "pch.h"


// CEnterStringDlg dialog

IMPLEMENT_DYNAMIC(CEnterStringDlg, CDialogEx)

CEnterStringDlg::CEnterStringDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ENTERSTRING, pParent)
{

}

CEnterStringDlg::~CEnterStringDlg()
{
}

void CEnterStringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ENTR_STRING, m_Edit_String);
}


BEGIN_MESSAGE_MAP(CEnterStringDlg, CDialogEx)
END_MESSAGE_MAP()


// CEnterStringDlg message handlers


BOOL CEnterStringDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Edit_String.SetWindowTextW(m_S);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CEnterStringDlg::OnOK()
{
	m_Edit_String.GetWindowTextW(m_S);
	CDialogEx::OnOK();
}
