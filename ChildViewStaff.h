
// CChildViewStaff.h : interface of the CChildViewStaff class
//
#pragma once

constexpr auto STAFFVIEW_MIDI_IN_DISPATCH_NUMENTRIES = 4;
constexpr auto INVALID_PITCH = -1;


constexpr auto	BM_NOTE_INDEX_WHOLE = 0;
constexpr auto	BM_NOTE_INDEX_HALF = 1;
constexpr auto	BM_NOTE_INDEX_QUARTER = 2;
constexpr auto	BM_NOTE_INDEX_EIGTH = 3;
constexpr auto	BM_NOTE_INDEX_SIXTEENTH = 4;
constexpr auto	BM_NOTE_INDEX_THIRTYSECOND = 5;


// CChildViewStaff window

class CChildViewStaff : public CChildViewBase
{
public:
	struct DrawStateItem {
		DRAWSTATE m_State;
		const char* m_pName;
		DrawStateItem(){
			m_State = DRAWSTATE::NA;
			m_pName = "NA";
		}
		DrawStateItem(DRAWSTATE state, const char* name) {
			m_State = state;
			m_pName = name;
		}
		bool IsState(DRAWSTATE state) const {
			return (m_State == state);
		}
	};
	inline static const DrawStateItem DrawStateLUT[] = {
		{ DRAWSTATE::NA, "NA" },
		{ DRAWSTATE::SET_ATTRIBUTES, "Set Attributes" },
		{ DRAWSTATE::WAITFORMOUSE_DOWN, "Wait for Mouse Down" },
		{ DRAWSTATE::MOVE_OBJECT_AROUND, "Move Object Around" },
		{ DRAWSTATE::PLACE, "Place Object" },
		{ DRAWSTATE::TIE_FIRSTNOTE, "Tie First Note" },
		{ DRAWSTATE::TIE_SECONDNOTE, "Tie Second Note" },
		{ DRAWSTATE::GLISSANDO_FIRST_NOTE, "Glissando First Note" },
		{ DRAWSTATE::GLISSANDO_SECOND_NOTE, "Glissando Second Note" },
		{ DRAWSTATE::GLISSANDO_END, "Glissando End" },
		{ DRAWSTATE(-1), NULL } // End Marker
	};
	inline static int CBDecorationFlags[APP_NUM_DECORATIONS] = {
		COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_CHECKED,
		COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_EXCLUSIVE | COMBOTOGGLE_TOGGLE_CHECKED,
		COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_EXCLUSIVE | COMBOTOGGLE_TOGGLE_CHECKED,
		COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_EXCLUSIVE | COMBOTOGGLE_TOGGLE_CHECKED
	};
	enum class DrawMode : int {
		NOP,
		NOTE,
		REST,
		ENDBAR,
		GLISSANDO,
		BAR,
		TIE,
		COPY,
		MOVE,
		REPEAT,
		TEMPO,
		TIMESIG,
		KEYSIG,
		LOUDNESS,
		INSTCHANGE,
		CHANGEDUR,
		INCREASEDUR,
		DECREASEDUR,	//16
		INCRPITCH,		//17
		DECRPITCH,		//18
		INSERTBLOCK,	//19
		DRAW_NOTES_VIA_MIDI	//20
	};
	enum class EventObjectSignatureTypes {
		EVENT_TEMPO_TIMESIG,
		EVENT_LOUDNESS_KEYSIG
	};

	enum class StaffViewMouseState {
		STAFFVIEW_MOUSEUP,
		STAFFVIEW_MOUSEDOWN
	};

	//-------------------------------
	// rest bitmap indexes
	//------------------------------

	enum class RestBitmapIndex {
		BM_REST_INDEX_WHOLE,
		BM_REST_INDEX_HALF,
		BM_REST_INDEX_QUARTER,
		BM_REST_INDEX_EIGTH,
		BM_REST_INDEX_SIXTEENTH,
		BM_REST_INDEX_THIRTYSECOND
	};
	//---------------------------
	// Note bitmap indexes
	//---------------------------
	enum class MiscBitmapIndex {
		BM_MISC_INDEX_MEASUREBAR,
		BM_MISC_INDEX_NOTETIE,
		BM_MISC_INDEX_LOUDNESS,
		BM_MISC_INDEX_TEMPO,
		BM_MISC_INDEX_INSTUMENT_CHANGE
	};
	//---------------------------
	// String Lookup Tables
	//---------------------------

	struct RegionLUTItem {
		MouseRegions Region;
		const char* name;
	};

	inline static const RegionLUTItem RegionLUT[] = {
		{ MouseRegions::MOUSE_OUTSIDE, "Outside" },
		{ MouseRegions::MOUSE_IN_EDITREG, "Edit Region" },
		{ MouseRegions::MOUSE_IN_UPPERSEL, "Upper Selection" },
		{ MouseRegions::MOUSE_IN_LOWERSEL, "Lower Selection" },
		{ MouseRegions::MOUSE_IN_UPPERDRAW, "Upper Draw Region" },
		{ MouseRegions::MOUSE_IN_LOWERDRAW, "Lower Draw Region" },
		{ MouseRegions::MOUSE_NONE, "Error" }
	};

	struct StaffMouseStatesLUTItem {
		StaffMouseStates State;
		const char* pName;
	};

	inline static const StaffMouseStatesLUTItem StaffMouseStateLUT[] = {
		{StaffMouseStates::MOUSE_STAFF_STATE_NONE, "None" },
		{ StaffMouseStates::MOUSE_STAFF_STATE_NOTE_CHANGE, "Note Change" },
		{ StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE, "Event Change" },
		{ StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE, "Note and Event Change"},
		{ StaffMouseStates::END_STATE, "Error State" }
	};

	struct RegionTransitionLUTItem {
		MouseRegionTransitionState Transition;
		const char* pName;
	};

	inline static const RegionTransitionLUTItem RegionTransitionLUT[] = {
		{ MouseRegionTransitionState::MOUSE_TRANSITION_NONE, "None" },
		//------------------------------------------
		{ MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_OUTSIDE, "UpperDraw to Outside" },
		{ MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT, "UpperDraw to Edit" },
		{ MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_UPPER_SEL, "UpperDraw to UpperSel" },
		//-----------------------------------------
		{ MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE, "Edit to Outside" },
		{ MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW, "Edit to LowerDraw" },
		{ MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW, "Edit to UpperDraw" },
		//-----------------------------------------
		{ MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_EDIT, "LowerDraw to Edit" },
		{ MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_OUTSIDE, "LowerDraw to Outside" },
		{ MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_LOWER_SEL, "LowerDraw to LowerSel" },
		//-----------------------------------------
		{ MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_LOWERSEL, "Outside to LowerSel" },
		{ MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_UPPERSEL, "Outside to UpperSel" },
		{ MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_EDIT, "Outside to Edit" },
		{ MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_LOWER_DRAW, "Outside to LowerDraw" },
		{ MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_UPPER_DRAW, "Outside to UpperDraw" },
		//-----------------------------------------
		{ MouseRegionTransitionState::MOUSE_TRANSITION_ERROR, "Error" }
	};
	//---------------------------
	// Data Members
	//---------------------------
private:
	bool m_EscapeFlag;
	UINT m_TimerID;
	UINT m_DragFlag;
	UINT m_AltKeyDown;
	UINT m_CtrlKeyDown;
	UINT m_ShiftKeyDown;

	//---------- View Colors -------------
	SColorPalette m_ColorPalette;
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
	CMyButton m_Button_Pause;
	CMyButton m_Button_Stop;
	CStaticStatus m_Status;
	//------- Song Position ----------------
	int m_nLastSongPosition;
	int m_SongScrollPos;	//Event that staff
							//display starts at
	int m_nDrawEvent;		// logical event under mouse
	int m_nRawEvent;		// physical event under mouse
	int m_FirstSelectedEvent;
	int m_LastSelectedEventIndex;
	//------- Tie data -------------------
	CPoint m_TieEndPoint;
	CPoint m_TieStartPoint;
	CMsNote* m_pSecondTieNote;
	CMsNote* m_pFirstTieNote;
	//-------------------------------------
	DRAWSTATE m_nDrawState;
	INT m_LastPitch;
	CMsNote* pLastNote;
	CMsObject* m_pDrawObject;
	DrawMode m_dmDrawMode;
	MouseRegions m_MouseRegion;
	MouseRegionTransitionState m_MouseRegionTransitionState;
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
	StaffViewMouseState m_nMouseState;
//--------- View Regions -------------------
	CMyRgn m_rgnUpperSelect;
	CRect m_UpperSelRect;

	CMyRgn m_rgnUpperDraw;
	CRect m_rectUpperDraw;

	CMyRgn m_rgnEdit;
	CRect m_rectEdit;

	CMyRgn m_rgnLowerDraw;
	CRect m_rectLowerDraw;

	CMyRgn m_rgnLowerSelect;
	CRect m_LowerSelRect;

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
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnDraw(CDC* pDC);
	void DrawControls(CDC* pDC);
	//---- Get/Set Data Member Access Methods -----
	NoteData& GetNoteData() { return m_CurrentNoteData; }
	CMsNote* GetLastNote() { return pLastNote; }
	void SetLastNote(CMsNote* pLN) { pLastNote = pLN; }
	INT GetLastPitch() const { return m_LastPitch; }
	bool LastPitchIsValid() const {
		bool rv = false;
		if (m_LastPitch >= 0)
		{
			rv = true;
		}
		return rv;
	}
	void SetLastPitch(INT lastPitch) { m_LastPitch = lastPitch; }

	int GetSongScrollPosition() { return m_SongScrollPos; }
	void SetSongScrollPosition(int SSP) { m_SongScrollPos = SSP; }

	int GetLastSongPosition() { return m_nLastSongPosition; }
	void SetLastSongPosition(int LSP) { m_nLastSongPosition = LSP; }

	int GetPitch() { return GetNoteData().GetPitch(); }
	void SetPitch(int p) { GetNoteData().SetPitch(p); }

	INT GetNoteDuration() { return GetNoteData().GetDuration(); }
	void SetNoteDuration(INT nd) { GetNoteData().SetDuration(nd); }

	INT GetTrack() { return GetNoteData().GetTrack(); }
	void SetTrack(INT T) { GetNoteData().SetTrack(T); }

	int GetAccent() { return GetNoteData().GetAccent(); }
	void SetAccent(int A) { GetNoteData().SetAccent(A); }

	int GetDotted() { return GetNoteData().GetDotted(); }
	void SetDotted(UINT D) { GetNoteData().SetDotted(D); }

	UINT GetTriplet() { return GetNoteData().GetTriplet(); }
	void SetTriplet(UINT nT) { GetNoteData().SetTriplet(nT); }

	UINT GetRest() { return GetNoteData().GetRest(); }
	void SetRest(UINT NR) { GetNoteData().SetRest(NR); }

	INT GetAccidental() { return GetNoteData().GetAccidental(); }
	void SetAccidental(INT NA) { GetNoteData().SetAccidental(NA); }

	bool GetStemDown() { return GetNoteData().GetStemDown(); }
	void SetUpSideDown(bool USD) { GetNoteData().SetStemDown(USD); }

	UINT GetHeadFlipped() { return GetNoteData().GetHeadFlipped(); }
	void SetHeadFlipped(UINT flipped) { GetNoteData().SetHeadFlipped(flipped); }

	int GetDrawEvent() const { 
		return m_nDrawEvent;
	}
	void SetDrawEvent(int drawEvent) {	m_nDrawEvent = drawEvent; }
	//---------------------------------------------------
	int GetMaxEvents() const {return m_MaxEvents;}
	void SetMaxEvents(int mE) { m_MaxEvents = mE; }
	int GetRawEvent() const { return m_nRawEvent; }
	inline CMsSong* GetSong() { 
		return m_pSong; 
	}
	inline void SetSong(CMsSong* pMS) { m_pSong = pMS; }
	// Operations
	void CheckAndDoScroll(CPoint point);
	void DoBlockOps(int Op);
	void MoveBlock(int dest);
	void CopyBlock(int dest);
	void ChangeDuration(INT From, INT To);
	void ChangeInst(INT From, INT To);
	void AddRepeat(UINT n);
	void InsertBlock(void);
	void DecrPitch();
	void IncrPitch(void);
	void IncreaseDuration(void);
	//----------------------------------
	CMsObject* GetDrawObject() { return m_pDrawObject; }
	void SetDrawObject(CMsObject* pDObj) { m_pDrawObject = pDObj; }
	int CalcMaxEvents(void);
	UINT GetRawEventNumber(int x);
	CSize GetSelectionRegionSize() const { return m_szSelectRect; }
	int GetSelectionRegionTop() const { return m_SelectRectTop; }
	int IsEventDisplayed(CMsEvent* pEV);
	MouseRegions MouseInRegion(CPoint p);
	virtual void OnInitialUpdate();
	int QuantizeY(int y);
	void SetupDrawMode(DrawMode Mode, long v = 0);
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
	CMsSongInfo* GetSongInfo(){return GetSong()->GetSongInfo();}
	CMsTrack* GetTrackInfo(int TrackID);
	SColorPalette* GetColorPalette() { return &m_ColorPalette; }
	//--------------- View Regions ------------------
	CRect GetUpperSelRect() const {
		return m_UpperSelRect;
	}
	CRect GetUpperDrawRect() const {
		return m_rectUpperDraw;
	}
	CRect GetEditRect() const {
		return m_rectEdit;
	}
	CRect GetLowerDrawRect() const {
		return m_rectLowerDraw;
	}
	CRect GetLowerSelRect() const {
		return m_LowerSelRect;
	}
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

    afx_msg void OnMenuMsFileOpen();
    afx_msg void OnMenuMsFileSave();
    afx_msg void OnUpdateMenuMsFileOpen(CCmdUI* pCmdUI);
    afx_msg void OnUpdateMenuMsFileSave(CCmdUI* pCmdUI);
	afx_msg void OnMenuMsFileSaveAs();
	afx_msg void OnUpdateMenuMsFileSaveAs(CCmdUI* pCmdUI);
	//-----------------------------
	MouseRegionTransitionState RegionTransition(CPoint ptMousePos);
	MouseRegionTransitionState GetTransitionState() const {
		return m_MouseRegionTransitionState;
	}
	MouseRegions GetMouseRegion() const {
		return m_MouseRegion;
	}
	static const char* GetMouseRegionName(MouseRegions region);
	static const char* GetMouseRegionTransitionName(MouseRegionTransitionState transition);
	static const char* GetStaffMouseStateName(StaffMouseStates state);
	static const char* GetDrawStateName(DRAWSTATE state);
    afx_msg void OnSettingsTracksettinigs();
    afx_msg void OnUpdateSettingsTracksettinigs(CCmdUI* pCmdUI);
};
