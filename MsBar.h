// MsBar.h: interface for the CMsBar class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


constexpr auto MEASUREBAR_OFFSET = EVENT_WIDTH - 1;

class CMsBar : public CMsObject
{
	static UINT LastBarNumber;
	int m_BarNumber;	// Measure Bar Number
public:
	CMsBar();
	virtual ~CMsBar();
	void Create(CMsSong* pSong, UINT Event);
	void Create(CMsSong* pSong, CMsEvent* pEvent);
	virtual void Print(FILE* pO);
	virtual void Save(FILE* pO);
	virtual void Copy(CMsObject* pSource);
	virtual void Draw(CDC* pDC, int event, int maxevent);
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

