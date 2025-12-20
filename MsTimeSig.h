// MsTimeSig.h: interface for the CMsTimeSig class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

class CMsTimeSignature : public CMsObject
{
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
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse);
	virtual int IsTimedObject() {
		return 0;
	}
	virtual bool DoesSomething() {
		return false;
	}
	//------------------------------------------------------
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	virtual void Save(FILE *pO);
	virtual void Copy(CMsObject* Source);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual void Print(FILE *pO, int Indent);
	INT GetTimeSignature(void){return m_TimeSig;}
	void SetTimeSignature(INT t){m_TimeSig = t;}

};

