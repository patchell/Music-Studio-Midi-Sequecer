// MsSharp.cpp: implementation of the CMsSharp class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsSharp::CMsSharp()
{

}

CMsSharp::~CMsSharp()
{

}

bool CMsSharp::Create(CMsSong* pSong)
{
	CMsObject::Create(pSong, 0);
	return true;
}

UINT CMsSharp::Process()
{
	return 0;
}

UINT CMsSharp::Play()
{
	return 0;
}

DRAWSTATE CMsSharp::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsSharp::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsSharp::MouseMove(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

void CMsSharp::Draw(CDC *pDC, COLORREF c, int x, int y)
{
	CPen pen,*oldpen;

	y+=4;
	pen.CreatePen(PS_SOLID,1,c);
	oldpen = pDC->SelectObject(&pen);
	pDC->MoveTo(x-4,y-2);
	pDC->LineTo(x+4,y-2);
	pDC->MoveTo(x-4,y+2);
	pDC->LineTo(x+4,y+2);
	pDC->MoveTo(x-2,y-4);
	pDC->LineTo(x-2,y+4);
	pDC->MoveTo(x+2,y-4);
	pDC->LineTo(x+2,y+4);
	pDC->SelectObject(oldpen);
}

