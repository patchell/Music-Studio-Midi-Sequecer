// DlgGlissandoProperties.cpp : implementation file
//

#include "pch.h"

// CDlgGlissandoProperties dialog

IMPLEMENT_DYNAMIC(CDlgGlissandoProperties, CDialog)

CDlgGlissandoProperties::CDlgGlissandoProperties(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CDlgGlissandoProperties, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CDlgGlissandoProperties::~CDlgGlissandoProperties()
{
}

void CDlgGlissandoProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgGlissandoProperties, CDialog)
END_MESSAGE_MAP()


// CDlgGlissandoProperties message handlers

BOOL CDlgGlissandoProperties::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
