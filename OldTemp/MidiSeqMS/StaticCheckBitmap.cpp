// StaticCheckBitmap.cpp : implementation file
//

#include "pch.h"


// CStaticCheckBitmap

IMPLEMENT_DYNAMIC(CStaticCheckBitmap, CStatic)

CStaticCheckBitmap::CStaticCheckBitmap()
{
	m_Check = 0;
	m_Color = RGB(0, 0, 255);
	m_TrackID = 0;
}

CStaticCheckBitmap::~CStaticCheckBitmap()
{
}


BEGIN_MESSAGE_MAP(CStaticCheckBitmap, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CStaticCheckBitmap message handlers




void CStaticCheckBitmap::OnPaint()
{
	CPoint ptPt;
	CPen *oldPen, Pen;
	CBrush* oldBrush, Brush, OtherBrush, Black, Highlight;
	CRect rect,innerRect;

	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages
	GetClientRect(&rect);
	innerRect.CopyRect(&rect);
	Black.CreateStockObject(BLACK_BRUSH);
	Highlight.CreateStockObject(GETMIDIINFO->GetTextColor(m_TrackID));
	Brush.CreateSolidBrush(m_Color);
	dc.SetTextColor(GETMIDIINFO->GetTextColor(m_TrackID));
	dc.SetBkColor(m_Color);
	if (m_Check)
	{
		dc.FillRect(&rect, &Highlight);
		OtherBrush.CreateStockObject(NULL_BRUSH);
		innerRect.DeflateRect(CSize(2, 2));
		Pen.CreatePen(PS_SOLID, 2, GETMIDIINFO->GetTextColor(m_TrackID));
		oldPen = dc.SelectObject(&Pen);
		oldBrush = dc.SelectObject(&OtherBrush);
		dc.Rectangle(rect);
		dc.FillRect(&innerRect, &Brush);
		dc.SelectObject(oldPen);
		dc.SelectObject(oldBrush);
	}
	else
	{
		dc.FillRect(&rect, &Black);
		dc.FillRect(&innerRect, &Brush);
	}
	char* s = new char[4];
	sprintf_s(s, 4, "%d", m_TrackID);
	ptPt = CPoint(innerRect.left + 2, innerRect.top + 2);
	dc.TextOutW(ptPt.x, ptPt.y, CString(s));
}

void CStaticCheckBitmap::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Check ^= 1;
	Invalidate();
	CStatic::OnLButtonDown(nFlags, point);
}
