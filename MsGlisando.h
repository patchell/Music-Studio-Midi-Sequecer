#pragma once

class CMsGlisando : public CMsObject
{
public:
	enum class GlisandoState
	{
		START,
		RUNNING,
		END
	};
	enum class GlisandoDirection
	{
		UP,
		DOWN
	};
private:
	GlisandoState m_GlisandoState;
	GlisandoDirection m_GlisandoDirection;
	CMsEvent* m_pEvEnd;
	int m_Track;
	int m_TotalDurationTicks;
	int m_TotalIntervals;
	int m_StartPitch;
	int m_EndPitch;
	int m_CurrentPitch;
	int m_TicksPerInterval;
	int m_Ticks;
public:
	CMsGlisando();
	~CMsGlisando();
	bool Create(CMsSong* pSong, CMsEvent* pEvStart, CMsEvent* pEvEnd);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse);
	virtual int IsTimedObject();
	virtual bool DoesSomething() {
		return true;
	}
	//------------------------------------------------------
	virtual CMsObject* Copy(void);
	virtual void Draw(CDC* pDC, int event, int maxevent);
	virtual void Save(FILE* pO);
	virtual void Print(FILE* pO, int Indent);
	//------------------------------------------------------
	// Midi Methodes
	//------------------------------------------------------
	void NoteOff(UINT Velociry);
	void NoteOn(UINT Velociry);
	//------------------------------------------------------
	int CalculateTotalDuration();
	void SetEndEvent(CMsEvent* pEvEnd) { m_pEvEnd = pEvEnd; }
	CMsEvent* GetEndEvent() { return m_pEvEnd; }
	int GetTrack() { return m_Track; }
	void SetTrack(int t) { m_Track = t; }
};

