/****************************************
** CLASS CMyBitmap
**
** Derived from CMyBitmap, this class adds
** the functionality of being able to
** load a CMyBitmap from a file.
****************************************/
#include "pch.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBitmap::CMyBitmap():CBitmap()
{

}

CMyBitmap::~CMyBitmap()
{

}

BOOL CMyBitmap::LoadBitmapFromFile(const char *filename)
{
	/********************************************
	**	LoadBitmapFromFile
	** This function loads a bitmap from a
	** bmp file.
	**
	** parameter:
	**	filename....points to the name of
	**              file that contains the bitmap
	********************************************/
	DeleteObject();
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage(NULL, CString(filename), IMAGE_BITMAP, 0, 0,
	LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	return Attach(hBitmap);
}


CSize CMyBitmap::GetBmDim()
{
	BITMAP bm;

	GetBitmap(&bm);
	return CSize(bm.bmWidth,bm.bmHeight);
}

CMyBitmap * CMyBitmap::CoppyBitmap()
{
	CMyBitmap* pNewBM = 0;
	BITMAP bm;

	GetBitmap(&bm);
	pNewBM = new CMyBitmap;
	pNewBM->CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes, bm.bmBitsPixel, NULL);
	DWORD count = bm.bmWidth * bm.bmHeight * bm.bmBitsPixel / 8;
	pNewBM->SetBitmapBits(count,bm.bmBits);
	return pNewBM;
}

