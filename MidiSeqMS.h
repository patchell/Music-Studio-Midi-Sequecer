// MidiSeqMS.h : main header file for the MidiSeqMS application
//
#pragma once

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;
	enum { IDD = IDD_ABOUTBOX };
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};

class CMainFrame;

// CMidiSeqMSApp:
// See MidiSeqMS.cpp for the implementation of this class
//

class CMidiSeqMSApp : public CWinApp
{
	inline static int BmIdMisStuffTypes[APP_NUM_MISC] = {
		IDB_MEASUREBAR,
		IDB_NOTETIE,
		IDB_LOUDNESS,
		IDB_TEMPO,
		IDB_INST_CHANGE,
		IDB_CB_MISC_ENDBAR,
		IDB_GLISSANDO
	};
	inline static int  BmIdKeySigStringBitmapIDsTab[APP_NUM_KEYSIGNATURES + 1] = {
		-1,
		IDB_CB_KEY_CMAJ,	//1X
		IDB_CB_KEY_GMAJ,	//2X
		IDB_CB_KEY_DMAJ,	//3X
		IDB_CB_KEY_AMAJ,	//4X
		IDB_CB_KEY_EMAJ,	//5X
		IDB_CB_KEY_BMAJ,	//6X
		IDB_CB_KEY_FSMAJ,	//7X
		IDB_CB_KEY_CSMAJ,	//8X
		IDB_CB_KEY_FMAJ,	//9X
		IDB_CB_KEY_BFMAJ,	//10X
		IDB_CB_KEY_EFMAJ,	//11X
		IDB_CB_KEY_AFMAJ,	//12X
		IDB_CB_KEY_DFMAJ,	//13X
		IDB_CB_KEY_GFMAJ,	//14X
		IDB_CB_KEY_CFMAJ	//15X
	};
	inline static int BmIdTimeSigCBbMIDs[APP_NUM_TIMESIG] = {
		IDB_CB_TIMESIG_2_2,
		IDB_CB_TIMESIG_3_2,
		IDB_CB_TIMESIG_2_4,
		IDB_CB_TIMESIG_3_4,
		IDB_CB_TIMESIG_4_4,
		IDB_CB_TIMESIG_5_4,
		IDB_CB_TIMESIG_6_8
	};
	inline static int BmIdTimeSig[APP_NUM_TIMESIG] = {
		IDB_TIMESIG_2_2,
		IDB_TIMESIG_3_2,
		IDB_TIMESIG_2_4,
		IDB_TIMESIG_3_4,
		IDB_TIMESIG_4_4,
		IDB_TIMESIG_5_4,
		IDB_TIMESIG_6_8
	};
	inline static int BmIdRestsTypes[APP_NUM_RESTTYPES] = {
		IDB_WHOLEREST,
		IDB_HALFREST,
		IDB_BITMAP_QUARTERREST,
		IDB_EIGHTHREST,
		IDB_SIXTEENTHREST,
		IDB_THIRTYSEC_REST
	};
	inline static int BmIdRestComboBoxTypes[APP_NUM_RESTTYPES] = {
		IDB_CB_REST_WHOLE,
		IDB_CB_REST_HALF,
		IDB_CB_REST_QUARTER,
		IDB_CB_REST_EIGTH,
		IDB_CB_REST_SIXTEENTH,
		IDB_CB_REST_THIRTYSECOND
	};
	inline static int BmIdNotesTypes[APP_NUM_NOTETYPES] = {
		IDB_NOTE_WHOLE,
		IDB_NOTE_HALF,
		IDB_NOTE_QUARTER,
		IDB_NOTE_EIGTH,
		IDB_NOTE_SIXTEENTH,
		IDB_NOTE_THIRTYSECOND
	};
	inline static int BmIdDecorations[APP_NUM_DECORATIONS] = {
		IDB_ACCENT,
		IDB_DECOR_NONE,
		IDB_DOT,
		IDB_TRIPLET
	};
	inline static int DecorationsBmCbIdsSel[APP_NUM_DECORATIONS] = {
		IDB_CB_DECORATIONS_ACCENT_SEL,
		IDB_CB_DECORATIONS_NONE_SEL,
		IDB_CB_DECORATIONS_DOTTED_SEL,
		IDB_CB_DECORATIONS_TRIPLET_SEL
	};
	inline static int DecorationsBmCbIdsNotSel[APP_NUM_DECORATIONS] = {
		IDB_CB_DECORATIONS_ACCENT_NOTSEL,
		IDB_CB_DECORATIONS_NONE,
		IDB_CB_DECORATIONS_DOTTED_NOTSEL,
		IDB_CB_DECORATIONS_TRIPLET_NOTSEL
	};
	inline static int BmIdBlockOpTypes[APP_NUM_BLOCKOPTYPES] = {
		IDB_BLOCK_COPY,
		IDB_BLOCK_MOVE,
		IDB_BLOCK_CHANGEINSTRUMENT,
		IDB_BLOCK_CHANGEDUR,
		IDB_BLOCK_INCREASE,
		IDB_BLOCK_DECREASE,
		IDB_BLOCKUP,
		IDB_BLOCKDOWN,
		IDB_BLOCK_REPEAT,
		IDB_BLOCK_INSERT,
		IDB_BLOCK_MIDIINPUT
	};
	inline static int AccidentalBmIdsTypes[APP_NUM_ACCIDENTALTYPES] = {
		IDB_INKEY,
		IDB_SHARP,
		IDB_FLAT,
		IDB_NATURAL
	};
	inline static int AccidentalBmCBIdsTypes[APP_NUM_ACCIDENTALTYPES] = {
		IDB_CB_ACCIDENTAL_INKEY,
		IDB_CB_ACCIDENTALS_SHARP,
		IDB_CB_ACCIDENTAL_FLAT,
		IDB_CB_ACCIDENTAL_NATURAL
	};


	//---------------------------------------
	// Bitmap Tables for music editor
	//---------------------------------------

	inline static MIDI_STATUS MidiStatusCommands[DISPATCHEDIT__NUM_MIDI_CMDS] = {
		//------------ Channel Status ---------------------
		{CString("Note Off"), (unsigned)MidiChannelCmds::NOTEOFF,1,1,1, CString("Note"), CString("Velocity")},
		{CString("Note On"), (unsigned)MidiChannelCmds::NOTEON,1,1,1, CString("Note"), CString("Velocity")},
		{CString("Poly AftrTch"), (unsigned)MidiChannelCmds::POLYPRESS,1,1,1, CString("Note"), CString("Pres")},
		{CString("Control/Mode"),(unsigned)MidiChannelCmds::CTRLCHNG,1,1,1, CString("Type"), CString("Value")},
		{CString("Prog Cnance"),(unsigned)MidiChannelCmds::PGMCHANGE,1,0,1, CString("Prgm"), CString("-----")},
		{CString("After Touch"), (unsigned)MidiChannelCmds::CHNLPRESS,1,0,1, CString("Pres"), CString("----")},
		{CString("Pitch Bend"),(unsigned)MidiChannelCmds::PITCHBEND,1,1,1, CString("LSB"), CString("MSB")},
		//----- System Status ----------------------
		{CString("TimeCode"),(unsigned)MidiSystemCmds::TIMECODE, 0, 0, 0, CString("----"), CString("----")},
		{CString("Song Pos"),(unsigned)MidiSystemCmds::SONG_POSITION, 1, 1, 0, CString("LSB"), CString("MSB")},
		{CString("Song Sel"),(unsigned)MidiSystemCmds::SONG_SELECT, 1, 0, 0, CString("Song #"), CString("----")},
		{CString("Reserved"),(unsigned)MidiSystemCmds::RSVRD_F4, 0, 0, 0, CString("----"), CString("----")},
		{CString("Reserved"),(unsigned)MidiSystemCmds::RSVRD_F5, 0, 0, 0, CString("----"), CString("----")},
		{CString("Tune Request"),(unsigned)MidiSystemCmds::TUNE_REQUEST, 0, 0, 0, CString("----"), CString("----")},
		{CString("Timing Clock"),(unsigned)MidiRealTimeMsgs::CLOCK, 0, 0, 0, CString("----"), CString("----")},
		{CString("Reserved"),(unsigned)MidiRealTimeMsgs::RSVRD_F9, 0, 0, 0, CString("----"), CString("----")},
		{CString("Start"),(unsigned)MidiRealTimeMsgs::START, 0, 0, 0, CString("----"), CString("----")},
		{CString("Continue"),(unsigned)MidiRealTimeMsgs::CONTINUE, 0, 0, 0, CString("----"), CString("----")},
		{CString("Stop"),(unsigned)MidiRealTimeMsgs::STOP, 0, 0, 0, CString("----"), CString("----")},
		{CString("Reserved"),(unsigned)MidiRealTimeMsgs::RSVRD_FD, 0, 0, 0, CString("----"), CString("----")},
		{CString("Active Sensing"),(unsigned)MidiRealTimeMsgs::ACTIVE_SENSING, 0, 0, 0, CString("----"), CString("----")},
		{CString("Reset"),(unsigned)MidiRealTimeMsgs::RESET, 0, 0, 0, CString("----"), CString("----")},
		{CString("SYSEX"),(unsigned)MidiSystemCmds::SYSEX, 0, 0, 0, CString("----"), CString("----")},
		{CString("SYSEX END"),(unsigned)MidiSystemCmds::SYSEXEND, 0, 0, 0, CString("----"), CString("----")}
	};


	//-------------------------------------------
	//	Tbles for TX816 Editor
	//-------------------------------------------
	inline static UINT BmIdTX816_Algorithm[APP_TX816_NUM_ALG_BITMAPS] = {
		IDB_DX7_ALG01,
		IDB_DX7_ALG02,
		IDB_DX7_ALG03,
		IDB_DX7_ALG04,
		IDB_DX7_ALG05,
		IDB_DX7_ALG06,
		IDB_DX7_ALG07,
		IDB_DX7_ALG08,
		IDB_DX7_ALG09,
		IDB_DX7_ALG10,
		IDB_DX7_ALG11,
		IDB_DX7_ALG12,
		IDB_DX7_ALG13,
		IDB_DX7_ALG14,
		IDB_DX7_ALG15,
		IDB_DX7_ALG16,
		IDB_DX7_ALG17,
		IDB_DX7_ALG18,
		IDB_DX7_ALG19,
		IDB_DX7_ALG20,
		IDB_DX7_ALG21,
		IDB_DX7_ALG22,
		IDB_DX7_ALG23,
		IDB_DX7_ALG24,
		IDB_DX7_ALG25,
		IDB_DX7_ALG26,
		IDB_DX7_ALG27,
		IDB_DX7_ALG28,
		IDB_DX7_ALG29,
		IDB_DX7_ALG30,
		IDB_DX7_ALG31,
		IDB_Dx7_ALG32
	};

	inline static UINT BmIdTX816_LFO_Wave[APP_TX816_NUM_LFO_WAVES] = {
		IDB_TX816_LFO_TRI_0,
		IDB_TX816_LFO_SAWDOWN_1,
		IDB_TX816_LFO_SAWUP_2,
		IDB_TX816_LFO_SQUARE_3,
		IDB_TX816_LFO_SINE_4,
		IDB_TX816_LFO_SAMPLE_HOLD_5
	};



	//-------------------------------------
	// Tx816 Editor
	//-------------------------------------

	inline static CString MidiNoteToNoteString[127] = {
		_T("C -2"),	// 0
		_T("C#-2"),
		_T("D -2"),
		_T("D#-2"),
		_T("E -2"),
		_T("F -2"),
		_T("F#-2"),
		_T("G -2"),
		_T("G#-2"),
		_T("A -2"),
		_T("A#-2"),
		_T("B -2"),
		_T("C -1"),	// 12
		_T("C#-1"),
		_T("D -1"),
		_T("D#-1"),
		_T("E -1"),
		_T("F -1"),
		_T("F#-1"),
		_T("G -1"),
		_T("G#-1"),
		_T("A -1"),	// 21
		_T("A#-1"),	// 22
		_T("B -1"),	// 23
		_T("C  0"),	// 24
		_T("C# 0"),
		_T("D  0"),
		_T("D# 0"),
		_T("E  0"),
		_T("F  0"),
		_T("F# 0"),
		_T("G  0"),
		_T("G# 0"),
		_T("A  0"),
		_T("A# 0"),
		_T("B  0"),
		_T("C  1"),
		_T("C# 1"),
		_T("D  1"),
		_T("D# 1"),
		_T("E  1"),
		_T("F  1"),
		_T("F# 1"),
		_T("G  1"),
		_T("G# 1"),
		_T("A  1"),
		_T("A# 1"),
		_T("B  1"),
		_T("C  2"),
		_T("C# 2"),
		_T("D  2"),
		_T("D# 2"),
		_T("E  2"),
		_T("F  2"),
		_T("F# 2"),
		_T("G  2"),
		_T("G# 2"),
		_T("A  2"),
		_T("A# 2"),
		_T("B  2"),
		_T("C  3"),
		_T("C# 3"),
		_T("D  3"),
		_T("D# 3"),
		_T("E  3"),
		_T("F  3"),
		_T("F# 3"),
		_T("G  3"),
		_T("G# 3"),
		_T("A  3"),
		_T("A# 3"),
		_T("B  3"),
		_T("C  4"),
		_T("C# 4"),
		_T("D  4"),
		_T("D# 4"),
		_T("E  4"),
		_T("F  4"),
		_T("F# 4"),
		_T("G  4"),
		_T("G# 4"),
		_T("A  4"),
		_T("A# 4"),
		_T("B  4"),
		_T("C  5"),
		_T("C# 5"),
		_T("D  5"),
		_T("D# 5"),
		_T("E  5"),
		_T("F  5"),
		_T("F# 5"),
		_T("G  5"),
		_T("G# 5"),
		_T("A  5"),
		_T("A# 5"),
		_T("B  5"),
		_T("C  6"),
		_T("C# 6"),
		_T("D  6"),
		_T("D# 6"),
		_T("E  6"),
		_T("F  6"),
		_T("F# 6"),
		_T("G  6"),
		_T("G# 6"),
		_T("A  6"),
		_T("A# 6"),
		_T("B  6"),
		_T("C  7"),
		_T("C# 7"),
		_T("D  7"),
		_T("D# 7"),
		_T("E  7"),
		_T("F  7"),
		_T("F# 7"),
		_T("G  7"),
		_T("G# 7"),
		_T("A  7"),
		_T("A# 7"),
		_T("B  7"),
		_T("C  8"),	//120
		_T("C# 8"),
		_T("D  8"),
		_T("D# 8"),
		_T("E  8"),
		_T("F  8"),
		_T("F# 8"),
	};

	inline static CString TX816_OP_KEYBORD_Level_Scaling_Curve[4] = {
		_T("-LIN"),
		_T("-EXP"),
		_T("+EXP"),
		_T("+LIN")
	};
private:

	//------------------------------------------
	static UINT PlayWorkerThread(LPVOID param);
	CFont m_fFont;
	CMyBitmap m_BmQuarterNoteEqQNPM;
	//--------- Misc -----------------------------
	CMyBitmap** m_ppBmMisc;
	//---------------------
	// Time Signatures
	//---------------------
	CMyBitmap** m_ppBmCbTimeSig;
	CMyBitmap** m_ppBmTimeSig;
	//---------------------------------------
	// Key Signature
	//---------------------------------------
	CMyBitmap** m_ppBmCbKeySig;
	//---------------------
	// Accidentals
	//---------------------
	CMyBitmap** m_ppBmAccidentalTypes;
	CMyBitmap** m_ppBmCBAccidentalTypes;
	//---------------------
	// Block Op Graphics
	//---------------------
	CMyBitmap** m_ppBmBlockOps;
	//--------------
	// NOTE graphics
	//--------------
	CMyBitmap** m_ppBmNoteTypes;
	//----------------
	// Rest Graphics
	//----------------
	CMyBitmap** m_ppBmRestTypes;
	CMyBitmap** m_ppBmCBRestTypes;
	//---------------------
	// Decoration
	//---------------------
	CMyBitmap** m_ppBmDecorationsNotSel;
	CMyBitmap** m_ppBmCBDecorationsSel;
	//---------------------------------------------
	// 	   TX816 Editor Bitmaps
	//--------------------------------------------
	CMyBitmap m_aBmLfoWaves[APP_TX816_NUM_LFO_WAVES];
	CMyBitmap m_aBmAlgrithm[APP_TX816_NUM_ALG_BITMAPS];
	//----------------------------------------
	// Midi Information
	//----------------------------------------
//	CMidiInfo m_MidiInfo;
	//----------------------------------------
	FILE* m_pLog;
	FILE* pConsol;
	//------------------- Midi ---------------------
	CMidiOutTable m_MidiOutTable;
	CMidiInTable m_MidiInTable;
	//--------------Player Thread Stuff -------------
	HANDLE m_hPlayThread;
	DWORD m_idPlayerThread;
	//-------------- Timer Stuff --------------------
	CMyMMtimer m_MMtimer;
	CMyEvent m_ThredRunningEvent;
	//-------------------------------------------
	unsigned m_UniqueID;
public:
	CMidiSeqMSApp() noexcept;
	void CloseAllLogFiles();
	bool OpenLogFile(const char* pLogName);
	bool OpenConsol();
	HMIDIOUT GetMidiOutHandle(int MidiID) { return  GetMidiOutTable()->GetOutHandle(MidiID); }
	//	HMIDIIN GetMidiIn() {return  GetMidi()->GetMidiInHandle(); }
	FILE* LogFile() { return m_pLog; }
	//------------- Midi Info -----------------------
//	CMidiInfo* GetMidiInfo() { return &m_MidiInfo; }
	//------------ Bitmap Symbols -------------------
	CMyBitmap* bmGetQNoteEqQNPM() { return &m_BmQuarterNoteEqQNPM; }

	int GetNumTimeSig() { return APP_NUM_TIMESIG; }
	CMyBitmap* bmGetTimeSig(INT TimeSig) { return m_ppBmTimeSig[TimeSig]; }
	CMyBitmap* bmGetCbTimeSig(INT TimSig) { return m_ppBmCbTimeSig[TimSig]; }
	CMyBitmap** bmGetTimeSig() { return m_ppBmTimeSig; }
	CMyBitmap** bmGetCbTimeSig() { return m_ppBmCbTimeSig; }
	int GetTimeSigCbBmID(int TimeSig) { return BmIdTimeSigCBbMIDs[TimeSig]; }
	int GetTimeSigBmID(int TimeSig) { return BmIdTimeSig[TimeSig]; }	

	int GetNumNoteTypes() { return APP_NUM_NOTETYPES; }
	CMyBitmap* bmGetNoteType(int NoteType) { return m_ppBmNoteTypes[NoteType]; }
	int GetBmIdNoteType(int NoteType) { return BmIdNotesTypes[NoteType]; }	

	int GetNumRestTypes() { return APP_NUM_RESTTYPES; }
	CMyBitmap* bmGetCBRestTypes(int RestType) { return m_ppBmCBRestTypes[RestType]; }
	CMyBitmap* bmGetRestType(int RestType) { return m_ppBmCBRestTypes[RestType]; }
	static int GetRestBmIdsTypes(int restBmID) { return BmIdRestsTypes[restBmID]; }
	static int* GetRestBmIdsTypes() { return BmIdRestsTypes; }
	static int GetBmIdRestComboBoxTypes(int ID) {
		int rV = 0;
		if (ID < 0 || ID > APP_NUM_RESTTYPES)
			rV = -1;
		else
			rV = BmIdRestComboBoxTypes[ID];
		return rV;
	}
	static int* GetBmIdRestComboBoxTypes() {
		return BmIdRestComboBoxTypes;
	}

	int GetNumDecorations() { return APP_NUM_DECORATIONS; }
	CMyBitmap* bmGetDecorationNotSel(int Dec) { return m_ppBmDecorationsNotSel[Dec]; }
	CMyBitmap* bmGetDecorationSel(int Dec) { return m_ppBmCBDecorationsSel[Dec]; }
	static int GetDecorationsBmCbIdsNotSel(int Id) { return DecorationsBmCbIdsNotSel[Id]; }
	static int GetDecorationsBmCbIdsSel(int Id) { return DecorationsBmCbIdsSel[Id]; }

	int GetNumAccidentalTypes() { return APP_NUM_ACCIDENTALTYPES; }
	CMyBitmap* bmGetAccidentalType(int Accidnet) { return m_ppBmAccidentalTypes[Accidnet]; }
	CMyBitmap* bmGetCBAccidentalType(int Accident) { return m_ppBmCBAccidentalTypes[Accident]; }
	static int GetAccidentalBmCBIdsTypes(int id) { return AccidentalBmCBIdsTypes[id]; }
	static int GetAccidentalBmIdsTypes(int id) { return AccidentalBmIdsTypes[id]; }	

	int GetNumBlockOps() { return APP_NUM_BLOCKOPTYPES; }
	CMyBitmap* bmGetBlockOpType(int BO) { return  m_ppBmBlockOps[BO]; }
	static int GetBlockOpTypeBmID(int BO) { return BmIdBlockOpTypes[BO]; }

	int GetNumMisc() { return APP_NUM_MISC; }
	CMyBitmap* bmGetMiscType(int Misc) { 
		return m_ppBmMisc[Misc]; 
	}
	static int GetMiscTypeBmID(int Misc) { 
		return BmIdMisStuffTypes[Misc]; 
	}

	int GetNumKeySigs() { return APP_NUM_KEYSIGNATURES; }
	CMyBitmap* bmGetCBKeySignature(int KS) { return m_ppBmCbKeySig[KS]; }
	static int GetKeySigBmId(int KS) { return BmIdKeySigStringBitmapIDsTab[KS]; }

	int GetNumTX816LfoWaves() { return APP_TX816_NUM_LFO_WAVES; }
	CMyBitmap* bmGetTX816LFOWave(int LFOW) { return &m_aBmLfoWaves[LFOW]; }
	static int GetTx816BmIdLfoWave(int LFOW) { return BmIdTX816_LFO_Wave[LFOW]; }

	int GetNumAlgrithms() { return APP_TX816_NUM_ALG_BITMAPS; }
	CMyBitmap* bmGetTX816AlgBitmap(int alg) { return &m_aBmAlgrithm[alg]; }
	static int GetTx816AlgBitmapID(int alg) { return BmIdTX816_Algorithm[alg]; }
	//----------------------
	CFont* GetFont() { return &m_fFont; }
	CMidiOutTable* GetMidiOutTable() { 
		return &m_MidiOutTable; 
	}
	CMidiInTable* GetMidiInTable() { return &m_MidiInTable; }
	void InitBitMaps();
	void OnMenuSetupSelmidi();
	bool RegisterBitmapComboBoxClass();
	// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//--------------------------
	// Timer
	//--------------------------
	CMyMMtimer* GetTimer() { return &m_MMtimer; }
	//----------- Thread Stuff --------------------
	void CreatePlayerThread();
	UINT DoPlayThread();
	int KillPlayerThead();
	DWORD GetPlayerThreadID() { return m_idPlayerThread; }
	void PlayerThreadAddSong(CMsSong* pEQ);
	void PlayerThreadDeleteSong(CMsSong* pSong);
	void PlayerThreadEnableTimer(int Id, int enable);
	void PlayerThreadSetTempo(int QnPMinute);
	CMsSong* FindSongFromID(CMsSong* pEqTable, int ID);
	void AddToSongPlayingList(
		CMsSong** ppHead,
		CMsSong** ppTail,
		CMsSong* pEQtoAdd
	);
	void RemoveFromSongList(
		CMsSong** ppHead,
		CMsSong** ppTail,
		CMsSong* pESongToRemove
	);
protected:
	HMENU  m_hMDIMenu;
	HACCEL m_hMDIAccel;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAppAbout();
	afx_msg void OnFileNewStaff();
	void LoadSettings();
	void SaveSettings();
	afx_msg void OnSettingsTracksettinigs();
	afx_msg void OnSettingsMidiin();
	afx_msg void OnSettingsAdddispatcher();
	int UpdateColors();
	CMainFrame* GetMainFrame() { return (CMainFrame*)m_pMainWnd; }
	afx_msg void OnSettingsAddmidiOut();
	UINT CalculateTempo(int QuarterNotesPerMinute);
	afx_msg void OnNewNewtx816edit();
	afx_msg void OnNewNewlaunchpad();
	afx_msg void OnNewNewlaunchcontroledit();
	int FontPixelsToLogicalUnits(CDC* pDC, int heightInPixels);
	unsigned GetUniqueID();
	void Dump(FILE* pOut, const char* pData, int len, int StartAddress);
	char* IndentString(char* pDest, int StringLength, int Indent, int c = ' ');
	static CString* GetMidiNoteToNoteString(int MidiNote = 0) {
		if (MidiNote < 0 || MidiNote > 126)
			return nullptr;
		return &MidiNoteToNoteString[MidiNote];
	}
	static CString* GetTX816OpKeybordLevelScalingCurve(int Id = 0) {
		if (Id < 0 || Id > 3)
			return nullptr;
		return &TX816_OP_KEYBORD_Level_Scaling_Curve[Id];
	}
	static MIDI_STATUS* GetMidiStatusCommands() {
		return MidiStatusCommands;
	}
	//----------------------- Static Data----------------------------
};

//----------- function prototypes -------------
extern char* WcharToChar(char* pcDest, WCHAR* pwSrc, int len);
//----------- global variable -----------------------
extern CMidiSeqMSApp theApp;
//----------- handy macros -------------------
constexpr auto GETAPP = &theApp;
#define GETMAINFRAME (GETAPP->GetMainFrame())
#define GETTIMERCONTEXT (GETAPP->GetTimerContext())
#define GETTIMER	(GETAPP->GetTimer())