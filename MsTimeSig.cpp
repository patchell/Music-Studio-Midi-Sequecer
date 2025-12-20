// MsTimeSig.cpp: implementation of the CMsTimeSig class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "MsTimeSig.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsTimeSignature::CMsTimeSignature():CMsObject()
{
	m_ObjType = CMsObject::MsObjType::TIMESIG;
	m_TimeSig = 0;
}

CMsTimeSignature::~CMsTimeSignature()
{

}

bool CMsTimeSignature::Create(CMsSong* pSong, CMsEvent* pEvent, INT TS)
{
	m_TimeSig = TS;
	pSong->SetCurrentTimeSignature(this);
	return CMsObject::Create(pSong, pEvent);
}

UINT CMsTimeSignature::Process()
{
	return 1;
}

UINT CMsTimeSignature::Play()
{
	GetSong()->SetCurrentTimeSignature(this);
	return PLAY_OBJECT_DONE;
}

DRAWSTATE CMsTimeSignature::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	DrawState = DRAWSTATE::PLACE;
	return DrawState;
}

DRAWSTATE CMsTimeSignature::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	CMsTimeSignature* pTS = nullptr;
	CString csText;
	int EventIndex = 0;
	CMsEvent* pEvent = nullptr;

	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::PLACE:
		EventIndex = GetStaffChildView()->GetDrawEvent();
		pEvent = GetSong()->GetEventObject(EventIndex);
		pEvent->AddObject(this);
		//-----------------------------
		pTS = new CMsTimeSignature;
		pTS->Create(GetSong(), 0, 0);
		GetStaffChildView()->SetDrawObject(pTS);
		GetStaffChildView()->CheckAndDoScroll(pointMouse);
		csText.Format(_T("Place Time Signature at Event %d"), GetStaffChildView()->GetDrawEvent());
		GetStaffChildView()->GetStatusBar()->SetText(csText);
		DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
		GetStaffChildView()->Invalidate();
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsTimeSignature::MouseMove(DRAWSTATE DrawState, CPoint pointMouse)
{
	CString csText;

	csText.Format(_T("Draw Time Signature at event %d"), GetStaffChildView()->GetDrawEvent());
	GetStaffChildView()->GetStatusBar()->SetText(csText);
	GetStaffChildView()->Invalidate();
	return DrawState;
}

UINT CMsTimeSignature::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

void CMsTimeSignature::ObjectRectangle(CRect& rect, UINT Event)
{
}

static const char *TimeSigLut[] = {
	"NA",		//0
	"2/2",		//1
	"3/2",		//2
	"2/4",		//3
	"3/4",		//4
	"4/4",		//5
	"5/4",		//6
	"6/8",		//7
	"NA"
};

void CMsTimeSignature::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO,"%sTime Signature:%s\n", pIndentString, TimeSigLut[m_TimeSig]);
	delete[] pIndentString;
}

void CMsTimeSignature::Draw(CDC *pDC, int event, int maxevent)
{
	int x;
	CBitmap* oldBitmap;
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	x = EVENT_OFFSET+EVENT_WIDTH*event;
	oldBitmap = dc.SelectObject(GETAPP->bmGetTimeSig(m_TimeSig));
	pDC->BitBlt(x,CENTER_OF_TREBEL,16,33,&dc,0,0,SRCAND);
	pDC->BitBlt(x,CENTER_OF_BASS,16,33,&dc,0,0,SRCAND);
	dc.SelectObject(oldBitmap);
}

void CMsTimeSignature::Copy(CMsObject* pSource)
{
	m_TimeSig = ((CMsTimeSignature*)pSource)->m_TimeSig;
	CMsObject::Copy(pSource);
}

void CMsTimeSignature::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_TIME_SIGNATURE,pO);	//token
	fputc((int)m_TimeSig,pO);			//value
}
