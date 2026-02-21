// MsBar.h: interface for the CMsBar class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


constexpr auto MEASUREBAR_OFFSET = EVENT_WIDTH - 1;
constexpr auto MEASUREBAR_NUMBER_HEIGHT = 14;
constexpr auto MEASUREBAR_NUMBER_OFFSET_Y = 12;
constexpr auto MEASUREBAR_NUMBER_OFFSET_X = 10;

class CMsBar : public CMsObject
{
	inline static UINT LastBarNumber = 0;
	int m_BarNumber;	// Measure Bar Number
public:
	CMsBar();
	virtual ~CMsBar();
	bool Create(CMsSong* pSong, CMsEvent* pEvent);
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
	virtual void Draw(CDC* pDC);
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	virtual void Print(FILE* pO, int Indent);
	virtual void Save(FILE* pO);
	virtual void Copy(CMsObject* pSource);
	virtual UINT Process(CMsSong* pSong);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual CMsObject* MakeANewObject();
	//------------------------------------------------------
	UINT GetBarNumber(void){return m_BarNumber;}
	void SetBarNumber(UINT Bm) { m_BarNumber = Bm; }
	UINT GenBarNumber() {
		CMsBar::LastBarNumber++;
		return CMsBar::LastBarNumber;
	}
};

