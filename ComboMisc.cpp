#include "pch.h"


CComboMisc::CComboMisc() : CComboDropDown(ComboDropDownTypes::MISC)
{
}

CComboMisc::~CComboMisc()
{
}

bool CComboMisc::Create(CPoint ptULCorner, CWnd* pParentWnd)
{
	int i, nItems;
	CSize szItemSize;

	nItems = GETAPP->GetNumMisc();
	szItemSize = GETAPP->bmGetMiscType(0)->GetBmDim();
	szItemSize += CSize(4, 4);
	CComboDropDown::Create(
		nItems - 2,
		nItems,
		szItemSize,
		ptULCorner,
		CSize(
			DROPBOX_ARROW_WIDTH,
			szItemSize.cy
		),
		pParentWnd,
		IDC_COMBO_MISCSTUFF
	);
	for (i = 0; i < nItems; ++i)
		AddBitmap(GETAPP->bmGetMiscType(i));
	SetName("Misc");
	return true;
}
