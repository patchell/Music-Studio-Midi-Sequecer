// MsEnd.h: interface for the CMsEndBar class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

constexpr auto ENDBAR_OFFSET = EVENT_WIDTH - 1;
constexpr auto ENDBAR_NUMBER_HEIGHT = 14;
constexpr auto ENDBAR_NUMBER_OFFSET_Y = 12;
constexpr auto ENDBAR_NUMBER_OFFSET_X = 10;

class CMsEndBar : public CMsObject  
{
public:
	CMsEndBar();
	virtual ~CMsEndBar();
	virtual bool Create(CMsSong* pSong, CMsEvent* pParentEvent);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual DRAWSTATE  MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual int IsTimedObject(){ return 0; }
	virtual bool DoesSomething() {
		return false;
	}
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void Draw(CDC* pDC);
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	virtual CMsObject* MakeANewObject(CMsSong* pSong, CMsEvent* pPqarentEvent);
	virtual void Print(FILE *pO, int Indent);
	virtual void Save(FILE *pO);
	virtual void Copy(CMsObject* Source);
	virtual bool AddToQueue() { return false; }
	virtual bool RemoveFromQueue(CMsSong* pSong) { return false; }
	virtual void ObjectRectangle(CRect& rect, UINT Event);
};
