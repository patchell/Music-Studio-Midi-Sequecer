#pragma once

class CMsGlissando : public CMsObject
{
	inline static
		int NotePos[12] = {
			24,	//C
			24,	//C#
			20,	//D
			20,	//D#
			16,	//E
			12,	//F
			12,	//F#
			8,	//G
			8,	//G#
			4,	//A
			4,	//A#
			0	//B
	};
	inline static
		int NoteLut[7] = {
			0,	//C
			2,	//D
			4,	//E
			5,	//F
			7,	//G
			9,	//A
			11	//B
	};

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
	DRAWSTATE m_DrawState;
	GlisandoState m_GlisandoState;
	GlisandoDirection m_GlisandoDirection;
	CMsEvent* m_pEvEnd;
	int m_Track;
	int m_TotalDurationTicks;
	int m_TotalIntervals;
	int m_StartPitch;
	bool m_StartStemDown;
	int m_EndPitch;
	bool m_EndStemDown;
	int m_CurrentPitch;
	int m_TicksPerInterval;
	int m_Ticks;
	bool m_bSelected;
public:
	CMsGlissando();
	~CMsGlissando();
	bool Create(CMsSong* pSong, CMsEvent* pEvStart, CMsEvent* pEvEnd);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual int IsTimedObject();
	virtual bool DoesSomething() {
		return true;
	}
	UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	virtual void Copy(CMsObject* Source);
	virtual void Draw(CDC* pDC);
	virtual void Save(FILE* pO);
	virtual void Print(FILE* pO, int Indent);
	//------------------------------------------------------
	// Drawing Methodes
	//------------------------------------------------------
	void DrawNote(CDC* pDC, int yPos, bool bStart, bool bStemDown);
	void DrawGlisandoSquiggleLine(CDC* pDC, int xStart, int yStart, int xEnd, int yEnd);
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
	int GetStartPitch() { return m_StartPitch; }
	void SetStartPitch(int p) { m_StartPitch = p; }
	int GetEndPitch() { return m_EndPitch; }
	void SetEndPitch(int p) { m_EndPitch = p; }
	bool IsSelected() { return m_bSelected; }
	void SetSelected(bool sel) { m_bSelected = sel; }
	int NotePitchToYPos(int Pitch);
	int YPosToNotePitch(int YPos);
	int QuantizeY(int Y);
	int NeedsLine(int Pitch);
};

