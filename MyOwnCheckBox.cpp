// MyOwnCheckBox.cpp : implementation file
//

#include "pch.h"


// CMyOwnCheckBox

IMPLEMENT_DYNAMIC(CMyOwnCheckBox, CWnd)

CMyOwnCheckBox::CMyOwnCheckBox()
{

	m_CheckState = 0;
	m_ControlMessage = 0;
	m_colorBackGround = 0;
	m_colorBox = 0;
	m_colorCheck = 0;
	m_colorText = 0;
}

CMyOwnCheckBox::~CMyOwnCheckBox()
{
}


BEGIN_MESSAGE_MAP(CMyOwnCheckBox, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CMyOwnCheckBox message handlers




void CMyOwnCheckBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	PrintPoint("Check Box Button Down", point);
	CWnd::OnLButtonDown(nFlags, point);
}


void CMyOwnCheckBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	PrintPoint("My OWN Checkbox", point);
	m_CheckState ^= 1;	//toggle state
	GetParent()->Invalidate();
	GetParent()->PostMessageW(
		m_ControlMessage,
		GetDlgCtrlID(),
		m_CheckState
	);
	GetParent()->Invalidate();
	CWnd::OnLButtonUp(nFlags, point);
}


void CMyOwnCheckBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//BUT, we do nothing!
}


BOOL CMyOwnCheckBox::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


BOOL CMyOwnCheckBox::Create(
	CString& stringTrueCaption,
	CString& stringFalseCaption,
	const CRect& rect,
	CWnd* pParentWnd,
	UINT nID,
	COLORREF colorBackGround,
	COLORREF colorBox,
	COLORREF colorCheck,
	COLORREF colorText,
	unsigned ControlMessage
)
{
	LPCTSTR lpszWindowName = NULL;
	LPCTSTR lpszClassName = _T("BitmapComboBox");
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	CCreateContext* pContext = NULL;
	int fontHeight;

	m_pointOrigin = rect.TopLeft();
	m_ControlMessage = ControlMessage;
	m_colorBackGround = colorBackGround;
	m_colorBox = colorBox;
	m_colorCheck = colorCheck;
	m_colorText = colorText;

	m_stringTrueCaption = stringTrueCaption;
	m_stringFalseCaption = stringFalseCaption;
	fontHeight = GETAPP->FontPixelsToLogicalUnits(
		pParentWnd->GetDC(), 
		(rect.Height() * 65)/100
	);
	m_fontCaption.CreateFontW(
			fontHeight,
			0,
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
			_T("Arial")
	);
	m_rectBox = CRect(
		CPoint(0,0),
		CSize(rect.Height(),rect.Height())
	);
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CMyOwnCheckBox::Draw(CDC* pDC)
{
	CPoint pointOldViewPort;
	CRect rectClient;
	CBrush brushBackGround;
	CBrush* pOldBrush;
	CPen penBox;
	CPen penCheck;
	CPen* pOldPen;
	CFont* pOldFont;
	COLORREF oldBKcolor, oldTextcolor;

	if (IsWindowVisible() && IsWindowEnabled())
	{
		pointOldViewPort = pDC->SetViewportOrg(m_pointOrigin);
		GetClientRect(&rectClient);
		brushBackGround.CreateSolidBrush(m_colorBackGround);
		penBox.CreatePen(PS_SOLID, 1, m_colorBox);
		pDC->FillRect(&rectClient, &brushBackGround);
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);;
		pOldPen = pDC->SelectObject(&penBox);
		pDC->Rectangle(&m_rectBox);
		pOldFont = pDC->SelectObject(&m_fontCaption);
		oldBKcolor = pDC->SetBkColor(m_colorBackGround);
		oldTextcolor = pDC->SetTextColor(m_colorText);
		if (m_CheckState)
		{
			pDC->TextOutW(m_rectBox.Width() + 2, 0, m_stringTrueCaption);
			penCheck.CreatePen(PS_SOLID, 1, m_colorCheck);
			pDC->SelectObject(&penCheck);
			pDC->MoveTo(m_rectBox.left, m_rectBox.top);
			pDC->LineTo(m_rectBox.right, m_rectBox.bottom);
			pDC->MoveTo(m_rectBox.right, m_rectBox.top);
			pDC->LineTo(m_rectBox.left, m_rectBox.bottom);
		}
		else
			pDC->TextOutW(m_rectBox.Width() + 2, 0, m_stringFalseCaption);
		// Cleanup
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
		pDC->SetBkColor(oldBKcolor);
		pDC->SetTextColor(oldTextcolor);
		pDC->SelectObject(pOldFont);
		pDC->SetViewportOrg(pointOldViewPort);
	}
}
