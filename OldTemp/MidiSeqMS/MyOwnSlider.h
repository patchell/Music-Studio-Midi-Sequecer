#pragma once


// CMyOwnSlider


constexpr auto MYOWNSLIDER_STYLE_TEXT = (0);
constexpr auto MYOWNSLIDER_STYLE_BITMAP = (1);
constexpr auto MYOWNSLIDER_STYLE_TICKS = (2);
constexpr auto MYOWNSLIDER_STYLE_STRING_TABLE = (4);

constexpr auto MYOWNSLIDER_MOUSEDOWN_AUTOINC_DELAY = (400);

constexpr auto MYOWNSLIDER_NOARROW_CLICKED = -1;
constexpr auto MYOWNSLIDER_ARROW_LEFT = 0;
constexpr auto MYOWNSLIDER_ARROW_RIGHT = 1;
constexpr auto MYOWNSLIDER_TIMERID_LEFTARROW = 4000;
constexpr auto MYOWNSLIDER_TIMERID_RIGHTARROW = 4001;
constexpr auto MYOWNSLIDER_TIMERID_LBUTTON_DOWN_TIMER = 4002;


class CMyOwnSlider : public CWnd
{
	UINT m_SliderStyle;
	UINT m_TimerID;
	UINT m_WmMessage;
	CPoint m_pointOrigin;
	CRect m_rectClient;

	CRect m_rectLeftArrow;
	CRect m_rectRightArrow;
	CRect m_rectTrack;
	UINT m_ThumbWidth;
	CMyPolygon m_polyThumb;
	CPoint m_pThumbPoints[5];
	CPoint m_triangleLeftArrow[3];
	CPoint m_triangleRightArrow[3];
	//------------------------------
	// Caption stuff
	//------------------------------
	int m_CaptionTop;
	CString m_csCaption;
	CFont m_fontCaption;
	//------------------------------
	// Text display stuff
	//------------------------------
	CRect m_rectDisplay;
	CFont m_fontDisplay;
	int m_fontHeight;
	CPoint m_pointTextStart;
	//--------- Colors -------------
	COLORREF m_colorArrow;
	COLORREF m_colorArrowBox;
	COLORREF m_colorTrack;
	COLORREF m_colorThumb;
	COLORREF m_colorThumbPointer;
	COLORREF m_colorScale;
	COLORREF m_colorBackGround;
	COLORREF m_colorTextBackGround;
	COLORREF m_colorTextColor;
	//------ Scale Marks ----------
	int m_ScaleBottom;
	UINT m_ScaleStartX;	//X coordinate for start of scale (min value)
	UINT m_ScaleEndX;	//X coordinate for end of scale (max value)
	int m_TickHeightEnds;		//height of end tics
	int m_TickHeightMiddle;	//height of the middle tick
	int m_TickHeightMajor;	//major tick marks
	int m_TickIncrementMajor;	//Increment of MajorTicks
	int m_TickHeightMinor;	//Tick Height of the other ticks
	int m_TickIncrementMinor;//Spacing between all tics
	int m_TotalTicks;		//Number of ticks marks on scale
	//------- Various Status flags ---------
	int m_MouseIsDown;
	int m_ThumbDragX;
	int m_ArrowClicked;	//arrow that is clicked
	//Thumb Position Attributes
	int m_Position;	// position of the thumb
	int m_DisplayOffset;
	int m_ThumXRef;
	int m_MaxVal;	// maximum value of slider
	int m_MinVal;	// minimum value of slider
	int m_Ticks;	// max - min
	int m_PosGapp;	// gapp at max end due to division remainder
	int m_TrackSlideWidth;	//distance thumb will tavel
	int m_XPosition;	// x posistion of thumb
	int m_MaxX;			// minimum value posisiton
	int m_MinX;			// maximum value Posd
	//---------------- Bitmap Display --------------------
	CMyBitmap** m_ppbmBitmaps;
	int m_NumberOfBitmaps;
	//------------------ String Table -------------------
	CString* m_pcsStringTablel;
	//---------------------------------------------------
	DECLARE_DYNAMIC(CMyOwnSlider)
public:
	CMyOwnSlider();
	virtual ~CMyOwnSlider();
	virtual bool Create(
		CPoint ptControlOrigin,//upper left corner 1
		int SliderHight,	//slider height 2
		int SliderArrowWidth,	// width of arrows 3
		int TrackWidth,		//width of track 4
		int TrackHeight,	//height of slider track 5
		int ThumbWidth,		//width of thumb 6
		int TickHeightEnds,		//height of end tics 7
		int TickHeightMiddle,	//height of the middle tick 8
		int TickHeightMajor,	//major tick marks  9
		int TickIncrementMajor,	//Increment of Ticks 10
		int TickHeightMinor,	//Tick Height of the other ticks 11
		int TickIncrementMinor, //Increment between all ticks 12
		int TotalTicks,		//Number of ticks marks on scale 13
		CSize szDisplaySize,	// size of display box 14
		int Spacing,	//Space between slider and display
		CSize TextMargin,	//margin between text and display box
		CWnd* pParentWnd,	//parent window
		COLORREF TrackColor,//track color
		COLORREF ArrowBoxColor,
		COLORREF ArrowColor,//arrow color
		COLORREF ThumbColor,//Thumb Color
		COLORREF ThumbPointerColor,	//Thumb Pointer Color
		COLORREF ScaleColor,	// color of ticks scale
		COLORREF SliderBackgroundColor,	//background color of slider
		COLORREF TextBackgroundColor,	//text and display rect color
		COLORREF TextColor,	//color of the text
		int MinValue,	//minimum value
		int MaxValue,	//Maximum value
		UINT nID,		//control ID
		UINT WmMessage,	//message to send parent
		UINT nNumBitmaps = 0,	//number of bitmaps
		UINT nSliderStyle = 0,	// Style of slider
		CString csCaption = _T("caption"),	// The name of the control
		int captionHeight = 12,	//height of the caption
		int DisplayOffset = 0	// amount to offset the display by
	);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void Draw(CDC* pDC);
	int XtoPos(int x);
	void SetMax(int m) { m_MaxVal = m; }
	int GetMax() { return m_MaxVal; }
	void SetMin(int m) { m_MinVal = m; }
	int GetMin() { return m_MinVal; }
	void AddBitmap(int index, CMyBitmap* bm) { m_ppbmBitmaps[index] = bm; }
	void SetStringTable(CString* pST) { m_pcsStringTablel = pST; }
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int PosToX();
	void DrawStrings(CDC* pDC);
	void DrawScale(CDC* pDC, CRect& clidentRect);
	void DrawText(CDC* pDC);
	void DrawCaption(CDC* pDC);
	void DrawBitmaps(CDC* pDC);
	int NumberOfDigits(int v);
	void PositionThumb();
	void SetValue(int v) { 
		if (v < m_MinVal) v = m_MinVal;
		else if (v > m_MaxVal) v = m_MaxVal;
		m_Position = v - m_MinVal; 
		PositionThumb();
	}
	int GetValue() { return m_Position + m_MinVal; }
protected:
	DECLARE_MESSAGE_MAP()
};


