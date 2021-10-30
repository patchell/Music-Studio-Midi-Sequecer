// StaticBitmap.cpp : implementation file
//

#include "pch.h"

/////////////////////////////////////////////////////////////////////////////
// CStaticBitmap

CStaticBitmap::CStaticBitmap()
{
	m_pMaster = 0;
	m_Color = RGB(0, 0, 121);
	m_Msg = 0;
	m_WMsg = 0;
	for (int i = 0; i < 10; ++i) m_aTextString[i] = 0;
}

CStaticBitmap::~CStaticBitmap()
{
}


BEGIN_MESSAGE_MAP(CStaticBitmap, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticBitmap message handlers

void CStaticBitmap::Init()
{
	CRect rect;

	GetClientRect(&rect);
	m_Textfont.CreateFontW(
		rect.Height() - 10,
		(rect.Width() - 5) / 2,
		0,
		0,
		FW_DONTCARE,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		DEFAULT_PITCH,
		_T("Courier")
	);
}

void CStaticBitmap::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CFont* oldFont;
	CRect rect;
	GetClientRect(&rect);
	CBrush br;
	CSize csTextSize;
	COLORREF oldcolor,oldTextColor;

	br.CreateSolidBrush(m_Color);
	dc.FillRect(&rect,&br);
	if (strlen(m_aTextString))
	{
		oldFont = dc.SelectObject(&m_Textfont);
		csTextSize = dc.GetTextExtent(CString(m_aTextString));
		int X, Y;
		X = rect.Size().cx - csTextSize.cx;
		X /= 2;
		Y = rect.Size().cy - csTextSize.cy;
		Y /= 2;
		oldcolor = dc.SetBkColor(m_Color);
		oldTextColor = dc.SetTextColor(m_Color ^ 0x00ffffff);
		dc.TextOutW(X, Y, CString(m_aTextString));
		dc.SetBkColor(oldcolor);
		dc.SetTextColor(oldTextColor);
	}
}

void CStaticBitmap::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	if(m_pMaster) m_pMaster->PostMessageW(WM_UTILMESSAGE,m_Msg);
	CStatic::OnLButtonDown(nFlags, point);
}

void CStaticBitmap::SendChange()
{
	if(m_pMaster) m_pMaster->PostMessageW(
		m_WMsg,
		m_Msg,
		m_Color
	);

}

void CStaticBitmap::SetupMessaging(CWnd *pW, int Wmsg, int Smsg)
{
	m_pMaster = pW;
	m_WMsg = Wmsg;
	m_Msg = Smsg;
}
