// MsFlat.cpp: implementation of the CMsFlat class.
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

CMsFlat::CMsFlat()
{

}

CMsFlat::~CMsFlat()
{

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
