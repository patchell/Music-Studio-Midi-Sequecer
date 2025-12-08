// MyOwnSlider.cpp : implementation file
//

#include "pch.h"


// CMyOwnSlider

IMPLEMENT_DYNAMIC(CMyOwnSlider, CWnd)

CMyOwnSlider::CMyOwnSlider()
{
	m_SliderStyle = 0;
	m_DisplayOffset = 0;
	m_ScaleBottom = 0;
	m_CaptionTop = 0;
	m_colorScale = RGB(255, 255, 255);
	m_colorArrow = RGB(255, 0, 0);
	m_colorArrowBox = RGB(0, 255, 255);
	m_colorThumb = RGB(0, 255, 0);
	m_colorTrack = RGB(0, 0, 255);
	m_colorThumbPointer = RGB(255, 255, 255);
	m_colorBackGround = RGB(0, 0, 0);
	m_colorTextBackGround = RGB(64, 0, 0);
	m_colorTextColor = RGB(255, 255, 255);
	m_MaxVal = 0;
	m_MinVal = 0;
	m_Position = 0;
	m_MouseIsDown = 0;
	m_ThumbDragX = 0;
	m_MinX = 0;
	m_MaxX = 0;
	m_WmMessage = 0;
	m_ppbmBitmaps = 0;
	m_NumberOfBitmaps = 0;
	m_ArrowClicked = 0;
	m_PosGapp = 0;
	m_ThumXRef = 0;
	m_ThumbWidth = 0;
	m_Ticks = 0;
	m_TrackSlideWidth = 0;
	m_XPosition = 0;
	m_TimerID = 0;
	m_ScaleStartX = 0;
	m_ScaleEndX = 0;
	m_TickHeightEnds = 0;		//height of end tics
	m_TickHeightMiddle = 0;	//height of the middle tick
	m_TickHeightMajor = 0;	//major tick marks
	m_TickIncrementMajor = 0;	//Increment of Ticks
	m_TickHeightMinor = 0;	//Tick Height of the other ticks
	m_TickIncrementMinor = 0;//Spacing between all tics
	m_TotalTicks = 0;		//Number of ticks marks on scale
	m_pcsStringTablel = 0;
	m_fontHeight = 0;
}

CMyOwnSlider::~CMyOwnSlider()
{
}


BEGIN_MESSAGE_MAP(CMyOwnSlider, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CMyOwnSlider message handlers

void CMyOwnSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_MouseIsDown = 1;
	m_ThumbDragX = point.x;

	if (m_rectLeftArrow.PtInRect(point))
	{
		m_ArrowClicked = MYOWNSLIDER_ARROW_LEFT;
		m_TimerID = SetTimer(MYOWNSLIDER_TIMERID_LBUTTON_DOWN_TIMER, MYOWNSLIDER_MOUSEDOWN_AUTOINC_DELAY, 0);
	}
	else if (m_rectRightArrow.PtInRect(point))
	{
		m_ArrowClicked = MYOWNSLIDER_ARROW_RIGHT;
		m_TimerID = SetTimer(MYOWNSLIDER_TIMERID_LBUTTON_DOWN_TIMER, MYOWNSLIDER_MOUSEDOWN_AUTOINC_DELAY, 0);
	}
	else if (m_polyThumb.PointEnclosed(point))
	{

	}
	else if (m_rectTrack.PtInRect((point)))
	{

	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CMyOwnSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_MouseIsDown = 0;
	if (m_rectLeftArrow.PtInRect(point))
	{
		m_Position++;
		if (m_Position > (m_MaxVal - m_MinVal)) m_Position = m_MaxVal - m_MinVal;
		PositionThumb();
		if (m_TimerID)
		{
			KillTimer(m_TimerID);
			m_TimerID = 0;
		}
		if (m_WmMessage)
			GetParent()->PostMessage(m_WmMessage, GetDlgCtrlID(), m_Position + m_MinVal);
		GetParent()->Invalidate();
	}
	else if (m_rectRightArrow.PtInRect(point))
	{
		m_Position--;
		if ((m_Position) < 0) m_Position = 0;
		PositionThumb();
		if (m_WmMessage)
			GetParent()->PostMessage(m_WmMessage, GetDlgCtrlID(), m_Position + m_MinVal);
		GetParent()->Invalidate();
	}
	else if (m_polyThumb.PointEnclosed(point))
	{

	}
	else if (m_rectTrack.PtInRect((point)))
	{

	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CMyOwnSlider::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseIsDown)
	{
		if (m_rectLeftArrow.PtInRect(point))
		{

		}
		else if (m_rectRightArrow.PtInRect(point))
		{

		}
		else if (m_polyThumb.PointEnclosed(point))
		{
			int x = point.x;

			if (m_MouseIsDown)
			{
				int deltaX;
				deltaX = x - m_ThumbDragX;
				m_XPosition += deltaX;
				if (m_XPosition < m_MinX)
					m_XPosition = m_MinX;
				else if (m_XPosition > m_MaxX)
					m_XPosition = m_MaxX;
				m_Position = XtoPos(m_XPosition);
				m_polyThumb.Offset( m_XPosition );
				m_ThumbDragX = x;
				if (m_WmMessage)
					GetParent()->PostMessage(m_WmMessage, GetDlgCtrlID(), m_Position);
				GetParent()->Invalidate();
			}

		}
		else if (m_rectTrack.PtInRect((point)))
		{

		}
	}
	CWnd::OnMouseMove(nFlags, point);
}


void CMyOwnSlider::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages
}

BOOL CMyOwnSlider::OnEraseBkgnd(CDC* pDC)
{
	return true;
}


bool CMyOwnSlider::Create(
	CPoint ptControlOrigin,//upper left corner 1
	int SliderHeight,	//slider height 2
	int SliderArrowWidth,	// width of arrows 3
	int TrackWidth,		//width of track 4
	int TrackHeight,	//height of track 5
	int ThumbWidth,		//width of thumb 6
	int TickHeightEnds,		//height of end tics 7
	int TickHeightMiddle,	//height of the middle tick 8
	int TickHeightMajor,	//major tick marks 9
	int TickIncrementMajor,	//Increment of Ticks 10
	int TickHeightMinor,	//Tick Height of the other ticks 11
	int TickIncrementMinor,	//spacing between ALL ticks 12
	int TotalTicks,		//Number of ticks marks on scale 13
	CSize szDisplaySize,	// size of display box 14
	int Spacing,	//Space between slider and display
	CSize szDisplayMargins,	//Spacing between dispolay text and box
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
	UINT nNumBitmaps,	//number of bitmaps
	UINT nSliderStyle,	// Style of slider
	CString csCaption,	//name of the control
	int captionHeight,	//height of the caption
	int DisplayOffset	//amount to offset display by
)
{
	int x, y;
	int cx, cy;
	bool boolRV;
	CDC* pDC;

	m_DisplayOffset = DisplayOffset;
	m_SliderStyle = nSliderStyle;
	pDC = pParentWnd->GetDC();
	m_csCaption = csCaption;

	m_ScaleBottom = SliderHeight + TickHeightEnds + 4;
	cy = m_ScaleBottom + captionHeight;
	m_ScaleBottom -= 1;
	if (cy < szDisplaySize.cy)
		cy = szDisplaySize.cy;
	cx = TrackWidth + ThumbWidth / 2 + SliderArrowWidth * 2 + Spacing + szDisplaySize.cx;
	m_rectClient = CRect(ptControlOrigin, CSize(cx, cy));
	//-----------------------------------------
	LPCTSTR lpszClassName = _T("BitmapComboBox");
	LPCTSTR lpszWindowName = nullptr;
	CCreateContext* pContext = nullptr;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	//-----------------------------------------
	m_WmMessage = WmMessage;
	m_MaxVal = MaxValue;
	m_MinVal = MinValue;
	m_pointOrigin = ptControlOrigin;
	m_TrackSlideWidth = TrackWidth;
	//-------------------------------------
	// 	   Bitmaps
	//-------------------------------------
	m_NumberOfBitmaps = nNumBitmaps;
	if (nNumBitmaps)
	{
		m_ppbmBitmaps = new CMyBitmap * [nNumBitmaps];
	}
	//-------------------------------------
	// 	   Scale
	//------------------------------------
	m_ScaleStartX = SliderArrowWidth + TrackWidth + ThumbWidth / 2;
	m_ScaleEndX = SliderArrowWidth + ThumbWidth / 2;
	m_TickHeightEnds = TickHeightEnds;		//height of end tics
	m_TickHeightMiddle = TickHeightMiddle;	//height of the middle tick
	m_TickHeightMajor = TickHeightMajor;	//major tick marks
	m_TickIncrementMajor = TickIncrementMajor;	//Increment of Ticks
	m_TickHeightMinor = TickHeightMinor;	//Tick Height of the other ticks
	m_TickIncrementMinor = TickIncrementMinor;//Spacing between all tics
	m_TotalTicks = TotalTicks;		//Number of ticks marks on scale
	//-------------------------------------
	// Arrows
	//-------------------------------------
	x = y = 0;
	cy = SliderHeight;
	cx = SliderArrowWidth;
	m_rectLeftArrow = CRect(CPoint(x, y), CSize(cx, cy));
	m_triangleLeftArrow[0] = CPoint(x,cy/2);
	m_triangleLeftArrow[1] = CPoint(x + cx-1, cy-1);
	m_triangleLeftArrow[2] = CPoint(x + cx-1, y);
	x = TrackWidth + ThumbWidth + SliderArrowWidth;
	m_rectRightArrow = CRect(CPoint(x, y), CSize(cx, cy));
	m_triangleRightArrow[0] = CPoint(x, y);
	m_triangleRightArrow[1] = CPoint(x, y + cy-1);
	m_triangleRightArrow[2] = CPoint(x + cx-1, cy / 2);

	//------------------------------------------
	// Track
	//------------------------------------------
	x = SliderArrowWidth;
	cx = TrackWidth + ThumbWidth;
	cy = TrackHeight;
	y = m_rectLeftArrow.CenterPoint().y - cy / 2;
	m_rectTrack = CRect(CPoint(x, y), CSize(cx, cy));
	//-------------------------------------------------
	//	Thumb
	//-------------------------------------------------
	m_pThumbPoints[0] = CPoint(0, 0);
	m_pThumbPoints[1] = CPoint(ThumbWidth, 0);
	m_pThumbPoints[2] = CPoint(ThumbWidth, SliderHeight - 2);
	m_pThumbPoints[3] = CPoint(ThumbWidth/2, SliderHeight + 2);
	m_pThumbPoints[4] = CPoint(0, SliderHeight - 2);
	m_polyThumb.Create(5, m_pThumbPoints);
	m_MinX = SliderArrowWidth;
	m_MaxX = m_MinX + m_TrackSlideWidth;
	m_XPosition = m_MaxX;
	m_polyThumb.Offset(m_MaxX);
	//------------------------------------------
	m_Ticks = m_MaxVal - m_MinVal;
	m_PosGapp = m_TrackSlideWidth - (m_TrackSlideWidth * m_Ticks) / m_Ticks;
	m_ThumXRef = SliderArrowWidth + m_TrackSlideWidth;
	//------------- Display ---------------------------
	m_pointTextStart = CPoint(
		m_rectClient.Width() - szDisplaySize.cx,
		0
	);
	m_rectDisplay = CRect(
		m_pointTextStart,
		szDisplaySize
	);
	y = szDisplaySize.cy - (2 * szDisplayMargins.cy - 1);
	m_fontHeight = GETAPP->FontPixelsToLogicalUnits(pDC, y);
	m_pointTextStart += szDisplayMargins;
	m_fontDisplay.CreateFontW(
		m_fontHeight,
		0,
		0,
		0,
		FW_DONTCARE,
		false,
		false,
		false,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		DEFAULT_PITCH,
		_T("Courier")
	);
	//----------- Captiohn --------------------------------
	m_CaptionTop = m_ScaleBottom + 1;
	y = GETAPP->FontPixelsToLogicalUnits(pDC, captionHeight);
//	y = -((captionHeight * logpix) / 76);
	m_fontCaption.CreateFontW(
		y,
		0,
		0,
		0,
		FW_DONTCARE,
		false,
		false,
		false,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		DEFAULT_PITCH,
		_T("Courier")
	);

	//----------- slider general attributes -----------------
	m_colorArrow = ArrowColor;
	m_colorArrowBox = ArrowBoxColor;
	m_colorThumb = ThumbColor;
	m_colorTrack = TrackColor;
	m_colorThumbPointer = ThumbPointerColor;
	m_colorBackGround = SliderBackgroundColor;
	m_colorTextBackGround = TextBackgroundColor;
	m_colorTextColor = TextColor;
	boolRV = CWnd::Create(
		lpszClassName, 
		lpszWindowName, 
		dwStyle, 
		m_rectClient, 
		pParentWnd, 
		nID, 
		pContext
	);
	PostMessage(WM_MYOWNSCROLLBAR_INIT);
	return boolRV;
}


void CMyOwnSlider::Draw(CDC* pDC)
{
	CPoint ptViewPort;
	CRect rectClient;
	CBrush brushArrow;
	CBrush brushArrowBox;
	CBrush brushTrack;
	CBrush brushThumb;
	CBrush brushBackground;
	CBrush* pbrushOld;
	CPen penBorder;
	CPen penArrowBox;
	CPen penArrow;
	CPen penTrack;
	CPen penThumb;
	CPen* ppenOld;

	if (IsWindowVisible() && IsWindowEnabled())
	{
		//control origin
		ptViewPort = pDC->SetViewportOrg(m_pointOrigin);
		GetClientRect(&rectClient);
		// Brushes
		brushBackground.CreateSolidBrush(m_colorBackGround);
		brushArrow.CreateSolidBrush(m_colorArrow);
		brushArrowBox.CreateSolidBrush(m_colorArrowBox);
		brushTrack.CreateSolidBrush(m_colorTrack);
		brushThumb.CreateSolidBrush(m_colorThumb);
		// Pens
		penArrowBox.CreatePen(PS_SOLID, 1, m_colorArrowBox ^ 0x00ffffff);
		penArrow.CreatePen(PS_SOLID, 1, m_colorArrow ^ 0x00ffffff);
		penTrack.CreatePen(PS_SOLID, 1, m_colorTrack);
		penThumb.CreatePen(PS_SOLID, 1, m_colorThumb);
		pDC->FillRect(&rectClient, &brushBackground);
		// Draw Scale
		DrawScale(pDC, rectClient);
		if (m_SliderStyle & MYOWNSLIDER_STYLE_BITMAP)
			DrawBitmaps(pDC);
		else if (m_SliderStyle & MYOWNSLIDER_STYLE_STRING_TABLE)
			DrawStrings(pDC);
		else
			DrawText(pDC);
		DrawCaption(pDC);
		//draw arrow background
		pbrushOld = pDC->SelectObject(&brushArrowBox);
		ppenOld = pDC->SelectObject(&penArrowBox);
		pDC->Rectangle(&m_rectLeftArrow);
		pDC->Rectangle(&m_rectRightArrow);
		//Draw Arrpws
		pDC->SelectObject(&brushArrow);
		pDC->SelectObject(&penArrow);
		pDC->Polygon(m_triangleLeftArrow, 3);
		pDC->Polygon(m_triangleRightArrow, 3);
		//Draw Track
		pDC->SelectObject(&brushTrack);
		pDC->SelectObject(&penTrack);
		pDC->Rectangle(&m_rectTrack);
		// Draw Thumb
		pDC->SelectObject(&brushThumb);
		pDC->SelectObject(&penThumb);
		m_polyThumb.Draw(pDC);
		m_polyThumb.DrawTick(pDC, m_colorThumbPointer);
		//cleanup
		pDC->SelectObject(ppenOld);
		pDC->SelectObject(pbrushOld);
		pDC->SetViewportOrg(ptViewPort);	//restore
	}
}

int CMyOwnSlider::XtoPos(int x)
{
	//-------------------------------
	// converts the x coordinate
	// to position.
	//-------------------------------
	double ratio;
	int diff;

	diff = m_MaxX - x;
	ratio = double(diff) / double(m_TrackSlideWidth);
	return int( ratio * double(m_Ticks));
}

void CMyOwnSlider::OnTimer(UINT_PTR nIDEvent)
{
	int XCorrection;

	switch (nIDEvent)
	{
		case MYOWNSLIDER_TIMERID_LEFTARROW:
			if (m_MouseIsDown)
			{
				m_Position++;
				if (m_Position > (m_MaxVal - m_MinVal)) m_Position = m_MaxVal - m_MinVal;
				m_XPosition = (m_Position * m_TrackSlideWidth) / m_Ticks;
				XCorrection = (m_XPosition * m_PosGapp) / m_TrackSlideWidth;
				m_XPosition += XCorrection;
				m_XPosition = m_ThumXRef - m_XPosition;
				m_polyThumb.Offset(m_XPosition);
				if (m_WmMessage)
					GetParent()->PostMessage(m_WmMessage, GetDlgCtrlID(), m_Position);
				GetParent()->Invalidate();
			}
			else
			{
				m_ArrowClicked = MYOWNSLIDER_NOARROW_CLICKED;
				KillTimer(m_TimerID);
				m_TimerID = 0;
			}
			break;
		case MYOWNSLIDER_TIMERID_RIGHTARROW:
			if (m_MouseIsDown)
			{
				m_Position--;
				if (m_Position < 0) m_Position = 0;
				m_XPosition = (m_Position * m_TrackSlideWidth) / m_Ticks;
				XCorrection = (m_PosGapp * m_XPosition) / m_TrackSlideWidth;
				m_XPosition += XCorrection;
				m_XPosition = m_ThumXRef - m_XPosition;
				m_polyThumb.Offset(m_XPosition);
				if (m_WmMessage)
					GetParent()->PostMessage(m_WmMessage, GetDlgCtrlID(), m_Position + m_MinVal);
				GetParent()->Invalidate();
			}
			else
			{
				m_ArrowClicked = MYOWNSLIDER_NOARROW_CLICKED;
				KillTimer(m_TimerID);
				m_TimerID = 0;
			}
			break;
		case MYOWNSLIDER_TIMERID_LBUTTON_DOWN_TIMER:
			if (m_MouseIsDown)
			{
				KillTimer(m_TimerID);
				if (m_ArrowClicked == MYOWNSLIDER_ARROW_LEFT)
				{
					m_TimerID = SetTimer(MYOWNSLIDER_TIMERID_LEFTARROW, 10, 0);
				}
				else if (m_ArrowClicked == MYOWNSLIDER_ARROW_RIGHT)
				{
					m_TimerID = SetTimer(MYOWNSLIDER_TIMERID_RIGHTARROW, 10, 0);
				}
			}
			else
			{
				if (m_TimerID)
				{
					KillTimer(m_TimerID);
					m_TimerID = 0;
				}
			}
			break;
		default:
			break;
	}
	CWnd::OnTimer(nIDEvent);
}

int CMyOwnSlider::PosToX()
{
	return m_MaxX - (m_Position * m_TrackSlideWidth) / m_Ticks;
}

void CMyOwnSlider::DrawStrings(CDC* pDC)
{
	int x, y;
	CPoint ptCenter;
	COLORREF oldText, oldBackground;
	CBrush brBKG;
	CFont* oldFont;
	int digits;

	digits = NumberOfDigits(m_Position);
	brBKG.CreateSolidBrush(m_colorTextBackGround);
	oldText = pDC->SetTextColor(m_colorTextColor);
	oldBackground = pDC->SetBkColor(m_colorTextBackGround);
	pDC->FillRect(&m_rectDisplay, &brBKG);
	ptCenter = m_rectDisplay.CenterPoint();
	x = m_rectDisplay.left + m_rectDisplay.Width() / 4;
	y = m_rectDisplay.top + m_rectDisplay.Height() / 4;
	oldFont = pDC->SelectObject(&m_fontDisplay);
	pDC->TextOutW(
		m_pointTextStart.x, 
		m_pointTextStart.y, 
		m_pcsStringTablel[m_Position + m_MinVal]
	);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldText);
	pDC->SetBkColor(oldBackground);
}

void CMyOwnSlider::DrawScale(CDC* pDC, CRect& rectClident)
{
	int y;
	int x;
	int Xmiddle;

	y = m_ScaleBottom;
	CPen whitePen, * oldPen;
	whitePen.CreatePen(PS_SOLID, 1, m_colorScale);
	oldPen = pDC->SelectObject(&whitePen);
	//Draw main line
	pDC->MoveTo(CPoint(m_ScaleStartX, y));
	pDC->LineTo(CPoint(m_ScaleEndX, y));
	// Draw vertical End ticks
	pDC->LineTo(CPoint(m_ScaleEndX, y - m_TickHeightEnds));
	pDC->MoveTo(CPoint(m_ScaleStartX, y));
	pDC->LineTo(CPoint(m_ScaleStartX, y - m_TickHeightEnds));
	//draw all of the tick marks
	Xmiddle = m_TotalTicks % 2;	//even?  odd?
	if (Xmiddle)
		Xmiddle = -1;
	else
		Xmiddle = (m_TotalTicks * m_TickIncrementMinor) / 2;
	for (int i = 0; i < m_TotalTicks; ++i)
	{
		x = (i + 1) * m_TickIncrementMinor;
		pDC->MoveTo(m_ScaleStartX - x, y);
		if (x == Xmiddle)
			pDC->LineTo(m_ScaleStartX - x, y - m_TickHeightMiddle);
		else if (!(x % m_TickIncrementMajor))
			pDC->LineTo(m_ScaleStartX - x, y - m_TickHeightMajor);
		else
			pDC->LineTo(m_ScaleStartX - x, y - m_TickHeightMinor);
	}
	pDC->SelectObject(oldPen);
}

void CMyOwnSlider::DrawText(CDC* pDC)
{
	int x, y;
	CPoint ptCenter;
	CString csText;
	COLORREF oldText, oldBackground;
	CBrush brBKG;
	CFont* oldFont;
	int digits;
	CString formatString;

	digits = NumberOfDigits(m_Position);
	formatString.Format(_T("%c%dd"),'%', digits);
	brBKG.CreateSolidBrush(m_colorTextBackGround);
	oldText = pDC->SetTextColor(m_colorTextColor);
	oldBackground = pDC->SetBkColor(m_colorTextBackGround);
	pDC->FillRect(&m_rectDisplay,&brBKG);
	ptCenter = m_rectDisplay.CenterPoint();
	x = m_rectDisplay.left + m_rectDisplay.Width() / 4;
	y = m_rectDisplay.top + m_rectDisplay.Height() / 4;
	csText.Format(formatString, m_Position + m_MinVal - m_DisplayOffset);
	oldFont = pDC->SelectObject(&m_fontDisplay);
	pDC->TextOutW(m_pointTextStart.x, m_pointTextStart.y, csText);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldText);
	pDC->SetBkColor(oldBackground);
}

void CMyOwnSlider::DrawCaption(CDC* pDC)
{
	COLORREF oldTextColor, oldBackgroundColor;
	CFont* oldFont;

	oldTextColor = pDC->SetTextColor(m_colorTextColor);
	oldBackgroundColor = pDC->SetBkColor(m_colorTextBackGround);
	oldFont = pDC->SelectObject(&m_fontCaption);
	pDC->TextOutW(1, m_CaptionTop, m_csCaption);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldTextColor);
	pDC->SetBkColor(oldBackgroundColor);

}

void CMyOwnSlider::DrawBitmaps(CDC* pDC)
{
	CDC bmDC;

	bmDC.CreateCompatibleDC(pDC);
	bmDC.SelectObject(m_ppbmBitmaps[m_Position]);
	pDC->BitBlt(
		m_rectDisplay.left,
		m_rectDisplay.top,
		m_rectDisplay.Width(),
		m_rectDisplay.Height(),
		&bmDC,
		0,
		0,
		SRCCOPY
	);
	bmDC.DeleteDC();
}

int CMyOwnSlider::NumberOfDigits(int v)
{
	int digits = 1;

	while (v > 10)
	{
		digits += 1;
		v /= 10;
	}
	return digits;
}

void CMyOwnSlider::PositionThumb()
{
	int XCorrection;

	m_XPosition = PosToX();
	XCorrection = (m_XPosition * m_PosGapp) / m_TrackSlideWidth;
	m_XPosition += XCorrection;
	m_polyThumb.Offset(m_XPosition);
}
