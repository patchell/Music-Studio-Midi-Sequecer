// MsRepeatEnd.cpp: implementation of the CMsRepeatEnd class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsRepeatEnd::CMsRepeatEnd():CMsObject()
{
	m_ObjType = MSOBJ_REPEATEND;
}

CMsRepeatEnd::~CMsRepeatEnd()
{

}

void CMsRepeatEnd::Print(FILE *pO)
{
	fprintf(pO,"Repeat End :|\n");
}

UINT CMsRepeatEnd::Process()
{
	CMsEvent* pRepeatStartEvent;
	CMsRepeatStart* pRPS;

	pRepeatStartEvent = (CMsEvent*)GetSong()->GetRepeatStack().PeakTop();
	pRPS = (CMsRepeatStart*)pRepeatStartEvent->ContainsObjectType(MSOBJ_REPEATSTART);
	printf("Repeat end Countdown %d\n",pRPS->GetCountDown());
	if (pRPS->DecrementRepeatCount())
	{
		//------------------------------
		// Done With Repeat Loop
		//-------------------------------
		GetSong()->GetRepeatStack().PopLIFO();
	}
	else
	{
		//-----------------------------
		// Keep Looping
		//-----------------------------
		GetSong()->SetSongPosition(pRepeatStartEvent);
	}
	return 0;
}

UINT CMsRepeatEnd::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

void CMsRepeatEnd::ObjectRectangle(CRect& rect, UINT Event)
{
}

void CMsRepeatEnd::Draw(CDC *pDC, int event, int maxevent)
{
	CPen Light, Heavy, *pOld;
	LOGBRUSH Lb;
	if (IsSelected())
	{
		Light.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		Lb.lbColor = RGB(255, 0, 0);
	}
	else
	{
		Light.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		Lb.lbColor = RGB(0, 0, 0);
	}
	Lb.lbStyle = BS_SOLID;
	Lb.lbHatch = 0;

	Heavy.CreatePen(PS_GEOMETRIC | PS_ENDCAP_FLAT,6,&Lb);
	int x = EVENT_OFFSET+EVENT_WIDTH*event+MEASUREBAR_OFFSET;
	pOld = pDC->SelectObject(&Light);
	pDC->MoveTo(x-6,STAVE_OFFSET);
	pDC->LineTo(x-6,STAVE_OFFSET+STAVE_HEIGHT);
	pDC->SelectObject(&Heavy);
	pDC->MoveTo(x,STAVE_OFFSET);
	pDC->LineTo(x,STAVE_OFFSET+STAVE_HEIGHT);
	pDC->SetPixel(x - 8, STAVE_OFFSET + 12, RGB(0, 0, 0));
	pDC->SetPixel(x - 8, STAVE_OFFSET + 20, RGB(0, 0, 0));
	pDC->SetPixel(x - 8, STAVE_OFFSET + 60, RGB(0, 0, 0));
	pDC->SetPixel(x - 8, STAVE_OFFSET + 68, RGB(0, 0, 0));
	pDC->SelectObject(pOld);
}

CMsObject * CMsRepeatEnd::Copy()
{
	CMsObject *pOb = 0;
	CMsRepeatEnd *pRE = new CMsRepeatEnd;
	*pRE = *this;
	pOb = pRE;
	return pOb;
}

void CMsRepeatEnd::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_REPEAT_STOP,pO);	//token
}

void CMsRepeatEnd::Create(CMsSong* pSong, UINT ParentEvent)
{
	CMsObject::Create(pSong, ParentEvent);
}

void CMsRepeatEnd::Create(CMsSong* pSong, CMsEvent* pEvent)
{
	CMsObject::Create(pSong, pEvent);
}
