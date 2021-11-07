// MsKeySignature.cpp: implementation of the CMsKeySignature class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"


CString CMsKeySignature::KeySigStringTab[APP_NUM_KEYSIGNATURES + 1] = {
	_T("ERROR"),	//5
	_T("C MAJ"),	//1X
	_T("G MAJ"),	//2X
	_T("D MAJ"),	//3X
	_T("A MAJ"),	//4X
	_T("E MAJ"),	//5X
	_T("B MAJ"),	//6X
	_T("F# MAJ"),	//7X
	_T("C# MAJ"),	//8X
	_T("F MAJ"),	//9X
	_T("Bb MAJ"),	//10X
	_T("Eb MAJ"),	//11X
	_T("Ab MAJ"),	//12X
	_T("Db MAJ"),	//13X
	_T("Gb MAJ"),	//14X
	_T("Cb MAJ")	//15X
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsKeySignature::CMsKeySignature():CMsObject()
{
	m_ObjType = MSOBJ_KEYSIG;
	m_KeySignature = 0;
	SetKeySigCorrection();
}

CMsKeySignature::~CMsKeySignature()
{

}

void CMsKeySignature::Print(FILE *pO)
{
//	fprintf(pO,"Key Signature:%s\n",CMsKeySignature::KeySigStringTab[m_KeySignature]);
}

void CMsKeySignature::Draw(CDC *pDC, int event, int maxevent)
{
	CMsSharp sharp;
	CMsFlat flat;
	COLORREF color;

	if (IsSelected())
		color = RGB(255, 0, 0);
	else
		color = RGB(0, 0, 0);
	switch(m_KeySignature)
	{
		case MSFF_CMAJ:	///do nothing
			break;
		case MSFF_CSMAJ:	///seven shrps
			sharp.Draw(pDC,color,EVENT_OFFSET+EVENT_WIDTH*event+24,NoteToPosition(MSFF_NOTE_B2));	//base staff
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+24,NoteToPosition(MSFF_NOTE_B2+24));	//trebble staff
		case MSFF_FSMAJ:	///six sharps
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+12,NoteToPosition(MSFF_NOTE_E2+12));	//base staff
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+20,NoteToPosition(MSFF_NOTE_E2+24));	//trebble staff
		case MSFF_BMAJ:	///five sharps
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+16,NoteToPosition(MSFF_NOTE_A2));	//base staff
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+16,NoteToPosition(MSFF_NOTE_A2+24));	//trebble staff
		case MSFF_EMAJ:	///four D sharps
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+20,NoteToPosition(MSFF_NOTE_D2+12));	//base staff
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+12,NoteToPosition(MSFF_NOTE_D2+36));	//trebble staff
		case MSFF_AMAJ:	//three G sharps
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event,NoteToPosition(MSFF_NOTE_G2+12));	//base staff
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+8,NoteToPosition(MSFF_NOTE_G2+36));		//trebble staff
		case MSFF_DMAJ:	//two C sharps
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+4,NoteToPosition(MSFF_NOTE_C2+12));	//base staff
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+4,NoteToPosition(MSFF_NOTE_C2+36));	//trebble staff
		case MSFF_GMAJ:	///one F sharp
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+8,NoteToPosition(MSFF_NOTE_F2+12));	//base staff
			sharp.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event,NoteToPosition(MSFF_NOTE_F2+36));	//trebble staff
			break;
		case MSFF_CFMAJ:	//seven flats
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event,NoteToPosition(MSFF_NOTE_F2+12));	//bass staff
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event,NoteToPosition(MSFF_NOTE_F2+36));	//trebble staff
		case MSFF_GFMAJ:	//six flats
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+24,NoteToPosition(MSFF_NOTE_C2+12));	//bass staff
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+24,NoteToPosition(MSFF_NOTE_C2+36));	//trebble staff
		case MSFF_DFMAJ:	//five flats
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+24,NoteToPosition(MSFF_NOTE_G2));	//bass staff
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+24,NoteToPosition(MSFF_NOTE_G2+24));	//trebble staff
		case MSFF_AFMAJ:	//four flats
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event +16,NoteToPosition(MSFF_NOTE_D2+12));	//bass staff
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+16,NoteToPosition(MSFF_NOTE_D2+36));	//trebble staff
		case MSFF_EFMAJ:	//three flats
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+16,NoteToPosition(MSFF_NOTE_A2));	//bass staff
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+16,NoteToPosition(MSFF_NOTE_A2+24));	//trebble staff
		case MSFF_BFMAJ:	//two flats
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+8,NoteToPosition(MSFF_NOTE_E2+12));	//bass staff
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+8,NoteToPosition(MSFF_NOTE_E2+36));	//trebble staff
		case MSFF_FMAJ:	//one flat
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+8,NoteToPosition(MSFF_NOTE_B2));	//bass staff
			flat.Draw(pDC, color,EVENT_OFFSET+EVENT_WIDTH*event+8,NoteToPosition(MSFF_NOTE_B2+24));	//trebble staff
			break;
	}
}

UINT CMsKeySignature::Process()
{
	GetSong()->SetCurrentKeySignature(this);
	return 0;
}

void CMsKeySignature::ObjectRectangle(CRect& rect, UINT Event)
{
	rect.SetRect(0, 0, 0, 0);
}

void CMsKeySignature::SetKeySigCorrection(void)
{
	int KeySig = m_KeySignature;
	int i;
	for(i=0;i<12;++i)
	{
		m_KeySigCorrection[i] = 0;
		//---------------------------------
		// You might be wondering just
		// how exactly the following switch
		// statement works.  Except for
		// C major (MSFF_CMAJ), all of the
		// cases do not have a break.  For
		// example, C# major (MSFF_CSMAJ)
		// will go through all of the cases
		switch(KeySig)
		{
			case MSFF_CMAJ:
				break;
			case MSFF_CSMAJ:
				if(i == MSFF_NOTE_C)
					m_KeySigCorrection[i] = 1;
			case MSFF_FSMAJ:
				if(i == MSFF_NOTE_F)
					m_KeySigCorrection[i] = 1;
			case MSFF_BMAJ:
				if(i == MSFF_NOTE_A)
					m_KeySigCorrection[i] = 1;
			case MSFF_EMAJ:
				if(i == MSFF_NOTE_D)
					m_KeySigCorrection[i] = 1;
			case MSFF_AMAJ:
				if(i == MSFF_NOTE_G)
					m_KeySigCorrection[i] = 1;
			case MSFF_DMAJ:	//two sharps
				if(i == MSFF_NOTE_F)
					m_KeySigCorrection[i] = 1;
			case MSFF_GMAJ:	//one sharp
				if(i == MSFF_NOTE_C)
					m_KeySigCorrection[i] = 1;
				break;
			case MSFF_CFMAJ:
				if(i == MSFF_NOTE_F)
					m_KeySigCorrection[i] = -1;
			case MSFF_GFMAJ:
				if(i == MSFF_NOTE_C)
					m_KeySigCorrection[i] = -1;
			case MSFF_DFMAJ:
				if(i == MSFF_NOTE_G)
					m_KeySigCorrection[i] = -1;
			case MSFF_AFMAJ:
				if(i == MSFF_NOTE_D)
					m_KeySigCorrection[i] = -1;
			case MSFF_EFMAJ:
				if(i == MSFF_NOTE_A)
					m_KeySigCorrection[i] = -1;
			case MSFF_BFMAJ:
				if(i == MSFF_NOTE_E)
					m_KeySigCorrection[i] = -1;
			case MSFF_FMAJ:
				if(i == MSFF_NOTE_B)
					m_KeySigCorrection[i] = -1;
				break;
		}
	}
}

UINT CMsKeySignature::GetKeySigCorrection(UINT note, UINT accidental)
{
	UINT n = note % 12;
	switch(accidental)
	{
		case CMsNote::MSFF_ACCIDENTAL_INKEY:	//default
			note += m_KeySigCorrection[n];
			break;
		case CMsNote::MSFF_ACCIDENTAL_NATURAL:
			break;
		case CMsNote::MSFF_ACCIDENTAL_SHARP:
			note += 1;
			break;
		case CMsNote::MSFF_ACCIDNETAL_FLAT:
			note -= 1;
			break;
	}
	return note;
}

CMsObject * CMsKeySignature::Copy()
{
	CMsObject *pOb = 0;
	CMsKeySignature *pB = new CMsKeySignature;
	*pB = *this;
	pOb = pB;
	return pOb;
}

void CMsKeySignature::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_KEY_SIGNATURE,pO);
	fputc(0x01,pO);
	fputc(m_KeySignature,pO);
}

int CMsKeySignature::NoteToPosition(int Note)
{
	extern int NotePos[12];
	return NotePos[Note % 12] + 28 * (7 - (Note / 12)) + (STAVE_OFFSET - 44);
}