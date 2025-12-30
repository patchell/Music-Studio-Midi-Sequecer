// MsFlat.h: interface for the CMsFlat class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CMsFlat : public CMsObject
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
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse);
	virtual int IsTimedObject(){ return 0; }
	virtual bool DoesSomething() {
		return false;
	}
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void Draw(CDC* pDC, int event, int maxevent) {}
	//------------------------------------------------------
	void Draw(CDC *pDC, COLORREF c, int x, int y);

};
