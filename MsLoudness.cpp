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

bool CMsLoudness::Create(CMsSong* pSong, CMsEvent* pEvent, UINT Loudness)
{
	SetLoudness(Loudness);
	pSong->SetCurrentLoudness(this);
	return CMsObject::Create(pSong, pEvent);
}

UINT CMsLoudness::Process()
{
	return 1;
}

UINT CMsLoudness::Play()
{
	GetSong()->SetCurrentLoudness(this);
	return PLAY_OBJECT_DONE;
}

DRAWSTATE CMsLoudness::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsLoudness::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsLoudness::MouseMove(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

void CMsLoudness::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO,"%sLoudness:%d\n", pIndentString, m_Loudness);
	delete[] pIndentString;
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
