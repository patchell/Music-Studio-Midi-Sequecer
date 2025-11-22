#include "pch.h"

CMsChord::CMsChord()
{
	m_pNoteChainHead = 0;
	m_pNoteChainTail = 0;
	m_ArpegioDelay = 0;
	m_ArpegioDirection = false;
	m_ArpegioInterations = 0;
	m_ChordId = -1;
	m_NumberOfIntervals = 0;
	m_ObjType = MSOBJ_CHORD;
	m_RootPitch = 0;
	m_pIntervals = new UCHAR[12];
}

CMsChord::~CMsChord()
{
	delete[] m_pIntervals;
}

bool CMsChord::Create(CMsSong* pSong, CMsEvent* pEvent)
{
	CMsObject::Create(pSong, pEvent);
	return true;
}

UINT CMsChord::Process()
{
	return 0;
}

UINT CMsChord::Play()
{
	return 0;
}

int CMsChord::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsChord::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsChord::MouseMove(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

CMsObject* CMsChord::Copy(void)
{
	return nullptr;
}

void CMsChord::Draw(CDC* pDC, int event, int maxevent)
{
}

void CMsChord::Save(FILE* pO)
{
}

void CMsChord::Print(FILE* pO, int Indent)
{
}

void CMsChord::AddChordNoteAtHead(CMsChordNote* pCN)
{
	if (m_pNoteChainHead == 0)
	{
		m_pNoteChainHead = pCN;
		m_pNoteChainTail = pCN;
	}
	else
	{
		pCN->SetNext(m_pNoteChainHead);
		m_pNoteChainHead->SetPrev(pCN);
		m_pNoteChainHead = pCN;
	}
}

void CMsChord::AddChordNoteAtTail(CMsChordNote* pCN)
{
	if (m_pNoteChainHead == 0)
	{
		m_pNoteChainHead = pCN;
		m_pNoteChainTail = pCN;
	}
	else
	{
		pCN->SetPrev(m_pNoteChainTail);
		m_pNoteChainTail->SetNext(pCN);
		m_pNoteChainTail = pCN;
	}
}

void CMsChord::RemoveChordNote(CMsChordNote* pCN)
{
	if (pCN == m_pNoteChainHead)
	{
		// remove from head
		m_pNoteChainHead =(CMsChordNote *) pCN->GetNext();
		if (m_pNoteChainHead)
			m_pNoteChainHead->SetPrev(0);
		else
			m_pNoteChainTail = 0;
	}
	else if (pCN == m_pNoteChainTail)
	{
		// remove from tail
		m_pNoteChainTail = (CMsChordNote*)pCN->GetPrev();
		if (m_pNoteChainTail)
			m_pNoteChainTail->SetNext(0);
		else
			m_pNoteChainHead = 0;
	}
	else
	{
		// remove from middleish
		pCN->GetNext()->SetPrev(pCN->GetPrev());
		pCN->GetPrev()->SetNext(pCN->GetNext());
	}
}

void CMsChord::AddInterval(UINT Interval)
{
	CMsChordNote* pCN;
	//----------------------------
	// Create a note for the chord
	//----------------------------
	pCN = new CMsChordNote;
	pCN->SetPitch(IntervalLUT[Interval].Semitones);
}
