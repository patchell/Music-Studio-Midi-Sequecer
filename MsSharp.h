// MsSharp.h: interface for the CMsSharp class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


class CMsSharp : public CMsObject
{
public:
	CMsSharp();
	virtual ~CMsSharp();
	bool Create(CMsSong* pSong);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse);
	virtual int IsTimedObject() {
		return 0;
	}
	virtual bool DoesSomething() {
		return false;
	}
	virtual UINT ObjectToString(CString& csString, UINT mode = 0)
	{
		csString.Format(_T("Sharp"));
		return csString.GetLength();
	}
	//------------------------------------------------------
	void Draw(CDC* pDC, COLORREF c, int x, int y);

};
