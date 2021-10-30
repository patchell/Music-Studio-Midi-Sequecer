// MsTimeSig.h: interface for the CMsTimeSig class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

class CMsTimeSignature : public CMsObject
{
	UINT m_TimeSig;
public:
	CMsTimeSignature();
	virtual ~CMsTimeSignature();
	void Create(CMsSong* pSong, UINT ParentEvent, UINT TS);
	virtual UINT Process();
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual void Print(FILE *pO);
	UINT GetTimeSignature(void){return m_TimeSig;}
	void SetTimeSignature(UINT t){m_TimeSig = t;}
};

