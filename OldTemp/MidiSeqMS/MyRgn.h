#pragma once

class CMyRgn :public CRgn
{
public:
    CMyRgn();
    virtual ~CMyRgn();
    virtual bool CreateRectRgn(CRect rect);
};

