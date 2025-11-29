#pragma once

class CMsChordNote : public CMsNote
{
    UINT m_IntervalType;
public:
    CMsChordNote();
    virtual ~CMsChordNote();
	bool Create(CMsSong* pSong);
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

};

