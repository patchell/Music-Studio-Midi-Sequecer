#include "pch.h"

static COLORREF DefaultColors[16] = {
	RGB(0,0,0),			//Not used
	RGB(0,0,0),			//Track 1
	RGB(0,0,192),		//Track 2
	RGB(100,0,192),		//Track 3
	RGB(0,100,192),		//Track 4
	RGB(0,96,255),		//Track 5
	RGB(0,192,0),		//Track 6
	RGB(96,208,0),		//Track 7
	RGB(0,208,96),		//Track 8
	RGB(96,208,128),	//Track 9
	RGB(208,255,64),	//Track 10
	RGB(192,0,0),		//Track 11
	RGB(192,0,128),		//Track 12
	RGB(224,86,96),		//Track 13
	RGB(224,67,148),		//Track 14
	RGB(255,80,192)		//Track 15
};

static COLORREF DefaultTextColors[16] = {

	RGB(255,255,255),			//Not used
	RGB(255,255,255),			//Track 1
	RGB(255,255,255-192),		//Track 2
	RGB(155,255,255-192),		//Track 3
	RGB(255,155,255-192),		//Track 4
	RGB(255,255-96,0),			//Track 5
	RGB(255,255-192,255),		//Track 6
	RGB(255-96,255-208,255),	//Track 7
	RGB(255,255-208,255-96),	//Track 8
	RGB(255-96,255-208,155-128),	//Track 9
	RGB(255-208,0,255-64),		//Track 10
	RGB(255-192,255,255),		//Track 11
	RGB(255-192,255,255-128),	//Track 12
	RGB(255-224,255-86,255-96),	//Track 13
	RGB(255-224,255-67,255-148),//Track 14
	RGB(0,255-80,255-192)		//Track 15
};

CMsTrackSettings  CMsTrack::Settings[16];

void CMsTrack::GetSettings()
{
	int i;
	CString s;

	for (i = 1; i < 16; ++i)
	{
		s.Format(_T("COLOR_%d"), i);
		CMsTrack::Settings[i].m_Color = GETAPP->GetProfileIntW(
			_T("MsTrack"), 
			s,
			(INT)DefaultColors[i]
		);
		s.Format(_T("TEXT_COLOR_%d"), i);
		CMsTrack::Settings[i].m_TextColor = GETAPP->GetProfileIntW(
			_T("MsTrack"), 
			s, 
			DefaultTextColors[i]
		);
	}
}

void CMsTrack::SaveSettings()
{
	int i;
	CString s;

	for (i = 1; i < 16; ++i)
	{
		s.Format(_T("COLOR_%d"), i);
		GETAPP->WriteProfileInt(
			_T("MsTrack"),
			s,
			CMsTrack::Settings[i].m_Color
		);
		s.Format(_T("TEXT_COLOR_%d"), i);
		 GETAPP->WriteProfileInt(
			_T("MsTrack"),
			s,
			 CMsTrack::Settings[i].m_TextColor
		);
	}

}

bool CMsTrack::Create(
	CDC* pDC,
	int TrackID,
	CRect& rect,
	int MidiOutDevice,
	int Range,
	int MidiChannel,
	int Patch
)
{
	m_MidiDeviceID = MidiOutDevice;
	m_PitchRange = Range;
	m_MidiChannel = MidiChannel;
	m_PatchNumber = Patch;
	m_Color = CMsTrack::Settings[TrackID].m_Color;
	m_TextColor = CMsTrack::Settings[TrackID].m_TextColor;
	m_Number = TrackID;
	m_ptUpperLeft = rect.TopLeft();
	m_szDimensions = rect.Size();
	m_bmTrack.CreateBitmap(
		rect.Width(),
		rect.Height(),
		1,
		32,
		NULL
	);
	DWORD count = rect.Width() * rect.Height() * sizeof(long);
	bmBits = new char[count];
	m_bmTrack.SetBitmapBits(count, bmBits);
	m_Font.CreateFontW(
		rect.Height() - 10,
		(rect.Width() -5)/2,
		0,
		0,
		FW_DONTCARE,
		false,
		false,
		false,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		DEFAULT_PITCH,
		_T("Courier")
	);
	char* s = new char[4];
	if(m_Number > 9)
		sprintf_s(s, 4, "%2d", m_Number);
	else
		sprintf_s(s, 4, "%d", m_Number);
	m_csText = CString(s);
	delete[] s;
	Draw( pDC);
	return true;
}

void CMsTrack::Draw(CDC* pDC)
{
	CDC dc;
	CFont* pOldFont;
	CMyBitmap* pOldBM;
	CSize TextSize;
	CPoint ptCenter;
	CBrush brush, *pOldBrush;

	dc.CreateCompatibleDC(pDC);
	brush.CreateSolidBrush(CMsTrack::Settings[m_Number].m_Color);
	pOldBrush = dc.SelectObject(&brush);
	pOldBM = (CMyBitmap *)dc.SelectObject(&m_bmTrack);
	dc.SetTextColor(m_TextColor);
	dc.SetBkColor(m_Color);
	dc.FloodFill(1, 1, m_Color);
	pOldFont = dc.SelectObject(&m_Font);
	TextSize = dc.GetTextExtent(m_csText);
	ptCenter = CPoint(m_szDimensions.cx / 2, m_szDimensions.cy / 2);
	m_ptText = CPoint(
		ptCenter.x - TextSize.cx / 2,
		ptCenter.y - TextSize.cy / 2
	);
	dc.TextOutW(
		m_ptText.x,
		m_ptText.y,
		m_csText);
	dc.SelectObject(pOldBM);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldFont);
}

void CMsTrack::UpdateTrackBitmap()
{
	m_bmTrack.DeleteObject();
	m_bmTrack.CreateBitmap(
		m_szDimensions.cx,
		m_szDimensions.cx,
		1,
		32,
		NULL
	);
	DWORD count = m_szDimensions.cx * m_szDimensions.cy * sizeof(long);
	bmBits = new char[count];
	m_bmTrack.SetBitmapBits(count, bmBits);
	CDC* pDC = GETMAINFRAME->GetDC();
	Draw(pDC);
}
