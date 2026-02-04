#pragma once

class CMsPortamentoStart : public CMsObject
{
public:
	CMsPortamentoStart();
	~CMsPortamentoStart();
	bool Create(CMsSong* pSong, CMsEvent* pEvent);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse) = 0;
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse) = 0;
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition) = 0;
	virtual int IsTimedObject() {
		return 0;
	}
	virtual bool DoesSomething() {
		return true;
	}
	virtual void Draw(CDC* pDC);
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	virtual void Save(FILE* pO);
	virtual void Copy(CMsObject* Source);
	virtual void Print(FILE* pO, int Indent);
};

