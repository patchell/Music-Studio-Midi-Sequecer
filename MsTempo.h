// MsTempo.h: interface for the CMsTempo class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMsTempo : public CMsObject  
{
	UINT m_Tempo;	//quarter notes/minute
public:
	CMsTempo();
	virtual ~CMsTempo();
	bool Create(CMsSong* pSong, CMsEvent* pEvent, UINT Tempo) {
		m_Tempo = Tempo;
		return CMsObject::Create(pSong, pEvent);
	}
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
	UINT GetQNPM() { return m_Tempo; }
	void SetQNPM(UINT t) {
		if (this)
		{
			m_Tempo = t;
		}
		else
			::MessageBox(NULL, _T("ERROR"), _T("EventQueue NULL"), MB_OK);
	}
};


