#include "pch.h"

CMsGlisandoStart::CMsGlisandoStart():CMsObject()
{
    m_ObjType = MSOBJ_GLISANDO_START;
}

CMsGlisandoStart::~CMsGlisandoStart()
{
}

bool CMsGlisandoStart::Create(CMsSong* pSong)
{
	CMsObject::Create(pSong, 0);
	return true;
}

UINT CMsGlisandoStart::Process()
{
	return 0;
}

UINT CMsGlisandoStart::Play()
{
	return 0;
}

int CMsGlisandoStart::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsGlisandoStart::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsGlisandoStart::MouseMove(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

CMsObject* CMsGlisandoStart::Copy(void)
{
    return nullptr;
}

void CMsGlisandoStart::Draw(CDC* pDC, int event, int maxevent)
{
}

void CMsGlisandoStart::Save(FILE* pO)
{
}

void CMsGlisandoStart::Print(FILE* pO, int Indent)
{
}
