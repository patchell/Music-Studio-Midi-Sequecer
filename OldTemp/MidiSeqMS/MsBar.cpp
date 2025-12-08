// MsBar.cpp: implementation of the CMsBar class.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UINT CMsBar::LastBarNumber = 0;

CMsBar::CMsBar():CMsObject()
{
	m_ObjType = MSOBJ_BAR;
	m_BarNumber = GenBarNumber();
}

CMsBar::~CMsBar()
{

}

bool CMsBar::Create(CMsSong* pSong, CMsEvent* pEvent)
{
	return CMsObject::Create(pSong, pEvent);
}

UINT CMsBar::Play()
{
	return 0;
}

int CMsBar::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsBar::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsBar::MouseMove(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

UINT CMsBar::Process()
{
	return 0;
}

void CMsBar::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
	fprintf(pO,"%sMeasure Bar:%d\n", pIndentString,m_BarNumber);
	delete[] pIndentString;
}


void CMsBar::Draw(CDC *pDC, int event, int maxevent)
{
	CPen penSelectHighlight;
	CPen penBlackLine;
	CRect rectHighlight;
	CPen* p_penOld;
	CGdiObject* p_oldObject;

	penBlackLine.CreatePen(1, PS_SOLID, RGB(0, 0, 0));
	penSelectHighlight.CreatePen(2, PS_SOLID, RGB(255, 0, 0));
	int x = EVENT_OFFSET+EVENT_WIDTH*event+MEASUREBAR_OFFSET;
	if (IsSelected() || IsHighLighted())
	{
		//----------------------------------------
		// Bar
		//-----------------------------------------
		CPoint pointUpperLeft;
		CSize szRectSize;
	
		pointUpperLeft = CPoint(x - 2, STAVE_OFFSET - 2);
		szRectSize = CSize(5, STAVE_OFFSET + STAVE_HEIGHT + 2);
		rectHighlight = CRect(pointUpperLeft, szRectSize);
		p_penOld = pDC->SelectObject(&penSelectHighlight);
		p_oldObject = pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(&rectHighlight);
		pDC->SelectObject(p_oldObject);
		pDC->SelectObject(p_penOld);
	}
	p_penOld = pDC->SelectObject(&penBlackLine);
	pDC->MoveTo(x,STAVE_OFFSET);
	pDC->LineTo(x,STAVE_OFFSET+STAVE_HEIGHT);
	pDC->SelectObject(p_penOld);
}

UINT CMsBar::Process(CMsSong* pSong)
{
	return 0;
}

void CMsBar::ObjectRectangle(CRect& rect, UINT Event)
{
	rect.SetRect((EVENT_OFFSET + EVENT_WIDTH * Event) -2, STAVE_OFFSET, EVENT_OFFSET + EVENT_WIDTH * Event + 2, STAVE_OFFSET + STAVE_HEIGHT);
}

UINT CMsBar::ObjectToString(CString& csString, UINT mode)
{
	csString.Format(_T("Measure Bar %d at %d"), m_BarNumber, GetParentEvent()->GetIndex());
	return csString.GetLength();
}

CMsObject* CMsBar::MakeANewObject()
{
	printf("Make a new Bar\n");
	CMsBar* pBar;
	pBar = new CMsBar;
	return pBar;
}

 void CMsBar::Copy(CMsObject* pSource)
{
	 CMsBar* pBar = (CMsBar*)pSource;
	CMsObject::Copy(pSource);
	m_BarNumber = GenBarNumber();
}

void CMsBar::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_BAR,pO);
}

