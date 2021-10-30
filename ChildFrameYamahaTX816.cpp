// ChildFrm.cpp : implementation of the ChildFrameYamahaTX816 class
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrameYamahaTX816

IMPLEMENT_DYNCREATE(CChildFrameYamahaTX816, CChildFrameBase)

BEGIN_MESSAGE_MAP(CChildFrameYamahaTX816, CChildFrameBase)
	ON_COMMAND(ID_FILE_CLOSE, &CChildFrameYamahaTX816::OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CChildFrameYamahaTX816 construction/destruction

CChildFrameYamahaTX816::CChildFrameYamahaTX816() noexcept
{
	// TODO: add member initialization code here
}

CChildFrameYamahaTX816::~CChildFrameYamahaTX816()
{
}


BOOL CChildFrameYamahaTX816::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if (!CChildFrameBase::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// ChildFrameYamahaTX816 diagnostics

#ifdef _DEBUG
void CChildFrameYamahaTX816::AssertValid() const
{
	CChildFrameBase::AssertValid();
}

void CChildFrameYamahaTX816::Dump(CDumpContext& dc) const
{
	CChildFrameBase::Dump(dc);
}
#endif //_DEBUG

// ChildFrameYamahaTX816 message handlers
void CChildFrameYamahaTX816::OnFileClose()
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.
	SendMessage(WM_CLOSE);
}

int CChildFrameYamahaTX816::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CChildFrameBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	return 0;
}

void CChildFrameYamahaTX816::OnSetFocus(CWnd* pOldWnd)
{
	CChildFrameBase::OnSetFocus(pOldWnd);

	m_wndView.SetFocus();
}

BOOL CChildFrameYamahaTX816::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CChildFrameBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
