// MsObject.h: interface for the CMsObject class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

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
class CMsGlisandoStart;
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
	CMsGlisandoEnd* pGlisEnd;
	CMsGlisandoStart* pGlisStart;
};


enum MusicScoreOpbjects{
	MSOBJ_BAR,
	MSOBJ_ENDBAR,
	MSOBJ_KEYSIG,
	MSOBJ_LOUDNESS,
	MSOBJ_NOTE,
	MSOBJ_REPEATEND,
	MSOBJ_REPEATSTART,
	MSOBJ_TEMPO,
	MSOBJ_TIMESIG,
	MSOBJ_PORTAMENTO_START,
	MSOBJ_PORTAMENTO_STOP,
	MSOBJ_GLISANDO_START,
	MSOBJ_GLISANDO_END,
	MSOBJ_CHORD,
	MSOBJ_CHORD_NOTE
};

enum {
	MSOBJ_PLAY_KEEP_TICKING,
	MSOBJ_PLAY_DONE,
	MSOBJ_PLAY_DONE_BUT_DO_NOT_REMOVE
};
class CMsObject  
{
	UINT m_ObjectID;
	BOOL m_Selected;	//object is selected
	BOOL m_HighLight;
	CMsObject *m_pNext;
	CMsObject *m_pPrev;
	CMsObject* m_pNextQueueObj;
	CMsObject* m_pPrevQueueObj;
	CMsObject* m_pSelectedObjectNext;
	CMsObject* m_pSelectedObjectPrev;
	CMsEvent* m_pParentEvent;
	CMsSong* m_pSong;		// pointer to song for this obje4ct
protected:
	UINT m_ObjType;
public:
	CMsObject();
	virtual ~CMsObject();
	void Create(CMsSong* pSong, UINT ParentEvent); 
	virtual void Copy(CMsObject *Source);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual void Save(FILE* pO);
	virtual void Print(FILE *pO);
	virtual UINT Play();
	virtual BOOL AddToQueue(CMsSong* pSong) { return FALSE; }
	virtual UINT Process();
	virtual BOOL MouseOverObject(CPoint pt);
	virtual void ObjectRectangle(CRect& rect, UINT Event) { rect.SetRect(0, 0, 0, 0); }
	virtual BOOL HighLight(BOOL HL, CPoint ptObj);
	virtual BOOL Select(BOOL Select, CPoint ptObj);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0) { return 0; }
	virtual void SetParentEvent(UINT ParrentEvent);
	virtual CMsEvent* GetParentEvent() { return m_pParentEvent; }
	virtual CMsObject* MakeANewObject() { return nullptr; }
	virtual int MouseLButtonDown( int DrawState, CPoint pointMouse);
	virtual int MouseLButtonUp(int DrawState, CPoint pointMouse);
	virtual int MouseMove(int DrawState, CPoint pointMouse);
	virtual void DebugDump();
//-------------------------------------------------
	// Attribute Methods
	//-------------------------------------------------
	void SetType(int t){m_ObjType = t;}
	int GetType(void){return m_ObjType;}
	void GetTypeString(CString& csType);
	void SetSelected(BOOL s) { m_Selected = s; }
	BOOL IsSelected(void) { return m_Selected; }
	void SetHighLight(BOOL hl) { m_HighLight = hl; }
	BOOL IsHighLighted() { return m_HighLight; }
	UINT GetObjectID() { return m_ObjectID; }
	CMsSong* GetSong() { return m_pSong; }
	void SetSong(CMsSong* pSong) { m_pSong = pSong; }
	//--------------------------------------------------
	// Linked List Methodes
	//--------------------------------------------------
	CMsObject* GetNext(void) { return m_pNext; }
	void SetNext(CMsObject* pO) { m_pNext = pO; }
	void SetPrev(CMsObject* pO) { m_pPrev = pO; }
	CMsObject* GetPrev(void) { return m_pPrev; }
	//-----------------------------------------
	CMsObject* GetNextQueueObj(void) { return m_pNextQueueObj; }
	void SetNextQueueObj(CMsObject* pO) { m_pNextQueueObj = pO; }
	void SetPrevQueueObj(CMsObject* pO) { m_pPrevQueueObj = pO; }
	CMsObject* GetPrevQueueObj(void) { return m_pPrevQueueObj; }
	//------------------- Selected Object List -------------------------
	CMsObject* GetNextSelectedObject() { return m_pSelectedObjectNext; }
	void SetNextSelectedObject(CMsObject* pOBJ) { m_pSelectedObjectNext = pOBJ; }
	CMsObject* GetPrevSelectedObject() { return m_pSelectedObjectPrev; }
	void SetPrevSelectedObject(CMsObject* pObj) { m_pSelectedObjectPrev = pObj; }
};
