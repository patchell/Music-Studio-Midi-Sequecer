// MsSong.cpp: implementation of the CMsSong class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction CMsSong
//////////////////////////////////////////////////////////////////////


CMsSong::CMsSong()
{
	m_nMeasureBarCount = 0;
	m_pEventListHead = 0;
	m_pEventListTail = 0;
	m_nTotalEvents = 0;
	m_pChildView = 0;
	m_SongID = GETAPP->GetUniqueID();
	m_pNextSong = 0;
	m_pPrevSong = 0;
	m_pSongPosition = 0;
	m_PlaySongTimerEnable = 0;
	for(int i=0;i<16;++i)
		m_Patches[i] = 0;
	m_pLastLoudness = 0;
	m_pLastKeySignature = 0;
	m_pLastTimeSignature = 0;
	m_pLastTempo = 0;
	m_BufIndex = 0;
	m_pFileBuffer = 0;
	m_nFileBufferSize = 0;
	m_InFileSize = 0;
	m_pPlayerObjectQueue = 0;
	m_NoteCountOn = 0;     // count of note on events sent
	m_NoteCountOff = 0;    // count of note off events sent
	m_TickerState = TickerState::STOPPED;

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
			delete pEvDel;
		}
	}
	if(m_pPlayerObjectQueue)
	{
		delete m_pPlayerObjectQueue;
	}
}

void CMsSong::AddEventAtEnd(CMsEvent *pE)
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
		pE->SetPrev(m_pEventListTail);
		m_pEventListTail->SetNext(pE);
		m_pEventListTail = pE;
	}
	++m_nTotalEvents;
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

	UINT KeySig;
	UINT Tempo;
	UINT TimeSig;
	UINT Loudness;
	UINT Repeat;
	int t1, t2;
	ObjectTypes obj;
	obj.pObj = 0;
	CMsEvent *pEv = MakeNewEvent();
	pEv->SetIndex(Event++);

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
				obj.pRepE->Create(this, pEv);
				break;
			case MSFF_TOKEN_EVENT_END:
				AddEventAtEnd(pEv);
				pEv = MakeNewEvent();
				pEv->SetIndex(Event++);
				break;
			case MSFF_TOKEN_KEY_SIGNATURE:
				KeySig = ParserGetC();
				if(KeySig != 1)
					printf("*********** ERROR: KeySig != 1 **************\n");
				KeySig = ParserGetC();
				obj.pKey = new CMsKeySignature();
				obj.pKey->Create(this,pEv,KeySig);
				break;
			case MSFF_TOKEN_TEMPO:
				Tempo = ParserGetC();
				obj.pTemp = new CMsTempo();
				obj.pTemp->Create(this, pEv,Tempo);
				break;
			case MSFF_TOKEN_BAR:	//don't do much of anything
				obj.pBar = new CMsBar;
				obj.pBar->Create(this, pEv);
				break;
			case MSFF_TOKEN_TIME_SIGNATURE:
				TimeSig = ParserGetC();
				obj.pTime = new CMsTimeSignature();
				obj.pTime->Create(this, pEv, TimeSig);
				break;
			case MSFF_TOKEN_LOUDNESS:
				Loudness = ParserGetC();
				if(Loudness != 1)
					printf("*********** ERROR: Loudness != 1 **************\n");
				Loudness = ParserGetC();
				obj.pLoud = new CMsLoudness();
				obj.pLoud->Create(this,pEv, Loudness);
				break;
			case MSFF_TOKEN_REPEAT_START:
				Repeat = ParserGetC();
				obj.pRepS = new CMsRepeatStart();
				obj.pRepS->Create(this, 1,pEv);
				break;
			case MSFF_TOKEN_END:
				obj.pEnd = new CMsEndBar;
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
				obj.pNote->Create(NULL,this, pEv);
				//----------------------------------
				// Fill in the data
				//----------------------------------
				obj.pNote->SetAccent((t1 & MSFF_ACCENT_FLAG)?1:0);
				obj.pNote->SetDuration(t1 & MSFF_DURATION_MASK);
				obj.pNote->SetAccidental((t1 & MSFF_KEY_MASK) >> MSFF_KEY_SHIFT);
				obj.pNote->SetLegato(0);		//Not supported
				obj.pNote->SetPitch(t2 & 0x7f);
				obj.pNote->SetRest((c & MSFF_REST_FLAG)?1:0);
				obj.pNote->SetStacato(0);		//Not supported
				obj.pNote->SetTieBeg((c & MSFF_BEGTIE_FLAG)?1:0);
				obj.pNote->SetTieEnd((c & MSFF_ENDTIE_FLAG)?1:0);
				obj.pNote->SetTrack((c & MSFF_TRACK_MASK));
				obj.pNote->SetStemDown((t2 & MSFF_NOTE_UPSIDE_DOWN)?true:false);
				break;
		}	/*	end of switch (c)	*/
		if(obj.pObj)
		{
			pEv->AddObjectAtEnd(obj.pObj);
//			obj.pObj->Print(stdout);
		}
	}	/*	end while	*/
	RenumberEvents(NULL,NULL);
	return rV;
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

void CMsSong::Draw(CDC *pDC, int event, int maxevent,CRect *pRect)
{
	//------------------------------------
	// <summary>
	// 
	// </summary>
	// <param name="pDC"></param>
	// <param name="event"></param>
	// <param name="maxevent"></param>
	// <param name="pRect"></param>
	//------------------------------------
	int i = 0;
	int MaxX = pRect->right;
	CPen blk,*oldpen;

	blk.CreatePen(PS_SOLID,1,RGB(0,0,0));
	oldpen = pDC->SelectObject(&blk);
	//------------------------------------
	// draw staves
	//------------------------------------
	for(i=0;i<5;++i)	//treble
	{
		int y = i * 8 + STAVE_OFFSET;
		pDC->MoveTo(0,y);
		pDC->LineTo(MaxX,y);
	}
	int o = i * 8 + STAVE_OFFSET + 8;
	for(i=0;i<5;++i)	//bass
	{
		pDC->MoveTo(0,i*8+o);
		pDC->LineTo(MaxX,i*8+o);
	}
	//-----------------------------
	// draw clefs
	//-----------------------------
	CMyBitmap Treb,Bass, *oldBM;
	CSize szBass, szTreb;

	Treb.LoadBitmap(IDB_TREBLE_CLEFF);
	Bass.LoadBitmap(IDB_BASS_CLEFF);
	szTreb = Treb.GetBmDim();
	szBass = Bass.GetBmDim();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	oldBM = (CMyBitmap *)memDC.SelectObject(&Treb);
	pDC->BitBlt(2, TREBLE_CLEF_OFFSET,szTreb.cx,szTreb.cy,&memDC,0,0,SRCAND);
	memDC.SelectObject(&Bass);
	pDC->BitBlt(2, BASS_CLEF_OFFSET,szBass.cx,szBass.cy,&memDC,0,0,SRCAND);
	memDC.SelectObject(oldBM);
	pDC->SelectObject(oldpen);
	CMsEvent *pEv = GetEventObject(event);
	for(i=0;(i<maxevent+1) && pEv;++i,pEv = pEv->GetNext())
	{
		if(pEv)pEv->Draw(pDC,i,maxevent);
	}
}

int CMsSong::AddObjectToSong(int event, CMsObject *pObjectToAdd)
{
	CMsEvent *pEventList = m_pEventListHead;
	CMsObject* pObjectToRemove;;
	int loop = 1;
	CMsObject *pObj;
	int rV = 1;		//indicate object was added
	CMsEndBar* pEndBar = 0;

	while(loop && pEventList)
	{
		if ((pObj = pEventList->ContainsObjectType(MSOBJ_ENDBAR)))
		{
			pEventList->RemoveObject(pObj);
			if (pEventList->GetIndex() != event)
			{
				pEndBar = new CMsEndBar;
				pEndBar->Create(this, GetEventObject(event));
			}
		}
		if(pEventList->GetIndex() == event)
			loop = 0;
		else
			pEventList = pEventList->GetNext();
	}
	if(pEventList)	//found event, add object
	{
		if((pObjectToRemove = pEventList->ObjectAlreadyHere(pObjectToAdd)) != NULL)
			pEventList->RemoveObject(pObjectToRemove);
		else if (pObjectToAdd->GetType() != MSOBJ_ENDBAR)
		{
			if (pObjectToAdd->GetType() == MSOBJ_NOTE)
			{
				//-----------------------------
				// If the object is a note,
				// check to see if it is close
				// enough to other notes to
				// determine if the note head
				// need to be flipped
				//-----------------------------
			}
			pEventList->AddObjectAtEnd(pObjectToAdd);
		}

		if (pEndBar)
		{
			CMsEvent* pNewEvent = 0;
			pNewEvent = MakeNewEvent();
			AddEventAtEnd(pNewEvent);
			pNewEvent->AddObjectAtEnd(pEndBar);
		}
	}
	else
	{
		//--------------------------------------
		//we did not find event, so add events
		//until we get up to the right place
		//--------------------------------------
		CMsEventChain* pEvC = 0;
		UINT nEvents;
		CMsEvent* pEndList;

		nEvents = event - m_nTotalEvents + 1;
		if (nEvents > 1)
		{
			pEvC = new CMsEventChain;
			pEvC->CreateChain(nEvents, this, GetStaffChildView(), m_nTotalEvents);
			AddEventChain(m_nTotalEvents, pEvC);
		}
		else
		{
			CMsEvent* pEV;
			pEV = new CMsEvent;
			pEV->Create(this, GetStaffChildView(), GetStaffChildView()->GetDrawEvent());
			AddEventAtEnd(pEV);
		}
		RenumberEvents(nullptr, nullptr);
		pEndList = GetEventListTail();
		pEndList->AddObjectAtEnd(pObjectToAdd);
		if(pEndBar)
		{
			CMsEvent* pNewEvent = 0;
			pNewEvent = MakeNewEvent();
			AddEventAtEnd(pNewEvent);
			pNewEvent->AddObjectAtEnd(pEndBar);
		}
	}
//	GetEventListHead()->PrintEvents(theApp.LogFile(), "AddObjectToSong", 2);
	return rV;
}

UINT CMsSong::AddMoreEvenrsAtEnd(UINT NewEndEvent)
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
//	GetEventListHead()->PrintEvents(theApp.LogFile(), "AddMoreEventsAtEnd", 2);
	return NumberOfEvents;
}

void CMsSong::RenumberEvents(int *First, int *Last)
{
	int SelFlag = 0;
	int i = 0;
	m_nTotalEvents = 0;
	CMsEvent *pEv = GetEventListHead();
	
	while(pEv)
	{
		m_nTotalEvents++;
		pEv->SetIndex(i++);
		if(pEv->IsSelected() && !SelFlag)
		{
			SelFlag = 1;
			if(First) *First = pEv->GetIndex();
		}
		else if (pEv->IsSelected() && SelFlag)
		{
			if(pEv->GetNext())
			{
				if(!pEv->GetNext()->IsSelected())
					if(Last)*Last = pEv->GetIndex();
			}
			else
			{
				if(Last) *Last = pEv->GetIndex();
			}
		}
		pEv = pEv->GetNext();
	}
//	GetEventListHead()->PrintEvents(theApp.LogFile(), "RenumberEvents", 2);
}

CMsEvent *CMsSong::InsertEvent(int e)
{
	//--------------------------------
	//	InsertEvent
	//
	//		This function inserts an
	//	empty event at the given
	//	position.  If there are no
	// events at that position, then
	// this function will create
	// them until there is.
	//
	// parameters:
	//		e.....event position to insert new event
	//
	// return value:
	//		returns a pointer to the new
	//	event.
	//-------------------------------------
	CMsEvent *pEv = GetEventListHead();
	CMsEvent *pNewEv=0;

	int loop = 1;

	while(pEv && loop)
	{
		if(pEv->GetIndex() == e)	// find event position?
			loop = 0;	// yes break from loop
		else
		{
			if (NULL == pEv->GetNext())
			{
				//----------------------------
				// there are no more events
				// so we need to create them
				//----------------------------
				int i;
				for (i = pEv->GetIndex(); i <= e; ++i)
				{
					// create the needed number of events
					pEv = MakeNewEvent();
					AddEventAtEnd(pEv);
				}
				RenumberEvents(NULL,NULL);
				pNewEv = pEv;
				pEv = 0;
				loop = 0;
			}
			else
				pEv = pEv->GetNext();
		}
	}
	if(pEv)
	{
		pNewEv = MakeNewEvent();
		pNewEv->SetPrev(pEv);
		pNewEv->SetNext(pEv->GetNext());
		pEv->SetNext(pNewEv);
		if(pNewEv->GetNext())
			pNewEv->GetNext()->SetPrev(pNewEv);
	}
//	GetEventListHead()->PrintEvents(theApp.LogFile(), "InsertEvent", 2);
	return pNewEv;
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
		pOb = pEv->GetEventObjectHead();
		loop = 1;
		while(loop && pOb)
		{
			if(pOb->GetType() == MSOBJ_NOTE)
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

CMsEvent *CMsSong::GetEventObject(int Event)
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
	//		Event....Index of desired event
	//
	//	returns: pointer to the CMsEvent
	//			NULL if not found
	//-------------------------------------
	CMsEvent *pEv = GetEventListHead();
	int loop = 1;
	while(pEv && loop)
	{
		// Is this the one?
		if(pEv->GetIndex() == Event)
			loop = 0;	//Yes, stop seraching
		else
			// Keep seraching
			pEv = pEv->GetNext();
	}
	return pEv;
}

//--------------------------------------------
// The following is what I will consider to be
// the default Atari sound data.  It was taken
// from one of the song files that came on the 
// original disk.
//---------------------------------------------

int AtariInst[270] = {
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
		CMsObject *pOb = m_pEventListTail->GetEventObjectHead();
		while(pOb && NoEnd)
		{
			if(MSOBJ_ENDBAR == pOb->GetType())
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
		pEV = MakeNewEvent();
		AddEventAtEnd(pEV);
		pEV = MakeNewEvent();
		AddEventAtEnd(pEV);
		m_pEventListTail->AddObjectAtEnd(pME);
		RenumberEvents(NULL,NULL);
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
	GETMIDIINFO->Save(pO);
	//-----------------------------------
	// Save song content
	//-----------------------------------
	CMsEvent *pEv = GetEventListHead();
	while(pEv)
	{
		CMsObject *pObj;
		pObj = pEv->GetEventObjectHead();
		while(pObj)
		{
			pObj->Save(pO);
			pObj = pObj->GetNext();
		}
		fputc(0,pO);	//event delemiter
		pEv = pEv->GetNext();
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
					fprintf(theApp.LogFile(), "File:%s has %d Bytes\n", pName, m_InFileSize);
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
	delete[] pName;
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
	RenumberEvents(0, 0);
	delete pEvent;
}

CMsEvent* CMsSong::MakeNewEvent()
{
	CMsEvent* pEV = new CMsEvent();
	pEV->Create(this, GetStaffChildView(), GetStaffChildView()->GetDrawEvent());
//	printf("CMsSong Child View %p\n", m_pChildView);
	return pEV;
}

CMsObject * CMsSong::GetObjectTypeInEvent(int nType, int nEvent)
{
	CMsEvent *pEV = GetEventObject(nEvent);
	CMsObject *pObj = 0;
	if (pEV) pObj = pEV->ContainsObjectType(nType);
	return pObj;
}

CMsObject * CMsSong::GetMsObject(
	INT ObjType,
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

//		printf("    Search for Object in event %d\n", pEV->GetIndex());
		pObj = pEV->GetEventObjectHead();
		while (pObj && loop)
		{
			if (pObj->GetType() == ObjType)
			{
				pRetObj = pObj;
				loop = 0;
//				printf("      Object Found\n");
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


bool CMsSong::Create(CChildViewStaff* pCCV)
{
	m_pChildView = pCCV;
	m_pPlayerObjectQueue = new CMsPlayerQueue;
	m_pPlayerObjectQueue->Create(this);
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
			GetStaffChildView()->MessageBox(_T("InTernal Error"), _T("Bad Thing"), MB_OK);
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
			GetStaffChildView()->MessageBox(_T("InTernal Error"), _T("Bad Thing"), MB_OK);
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
			GetStaffChildView()->MessageBox(_T("InTernal Error"), _T("Bad Thing"), MB_OK);
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
		MidiStart();			// Send MidiStart command to Midi device
		m_MidiClockToggleFlag = 0;
		m_TickerState = TickerState::RUNNING;
		break;
	case TickerState::RUNNING:
		//--------------------------------
		// Send Midi Clock every other tick
		//-------------------------------
		if(!m_MidiClockToggleFlag)
		{
			m_MidiClockToggleFlag = 1;
		}
		else
		{
			MidiClock();			// Send first Midi Clock command
			m_MidiClockToggleFlag = 0;
		}
		PlayStatus = GetPlayerQueue()->Play(this);
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
			pNextEvent = GetNextEventToProcess();
			if (pNextEvent == 0)
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
				SetSongPosition(pNextEvent);
				GetPlayerQueue()->ProcessQueue(pNextEvent);
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

	bool Loop = true;

	if (pEv)
	{
		pEv = pEv->GetNext();
		while (pEv && Loop)
		{
			pObj = pEv->GetEventObjectHead();
			if (pObj)
				Loop = false;
			else
				pEv = pEv->GetNext();
		}
	}
	return pEv;
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
	NumMidiOutDevices = GETMIDIOUTTABLE.GetNumDevices();
	int i;
	for (i = 0; i < NumMidiOutDevices; ++i)
	{
		GETMIDIOUTTABLE.Clock(i);
	}
}

void CMsSong::MidiStart()
{
	//--------------------------------------
	// MidiStart
	//	Send the Midi Start out of the 
	// specified midi port
	//--------------------------------------
	int NumMidiOutDevices;
	NumMidiOutDevices = GETMIDIOUTTABLE.GetNumDevices();
	int i;
	for (i = 0; i < NumMidiOutDevices; ++i)
	{
		GETMIDIOUTTABLE.Start(i);
	}
}

void CMsSong::MidiStop()
{
	//---------------------------------------
	// MidiStop
	//	Send the midi STOP message out of the
	// specified midi port
	//---------------------------------------
	int NumMidiOutDevices;
	NumMidiOutDevices = GETMIDIOUTTABLE.GetNumDevices();
	int i;
	for (i = 0; i < NumMidiOutDevices; ++i)
	{
		GETMIDIOUTTABLE.Stop(i);
	}
}

void CMsSong::MidiContinue()
{
	//------------------------------------------
	// MidiContinue
	//	Send the Midi CONTINUE message out of
	// the specified midi port
	//-----------------------------------------
	int NumMidiOutDevices;
	NumMidiOutDevices = GETMIDIOUTTABLE.GetNumDevices();
	int i;
	for (i = 0; i < NumMidiOutDevices; ++i)
	{
		GETMIDIOUTTABLE.Continue(i);
	}
}

void CMsSong::ChangePatch(int Track, int chan, int patch)
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

	DeviceID = GETMIDIINFO->GetTrack(Track).GetMidiOutDeviceID();
	GETMIDIOUTDEVICE(DeviceID).PgmChange(chan, patch);
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

