// MsNatural.h: interface for the CMsNatural class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMsNatural  
{
public:
	CMsNatural();
	virtual ~CMsNatural();
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual int MouseLButtonDown(int DrawState, CPoint pointMouse);
	virtual int MouseLButtonUp(int DrawState, CPoint pointMouse);
	virtual int MouseMove(int DrawState, CPoint pointMouse);
	virtual bool IsTimedObject() {
		return false;
	};
	virtual bool DoesSomething() {
		return false;
	}
	//------------------------------------------------------
	void Draw(CDC *pDC, COLORREF c, int x, int y);
};
