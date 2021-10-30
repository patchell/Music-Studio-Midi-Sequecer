// MsRepeatEnd.h: interface for the CMsRepeatEnd class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


class CMsRepeatEnd : public CMsObject  
{
public:
	CMsRepeatEnd();
	virtual ~CMsRepeatEnd();
	void Create(CMsSong* pSong, UINT ParentEvent);
	virtual void Save(FILE *pO);
	virtual CMsObject * Copy(void);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual void Print(FILE *pO);
	virtual UINT Process();
	virtual UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual void ObjectRectangle(CRect& rect, UINT Event);

};
