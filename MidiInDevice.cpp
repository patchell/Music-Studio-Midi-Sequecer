//-----------------------
#include "pch.h"

UINT CMidiInDevice::MidiInWorkerThread(LPVOID pP)
{
	//---------------------------------
	// the parameter pP is apointer
	// to a CMidiInDevice object.
	// Each object will have it's very
	// own thread by using parameter
	//----------------------------------
	CMidiInDevice* pMID = (CMidiInDevice*)pP;
	return pMID->MidiInThread();
}

CMidiInDevice::CMidiInDevice()
{
	m_hMidiIn = 0; 
	m_hMidiInThreadHandle = 0;
	m_nMidiInThreadID = 0;
	m_pDestWindow = 0;
	m_hMidiIn = 0;
	m_EditNote = 0;
	m_EditRest = 0;
	m_EditAccent = 0;
	m_EditDecorations = 0;
	m_EditAccidentals = 0;
	m_EditInstrument = 0;
	m_EditTimeSignature = 0;
	m_EditKeySignature = 0;
	m_EditMisc = 0;
	m_pDestWindow = 0;
	m_pHead = 0;
	m_pTail = 0;
	memset(&m_mhMidiIn2, 0, sizeof(MIDIHDR));
	memset(&m_mhMidiIn1, 0, sizeof(MIDIHDR));
}

CMidiInDevice::~CMidiInDevice()
{
}

MMRESULT CMidiInDevice::Open(int InId)
{
	MMRESULT MSerror = MMSYSERR_NOERROR;
	MIDIINCAPS MinCaps;
	char* s = new char[512];
	//-------------------------------------
	// Create Worker Thread
	//-------------------------------------
	m_hMidiInThreadHandle = ::CreateThread(
		nullptr,
		0,
		(LPTHREAD_START_ROUTINE)CMidiInDevice::MidiInWorkerThread,
		this,
		0,
		&m_nMidiInThreadID
	);
	//---------------------------------------------
	// Open Up Midi In Port
	//---------------------------------------------
	if (InId >= 0)
	{
		MSerror = ::midiInOpen(
			&m_hMidiIn,
			InId,
			DWORD_PTR(m_nMidiInThreadID),
			DWORD_PTR(this),
			CALLBACK_THREAD
		);
		midiInGetDevCaps(InId, &MinCaps, sizeof(MIDIINCAPS));
		SetPortName(MinCaps.szPname);
		if (MSerror == MMSYSERR_NOERROR)
		{
			MSerror = midiInPrepareHeader(m_hMidiIn, &m_mhMidiIn1, sizeof(MIDIHDR));
			if (MSerror != MMSYSERR_NOERROR) printf("1 ERROR %d\n", MSerror);
			MSerror = midiInPrepareHeader(m_hMidiIn, &m_mhMidiIn2, sizeof(MIDIHDR));
			if (MSerror != MMSYSERR_NOERROR) printf("2 ERROR %d\n", MSerror);

			m_mhMidiIn1.lpData = new char[4096];
			m_mhMidiIn1.dwBufferLength = 4096;

			m_mhMidiIn2.lpData = new char[4096];
			m_mhMidiIn2.dwBufferLength = 4096;

			MSerror = midiInStart(m_hMidiIn);
			if (MSerror != MMSYSERR_NOERROR) printf("ERROR %d\n", MSerror);

		}
		else
			printf("Could not open midi\n");
	}
	return MSerror;
}

UINT CMidiInDevice::MidiInThread()
{
	UINT rV = 0;
	MSG Msg;
	int Cmd;
	int Vel;
	int Note;
	int Chan;
	int Enumeration;

	static int Count = 0;

	printf("Midi In Thread %d Started\n",++Count);
	Enumeration = Count;
	while (GetMessageW(&Msg, NULL, 0, 0))
	{
		switch (Msg.message)
		{
		case MM_MIM_DATA:
			//---------------------------
			// wparam...Midi Port Handel
			// lparam...Midi Message
			//---------------------------
			Chan = CHAN(Msg.lParam);
			Note = NOTE(Msg.lParam);
			Vel = VEL(Msg.lParam);
			Cmd = CMD(Msg.lParam);
			printf("Device %d: Chan=%d  CMD=%02x Note=%d Vel=%d\n", Enumeration, Chan, Cmd, Note, Vel);
			DispatchShortMessage(Msg.wParam, Msg.lParam);
			break;
		case MM_MIM_LONGERROR:
			break;
		case MM_MIM_LONGDATA:
			GetMidiLongComplete().Post();
			break;
		case MM_MIM_MOREDATA:
			printf("More Data?\n");
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

int CMidiInDevice::KillThread()
{
	//-------------------------------------------
	// Close the midi in port
	//-------------------------------------------
	midiInStop(m_hMidiIn);
	if (m_hMidiIn) midiInClose(m_hMidiIn);
	if(m_mhMidiIn1.lpData) delete[] m_mhMidiIn1.lpData;
	if(m_mhMidiIn2.lpData) delete[] m_mhMidiIn2.lpData;
	//------------------------------------------
	// Kill the thread
	//------------------------------------------
	PostThreadMessageW(m_nMidiInThreadID, WM_KILLTHREAD, 0, 0);
	PostThreadMessageW(m_nMidiInThreadID, WM_QUIT, 0, 0);
	//------------------------------------------
	// Wait for thread to die
	//-----------------------------------------
	if (m_hMidiInThreadHandle)
	{
		WaitForSingleObject(m_hMidiInThreadHandle, INFINITE);

		CloseHandle(m_hMidiInThreadHandle);
		m_hMidiInThreadHandle = NULL;
	}
	return 0;
}

void CMidiInDevice::AddDispatcherToHead(CMidiInMessageDispatcher* pMIMD)
{
	//***************************************************
	// AddObjectToHead
	//	Adds object to the Head of the list
	//	Adds a child object to an object.  This function
	// is not supported by all objects, because they just
	// can have children.  A good example of an object
	// that has children would be a library object
	//
	// parameters:
	//	pMIMD.....pointer to object to add.
	//
	// return value:none
	//--------------------------------------------------
	if (m_pHead == 0)	//nothing in drawing
	{
		m_pHead = pMIMD;
		m_pTail = pMIMD;
	}
	else  //add object to Head of list
	{
		m_pHead->SetPrev(pMIMD);
		pMIMD->SetNext(m_pHead);
		m_pHead = pMIMD;
	}
}

void CMidiInDevice::AddDispatcherToTail(CMidiInMessageDispatcher* pMIMD)
{
	//***************************************************
	// AddObjectToTail
	//	Adds object to the tail of the list
	//	Adds a child object to an object.  This function
	// is not supported by all objects, because they just
	// can have children.  A good example of an object
	// that has children would be a library object
	//
	// parameters:
	//	pMIMD.....pointer to object to add.
	//
	// return value:none
	//--------------------------------------------------
	if (m_pHead == 0)	//nothing in drawing
	{
		m_pHead = pMIMD;
		m_pTail = pMIMD;
	}
	else  //add object to end of list
	{
		m_pTail->SetNext(pMIMD);
		pMIMD->SetPrev(m_pTail);
		m_pTail = pMIMD;
	}
}

void CMidiInDevice::RemoveDispatcher(CMidiInMessageDispatcher* pMIMD)
{
	//***************************************************
	// RemoveObject
	//	Removes a child object from an object
	//
	// parameters:
	//	pO.....pointer to object to remove
	// return value:none
	//--------------------------------------------------
	if (pMIMD == m_pHead)
	{
		m_pHead = m_pHead->GetNext();
		if (m_pHead) m_pHead->SetPrev(0);
	}
	else if (pMIMD == m_pTail)
	{
		m_pTail = m_pTail->GetPrev();
		if (m_pTail) m_pTail->SetNext(0);
	}
	else
	{
		pMIMD->GetPrev()->SetNext(pMIMD->GetNext());
		pMIMD->GetNext()->SetPrev(pMIMD->GetPrev());
	}
	pMIMD->SetPrev(0);
	pMIMD->SetNext(0);
}

int CMidiInDevice::CheckDispatcherList()
{
	int DispatchersRemoved = 0;
	CMidiInMessageDispatcher* pMIMD = GetDispatcherHead();
	CMidiInMessageDispatcher* pBadMIMD;
	int loop = 1;

	while (pMIMD && loop)
	{
		if (GETMAINFRAME->IsChildStillAlive(pMIMD->GetDestinationID()))
		{
			pMIMD->GetNext();
		}
		else
		{
			pBadMIMD = pMIMD;
			pMIMD->GetNext();
			//---------------------------------
			// This is dangerous.  Messing with
			// the dispatcher list while it is
			// being used by the MidiIN thread
			// could very well cause a crash
			//----------------------------------
			RemoveDispatcher(pMIMD);
			++DispatchersRemoved;
		}
	}
	return DispatchersRemoved;
}

void CMidiInDevice::DispatchShortMessage(WPARAM wParam, LPARAM lParam)
{
	CMidiInMessageDispatcher* pMIMD = GetDispatcherHead();
	int Status = CMD(lParam);
	int Data1 = NOTE(lParam);
	int Data2 = VEL(lParam);
	int Chan = CHAN(lParam);
	unsigned StatusToken = 0;

	while (pMIMD)
	{
		//----------------------------------
		// Convert the midi status into
		// its corresponding token (Midi
		// Status ID)
		//----------------------------------
		StatusToken = MidiStatusToMidiID(Status);
		if (StatusToken == pMIMD->GetWindowMessageID())
		{
			//-----------------------------------
			// Status matches
			// Check for Channel Number
			//-----------------------------------
			if (IsChannelStatus(StatusToken))
			{
				if (pMIMD->GetMidiChannel() == Chan)
				{
					//-------------------------------
					// See if the data bytes are within
					// range
					//--------------------------------
					switch (StatusToken)
					{
						//----------------------------
						// Three Byte Midi Status
						//----------------------------
					case STATUS_ID_MIDI_NOTEOFF:
					case STATUS_ID_MIDI_NOTEON:
					case STATUS_ID_MIDI_POLYPRESS:
					case STATUS_ID_MIDI_CTRLCHNG:
					case STATUS_ID_MIDI_PITCHBEND:
						if (Data1 >= pMIMD->GetMidiData1Min() &&
							Data1 <= pMIMD->GetMidiData1Max() &&
							Data2 >= pMIMD->GetMidiData2Min() &&
							Data2 <= pMIMD->GetMidiData2Max()
							)
						{
							pMIMD->GetDestinationWindow()->PostMessageW(
								pMIMD->GetDestinationWindow()->GetMessageCommand(pMIMD->GetWindowMessageID()),
								0,
								lParam
							);
						}
						break;
						//-----------------------
						//two byte Midi Status
						//-----------------------
					case STATUS_ID_MIDI_PGMCHANGE:
					case STATUS_ID_MIDI_CHNLPRESS:
						break;
					}
				}
			}
			else
			{
				// System Midi Status don't need a channel
				switch (StatusToken)
				{
					//---------------------------
					// Single Byte Midi System Status
					//---------------------------
				case STATUS_ID_MIDI_TIMECODE:
				case STATUS_ID_MIDI_RESERVED_F4:
				case STATUS_ID_MIDI_RESERVED_F5:
				case STATUS_ID_MIDI_TUNEREQUEST:
				case STATUS_ID_MIDI_CLOCK:
				case STATUS_ID_MIDI_RTM_RSVRD_F9:
				case STATUS_ID_MIDI_START:
				case STATUS_ID_MIDI_CONTINUE:
				case STATUS_ID_MIDI_STOP:
				case STATUS_ID_MIDI_RTM_RSVRD_FD:
				case STATUS_ID_MIDI_ACTIVE_SENSING:
				case STATUS_ID_MIDI_RESET:
				case STATUS_ID_MIDI_SYSEX:
				case STATUS_ID_MIDI_SYSEXEND:
					break;
					//------------------------------
					// Two Byte Midi System Status
					//-----------------------------
				case STATUS_ID_MIDI_SONGSELECT:
					break;
					//------------------------------
					// Three byte Midi System Status
					//-------------------------------
				case STATUS_ID_MIDI_SONGPOSITION:
					break;
				}
			}
		}
		pMIMD = pMIMD->GetNext();
	}
}

void CMidiInDevice::DispatchLongMessage(WPARAM wParam, LPARAM lParam)
{
	//--------------------------------------------
	// System Exclusive Status
	//--------------------------------------------
	MIDIHDR* pInBuffer = (MIDIHDR*)lParam;
	unsigned char* pBuff = (unsigned char*)pInBuffer->lpData;
	if (pBuff[0] == (unsigned char)MidiSystemCmds::SYSEX)
	{

	}
}