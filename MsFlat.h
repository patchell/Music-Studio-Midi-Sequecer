// MsFlat.h: interface for the CMsFlat class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CMsFlat  
{
public:
	CMsFlat();
	virtual ~CMsFlat();
	bool Create(CMsSong* pSong);
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
