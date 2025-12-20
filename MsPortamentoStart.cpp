#include "pch.h"

CMsPortamentoStart::CMsPortamentoStart():CMsObject()
{
	m_ObjType = CMsObject::MsObjType::PORTAMENTO_START;
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

DRAWSTATE CMsPortamentoStart::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::PLACE:
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsPortamentoStart::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::PLACE:
		break;
	default:
		break;
	}
	return DrawState;
}	

DRAWSTATE CMsPortamentoStart::MouseMove(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::PLACE:
		break;
	default:
		break;
	}
	return DrawState;
}

void CMsPortamentoStart::Save(FILE* pO)
{
}

void CMsPortamentoStart::Copy(CMsObject* pSource)
{
	CMsObject::Copy(pSource);
}

void CMsPortamentoStart::Draw(CDC* pDC, int event, int maxevent)
{
}

void CMsPortamentoStart::Print(FILE* pO, int Indent)
{
}

