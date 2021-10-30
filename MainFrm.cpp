
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame


IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	EnableActiveAccessibility();
	m_pHead = 0;
	m_pTail = 0;
	m_NumChildren = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::AddChild(CChildFrameBase* pCF)
{
	if ( m_pHead == 0)	//nobody home
	{
		m_pHead = pCF;
		m_pTail = pCF;
	}
	else
	{

		GetTail()->SetNext(pCF);
		pCF->SetPrev(GetTail());
		SetTail(pCF);
	}
	m_NumChildren++;
}

void CMainFrame::RemoveChild(CChildFrameBase* pCF)
{
	if (m_pHead == pCF)
	{
		m_pHead = pCF->GetNext();
		m_pHead->SetPrev(0);
	}
	else if (m_pTail == pCF)
	{
		m_pTail = pCF->GetPrev();
		m_pTail->SetNext(0);
	}
	else
	{
		pCF->GetNext()->SetPrev(pCF->GetPrev());
		pCF->GetPrev()->SetNext(pCF->GetNext());
	}
	m_NumChildren--;
}

BOOL CMainFrame::IsChildStillAlive(unsigned childID)
{
	BOOL ChildIsAlive = FALSE;
	CChildFrameBase* pCFB = GetHead();
	int loop = 1;

	while (pCFB && loop)
	{
		if (childID == pCFB->GetView()->GetID())
		{
			loop = 0;
			ChildIsAlive = TRUE;
		}
		else
			pCFB = pCFB->GetNext();
	}
	return ChildIsAlive;
}


void CMainFrame::OnInitialUpdate()
{
}

void CMainFrame::UpdateColors()
{
	CChildFrameBase* pCV;

	pCV = GetHead();
	while (pCV)
	{
		pCV->UpdateColors();
		pCV = pCV->GetNext();
	}
}
