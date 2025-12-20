// MsEnd.h: interface for the CMsEndBar class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CMsEndBar : public CMsObject  
{
public:
	CMsEndBar();
	virtual ~CMsEndBar();
	bool Create(CMsSong* pSong, CMsEvent* pParentEvent);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse);
	virtual int IsTimedObject(){ return 0; }
	virtual bool DoesSomething() {
		return false;
	}
	//------------------------------------------------------
	virtual void Print(FILE *pO, int Indent);
	virtual void Save(FILE *pO);
	virtual void Copy(CMsObject* Source);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual bool AddToQueue() { return false; }
	virtual bool RemoveFromQueue(CMsSong* pSong) { return false; }
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
};
