#pragma once

// CComboDropDownToggle

class CComboDropDownToggle : public CWnd
{
	DECLARE_DYNAMIC(CComboDropDownToggle)
	CWnd* m_pWndLastFocus;
	//---------- Selection Items ------------
	// These are the items that you can choose
	// from whent the combo box is expanded
	//------------------------------------------
	CMyRgn** m_apRgnItemControls;	//regions where Items to select are
	CRect** m_apRectItemControls;	//rectangles of the same
	//-------- BitMaps ------------
	CMyBitmap** m_apBmSelItems;
	CMyBitmap** m_apBmNotSelItems;
	int m_nSelBitmapsAdded;
	int m_nNotSelBitmapsAdded;
	//-------- Drop Arrow ---------------
	// This is the arrow you press to
	// to expand the combo box
	//-----------------------------------
	CPoint m_ptDropCorner;
	CSize m_szDropArrow;	//size of drop arrow
	CRect	m_rectDropArrow;	//rectangle for drop arrow Not Selected
	CMyRgn	m_rgnDropArrow;		// choose between slected and not
	//---------- Selection Box -----------------
	// This is the box that the selection
	// shows up in.
	//---------------------------------------
	CPoint m_ptSelectionUpLeft;
	CSize	m_szSelectionBox;
	CRect	m_rectSelectionBox;
	CMyRgn	m_rgnSelectionBox;
	//------ Window Not Selected ---------------
	// Items that define the window when
	// the control is NOT selected.
	// These are using the parrents coordinates
	//-----------------------------------------
	CPoint	m_ptUpLeftCorn;
	CSize	m_szNotSelectedWindow;
	CRect	m_rectNotSelectedWindow;
	//-------Window Selected ---------------
	// Items that define the size of the
	// window when it IS selected
	//---------------------------------------
	CSize	m_szSelectedWindow;
	CRect	m_rectSelectedWindow;
	//---- Used to Size Control -----
	CRect m_rectThisControl;
	//------- Object Properties
	CSize m_szItemSize;
	//------ Client Sizes and rects
	CSize m_szClientRecSize;
	CSize m_szItemList;
	//****** Slider Stuff ***********
	//------ Slider Track -------------
	CPoint m_ptTrackCorner;
	CSize m_szSliderTrack;
	CRect m_rectSliderTrack;
	CMyRgn	m_rgnSliderTrack;
	//------- Slider Thumb --------
	CPoint m_ptThumbCorner;
	CSize m_szThumb;
	CRect m_rectThumb;
	CMyRgn m_rgnThumb;
	//------- Up and Down Arrows -------
	CPoint m_ptUpArrowCorner;
	CPoint m_ptDownArrowCorner;
	CSize m_szUpDnArrow;
	CRect m_rectUpperArrow;
	CRect m_rectLowerArrow;
	CMyRgn	m_rgnUpperArrow;
	CMyRgn	m_rgnLowerArrow;
	//------- Slider Variables -------
	int m_Pos;	//thumb position
	int m_State;
	int m_LButtonDown;
	int m_nItems;
	int m_nTotalItems;
	int m_nCurSel;
	int m_nDragThumb;
	int m_nDragLastY;
	//----------------------
	// Color pallate
	//----------------------
	COLORREF	m_Color_ListBG;
	COLORREF	m_Color_ListSelBG;
	COLORREF	m_Color_SelectionBG;
	COLORREF	m_Color_UpDownArrowBG;
	COLORREF	m_Color_UpDownArrow;
	COLORREF	m_Color_DropArrowBG;
	COLORREF	m_Color_Track;
	COLORREF	m_Color_Thumb;
	COLORREF	m_Color_Highlight;
	//-------------------------------------
	// Toggle Flags
	//------------------------------------
	int* m_pToggleFlags;
	int* m_pToggleValue;
public:
	CComboDropDownToggle();
	virtual ~CComboDropDownToggle();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL Create(
		int nItems,
		int nTotalItems,
		CSize szItemSize,
		CPoint ptUpperLeft,
		CSize szDropArrowSize,
		CWnd* pParentWnd,
		UINT nID	//control ID
	);
	int GetTotalWidth();
	void OnDraw(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	int AddSelBitmapID(int ID);
	int AddNotSelBitmapID(int ID);
	int GetItemValue(int item) { return m_pToggleValue[item]; }
	void SetItemValue(int item, int val){m_pToggleValue[item] = val;}
	void SetItemFlags(int item, int flag) { m_pToggleFlags[item] = flag; }
	int GetItemFlags(int item) { return m_pToggleFlags[item]; }
	void SetCurSel(int nSel);
	void MoveThumb(int amount);
	int GetCurSel();
	int ToggleItem(int ItemId);
	void dblMoveThumb(double amount);
	int YtoPos(int y);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	void Colapse();
	void Expand();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	// Clears all items and set current selection
	void ClearAllItems(int nItem);
	void Reset() { m_nSelBitmapsAdded = 0; }
};



