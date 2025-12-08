// MsLoudness.h: interface for the CMsLoudness class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


class CMsLoudness : public CMsObject  
{
	UINT m_Loudness;
public:
	CMsLoudness();
	virtual ~CMsLoudness();
	bool Create(CMsSong* pSong, CMsEvent* pEvent, UINT Loudness);
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
		return true;
	}
	//------------------------------------------------------
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual void Print(FILE *pO, int Indent);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	UINT GetLoudness(void){return m_Loudness;}
	void SetLoudness(UINT l){m_Loudness = l;}
};
