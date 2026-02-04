// MsNatural.cpp: implementation of the CMsNatural class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsNatural::CMsNatural() : CMsObject(CMsObject::MsObjType::NATURAL)
{

}

CMsNatural::~CMsNatural()
{

}

UINT CMsNatural::Process()
{
	return 0;
}

UINT CMsNatural::Play()
{
	return 0;
}

DRAWSTATE CMsNatural::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsNatural::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsNatural::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	return DrawState;
}

void CMsNatural::Draw(CDC *pDC, COLORREF c, int x, int y)
{
	CPen pen,*oldpen;
	pen.CreatePen(PS_SOLID,2,c);
	oldpen = pDC->SelectObject(&pen);
	pDC->MoveTo(x-2,y-4);
	pDC->LineTo(x-2,y+2);
	pDC->LineTo(x+2,y+2);
	pDC->MoveTo(x+2,y+4);
	pDC->LineTo(x+2,y-2);
	pDC->LineTo(x-2,y-2);
	pDC->SelectObject(oldpen);
}

UINT CMsNatural::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}
