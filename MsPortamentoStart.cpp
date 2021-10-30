#include "pch.h"

CMsPortamentoStart::CMsPortamentoStart():CMsObject()
{
	m_ObjType = MSOBJ_PORTAMENTO_START;
}

CMsPortamentoStart::~CMsPortamentoStart()
{
}

void CMsPortamentoStart::Save(FILE* pO)
{
}

CMsObject* CMsPortamentoStart::Copy(void)
{
	return nullptr;
}

void CMsPortamentoStart::Draw(CDC* pDC, int event, int maxevent)
{
}

void CMsPortamentoStart::Print(FILE* pO)
{
}

