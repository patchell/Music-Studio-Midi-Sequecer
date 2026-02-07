// MsKeySignature.h: interface for the CMsKeySignature class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CMsNote;

class CMsKeySignature : public CMsObject  
{
	inline static int m_KeySigCorrectionCMaj[12] = {
		0,	//C
		0,	//C#
		0,	//D
		0,	//D#
		0,	//E
		0,	//F
		0,	//F#
		0,	//G
		0,	//G#
		0,	//A
		0,	//A#
		0	//B
	};
	inline static int m_KeySigCorrectionGMaj[12] = {
		0,	//C
		0,	//C#
		0,	//D
		0,	//D#
		0,	//E
		1,	//F
		0,	//F#
		0,	//G
		0,	//G#
		0,	//A
		0,	//A#
		0	//B
	};
	inline static int m_KeySigCorrectionDMaj[12] = {
		1,	//C
		0,	//C#
		0,	//D
		0,	//D#
		0,	//E
		1,	//F
		0,	//F#
		0,	//G
		0,	//G#
		0,	//A
		0,	//A#
		0	//B
	};
	inline static int m_KeySigCorrectionAMaj[12] = {
		1,	//C
		0,	//C#
		0,	//D
		0,	//D#
		0,	//E
		1,	//F
		0,	//F#
		1,	//G
		0,	//G#
		0,	//A
		0,	//A#
		0	//B
	};
	inline static int m_KeySigCorrectionEMaj[12] = {
		1,	//C
		0,	//C#
		1,	//D
		0,	//D#
		0,	//E
		1,	//F
		0,	//F#
		1,	//G
		0,	//G#
		0,	//A
		0,	//A#
		0	//B
	};
	inline static int m_KeySigCorrectionBMaj[12] = {
		1,	//C
		0,	//C#
		1,	//D
		0,	//D#
		0,	//E
		1,	//F
		0,	//F#
		1,	//G
		0,	//G#
		1,	//A
		0,	//A#
		0	//B
	};
	inline static int m_KeySigCorrectionFSharpMaj[12] = {
		1,	//C
		0,	//C#
		1,	//D
		0,	//D#
		1,	//E
		1,	//F
		0,	//F#
		1,	//G
		0,	//G#
		1,	//A
		0,	//A#
		0	//B
	};
	inline static int m_KeySigCorrectionCSharpMaj[12] = {
		1,	//C
		0,	//C#
		1,	//D
		0,	//D#
		1,	//E
		1,	//F
		0,	//F#
		1,	//G
		0,	//G#
		1,	//A
		0,	//A#
		1	//B
	};
	//-----------------------------------
	inline static int m_KeySigCorrectionFMaj[12] = {
		0,	//C
		0,	//Db
		0,	//D
		0,	//Eb
		0,	//E- Fb
		0,	//F
		0,	//Gb
		0,	//G
		0,	//Ab
		0,	//A
		0,	//Bb
		-1	//B - Cb
	};
	inline static int m_KeySigCorrectionBFlatMaj[12] = {
		0,	//C
		0,	//Db
		0,	//D
		0,	//Eb
		-1,	//E- Fb
		0,	//F
		0,	//Gb
		0,	//G
		0,	//Ab
		0,	//A
		0,	//Bb
		-1	//B - Cb
	};
	inline static int m_KeySigCorrectionEFlatMaj[12] = {
		0,	//C
		0,	//Db
		0,	//D
		0,	//Eb
		-1,	//E- Fb
		0,	//F
		0,	//Gb
		0,	//G
		0,	//Ab
		-1,	//A
		0,	//Bb
		-1	//B - Cb
	};
	inline static int m_KeySigCorrectionDFlatMaj[12] = {
		0,	//C
		0,	//Db
		-1,	//D
		0,	//Eb
		-1,	//E- Fb
		0,	//F
		0,	//Gb
		0,	//G
		0,	//Ab
		-1,	//A
		0,	//Bb
		-1	//B - Cb
	};
	inline static int m_KeySigCorrectionAFlatMaj[12] = {
		0,	//C
		0,	//Db
		-1,	//D
		0,	//Eb
		-1,	//E- Fb
		0,	//F
		0,	//Gb
		-1,	//G
		0,	//Ab
		-1,	//A
		0,	//Bb
		-1	//B - Cb
	};

	inline static int m_KeySigCorrectionGFlatMaj[12] = {
	-1,	//C
	0,	//Db
	-1,	//D
	0,	//Eb
	-1,	//E- Fb
	0,	//F
	0,	//Gb
	-1,	//G
	0,	//Ab
	-1,	//A
	0,	//Bb
	-1	//B - Cb
	};
	inline static int m_KeySigCorrectionCFlatMaj[12] = {
	-1,	//C
	0,	//Db
	-1,	//D
	0,	//Eb
	-1,	//E- Fb
	-1,	//F
	0,	//Gb
	-1,	//G
	0,	//Ab
	-1,	//A
	0,	//Bb
	-1	//B - Cb
	};

public:
	enum class KeySigID : int {
		KS_ERROR = 0,
		// Sharps
		CMAJ = 1,
		GMAJ,
		DMAJ,
		AMAJ,
		EMAJ,
		BMAJ,
		FsMAJ,
		CsMAJ,
		// Flats
		FMAJ,
		BbMAJ,
		EbMAJ,
		AbMAJ,
		DbMAJ,
		GbMAJ,
		CbMAJ
	};
private:
	struct KeySigCorrectionLUTItem {
		KeySigID m_KeySigID;
		const char* m_pKeySigString;
		int * m_pKeySigCorrection;
		KeySigCorrectionLUTItem() {
			m_KeySigID = KeySigID::KS_ERROR;
			m_pKeySigString = nullptr;
			m_pKeySigCorrection = nullptr;
		}
		KeySigCorrectionLUTItem(KeySigID id, const char* pString, int* pCorrection) {
			m_KeySigID = id;
			m_pKeySigString = pString;
			m_pKeySigCorrection = pCorrection;
		}
	};
	inline static const KeySigCorrectionLUTItem KeySigCorrectionLUT[APP_NUM_KEYSIGNATURES + 1] = {
		{KeySigCorrectionLUTItem(KeySigID::KS_ERROR, "ERROR", nullptr)},
		{KeySigCorrectionLUTItem(KeySigID::CMAJ, "C MAJ", m_KeySigCorrectionCMaj)},
		{KeySigCorrectionLUTItem(KeySigID::GMAJ, "G MAJ", m_KeySigCorrectionGMaj)},
		{KeySigCorrectionLUTItem(KeySigID::DMAJ, "D MAJ", m_KeySigCorrectionDMaj)},
		{KeySigCorrectionLUTItem(KeySigID::AMAJ, "A MAJ", m_KeySigCorrectionAMaj)},
		{KeySigCorrectionLUTItem(KeySigID::EMAJ, "E MAJ", m_KeySigCorrectionEMaj)},
		{KeySigCorrectionLUTItem(KeySigID::BMAJ, "B MAJ", m_KeySigCorrectionBMaj)},
		{KeySigCorrectionLUTItem(KeySigID::FsMAJ, "F# MAJ", m_KeySigCorrectionFSharpMaj)},
		{KeySigCorrectionLUTItem(KeySigID::CsMAJ, "C# MAJ", m_KeySigCorrectionCSharpMaj)},
		{KeySigCorrectionLUTItem(KeySigID::FMAJ, "F MAJ", m_KeySigCorrectionFMaj)},	//four flats
		{KeySigCorrectionLUTItem(KeySigID::BbMAJ, "Bb MAJ", m_KeySigCorrectionBFlatMaj)},	//two flats
		{KeySigCorrectionLUTItem(KeySigID::EbMAJ, "Eb MAJ", m_KeySigCorrectionEFlatMaj)},	//three flats
		{KeySigCorrectionLUTItem(KeySigID::AbMAJ, "Ab MAJ", m_KeySigCorrectionAFlatMaj)},	//four flats
		{KeySigCorrectionLUTItem(KeySigID::DbMAJ, "Db MAJ", m_KeySigCorrectionDFlatMaj)},	//five flats
		{KeySigCorrectionLUTItem(KeySigID::GbMAJ, "Gb MAJ", m_KeySigCorrectionGFlatMaj)},	//six flats
		{KeySigCorrectionLUTItem(KeySigID::CbMAJ, "Cb MAJ", m_KeySigCorrectionCFlatMaj)}	//seven flats
	};
	KeySigID m_KeySignature;
public:
	CMsKeySignature();
	virtual ~CMsKeySignature();
	bool Create(CMsSong* pSong, CMsEvent* pParentEvent, KeySigID key);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual int IsTimedObject() { return 0; }
	virtual bool DoesSomething() {
		return true;
	}
	virtual void Draw(CDC* pDC);
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	bool IsFlatKeySig();
	virtual void Print(FILE *pO, int Indent);
	virtual void Save(FILE *pO);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0) { return 0; }
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	KeySigID GetKeySignature() {return m_KeySignature;}
	void SetKeySignature(KeySigID k){m_KeySignature = k;}
	int CorrectNoteByKeySig(int Note, int accidental);
private:
	int GetKeySigCorrection(int note);
public:
	void Copy(CMsObject* Source);
	int NoteToPosition(int Note);
	//--------------------------------------------------
	inline static CString KeySigStringTab[APP_NUM_KEYSIGNATURES + 1] = {
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

};
