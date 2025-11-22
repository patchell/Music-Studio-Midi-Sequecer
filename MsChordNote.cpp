#include "pch.h"

CMsChordNote::CMsChordNote()
{
	m_ObjType = MSOBJ_CHORD_NOTE;
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

int CMsChordNote::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}
int CMsChordNote::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsChordNote::MouseMove(int DrawState, CPoint pointMouse)
{
	return DrawState;
}
