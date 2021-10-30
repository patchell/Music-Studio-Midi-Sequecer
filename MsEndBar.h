// MsEnd.h: interface for the CMsEndBar class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CMsEndBar : public CMsObject  
{
public:
	CMsEndBar();
	virtual ~CMsEndBar();
	void Create(CMsSong* pSong, UINT nParentEvent);
	virtual void Print(FILE *pO);
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual UINT Play(CMsSong* pSong);
	virtual BOOL AddToQueue() { return FALSE; }
	virtual UINT Process(CMsSong* pSong);
	virtual BOOL RemoveFromQueue(CMsSong* pSong) { return FALSE; }
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
};
