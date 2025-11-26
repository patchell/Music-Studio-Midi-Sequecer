// MsEnd.cpp: implementation of the CMsEndBar class.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsEndBar::CMsEndBar():CMsObject()
{
	m_ObjType = MSOBJ_ENDBAR;
}

CMsEndBar::~CMsEndBar()
{

}

bool CMsEndBar::Create(CMsSong* pSong, CMsEvent* pParentEvent)
{
	bool rV = true;
	rV = CMsObject::Create(pSong, pParentEvent);
	return rV;
}


UINT CMsEndBar::Play()
{
	UINT rV = PLAY_OBJECT_DONE;	//means the song has ended
	GetSong()->GetStaffChildView()->PostMessageW(
		WM_STAFF_DISP_EVENT,
		0,
		STAFF_DISP_EVENT_END
	);
	GetSong()->SetTickerState(CMsSong::TickerState::WIND_DOWN);
	return rV;
}

UINT CMsEndBar::Process()
{
	return 0;
}

int CMsEndBar::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsEndBar::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsEndBar::MouseMove(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

void CMsEndBar::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO,"%s-------Song End------\n", pIndentString);
	delete[] pIndentString;
}

void CMsEndBar::Draw(CDC *pDC, int event, int maxevent)
{
	CPen Heavy, *pOldPen;
	LOGBRUSH Lb;
	Lb.lbColor = RGB(0, 0, 0);
	Lb.lbStyle = BS_SOLID;
	Lb.lbHatch = 0;
	CRect rectHighlight;
	COLORREF colorHLrect = RGB(255,0,0);
	CPen penHLrect;
	CGdiObject* p_oldObject;

	Heavy.CreatePen(PS_GEOMETRIC | PS_ENDCAP_FLAT, 6, &Lb);
	int x = EVENT_OFFSET + EVENT_WIDTH*event + MEASUREBAR_OFFSET;
	pDC->MoveTo(x - 6, STAVE_OFFSET);
	pDC->LineTo(x - 6, STAVE_OFFSET + STAVE_HEIGHT);
	pOldPen = pDC->SelectObject(&Heavy);
	pDC->MoveTo(x, STAVE_OFFSET);
	pDC->LineTo(x, STAVE_OFFSET + STAVE_HEIGHT);
	pDC->SelectObject(pOldPen);
	if (IsSelected() || IsHighLighted())
	{
		CRect rectHL;
		UINT Event;

		Event = GetParentEvent()->GetIndex();
		rectHL.SetRect(EVENT_OFFSET + EVENT_WIDTH * Event - 4, STAVE_OFFSET, EVENT_OFFSET + EVENT_WIDTH * Event + 5, STAVE_OFFSET + STAVE_HEIGHT);

		penHLrect.CreatePen(PS_SOLID, 3, colorHLrect);
		pOldPen = pDC->SelectObject(&penHLrect);
		p_oldObject = pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(&rectHL);
		pDC->SelectObject(p_oldObject);
		pDC->SelectObject(pOldPen);
	}
}

UINT CMsEndBar::ObjectToString(CString& csString, UINT mode)
{
	csString.Format(_T("End Bar Event = %d"), GetParentEvent()->GetIndex());
	return csString.GetLength();
}

void CMsEndBar::ObjectRectangle(CRect& rect, UINT Event)
{
	rect.SetRect((EVENT_OFFSET + EVENT_WIDTH * Event) - 4, STAVE_OFFSET, EVENT_OFFSET + EVENT_WIDTH * Event + 5, STAVE_OFFSET + STAVE_HEIGHT);
}

CMsObject * CMsEndBar::Copy()
{
	CMsObject *pOb = 0;
	CMsEndBar *pTS = new CMsEndBar;
	*pTS = *this;
	pOb = pTS;
	return pOb;
}

void CMsEndBar::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_END,pO);
}
