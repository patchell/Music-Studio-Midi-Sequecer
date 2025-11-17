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
	void Create(CMsSong*pSong, UINT ParentEvent,UINT Loudness);
	void Create(CMsSong* pSong, CMsEvent* pEvent, UINT Loudness);
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual void Print(FILE *pO);
	virtual UINT Process();
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	UINT GetLoudness(void){return m_Loudness;}
	void SetLoudness(UINT l){m_Loudness = l;}
};
