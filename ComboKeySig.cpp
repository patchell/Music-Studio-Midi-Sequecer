#include "pch.h"

CComboKeySig::CComboKeySig()
{
}

CComboKeySig::~CComboKeySig()
{
}

bool CComboKeySig::Create(
	CPoint ptUpperLeft,
	CWnd* pParentWnd
)
{
	CSize szDropArrowSize;
	int nTotalItems = GETAPP->GetNumKeySigs();
	CSize szItemSize;
	int i;

	szItemSize = GETAPP->bmGetCBKeySignature(1)->GetBmDim();
	szItemSize += CSize(8, 4);
	szDropArrowSize = CSize(16, szItemSize.cy);
	CComboDropDown::Create(
		nTotalItems - 2,			//Number of items to display
		nTotalItems,				//Total Items
		szItemSize,					//size of items in combobox
		ptUpperLeft,				//upper left corner
		szDropArrowSize,			//size of drop arrow
		pParentWnd,					//parent window
		IDC_COMBO_KEYSIG			//control ID
	);
	for (i = 0; i < nTotalItems; ++i)
		AddBitmap(GETAPP->bmGetCBKeySignature(i + 1));
	return true;
}