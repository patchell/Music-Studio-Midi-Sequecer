// MsSong.h: interface for the CMsSong class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


constexpr int SEARCH_FORWARD = 0;
constexpr int SEARCH_REVERSE = 1;

constexpr auto SONG_NOT_PLAYING = 0;
constexpr auto SONG_IS_PLAYING = 1;
constexpr auto SONG_STOP = 2;	// This Status means we need
								// to wait for the events
								// in the playlist queue
								// to time out.


class CMsSong
{
	//--- Data for Song Editor View ----
	CMsEvent * m_pEventListTail;
	CMsEvent * m_pEventListHead;
	int m_nMeasureBarCount;
	int m_nTotalEvents;
	//--------------------------------
	int m_nIsPlaying;
	CMsSong* m_pNextSong;
	CMsSong* m_pPrevSong;
	//*************************************
	// Attributes used to play the song
	//*************************************
	int m_SongID;
	int m_PlaySongTimerEnable;
	int m_PlayState;
	char m_Patches[16];
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
	//--------------------------------------
	// Song position pointer
	//--------------------------------------
	CMsEvent* m_pSongPosition;			//current song position
	//---------------------------------------
	// List of Midi operations, like note
	// timers that are currently actively
	// playing.
	//---------------------------------------
	CMsObject* m_pPlayerObjectListHead;	
	CMsObject* m_pPlayerObjectListTail;
	//----------------------------------------
	// variables for playing back song
	//----------------------------------------
	UINT m_MidiClockFlag;

	CMsKeySignature* m_pLastKeySignature;
	CMsTimeSignature* m_pLastTimeSignature;
	CMsTempo* m_pLastTempo;
	CMsLoudness* m_pLastLoudness;

	CMsStack m_stackRepeat;
	//--------------------------------------
	// Parent Objects
	//---------------------------------------
	CChildViewStaff* m_pChildView;	//staff view window
public:
	CMsSong();
	virtual ~CMsSong();
	void Create(CChildViewStaff* pCCV);
	UINT LittleEndian(UINT bE);
	//--------------Access to Attributes ------
	CChildViewStaff* GetStaffChildView() {
		return m_pChildView;
	}
	
	CMsLoudness* GetCurrentLoudness() { 
		return m_pLastLoudness; 
	}
	void SetCurrentLoudness(CMsLoudness* pNewLoudness) {
		m_pLastLoudness = pNewLoudness;
	}
	//------------- Tempo -------------------------
	CMsTempo* GetCurrentTempo() {
		return m_pLastTempo;
	}
	void SetCurrentTempo(CMsTempo* pNewTempo) {
		m_pLastTempo = pNewTempo;
	}
	//----------Time Signature ---------------------
	CMsTimeSignature* GetCurrentTimeSignature() {
		return m_pLastTimeSignature;
	}
	void SetCurrentTimeSignature(CMsTimeSignature* pNewTS) {
		m_pLastTimeSignature = pNewTS;
	}
	//------------------ Key Signature ----------------
	CMsKeySignature* GetCurrentKeySignature() {
		return m_pLastKeySignature;
	}
	void SetCurrentKeySignature(CMsKeySignature* pNewKS) {
		m_pLastKeySignature = pNewKS;
	}

	UINT GetSongPlayState() {
		return m_PlayState;
	}
	void SetSongPlayState(UINT PS) { m_PlayState = PS; }

	//--------- Screen Management ---------------------
	virtual void Draw(CDC* pDC, int event, int maxevent, CRect* pRect);
	//------------ Song Playing Functions
	// //indicates that the song is playing
	int IsPlaying() { return m_nIsPlaying; }
	void SetIsPlaying(int v) { m_nIsPlaying = v; }
	//---------------- Play --------------
	bool Play(CChildViewStaff* pCChildView);
	//-------------File Save -----------------------------
	void Save(FILE *pO);
	int Parse(char *pSongData);
	void Print(FILE *pO);
	// ------------ Note Managment ------------------
	CMsNote* CheckForNotePresence(int Event, int Note);
	//-----------Object Managment --------------------
	int AddObjectToSong(int event, CMsObject* pO);
	UINT AddMoreEvenrsAtEnd(UINT NewEndEvent);
	CMsObject* GetMsObject(INT ObjType, CMsEvent* pStartEventObject, INT Direction);
	CMsObject* GetObjectTypeInEvent(int nType, int nEvent);
	//---------- Event Management -------------------------
	CMsEvent* GetEventListHead(void) { return m_pEventListHead; }
	void SetEventListHead(CMsEvent* pEV) { m_pEventListHead = pEV; }
	CMsEvent* GetEventListTail() { return m_pEventListTail; }
	void SetEventListTail(CMsEvent* pEV) { m_pEventListTail = pEV; }
	CMsEvent* GetEventObject(int Event);
	CMsEvent* InsertEvent(int e);
	void AddEventAtStart(CMsEvent* pE);
	void AddEventAtEnd(CMsEvent* pE);
	void AddEventChain(int EventDest, CMsEventChain* pEvC);
	void AddEventChain(CMsEvent* Destination, CMsEventChain* pEvC, int DestIndex, int AddBlanks);
	void RemoveEvent(int Event);
	void DeleteEvent(CMsEvent* pEvent);
	CMsEvent* MakeNewEvent();
	int GetTotalEvents() { return m_nTotalEvents; }
	void SetTotalEvents(int t) { m_nTotalEvents = t; }
	void RenumberEvents(int* First = nullptr, int* Last = nullptr);
	int GetSongId() { return m_SongID; }
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
	void SetPlaySongTimerEnable(int enable) {
		m_PlaySongTimerEnable = enable;
	}
	int GetPlaySongTimerEnable() {
		return m_PlaySongTimerEnable;
	}
	//--------------------------------
	// Song playing functions
	//---------------------------------
	CMsStack& GetRepeatStack() { return m_stackRepeat; }
	UINT Ticker(void);
	int ProcessEvent(void);
	void SetSongPosition(CMsEvent* pEv) {
		m_pSongPosition = pEv;
//		if (pEv) printf(">****** Set Posirion %d ******<\n", pEv->GetIndex());
	}
	CMsEvent* GetSongPosition() { return m_pSongPosition; }
	void Start(void);
	int Stop(void);
	//------------------------------------
	bool CheckChan(int track, int chan);
	//--------------------------------------------
	// Methodes for managing the list of objects
	// that are currently active in  the playing
	// of the song
	//--------------------------------------------
	void AddObjectToPlayerQueue(CMsObject* pOBJ);
	CMsObject* RemoveObjectFromPlayerQueue(CMsObject* pObj);
	void SetPlayerObjectQueueHead(CMsObject* pOBJ) { m_pPlayerObjectListHead = pOBJ; }
	void SetPlayerObjectQueueTail(CMsObject* pOBJ) { m_pPlayerObjectListTail = pOBJ; }
	CMsObject* GetPlayerObjectQueueHead() { return m_pPlayerObjectListHead; }
	CMsObject* GetPlayerObjectQueueTail() { return m_pPlayerObjectListTail; }
	UINT PendingObjects();
	UINT CountObjectPlayingInQueue();
	//---------- Song List Methods --------------------
	// BoGuss?
	CMsSong* GetSongListNext() { return m_pNextSong; }
	void SetSongListNext(CMsSong* pSong) { m_pNextSong = pSong; }
	CMsSong* GetSongListPrev() { return m_pPrevSong; }
	void SetSongListPrev(CMsSong* pSong) { m_pPrevSong = pSong; }
	CMsNote* FindNoteInPlayList(CMsNote* pNote); 
	//----------------------------------
	// Song Playing State Defines
	// --------------------------------
};

