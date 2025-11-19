#include "pch.h"

CMsGlisandoStart::CMsGlisandoStart():CMsObject()
{
    m_ObjType = MSOBJ_GLISANDO_START;
}

CMsGlisandoStart::~CMsGlisandoStart()
{
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
