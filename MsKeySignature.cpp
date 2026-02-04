// MsKeySignature.cpp: implementation of the CMsKeySignature class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsKeySignature::CMsKeySignature():CMsObject()
{
	m_ObjType = CMsObject::MsObjType::KEYSIG;
	m_KeySignature = 0;
	SetKeySigCorrection();
}

CMsKeySignature::~CMsKeySignature()
{

}

bool CMsKeySignature::Create(CMsSong* pSong, CMsEvent* pEvent, UINT key)
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
		case MSFF_CMAJ:	///do nothing
			break;
		case MSFF_CSMAJ:	///seven shrps
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC,color,X+24,NoteToPosition(MSFF_NOTE_B2));	//base staff
			pSharp->Draw(pDC, color,X+24,NoteToPosition(MSFF_NOTE_B2+24));	//trebble staff
			[[fallthrough]];
		case MSFF_FSMAJ:	///six sharps
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X+12,NoteToPosition(MSFF_NOTE_E2+12));	//base staff
			pSharp->Draw(pDC, color,X+20,NoteToPosition(MSFF_NOTE_E2+24));	//trebble staff
			[[fallthrough]];
		case MSFF_BMAJ:	///five sharps
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X+16,NoteToPosition(MSFF_NOTE_A2));	//base staff
			pSharp->Draw(pDC, color,X+16,NoteToPosition(MSFF_NOTE_A2+24));	//trebble staff
			[[fallthrough]];
		case MSFF_EMAJ:	///four D sharps
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X +20,NoteToPosition(MSFF_NOTE_D2+12));	//base staff
			pSharp->Draw(pDC, color,X+12,NoteToPosition(MSFF_NOTE_D2+36));	//trebble staff
			[[fallthrough]];
		case MSFF_AMAJ:	//three G sharps
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X,NoteToPosition(MSFF_NOTE_G2+12));	//base staff
			pSharp->Draw(pDC, color,X+8,NoteToPosition(MSFF_NOTE_G2+36));		//trebble staff
			[[fallthrough]];
		case MSFF_DMAJ:	//two C sharps
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X+4,NoteToPosition(MSFF_NOTE_C2+12));	//base staff
			pSharp->Draw(pDC, color,X+4,NoteToPosition(MSFF_NOTE_C2+36));	//trebble staff
			[[fallthrough]]; 
		case MSFF_GMAJ:	///one F sharp
			if (!pSharp) pSharp = new CMsSharp;
			pSharp->Draw(pDC, color,X+8,NoteToPosition(MSFF_NOTE_F2+12));	//base staff
			pSharp->Draw(pDC, color,X,NoteToPosition(MSFF_NOTE_F2+36));	//trebble staff
			break;
		case MSFF_CFMAJ:	//seven flats
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X,NoteToPosition(MSFF_NOTE_F2+12));	//bass staff
			pFlat->Draw(pDC, color,X,NoteToPosition(MSFF_NOTE_F2+36));	//trebble staff
			[[fallthrough]];
		case MSFF_GFMAJ:	//six flats
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X+24,NoteToPosition(MSFF_NOTE_C2+12));	//bass staff
			pFlat->Draw(pDC, color,X+24,NoteToPosition(MSFF_NOTE_C2+36));	//trebble staff
			[[fallthrough]];
		case MSFF_DFMAJ:	//five flats
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X+24,NoteToPosition(MSFF_NOTE_G2));	//bass staff
			pFlat->Draw(pDC, color,X+24,NoteToPosition(MSFF_NOTE_G2+24));	//trebble staff
			[[fallthrough]];
		case MSFF_AFMAJ:	//four flats
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X +16,NoteToPosition(MSFF_NOTE_D2+12));	//bass staff
			pFlat->Draw(pDC, color,X+16,NoteToPosition(MSFF_NOTE_D2+36));	//trebble staff
			[[fallthrough]];
		case MSFF_EFMAJ:	//three flats
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X+16,NoteToPosition(MSFF_NOTE_A2));	//bass staff
			pFlat->Draw(pDC, color,X+16,NoteToPosition(MSFF_NOTE_A2+24));	//trebble staff
			[[fallthrough]];
		case MSFF_BFMAJ:	//two flats
			if (!pFlat) pFlat = new CMsFlat;
			pFlat->Draw(pDC, color,X+8,NoteToPosition(MSFF_NOTE_E2+12));	//bass staff
			pFlat->Draw(pDC, color,X+8,NoteToPosition(MSFF_NOTE_E2+36));	//trebble staff
			[[fallthrough]];
		case MSFF_FMAJ:	//one flat
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

void CMsKeySignature::SetKeySigCorrection(void)
{
	int KeySig = m_KeySignature;
	int i;
	for(i=0;i<12;++i)
	{
		m_KeySigCorrection[i] = 0;
		//---------------------------------
		// You might be wondering just
		// how exactly the following switch
		// statement works.  Except for
		// C major (MSFF_CMAJ), all of the
		// cases do not have a break.  For
		// example, C# major (MSFF_CSMAJ)
		// will go through all of the cases
		switch(KeySig)
		{
			case MSFF_CMAJ:
				break;
			case MSFF_CSMAJ:
				if(i == MSFF_NOTE_C)
					m_KeySigCorrection[i] = 1;
				[[fallthrough]];
			case MSFF_FSMAJ:
				if(i == MSFF_NOTE_F)
					m_KeySigCorrection[i] = 1;
				[[fallthrough]];
			case MSFF_BMAJ:
				if(i == MSFF_NOTE_A)
					m_KeySigCorrection[i] = 1;
				[[fallthrough]];
			case MSFF_EMAJ:
				if(i == MSFF_NOTE_D)
					m_KeySigCorrection[i] = 1;
				[[fallthrough]];
			case MSFF_AMAJ:
				if(i == MSFF_NOTE_G)
					m_KeySigCorrection[i] = 1;
				[[fallthrough]];
			case MSFF_DMAJ:	//two sharps
				if(i == MSFF_NOTE_F)
					m_KeySigCorrection[i] = 1;
				[[fallthrough]];
			case MSFF_GMAJ:	//one sharp
				if(i == MSFF_NOTE_C)
					m_KeySigCorrection[i] = 1;
				break;
			case MSFF_CFMAJ:
				if(i == MSFF_NOTE_F)
					m_KeySigCorrection[i] = -1;
				[[fallthrough]];
			case MSFF_GFMAJ:
				if(i == MSFF_NOTE_C)
					m_KeySigCorrection[i] = -1;
				[[fallthrough]];
			case MSFF_DFMAJ:
				if(i == MSFF_NOTE_G)
					m_KeySigCorrection[i] = -1;
				[[fallthrough]];
			case MSFF_AFMAJ:
				if(i == MSFF_NOTE_D)
					m_KeySigCorrection[i] = -1;
				[[fallthrough]];
			case MSFF_EFMAJ:
				if(i == MSFF_NOTE_A)
					m_KeySigCorrection[i] = -1;
				[[fallthrough]];
			case MSFF_BFMAJ:
				if(i == MSFF_NOTE_E)
					m_KeySigCorrection[i] = -1;
				[[fallthrough]];
			case MSFF_FMAJ:
				if(i == MSFF_NOTE_B)
					m_KeySigCorrection[i] = -1;
				break;
		}
	}
}

UINT CMsKeySignature::GetKeySigCorrection(UINT note, INT accidental)
{
	UINT n = note % 12;
	switch(accidental)
	{
		case MSFF_ACCIDENTAL_INKEY:	//default
			note += m_KeySigCorrection[n];
			break;
		case MSFF_ACCIDENTAL_NATURAL:
			break;
		case MSFF_ACCIDENTAL_SHARP:
			note += 1;
			break;
		case MSFF_ACCIDENTAL_FLAT:
			note -= 1;
			break;
	}
	return note;
}

void CMsKeySignature::Copy(CMsObject* pSource)
{
	m_KeySignature = ((CMsKeySignature*)pSource)->m_KeySignature;
	for(int i=0;i<12;++i)
	{
		m_KeySigCorrection[i] = ((CMsKeySignature*)pSource)->m_KeySigCorrection[i];
	}
	CMsObject::Copy(pSource);
}

void CMsKeySignature::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_KEY_SIGNATURE,pO);
	fputc(0x01,pO);
	fputc(m_KeySignature,pO);
}

int CMsKeySignature::NoteToPosition(int Note)
{
	return CMsNote::NotePos[Note % 12] + 28 * (7 - (Note / 12)) + (TREBLE_TOP_LINE);
}