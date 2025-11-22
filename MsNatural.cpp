// MsNatural.cpp: implementation of the CMsNatural class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsNatural::CMsNatural()
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

int CMsNatural::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsNatural::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsNatural::MouseMove(int DrawState, CPoint pointMouse)
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
