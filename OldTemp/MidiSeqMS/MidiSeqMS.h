
// MidiSeqMS.h : main header file for the MidiSeqMS application
//
#pragma once
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
    afx_msg void OnMenuMsFileSaveAs();
    afx_msg void OnUpdateMenuMsFileSaveAs(CCmdUI* pCmdUI);
};

class CMainFrame;

// CMidiSeqMSApp:
// See MidiSeqMS.cpp for the implementation of this class
//

class CMidiSeqMSApp : public CWinApp
{
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
	CMidiInfo m_MidiInfo;
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
	HMIDIOUT GetMidiOutHandle(int MidiID) { return  GetMidiOutTab().GetOutHandle(MidiID); }
//	HMIDIIN GetMidiIn() {return  GetMidi()->GetMidiInHandle(); }
	inline FILE* LogFile() { return m_pLog; }
	//------------- Midi Info -----------------------
	CMidiInfo* GetMidiInfo() { return &m_MidiInfo; }
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
	inline CMyBitmap* bmGetCBRestTypes(int RestType) {return &m_aBmCBRestTypes[RestType];}
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
	CMidiOutTable& GetMidiOutTab() { return m_MidiOutTable; }
	CMidiInTable& GetMidiInTable() { return m_MidiInTable; }
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
	static int  KeySigStringBitmapIDsTab[APP_NUM_KEYSIGNATURES + 1];
	static int TimeSigCBbMIDs[APP_NUM_TIMESIG];
	static int TimeSigBmIds[APP_NUM_TIMESIG];
	static int RestBmIdsTypes[APP_NUM_RESTTYPES];
	static int NoteBmIdsTypes[APP_NUM_NOTETYPES];
	static int RestComboBoxTypes[APP_NUM_RESTTYPES];
	static int DecorationsBmIds[APP_NUM_DECORATIONS];
	static int DecorationsBmCbIdsSel[APP_NUM_DECORATIONS];
	static int DecorationsBmCbIdsNotSel[APP_NUM_DECORATIONS];
	static int BlockOpBmIdsTypes[APP_NUM_BLOCKOPTYPES];
	static int AccidentalBmIdsTypes[APP_NUM_ACCIDENTALTYPES];
	static int AccidentalBmCBIdsTypes[APP_NUM_ACCIDENTALTYPES];
	static int MisStuffTypes[APP_NUM_MISC];
};
//----------- function prototypes -------------
extern char* WcharToChar(char* pcDest, WCHAR* pwSrc, int len);
//----------- global variable -----------------------
extern CMidiSeqMSApp theApp;
//----------- handy macros -------------------
constexpr auto GETAPP = &theApp;
#define GETMIDIINTABLE	(GETAPP->GetMidiInTable())
#define GETMIDIOUTTABLE	(GETAPP->GetMidiOutTab())
template<typename T>
constexpr auto GETMIDIOUTDEVICE(T DevID) { return (GETAPP->GetMidiOutTab().GetDevice(DevID)); }
template<typename T>
constexpr auto GETMIDIINDEVICE(T DevID) { return (GETAPP->GetMidiInTable().GetMidiInDevice(DevID)); }
#define GETTRACKINFO(t)	(GETAPP->GetMidiInfo()->GetTrack(t))
#define GETMIDIINFO (GETAPP->GetMidiInfo())
#define GETMAINFRAME (GETAPP->GetMainFrame())
#define GETTIMERCONTEXT (GETAPP->GetTimerContext())
#define GETTIMER	(GETAPP->GetTimer())