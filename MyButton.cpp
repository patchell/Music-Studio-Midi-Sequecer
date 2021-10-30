// MyButton.cpp : implementation file
//

#include "pch.h"


// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CWnd)

CMyButton::CMyButton()
{
	m_pBmButtonDisp = 0;
	m_colorDark = RGB(0, 48, 128);
	m_colorLight = RGB(0, 64, 192);
	m_flagPushed = 0;
	m_flagLatched = 0;
	m_ID = 0;
	m_pParent = 0;
	m_pCWndUnLatch = 0;
	m_styleLatched = 0;
}

CMyButton::~CMyButton()
{
}

BOOL CMyButton::Create(
	const CPoint& ptUL, 
	CWnd* pParentWnd, 
	UINT nID, 
	UINT nBUbmId, 
	UINT nBDbmId,
	CWnd* pCWndUnLatch,
	int Style
)
{
	LPCTSTR lpszClassName = NULL;
	LPCTSTR lpszWindowName = NULL;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;

	m_pCWndUnLatch = pCWndUnLatch;
	if (Style & MYBUTTS_LATCHED)
		m_styleLatched = 1;
	m_pParent = pParentWnd;
	m_ID = nID;
	m_bmButtonOut.LoadBitmapW(nBUbmId);
	m_bmButtonPushed.LoadBitmapW(nBDbmId);
	m_pBmButtonDisp = &m_bmButtonOut;
	CSize itemsize = m_bmButtonOut.GetBmDim();
	itemsize += CSize(MYBUTTON_XPAD, MYBUTTON_YPAD);
	m_pBmButtonDisp = &m_bmButtonOut;
	m_ptUpLeftCorn = ptUL;
	m_szButtSize = itemsize;
	return CWnd::Create(
		lpszClassName, 
		lpszWindowName, 
		dwStyle, 
		CRect(ptUL, itemsize), 
		pParentWnd, 
		nID 
	);
}

BEGIN_MESSAGE_MAP(CMyButton, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MYBUTTON_UNLATCH, &CMyButton::OnMybuttonUnlatch)
END_MESSAGE_MAP()

// CMyButton message handlers

BOOL CMyButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CMyButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages
}


void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pBmButtonDisp = &m_bmButtonPushed;
	m_flagPushed = 1;
	if (m_styleLatched)
	{
		m_flagLatched = 1;
		m_pParent->SetFocus();
	}
	m_pParent->Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}


void CMyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_styleLatched)	// Normal Mode
	{
		m_pBmButtonDisp = &m_bmButtonOut;
		m_flagPushed = 0;
		m_pParent->Invalidate();
		if(m_pCWndUnLatch)
			m_pCWndUnLatch->PostMessageW(
				WM_MYBUTTON_UNLATCH, 
				0, 
				0
			);
	}
	GetParent()->PostMessageW(
		WM_MY_CONTROL_MESSAGES,
		GetDlgCtrlID(),
		0
	);
	CWnd::OnLButtonUp(nFlags, point);
	m_pParent->SetFocus();
}


void CMyButton::OnDraw(CDC* pDC)
{
	CRect ClientRect;
	CDC memDC;
	CPoint oldOrg;
	CBitmap *oldBM;
	CPen pen1;
	CPen pen2;
	CPen* oldPen;

	pen1.CreatePen(PS_SOLID, 5, m_flagPushed ? m_colorDark : m_colorLight);
	pen2.CreatePen(PS_SOLID, 5, m_flagPushed ? m_colorLight : m_colorDark);
	oldOrg = pDC->SetViewportOrg(m_ptUpLeftCorn);
	GetClientRect(&ClientRect);
	memDC.CreateCompatibleDC(pDC);
	oldBM = memDC.SelectObject(m_pBmButtonDisp);
	oldPen = pDC->SelectObject(&pen1);
	pDC->MoveTo(3, 3);
	pDC->LineTo(3, ClientRect.Height() - 6);
	pDC->LineTo(ClientRect.Width() - 6, ClientRect.Height() - 6);
	pDC->SelectObject(&pen2);
	pDC->LineTo(ClientRect.Width() - 6, 3);
	pDC->LineTo(3, 3);
	//Blit in the bitmap
	pDC->BitBlt(
		ClientRect.left + 5,
		ClientRect.top + 5,
		ClientRect.Width() - 10,
		ClientRect.Height() - 10,
		&memDC,
		0,
		0,
		SRCCOPY
	);
	//Cleanup
	pDC->SetViewportOrg(oldOrg);
	memDC.SelectObject(oldBM);
	pDC->SelectObject(oldPen);
}

void CMyButton::LikeA_ButtonPush()
{
	if (!m_styleLatched)	// Normal Mode
	{
		m_pBmButtonDisp = &m_bmButtonOut;
		m_flagPushed = 0;
		m_pParent->Invalidate();
		if (m_pCWndUnLatch)
			m_pCWndUnLatch->PostMessageW(
				WM_MYBUTTON_UNLATCH,
				0,
				0
			);
	}
}

CSize CMyButton::GetButtonSize(int ID)
{
	extern CSize GetBmDimensions(int ID);

	CSize szBmSize;
	szBmSize = GetBmDimensions(ID);
	szBmSize += CSize(MYBUTTON_XPAD, MYBUTTON_YPAD);
	return szBmSize;
}


afx_msg LRESULT CMyButton::OnMybuttonUnlatch(WPARAM wParam, LPARAM lParam)
{	
	m_flagLatched = 0;
	m_flagPushed = 0;
	m_pBmButtonDisp = &m_bmButtonOut;
	return 0;
}
