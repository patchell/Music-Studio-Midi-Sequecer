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
	virtual int MouseLButtonDown(int DrawState, CPoint pointMouse);
	virtual int MouseLButtonUp(int DrawState, CPoint pointMouse);
	virtual int MouseMove(int DrawState, CPoint pointMouse);
	virtual bool IsTimedObject() {
		return false;
	};
	virtual bool DoesSomething() {
		return false;
	}
	//------------------------------------------------------
	virtual void Print(FILE *pO, int Indent);
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual bool AddToQueue() { return false; }
	virtual bool RemoveFromQueue(CMsSong* pSong) { return false; }
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
};
