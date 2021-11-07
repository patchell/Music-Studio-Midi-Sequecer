
// CChildViewStaff.h : interface of the CChildViewStaff class
//
#pragma once

constexpr auto STAFFVIEW_MIDI_IN_DISPATCH_NUMENTRIES = 4;
constexpr auto INVALID_PITCH = -1;

// Regions that the mouse can be in
enum {
	MOUSE_OUTSIDE,
	MOUSE_INEDITREG,
	MOUSE_INUPPERSEL,
	MOUSE_INLOWERSEL
};

// different draw mode
enum {
	DRAWMODE_NOP,
	DRAWMODE_NOTE,
	DRAWMODE_BAR,
	DRAWMODE_ENDBAR,
	DRAWMODE_KKEYSIG,
	DRAWMODE_TIMESIG,
	DRAWMODE_TIE,
	DRAWMODE_LOUDNESS,
	DRAWMODE_TEMPO,
	DRAWMODE_COPY,
	DRAWMODE_MOVE,
	DRAWMODE_MIDINOTEIN
};

// State for drawing automiton
enum {
	DRAWSTATE_TIE_FIRSTNOTE,
	DRAWSTATE_TIE_SECONDNOTE
};

enum {
	STAFFVIEW_MOUSEUP,
	STAFFVIEW_MOUSEDOWN
};

// CChildViewStaff window

class CChildViewStaff : public CChildViewBase
{
	BOOL m_EscapeFlag;
	UINT m_TimerID;
	UINT m_DragFlag;
	UINT m_AltKeyDown;
	UINT m_CtrlKeyDown;
	UINT m_ShiftKeyDown;

	CRect m_UpperSelRect;
	CRect m_LowerSelRect;
	//---------- View Colors -------------
	COLORREF m_color_BackGround;

	//---------- Combo Boxes --------------
	CComboDropDown m_Combo_BlockOps;
	CComboDropDown m_Combo_Instrument;
	CComboDropDown m_Combo_TimeSig;
	CComboDropDown m_Combo_Rests;
	CComboDropDown m_Combo_Accidentals;
	CComboDropDownToggle m_Combo_Decorations;
	CComboDropUp m_Combo_Misc;
	CComboDropDown m_Combo_NoteType;
	CComboDropDown m_Combo_KeySig;
	//--------- View Controls ------------------
	CMyButton m_Button_Play;
	CMyButton m_Button_Stop;
	CStaticStatus m_Status;
	//------- Song Position ----------------
	int m_nLastSongPosition;
	int m_SongScrollPos;	//Event that staff
							//display starts at
	int m_FirstSelectedEvent;
	int m_LastSelectedEventIndex;
	//------- Tie data -------------------
	CPoint m_TieEndPoint;
	CPoint m_TieStartPoint;
	CMsNote* m_pSecondTieNote;
	CMsNote* m_pFirstTieNote;
	//-------------------------------------
	int m_nDrawState;
	int m_ExitEditRegion;
	INT m_LastPitch;
	CMsNote* pLastNote;
	int m_nDrawEvent;
	int m_nRawEvent;
	CMsObject* m_pDrawObject;
	int m_nDrawMode;
	int m_MouseInEditRegion;
	int m_MaxEvents;		//maximum number of events that can be displayed
	CMsSong* m_pSong;
	// Keeps track of the number of notes pressed down for an event in Midi Input Mode
	int m_nMidiNotesOn;
	int m_nMidiInputNoteSetup;
	// Pointer to window that has the midi program change message handler
	CWnd* m_pPgmChng;
	// Last note that was hovered over
	//-----------------------------------
	CMsRepeatStart* m_pRepeatStartSelected;
	CMsRepeatEnd* m_pRepeatEndSelected;
//	CMsKeySignature* m_pKeySig;
//-------------- Mouse ---------------------
	int m_nMouseState;
//--------- View Regions -------------------
	CMyRgn m_rgnUpper;
	CMyRgn m_rgnEdit;
	CMyRgn m_rgnLower;

	int m_SelectRectTop;
	CSize m_szSelectRect;

	//----------------Note Object Data  --------------
	NoteData m_CurrentNoteData;
	//--------- Object Highlight -----------------=
	CMsObject* m_pHighLightedObject;
	//------Selected Object List ----------------
	CMsObject* m_pSelectedObjectHead;
	CMsObject* m_pSelectedObjectTail;
	//------------ Draw Ready Flag --------------
	int m_ReadyToDraw;
	// Implementation
public:
	CChildViewStaff();
	virtual ~CChildViewStaff();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	int SetScrollPos(int pos, const char* Title = 0);
	void SetScrollRange(int Range, const char* Title = 0);
	void UpdateComboBoxes();
	void UpdateNoteDrawObject();
	void UpdateColors();
	CStaticStatus* GetStatusBar() { return &m_Status; }
	//------------ Draw Screen ----------------------
	BOOL IsMouseInEditRegion() { return m_MouseInEditRegion; }
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnDraw(CDC* pDC);
	void DrawControls(CDC* pDC);
	//---- Get/Set Data Member Access Methods -----
	NoteData& GetNoteData() { return m_CurrentNoteData; }
	INT GetLastPitch() { return m_LastPitch; }
	BOOL LastPitchIsValid() {
		BOOL rv = FALSE;
		if (m_LastPitch >= 0)
		{
			rv = TRUE;
		}
		return rv;
	}
	void SetLastPitch(INT lastPitch) { m_LastPitch = lastPitch; }
	int GetSongScrollPosition() { return m_SongScrollPos; }
	void SetSongScrollPosition(int SSP) { m_SongScrollPos = SSP; }
	int GetPitch() { return GetNoteData().GetPitch(); }
	void SetPitch(int p) { GetNoteData().SetPitch(p); }
	int GetDuration() { return GetNoteData().GetDuration(); }
	void SetDuration(int nd) { GetNoteData().SetDuration(nd); }
	int GetTrack() { return GetNoteData().GetTrack(); }
	void SetTrack(int T) { GetNoteData().SetTrack(T); }
	int GetAccent() { return GetNoteData().GetAccent(); }
	void SetAccent(int A) { GetNoteData().SetAccent(A); }
	int GetDotted() { return GetNoteData().GetDotted(); }
	void SetDotted(UINT D) { GetNoteData().SetDotted(D); }
	UINT GetTriplet() { return GetNoteData().GetTriplet(); }
	void SetTriplet(UINT nT) { GetNoteData().SetTriplet(nT); }
	UINT GetRest() { return GetNoteData().GetRest(); }
	void SetRest(UINT NR) { GetNoteData().SetRest(NR); }
	UINT GetAccidental() { return GetNoteData().GetAccidental(); }
	void SetAccidental(UINT NA) { GetNoteData().SetAccidental(NA); }
	int GetUpSideDown() { return GetNoteData().GetUpsideDown(); }
	void SetUpSideDown(int USD) { GetNoteData().SetUpsideDown(USD); }
	UINT GetHeadFlipped() { return GetNoteData().GetHeadFlipped(); }
	void SetHeadFlipped(UINT flipped) { GetNoteData().SetHeadFlipped(flipped); }
	UINT GetDrawEvent() { return m_nDrawEvent; }
	void IncDrawEvent() {
		m_nDrawEvent++;
	}
	UINT GetDrawEventAndInc() {
		UINT DrawEvent = GetDrawEvent();
		IncDrawEvent();
		return DrawEvent;
	}
	void SetDrawEvent(UINT drawEvent) {	m_nDrawEvent = drawEvent; }
	//---------------------------------------------------
	int GetMaxEvents() {return m_MaxEvents;}
	void SetMaxEvents(int mE) { m_MaxEvents = mE; }
	int GetRawEvent() { return m_nRawEvent; }
	inline CMsSong* GetSong() { return m_pSong; }
	inline void SetSong(CMsSong* pMS) { m_pSong = pMS; }
	// Operations
	void CheckAndDoScroll(CPoint point) 
	{
		if (GetRawEventNumber(point.x) == m_MaxEvents)
		{
			m_SongScrollPos += 4;
			for (int i = 0; i < 4; ++i)
				GetSong()->AddEventAtEnd(GetSong()->MakeNewEvent());
			SetScrollRange(GetSong()->GetTotalEvents() - GetMaxEvents());
			SetScrollPos(GetSong()->GetTotalEvents() - GetMaxEvents());
		}
	}
	void DoBlockOps(int Op);
	void MoveBlock(int dest);
	void CopyBlock(int dest);
	void ChangeDuration(int From, int To);
	void ChangeInst(int From, int To);
	void AddRepeat(UINT n);
	void InsertBlock(void);
	void DecrPitch();
	void IncrPitch(void);
	void IncreaseDuration(void);
	//----------------------------------
	CMsObject* GetDrawObject() { return m_pDrawObject; }
	void SetDrawObject(CMsObject* pDObj) { m_pDrawObject = pDObj; }
	int CalcMaxEvents(void);
	void GetEventRect(int Event, CRect& rect);
	UINT GetRawEventNumber(int x);
	CSize GetSelectionRegionSize() { return m_szSelectRect; }
	int GetSelectionRegionTop() { return m_SelectRectTop; }
	int IsEventDisplayed(CMsEvent* pEV);
	int MouseInRegion(CPoint p);
	//	int NoteToY(int Note);
	virtual void OnInitialUpdate();
	int QuantizeY(int y);
	void SetupDrawMode(int DrawMode, long v = 0);
	void UpdateNoteInfo(int RestFlag);
	void UpdateScrollbarInfo(int TotalEvents, const char* Title = 0);
	int XtoEventIndex(int x);
	int YtoNote(int y);
	CString GetSongTitle() {
		CString rStr;
		GetParentFrame()->GetWindowTextW(rStr);
		return rStr;
	}
	//-------Selected List Methodes ---------------
	CMsObject* GetSelectedObjectHead() { return m_pSelectedObjectHead; }
	void SetSelectedObjectHead(CMsObject* pObj) { m_pSelectedObjectHead = pObj; }
	CMsObject* GetSelectedObjectTail() { return m_pSelectedObjectTail; }
	void SetSelectedObjectTail(CMsObject* pObj) { m_pSelectedObjectTail = pObj; }
	void AddSelectedObject(CMsObject* pObj);
	void RemoveSelectedObject(CMsObject* pObj);
	CMsObject* MatchMouseToObjectInEvent(UINT Event, CPoint MousePointer);
	CMsObject* MatchMouseToSelectedObject(CPoint MousePointer);
	// Generated message map functions
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg	LRESULT OnLongMidiMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShortmidimsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStaffDispEvent(WPARAM wParam, LPARAM lParam);
	//-------- Combo Boxes -------------
	afx_msg LRESULT MyControlsMessages(WPARAM wp, LPARAM lp);
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonStop();
	afx_msg LRESULT OnMidiEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChildviewPlayerthread(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void MidiPlayNote(CMsNote* pNote, UINT NoteOnFlag);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
