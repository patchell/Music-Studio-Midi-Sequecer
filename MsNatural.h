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
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse);
	virtual int IsTimedObject() {
		return 0;
	}
	virtual bool DoesSomething() {
		return false;
	}
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void Draw(CDC* pDC, int event, int maxevent) {}
	//------------------------------------------------------
	void Draw(CDC *pDC, COLORREF c, int x, int y);
};
