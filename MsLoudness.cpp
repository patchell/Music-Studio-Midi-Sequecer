// MsLoudness.cpp: implementation of the CMsLoudness class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsLoudness::CMsLoudness():CMsObject()
{
	m_ObjType = MSOBJ_LOUDNESS;
	m_Loudness = 100;
}

CMsLoudness::~CMsLoudness()
{

}

void CMsLoudness::Create(CMsSong* pSong, UINT ParentEvent, UINT Loudness)
{
	SetLoudness(Loudness);
	CMsObject::Create(pSong, ParentEvent);
}

void CMsLoudness::Print(FILE *pO)
{
	fprintf(pO,"Loudness:%d\n",m_Loudness);
}

UINT CMsLoudness::Process()
{
	GetSong()->SetCurrentLoudness(this);
	return 0;
}

UINT CMsLoudness::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

void CMsLoudness::ObjectRectangle(CRect& rect, UINT Event)
{
}

void CMsLoudness::Draw(CDC *pDC, int event, int maxevent)
{
	int x,y;
	CFont* oldFont;
	CString csText;

	oldFont = pDC->SelectObject(GETAPP->GetFont());
	x = EVENT_OFFSET+EVENT_WIDTH*event;
	y = STAVE_OFFSET-16;
	csText.Format(_T("V="));
	pDC->TextOut(x,y-12, csText);
	csText.Format(_T("%d"), m_Loudness);
	pDC->TextOut(x,y, csText);
	pDC->SelectObject(oldFont);
}

CMsObject * CMsLoudness::Copy()
{
	CMsObject *pOb = 0;
	CMsLoudness *pB = new CMsLoudness;
	*pB = *this;
	pOb = pB;
	return pOb;
}

void CMsLoudness::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_LOUDNESS,pO);	//token
	fputc(0x01,pO);	//who knows what this is for
	fputc(m_Loudness,pO);	//value
}
