// ComboDropUp.cpp : implementation file
//

#include "pch.h"

// CComboDropUp

void PrintRec(const char *pTit, CRect rec)
{
	//printf("RECT :%s:Point(%d, %d) Size(%d, %d)\n",
	//	pTit,
	//	rec.left, 
	//	rec.top, 
	//	rec.Size().cx, 
	//	rec.Size().cy);
}

IMPLEMENT_DYNAMIC(CComboDropUp, CWnd)


CComboDropUp::CComboDropUp()
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_State = 0;
	m_LButtonDown = 0;
	m_nItems = 0;
	m_nTotalItems = 0;
	m_apRectItemControls = 0;
	m_apRgnItemControls = 0;
	// Slider Stuff
	m_Pos = 0;
	m_apBmItems = 0;
	m_nBitmapsAdded = 0;
	m_nCurSel = 0;
	m_nDragThumb = 0;
	m_nDragLastY = 0;
	m_pPrevFocus = 0;
	//----------------------
	// Color pallate
	//----------------------
	m_Color_ListBG = RGB(32,16,64);
	m_Color_ListSelBG = RGB(64,32,192);
	m_Color_SelectionBG = RGB(192,32,0);
	m_Color_UpDownArrowBG = RGB(32,64,32);
	m_Color_UpDownArrow = RGB(192,32,192);
	m_Color_DropArrowBG = RGB(64,192,128);
	m_Color_Track = RGB(0,48,75);
	m_Color_Thumb = RGB(192, 64, 16);
	m_Color_Highlight = RGB(64,0,208);
}

CComboDropUp::~CComboDropUp()
{
	if (m_apBmItems) delete [] m_apBmItems;
}


BEGIN_MESSAGE_MAP(CComboDropUp, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// CComboDropUp message handlers

void CComboDropUp::OnPaint()
{
	CPaintDC dc(this);	//this must be here.  <shrug>
}


BOOL CComboDropUp::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}



void CComboDropUp::OnDraw(CDC* pDC)
{
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
	bool b;
	CPen Draw,*oldPen;
	CRect bmr;
	CDC bmdc;

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
	 Black.CreateSolidBrush(RGB(0,0,0));

	Draw.CreatePen(PS_SOLID, 2, m_Color_UpDownArrow);
	GetClientRect(&rect);
	oldOrg = pDC->SetViewportOrg(m_rectThisControl.TopLeft());
	oldPen = pDC->SelectObject(&Draw);
	if (m_State == int(DropUpItemState::DROP_UP_SELECTED))	//selected
	{
		pDC->FillRect(&m_arectDropArrow[int(DropUpItemState::DROP_UP_SELECTED)], &Brush_DropArrowBG);
		pDC->FillRect(&m_arectSelectBox[int(DropUpItemState::DROP_UP_SELECTED)], &Brush_SelectionBG);
		int i;
		CBitmap* oldBM;

		for (i = 0; i < m_nItems; ++i)
		{
			CBitmap* pBM;
			pBM = m_apBmItems[m_Pos + i];
			oldBM = bmdc.SelectObject(pBM);
			bmr = *m_apRectItemControls[i];
			CBrush* pB;
			pB = (m_nCurSel == i)?&Brush_ListSelBG:&Brush_ListBG;
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
		pDC->MoveTo(m_aptDropCorner[int(DropUpItemState::DROP_UP_SELECTED)].x + 2, m_aptDropCorner[int(DropUpItemState::DROP_UP_SELECTED)].y+2);
		pDC->LineTo(m_aptDropCorner[int(DropUpItemState::DROP_UP_SELECTED)].x + m_szDropArrow.cx / 2, m_aptDropCorner[int(DropUpItemState::DROP_UP_SELECTED)].y + m_szDropArrow.cy - 2);
		pDC->LineTo(m_aptDropCorner[int(DropUpItemState::DROP_UP_SELECTED)].x + m_szDropArrow.cx - 2, m_aptDropCorner[int(DropUpItemState::DROP_UP_SELECTED)].y + 2);
		//---------Current Selection -----
		bmr = m_arectSelectBox[int(DropUpItemState::DROP_UP_SELECTED)];
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
//		PrintRec("DropUp Selection Box", m_arectSelectBox[DROP_UP_NOTESELECTED]);
//		PrintRec("DropUp Arrow Box", m_arectDropArrow[DROP_UP_NOTESELECTED]);
		pDC->FillRect(&m_arectDropArrow[int(DropUpItemState::DROP_UP_NOTSELECTED)], &Brush_DropArrowBG);
		pDC->FillRect(&m_arectSelectBox[int(DropUpItemState::DROP_UP_NOTSELECTED)], &Brush_SelectionBG);
		bmr = &m_arectSelectBox[int(DropUpItemState::DROP_UP_NOTSELECTED)];
		pDC->FillRect(&bmr, &Brush_DropArrowBG);

		bmdc.SelectObject(m_apBmItems[m_nCurSel]);
		b = pDC->BitBlt(
			bmr.left + 1,
			bmr.top + 1,
			bmr.Width() - 1,
			bmr.Height() - 1,
			&bmdc,
			0, 0, SRCCOPY);

		pDC->MoveTo(m_aptDropCorner[int(DropUpItemState::DROP_UP_NOTSELECTED)].x + 2, m_szDropArrow.cy - 2);
		pDC->LineTo(m_aptDropCorner[int(DropUpItemState::DROP_UP_NOTSELECTED)].x + m_szDropArrow.cx /2, 2);
		pDC->LineTo(m_aptDropCorner[int(DropUpItemState::DROP_UP_NOTSELECTED)].x + m_szDropArrow.cx - 2, m_szDropArrow.cy - 2);
	}
	pDC->SelectObject(oldPen);
	pDC->SetViewportOrg(oldOrg);
}

bool CComboDropUp::Create(
	int nItems,			// Number of Items Displayed
	int nTotalItems,	// TOTAL items in control
	CSize szItemSize,	// size of single item to be selected
	CPoint ptLowLeft,	// Lower left corner
	CSize szDropArrowSize,
	CWnd* pParentWnd,
	UINT nID)			//control ID
{
	bool rV;

	LPCTSTR lpszClassName = _T("BitmapComboBox");
	LPCTSTR lpszWindowName = nullptr;
	CCreateContext* pContext = nullptr;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	int i;

	//-------- Drop Arrow ---------------
	// This is the arrow you press to
	// to expand the combo box
	//-----------------------------------
	m_aptDropCorner[int(DropUpItemState::DROP_UP_NOTSELECTED)] = CPoint(szItemSize.cx,0);
	m_aptDropCorner[int(DropUpItemState::DROP_UP_SELECTED)] = CPoint(szItemSize.cx, nItems * szItemSize.cy);
	m_szDropArrow = szDropArrowSize;
	m_arectDropArrow[int(DropUpItemState::DROP_UP_NOTSELECTED)] = CRect(m_aptDropCorner[int(DropUpItemState::DROP_UP_NOTSELECTED)], m_szDropArrow);
	m_arectDropArrow[int(DropUpItemState::DROP_UP_SELECTED)] = CRect(m_aptDropCorner[int(DropUpItemState::DROP_UP_SELECTED)], m_szDropArrow);
//	PrintRec("Drop Arrow NS", m_arectDropArrow[DROP_UP_NOTESELECTED]);
//	m_argnDropArrow[DROP_UP_NOTESELECTED].CreateRectRgn(m_arectDropArrow[DROP_UP_NOTESELECTED]);
	m_argnDropArrow[int(DropUpItemState::DROP_UP_SELECTED)].CreateRectRgn(m_arectDropArrow[int(DropUpItemState::DROP_UP_SELECTED)]);
	//---------- Selection Box -----------------
	// This is the box that the selection
	// shows up in.
	//---------------------------------------
	m_szSelectBox = szItemSize;
	m_arectSelectBox[int(DropUpItemState::DROP_UP_NOTSELECTED)] = CRect(CPoint(0, 0), m_szSelectBox);
	m_arectSelectBox[int(DropUpItemState::DROP_UP_SELECTED)] = CRect(CPoint(0, nItems * szItemSize.cy), m_szSelectBox);
	m_argnSelectBox[int(DropUpItemState::DROP_UP_NOTSELECTED)].CreateRectRgn(m_arectSelectBox[int(DropUpItemState::DROP_UP_NOTSELECTED)]);
	m_argnSelectBox[int(DropUpItemState::DROP_UP_SELECTED)].CreateRectRgn(m_arectSelectBox[int(DropUpItemState::DROP_UP_SELECTED)]);

	//------ Window Not Selected ---------------
	// Items that define the window when
	// the control is NOT selected.
	// These are using the parrents coordinates
	//-----------------------------------------
	m_ptNotSelUpLeftCorn = CPoint(ptLowLeft.x, ptLowLeft.y - szItemSize.cy);
	m_szNotSel = CSize(szItemSize.cx + szDropArrowSize.cx,szItemSize.cy);
//	CMyRgn	m_rgnNotSel;
	m_rectNotSel = CRect(m_ptNotSelUpLeftCorn, m_szNotSel);
	//-------Window Selected ---------------
	// Items that define the size of the
	// window when it IS selected
	// These are in the parrent windows
	// coordinates
	//---------------------------------------
	int y;
	y = ptLowLeft.y - (nItems + 1) * szItemSize.cy;
	m_ptSelUpLeftCorn = CPoint(ptLowLeft.x, y);
	m_szSel = CSize(szItemSize.cx + szDropArrowSize.cx,
		(nItems + 1) * szItemSize.cy);
	m_rectSel = CRect(m_ptSelUpLeftCorn, m_szSel);
	//---- Used to Size Control -----
	m_rectThisControl = m_rectNotSel;
	//------- Object Properties
	m_szItemSize = szItemSize;;
	//****** Slider Stuff ***********
	//------ Slider Track -------------
	m_ptTrackCorner = CPoint(szItemSize.cx, szDropArrowSize.cx);	//square arrows
	y = nItems * szItemSize.cy - 2 * m_ptTrackCorner.y;
	m_szSliderTrack = CSize(szDropArrowSize.cx,y);
	m_rectSliderTrack = CRect(m_ptTrackCorner, m_szSliderTrack);
	m_rgnSliderTrack.CreateRectRgn(m_rectSliderTrack);
	//------- Up and Down Arrows -------
	m_ptUpArrowCorner = CPoint(szItemSize.cx,0);
	m_szUpDnArrow = CSize(szDropArrowSize.cx, szDropArrowSize.cx);
	m_ptDownArrowCorner = CPoint(szItemSize.cx, m_szUpDnArrow.cy + y);
	m_rectUpperArrow = CRect(m_ptUpArrowCorner, m_szUpDnArrow);
	m_rectLowerArrow = CRect(m_ptDownArrowCorner, m_szUpDnArrow);
	m_rgnUpperArrow.CreateRectRgn(m_rectUpperArrow);
	m_rgnLowerArrow.CreateRectRgn(m_rectLowerArrow);
	//------- Slider Thumb --------
	m_szThumb = m_szUpDnArrow;
	y = m_ptTrackCorner.y +  m_szSliderTrack.cy / 2 - m_szThumb.cy/2;
	m_ptThumbCorner = CPoint(m_ptTrackCorner.x,y);
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
	CPoint UpperLeft = CPoint(0, 0);
	CPoint Inc = CPoint(0, szItemSize.cy);
	for (i = 0; i < nItems; ++i,UpperLeft += Inc)
	{
		m_apRectItemControls[i] = new CRect;
		*(m_apRectItemControls[i]) = CRect(UpperLeft,szItemSize);
		m_apRgnItemControls[i] = new CMyRgn;
		m_apRgnItemControls[i]->CreateRectRgn(*(m_apRectItemControls[i]));
	}
	for (i = 0; i < m_nTotalItems; ++i)
	{
		m_apBmItems[i] = 0;
	}
//	PrintRec("Create Control NotSel:", m_rectNotSel);
//	PrintRec("Item Rec", m_arectSelectBox[0]);
//	PrintRec("Create Control Selected:", m_rectSel);

	rV = CWnd::Create(
		lpszClassName, 
		lpszWindowName, 
		dwStyle, 
		m_rectNotSel, 
		pParentWnd, 
		nID, 
		pContext
	);
	return rV;
}

void CComboDropUp::OnLButtonDown(UINT nFlags, CPoint point)
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


void CComboDropUp::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_LButtonDown = 0;
	m_nDragThumb = false;
	int i;

	if (m_argnSelectBox[m_State].PtInRegion(point))
	{
		if (m_State == int(DropUpItemState::DROP_UP_NOTSELECTED))
		{
			m_pPrevFocus = SetFocus();
			m_State = int(DropUpItemState::DROP_UP_SELECTED);
			Expand();
		}
		else       //Selected
		{
			m_State = int(DropUpItemState::DROP_UP_NOTSELECTED);
			m_pPrevFocus->SetFocus();
			Colapse();
		}
	}
	else if (m_State == int(DropUpItemState::DROP_UP_SELECTED))
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
				GetParent()->Invalidate();
				m_State = int(DropUpItemState::DROP_UP_NOTSELECTED);
				Colapse();
				m_pPrevFocus->SetFocus();
			}
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CComboDropUp::OnMouseMove(UINT nFlags, CPoint point)
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

BOOL CComboDropUp::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	return CWnd::OnChildNotify(message, wParam, lParam, pLResult);
}

int CComboDropUp::AddBitmap(CMyBitmap* bmItem)
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
		m_apBmItems[m_nBitmapsAdded++] = bmItem;
	return rv;
}


int CComboDropUp::GetCurSel()
{
	return m_nCurSel;
}

void CComboDropUp::SetCurSel(int Sel)
{
	double m;
	int pos;
	int oldsel;

	m = double(m_nItems) / double(m_nTotalItems);
	oldsel = m_nCurSel;
	m_nCurSel = Sel;;
	pos = int(m * double( Sel));
	MoveThumb(pos);
	if (m_nCurSel != oldsel)
	{
		int id = GetDlgCtrlID();
		GetParent()->PostMessageW(
			WM_MY_CONTROL_MESSAGES,
			id,
			m_nCurSel
		);
	}
	GetParent()->Invalidate();
}

void CComboDropUp::MoveThumb(int pos)
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

void CComboDropUp::dblMoveThumb(double pos)
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


int CComboDropUp::YtoPos(int y)
{
	double slider_h = double(m_szSliderTrack.cy);
	double  thumb_h = double(m_szThumb.cy);
	slider_h -= thumb_h;
	double inc = slider_h / (double(m_nTotalItems) - double(m_nItems));
	int thumbY = m_ptThumbCorner.y - m_ptTrackCorner.y;
	int pos = int(double(thumbY) / inc);
	return pos;
}

void CComboDropUp::Colapse()
{
	m_rectThisControl = m_rectNotSel;
	CWnd::OnSize(SIZE_RESTORED, m_szNotSel.cx, m_szClientRecSize.cy);
	CWnd::MoveWindow(m_rectThisControl, 0);
	int id = GetDlgCtrlID();
	GetParent()->PostMessageW(
		WM_MY_CONTROL_MESSAGES, 
		id, 
		m_nCurSel
	);
	GetParent()->Invalidate();
}

void CComboDropUp::Expand()
{
	m_rectThisControl = m_rectSel;
	CWnd::OnSize(SIZE_RESTORED, m_szSel.cx, m_szSel.cy);
	CWnd::MoveWindow(m_rectThisControl, 0);
	GetParent()->Invalidate();
}

void CComboDropUp::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	if (m_State == int(DropUpItemState::DROP_UP_SELECTED))
	{
		m_State = int(DropUpItemState::DROP_UP_NOTSELECTED);
		Colapse();
	}
}
