// ComboBoxItem.h: interface for the CComboBoxItem class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CComboBoxItem  
{
public:
	int m_nCheckboxChecked;
	int m_nCheckboxEnabled;
	CMyBitmap * m_pbmBitmap;
	CComboBoxItem();
	virtual ~CComboBoxItem();

};
