#include "pch.h"

CMsGlissandoEnd::CMsGlissandoEnd() :CMsObject()
{
	m_ObjType = CMsObject::MsObjType::GLISSANDO_END;
	m_bSelected = false;
	m_bStemDown = false;
	m_Pitch = 60;
	m_pParentGlissando = nullptr;
}

CMsGlissandoEnd::~CMsGlissandoEnd()
{
}

bool CMsGlissandoEnd::Create(CMsSong* pSong, CMsGlissando* pParent, CMsEvent* pEvEnd)
{
	m_pParentGlissando = pParent;
	CMsObject::Create(pSong, pEvEnd);
	return true;
}

UINT CMsGlissandoEnd::Process()
{
	return 0;
}

UINT CMsGlissandoEnd::Play()
{
	return 0;
}

DRAWSTATE CMsGlissandoEnd::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsGlissandoEnd::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsGlissandoEnd::MouseMove(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	default:
		break;
	}
	return DrawState;
}

void CMsGlissandoEnd::Print(FILE* pO, int Indent)
{
}

void CMsGlissandoEnd::Save(FILE* pO)
{
}

void CMsGlissandoEnd::Copy(CMsObject* Source)
{
	CMsObject::Copy(Source);
}

void CMsGlissandoEnd::Draw(CDC* pDC, int event, int maxevent)
{
}

UINT CMsGlissandoEnd::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

void CMsGlissandoEnd::ObjectRectangle(CRect& rect, UINT Event)
{
}

