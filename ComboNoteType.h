#pragma once

class CComboNoteType : public CComboDropDown
{
public:
	CComboNoteType();
	virtual ~CComboNoteType();
	bool Create(
		CPoint ptUpperLeft,
		CWnd* pParentWnd
	);
};

