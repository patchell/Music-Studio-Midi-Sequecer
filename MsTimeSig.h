// MsTimeSig.h: interface for the CMsTimeSig class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

class CMsTimeSignature : public CMsObject
{
	inline static const char* TimeSigLut[] = {
			"NA",		//0
			"2/2",		//1
			"3/2",		//2
			"2/4",		//3
			"3/4",		//4
			"4/4",		//5
			"5/4",		//6
			"6/8",		//7
			"NA"
	};

	INT m_TimeSig;
public:
	CMsTimeSignature();
	virtual ~CMsTimeSignature();
	bool Create(CMsSong* pSong, CMsEvent* pEvent, INT TS);
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
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	virtual void Save(FILE *pO);
	virtual void Copy(CMsObject* Source);
	virtual void Print(FILE *pO, int Indent);
	INT GetTimeSignature(void){return m_TimeSig;}
	void SetTimeSignature(INT t){m_TimeSig = t;}

};

