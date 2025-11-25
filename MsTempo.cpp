// MsTempo.cpp: implementation of the CMsTempo class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsTempo::CMsTempo():CMsObject()
{
	m_ObjType = MSOBJ_TEMPO;
	m_Tempo = 1;
}


CMsTempo::~CMsTempo()
{

}

UINT CMsTempo::Process()
{
	return 1;
}

UINT CMsTempo::Play()
{
	GetSong()->SetCurrentTempo(this);
	GETAPP->PlayerThreadSetTempo(GetQNPM());
	return PLAY_OBJECT_DONE;
}

int CMsTempo::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsTempo::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsTempo::MouseMove(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

void CMsTempo::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO,"%sTempo:%d\n", pIndentString, m_Tempo);
	delete[] pIndentString;
}

UINT CMsTempo::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

void CMsTempo::ObjectRectangle(CRect& rect, UINT Event)
{
}

void CMsTempo::Draw(CDC *pDC, int event, int maxevent)
{
	int x,y;
	char *s = new char[16];
	CDC dc;
	CBitmap* oldBM;
	COLORREF OldTextColor;
	int red = 0;
	CMyBitmap Bm;
	CPen BlkPen;
	CBrush BlkBr;
	CRect rect;
	CBrush WhtBr, *oldBr;
	CFont* oldFont;

	WhtBr.CreateSolidBrush(RGB(0, 255, 200));
	rect = CRect(CPoint(0, 0), CSize(15, 15));
	BlkBr.CreateSolidBrush(RGB(0, 0, 0));
	BlkPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	if (IsSelected()) red = 255;
	OldTextColor = pDC->SetTextColor(RGB(red, 0, 0));

	dc.CreateCompatibleDC(pDC);
	Bm.CreateBitmap(16, 16, 1, 32, NULL);
	oldBM = dc.SelectObject(&Bm);
	oldBr = dc.SelectObject(&BlkBr);
	dc.FillRect(&rect, &WhtBr);
	dc.Ellipse(CRect(CPoint(2, 10), CSize(6, 3)));
	dc.MoveTo(6, 11);
	dc.LineTo(6, 2);
	dc.MoveTo(8, 8);
	dc.LineTo(12, 8);
	dc.MoveTo(8, 11);
	dc.LineTo(12, 11);

	x = EVENT_OFFSET+EVENT_WIDTH*event;
	y = STAVE_OFFSET-16;

	oldFont = pDC->SelectObject(GETAPP->GetFont());
	pDC->BitBlt(x,y-16,16,16,&dc,0,0,SRCCOPY);
	int l;
	l = sprintf_s(s,16,"%d",m_Tempo);
	pDC->TextOut(x,y,CString(s),l);
	pDC->SetTextColor(OldTextColor);
	dc.SelectObject(oldBM);
	pDC->SelectObject(oldFont);
	delete[] s;
}

CMsObject * CMsTempo::Copy()
{
	CMsObject *pOb = 0;
	CMsTempo *pRS = new CMsTempo;
	*pRS = *this;
	pOb = pRS;
	return pOb;
}

void CMsTempo::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_TEMPO,pO);		//token	
	fputc(m_Tempo,pO);	//value
}

