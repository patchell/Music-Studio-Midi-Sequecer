#include "pch.h"

CComboAccidentals::CComboAccidentals()
{
}

CComboAccidentals::~CComboAccidentals()
{
}

bool CComboAccidentals::Create(CPoint ptULCorner, CWnd* pParentWnd)
{
	/****************************************************
	**	Create
	** This function creates the combo box for
	** accidentals.
	**
	** parameters:
	**	ptULCorner...the upper left corner of the combo
	**                box in the parent window's
	**                coordinates.
	**  pParentWnd..the parent window of the combo box.
	****************************************************/
	int i, nItems;
	CSize szItemSize;

	nItems = GETAPP->GetNumAccidentalTypes();
	szItemSize = GETAPP->bmGetCbAccidentalType(0)->GetBmDim();
	szItemSize += CSize(8, 4);
	CComboDropDown::Create(
		nItems,
		nItems,
		szItemSize,
		ptULCorner,
		CSize(16, szItemSize.cy),
		pParentWnd,
		IDC_COMBO_ACCIDENTALS
	);
	for (i = 0; i < nItems; ++i)
		AddBitmap(GETAPP->bmGetCbAccidentalType(i));
	SetName("Accidentals");
	return true;
}
