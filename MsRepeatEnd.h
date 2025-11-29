// MsRepeatEnd.h: interface for the CMsRepeatEnd class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


class CMsRepeatEnd : public CMsObject  
{
public:
	CMsRepeatEnd();
	virtual ~CMsRepeatEnd();
	bool Create(CMsSong* pSong, CMsEvent* pEvent);
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
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual void Print(FILE *pO, int Indent);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);

};
