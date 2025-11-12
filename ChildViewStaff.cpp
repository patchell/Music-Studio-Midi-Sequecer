//------------------------------------
// ChildViewStaff.cpp : implementation of the CChildViewStaff class
//--------------------------------------

///


#include "pch.h"

// CChildViewStaff

int CBDecorationFlags[APP_NUM_DECORATIONS] = {
	COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_CHECKED,
	COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_EXCLUSIVE | COMBOTOGGLE_TOGGLE_CHECKED,
	COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_EXCLUSIVE | COMBOTOGGLE_TOGGLE_CHECKED,
	COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_EXCLUSIVE | COMBOTOGGLE_TOGGLE_CHECKED
};

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
	m_nDrawState = 0;			// draw state machine
	m_nDrawMode = 0;			// Drawing mode ie notes, etc
	m_nDrawEvent = 0;			// Current event mouse is over
	m_pSong = 0;				// pointer to song
	m_MaxEvents = 0;			// maximum events displayed
	m_pDrawObject = 0;			// current objedt being drawn
	pLastNote = 0;				// pointer to the last note placed
	m_MouseInEditRegion = 0;	// flag indicating pointer in edit region
	m_ExitEditRegion = 0;		// flag indicates that edit region exited
	m_LastPitch = INVALID_PITCH;	//last pitch is invalid
	m_LastSelectedEventIndex = -1;
	m_FirstSelectedEvent = -1;
	m_SongScrollPos = 0;
	m_nMidiNotesOn = 0;
	m_color_BackGround = RGB(172, 162, 172);
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
		pObjFound = pEvent->GetEventObjectHead();
	else
		pObjFound = 0;
	Loop = 1;
	while (pObjFound && Loop)
	{
		if (pObjFound && pObjFound->Select(TRUE, MousePointer))
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

void CChildViewStaff::OnLButtonDown(UINT nFlags, CPoint pointMouse)
{
	int Event = XtoEventIndex(pointMouse.x);
	CMsObject* pObject;
	int Note = YtoNote(pointMouse.y);
	int Region;
	UINT Loop = 1;

	if (this->GetFocus() != this)
		SetFocus();

	m_nMouseState = StaffViewMouseState::STAFFVIEW_MOUSEDOWN;
	Region = MouseInRegion(pointMouse);
	switch (Region)
	{
	case MOUSE_OUTSIDE:
		break;
	case MOUSE_INUPPERSEL:
	case MOUSE_INLOWERSEL:
		break;
	case MOUSE_INEDITREG:
		switch (m_nDrawMode)
		{
		case DRAWMODE_NOP:
			pObject = MatchMouseToSelectedObject(pointMouse);
			if (pObject == nullptr)
				pObject = MatchMouseToObjectInEvent(Event, pointMouse);
			break;
		}
		Invalidate();
		break;
	}
	CWnd::OnLButtonDown(nFlags, pointMouse);
}

void CChildViewStaff::OnLButtonUp(UINT nFlags, CPoint pointMouseLButtUp)
{
	int Region;
	CMsEvent* pEv;
	CMsNote* pNote;

	m_nMouseState = StaffViewMouseState:: STAFFVIEW_MOUSEUP;
	m_nDrawEvent = XtoEventIndex(pointMouseLButtUp.x);
	Region = MouseInRegion(pointMouseLButtUp);
	switch (Region)
	{
	case MOUSE_OUTSIDE:		//OnLButtonUp
		break;
	case MOUSE_INUPPERSEL:		//OnLButtonUp
	case MOUSE_INLOWERSEL:
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
			else//select events		//OnLButtonUp
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
	case MOUSE_INEDITREG:		//OnLButtonUp
		switch (m_nDrawMode)
		{
		case DRAWMODE_NOP:		//OnLButtonUp
			break;
		case DRAWMODE_NOTE:		//OnLButtonUp
			pNote = (CMsNote*)m_pDrawObject;
//			pNote->Print(stdout);
			GetSong()->AddObjectToSong(m_nDrawEvent, m_pDrawObject);
			{
				CMsNote* pN = new CMsNote;
				SetPitch(pNote->GetPitch());
				if (GetRest())
					pN->Create(CMidiSeqMSApp::RestBmIdsTypes[pNote->GetShape()], GetSong(), m_nDrawEvent);	// Create Rest
				else
					pN->Create(0, GetSong(), m_nDrawEvent);	// Create Note
				pNote->SetParentEvent(m_nDrawEvent);
				//-----------------------------
				// Copy attributes
				//------------------------------
				pN->GetData().CopyData(GetNoteData());
				m_pDrawObject = pN;
				CheckAndDoScroll(pointMouseLButtUp);
			}
			Invalidate();
			break;
		case DRAWMODE_ENDBAR:
			m_pSong->AddObjectToSong(m_nDrawEvent, m_pDrawObject);
			{
				CMsEndBar* pEBar = new CMsEndBar;
				CMsEndBar* pOldEndBar = (CMsEndBar*)m_pDrawObject;
				pEBar->Create(m_pSong, m_nDrawEvent);
				m_pDrawObject = pEBar;
			}
			CheckAndDoScroll(pointMouseLButtUp);
			Invalidate();
			break;
		case DRAWMODE_BAR:		//OnLButtonUp
			m_pSong->AddObjectToSong(m_nDrawEvent, m_pDrawObject);
			{
				CMsBar* pBarTemp = new CMsBar;
				pBarTemp->Create(GetSong(), m_nDrawEvent);
				pBarTemp->Copy(m_pDrawObject);
				m_pDrawObject = pBarTemp;
			}
			CheckAndDoScroll(pointMouseLButtUp);
			Invalidate();
			break;
		case DRAWMODE_KKEYSIG:		//OnLButtonUp
			m_pSong->AddObjectToSong(m_nDrawEvent, m_pDrawObject);
			{
				CMsKeySignature* pKS = new CMsKeySignature;
				CMsKeySignature* pOld = (CMsKeySignature*)m_pDrawObject;
				pKS->Create(GetSong(), GetDrawEvent(), pOld->GetKeySignature());
				m_pDrawObject = pKS;
			}
			CheckAndDoScroll(pointMouseLButtUp);
			Invalidate();
			break;
		case DRAWMODE_TIMESIG:		//OnLButtonUp
			m_pSong->AddObjectToSong(m_nDrawEvent, m_pDrawObject);
			{
				CMsTimeSignature* pTS = new CMsTimeSignature;
				CMsTimeSignature* pOld = (CMsTimeSignature*)m_pDrawObject;
				pTS->SetTimeSignature(pOld->GetTimeSignature());
				m_pDrawObject = pTS;
			}
			CheckAndDoScroll(pointMouseLButtUp);
			Invalidate();
			break;
		case DRAWMODE_TIE:		//OnLButtonUp
			///-----------------------------------
			/// This is where we change the state of
			/// drawing a tie.  When the mouse releases
			/// we need to check to see if there really
			/// is a note under the mouse pointer.
			///  If there is, record the note and the
			/// points.
			///------------------------------------------
			switch (m_nDrawState)
			{
			case (int)DrawState::DRAWSTATE_TIE_FIRSTNOTE:		//OnLButtonUp
			{
				int note = YtoNote(pointMouseLButtUp.y);
				CMsNote* pN = m_pSong->CheckForNotePresence(m_nDrawEvent, note);
				if (pN)
				{
					m_pFirstTieNote = pN;
					m_nDrawState = DrawState::DRAWSTATE_TIE_SECONDNOTE;
					m_TieStartPoint = pointMouseLButtUp + CSize(0, 4);
					CString s,csTemp;
					s = CString("First Note ");
					pN->ObjectToString(csTemp);
					s.Append(csTemp);
					csTemp.Format(_T(" Selected at Event %d"), m_nDrawEvent) ;
					s.Append(csTemp);
					m_Status.SetText(s);
				}
			}
			break;
			case (int)DrawState::DRAWSTATE_TIE_SECONDNOTE:		//OnLButtonUp
			{
				int note = YtoNote(pointMouseLButtUp.y);
				CMsNote* pN = m_pSong->CheckForNotePresence(m_nDrawEvent, note);
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
					m_nDrawState = DRAWSTATE_TIE_FIRSTNOTE;
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
			}
			break;
			}
			break;
		case DRAWMODE_LOUDNESS:		//OnLButtonUp
		case DRAWMODE_TEMPO:
			//--------------------------------------
			// we only need to place these items once
			//--------------------------------------
			m_pSong->AddObjectToSong(m_nDrawEvent, m_pDrawObject);
			m_pDrawObject = 0;
			m_nDrawMode = DRAWMODE_NOP;
			Invalidate();
			break;
		case DRAWMODE_COPY:		//OnLButtonUp
			CopyBlock(m_nDrawEvent);
			m_pDrawObject = 0;
			m_nDrawMode = DRAWMODE_NOP;
			Invalidate();
			break;
		case DRAWMODE_MOVE:		//OnLButtonUp
			MoveBlock(m_nDrawEvent);
			m_pDrawObject = 0;
			m_nDrawMode = DRAWMODE_NOP;
			Invalidate();
			break;
		}//end of switch(m_nDrawMode)
		break;
	}	//end of switch(Region)
	CWnd::OnLButtonUp(nFlags, pointMouseLButtUp);
}

void CChildViewStaff::OnMouseMove(UINT nFlags, CPoint pointMouse)
{
	CMsEvent* pEV = 0;
	CMsNote* pN;
	CMsObject* pObj;
	int Region;
	int note;
	CString StatusString,csTemp;
	BOOL Loop = FALSE;

	m_nDrawEvent = XtoEventIndex(pointMouse.x);
	Region = MouseInRegion(pointMouse);
	switch (Region)
	{
	case MOUSE_INUPPERSEL:	///OnMouseMove
	case MOUSE_INLOWERSEL:
		//----------------------------------
		// Update the state as to where the
		// mouse happens to be.  If the mouse
		// has gone from the edit area to
		// the sel area, then make a note
		// of that as well.
		//-----------------------------------
		if (m_MouseInEditRegion)
			m_ExitEditRegion = 1;
		else
			m_ExitEditRegion = 0;
		m_MouseInEditRegion = 0;
		//------------------------------------
		switch (m_nDrawMode)
		{
		case DRAWMODE_NOTE:		//OnMouseMove
			if (GetLastPitch() && m_ExitEditRegion)
			{
				pN = (CMsNote*)m_pDrawObject;
				if(pN)
					MidiPlayNote(pN, 0);// Note Off
				SetLastPitch(INVALID_PITCH);
			}
			break;
		}	//end of switch(m_nDrawMode) in case MOUSE_INUPPERSEL
		Invalidate();
		break;
	case MOUSE_OUTSIDE:	///On Mouse Move
		if (m_MouseInEditRegion)
			m_ExitEditRegion = 1;
		else
			m_ExitEditRegion = 0;
		m_MouseInEditRegion = 0;
		switch (m_nDrawMode)	//OnMouseMove
		{
		case DRAWMODE_NOTE:
			if (LastPitchIsValid() && m_ExitEditRegion)
			{
				pN = (CMsNote*)m_pDrawObject;
				if (pN)
				{
					MidiPlayNote(pN, 0);	//NoteOff
				}
				SetLastPitch(-1);
				SetLastPitch(INVALID_PITCH);
				Invalidate();
			}
			break;
		}	//end of switch(m_nDrawMode) in case MOUSE_OUTSIDE
		break;
	case MOUSE_INEDITREG:	///On Mouse Move
		m_MouseInEditRegion = 1;
		m_ExitEditRegion = 0;
		note = YtoNote(pointMouse.y);
		switch(m_nDrawMode)
		{
		case DRAWMODE_NOP:
			pEV = m_pSong->GetEventObject(m_nDrawEvent);
			if (pEV)
			{
				pObj = pEV->GetEventObjectHead();
				if (m_nMouseState == StaffViewMouseState::STAFFVIEW_MOUSEDOWN)
				{
					CMsObject* pSelectedObjects = NULL;
					Loop = TRUE;

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
				else // Mouse button is up
				{
					//--------------------------
					// highlight objects and
					// other nonsense
					//---------------------------
					Loop = TRUE;
					while (pObj && Loop)
					{
						if (pObj->HighLight(TRUE, pointMouse))
						{
//							printf("Highligt Set'\n");
							Loop = FALSE;
						}
						else
						{
							pObj = pObj->GetNext();
						}
					}
					if (pObj == NULL)
					{
//						printf("No Object Found, Clear Highlight\n");
						if (m_pHighLightedObject)
							m_pHighLightedObject->SetHighLight(FALSE);
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
		case DRAWMODE_NOTE:
			//-------------------------------
			// Move a note around the screen
			//--------------------------------
			pN = (CMsNote*)m_pDrawObject;
			printf("@@@@@@@@@@@@ LasrPitch = %d  Note = %d @@@@@@@@@@@\n", GetLastPitch(), note);
			if (LastPitchIsValid())
			{
				if (GetLastPitch() != note)
				{
					SetPitch(note);
					if (!pN->IsRest())
					{
						printf("    Note Off %d\n", pN->GetPitch());
						MidiPlayNote(pN, 0);// Note Off
					}
					pN->SetPitch(note);
					if (!pN->IsRest())
					{
						printf("    Note On %d\n", pN->GetPitch());
						MidiPlayNote(pN, 1);// Note On
					}
					SetLastPitch(note);
					if (pN->IsRest())
					{
						StatusString.Format(_T("Draw Rest at Event %d"), m_nDrawEvent);
					}
					else
					{
						pN->ObjectToString(csTemp);
						StatusString.Format(_T("%lS at Event %d: "), csTemp.GetString(), m_nDrawEvent);
					}
				}
			}
			else if (!LastPitchIsValid() && IsMouseInEditRegion())
			{
				pN->SetPitch(note);
				if (!pN->IsRest())
					MidiPlayNote(pN, 1);// Note On
				SetLastPitch(note);
			}
			Invalidate();
			break;
		case DRAWMODE_ENDBAR:
			if (MSOBJ_ENDBAR == m_pDrawObject->GetType())
			{
				StatusString.Format(_T("Draw Song End Bar At Event %d"), m_nDrawEvent);
				Invalidate();
			}
			break;
		case DRAWMODE_BAR:		///On Mouse Move
			if (MSOBJ_BAR == m_pDrawObject->GetType())
			{
				StatusString.Format(_T("Draw Measure Bar At Event %d"), m_nDrawEvent);
				Invalidate();
			}
			break;
		case DRAWMODE_KKEYSIG:		///On Mouse Move
			StatusString.Format(_T("Draw Key Signature at event %d"), m_nDrawEvent);
			Invalidate();
			break;
		case DRAWMODE_TIMESIG:		///On Mouse Move
			StatusString.Format(_T("Draw Time Signature at event %d"), m_nDrawEvent);
			Invalidate();
			break;
		case DRAWMODE_TEMPO:
			StatusString.Format(_T("Draw Tempo Change at event %d"), m_nDrawEvent);
			Invalidate();
			break;
		case DRAWMODE_TIE:		///On Mouse Move
			switch (m_nDrawState)
			{
			case DRAWSTATE_TIE_FIRSTNOTE:
				StatusString.Format(_T("Select First Note at Event %d"), m_nDrawEvent);
				break;
			case DRAWSTATE_TIE_SECONDNOTE:
				m_TieEndPoint = CPoint(pointMouse.x, m_TieStartPoint.y);
				StatusString.Format(_T("Select Second Note at Event %d"), m_nDrawEvent);
				Invalidate();
				break;
			}	//end of switch(m_nDrawState)
			break;
		case DRAWMODE_LOUDNESS:		///On Mouse Move
			StatusString.Format(_T("Change Loudness at event %d"), m_nDrawEvent);
			Invalidate();
			break;
		case DRAWMODE_MOVE:
			StatusString.Format(_T("Move Block To Event %d"), m_nDrawEvent);
			m_Status.SetText(StatusString);
			break;
		}
		break;
	}
	if (StatusString.GetLength())m_Status.SetText(StatusString);
	CWnd::OnMouseMove(nFlags, pointMouse);
}

void CChildViewStaff::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMsNote* pN;
	CMsObject* pTemp;
	int Trip, Dot;
	INT Dur;
	CString csBlank(_T(""));
	CString csTemp;

	switch (nChar)
	{
	case VK_MENU:
		m_AltKeyDown = 1;
		break;
	case VK_SHIFT:
		m_ShiftKeyDown = 1;
		break;
	case VK_CONTROL:
		m_CtrlKeyDown = 1;
		break;
	case 'R':	//toggle REST mode.
		if (DRAWMODE_NOTE != m_nDrawMode)
		{
			m_nDrawMode = DRAWMODE_NOTE;
			if (m_pDrawObject) delete m_pDrawObject;
			pN = new CMsNote;
			pN->Create(0, GetSong(), m_nDrawEvent);
			m_pDrawObject = pN;
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
			Dur = CMsNote::NoteDurLut[DurTab[(int)Dur].NoteShapIndex];
			pN->SetDuration(Dur);
			pN->SetRest(1);
		}
		else
		{
			pN = (CMsNote*)m_pDrawObject;
			pN->SetRest(pN->IsRest() ^ 1);
		}
		csTemp = CString("Draw Rest");
		m_Status.SetText(csTemp);
		break;
	case 'N':
		m_nDrawMode = DRAWMODE_NOTE;
		pN = new CMsNote;
		pN->Create(0, GetSong(), m_nDrawEvent);
		if (m_pDrawObject)
		{
			delete m_pDrawObject;
		}
		m_pDrawObject = pN;
		//-----------------------------
		// decode message word
		//------------------------------
		pN->GetData().CopyData(GetNoteData());
		csTemp = CString("Draw Note");
		m_Status.SetText(csTemp);
		break; 
	case 'B':
		m_nDrawMode = DRAWMODE_BAR;
		if (m_pDrawObject) delete m_pDrawObject;
		m_pDrawObject = new CMsBar;
		((CMsBar*)m_pDrawObject)->Create(GetSong(), m_nDrawEvent);
		csTemp = CString("Draw Measure Bar");
		m_Status.SetText(csTemp);
		break;
	case VK_OEM_PERIOD:	//dotted note toggle
		if (DRAWMODE_NOTE == m_nDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)m_pDrawObject;
			Dur = pN->GetDuration();
			Dot = DurTab[(int)Dur].Dotted;
			Trip = DurTab[(int)Dur].Triplet;
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
	case '3':	//triplet toggle
		if (DRAWMODE_NOTE == m_nDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)m_pDrawObject;
			Dur = pN->GetDuration();
			Dot = DurTab[(int)Dur].Dotted;
			Trip = DurTab[(int)Dur].Triplet;
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
	case 'T':	// change to thirty second note
		if (DRAWMODE_NOTE == m_nDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)m_pDrawObject;
			Dur = pN->GetDuration();
			Dot = DurTab[(int)Dur].Dotted;
			Trip = DurTab[(int)Dur].Triplet;
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
			Invalidate();
		}
		break;
	case 'S':	// change to Sixteenth  note
		if (DRAWMODE_NOTE == m_nDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)m_pDrawObject;
			Dur = pN->GetDuration();
			Dot = DurTab[(int)Dur].Dotted;
			Trip = DurTab[(int)Dur].Triplet;
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
			Invalidate();
		}
		break;
	case 'E':	// change to eight note
		if (DRAWMODE_NOTE == m_nDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)m_pDrawObject;
			Dur = pN->GetDuration();
			Dot = DurTab[(int)Dur].Dotted;
			Trip = DurTab[(int)Dur].Triplet;
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
			Invalidate();
		}
		break;
	case 'Q':	//change to quarter note
		if (DRAWMODE_NOTE == m_nDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)m_pDrawObject;
			Dur = pN->GetDuration();
			Dot = DurTab[(int)Dur].Dotted;
			Trip = DurTab[(int)Dur].Triplet;
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
			Invalidate();
		}
		break;
	case 'H':	//change to half note
		if (DRAWMODE_NOTE == m_nDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)m_pDrawObject;
			Dur = pN->GetDuration();
			Dot = DurTab[(int)Dur].Dotted;
			Trip = DurTab[(int)Dur].Triplet;
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
			Invalidate();
		}
		break;
	case 'W':	//change to whole note
		if (DRAWMODE_NOTE == m_nDrawMode)
		{
			int DurTemp;

			pN = (CMsNote*)m_pDrawObject;
			Dur = pN->GetDuration();
			Dot = DurTab[(int)Dur].Dotted;
			Trip = DurTab[(int)Dur].Triplet;
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
			Invalidate();
		}
		break;
	case VK_ESCAPE:	//exit draw mode
		switch (m_nDrawMode)
		{
		case DRAWMODE_NOTE:
			if (LastPitchIsValid())
			{
				//------------------------------
				// If the draw object is a NOTE,
				// we need to make sure that it
				// is turned off.
				//------------------------------
				pN = (CMsNote*)m_pDrawObject;
				MidiPlayNote(pN, 0);// Note Off
				SetLastPitch(INVALID_PITCH);
			}
			break;
		}	//end of switch(m_nDrawMode) in case MOUSE_INUPPERSEL
		m_nDrawMode = DRAWMODE_NOP;
		//----------------------------------
		// The Next Three Lines of code are
		// required to be in this order 
		// because we need to set m_pDrawObject
		// to zero first before deleting it.
		// The delete methode does not nessesarily
		// return here right away.  This means
		// that we need to insure that OnPaint
		// does not use m_pDrawObject befpre
		// It is deleted.
		//-----------------------------------------------
		pTemp = m_pDrawObject;
		m_pDrawObject = 0;
		delete pTemp;

		m_Status.SetText(csBlank);
		printf("################ Escape Key\n");
		m_EscapeFlag = 1;
		//		Invalidate();
		break;
	case VK_SPACE:	//flip note
		if (m_pDrawObject)
		{
			if (m_pDrawObject->GetType() == MSOBJ_NOTE)
			{
				CMsNote* pN = (CMsNote*)m_pDrawObject;
				if (m_CtrlKeyDown)
					pN->SetHeadFlipped(pN->GetHeadFlipped() ^ 1);
				else
				{
					pN->SetUpsideDown(pN->GetUpsideDown() ^ 1);
					if (pN->IsUpsideDown())
						pN->SetHeadFlipped(1);
					else
						pN->SetHeadFlipped(0);
				}
				Invalidate();
				pN->Print(stdout);
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
	case VK_MENU:
		m_AltKeyDown = 0;
		break;
	case VK_SHIFT:
		m_ShiftKeyDown = 0;
		break;
	case VK_CONTROL:
		m_CtrlKeyDown = 0;
		break;
	default:
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
	if ((m_nDrawMode == DRAWMODE_MIDINOTEIN) && pEv)
	{
		ConTexMenu.AppendMenuW(MF_STRING, ID_CM_CHANGEINST, _T("Change Instrument"));
		ConTexMenu.AppendMenuW(MF_STRING, ID_CM_CHANGEDURATION, _T("Change Note Duration"));
	}
	if ((m_nDrawMode == DRAWMODE_NOTE) && pEv)
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
				case MSOBJ_BAR:	//OnContextMenu
					break;
				case MSOBJ_ENDBAR:	//OnContextMenu
					break;
				case MSOBJ_KEYSIG:	//OnContextMenu
					csStatus.Format(_T("Key Signature"));
					ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EXTRA_ITEMS + i, csStatus);
					pContextMenueList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case MSOBJ_LOUDNESS:	//OnContextMenu
					csStatus.Format(_T( "Loudness"));
					ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EXTRA_ITEMS + i, csStatus);
					pContextMenueList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case MSOBJ_NOTE:	//OnContextMenu
					csStatus = CString("Note:");
					Obj.pNote->ObjectToString(csTemp);
					csStatus.Append(csTemp);
					ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EXTRA_ITEMS + i, csStatus);
					pContextMenueList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case MSOBJ_REPEATEND:	//OnContextMenu
				case MSOBJ_REPEATSTART:
					csStatus.Format(_T("Repeat"));
					ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EXTRA_ITEMS + i, csStatus);
					pContextMenueList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case MSOBJ_TEMPO:	//OnContextMenu
					csStatus.Format(_T("Tempo"));
					ConTexMenu.AppendMenuW(MF_STRING, ID_CM_EXTRA_ITEMS + i, csStatus);
					pContextMenueList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case MSOBJ_TIMESIG:	//OnContextMenu
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

		Dlg.SetBitmaps((GETAPP->bmGetIntrument(0)));
		Dlg.SetNumberOfBitmaps(GETAPP->GetNumInstruments());
		Dlg.SetSelection((int)((m_nMidiInputNoteSetup >> DRAW_NOTE_TRACK_SHIFT) & 0x0f)-1);
		Dlg.SetCaption(CString( "Select New Instrument"));
		if (IDOK == Dlg.DoModal())
		{
			m_nMidiInputNoteSetup &= ~(0x0f << DRAW_NOTE_TRACK_SHIFT);
			m_nMidiInputNoteSetup |= (Dlg.GetSelection() +1) << DRAW_NOTE_TRACK_SHIFT;
			int Track = Dlg.GetSelection() + 1;
			int Patch = GETMIDIINFO->GetPatch(Track);
			int DeviceID = GETMIDIINFO->GetMidiOutDeviceId(Track);
			int Channel = GETMIDIINFO->GetChannel(Track);
			GETMIDIOUTDEVICE(DeviceID).PgmChange(Channel, Patch);
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
				case MSOBJ_BAR:	//OnContextMenu
				case MSOBJ_ENDBAR:
					break;
				case MSOBJ_KEYSIG:	//OnContextMenu
				{
					CSelectStringDlg Dlg;
					Dlg.SetCaptionString( CString("Change Key Signature"));
					Dlg.SetNumStrings(GETAPP->GetNumKeySigs() );
					for (int i = 0; i < (GETAPP->GetNumKeySigs()); i++)
					{
						Dlg.SetSelectionString(i, CMsKeySignature::KeySigStringTab[i + 1]);
					}
					Dlg.m_nSelection = Obj.pKey->GetKeySignature() - 1;
					if (IDOK == Dlg.DoModal())
						Obj.pKey->SetKeySignature(Dlg.m_nSelection);
				}
				break;
				case MSOBJ_LOUDNESS:	//OnContextMenu
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
				case MSOBJ_NOTE:	//OnContextMenu
				{
					CNotePropertiesDlg Dlg;

					Dlg.SetNoteToEdit(Obj.pNote);
					Dlg.DoModal();
				}
				break;
				case MSOBJ_REPEATEND:	//OnContextMenu
				case MSOBJ_REPEATSTART:
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
				case MSOBJ_TEMPO:	//OnContextMenu
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
				case MSOBJ_TIMESIG:	//OnContextMenu
				{
					CSelectorDlg Dlg;
					Dlg.SetBitmaps(GETAPP->bmGetCbTimeSig(COMBO_TIMESIG_2_2));
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

int NoteLut[7] = {
	0,	//C
	2,	//D
	4,	//E
	5,	//F
	7,	//G
	9,	//A
	11	//B
};


int CChildViewStaff::YtoNote(int mouseY)
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
	mouseY -= HIGHC_OFFSET;
	//----------------------------------
	// quantize the y coordiant so that
	// the note will only be drawn either
	// in the space, or on the line.  We do
	// not quantize the mouse pointer
	//-----------------------------------
	mouseY = QuantizeY(mouseY);
	mouseY = QUANTIZED_STAFF_HEIGHT - mouseY;	//invert y
	mouseY -= STAVE_LINE_SPACING / 2;	//center
	//------------------------------------
	// Limit the range that y can take
	//------------------------------------
	if (mouseY < 0) mouseY = 0;
	NoteIndex = mouseY % 7;
	Note = NoteLut[NoteIndex];
	Octave += (mouseY / 7) * 12;	/// calculate octave
	Note = Note + Octave + 0x24;
	if (Note > 0x60)
		Note = 0x60;	// limit max value of Note
	return Note;
}

int NoteToY(int Note)
{
	int rV = 0;
	return rV;
}


UINT CChildViewStaff::GetRawEventNumber(int x)
{
	//----------------------------------
	// calculates the "event" to its
	// physical location on the staff
	// For the logical "event" use
	// the function XtoEventIndex
	//-----------------------------------
	int ev = x - EVENT_OFFSET;
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
	Width -= EVENT_OFFSET;
	return Width / EVENT_WIDTH;
}

int CChildViewStaff::QuantizeY(int y)
{
	return y / 4;
}

void CChildViewStaff::SetupDrawMode(int DrawMode,long v)
{
	CString csStatus,csTemp;
	CString csBlank(_T(""));
	int From, To;
	//------------------------
	// Decode sub message
	//------------------------
	switch (DrawMode)
	{
	case DRAW_NOP:
		if (m_pDrawObject) delete m_pDrawObject;
		m_pDrawObject = 0;
		m_nDrawMode = DRAWMODE_NOP;
		m_Status.SetText(csBlank);
		break;
	case DRAW_NOTE:
	{
		CMsNote* pN = new CMsNote;
		if (m_pDrawObject)
		{
			delete m_pDrawObject;
		}
		m_pDrawObject = pN;
		m_nDrawMode = DRAWMODE_NOTE;
		//-----------------------------
		// decode message word
		//------------------------------
		pN->GetData().CopyData(GetNoteData());
		if (pN->IsRest())
		{
			int Shape;
			Shape = pN->GetShape();
			int Id = CMidiSeqMSApp::RestBmIdsTypes[Shape];
			pN->Create(Id, GetSong(), m_nDrawEvent);
		}
		else
			pN->Create(0, GetSong(), m_nDrawEvent);

		pN->ObjectToString(csTemp);
		csStatus.Format(_T("Draw %lS Event %d"), csTemp.GetString(),m_nDrawEvent);
	}
	m_Status.SetText(csStatus);
	break;
	case DRAW_REST:
		break;
	case DRAW_ENDBAR:
		{
			CMsEndBar* pEB = new CMsEndBar;
			pEB->Create(GetSong(), m_nDrawEvent);
			m_nDrawMode = DRAWMODE_ENDBAR;
			m_pDrawObject = pEB;
		}
		csTemp = CString("Draw Measure End Bar");
		m_Status.SetText(csTemp);
		break;
	case DRAW_BAR:
	{
		CMsBar* pB = new CMsBar;
		pB->Create(GetSong(), m_nDrawEvent);
		m_nDrawMode = DRAWMODE_BAR;
		m_pDrawObject = pB;
	}
	csTemp = CString("Draw Measure Bar");
	m_Status.SetText(csTemp);
	break;
	case DRAW_TIE:
		if (m_pDrawObject) delete m_pDrawObject;
		m_pDrawObject = 0;
		m_nDrawMode = DRAWMODE_TIE;
		m_nDrawState = DRAWSTATE_TIE_FIRSTNOTE;
		m_pFirstTieNote = 0;
		m_pSecondTieNote = 0;
		csTemp = CString("Draw Tie");
		m_Status.SetText(csTemp);
		break;
	case DRAW_COPY:
		m_nDrawMode = DRAWMODE_COPY;
		csTemp = CString("Copy Block");
		m_Status.SetText(csTemp);
		break;
	case DRAW_MOVE:
		m_nDrawMode = DRAWMODE_MOVE;
		csStatus.Format(_T("Move Block To Event %d"), m_nDrawEvent);
		m_Status.SetText(csStatus);
		break;
	case DRAW_REPEAT:
		AddRepeat(v);
		Invalidate();
		csTemp = CString("Add Repeat");
		m_Status.SetText(csTemp);
		break;
	case DRAW_TEMPO:
	{
		CMsTempo* pMT = new CMsTempo();
		pMT->Create(GetSong(),GetDrawEvent(),v);
		m_nDrawMode = DRAWMODE_TEMPO;
		m_pDrawObject = pMT;
	}
	csTemp = CString("Add Tempo");
	m_Status.SetText(csTemp);
	break;
	case DRAW_TIMESIG:
	{
		CMsTimeSignature* pTS = new CMsTimeSignature;
		pTS->Create(GetSong(), GetDrawEvent(), COMBO_TIMESIG_4_4);
		m_nDrawMode = DRAWMODE_TIMESIG;
		m_pDrawObject = pTS;
	}
	csTemp = CString("Add Time Signature");
	m_Status.SetText(csTemp);
	break;
	case DRAW_KEYSIG:
	{
		CMsKeySignature* pKS = new CMsKeySignature;
		pKS->SetKeySignature(v);
		m_nDrawMode = DRAWMODE_KKEYSIG;
		m_pDrawObject = pKS;
	}
	csTemp = CString("Add Key Signature");
	m_Status.SetText(csTemp);
	break;
	case DRAW_LOUDNESS:
	{
		CMsLoudness* pLD = new CMsLoudness;
		pLD->SetLoudness(v);
		m_nDrawMode = DRAWMODE_LOUDNESS;
		m_pDrawObject = pLD;
	}
	csTemp = CString("Add Loudness");
	m_Status.SetText(csTemp);
	break;
	case DRAW_INSTCHANGE:
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
	case DRAW_CHANGEDUR:
		From = v & 0x1f;
		To = (v >> 5) & 0x1f;
		ChangeDuration(From, To);
		Invalidate();
		csTemp = CString("Change Duration in Selection");
		m_Status.SetText(csTemp);
		break;
	case DRAW_INCREASEDUR:
		IncreaseDuration();
		csTemp = CString("Increase Duration in Selection");
		m_Status.SetText(csTemp);
		break;
	case DRAW_DECREASEDUR:
		//		DecreaseDuration();  ToDo Add this function
		csTemp = CString("Decrease Duration in Selection");
		m_Status.SetText(csTemp);
		break;
	case DRAW_INCRPITCH:
		IncrPitch();
		csTemp = CString("Increase Pitch In Selection");
		m_Status.SetText(csTemp);
		Invalidate();
		break;
	case DRAW_DECRPITCH:
		DecrPitch();
		csTemp = CString("Decrease Pitch in Selection");
		m_Status.SetText(csTemp);
		Invalidate();
		break;
	case DRAW_INSERTBLOCK:
		InsertBlock();
		// ToDo Might need more code here
		SetScrollRange(GetSong()->GetTotalEvents() - GetMaxEvents());
		csTemp = CString("Insert Event");
		m_Status.SetText(csTemp);
		Invalidate();
		break;
	case DRAW_DRAWNOTESVIAMIDI:
		m_nDrawMode = DRAWMODE_MIDINOTEIN;
		m_nMidiInputNoteSetup = v;
		csTemp = CString("Step Input Notes Via MIDI");
		m_Status.SetText(csTemp);
		break;
	}
}

void CChildViewStaff::GetEventRect(int Event,CRect& rect)
{
	int x = EVENT_OFFSET + EVENT_WIDTH * Event;
	int y = STAVE_OFFSET - MAXNOTE_HEIGHT;
	int cx = EVENT_WIDTH;
	int cy = STAFF_HEIGHT;
	rect = CRect(CPoint(x, y), CSize(cx, cy));
}

int CChildViewStaff::MouseInRegion(CPoint p)
{
	int rV = MOUSE_OUTSIDE;

	if (m_rgnEdit.PtInRegion(p)) rV = MOUSE_INEDITREG;
	else if (m_rgnUpper.PtInRegion(p)) rV = MOUSE_INUPPERSEL;
	else if (m_rgnLower.PtInRegion(p)) rV = MOUSE_INLOWERSEL;
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
			CMsObject* pOb = pEv->GetEventObjectHead();
			while (pOb)
			{
				if (MSOBJ_NOTE == pOb->GetType())
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
			CMsObject* pOb = pEv->GetEventObjectHead();
			while (pOb)
			{
				if (MSOBJ_NOTE == pOb->GetType())
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
		///--------------------------------
		/// We add the repeat by adding
		/// a new event before and after
		/// the selected block.
		///---------------------------------
		CMsEvent* pRS, * pRE;
		CMsRepeatStart* pRepStrt;
		CMsRepeatEnd* pRepEnd;

		pRS = m_pSong->InsertEvent(m_FirstSelectedEvent - 1);
		m_pSong->RenumberEvents(&m_FirstSelectedEvent, &m_LastSelectedEventIndex);
		pRE = m_pSong->InsertEvent(m_LastSelectedEventIndex);
		m_pSong->RenumberEvents(&m_FirstSelectedEvent, &m_LastSelectedEventIndex);
		pRepStrt = new CMsRepeatStart();
		pRepStrt->Create(GetSong(), nRepeatCount, pRS->GetIndex());
		pRS->AddObjectAtStart(pRepStrt);
		pRepEnd = new CMsRepeatEnd;
		pRepEnd->Create(GetSong(), pRE->GetIndex());
		pRE->AddObjectAtStart(pRepEnd);
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
			CMsObject* pOb = pEv->GetEventObjectHead();
			while (pOb)
			{
				if (MSOBJ_NOTE == pOb->GetType())
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
			CMsObject* pOb = pEv->GetEventObjectHead();
			while (pOb)
			{
				if (MSOBJ_NOTE == pOb->GetType())
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
			pOb = pEv->GetEventObjectHead();

			while (pOb)
			{
				CMsObject* pNewObj;
				pNewObj = pOb->MakeANewObject();
				pNewObj->Copy(pOb);
				pEvNew->AddObjectAtEnd(pNewObj);
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

	CWnd::SetScrollInfo(SB_HORZ, &si, TRUE);
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

int NoteAdjustLUT[12] = {
	0,
	1,
	0,
	1,
	0,
	0,
	1,
	0,
	1,
	0,
	1,
	0
};

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
	case MIDI_NOTEOFF:		//OnShortmidimsg
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
				adj = NoteAdjustLUT[note];
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
	case MIDI_NOTEON:		//OnShortmidimsg
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
			else if ((m_nDrawMode == DRAWMODE_MIDINOTEIN) && (m_LastSelectedEventIndex >= 0))
			{		//this is a NOTE on, so Add a note
//				int note, adj;
				note = Note % 12;
				adj = NoteAdjustLUT[note];
				note -= adj;
				note += 12 * (Note / 12);
				CMsEvent* pEv = m_pSong->GetEventObject(m_LastSelectedEventIndex);
				m_nMidiNotesOn++;
				CMsNote* pN = new CMsNote();
				pN->Create(0, GetSong(), m_nDrawEvent);
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
					pEv->AddObjectAtEnd(pN);

				}
				else
					pEv->RemoveObject(pO);
				CMsObject* pObj;
				pObj = pEv->GetEventObjectHead();
				csStatus.Format(_T("%lS"), _T("Draw Note(s) :"));
				int count = 0;
				while (pObj)
				{
					if (MSOBJ_NOTE == pObj->GetType())
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
	case MIDI_POLYPRESS:
		break;
	case MIDI_CTRLCHNG:
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
						pOb = pEv->GetEventObjectHead();
						while (pOb)
						{
							pObT = pOb->GetNext();
							if (MSOBJ_NOTE == pOb->GetType())
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
	case MIDI_CHNLPRESS:
		break;
	case MIDI_PITCHBEND:
		break;
	case MIDI_PGMCHANGE:
		break;
	case MIDI_CLOCK:
		break;
	case MIDI_START:
		break;
	case MIDI_STOP:
		break;
	case MIDI_CONTINUE:
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

	if ((m_nDrawMode != DRAWMODE_NOTE) && (m_nDrawMode != DRAWMODE_MIDINOTEIN))
	{
		if (m_pDrawObject) delete m_pDrawObject;
		m_pDrawObject = new CMsNote;
		m_nDrawMode = DRAWMODE_NOTE;
	}
	if (m_pDrawObject == 0) m_pDrawObject = new CMsNote;
	pN = (CMsNote *)m_pDrawObject;
	pN->Create(0, GetSong(), m_nDrawEvent);
	UpdateComboBoxes();
	pN->GetData().CopyData(GetNoteData());

	int Dur = m_nMidiInputNoteSetup & DRAW_NOTE_DURATION;
	Dur = CMsNote::NoteDurLut[DurTab[(int)pN->GetDuration()].NoteShapIndex];
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
	int i;
	int n;
	int y;
	int x;
	CSize itemSize;
	int x1;
	int x2;
	int y1;
	int y2;

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
	m_pSong->Create(this);

//------------ Event 0 ------------------------------
	CMsTempo* pTM = new CMsTempo();
	pTM->Create(GetSong(), GetDrawEvent(),100);
	GetSong()->AddObjectToSong(GetDrawEvent(), pTM);
	CMsTimeSignature* pTS = new CMsTimeSignature();
	pTS->Create(GetSong(), GetDrawEvent(), COMBO_TIMESIG_4_4);
	GetSong()->AddObjectToSong(GetDrawEventAndInc(), pTS);
	//-------------- Event 1 -----------------------
	CMsKeySignature* pKS = new CMsKeySignature;
	pKS->Create(GetSong(), GetDrawEvent(), MSFF_CSMAJ);
	GetSong()->AddObjectToSong(GetDrawEvent(), pKS);
	CMsLoudness* pLD = new CMsLoudness();
	pLD->Create(GetSong(), GetDrawEvent(),100);
	GetSong()->AddObjectToSong(GetDrawEventAndInc(), pLD);
	//----------------------------------------------------
	UpdateScrollbarInfo(m_pSong->GetTotalEvents(),"Init");
	//----------------------------------------
	// Initialize Combo Boxes
	// Instrument Selection
	//---------------------------------------
	m_Combo_Instrument.Create(
		5,
		15,
		CSize(28, 28),
		CPoint(ControlX, 0),
		CSize(16,28),
		this,
		IDC_COMBO_INSTRUMENT
	);
	n = GETAPP->GetNumInstruments();
	for (i = 1; i < (n+1); ++i)
	{
		m_Combo_Instrument.AddBitmap(
			GETMIDIINFO->GetTrack(i).GetBitmap());

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
	n = GETAPP->GetNumNoteTypes();
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
	itemSize = GetBmDimensions(CMidiSeqMSApp::DecorationsBmCbIdsSel[COMBO_DECORATION_ACCENT]);
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
		m_Combo_Decorations.AddNotSelBitmapID(CMidiSeqMSApp::DecorationsBmCbIdsNotSel[i]);
		m_Combo_Decorations.AddSelBitmapID(CMidiSeqMSApp::DecorationsBmCbIdsSel[i]);
		m_Combo_Decorations.SetItemFlags(i, CBDecorationFlags[i]);
	}
	ControlX += m_Combo_Decorations.GetTotalWidth();
	//--------------------------------------
	// Initialize accidental combo box
	//---------------------------------------
	n = GETAPP->GetNumAccidentalTypes();
	itemSize = GetBmDimensions(CMidiSeqMSApp::AccidentalBmCBIdsTypes[COMBO_ACCIDENTAL_INKEY]);
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
		CPoint(100,clientRect.bottom - 25),
		CSize(16, itemSize.cy),		// widty of arrow reginon
		this,	//	parrent
		IDC_COMBO_MISCSTUFF			// ID
	);
	for (i = 0; i < n; ++i)
	{
		m_Combo_Misc.AddBitmap(GETAPP->bmGetMiscType(i));
	}

//----------------------------------------------
	y = clientRect.bottom - 25;
	x = 0;
	m_Status.Create(
		CRect(CPoint(x, y),CSize(clientRect.Width(), 25)),
		this,
		IDC_STATIC_STATUSDISP);
	m_Status.ShowWindow(TRUE);
	//----- Create Regions ---------
	CRect EditRect;
	CPoint ptRectUL;
	CSize szRect;

	ptRectUL = CPoint(clientRect.left + EVENT_OFFSET, STAVE_OFFSET - MAXNOTE_HEIGHT);
	szRect = CSize(clientRect.Width() - EVENT_OFFSET, STAFF_HEIGHT);

	EditRect = CRect(ptRectUL, szRect);
	PrintRec("ChildView EditRgn", EditRect);
	EditRect.NormalizeRect();
	m_rgnEdit.CreateRectRgn(EditRect);

	ptRectUL = CPoint(ptRectUL.x, EditRect.top - 20);
	szRect = CSize(szRect.cx, 20);

	m_UpperSelRect = CRect(ptRectUL, szRect);
	PrintRec("ChildView UpperRgn", m_UpperSelRect);
	m_UpperSelRect.NormalizeRect();
	m_rgnUpper.CreateRectRgn(m_UpperSelRect);

	ptRectUL = CPoint(ptRectUL.x, EditRect.bottom);
	szRect = CSize(szRect.cx, 20);

	m_LowerSelRect = CRect(ptRectUL, szRect);
	m_LowerSelRect.NormalizeRect();
	m_rgnLower.CreateRectRgn(m_LowerSelRect);

	m_SelectRectTop = m_UpperSelRect.bottom;
	m_szSelectRect = CSize(EVENT_WIDTH, m_LowerSelRect.top - m_UpperSelRect.bottom);

	//-------------- Button Controls ---------
	itemSize = m_Button_Play.GetButtonSize(IDB_BUTTON_PLAY_UP);
	m_Button_Play.Create(
	CPoint(0, clientRect.bottom - itemSize.cy -25),
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
		CPoint(48, clientRect.bottom - itemSize.cy - 25),
		this,
		IDC_BUTTON_STOP,
		IDB_BUTTON_STOP_UP,
		IDB_BUTTON_STOP_DOWN,
		&m_Button_Play
	);
	m_Button_Stop.EnableWindow(0);
	//----------------------------------
	// Set combo boxes to their defaul
	// Selectopm
	//----------------------------------
	m_Combo_Instrument.SetCurSel((int)GetTrack());

	m_Combo_BlockOps.SetCurSel(COMBO_BLOCK_COPY);

	m_Combo_Misc.SetCurSel(COMBO_MISC_MEASUREBAR);

	pKS = (CMsKeySignature*)GetSong()->GetObjectTypeInEvent(MSOBJ_KEYSIG, (int)EventObjectSignatureTypes::EVENT_LOUDNESS_KEYSIG);
	if(pKS)
		m_Combo_KeySig.SetCurSel(pKS->GetKeySignature() - 1);

	pTS = (CMsTimeSignature*)GetSong()->GetObjectTypeInEvent(MSOBJ_TIMESIG,  (int)EventObjectSignatureTypes::EVENT_TEMPO_TIMESIG);
	if(pTS)
		m_Combo_TimeSig.SetCurSel((int)pTS->GetTimeSignature() - 1);

	m_Combo_Decorations.SetCurSel(COMBO_DECORATION_NONE);

	m_Combo_Accidentals.SetCurSel(COMBO_ACCIDENTAL_INKEY);
	SetAccidental(MSFF_ACCIDENTAL_INKEY);

	m_Combo_Rests.SetCurSel(COMBO_REST_QUARTER);

	m_Combo_NoteType.SetCurSel(COMBO_NOTE_QUARTER);
	m_ReadyToDraw = 1;
//	GETMIDI->SetMessageDestWind(this);
}

void CChildViewStaff::OnDraw(CDC* pDC)
{
	///----------------------------------------
	/// OnDraw
	///		This is the function that is called
	/// when the scre
	/// en needs to be redrawn.
	/// To minimize screen flicker, all drawing
	/// is done to a Memory DC, and then it is
	/// blt to the screen.
	///
	///	parameters:
	///		pDC....pointer to the device context
	///----------------------------------------
	CDC DCm;	//memory device context
	CMyBitmap bm,*pOldBM;
	CRect rect;
	CBrush br, green, blue;

	if (m_ReadyToDraw)
	{
		GetClientRect(&rect);
		//create a compatable memory device context
		DCm.CreateCompatibleDC(pDC);
		bm.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		pOldBM = (CMyBitmap*)DCm.SelectObject(&bm);
		// Fill in background
		br.CreateSolidBrush(m_color_BackGround);
		blue.CreateSolidBrush(RGB(60, 100, 224));
		green.CreateSolidBrush(RGB(88, 128, 88));
		DCm.FillRect(&rect, &br);
		DCm.FillRect(&m_LowerSelRect, &green);
		DCm.FillRect(&m_UpperSelRect, &green);
		CRect lastEvent;
		GetEventRect(m_MaxEvents, lastEvent);
		DCm.FillRect(&lastEvent, &blue);
		if (m_pSong)
		{
			if (m_EscapeFlag)
			{
				m_EscapeFlag = 0;
			}
			m_pSong->Draw(&DCm, m_SongScrollPos, m_MaxEvents, &rect);
			if (m_nDrawMode && m_pDrawObject && m_MouseInEditRegion)
			{
				m_pDrawObject->Draw(&DCm, m_nDrawEvent - m_SongScrollPos, m_MaxEvents);
			}
		}
		if (m_nDrawMode == DRAWMODE_TIE && m_nDrawState == DRAWSTATE_TIE_SECONDNOTE)
		{
			CRect r;
			r.SetRect(m_TieEndPoint + CSize(0, -16), m_TieStartPoint + CSize(0, 16));
			r.NormalizeRect();
			DCm.Arc(r, m_TieStartPoint - CSize(1, 0), m_TieEndPoint - CSize(1, 0));
		}
		DrawControls(&DCm);
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &DCm, 0, 0, SRCCOPY);
		DCm.SelectObject(pOldBM);
	}
}

BOOL CChildViewStaff::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
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
	case COMBO_DECOR_NONE:
		SetDotted(0);
		SetTriplet(0);
		break;
	case COMBO_DECOR_DOT:
		SetDotted(1);
		SetTriplet(0);
		break;
	case COMBO_DECOR_TRIPLET:
		SetDotted(0);
		SetTriplet(1);
		break;
	}
	SetTrack(m_Combo_Instrument.GetCurSel() );		//Instrument number needs +1
	INT Track = GetTrack();
	int Patch = GETMIDIINFO->GetPatch((int)Track);
	int DeviceID = GETMIDIINFO->GetMidiOutDeviceId((int)Track);
	int Channel = GETMIDIINFO->GetChannel((int)Track);
	GETMIDIOUTDEVICE(DeviceID).PgmChange(Channel, Patch);
	SetDuration(CMsNote::NoteDurLut[m_Combo_NoteType.GetCurSel()]);
}

LRESULT CChildViewStaff::MyControlsMessages(WPARAM ComboID, LPARAM nSelection)
{
	int Selection, mode = 0;
	int v = 0;
	CBToggle ToggleMSG;
	int op;
	CMsNote* pNote;

	switch (ComboID)
	{
	case IDC_COMBO_MISCSTUFF:
		Selection = m_Combo_Misc.GetCurSel();
		switch (Selection)
		{
		case COMBO_MISC_ENDBAR:
			mode = DRAW_ENDBAR;
			break;
		case COMBO_MISC_MEASUREBAR:	//measure bar
			mode = DRAW_BAR;
			break;
		case COMBO_MISC_NOTETIE:	//note tie
			mode = DRAW_TIE;
			break;
		case COMBO_MISC_LOUDNESS:	//Loudness (Note Velocity)
			mode = DRAW_LOUDNESS;
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
			mode = DRAW_TEMPO;
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
			mode = DRAW_INSTCHANGE;
			break;
		}	// End of MISC switch (Selection)
		SetupDrawMode(mode, v);
		break;
	case IDC_COMBO_NOTETYPES:
		v = CMsNote::NoteDurLut[nSelection];
		if (GetTriplet()) v -= 2;
		else if (GetDotted()) v += 2;
		SetDuration(v);
		m_Combo_Decorations.ShowWindow(SW_SHOW);
		m_Combo_Accidentals.ShowWindow(SW_SHOW);
		m_Combo_Decorations.EnableWindow(1);
		m_Combo_Accidentals.EnableWindow(1);
		SetRest(0);
		if (m_pDrawObject)
		{
			if (m_pDrawObject->GetType() != MSOBJ_NOTE)
			{
				delete m_pDrawObject;
				pNote = new CMsNote;
				pNote->Create(0, GetSong(), m_nDrawEvent);
				m_pDrawObject = pNote;
			}
		}
		else
		{
			pNote = new CMsNote;
			pNote->Create(0, GetSong(), m_nDrawEvent);
			m_pDrawObject = pNote;
		}
		m_nDrawMode = DRAWMODE_NOTE;
		UpdateNoteDrawObject();
		break;
	case IDC_COMBO_RESTTYPES:
		m_Combo_Decorations.ClearAllItems(COMBO_DECORATION_NONE);
		m_Combo_Accidentals.SetCurSel(COMBO_ACCIDENTAL_INKEY);
		m_Combo_Accidentals.EnableWindow(0);
		m_Combo_Decorations.EnableWindow(0);
		m_Combo_Accidentals.ShowWindow(SW_HIDE);
		m_Combo_Decorations.ShowWindow(SW_HIDE);
		SetDuration(CMsNote::NoteDurLut[nSelection]);
		SetFocus();
		SetRest(1);
		if (m_pDrawObject)
			delete m_pDrawObject;
		pNote = new CMsNote;
		pNote->Create((COMBO_REST_HALF < nSelection) ? CMidiSeqMSApp::RestBmIdsTypes[nSelection] : 0, GetSong(), m_nDrawEvent);
		m_pDrawObject = pNote;
		m_nDrawMode = DRAWMODE_NOTE;
		UpdateNoteDrawObject();
		break;
	case IDC_COMBO_INSTRUMENT:
		SetTrack(m_Combo_Instrument.GetCurSel() + 1);		//Instrument number needs +1
		SetFocus();
		UpdateNoteDrawObject();
		break;
	case IDC_COMBO_BLOCKOPS:
		op = m_Combo_BlockOps.GetCurSel();
		DoBlockOps(op);
		break;
	case IDC_COMBO_TIMESIG:
		SetFocus();
		SetupDrawMode(DRAW_TIMESIG, m_Combo_TimeSig.GetCurSel() + 1);
		break;
	case IDC_COMBO_KEYSIG:
		SetFocus();
		SetupDrawMode(DRAW_KEYSIG, m_Combo_KeySig.GetCurSel() + 1);
		break;
	case IDC_BUTTON_PLAY:
		OnButtonPlay();
		break;
	case IDC_BUTTON_STOP:
		OnButtonStop();
		break;
	case IDC_COMBO_ACCIDENTALS:
		SetAccidental(CMsNote::AccedentalsLUT[nSelection] );
		UpdateNoteDrawObject();
		break;
	case IDC_COMBO_DECORATIONS:
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
					SetDuration(CMsNote::NoteDurLut[DurTab[GetDuration()].NoteShapIndex]);
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
				SetDuration(CMsNote::NoteDurLut[DurTab[GetDuration()].NoteShapIndex] + 2);
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

				SetDuration(CMsNote::NoteDurLut[DurTab[GetDuration()].NoteShapIndex] - 2);
				break;
			}	//end of switch (ToggleMSG.index)
			break;
		case CBT_INDEX_ONLY:
			break;
		} //end if switch (ToggleMSG.command)
		SetFocus();
		SetupDrawMode(DRAW_NOTE);
		break;
	default:
		break;
	}	// end of switch (ComboID)
	return 0;
}

//---------------- Combo Boxes ---------------

void CChildViewStaff::DoBlockOps(int Op)
{
	int mode = 0, v = 0;

	SetFocus();
	switch (Op)
	{
	case COMBO_BLOCK_COPY:	//block copy
		mode = DRAW_COPY;
		break;
	case COMBO_BLOCK_MOVE:	//block move
		mode = DRAW_MOVE;
		break;
	case COMBO_BLOCK_CHANGEINSTRUMENT:	//change instrument
	{
		int InstFrom, InstTo;
		{
			CSelectorDlg Dlg;
			Dlg.SetBitmaps(GETAPP->bmGetIntrument(0));
			Dlg.SetNumberOfBitmaps(GETAPP->GetNumInstruments());
			Dlg.SetCaption(CString("Instrument FROM"));
			Dlg.SetSelection(0);
			//					CSelInstDlg Dlg;
			//					Dlg.m_pPromtString = "FROM";
			if (IDOK == Dlg.DoModal())
			{
				InstFrom = Dlg.GetSelection() + 1;
				Dlg.SetBitmaps(GETAPP->bmGetIntrument(0));
				Dlg.SetCaption(CString("Instrument TO"));
				Dlg.SetSelection(0);
				if (IDOK == Dlg.DoModal())
				{
					InstTo = Dlg.GetSelection() + 1;
					v = (InstFrom & 0xf) | (InstTo << 4);
					mode = DRAW_INSTCHANGE;
				}
				else
					mode = DRAW_NOP;
			}
			else
				mode = DRAW_NOP;
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
			mode = DRAW_CHANGEDUR;
		}
		{
			CSelDurDlg Dlg;
			Dlg.m_pPrompt = "TO";
			Dlg.m_nDuration = 12;
			Dlg.DoModal();
			DurTo = Dlg.m_nDuration;
			v = (DurFrom & 0x1f) | (DurTo << 5);
			mode = DRAW_CHANGEDUR;
		}
	}
	break;
	case COMBO_BLOCK_INCREASE:	//Increase duration
		mode = DRAW_INCREASEDUR;
		break;
	case COMBO_BLOCK_DECREASE:	//Decrease duration
		mode = DRAW_DECREASEDUR;
		break;
	case COMBO_BLOCKUP:	//increase pitch up
		mode = DRAW_INCRPITCH;
		break;
	case COMBO_BLOCKDOWN:	//increase pitch down
		mode = DRAW_DECRPITCH;
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
		mode = DRAW_REPEAT;
	}
	break;
	case COMBO_BLOCK_INSERT:	//insert blok
		mode = DRAW_INSERTBLOCK;
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
	if (GETMIDIOUTTABLE.GetNumDevices())
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
	if (m_pDrawObject)
	{
		if (m_pDrawObject->GetType() == MSOBJ_NOTE)
		{
			CMsNote* pNote = (CMsNote*)m_pDrawObject;
			pNote->GetData().CopyData(GetNoteData());
		}
	}
}

void CChildViewStaff::UpdateColors()
{
	m_Combo_Instrument.Reset();
	for (int i = 0; i < GETAPP->GetNumInstruments(); ++i)
	{
		m_Combo_Instrument.AddBitmap(GETMIDIINFO->GetTrack(i+1).GetBitmap());
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
GetSong()->SetIsPlaying(0);
GETAPP->PlayerThreadDeleteSong(GetSong());
GetSong()->GetDelSongCompleteEV().Pend();
break;
	}
	return 0;
}

void CChildViewStaff::OnTimer(UINT_PTR nIDEvent)
{
	printf("***************** One Minute ***************************\n");
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
		MSOBJ_KEYSIG,
		pEV,
		SEARCH_REVERSE
	);
	pLoudness = GetSong()->GetMsObject(
		MSOBJ_LOUDNESS,
		pEV,
		SEARCH_REVERSE
	);
	if (pKeySig && pLoudness)
	{
		if (NoteOnFlag)
		{
			pNote->NoteOn((CMsKeySignature*)pKeySig, 100);
		}
		else
		{
			pNote->NoteOff((CMsKeySignature*)pKeySig, 100);
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
			return TRUE;
			break;
		}
		break;
	case WM_SYSKEYUP:
		switch (pMsg->wParam)
		{
		case VK_MENU:
			OnKeyUp(pMsg->wParam, 1, 0);
			return TRUE;
			break;
		}
		break;
	}
	return CChildViewBase::PreTranslateMessage(pMsg);
}
