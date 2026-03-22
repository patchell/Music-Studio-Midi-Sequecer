// MsFlat.h: interface for the CMsFlat class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CMsFlat : public CMsObject
{
public:
	CMsFlat();
	virtual ~CMsFlat();
	virtual bool Create(CMsSong* pSong, CMsEvent* pEvent = 0);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual DRAWSTATE  MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual int IsTimedObject(){ return 0; }
	virtual bool DoesSomething() {
		return false;
	}
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void Draw(CDC* pDC) {}
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	void Draw(CDC *pDC, COLORREF c, int x, int y);
	virtual CMsObject* MakeANewObject(CMsSong* pSong, CMsEvent* pPqarentEvent);

};
