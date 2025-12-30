#pragma once

class CMsSongInfo;

// CStaticCheckBitmap

class CStaticCheckBitmap : public CStatic
{
	DECLARE_DYNAMIC(CStaticCheckBitmap)
	int m_Check;
	COLORREF m_Color;
	int m_TrackID;
	CMsSongInfo* m_pSongInfo;
public:
	CStaticCheckBitmap();
	virtual ~CStaticCheckBitmap();
	int GetCheck() { return m_Check; }
	void SetColor(COLORREF color) { m_Color = color; }
	void SetTrackID(int id) { m_TrackID = id; }
	void SetSongInfo(CMsSongInfo* pInfo) { m_pSongInfo = pInfo; }
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


