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
	m_ObjType = MSOBJ_TIMESIG;
	m_TimeSig = 0;
}

CMsTimeSignature::~CMsTimeSignature()
{

}

void CMsTimeSignature::Create(CMsSong* pSong, UINT ParentEvent, INT TS)
{
	m_TimeSig = TS;
	CMsObject::Create(pSong, ParentEvent);
}

UINT CMsTimeSignature::Process()
{
	GetSong()->SetCurrentTimeSignature(this);
	return 0;
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

void CMsTimeSignature::Print(FILE *pO)
{
	fprintf(pO,"Time Signature:%s\n",TimeSigLut[m_TimeSig]);
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

CMsObject * CMsTimeSignature::Copy()
{
	CMsObject *pOb = 0;
	CMsTimeSignature*pTS = new CMsTimeSignature;
	*pTS = *this;
	pOb = pTS;
	return pOb;
}

void CMsTimeSignature::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_TIME_SIGNATURE,pO);	//token
	fputc((int)m_TimeSig,pO);			//value
}
