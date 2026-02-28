// MsTimeSig.h: interface for the CMsTimeSig class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

constexpr auto TS_OFFSET_X = EVENT_WIDTH - (EVENT_WIDTH * 3) / 4;	// offset of time signature from left edge of event
class CMsTimeSignature : public CMsObject
{
public:
	enum class TimeSigID : int {
		TS_2_2,
		TS_3_2,
		TS_2_4,
		TS_3_4,
		TS_4_4,
		TS_5_4,
		TS_6_8,
		TS_ERROR
	};
private:
	struct TimeSigInfoItem {
		TimeSigID m_id;
		const char* m_pTimeSigString;
	};

	inline static const TimeSigInfoItem TimeSigLut[] = {
			{TimeSigID::TS_2_2, "2/2"},		//1
			{TimeSigID::TS_3_2, "3/2"},		//2
			{TimeSigID::TS_2_4, "2/4"},		//3
			{TimeSigID::TS_3_4, "3/4"},		//4
			{TimeSigID::TS_4_4, "4/4"},		//5
			{TimeSigID::TS_5_4, "5/4"},		//6
			{TimeSigID::TS_6_8, "6/8"},		//7
			{TimeSigID::TS_ERROR, "NA"}		//8
	};

	TimeSigID m_TimeSig;
public:
	CMsTimeSignature();
	virtual ~CMsTimeSignature();
	bool Create(CMsSong* pSong, CMsEvent* pEvent, TimeSigID TS);
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
	TimeSigID GetTimeSignature(void) const {return m_TimeSig;}
	void SetTimeSignature(TimeSigID t){m_TimeSig = t;}

};

