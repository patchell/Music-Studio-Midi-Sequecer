// ChildFrameBase.cpp : implementation file
//

#include "pch.h"


// CChildFrameBase

IMPLEMENT_DYNAMIC(CChildFrameBase, CMDIChildWnd)

CChildFrameBase::CChildFrameBase() noexcept
{
	m_pNext = 0;
	m_pPrev = 0;
}

CChildFrameBase::~CChildFrameBase()
{
}


BEGIN_MESSAGE_MAP(CChildFrameBase, CMDIChildWnd)
END_MESSAGE_MAP()



// CChildFrameBase message handlers

BOOL CChildFrameBase::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.cx = CHILD_FRAME_WIDTH;
	cs.cy = CHILD_FRAME_HEIGHT;	
	cs.style |= WS_CHILD | WS_VISIBLE | WS_THICKFRAME | WS_CAPTION;
	cs.style &= ~WS_SIZEBOX;
	cs.style &= ~FWS_ADDTOTITLE;
	if (!CMDIChildWnd::PreCreateWindow(cs))
		return false;

	return true;
}
