// CComboDropDownToggle.cpp : implementation file
//

#include "pch.h"


// CComboDropDownToggle


IMPLEMENT_DYNAMIC(CComboDropDownToggle, CWnd)

CComboDropDownToggle::CComboDropDownToggle()
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_pToggleValue = 0;
	m_pToggleFlags = 0;
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
	m_apBmSelItems = 0;
	m_apBmNotSelItems = 0;
	m_nSelBitmapsAdded = 0;
	m_nNotSelBitmapsAdded = 0;
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

CComboDropDownToggle::~CComboDropDownToggle()
{
	if (m_pToggleValue) delete[]m_pToggleValue;
	if (m_pToggleFlags) delete[] m_pToggleFlags;
	for (int i = 0; i < m_nTotalItems; ++i)
	{
		if (m_apBmSelItems[i]) delete m_apBmSelItems[i];
		if (m_apBmNotSelItems[i]) delete m_apBmNotSelItems[i];
	}
	if (m_apBmSelItems)delete[] m_apBmSelItems;
	if (m_apBmNotSelItems)delete[] m_apBmNotSelItems;
	for (int i = 0; i < m_nItems; ++i)
	{
		if (m_apRectItemControls[i]) delete m_apRectItemControls[i];
		if (m_apRgnItemControls[i]) delete m_apRgnItemControls[i];
	}
	delete[] m_apRgnItemControls;
	delete[] m_apRectItemControls;
}

BOOL CComboDropDownToggle::Create(
	int nItems,			//Number of items to display
	int nTotalItems,	//Total number of items in control
	CSize szItemSize,	//Size of the item
	CPoint ptUpperLeft,	//Upper Left corerer of control
	CSize szDropArrowSize,	// Size of the drop arrow
	CWnd* pParentWnd,
	UINT nID			//Control ID
)
{
	BOOL rV;

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
	m_apBmSelItems = new CMyBitmap * [m_nTotalItems];
	m_apBmNotSelItems = new CMyBitmap * [m_nTotalItems];
	// Initialize Arrays
	CPoint UpperLeft = m_ptUpLeftCorn;
	CSize Inc = CSize(0, szItemSize.cy);
	UpperLeft += Inc;
	for (i = 0; i < nItems; ++i, UpperLeft += Inc)
	{
		m_apRectItemControls[i] = new CRect;
		*(m_apRectItemControls[i]) = CRect(UpperLeft, szItemSize);
		m_apRgnItemControls[i] = new CMyRgn;
		m_apRgnItemControls[i]->CreateRectRgn(*(m_apRectItemControls[i]));
	}
	m_pToggleFlags = new int[nTotalItems];
	m_pToggleValue = new int[nTotalItems];
	for (i = 0; i < m_nTotalItems; ++i)
	{
		m_apBmSelItems[i] = 0;
		m_apBmNotSelItems[i] = 0;
		m_pToggleFlags[i] = 0;
		m_pToggleValue[i] = 0;
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
	if (rV == FALSE)
		printf("Could not create DropDown\n");
	return rV;
}

BEGIN_MESSAGE_MAP(CComboDropDownToggle, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// CComboDropDownToggle message handlers

void CComboDropDownToggle::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_LButtonDown = 1;
	if (m_rgnSliderTrack.PtInRegion(point))
	{
		if (m_rgnThumb.PtInRegion(point))
		{
			m_nDragThumb = TRUE;
			m_nDragLastY = point.y;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CComboDropDownToggle::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_LButtonDown = 0;
	m_nDragThumb = FALSE;
	static int count = 0;
	int i;
	CRect rect;

	m_rgnSelectionBox.GetRgnBox(&rect);
	if (m_rgnSelectionBox.PtInRegion(point))
	{
		if (m_State == DROP_DOWN_TOGGLE_NOTSELECTED)
		{
			m_pWndLastFocus = SetFocus();
			m_State = DROP_DOWN_TOGGLE_SELECTED;
			Expand();
			GetParent()->Invalidate();
		}
		else       //Selected
		{
			m_State = DROP_DOWN_TOGGLE_NOTSELECTED;
			Colapse();
			m_pWndLastFocus->SetFocus();
			GetParent()->Invalidate();
			GetParent()->SetFocus();
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
				m_State = DROP_DOWN_TOGGLE_NOTSELECTED;
				if (m_pToggleFlags[m_nCurSel] & COMBOTOGGLE_TOGGLE_ENABLE)
				{
					if ((m_pToggleFlags[m_nCurSel] & COMBOTOGGLE_TOGGLE_CHECKED) &&
						!(m_pToggleFlags[m_nCurSel] & COMBOTOGGLE_TOGGLE_EXCLUSIVE)
					)
					{
						m_pToggleValue[m_nCurSel] ^= 1;
						int id = GetDlgCtrlID();
						CBToggle data;
						data.Wparam = 0;
						data.value = m_pToggleValue[m_nCurSel];
						data.index = m_nCurSel;
						data.command = CBT_SETVALUE;
						GetParent()->PostMessageW(
							WM_MY_CONTROL_MESSAGES,
							id,
							data.Wparam
						);
					}
					else if ((m_pToggleFlags[m_nCurSel] & COMBOTOGGLE_TOGGLE_CHECKED) && 
						(m_pToggleFlags[m_nCurSel]&COMBOTOGGLE_TOGGLE_EXCLUSIVE)
					)
					{
						for (int i = 0; i < m_nTotalItems; ++i)
						{
							if (i != m_nCurSel &&
								m_pToggleFlags[i] & COMBOTOGGLE_TOGGLE_EXCLUSIVE)
							{
								m_pToggleValue[i] = 0;
							}
							else if (i == m_nCurSel &&
								m_pToggleFlags[i] & COMBOTOGGLE_TOGGLE_EXCLUSIVE)
							{
								m_pToggleValue[i] = 1;
								CBToggle data;
								data.Wparam = 0;
								data.command = CBT_SETVALUE;
								data.index = i;
								data.value = m_pToggleValue[i];
								GetParent()->PostMessageW(
									WM_MY_CONTROL_MESSAGES,
									GetDlgCtrlID(),
									data.Wparam
								);
							}

						}
					}
				}
				else
				{
					CBToggle data;

					data.Wparam = 0;
					data.command = CBT_INDEX_ONLY;
					data.index = m_nCurSel;;
					data.value = m_nCurSel;
					GetParent()->PostMessageW(
						WM_MY_CONTROL_MESSAGES,
						GetDlgCtrlID(),
						data.Wparam
					);
				}
				Colapse();
				GetParent()->SetFocus();
				GetParent()->Invalidate();
			}
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CComboDropDownToggle::OnMouseMove(UINT nFlags, CPoint point)
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


void CComboDropDownToggle::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}


BOOL CComboDropDownToggle::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CComboDropDownToggle::OnDraw(CDC* pDC)
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
	BOOL b = 0;
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
	if (m_State == DROP_DOWN_TOGGLE_SELECTED)	//selected
	{
		pDC->FillRect(&m_rectDropArrow, &Brush_DropArrowBG);
		pDC->FillRect(&m_rectSelectionBox, &Brush_SelectionBG);
		int i;
		CBitmap* oldBM;

		for (i = 0; i < m_nItems; ++i)
		{
			CBitmap* pBM;
			pBM = m_pToggleValue[i]?m_apBmSelItems[m_Pos + i]: m_apBmNotSelItems[m_Pos + i];
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
		bmdc.SelectObject(m_pToggleValue[m_nCurSel]?m_apBmSelItems[m_nCurSel]: m_apBmNotSelItems[m_nCurSel]);
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

		bmdc.SelectObject(m_pToggleValue[m_nCurSel] ? m_apBmSelItems[m_nCurSel] : m_apBmNotSelItems[m_nCurSel]);
		b = pDC->BitBlt(
			m_rectSelectionBox.left + 1,
			m_rectSelectionBox.top + 1,
			m_rectSelectionBox.Width() - 1,
			m_rectSelectionBox.Height() - 1,
			&bmdc,
			0,
			0,
			SRCCOPY
		);

		pDC->MoveTo(m_ptDropCorner.x + 2, 2);
		pDC->LineTo(m_ptDropCorner.x + m_szDropArrow.cx / 2, m_szDropArrow.cy - 2);
		pDC->LineTo(m_ptDropCorner.x + m_szDropArrow.cx - 2, 2);
	}
	pDC->SelectObject(oldPen);
	pDC->SetViewportOrg(oldOrg);
}

void CComboDropDownToggle::SetCurSel(int nSel)
{
	double m;
	int pos;
	int oldsel;

	m = double(m_nItems) / double(m_nTotalItems);
	oldsel = m_nCurSel;
	m_nCurSel = nSel;;
	pos = int(m * double(nSel));
	MoveThumb(pos);
	GetParent()->Invalidate();
}


int CComboDropDownToggle::GetCurSel()
{
	return m_nCurSel;
}

int CComboDropDownToggle::ToggleItem(int ItemId)
{
	if (m_pToggleFlags[ItemId] & COMBOTOGGLE_TOGGLE_ENABLE)
	{
		if (m_pToggleValue[ItemId])
		{
			m_pToggleValue[ItemId] = 0;
		}
		else if (!m_pToggleValue[ItemId])
		{
			if (m_pToggleFlags[ItemId] & COMBOTOGGLE_TOGGLE_CHECKED)
			{
				m_pToggleValue[ItemId] = 1;
				if (m_pToggleFlags[ItemId] & COMBOTOGGLE_TOGGLE_EXCLUSIVE)
				{
					for (int i = 0; i < m_nTotalItems; ++i)
					{
						if ((i != ItemId) && (m_pToggleFlags[ItemId] & COMBOTOGGLE_TOGGLE_EXCLUSIVE))
						{
							m_pToggleValue[i] = 0;
						}
					}
				}
			}
		}
	}
	return m_pToggleValue[ItemId];
}

int CComboDropDownToggle::AddNotSelBitmapID(int ID)
{
	//-------------------------
	// parameters:
	//	ID....Bitmap ID of item to add
	//
	//	returns 1 on success
	//	returns 0 on error
	//--------------------------
	int rv = 1;

	if (m_nNotSelBitmapsAdded == m_nTotalItems)
		rv = 0;
	else
	{
		m_apBmNotSelItems[m_nNotSelBitmapsAdded] = new CMyBitmap;
		m_apBmNotSelItems[m_nNotSelBitmapsAdded]->LoadBitmapW(ID);
		m_nNotSelBitmapsAdded++;
	}
	return rv;
}

int CComboDropDownToggle::AddSelBitmapID(int ID)
{
	//-------------------------
	// parameters:
	//	ID....Bitmap ID of item to add
	//
	//	returns 1 on success
	//	returns 0 on error
	//--------------------------
	int rv = 1;

	if (m_nSelBitmapsAdded == m_nTotalItems)
		rv = 0;
	else
	{
		m_apBmSelItems[m_nSelBitmapsAdded] = new CMyBitmap;
		m_apBmSelItems[m_nSelBitmapsAdded]->LoadBitmapW(ID);
		m_nSelBitmapsAdded++;
	}
	return rv;
}

void CComboDropDownToggle::MoveThumb(int pos)
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

void CComboDropDownToggle::dblMoveThumb(double pos)
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


int CComboDropDownToggle::YtoPos(int y)
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

int CComboDropDownToggle::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_ACTIVATE;
}


BOOL CComboDropDownToggle::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	return CWnd::OnChildNotify(message, wParam, lParam, pLResult);
}

void CComboDropDownToggle::Colapse()
{
	m_rectThisControl = m_rectNotSelectedWindow;
	CWnd::OnSize(SIZE_RESTORED, m_szNotSelectedWindow.cx, m_szClientRecSize.cy);
	CWnd::MoveWindow(m_rectThisControl, 0);
}

void CComboDropDownToggle::Expand()
{
	m_rectThisControl = m_rectSelectedWindow;
	CWnd::OnSize(SIZE_RESTORED, m_szSelectedWindow.cx, m_szSelectedWindow.cy);
	CWnd::MoveWindow(m_rectThisControl, 0);
}



void CComboDropDownToggle::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	if (m_State == DROP_DOWN_TOGGLE_SELECTED)
	{
		m_State = DROP_DOWN_TOGGLE_NOTSELECTED;
		Colapse();
	}
}

int CComboDropDownToggle :: GetTotalWidth()
{
	int width;
	width = m_szItemSize.cx + m_szDropArrow.cx;
	return width;
}


// Clears all items and set current selection
void CComboDropDownToggle::ClearAllItems(int nItem)
{
	CBToggle ToggleMSG;

	for (int i = 0; i < m_nItems; ++i)
	{
		if (GetItemValue(i) || (nItem == i) )
		{
			if(nItem == i) m_pToggleValue[i] = 1;
			else m_pToggleValue[i] = 0;
			ToggleMSG.Wparam = 0;
			ToggleMSG.command = CBT_SETVALUE;
			ToggleMSG.index = i;
			ToggleMSG.value = m_pToggleValue[i];
			GetParent()->SendMessageW(
				WM_MY_CONTROL_MESSAGES,
				GetDlgCtrlID(),
				ToggleMSG.Wparam
			);
		}
	}
	GetParent()->Invalidate();
	SetCurSel(nItem);
}
