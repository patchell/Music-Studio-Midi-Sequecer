#include "pch.h"

CComboBlockOps::CComboBlockOps() : CComboDropDown(ComboDropDownTypes::BLOCK)
{
}

CComboBlockOps::~CComboBlockOps()
{
}

bool CComboBlockOps::Create(CPoint ptULCorner, CWnd* pParentWnd)
{
	/****************************************************
	**	Create
	** This function creates the combo box for block
	** operations.
	**
	** parameters:
	**	ptULCorner...the upper left corner of the combo
	**                box in the parent window's
	**                coordinates.
	**  pParentWnd..the parent window of the combo box.
	****************************************************/
	int i, nItems;
	CSize szItemSize;

	nItems = GETAPP->GetNumBlockOps();
	szItemSize = GETAPP->bmGetBlockOpType(0)->GetBmDim();
	szItemSize += CSize(4, 4);
	CComboDropDown::Create(
		nItems - 2,
		nItems,
		szItemSize,
		ptULCorner,
		CSize(16, szItemSize.cy),
		pParentWnd,
		IDC_COMBO_BLOCKOPS
	);
	for (i = 0; i < nItems; ++i)
		AddBitmap(GETAPP->bmGetBlockOpType(i));
	return true;
}