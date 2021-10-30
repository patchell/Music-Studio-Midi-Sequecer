// StaffView.cpp : implementation file
//

#include "stdafx.h"
#include "musicstudioplayer.h"
#include "MSPlayerDlg.h"
#include "StaffView.h"
#include "MsObject.h"
#include "MsBar.h"
#include "MsKeySignature.h"
#include "MsLoudness.h"
#include "MsNote.h"
#include "MsTempo.h"
#include "MsTimeSig.h"
#include "MsRepeatEnd.h"
#include "MsRepeatStart.h"
#include "SelDurDlg.h"
#include "ContextMenuList.h"
#include "NotePropertiesDlg.h"
#include "ParamDlg.h"
#include "SelectorDlg.h"
#include "SelectStringDlg.h"
#include "WindowMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MOUSE_OUTSIDE		0
#define MOUSE_INEDITREG		1
#define MOUSE_INUPPERSEL	2
#define MOUSE_INLOWERSEL	3

/////////////////////////////////////////////////////////////////////////////
// CStaffView

extern DUR DurTab[0x15];
extern int NoteDurLut[6];

IMPLEMENT_DYNCREATE(CStaffView, CView)

CStaffView::CStaffView()
	: m_nMidiNotesOn(0)
	, m_nMidiInputNoteSetup(0)
	, m_pPgmChng(NULL)
	, m_pLastSelNote(NULL)
	, m_nLastSongPosition(0)
	, m_nMouseState(0)
	,m_pRepeatEndSelected(0)
	,m_pRepeatStartSelected(0)
{
	m_pSong = 0;
	m_pDisp = 0;
	m_MaxEvents = 0;
	m_pDrawObject = 0;
	pLastNote = 0;
	m_MouseInEditRegion = 0;
	m_ExitEditRegion = 0;
	m_lastpitch=0;
	m_LastSelectedEventIndex = -1;
	m_FirstSelectedEvent = -1;
	m_SongScrollPos = 0;
	m_pStatus = 0;
	m_nMidiNotesOn = 0;
	m_pTempoSelected = 0;
	m_pLoudnessSelected = 0;
	m_pTimeSigSelected = 0;
	m_pKeySigSelected = 0;
}

CStaffView::~CStaffView()
{
}


BEGIN_MESSAGE_MAP(CStaffView, CView)
	//{{AFX_MSG_MAP(CStaffView)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DRAWMODE,OnDrawMessage)
	ON_MESSAGE(WM_INIT_STAFF,OnInitStaff)
	ON_MESSAGE(WM_SHORTMIDIMSG, &CStaffView::OnShortmidimsg)
	ON_MESSAGE(WM_STAFF_DISP_EVENT, &CStaffView::OnStaffDispEvent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaffView drawing

void CStaffView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
	CRect rect;
	CBrush br(RGB(255,255,255));


	GetClientRect(&rect);
	rect.NormalizeRect();
	pDC->FillRect(&rect,&br);
	CPoint Offset = rect.CenterPoint();
	if(m_pSong)
	{
		m_pSong->Draw(pDC,m_SongScrollPos,m_MaxEvents,&rect);
		if(this->m_nDrawMode && this->m_pDrawObject && m_MouseInEditRegion)
		{
			m_pDrawObject->Draw(pDC,m_nDrawEvent-m_SongScrollPos,m_MaxEvents);
		}
	}
	if(m_nDrawMode == DRAWMODE_TIE && m_nDrawState == DRAWSTATE_TIE_SECONDNOTE)
	{
		CRect r;
		r.SetRect(m_TieEndPoint+ CSize(0,-16),m_TieStartPoint + CSize(0,16));
		r.NormalizeRect();
		pDC->Arc(r,m_TieStartPoint-CSize(1,0),m_TieEndPoint-CSize(1,0));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CStaffView diagnostics

#ifdef _DEBUG
void CStaffView::AssertValid() const
{
	CView::AssertValid();
}

void CStaffView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStaffView message handlers

void CStaffView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CalcMaxEvents();
	ShowScrollBar(SB_HORZ);
	UpdateScrollbarInfo(1);
	this->EnableScrollBar(SB_HORZ,ESB_ENABLE_BOTH);
}

void CStaffView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	char *s = new char[256];
	int event = this->XtoEventIndex(point.x);
	int Note = YtoNote(point.y);
	int Region;
	CMsEvent *pEv;
	ObjectTypes obj;

	m_nMouseState = STAFFVIEW_MOUSEDOWN;
	Region = this->MouseInRegion(point);
	switch (Region)
	{
	case MOUSE_OUTSIDE:
		break;
	case MOUSE_INUPPERSEL:
	case MOUSE_INLOWERSEL:
		break;
	case MOUSE_INEDITREG:
		switch (this->m_nDrawMode)
		{
			case DRAWMODE_NOP:
				obj.pNote = m_pSong->GetNoteAtEvent(Note, event);
				if (obj.pNote)
				{
					obj.pNote->SetSelected(1);
					m_pDrawObject = obj.pObj;
				}
				break;
			case DRAWMODE_NOTE:
				break;
		}
		break;
	}

	sprintf_s(s,256,"x=%d y=%d  EVENT=%d  NOTE=%d",point.x,point.y,event,Note);
	m_pDisp->SetWindowText(s);
	delete[] s;
	CView::OnLButtonDown(nFlags, point);
}

void CStaffView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int Region;
	CMsEvent *pEv;

	m_nMouseState = STAFFVIEW_MOUSEUP;
	m_nDrawEvent = this->XtoEventIndex(point.x);
	Region = this->MouseInRegion(point);
	switch(Region)
	{
		case MOUSE_OUTSIDE:
			break;
		case MOUSE_INUPPERSEL:
		case MOUSE_INLOWERSEL:
			if((pEv = m_pSong->GetEventObject(m_nDrawEvent)) != NULL)
			{
				if(pEv->IsSelected())	//deselect block
				{
					if(this->m_FirstSelectedEvent < 0)
					{
						pEv->SetSelected(0);
						m_LastSelectedEventIndex = -1;
					}
					else
					{
						int i;
						int loop;
						pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
						for(i=m_FirstSelectedEvent,loop = 1;(i<m_LastSelectedEventIndex+1) && loop;++i)
						{
							if(pEv)	
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
					this->m_pStatus->SetText("");
				}
				else//select events
				{
					if(m_LastSelectedEventIndex >= 0)
					{
						//we are going to select a block
						if(m_FirstSelectedEvent < 0)
						{
							m_FirstSelectedEvent = m_LastSelectedEventIndex;
							m_LastSelectedEventIndex = m_nDrawEvent;
							if(m_LastSelectedEventIndex < m_FirstSelectedEvent)
							{
								int temp = m_LastSelectedEventIndex;
								m_LastSelectedEventIndex = m_FirstSelectedEvent;
								m_FirstSelectedEvent = temp;
							}
							int i;
							pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
							for(i=m_FirstSelectedEvent;i<m_LastSelectedEventIndex+1;++i)
							{
								pEv->SetSelected(1);
								pEv = pEv->GetNext();
							}
							char *s = new char[256];
							sprintf_s(s, 256, "Block Selected %d:%d", m_FirstSelectedEvent, m_LastSelectedEventIndex);
							m_pStatus->SetText(s);
							delete[]s;
						}
						else	//add more events to the block
						{
							if(m_nDrawEvent < m_FirstSelectedEvent)
							{
								pEv = m_pSong->GetEventObject(m_nDrawEvent);
								for(int i=m_nDrawEvent;i<m_FirstSelectedEvent+1;++i)
								{
									pEv->SetSelected(1);
									pEv = pEv->GetNext();
								}
								m_FirstSelectedEvent = m_nDrawEvent;
							}
							else if (m_nDrawEvent > m_LastSelectedEventIndex)
							{
								pEv = m_pSong->GetEventObject(m_LastSelectedEventIndex);
								for(int i=m_LastSelectedEventIndex;i<m_nDrawEvent+1;++i)
								{
									pEv->SetSelected(1);
									pEv = pEv->GetNext();
								}
								m_LastSelectedEventIndex = m_nDrawEvent;
							}
							char *s = new char[256];
							sprintf_s(s, 256, "Block Selected %d:%d", m_FirstSelectedEvent, m_LastSelectedEventIndex);
							m_pStatus->SetText(s);
							delete[]s;
						}
					}
					else
					{
						pEv->SetSelected(1);
						m_LastSelectedEventIndex = m_nDrawEvent;
						char *s = new char[256];
						sprintf_s(s, 256, "Event Selected:%d", m_nDrawEvent);
						m_pStatus->SetText(s);
						delete[] s;
					}
				}
				Invalidate();
			}
			break;
		case MOUSE_INEDITREG:	
			switch(this->m_nDrawMode)
			{
				case DRAWMODE_NOP:
					break;
				case DRAWMODE_NOTE:
					m_pSong->AddObjectToSong(m_nDrawEvent,m_pDrawObject);
					SetScrollRange(SB_HORZ,0,m_pSong->m_nTotalEvents);
					{
						CMsNote *pN = new CMsNote;
						CMsNote *pOld = (CMsNote *) m_pDrawObject;
						pOld->m_pEvParent = m_pSong->GetEventObject(m_nDrawEvent);
						//-----------------------------
						// Copy attributes
						//------------------------------
						pN->m_hMidiOut = m_pSong->m_hMidiOut;
						pN->m_pMidiInfo = m_pSong->GetMidiInfo();
						pN->SetDuration(pOld->GetDuration());
						pN->SetPitch(pOld->GetPitch());
						pN->SetAccent(pOld->IsAccent());
						pN->SetKey(pOld->GetKey());
						pN->SetTrack(pOld->GetTrack());
						pN->SetRest(pOld->IsRest());
						pN->SetUpsideDown(pOld->GetUpsideDown());
						m_pDrawObject = pN;
					}
					Invalidate();
					break;
				case DRAWMODE_BAR:
					m_pSong->AddObjectToSong(m_nDrawEvent,m_pDrawObject);
					SetScrollRange(SB_HORZ,0,m_pSong->m_nTotalEvents);
					{
						CMsBar *pB = new CMsBar;
						m_pDrawObject = pB;
					}
					Invalidate();
					break;
				case DRAWMODE_KKEYSIG:
					m_pSong->AddObjectToSong(m_nDrawEvent,m_pDrawObject);
					SetScrollRange(SB_HORZ,0,m_pSong->m_nTotalEvents);
					{
						CMsKeySignature *pKS = new CMsKeySignature;
						CMsKeySignature *pOld = (CMsKeySignature *)m_pDrawObject;
						pKS->SetKey(pOld->GetKey());
						m_pDrawObject = pKS;
					}
					Invalidate();
					break;
				case DRAWMODE_TIMESIG:
					m_pSong->AddObjectToSong(m_nDrawEvent,m_pDrawObject);
					SetScrollRange(SB_HORZ,0,m_pSong->m_nTotalEvents);
					{
						CMsTimeSig *pTS,*pOld;
						pTS = new CMsTimeSig;
						pOld = (CMsTimeSig *)m_pDrawObject;
						pTS->SetTimeSig(pOld->GetTimeSig());
						m_pDrawObject = pTS;
					}
					Invalidate();
					break;
				case DRAWMODE_TIE:	///OnLButtonDown
					///-----------------------------------
					/// This is where we change the state of
					/// drawing a tie.  When the mouse releases
					/// we need to check to see if there really
					/// is a note under the mouse pointer.
					///  If there is, record the note and the
					/// points.
					///------------------------------------------
					switch(this->m_nDrawState)
					{
						case DRAWSTATE_TIE_FIRSTNOTE:
							{
								int note = YtoNote(point.y);
								CMsNote *pN = m_pSong->CheckForNotePresence(m_nDrawEvent,note);
								if(pN)
								{
									this->m_pFirstTieNote = pN;
									this->m_nDrawState = DRAWSTATE_TIE_SECONDNOTE;
									this->m_TieStartPoint = point + CSize(0,4);
									char *s = new char[256];
									int l = sprintf_s(s, 256, "First Note ");
									l = pN->NoteToString(s, 256);
									sprintf_s(&s[l], 256-l, " Selected at Event %d", m_nDrawEvent);
									m_pStatus->SetText(s);
									delete[] s;
								}
							}
							break;
						case DRAWSTATE_TIE_SECONDNOTE:
							{
								int note = YtoNote(point.y);
								CMsNote *pN = m_pSong->CheckForNotePresence(m_nDrawEvent,note);
								if(pN)
								{
									this->m_pSecondTieNote = pN;
									char *s = new char[256];
									int l = sprintf_s(s, 256, "Second Note ");
									l += pN->NoteToString(&s[l], 256-l);
									sprintf_s(&s[l], 256 - l, " Selected at Event %d", m_nDrawEvent);
									m_pStatus->SetText(s);
									delete[] s;

									this->m_nDrawState = DRAWSTATE_TIE_FIRSTNOTE;
									this->m_TieEndPoint = point + CSize(0,4);
									if(m_TieEndPoint.x > m_TieStartPoint.x)
									{
										m_pFirstTieNote->SetTieBeg(1);
										m_pSecondTieNote->SetTieEnd(1);
									}
									else
									{
										m_pFirstTieNote->SetTieEnd(1);
										m_pSecondTieNote->SetTieBeg(1);
									}
									m_pFirstTieNote = 0;
									m_pSecondTieNote = 0;
									Invalidate();
								}
							}
							break;
					}
					break;
				case DRAWMODE_LOUDNESS:
				case DRAWMODE_TEMPO:
					m_pSong->AddObjectToSong(m_nDrawEvent,m_pDrawObject);
					SetScrollRange(SB_HORZ,0,m_pSong->m_nTotalEvents);
					m_pDrawObject = 0;
					m_nDrawMode = DRAWMODE_NOP;
					Invalidate();
					break;
				case DRAWMODE_COPY:
					this->CopyBlock(m_nDrawEvent);
					SetScrollRange(SB_HORZ,0,m_pSong->m_nTotalEvents);
					m_pDrawObject = 0;
					m_nDrawMode = DRAWMODE_NOP;
					Invalidate();
					break;
				case DRAWMODE_MOVE:
					MoveBlock(m_nDrawEvent);
					m_pDrawObject = 0;
					m_nDrawMode = DRAWMODE_NOP;
					Invalidate();
					break;
			}//end of switch(this->m_nDrawMode)
			break;
	}	//end of switch(Region)
	CView::OnLButtonUp(nFlags, point);
}

void CStaffView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMsNote *pN;
	CMsObject *pObj;
	int Region;
	int note;
	int flag;

	m_nDrawEvent = this->XtoEventIndex(point.x);
	Region = this->MouseInRegion(point);
	switch(Region)
	{
		case MOUSE_INUPPERSEL:	///OnMouseMove
		case MOUSE_INLOWERSEL:
			if(m_MouseInEditRegion)
				m_ExitEditRegion = 1;
			else
				m_ExitEditRegion = 0;
			m_MouseInEditRegion = 0;
			if (m_pLastSelNote)
			{
				m_pLastSelNote->SetSelected(0);
				m_pLastSelNote = 0;
			}
			switch(m_nDrawMode)
			{
				case DRAWMODE_NOTE:
					if(m_lastpitch && m_ExitEditRegion)
					{
						pN = (CMsNote *)m_pDrawObject;
						pN->NoteOff(0,m_pKeySig);
						m_lastpitch = 0;
					}
					break;
			}	//end of switch(m_nDrawMode) in case MOUSE_INUPPERSEL
			Invalidate();
			break;
		case MOUSE_OUTSIDE:	///On Mouse Move
			if(m_MouseInEditRegion)
				m_ExitEditRegion = 1;
			else
				m_ExitEditRegion = 0;
			m_MouseInEditRegion = 0;
			if (m_pLastSelNote)
			{
				m_pLastSelNote->SetSelected(0);
				m_pLastSelNote = 0;
			}
			switch(m_nDrawMode)
			{
				case DRAWMODE_NOTE:
					if(m_lastpitch && m_ExitEditRegion)
					{
						pN = (CMsNote *)m_pDrawObject;
						pN->NoteOff(0,m_pKeySig);
						m_lastpitch = 0;
					}
					break;
			}	//end of switch(m_nDrawMode) in case MOUSE_OUTSIDE
			Invalidate();
			break;
		case MOUSE_INEDITREG:	///On Mouse Move
			m_MouseInEditRegion = 1;
			m_ExitEditRegion = 0;
			note = YtoNote(point.y);
			//-----------------------------------
			// check to see if mouse is over
			// a Tempo Object
			//-----------------------------------
			MouseOverObject(m_nDrawEvent, MSOBJ_TEMPO, &m_pTempoSelected);
			//-----------------------------------
			// Check to see if the mouse is over
			// a Loudness Object.
			//-----------------------------------
			MouseOverObject(m_nDrawEvent, MSOBJ_LOUDNESS, &m_pLoudnessSelected);
			//------------------------------------
			// check to see if the mouse is over
			// a key signature object
			//-----------------------------------
			MouseOverObject(m_nDrawEvent, MSOBJ_KEYSIG, &m_pKeySigSelected);
			//-----------------------------------
			// check to see if the mouse is over
			// a time signature object
			//-----------------------------------
			MouseOverObject(m_nDrawEvent,MSOBJ_TIMESIG,&m_pTimeSigSelected);
			//-----------------------------------
			// Check to see if the mouse is over
			// a repeat symbol
			//-----------------------------------
			if (m_pSong->IsRepeatInThisEvent(m_nDrawEvent))
			{
				int matchingEvent;

				pObj = m_pSong->GetRepeatObject(m_nDrawEvent);
				if (MSOBJ_REPEATSTART == pObj->GetType())
				{
					m_pRepeatStartSelected = (CMsRepeatStart *)pObj;
					m_pRepeatStartSelected->SetSelected(1);
					m_pRepeatEndSelected = (CMsRepeatEnd *)m_pSong->FindMatchingRepeatObject(m_pRepeatStartSelected, m_nDrawEvent, &matchingEvent);
					if (m_pRepeatEndSelected)m_pRepeatEndSelected->SetSelected(1);
					Invalidate();
				}
				else
				{
					m_pRepeatEndSelected = (CMsRepeatEnd *)pObj;
					m_pRepeatEndSelected->SetSelected(1);
					m_pRepeatStartSelected = (CMsRepeatStart *)m_pSong->FindMatchingRepeatObject(m_pRepeatEndSelected, m_nDrawEvent, &matchingEvent);
					if (m_pRepeatStartSelected)m_pRepeatStartSelected->SetSelected(1);
					Invalidate();
				}
			}
			else
			{
				if (m_pRepeatEndSelected)
				{
					m_pRepeatEndSelected->SetSelected(0);
					m_pRepeatEndSelected = 0;
				}
				if (m_pRepeatStartSelected)
				{
					m_pRepeatStartSelected->SetSelected(0);
					m_pRepeatStartSelected = 0;
					Invalidate();
				}
			}
			//--------------------------------------
			// Check to see if the mouse cursor is
			// over a Note.
			//--------------------------------------
			if (m_pSong->IsNoteInThisEvent(note, m_nDrawEvent))
			{
				if (m_pLastSelNote)m_pLastSelNote->SetSelected(0);
				CMsNote *pN = m_pSong->GetNoteAtEvent(note, m_nDrawEvent);
				pN->SetSelected(1);
				m_pLastSelNote = pN;
				Invalidate();
			}
			else
			{
				if (m_pLastSelNote)
				{
					m_pLastSelNote->SetSelected(0);
					m_pLastSelNote = 0;
					Invalidate();
				}
			}
			//--------------------------------
			// do mouse things for various
			// Draw modes.
			//---------------------------------
			switch(m_nDrawMode)
			{
				case DRAWMODE_NOP:
					if (m_nMouseState == STAFFVIEW_MOUSEDOWN)
					{
						CMsNote *pN = (CMsNote *)m_pDrawObject;
						if(pN) pN->SetPitch(note);
					}
					break;
				case DRAWMODE_NOTE:		///On Mouse Move
					if(MSOBJ_NOTE == this->m_pDrawObject->GetType())
					{
						pN = (CMsNote *)m_pDrawObject;
						if(m_lastpitch)
						{
							if(m_lastpitch != note)
							{
								if(!pN->IsRest()) pN->NoteOff(0,m_pKeySig);
								pN->SetPitch(note);
								if(!pN->IsRest()) pN->NoteOn(127,m_pKeySig);
								m_lastpitch = note;
								char *s = new char[256];
								int l;
								if (pN->IsRest())
								{
									l = sprintf_s(s, 256, "Draw Rest at Event %d",this->m_nDrawEvent);
								}
								else
								{
									l = sprintf_s(s, 256, "Draw Note at Event %d: ",m_nDrawEvent);
									l += pN->NoteToString(&s[l], 256 - l);
								}
								this->m_pStatus->SetText(s);
								//--------------------------------------
								// if note is on top of another note
								// that is the same, invert color
								//--------------------------------------
								CMsEvent *pEv = m_pSong->GetEventObject(m_nDrawEvent);
								if (pEv)
								{
									if (pEv->ObjectAlreadyHere(pN))
									{
										pN->SetSelected(1);
									}
									else
									{
										pN->SetSelected(0);
									}
								}
								delete[] s;
							}
						}
						else if(m_lastpitch && m_ExitEditRegion && !m_MouseInEditRegion)
						{
							if(!pN->IsRest()) pN->NoteOff(0,m_pKeySig);
							m_lastpitch = 0;
						}
						else if(!m_lastpitch && m_MouseInEditRegion)
						{
							pN->SetPitch(note);
							if(!pN->IsRest()) pN->NoteOn(127,m_pKeySig);
							m_lastpitch = note;
						}
						Invalidate();
					}
					break;
				case DRAWMODE_BAR:		///On Mouse Move
					if(MSOBJ_BAR == this->m_pDrawObject->GetType())
					{
						char *s = new char[256];
						sprintf_s(s, 256, "Draw Measure Bar At Event %d", m_nDrawEvent);
						m_pStatus->SetText(s);
						delete[] s;
						Invalidate();
					}
					break;
				case DRAWMODE_KKEYSIG:		///On Mouse Move
					{
						char *s = new char[256];
						sprintf_s(s, 256, "Draw Key Signature at event %d", m_nDrawEvent);
						m_pStatus->SetText(s);
						delete[]s;
					}
					Invalidate();
					break;
				case DRAWMODE_TIMESIG:		///On Mouse Move
				{
					char *s = new char[256];
					sprintf_s(s, 256, "Draw Time Signature at event %d", m_nDrawEvent);
					m_pStatus->SetText(s);
					delete[]s;
				}
				Invalidate();
				case DRAWMODE_TEMPO:
				{
					char *s = new char[256];
					sprintf_s(s, 256, "Draw Tempo Change at event %d", m_nDrawEvent);
					m_pStatus->SetText(s);
					delete[]s;
				}
				Invalidate();
				break;
				case DRAWMODE_TIE:		///On Mouse Move
					switch(this->m_nDrawState)
					{
						case DRAWSTATE_TIE_FIRSTNOTE:
							{
								char *s = new char[256];
								sprintf_s(s, 256, "Select First Note at Event %d", m_nDrawEvent);
								m_pStatus->SetText(s);
								delete[] s;
							}
							break;
						case DRAWSTATE_TIE_SECONDNOTE:
							this->m_TieEndPoint = CPoint(point.x,m_TieStartPoint.y);
							{
								char *s = new char[256];
								sprintf_s(s, 256, "Select Second Note at Event %d", m_nDrawEvent);
								m_pStatus->SetText(s);
								delete[] s;
							}
							Invalidate();
							break;
					}
					break;
				case DRAWMODE_LOUDNESS:		///On Mouse Move
				{
					char *s = new char[256];
					sprintf_s(s, 256, "Change Loudness at event %d", m_nDrawEvent);
					m_pStatus->SetText(s);
					delete[]s;
				}
				Invalidate();
				break;
			}
			break;
	}
	CView::OnMouseMove(nFlags, point);
}

void CStaffView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CStaffView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CMsNote *pN;
	int Dur, Trip, Dot;
	long lp;
	char *s;

	switch(nChar)
	{
		case 'R':	//toggle REST mode.
			if (DRAWMODE_NOTE != m_nDrawMode)
			{
				m_nDrawMode = DRAWMODE_NOTE;
				if (m_pDrawObject) delete m_pDrawObject;
				pN = new CMsNote;
				m_pDrawObject = pN;
				//-----------------------------
				// decode message word
				//------------------------------
				lp = ((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->CreateNoteMessageWord();
				pN->m_pMidiInfo = m_pSong->GetMidiInfo();
				pN->m_hMidiOut = m_pSong->m_hMidiOut;
				Dur = lp & DRAW_NOTE_DURATION;
				Dur = NoteDurLut[DurTab[Dur].NoteShapIndex];
				pN->SetDuration(Dur);
				pN->SetAccent((lp >> DRAW_NOTE_ACCENT_SHIFT) & 0x01);
				pN->SetKey((lp >> DRAW_NOTE_KEY_SHIFT) & 0x03);
				pN->SetTrack((lp >> DRAW_NOTE_TRACK_SHIFT) & 0x0f);
				pN->SetRest(1);
			}
			else
			{
				pN = (CMsNote *)m_pDrawObject;
				pN->SetRest(pN->IsRest() ^ 1);
			}
			m_pStatus->SetText("Draw Rest");
			break;
		case 'N':
			m_nDrawMode = DRAWMODE_NOTE;
			lp = ((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->CreateNoteMessageWord();
			pN = new CMsNote;
			if (m_pDrawObject)
			{
				delete m_pDrawObject;
			}
			m_pDrawObject = pN;
			//-----------------------------
			// decode message word
			//------------------------------
			pN->m_pMidiInfo = m_pSong->GetMidiInfo();
			pN->m_hMidiOut = m_pSong->m_hMidiOut;
			pN->SetDuration(lp & DRAW_NOTE_DURATION);
			pN->SetAccent((lp >> DRAW_NOTE_ACCENT_SHIFT) & 0x01);
			pN->SetKey((lp >> DRAW_NOTE_KEY_SHIFT) & 0x03);
			pN->SetTrack((lp >> DRAW_NOTE_TRACK_SHIFT) & 0x0f);
			pN->SetRest((lp >> DRAW_NOTE_REST_SHIFT) & 1);
			s = new char[256];
			sprintf_s(s, 256, "Draw Note Message");
			this->m_pDisp->SetWindowText(s);
			m_pStatus->SetText("Draw Note");
			delete[] s;
			break;
		case 'B':
			m_nDrawMode = DRAWMODE_BAR;
			if (m_pDrawObject) delete m_pDrawObject;
			m_pDrawObject = new CMsBar;
			m_pStatus->SetText("Draw Measure Bar");
			break;
		case VK_OEM_PERIOD:	//dotted note toggle
			if (DRAWMODE_NOTE == m_nDrawMode)
			{
				pN = (CMsNote *)m_pDrawObject;
				Dur = pN->GetDuration();
				Dot = DurTab[Dur].Dotted;
				Trip = DurTab[Dur].Triplet;
				if (Dot) Dur -= 2;	//undot
				else if (Trip) Dur += 4;	//untrip and dot
				else Dur += 2;	//Dot
				pN->SetDuration(Dur);
				Invalidate();
			}
			break;
		case '3':	//triplet toggle
			if (DRAWMODE_NOTE == m_nDrawMode)
			{
				pN = (CMsNote *)m_pDrawObject;
				Dur = pN->GetDuration();
				Dot = DurTab[Dur].Dotted;
				Trip = DurTab[Dur].Triplet;
				if (Dot) Dur -= 4;	//undot and trip
				else if (Trip) Dur += 2;	//untrip
				else Dur -= 2;	//Tripplet
				pN->SetDuration(Dur);
				Invalidate();
			}
			break;
		case 'T':	// change to thirty second note
			if (DRAWMODE_NOTE == m_nDrawMode)
			{
				pN = (CMsNote *)m_pDrawObject;
				Dur = pN->GetDuration();
				Dot = DurTab[Dur].Dotted;
				Trip = DurTab[Dur].Triplet;
				Dur = THIRTYSEC_NOTE;
				if (Dot) Dur += 2;
				else if (Trip) Dur -= 2;
				pN->SetDuration(Dur);
				Invalidate();
			}
			break;
		case 'S':	// change to Sixteenth  note
			if (DRAWMODE_NOTE == m_nDrawMode)
			{
				pN = (CMsNote *)m_pDrawObject;
				Dur = pN->GetDuration();
				Dot = DurTab[Dur].Dotted;
				Trip = DurTab[Dur].Triplet;
				Dur = SIXTEENTH_NOTE;
				if (Dot) Dur += 2;
				else if (Trip) Dur -= 2;
				pN->SetDuration(Dur);
				Invalidate();
			}
			break;
		case 'E':	// change to eight note
			if (DRAWMODE_NOTE == m_nDrawMode)
			{
				pN = (CMsNote *)m_pDrawObject;
				Dur = pN->GetDuration();
				Dot = DurTab[Dur].Dotted;
				Trip = DurTab[Dur].Triplet;
				Dur = EIGTH_NOTE;
				if (Dot) Dur += 2;
				else if (Trip) Dur -= 2;
				pN->SetDuration(Dur);
				Invalidate();
			}
			break;
		case 'Q':	//change to quarter note
			if (DRAWMODE_NOTE == m_nDrawMode)
			{
				pN = (CMsNote *)m_pDrawObject;
				Dur = pN->GetDuration();
				Dot = DurTab[Dur].Dotted;
				Trip = DurTab[Dur].Triplet;
				Dur = QUARTER_NOTE;
				if (Dot) Dur += 2;
				else if (Trip) Dur -= 2;
				pN->SetDuration(Dur);
				Invalidate();
			}
			break;
		case 'H':	//change to half note
			if (DRAWMODE_NOTE == m_nDrawMode)
			{
				pN = (CMsNote *)m_pDrawObject;
				Dur = pN->GetDuration();
				Dot = DurTab[Dur].Dotted;
				Trip = DurTab[Dur].Triplet;
				Dur = HALF_NOTE;
				if (Dot) Dur += 2;
				else if (Trip) Dur -= 2;
				pN->SetDuration(Dur);
				Invalidate();
			}
			break;
		case 'W':	//change to whole note
			if (DRAWMODE_NOTE == m_nDrawMode)
			{
				pN = (CMsNote *)m_pDrawObject;
				Dur = pN->GetDuration();
				Dot = DurTab[Dur].Dotted;
				Trip = DurTab[Dur].Triplet;
				Dur = WHOLE_NOTE;
				if (Dot) Dur += 2;
				else if (Trip) Dur -= 2;
				pN->SetDuration(Dur);
				Invalidate();
			}
			break;
		case VK_ESCAPE:	//exit draw mode
			switch(m_nDrawMode)
			{
				case DRAWMODE_NOTE:
					if(m_lastpitch)
					{
						pN = (CMsNote *)m_pDrawObject;
						pN->NoteOff(0,m_pKeySig);
						m_lastpitch = 0;
					}
					break;
			}	//end of switch(m_nDrawMode) in case MOUSE_INUPPERSEL
			this->m_nDrawMode = DRAWMODE_NOP;
			delete this->m_pDrawObject;
			m_pDrawObject = 0;
			this->m_pStatus->SetText("");
			Invalidate();
			break;
		case VK_SPACE:	//flip note
			if(m_pDrawObject)
			{
				if(m_pDrawObject->GetType() == MSOBJ_NOTE)
				{
					CMsNote *pN = (CMsNote *)m_pDrawObject;
					pN->SetUpsideDown(pN->GetUpsideDown() ^ 1);
					Invalidate();
				}
			}
			break;
		default:
			CView::OnKeyUp(nChar, nRepCnt, nFlags);
			break;
	}	
}


#define ID_CM_INSERTEVEHNT		100
#define ID_CM_DELETEEVENT		101
#define ID_CM_CHANGEINST		102
#define ID_CM_CHANGEDURATION	103
#define ID_CM_EXTRA_ITEMS		120

void CStaffView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu ConTexMenu;
	CRect rect;
	GetWindowRect(&rect);
	int Event = this->XtoEventIndex(point.x - rect.left);
	int id;
	CContextMenuList *pList=0;

	CMsEvent *pEv = m_pSong->GetEventObject(Event);

	ConTexMenu.CreatePopupMenu();
	ConTexMenu.AppendMenu(MF_STRING,ID_CM_INSERTEVEHNT,"Insert Event");
	if(pEv)ConTexMenu.AppendMenu(MF_STRING,ID_CM_DELETEEVENT,"Delete Event");
	if ((m_nDrawMode == DRAWMODE_MIDINOTEIN) && pEv)
	{
		ConTexMenu.AppendMenu(MF_STRING, ID_CM_CHANGEINST, "Change Instrument");
		ConTexMenu.AppendMenu(MF_STRING, ID_CM_CHANGEDURATION, "Change Note Duration");
	}
	ObjectTypes Obj;
	if (pEv)
	{
		int n = pEv->AreObjectsSelected();
		if (n)
		{
			int l;
			pList = new CContextMenuList(n);
			char *s = new char[256];
			Obj.pObj = pEv->GetFirstSelectedObject();
			for (int i = 0; i < n; ++i)
			{
				l = 0;
				switch (Obj.pObj->GetType())
				{
				case MSOBJ_BAR:
					break;
				case MSOBJ_END:
					break;
				case MSOBJ_KEYSIG:
					l = sprintf_s(s, 256, "Key Signature");
					ConTexMenu.AppendMenu(MF_STRING, ID_CM_EXTRA_ITEMS + i, CString(s));
					pList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case MSOBJ_LOUDNESS:
					l = sprintf_s(s, 256, "Loudness");
					ConTexMenu.AppendMenu(MF_STRING, ID_CM_EXTRA_ITEMS + i, CString(s));
					pList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case MSOBJ_NOTE:
					l = sprintf_s(s, 256, "Note:");
					Obj.pNote->NoteToString(&s[l], 256 - l);
					ConTexMenu.AppendMenu(MF_STRING, ID_CM_EXTRA_ITEMS + i, CString(s));
					pList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case MSOBJ_REPEATEND:
				case MSOBJ_REPEATSTART:
					sprintf_s(s, 256, "Repeat");
					ConTexMenu.AppendMenu(MF_STRING, ID_CM_EXTRA_ITEMS + i, CString(s));
					pList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case MSOBJ_TEMPO:
					sprintf_s(s, 256, "Tempo");
					ConTexMenu.AppendMenu(MF_STRING, ID_CM_EXTRA_ITEMS + i, CString(s));
					pList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				case MSOBJ_TIMESIG:
					sprintf_s(s, 256, "Time Signature");
					ConTexMenu.AppendMenu(MF_STRING, ID_CM_EXTRA_ITEMS + i, CString(s));
					pList->AddObject(Obj.pObj, ID_CM_EXTRA_ITEMS + i);
					break;
				}
				Obj.pObj = pEv->GetNextSelectedObject(Obj.pObj);
			}	//end of for loop
			delete[] s;
		}
	}
	id = ConTexMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,point.x,point.y, this);
	switch(id)
	{
		case ID_CM_INSERTEVEHNT:
			m_pSong->InsertEvent(Event);
			m_pSong->RenumberEvents(&m_FirstSelectedEvent,&m_LastSelectedEventIndex);
			Invalidate();
			break;
		case ID_CM_DELETEEVENT:
			m_pSong->RemoveEvent(Event);
			Invalidate();
			break;
		case ID_CM_CHANGEINST:
			{
				CSelectorDlg Dlg;
				CMSPlayerDlg *pMain = (CMSPlayerDlg *)GetParent();
				Dlg.m_pBitmaps = &pMain->m_BmInst1;
				Dlg.m_nBitmaps = 15;
				Dlg.m_nSelection = ((m_nMidiInputNoteSetup >> DRAW_NOTE_TRACK_SHIFT) & 0x0f)-1;
				Dlg.m_pCaptionString = "Select New Instrument";
				if (IDOK == Dlg.DoModal())
				{
					m_nMidiInputNoteSetup &= ~(0x0f << DRAW_NOTE_TRACK_SHIFT);
					m_nMidiInputNoteSetup |= (Dlg.m_nSelection+1) << DRAW_NOTE_TRACK_SHIFT;
					this->m_pPgmChng->PostMessageA(WM_PGMCHANGE,m_pSong->GetMidiInfo()->GetPatch(Dlg.m_nSelection + 1) ,Dlg.m_nSelection+1);
				}
			}
			break;
		case ID_CM_CHANGEDURATION:
			{
				CSelDurDlg Dlg;
				Dlg.m_pPrompt = "Select New Note Duration";
				Dlg.m_nDuration = m_nMidiInputNoteSetup & DRAW_NOTE_DURATION;
				Dlg.DoModal();
				m_nMidiInputNoteSetup &= ~DRAW_NOTE_DURATION;
				m_nMidiInputNoteSetup |= Dlg.m_nDuration;
			}
			break;
		default:
			if (pList)
			{
				int index = pList->FindID(id);
				if (index >= 0)
				{
					Obj.pObj = pList->GetObjectA(index);
					switch (Obj.pObj->GetType())
					{
						case MSOBJ_BAR:
						case MSOBJ_END:
							break;
						case MSOBJ_KEYSIG:
						{
							CSelectStringDlg Dlg;
							Dlg.m_pCaptionString = "Change Key Signature";
							Dlg.m_ppStrings = &KeySigStringTab[1];
							Dlg.m_nStrings = 16;
							Dlg.m_nSelection = Obj.pKey->GetKey() - 1;
							if (IDOK == Dlg.DoModal())
								Obj.pKey->SetKey(Dlg.m_nSelection);
						}
						break;
						case MSOBJ_LOUDNESS:
						{
							CParamDlg Dlg;

							Dlg.m_pCaption = "Change Loudness";
							Dlg.m_Val = Obj.pLoud->GetLoudness();
							Dlg.m_Max = 127;
							Dlg.m_Min = 1;
							if (IDOK == Dlg.DoModal())
							{
								Obj.pLoud->SetLoudness(Dlg.m_Val);
							}
						}
						break;
						case MSOBJ_NOTE:
						{
							CNotePropertiesDlg Dlg;
							Dlg.m_pNote = Obj.pNote;
							Dlg.DoModal();
						}
						break;
						case MSOBJ_REPEATEND:
						case MSOBJ_REPEATSTART:
							if (m_pRepeatEndSelected && m_pRepeatStartSelected)
							{
								CParamDlg Dlg;

								Dlg.m_pCaption = "Set Repeat Count";
								Dlg.m_Val = m_pRepeatStartSelected->GetCount();
								Dlg.m_Max = 100;
								Dlg.m_Min = 1;
								if (IDOK == Dlg.DoModal())
								{
									m_pRepeatStartSelected->SetCount(Dlg.m_Val);
								}
							}
							break;
						case MSOBJ_TEMPO:
						{
							CParamDlg Dlg;

							Dlg.m_pCaption = "Change Tempo";
							Dlg.m_Val = Obj.pTemp->m_Tempo;
							Dlg.m_Max = 230;
							Dlg.m_Min = 1;
							if (IDOK == Dlg.DoModal())
							{
								Obj.pTemp->m_Tempo = Dlg.m_Val;
							}
						}
						break;
						case MSOBJ_TIMESIG:
						{
							CSelectorDlg Dlg;
							Dlg.m_pCaptionString = "Change Time Signature";
							CMSPlayerDlg *pMain = (CMSPlayerDlg *)AfxGetApp()->m_pMainWnd;
							Dlg.m_pBitmaps = &pMain->m_BmTime22;
							Dlg.m_nBitmaps = 7;
							Dlg.m_nSelection = Obj.pTime->GetTimeSig() - 1;
							if (IDOK == Dlg.DoModal())
								Obj.pTime->SetTimeSig(Dlg.m_nSelection + 1);
						}
						break;
					}//end of switch(Obj.pObj->GetType()
				}//end of if(index >= 0)
			}//end of if(ppList)
			break;
	}//end of switch(id)
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

int CStaffView::YtoNote(int y)
{
	int r;
	int o = 0;
	//----------------------------
	// Notes place on staff will
	// start at HIGHC_OFFSET and
	// go to HIGHC_OFFSET- 8, so
	// we need to offset this by
	// 4
	//----------------------------
	y -= HIGHC_OFFSET-4;
	y = QuantizeY(y);
	y = 35- y;
	if(y < 0) y = 0;
	r = y % 7;
	int n = NoteLut[r];
	o += (y / 7) * 12;	/// calculate octave
	n = n + o+0x24;
	if(n > 0x60)
		n = 0x60;
	return n;
}

int CStaffView::XtoEventIndex(int x)
{
	int ev = x - EVENT_OFFSET;
	int r = ev % EVENT_WIDTH;
	ev /= (int)EVENT_WIDTH;
	if(ev < 0) ev = 0;
	else if (ev >= m_MaxEvents) ev = m_MaxEvents-1;
	return ev + m_SongScrollPos;
}

void CStaffView::CalcMaxEvents()
{
	CRect rect;

	GetClientRect(&rect);
	int Width = rect.Width();
	Width -= EVENT_OFFSET;
	m_MaxEvents = Width / EVENT_WIDTH;
}

int CStaffView::QuantizeY(int y)
{
	return y / 4;
}

LRESULT CStaffView::OnDrawMessage(WPARAM wp, LPARAM lp)
{
	char *s = new char[256];
	int From,To;
	//------------------------
	// Decode sub message
	//------------------------
	switch(wp)
	{
		case DRAW_NOP:
			if(m_pDrawObject) delete m_pDrawObject;
			m_pDrawObject = 0;
			this->m_nDrawMode = DRAWMODE_NOP;
			this->m_pStatus->SetText("");
			break;
		case DRAW_NOTE:
			{
				CMsNote *pN = new CMsNote;
				if(m_pDrawObject)
				{
					delete m_pDrawObject;
				}
				m_pDrawObject = pN;
				m_nDrawMode = DRAWMODE_NOTE;
				//-----------------------------
				// decode message word
				//------------------------------
				pN->m_pMidiInfo = m_pSong->GetMidiInfo();
				pN->m_hMidiOut = m_pSong->m_hMidiOut;
				pN->SetDuration(lp & DRAW_NOTE_DURATION);
				pN->SetAccent((lp >> DRAW_NOTE_ACCENT_SHIFT) & 0x01);
				pN->SetKey((lp >> DRAW_NOTE_KEY_SHIFT) & 0x03);
				pN->SetTrack((lp >> DRAW_NOTE_TRACK_SHIFT) & 0x0f);
				pN->SetRest((lp >> DRAW_NOTE_REST_SHIFT) & 1);
				sprintf_s(s,256,"Draw Note Message");
				this->m_pDisp->SetWindowText(s);
			}
			m_pStatus->SetText("Draw Note");
			break;
		case DRAW_REST:
			break;
		case DRAW_BAR:
			{
				CMsBar *pB = new CMsBar;
				m_nDrawMode = DRAWMODE_BAR;
				m_pDrawObject = pB;
			}
			this->m_pStatus->SetText("Draw Measure Bar");
			break;
		case DRAW_TIE:
			if(m_pDrawObject) delete m_pDrawObject;
			m_pDrawObject = 0;
			m_nDrawMode = DRAWMODE_TIE;
			m_nDrawState = DRAWSTATE_TIE_FIRSTNOTE;
			m_pFirstTieNote = 0;
			m_pSecondTieNote = 0;
			this->m_pStatus->SetText("Draw Tie");
			break;
		case DRAW_COPY:
			m_nDrawMode = DRAWMODE_COPY;
			this->m_pStatus->SetText("Copy Block");
			break;
		case DRAW_MOVE:
			m_nDrawMode = DRAWMODE_MOVE;
			this->m_pStatus->SetText("Move Block");
			break;
		case DRAW_REPEAT:
			AddRepeat(lp);
			SetScrollRange(SB_HORZ,0,m_pSong->m_nTotalEvents);
			Invalidate();
			this->m_pStatus->SetText("Add Repeat");
			break;
		case DRAW_TEMPO:
			{
				CMsTempo *pMT = new CMsTempo(lp);
				m_nDrawMode = DRAWMODE_TEMPO;
				m_pDrawObject = pMT;
			}
			this->m_pStatus->SetText("Add Tempo");
			break;
		case DRAW_TIMESIG:
			{
				CMsTimeSig *pTS = new CMsTimeSig;
				pTS->SetTimeSig(lp);
				m_nDrawMode = DRAWMODE_TIMESIG;
				m_pDrawObject = pTS;
			}
			this->m_pStatus->SetText("Add Time Signature");
			break;
		case DRAW_KEYSIG:
			{
				CMsKeySignature *pKS = new CMsKeySignature;
				pKS->SetKey(lp);
				m_nDrawMode = DRAWMODE_KKEYSIG;
				m_pDrawObject = pKS;
			}
			this->m_pStatus->SetText("Add Key Signature");
			break;
		case DRAW_LOUDNESS:
			{
				CMsLoudness *pLD = new CMsLoudness;
				pLD->SetLoudness(lp);
				m_nDrawMode = DRAWMODE_LOUDNESS;
				m_pDrawObject = pLD;
			}
			this->m_pStatus->SetText("Add Loudness");
			break;
		case DRAW_INSTCHANGE:
			From = lp & 0xf;
			To = (lp >> 4) & 0x0f;
			ChangeInst(From,To);
			Invalidate();
			this->m_pStatus->SetText("Change Instrument in Selection");
			break;
		case DRAW_CHANGEDUR:
			From = lp & 0x1f;
			To = (lp >> 5) & 0x1f;
			ChangeDuration(From,To);
			Invalidate();
			this->m_pStatus->SetText("Change Duration in Selection");
			break;
		case DRAW_INCREASEDUR:
			IncreaseDuration();
			this->m_pStatus->SetText("Increase Duration in Selection");
			break;
		case DRAW_DECREASEDUR:
			this->m_pStatus->SetText("Decrease Duration in Selection");
			break;
		case DRAW_INCRPITCH:
			IncrPitch();
			this->m_pStatus->SetText("Increase Pitch In Selection");
			Invalidate();
			break;
		case DRAW_DECRPITCH:
			DecrPitch();
			this->m_pStatus->SetText("Decrease Pitch in Selection");
			Invalidate();
			break;
		case DRAW_INSERTBLOCK:
			InsertBlock();
			SetScrollRange(SB_HORZ,0,m_pSong->m_nTotalEvents);
			this->m_pStatus->SetText("Insert Event");
			Invalidate();
			break;
		case DRAW_DRAWNOTESVIAMIDI:
			m_nDrawMode = DRAWMODE_MIDINOTEIN;
			m_nMidiInputNoteSetup = lp;
			m_pStatus->SetText("Step Input Notes Via MIDI");
			break;
	}
	delete [] s;
	return 0;
}

int CStaffView::MouseInRegion(CPoint p)
{
	int rV =MOUSE_OUTSIDE;
	CRect EditRect,rect,UpperSelRect,LowerSelRect;

	GetClientRect(&rect);
	EditRect.SetRect(rect.left+EVENT_OFFSET,rect.top+HIGHC_OFFSET-4,rect.right,rect.top + 162);
	UpperSelRect.SetRect(rect.left+EVENT_OFFSET,rect.top,rect.right,rect.top+HIGHC_OFFSET-3);
	LowerSelRect.SetRect(rect.left+EVENT_OFFSET,rect.top + 162,rect.right,rect.bottom);
	if(EditRect.PtInRect(p)) rV = MOUSE_INEDITREG;
	else if(UpperSelRect.PtInRect(p)) rV = MOUSE_INUPPERSEL;
	else if(LowerSelRect.PtInRect(p)) rV = MOUSE_INLOWERSEL;
	return rV;
}

void CStaffView::IncreaseDuration()
{
}

void CStaffView::IncrPitch()
{
	if((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox("No Block Selected","Oppsie");
	else
	{
		CMsEvent *pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
		for(int i=m_FirstSelectedEvent;i<m_LastSelectedEventIndex+1;++i)
		{
			CMsObject *pOb = pEv->GetHead();
			while(pOb)
			{
				if(MSOBJ_NOTE == pOb->GetType())
				{
					CMsNote *pN = (CMsNote *)pOb;
					pN->IncrNote();
				}
				pOb = pOb->GetNext();
			}
			pEv = pEv->GetNext();
		}
	}
}

void CStaffView::DecrPitch()
{
	if((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox("No Block Selected","Oppsie");
	else
	{
		CMsEvent *pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
		for(int i=m_FirstSelectedEvent;i<m_LastSelectedEventIndex+1;++i)
		{
			CMsObject *pOb = pEv->GetHead();
			while(pOb)
			{
				if(MSOBJ_NOTE == pOb->GetType())
				{
					CMsNote *pN = (CMsNote *)pOb;
					pN->DecrNote();
				}
				pOb = pOb->GetNext();
			}
			pEv = pEv->GetNext();
		}
	}
}

void CStaffView::InsertBlock()
{
	int i,n;
	n = m_LastSelectedEventIndex-m_FirstSelectedEvent+1;
	for(i=0;i<n;++i)
		m_pSong->InsertEvent(m_FirstSelectedEvent);
	m_pSong->RenumberEvents(&m_FirstSelectedEvent,&m_LastSelectedEventIndex);
}

void CStaffView::AddRepeat(int n)
{
	if(this->m_FirstSelectedEvent < 0 || this->m_LastSelectedEventIndex < 0)
		MessageBox("Block Not Selected","Oopsie");
	else
	{
		///--------------------------------
		/// We add the repeat by adding
		/// a new event before and after
		/// the selected block.
		///---------------------------------
		CMsEvent *pRS,*pRE;
		CMsRepeatStart *pRepStrt;
		CMsRepeatEnd *pRepEnd;

		pRS = m_pSong->InsertEvent(m_FirstSelectedEvent-1);
		m_pSong->RenumberEvents(&m_FirstSelectedEvent,&m_LastSelectedEventIndex);
		pRE = m_pSong->InsertEvent(m_LastSelectedEventIndex);
		m_pSong->RenumberEvents(&m_FirstSelectedEvent,&m_LastSelectedEventIndex);
		pRepStrt = new CMsRepeatStart(n);
		pRepEnd = new CMsRepeatEnd;
		pRS->AddObjectAtStart(pRepStrt);
		pRE->AddObjectAtStart(pRepEnd);
	}
}

void CStaffView::ChangeInst(int From, int To)
{
	if((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox("No Block Selected","Oppsie");
	else
	{
		CMsEvent *pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
		for(int i=m_FirstSelectedEvent;i<m_LastSelectedEventIndex+1;++i)
		{
			CMsObject *pOb = pEv->GetHead();
			while(pOb)
			{
				if(MSOBJ_NOTE == pOb->GetType())
				{
					CMsNote *pN = (CMsNote *)pOb;
					if(pN->GetTrack() == (unsigned)From)
						pN->SetTrack(To);
				}
				pOb = pOb->GetNext();
			}
			pEv = pEv->GetNext();
		}
	}
}

void CStaffView::ChangeDuration(int From, int To)
{
	if((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox("No Block Selected","Oppsie");
	else
	{
		CMsEvent *pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
		for(int i=m_FirstSelectedEvent;i<m_LastSelectedEventIndex+1;++i)
		{
			CMsObject *pOb = pEv->GetHead();
			while(pOb)
			{
				if(MSOBJ_NOTE == pOb->GetType())
				{
					CMsNote *pN = (CMsNote *)pOb;
					if(pN->GetDuration() == From)
						pN->SetDuration(To);
				}
				pOb = pOb->GetNext();
			}
			pEv = pEv->GetNext();
		}
	}
}

void CStaffView::CopyBlock(int dest)
{
	//-------------------------------
	// first thing, copy the block
	//-------------------------------
	CMsEvent *pHead=0,*pTail=0;
	CMsEvent *pEv,*pEvNew;

	if((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox("No Block Selected","Oppsie");
	else
	{
		pEv = m_pSong->GetEventObject(m_FirstSelectedEvent);
		if (!pEv)
		{
			//-------------------------
			// OK, we have selected an
			// event that does not exist
			// so we need to add events
			// until we get up to where
			// we need to be
			//--------------------------------
//			do
//			{
//
//			}while(pEv->m_Index < 
		}
		while(pEv->m_Index != m_LastSelectedEventIndex+1)
		{
			CMsObject *pOb;
			pEvNew = new CMsEvent;
			pOb = pEv->GetHead();
			while(pOb)
			{
				CMsObject *pNewObj;
				pNewObj = pOb->Copy();
				pNewObj->SetNext(0);
				pNewObj->SetPrev(0);
				pEvNew->AddObjectAtEnd(pNewObj);
				pOb = pOb->GetNext();
			}
			if(pHead == 0)
			{
				pHead = pEvNew;
				pTail = pEvNew;
			}
			else
			{
				pTail->SetNext(pEvNew);
				pEvNew->SetPrev(pTail);
				pTail = pEvNew;
			}
			pEv = pEv->GetNext();
		}
		///--------------------------------
		/// Chain is created.  Now, insert
		/// into the song at the desired
		/// spot
		///--------------------------------
		CMsEvent *pDest = m_pSong->GetEventObject(dest);
		pTail->SetNext(pDest->GetNext());
		pDest->SetNext(pHead);
		pHead->SetPrev(pDest->GetPrev());
		if(pTail->GetNext())
			pTail->GetNext()->SetPrev(pTail);
	}
	m_pSong->RenumberEvents(&m_FirstSelectedEvent,&m_LastSelectedEventIndex);
}

void CStaffView::MoveBlock(int dest)
{
	CMsEvent *pHead=0,*pTail=0,*pDest=0;
	CMsEvent *pEv;

	///--------------------------
	/// snip out block
	///----------------------------
	if((m_LastSelectedEventIndex < 0) || (m_FirstSelectedEvent < 0))
		MessageBox("No Block Selected","Oppsie");
	else
	{
		pDest = m_pSong->GetEventObject(dest);
		pHead = m_pSong->GetEventObject(m_FirstSelectedEvent);
		pTail = m_pSong->GetEventObject(m_LastSelectedEventIndex);
		pEv = pHead->GetPrev();
		pEv->SetNext(pTail->GetNext());
		pEv->GetNext()->SetPrev(pEv);
		pHead->SetPrev(0);
		pTail->SetNext(0);
		///--------------------------------
		/// Chain is created.  Now, insert
		/// into the song at the desired
		/// spot
		///--------------------------------
		pTail->SetNext(pDest->GetNext());
		pDest->SetNext(pHead);
		pHead->SetPrev(pDest->GetPrev());
		if(pTail->GetNext())
			pTail->GetNext()->SetPrev(pTail);
		m_pSong->RenumberEvents(&m_FirstSelectedEvent,&m_LastSelectedEventIndex);
		pEv = pHead;
		while(pEv != pTail)
		{
			pEv->SetSelected(0);
			pEv = pEv->GetNext();
		}
		pEv->SetSelected(0);
		m_FirstSelectedEvent = -1;
		m_LastSelectedEventIndex = -1;
	}
}

LRESULT CStaffView::OnInitStaff(WPARAM wp, LPARAM lp)
{
	return 0;
}

void CStaffView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
//	m_SongScrollPos = pScrollBar->GetScrollPos();
	switch(nSBCode)
	{
		case SB_LINELEFT:
			m_SongScrollPos--;
			if(m_SongScrollPos < 0) m_SongScrollPos = 0;
			SetScrollPos(SB_HORZ,m_SongScrollPos);
			break;
		case SB_LINERIGHT:
			m_SongScrollPos++;
			if(m_SongScrollPos > m_pSong->m_nTotalEvents) m_SongScrollPos =  m_pSong->m_nTotalEvents;
			SetScrollPos(SB_HORZ,m_SongScrollPos);
			break;
		case SB_PAGELEFT:
			m_SongScrollPos -=10;
			if(m_SongScrollPos > m_pSong->m_nTotalEvents) m_SongScrollPos =  m_pSong->m_nTotalEvents;
			SetScrollPos(SB_HORZ,m_SongScrollPos);
			break;
		case SB_PAGERIGHT:
			m_SongScrollPos += 10;
			if(m_SongScrollPos > m_pSong->m_nTotalEvents) m_SongScrollPos =  m_pSong->m_nTotalEvents;
			SetScrollPos(SB_HORZ,m_SongScrollPos);
			break;
		case SB_THUMBTRACK:
			m_SongScrollPos = nPos;
			SetScrollPos(SB_HORZ,m_SongScrollPos);
			break;
	}
	Invalidate();
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CStaffView::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CView::PreSubclassWindow();
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

afx_msg LRESULT CStaffView::OnShortmidimsg(WPARAM wMsg, LPARAM timestamp)
{
	int Cmd;
	int Vel;
	int Note;
	int Chan;
	int Instrument;
	int UpdateNoteInfoFlag = 0;
	int RestFlag = 0;
	int v;

	Cmd = wMsg & 0xf0;
	Chan = wMsg & 0x0f;
	Note = (wMsg & 0x0ff00) >> 8;
	Vel = (wMsg & 0x0ff0000) >> 16;
	switch (Cmd)
	{
	case MIDI_NOTEOFF:
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
				int note, adj;
				note = Note % 12;
				adj = NoteAdjustLUT[note];
				note -= adj;
				note += 12 * (Note / 12);
				CMsEvent *pEv = m_pSong->GetEventObject(m_LastSelectedEventIndex);
				CMsNote *pN = pEv->FindNote(note, adj ? ACCIDENT_SHARP : ACCIDENT_INKEY);
				if (pN) pN->NoteOff(0, this->m_pKeySig);
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
						pEv = new CMsEvent();
						m_pSong->AddEventAtEnd(pEv);
						pEv->SetSelected(1);
						m_LastSelectedEventIndex++;
					}
					if ((v = IsEventDisplayed(pEv)) != 0)
					{
						m_SongScrollPos += v+1;
						UpdateScrollbarInfo(m_pSong->m_nTotalEvents);
						Invalidate();
					}
				}
			}
		}
		else if (Chan == BUTTON_MIDI_CHANNEL)
		{
			switch (Note)
			{
			case BUTTON_UPPER_1:
				break;
			case BUTTON_UPPER_ACCENT:
				break;
			case BUTTON_UPPER_3:
				break;
			case BUTTON_UPPER_4:
				break;
			case BUTTON_UPPER_5:
				break;
			case BUTTON_UPPER_6:
				break;
			case BUTTON_UPPER_7:
				break;
			case BUTTON_UPPER_8:
				break;
			case BUTTON_LOWER_1:
				break;
			case BUTTON_LOWER_2:
				break;
			case BUTTON_LOWER_3:
				break;
			case BUTTON_LOWER_4:
				break;
			case BUTTON_LOWER_5:
				break;
			case BUTTON_LOWER_6:
				break;
			case BUTTON_LOWER_7:
				break;
			case BUTTON_LOWER_8:
				break;
			}
		}
		Invalidate();
		break;
	case MIDI_NOTEON:
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
					m_nMidiNotesOn--;
					int note, adj;
					note = Note % 12;
					adj = NoteAdjustLUT[note];
					note -= adj;
					note += 12 * (Note / 12);
					CMsEvent *pEv = m_pSong->GetEventObject(m_LastSelectedEventIndex);
					CMsNote *pN = pEv->FindNote(note, adj ? ACCIDENT_SHARP : ACCIDENT_INKEY);
					if (pN) pN->NoteOff(0, this->m_pKeySig);
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
				int note, adj;
				note = Note % 12;
				adj = NoteAdjustLUT[note];
				note -= adj;
				note += 12 * (Note / 12);
				CMsEvent *pEv = m_pSong->GetEventObject(m_LastSelectedEventIndex);
				m_nMidiNotesOn++;
				CMsNote *pN = new CMsNote(pEv);
				pN->m_pMidiInfo = m_pSong->GetMidiInfo();
				pN->m_hMidiOut = m_pSong->m_hMidiOut;
				pN->SetDuration(m_nMidiInputNoteSetup & DRAW_NOTE_DURATION);
				pN->SetAccent((m_nMidiInputNoteSetup >> DRAW_NOTE_ACCENT_SHIFT) & 0x01);
				pN->SetKey(adj ? ACCIDENT_SHARP : ACCIDENT_INKEY);
				pN->SetTrack((m_nMidiInputNoteSetup >> DRAW_NOTE_TRACK_SHIFT) & 0x0f);
				pN->SetRest((m_nMidiInputNoteSetup >> DRAW_NOTE_REST_SHIFT) & 1);
				pN->SetPitch(note);
				CMsObject *pO;
				if ((pO = pEv->ObjectAlreadyHere(pN)) == NULL)
				{
					pN->NoteOn(Vel, this->m_pKeySig);
					pEv->AddObjectAtEnd(pN);

				}
				else
					pEv->RemoveObject(pO);
				CMsObject *pObj;
				char *s = new char[256];
				pObj = pEv->GetHead();
				int l = sprintf_s(s, 256, "%s", "Draw Note(s) :");
				int count = 0;
				while (pObj)
				{
					if (MSOBJ_NOTE == pObj->GetType())
					{
						CMsNote *pNote = (CMsNote *)pObj;
						if (!pNote->IsRest())
						{
							if (count)
								l += sprintf_s(&s[l], 256 - l, ", ");
							l += pNote->NoteToString(&s[l], 256 - l);
							++count;
						}
					}
					pObj = pObj->GetNext();
				}//end of while(pObj)
				if (count)
				{
					sprintf_s(&s[l], 256 - l, " On Instrument %d", (m_nMidiInputNoteSetup >> DRAW_NOTE_TRACK_SHIFT) & 0x0f);
					this->m_pStatus->SetText(s);
				}
				delete[]s;
				Invalidate();
			}
		}	//end of if chan==0
		else if (Chan == BUTTON_MIDI_CHANNEL)	//button channel NOTEON
		{
			int check;

			switch (Note)
			{
				case BUTTON_UPPER_1:
					break;
				case BUTTON_UPPER_ACCENT:
					//toggle checkbox
					check = ((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->m_Combo_Decorations.GetCheck(0) ^ 1;
					((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->m_Combo_Decorations.SetCheck(0, check);
					((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->m_Combo_Decorations.Invalidate();
					UpdateNoteInfo(0);
					break;
				case BUTTON_UPPER_3:
					break;
				case BUTTON_UPPER_4:
					break;
				case BUTTON_UPPER_5:
					break;
				case BUTTON_UPPER_6:
					break;
				case BUTTON_UPPER_7:
					break;
				case BUTTON_UPPER_8:
					break;
				case BUTTON_LOWER_1:
					break;
				case BUTTON_LOWER_2:
					break;
				case BUTTON_LOWER_3:
					break;
				case BUTTON_LOWER_4:
					break;
				case BUTTON_LOWER_5:
					break;
				case BUTTON_LOWER_6:
					break;
				case BUTTON_LOWER_7:
					break;
				case BUTTON_LOWER_8:
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
					CMsEvent *pEv;
					CMsObject *pOb;

					pEv = m_pSong->GetEventObject(m_LastSelectedEventIndex);
					if (pEv)
					{
						CMsObject *pObT;
						pOb = pEv->GetHead();
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
				((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->m_Combo_Instrument.SetCurSel(Instrument);
				UpdateNoteInfoFlag = 1;
				break;
			case MIDI_CTRLCHG_DECORATION:
				((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->m_Combo_Decorations.SetCurSel(1 + Vel / 43);
				UpdateNoteInfoFlag = 1;
				break;
			case MIDI_CTRLCHNG_KNOB_NOTE_DUR:
				((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->m_Combo_NoteType.SetCurSel(Vel/21);
				UpdateNoteInfoFlag = 1;
				break;
			case MIDI_CTRLCHNG_KNOB_REST_DUR:
				((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->m_Combo_Rests.SetCurSel(Vel / 21);
				UpdateNoteInfoFlag = 1;
				RestFlag = 1;
				break;
			case MIDI_CTRLCHNG_ACCIDENTAL:
				((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->m_Combo_Accident.SetCurSel(Vel / 42);
				UpdateNoteInfoFlag = 1;
				break;
			case MIDI_CTRLCHNG_KEY:
				((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->m_Combo_KeySig.SetCurSel(Vel / 8);
				UpdateNoteInfoFlag = 1;
				break;
			case MIDI_CTRLCHNG_REWIND:	//move selection back
				if ((m_LastSelectedEventIndex >= 0) && (Vel > 0))
				{
					CMsEvent *pEV = m_pSong->GetEventObject(m_LastSelectedEventIndex);
					pEV->SetSelected(0);
					pEV = pEV->GetPrev();
					m_LastSelectedEventIndex--;
					if (pEV)
					{
						pEV->SetSelected(1);
						m_SongScrollPos += IsEventDisplayed(pEV);
					}
					UpdateScrollbarInfo(m_pSong->m_nTotalEvents);
					Invalidate();
				}
				break;
			case MIDI_CTRLCHNG_FASTFORWARD:	//move selection forward
				if ((m_LastSelectedEventIndex >= 0) && (Vel > 0))
				{
					CMsEvent *pEV = m_pSong->GetEventObject(m_LastSelectedEventIndex);
					pEV->SetSelected(0);
					pEV = pEV->GetNext();
					if (pEV)
					{
						pEV->SetSelected(1);
						m_LastSelectedEventIndex++;
					}
					else
					{
						pEV = new CMsEvent;
						pEV->SetSelected(1);
						m_pSong->AddEventAtEnd(pEV);
						m_LastSelectedEventIndex++;
						m_pSong->RenumberEvents(&m_FirstSelectedEvent,& m_LastSelectedEventIndex);
					}
					if ((v = IsEventDisplayed(pEV))!=0)
					{
						if (v < 4)
							m_SongScrollPos += 4;
						else
							m_SongScrollPos += v;
					}
					UpdateScrollbarInfo(m_pSong->m_nTotalEvents);
					Invalidate();
				}
				break;
			case MIDI_CTRLCHNG_STOP:
				if (Vel > 0)((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->OnButtonStop();
				break;
			case MIDI_CTRLCHNG_PLAY:
				if(Vel > 0 && !m_pSong->m_nIsPlaying)((CMSPlayerDlg *)(AfxGetApp()->m_pMainWnd))->OnButtonPlay();
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

afx_msg LRESULT CStaffView::OnStaffDispEvent(WPARAM Event, LPARAM cmd)
{
	switch (cmd)
	{
		case STAFF_DISP_EVENT_NEXT:
			{
				CMsEvent *pEv = m_pSong->GetEventObject(m_nLastSongPosition);
				if (pEv)pEv->SetSelected(0);
				pEv = m_pSong->GetEventObject(Event);
				if (pEv)pEv->SetSelected(1);
				//----------------------------------
				// we need to figure out where we are
				// and where we want to go.
				//----------------------------------
				int CurrentStartPos = this->m_SongScrollPos;
				int DispEvents = this->m_MaxEvents;
				if (Event < CurrentStartPos)
					m_SongScrollPos = Event;
				else if (Event >= (CurrentStartPos + DispEvents))
					m_SongScrollPos = Event;
				UpdateScrollbarInfo(m_pSong->m_nTotalEvents);
				m_nLastSongPosition = Event;
			}
			break;
		case STAFF_DISP_EVENT_END:
			{
				CMsEvent *pEv = m_pSong->GetEventObject(m_nLastSongPosition);
				if (pEv)pEv->SetSelected(0);
				m_nLastSongPosition = 0;
				m_SongScrollPos = 0;
				UpdateScrollbarInfo(m_pSong->m_nTotalEvents);
			}
			break;
	}
	Invalidate();
	return 0;
}


void CStaffView::UpdateNoteInfo(int RestFlag)
{
	CMsNote *pN;

	if ((m_nDrawMode != DRAWMODE_NOTE) && (m_nDrawMode != DRAWMODE_MIDINOTEIN))
	{
		if (m_pDrawObject) delete m_pDrawObject;
		m_pDrawObject = new CMsNote;
		m_nDrawMode = DRAWMODE_NOTE;
	}
	if (m_pDrawObject == 0) m_pDrawObject = new CMsNote;
	pN = (CMsNote *)m_pDrawObject;
	((CMSPlayerDlg *)this->GetParent())->UpdateComboBoxes();
	m_nMidiInputNoteSetup = ((CMSPlayerDlg *)this->GetParent())->CreateNoteMessageWord();

	pN->m_pMidiInfo = m_pSong->GetMidiInfo();
	pN->m_hMidiOut = m_pSong->m_hMidiOut;
	int Dur = m_nMidiInputNoteSetup & DRAW_NOTE_DURATION;
	Dur = NoteDurLut[DurTab[Dur].NoteShapIndex];
	pN->SetDuration(Dur);
	pN->SetAccent((m_nMidiInputNoteSetup >> DRAW_NOTE_ACCENT_SHIFT) & 0x01);
	pN->SetKey((m_nMidiInputNoteSetup >> DRAW_NOTE_KEY_SHIFT) & 0x03);
	pN->SetTrack((m_nMidiInputNoteSetup >> DRAW_NOTE_TRACK_SHIFT) & 0x0f);
	pN->SetRest(RestFlag);
}


int CStaffView::IsEventDisplayed(CMsEvent * pEV)
{
	int rV = 0;
	int Event = pEV->m_Index;
	if (Event > (m_MaxEvents + m_SongScrollPos-1))
		rV =  Event - (m_MaxEvents + m_SongScrollPos -1);
	else if (Event < m_SongScrollPos)
		rV = -1;
	return rV;
}

void CStaffView::UpdateScrollbarInfo(int TotalEvents)
{
	CRect clientrect;
	GetClientRect(&clientrect);
	SCROLLINFO si;
	int cx = clientrect.Width();
	int HScrollMax =  TotalEvents;

	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = HScrollMax;
	si.nPos = m_SongScrollPos;
	si.nPage = m_MaxEvents;
	SetScrollInfo(SB_HORZ, &si, TRUE);
}


int CStaffView::MouseOverObject(int nEvent, int nObjectType, CMsObject ** ppObjectSelected)
{
	CMsObject *pObj;
	int flag;

	pObj = m_pSong->IsObjectInEvent(nEvent, nObjectType, flag);
	if (pObj)
	{
		if (*ppObjectSelected && pObj != *ppObjectSelected)
		{
			(*ppObjectSelected)->SetSelected(0);
			*ppObjectSelected = pObj;
			(*ppObjectSelected)->SetSelected(1);
			Invalidate();
		}
		else if (!*ppObjectSelected)
		{
			pObj->SetSelected(1);
			*ppObjectSelected = pObj;
			Invalidate();
		}
	}
	else if (*ppObjectSelected)
	{
		(*ppObjectSelected)->SetSelected(0);
		*ppObjectSelected = 0;
		Invalidate();
	}
	return flag;
}
