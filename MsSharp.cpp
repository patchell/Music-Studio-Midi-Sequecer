// MsSharp.cpp: implementation of the CMsSharp class.
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

CMsSharp::CMsSharp()
{

}

CMsSharp::~CMsSharp()
{

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

