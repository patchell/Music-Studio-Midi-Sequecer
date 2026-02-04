
// ChildFrm.cpp : implementation of the CChildFrameStaff class
//

#include "pch.h"


// CChildFrameStaff

IMPLEMENT_DYNCREATE(CChildFrameStaff, CChildFrameBase)

BEGIN_MESSAGE_MAP(CChildFrameStaff, CChildFrameBase)
	ON_COMMAND(ID_FILE_CLOSE, &CChildFrameStaff::OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CChildFrameStaff construction/destruction

CChildFrameStaff::CChildFrameStaff() noexcept
{
	EnableActiveAccessibility();
}

CChildFrameStaff::~CChildFrameStaff()
{
}


// CChildFrameStaff message handlers
void CChildFrameStaff::OnFileClose() 
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.
	PostMessageW(WM_CLOSE);
}

void CChildFrameStaff::OnSetFocus(CWnd* pOldWnd) 
{
	CChildFrameBase::OnSetFocus(pOldWnd);

	GetView()->SetFocus();
}

BOOL CChildFrameStaff::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// let the view have first crack at the command
	if (GetView()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	// otherwise, do default handling
	return CChildFrameBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CChildFrameStaff::UpdateColors()
{
	GetView()->UpdateColors();
}

int CChildFrameStaff::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CChildFrameBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if (!m_wndStaffView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	static int count = 0;
	CString csSongName;
	CString csTitle;
	// Set the title of the window to "SongX" where X is a number
	csSongName.Format(_T("Song%d"), ++count);
	csTitle.Format(_T("Enter Song Name"));
	CEnterStringDlg Dlg;
	Dlg.SetTitle(csTitle);
	Dlg.SetString(csSongName);
	Dlg.DoModal();
	csSongName = Dlg.GetString();
	SetWindowTextW(csSongName);
	return 0;
}

