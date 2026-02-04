// MsFlat.cpp: implementation of the CMsFlat class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsFlat::CMsFlat() :CMsObject(CMsObject::MsObjType::FLAT)
{

}

CMsFlat::~CMsFlat()
{

}

bool CMsFlat::Create(CMsSong* pSong)
{
    return false;
}

UINT CMsFlat::Process()
{
	return 0;
}

UINT CMsFlat::Play()
{
	return 0;
}

DRAWSTATE CMsFlat::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsFlat::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsFlat::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	return DrawState;
}

void CMsFlat::Draw(CDC *pDC, COLORREF c, int x, int y)
{
	CPen pen,*oldpen;

	pen.CreatePen(PS_SOLID,2,c);
	oldpen = pDC->SelectObject(&pen);
	pDC->MoveTo(x-2,y-4);
	pDC->LineTo(x-2,y+6);
	pDC->SetPixel(x-1,y+6,c);
	pDC->SetPixel(x,y+5,c);
	pDC->SetPixel(x+1,y+5,c);
	pDC->SetPixel(x+2,y+4,c);
	pDC->SetPixel(x+2,y+3,c);
	pDC->SetPixel(x+1,y+2,c);
	pDC->SetPixel(x,y+1,c);
	pDC->SetPixel(x-1,y+2,c);
	pDC->SetPixel(x-1,y+2,c);
	pDC->SelectObject(oldpen);
}

UINT CMsFlat::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

StaffMouseStates CMsFlat::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
{
	return StaffMouseStates::MOUSE_STAFF_STATE_NONE;
}	
