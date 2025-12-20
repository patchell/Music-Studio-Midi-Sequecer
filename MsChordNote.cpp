#include "pch.h"

CMsChordNote::CMsChordNote()
{
	m_ObjType = CMsObject::MsObjType::CHORD_NOTE;
	m_IntervalType = 0;
}

CMsChordNote::~CMsChordNote()
{
}

bool CMsChordNote::Create(CMsSong* pSonge)
{
	bool rV = true;
    return rV;
}

UINT CMsChordNote::Process()
{
	return 0;
}

UINT CMsChordNote::Play()
{
	return 0;
}

DRAWSTATE CMsChordNote::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}
DRAWSTATE CMsChordNote::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsChordNote::MouseMove(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsChordNote::IsTimedObject()
{
	return GetDuration();
}
