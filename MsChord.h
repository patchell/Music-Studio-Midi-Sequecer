#pragma once

class CMsChord : public CMsObject
{
	CMsChordNote* m_pNoteChainHead;
	CMsChordNote* m_pNoteChainTail;
	UINT m_ArpegioInterations;	//Sets the number of Octaves from Base
								// ==0 -> Disabled
	UINT m_ArpegioDelay;	//amount of time (Ticks) between notes
							//The sum of the number of notes
							//In the chord times delay must be
							//less than the "duration" of the
							//chord
	BOOL m_ArpegioDirection;	//0 -> Up, 1 -> Down
	INT m_ChordId;				//what cord is it, -1 = Custom
	UCHAR* m_pIntervals;
	UINT m_NumberOfIntervals;
	UINT m_RootPitch;
public:
	CMsChord();
	virtual ~CMsChord();
	virtual CMsObject* Copy(void);
	virtual void Draw(CDC* pDC, int event, int maxevent);
	virtual void Save(FILE* pO);
	virtual void Print(FILE* pO);
	void AddChordNoteAtHead(CMsChordNote* pCN);
	void AddChordNoteAtTail (CMsChordNote* pCN);
	void RemoveChordNote(CMsChordNote* pCN);
	void AddInterval(UINT Interval);
	CMsChordNote* GetChordNoteHead() { return m_pNoteChainHead; }
	CMsChordNote* GetChordNoteTail() { return m_pNoteChainTail;  }
	void SetChordNoteHead(CMsChordNote* pCN) { m_pNoteChainHead = pCN; }
	void SetCordNoteTail(CMsChordNote* pCM) { m_pNoteChainTail = pCM; }
	UINT GetArpegioIterations() {return m_ArpegioInterations;}
	void SetArpegioIterations(UINT AI) { m_ArpegioInterations = AI; }
	UINT GetArpegioDelay(){ return m_ArpegioDelay; }
	void SetArpegioDelay(UINT AD) { m_ArpegioDelay = AD; }
	void SetArpegioDirection(BOOL Dir) { m_ArpegioDirection = Dir; }
	BOOL IsArpegioDirectionUp() { return (m_ArpegioDirection == FALSE); }
	BOOL IsArpegioDirectionDown() { return (m_ArpegioDirection == TRUE); }

};

