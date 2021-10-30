
#include "pch.h"

//class CChildView;

//-------------------------------------------------
// MidiInProc
//-------------------------------------------------
#define MIDIMSG_MIDI_IN		0x100

DWORD CMidi::MidiWorker(LPVOID pP)
{
	CMidi* pSMI = (CMidi*)pP;
	return pSMI->MidiInThread();
}

/*
//-------------------------------------------------
// MidiInProc
//-------------------------------------------------

void CALLBACK seqMidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInst, DWORD dwP1, DWORD dwP2)
{
	CWnd* pDestWnd = (CWnd *) (dwInst);
	switch (wMsg)
	{
	case MIM_CLOSE:
		break;
	case MIM_DATA:
		pCV->PostMessageW(
			WM_SHORTMIDIMSG, 
			dwP1, 
			dwP2
		);	//??
		pCV->PostMessageW(
			WM_MIDIIN, 
			dwP1, 
			dwP2
		);		//??
		break;
	case MIM_ERROR:
		break;
	case MIM_LONGDATA:
	{
		MIDIHDR* pMHdr = (MIDIHDR*)dwP1;	//??
		pCV->PostMessageW(
			WM_MIDIIN_LONG, 
			dwP1, 
			dwP2
		);
	}
	break;
	case MIM_LONGERROR:
		break;
	case MIM_MOREDATA:
		break;
	case MIM_OPEN:
		break;
	}
}

*/

CMidi::CMidi()
{
	m_pDestWindow = 0;
	m_hMidiIn = 0;
	m_hMidiOut = 0;
	m_nThreadID = 0;
	m_hThreadHandle = 0;
	memset(&m_MidiIn1, 0, sizeof(MIDIHDR));
	memset(&m_MidiIn2, 0, sizeof(MIDIHDR));
}

CMidi::~CMidi()
{
	KillThead();
}

void CMidi::Create(int OutId, int InId)
{
	int MSerror;
	//-------------------------------------
	// Create Worker Thread
	//-------------------------------------
	m_hThreadHandle = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)CMidi::MidiWorker,
		this,
		0,
		&m_nThreadID
	);
	printf("Open Up Midi\n");
	//---------------------------------------------
	// Open Up Midi In Port
	//---------------------------------------------
	if (InId >= 0)
	{
		MSerror = midiInOpen(
			&m_hMidiIn, 
			InId,
			DWORD_PTR(m_nThreadID),
			DWORD_PTR(this),
			CALLBACK_THREAD
		);
		if (MSerror == MMSYSERR_NOERROR)
		{
			MSerror = midiInPrepareHeader(m_hMidiIn, &m_MidiIn1, sizeof(MIDIHDR));
			if (MSerror != MMSYSERR_NOERROR) printf("1 ERROR %d\n", MSerror);
			MSerror = midiInPrepareHeader(m_hMidiIn, &m_MidiIn2, sizeof(MIDIHDR));
			if (MSerror != MMSYSERR_NOERROR) printf("2 ERROR %d\n", MSerror);

			m_MidiIn1.lpData = new char[4096];
			m_MidiIn1.dwBufferLength = 4096;

			m_MidiIn2.lpData = new char[4096];
			m_MidiIn2.dwBufferLength = 4096;

			MSerror = midiInStart(m_hMidiIn);
			if (MSerror != MMSYSERR_NOERROR) printf("ERROR %d\n", MSerror);

		}
		else
			printf("Could not open midi\n");
	}

	//---------------------------------------------
	// Open Midi Out Port
	//---------------------------------------------
	if (OutId >= 0)
	{
		midiOutOpen(
			&m_hMidiOut,
			OutId,
			NULL,
			NULL,
			CALLBACK_NULL
		);
	}
}


DWORD CMidi::MidiInThread()
{
	UINT rV = 0;
	MSG Msg;
	int Cmd;
	int Vel;
	int Note;
	int Chan;
	
	printf("Midi Thread Started\n");
	while (GetMessageW(&Msg, NULL, 0, 0))
	{
		switch (Msg.message)
		{
		case MM_MIM_DATA:
			Chan = CHAN(Msg.lParam);
			Note = NOTE(Msg.lParam);
			Vel = VEL(Msg.lParam);
			Cmd = CMD(Msg.lParam);
			DecodeShortMidiMessageShort(Chan, Cmd, Note, Vel);
			break;
		case MM_MIM_LONGERROR:
			break;
		case MM_MIM_MOREDATA:
			break;
		case MM_MIM_OPEN:
			break;
		case MM_MIM_CLOSE:
			break;
		case WM_KILLTHREAD:
			// cleanup
			break;
		default:
			DispatchMessage(&Msg);
			break;
		}
	}
	return rV;
}

int CMidi::KillThead()
{
	//-------------------------------------------
	// Close the midi ports
	//-------------------------------------------
	midiInStop(m_hMidiIn);
	if (m_hMidiIn) midiInClose(m_hMidiIn);
	if (m_hMidiOut) midiOutClose(m_hMidiOut);
	delete[] m_MidiIn1.lpData;
	delete[] m_MidiIn2.lpData;
	//------------------------------------------
	// Kill the thread
	//------------------------------------------
	PostThreadMessageW(m_nThreadID, WM_KILLTHREAD, 0, 0);
	PostThreadMessageW(m_nThreadID, WM_QUIT, 0, 0);
	//------------------------------------------
	// Wait for thread to die
	//-----------------------------------------
	if (m_hThreadHandle)
	{
		WaitForSingleObject(m_hThreadHandle, INFINITE);

		CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
	}
	return 0;
}


void CMidi::DecodeShortMidiMessageShort(int Chan, int Cmd, int Note, int Vel)
{
	static int NoteCountDown = 0;
	static int NoteQueue[20];
	int Value;
	printf("*** Chan = %d  Cmd = %2x  Note = %d  Vel = %d ***\n", Chan, Cmd, Note, Vel);
	switch (Cmd)
	{
	case MIDI_NOTEOFF:
		//		printf("Note Off\n");
		if (Chan == 0)	// midi channel 1
		{
			--NoteCountDown;
		}
		else if (Chan == 9)	// midi channel 10
		{
		}
		break;
	case MIDI_NOTEON:
		printf("Note On %d\n", NoteCountDown);
		if (Chan == 0)	//our midi channel?
		{
			if (m_pDestWindow) m_pDestWindow->PostMessage(MAKEMIDIMSG(Cmd, Chan, Note, Vel), 0);
			NoteQueue[NoteCountDown++] = Note;
		}
		else if (Chan == 9)
		{
			switch (Note)
			{
			case LK25_BUTTON_TOP1:	//WHOLE-Half-quater-eigth-sixteenth-thirtysecond NOTE
				if (++m_EditNote >= MIDIEDIT_THIRTYSECOND + 1)
					m_EditNote = MIDIEDIT_WHOLE;
				if (m_pDestWindow)
					m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_NOTE, m_EditNote);
				break;
			case LK25_BUTTON_TOP2:	//WHOLE-Half-quater-eigth-sixteenth-thirtysecond REST	
				if (++m_EditRest >= MIDIEDIT_THIRTYSECOND + 1)
					m_EditRest = MIDIEDIT_WHOLE;
				if (m_pDestWindow)
					m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_REST, m_EditRest);
				break;
			case LK25_BUTTON_TOP3:	//Accent
				m_EditAccent ^= 1;
				if (m_pDestWindow)
					m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_DECORATIONS_ACCENT, m_EditAccent);
				break;
			case LK25_BUTTON_TOP4:	//Sharp-Flat-Natural-None
				if (++m_EditAccidentals >= MIDIEDIT_ACCIDNETALS_NATURAL + 1)
					m_EditAccidentals = MIDIEDIT_ACCIDENTALS_NONE;
				if (m_pDestWindow)
					m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_ACCIDENTALS, m_EditAccidentals);
				break;
			case LK25_BUTTON_TOP5:	//Tripplet-Dotted-Niether
				if (++m_EditDecorations >= MIDIEDIT_DECORATIONS_TRIPLET + 1)
					m_EditDecorations = MIDIEDIT_DECORATIONS_NONE;
				if (m_pDestWindow)
					m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_DECORATIONS, m_EditDecorations);
				break;
			case LK25_BUTTON_TOP6:	//Instruments 1-> 15
				if (++m_EditInstrument > MIDIEDIT_INST15)
					m_EditInstrument = MIDIEDIT_INST1;
				if(m_pDestWindow)
					m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_INSTRUMENT, m_EditInstrument);
				break;
			case LK25_BUTTON_TOP7:	//Key Signatures- C, A, B, etc
				if (++m_EditKeySignature > MIDIEDIT_KEY_CFMAJ)
					m_EditKeySignature = MIDIEDIT_KEY_CMAJ;
				if (m_pDestWindow)
					m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_KEYSIGNATURE, m_EditKeySignature);
				break;
			case LK25_BUTTON_TOP8:	//Time Signature 2/2 3/1 2/4 etc
				if (++m_EditTimeSignature > MIDIEDIT_TIMESIG_6_8)
					m_EditTimeSignature = MIDIEDIT_TIMESIG_2_2;
				if (m_pDestWindow)
					m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_TIMESIGNATURE, m_EditTimeSignature);
				break;
			case LK25_BUTTON_BOT1:	// Misc. functions
				if (++m_EditMisc > MIDIEDIT_MISC_INST_CHANGE)
					m_EditMisc = MIDIEDIT_MISC_MEASUREBAR;
				if (m_pDestWindow)
					m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_MISC, m_EditMisc);
				break;
			case LK25_BUTTON_BOT2:	
				break;
			case LK25_BUTTON_BOT3:	
				break;
			case LK25_BUTTON_BOT4:	
				break;
			case LK25_BUTTON_BOT5:	
				break;
			case LK25_BUTTON_BOT6:	
				break;
			case LK25_BUTTON_BOT7:
				break;
			case LK25_BUTTON_BOT8:
				break;
			}
		}
		break;
	case MIDI_POLYPRESS:
		break;
	case MIDI_CTRLCHNG:
		if (Chan == 0)
		{
			//			CheckButtonMessages(Note, Vel);
			switch (Note)
			{
			case LK25_KNOB_TOP1:	//Attack
				break;
			case LK25_KNOB_TOP2:	//Decay
				break;
			case LK25_KNOB_TOP3:	//Sustain
				break;
			case LK25_KNOB_TOP4:	//Release
				break;
			case LK25_KNOB_TOP5:	//filter Q
				break;
			case LK25_KNOB_TOP6:	//filter F
				break;
			case LK25_KNOB_TOP7:	//pwm
				break;
			case LK25_KNOB_TOP8:
				break;
			}
		}
		break;
	case MIDI_CHNLPRESS:
		break;
	case MIDI_PITCHBEND:
		Value = Vel;
		break;
	case MIDI_PGMCHANGE:
		break;
	case MIDI_CLOCK:
		break;
	case MIDI_START:
		break;
	case MIDI_STOP:
		break;
	case MIDI_CONTINUE:
		break;
	}
}

void CMidi::UpdateScreenControls()
{
	if (m_pDestWindow)
	{
		m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_NOTE, m_EditNote);
		m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_REST, m_EditRest);
		m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_DECORATIONS_ACCENT, m_EditAccent);
		m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_DECORATIONS, m_EditDecorations);
		m_pDestWindow->PostMessageW(WM_MIDI_EDIT, MIDIEDIT_ACCIDENTALS, m_EditAccidentals);
	}
}


