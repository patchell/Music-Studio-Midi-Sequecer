#include "pch.h"

CMsGlisandoEnd::CMsGlisandoEnd():CMsObject()
{
    m_ObjType = MSOBJ_GLISANDO_END;
}

CMsGlisandoEnd::~CMsGlisandoEnd()
{
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

void CMsGlisandoEnd::Print(FILE* pO)
{
}
