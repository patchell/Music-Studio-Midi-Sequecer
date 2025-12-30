
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
	inline static int MisStuffTypes[APP_NUM_MISC] = {
		IDB_MEASUREBAR,
		IDB_NOTETIE,
		IDB_LOUDNESS,
		IDB_TEMPO,
		IDB_INST_CHANGE,
		IDB_CB_MISC_ENDBAR,
		IDB_GLISSANDO
	};
	inline static int  KeySigStringBitmapIDsTab[APP_NUM_KEYSIGNATURES + 1] = {
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
	inline static int TimeSigCBbMIDs[APP_NUM_TIMESIG] = {
		IDB_CB_TIMESIG_2_2,
		IDB_CB_TIMESIG_3_2,
		IDB_CB_TIMESIG_2_4,
		IDB_CB_TIMESIG_3_4,
		IDB_CB_TIMESIG_4_4,
		IDB_CB_TIMESIG_5_4,
		IDB_CB_TIMESIG_6_8
	};
	inline static int TimeSigBmIds[APP_NUM_TIMESIG] = {
		IDB_TIMESIG_2_2,
		IDB_TIMESIG_3_2,
		IDB_TIMESIG_2_4,
		IDB_TIMESIG_3_4,
		IDB_TIMESIG_4_4,
		IDB_TIMESIG_5_4,
		IDB_TIMESIG_6_8
	};
	inline static int RestBmIdsTypes[APP_NUM_RESTTYPES] = {
		IDB_WHOLEREST,
		IDB_HALFREST,
		IDB_BITMAP_QUARTERREST,
		IDB_EIGHTHREST,
		IDB_SIXTEENTHREST,
		IDB_THIRTYSEC_REST
	};
	inline static int NoteBmIdsTypes[APP_NUM_NOTETYPES] = {
		IDB_NOTE_WHOLE,
		IDB_NOTE_HALF,
		IDB_NOTE_QUARTER,
		IDB_NOTE_EIGTH,
		IDB_NOTE_SIXTEENTH,
		IDB_NOTE_THIRTYSECOND
	};
	inline static int RestComboBoxTypes[APP_NUM_RESTTYPES] = {
		IDB_CB_REST_WHOLE,
		IDB_CB_REST_HALF,
		IDB_CB_REST_QUARTER,
		IDB_CB_REST_EIGTH,
		IDB_CB_REST_SIXTEENTH,
		IDB_CB_REST_THIRTYSECOND
	};
	inline static int DecorationsBmIds[APP_NUM_DECORATIONS] = {
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
	inline static int BlockOpBmIdsTypes[APP_NUM_BLOCKOPTYPES] = {
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

	//------------------------------------------
	static UINT PlayWorkerThread(LPVOID param);
	CFont m_fFont;
	CMyBitmap m_BmQuarterNoteEqQNPM;
	//--------- Misc -----------------------------
	CMyBitmap m_aBmMisc[APP_NUM_MISC];
	//---------------------
	// Time Signatures
	//---------------------
	CMyBitmap m_aBmCbTimeSig[APP_NUM_TIMESIG];
	CMyBitmap m_aBmTimeSig[APP_NUM_TIMESIG];
	//---------------------------------------
	// Key Signature
	//---------------------------------------
	CMyBitmap m_aBmCbKeySig[APP_NUM_KEYSIGNATURES + 1];
	//-------------------------------------
	// Instrument Bitmaps
	//------------------------------------
	CMyBitmap m_aBmInst[APP_NUM_INSTRUMENTS];
	//---------------------
	// Accidentals
	//---------------------
	CMyBitmap m_aBmAccidentalTypes[APP_NUM_ACCIDENTALTYPES];
	CMyBitmap m_aBmCBAccidentalTypes[APP_NUM_ACCIDENTALTYPES];
	//---------------------
	// Block Op Graphics
	//---------------------
	CMyBitmap m_aBmBlockOps[APP_NUM_BLOCKOPTYPES];
	//--------------
	// NOTE graphics
	//--------------
	CMyBitmap m_aBmNoteTypes[APP_NUM_NOTETYPES];
	//----------------
	// Rest Graphics
	//----------------
	CMyBitmap m_aBmRestTypes[APP_NUM_RESTTYPES];
	CMyBitmap m_aBmCBRestTypes[APP_NUM_RESTTYPES];
	//---------------------
	// Decorations
	//---------------------
	CMyBitmap m_aBmDecorations[APP_NUM_DECORATIONS];
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
	HMIDIOUT GetMidiOutHandle(int MidiID) { return  GetMidiOutTable()->GetOutHandle(MidiID); }
	//	HMIDIIN GetMidiIn() {return  GetMidi()->GetMidiInHandle(); }
	inline FILE* LogFile() { return m_pLog; }
	//------------- Midi Info -----------------------
//	CMidiInfo* GetMidiInfo() { return &m_MidiInfo; }
	//------------ Bitmap Symbols -------------------
	inline CMyBitmap* bmGetQNoteEqQNPM() { return &m_BmQuarterNoteEqQNPM; }
	inline int GetNumTimeSig() { return APP_NUM_TIMESIG; }
	inline CMyBitmap* bmGetTimeSig(INT TimeSig) { return &m_aBmTimeSig[TimeSig]; }
	inline CMyBitmap* bmGetCbTimeSig(INT TimSig) { return &m_aBmCbTimeSig[TimSig]; }
	inline int GetNumInstruments() { return APP_NUM_INSTRUMENTS; }
	inline CMyBitmap* bmGetIntrument(int Inst) { return &m_aBmInst[Inst]; }
	inline int GetNumNoteTypes() { return APP_NUM_NOTETYPES; }
	inline CMyBitmap* bmGetNoteType(int NoteType) { return &m_aBmNoteTypes[NoteType]; }
	inline int GetNumRestTypes() { return APP_NUM_RESTTYPES; }
	inline CMyBitmap* bmGetCBRestTypes(int RestType) { return &m_aBmCBRestTypes[RestType]; }
	inline CMyBitmap* bmGetRestType(int RestType) { return &m_aBmCBRestTypes[RestType]; }
	inline int GetRestTypeID(int restBmID) { return RestBmIdsTypes[restBmID]; }
	inline int GetNumDecorations() { return APP_NUM_DECORATIONS; }
	inline CMyBitmap* bmGetDecoration(int Dec) { return &m_aBmDecorations[Dec]; }
	inline int GetNumAccidentalTypes() { return APP_NUM_ACCIDENTALTYPES; }
	inline CMyBitmap* bmGetAccidentalType(int Accidnet) { return &m_aBmAccidentalTypes[Accidnet]; }
	inline CMyBitmap* bmGetCBAccidentalType(int Accident) { return &m_aBmCBAccidentalTypes[Accident]; }
	inline int GetNumBlockOps() { return APP_NUM_BLOCKOPTYPES; }
	inline CMyBitmap* bmGetBlockOpType(int BO) { return  &m_aBmBlockOps[BO]; }
	inline int GetNumMisc() { return APP_NUM_MISC; }
	inline CMyBitmap* bmGetMiscType(int Misc) { return &m_aBmMisc[Misc]; }
	inline int GetNumKeySigs() { return APP_NUM_KEYSIGNATURES; }
	inline CMyBitmap* bmGetCBKeySignature(int KS) { return &m_aBmCbKeySig[KS]; }
	inline int GetNumTX816LfoWaves() { return APP_TX816_NUM_LFO_WAVES; }
	inline CMyBitmap* bmGetTX816LFOWave(int LFOW) { return &m_aBmLfoWaves[LFOW]; }
	inline int GetNumAlgrithms() { return APP_TX816_NUM_ALG_BITMAPS; }
	inline CMyBitmap* bmGetTX816AlgBitmap(int alg) { return &m_aBmAlgrithm[alg]; }
	inline CFont* GetFont() { return &m_fFont; }
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
	//----------------------- Static Data----------------------------
	static int* GetAccidentalBmCBIdsTypes() { return AccidentalBmCBIdsTypes; }
	static int* GetDecorationsBmCbIdsNotSel() { return DecorationsBmCbIdsNotSel; }
	static int* GetDecorationsBmCbIdsSel() { return DecorationsBmCbIdsSel; }
	static int* GetRestBmIdsTypes() { return RestBmIdsTypes; }
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