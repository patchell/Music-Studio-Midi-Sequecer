#include "pch.h"

CMsBassCleff::CMsBassCleff() : CMsObject(MsObjType::BASS_CLEF)
{
}

CMsBassCleff::~CMsBassCleff()
{
}

bool CMsBassCleff::Create(CMsSong* pSong, CMsEvent* pEvent)
{
	SetType(MsObjType::BASS_CLEF);
	return CMsObject::Create(pSong, pEvent);
}

//-------------------------------------------------
// Pure Virtual Methods
//-------------------------------------------------

UINT CMsBassCleff::Process()
{
	return 0;
}

UINT CMsBassCleff::Play()
{
	return 0;
}

DRAWSTATE CMsBassCleff::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsBassCleff::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsBassCleff::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	return DrawState;
}

UINT CMsBassCleff::ObjectToString(CString& csString, UINT mode)
{
	csString = "Bass Cleff";
	return csString.GetLength();
}

void CMsBassCleff::Draw(CDC* pDC)
{
	CMyBitmap Bass, * oldBM;
	CSize szBass;

	Bass.LoadBitmap(IDB_BASS_CLEFF);
	szBass = Bass.GetBmDim();
	szBass = Bass.GetBmDim();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	oldBM = (CMyBitmap*)memDC.SelectObject(&Bass);
	pDC->BitBlt(2, BASS_CLEF_Y, szBass.cx, szBass.cy, &memDC, 0, 0, SRCAND);
	memDC.SelectObject(oldBM);
}

StaffMouseStates CMsBassCleff::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
{
	return StaffMouseStates::MOUSE_STAFF_STATE_NONE;
}

