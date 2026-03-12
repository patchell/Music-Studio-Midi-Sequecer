// MsRepeatEnd.h: interface for the CMsRepeatEnd class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMsRepeatStart;

class CMsRepeatEnd : public CMsObject  
{
	UINT m_Count;		// Number of times to repeat.  This is initialized when the repeat end is created, and is not changed during playback.
	UINT m_CountDown;	// Current repeat count during playback.  This is initialized to m_Count when the repeat start is processed, and is decremented each time the repeat start is encountered during playback.  When it reaches 0, the repeat end will allow the song to continue past the repeat end.
	CMsRepeatStart* m_pMatchingRepeatStart;
public:
	CMsRepeatEnd();
	virtual ~CMsRepeatEnd();
	virtual bool Create(CMsSong* pSong, CMsEvent* pEvent, int Count);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual DRAWSTATE  MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual int IsTimedObject() {
		return 0;
	}
	virtual bool DoesSomething() {
		return true;
	}
	virtual void Draw(CDC* pDC);
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	virtual void Save(FILE *pO);
	virtual void  Copy(CMsObject* Source);
	virtual void Print(FILE *pO, int Indent);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	bool DecrementRepeatCount() {
		bool rV = false;
		if (0 == --m_CountDown)
			rV = true;
		return rV;
	}
	UINT GetCountDown() { return m_CountDown; }
	CMsRepeatStart* GetMatchingRepeatStart() { return m_pMatchingRepeatStart; }
	void SetMatchingRepeatStart(CMsRepeatStart* pStart) { m_pMatchingRepeatStart = pStart; }
};
