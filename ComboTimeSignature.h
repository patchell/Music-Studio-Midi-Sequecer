#pragma once

class CComboTimeSignature : public CComboDropDown
{
public:
	CComboTimeSignature();
	virtual ~CComboTimeSignature();
	bool Create(
		CPoint ptUpperLeft,
		CWnd* pParentWnd
	);
};

