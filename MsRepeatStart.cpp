// MsRepeatStart.cpp: implementation of the CMsRepeatStart class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsRepeatStart::CMsRepeatStart() :CMsObject()
{
	m_ObjType = MSOBJ_REPEATSTART;
	m_Count = 1;
	m_CountDown = 0;
}

CMsRepeatStart::~CMsRepeatStart()
{

}

bool CMsRepeatStart::Create(CMsSong* pSong, UINT Count, CMsEvent* pEvent)
{
	bool rV = true;
	rV = CMsObject::Create(pSong, pEvent);
	m_Count = Count;
	return rV;
}

UINT CMsRepeatStart::Process()
{
	m_CountDown = m_Count;
	CMsEvent* pEV = GetParentEvent();
	GetSong()->GetRepeatStack().Push(pEV);
	printf("Repeat Start CD=%d\n", m_CountDown);
	return 0;
}

UINT CMsRepeatStart::Play()
{
	return PLAY_OBJECT_DONE;
}

int CMsRepeatStart::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsRepeatStart::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsRepeatStart::MouseMove(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

void CMsRepeatStart::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
	fprintf(pO,"%sRepeat Start |:(%d)\n", pIndentString, m_Count);
	delete[] pIndentString;
}


void CMsRepeatStart::Draw(CDC *pDC, int event, int maxevent)
{
	CPen Light,Heavy,*pOld;
	COLORREF oldcolor;
	LOGBRUSH Lb;
	CString csCount;
	if (IsSelected())
	{
		oldcolor = pDC->SetTextColor(RGB(255, 0, 0));
		Light.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		Lb.lbColor = RGB(255, 0, 0);
	}
	else
	{
		oldcolor = pDC->SetTextColor(RGB(0, 0, 0));
		Light.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		Lb.lbColor = RGB(0, 0, 0);
	}
	Lb.lbStyle = BS_SOLID;
	Lb.lbHatch = 0;

	Heavy.CreatePen(PS_GEOMETRIC | PS_ENDCAP_FLAT,6,&Lb);
	int x = EVENT_OFFSET+EVENT_WIDTH*event+MEASUREBAR_OFFSET;
	pOld = pDC->SelectObject(&Light);
	pDC->MoveTo(x,STAVE_OFFSET);
	pDC->LineTo(x,STAVE_OFFSET+STAVE_HEIGHT);
	pDC->SetPixel(x+4,STAVE_OFFSET+12,RGB(0,0,0));
	pDC->SetPixel(x+4,STAVE_OFFSET+20,RGB(0,0,0));
	pDC->SetPixel(x+4,STAVE_OFFSET+60,RGB(0,0,0));
	pDC->SetPixel(x+4,STAVE_OFFSET+68,RGB(0,0,0));
	pDC->SelectObject(&Heavy);
	pDC->MoveTo(x-6,STAVE_OFFSET);
	pDC->LineTo(x-6,STAVE_OFFSET+STAVE_HEIGHT);
	char *s = new char[16];
	if (GetSong()->IsPlaying())
		csCount.Format(_T("%d"), m_CountDown);
	else
		csCount.Format(_T("%d"), m_Count);
	pDC->TextOutW(x - 20, STAVE_OFFSET - 16, csCount);
	delete[] s;
	pDC->SelectObject(pOld);
	pDC->SetTextColor(oldcolor);
}

UINT CMsRepeatStart::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

void CMsRepeatStart::ObjectRectangle(CRect& rect, UINT Event)
{
}

CMsObject * CMsRepeatStart::Copy()
{
	CMsObject *pOb = 0;
	CMsRepeatStart *pRS = new CMsRepeatStart;
	*pRS = *this;
	pOb = pRS;
	return pOb;
}

void CMsRepeatStart::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_REPEAT_START,pO);	//token	
	fputc(m_Count,pO);				//value
}
