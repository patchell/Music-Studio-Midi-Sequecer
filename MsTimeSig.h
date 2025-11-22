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
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual void Print(FILE *pO, int Indent);
	INT GetTimeSignature(void){return m_TimeSig;}
	void SetTimeSignature(INT t){m_TimeSig = t;}

};

