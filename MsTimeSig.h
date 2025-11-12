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
	void Create(CMsSong* pSong, UINT ParentEvent, INT TS);
	virtual UINT Process();
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual void Print(FILE *pO);
	INT GetTimeSignature(void){return m_TimeSig;}
	void SetTimeSignature(INT t){m_TimeSig = t;}

};

