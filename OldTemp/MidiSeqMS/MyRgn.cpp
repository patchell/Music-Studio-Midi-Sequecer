#include "pch.h"

CMyRgn::CMyRgn()
{

}

CMyRgn::~CMyRgn()
{

}


bool CMyRgn::CreateRectRgn(CRect rect)
{
    return CRgn::CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);;
}
