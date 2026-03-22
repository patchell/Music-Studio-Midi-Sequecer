// MsSong.h: interface for the CMsSong class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


constexpr int SEARCH_FORWARD = 0;
constexpr int SEARCH_REVERSE = 1;


class CChildViewStaff;
class CMsPlayerQueue;

class CMsSong
{
public:
	enum Insert {
		AFTER,
		BEFORE
	};
	inline static const int AtariInst[270] = {
		0x42,0x6c,0x6f,0x63,0x6b,0x73,
		0x00,0x00,0x00,0x00,0x48,0x61,0x72,0x6d,
		0x6f,0x6e,0x69,0x63,0x61,0x00,0x47,0x75,
		0x69,0x74,0x61,0x72,0x00,0x00,0x00,0x00,
		0x46,0x6c,0x75,0x74,0x65,0x00,0x00,0x00,
		0x00,0x00,0x43,0x6c,0x61,0x72,0x69,0x6e,
		0x65,0x74,0x00,0x00,0x42,0x61,0x72,0x69,
		0x74,0x6f,0x6e,0x65,0x00,0x00,0x48,0x69,
		0x68,0x61,0x74,0x00,0x6f,0x00,0x6e,0x00,
		0x53,0x6e,0x61,0x72,0x65,0x00,0x6f,0x00,
		0x00,0x00,0x42,0x2e,0x46,0x69,0x64,0x64,
		0x6c,0x65,0x00,0x00,0x53,0x61,0x78,0x00,
		0x74,0x6f,0x6e,0x65,0x00,0x00,0x43,0x6c,
		0x61,0x72,0x69,0x6e,0x65,0x74,0x00,0x00,
		0x50,0x69,0x61,0x6e,0x6f,0x00,0x00,0x00,
		0x00,0x00,0x42,0x61,0x73,0x73,0x00,0x73,
		0x00,0x00,0x00,0x00,0x56,0x69,0x62,0x65,
		0x73,0x00,0x65,0x74,0x00,0x00,0x42,0x65,
		0x6c,0x6c,0x73,0x00,0x00,0x00,0x00,0x00,
		0x8f,0x23,0x0c,0x06,0x07,0x08,0x06,0x09,
		0x86,0x22,0x0c,0x05,0x0b,0x00,0x05,0x00,
		0x80,0x12,0x0d,0x03,0x07,0x04,0x05,0x02,
		0x82,0x14,0x0a,0x08,0x0c,0x08,0x08,0x07,
		0x8f,0x29,0x0d,0x06,0x0c,0x08,0x0a,0x0c,
		0x85,0x38,0x0a,0x0e,0x09,0x0a,0x07,0x04,
		0x1e,0x03,0x0d,0x05,0x05,0x03,0x05,0x05,
		0x1e,0x46,0x0d,0x0f,0x07,0x0f,0x04,0x0f,
		0x85,0x43,0x0e,0x07,0x08,0x0e,0x07,0x01,
		0x8d,0x34,0x0e,0x0f,0x08,0x04,0x05,0x07,
		0x82,0x46,0x08,0x07,0x06,0x0a,0x0b,0x0a,
		0x80,0x22,0x0e,0x07,0x08,0x05,0x06,0x00,
		0x8f,0x32,0x0e,0x05,0x0b,0x08,0x0a,0x0f,
		0x84,0x03,0x0f,0x05,0x09,0x03,0x00,0x01,
		0x8f,0x01,0x0c,0x01,0x08,0x02,0x07,0x00
	};

	struct SongPlayerGlobals {
		CMsKeySignature* m_pLastKeySignature = 0;
		CMsTimeSignature* m_pLastTimeSignature = 0;
		CMsTempo* m_pLastTempo;
		CMsLoudness* m_pLastLoudness;
		SongPlayerGlobals() {
			m_pLastKeySignature = 0;
			m_pLastTimeSignature = 0;
			m_pLastTempo = 0;
			m_pLastLoudness = 0;
		}
	};
	enum class TickerState
	{
		START,
		RUNNING,
		WIND_DOWN,
		STOPPED
	};
private:
	//-------------------------------------
	// Song Midi Info and other stuff
	//-------------------------------------
	CMsSongInfo m_SongInfo;
	//--------------------------------------
	// Events
	//--------------------------------------
	CMsEventDirectory* m_pEventDirectory;
	//-------------------------------
	// Event Linked List
	//-------------------------------
	CMsEvent* m_pEventListHead;
	CMsEvent* m_pEventListTail;
	//--- Data for Song Editor View ----
	CMsEvent* m_pCleffEvent;
	CMsEvent* m_pEditEvent;
	int m_nMeasureBarCount;
	int m_nTotalEvents;
	//---------------File Loading Data-----------------------
	CString m_csFileName;
	char* m_pFileBuffer;
	int m_nFileBufferSize;
	int m_InFileSize;
	int m_BufIndex;
	//--------------------------------
	//	Song List Management
	//--------------------------------
	CMsSong* m_pNextSong;
	CMsSong* m_pPrevSong;
	//*************************************
	// Attributes used to play the song
	//*************************************
	//----------------------------------
	// Synchronizing objects between
	// playing the song and the thread
	// that actually plays it.
	// NOTE:
	//	CMyEvent has nothing in common with
	// CMsMyEvent.  Sort of a misfortunate
	// choice of names.  CMyEvent is a class
	// that encapsulates the Windows Event
	// object.
	// CMsEvent is just a single event in the
	// song.
	//-------------------------------------
	CMyEvent m_EvEnableTimerComplete;
	CMyEvent m_EvDisableTimerComplete;
	CMyEvent m_EvAddEventQueueComplete;
	CMyEvent m_EvDelEventQueueComplete;
	//----------------------------------------
	// variables for playing back song
	//----------------------------------------
	CMsEvent* m_pCurrentSongPosition;			//current song position
	TickerState m_TickerState;
	int m_MidiClockToggleFlag;
	int m_TotalTicks;
	CMsStack m_stackRepeat;
	SongPlayerGlobals m_SongGlobals;
	CMsPlayerQueue* m_pPlayerObjectQueue;
	int m_SongID;
	int m_PlaySongTimerEnable;
	char m_Patches[16];
	//--------------------------------------
	// Parent Objects
	//---------------------------------------
	CChildViewStaff* m_pChildView;	//staff view window
	int m_NoteCountOn[128];     // count of note on events sent
	int m_NoteCountOff[128];    // count of note off events sent
public:
	CMsSong();
	virtual ~CMsSong();
	bool Create(
		CChildViewStaff* pCCV,
		CSize szTrackIconSize
	);
	UINT LittleEndian(UINT bE);
	//--------------Access to Attributes ------
	CChildViewStaff* GetStaffChildView() {
		return m_pChildView;
	}
	
	CMsLoudness* GetCurrentLoudness() const { 
		return m_SongGlobals.m_pLastLoudness; 
	}
	void SetCurrentLoudness(CMsLoudness* pNewLoudness) {
		m_SongGlobals.m_pLastLoudness = pNewLoudness;
	}
	//------------- Tempo -------------------------
	CMsTempo* GetCurrentTempo() const{
		return m_SongGlobals.m_pLastTempo;
	}
	void SetCurrentTempo(CMsTempo* pNewTempo) {
		m_SongGlobals.m_pLastTempo = pNewTempo;
	}
	//----------Time Signature ---------------------
	CMsTimeSignature* GetCurrentTimeSignature(); 
	void SetCurrentTimeSignature(CMsTimeSignature* pNewTS) {
		m_SongGlobals.m_pLastTimeSignature = pNewTS;
	}
	//------------------ Key Signature ----------------
	CMsKeySignature* GetCurrentKeySignature();
	void SetCurrentKeySignature(CMsKeySignature* pNewKS) {
		m_SongGlobals.m_pLastKeySignature = pNewKS;
	}
	//--------- Screen Management ---------------------
	virtual void Draw(CDC* pDC, int event, int maxevent, CRect* pRect);
	//------------ Song Playing Functions
	// //indicates that the song is playing
	bool IsPlaying() const { return m_TickerState != TickerState::STOPPED; }
	void SetTickerState(TickerState v) { m_TickerState = v; }
	TickerState GetTickerState() const {
		return m_TickerState;
	}
	CMsPlayerQueue* GetPlayerQueue() { return m_pPlayerObjectQueue; }
	//---------------- Play --------------
	bool Play(CChildViewStaff* pCChildView);
	//-------------File Save -----------------------------
	void Save(FILE *pO);
	void SaveTracks(FILE* pO);
	bool Open(CString& csFileName);
	int ParserGetC();
	bool SetGetPosition(int pos);
	int Parse(char *pSongData);
	void Print(FILE *pO);
	// ------------ Note Managment ------------------
	CMsNote* CheckForNotePresence(int Event, int Note);
	//-----------Object Managment --------------------
//	int AddObjectToSong(int event, CMsObject* pO);
	UINT AddMoreEventsAtEnd(UINT NewEndEvent);
	CMsObject* GetMsObject(CMsObject::MsObjType ObjType, CMsEvent* pStartEventObject, INT Direction);
	CMsObject* GetObjectTypeInEvent(CMsObject::MsObjType nType, int nEvent);
	//---------- Event Management -------------------------
	CMsEvent* GetEventListHead(void) { return m_pEventListHead; }
	void SetEventListHead(CMsEvent* pEV) { m_pEventListHead = pEV; }
	CMsEvent* GetEventListTail() { return m_pEventListTail; }
	void SetEventListTail(CMsEvent* pEV) { m_pEventListTail = pEV; }
	CMsEvent* GetEventObject(int Event);
	CMsEvent* InsertEmptyEvent(int EventIndex, int BeforeNotAfter = 0);
	CMsEvent* InsertEmptyEvent(CMsEvent* pEv, int BeforeNotAfter = 0);
	CMsEvent* InsertEvent(CMsEvent* pEvInsertHere, CMsEvent* pInsertThis, int BeforeNotAfter = 0);
	void AddEventAtStart(CMsEvent* pE);
	void AddEventAtEnd(CMsEvent* pE);
	void AddEventChain(int EventDest, CMsEventChain* pEvC);
	void AddEventChain(
		CMsEvent* Destination,
		CMsEventChain* pEvC, 
		int DestIndex, 
		int AddBlanks
	);
	void RemoveEvent(int Event);
	void DeleteEvent(CMsEvent* pEvent);
	CMsEvent* MakeNewEvent();
	CMsEvent* MakeNewEvent(int EventIndex);
	int GetTotalEvents() const { return m_nTotalEvents; }
	void SetTotalEvents(int t) { m_nTotalEvents = t; }
	void RenumberEvents();
	bool GetSeledtedEventBlock(CMsEvent** ppEvFirst, CMsEvent** ppEvLast);
	void RenumberMeasureBars();
	bool SelectEventsFrom(CMsEvent* pEventID);
	//----------------- Song ID Management ----------------
	int GetSongId() const { return m_SongID; }
	//**************************************
	// Methods for playing the song
	//**************************************
	//-------------------------------
	// Play Midi Thread Stuff
	//-------------------------------------
	CMyEvent& GetEnableTimerCompleteEV() { return m_EvEnableTimerComplete; }
	CMyEvent& GetDisableTimerCompleteEV() { return m_EvDisableTimerComplete; }
	CMyEvent& GetAddSongCompleteEV() { return m_EvAddEventQueueComplete; }
	CMyEvent& GetDelSongCompleteEV() { return m_EvDelEventQueueComplete; }
	//-------------------------------
	// Midi Message Functions
	//-------------------------------
	void ChangePatch(int Track, int chan, int patch);
	void MidiContinue(void);
	void MidiStop(void);
	void MidiStart(void);
	void MidiClock(void);
	void MidiAllNotesOff();
	void SetPlaySongTimerEnable(int enable) {
		m_PlaySongTimerEnable = enable;
	}
	int GetPlaySongTimerEnable() const {
		return m_PlaySongTimerEnable;
	}
	//--------------------------------
	// Song playing functions
	//---------------------------------
	CMsStack& GetRepeatStack() { return m_stackRepeat; }
	UINT Ticker(void);
	CMsEvent* GetNextEventToProcess();
	void SetSongPosition(CMsEvent* pEv);
	CMsEvent* GetSongPosition();
	void Start(void);
	int Stop(void);
	//------------------------------------
	bool CheckChan(int track, int chan);
	//---------- Song List Methods --------------------
	// BoGuss?
	CMsSong* GetSongListNext() { return m_pNextSong; }
	void SetSongListNext(CMsSong* pSong) { m_pNextSong = pSong; }
	CMsSong* GetSongListPrev() { return m_pPrevSong; }
	void SetSongListPrev(CMsSong* pSong) { m_pPrevSong = pSong; }
	bool ValidateFile();
	int DumpSong(FILE* pOutFile);
	UINT GetTotalTicks() const { return m_TotalTicks; }
	//--------- Midi Diagnostics ----------------------
	void IncNoteOnCount(int Note) { m_NoteCountOn[Note]++; };
	void IncNoteOffCount(int Note) { m_NoteCountOff[Note]++; }
	int GetNoteOnCount(int Note) const { return m_NoteCountOn[Note]; };
	int GetNoteOffCount(int Note) const { return m_NoteCountOff[Note]; };
	//=------------------------------------------------
	CMsEventDirectory* GetEventDirectory(){ return m_pEventDirectory; }
	CMsTrack* GetTrackInfo(int TrackNum); 
	CMsSongInfo* GetSongInfo() { return &m_SongInfo; }
	SColorPalette* GetColorPalette();
	int GetNumberOfTracks() const { return m_SongInfo.GetNumberOfTracks(); }
	//----------------- Edit Event Management ----------------
	void SetEditEvent(CMsEvent* pEv) { m_pEditEvent = pEv; }
	void SetEditEvent(int Event) { m_pEditEvent = GetEventObject(Event); }
	CMsEvent* GetEditEvent() { return m_pEditEvent; }
};

