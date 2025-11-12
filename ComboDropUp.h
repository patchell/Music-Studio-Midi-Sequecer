#pragma once


// CComboDropUp

class CComboDropUp : public CWnd
{
	DECLARE_DYNAMIC(CComboDropUp)
	CWnd* m_pPrevFocus;
	//---------- Selection Items ------------
	// These are the items that you can choose
	// from whent the combo box is expanded
	//------------------------------------------
	CMyRgn** m_apRgnItemControls;	//regions where Items to select are
	CRect**	m_apRectItemControls;	//rectangles of the same
	//-------- BitMaps ------------
	CMyBitmap** m_apBmItems;
	//-------- Drop Arrow ---------------
	// This is the arrow you press to
	// to expand the combo box
	//-----------------------------------
	CPoint m_aptDropCorner[2];
	CSize m_szDropArrow;	//size of drop arrow
	CRect	m_arectDropArrow[2];	//rectangle for drop arrow Not Selected
	CMyRgn	m_argnDropArrow[2];		// choose between slected and not
	//---------- Selection Box -----------------
	// This is the box that the selection
	// shows up in.
	//---------------------------------------
	CSize	m_szSelectBox;
	CRect	m_arectSelectBox[2];
	CMyRgn	m_argnSelectBox[2];
	//------ Window Not Selected ---------------
	// Items that define the window when
	// the control is NOT selected.
	// These are using the parrents coordinates
	//-----------------------------------------
	CPoint	m_ptNotSelUpLeftCorn;
	CSize	m_szNotSel;
//	CMyRgn	m_rgnNotSel;
	CRect	m_rectNotSel;
	//-------Window Selected ---------------
	// Items that define the size of the
	// window when it IS selected
	//---------------------------------------
	CPoint	m_ptSelUpLeftCorn;
	CSize	m_szSel;
	CRect	m_rectSel;
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
	int m_nBitmapsAdded;
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
public:
	CComboDropUp();
	virtual ~CComboDropUp();
public:
	virtual BOOL Create(
		int nItems,		//number of items to display
		int nTotalItems,	//total number of items
		CSize szItemSize,	//size of an item
		CPoint ptLowLeft,	//lowqer left corner
		CSize szDropArrowSize,	//size of drop arrow
		CWnd* pParentWnd,	//parent window
		UINT nID		//control ID
	);		
	void OnDraw(CDC* pDC);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	int AddBitmap(CMyBitmap* bmItem);
	int GetCurSel();
	void MoveThumb(int amount);
	void SetCurSel(int Sel);
	void dblMoveThumb(double amount);
	int YtoPos(int y);
	void Colapse();
	void Expand();
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	void Reset(){ m_nBitmapsAdded = 0; }
	enum class DropUpItemState {
		DROP_UP_NOTSELECTED,
		DROP_UP_SELECTED
	};
};


void PrintRec(const char* pTit, CRect rec);
