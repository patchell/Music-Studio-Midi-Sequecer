//------------------------------------
// ChildViewStaff.cpp : implementation of the CChildViewStaff class
//--------------------------------------


#include "pch.h"

// CChildViewStaff


CSize GetBmDimensions(int ID)
{
	CSize size;
	CMyBitmap Bm;

	Bm.LoadBitmapW(ID);
	size = Bm.GetBmDim();
	return size;
}

CChildViewStaff::CChildViewStaff()
	: m_nMidiNotesOn(0)		//number of midi note on events
	, m_nMidiInputNoteSetup(0)	//setup using midi to enter notes
	, m_pPgmChng(NULL)			// window to send patch change?
	, m_nLastSongPosition(0)	// Like it says
	, m_pRepeatEndSelected(0)	// End of a repeat
	, m_pRepeatStartSelected(0)	// Start of a repeat
	, m_UpperSelRect(0, 0, 0, 0)
	, m_LowerSelRect(0, 0, 0, 0)
{
	EnableActiveAccessibility();
	m_nMouseState = StaffViewMouseState::STAFFVIEW_MOUSEUP;			// Mouse state machine
	m_pFirstTieNote = 0;		// pointer to first tie note
	m_pSecondTieNote = 0;		// Pointer to second tie note
	m_nDrawState = DRAWSTATE::NA;			// draw state machine
	m_dmDrawMode = DrawMode::NOP;			// Drawing mode ie notes, etc
	m_nDrawEvent = 0;			// Current event mouse is over
	m_pSong = 0;				// pointer to song
	m_MaxEvents = 0;			// maximum events displayed
	m_pDrawObject = 0;			// current objedt being drawn
	pLastNote = 0;				// pointer to the last note placed
	m_LastSelectedEventIndex = -1;
	m_FirstSelectedEvent = -1;
	m_SongScrollPos = 0;
	m_nMidiNotesOn = 0;
	m_nRawEvent = 0;
	m_pHighLightedObject = 0;
	m_DragFlag = 0;
	//------------------------------
	m_SelectRectTop = 0;
	//--------------------------
	// Initialize Dispatch List
	//--------------------------
	m_pMessageList = new MessageList[STAFFVIEW_MIDI_IN_DISPATCH_NUMENTRIES];
	m_NumOfMessListEntries = STAFFVIEW_MIDI_IN_DISPATCH_NUMENTRIES;
	m_pMessageList[0].m_csMessageName = CString(_T("UINT Midi Msg"));
	m_pMessageList[0].m_WM_messase = WM_SHORTMIDIMSG;
	m_pMessageList[1].m_csMessageName = CString(_T("Long Midi Msg"));
	m_pMessageList[1].m_WM_messase = WM_LONGMIDIMSG;
	m_pMessageList[2].m_csMessageName = CString(_T("Song Edit"));
	m_pMessageList[2].m_WM_messase = WM_STAFF_MIDI_EDIT;
	m_pMessageList[3].m_csMessageName = CString(_T("Trigger"));
	m_pMessageList[3].m_WM_messase = WM_STAFF_MIDI_TRIGGER;
	m_pSelectedObjectHead = 0;
	m_pSelectedObjectTail = 0;
	m_EscapeFlag = 0;
	m_ReadyToDraw = 0;
	//------ Inirialize Key States -----------------
	m_AltKeyDown = 0;
	m_CtrlKeyDown = 0;
	m_ShiftKeyDown = 0;
	m_TimerID = 0;
	m_MouseRegion = MouseRegions::MOUSE_OUTSIDE;
	m_MouseRegionTransitionState = MouseRegionTransitionState::MOUSE_TRANSITION_NONE;
}

CChildViewStaff::~CChildViewStaff()
{
}


BEGIN_MESSAGE_MAP(CChildViewStaff, CChildViewBase)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_WM_HSCROLL()
	// Custom Messages
	ON_MESSAGE(WM_SHORTMIDIMSG, &CChildViewStaff::OnShortmidimsg)
	ON_MESSAGE(WM_LONGMIDIMSG,&CChildViewStaff::OnLongMidiMsg)
	ON_MESSAGE(WM_STAFF_DISP_EVENT, &CChildViewStaff::OnStaffDispEvent)
	ON_MESSAGE(WM_MY_CONTROL_MESSAGES, &CChildViewStaff::MyControlsMessages)
	ON_MESSAGE(WM_STAFF_MIDI_EDIT, &CChildViewStaff::OnMidiEdit)
	ON_MESSAGE(WM_CHILDVIEW_PLAYERTHREAD, &CChildViewStaff::OnChildviewPlayerthread)
	ON_WM_TIMER()
	ON_COMMAND(MENU_MS_FILE_OPEN, &CChildViewStaff::OnMenuMsFileOpen)
	ON_COMMAND(MENU_MS_FILE_SAVE, &CChildViewStaff::OnMenuMsFileSave)
	ON_UPDATE_COMMAND_UI(MENU_MS_FILE_OPEN, &CChildViewStaff::OnUpdateMenuMsFileOpen)
	ON_UPDATE_COMMAND_UI(MENU_MS_FILE_SAVE, &CChildViewStaff::OnUpdateMenuMsFileSave)
	ON_COMMAND(MENU_MS_FILE_SAVE_as, &CChildViewStaff::OnMenuMsFileSaveAs)
	ON_UPDATE_COMMAND_UI(MENU_MS_FILE_SAVE_as, &CChildViewStaff::OnUpdateMenuMsFileSaveAs)
	ON_COMMAND(ID_SETTINGS_TRACKSETTINIGS, &CChildViewStaff::OnSettingsTracksettinigs)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_TRACKSETTINIGS, &CChildViewStaff::OnUpdateSettingsTracksettinigs)
END_MESSAGE_MAP()

// CChildViewStaff message handlers


void CChildViewStaff::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC* pDC = (CDC*)&dc;
	OnDraw(pDC);
}

//--------------- Selected Object List Methods ------

void CChildViewStaff::AddSelectedObject(CMsObject* pObj)
{
	if (GetSelectedObjectHead() == 0)
	{
		SetSelectedObjectHead(pObj);
		SetSelectedObjectTail(pObj)
;
	}
	else
	{
		pObj->SetNextSelectedObject(GetSelectedObjectHead());
		GetSelectedObjectHead()->SetPrevSelectedObject(pObj);
		SetSelectedObjectHead(pObj);
	}
}

void CChildViewStaff::RemoveSelectedObject(CMsObject* pObj)
{
	if (pObj == GetSelectedObjectHead())
	{
		SetSelectedObjectHead(pObj->GetNextSelectedObject());
		if (GetSelectedObjectHead())
			GetSelectedObjectHead()->SetPrevSelectedObject(0);
		else
			SetSelectedObjectTail(0);
	}
	else if (pObj == GetSelectedObjectTail())
	{
		SetSelectedObjectTail(pObj->GetPrevSelectedObject());
		if (GetSelectedObjectTail())
			GetSelectedObjectTail()->SetNextSelectedObject(0);
		else
			SetSelectedObjectHead(0);
	}
	else
	{
		pObj->GetNextSelectedObject()->SetPrevSelectedObject(pObj->GetPrevSelectedObject());
		pObj->GetPrevSelectedObject()->SetNextSelectedObject(pObj->GetNextSelectedObject());
	}
}

CMsObject* CChildViewStaff::MatchMouseToObjectInEvent(UINT Event, CPoint MousePointer)
{
	CMsObject* pObjFound = nullptr;
	UINT Loop;
	CMsEvent* pEvent;

	pEvent = m_pSong->GetEventObject(Event);
	if (pEvent)
		pObjFound = pEvent->GetEventMsObjectHead();
	else
		pObjFound = 0;
	Loop = 1;
	while (pObjFound && Loop)
	{
		if (pObjFound && pObjFound->Select(true, MousePointer))
		{
			CString csMsgString, csScratch;
			Loop = 0;
			AddSelectedObject(pObjFound);
			pObjFound->ObjectToString(csScratch, 1);
			csMsgString.Format(_T("Selected %lS"), csScratch.GetString());
			m_Status.SetText(csMsgString);
		}
		else
			pObjFound = pObjFound->GetNext();
	}	//end of else
	return pObjFound;
}

CMsObject* CChildViewStaff::MatchMouseToSelectedObject(CPoint MousePointer)
{
	CMsObject* pFoundOject = nullptr;
	UINT Loop;

	pFoundOject = GetSelectedObjectHead();
	if (pFoundOject)
	{
		//------------------------------
		// Check to see if the mouse
		// is over this object when it
		// was clicked
		//-------------------------------
		Loop = 1;
		while (pFoundOject && Loop)
		{
			if (pFoundOject->MouseOverObject(MousePointer))
			{
				if (pFoundOject->IsSelected())
				{
					pFoundOject->SetSelected(0);	// toggle selection
					RemoveSelectedObject(pFoundOject);
					Loop = 0;
				}
			}
			else
				pFoundOject = pFoundOject->GetNextSelectedObject();
		}
	}
	return pFoundOject;
}

CMsTrack* CChildViewStaff::GetTrackInfo(int TrackID)
{
	return GetSongInfo()->GetTrack(TrackID);
}

void CChildViewStaff::OnLButtonDown(UINT nFlags, CPoint pointMouse)
{
	int Event = XtoEventIndex(pointMouse.x);
	CMsObject* pObject;
	int Note = YtoNote(pointMouse.y);
	MouseRegions Region;
	MouseRegionTransitionState TransitionState = MouseRegionTransitionState::MOUSE_TRANSITION_NONE;
	UINT Loop = 1;

	if (this->GetFocus() != this)
		SetFocus();

	m_nMouseState = StaffViewMouseState::STAFFVIEW_MOUSEDOWN;
	Region = MouseInRegion(pointMouse);
	switch (Region)
	{
	case MouseRegions::MOUSE_OUTSIDE:
		break;
	case MouseRegions::MOUSE_IN_UPPERSEL:
	case MouseRegions::MOUSE_IN_LOWERSEL:
		break;
	case MouseRegions::MOUSE_IN_EDITREG:
		switch (m_dmDrawMode)	//OnLButtonDown
		{
		case DrawMode::NOP:
			pObject = MatchMouseToSelectedObject(pointMouse);
			if (pObject == nullptr)
				pObject = MatchMouseToObjectInEvent(Event, pointMouse);
			break;
		case DrawMode::GLISSANDO:	//OnLButtonDown
			m_nDrawState = GetDrawObject()->MouseLButtonDown(m_nDrawState, pointMouse, Region, TransitionState);
			Invalidate();
			break;
		case DrawMode::NOTE:	//OnLButtonDown
			m_nDrawState = GetDrawObject()->MouseLButtonDown(m_nDrawState, pointMouse, Region, TransitionState);
			break;
		default:
			m_nDrawState = GetDrawObject()->MouseLButtonDown(m_nDrawState, pointMouse, Region, TransitionState);
			break;
		}//end of switch(m_dmDrawMode)
	}
	CWnd::OnLButtonDown(nFlags, pointMouse);
}

void CChildViewStaff::OnLButtonUp(UINT nFlags, CPoint pointMouseLButtUp)
{
	MouseRegions Region;
	CMsEvent* pEv;
	CString csTemp;
	CMsGlissando* pGliss = 0;
	CMsNote* pN = 0;
	int note = 0;
	MouseRegionTransitionState TransitionState;

	m_nMouseState = StaffViewMouseState:: STAFFVIEW_MOUSEUP;
	m_nDrawEvent = XtoEventIndex(pointMouseLButtUp.x);
	m_MouseRegionTransitionState = RegionTransition(pointMouseLButtUp);
	Region = MouseInRegion(pointMouseLButtUp);
	TransitionState = RegionTransition(pointMouseLButtUp);
	switch (Region)
	{
	case MouseRegions::MOUSE_OUTSIDE:		//OnLButtonUp
		break;
	case MouseRegions::MOUSE_IN_UPPERSEL:		//OnLButtonUp
	case MouseRegions::MOUSE_IN_LOWERSEL:
		if ((pEv = m_pSong->GetEventObject(m_nDrawEvent)) != NULL)
		{
			if (pEv->IsSelected())	//deselect block
			{
				if (m_FirstSelectedEvent < 0)
				{
					pEv->SetSelected(0);
					m_LastSelectedEventIndex = -1;
				}
				else
				{
					int i;
					int loop;
					pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
					for (i = m_FirstSelectedEvent, loop = 1; (i < m_LastSelectedEventIndex + 1) && loop; ++i)
					{
						if (pEv)
						{
							pEv->SetSelected(0);
							pEv = pEv->GetNext();
						}
						else
							loop = 0;
					}
					m_FirstSelectedEvent = -1;
					m_LastSelectedEventIndex = -1;
				}
				CString csBlank(_T(""));
				m_Status.SetText(csBlank);
			}
			else    //select events		OnLButtonUp
			{
				if (m_LastSelectedEventIndex >= 0)
				{
					//we are going to select a block
					if (m_FirstSelectedEvent < 0)
					{
						m_FirstSelectedEvent = m_LastSelectedEventIndex;
						m_LastSelectedEventIndex = m_nDrawEvent;
						if (m_LastSelectedEventIndex < m_FirstSelectedEvent)
						{
							int temp = m_LastSelectedEventIndex;
							m_LastSelectedEventIndex = m_FirstSelectedEvent;
							m_FirstSelectedEvent = temp;
						}
						int i;
						pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
						for (i = m_FirstSelectedEvent; i < m_LastSelectedEventIndex + 1; ++i)
						{
							pEv->SetSelected(1);
							pEv = pEv->GetNext();
						}
						CString s;
						s.Format(_T("Block Selected %d:%d"), m_FirstSelectedEvent, m_LastSelectedEventIndex);
						m_Status.SetText(s);
					}
					else	//add more events to the block		//OnLButtonUp
					{
						if (m_nDrawEvent < m_FirstSelectedEvent)
						{
							pEv = m_pSong->GetEventObject(m_nDrawEvent);
							for (int i = m_nDrawEvent; i < m_FirstSelectedEvent + 1; ++i)
							{
								pEv->SetSelected(1);
								pEv = pEv->GetNext();
							}
							m_FirstSelectedEvent = m_nDrawEvent;
						}
						else if (m_nDrawEvent > m_LastSelectedEventIndex)		//OnLButtonUp
						{
							pEv = m_pSong->GetEventObject(m_LastSelectedEventIndex);
							for (int i = m_LastSelectedEventIndex; i < m_nDrawEvent + 1; ++i)
							{
								pEv->SetSelected(1);
								pEv = pEv->GetNext();
							}
							m_LastSelectedEventIndex = m_nDrawEvent;
						}
						CString s;
						s.Format(_T("Block Selected %d:%d"), m_FirstSelectedEvent, m_LastSelectedEventIndex);
					}
				}
				else		//OnLButtonUp
				{
					pEv->SetSelected(1);
					m_LastSelectedEventIndex = m_nDrawEvent;
					CString s;
					s.Format(_T("Event Selected:%d"), m_nDrawEvent);
					m_Status.SetText(s);
				}
			}
			Invalidate();
		}
		break;
	case MouseRegions::MOUSE_IN_EDITREG:		//OnLButtonUp
		switch (m_dmDrawMode)
		{
		case DrawMode::NOP:		//OnLButtonUp
			break;
		case DrawMode::NOTE:		//OnLButtonUp
			m_nDrawState = GetDrawObject()->MouseLButtonUp(
				m_nDrawState, 
				pointMouseLButtUp,
				Region,
				TransitionState
			);;
			Invalidate();
			break;
		case DrawMode::TEMPO:
		case DrawMode::LOUDNESS:		//OnLButtonUp
		case DrawMode::TIMESIG:		//OnLButtonUp
		case DrawMode::KEYSIG:		//OnLButtonUp
		case DrawMode::GLISSANDO:		//OnLButtonUp
		case DrawMode::ENDBAR:
			[[fallthrough]];
		case DrawMode::BAR:		//OnLButtonUp
			m_nDrawState = GetDrawObject()->MouseLButtonUp(
				m_nDrawState, 
				pointMouseLButtUp,
				Region,
				TransitionState
			);
			break;
		case DrawMode::TIE:		//OnLButtonUp
			//-----------------------------------
			// This is where we change the state of
			// drawing a tie.  When the mouse releases
			// we need to check to see if there really
			// is a note under the mouse pointer.
			//  If there is, record the note and the
			// points.
			//------------------------------------------
			switch (m_nDrawState)
			{
			case DRAWSTATE::TIE_FIRSTNOTE:		//OnLButtonUp
				note = YtoNote(pointMouseLButtUp.y);
				pN = m_pSong->CheckForNotePresence(m_nDrawEvent, note);
				if (pN)
				{
					m_pFirstTieNote = pN;
					m_nDrawState = DRAWSTATE::TIE_SECONDNOTE;
					m_TieStartPoint = pointMouseLButtUp + CSize(0, 4);
					CString s, csTemp;
					s = CString("First Note ");
					pN->ObjectToString(csTemp);
					s.Append(csTemp);
					csTemp.Format(_T(" Selected at Event %d"), m_nDrawEvent);
					s.Append(csTemp);
					m_Status.SetText(s);
				}
				break;
			case DRAWSTATE::TIE_SECONDNOTE:		//OnLButtonUp
				note = YtoNote(pointMouseLButtUp.y);
				pN = m_pSong->CheckForNotePresence(m_nDrawEvent, note);
				if (pN)
				{
					//------------------------------------
					// create string for status box
					//------------------------------------
					CString s, csTemp;
					m_pSecondTieNote = pN;
					s = CString("Second Note ");
					pN->ObjectToString(csTemp);
					s.Append(csTemp);
					csTemp.Format(_T(" Selected at Event %d"), m_nDrawEvent);
					s.Append(csTemp);
					m_Status.SetText(s);
					//-------------------------------------------
					m_nDrawState = DRAWSTATE::TIE_FIRSTNOTE;
					m_TieEndPoint = pointMouseLButtUp + CSize(0, 4);
					//----------------------------------------------
					// setting the tie end and begin is a little
					// tricky.
					//----- check which direction tie was drawn ----
					if (m_TieEndPoint.x > m_TieStartPoint.x)
					{
						// if tie is drawn forwards
						m_pFirstTieNote->SetTieBeg(1);
						m_pSecondTieNote->SetTieEnd(1);
						m_pFirstTieNote->SetTieNoteNext(m_pSecondTieNote);
						m_pSecondTieNote->SetTieNotePrev(m_pFirstTieNote);
					}
					else		//OnLButtonUp
					{
						//if tie is drawn backwards
						m_pFirstTieNote->SetTieEnd(1);
						m_pSecondTieNote->SetTieBeg(1);
						m_pSecondTieNote->SetTieNoteNext(m_pFirstTieNote);
						m_pFirstTieNote->SetTieNotePrev(m_pSecondTieNote);
					}
					m_pFirstTieNote = 0;
					m_pSecondTieNote = 0;
					Invalidate();
				}
				break;
			}	//end of switch(m_nDrawState) in case DrawMode::TIE
			break;
		case DrawMode::COPY:		//OnLButtonUp
			CopyBlock(m_nDrawEvent);
			SetDrawObject(0);
			m_dmDrawMode = DrawMode::NOP;
			Invalidate();
			break;
		case DrawMode::MOVE:		//OnLButtonUp
			MoveBlock(m_nDrawEvent);
			SetDrawObject(0);
			m_dmDrawMode = DrawMode::NOP;
			Invalidate();
			break;
		}//end of switch(m_dmDrawMode)
		break;
	}	//end of switch(Region)
	CWnd::OnLButtonUp(nFlags, pointMouseLButtUp);
}

void CChildViewStaff::OnMouseMove(UINT nFlags, CPoint pointMouse)
{
	CMsEvent* pEV = 0;
	MouseRegions Region;
	MouseRegionTransitionState TransitionState;
	int NoteLocation = 0;	// location of note on the stave
	int NoteEvent = 0;		// event index of note on the stave
	CString StatusString,csTemp;
	bool Loop = false;
	int NoteActuallyPlayed = 0;
	int NoteIndex = 0;

	
	m_MouseRegionTransitionState = RegionTransition(pointMouse);
	Region = MouseInRegion(pointMouse);
	TransitionState = RegionTransition(pointMouse);
	switch (TransitionState)
	{
	case MouseRegionTransitionState::MOUSE_TRANSITION_NONE:
		switch (Region)
		{
		case MouseRegions::MOUSE_OUTSIDE:
		case MouseRegions::MOUSE_IN_UPPERSEL:
		case MouseRegions::MOUSE_IN_LOWERSEL:
			break;
		case MouseRegions::MOUSE_IN_EDITREG:
			break;
		}
		break;
	case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW:
	case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW:
	case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE:
		break;
	case MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT:
	case MouseRegionTransitionState::MOUSE_TRANSITION_UPPERSEL_TO_OUTSIDE:
		//--------------------------------
	case MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_EDIT:
	case MouseRegionTransitionState::MOUSE_TRANSITION_LOWERSEL_TO_OUTSIDE:
		break;
	case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_EDIT:
		break;
	case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_LOWERSEL:
	case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_UPPERSEL:
		break;
	case MouseRegionTransitionState::MOUSE_TRANSITION_ERROR:
		break;
	}
	switch (m_dmDrawMode)
	{
	case DrawMode::BAR:		// OnMouseMove
	case DrawMode::ENDBAR:
	case DrawMode::GLISSANDO:
	case DrawMode::KEYSIG:
	case DrawMode::LOUDNESS:
	case DrawMode::NOTE:
	case DrawMode::REPEAT_START:
	case DrawMode::REPEAT_END:
	case DrawMode::REST:
		[[fallthrough]]; 
	case DrawMode::TEMPO:
		if(GetDrawObject())
			m_nDrawState = GetDrawObject()->MouseMove(m_nDrawState, pointMouse, Region, TransitionState);
		break;
	}
	m_MouseRegionTransitionState = TransitionState;
	m_MouseRegion = Region;
	CWnd::OnMouseMove(nFlags, pointMouse);
}

void CChildViewStaff::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMsNote* pN;
	CMsObject* pTemp = 0, *pObj = 0;
	int Trip, Dot;
	INT Dur;
	CString csBlank(_T(""));
	CString csTemp;

	switch (nChar)
	{
	case VK_MENU:		// OnKeyDown
		m_AltKeyDown = 1;
		break;
	case VK_SHIFT:		// OnKeyDown
		m_ShiftKeyDown = 1;
		break;
	case VK_CONTROL:		// OnKeyDown
		m_CtrlKeyDown = 1;
		break;
	case 'R':	//OnKeyDown toggle REST mode.
		if (DrawMode::NOTE != m_dmDrawMode)
		{
			m_dmDrawMode = DrawMode::NOTE;
			if (GetDrawObject()) {
				delete m_pDrawObject;
				SetDrawObject(0);
			}
			pN = new CMsNote;
			pN->Create(0, GetSong(), GetSong()->GetEventObject(m_nDrawEvent));
			SetDrawObject(pN);
			//-----------------------------
			// decode message word
			//------------------------------
			SetTriplet(0);
			SetDotted(0);
			pN->GetData().CopyData(GetNoteData());
			//--------------------------------------
			// when going from a note to a rest,
			// we need to strip off such things as
			// decorations like dotted and triplet
			// from the duration
			//--------------------------------------
			Dur = pN->GetDuration();
			Dur = CMsNote::NoteDurLut[(int)CMsNote::GetDurationTable()[Dur].NoteShapeIndex];
			pN->SetDuration(Dur);
			pN->SetRest(1);
		}
		else
		{
			pN = (CMsNote*)GetDrawObject();
			pN->SetRest(pN->IsRest() ^ 1);
		}
		csTemp = CString("Draw Rest");
		m_Status.SetText(csTemp);
		break;
	case 'N':	// OnKeyDown
		m_dmDrawMode = DrawMode::NOTE;
		pN = new CMsNote;
		pN->Create(0, GetSong(), GetSong()->GetEventObject(m_nDrawEvent));
		if (GetDrawObject())
		{
			delete m_pDrawObject;
			SetDrawObject(0);
		}
		SetDrawObject(pN);
		//-----------------------------
		// decode message word
		//------------------------------
		pN->GetData().CopyData(GetNoteData());
		csTemp = CString("Draw Note");
		m_Status.SetText(csTemp);
		break; 
	case 'B':	// OnKeyDown
		m_dmDrawMode = DrawMode::BAR;
		if (GetDrawObject())
		{
			delete m_pDrawObject;
			SetDrawObject(0);
		}
		SetDrawObject(new CMsBar);
		((CMsBar*)GetDrawObject())->Create(GetSong(), GetSong()->GetEventObject(m_nDrawEvent));
		csTemp = CString("Draw Measure Bar");
		m_Status.SetText(csTemp);
		break;
	case VK_OEM_PERIOD:	// OnKeyDown dotted note toggle
		if (DrawMode::NOTE == m_dmDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)GetDrawObject();
			Dur = pN->GetDuration();
			Dot = CMsNote::GetDurationTable()[(int)Dur].Dotted;
			Trip = CMsNote::GetDurationTable()[(int)Dur].Triplet;
			if (Dot)
			{
				DurTemp = (int)Dur;
				DurTemp -= 2;	//undot
				Dur = DurTemp;
			}
			else if (Trip)
			{
				DurTemp = (int)Dur;
				DurTemp += 4;	//untrip and dot
				Dur = DurTemp;
			}
			else
			{
				DurTemp = (int)Dur;
				DurTemp += 2;	//Dot
				Dur = DurTemp;
			}
			pN->SetDuration(Dur);
			Invalidate();
		}
		break;
	case '3':	// OnKeyDown triplet toggle
		if (DrawMode::NOTE == m_dmDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)GetDrawObject();
			Dur = pN->GetDuration();
			Dot = CMsNote::GetDurationTable()[(int)Dur].Dotted;
			Trip = CMsNote::GetDurationTable()[(int)Dur].Triplet;
			if (Dot)
			{
				DurTemp = (int)Dur;
				DurTemp -= 4;	//undot and trip
				Dur = DurTemp;
			}
			else if (Trip)
			{
				DurTemp = (int)Dur;
				DurTemp += 2;	//untrip
				Dur = DurTemp;
			}
			else
			{
				DurTemp = (int)Dur;
				DurTemp -= 2;	//Tripplet
				Dur = DurTemp;
			}
			pN->SetDuration(Dur);
			Invalidate();
		}
		break;
	case 'T':	// OnKeyDown change to thirty second note
		if (DrawMode::NOTE == m_dmDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)GetDrawObject();
			Dur = pN->GetDuration();
			Dot = CMsNote::GetDurationTable()[(int)Dur].Dotted;
			Trip = CMsNote::GetDurationTable()[(int)Dur].Triplet;
			Dur = MSFF_THIRTYSEC_NOTE;
			if (Dot)
			{
				DurTemp = (int)Dur;
				DurTemp += 2;
				Dur = DurTemp;
			}
			else if (Trip)
			{
				DurTemp = (int)Dur;
				DurTemp -= 2;
				Dur = DurTemp;
			}
			pN->SetDuration(Dur);
			pN->SetFlags(3); // set thirty second flag
			Invalidate();
		}
		break;
	case 'S':	// OnKeyDown change to Sixteenth  note
		if (DrawMode::NOTE == m_dmDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)GetDrawObject();
			Dur = pN->GetDuration();
			Dot = CMsNote::GetDurationTable()[(int)Dur].Dotted;
			Trip = CMsNote::GetDurationTable()[(int)Dur].Triplet;
			Dur = MSFF_SIXTEENTH_NOTE;
			if (Dot)
			{
				DurTemp = (int)Dur;
				DurTemp += 2;
				Dur = DurTemp;
			}
			else if (Trip)
			{
				DurTemp = (int)Dur;
				DurTemp -= 2;
				Dur = DurTemp;
			}
			pN->SetDuration(Dur);
			pN->SetFlags(2); // set sixteenth note flag
			Invalidate();
		}
		break;
	case 'E':	// OnKeyDown change to eight note
		if (DrawMode::NOTE == m_dmDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)GetDrawObject();
			Dur = pN->GetDuration();
			Dot = CMsNote::GetDurationTable()[(int)Dur].Dotted;
			Trip = CMsNote::GetDurationTable()[(int)Dur].Triplet;
			Dur = MSFF_EIGTH_NOTE;
			if (Dot)
			{
				DurTemp = (int)Dur;
				DurTemp += 2;
				Dur = DurTemp;
			}
			else if (Trip)
			{
				DurTemp = (int)Dur;
				DurTemp -= 2;
				Dur = DurTemp;
			}
			pN->SetDuration(Dur);
			pN->SetFlags(1); // set eighth note flag
			Invalidate();
		}
		break;
	case 'Q':	// OnKeyDown change to quarter note
		if (DrawMode::NOTE == m_dmDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)GetDrawObject();
			Dur = pN->GetDuration();
			Dot = CMsNote::GetDurationTable()[(int)Dur].Dotted;
			Trip = CMsNote::GetDurationTable()[(int)Dur].Triplet;
			Dur = MSFF_QUARTER_NOTE;
			if (Dot)
			{
				DurTemp = (int)Dur;
				DurTemp += 2;
				Dur = DurTemp;
			}
			else if (Trip)
			{
				DurTemp = (int)Dur;
				DurTemp -= 2;
				Dur = DurTemp;
			}
			pN->SetDuration(Dur);
			pN->SetFlags(0); // set quarter note flag
			Invalidate();
		}
		break;
	case 'H':	// OnKeyDown change to half note
		if (DrawMode::NOTE == m_dmDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)GetDrawObject();
			Dur = pN->GetDuration();
			Dot = CMsNote::GetDurationTable()[(int)Dur].Dotted;
			Trip = CMsNote::GetDurationTable()[(int)Dur].Triplet;
			Dur = MSFF_HALF_NOTE;
			if (Dot)
			{
				DurTemp = (int)Dur;
				DurTemp += 2;
				Dur = DurTemp;
			}
			else if (Trip)
			{
				DurTemp = (int)Dur;
				DurTemp -= 2;
				Dur = DurTemp;
			}
			pN->SetDuration(Dur);
			pN->SetFlags(0); // set half note flag
			Invalidate();
		}
		break;
	case 'W':	// OnKeyDown change to whole note
		if (DrawMode::NOTE == m_dmDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)GetDrawObject();
			Dur = pN->GetDuration();
			Dot = CMsNote::GetDurationTable()[(int)Dur].Dotted;
			Trip = CMsNote::GetDurationTable()[(int)Dur].Triplet;
			Dur = MSFF_WHOLE_NOTE;
			if (Dot)
			{
				DurTemp = (int)Dur;
				DurTemp += 2;
				Dur = DurTemp;
			}
			else if (Trip)
			{
				DurTemp = (int)Dur;
				DurTemp -= 2;
				Dur = DurTemp;
			}
			pN->SetDuration(Dur);
			pN->SetFlags(0); // set whole note flag	
			Invalidate();
		}
		break;
	case VK_ESCAPE:	// OnKeyDown exit draw mode
		switch (m_dmDrawMode)
		{
		case DrawMode::NOTE:	// OnKeyDown exit draw mode
			pN = (CMsNote*)GetDrawObject();
			if (pN && pN->IsValidNote())
			{
				//------------------------------
				// If the draw object is a NOTE,
				// we need to make sure that it
				// is turned off.
				//------------------------------
				pN->NoteOff(0);
				CMsEvent* pEv = pN->GetParentEvent();
				if (pEv)
				{
					if(pEv->IsThisObjectInThisEvent(pN))
						pEv->RemoveObject(pN);
					else
						fprintf(stderr, "Error: OnKeyDown Escape Key: Note not found in event.\n");
				}
			}
			break;
		case DrawMode::GLISSANDO:		// OnKeyDown exit draw mode
			break;
		case DrawMode::BAR:		// OnKeyDown exit draw mode
			pObj = GetDrawObject();
			if (pObj)
			{
				CMsBar* pBar = (CMsBar*)pObj;
				CMsEvent* pEv = pBar->GetParentEvent();
				if (pEv)
				{
					if (pEv->IsThisObjectInThisEvent(pBar))
						pEv->RemoveObject(pBar);
					else
						fprintf(stderr, "Error: OnKeyDown Escape Key: Bar not found in event.\n");
				}
			}
			SetDrawObject(0);
			m_dmDrawMode = DrawMode::NOP;
			break;
		case DrawMode::ENDBAR:		// OnKeyDown exit draw mode
			pObj = GetDrawObject();
			if (pObj)
			{
				CMsEndBar* pEndBar = (CMsEndBar*)pObj;
				CMsEvent* pEv = pEndBar->GetParentEvent();
				if (pEv)
				{
					if (pEv->IsThisObjectInThisEvent(pEndBar))
						pEv->RemoveObject(pEndBar);
					else
						fprintf(stderr, "Error: OnKeyDown Escape Key: End Bar not found in event.\n");
				}
			}
			SetDrawObject(0);
			m_dmDrawMode = DrawMode::NOP;
			break;
		case DrawMode::KEYSIG:		// OnKeyDown exit draw mode
			pObj = GetDrawObject();
			if (pObj)
			{
				CMsKeySignature* pKeySig = (CMsKeySignature*)pObj;
				CMsEvent* pEv = pKeySig->GetParentEvent();
				if (pEv)
				{
					if (pEv->IsThisObjectInThisEvent(pKeySig))
						pEv->RemoveObject(pKeySig);
					else
						fprintf(stderr, "Error: OnKeyDown Escape Key: Key Signature not found in event.\n");
				}
			}
			SetDrawObject(0);
			m_dmDrawMode = DrawMode::NOP;
			break;
		case DrawMode::LOUDNESS:		// OnKeyDown exit draw mode
			pObj = GetDrawObject();
			if (pObj)
			{
				CMsLoudness* pLoudness = (CMsLoudness*)pObj;
				CMsEvent* pEv = pLoudness->GetParentEvent();
				if (pEv)
				{
					if (pEv->IsThisObjectInThisEvent(pLoudness))
						pEv->RemoveObject(pLoudness);
					else
						fprintf(stderr, "Error: OnKeyDown Escape Key: Loudness not found in event.\n");
				}
			}
			SetDrawObject(0);
			m_dmDrawMode = DrawMode::NOP;
			break;
		case DrawMode::REPEAT_START:		// OnKeyDown exit draw mode
			pObj = GetDrawObject();
			if (pObj)
			{
				CMsRepeatStart* pRepeat = (CMsRepeatStart*)pObj;
				CMsEvent* pEv = pRepeat->GetParentEvent();
				if (pEv)
				{
					if (pEv->IsThisObjectInThisEvent(pRepeat))
						pEv->RemoveObject(pRepeat);
					else
						fprintf(stderr, "Error: OnKeyDown Escape Key: Repeat not found in event.\n");
				}

			}
			SetDrawObject(0);
			m_dmDrawMode = DrawMode::NOP;
			break;
		case DrawMode::REST:		// OnKeyDown exit draw mode
			pObj = GetDrawObject();
			if (pObj)
			{

			}
			SetDrawObject(0);
			m_dmDrawMode = DrawMode::NOP;
			break;
		case DrawMode::TEMPO:		// OnKeyDown exit draw mode
			pObj = GetDrawObject();
			if (pObj)
			{

			}
			SetDrawObject(0);
			m_dmDrawMode = DrawMode::NOP;
			break;
		case DrawMode::TIMESIG:		// OnKeyDown exit draw mode
			pObj = GetDrawObject();
			if (pObj)
			{

			}
			SetDrawObject(0);
			m_dmDrawMode = DrawMode::NOP;
			break;
		case DrawMode::TIE:		// OnKeyDown exit draw mode
			break;
		case DrawMode::COPY:		// OnKeyDown exit draw mode
			[[fallthrough]];
		case DrawMode::MOVE:		// OnKeyDown exit draw mode
			break;
		case DrawMode::NOP:
			break;
		}	//end of switch(m_dmDrawMode) in case MOUSE_IN_UPPERSEL
		m_dmDrawMode = DrawMode::NOP;
		//----------------------------------
		// The Next Three Lines of code are
		// required to be in this order 
		// because we need to set GetDrawObject()
		// to zero first before deleting it.
		// The delete methode does not nessesarily
		// return here right away.  This means
		// that we need to insure that OnPaint
		// does not use GetDrawObject() befpre
		// It is deleted.
		//-----------------------------------------------
		pTemp = m_pDrawObject;
		SetDrawObject(0);
		delete pTemp;
		pTemp = 0;
		m_Status.SetText(csBlank);
//		if(LogFile()) fprintf(LogFile(),"################ Escape Key\n");
		m_EscapeFlag = 1;
		//		Invalidate();
		break;
	case VK_SPACE:	// OnKeyDown toggle stemp UP/Down
		if (GetDrawObject())
		{
			if (GetDrawObject()->GetType() == CMsObject::MsObjType::NOTE)
			{
				CMsNote* pN = (CMsNote*)GetDrawObject();
				if (m_CtrlKeyDown)
					pN->SetHeadFlipped(pN->GetHeadFlipped() ^ 1);
				else
				{
					pN->SetStemDown(pN->GetStemDown() ^ 1);
					if (pN->IsStemDown())
						pN->SetHeadFlipped(1);
					else
						pN->SetHeadFlipped(0);
				}
				Invalidate();
				pN->Print(stdout, 0);
			}
		}
		break;
	default:
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		break;
	}
}

void CChildViewStaff::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_MENU:		// OnKeyUp
		m_AltKeyDown = 0;
		break;
	case VK_SHIFT:		// OnKeyUp
		m_ShiftKeyDown = 0;
		break;
	case VK_CONTROL:		// OnKeyUp	
		m_CtrlKeyDown = 0;
		break;
	default:		// OnKeyUp
		CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
		break;
	}
}

#define ID_CM_INSERTEVEHNT		100
#define ID_CM_DELETEEVENT		101
#define ID_CM_CHANGEINST		102
#define ID_CM_CHANGEDURATION	103
#define ID_CM_EXTRA_ITEMS		120

void CChildViewStaff::OnContextMenu(CWnd* pWnd, CPoint pointMouseCntxMen)
{
	CMenu ConTexMenu;	//OnContextMenu
	CRect rect;
	GetWindowRect(&rect);
	int Event = XtoEventIndex(pointMouseCntxMen.x - rect.left);
	int id;
	CContextMenuList* pContextMenueList = 0;
	CString csStatus, csTemp;

	//-------------- Get the current event ----------------------
	CMsEvent* pEv = m_pSong->GetEventObject(Event);

	ConTexMenu.CreatePopupMenu();
	ConTexMenu.AppendMenuW(MF_STRING, ID_CM_INSERTEVEHNT, _T("Insert Event"));
	if (pEv)ConTexMenu.AppendMenuW(MF_STRING, ID_CM_DELETEEVENT, _T("Delete Event"));
	if ((m_dmDrawMode == DrawMode::DRAW_NOTES_VIA_MIDI) && pEv)
	{
		ConTexMenu.AppendMenuW(MF_STRING, ID_CM_CHANGEINST, _T("Change Instrument"));
		ConTexMenu.AppendMenuW(MF_STRING, ID_CM_CHANGEDURATION, _T("Change Note Duration"));
	}
	if ((m_dmDrawMode == DrawMode::NOTE) && pEv)
	{

	}
	ObjectTypes Obj;
	if (pEv)
	{
		//---------------------------------------
		// Get the number of selected objects
		//----------------------------------------
		int n = pEv->AreObjectsSelected();
		if (n)
		{
			int l;
			pContextMenueList = new CContextMenuList(n);
			Obj.pObj = pEv->GetFirstSelectedObject();
			//--------------------------------------
			// Make menu entries for objects
			// if needed
			//--------------------------------------
			for (int i = 0; i < n; ++i)
			{
				l = 0;
				switch (Obj.pObj->GetType())
				{
				case CMsObject::MsObjType::BAR:	//OnContextMenu
					break;
				case CMsObject::MsObjType::ENDBAR:	//OnContextMenu
					break;
				case CMsObject::MsObjType::KEYSIG:	//OnContextMenu
					csStatus.Format(_T("Key Signature"));
					ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EXTRA_ITEMS + i, csStatus);
					pContextMenueList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case CMsObject::MsObjType::LOUDNESS:	//OnContextMenu
					csStatus.Format(_T( "Loudness"));
					ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EXTRA_ITEMS + i, csStatus);
					pContextMenueList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case CMsObject::MsObjType::NOTE:	//OnContextMenu
					csStatus = CString("Note:");
					Obj.pNote->ObjectToString(csTemp);
					csStatus.Append(csTemp);
					ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EXTRA_ITEMS + i, csStatus);
					pContextMenueList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case CMsObject::MsObjType::REPEATEND:	//OnContextMenu
				case CMsObject::MsObjType::REPEATSTART:
					csStatus.Format(_T("Repeat"));
					ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EXTRA_ITEMS + i, csStatus);
					pContextMenueList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case CMsObject::MsObjType::TEMPO:	//OnContextMenu
					csStatus.Format(_T("Tempo"));
					ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EXTRA_ITEMS + i, csStatus);
					pContextMenueList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case CMsObject::MsObjType::TIMESIG:	//OnContextMenu
					csStatus.Format(_T("Time Signature"));
					ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EXTRA_ITEMS + i, csStatus);
					pContextMenueList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				}
				Obj.pObj = pEv->GetNextSelectedObject(Obj.pObj);
			}	//end of for loop
		}
	}
	id = ConTexMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pointMouseCntxMen.x, pointMouseCntxMen.y, this);
	switch (id)
	{
	case ID_CM_INSERTEVEHNT:	//OnContextMenu
		m_pSong->InsertEvent(Event);
		m_pSong->RenumberEvents(&m_FirstSelectedEvent, &m_LastSelectedEventIndex);
		UpdateScrollbarInfo(m_pSong->GetTotalEvents());
		Invalidate();
		break;
	case ID_CM_DELETEEVENT:	//OnContextMenu
		m_pSong->RemoveEvent(Event);
		UpdateScrollbarInfo(m_pSong->GetTotalEvents());
		Invalidate();
		break;
	case ID_CM_CHANGEINST:	//OnContextMenu
	{
		CSelectorDlg Dlg;
		int nTracks = GetSong()->GetNumberOfTracks();
		CMyBitmap** ppBmps = new CMyBitmap * [nTracks];
		for(int i = 0; i < nTracks; ++i)
		{
			ppBmps[i] = GetSong()->GetSongInfo()->GetTrack(i + 1)->GetBitmap();
		}
		Dlg.SetBitmaps(ppBmps);
		Dlg.SetNumberOfBitmaps(nTracks);
		Dlg.SetSelection((int)((m_nMidiInputNoteSetup >> DRAW_NOTE_TRACK_SHIFT) & 0x0f)-1);
		Dlg.SetCaption(CString( "Select New Instrument"));
		if (IDOK == Dlg.DoModal())
		{
			m_nMidiInputNoteSetup &= ~(0x0f << DRAW_NOTE_TRACK_SHIFT);
			m_nMidiInputNoteSetup |= (Dlg.GetSelection() +1) << DRAW_NOTE_TRACK_SHIFT;
			int Track = Dlg.GetSelection() + 1;
			int Patch = GetTrackInfo(Track)->GetPatch();
			int DeviceID = GetTrackInfo(Track)->GetMidiOutDeviceID();
			int Channel = GetTrackInfo(Track)->GetChannel();
			GETAPP->GetMidiOutTable()->GetDevice(DeviceID).PgmChange(Channel, Patch);	// change the patch on the currently selected midi out device and channel to match the patch for the track that is currently selected for note input
		}
	}
	break;
	case ID_CM_CHANGEDURATION:	//OnContextMenu
	{
		CSelDurDlg Dlg;
		Dlg.m_pPrompt = "Select New Note Duration";
		Dlg.m_nDuration = m_nMidiInputNoteSetup & DRAW_NOTE_DURATION;
		Dlg.DoModal();
		m_nMidiInputNoteSetup &= ~DRAW_NOTE_DURATION;
		m_nMidiInputNoteSetup |= Dlg.m_nDuration;
	}
	break;
	default:	//OnContextMenu
		if (pContextMenueList)
		{
			int index = pContextMenueList->FindID(id);
			if (index >= 0)
			{
				Obj.pObj = pContextMenueList->GetObject(index);
				switch (Obj.pObj->GetType())
				{
				case CMsObject::MsObjType::BAR:	//OnContextMenu
				case CMsObject::MsObjType::ENDBAR:
					break;
				case CMsObject::MsObjType::KEYSIG:	//OnContextMenu
				{
					CSelectStringDlg Dlg;
					Dlg.SetCaptionString( CString("Change Key Signature"));
					Dlg.SetNumStrings(GETAPP->GetNumKeySigs() );
					for (int i = 0; i < (GETAPP->GetNumKeySigs()); i++)
					{
						Dlg.SetSelectionString(i, CMsKeySignature::KeySigStringTab[i + 1]);
					}
					Dlg.m_nSelection = (int)Obj.pKey->GetKeySignature() - 1;
					if (IDOK == Dlg.DoModal())
						Obj.pKey->SetKeySignature(CMsKeySignature::KeySigID(Dlg.m_nSelection));
				}
				break;
				case CMsObject::MsObjType::LOUDNESS:	//OnContextMenu
				{
					CParamDlg Dlg;

					Dlg.SetCaption(CString("Change Loudness"));
					Dlg.SetValue( Obj.pLoud->GetLoudness());
					Dlg.SetMax(127);
					Dlg.SetMin(1);
					if (IDOK == Dlg.DoModal())
					{
						Obj.pLoud->SetLoudness(Dlg.GetValue());
					}
				}
				break;
				case CMsObject::MsObjType::NOTE:	//OnContextMenu
				{
					CNotePropertiesDlg Dlg;

					Dlg.SetNoteToEdit(Obj.pNote);
					Dlg.DoModal();
				}
				break;
				case CMsObject::MsObjType::REPEATEND:	//OnContextMenu
				case CMsObject::MsObjType::REPEATSTART:
					if (m_pRepeatEndSelected && m_pRepeatStartSelected)
					{
						CParamDlg Dlg;

						Dlg.SetCaption(CString("Set Repeat Count"));
						Dlg.SetValue(m_pRepeatStartSelected->GetCount());
						Dlg.SetMax(100);
						Dlg.SetMin(1);
						if (IDOK == Dlg.DoModal())
						{
							m_pRepeatStartSelected->SetCount(Dlg.GetValue());
						}
					}
					break;
				case CMsObject::MsObjType::TEMPO:	//OnContextMenu
				{
					CParamDlg Dlg;
				
					Dlg.SetCaption(CString("Change Tempo"));
					Dlg.SetValue(Obj.pTemp->GetQNPM());
					Dlg.SetMax(230);
					Dlg.SetMin(1);
					if (IDOK == Dlg.DoModal())
					{
						Obj.pTemp->SetQNPM(Dlg.GetValue());
					}
				}
				break;
				case CMsObject::MsObjType::TIMESIG:	//OnContextMenu
				{
					CSelectorDlg Dlg;
					Dlg.SetBitmaps(GETAPP->bmGetCbTimeSig());
					Dlg.SetNumberOfBitmaps(GETAPP->GetNumTimeSig());
					Dlg.SetSelection((int)Obj.pTime->GetTimeSignature());
					Dlg.SetCaption(CString("Change Time Signature"));
					if (IDOK == Dlg.DoModal())
						Obj.pTime->SetTimeSignature((Dlg.GetSelection() + 1));
				}
				break;
				}//end of switch(Obj.pObj->GetType()
			}//end of if(index >= 0)
		}//end of if(ppList)
		break;
	}//end of switch(id)		OnContextMenu
}

int CChildViewStaff::YtoNote(int MouseY)
{
	int NoteIndex;
	int Octave = 0;
	int Note;
	//----------------------------
	// Notes place on staff will
	// start at HIGHC_OFFSET and
	// go to HIGHC_OFFSET- 8, so
	// we need to offset this by
	// 4
	//---------------------------------
	// subract off the offset for
	// C8 from the mouse Y coordiant
	// so that we know where to draw
	//----------------------------------
	MouseY -= CLIENT_Y_TO_TOP_OF_EDIT_RECT;
	//------- Invert Y Coord --------------
	MouseY = EDIT_RECT_HEIGHT - MouseY;
	//----------------------------------
	// quantize the y coordiant so that
	// the note will only be drawn either
	// in the space, or on the line.  We do
	// not quantize the mouse pointer
	//-----------------------------------
	MouseY -= STAVE_LINE_SPACING / 2;	//center
	MouseY = QuantizeY(MouseY);
	//------------------------------------
	// Limit the range that y can take
	//------------------------------------
	if (MouseY < 0)
		MouseY = 0;
	NoteIndex = MouseY % 7;
	Note = CMsNote::NoteLut[NoteIndex];
	Octave += (MouseY / 7) * 12;	// calculate octave
	Note = Note + Octave + 0x24;
	if (Note > 0x60)
		Note = 0x60;	// limit max value of Note
	return Note;
}


UINT CChildViewStaff::GetRawEventNumber(int x)
{
	//----------------------------------
	// calculates the "event" to its
	// physical location on the staff
	// For the logical "event" use
	// the function XtoEventIndex
	//-----------------------------------
	int ev = x - EVENT_WIDTH;
	int r = ev % EVENT_WIDTH;
	ev /= (int)EVENT_WIDTH;
	if (ev < 0) ev = 0;
	else if (ev > m_MaxEvents) ev = m_MaxEvents - 1;
	return (UINT)ev;
}

int CChildViewStaff::XtoEventIndex(int x)
{
	m_nRawEvent = GetRawEventNumber(x);
	return m_nRawEvent + m_SongScrollPos;
}

int CChildViewStaff::CalcMaxEvents()
{
	CRect rect;

	GetClientRect(&rect);
	int Width = rect.Width();
	Width -= EVENT_WIDTH;
	return Width / EVENT_WIDTH;
}

int CChildViewStaff::QuantizeY(int y)
{
	return y / 4;
}

void CChildViewStaff::SetupDrawMode(DrawMode Mode,long v)
{
	//------------------------
	// SetupDrawMode:
	//
	// This function sets up
	// the drawing mode for the
	// for the appropriate
	// drawing object.
	// anew object is created
	// as well of other
	// parameters.
	//------------------------
	CMsEvent* pEv = 0;
	CString csStatus,csTemp;
	CString csBlank(_T(""));
	int From, To;
	//------------------------
	// Decode sub message
	//------------------------
	switch (Mode)
	{
	case DrawMode::NOP:		// SetupDrawMode
		if (GetDrawObject())
		{
			pEv = GetDrawObject()->GetParentEvent();
			if (pEv)
			{
				pEv->RemoveObject(GetDrawObject());
			}
			delete m_pDrawObject;
			SetDrawObject(0);
		}
		m_dmDrawMode = DrawMode::NOP;
		m_Status.SetText(csBlank);
		break;
	case DrawMode::NOTE:	// SetupDrawMode
		{
			CMsNote* pN = new CMsNote;
			pN->Create(0, GetSong(), GetSong()->GetEventObject(m_nDrawEvent));
			if (GetDrawObject())
			{
				pEv = GetDrawObject()->GetParentEvent();
				if (pEv)
				{
					pEv->RemoveObject(GetDrawObject());
				}
				delete m_pDrawObject;
				SetDrawObject(0);
			}
			SetDrawObject(pN);
			m_dmDrawMode = DrawMode::NOTE;
			m_nDrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
			//-----------------------------
			// decode message word
			//------------------------------
			pN->GetData().CopyData(GetNoteData());
			if (pN->IsRest())
			{
				int Shape;
				Shape = pN->GetShape();
				int Id = CMidiSeqMSApp::GetRestBmIdsTypes()[Shape];
				pN->Create(Id, GetSong(), GetSong()->GetEventObject(m_nDrawEvent));
			}
			else
				pN->Create(0, GetSong(), GetSong()->GetEventObject(m_nDrawEvent));

			pN->ObjectToString(csTemp);
			if(GetSong()->GetEventObject(m_nDrawEvent))
				csStatus.Format(_T("Draw %lS Event %d"), 
					csTemp.GetString(), 
					GetSong()->GetEventObject(m_nDrawEvent)->GetIndex()
				);
		}
		m_Status.SetText(csStatus);
		break;
	case DrawMode::GLISSANDO:	// SetupDrawMode
		{
			CMsGlissando* pMG = new CMsGlissando;
			if (GetDrawObject())
			{
				pEv = GetDrawObject()->GetParentEvent();
				if (pEv)
				{
					pEv->RemoveObject(GetDrawObject());
				}
				delete m_pDrawObject;
				SetDrawObject(0);
			}
			SetDrawObject(pMG);
			m_dmDrawMode = DrawMode::GLISSANDO;
			m_nDrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
			//-----------------------------
			// decode message word
			//------------------------------
			pMG->Create(GetSong(), GetSong()->GetEventObject(m_nDrawEvent), 0);
			pMG->ObjectToString(csTemp);
			if (!GetSong()->GetEventObject(m_nDrawEvent))
			{
				GetSong()->AddMoreEventsAtEnd(m_nDrawEvent);
			}
		}
		csStatus.Format(_T("Draw %lS Event %d"), csTemp.GetString(), GetSong()->GetEventObject(m_nDrawEvent)->GetIndex());
		m_Status.SetText(csStatus);
		break;
	case DrawMode::REST:	// SetupDrawMode
		break;
	case DrawMode::ENDBAR:	// SetupDrawMode
		{
			if (GetDrawObject())
				{
					pEv = GetDrawObject()->GetParentEvent();
					if (pEv)
					{
						pEv->RemoveObject(GetDrawObject());
					}
					delete m_pDrawObject;
					SetDrawObject(0);
			}	
			CMsEndBar* pEB = new CMsEndBar;
			pEB->Create(GetSong(), GetSong()->GetEventObject(m_nDrawEvent));
			m_dmDrawMode = DrawMode::ENDBAR;
			SetDrawObject(pEB);
		}
		csTemp = CString("Draw Measure End Bar");
		m_Status.SetText(csTemp);
		break;
	case DrawMode::BAR:	// SetupDrawMode
	{
		if(GetDrawObject())
		{
			pEv = GetDrawObject()->GetParentEvent();
			if (pEv)
			{
				pEv->RemoveObject(GetDrawObject());
			}
			delete m_pDrawObject;
			SetDrawObject(0);
		}
		CMsBar* pB = new CMsBar;
		pB->Create(GetSong(), GetSong()->GetEventObject(m_nDrawEvent));
		m_dmDrawMode = DrawMode::BAR;
		SetDrawObject(pB);
	}
	csTemp = CString("Draw Measure Bar");
	m_Status.SetText(csTemp);
	break;
	case DrawMode::TIE:		// SetupDrawMode
		if (GetDrawObject())
		{
			pEv = GetDrawObject()->GetParentEvent();
			if (pEv)
			{
				pEv->RemoveObject(GetDrawObject());
			}
			delete m_pDrawObject;
			SetDrawObject(0);
		}
		m_dmDrawMode = DrawMode::TIE;
		m_nDrawState = DRAWSTATE::TIE_FIRSTNOTE;
		m_pFirstTieNote = 0;
		m_pSecondTieNote = 0;
		csTemp = CString("Draw Tie");
		m_Status.SetText(csTemp);
		break;
	case DrawMode::COPY:		// SetupDrawMode
		m_dmDrawMode = DrawMode::COPY;
		csTemp = CString("Copy Block");
		m_Status.SetText(csTemp);
		break;
	case DrawMode::MOVE:		// SetupDrawMode
		m_dmDrawMode = DrawMode::MOVE;
		csStatus.Format(_T("Move Block To Event %d"), m_nDrawEvent);
		m_Status.SetText(csStatus);
		break;
	case DrawMode::REPEAT_START:		// SetupDrawMode
		AddRepeat(v);
		Invalidate();
		csTemp = CString("Add Repeat");
		m_Status.SetText(csTemp);
		break;
	case DrawMode::TEMPO:		// SetupDrawMode
	{
		if (GetDrawObject())
		{
			pEv = GetDrawObject()->GetParentEvent();
			if (pEv)
			{
				pEv->RemoveObject(GetDrawObject());
			}
			delete m_pDrawObject;
			SetDrawObject(0);
		}
		CMsTempo* pMT = new CMsTempo();
		pMT->Create(GetSong(), GetSong()->GetEventObject(m_nDrawEvent), v);
		m_dmDrawMode = DrawMode::TEMPO;
		SetDrawObject(pMT);
	}
	csTemp = CString("Add Tempo");
	m_Status.SetText(csTemp);
	break;
	case DrawMode::TIMESIG:		// SetupDrawMode
	{
		if (GetDrawObject())
		{
			pEv = GetDrawObject()->GetParentEvent();
			if (pEv)
			{
				pEv->RemoveObject(GetDrawObject());
			}
			delete m_pDrawObject;
			SetDrawObject(0);
		}
		CMsTimeSignature* pTS = new CMsTimeSignature;
		pTS->Create(GetSong(), GetSong()->GetEventObject(m_nDrawEvent), COMBO_TIMESIG_4_4);
		m_dmDrawMode = DrawMode::TIMESIG;
		SetDrawObject(pTS);
	}
	csTemp = CString("Add Time Signature");
	m_Status.SetText(csTemp);
	break;
	case DrawMode::KEYSIG:		// SetupDrawMode
	{
		CMsKeySignature::KeySigID KS;

		KS = (CMsKeySignature::KeySigID)(v & 0x1f);
		if (GetDrawObject())
		{
			pEv = GetDrawObject()->GetParentEvent();
			if (pEv)
			{
				pEv->RemoveObject(GetDrawObject());
			}
			delete m_pDrawObject;
			SetDrawObject(0);
		}
		CMsKeySignature* pKS = new CMsKeySignature;
		pKS->Create(GetSong(), GetSong()->GetEventObject(m_nDrawEvent), KS);
		m_dmDrawMode = DrawMode::KEYSIG;
		SetDrawObject(pKS);
	}
	csTemp = CString("Add Key Signature");
	m_Status.SetText(csTemp);
	break;
	case DrawMode::LOUDNESS:		// SetupDrawMode
	{
		if (GetDrawObject())
		{
			pEv = GetDrawObject()->GetParentEvent();
			if (pEv)
			{
				pEv->RemoveObject(GetDrawObject());
			}
			delete m_pDrawObject;
			SetDrawObject(0);
		}
		CMsLoudness* pLD = new CMsLoudness;
		pLD->Create(GetSong(), GetSong()->GetEventObject(m_nDrawEvent), v);
		m_dmDrawMode = DrawMode::LOUDNESS;
		SetDrawObject(pLD);
	}
	csTemp = CString("Add Loudness");
	m_Status.SetText(csTemp);
	break;
	case DrawMode::INSTCHANGE:		// SetupDrawMode
		From = v & 0xf;
		To = (v >> 4) & 0x0f;
		ChangeInst(
			From, 
			To
		);
		Invalidate();
		csTemp = CString("Change Instrument in Selection");
		m_Status.SetText(csTemp);
		break;
	case DrawMode::CHANGEDUR:		// SetupDrawMode
		From = v & 0x1f;
		To = (v >> 5) & 0x1f;
		ChangeDuration(From, To);
		Invalidate();
		csTemp = CString("Change Duration in Selection");
		m_Status.SetText(csTemp);
		break;
	case DrawMode::INCREASEDUR:		// SetupDrawMode
		IncreaseDuration();
		csTemp = CString("Increase Duration in Selection");
		m_Status.SetText(csTemp);
		break;
	case DrawMode::DECREASEDUR:		// SetupDrawMode
		//		DecreaseDuration();  ToDo Add this function
		csTemp = CString("Decrease Duration in Selection");
		m_Status.SetText(csTemp);
		break;
	case DrawMode::INCRPITCH:
			IncrPitch();
		csTemp = CString("Increase Pitch In Selection");
		m_Status.SetText(csTemp);
		Invalidate();
		break;
	case DrawMode::DECRPITCH:		// SetupDrawMode
		DecrPitch();
		csTemp = CString("Decrease Pitch in Selection");
		m_Status.SetText(csTemp);
		Invalidate();
		break;
	case DrawMode::INSERTBLOCK:		// SetupDrawMode
		InsertBlock();
		// ToDo Might need more code here
		SetScrollRange(GetSong()->GetTotalEvents() - GetMaxEvents());
		csTemp = CString("Insert Event");
		m_Status.SetText(csTemp);
		Invalidate();
		break;
	case DrawMode::DRAW_NOTES_VIA_MIDI:		// SetupDrawMode
		m_dmDrawMode = DrawMode::DRAW_NOTES_VIA_MIDI;
		m_nMidiInputNoteSetup = v;
		csTemp = CString("Step Input Notes Via MIDI");
		m_Status.SetText(csTemp);
		break;
	default:	// SetupDrawMode
		break;
	}
}

//void CChildViewStaff::GetEventRect(int Event,CRect& rect)
//{
//	int x = FIRST_EVENT_FROM_CLIENT_X + EVENT_WIDTH * Event;
//	int y = CLIENT_TO_STAVE_TOP_Y - HIGH_C_FROM_TOP_OF_TREBLE_STAVE;
//	int cx = EVENT_WIDTH;
//	int cy = STAFF_HEIGHT;
//	rect = CRect(CPoint(x, y), CSize(cx, cy));
//}

MouseRegions CChildViewStaff::MouseInRegion(CPoint p)
{
	MouseRegions rV = MouseRegions::MOUSE_OUTSIDE;

	if (m_rgnEdit.PtInRegion(p)) 
		rV = MouseRegions::MOUSE_IN_EDITREG;
	else if (m_rgnUpperSelect.PtInRegion(p)) 
		rV = MouseRegions::MOUSE_IN_UPPERSEL;
	else if (m_rgnUpperDraw.PtInRegion(p)) 
		rV = MouseRegions::MOUSE_IN_UPPERDRAW;
	else if (m_rgnLowerDraw.PtInRegion(p)) 
		rV = MouseRegions::MOUSE_IN_LOWERDRAW;
	else if (m_rgnLowerSelect.PtInRegion(p)) 
		rV = MouseRegions::MOUSE_IN_LOWERSEL;
	return rV;
}

void CChildViewStaff::IncreaseDuration()
{
}

void CChildViewStaff::IncrPitch()
{
	if ((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox(_T("No Block Selected"), _T("Oppsie"));
	else
	{
		CMsEvent* pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
		for (int i = m_FirstSelectedEvent; i < m_LastSelectedEventIndex + 1; ++i)
		{
			CMsObject* pOb = pEv->GetEventMsObjectHead();
			while (pOb)
			{
				if (CMsObject::MsObjType::NOTE == pOb->GetType())
				{
					CMsNote* pN = (CMsNote*)pOb;
					pN->IncrNote();
				}
				pOb = pOb->GetNext();
			}
			pEv = pEv->GetNext();
		}
	}
}

void CChildViewStaff::DecrPitch()
{
	if ((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox(_T("No Block Selected"), _T("Oppsie"));
	else
	{
		CMsEvent* pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
		for (int i = m_FirstSelectedEvent; i < m_LastSelectedEventIndex + 1; ++i)
		{
			CMsObject* pOb = pEv->GetEventMsObjectHead();
			while (pOb)
			{
				if (CMsObject::MsObjType::NOTE == pOb->GetType())
				{
					CMsNote* pN = (CMsNote*)pOb;
					pN->DecrNote();
				}
				pOb = pOb->GetNext();
			}
			pEv = pEv->GetNext();
		}
	}
}

void CChildViewStaff::InsertBlock()
{
	int i, n;
	n = m_LastSelectedEventIndex - m_FirstSelectedEvent + 1;
	for (i = 0; i < n; ++i)
		m_pSong->InsertEvent(m_FirstSelectedEvent);
	m_pSong->RenumberEvents(&m_FirstSelectedEvent, &m_LastSelectedEventIndex);
}

void CChildViewStaff::AddRepeat(UINT nRepeatCount)
{
	if (m_FirstSelectedEvent < 0 || m_LastSelectedEventIndex < 0)
		MessageBox(_T("Block Not Selected"), _T("Oopsie"));
	else
	{
		//--------------------------------
		// We add the repeat by adding
		// a new event before and after
		// the selected block.
		//---------------------------------
		CMsEvent* pRS, * pRE;
		CMsRepeatStart* pRepStrt;
		CMsRepeatEnd* pRepEnd;

		pRS = m_pSong->InsertEvent(m_FirstSelectedEvent - 1);
		m_pSong->RenumberEvents(&m_FirstSelectedEvent, &m_LastSelectedEventIndex);
		pRE = m_pSong->InsertEvent(m_LastSelectedEventIndex);
		m_pSong->RenumberEvents(&m_FirstSelectedEvent, &m_LastSelectedEventIndex);
		pRepStrt = new CMsRepeatStart();
		pRepStrt->Create(GetSong(), nRepeatCount, pRS);
		pRS->AddObject(pRepStrt);
		pRepEnd = new CMsRepeatEnd;
		pRepEnd->Create(GetSong(), pRE);
		pRE->AddObject(pRepEnd);
	}
}

void CChildViewStaff::ChangeInst(INT From, INT To)
{
	if ((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox(_T("No Block Selected"), _T("Oppsie"));
	else
	{
		CMsEvent* pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
		for (int i = m_FirstSelectedEvent; i < m_LastSelectedEventIndex + 1; ++i)
		{
			CMsObject* pOb = pEv->GetEventMsObjectHead();
			while (pOb)
			{
				if (CMsObject::MsObjType::NOTE == pOb->GetType())
				{
					CMsNote* pN = (CMsNote*)pOb;
					if (pN->GetTrack() == From)
						pN->SetTrack(To);
				}
				pOb = pOb->GetNext();
			}
			pEv = pEv->GetNext();
		}
	}
}

void CChildViewStaff::ChangeDuration(INT From, INT To)
{
	if ((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox(_T("No Block Selected"), _T("Oppsie"));
	else
	{
		CMsEvent* pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
		for (int i = m_FirstSelectedEvent; i < m_LastSelectedEventIndex + 1; ++i)
		{
			CMsObject* pOb = pEv->GetEventMsObjectHead();
			while (pOb)
			{
				if (CMsObject::MsObjType::NOTE == pOb->GetType())
				{
					CMsNote* pN = (CMsNote*)pOb;
					if (pN->GetDuration() == From)
						pN->SetDuration(To);
				}
				pOb = pOb->GetNext();
			}
			pEv = pEv->GetNext();
		}
	}
}

void CChildViewStaff::CopyBlock(int dest)
{
	//-------------------------------
	// first thing, copy the block
	//-------------------------------
	CMsEvent* pHead = 0, * pTail = 0;
	CMsEvent* pEv, * pEvNew;
	CMsEventChain pEventChain;

	if ((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox(_T("No Block Selected"), _T("Oppsie"));
	else
	{
		pEv = GetSong()->GetEventObject(m_FirstSelectedEvent);
		while (pEv && (pEv->GetIndex() != m_LastSelectedEventIndex + 1) )
		{
			CMsObject* pOb;
			pEvNew = m_pSong->MakeNewEvent();;
			pOb = pEv->GetEventMsObjectHead();

			while (pOb)
			{
				CMsObject* pNewObj;
				pNewObj = pOb->MakeANewObject();
				pNewObj->Copy(pOb);
				pEvNew->AddObject(pNewObj);
				pOb = pOb->GetNext();
			}
			pEventChain.AddEventAtEnd(pEvNew);
			pEv = pEv->GetNext();
		}
		//--------------------------------
		// Chain is created.  Now, insert
		// into the song at the desired
		// spot
		//--------------------------------
		GetSong()->AddEventChain(dest, &pEventChain);
	}
	m_pSong->RenumberEvents(&m_FirstSelectedEvent, &m_LastSelectedEventIndex);
}

void CChildViewStaff::MoveBlock(int dest)
{
	CMsEvent* pDest = 0;
	CMsEventChain EventChain;
	CString csTemp;
	int n;

	n = dest - GetSong()->GetEventListTail()->GetIndex() - 1;
	///--------------------------
	/// snip out block
	///----------------------------
	if ((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox(_T("No Block Selected"), _T("Oppsie"));
	else if ((dest >= m_FirstSelectedEvent) && (dest <= m_LastSelectedEventIndex))
	{
		csTemp = CString("Error:Move:Dest cannot Include Selected Events");
		m_Status.SetText(csTemp);
	}
	else
	{
		pDest = m_pSong->GetEventObject(dest);
		//----- Get chain to move ----
		EventChain.SetHead(m_pSong->GetEventObject(m_FirstSelectedEvent));
		EventChain.SetTail(m_pSong->GetEventObject(m_LastSelectedEventIndex));
		//---- Unhook Chain ----
		EventChain.UnHookChain();
		///--------------------------------
		/// Chain is created.  Now, insert
		/// into the song at the desired
		/// spot
		///--------------------------------
		GetSong()->AddEventChain(pDest,&EventChain,dest,n);
		m_pSong->RenumberEvents(&m_FirstSelectedEvent, &m_LastSelectedEventIndex);
		EventChain.SetSelected(0);
		m_FirstSelectedEvent = -1;
		m_LastSelectedEventIndex = -1;
	}
}

void CChildViewStaff::UpdateScrollbarInfo(int TotalEvents, const char *Title)
{
	CRect clientrect;
	GetClientRect(&clientrect);
	SCROLLINFO si;
	int HScrollMax;

	if (TotalEvents > m_MaxEvents)
		HScrollMax = TotalEvents;
	else
		HScrollMax = m_MaxEvents;
	memset(&si, 0, sizeof(SCROLLINFO));
	int Max = GetSong()->GetTotalEvents() - GetMaxEvents();
	if (Max < 0) Max = 0;
	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = Max;
	si.nPos = m_nDrawEvent;
	si.nPage = (m_MaxEvents - 1) / 2;
	si.cbSize = sizeof(SCROLLINFO);

	CWnd::SetScrollInfo(SB_HORZ, &si, true);
}


int CChildViewStaff::SetScrollPos(int pos, const char* Title)
{
	return CWnd::SetScrollPos(SB_HORZ, pos, 1);
}


void CChildViewStaff::SetScrollRange(int Range, const char* Title)
{
	int minPos,maxPos;

	--Range;
	GetScrollRange(SB_HORZ, &minPos, &maxPos);
	if (Range < 0) Range = 0;
	CWnd::SetScrollRange(SB_HORZ, 0, Range, 1);
}

void CChildViewStaff::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int Max = GetSong()->GetTotalEvents() - GetMaxEvents();
	if (Max < 0) Max = 0;
	switch (nSBCode)
	{
	case SB_LINELEFT:
		m_SongScrollPos--;
		if (m_SongScrollPos < 0) m_SongScrollPos = 0;
		SetScrollPos(m_SongScrollPos,"  SB_LINE_LEFT ");
		break;
	case SB_LINERIGHT:
		m_SongScrollPos++;
		if (m_SongScrollPos > Max) m_SongScrollPos = Max;
		if(Max) SetScrollPos(m_SongScrollPos, "  SB_LINE_RIGHT ");
		break;
	case SB_PAGELEFT:
		m_SongScrollPos -= 4;
		if (m_SongScrollPos < 0 ) m_SongScrollPos = 0;
		if(m_SongScrollPos) SetScrollPos(m_SongScrollPos, "  SB_PAGE_LEFT ");
		break;
	case SB_PAGERIGHT:
		m_SongScrollPos += 4;
		if (m_SongScrollPos > Max) m_SongScrollPos = Max;
		SetScrollPos(m_SongScrollPos, "  SB_PAGE_RIGHT ");
		break;
	case SB_THUMBTRACK:
		m_SongScrollPos = nPos;
		SetScrollPos(m_SongScrollPos, "  SB_THUMB ");
		break;
	case SB_ENDSCROLL:
		break;
	default:
		break;
	}
	Invalidate();
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

afx_msg LRESULT CChildViewStaff::OnLongMidiMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

afx_msg LRESULT CChildViewStaff::OnShortmidimsg(WPARAM wMsg, LPARAM timestamp)
{
	int Cmd;
	int Vel;
	int Note;
	int note = 0, adj = 0;
	int Chan;
	int Instrument;
	int UpdateNoteInfoFlag = 0;
	int RestFlag = 0;
	int v;
	CString csStatus, csTemp;

	Cmd = CMD(wMsg);
	Chan = CHAN(wMsg);
	Note = NOTE(wMsg);
	Vel = VEL(wMsg);
	switch (Cmd)
	{
	case (int) CMidi::MidiChannelCmds::NOTEOFF:		//OnShortmidimsg
		//--------------------------------
		// what we are going to here is
		// keep track of the notes that
		// are going off.  When no more
		// notes are on, then we deselect
		// the current event, and then
		// select the next event.
		//--------------------------------
		if (Chan == KEYBOARD_MIDI_CHANNEL)
		{
			if (m_nMidiNotesOn)
			{
				m_nMidiNotesOn--;
//				int note, adj;
				note = Note % 12;
				adj = CMsNote::NoteAdjustLUT[note];
				note -= adj;
				note += 12 * (Note / 12);
				CMsEvent* pEv = m_pSong->GetEventObject(m_LastSelectedEventIndex);
				CMsNote* pN = pEv->FindNote(note, adj ? MSFF_ACCIDENTAL_SHARP : MSFF_ACCIDENTAL_INKEY);
				if (pN) 
					MidiPlayNote(pN, 0);// Note Off
				if (m_nMidiNotesOn == 0)
				{
					pEv->SetSelected(0);
					pEv = pEv->GetNext();
					if (pEv)
					{
						m_LastSelectedEventIndex++;
						pEv->SetSelected(1);
					}
					else
					{
						pEv = m_pSong->MakeNewEvent();
						m_pSong->AddEventAtEnd(pEv);
						pEv->SetSelected(1);
						m_LastSelectedEventIndex++;
					}
					if ((v = IsEventDisplayed(pEv)) != 0)
					{
						m_SongScrollPos += v + 1;	//wrong??
						UpdateScrollbarInfo(m_pSong->GetTotalEvents());
						Invalidate();
					}
				}
			}
		}
		else if (Chan == BUTTON_MIDI_CHANNEL)
		{
			switch (Note)
			{
			case LK25_BUTTON_TOP1:			//OnShortmidimsg
				break;
			case LK25_BUTTON_TOP2:		//OnShortmidimsg
				break;
			case LK25_BUTTON_TOP3:		//OnShortmidimsg
				break;
			case LK25_BUTTON_TOP4:		//OnShortmidimsg
				break;
			case LK25_BUTTON_TOP5:		//OnShortmidimsg
				break;
			case LK25_BUTTON_TOP6:		//OnShortmidimsg
				break;
			case LK25_BUTTON_TOP7:		//OnShortmidimsg
				break;
			case LK25_BUTTON_TOP8:		//OnShortmidimsg
				break;
			case LK25_BUTTON_BOT1:		//OnShortmidimsg
				break;
			case LK25_BUTTON_BOT2:		//OnShortmidimsg
				break;
			case LK25_BUTTON_BOT3:		//OnShortmidimsg
				break;
			case LK25_BUTTON_BOT4:		//OnShortmidimsg
				break;
			case LK25_BUTTON_BOT5:		//OnShortmidimsg
				break;
			case LK25_BUTTON_BOT6:		//OnShortmidimsg
				break;
			case LK25_BUTTON_BOT7:		//OnShortmidimsg
				break;
			case LK25_BUTTON_BOT8:		//OnShortmidimsg
				break;
			}
		}
		Invalidate();
		break;
	case (int)CMidi::MidiChannelCmds::NOTEON:		//OnShortmidimsg
		if (Chan == 0)	//keyboard channel
		{
			//--------------------------------
			// What we are going to do here
			// is keep track of the number of
			// notes that go on (or off, if vel == 0,
			// then that is a note off).
			//---------------------------------
			if (Vel == 0)	//Midi Note Off
			{
				if (m_nMidiNotesOn)
				{
//					m_nMidiNotesOn--;
//					note = Note % 12;
//					adj = NoteAdjustLUT[note];
//					note -= adj;
//					note += 12 * (Note / 12);
					CMsEvent* pEv = m_pSong->GetEventObject(m_LastSelectedEventIndex);
					CMsNote* pN = pEv->FindNote(note, adj ? MSFF_ACCIDENTAL_SHARP : MSFF_ACCIDENTAL_INKEY);
					if (pN) 
						MidiPlayNote(pN, 0);// Note Off
					if (m_nMidiNotesOn == 0)
					{
						pEv->SetSelected(0);
						pEv = pEv->GetNext();
						if (pEv)
						{
							m_LastSelectedEventIndex++;
							pEv->SetSelected(1);
						}
						else
							m_LastSelectedEventIndex = -1;
					}
					Invalidate();
				}
			}
			else if ((m_dmDrawMode == DrawMode::DRAW_NOTES_VIA_MIDI) && (m_LastSelectedEventIndex >= 0))
			{		//this is a NOTE on, so Add a note
//				int note, adj;
				note = Note % 12;
				adj = CMsNote::NoteAdjustLUT[note];
				note -= adj;
				note += 12 * (Note / 12);
				CMsEvent* pEv = m_pSong->GetEventObject(m_LastSelectedEventIndex);
				m_nMidiNotesOn++;
				CMsNote* pN = new CMsNote();
				pN->Create(0, GetSong(), GetSong()->GetEventObject(m_nDrawEvent));
				pN->SetDuration((m_nMidiInputNoteSetup & DRAW_NOTE_DURATION));
				pN->SetAccent((m_nMidiInputNoteSetup >> DRAW_NOTE_ACCENT_SHIFT) & 0x01);
				pN->SetAccidental(adj ? MSFF_ACCIDENTAL_SHARP : MSFF_ACCIDENTAL_INKEY);
				pN->SetTrack(((m_nMidiInputNoteSetup >> DRAW_NOTE_TRACK_SHIFT) & 0x0f));
				pN->SetRest((m_nMidiInputNoteSetup >> DRAW_NOTE_REST_SHIFT) & 1);
				pN->SetPitch(note);
				CMsObject* pO;
				if ((pO = pEv->ObjectAlreadyHere(pN)) == NULL)
				{
					MidiPlayNote(pN, 1);// Note On
					pEv->AddObject(pN);

				}
				else
					pEv->RemoveObject(pO);
				CMsObject* pObj;
				pObj = pEv->GetEventMsObjectHead();
				csStatus.Format(_T("%lS"), _T("Draw Note(s) :"));
				int count = 0;
				while (pObj)
				{
					if (CMsObject::MsObjType::NOTE == pObj->GetType())
					{
						CMsNote* pNote = (CMsNote*)pObj;
						if (!pNote->IsRest())
						{
							if (count)
								csStatus.Append(CString(", "));
							pNote->ObjectToString(csTemp);
							csStatus.Append(csTemp);
							++count;
						}
					}
					pObj = pObj->GetNext();
				}//end of while(pObj)
				if (count)
				{
					csTemp.Format(_T(" On Instrument %d"), (m_nMidiInputNoteSetup >> DRAW_NOTE_TRACK_SHIFT) & 0x0f);
					csStatus.Append(csTemp);
					m_Status.SetText(csStatus);
				}
				Invalidate();
			}
		}	//end of if chan==0			//OnShortmidimsg
		else if (Chan == BUTTON_MIDI_CHANNEL)	//button channel NOTEON
		{
			switch (Note)
			{
			case LK25_BUTTON_TOP1:		//OnShortmidimsg
				break;
			case LK25_BUTTON_TOP2:		//OnShortmidimsg
				//toggle checkbox
				m_Combo_Decorations.ToggleItem(COMBO_DECORATION_ACCENT);
				Invalidate();
				UpdateNoteInfo(0);
				break;
			case LK25_BUTTON_TOP3:
				break;
			case LK25_BUTTON_TOP4:
				break;
			case LK25_BUTTON_TOP5:
				break;
			case LK25_BUTTON_TOP6:
				break;
			case LK25_BUTTON_TOP7:
				break;
			case LK25_BUTTON_TOP8:
				break;
			case LK25_BUTTON_BOT1:
				break;
			case LK25_BUTTON_BOT2:
				break;
			case LK25_BUTTON_BOT3:
				break;
			case LK25_BUTTON_BOT4:
				break;
			case LK25_BUTTON_BOT5:
				break;
			case LK25_BUTTON_BOT6:
				break;
			case LK25_BUTTON_BOT7:
				break;
			case LK25_BUTTON_BOT8:
				break;
			}
		}
		break;
	case (int)CMidi::MidiChannelCmds::POLYPRESS:
		break;
	case (int)CMidi::MidiChannelCmds::CTRLCHNG:
		if (Chan == 0)	//Midi channel 1
		{
			switch (Note)	//note is the controller number
			{
			case MIDI_CTRLCHNG_ROUNDUPPER:	//delete notes in event
				if (Vel)
				{
					CMsEvent* pEv;
					CMsObject* pOb;

					pEv = m_pSong->GetEventObject(m_LastSelectedEventIndex);
					if (pEv)
					{
						CMsObject* pObT;
						pOb = pEv->GetEventMsObjectHead();
						while (pOb)
						{
							pObT = pOb->GetNext();
							if (CMsObject::MsObjType::NOTE == pOb->GetType())
								pEv->RemoveObject(pOb);
							pOb = pObT;
						}
						Invalidate();
					}
				}
				break;
			case MIDI_CTRLCHGN_ROUNDLOWER:	//insert event
				if (Vel)
				{
					m_pSong->InsertEvent(m_LastSelectedEventIndex);
					m_pSong->RenumberEvents(NULL, NULL);
					Invalidate();
				}
				break;
			case MIDI_CTRLCHNG_KNOB_INSTRUMENT:
				Instrument = Vel / 9;
				m_Combo_Instrument.SetCurSel(Instrument);
				UpdateNoteInfoFlag = 1;
				break;
			case MIDI_CTRLCHG_DECORATION:
				m_Combo_Decorations.SetCurSel(1 + Vel / 43);
				UpdateNoteInfoFlag = 1;
				break;
			case MIDI_CTRLCHNG_KNOB_NOTE_DUR:
				m_Combo_NoteType.SetCurSel(Vel/21);
				UpdateNoteInfoFlag = 1;
				break;
			case MIDI_CTRLCHNG_KNOB_REST_DUR:
				m_Combo_Rests.SetCurSel(Vel / 21);
				UpdateNoteInfoFlag = 1;
				RestFlag = 1;
				break;
			case MIDI_CTRLCHNG_ACCIDENTAL:
				m_Combo_Accidentals.SetCurSel(Vel / 42);
				UpdateNoteInfoFlag = 1;
				break;
			case MIDI_CTRLCHNG_KEY:
				m_Combo_KeySig.SetCurSel(Vel / 8);
				UpdateNoteInfoFlag = 1;
				break;
			case MIDI_CTRLCHNG_REWIND:	//move selection back
				if ((m_LastSelectedEventIndex >= 0) && (Vel > 0))
				{
					CMsEvent* pEV = m_pSong->GetEventObject(m_LastSelectedEventIndex);
					pEV->SetSelected(0);
					pEV = pEV->GetPrev();
					m_LastSelectedEventIndex--;
					if (pEV)
					{
						pEV->SetSelected(1);
						m_SongScrollPos += IsEventDisplayed(pEV);
					}
					UpdateScrollbarInfo(m_pSong->GetTotalEvents());
					Invalidate();
				}
				break;
			case MIDI_CTRLCHNG_FASTFORWARD:	//move selection forward
				if ((m_LastSelectedEventIndex >= 0) && (Vel > 0))
				{
					CMsEvent* pEV = m_pSong->GetEventObject(m_LastSelectedEventIndex);
					pEV->SetSelected(0);
					pEV = pEV->GetNext();
					if (pEV)
					{
						pEV->SetSelected(1);
						m_LastSelectedEventIndex++;
					}
					else
					{
						pEV = m_pSong->MakeNewEvent();
						pEV->SetSelected(1);
						m_pSong->AddEventAtEnd(pEV);
						m_LastSelectedEventIndex++;
						m_pSong->RenumberEvents(&m_FirstSelectedEvent, &m_LastSelectedEventIndex);
					}
					if ((v = IsEventDisplayed(pEV)) != 0)
					{
						if (v < 4)
							m_SongScrollPos += 4;
						else
							m_SongScrollPos += v;
					}
					UpdateScrollbarInfo(m_pSong->GetTotalEvents());
					Invalidate();
				}
				break;
			case MIDI_CTRLCHNG_STOP:
				if (Vel > 0) OnButtonStop();
				break;
			case MIDI_CTRLCHNG_PLAY:
				if((Vel > 0) && !m_pSong->IsPlaying())
					OnButtonPlay();
				break;
			case MIDI_CTRLCHNG_PAUSE:
				break;
			case MIDI_CTRLCHNG_RECORD:
				break;
			}//end of switch(Note) Note is the controller number
			if (UpdateNoteInfoFlag)	//do we need to update note draw mode?
			{
				UpdateNoteInfo(RestFlag);
			}
		}	//end of if(chan == 0)
		break;
	case (int)CMidi::MidiChannelCmds::CHNLPRESS:
		break;
	case (int)CMidi::MidiChannelCmds::PITCHBEND:
		break;
	case (int)CMidi::MidiChannelCmds::PGMCHANGE:
		break;
	case (int)CMidi::MidiRealTimeMsgs::CLOCK:
		break;
	case (int)CMidi::MidiRealTimeMsgs::START:
		break;
	case (int)CMidi::MidiRealTimeMsgs::STOP:
		break;
	case (int)CMidi::MidiRealTimeMsgs::CONTINUE:
		break;
	}
	return 0;
}

afx_msg LRESULT CChildViewStaff::OnStaffDispEvent(WPARAM Event, LPARAM cmd)
{
	switch (cmd)
	{
	case STAFF_DISP_EVENT_NEXT:	//OnStaffDispEvent
	{
		CMsEvent* pEv = m_pSong->GetEventObject(m_nLastSongPosition);
		if (pEv)pEv->SetSelected(0);
		pEv = m_pSong->GetEventObject(Event);
		if (pEv)pEv->SetSelected(1);
		//----------------------------------
		// we need to figure out where we are
		// and where we want to go.
		//----------------------------------
		unsigned CurrentStartPos = (unsigned)m_SongScrollPos;
		int DispEvents = m_MaxEvents;
		if (Event < CurrentStartPos)
			m_SongScrollPos = Event;
		else if (Event >= (CurrentStartPos + DispEvents))
			m_SongScrollPos = Event;
		UpdateScrollbarInfo(m_pSong->GetTotalEvents());
		m_nLastSongPosition = Event;
	}
	break;
	case STAFF_DISP_EVENT_END://OnStaffDispEvent
	{
		CMsEvent* pEv = m_pSong->GetEventObject(m_nLastSongPosition);
		if (pEv)pEv->SetSelected(0);
		m_nLastSongPosition = 0;
		m_SongScrollPos = 0;
		UpdateScrollbarInfo(m_pSong->GetTotalEvents());
	}
	break;
	}
	Invalidate();
	return 0;
}

void CChildViewStaff::UpdateNoteInfo(int RestFlag)
{
	CMsNote* pN;

	if ((m_dmDrawMode != DrawMode::NOTE) && (m_dmDrawMode != DrawMode::DRAW_NOTES_VIA_MIDI))
	{
		if (GetDrawObject())
		{
			delete m_pDrawObject;
			SetDrawObject(0);
		}
		m_dmDrawMode = DrawMode::NOTE;
	}
	if (GetDrawObject() == 0) SetDrawObject(new CMsNote);
	pN = (CMsNote *)GetDrawObject();
	pN->Create(0, GetSong(), GetSong()->GetEventObject(m_nDrawEvent));
	UpdateComboBoxes();
	pN->GetData().CopyData(GetNoteData());
	//WTF?
	int Dur = m_nMidiInputNoteSetup & DRAW_NOTE_DURATION;	//this line does nothing
	//-----------------------------
	// Get the duration by first
	// Oh, this is just too
	// convoluted....
	//-----------------------------
	Dur = CMsNote::NoteDurLut[(int)CMsNote::GetDurationTable()[(int)pN->GetDuration()].NoteShapeIndex];
	pN->SetDuration(Dur);
	pN->SetRest(RestFlag);
}

int CChildViewStaff::IsEventDisplayed(CMsEvent* pEV)
{
	int rV = 0;
	int Event = pEV->GetIndex();
	if (Event > (m_MaxEvents + m_SongScrollPos - 1))
		rV = Event - (m_MaxEvents + m_SongScrollPos - 1);
	else if (Event < m_SongScrollPos)
		rV = -1;
	return rV;
}

BOOL CChildViewStaff::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	BOOL rV;

	rV = CChildViewBase::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	EnableScrollBar(SB_HORZ, ESB_ENABLE_BOTH);
	ShowScrollBar(SB_HORZ);
	return rV;
}

void CChildViewStaff::OnInitialUpdate()
{
	CRect clientRect;
	int ControlX = 0;
	int i = 0;
	int n = 0;
	int y = 0;
	int x = 0;
	CSize itemSize;
	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	int MaxYSize = 0;
	CMsEvent* pEV = 0;

	//---------------------------------
	m_TimerID = SetTimer(1000, 60000,NULL);
	GetClientRect(&clientRect);
	x1 = clientRect.left;
	x2 = clientRect.right;
	y1 = clientRect.top;
	y2 = clientRect.bottom;
	//---------------------------------
	// If the number of events that
	// calculated is 19, events are
	// numbered 0 -> 18, so the max 
	// event index will be 
	// NumberOfEvents - 1
	//---------------------------------
	m_MaxEvents = CalcMaxEvents() - 1;
	//---------------------------------
	// Create default song
	//---------------------------------
	m_pSong = new CMsSong;
	CSize szTrackIconSize = CSize(28, 28);
	m_pSong->Create(this, szTrackIconSize);
	//------------------------------
	// Add Initial Events to song
	//------------------------------
	for(i=0;i< m_MaxEvents;++i)
	{
		pEV = new CMsEvent;
		pEV->Create(GetSong(), this);
		pEV->SetEventIndex(i);
		m_pSong->AddEventAtEnd(pEV);
	}
	//------------ Event 0 ------------------------------
	CMsTempo* pTM = new CMsTempo();
	pEV = m_pSong->GetEventObject(0);
	pTM->Create(GetSong(), pEV,100);
	pEV->AddObject(pTM);
	CMsTimeSignature* pTS = new CMsTimeSignature();
	pTS->Create(GetSong(), pEV, COMBO_TIMESIG_4_4);
	pEV->AddObject(pTS);
	//-------------- Event 1 -----------------------
	CMsKeySignature* pKS = new CMsKeySignature;
	pEV = m_pSong->GetEventObject(1);
	pKS->Create(GetSong(), pEV, CMsKeySignature::KeySigID::CMAJ);
	pEV->AddObject(pKS);
	CMsLoudness* pLD = new CMsLoudness();
	pLD->Create(GetSong(), pEV,100);
	pEV->AddObject(pLD);
	//----------------------------------------------------
	UpdateScrollbarInfo(m_pSong->GetTotalEvents(),"Init");
	//----------------------------------------
	// Initialize Combo Boxes
	// Instrument Selection
	//---------------------------------------
	n = GetSong()->GetNumberOfTracks();
	itemSize = 28;
	m_Combo_Instrument.Create(
		5,	//number of instruments to display
		15,	//total number of instruments
		CSize(28, 28),	//size of items in combobox
		CPoint(ControlX, 0),	//upper left corner
		CSize(16, 28),	//size of drop arrow
		this,			//parent window
		IDC_COMBO_INSTRUMENT	// control ID
	);
	for (i = 1; i < (n+1); ++i)
	{
		m_Combo_Instrument.AddBitmap(
			GetTrackInfo(i)->GetBitmap());

	}
	ControlX += m_Combo_Instrument.GetTotalWidth();
	//---------------------------------------
	// Initailize keysignature combo box
	//----------------------------------------
	n = GETAPP->GetNumKeySigs();
	itemSize = GETAPP->bmGetCBKeySignature(1)->GetBmDim();
	itemSize += CSize(8, 8);
	m_Combo_KeySig.Create(
		5,
		n,
		itemSize,
		CPoint(ControlX, 0),
		CSize(16,itemSize.cy),
		this,
		IDC_COMBO_KEYSIG
	);
	for (i = 0; i < n; ++i)
		m_Combo_KeySig.AddBitmap(GETAPP->bmGetCBKeySignature(i + 1));
	ControlX += m_Combo_KeySig.GetTotalWidth();
	//----------------------------------------
	// Initialize time signature combo box
	//-----------------------------------------
	n = GETAPP->GetNumTimeSig();
	itemSize = GETAPP->bmGetCbTimeSig(COMBO_TIMESIG_2_2)->GetBmDim();
	itemSize += CSize(8, 4);
	m_Combo_TimeSig.Create(
		4,
		n,
		itemSize,
		CPoint(ControlX,0),
		CSize(20,itemSize.cy),
		this,
		IDC_COMBO_TIMESIG
	);
	for(i=0;i<n;++i)
		m_Combo_TimeSig.AddBitmap(GETAPP->bmGetCbTimeSig(i));
	ControlX += m_Combo_TimeSig.GetTotalWidth();
	//------------------------------------------
	// Initialize note duration combox box
	//------------------------------------------
	n = GETAPP->GetNumNoteTypes ();
	itemSize = GETAPP->bmGetNoteType(0)->GetBmDim();
	itemSize += CSize(4, 4);
	m_Combo_NoteType.Create(
		n - 2, // Number of items to display
		n, // Total Items
		itemSize,	//size of items in combobox
		CPoint(ControlX, 0),	//uper left corner
		CSize(16, itemSize.cy),	//size of drop arrow
		this,				//parent window
		IDC_COMBO_NOTETYPES	// control ID
	);
	for (i = 0; i < n; ++i)
		m_Combo_NoteType.AddBitmap(GETAPP->bmGetNoteType(i));
	ControlX += m_Combo_NoteType.GetTotalWidth();

	//---------------------------------------
	// Initialize rest duration combo box
	//---------------------------------------
	n = GETAPP->GetNumRestTypes();
	itemSize = GETAPP->bmGetCBRestTypes(0)->GetBmDim();
	itemSize += CSize(8, 4);
	m_Combo_Rests.Create(
		4,
		n,
		itemSize,
		CPoint(ControlX,0),
		CSize(16,itemSize.cy),
		this,
		IDC_COMBO_RESTTYPES
	);
	for(i=0;i<n;++i)
		m_Combo_Rests.AddBitmap(GETAPP->bmGetCBRestTypes(i));
	ControlX += m_Combo_Rests.GetTotalWidth();
	//-------------------------------------------
	// Initialize decorations combo box
	//--------------------------------------------
	n = GETAPP->GetNumDecorations();
	itemSize = GetBmDimensions(CMidiSeqMSApp::GetDecorationsBmCbIdsSel(COMBO_DECORATION_ACCENT));
	itemSize += CSize(8, 8);
	m_Combo_Decorations.Create(
		n,
		n,
		itemSize,
		CPoint(ControlX, 0),
		CSize(18,itemSize.cy),
		this,
		IDC_COMBO_DECORATIONS
	);
	for (i = 0; i < n; ++i)
	{
		m_Combo_Decorations.AddNotSelBitmapID(CMidiSeqMSApp::GetDecorationsBmCbIdsNotSel(i));
		m_Combo_Decorations.AddSelBitmapID(CMidiSeqMSApp::GetDecorationsBmCbIdsSel(i));
		m_Combo_Decorations.SetItemFlags(i, CBDecorationFlags[i]);
	}
	ControlX += m_Combo_Decorations.GetTotalWidth();
	//--------------------------------------
	// Initialize accidental combo box
	//---------------------------------------
	n = GETAPP->GetNumAccidentalTypes();
	itemSize = GetBmDimensions(CMidiSeqMSApp::GetAccidentalBmCBIdsTypes(COMBO_ACCIDENTAL_INKEY));
	itemSize += CSize(8, 4);
	m_Combo_Accidentals.Create(
		n,
		n,
		itemSize,
		CPoint(ControlX, 0),
		CSize(16, itemSize.cy),
		this,
		IDC_COMBO_ACCIDENTALS
	);
	for(i=0;i<n;++i)
		m_Combo_Accidentals.AddBitmap(GETAPP->bmGetCBAccidentalType(i));
	ControlX += m_Combo_Accidentals.GetTotalWidth();
	//-----------------------------------------
	// Initialize Block Operations combo box
	//------------------------------------------
	n = GETAPP->GetNumBlockOps();
	itemSize = GETAPP->bmGetBlockOpType(0)->GetBmDim();
	itemSize += CSize(4, 4);
	m_Combo_BlockOps.Create(
		5,
		n,
		itemSize,
		CPoint(ControlX, 0),
		CSize(16,itemSize.cy),
		this,
		IDC_COMBO_BLOCKOPS
	);
	for(i=0;i<n;++i)
		m_Combo_BlockOps.AddBitmap(GETAPP->bmGetBlockOpType(i));
	//------------------ Misc Stuff ---------------------
	itemSize = GETAPP->bmGetMiscType(0)->GetBmDim();
	itemSize += CSize(4, 4);
	n = GETAPP->GetNumMisc();

	m_Combo_Misc.Create(
		4,		//Number of Items to display
		n,		//Total Items
		itemSize,	//size of one item
		CPoint(				//low left corner of combo box
			clientRect.Width() - itemSize.cx - DROPBOX_ARROW_WIDTH, 
			clientRect.bottom - STATUS_BAR_HEIGHT
		),
		CSize(
			DROPBOX_ARROW_WIDTH, 
			itemSize.cy
		),		// Size of drop arrow
		this,	//	parrent
		IDC_COMBO_MISCSTUFF			// ID
	);
	//------------------ Add bitmaps to misc combo box ---------------------
	for (i = 0; i < n; ++i)
	{
		m_Combo_Misc.AddBitmap(GETAPP->bmGetMiscType(i));
	}

//----------------------------------------------
	y = clientRect.bottom - STATUS_BAR_HEIGHT;
	x = 0;
	m_Status.Create(
		CRect(CPoint(x, y),CSize(clientRect.Width(), STATUS_BAR_HEIGHT)),
		this,
		IDC_STATIC_STATUSDISP);
	m_Status.ShowWindow(true);
	//----- Create Regions ---------
	CPoint ptRectUL;
	CSize szRect;
	CRect rectRgn;

	y = CLIENT_TO_TOP_UPPER_SELECT_RECT;
	//-------------- Upper Selection Bar -----
	ptRectUL = CPoint(EVENT_WIDTH, y);
	szRect = CSize(szRect.cx, SELECTION_BAR_HEIGHT);
	m_UpperSelRect = CRect(ptRectUL, szRect);
	PrintRec("ChildView UpperRgn", m_UpperSelRect);
	m_UpperSelRect.NormalizeRect();
	m_rgnUpperSelect.CreateRectRgn(m_UpperSelRect);
	//-----------Upper Draw Region -------------
	y += UPPER_SELECTION_BAR_HEIGHT;
	ptRectUL = CPoint(EVENT_WIDTH, y);
	szRect = CSize(clientRect.Width() - EVENT_WIDTH, UPPER_DRAW_RECT_HEIGHT);
	m_rectUpperDraw = CRect(ptRectUL, szRect);
	PrintRec("ChildView UpperDrawRgn", m_rectUpperDraw);
	m_rectUpperDraw.NormalizeRect();
	m_rgnUpperDraw.CreateRectRgn(m_rectUpperDraw);
	//-------------- Edit Region -----
	y += UPPER_DRAW_RECT_HEIGHT;
	ptRectUL = CPoint(EVENT_WIDTH, y);
	szRect = CSize(clientRect.Width() - EVENT_WIDTH, EDIT_RECT_HEIGHT);
	m_rectEdit = CRect(ptRectUL, szRect);
	PrintRec("ChildView EditRgn", m_rectEdit);
	m_rectEdit.NormalizeRect();
	m_rgnEdit.CreateRectRgn(m_rectEdit);
	//------------- Lower Draw Region -------------
	y += EVENT_HEIGHT;
	ptRectUL = CPoint(EVENT_WIDTH, y);
	szRect = CSize(clientRect.Width() - EVENT_WIDTH, LOWER_DRAW_RECT_HEIGHT);
	m_rectLowerDraw = CRect(ptRectUL, szRect);
	PrintRec("ChildView LowerDrawRgn", m_rectLowerDraw);
	m_rectLowerDraw.NormalizeRect();
	m_rgnLowerDraw.CreateRectRgn(m_rectLowerDraw);
	//-------------- Lower Selection Bar -----
	y += LOWER_DRAW_RECT_HEIGHT;
	ptRectUL = CPoint(EVENT_WIDTH, y);
	szRect = CSize(szRect.cx, LOWER_SELECTION_BAR_HEIGHT);
	m_LowerSelRect = CRect(ptRectUL, szRect);
	m_LowerSelRect.NormalizeRect();
	m_rgnLowerSelect.CreateRectRgn(m_LowerSelRect);
	//----------------------------------
	m_SelectRectTop = m_UpperSelRect.bottom;
	m_szSelectRect = CSize(EVENT_WIDTH, m_LowerSelRect.top - m_UpperSelRect.bottom);

	//-------------- Button Controls ---------
	itemSize = m_Button_Play.GetButtonSize(IDB_BUTTON_PLAY_UP);
	m_Button_Play.Create(
		CPoint(0, clientRect.bottom - itemSize.cy - STATUS_BAR_HEIGHT),
		this,
		IDC_BUTTON_PLAY,
		IDB_BUTTON_PLAY_UP,
		IDB_BUTTON_PLAY_DOWN,
		NULL,
		MYBUTTS_LATCHED
	);
	m_Button_Play.EnableWindow(1);

	itemSize = m_Button_Stop.GetButtonSize(IDB_BUTTON_STOP_UP);
	m_Button_Stop.Create(
		CPoint(96, clientRect.bottom - itemSize.cy - STATUS_BAR_HEIGHT),
		this,
		IDC_BUTTON_STOP,
		IDB_BUTTON_STOP_UP,
		IDB_BUTTON_STOP_DOWN,
		&m_Button_Play
	);
	m_Button_Stop.EnableWindow(0);

	itemSize = m_Button_Pause.GetButtonSize(IDB_BUTTON_PAUSE_UP);
	m_Button_Pause.Create(
		CPoint(48, clientRect.bottom - itemSize.cy - STATUS_BAR_HEIGHT),
		this,
		IDC_BUTTON_PAUSE,
		IDB_BUTTON_PAUSE_UP,
		IDB_BUTTON_PAUSE_DOWN,
		&m_Button_Pause
	);
	m_Button_Pause.EnableWindow(0);

	//----------------------------------
	// Set combo boxes to their default
	// Selectoins
	//----------------------------------
	m_Combo_Instrument.SetCurSel((int)GetTrack());

	m_Combo_BlockOps.SetCurSel(COMBO_BLOCK_COPY);

	m_Combo_Misc.SetCurSel(COMBO_MISC_MEASUREBAR);

	pKS = (CMsKeySignature*)GetSong()->GetObjectTypeInEvent(CMsObject::MsObjType::KEYSIG, (int)EventObjectSignatureTypes::EVENT_LOUDNESS_KEYSIG);
	if(pKS)
		m_Combo_KeySig.SetCurSel((int)pKS->GetKeySignature() - 1);

	pTS = (CMsTimeSignature*)GetSong()->GetObjectTypeInEvent(CMsObject::MsObjType::TIMESIG,  (int)EventObjectSignatureTypes::EVENT_TEMPO_TIMESIG);
	if(pTS)
		m_Combo_TimeSig.SetCurSel((int)pTS->GetTimeSignature() - 1, false);

	m_Combo_Decorations.SetCurSel(COMBO_DECORATION_NONE);

	m_Combo_Accidentals.SetCurSel(COMBO_ACCIDENTAL_INKEY);
	SetAccidental(MSFF_ACCIDENTAL_INKEY);

	m_Combo_Rests.SetCurSel(COMBO_REST_QUARTER, false);

	m_Combo_NoteType.SetCurSel(COMBO_NOTE_QUARTER);
	m_ReadyToDraw = 1;
//	GETMIDI->SetMessageDestWind(this);
}

void CChildViewStaff::OnDraw(CDC* pDC)
{
	//----------------------------------------
	// OnDraw
	//		This is the function that is called
	// when the scre
	// en needs to be redrawn.
	// To minimize screen flicker, all drawing
	// is done to a Memory DC, and then it is
	// blt to the screen.
	//
	//	parameters:
	//		pDC....pointer to the device context
	//----------------------------------------
	CDC DCm;	//memory device context
	CDC DCmEdit;	//memory device context for edit region
	CMyBitmap bmEdit, *pOldBMEdit;	// bitmap for edit region memory context
	CMyBitmap bm,*pOldBM;
	CRect rectClient;
	CBrush brushBackGround, brushUpperSelBar, brushLowerSelBar, brushLastEventBKG;

	if (m_ReadyToDraw)
	{
		GetClientRect(&rectClient);
		//create a compattable memory device context
		DCm.CreateCompatibleDC(pDC);
		DCmEdit.CreateCompatibleDC(pDC);
		// Create a bitmap for the memory DC
		bm.CreateCompatibleBitmap(
			pDC, 
			rectClient.Width(), 
			rectClient.Height()
		);
		bmEdit.CreateCompatibleBitmap(
			pDC, 
			(EVENT_WIDTH + 1) * m_MaxEvents,
			EVENT_HEIGHT
		);
		pOldBM = (CMyBitmap*)DCm.SelectObject(&bm);
		pOldBMEdit = (CMyBitmap*)DCmEdit.SelectObject(&bmEdit);
		// Create brushes
		brushBackGround.CreateSolidBrush(RGB(255,255,255));
		//brushBackGround.CreateSolidBrush(GetColorPalette()->color_BackGround);
		brushLastEventBKG.CreateSolidBrush(GetColorPalette()->color_LastEventBKG);
		brushUpperSelBar.CreateSolidBrush(GetColorPalette()->color_UpperSelBar);
		brushLowerSelBar.CreateSolidBrush(GetColorPalette()->color_LowerSelBar);
		// Fill in background rectangles
		DCm.FillRect(&rectClient, &brushBackGround);
		//DCm.FillRect(&m_LowerSelRect, &brushLowerSelBar);
		//DCm.FillRect(&m_UpperSelRect, &brushUpperSelBar);
		//CRect rectLastEvent;
		//GetEventRect(m_MaxEvents, rectLastEvent);
		//DCm.FillRect(&rectLastEvent, &brushLastEventBKG);
		// Draw the staff and all objects
		if (m_pSong)
		{
			if (m_EscapeFlag)
			{
				m_EscapeFlag = 0;
			}
			m_pSong->Draw(&DCmEdit, m_SongScrollPos, m_MaxEvents, &rectClient);
		}
		// Draw Special Objects
		// Draw Tie if needed
		if (m_dmDrawMode == DrawMode::TIE && m_nDrawState == DRAWSTATE::TIE_SECONDNOTE)
		{
			CRect r;
			r.SetRect(m_TieEndPoint + CSize(0, -16), m_TieStartPoint + CSize(0, 16));
			r.NormalizeRect();
			DCm.Arc(r, m_TieStartPoint - CSize(1, 0), m_TieEndPoint - CSize(1, 0));
		}
		// Draw the view controls
		DCm.BitBlt(
			0, 
			CLIENT_TO_TOP_UPPER_SELECT_RECT,
			EVENT_WIDTH * m_MaxEvents, 
			EVENT_HEIGHT, 
			&DCmEdit, 
			0, 
			0, 
			SRCCOPY
		);
		DrawControls(&DCm);
		pDC->BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &DCm, 0, 0, SRCCOPY);
		DCm.SelectObject(pOldBM);
	}
}

BOOL CChildViewStaff::OnEraseBkgnd(CDC* pDC)
{
	return true;
}

void CChildViewStaff::DrawControls(CDC* pDC)
{
	m_Combo_Misc.OnDraw(pDC);
	m_Combo_NoteType.OnDraw(pDC);
	m_Combo_Instrument.OnDraw(pDC);
	m_Combo_BlockOps.OnDraw(pDC);
	m_Combo_Rests.OnDraw(pDC);
	m_Combo_TimeSig.OnDraw(pDC);
	m_Combo_Decorations.OnDraw(pDC);
	m_Combo_Accidentals.OnDraw(pDC);
	m_Combo_KeySig.OnDraw(pDC);
	m_Status.OnDraw(pDC);
	m_Button_Play.OnDraw(pDC);
	m_Button_Pause.OnDraw(pDC);
	m_Button_Stop.OnDraw(pDC);
}

// This function is used to update the combo box selection variables.
void CChildViewStaff::UpdateComboBoxes()
{
	SetAccidental(CMsNote::AccedentalsLUT[m_Combo_Accidentals.GetCurSel()]);
	int sel;
	SetAccent(m_Combo_Decorations.GetItemValue(COMBO_DECOR_ACCENT));
	sel = m_Combo_Decorations.GetCurSel();
	switch (sel)
	{
	case COMBO_DECOR_NONE:		// UpdateComboBoxes
		SetDotted(0);
		SetTriplet(0);
		break;
	case COMBO_DECOR_DOT:		// UpdateComboBoxes
		SetDotted(1);
		SetTriplet(0);
		break;
	case COMBO_DECOR_TRIPLET:		// UpdateComboBoxes
		SetDotted(0);
		SetTriplet(1);
		break;
	}
	SetTrack(m_Combo_Instrument.GetCurSel() );		//Instrument number needs +1
	INT Track = GetTrack();
	int Patch = GetTrackInfo(Track)->GetPatch();
	int DeviceID = GetTrackInfo(Track)->GetMidiOutDeviceID();
	int Channel = GetTrackInfo(Track)->GetChannel();
	GETAPP->GetMidiOutTable()->GetDevice(DeviceID).PgmChange(Channel, Patch);	// Change patch
	SetNoteDuration(CMsNote::NoteDurLut[m_Combo_NoteType.GetCurSel()]);
}

LRESULT CChildViewStaff::MyControlsMessages(WPARAM ComboID, LPARAM nSelection)
{
	//----------------------------------------
	// MyControlsMessages
	//		This function is called when  a
	// view child object posts a message to
	// the parent window.
	//	The appropriate action is taken based
	// on the control ID and selection and
	// dispatches a call to the methodes that
	// handle the specific control.
	//----------------------------------------
	DrawMode DrawingMode = DrawMode::NOP;
	int v = 0;
	CBToggle ToggleMSG;
	int op;
	int Selection = 0;
	CMsNote* pNote;
	int Track = 0, Patch = 0, Chan = 0;
	CMsEvent* pEv = 0;
	CMsObject* pDrawObj = 0;

	switch (ComboID)
	{
	case IDC_COMBO_MISCSTUFF:		// MyControlsMessages
		Selection = m_Combo_Misc.GetCurSel();
		switch (Selection)
		{
		case COMBO_MISC_GLISSANDO:	//glissando
			DrawingMode = DrawMode::GLISSANDO;
			break;
		case COMBO_MISC_ENDBAR:
			DrawingMode = DrawMode::ENDBAR;
			break;
		case COMBO_MISC_MEASUREBAR:	//measure bar
			DrawingMode = DrawMode::BAR;
			break;
		case COMBO_MISC_NOTETIE:	//note tie
			DrawingMode = DrawMode::TIE;
			break;
		case COMBO_MISC_LOUDNESS:	//Loudness (Note Velocity)
			DrawingMode = DrawMode::LOUDNESS;
			{
				CParamDlg Dlg;
				Dlg.SetCaption(CString("Set Loudness"));
				Dlg.SetMin(1);
				Dlg.SetMax(127);
				Dlg.SetValue(100);
				if (IDOK == Dlg.DoModal())
				{
					v = Dlg.GetValue();
				}
				else
					return 0;
			}
			break;
		case COMBO_MISC_TEMPO:	//Song Tempo
			DrawingMode = DrawMode::TEMPO;
			{
				CParamDlg Dlg;
				Dlg.SetCaption(CString("Set Tempo"));
				Dlg.SetMin(56);
				Dlg.SetMax(200);
				Dlg.SetValue(160);
				if (IDOK == Dlg.DoModal())
				{
					v = Dlg.GetValue();
				}
				else
					return 0;
			}
			break;
		case COMBO_MISC_INST_CHANGE:	//change instrument
			DrawingMode = DrawMode::INSTCHANGE;
			break;
		}	// End of MISC switch (Selection)
		SetupDrawMode(DrawingMode, v);
		break;
	case IDC_COMBO_NOTETYPES:		// MyControlsMessages
		v = CMsNote::NoteDurLut[nSelection];
		if (GetTriplet()) 
			v -= 2;
		else if (GetDotted()) 
			v += 2;
		SetNoteDuration(v);
		//-----------------------------
		// Show decorations and
		// accidentals combo boxes
		// Do these actually work
		//-----------------------------
		m_Combo_Decorations.ShowWindow(SW_SHOW);
		m_Combo_Accidentals.ShowWindow(SW_SHOW);
		m_Combo_Decorations.EnableWindow(1);
		m_Combo_Accidentals.EnableWindow(1);
		//-----------------------------
		// This is a note, not a rest
		//-----------------------------
		SetupDrawMode(DrawMode::NOTE);
		break;
	case IDC_COMBO_RESTTYPES:		// MyControlsMessages
		m_Combo_Decorations.ClearAllItems(COMBO_DECORATION_NONE);
		m_Combo_Accidentals.SetCurSel(COMBO_ACCIDENTAL_INKEY);
		m_Combo_Accidentals.EnableWindow(0);
		m_Combo_Decorations.EnableWindow(0);
		m_Combo_Accidentals.ShowWindow(SW_HIDE);
		m_Combo_Decorations.ShowWindow(SW_HIDE);
		SetNoteDuration(CMsNote::NoteDurLut[nSelection]);
		SetFocus();
		SetRest(1);
		if (GetDrawObject())
		{
			delete m_pDrawObject;
			SetDrawObject(0);
		}
		pNote = new CMsNote;
		pNote->Create((COMBO_REST_HALF < nSelection) ? CMidiSeqMSApp::GetRestBmIdsTypes()[nSelection] : 0, GetSong(), GetSong()->GetEventObject(m_nDrawEvent));
		SetDrawObject(pNote);
		m_dmDrawMode = DrawMode::NOTE;
		UpdateNoteDrawObject();
		break;
	case IDC_COMBO_INSTRUMENT:		// MyControlsMessages
		Track = m_Combo_Instrument.GetCurSel() + 1;
		Patch = GetTrackInfo(Track)->GetPatch();
		Chan = GetTrackInfo(Track)->GetChannel();
		SetTrack(Track);		//Instrument number needs +1
		GetSong()->ChangePatch(Track, Chan, Patch);
		SetFocus();
		UpdateNoteDrawObject();
		break;
	case IDC_COMBO_BLOCKOPS:		// MyControlsMessages
		op = m_Combo_BlockOps.GetCurSel();
		DoBlockOps(op);
		break;
	case IDC_COMBO_TIMESIG:		// MyControlsMessages
		SetFocus();
		SetupDrawMode(DrawMode::TIMESIG, m_Combo_TimeSig.GetCurSel() + 1);
		break;
	case IDC_COMBO_KEYSIG:		// MyControlsMessages
		SetFocus();
		SetupDrawMode(DrawMode::KEYSIG, m_Combo_KeySig.GetCurSel() + 1);
		break;
	case IDC_BUTTON_PLAY:		// MyControlsMessages
		OnButtonPlay();
		break;
	case IDC_BUTTON_STOP:		// MyControlsMessages
		OnButtonStop();
		break;
	case IDC_COMBO_ACCIDENTALS:	// MyControlsMessages
		SetAccidental(CMsNote::AccedentalsLUT[nSelection] );
		UpdateNoteDrawObject();
		break;
	case IDC_COMBO_DECORATIONS:	// MyControlsMessages
		ToggleMSG.Wparam = nSelection;
		switch (ToggleMSG.command)
		{
		case CBT_SETVALUE:
			switch (ToggleMSG.index)
			{
			case COMBO_DECORATION_ACCENT:
				SetAccent(ToggleMSG.value);
				break;
			case COMBO_DECORATION_NONE:
				if (ToggleMSG.value)
				{
					SetTriplet(0);
					SetDotted(0);
					SetNoteDuration(CMsNote::NoteDurLut[(int)CMsNote::GetDurationTable()[GetNoteDuration()].NoteShapeIndex]);
				}
				break;
			case COMBO_DECORATION_DOT:
				if (ToggleMSG.value)
				{
					SetDotted(ToggleMSG.value);
					SetTriplet(0);
				}
				else
					SetDotted(ToggleMSG.value);
				SetNoteDuration(CMsNote::NoteDurLut[(int)CMsNote::GetDurationTable()[GetNoteDuration()].NoteShapeIndex] + 2);
				break;
			case COMBO_DECORATION_TRIPLET:
				if (ToggleMSG.value)
				{
					SetTriplet(ToggleMSG.value);
					SetDotted(0);
				}
				else
					SetTriplet(ToggleMSG.value);
				SetDotted(0);

				SetNoteDuration(CMsNote::NoteDurLut[(int)CMsNote::GetDurationTable()[GetNoteDuration()].NoteShapeIndex] - 2);
				break;
			}	//end of switch (ToggleMSG.index)
			break;
		case CBT_INDEX_ONLY:
			break;
		} //end if switch (ToggleMSG.command)
		SetFocus();
		SetupDrawMode(DrawMode::NOTE);
		break;
	default:		// MyControlsMessages
		break;
	}	// end of switch (ComboID)
	return 0;
}

//---------------- Combo Boxes ---------------

void CChildViewStaff::CheckAndDoScroll(CPoint point)
{
	if (GetRawEventNumber(point.x) == m_MaxEvents)
	{
		m_SongScrollPos += 4;
		for (int i = 0; i < 4; ++i)
			GetSong()->AddEventAtEnd(GetSong()->MakeNewEvent());
		GetSong()->RenumberEvents(NULL, NULL);
		SetScrollRange(GetSong()->GetTotalEvents() - GetMaxEvents());
		SetScrollPos(GetSong()->GetTotalEvents() - GetMaxEvents());
	}
}

void CChildViewStaff::DoBlockOps(int Op)
{
	DrawMode mode = DrawMode::NOP;
	int v = 0;

	SetFocus();
	switch (Op)
	{
	case COMBO_BLOCK_COPY:	//block copy
		mode = DrawMode::COPY;
		break;
	case COMBO_BLOCK_MOVE:	//block move
		mode = DrawMode::MOVE;
		break;
	case COMBO_BLOCK_CHANGEINSTRUMENT:	//change instrument
	{
		int InstFrom, InstTo;
		{
			CSelectorDlg Dlg;
			int nTracks = GetSong()->GetNumberOfTracks();
			CMyBitmap** ppBmps = new CMyBitmap * [nTracks];
			for (int i = 0; i < nTracks; ++i)
			{
				ppBmps[i] = GetSong()->GetSongInfo()->GetTrack(i + 1)->GetBitmap();
			}
			Dlg.SetBitmaps(ppBmps);
			Dlg.SetNumberOfBitmaps(nTracks);
			Dlg.SetCaption(CString("From Instrument FROM"));
			Dlg.SetSelection(0);
			//					CSelInstDlg Dlg;
			//					Dlg.m_pPromtString = "FROM"
			if (IDOK == Dlg.DoModal())
			{
				InstFrom = Dlg.GetSelection() + 1;
				Dlg.SetCaption(CString("To Instrument TO"));
				Dlg.SetSelection(InstFrom);
				if (IDOK == Dlg.DoModal())
				{
					InstTo = Dlg.GetSelection() + 1;
					v = (InstFrom & 0xf) | (InstTo << 4);
					mode = DrawMode::INSTCHANGE;
				}
				else
					mode = DrawMode::NOP;
			}
			else
				mode = DrawMode::NOP;
			if (ppBmps)
			{
				for (int i = 0; i < nTracks; ++i)
				{
					if (ppBmps[i])
					{
						delete ppBmps[i];
						ppBmps[i] = 0;
					}
				}
				delete[] ppBmps;
				ppBmps = 0;
			}
		}
	}
	break;
	case COMBO_BLOCK_CHANGEDUR:	//change duration
	{
		int DurFrom, DurTo;
		{
			CSelDurDlg Dlg;
			Dlg.m_nDuration = 12;
			Dlg.m_pPrompt = "FROM";
			Dlg.DoModal();
			DurFrom = Dlg.m_nDuration;
			mode = DrawMode::CHANGEDUR;
		}
		{
			CSelDurDlg Dlg;
			Dlg.m_pPrompt = "TO";
			Dlg.m_nDuration = 12;
			Dlg.DoModal();
			DurTo = Dlg.m_nDuration;
			v = (DurFrom & 0x1f) | (DurTo << 5);
			mode = DrawMode::CHANGEDUR;
		}
	}
	break;
	case COMBO_BLOCK_INCREASE:	//Increase duration
		mode = DrawMode::INCREASEDUR;
		break;
	case COMBO_BLOCK_DECREASE:	//Decrease duration
		mode = DrawMode::DECREASEDUR;
		break;
	case COMBO_BLOCKUP:	//increase pitch up
		mode = DrawMode::INCRPITCH;
		break;
	case COMBO_BLOCKDOWN:	//increase pitch down
		mode = DrawMode::DECRPITCH;
		break;
	case COMBO_BLOCK_REPEAT:	//add repeat
	{
		CParamDlg Dlg;
		Dlg.SetCaption(CString("Repeat Count"));
		Dlg.SetMin(1);
		Dlg.SetMax(255);
		Dlg.SetValue(2);
		Dlg.DoModal();
		v = Dlg.GetValue();
		mode = DrawMode::REPEAT_START;
	}
	break;
	case COMBO_BLOCK_INSERT:	//insert blok
		mode = DrawMode::INSERTBLOCK;
		break;
	case COMBO_BLOCK_MIDIINPUT:	//Insert notes via midi
//		mode = DRAW_DRAWNOTESVIAMIDI;
//		SetNoteDur(CMsNote::NoteDurLut[m_Combo_NoteType.GetCurSel()]);
//		SetNoteRest(0);
//		v = CreateNote();
		break;
	}
	SetupDrawMode(mode, v);
}


void CChildViewStaff::OnButtonPlay()
{
	if (GETAPP->GetMidiOutTable()->GetNumDevices())
	{
		m_Button_Play.EnableWindow(0);
		m_Button_Stop.EnableWindow(1);
		if (m_pSong->Play(this))
			m_pSong->Start();
	}
	else
		MessageBox(_T("Open up a Midi Port"), _T("ERROR"));
}


void CChildViewStaff::OnButtonStop()
{
	m_pSong->Stop();
}

afx_msg LRESULT CChildViewStaff::OnMidiEdit(WPARAM SubFunction, LPARAM SubSubFunction)
{
	//----------------------------------------
	// OnMidiEdit
	//	Recieves message from a midi device
	// That allows for editing the score
	//
	// parameters:
	//	Subfunction.....The edit command
	//	SubSubFunction..Other data
	//-----------------------------------------
	switch (SubFunction)
	{
	case MIDIEDIT_NOTE:
		m_Combo_NoteType.SetCurSel(SubSubFunction);
//		PostMessageW(WM_MY_CONTROL_MESSAGES,IDC_COMBO_NOTETYPES, SubSubFunction);
		break;
	case MIDIEDIT_REST:
		m_Combo_Rests.SetCurSel(SubSubFunction);
//		PostMessageW(WM_MY_CONTROL_MESSAGES, IDC_COMBO_RESTTYPES, SubSubFunction);
		break;
	case MIDIEDIT_DECORATIONS:
		m_Combo_Decorations.ToggleItem(SubSubFunction);
		m_Combo_Decorations.SetCurSel(SubSubFunction);
//		PostMessageW(WM_MY_CONTROL_MESSAGES, IDC_COMBO_DECORATIONS, SubSubFunction);
		break;
	case MIDIEDIT_DECORATIONS_ACCENT:
		m_Combo_Decorations.ToggleItem(COMBO_DECORATION_ACCENT);
		m_Combo_Decorations.SetCurSel(COMBO_DECORATION_ACCENT);
//		PostMessageW(WM_MY_CONTROL_MESSAGES, IDC_COMBO_DECORATIONS, SubSubFunction);
		break;
	case MIDIEDIT_ACCIDENTALS:
		m_Combo_Accidentals.SetCurSel(SubSubFunction);
		break;
	case MIDIEDIT_INSTRUMENT:
		m_Combo_Instrument.SetCurSel(SubSubFunction);
		break;
	case MIDIEDIT_TIMESIGNATURE:
		m_Combo_TimeSig.SetCurSel(SubSubFunction);
		break;
	case MIDIEDIT_KEYSIGNATURE:
		m_Combo_KeySig.SetCurSel(SubSubFunction);
		break;
	case MIDIEDIT_MISC:
		m_Combo_Misc.SetCurSel(SubSubFunction);
		break;
	}
	return 0;
}

void CChildViewStaff::UpdateNoteDrawObject()
{
	if (GetDrawObject())
	{
		if (GetDrawObject()->GetType() == CMsObject::MsObjType::NOTE)
		{
			CMsNote* pNote = (CMsNote*)GetDrawObject();
			pNote->GetData().CopyData(GetNoteData());
		}
	}
}

void CChildViewStaff::UpdateColors()
{
	int n = GetSong()->GetNumberOfTracks();
	m_Combo_Instrument.Reset();
	for (int i = 0; i < n; ++i)
	{
		m_Combo_Instrument.AddBitmap(GetTrackInfo(i+1)->GetBitmap());
	}
}

afx_msg LRESULT CChildViewStaff::OnChildviewPlayerthread(WPARAM SubCommand, LPARAM Data)
{

	switch (SubCommand)
	{
	case CHILD_VIEW_PLAYERTHREAD_SONGDONE:
		//--------------------------------
		// This is where we end up if the
		// song that is playing stops at
		// the end on its own.
		//
		// Call the methode in the button
		// that sort of acts like it was
		// pushed, but doesn't send a
		// notification that the button
		// was pressed.
		//-------------------------------
		m_Button_Stop.LikeA_ButtonPush();
		//----------------------------------
		// Disable stop button,
		// Enable Play Button
		//----------------------------------
		m_Button_Stop.EnableWindow(0);
		m_Button_Play.EnableWindow(1);
		//---------------------------------
		// delete the event queue
		//---------------------------------
		GETAPP->PlayerThreadDeleteSong(GetSong());
		GetSong()->GetDelSongCompleteEV().Pend();
		break;
	}
	return 0;
}

void CChildViewStaff::OnTimer(UINT_PTR nIDEvent)
{
	CChildViewBase::OnTimer(nIDEvent);
}

void CChildViewStaff::MidiPlayNote(CMsNote* pNote, UINT NoteOnFlag)
{
	CMsObject* pKeySig, * pLoudness;
	UINT DrawEvent = GetDrawEvent();
	if (DrawEvent < 1)
		DrawEvent = 2;
	CMsEvent* pEV = GetSong()->GetEventObject(DrawEvent);
	pKeySig = GetSong()->GetMsObject(
		CMsObject::MsObjType::KEYSIG,
		pEV,
		SEARCH_REVERSE
	);
	pLoudness = GetSong()->GetMsObject(
		CMsObject::MsObjType::LOUDNESS,
		pEV,
		SEARCH_REVERSE
	);
	if (pKeySig && pLoudness)
	{
		if (NoteOnFlag)
		{
			pNote->NoteOn(100);
		}
		else
		{
			pNote->NoteOff(100);
		}
	}
}

BOOL CChildViewStaff::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_SYSKEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_MENU:
			OnKeyDown(pMsg->wParam, 1, 0);
			return true;
			break;
		}
		break;
	case WM_SYSKEYUP:
		switch (pMsg->wParam)
		{
		case VK_MENU:
			OnKeyUp(pMsg->wParam, 1, 0);
			return true;
			break;
		}
		break;
	}
	return CChildViewBase::PreTranslateMessage(pMsg);
}

void CChildViewStaff::OnMenuMsFileOpen()
{
	CFileDialog Dlg(TRUE, _T("sng"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Midi Sequence Files (*.sng)|*.sng|All Files (*.*)|*.*||"));
	if (IDOK == Dlg.DoModal())
	{
		CString FileName = Dlg.GetPathName();
		GetSong()->Open(FileName);
		Invalidate();
	}
}

void CChildViewStaff::OnUpdateMenuMsFileOpen(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CChildViewStaff::OnUpdateMenuMsFileSave(CCmdUI* pCmdUI)
{
}

void CChildViewStaff::OnMenuMsFileSaveAs()
{
}

void CChildViewStaff::OnUpdateMenuMsFileSaveAs(CCmdUI* pCmdUI)
{
}

MouseRegionTransitionState CChildViewStaff::RegionTransition(CPoint ptMousePos)
{
	MouseRegionTransitionState NextState = MouseRegionTransitionState::MOUSE_TRANSITION_NONE;
	MouseRegions Region = MouseInRegion(ptMousePos);

	//--------------------------------
	// RegionTransition
	// Generates the state transition
	// when the mouse is moved.
	//--------------------------------
	// m_MouseRegion is the previous
	// region the mouse was in.
	//--------------------------------
	// Region is the current region
	// the mouse is in.
	//--------------------------------

	switch(m_MouseRegion)
	{
	case MouseRegions::MOUSE_OUTSIDE:
		switch (Region)
		{
		case MouseRegions::MOUSE_OUTSIDE:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_NONE;
			break;
		case MouseRegions::MOUSE_IN_UPPERSEL:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_UPPERSEL;
			break;
		case MouseRegions::MOUSE_IN_LOWERSEL:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_LOWERSEL;
			break;
		case MouseRegions::MOUSE_IN_EDITREG:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_EDIT;
			break;
		case MouseRegions::MOUSE_IN_UPPERDRAW:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_UPPER_DRAW;
			break;
		case MouseRegions::MOUSE_IN_LOWERDRAW:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_LOWER_DRAW;
			break;
		}
		break;
		//--------------------------------
	case MouseRegions::MOUSE_IN_UPPERSEL:
		switch (Region)
		{
		case MouseRegions::MOUSE_OUTSIDE:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_UPPERSEL_TO_OUTSIDE;
			break;
		case MouseRegions::MOUSE_IN_UPPERSEL:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_NONE;
			break;
		case MouseRegions::MOUSE_IN_LOWERSEL:
			if (LogFile()) fprintf(LogFile(), "Transitioning from UpperSel to LowerSel Not Possible\n");
			break;
		case MouseRegions::MOUSE_IN_EDITREG:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT;
			break;
		case MouseRegions::MOUSE_IN_UPPERDRAW:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_UPPERSEL_TO_UPPER_DRAW;
			break;
		case MouseRegions::MOUSE_IN_LOWERDRAW:
			if (LogFile()) fprintf(LogFile(), "Transitioning from UpperSel to LowerdRAW Not Possible\n");
			break;
		}
		break;
		//--------------------------------
	case MouseRegions::MOUSE_IN_LOWERSEL:
		switch (Region)
		{
		case MouseRegions::MOUSE_OUTSIDE:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_LOWERSEL_TO_OUTSIDE;
			break;
		case MouseRegions::MOUSE_IN_UPPERSEL:
			if (LogFile()) fprintf(LogFile(), "Transitioning from LowerSel to UpperSel Not Possible\n");
			break;
		case MouseRegions::MOUSE_IN_LOWERSEL:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_NONE;
			break;
		case MouseRegions::MOUSE_IN_EDITREG:
			if (LogFile()) fprintf(LogFile(), "Transitioning from LowerSel to EDIT Not Possible\n");
			break;
		case MouseRegions::MOUSE_IN_UPPERDRAW:
			if (LogFile()) fprintf(LogFile(), "Transitioning from LowerSel to UPPER DRAW Not Possible\n");
			break;
		case MouseRegions::MOUSE_IN_LOWERDRAW:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_LOWERSEL_TO_LOWER_DRAW;
			break;
		}
		break;
		//--------------------------------
	case MouseRegions::MOUSE_IN_EDITREG:
		switch (Region)
		{
		case MouseRegions::MOUSE_OUTSIDE:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE;
			break;
		case MouseRegions::MOUSE_IN_UPPERSEL:
			if (LogFile()) fprintf(LogFile(), "Transitioning from Edit to Upper Sel Not Possible\n");
			break;
		case MouseRegions::MOUSE_IN_LOWERSEL:
			if (LogFile()) fprintf(LogFile(), "Transitioning from Edit to Lower Sel Not Possible\n");
			break;
		case MouseRegions::MOUSE_IN_EDITREG:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_NONE;
			break;
		case MouseRegions::MOUSE_IN_UPPERDRAW:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW;
			break;
		case MouseRegions::MOUSE_IN_LOWERDRAW:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW;
			break;
		}
		break;
		//--------------------------------
	case MouseRegions::MOUSE_IN_UPPERDRAW:
		switch (Region)
		{
		case MouseRegions::MOUSE_OUTSIDE:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_OUTSIDE;
			break;
		case MouseRegions::MOUSE_IN_UPPERSEL:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_UPPER_SEL;
			break;
		case MouseRegions::MOUSE_IN_LOWERSEL:
			if (LogFile()) fprintf(LogFile(), "Transitioning from Upper Draw to Lower Sel Not Possible\n");
			break;
		case MouseRegions::MOUSE_IN_EDITREG:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT;
			break;
		case MouseRegions::MOUSE_IN_UPPERDRAW:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_NONE;
			break;
		case MouseRegions::MOUSE_IN_LOWERDRAW:
			if (LogFile()) fprintf(LogFile(), "Transitioning from Upper Draw to Lower Draw Not Possible\n");
			break;
		}
		break;
		//--------------------------------
	case MouseRegions::MOUSE_IN_LOWERDRAW:
		switch (Region)
		{
		case MouseRegions::MOUSE_OUTSIDE:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_OUTSIDE;
			break;
		case MouseRegions::MOUSE_IN_UPPERSEL:
			if (LogFile()) fprintf(LogFile(), "Transitioning from Lower Draw to Upper Sel Not Possible\n");
			break;
		case MouseRegions::MOUSE_IN_LOWERSEL:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_LOWER_SEL;
			break;
		case MouseRegions::MOUSE_IN_EDITREG:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_EDIT;
			break;
		case MouseRegions::MOUSE_IN_UPPERDRAW:
			if (LogFile()) fprintf(LogFile(), "Transitioning from Lower Draw to Upper Draw Not Possible\n");
			break;
		case MouseRegions::MOUSE_IN_LOWERDRAW:
			NextState = MouseRegionTransitionState::MOUSE_TRANSITION_NONE;
			break;
		}
		break;
		//--------------------------------
	default:
		break;
	}
	return NextState;
}

const char* CChildViewStaff::GetMouseRegionName(MouseRegions region)
{
	const char* pName = nullptr;	

	for (int i = 0; RegionLUT[i].Region != MouseRegions::MOUSE_NONE; ++i)
	{
		if (RegionLUT[i].Region == region)
		{
			pName = RegionLUT[i].name;
			break;
		}
	}
	return pName;
}

const char* CChildViewStaff::GetMouseRegionTransitionName(MouseRegionTransitionState transition)
{
	const char* pName = nullptr;
	int i = 0;

	for (i = 0; RegionTransitionLUT[i].Transition != MouseRegionTransitionState::MOUSE_TRANSITION_ERROR; ++i)
	{
		if (RegionTransitionLUT[i].Transition == transition)
		{
			pName = RegionTransitionLUT[i].pName;
			break;
		}
	}
	if(RegionTransitionLUT[i].Transition == MouseRegionTransitionState::MOUSE_TRANSITION_ERROR)
		pName = RegionTransitionLUT[i].pName;
	return pName;
}

const char* CChildViewStaff::GetStaffMouseStateName(StaffMouseStates state)
{
	const char* pName = nullptr;
	int i = 0;

	for (i = 0; StaffMouseStateLUT[i].State != StaffMouseStates::END_STATE; ++i)
	{
		if (StaffMouseStateLUT[i].State == state)
		{
			pName = StaffMouseStateLUT[i].pName;
			break;
		}
	}
	if (StaffMouseStateLUT[i].State == StaffMouseStates::END_STATE)
		pName = StaffMouseStateLUT[i].pName;
	return pName;
}

const char* CChildViewStaff::GetDrawStateName(DRAWSTATE state)
{
	int i = 0;
	const char* pName = nullptr;
	bool Found = false;
	for (i = 0; DrawStateLUT[i].m_State != DRAWSTATE(-1) && !Found; ++i)
	{
		if (DrawStateLUT[i].m_State == state)
		{
			pName = DrawStateLUT[i].m_pName;
			Found = true;
		}
	}
    return pName;
}

const char* CChildViewStaff::GetDrawModeName(DrawMode mode)
{
	const char* pName = nullptr;
	bool Found = false;

	for (int i = 0; i < sizeof(DrawModeLUT) / sizeof(DrawModeItem) && !Found; ++i)
	{
		if (DrawModeLUT[i].m_Mode == mode)
		{
			pName = DrawModeLUT[i].m_pName;
			Found = true;
		}
	}
	return pName;
}

void CChildViewStaff::OnMenuMsFileSave()
{
}

/*

void CChildViewStaff::OnMouseMove(UINT nFlags, CPoint pointMouse)
{
	CMsEvent* pEV = 0;
	CMsNote* pN;
	CMsObject* pObj;
	MouseRegions Region;
	MouseRegionTransitionState TransitionState;
	int NoteLocation = 0;	// location of note on the stave
	CString StatusString,csTemp;
	bool Loop = false;
	int NoteActuallyPlayed = 0;
	int NoteIndex = 0;

	m_nDrawEvent = XtoEventIndex(pointMouse.x);
	m_MouseRegionTransitionState = RegionTransition(pointMouse);
	Region = MouseInRegion(pointMouse);
	TransitionState = RegionTransition(pointMouse);
	m_MouseRegionTransitionState = TransitionState;
	m_MouseRegion = Region;
	if (LogFile()) fprintf(LogFile(), "Mouse Move: Region %s Transition: %s\n",
		GetMouseRegionName(Region),
		GetMouseRegionTransitionName(TransitionState)
	);
	switch (Region)
	{
	case MouseRegions::MOUSE_IN_UPPERSEL:	//OnMouseMove
	case MouseRegions::MOUSE_IN_LOWERSEL:
		//----------------------------------
		// Update the state as to where the
		// mouse happens to be.  If the mouse
		// has gone from the edit area to
		// the sel area, then make a note
		// of that as well.
		//-----------------------------------

		//------------------------------------
		switch (m_dmDrawMode)
		{
		case DrawMode::NOTE:		//OnMouseMove
			pN = (CMsNote*)GetDrawObject();
			//if (pN)
			//{
			//	pN->NoteOff(0);	//NoteOff
			//	pEV = pN->GetParentEvent();
			//	pEV->RemoveObject(pN);
			//	delete GetDrawObject();
			//	SetLastNote(nullptr);
			//	GetDrawObject() = 0;
			//}
			if (GetLastNote() && TransitionState == MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_OUTSIDE)
			{
				GetLastNote()->NoteOff(0);	//NoteOff
				SetLastNote(nullptr);
			}
			break;
		case DrawMode::BAR:
		case DrawMode::ENDBAR:
		case DrawMode::GLISSANDO:
		case DrawMode::KEYSIG:
		case DrawMode::LOUDNESS:
		case DrawMode::REPEAT:
		case DrawMode::REST:
			break;
		}	//end of switch(m_dmDrawMode) in case MOUSE_IN_UPPERSEL
		Invalidate();
		break;
	case MouseRegions::MOUSE_OUTSIDE:	///On Mouse Move
		switch (m_dmDrawMode)	//OnMouseMove
		{
		case DrawMode::NOTE:
			//if (GetLastNote() && m_ExitEditRegion)
			//{
			//	GetLastNote()->NoteOff(0);	//NoteOff
			//	SetLastNote(nullptr);
			//	Invalidate();
			//}
			break;
		case DrawMode::BAR:
		case DrawMode::ENDBAR:
		case DrawMode::GLISSANDO:
		case DrawMode::KEYSIG:
		case DrawMode::LOUDNESS:
		case DrawMode::REPEAT:
		case DrawMode::REST:
			break;
		}	//end of switch(m_dmDrawMode) in case MOUSE_OUTSIDE
		break;
	case MouseRegions::MOUSE_IN_EDITREG:	//On Mouse Move In Edit Region
		//-----------------------------
		// change the mouse Y coordinate
		// into a note location on the
		// stave, ie C, D, E, F, etc
		// The note location is NOT the
		// actual pitch.  The actual
		// pitch is determined by the
		// Key Signature or an accidental
		// if one is present.
		//-----------------------------
		NoteLocation = YtoNote(pointMouse.y);
		switch(m_dmDrawMode)
		{
		case DrawMode::NOP:		// OnMouseMove  Move In Edit Region
			pEV = m_pSong->GetEventObject(m_nDrawEvent);
			if (pEV)
			{
				pObj = pEV->GetEventMsObjectHead();
				if (m_nMouseState == StaffViewMouseState::STAFFVIEW_MOUSEDOWN)
				{
					CMsObject* pSelectedObjects = NULL;
					Loop = true;

					if (m_DragFlag)
					{

					}
					else
					{
						//-------------------------------------
						// Determin if the mouse happens to be
						// over one of the selected objects
						//--------------------------------------
						pSelectedObjects = GetSelectedObjectHead();
						while (pSelectedObjects && Loop)
						{
							if (pSelectedObjects->MouseOverObject(pointMouse))
							{
								Loop = 0;
								m_DragFlag = 1;
							}
							else
							{
								pSelectedObjects = pSelectedObjects->GetNextSelectedObject();
							}
						}

					}
					//------------------------
					// Do Drags and such
					//------------------------
					pSelectedObjects = GetSelectedObjectHead();
				}
				else // Mouse button is up	//OnMouseMove  Move In Edit Region
				{
					//--------------------------
					// highlight objects and
					// other nonsense
					//---------------------------
					Loop = true;
					while (pObj && Loop)
					{
						if (pObj->HighLight(true, pointMouse))
						{
							Loop = false;
						}
						else
						{
							pObj = pObj->GetNext();
						}
					}
					if (pObj == NULL)
					{
//						fprintf(stderr, "No Object Found, Clear Highlight\n");
						if (m_pHighLightedObject)
							m_pHighLightedObject->SetHighLight(false);
						m_pHighLightedObject = NULL;
					}
					else
					{
						m_pHighLightedObject = pObj;
					}
				}
				Invalidate();
			}
			break;
		case DrawMode::NOTE:		//OnMouseMove Move In Edit Region
			//-------------------------------
			// Move a note around the screen
			//--------------------------------
			pN = (CMsNote*)GetDrawObject();
			if (pN)
			{

				if (!m_nLastSongPosition)
				{
					pEV = m_pSong->GetEventObject(m_nDrawEvent);
					if(pEV)
					{
						pEV	->AddObject(pN);
						m_nLastSongPosition = m_nDrawEvent;
						fprintf(GETAPP->LogFile(),"Added note in order at event %d\n", m_nDrawEvent);
					}
				}
				else if (m_nLastSongPosition != m_nDrawEvent)
				{
					pEV = m_pSong->GetEventObject(m_nLastSongPosition);
					if (pEV)
					{
;						pEV->RemoveObject(pN);
						pEV = m_pSong->GetEventObject(m_nDrawEvent);
						pEV->AddObject(pN);
						m_nLastSongPosition = m_nDrawEvent;
					}
					else
					{
						if (LogFile()) fprintf(GETAPP->LogFile(),"t\t\t ? ? ? ? No Event at Last Song Position % d\n", m_nLastSongPosition);
					}
				}
				else
				{
//					m_nLastSongPosition = m_nDrawEvent;
					if (LogFile()) fprintf(LogFile(), "Same Event Last:%d Current:%d\n", m_nLastSongPosition, m_nDrawEvent);
				}
				pN->MouseMove(m_nDrawState, pointMouse);
			}
			break;
		case DrawMode::GLISSANDO:		//On Mouse Move Move In Edit Region
			switch (m_nDrawState)
			{
			case DRAWSTATE::WAITFORMOUSE_DOWN:
				GetDrawObject()->MouseMove(m_nDrawState, pointMouse);
				break;
			case DRAWSTATE::GLISSANDO_FIRST_NOTE:
				GetDrawObject()->MouseMove(m_nDrawState, pointMouse);
				break;
			case DRAWSTATE::GLISSANDO_SECOND_NOTE:
//				m_GlissEndPoint = CPoint(pointMouse.x, m_GlissStartPoint.y);
				GetDrawObject()->MouseMove(m_nDrawState, pointMouse);
				Invalidate();
				break;
			default:
				if (LogFile()) fprintf(LogFile(), "Unknown Glissando State\n");
				break;
			}	//end of switch(m_nDrawState)
			break;
		case DrawMode::ENDBAR:		//On Mouse Move Move In Edit Region
			if (CMsObject::MsObjType::ENDBAR == GetDrawObject()->GetType())
			{
				StatusString.Format(_T("Draw Song End Bar At Event %d"), m_nDrawEvent);
				Invalidate();
			}
			break;
		case DrawMode::BAR:		//On Mouse Move Move In Edit Region
			if (CMsObject::MsObjType::BAR == GetDrawObject()->GetType())
			{
				StatusString.Format(_T("Draw Measure Bar At Event %d"), m_nDrawEvent);
				Invalidate();
			}
			break;
		case DrawMode::KEYSIG:		//On Mouse Move Move In Edit Region
			GetDrawObject()->MouseMove(m_nDrawState, pointMouse);
			break;
		case DrawMode::TIMESIG:		//On Mouse Move Move In Edit Region
			StatusString.Format(_T("Draw Time Signature at event %d"), m_nDrawEvent);
			Invalidate();
			break;
		case DrawMode::TEMPO:		//On Mouse Move Move In Edit Region
			StatusString.Format(_T("Draw Tempo Change at event %d"), m_nDrawEvent);
			Invalidate();
			break;
		case DrawMode::TIE:		//On Mouse Move Move In Edit Region
			switch (m_nDrawState)
			{
			case DRAWSTATE::TIE_FIRSTNOTE:
				StatusString.Format(_T("Select First Note at Event %d"), m_nDrawEvent);
				break;
			case DRAWSTATE::TIE_SECONDNOTE:
				m_TieEndPoint = CPoint(pointMouse.x, m_TieStartPoint.y);
				StatusString.Format(_T("Select Second Note at Event %d"), m_nDrawEvent);
				Invalidate();
				break;
			}	//end of switch(m_nDrawState)
			break;
		case DrawMode::LOUDNESS:		//On Mouse Move Move In Edit Region
			StatusString.Format(_T("Change Loudness at event %d"), m_nDrawEvent);
			Invalidate();
			break;
		case DrawMode::MOVE:		//On Mouse Move Move In Edit Region
			StatusString.Format(_T("Move Block To Event %d"), m_nDrawEvent);
			m_Status.SetText(StatusString);
			break;
		}
		break;
	}
//	if (StatusString.GetLength())m_Status.SetText(StatusString);
	CWnd::OnMouseMove(nFlags, pointMouse);
}
*/
void CChildViewStaff::OnSettingsTracksettinigs()
{
	CDlgMidiInfo Dlg;

	if (GetSong())
	{
		if (GetSong()->GetSongInfo())
		{
			Dlg.SetSongInfo(GetSong()->GetSongInfo());
			Dlg.DoModal();
		}
	}
}

void CChildViewStaff::OnUpdateSettingsTracksettinigs(CCmdUI* pCmdUI)
{
	
}
