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
	virtual void Print(FILE *pO, int Indent);
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual UINT Play(CMsSong* pSong);
	virtual bool AddToQueue() { return false; }
	virtual UINT Process(CMsSong* pSong);
	virtual bool RemoveFromQueue(CMsSong* pSong) { return false; }
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);
};
