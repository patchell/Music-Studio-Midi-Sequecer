#include "pch.h"

CMsPortamentoEnd::CMsPortamentoEnd() : CMsObject()
{
	m_ObjType =  MSOBJ_PORTAMENTO_STOP;
}

CMsPortamentoEnd::~CMsPortamentoEnd()
{
}

CMsObject* CMsPortamentoEnd::Copy(void)
{
	return nullptr;
}

void CMsPortamentoEnd::Draw(CDC* pDC, int event, int maxevent)
{
}

void CMsPortamentoEnd::Save(FILE* pO)
{
}

void CMsPortamentoEnd::Print(FILE* pO, int Indent)
{
}
