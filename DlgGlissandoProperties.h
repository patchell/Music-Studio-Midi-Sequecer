#pragma once
#include "afxdialogex.h"


// CDlgGlissandoProperties dialog

class CDlgGlissandoProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgGlissandoProperties)

public:
	CDlgGlissandoProperties(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgGlissandoProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlgGlissandoProperties };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
