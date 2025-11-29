// MsObject.h
// interface for the CMsObject class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

	//--------------------------------
	// States for something
	//--------------------------------

constexpr auto MSOBJ_PLAY_KEEP_TICKING = 0;
constexpr auto MSOBJ_PLAY_DONE = 1;
constexpr auto MSOBJ_PLAY_DONE_BUT_DO_NOT_REMOVE = 2;

enum {
	PLAYSTATE_NOT_STARTED = 0,
	PLAYSTATE_START,
	PLAYSTATE_RUNNING,
	PLAYSTATE_NOTE_OFF,
	PLAYSTATE_DONE
};

enum {
	PLAY_NO_ACTION = 0,
	PLAY_OBJECT_TIMED_OUT,
	PLAY_OBJECT_DONE,
};

extern CString csObjectTypeString[];

class CMsBar;
class CMsEndBar;
class CMsKeySignature;
class CMsLoudness;
class CMsNote;
class CMsObject;
class CMsRepeatEnd;
class CMsRepeatStart;
class CMsTempo;
class CMsTimeSignature;
class CMsPortamentoStart;
class CMsPortamentoEnd;
class CMsGlisando;
class CMsGlisandoEnd;
class CMsSong;
class CMsEvent;
class CChildViewStaff;

constexpr auto STAVE_OFFSET = 132;	//DISTANCE from top of client rect;
									// to the top of the treble stave
constexpr auto STAVE_LINE_SPACING = 8;
constexpr auto STAFF_HEIGHT = (19 * STAVE_LINE_SPACING);
constexpr auto EVENT_WIDTH = 40;	// width of each event5;
constexpr auto EVENT_OFFSET = 32;	// distance from left edge;
									// of the client rect to
									// where the first event is
constexpr auto STAVE_HEIGHT = STAVE_LINE_SPACING * 10;	// Distance from top line of;
									// The treble stave to the
									// bottom line of the base stave
constexpr auto MAXNOTE_HEIGHT = STAVE_LINE_SPACING * 6;	//how high high C is above stave;
constexpr auto QUANTIZED_STAFF_HEIGHT = STAFF_HEIGHT / 4;
constexpr auto CURSOR_CENTER = STAVE_LINE_SPACING;
constexpr auto HIGHC_OFFSET = STAVE_OFFSET - MAXNOTE_HEIGHT + CURSOR_CENTER;
constexpr auto CENTER_OF_TREBEL = (STAVE_OFFSET);
constexpr auto CENTER_OF_BASS = (STAVE_OFFSET + 48);

constexpr auto NOTE_LINE_OFFSET = EVENT_WIDTH / 2;
constexpr auto NOTE_HEAD_WIDTH = 10;
constexpr auto NOTE_HEAD_HIEGTH = 8;

constexpr auto TREBLE_CLEF_OFFSET = (STAVE_OFFSET - 8);
constexpr auto BASS_CLEF_OFFSET = (STAVE_OFFSET + 46);

constexpr auto MSOBJ_BAR = 0;
constexpr auto MSOBJ_ENDBAR = 1;
constexpr auto MSOBJ_KEYSIG = 2;
constexpr auto MSOBJ_LOUDNESS = 3;
constexpr auto MSOBJ_NOTE = 4;
constexpr auto MSOBJ_REPEATEND = 5;
constexpr auto MSOBJ_REPEATSTART = 6;
constexpr auto MSOBJ_TEMPO = 7;
constexpr auto MSOBJ_TIMESIG = 8;
constexpr auto MSOBJ_PORTAMENTO_START = 9;
constexpr auto MSOBJ_PORTAMENTO_STOP = 10;
constexpr auto MSOBJ_GLISANDO = 11;
constexpr auto MSOBJ_CHORD = 12;
constexpr auto MSOBJ_CHORD_NOTE = 13;

union ObjectTypes{
	CMsBar *pBar;
	CMsEndBar *pEnd;
	CMsKeySignature *pKey;
	CMsLoudness *pLoud;
	CMsNote *pNote;
	CMsObject *pObj;
	CMsRepeatEnd *pRepE;
	CMsRepeatStart *pRepS;
	CMsTempo *pTemp;
	CMsTimeSignature*pTime;
	CMsPortamentoEnd* pPortEnd;
	CMsPortamentoStart* pPortStart;
	CMsGlisando* pGlissando;
};

class CMsObject  
{
private:
	UINT m_PlayState;
	UINT m_ObjectID;
	bool m_Selected;	//object is selected
	bool m_HighLight;
	CMsObject *m_pNext;
	CMsObject *m_pPrev;
	CMsObject* m_pSelectedObjectNext;
	CMsObject* m_pSelectedObjectPrev;
	CMsEvent* m_pParentEvent;
	CMsSong* m_pSong;		// pointer to song for this obje4ct
protected:
	INT m_ObjType;
public:
	CMsObject();
	virtual ~CMsObject();
	bool Create(CMsSong* pSong, CMsEvent* pEvent);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process() = 0;
		//-------------------------------------------------
		// Play Method
		//	The return value says a lot.
		// 	PLAY_NO_ACTION
		//	PLAY_OBJECT_TIMED_OUT
		//
		//-------------------------------------------------
	virtual UINT Play() = 0;
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse) = 0;
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse) = 0;
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse) = 0;
	virtual int IsTimedObject() = 0;
	virtual bool DoesSomething() = 0;
	//-------------------------------------------------
	// Back to our regularly scheduled Methods
	//-------------------------------------------------
	virtual void PlayStateStart() { m_PlayState = PLAYSTATE_START; }
	virtual UINT GetPlayState() { return m_PlayState; }
	virtual void SetPlayState(UINT state) { m_PlayState = state; }
	//-------------------------------------------------
	virtual void Copy(CMsObject *Source);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual void Save(FILE* pO);
	virtual void Print(FILE *pO, int Indent);
	virtual bool AddToQueue(CMsSong* pSong) { return false; }
	virtual bool MouseOverObject(CPoint pt);
	virtual void ObjectRectangle(CRect& rect, UINT Event) { rect.SetRect(0, 0, 0, 0); }
	virtual bool HighLight(bool HL, CPoint ptObj);
	virtual bool Select(bool Select, CPoint ptObj);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0) { return 0; }
	virtual void SetParentEvent(UINT ParrentEvent);
	virtual CMsEvent* GetParentEvent() { return m_pParentEvent; }
	virtual CMsObject* MakeANewObject() { return nullptr; }
	virtual void DebugDump();
	//-------------------------------------------------
	// Attribute Methods
	//-------------------------------------------------
	void SetType(INT t){m_ObjType = t;}
	INT GetType(void) const {return m_ObjType;}
	bool Is(INT t) const { return (m_ObjType == t); }
	void GetTypeString(CString& csType);
	void SetSelected(bool s) { m_Selected = s; }
	bool IsSelected(void) { return m_Selected; }
	void SetHighLight(bool hl) { m_HighLight = hl; }
	bool IsHighLighted() { return m_HighLight; }
	UINT GetObjectID() { return m_ObjectID; }
	CMsSong* GetSong() { return m_pSong; }
	void SetSong(CMsSong* pSong) { m_pSong = pSong; }
	//--------------------------------------------------
	// Linked List Methodes
	//--------------------------------------------------
	CMsObject* GetNext(void) { return m_pNext; }
	void SetNext(CMsObject* pO) { 
		m_pNext = pO; 
	}
	void SetPrev(CMsObject* pO) { m_pPrev = pO; }
	CMsObject* GetPrev(void) { return m_pPrev; }
	//------------------- Selected Object List -------------------------
	CMsObject* GetNextSelectedObject() { return m_pSelectedObjectNext; }
	void SetNextSelectedObject(CMsObject* pOBJ) { m_pSelectedObjectNext = pOBJ; }
	CMsObject* GetPrevSelectedObject() { return m_pSelectedObjectPrev; }
	void SetPrevSelectedObject(CMsObject* pObj) { m_pSelectedObjectPrev = pObj; }
};
