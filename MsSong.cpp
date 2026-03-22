// MsSong.cpp: implementation of the CMsSong class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction CMsSong
//////////////////////////////////////////////////////////////////////


CMsSong::CMsSong()
{
	m_BufIndex = 0;
	m_pFileBuffer = 0;
	m_MidiClockToggleFlag = 0;
	m_TotalTicks = 0;
	m_nMeasureBarCount = 0;
	m_pEventListHead = 0;
	m_pEventListTail = 0;
	m_pCleffEvent = 0;
	m_nTotalEvents = 0;
	m_pChildView = 0;
	m_SongID = GETAPP->GetUniqueID();
	m_pNextSong = 0;
	m_pPrevSong = 0;
	m_pCurrentSongPosition = 0;
	m_PlaySongTimerEnable = 0;
	for(int i=0;i<16;++i)
		m_Patches[i] = 0;
	m_BufIndex = 0;
	m_pFileBuffer = 0;
	m_nFileBufferSize = 0;
	m_InFileSize = 0;
	m_pPlayerObjectQueue = 0;
	for(int i=0;i<128;++i)
	{
		m_NoteCountOn[i] = 0;
		m_NoteCountOff[i] = 0;
	}
	m_TickerState = TickerState::STOPPED;
	m_pEventDirectory = new CMsEventDirectory();
	m_pEventDirectory->Create(this);
	m_pEditEvent = 0;
}

CMsSong::~CMsSong()
{
	// <summary>
	// 
	// </summary>
	if(m_pEventListHead)
	{
		CMsEvent *pEv,*pEvDel;
		pEv = m_pEventListHead;
		while(pEv)
		{
			pEvDel = pEv;
			pEv = pEv->GetNext();
			if(pEvDel) delete pEvDel;
		}
	}
	if(m_pPlayerObjectQueue)
	{
		delete m_pPlayerObjectQueue;
		m_pPlayerObjectQueue = 0;
	}
	if(m_pEventDirectory)
	{
		delete m_pEventDirectory;
		m_pEventDirectory = 0;
	}
}

void CMsSong::AddEventAtEnd(CMsEvent *pE)
{
	// <summary>
	// 
	// </summary>
	// <param name="pE"></param>
	if (pE)
	{
		//if(m_pEventDirectory->GetTotalEvents() == pE->GetIndex())
		//{
		//	m_pEventDirectory->SetEvent(pE);
		//}
		if (m_pEventListHead == 0)
		{
			m_pEventListHead = pE;
			m_pEventListTail = pE;
		}
		else
		{
			pE->SetPrev(m_pEventListTail);
			m_pEventListTail->SetNext(pE);
			m_pEventListTail = pE;
		}
		++m_nTotalEvents;
	}

//	GetEventListHead()->PrintEvents(theApp.LogFile(),"AddEvent At End",2);
}

void CMsSong::AddEventAtStart(CMsEvent *pE)
{
	// <summary>
	// 
	// </summary>
	// <param name="pE"></param>
	if(m_pEventListHead == 0)
	{
		m_pEventListHead = pE;
		m_pEventListTail = pE;
	}
	else
	{
		pE->SetNext(m_pEventListHead);
		m_pEventListHead->SetPrev(pE);
		m_pEventListHead = pE;
	}
	++m_nTotalEvents;
//	GetEventListHead()->PrintEvents(theApp.LogFile(), "AddEventAtStart", 2);
}

bool CMsSong::SetGetPosition(int pos)
{
	bool  rV = false;

	if(pos < m_InFileSize)
	{
		m_BufIndex = pos;
		rV= true;
	}
	return rV;
}

int CMsSong::Parse(char *pSongData)
{
	//--------------------------------------
	// <summary>
	// 	** Parse **
	// This function is used to parse
	// the raw data of the Music Studio Song
	// file data to create a data base
	//
	//	parameter:
	//		pSongData....pointer to the song data
	// </summary>
	// <param name="pSongData"></param>
	// <returns></returns>
	//--------------------------------------
	int rV=0;
	int loop = 1;
	int c;
	int i=0;
	UINT Event=0;

	CMsKeySignature::KeySigID KeySig;
	UINT Tempo;
	UINT TimeSig;
	UINT Loudness;
	UINT Repeat;
	int t1, t2;
	ObjectTypes obj;
	obj.pObj = 0;
	CMsEvent *pEv = MakeNewEvent();
	pEv->SetEventIndex(Event++);

	while(loop)
	{
		c = ParserGetC();
		obj.pObj = 0;
		switch(c)	/*	switch song data	*/
		{
			case MSFF_TOKEN_EOF:
				loop = 0;
				rV = 0;
				break;
			case MSFF_TOKEN_REPEAT_STOP:	//basically the same thing as an End Event
				obj.pRepE = new CMsRepeatEnd;
				obj.pRepE->Create(this, pEv,0);
				break;
			case MSFF_TOKEN_EVENT_END:
				AddEventAtEnd(pEv);
				pEv = MakeNewEvent();
				pEv->SetEventIndex(Event++);
				break;
			case MSFF_TOKEN_KEY_SIGNATURE:
				KeySig = (CMsKeySignature::KeySigID)ParserGetC();;
/*				if(int(KeySig) != 1)
					if (LogFile()) fprintf(LogFile(), "*********** ERROR: KeySig != 1 **************\n");*/
				KeySig = (CMsKeySignature::KeySigID)ParserGetC();
				obj.pKey = new CMsKeySignature();
				obj.pKey->Create(this,pEv,KeySig);
				break;
			case MSFF_TOKEN_TEMPO:
				Tempo = ParserGetC();
				obj.pTemp = new CMsTempo();
				obj.pTemp->Create(this, pEv);
				obj.pTemp->SetQNPM(Tempo);
				break;
			case MSFF_TOKEN_BAR:	//don't do much of anything
				obj.pBar = new CMsBar;
				obj.pBar->Create(this, pEv);
				break;
			case MSFF_TOKEN_TIME_SIGNATURE:
				TimeSig = ParserGetC();
				obj.pTime = new CMsTimeSignature();
				obj.pTime->Create(this, pEv);
				obj.pTime->SetTimeSignature(CMsTimeSignature::TimeSigID(TimeSig));
				break;
			case MSFF_TOKEN_LOUDNESS:
				Loudness = ParserGetC();
/*				if(Loudness != 1)
					if (LogFile()) fprintf(LogFile(), "*********** ERROR: Loudness != 1 **************\n");*/
				Loudness = ParserGetC();
				obj.pLoud = new CMsLoudness();
				obj.pLoud->Create(this,pEv, Loudness);
				break;
			case MSFF_TOKEN_REPEAT_START:
				Repeat = ParserGetC();
				obj.pRepS = new CMsRepeatStart();
				obj.pRepS->Create(this,pEv, 1);
				break;
			case MSFF_TOKEN_END:
				obj.pEnd = new CMsEndBar;
				obj.pEnd->Create(this, pEv);
				loop = 0;
				rV = 0;
				break;
			default:
				//--------------------------------`
				//if we got down here, what was found must be a note
				//so, we first get the track/type,duration,accidentals
				//and then the note number
				//If, the note starts a tie, treat it like any other
				//note, if a note is at the end of a tie, then find
				//the note in the time out buffer.  If the note is
				//the beginning and end of a tie, still find it
				//in the timeout buffer
				//-------------------------------
				t1 = ParserGetC();	/*	duration	*/
				t2 = ParserGetC();	/*	note mask off MSB (upside down note)	*/
				//----------------------------------
				// Create a new Note Object
				//----------------------------------
				obj.pNote = new CMsNote;
				obj.pNote->Create(this, pEv, 0);
				//----------------------------------
				// Fill in the data
				//----------------------------------
				obj.pNote->SetAccent((t1 & MSFF_ACCENT_FLAG)?1:0);
				obj.pNote->SetDuration(t1 & MSFF_DURATION_MASK);
				obj.pNote->SetAccidental((t1 & MSFF_KEY_MASK) >> MSFF_KEY_SHIFT);
				obj.pNote->SetLegato(0);		//Not supported
				obj.pNote->SetPitch(t2 & 0x7f);
				obj.pNote->SetRest((c & MSFF_REST_FLAG)?1:0);
				obj.pNote->SetStaccato(0);		//Not supported
				obj.pNote->SetTieBeg((c & MSFF_BEGTIE_FLAG)?1:0);
				obj.pNote->SetTieEnd((c & MSFF_ENDTIE_FLAG)?1:0);
				obj.pNote->SetTrack((c & MSFF_TRACK_MASK));
				obj.pNote->SetStemDown((t2 & MSFF_NOTE_UPSIDE_DOWN)?true:false);
				break;
		}	/*	end of switch (c)	*/
		if(obj.pObj)
		{
			pEv->AddObject(obj.pObj);
//			obj.pObj->Print(stdout);
		}
	}	/*	end while	*/
	RenumberEvents();
	return rV;
}

void CMsSong::Print(FILE* pO)
{
}

UINT CMsSong::LittleEndian(UINT bE)
{
    union {
        unsigned char m_C[2];
        UINT m_S;
    }CV = {0};
    unsigned char t;

    CV.m_S = bE;
    t = CV.m_C[0];
    CV.m_C[0] = CV.m_C[1];
    CV.m_C[1] = t;
    return CV.m_S;
}

CMsTimeSignature* CMsSong::GetCurrentTimeSignature()
{
	CMsTimeSignature* pTS = 0;
	bool bFound = false;
	CMsEvent* pEV = 0;
	;
	if(IsPlaying())
		pTS = m_SongGlobals.m_pLastTimeSignature;
	else
	{
		pEV = GetEventObject(GetStaffChildView()->GetDrawEvent());
		while (pEV && !bFound)
		{
			if(pEV->ContainsObjectType(CMsObject::MsObjType::TIMESIG))
			{
				pTS = (CMsTimeSignature*)pEV->FindFirstObjectOfType(CMsObject::MsObjType::TIMESIG);
				bFound = true;
			}
			else
				pEV = pEV->GetPrev();
		}
	}
	return pTS;
}

CMsKeySignature* CMsSong::GetCurrentKeySignature()
{
	CMsKeySignature* pKS = 0;
	bool bFound = false;
	CMsEvent* pEV = 0;

	if(IsPlaying())
		pKS = m_SongGlobals.m_pLastKeySignature;
	else
	{
		pEV = GetEventObject(GetStaffChildView()->GetDrawEvent());
		while (pEV && !bFound)
		{
/*			fprintf(LogFile(), "\t\tFind Key Signature-> Event:%d Found:%s\n",
				pEV->GetIndex(),
				bFound ? "Yes" : "No"
			);*/
			if(pEV->ContainsObjectType(CMsObject::MsObjType::KEYSIG))
			{
				pKS = (CMsKeySignature*)pEV->FindFirstObjectOfType(CMsObject::MsObjType::KEYSIG);
				bFound = true;
			}
			else
				pEV = pEV->GetPrev();
		}
	}
//	fprintf(LogFile(), "\t\tCurrent Key Signature: %s\n", pKS ? CMsKeySignature::KeySigIDToString(pKS->GetKeySignature()) : "Not Found");
	return pKS;
}

void CMsSong::Draw(CDC *pDC, int StartEvent, int maxevent,CRect *pRect)
{
	//------------------------------------
	// Draw
	// pDC........Device Context for the
	//            Entire Area of the Event
	//			  Chain to be drawn
	//------------------------------------
	int i = 0;
	int MaxX = pRect->right;
	CPen penStaffLines,*oldpen;
	CMyBitmap bmEvent, *oldBM;
	CDC memDCEvent;

	penStaffLines.CreatePen(PS_SOLID,1, GetColorPalette()->color_StaffLines);
	oldpen = pDC->SelectObject(&penStaffLines);
	//-----------------------------
	// create a memory DC for event
	// drawing.
	//-----------------------------
	bmEvent.CreateCompatibleBitmap(pDC, EVENT_WIDTH, EVENT_HEIGHT);
	memDCEvent.CreateCompatibleDC(pDC);
	oldBM = (CMyBitmap*)memDCEvent.SelectObject(&bmEvent);
	//-----------------------------
	// draw clefs
	//-----------------------------
	m_pCleffEvent->Draw(&memDCEvent);
	pDC->BitBlt(
		0, 
		0, 
		EVENT_WIDTH,
		EVENT_HEIGHT, 
		&memDCEvent, 
		0, 
		0, 
		SRCCOPY
	);
	//-----------------------------
	// draw music objects onto
	// music staff
	//-----------------------------
	CMsEvent *pEv = GetEventObject(StartEvent);
	for(i=0;(i<maxevent+1) && pEv;++i,pEv = pEv->GetNext())
	{
		if(pEv)pEv->Draw(&memDCEvent);
		pDC->BitBlt(
			(i + 1) * EVENT_WIDTH, 
			0, 
			EVENT_WIDTH,
			EVENT_HEIGHT, 
			&memDCEvent, 
			0, 
			0, 
			SRCCOPY
		);
	}
}
UINT CMsSong::AddMoreEventsAtEnd(UINT NewEndEvent)
{
	// <summary>
	// 
	// </summary>
	// <param name="NewEndEvent"></param>
	// <returns></returns>
	// 

	UINT NumberOfEvents = 0;
	CMsEvent* pEndEvent = GetEventListTail();
	CMsEvent* pEVTemp;

	NumberOfEvents = NewEndEvent - pEndEvent->GetIndex();
	if (NumberOfEvents > 1)
	{
		for (UINT i = 0; i < NumberOfEvents; i++)
		{
			pEVTemp = MakeNewEvent();
			AddEventAtEnd(pEVTemp);
		}
	}
	else
	{
		pEVTemp = MakeNewEvent();
		AddEventAtEnd(pEVTemp);
	}
	RenumberEvents();
//	GetEventListHead()->PrintEvents(theApp.LogFile(), "AddMoreEventsAtEnd", 2);
	return NumberOfEvents;
}

void CMsSong::RenumberEvents()
{
	int i = 0;
	m_nTotalEvents = 0;
	CMsEvent *pEv = GetEventListHead();
	
	while(pEv)
	{
		m_nTotalEvents++;
		pEv->SetEventIndex(i++);
		pEv = pEv->GetNext();
	}
//	GetEventListHead()->PrintEvents(theApp.LogFile(), "RenumberEvents", 2);
}

bool CMsSong::GetSeledtedEventBlock(CMsEvent** ppEvFirst, CMsEvent** ppEvLast)
{
	CMsEvent* pEv = GetEventListHead();
	bool bLoop = true;
	bool bFoundFirst = false;

	while (pEv && bLoop)
	{
		if (bFoundFirst == false)
		{
			if(pEv->IsSelected())
			{
				*ppEvFirst = pEv;
				*ppEvLast = pEv;
				bFoundFirst = true;
			}
		}
		else
		{
			if(pEv->IsSelected())
				*ppEvLast = pEv;
			else
				bLoop = false;
		}
		pEv = pEv->GetNext();
	}
	return bFoundFirst;
}

void CMsSong::RenumberMeasureBars()
{
	CMsEvent* pEv = GetEventListHead();
	int MeasureBarCount = 0;
	int DuplicateBarCount = 0;
	CMsObject* pObj = nullptr;
	CMsObject* pTempObj = nullptr;	

	while (pEv)
	{
		pObj = pEv->FindFirstObjectOfType(CMsObject::MsObjType::BAR);
		DuplicateBarCount = 0;
		while (pObj)
		{
			if(DuplicateBarCount > 0)
			{
				//---------------------------
				// This is a duplicate bar
				// remove it
				//---------------------------
				pEv->RemoveObject(pObj);
				pTempObj = pObj;
				pObj = pEv->FindNextObjectOfType(CMsObject::MsObjType::BAR, pObj);
				if(pTempObj) delete pTempObj;
			}
			else
			{
				//---------------------------
				// This is the first bar
				// keep it
				//---------------------------
				CMsBar* pBar = (CMsBar*)pObj;
				pBar->SetBarNumber(++MeasureBarCount);
				pObj = pEv->FindNextObjectOfType(CMsObject::MsObjType::BAR, pObj);
			}
			++DuplicateBarCount;
		}
		pEv = pEv->GetNext();
	}
}

bool CMsSong::SelectEventsFrom(CMsEvent* pEvent)
{
	//-------------------------------
	// Given an Event ID, find
	// another event that is also
	// selected, and select all
	// events in between.
	// If the EventID is already
	// selected, deselect all
	// contiguous events.
	//-------------------------------
	CMsEvent* pEV = 0;
	if (pEvent->IsSelected())
	{
		pEV = pEvent->GetNext();
		while (pEV && pEV->IsSelected())
		{
			pEV->SetSelected(false);
			pEV = pEV->GetNext();
		}
		pEV = pEvent->GetPrev();
		while (pEV && pEV->IsSelected())
		{
			pEV->SetSelected(false);
			pEV = pEV->GetPrev();
		}
	}
	else
	{
		//-------------------------------
		// Find, if any, the first
		// selected event after this one
		//-------------------------------
		pEV = pEvent->GetPrev();
		while (pEV && !pEV->IsSelected())
			pEV = pEV->GetPrev();
		if (pEV)
		{
			while (pEV != pEvent)
			{
				pEV->SetSelected(true);
				pEV = pEV->GetNext();
			}
		}
		pEV = pEvent->GetNext();
		while (pEV && !pEV->IsSelected())
			pEV = pEV->GetNext();
		if (pEV)
		{
			while (pEV != pEvent)
			{
				pEV->SetSelected(true);
				pEV = pEV->GetPrev();
			}
		}
		pEvent->SetSelected(true);
	}
	return true;
}

CMsEvent *CMsSong::InsertEmptyEvent(CMsEvent* pEvInsertHere, int BeforeNotAfter)
{
	//--------------------------------
	//	InsertEvent
	//
	//		This function inserts an
	//	empty event after the given
	//	position.  
	//
	// parameters:
	//		pEvInsertHere.....event position to insert new event
	//		BeforeNotAfter.... .if 1, insert before given event, 
	//							if 0, insert after given event
	//
	// return value:
	//		returns a pointer to the new
	//	event.
	//-------------------------------------
	CMsEvent *pNewEv= new CMsEvent();
	bool bLoop = true;

	if (pEvInsertHere)
	{
		pNewEv->Create(this, GetStaffChildView());
		if (BeforeNotAfter == Insert::BEFORE)
		{
			pNewEv->SetPrev(pEvInsertHere->GetPrev());
			pNewEv->SetNext(pEvInsertHere);
			if (pEvInsertHere->GetPrev())
				pEvInsertHere->GetPrev()->SetNext(pNewEv);
			pEvInsertHere->SetPrev(pNewEv);
		}
		else
		{
			pNewEv->SetPrev(pEvInsertHere);
			pNewEv->SetNext(pEvInsertHere->GetNext());
			pEvInsertHere->SetNext(pNewEv);
			if(pNewEv->GetNext())
				pNewEv->GetNext()->SetPrev(pNewEv);
		}
	}
//	GetEventListHead()->PrintEvents(theApp.LogFile(), "InsertEvent", 2);
	return pNewEv;
}

CMsEvent* CMsSong::InsertEvent(CMsEvent* pEvInsertHere, CMsEvent* pInsertThis, int BeforeNotAfter)
{
	if (pEvInsertHere && pInsertThis)
	{
		if (BeforeNotAfter == Insert::BEFORE)
		{
			pInsertThis->SetPrev(pEvInsertHere->GetPrev());
			pInsertThis->SetNext(pEvInsertHere);
			if (pEvInsertHere->GetPrev())
				pEvInsertHere->GetPrev()->SetNext(pInsertThis);
			pEvInsertHere->SetPrev(pInsertThis);
		}
		else
		{
			pInsertThis->SetPrev(pEvInsertHere);
			pInsertThis->SetNext(pEvInsertHere->GetNext());
			pEvInsertHere->SetNext(pInsertThis);
			if (pInsertThis->GetNext())
				pInsertThis->GetNext()->SetPrev(pInsertThis);
		}
	}
	return pInsertThis;
}

CMsNote * CMsSong::CheckForNotePresence(int Event, int Note)
{
	//-------------------------------
	// CheckForNotePresence
	//	This function is used to find
	// out if there is a particular 
	// note at a certain location
	//
	//	parameters:
	//		Event....Event to look in
	//		Note.....Note to check for
	//
	// returns NULL if not found,
	//	otherwise, returns point to note
	//------------------------------------

	CMsNote *pN=0;
	CMsObject *pOb;
	CMsEvent *pEv = GetEventListHead();
	int loop = 1;
	while(pEv && loop)
	{
		if(pEv->GetIndex() == Event)
			loop = 0;
		else
			pEv = pEv->GetNext();
	}
	if(pEv)
	{
		pOb = pEv->GetEventMsObjectHead();
		loop = 1;
		while(loop && pOb)
		{
			if(pOb->GetType() == CMsObject::MsObjType::NOTE)
			{
				CMsNote *pNC = (CMsNote *)pOb;
				if(!pNC->IsRest() && pNC->GetPitch() == Note)
				{
					pN = pNC;
					loop = 0;
				}
			}
			if(pN == 0)
				pOb = pOb->GetNext();
		}
	}
	return pN;
}

CMsEvent *CMsSong::GetEventObject(int EventIndex)
{
	//------------------------------------
	// GetEventObject
	//	This function takes an Event
	// number and turns it into the
	// appropriate pointer to an Event
	// object.  If there is no object
	// there yet, it returns NULL
	// 
	// parameter:
	//		EventIndex....Index of desired event
	//
	//	returns: pointer to the CMsEvent
	//			NULL if not found
	//-------------------------------------
	CMsEvent *pEv = GetEventListHead();
	bool loop = true;
	int SongEventIndex;

	while(pEv && loop)
	{
		// Is this the one?
		SongEventIndex = pEv->GetIndex();
		if(SongEventIndex == EventIndex)
			loop = false;	//Yes, stop seraching
		else
			// Keep seraching
			pEv = pEv->GetNext();
	}
	if(!pEv)
	{
		//-----------------------------
		// Did not find event
		// return NULL
		// If this method was called,
		// the APP must be pretty
		// serious about wanting
		// a pointer to an event,
		// so create a new event
		// to go here.
		//-----------------------------
		pEv = MakeNewEvent(EventIndex);
		loop = false;
	}
	return pEv;
}

CMsEvent* CMsSong::InsertEmptyEvent(int EventIndex, int BeforeNotAfter)
{
	CMsEvent* pEv = GetEventObject(EventIndex);
	return InsertEmptyEvent(pEv, BeforeNotAfter);
}

//--------------------------------------------
// The following is what I will consider to be
// the default Atari sound data.  It was taken
// from one of the song files that came on the 
// original disk.
//---------------------------------------------

void CMsSong::Save(FILE *pO)
{
	int NoEnd = 1;
	//---------------------------------------
	// Before we save, make sure that there
	// is an END object at the end of the 
	// song.
	//---------------------------------------
	if(m_pEventListTail)
	{
		CMsObject *pOb = m_pEventListTail->GetEventMsObjectHead();
		while(pOb && NoEnd)
		{
			if(CMsObject::MsObjType::ENDBAR == pOb->GetType())
				NoEnd = 0;
			pOb = pOb->GetNext();
		}
	}
	//-------------------------------------
	// If there is no End, Add one.
	//-------------------------------------
	if(NoEnd)
	{
		CMsEvent* pEV;

		CMsEndBar *pME = new CMsEndBar;
		pME->Create(this, m_pEventListTail);
		pEV = MakeNewEvent();
		AddEventAtEnd(pEV);
		pEV = MakeNewEvent();
		AddEventAtEnd(pEV);
		m_pEventListTail->AddObject(pME);
		RenumberEvents();
	}
	//-----------------------------------
	// Save
	// this function is used to save a 
	// song file
	//
	// parameter:
	//		pO.....pointer to output stream
	//-----------------------------------
	fputc(0xcd,pO);	//first character of file
	fprintf(pO,"Music Studio");
	fputc(0xcd,pO);
	fputc(0x02,pO);
	//------------------------------------
	// Next, we have the default atari
	// instrument data.  I just copied
	// this from a random Music Studio
	// file, as there is not a whole lot
	// that I can do with it.
	//------------------------------------
	int i;

	for(i=0;i<270;++i)
		fputc(AtariInst[i],pO);
	//-----------------------------------
	// Save Midi Info
	//-----------------------------------
	SaveTracks(pO);
	//-----------------------------------
	// Save song content
	//-----------------------------------
	CMsEvent *pEv = GetEventListHead();
	while(pEv)
	{
		CMsObject *pObj;
		pObj = pEv->GetEventMsObjectHead();
		while(pObj)
		{
			pObj->Save(pO);
			pObj = pObj->GetNext();
		}
		fputc(0,pO);	//event delemiter
		pEv = pEv->GetNext();
	}
}

void CMsSong::SaveTracks(FILE* pO)
{
	int i;

	for (i = 1; i < 16; ++i)
	{
		GetTrackInfo(i)->Save(pO);
	}
}

bool CMsSong::Open(CString& csFileName)
{
	char* pName = new char[512];
	FILE* pInFile;
	errno_t err;
	bool rV = true;
	struct _stat32 FileStats;
	unsigned BytesRead = 0;

	sprintf_s(pName, 512, "%S", csFileName.GetString());
	err = fopen_s(&pInFile, pName, "rb");
	if (err != 0)
	{
		rV = false;
	}
	else
	{
		_stat32(pName, &FileStats);
		m_InFileSize = FileStats.st_size;
		fopen_s(&pInFile, pName, "rb");
		if(pInFile == NULL)
		{
			MessageBox(NULL, _T("Could not open file"), _T("Error"), MB_OK);
			rV = false;
		}
		else
		{
			m_pFileBuffer = new char[m_InFileSize + 1];
			if (m_pFileBuffer)
			{
				BytesRead = (int)fread(m_pFileBuffer, 1, m_InFileSize, pInFile);
				if (pInFile) fclose(pInFile);
				if(!ValidateFile())
				{
					MessageBox(NULL, _T("Not a valid Music Studio File"), _T("Error"), MB_OK);
					rV = false;
				}
				else
				{
					if (BytesRead)
						m_InFileSize = BytesRead;
					fprintf(LogFile(), "File:%s has %d Bytes\n", pName, m_InFileSize);
					//			theApp.Dump(theApp.LogFile(), m_pFileBuffer, m_InFileSize, 0);
					if (m_pFileBuffer)
					{
						SetGetPosition(520);
//						DumpSong(theApp.LogFile());
						Parse(m_pFileBuffer);
					}
					else
						rV = false;
				}
			}
		}
	}
	if(pName) delete[] pName;
	return rV;
}

int CMsSong::ParserGetC()
{
	int rV = -1;
	if(m_InFileSize > m_BufIndex)
		rV = (int)m_pFileBuffer[m_BufIndex++] & 0xff;
	return rV;
}


//--------------------------------
// Removes an event from the song
//=------------------------------
void CMsSong::RemoveEvent(int Event)
{
	CMsEvent *pEv = GetEventObject(Event);
	if(pEv) DeleteEvent(pEv);
}

// Delete Event object
void CMsSong::DeleteEvent(CMsEvent * pEvent)
{
	if (pEvent && m_pEventListHead && m_pEventListTail)
	{
		if (pEvent == m_pEventListHead)
		{
			m_pEventListHead = pEvent->GetNext();
			if (m_pEventListHead)
				m_pEventListHead->SetPrev(0);
			else
				m_pEventListTail = 0;
		}
		else if (pEvent == m_pEventListTail)
		{
			m_pEventListTail = pEvent->GetPrev();
			if (m_pEventListTail)
				m_pEventListTail->SetNext(0);
			else
				m_pEventListHead = 0;
		}
		else
		{
			pEvent->GetNext()->SetPrev(pEvent->GetPrev());
			pEvent->GetPrev()->SetNext(pEvent->GetNext());
		}
		if(pEvent) delete pEvent;
	}
}

CMsEvent* CMsSong::MakeNewEvent()
{
	CMsEvent* pEV = new CMsEvent();
	//--------------------------------------------
	// Set "this" as Events Parent Song and
	// set the Staff View Parent
	//--------------------------------------------
	pEV->Create(this, GetStaffChildView());
	//--------------------------------------------
	// Set the Event index as unknown ( -1 )
	//--------------------------------------------
	pEV->SetEventIndex(-1);
	return pEV;
}

CMsEvent* CMsSong::MakeNewEvent(int EventIndex)
{
	CMsEvent* pTail = GetEventListTail();
	CMsEvent* pEV = new CMsEvent();
	CMsEvent* pBlankEv = nullptr;

	pEV->Create(this, GetStaffChildView());
	if(!pTail && (EventIndex == 0))
	{
		//--------------------------------------------
		// There are no events yet, and we want
		// to create the first one
		//--------------------------------------------
		pEV->SetEventIndex(0);
	}
	else if (!pTail && (EventIndex > 0))
	{
		//--------------------------------------------
		// We need to add blank events until we
		// get to the right index
		//--------------------------------------------
		int i;
		for (i = 0; i < EventIndex; ++i)
		{
			pBlankEv = MakeNewEvent();
			pBlankEv->SetEventIndex(i);
			AddEventAtEnd(pBlankEv);
		}
		pEV = MakeNewEvent();
		pEV->SetEventIndex(EventIndex);
		AddEventAtEnd(pEV);
	}
	else if (pTail && (EventIndex == (pTail->GetIndex() + 1)))
	{
		pEV->SetEventIndex(EventIndex);
	}
	else if (pTail && (EventIndex > (pTail->GetIndex() + 1)))
	{
		//--------------------------------------------
		// We need to add blank events until we
		// get to the right index
		//--------------------------------------------
		int i;
		for (i = pTail->GetIndex() + 1; i < EventIndex; ++i)
		{
			pBlankEv = MakeNewEvent();
			pBlankEv->SetEventIndex(i);
			AddEventAtEnd(pBlankEv);
		}
		pEV = MakeNewEvent();
		pEV->SetEventIndex(EventIndex);
		AddEventAtEnd(pEV);
	}
	else
	{
		AfxMessageBox(_T("Cannot make event at this index"), MB_OK);
		pEV->SetEventIndex(-1);
	}
	return pEV;
}

CMsObject * CMsSong::GetObjectTypeInEvent(CMsObject::MsObjType nType, int nEvent)
{
	CMsEvent *pEV = GetEventObject(nEvent);
	CMsObject *pObj = 0;
	if (pEV) pObj = pEV->ContainsObjectType(nType);
	return pObj;
}

CMsObject * CMsSong::GetMsObject(
	CMsObject::MsObjType ObjType,
	CMsEvent * pStartObject, 
	INT Direction)
{
	CMsEvent *pEV;
	CMsObject *pRetObj = NULL;

	if ((pStartObject == NULL) && (Direction == SEARCH_FORWARD))	//forward
		pEV = m_pEventListHead;
	else if (pStartObject == NULL && Direction == SEARCH_REVERSE)
		pEV = m_pEventListTail;
	else
		pEV = pStartObject;
	int loop;
	loop = 1;
	while (pEV && loop)
	{
		CMsObject *pObj;

//		if(LogFile()) fprintf(LogFile(),"    Search for Object in event %d\n", pEV->GetIndex());
		pObj = pEV->GetEventMsObjectHead();
		while (pObj && loop)
		{
			if (pObj->GetType() == ObjType)
			{
				pRetObj = pObj;
				loop = 0;
//				if(LogFile()) fprintf(LogFile(),"      Object Found\n");
			}
			pObj = pObj->GetNext();
		}
		if (Direction == SEARCH_FORWARD)	//forward
		{
			pEV = pEV->GetNext();
		}
		else // reverse
		{
			pEV = pEV->GetPrev();
		}
	}

	return pRetObj;
}


bool CMsSong::Create(
	CChildViewStaff* pCCV,
	CSize szTrackIconSize
)
{
	//------------------------------
	// Create Bass and Treble Cleff
	// events.
	//------------------------------
	CMsBassCleff* pBassCleff = new CMsBassCleff;
	CMsTrebleCleff* pTrebleCleff = new CMsTrebleCleff;

	//------------------------------

	m_pChildView = pCCV;
	m_pPlayerObjectQueue = new CMsPlayerQueue;
	m_pPlayerObjectQueue->Create(this);
	m_pEventDirectory->Create(this);
	//------------------------------
	// Create the Cleff Event
	//------------------------------
	m_pCleffEvent = new CMsEvent;
	m_pCleffEvent->Create(this, GetStaffChildView());
	pBassCleff->Create(this, m_pCleffEvent);
	pTrebleCleff->Create(this, m_pCleffEvent);
	m_pCleffEvent->AddObject(pBassCleff);
	m_pCleffEvent->AddObject(pTrebleCleff);

	GetSongInfo()->Create(this, szTrackIconSize);
	return true;
}

void CMsSong::AddEventChain(int EventDest, CMsEventChain* pEvC)
{
	CMsEvent* pEV;

	pEV = GetEventObject(EventDest);
	if (pEV == 0)
	{
		// Add Chain to end
		pEV = GetEventListTail();
		if (pEV)
		{
			pEV->SetNext(pEvC->GetHead());
			pEvC->GetHead()->SetPrev(pEV);
			SetEventListTail(pEvC->GetTail());
			m_nTotalEvents += pEvC->GetTotalEvents();
		}
		else
			AfxMessageBox(_T("InTernal Error"), MB_OK);
	}
	else if (pEV->GetPrev() == 0)
	{
		// Add chain to beginning
		pEV = GetEventListHead();
		if (pEV)
		{
			pEV->SetPrev(pEvC->GetTail());
			pEvC->GetTail()->SetNext(pEV);
			SetEventListHead(pEvC->GetHead());
			m_nTotalEvents += pEvC->GetTotalEvents();
		}
		else
			AfxMessageBox(_T("InTernal Error"), MB_OK);
	}
	else
	{
		// Insert chain
		pEV = GetEventObject(EventDest);
		if (pEV)
		{
			CMsEvent* pInsertEnd;
			pInsertEnd = pEV->GetNext();
			pEV->SetNext(pEvC->GetHead());
			pEvC->GetHead()->SetPrev(pEV);
			pInsertEnd->SetPrev(pEvC->GetTail());
			pEvC->GetTail()->SetNext(pInsertEnd);
			m_nTotalEvents += pEvC->GetTotalEvents();
		}
		else
			AfxMessageBox(_T("InTernal Error"), MB_OK);
	}
//	GetEventListHead()->PrintEvents(theApp.LogFile(),"AddEventChain #1", 2);
}

void CMsSong::AddEventChain(
	CMsEvent* pDestination, 
	CMsEventChain* pEvC, 
	int DestIndex, 
	int AddBlanks
)
{
	if (pDestination == 0)
	{
		//------------------------------------------
		// We need to add blank events at end, if 
		// needced before we add the chain
		//------------------------------------------
		if (DestIndex == GetEventListTail()->GetIndex() + 1)
		{
			// Easy Peasy
			GetEventListTail()->SetNext(pEvC->GetHead());
			pEvC->GetHead()->SetPrev(GetEventListTail());
			SetEventListTail(pEvC->GetTail());
		}
		else
		{
			// Not so Easy Peasy
			// Need to add blank events
			int i;
			CMsEvent* pEV;

			for (i = 0; i < AddBlanks; ++i)
			{
				pEV = MakeNewEvent();
				AddEventAtEnd(pEV);
			}
			GetEventListTail()->SetNext(pEvC->GetHead());
			pEvC->GetHead()->SetPrev(GetEventListTail());
			SetEventListTail(pEvC->GetTail());
		}
	}
	else
	{
		// Insert Chain into event list
		if (pDestination->GetPrev())
		{
			pDestination->GetPrev()->SetNext(pEvC->GetHead());
			pEvC->GetHead()->SetPrev(pDestination->GetPrev());
			pEvC->GetTail()->SetNext(pDestination);
			pDestination->SetPrev(pEvC->GetTail());
		}
		else
		{
			pDestination->SetPrev(pEvC->GetTail());
			SetEventListHead(pEvC->GetHead());
		}
	}
//	GetEventListHead()->PrintEvents(theApp.LogFile(),"AddEventChain #2", 2);
}

//*****************************************
// The following methods are used to play
// the midi song.
//*****************************************

void CMsSong::Start(void)
{
	CMsEvent* pEvent = GetSongPosition();

	if (pEvent)
	{
		if (GetPlayerQueue()->ProcessQueue(pEvent) > 0)
		{
			//Enable the timer
			GETAPP->PlayerThreadAddSong(this);	// Add song to player thread
			GetAddSongCompleteEV().Pend();		// clear song done event  object
			GETAPP->PlayerThreadEnableTimer(GetSongId(), 1);
			GetEnableTimerCompleteEV().Pend();
			m_TickerState = TickerState::START;
			pEvent->SetSelected(true);
			if (GetSongPosition())
			{
				GetStaffChildView()->PostMessageW(
					WM_STAFF_DISP_EVENT,
					GetSongPosition()->GetIndex(),
					STAFF_DISP_EVENT_NEXT
				);
			}
		}
	}
}

int CMsSong::Stop()
{
	//	GETAPP->PlayerThreadEnableTimer(GetEventQueueID(), 0);
	//	GetDisableTimerCompleteEV().Pend();
	//	GETAPP->PlayerThreadDeleteEventQueue(this);
	//	GetDelEventQueueCompleteEV().Pend();
		//-------------------------------------
		// 	   If the songs is playing, add the
		// 	   EVENTQUEUE_STOP flag to let the
		// 	   Player thread know that the song
		// 	   is to be terminated
		//-------------------------------------
	if(GetTickerState() != TickerState::STOPPED)
		m_TickerState = TickerState::WIND_DOWN;
	return 0;
}

//----------------------------------------------
UINT CMsSong::Ticker(void)
{
	//-----------------------------
	// Ticker
	//		This is the function that
	//	is calleed by the player thread
	// timer.  This is wherre
	// we figure out when to play
	// the midi event
	//
	// Also, we need to output a 
	// Midi clock 24 time per
	// quarter note.  A quarter
	// note is 48 ticks, so every
	// other time this function
	// is called, we need to output
	// a midi clock.
	// This function is only called
	// by a seperate Thread.
	//
	// RETURNS:0 when done,
	//         1 while in progress
	//-----------------------------
	int rV = 1;
	//-------------------------------
	// Flag to indicate it is time
	// to get another midi event
	//-------------------------------
	int AnObjectHasEnded = 0;
	int DeleteEventFlag = 0;
	CMsNote* pNote = 0;
	UINT TotalObjectInPlayerListQueue = GetPlayerQueue()->GetTotalObjects();
	CMsEvent* pNextEvent = 0;
	UINT PlayStatus = 0;

	switch (m_TickerState)
	{
	case TickerState::START:
		//--------------------------------
		// Starting up the ticker
		//--------------------------------
//		fprintf(LogFile(), "************************Starting Song %d Ticks:%d\n", GetSongId(), m_TotalTicks);
		MidiStart();			// Send MidiStart command to Midi device
		m_MidiClockToggleFlag = 0;
		m_TickerState = TickerState::RUNNING;
		m_TotalTicks = 0;
		break;
	case TickerState::RUNNING:
		//--------------------------------
		// Send Midi Clock every other tick
		//-------------------------------
//		fprintf(LogFile(), "--Running Song %d Ticks:%d\n", GetSongId(), m_TotalTicks);
		if(!m_MidiClockToggleFlag)
		{
			m_MidiClockToggleFlag = 1;
		}
		else
		{
			MidiClock();			// Send first Midi Clock command
			m_MidiClockToggleFlag = 0;
		}
		//-----------------------------
		PlayStatus = GetPlayerQueue()->Play(this);
		m_TotalTicks++;
		if (PlayStatus)
		{
			//-----------------------------
			// if PlayStatus is greater
			// than zero, then one or more
			// objects in the player queue
			// that have timed out and
			// it is time to go onto the
			// next event in the song
			//-----------------------------
//			fprintf(LogFile(), "Event %d has timed out\n", GetSongPosition()->GetIndex());
			pNextEvent = GetNextEventToProcess();
			if (pNextEvent == nullptr)
			{
				//-----------------------------
				// No more events, so set
				// the song to stop
				//-----------------------------
				m_TickerState = TickerState::WIND_DOWN;
			}
			else
			{
				//-----------------------------
				// Move the song position
				// to the next event to be
				// processed
				//-----------------------------
				GetSongPosition()->SetSelected(false);
				pNextEvent->SetSelected(true);
				SetSongPosition(pNextEvent);
				GetPlayerQueue()->ProcessQueue(pNextEvent);
				if (GetSongPosition())
				{
					GetStaffChildView()->PostMessageW(
						WM_STAFF_DISP_EVENT,
						GetSongPosition()->GetIndex(),
						STAFF_DISP_EVENT_NEXT
					);
				}
			}
		}

		break;
	case TickerState::WIND_DOWN:
		//--------------------------------
		// Send Midi Clock every other tick
		//-------------------------------
		if (!m_MidiClockToggleFlag)
		{
			m_MidiClockToggleFlag = 1;
		}
		else
		{
			MidiClock();			// Send first Midi Clock command
			m_MidiClockToggleFlag = 0;
		}
		PlayStatus = GetPlayerQueue()->WindDown();
		if(PlayStatus == 0)
		{
			if(GetSongPosition())
				GetSongPosition()->SetSelected(false);
			m_TickerState = TickerState::STOPPED;
			MidiStop();				// Send Midi Stop command to Midi device
			rV = 0;					// Indicate we are done
			GETAPP->PlayerThreadEnableTimer(GetSongId(),0);
			GetDisableTimerCompleteEV().Pend();
			GETAPP->PlayerThreadDeleteSong(this);
			GetDelSongCompleteEV().Pend();
		}
		break;
	case TickerState::STOPPED:
		break;
	default:
		break;
	}
	return rV;
}

CMsEvent* CMsSong::GetNextEventToProcess()
{
	CMsEvent* pEv = GetSongPosition();
	CMsObject* pObj = 0;
	int CurEventIndex = -1;
	bool Loop = true;

	if (pEv)
	{
		CurEventIndex = pEv->GetIndex();
		pEv = pEv->GetNext();
		while (pEv && Loop)
		{
			pObj = pEv->GetEventMsObjectHead();
			if (pObj)
				Loop = false;
			else
				pEv = pEv->GetNext();
		}
	}
	if (pEv == nullptr)
	{
		//-----------------------------
		// No more events, so set
		// the song to stop
		//-----------------------------
		fprintf(LogFile(), "No more events to process\n");
	}
	else
		fprintf(LogFile(), "Current Event:%d Next event to process is %d Ticks:%d\n", CurEventIndex, pEv->GetIndex(), m_TotalTicks);
	return pEv;
}

void CMsSong::SetSongPosition(CMsEvent* pEv)
{
	m_pCurrentSongPosition = pEv;
}

CMsEvent* CMsSong::GetSongPosition()
{
	fprintf(LogFile(), "\t\tGet Song Position is event %d\n", m_pCurrentSongPosition ? m_pCurrentSongPosition->GetIndex() : -1);
	return m_pCurrentSongPosition;
}
//--------------------------------
// Play
//	This is the method that gets
//	the ball rolling.
// Parameters:
//	pChildView..Pointer to the view
//		for this song
// return value
//	1 => Success
//	0 => Fail
//------------------------------
bool CMsSong::Play(CChildViewStaff* pChildView)
{
	bool Succes = false;

	if (GetTickerState() == TickerState::STOPPED)
	{
		SetSongPosition(GetEventListHead());
		SetTickerState(TickerState::RUNNING);
		m_TotalTicks = 0;
		Succes = true;
	}
	return Succes;
}

bool CMsSong::CheckChan(int track, int chan)
{
	bool rv = true;
	if (m_Patches[chan] != track)
		m_Patches[chan] = track;
	else
		rv = false;
	return rv;
}

void CMsSong::MidiClock()
{
	//---------------------------------------------
	// MidiClock
	//	This function sends a Midi CLOCK message
	// out of the specified midi port
	//---------------------------------------------
	int NumMidiOutDevices;
	NumMidiOutDevices = GETAPP->GetMidiOutTable()->GetNumDevices();
	int i;
	for (i = 0; i < NumMidiOutDevices; ++i)
	{
		GETAPP->GetMidiOutTable()->GetDevice(i).Clock();
	}
}

void CMsSong::MidiAllNotesOff()
{
	//--------------------------------------
	// MidiAllNotesOff
	//	Send the All Notes Off message out of the 
	// specified midi port
	//--------------------------------------
	int NumMidiOutDevices = GETAPP->GetMidiOutTable()->GetNumDevices();;
	int i;
	int Id;
	int Channel;

	for (i = 1; i < 16; ++i)
	{
		Channel = GetTrackInfo(i - 1)->GetChannel() - 1;	// logical to physical channel
		Id = GetTrackInfo(i - 1)->GetMidiOutDeviceID();
		GETAPP->GetMidiOutTable()->GetDevice(Id).CtrlChange(Channel, (int)CMidi::MidiCC::ALL_NOTES_OFF, 0);
	}
}

void CMsSong::MidiStart()
{
	//--------------------------------------
	// MidiStart
	//	Send the Midi Start out of the 
	// specified midi port
	//--------------------------------------
	int NumMidiOutDevices = GETAPP->GetMidiOutTable()->GetNumDevices();;
	int* DevIds = new int[NumMidiOutDevices];
	int i;
	int Id;

	for (i = 0; i < NumMidiOutDevices; ++i)
		DevIds[i] = 0;
	for (i = 1; i < 16; ++i)
	{
		Id = GetTrackInfo(i)->GetMidiOutDeviceID();
		if (DevIds[Id] == 0)
		{
			DevIds[Id] = 1;
			GETAPP->GetMidiOutTable()->GetDevice(Id).Start();
		}
	}
}

void CMsSong::MidiStop()
{
	//---------------------------------------
	// MidiStop
	//	Send the midi STOP message out of the
	// specified midi port
	//---------------------------------------
	int NumMidiOutDevices = GETAPP->GetMidiOutTable()->GetNumDevices();;
	int* DevIds = new int[NumMidiOutDevices];
	int i;
	int Id;

	for(i=0; i < NumMidiOutDevices; ++i)
		DevIds[i] = 0;

	for (i = 1; i < 16; ++i)
	{
		Id = GetTrackInfo(i)->GetMidiOutDeviceID();
		if (DevIds[Id] == 0)
		{
			DevIds[Id] = 1;
			GETAPP->GetMidiOutTable()->GetDevice(Id).Stop();
		}
	}
}

void CMsSong::MidiContinue()
{
	//------------------------------------------
	// MidiContinue
	//	Send the Midi CONTINUE message out of
	// the specified midi port
	//-----------------------------------------
	int NumMidiOutDevices = GETAPP->GetMidiOutTable()->GetNumDevices();;
	int* DevIds = new int[NumMidiOutDevices];
	int i;
	int Id;

	for (i = 0; i < NumMidiOutDevices; ++i)
		DevIds[i] = 0;
	for (i = 1; i < 16; ++i)
	{
		Id = GetTrackInfo(i)->GetMidiOutDeviceID();
		if (DevIds[Id] == 0)
		{
			DevIds[Id] = 1;
			GETAPP->GetMidiOutTable()->GetDevice(Id).Continue();
		}
	}
}

void CMsSong::ChangePatch(int Track, int MidiChannel, int Patch)
{
	//----------------------------------------------
	// ChangePatch
	//	Send the change patch message to the 
	// specified midi port to the specified
	// channel
	//
	// parameters:
	// 	Track.....Instrument to change patch on
	//	chan......Midi channel number
	//	patch.....Patch number to change to
	//-----------------------------------------------
	int DeviceID;

	DeviceID = GetTrackInfo(Track)->GetMidiOutDeviceID();
/*	if(LogFile())fprintf(
		LogFile(), 
		"Change Patch: Track %d, Midi Channel %d, Patch %d, DeviceID %d\n", 
		Track, 
		MidiChannel, 
		Patch, 
		DeviceID
	);*/
	GETAPP->GetMidiOutTable()->GetDevice(DeviceID).PgmChange(MidiChannel, Patch);	// Change patch
}

CMsTrack* CMsSong::GetTrackInfo(int TrackNumber)
{
	CMsTrack* pTrack = nullptr;

	if ((TrackNumber > 0) && (TrackNumber < 16))
	{
		pTrack = GetSongInfo()->GetTrack(TrackNumber);
	}
	else
	{
		CString cs;
		cs.Format(_T("Invalid Track Number %d"), TrackNumber);
		AfxMessageBox(cs, MB_OK);
	}
	return pTrack;
}

bool CMsSong::ValidateFile()
{
	bool rV = true;
	int c = m_pFileBuffer[0] & 0xff;
	char* pS = new char[16];
	int i;

	if (c != 0xcd)
		rV = false;
	else
	{
		for (i = 0; i < 7; i++)
		{
			pS[i] = m_pFileBuffer[i + 1];
		}
		pS[i] = 0;
		if (strcmp(pS, "Mstudio") != 0)
			rV = false;
	}
	return rV;
}

int CMsSong::DumpSong(FILE* pOutFile)
{
	//--------------------------------------
	// <summary>
	// 	** Parse **
	// This function is used to parse
	// the raw data of the Music Studio Song
	// file data to create a data base
	//
	//	parameter:
	//		pSongData....pointer to the song data
	// </summary>
	// <param name="pSongData"></param>
	// <returns></returns>
	//--------------------------------------
	int rV = 0;
	int loop = 1;
	int c;
	int i = 0;
	int Event = 0;

	int KeySig;
	int Tempo;
	int TimeSig;
	int Loudness;
	int Repeat;
	int t1, t2;
	int Size, ls = 0;
	int l = 2048;
	char* pS = new char[l];

	while (loop)
	{
		c = ParserGetC();
		ls = 0;
		switch (c)	/*	switch song data	*/
		{
		case MSFF_TOKEN_EOF:
			loop = 0;
			rV = 0;
			break;
		case MSFF_TOKEN_REPEAT_STOP:	//basically the same thing as an End Event
			Size = l - ls;
			ls = sprintf_s(&pS[ls], Size, "Repeat END");
			break;
		case MSFF_TOKEN_EVENT_END:
			Size = l - ls;
			ls = sprintf_s(&pS[ls], Size, "Event END");
			break;
		case MSFF_TOKEN_KEY_SIGNATURE:
			KeySig = ParserGetC();
			KeySig = ParserGetC();
			Size = l - ls;
			ls = sprintf_s(&pS[ls], Size, "KEY Signature:$%d", KeySig);
			break;
		case MSFF_TOKEN_TEMPO:
			Tempo = ParserGetC();
			Size = l - ls;
			ls = sprintf_s(&pS[ls], Size, "Tempo = %d", Tempo);
			break;
		case MSFF_TOKEN_BAR:	//don't do much of anything
			Size = l - ls;
			ls = sprintf_s(&pS[ls], Size, "Measure Bar");
			break;
		case MSFF_TOKEN_TIME_SIGNATURE:
			TimeSig = ParserGetC();
			Size = l - ls;
			ls = sprintf_s(&pS[ls], Size, "Time Signature %d", TimeSig);
			break;
		case MSFF_TOKEN_LOUDNESS:
			Loudness = ParserGetC();
			Loudness = ParserGetC();
			Size = l - ls;
			ls = sprintf_s(&pS[ls], Size, "Loudness = %d", Loudness);
			break;
		case MSFF_TOKEN_REPEAT_START:
			Repeat = ParserGetC();
			Size = l - ls;
			ls = sprintf_s(&pS[ls], Size, "Repeat Start = %d Counts", Repeat);
			break;
		case MSFF_TOKEN_END:
			Size = l - ls;
			ls = sprintf_s(&pS[ls], Size, "SONG END");
			loop = 0;
			rV = 0;
			break;
		default:
			//--------------------------------`
			//if we got down here, what was found must be a note
			//so, we first get the track/type,duration,accidentals
			//and then the note number
			//If, the note starts a tie, treat it like any other
			//note, if a note is at the end of a tie, then find
			//the note in the time out buffer.  If the note is
			//the beginning and end of a tie, still find it
			//in the timeout buffer
			//-------------------------------
			t1 = ParserGetC();	/*	duration	*/
			t2 = ParserGetC();	/*	note mask off MSB (upside down note)	*/
			//----------------------------------
			// Create a new Note Object
			//----------------------------------
			Size = l - ls;
			if (c & MSFF_REST_FLAG)
				ls += sprintf_s(&pS[ls], Size, "REST:%d", t2 & 0x7f);
			else
				ls += sprintf_s(&pS[ls], Size, "NOTE:%d", t2 & 0x7f);
			if ((t1 & MSFF_ACCENT_FLAG))
			{
				Size = l - ls;
				ls += sprintf_s(&pS[ls], Size, " Accent");
			}
			Size = l - ls;
			ls += sprintf_s(&pS[ls], Size, " Duration:%d", t2 & 0x1f);
			Size = l - ls;
			ls += sprintf_s(&pS[ls], Size, " Accidental:%d", (t1 & MSFF_KEY_MASK) >> MSFF_KEY_SHIFT);
			if (c & MSFF_BEGTIE_FLAG)
			{
				Size = l - ls;
				ls += sprintf_s(&pS[ls], Size, " Tie Start");
			}
			if (c & MSFF_ENDTIE_FLAG)
			{
				Size = l - ls;
				ls += sprintf_s(&pS[ls], Size, " Tie End");
			}
			Size = l - ls;
			ls += sprintf_s(&pS[ls], Size, " Track:%d", c & MSFF_TRACK_MASK);
			if (t2 & MSFF_NOTE_UPSIDE_DOWN)
			{
				Size = l - ls;
				ls += sprintf_s(&pS[ls], Size, " Stem Down");
			}
			break;
		}	/*	end of switch (c)	*/
//		fprintf(pOutFile, "%s\n", pS);
	}	/*	end while	*/
	return rV;
}

SColorPalette* CMsSong::GetColorPalette()
{
	return m_pChildView->GetColorPalette();
}