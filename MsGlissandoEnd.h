#pragma once

class CMsGlissandoEnd : public CMsObject
{
	CMsGlissando* m_pParentGlissando;
	bool m_bStemDown;
	bool m_bSelected;
	int m_Pitch;
public:
	CMsGlissandoEnd();
	virtual ~CMsGlissandoEnd();
	bool Create(CMsSong* pSong, CMsGlissando*  pParent, CMsEvent* pEvEnd);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual DRAWSTATE  MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual int IsTimedObject() {
		return 0;
	}
	virtual bool DoesSomething() {
		return false;
	}
	virtual void Draw(CDC* pDC);
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	virtual void Print(FILE* pO, int Indent);
	virtual void Save(FILE* pO);
	virtual void Copy(CMsObject* Source);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	//------------------------------------------------------
	int GetPitch() const {
		return m_Pitch;
	}
	void SetPitch(int pitch) {
		m_Pitch = pitch;
	}
	bool IsStemDown() const {
		return m_bStemDown;
	}
	void SetStemDown(bool down) {
		m_bStemDown = down;
	}
	bool IsSelected() const {
		return m_bSelected;
	}
	void SetSelected(bool selected) {
		m_bSelected = selected;
	}
};

