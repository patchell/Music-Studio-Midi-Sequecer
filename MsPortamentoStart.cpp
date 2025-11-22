#include "pch.h"

CMsPortamentoStart::CMsPortamentoStart():CMsObject()
{
	m_ObjType = MSOBJ_PORTAMENTO_START;
}

CMsPortamentoStart::~CMsPortamentoStart()
{
}

bool CMsPortamentoStart::Create(CMsSong* pSong, CMsEvent* pEvent)
{
	CMsObject::Create(pSong, pEvent);
	return true;
}

UINT CMsPortamentoStart::Process()
{
	return 0;
}

UINT CMsPortamentoStart::Play()
{
	return 0;
}

int CMsPortamentoStart::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsPortamentoStart::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}	

int CMsPortamentoStart::MouseMove(int DrawState, CPoint pointMouse)
{
	return DrawState;
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

void CMsPortamentoStart::Print(FILE* pO, int Indent)
{
}

