// ComboDropDown.cpp : implementation file
//

#include "pch.h"

void PrintPoint(const char *string,CPoint pt)
{
//	if(LogFile()) fprintf(LogFile(),"POIJNT:%s: X = %d Y = %d\n", string, pt.x, pt.y);
}

void PrintSize(const char* string, CSize sz)
{
//	if(LogFile()) fprintf(LogFile(),"SIZE :%s: X = %d Y = %d\n", string, sz.cx, sz.cy);

}
// CComboDropDown


IMPLEMENT_DYNAMIC(CComboDropDown, CWnd)

CComboDropDown::CComboDropDown()
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_State = 0;
	m_LButtonDown = 0;
	m_nItems = 0;
	m_nTotalItems = 0;
	for (int i = 0; i < m_nItems; ++i)
	{
		m_apRectItemControls[i] = 0;
		m_apRgnItemControls[i] = 0;
	}
	// Slider Stuff
	m_Pos = 0;
	m_apBmItems = 0;
	m_nBitmapsAdded = 0;
	m_nCurSel = 0;
	m_nDragThumb = 0;
	m_nDragLastY = 0;
	//----------------------
	// Color pallate
	//----------------------
	m_Color_ListBG = RGB(32, 16, 64);
	m_Color_ListSelBG = RGB(64, 32, 192);
	m_Color_SelectionBG = RGB(192, 32, 0);
	m_Color_UpDownArrowBG = RGB(255, 64, 32);
	m_Color_UpDownArrow = RGB(192, 32, 192);
	m_Color_DropArrowBG = RGB(64, 192, 128);
	m_Color_Track = RGB(0, 48, 75);
	m_Color_Thumb = RGB(255, 0, 0);
	m_Color_Highlight = RGB(64, 0, 208);
	//-------------------------------------
	m_pWndLastFocus = 0;
}

CComboDropDown::~CComboDropDown()
{
	if(m_apBmItems)
	{
		delete[] m_apBmItems;
		m_apBmItems = 0;
	}
	for (int i = 0; i < m_nItems; ++i)
	{
		if (m_apRectItemControls[i]) delete m_apRectItemControls[i];
		if (m_apRgnItemControls[i]) delete m_apRgnItemControls[i];
	}
	delete[] m_apRgnItemControls;
	delete[] m_apRectItemControls;
}


bool CComboDropDown::Create(
	int nItems,			//Number of items to display
	int nTotalItems,	//Total number of items in control
	CSize szItemSize,	//Size of the item
	CPoint ptUpperLeft,	//Upper Left corerer of control
	CSize szDropArrowSize,	// Size of the drop arrow
	CWnd* pParentWnd,
	UINT nID			//Control ID
)
{
	bool rV;

	LPCTSTR lpszClassName = _T("BitmapComboBox");
	LPCTSTR lpszWindowName = nullptr;
	CCreateContext* pContext = nullptr;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	int y;
	int i;

	//-------- Drop Arrow ---------------
	// This is the arrow you press to
	// to expand the combo box
	//-----------------------------------
	m_ptDropCorner = CPoint(0, 0) + CSize(szItemSize.cx, 0);	//???
	m_szDropArrow = szDropArrowSize;
	m_rectDropArrow = CRect(m_ptDropCorner, m_szDropArrow);
	//	PrintRec("Drop Arrow NS", m_rectDropArrow);
	m_rgnDropArrow.CreateRectRgn(m_rectDropArrow);
	//---------- Selection Box -----------------
	// This is the box that the selection
	// shows up in.
	//---------------------------------------
	m_ptSelectionUpLeft = ptUpperLeft;
	m_szSelectionBox = szItemSize;
	m_rectSelectionBox = CRect(CPoint(0, 0), m_szSelectionBox);
	m_rgnSelectionBox.CreateRectRgn(m_rectSelectionBox);

	//------ Window Not Selected ---------------
	// Items that define the window when
	// the control is NOT selected.
	// These are using the parrents coordinates
	//-----------------------------------------
	m_szNotSelectedWindow = CSize(szItemSize.cx + szDropArrowSize.cx, szItemSize.cy);
	//	CMyRgn	m_rgnNotSel;
	m_rectNotSelectedWindow = CRect(ptUpperLeft, m_szNotSelectedWindow);
	//-------Window Selected ---------------
	// Items that define the size of the
	// window when it IS selected
	// These are in the parrent windows
	// coordinates
	//---------------------------------------
	y = ptUpperLeft.y + (nItems + 1) * szItemSize.cy;
	m_szSelectedWindow = CSize(szItemSize.cx + szDropArrowSize.cx, y);
	m_rectSelectedWindow = CRect(ptUpperLeft, m_szSelectedWindow);
	//---- Used to Size Control -----
	m_rectThisControl = m_rectNotSelectedWindow;
	//------- Object Properties
	m_szItemSize = szItemSize;
	//****** Slider Stuff ***********
	//------ Slider Track -------------
	m_ptTrackCorner = CPoint(szItemSize.cx, szDropArrowSize.cy + szDropArrowSize.cx);	//square arrows
	y = nItems * szItemSize.cy - 2 * m_szDropArrow.cx;
	m_szSliderTrack = CSize(szDropArrowSize.cx, y);
	m_rectSliderTrack = CRect(m_ptTrackCorner, m_szSliderTrack);
	m_rgnSliderTrack.CreateRectRgn(m_rectSliderTrack);
	//------- Up and Down Arrows -------
	m_ptUpArrowCorner = CPoint(szItemSize.cx, szItemSize.cy);
	m_szUpDnArrow = CSize(szDropArrowSize.cx, szDropArrowSize.cx);
	m_ptDownArrowCorner = CPoint(szItemSize.cx, m_szDropArrow.cy + m_szUpDnArrow.cy + y);
	m_rectUpperArrow = CRect(m_ptUpArrowCorner, m_szUpDnArrow);
	m_rectLowerArrow = CRect(m_ptDownArrowCorner, m_szUpDnArrow);
	m_rgnUpperArrow.CreateRectRgn(m_rectUpperArrow);
	m_rgnLowerArrow.CreateRectRgn(m_rectLowerArrow);
	//------- Slider Thumb --------
	m_szThumb = m_szUpDnArrow;
	y = m_ptTrackCorner.y + m_szSliderTrack.cy / 2 - m_szThumb.cy / 2;
	m_ptThumbCorner = CPoint(m_ptTrackCorner.x, y);
	m_rectThumb = CRect(m_ptThumbCorner, m_szThumb);
	//	PrintRec("Thumb", m_rectThumb);
	m_rgnThumb.CreateRectRgn(m_rectThumb);
	//----- Various Control properties ------
	m_nItems = nItems;
	m_nTotalItems = nTotalItems;
	//---------- Selection Items ------------
	// These are the items that you can choose
	// from whent the combo box is expanded
	//------------------------------------------
	m_apRgnItemControls = new CMyRgn * [nItems];
	m_apRectItemControls = new CRect * [nItems];
	m_apBmItems = new CMyBitmap * [m_nTotalItems];
	// Initialize Arrays
	CPoint UpperLeft = m_ptUpLeftCorn;
	CSize Inc = CSize(0, szItemSize.cy);
	UpperLeft += Inc;
	for (i = 0; i < nItems; ++i, UpperLeft += Inc)
	{
		m_apRectItemControls[i] = new  CRect;
		*(m_apRectItemControls[i]) = CRect(UpperLeft, szItemSize);
		m_apRgnItemControls[i] = new CMyRgn;
		m_apRgnItemControls[i]->CreateRectRgn(*(m_apRectItemControls[i]));
	}
	for (i = 0; i < m_nTotalItems; ++i)
	{
		m_apBmItems[i] = 0;
	}
	rV = CWnd::Create(
		lpszClassName,
		lpszWindowName,
		dwStyle,
		m_rectNotSelectedWindow,
		pParentWnd,
		nID,
		pContext
	);
	if (rV == false)
		if (LogFile()) fprintf(LogFile(), "Could not create DropDown\n");
	return rV;
}


BEGIN_MESSAGE_MAP(CComboDropDown, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// CComboDropDown message handlers

void CComboDropDown::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_LButtonDown = 1;
	if (m_rgnSliderTrack.PtInRegion(point))
	{
		if (m_rgnThumb.PtInRegion(point))
		{
			m_nDragThumb = true;
		m_nDragLastY = point.y;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CComboDropDown::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_LButtonDown = 0;
	m_nDragThumb = false;
	static int count = 0;
	int i;
	CRect rect;

	m_rgnSelectionBox.GetRgnBox(&rect);
	if (m_rgnSelectionBox.PtInRegion(point))
	{
		if (m_State == DROP_DOWN_NOTSELECTED)
		{
			m_pWndLastFocus = SetFocus();
			m_State = DROP_DOWN_SELECTED;
			Expand();
			GetParent()->Invalidate();
		}
		else       //Selected
		{
			m_State = DROP_DOWN_NOTSELECTED;
			Colapse();
			m_pWndLastFocus->SetFocus();
			GetParent()->Invalidate();
		}
	}
	else if (m_State == DROP_DOWN_SELECTED)
	{
		if (m_rgnLowerArrow.PtInRegion(point))
		{
			if (m_Pos < (m_nTotalItems - m_nItems))
				m_Pos += 1;
			MoveThumb(m_Pos);
			GetParent()->Invalidate();
		}
		else if (m_rgnUpperArrow.PtInRegion(point))
		{
			if (m_Pos > 0)
				--m_Pos;
			MoveThumb(m_Pos);
			GetParent()->Invalidate();
		}
		else
		{
			//---------------------------
			// Check to see if pointer
			// is in one of the item
			// regions
			//---------------------------
			int loop;
			int item = -1;

			for (i = 0, loop = 1; (i < m_nItems) && loop; ++i)
			{
				if (m_apRgnItemControls[i]->PtInRegion(point))
				{
					item = i + m_Pos;
					loop = 0;
				}
			}
			if (item >= 0)
			{
				m_nCurSel = item;
				m_State = DROP_DOWN_NOTSELECTED;
				Colapse();
				m_pWndLastFocus->SetFocus();
				int id = GetDlgCtrlID();
				GetParent()->PostMessageW(
					WM_MY_CONTROL_MESSAGES, 
					id, 
					m_nCurSel
				);
				GetParent()->Invalidate();
			}
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CComboDropDown::OnMouseMove(UINT nFlags, CPoint point)
{
	//--------------------------------
	// The only object that needs to
	// be processed while the mouse
	// is being moved is the slider 
	// thumb object
	//--------------------------------
	if (m_LButtonDown && m_nDragThumb)
	{
		if (m_rgnThumb.PtInRegion(point))	//drag thumb
		{
			int diff;
			int maxY = m_ptTrackCorner.y + m_szSliderTrack.cy - m_szThumb.cy;
			int minY = m_ptTrackCorner.y;

			diff = point.y - m_nDragLastY;
			if ((m_ptThumbCorner.y + diff) > maxY)
			{
				diff = m_ptThumbCorner.y - maxY;
			}
			else if ((m_ptThumbCorner.y + diff) < minY)
			{
				diff = minY - m_ptThumbCorner.y;
			}
			m_ptThumbCorner.y += diff;
			m_rectThumb = CRect(m_ptThumbCorner, m_szThumb);;
			m_rgnThumb.OffsetRgn(0, diff);
			m_nDragLastY = point.y;
			m_Pos = YtoPos(m_ptThumbCorner.y - m_ptTrackCorner.y);
			GetParent()->Invalidate();
		}
		else
		{
			m_nDragThumb = 0;
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}


void CComboDropDown::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}


BOOL CComboDropDown::OnEraseBkgnd(CDC* pDC)
{
	return true;
}


void CComboDropDown::OnDraw(CDC* pDC)
{
	static int count = 0;
	CBrush Brush_ListBG;
	CBrush Brush_ListSelBG;
	CBrush Brush_SelectionBG;
	CBrush Brush_UpDownArrowBG;
	CBrush Brush_UpDownArrow;
	CBrush Brush_DropArrowBG;
	CBrush Brush_Track;
	CBrush Brush_Thumb;
	CBrush Brush_Highlight;
	CBrush Black;
	CPoint oldOrg;
	CPoint oldOrgSliderDraw;
	CRect rect;
	bool bFlag = false;
	CPen Draw, * oldPen;
	CRect bmr;
	CDC bmdc;

	m_rgnSelectionBox.GetRgnBox(&rect);
	bmdc.CreateCompatibleDC(pDC);
	Brush_ListBG.CreateSolidBrush(m_Color_ListBG);
	Brush_ListSelBG.CreateSolidBrush(m_Color_ListSelBG);
	Brush_SelectionBG.CreateSolidBrush(m_Color_SelectionBG);
	Brush_UpDownArrowBG.CreateSolidBrush(m_Color_UpDownArrowBG);
	Brush_UpDownArrow.CreateSolidBrush(m_Color_UpDownArrow);
	Brush_DropArrowBG.CreateSolidBrush(m_Color_DropArrowBG);
	Brush_Track.CreateSolidBrush(m_Color_Track);
	Brush_Thumb.CreateSolidBrush(m_Color_Thumb);
	Brush_Highlight.CreateSolidBrush(m_Color_Highlight);
	Black.CreateSolidBrush(RGB(0, 0, 0));

	Draw.CreatePen(PS_SOLID, 2, m_Color_UpDownArrow);
	GetClientRect(&rect);
	oldOrg = pDC->SetViewportOrg(m_rectThisControl.TopLeft());
	oldPen = pDC->SelectObject(&Draw);
	if (m_State == DROP_DOWN_SELECTED)	//selected
	{
		pDC->FillRect(&m_rectDropArrow, &Brush_DropArrowBG);
		pDC->FillRect(&m_rectSelectionBox, &Brush_SelectionBG);
		int i;
		CBitmap* oldBM;

		for (i = 0; i < m_nItems; ++i)
		{
			CBitmap* pBM;
			pBM = m_apBmItems[m_Pos + i];
			oldBM = bmdc.SelectObject(pBM);
			bmr = *m_apRectItemControls[i];
//			PrintRec("Item rect", bmr);
			CBrush* pB;
			pB = (m_nCurSel == i) ? &Brush_ListSelBG : &Brush_ListBG;
			pDC->FillRect(&bmr, pB);
			pDC->BitBlt(bmr.left + 1, bmr.top + 1, bmr.Width() - 1, bmr.Height() - 1, &bmdc, 0, 0, SRCCOPY);
			bmdc.SelectObject(oldBM);
		}
		pDC->FillRect(&m_rectSliderTrack, &Brush_Track);

		pDC->FillRect(&m_rectUpperArrow, &Brush_UpDownArrowBG);
		pDC->MoveTo(m_ptUpArrowCorner.x + 2, m_ptUpArrowCorner.y + m_szUpDnArrow.cy - 2);
		pDC->LineTo(m_ptUpArrowCorner.x + (m_szUpDnArrow.cx >> 1), m_ptUpArrowCorner.y + 2);
		pDC->LineTo(m_ptUpArrowCorner.x + m_szUpDnArrow.cx - 2, m_ptUpArrowCorner.y + m_szUpDnArrow.cy - 2);

		pDC->FillRect(&m_rectLowerArrow, &Brush_UpDownArrowBG);
		pDC->MoveTo(m_ptDownArrowCorner.x + 2, m_ptDownArrowCorner.y + 2);
		pDC->LineTo(m_ptDownArrowCorner.x + (m_szUpDnArrow.cx >> 1), m_ptDownArrowCorner.y + m_szUpDnArrow.cy - 2);
		pDC->LineTo(m_ptDownArrowCorner.x + m_szUpDnArrow.cx - 2, m_ptDownArrowCorner.y + 2);

		rect = m_rectThumb;
		rect.InflateRect(-3, -3);
		pDC->FillRect(&m_rectThumb, &Black);
		pDC->FillRect(&rect, &Brush_Thumb);
		// drop arrow
		pDC->MoveTo(m_ptDropCorner.x + 2, m_ptDropCorner.y + m_szDropArrow.cy - 2);
		pDC->LineTo(m_ptDropCorner.x + m_szDropArrow.cx / 2, m_ptDropCorner.y + 2);
		pDC->LineTo(m_ptDropCorner.x + m_szDropArrow.cx - 2, m_ptDropCorner.y + m_szDropArrow.cy - 2);
		//---------Current Selection -----
		bmr = m_rectSelectionBox;
		bmdc.SelectObject(m_apBmItems[m_nCurSel]);
		pDC->BitBlt(
			bmr.left + 1,
			bmr.top + 1,
			bmr.Width() - 1,
			bmr.Height() - 1,
			&bmdc,
			0, 0,
			SRCCOPY);
	}
	else	// Not Selected
	{
		pDC->FillRect(&m_rectDropArrow, &Brush_DropArrowBG);
		pDC->FillRect(&m_rectSelectionBox, &Brush_SelectionBG);

		bmdc.SelectObject(m_apBmItems[m_nCurSel]);
		bFlag = pDC->BitBlt(
			m_rectSelectionBox.left + 1,
			m_rectSelectionBox.top + 1,
			m_rectSelectionBox.Width() - 1,
			m_rectSelectionBox.Height() - 1,
			&bmdc,
			0,
			0, 
			SRCCOPY
		);

		pDC->MoveTo(m_ptDropCorner.x + 2,2);	 
		pDC->LineTo(m_ptDropCorner.x + m_szDropArrow.cx / 2, m_szDropArrow.cy - 2);
		pDC->LineTo(m_ptDropCorner.x + m_szDropArrow.cx - 2,2);
	}
	pDC->SelectObject(oldPen);
	pDC->SetViewportOrg(oldOrg);
}

void CComboDropDown::SetCurSel(int nSel, bool Notify)
{
	double m;
	int pos;
	int oldsel;

	m = double(m_nItems) / double(m_nTotalItems);
	oldsel = m_nCurSel;
	m_nCurSel = nSel;
	pos = int(m * double(nSel));
	MoveThumb(pos);
	if ((m_nCurSel != oldsel) && Notify)
	{
		int id = GetDlgCtrlID();
		GetParent()->SendMessageW(
			WM_MY_CONTROL_MESSAGES,
			id,
			m_nCurSel
		);
	}
	GetParent()->Invalidate();
}


int CComboDropDown::GetCurSel()
{
	return m_nCurSel;
}


int CComboDropDown::AddBitmap(CMyBitmap* pBM)
{
	//-------------------------
	// parameters:
	//	bmItem....item to add
	//
	//	returns 1 on success
	//	returns 0 on error
	//--------------------------
	int rv = 1;

	if (m_nBitmapsAdded == m_nTotalItems)
		rv = 0;
	else
		m_apBmItems[m_nBitmapsAdded++] = pBM;
	return rv;
}

void CComboDropDown::MoveThumb(int pos)
{
	CPoint oldcorner = m_ptThumbCorner;
	int slider_h = m_szSliderTrack.cy;
	int thumb_h = m_szThumb.cy;
	slider_h -= thumb_h;
	double inc = double(slider_h) / (double(m_nTotalItems) - double(m_nItems));
	m_ptThumbCorner.y = m_ptTrackCorner.y + int(double(pos) * inc);
	m_rectThumb = CRect(m_ptThumbCorner, m_szThumb);
	m_rgnThumb.OffsetRgn(0, m_ptThumbCorner.y - oldcorner.y);
}

void CComboDropDown::dblMoveThumb(double pos)
{
	CPoint oldcorner = m_ptThumbCorner;
	double slider_h = double(m_szSliderTrack.cy);
	double  thumb_h = double(m_szThumb.cy);
	slider_h -= thumb_h;
	double inc = slider_h / (double(m_nTotalItems) - double(m_nItems));
	m_ptThumbCorner.y = m_ptTrackCorner.y + int(pos * inc);
	m_rectThumb = CRect(m_ptThumbCorner, m_szThumb);
	m_rgnThumb.OffsetRgn(0, m_ptThumbCorner.y - oldcorner.y);
}


int CComboDropDown::YtoPos(int y)
{
	int pos = 0;

	double slider_h = double(m_szSliderTrack.cy);
	double  thumb_h = double(m_szThumb.cy);
	slider_h -= thumb_h;
	double inc = slider_h / (double(m_nTotalItems) - double(m_nItems));
	int thumbY = m_ptThumbCorner.y - m_ptTrackCorner.y;
	pos = int(double(thumbY) / inc);
	return pos;
}

int CComboDropDown::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_ACTIVATE;
}


BOOL CComboDropDown::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	return CWnd::OnChildNotify(message, wParam, lParam, pLResult);
}

void CComboDropDown::Colapse()
{
	m_rectThisControl = m_rectNotSelectedWindow;
	CWnd::OnSize(SIZE_RESTORED, m_szNotSelectedWindow.cx, m_szClientRecSize.cy);
	CWnd::MoveWindow(m_rectThisControl, 0);
}

void CComboDropDown::Expand()
{
	m_rectThisControl = m_rectSelectedWindow;
	CWnd::OnSize(SIZE_RESTORED, m_szSelectedWindow.cx, m_szSelectedWindow.cy);
	CWnd::MoveWindow(m_rectThisControl, 0);
}



void CComboDropDown::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	if (m_State == DROP_DOWN_SELECTED)
	{
		m_State = DROP_DOWN_NOTSELECTED;
		Colapse();
	}
}


int CComboDropDown::GetTotalWidth()
{
	int width;
	width = m_szItemSize.cx + m_szDropArrow.cx;
	return width;
}
