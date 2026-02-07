// MsKeySignature.cpp: implementation of the CMsKeySignature class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsKeySignature::CMsKeySignature():CMsObject(CMsObject::MsObjType::KEYSIG)
{
	m_KeySignature = KeySigID::KS_ERROR;
}

CMsKeySignature::~CMsKeySignature()
{

}

bool CMsKeySignature::Create(CMsSong* pSong, CMsEvent* pEvent, KeySigID key)
{
	m_KeySignature = key;
	pSong->SetCurrentKeySignature(this);
	return CMsObject::Create(pSong, pEvent);
}

UINT CMsKeySignature::Process()
{
	return 1;
}

UINT CMsKeySignature::Play()
{
	GetSong()->SetCurrentKeySignature(this);
	return PLAY_OBJECT_DONE;
}

DRAWSTATE CMsKeySignature::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	DrawState = DRAWSTATE::PLACE;
	return DrawState;
}

DRAWSTATE CMsKeySignature::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	CMsKeySignature* pKS = nullptr;
	CString csText;
	int EventIndex;
	CMsEvent* pEV = nullptr;

	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::PLACE:
		EventIndex = GetStaffChildView()->GetDrawEvent();
		pEV = GetSong()->GetEventObject(EventIndex);
		pEV->AddObject(this);
		pKS = new CMsKeySignature;
		pKS->Create(GetSong(), pEV, GetKeySignature());
		GetStaffChildView()->SetDrawObject(pKS);
		GetStaffChildView()->CheckAndDoScroll(pointMouse);
		csText.Format(_T("Place Key Signature at Event %d"), GetStaffChildView()->GetDrawEvent());
		GetStaffChildView()->GetStatusBar()->SetText(csText);
		DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
		GetStaffChildView()->Invalidate();
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsKeySignature::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	//-------------------------------------------------------
	// MouseMove
	//		This is the state machine for creating this
	//	object on the screen.  This function is for when
	//	the left mouse is moved.
	//
	//	parameters:
	//		pointMouse..Current Mouse Position
	//		DrawState...Current state of drawing process
	//		Region.....Current Regiion where mouse is
	//		Transition.Current mouse region transition state
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	CString csStatusString, csTemp;
	CMsEvent* pEV = 0;

	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:		//MouseMove
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:		//MouseMove
		switch (Transition)
		{
		case MouseRegionTransitionState::MOUSE_TRANSITION_NONE:
			if (Region == MouseRegions::MOUSE_IN_EDITREG)
			{
				switch (StaffTransition(pointMouse, 0, GetParentEvent()))
				{
				case StaffMouseStates::MOUSE_STAFF_STATE_NONE:
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE_CHANGE:
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE:
					if (GetParentEvent())
					{
						GetParentEvent()->RemoveObject(this);
						SetParentEvent(nullptr);
					}
					pEV = GetSong()->GetEventObject(GetStaffView()->XtoEventIndex(pointMouse.x));
					if (pEV)
					{
						pEV->AddObject(this);
						SetParentEvent(pEV);
					}
					break;
				}
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW:
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE:
			pEV = GetParentEvent();
			if (pEV)
			{
				pEV->RemoveObject(this);
				SetParentEvent(nullptr);
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_EDIT:
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_EDIT:
			pEV = GetSong()->GetEventObject(GetStaffView()->XtoEventIndex(pointMouse.x));
			if (pEV)
			{
				pEV->AddObject(this);
				SetParentEvent(pEV);
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_LOWERSEL_TO_OUTSIDE:			//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_UPPERSEL_TO_OUTSIDE:
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_LOWERSEL:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_UPPERSEL:
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_ERROR:		//MouseMove
			break;
		}
		//		GetSong()->GetStaffChildView()->GetStatusBar()->SetText(csStatusString);
		break;
	case DRAWSTATE::MOVE_OBJECT_AROUND:
		break;
	case DRAWSTATE::PLACE:
		break;
	}
	GetStaffView()->Invalidate();
	return DrawState;
}

bool CMsKeySignature::IsFlatKeySig()
{
	bool IsFlat = false;

	switch (m_KeySignature)
	{
	case KeySigID::FMAJ:
	case KeySigID::BbMAJ:
	case KeySigID::EbMAJ:
	case KeySigID::AbMAJ:
	case KeySigID::DbMAJ:
	case KeySigID::GbMAJ:
	case KeySigID::CbMAJ:
			IsFlat = true;
			break;
	}
	return IsFlat;
}

void CMsKeySignature::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO,"%sKey Signature:%wS\n", pIndentString, CMsKeySignature::KeySigStringTab[m_KeySignature].GetString());
	if(pIndentString) delete[] pIndentString;
}

void CMsKeySignature::Draw(CDC *pDC)
{
	CMsSharp* pSharp = 0;
	CMsFlat* pFlat = 0;
	COLORREF color;
	int X = X_CENTER_OF_EVENT;
	int Y = TREBLE_TOP_LINE;

	if (IsSelected())
		color = RGB(255, 0, 0);
	else
		color = RGB(0, 0, 0);
	switch(m_KeySignature)
	{
	case KeySigID::CMAJ:	///do nothing
			break;
		case KeySigID::CsMAJ:	//seven sharps B sharp
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC,color,X+24,NoteToPosition(MSFF_NOTE_B2));	//base staff
			pSharp->Draw(pDC, color,X+24,NoteToPosition(MSFF_NOTE_B2+24));	//trebble staff
			[[fallthrough]];
		case KeySigID::FsMAJ:	//six sharps E sharp
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X+12,NoteToPosition(MSFF_NOTE_E2+12));	//base staff
			pSharp->Draw(pDC, color,X+20,NoteToPosition(MSFF_NOTE_E2+24));	//trebble staff
			[[fallthrough]];
		case KeySigID::BMAJ:	//five sharps A sharp
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X+16,NoteToPosition(MSFF_NOTE_A2));	//base staff
			pSharp->Draw(pDC, color,X+16,NoteToPosition(MSFF_NOTE_A2+24));	//trebble staff
			[[fallthrough]];
		case KeySigID::EMAJ:	//four D sharp
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X +20,NoteToPosition(MSFF_NOTE_D2+12));	//base staff
			pSharp->Draw(pDC, color,X+12,NoteToPosition(MSFF_NOTE_D2+36));	//trebble staff
			[[fallthrough]];
		case KeySigID::AMAJ:	//three G sharp
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X,NoteToPosition(MSFF_NOTE_G2+12));	//base staff
			pSharp->Draw(pDC, color,X+8,NoteToPosition(MSFF_NOTE_G2+36));		//trebble staff
			[[fallthrough]];
		case KeySigID::DMAJ:	//two C sharp
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X+4,NoteToPosition(MSFF_NOTE_C2+12));	//base staff
			pSharp->Draw(pDC, color,X+4,NoteToPosition(MSFF_NOTE_C2+36));	//trebble staff
			[[fallthrough]]; 
		case KeySigID::GMAJ:	///one F sharp
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X+8,NoteToPosition(MSFF_NOTE_F2+12));	//base staff
			pSharp->Draw(pDC, color,X,NoteToPosition(MSFF_NOTE_F2+36));	//trebble staff
			break;
			// Key signatures with flats.  Notice that the order of flats is the opposite of sharps, so the first flat is B flat, then E flat, etc.
		case KeySigID::CbMAJ:	//seven flats F flat
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X,NoteToPosition(MSFF_NOTE_F2+12));	//bass staff
			pFlat->Draw(pDC, color,X,NoteToPosition(MSFF_NOTE_F2+36));	//trebble staff
			[[fallthrough]];
		case KeySigID::GbMAJ:	//six flats C flat
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X+24,NoteToPosition(MSFF_NOTE_C2+12));	//bass staff
			pFlat->Draw(pDC, color,X+24,NoteToPosition(MSFF_NOTE_C2+36));	//trebble staff
			[[fallthrough]];
		case KeySigID::DbMAJ:	//five flats G flat
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X+24,NoteToPosition(MSFF_NOTE_G2));	//bass staff
			pFlat->Draw(pDC, color,X+24,NoteToPosition(MSFF_NOTE_G2+24));	//trebble staff
			[[fallthrough]];
		case KeySigID::AbMAJ:	//four flats D flat
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X +16,NoteToPosition(MSFF_NOTE_D2+12));	//bass staff
			pFlat->Draw(pDC, color,X+16,NoteToPosition(MSFF_NOTE_D2+36));	//trebble staff
			[[fallthrough]];
		case KeySigID::EbMAJ:	//three flats A flat
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X+16,NoteToPosition(MSFF_NOTE_A2));	//bass staff
			pFlat->Draw(pDC, color,X+16,NoteToPosition(MSFF_NOTE_A2+24));	//trebble staff
			[[fallthrough]];
		case KeySigID::BbMAJ:	//two flats E flat
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X+8,NoteToPosition(MSFF_NOTE_E2+12));	//bass staff
			pFlat->Draw(pDC, color,X+8,NoteToPosition(MSFF_NOTE_E2+36));	//trebble staff
			[[fallthrough]];
		case KeySigID::FMAJ:	//one flat B flat
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X+8,NoteToPosition(MSFF_NOTE_B2));	//bass staff
			pFlat->Draw(pDC, color,X+8,NoteToPosition(MSFF_NOTE_B2+24));	//trebble staff
			break;
	}
	if (pFlat) delete pFlat;
	if (pSharp) delete pSharp;
}

StaffMouseStates CMsKeySignature::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
{
	return StaffMouseStates::MOUSE_STAFF_STATE_NONE;
}

void CMsKeySignature::ObjectRectangle(CRect& rect, UINT Event)
{
	rect.SetRect(0, 0, 0, 0);
}

int CMsKeySignature::GetKeySigCorrection(int Note)
{
	UINT n = Note % 12;
	int rNoteCorection;

	rNoteCorection = KeySigCorrectionLUT[int(m_KeySignature)].m_pKeySigCorrection[n];
	return rNoteCorection;
}

int CMsKeySignature::CorrectNoteByKeySig(int Note, int accidental)
{
	switch (accidental)
	{
	case MSFF_ACCIDENTAL_INKEY:	//default
		Note += GetKeySigCorrection(Note);
		break;
	case MSFF_ACCIDENTAL_NATURAL:
		break;
	case MSFF_ACCIDENTAL_SHARP:
		Note += 1;
		break;
	case MSFF_ACCIDENTAL_FLAT:
		Note -= 1;
		break;
	}
	return Note;
}

void CMsKeySignature::Copy(CMsObject* pSource)
{
	m_KeySignature = ((CMsKeySignature*)pSource)->m_KeySignature;
	CMsObject::Copy(pSource);
}

void CMsKeySignature::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_KEY_SIGNATURE,pO);
	fputc(0x01,pO);
	fputc(int(m_KeySignature),pO);
}

int CMsKeySignature::NoteToPosition(int Note)
{
	return CMsNote::NotePos[Note % 12] + 28 * (7 - (Note / 12)) + (TREBLE_TOP_LINE);
}