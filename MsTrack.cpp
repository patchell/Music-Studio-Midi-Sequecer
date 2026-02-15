#include "pch.h"


void CMsTrack::GetDefaultTrackSettings(int TrackNumber, int MidiOutDviceID)
{
	m_MidiChannel = DefaultMidiChannel[TrackNumber];
	m_PatchNumber = (int) DefaultPatch[TrackNumber];
	strncpy_s(m_InstrumentName, sizeof(m_InstrumentName), CMidi::GenMidiPatchNames[m_PatchNumber], _TRUNCATE);
	m_PitchRange = DefaultPitchRange[TrackNumber];
	m_Color = DefaultColors[TrackNumber];
	m_TextColor = DefaultTextColors[TrackNumber];
	m_MidiDeviceID = MidiOutDviceID;
}

void CMsTrack::GetColorSettings()
{
	CString s;

	s.Format(_T("COLOR_%d"), m_Number);
	m_Color = GETAPP->GetProfileIntW(
		_T("MsTrack"), 
		s,
		(INT)DefaultColors[m_Number]
	);
	s.Format(_T("TEXT_COLOR_%d"), m_Number);
	m_TextColor = GETAPP->GetProfileIntW(
		_T("MsTrack"), 
		s, 
		DefaultTextColors[m_Number]
	);
}

void CMsTrack::SaveColorSettings()
{
	CString s;

	s.Format(_T("COLOR_%d"), m_Number);
	GETAPP->WriteProfileInt(
		_T("MsTrack"),
		s,
		m_Color
	);
	s.Format(_T("TEXT_COLOR_%d"), m_Number);
		GETAPP->WriteProfileInt(
		_T("MsTrack"),
		s,
		m_TextColor
	);
}

bool CMsTrack::Create(
	CMsSongInfo* pParentSongInfo, // Parent song info
	CSize szTrackIconSize,		// size of track bitmap Icon
	int MidiOutDevId,	// Midi output device ID
	int TrackID,		// Track number 1->15
	int Range,			// Pitch range
	int MidiChannel,	// Midi channel
	int Patch			// Patch number
)
{
	m_pParentSongInfo = pParentSongInfo;
	m_MidiDeviceID = MidiOutDevId;
	m_PitchRange = Range;
	m_MidiChannel = MidiChannel;
	m_PatchNumber = Patch;
	m_Number = TrackID;
	GetColorSettings();
	m_ptUpperLeft = CPoint(0, 0);
	m_szDimensions = szTrackIconSize;
	m_bmTrack.CreateBitmap(
		szTrackIconSize.cx,
		szTrackIconSize.cy,
		1,
		32,
		NULL
	);
	DWORD count = szTrackIconSize.cx * szTrackIconSize.cy * sizeof(long);
	bmBits = new char[count];
	m_bmTrack.SetBitmapBits(count, bmBits);
	m_Font.CreateFontW(
		szTrackIconSize.cy - 10,
		(szTrackIconSize.cx -5)/2,
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
//	Draw( pDC);
	return true;
}

bool CMsTrack::Create(
	CMsSongInfo* pParentSongInfo, 
	CSize szTrackIconSize, 
	int MidiOutDevId, 
	int TrackID		// Track number 1->15
)
{
	m_Number = TrackID;
	m_pParentSongInfo = pParentSongInfo;
	m_ptUpperLeft = CPoint(0, 0);
	m_szDimensions = szTrackIconSize;
	GetDefaultTrackSettings(m_Number, MidiOutDevId);
	//-------------------------------
	// Create bitmap
	//-------------------------------
	m_bmTrack.CreateBitmap(
		szTrackIconSize.cx,
		szTrackIconSize.cy,
		1,
		32,
		NULL
	);
	DWORD count = szTrackIconSize.cx * szTrackIconSize.cy * sizeof(long);
	bmBits = new char[count];
	memset(bmBits, 0, count);
	m_bmTrack.SetBitmapBits(count, bmBits);
	//---------------------------------
	// Create font
	//---------------------------------
	m_Font.CreateFontW(
		szTrackIconSize.cy - 4,
		(szTrackIconSize.cx - 2) / 2,
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
	if (m_Number > 9)
		m_csText.Format(_T("%2d"), m_Number);
	else
		m_csText.Format(_T("%1d"), m_Number);
	CWnd* pWin = GetParentSongInfo()->GetParentSong()->GetStaffChildView();
	CDC* pDC = pWin->GetDC();
	Draw(pDC);
	pWin->ReleaseDC(pDC);
	GetColorSettings();
	return true;
}

void CMsTrack::Save(FILE* pO)
{
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
	brush.CreateSolidBrush(m_Color);
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
