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
	void Create(CMsSong* pSong, UINT ParrentEvent, UINT Tempo) {
		m_Tempo = Tempo;
		CMsObject::Create(pSong, ParrentEvent);
	}
	void Create(CMsSong* pSong, CMsEvent* pEvent, UINT Tempo) {
		m_Tempo = Tempo;
		CMsObject::Create(pSong, pEvent);
	}
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual void Print(FILE *pO);
	virtual UINT Process();
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


