#pragma once

class CComboTracks : public CComboDropDown
{
public:
	CComboTracks();
	virtual ~CComboTracks();
	bool Create(CPoint ptUpperLeft, CWnd* pParentWnd);
	CMyBitmap* GetTrackBitMap(int nTrack);
};

