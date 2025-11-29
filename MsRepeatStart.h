// MsRepeatStart.h: interface for the CMsRepeatStart class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CMsRepeatStart : public CMsObject  
{
	UINT m_Count;
	UINT m_CountDown;
public:
	CMsRepeatStart();
	virtual ~CMsRepeatStart();
	bool Create(CMsSong* pSong, UINT Count, CMsEvent* pEvent);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse);
	virtual int IsTimedObject() {
		return 0;
	}
	virtual bool DoesSomething() {
		return true;
	}
	//------------------------------------------------------
	virtual void Print(FILE* pO, int Indent);
	inline int GetCount(void) { return m_Count; }
	inline void SetCount(int c) { m_Count = c; }
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	bool DecrementRepeatCount() {
		bool rV = false;
		if (0 == --m_CountDown)
			rV = true;
		return rV;
	}
	UINT GetCountDown() { return m_CountDown; }
};

