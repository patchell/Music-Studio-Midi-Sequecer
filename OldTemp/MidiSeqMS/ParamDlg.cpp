// ParamDlg.cpp : implementation file
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamDlg dialog


CParamDlg::CParamDlg(CWnd* pParent )
	: CDialog(CParamDlg::IDD, pParent)
{
	m_Val = 0;
	m_Max = 0;
	m_Min = 0;
	m_csCaption = CString("");
}


void CParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PARAM, m_Static_Param);
	DDX_Control(pDX, IDC_SCROLLBAR_PARAM, m_ScrollBar_Param);
}


BEGIN_MESSAGE_MAP(CParamDlg, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamDlg message handlers

void CParamDlg::OnOK() 
{
	m_Val = m_ScrollBar_Param.GetScrollPos();
	CDialog::OnOK();
}

BOOL CParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ScrollBar_Param.SetScrollRange(m_Min,m_Max);
	m_ScrollBar_Param.SetDisplayText(&m_Static_Param);
	m_ScrollBar_Param.SetScrollPos(m_Val);
	SetWindowText(m_csCaption);
	return TRUE;  // return true unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return false
}
