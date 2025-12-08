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
	virtual int MouseLButtonDown(int DrawState, CPoint pointMouse);
	virtual int MouseLButtonUp(int DrawState, CPoint pointMouse);
	virtual int MouseMove(int DrawState, CPoint pointMouse);
	virtual bool IsTimedObject() {
		return true;
	};
	virtual bool DoesSomething() {
		return true;
	}
	//------------------------------------------------------

};

