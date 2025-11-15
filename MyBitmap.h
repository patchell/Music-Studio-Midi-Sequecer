// MyBitmap.h: interface for the CMyBitmap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMyBitmap : public CBitmap  
{
public:
	CMyBitmap();
	virtual ~CMyBitmap();

	virtual CSize GetBmDim();
	bool LoadBitmapFromFile(const char* filename);
	CMyBitmap* CoppyBitmap();
};

