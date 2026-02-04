// MsLoudness.h: interface for the CMsLoudness class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


class CMsLoudness : public CMsObject  
{
	UINT m_Loudness;
public:
	CMsLoudness();
	virtual ~CMsLoudness();
	bool Create(CMsSong* pSong, CMsEvent* pEvent, UINT Loudness);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual int IsTimedObject() { return 0; }
	virtual bool DoesSomething() {
		return true;
	}
	virtual void Draw(CDC* pDC);
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	virtual void Save(FILE *pO);
	virtual void Copy(CMsObject* Source);
	virtual void Print(FILE *pO, int Indent);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	UINT GetLoudness(void){
		return m_Loudness;
	}
	void SetLoudness(UINT l){m_Loudness = l;}
};
