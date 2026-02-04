// MsTempo.h: interface for the CMsTempo class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

constexpr auto TEMPO_X = 20;	//pixels from left side of event
constexpr auto TEMPO_Y = 10;	//pixels from top of event

class CMsTempo : public CMsObject  
{
	UINT m_Tempo;	//quarter notes/minute
public:
	CMsTempo();
	virtual ~CMsTempo();
	bool Create(CMsSong* pSong, CMsEvent* pEvent, UINT Tempo);	//-------------------------------------------------
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
	UINT GetQNPM() { return m_Tempo; }
	void SetQNPM(UINT t) {
		if (this)
		{
			m_Tempo = t;
		}
		else
			::MessageBox(NULL, _T("ERROR"), _T("EventQueue NULL"), MB_OK);
	}
};


