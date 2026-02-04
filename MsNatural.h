// MsNatural.h: interface for the CMsNatural class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMsNatural : public CMsObject
{
public:
	CMsNatural();
	virtual ~CMsNatural();
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual int IsTimedObject() {
		return 0;
	}
	virtual bool DoesSomething() {
		return false;
	}
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void Draw(CDC* pDC) {}
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent) {
		return StaffMouseStates::MOUSE_STAFF_STATE_NONE;
	}
	//------------------------------------------------------
	void Draw(CDC *pDC, COLORREF c, int x, int y);
};
