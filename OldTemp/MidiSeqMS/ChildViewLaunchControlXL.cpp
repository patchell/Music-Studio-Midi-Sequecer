// LaunchControlXLView.cpp : implementation file
//

#include "pch.h"

// CLaunchControlXLView

IMPLEMENT_DYNAMIC(CChildViewLaunchControlXL, CWnd)

CChildViewLaunchControlXL::CChildViewLaunchControlXL()
{

}

CChildViewLaunchControlXL::~CChildViewLaunchControlXL()
{
}


BEGIN_MESSAGE_MAP(CChildViewLaunchControlXL, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CChildViewLaunchControlXL message handlers




BOOL CChildViewLaunchControlXL::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CWnd::OnEraseBkgnd(pDC);
}


void CChildViewLaunchControlXL::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages
}


void CChildViewLaunchControlXL::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonDown(nFlags, point);
}


void CChildViewLaunchControlXL::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonUp(nFlags, point);
}


void CChildViewLaunchControlXL::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnMouseMove(nFlags, point);
}
