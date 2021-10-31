
// MidiSeqMS.cpp : Defines the class behaviors for the application.
//

#include "pch.h"

//--------------------- Song Playing Helper Thread ---------------------------------

UINT CMidiSeqMSApp::PlayWorkerThread(LPVOID param)
{
	CMidiSeqMSApp* pMSMSSA = (CMidiSeqMSApp*)param;
	return pMSMSSA->DoPlayThread();
}

char*  WcharToChar(char* pcDest, WCHAR* pwSrc, int len)
{
	int loop = 1;
	int i;
	for (i = 0; (i < (len - 1)) && loop; ++i)
	{
		if (pwSrc[i] < 0x80 && pwSrc[i] > 0)
			pcDest[i] = (char)pwSrc[i];
		else if (pwSrc[i] == 0)
			loop = 0;
		else
			pcDest[i] = '?';
	}
	pcDest[i] = 0;
	return pcDest;
}


// CMidiSeqMSApp

BEGIN_MESSAGE_MAP(CMidiSeqMSApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMidiSeqMSApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW_SONG, &CMidiSeqMSApp::OnFileNewStaff)
	ON_COMMAND(ID_SETTINGS_TRACKSETTINIGS, &CMidiSeqMSApp::OnSettingsTracksettinigs)
	ON_COMMAND(ID_SETTINGS_MIDIIN, &CMidiSeqMSApp::OnSettingsMidiin)
	ON_COMMAND(ID_SETTINGS_ADDMIDI_OUT, &CMidiSeqMSApp::OnSettingsAddmidiOut)
	ON_COMMAND(ID_SETTINGS_ADDDISPATCHER, &CMidiSeqMSApp::OnSettingsAdddispatcher)
	ON_COMMAND(ID_NEW_NEWTX816EDIT, &CMidiSeqMSApp::OnNewNewtx816edit)
	ON_COMMAND(ID_NEW_NEWLAUNCHPAD, &CMidiSeqMSApp::OnNewNewlaunchpad)
	ON_COMMAND(ID_NEW_NEWLAUNCHCONTROLEDIT, &CMidiSeqMSApp::OnNewNewlaunchcontroledit)
END_MESSAGE_MAP()

// CMidiSeqMSApp construction

CMidiSeqMSApp::CMidiSeqMSApp() noexcept
{
	m_hMDIAccel = 0;
	m_hMDIAccel = 0;
	m_pLog = 0;
	pConsol = 0;
	m_hMDIMenu = 0;
	m_UniqueID = 0;
	m_hPlayThread = 0;
	m_idPlayerThread = 0;
	SetAppID(_T("MidiSeqMS.AppID.1.0"));
}

	// The one and only CMidiSeqMSApp object

CMidiSeqMSApp theApp;


// CMidiSeqMSApp initialization

BOOL CMidiSeqMSApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);
	//------------------------------------------------------------------
	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// You should modify this string to be something appropriate
	// such as the name of your company or organization
	//-----------------------------------------------------------------
	SetRegistryKey(_T("James Patchell"));
	//------------------------------------------
	// create a window class for the drop
	// up  and drop down control
	//------------------------------------------
	RegisterBitmapComboBoxClass();
	//---------------------------------------
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	//---------------------------------------
	m_fFont.CreateFontW(
		10,
		6,
		0,
		0,
		FW_DONTCARE,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		DEFAULT_PITCH,
		_T("ARIAL")
	);
	LoadSettings();
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = (CMDIFrameWnd*) pFrame;
	//---------------------------------------
	// create main MDI frame window
	//---------------------------------------
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	//---------------------------------------
	// try to load shared MDI menus and accelerator table
	//add additional member variables and load calls for
	//	additional menu types your application may need
	//---------------------------------------
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_MidiSeqMSTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_MidiSeqMSTYPE));
	//---------------------------------------
	// Open up a "DOS" window to print debug
	// Information to.
	//---------------------------------------
	fopen_s(&m_pLog, "Log.txt", "w");
	if(m_pLog)fprintf(m_pLog, "-------- MisiSeqMS Start -------\n");
	AllocConsole();
	freopen_s(&pConsol, "CONOUT$", "w", stdout);
	printf("Ready\n");
	//--------------------------------------
	// The main window has been initialized, so show and update it
	//--------------------------------------
	pFrame->OnInitialUpdate();
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();
	//--------------------------------------
	// Load all of the bitmaps from the
	// windows resource file
	//---------------------------------------
	InitBitMaps();
	//--------------------------------------
	// Initialize Midi Information
	//--------------------------------------
	m_MidiInfo.Create(pFrame->GetDC());
	//---------------------------------------
	// 	   Create Song Player Thread
	//---------------------------------------
	m_ThredRunningEvent.Create(CString(""));
	CreatePlayerThread();
	int Status = m_ThredRunningEvent.Pend();
	if (Status != WAIT_OBJECT_0)
	{
		printf("Event Error %d\n", Status);
	}
	//------------ Create Timer ------------------------
	m_MMtimer.Create(
		m_idPlayerThread,
		WM_THREAD_PLAYER_TIMER,
		0,
		0,
		1,
		1
	);
	m_MMtimer.SetTempo(CalculateTempo(68));
	//------------ Select midi In/Out -----------------
	OnMenuSetupSelmidi();
	return TRUE;
}

int CMidiSeqMSApp::ExitInstance()
{
	if (m_hMDIMenu != nullptr)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != nullptr)
		FreeResource(m_hMDIAccel);

	KillPlayerThead();

	fprintf(m_pLog, "------ Exit MidiSeqMS-------\n");
	AfxOleTerm(FALSE);
	fclose(m_pLog);
	fclose(pConsol);
	return CWinApp::ExitInstance();
}

// CMidiSeqMSApp message handlers


// CMidiSeqMSApp message handlers

BOOL CMidiSeqMSApp::RegisterBitmapComboBoxClass()
{
	BOOL rV = TRUE;
	WNDCLASS windowclass;

	windowclass.lpfnWndProc = ::DefWindowProc;
	windowclass.lpszClassName = _T("BitmapComboBox");
	windowclass.hCursor = 0;
	windowclass.cbClsExtra = 0;
	windowclass.cbWndExtra = 0;
	windowclass.hbrBackground = 0;
	windowclass.hIcon = 0;
	windowclass.hInstance = AfxGetInstanceHandle();
	windowclass.style = 0;
	windowclass.lpszMenuName = 0;

	if (!AfxRegisterClass(&windowclass))
	{
		rV = FALSE;
		printf("!!!!!!!!! Could Not Register Drop Down !!!!!!!!!!\n");
	}
	return rV;
}

void CMidiSeqMSApp::InitBitMaps()
{
	int i;
	int n;

	//---------------------------------------
	// 	   Music Editor Bitmaps
	//----------------------------------------
	// Initialize Misc Stuff Combo box
	//-----------------------------------------
	n = GetNumMisc();
	for (i = 0; i < n; ++i)
		m_aBmMisc[i].LoadBitmapW(MisStuffTypes[i]);
	//-------------- Block Operations -------
	n = GetNumBlockOps();
	for (i = 0; i < n; ++i)
		m_aBmBlockOps[i].LoadBitmapW(BlockOpBmIdsTypes[i]);
	//--------------Accidental Type-----------------
	n = GetNumAccidentalTypes();
	for (i = 0; i < n; ++i)
	{
		m_aBmAccidentalTypes[i].LoadBitmapW(AccidentalBmIdsTypes[i]);
		m_aBmCBAccidentalTypes[i].LoadBitmapW(AccidentalBmCBIdsTypes[i]);
	}
	//--------------Decorations--------------
	n = GetNumDecorations();
	for (i = 0; i < n; ++i)
		m_aBmDecorations[i].LoadBitmapW(DecorationsBmIds[i]);
	//------------Rest Types--------------------
	n = GetNumRestTypes();
	for (i = 0; i < n; ++i)
	{
		m_aBmRestTypes[i].LoadBitmapW(RestBmIdsTypes[i]);
		m_aBmCBRestTypes[i].LoadBitmapW(RestComboBoxTypes[i]);
	}
	//-------------Note Types---------------
	n = GetNumNoteTypes();
	for (i = 0; i < n; ++i)
		m_aBmNoteTypes[i].LoadBitmapW(NoteBmIdsTypes[i]);
	//----------- Time Signature ---------------
	n = GetNumTimeSig();
	for (i = 0; i < n; ++i)
	{
		m_aBmTimeSig[i].LoadBitmapW(TimeSigBmIds[i]);
		m_aBmCbTimeSig[i].LoadBitmapW(TimeSigCBbMIDs[i]);
	}
	//------------- Key Signature ----------------
	n = GetNumKeySigs();
	for (i = 0; i < n; ++i)
	{
		m_aBmCbKeySig[i+1].LoadBitmapW(KeySigStringBitmapIDsTab[i+1]);
	}
	//----------------------------------------
	// Bitmaps for TX816
	//----------------------------------------
	// LFO waveforms
	//-----------------------------------------

	for (i = 0; i < APP_TX816_NUM_LFO_WAVES; ++i)
		m_aBmLfoWaves[i].LoadBitmapW(TX816_LFO_WaveBmAPP_TX816_NUM_LFO_WAVES[i]);

	//--------------------------------------------
	// Algrithmns
	//--------------------------------------------

	for ( i = 0; i < APP_TX816_NUM_ALG_BITMAPS; i++)
	{
		m_aBmAlgrithm[i].LoadBitmapW(TX816_AlgrithmBitmapID[i]);
	}
}

void CMidiSeqMSApp::OnMenuSetupSelmidi()
{
	CDlgSelectMidiOut dlg;

	if (IDOK == dlg.DoModal())
	{
		UINT MidiOutId = dlg.GetSelection();
		CMidiOutDevice* pMOD = new CMidiOutDevice;
		pMOD->Open(MidiOutId);
		int DeviceId = m_MidiOutTable.AddItem(pMOD);
		for (int TrackID = 1; TrackID < 16; ++TrackID)
		{
			if (dlg.GetInstrumentSel(TrackID))
			{
				GETTRACKINFO(TrackID).SetMidiOutDeviceID(DeviceId);
				GETMIDIOUTTABLE.GetDevice(DeviceId).SetName(dlg.GetNameString());
			}
		}
	}
}

// App command to run the dialog
void CMidiSeqMSApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CMidiSeqMSApp::LoadSettings()
{
	CMsTrack::GetSettings();
}


void CMidiSeqMSApp::SaveSettings()
{
}


void CMidiSeqMSApp::OnSettingsTracksettinigs()
{
	CDlgMidiInfo* pDlg = new CDlgMidiInfo;

	pDlg->DoModal();
	delete pDlg;
}

void CMidiSeqMSApp::OnSettingsMidiin()
{
	CDlgSelectMidiIn dlg;
	char* s = new char[256];
	MIDIINCAPS MinCaps;

	if (IDOK == dlg.DoModal())
	{
		UINT MidiInId = dlg.GetSelection();
		midiInGetDevCaps(MidiInId, &MinCaps, sizeof(MIDIINCAPS));
		CMidiInDevice* pMID = new CMidiInDevice;
		pMID->Open(MidiInId);
		m_MidiInTable.AddItem(pMID);
	}
	delete[] s;
}

int CMidiSeqMSApp::UpdateColors()
{
	GetMainFrame()->UpdateColors();
	return 0;
}

//-------------------------------------------
// CAboutDlg dialog used for App About
//-------------------------------------------


CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


void CMidiSeqMSApp::OnSettingsAddmidiOut()
{
	OnMenuSetupSelmidi();
}

//--------------------------------------------
// Thread Stuff
//--------------------------------------------

void CMidiSeqMSApp::PlayerThreadAddSong(CMsSong* pMsSong)
{
	::PostThreadMessage(
		m_idPlayerThread,
		WM_THREAD_PLAYER_ADD_SONG,
		pMsSong->GetSongId(),
		(LPARAM)pMsSong
	);
}

void CMidiSeqMSApp::PlayerThreadDeleteSong(CMsSong* pSong)
{
	::PostThreadMessage(
		m_idPlayerThread,
		WM_THREAD_PLAYER_DELETE_SONG,
		pSong->GetSongId(),
		(LPARAM)pSong
	);
}

void CMidiSeqMSApp::PlayerThreadEnableTimer(int EQid, int enable)
{
	::PostThreadMessage(
		m_idPlayerThread,
		WM_THREAD_PLAYER_ENABLE_TIMER,
		enable,
		EQid
	);
}

void CMidiSeqMSApp::PlayerThreadSetTempo(int QnPMinute)
{
	::PostThreadMessageW(
		m_idPlayerThread,
		WM_THREAD_PLAYER_TEMPO,
		QnPMinute,
		0
	);
}

int CMidiSeqMSApp::KillPlayerThead()
{
	::PostThreadMessageW(
		m_idPlayerThread, 
		WM_QUIT, 
		0, 
		0
	);
	if (m_hPlayThread)
	{
		::WaitForSingleObject(m_hPlayThread, INFINITE);

		::CloseHandle(m_hPlayThread);
		m_hPlayThread = NULL;
	}

	return 0;
}

void CMidiSeqMSApp::RemoveFromSongList(
	CMsSong** ppHead,
	CMsSong** ppTail,
	CMsSong* pSongToRemove
)
{
	if (pSongToRemove->GetSongListPrev() == 0)
	{
		if (pSongToRemove->GetSongListNext())
		{
			*ppHead = pSongToRemove->GetSongListNext();
			(*ppHead)->SetSongListPrev(0);
		}
		else
		{
			//-----------------------
			// All Gone
			//-----------------------
			*ppHead = 0;
			*ppTail = 0;
		}
	}
	else if (pSongToRemove->GetSongListNext() == 0)
	{
		*ppTail = pSongToRemove->GetSongListPrev();
		(*ppTail)->SetSongListNext(0);
	}
	else
	{
		pSongToRemove->GetSongListPrev()->SetSongListNext(pSongToRemove->GetSongListNext());
		pSongToRemove->GetSongListNext()->SetSongListPrev(pSongToRemove->GetSongListPrev());
	}
	pSongToRemove->SetSongListNext(0);
	pSongToRemove->SetSongListPrev(0);
}

void CMidiSeqMSApp::AddToSongPlayingList(
	CMsSong**ppHead,
	CMsSong** ppTail,
	CMsSong* pSongToAdd
)
{
	if (*ppHead == 0)
	{
		*ppHead = pSongToAdd;
		*ppTail = pSongToAdd;
	}
	else
	{
		/// add new Song to the tail
		pSongToAdd->SetSongListPrev(*ppTail);
		(*ppTail)->SetSongListNext(pSongToAdd);
		*ppTail = pSongToAdd;
	}

}

UINT CMidiSeqMSApp::DoPlayThread()
{
	DWORD TempoQNperS;
	char* s = new char[256];
	int count = 0;
	BOOL bF;	//message loop flag
	MSG msg;
	int TimerEnable = 0;
	int Total = 0;
	CMsSong* pSongPlayingListHead = 0;
	CMsSong* pSongPlayingListTail = 0;
	CMsSong* pMS = 0;
	int TotalSongsPlaying = 0;
	int SongID = 0;
	int doLoop = 0;

	printf("Player Thread Ready\n");
	//-------------------------------------
	//indicate that the thread is running
	//to the main thread
	//--------------------------------------
	m_ThredRunningEvent.Post();
	//---------------------------------------
	// Execute main part of thread
	//---------------------------------------
	while ((bF = GetMessage(&msg, NULL, 0, 0)) != FALSE)
	{
		switch (msg.message)
		{
		case WM_THREAD_PLAYER_TIMER:
			//---------------------------------------
			// Get the head of the Song list
			// and keep looping until we get to the
			// end of the list
			//----------------------------------------
			pMS = pSongPlayingListHead;
			while (pMS)
			{
				if (pMS->GetPlaySongTimerEnable())
				{
					if (0 == pMS->Ticker())
					{
						pMS->GetStaffChildView()->PostMessageW(
							WM_CHILDVIEW_PLAYERTHREAD, 
							CHILD_VIEW_PLAYERTHREAD_SONGDONE, 
							0
						);
						pMS->SetPlaySongTimerEnable(0);
					}
				}
				pMS = pMS->GetSongListNext();
			}
			break;
		case WM_THREAD_PLAYER_TEMPO:	//change tempo
			//------------------------------------
			// every body HAS to run at the same
			// tempo
			//------------------------------------
			TempoQNperS = msg.wParam;
			GETTIMER->SetTempo(CalculateTempo(TempoQNperS));
			break;
		case WM_THREAD_PLAYER_ADD_SONG:
			++TotalSongsPlaying;
			pMS = (CMsSong*)msg.lParam;
			AddToSongPlayingList(&pSongPlayingListHead, &pSongPlayingListTail,pMS);
			pMS->GetAddSongCompleteEV().Post();
			break;
		case WM_THREAD_PLAYER_DELETE_SONG:
			SongID = msg.wParam;
			//------------------------------------------
			// From the Event queue ID, locate the
			// the EventQueue object, if any
			//------------------------------------------
			pMS = FindSongFromID(pSongPlayingListHead,SongID);
			if (pMS)
			{
				--TotalSongsPlaying;	///decement event count
				RemoveFromSongList(&pSongPlayingListHead, &pSongPlayingListTail, pMS);
			}
			pMS->GetDelSongCompleteEV().Post();
			break;
		case WM_THREAD_PLAYER_ENABLE_TIMER:
			//-----------------------------------
			// if msg.wpram is true, enable timer
			// if msg.wpram is false, disable timer
			// msg.lparam hold the EventQueue ID
			//------------------------------------
			pMS = FindSongFromID(pSongPlayingListHead, msg.lParam );
			if (msg.wParam)
			{
				pMS->SetPlaySongTimerEnable(1);
				if (!TimerEnable++)
					GETTIMER->Enable(1);
				pMS->GetEnableTimerCompleteEV().Post();
			}
			else
			{
				--TimerEnable;
				if (!TimerEnable)
					GETTIMER->Enable(0);
				pMS->SetPlaySongTimerEnable(0);
				pMS->GetDisableTimerCompleteEV().Post();
			}
			break;
		}
	}
	return 0;
}

void CMidiSeqMSApp::CreatePlayerThread()
{
	//------------------------------
	// Set up thread
	//------------------------------
	m_hPlayThread = ::CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)(CMidiSeqMSApp::PlayWorkerThread),
		this,
		0,
		&m_idPlayerThread
	);
}

CMsSong* CMidiSeqMSApp::FindSongFromID(CMsSong* pSongTable, int ID)
{
	CMsSong* pMS = pSongTable;
	int loop = 1;
	while (pMS && loop)
	{
		if (ID == pMS->GetSongId())
			loop = 0;
		else
			pMS = pMS->GetSongListNext();
	}
	return pMS;
}

//-------------------------------------------------
// Timer Stuff
//-------------------------------------------------


//-----------------------------------------------
//	CalculateTempo
//
//	Calculates the value used for a timer in
//	order to get the desired quarter note per
//	minute tempo
//
// Parameters:
//	QuarterNotesPerMinute...desired tempo
//
//	Returns a UINT in mS/Tick
// 
// 
//----------------------------------------------

UINT CMidiSeqMSApp::CalculateTempo(int QuarterNotesPerMinute)
{
	UINT mSpTick;
	//----------------------------------------
	// Divide the constant 1250 by
	// Quarter Notes per Minute.
	// The constat 1250 is based on 1mS
	// resolution, and a quarter note will
	// be represented by 48 ticks per
	// quarter note.
	// mS/Tick = min/Qn * 60 sec/min * 1000 ms/sec / 48 Ticks/Qn
	// mS/Tick = 1250
	//----------------------------------------
	mSpTick = 1250 / QuarterNotesPerMinute;
	return mSpTick;
}

void CMidiSeqMSApp::OnSettingsAdddispatcher()
{
	CSelectStringDlg dlg;
	CDlgMidiInDispatchEditor dlgDispatch;
	UINT nMidiInPorts;
	UINT i;

	nMidiInPorts = GetMidiInTable().GetNumDevices();;
	dlg.SetNumStrings(nMidiInPorts);
	for ( i = 0; i < nMidiInPorts; i++)
	{
		dlg.SetSelectionString(i, GETMIDIINTABLE.GetMidiInDevice(i)->GetPortName());
	}
	if (IDOK == dlg.DoModal())
	{
		int DeviceID = dlg.GetSelection();
		dlgDispatch.SetDeviceID(DeviceID);
		dlgDispatch.DoModal();
	}
}

BOOL CAboutDlg::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}


void CMidiSeqMSApp::OnFileNewStaff()
{
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	CChildFrameStaff* pChildFrame;

	// create a new MDI child window

	pChildFrame = (CChildFrameStaff*)pMainFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrameStaff),
		IDR_MidiSeqMSTYPE,
		m_hMDIMenu,
		m_hMDIAccel);
	pMainFrame->AddChild(pChildFrame);
	pChildFrame->GetView()->OnInitialUpdate();
}

void CMidiSeqMSApp::OnNewNewtx816edit()
{
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	CChildFrameYamahaTX816* pChildFrame;

	// create a new MDI child window

	pChildFrame = (CChildFrameYamahaTX816*)pMainFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrameYamahaTX816),
		IDR_MidiSeqMSTYPE,
		m_hMDIMenu,
		m_hMDIAccel);
	pMainFrame->AddChild(pChildFrame);
	pChildFrame->GetView()->OnInitialUpdate();
}


void CMidiSeqMSApp::OnNewNewlaunchpad()
{
}


void CMidiSeqMSApp::OnNewNewlaunchcontroledit()
{
	// TODO: Add your command handler code here
}

int CMidiSeqMSApp::FontPixelsToLogicalUnits(CDC* pDC, int heightInPixels)
{
	int logpix = pDC->GetDeviceCaps(LOGPIXELSY);
	int FontHeight = -((heightInPixels * logpix) / 76);
	return FontHeight;

}

unsigned CMidiSeqMSApp::GetUniqueID()
{
	return ++m_UniqueID;
}


