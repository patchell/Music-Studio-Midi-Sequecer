#pragma once

class CComboDecorations : public CComboDropDownToggle
{
	inline static int CBDecorationFlags[APP_NUM_DECORATIONS] = {
		COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_CHECKED,
		COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_EXCLUSIVE | COMBOTOGGLE_TOGGLE_CHECKED,
		COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_EXCLUSIVE | COMBOTOGGLE_TOGGLE_CHECKED,
		COMBOTOGGLE_TOGGLE_ENABLE | COMBOTOGGLE_TOGGLE_EXCLUSIVE | COMBOTOGGLE_TOGGLE_CHECKED
	};
public:
	CComboDecorations();
	virtual ~CComboDecorations();
	bool Create(CPoint ptUpperLeft, CWnd* pParentWnd);
	int GetDecorationFlags(int nItemIndex) const { 
		return CBDecorationFlags[nItemIndex]; 
	}
};

