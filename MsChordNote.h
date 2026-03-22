#pragma once

class CMsChordNote : public CMsNote
{
    UINT m_IntervalType;
public:
    CMsChordNote();
    virtual ~CMsChordNote();
	virtual bool Create(CMsSong* pSong, CMsEvent* pEvent);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual DRAWSTATE  MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual int IsTimedObject();
	virtual bool DoesSomething() {
		return true;
	}
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	virtual CMsObject* MakeANewObject(CMsSong* pSong, CMsEvent* pPqarentEvent);

};

