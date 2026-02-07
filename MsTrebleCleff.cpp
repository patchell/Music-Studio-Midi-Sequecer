#include "pch.h"

CMsTrebleCleff::CMsTrebleCleff() : CMsObject(MsObjType::TREBLE_CLEF)
{
}

CMsTrebleCleff::~CMsTrebleCleff()
{
}

bool CMsTrebleCleff::Create(CMsSong* pSong, CMsEvent* pEvent)
{
	return CMsObject::Create(pSong, pEvent);
}

//-------------------------------------------------
// Pure Virtual Methods
//-------------------------------------------------

UINT CMsTrebleCleff::Process()
{
	return 0;
}

UINT CMsTrebleCleff::Play()
{
	return 0;
}

DRAWSTATE CMsTrebleCleff::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsTrebleCleff::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsTrebleCleff::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	return DrawState;
}

UINT CMsTrebleCleff::ObjectToString(CString& csString, UINT mode)
{
	csString = "Treble Cleff";
	return csString.GetLength();
}

void CMsTrebleCleff::Draw(CDC* pDC)
{
	CMyBitmap Treb, * oldBM;
	CSize szBass, szTreb;

	Treb.LoadBitmap(IDB_TREBLE_CLEFF);
	szTreb = Treb.GetBmDim();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	oldBM = (CMyBitmap*)memDC.SelectObject(&Treb);
	pDC->BitBlt(2, TREBLE_CLEF_Y, szTreb.cx, szTreb.cy, &memDC, 0, 0, SRCAND);
	memDC.SelectObject(oldBM);
}

StaffMouseStates CMsTrebleCleff::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
{
	return StaffMouseStates::MOUSE_STAFF_STATE_NONE;
}
