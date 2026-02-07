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
class CMsGlissando;
class CMsGlisandoEnd;
class CMsSong;
class CMsEvent;
class CChildViewStaff;

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
	CMsGlissando* pGlissando;
};

class CMsObject  
{
public:
	enum class MsObjType : int {
		NONE = 0,
		BAR,
		ENDBAR,
		KEYSIG,
		LOUDNESS,
		NOTE,
		FLAT,
		SHARP,
		NATURAL,
		REPEATEND,
		REPEATSTART,
		TEMPO,
		TIMESIG,
		PORTAMENTO_START,
		PORTAMENTO_STOP,
		GLISSANDO,
		GLISSANDO_END,
		CHORD,
		CHORD_NOTE,
		TREBLE_CLEF,
		BASS_CLEF
	};
private:
	struct ObjTypeStringItem {
		MsObjType m_type;
		const char* m_pTypeString;
		bool Is(MsObjType type) const {
			return m_type == type;
		}
		bool Compare(const char* pTypeString) const {
			return 0 == strcmp(m_pTypeString, pTypeString);
		}
	};
	static inline const ObjTypeStringItem ObjTypeStringTable[] = {
		{ MsObjType::NONE,			"None" },
		{ MsObjType::BAR,			"Bar" },
		{ MsObjType::ENDBAR,		"End Bar" },
		{ MsObjType::KEYSIG,		"Key Signature" },
		{ MsObjType::LOUDNESS,		"Loudness" },
		{ MsObjType::NOTE,			"Note" },
		{ MsObjType::FLAT,			"Flat" },
		{ MsObjType::SHARP,			"Sharp" },
		{ MsObjType::NATURAL,		"Natural" },
		{ MsObjType::REPEATEND,		"Repeat End" },
		{ MsObjType::REPEATSTART,	"Repeat Start" },
		{ MsObjType::TEMPO,			"Tempo" },
		{ MsObjType::TIMESIG,		"Time Signature" },
		{ MsObjType::PORTAMENTO_START,	"Portamento Start" },
		{ MsObjType::PORTAMENTO_STOP,	"Portamento Stop" },
		{ MsObjType::GLISSANDO,		"Glissando" },
		{ MsObjType::GLISSANDO_END,	"Glissando End" },
		{ MsObjType::CHORD,			"Chord" },
		{ MsObjType::CHORD_NOTE,	"Chord Note" },
		{ MsObjType::TREBLE_CLEF,	"Treble Clef" },
		{ MsObjType::BASS_CLEF,		"Bass Clef" },
		{ MsObjType(-1),			nullptr  }
	};
	//---------------------------------
	// Object Attributes
	//---------------------------------
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
	MsObjType m_ObjType;
public:
	CMsObject();
	CMsObject(MsObjType type);
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
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition) = 0;
	virtual int IsTimedObject() = 0;
	virtual bool DoesSomething() = 0;
	virtual UINT ObjectToString(CString& csString, UINT mode = 0) = 0;
	virtual void Draw(CDC* pDC) = 0;
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent) = 0;
	//-------------------------------------------------
	// Back to our regularly scheduled Methods
	//-------------------------------------------------
	virtual void PlayStateStart() { m_PlayState = PLAYSTATE_START; }
	virtual UINT GetPlayState() { return m_PlayState; }
	virtual void SetPlayState(UINT state) { m_PlayState = state; }
	//-------------------------------------------------
	virtual void Copy(CMsObject *Source);
	virtual void Save(FILE* pO);
	virtual void Print(FILE *pO, int Indent);
	virtual bool AddToQueue(CMsSong* pSong) { return false; }
	virtual bool MouseOverObject(CPoint pt);
	virtual void ObjectRectangle(CRect& rect, UINT Event) { rect.SetRect(0, 0, 0, 0); }
	virtual bool HighLight(bool HL, CPoint ptObj);
	virtual bool Select(bool Select, CPoint ptObj);
//	virtual void SetParentEvent(UINT ParrentEvent);
	virtual CMsEvent* GetParentEvent() { 
		return m_pParentEvent; 
	}
	virtual void SetParentEvent(CMsEvent* pEvent) { 
		//if (pEvent)
		//{
			m_pParentEvent = pEvent;
//		}
	}
	virtual CMsObject* MakeANewObject() { return nullptr; }
	virtual void DebugDump();
	//-------------------------------------------------
	// Attribute Methods
	//-------------------------------------------------
	void SetType(MsObjType t){m_ObjType = t;}
	MsObjType GetType(void) const { return m_ObjType; }
	bool Is(MsObjType t) const { return (m_ObjType == t); }
	void GetTypeString(CString& csType);
	void SetSelected(bool s) { m_Selected = s; }
	bool IsSelected(void) const { return m_Selected; }
	void SetHighLight(bool hl) { m_HighLight = hl; }
	bool IsHighLighted() const  { return m_HighLight; }
	UINT GetObjectID() const { 
		return m_ObjectID; 
	}
	CMsSong* GetSong() { return m_pSong; }
	void SetSong(CMsSong* pSong) { m_pSong = pSong; }
	CChildViewStaff* GetStaffView();

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
	CChildViewStaff* GetStaffChildView();
	//--------------------------------------------------
	// Type String Methods
	//--------------------------------------------------
	static MsObjType GetTypeFromString(const char* pTypeString);
	static const char* GetStringFromType(MsObjType type);

};
