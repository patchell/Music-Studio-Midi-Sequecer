#include "pch.h"

CComboTracks::CComboTracks()
{
}

CComboTracks::~CComboTracks()
{
}

bool CComboTracks::Create(
	CPoint ptUpperLeft,
	CWnd* pParentWnd
)
{
	CSize szDropArrowSize;
	int nTotalItems = ((CChildViewStaff*)pParentWnd)->GetSong()->GetNumberOfTracks();;
	CSize szItemSize;
	int i;

	szItemSize = CSize(28, 28);
	szItemSize += CSize(8, 4);
	szDropArrowSize = CSize(16, szItemSize.cy);
	CComboDropDown::Create(
		7,							//Number of items to display
		nTotalItems,				//Total Items
		szItemSize,					//size of items in combobox
		ptUpperLeft,				//upper left corner
		szDropArrowSize,			//size of drop arrow
		pParentWnd,					//parent window
		IDC_COMBO_INSTRUMENT			//control ID
	);
	for (i = 1; i < (nTotalItems + 1); ++i)
	{
		AddBitmap(GetTrackBitMap(i));

	}
	return true;
}

CMyBitmap* CComboTracks::GetTrackBitMap(int nTrack)
{
	return ((CChildViewStaff*)GetParent())->GetSongInfo()->GetTrackBitmap(nTrack);
}
