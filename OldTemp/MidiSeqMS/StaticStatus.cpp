// StaticStatus.cpp : implementation file
//

#include "pch.h"

/////////////////////////////////////////////////////////////////////////////
// CStaticStatus

CStaticStatus::CStaticStatus()
{
	color_BG = RGB(0, 0, 0);
	color_Text = RGB(64, 128, 255);
	color_Border = RGB(255, 255, 255);
}

CStaticStatus::~CStaticStatus()
{
}


BEGIN_MESSAGE_MAP(CStaticStatus, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticStatus message handlers

void CStaticStatus::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CStaticStatus::SetText(CString& csText)
{
	m_csDisplayText = csText;
	GetParent()->Invalidate();
}


void CStaticStatus::SetColors(COLORREF bGnd, COLORREF text, COLORREF Border)
{
	color_BG = bGnd;
	color_Text = text;
	color_Border = Border;
	GetParent()->Invalidate();
}

bool CStaticStatus::Create(const CRect& rect, CWnd* pParentWnd, UINT nID)
{
	m_ptUpperLeft = rect.TopLeft();
	m_szRectSize = rect.Size();
	return CWnd::Create(NULL,NULL,  WS_VISIBLE | WS_CHILD, rect, pParentWnd, nID);
}


BOOL CStaticStatus::OnEraseBkgnd(CDC* pDC)
{
	return true;
}


void CStaticStatus::OnDraw(CDC* pDC)
{
	CRect rect;
	CBrush br, * oldbrush;
	CPen border, * oldpen;
	CPoint Oldorg;
	int l;
	TEXTMETRIC metrics;

	Oldorg = pDC->SetViewportOrg(m_ptUpperLeft);
	pDC->GetTextMetrics(&metrics);
	border.CreatePen(PS_SOLID, 2, color_Border);
	br.CreateSolidBrush(color_BG);
	GetClientRect(&rect);
	oldpen = pDC->SelectObject(&border);
	oldbrush = pDC->SelectObject(&br);
	pDC->Rectangle(&rect);
	pDC->SetTextColor(color_Text);

	l = m_csDisplayText.GetLength();
	if (l)
	{
		int y = rect.CenterPoint().y;
		y -= metrics.tmHeight / 2;
		pDC->SetBkColor(color_BG);
		pDC->TextOutW(10, y, m_csDisplayText);
	}
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldpen);
	pDC->SetViewportOrg(Oldorg);
}
