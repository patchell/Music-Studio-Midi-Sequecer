#include "pch.h"

CMsPortamentoEnd::CMsPortamentoEnd() : CMsObject()
{
	m_ObjType =  MSOBJ_PORTAMENTO_STOP;
}

CMsPortamentoEnd::~CMsPortamentoEnd()
{
}

bool CMsPortamentoEnd::Create(CMsSong* pSong, CMsEvent* pEvent)
{
	CMsObject::Create(pSong, pEvent);
	return true;
}

UINT CMsPortamentoEnd::Process()
{
	return 0;
}

UINT CMsPortamentoEnd::Play()
{
	return 0;
}

int CMsPortamentoEnd::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsPortamentoEnd::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsPortamentoEnd::MouseMove(int DrawState, CPoint pointMouse)
{
	return DrawState;
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
