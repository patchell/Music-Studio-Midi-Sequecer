#pragma once

class CMyRgn :public CRgn
{
public:
    CMyRgn();
    virtual ~CMyRgn();
    virtual BOOL CreateRectRgn(CRect rect);
};

