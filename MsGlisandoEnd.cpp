#include "pch.h"

CMsGlisandoEnd::CMsGlisandoEnd():CMsObject()
{
    m_ObjType = MSOBJ_GLISANDO_END;
}

CMsGlisandoEnd::~CMsGlisandoEnd()
{
}

bool CMsGlisandoEnd::Create(CMsSong* pSong)
{
	CMsObject::Create(pSong, 0);
	return true;
}

UINT CMsGlisandoEnd::Process()
{
	return 0;
}

UINT CMsGlisandoEnd::Play()
{
	return 0;
}

int CMsGlisandoEnd::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsGlisandoEnd::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsGlisandoEnd::MouseMove(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

CMsObject* CMsGlisandoEnd::Copy(void)
{
    return nullptr;
}

void CMsGlisandoEnd::Draw(CDC* pDC, int event, int maxevent)
{
}

void CMsGlisandoEnd::Save(FILE* pO)
{
}

void CMsGlisandoEnd::Print(FILE* pO, int Indent)
{
}
