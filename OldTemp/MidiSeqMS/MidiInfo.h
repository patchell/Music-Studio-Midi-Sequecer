#pragma once

class CMidiInfo
{
	CMsTrack Tracks[16];
	char m_SongTitle[32];
public:
	CMidiInfo();
	virtual ~CMidiInfo();
	void Create( CDC* pDC);
//void Create(char* pRawMidiData);
	//-------------------------------------
	// Track Attributes
	//-------------------------------------
	CMsTrack& GetTrack(int TrackID) { return Tracks[TrackID]; }
	char* GetInstrumentName(int inst) { return GetTrack(inst).GetInstrumentName(); }
	void SetInstrumentName(int inst, const char* pName) { GetTrack(inst).SetInstrumentName(pName); }

	void SetEnable(int InstID, int Bank, int Enable) {
		GetTrack(InstID).SetEnable(Bank, Enable);
	}
	int GetEnable(int InstID, int Bank) { return GetTrack(InstID).GetEnable(Bank); }

	void SetInstrument(
		int InstrumentID, 
		const char* Name, 
		int Patch, 
		int Channel, 
		int Range, 
		int MidiOutDevID
	)
	{
		if (InstrumentID < 16 && InstrumentID > 0)
		{
			GetTrack(InstrumentID).SetMidiOutDeviceID(MidiOutDevID);
			GetTrack(InstrumentID).SetChanel(Channel);
			GetTrack(InstrumentID).SetPatch(Patch);
			GetTrack(InstrumentID).SetPitchRange(Range);
			GetTrack(InstrumentID).SetInstrumentName(Name);
		}
		else
			::MessageBoxW(NULL, _T("ERROR"), _T("Instrument ID out of Range"), MB_OK);
	}

	int GetPatch(int inst) { return GetTrack(inst).GetPatch(); }
	int* GetPatchPointer(int InstID) { return GetTrack(InstID).GetPatchPointer(); }
	void SetPatch(int InstID, int Patch) { GetTrack(InstID).SetPatch(Patch); }

	int GetChannel(int inst) { return GetTrack(inst).GetChanel(); }
	int* GetChannelPointer(int InstID){return GetTrack(InstID).GetChannelPointer();}
	void SetChannel(int InstID, int Chan) { GetTrack(InstID).SetChanel(Chan); }

	int GetRange(int inst){return GetTrack(inst).GetPitchRange();}
	int* GetRangePointer(int InstID) { return GetTrack(InstID).GetPitchRangePointer(); }
	void SetRange(int InstID, int Range) { GetTrack(InstID).SetPitchRange(Range); }

	COLORREF GetInstColor(int Inst){return GetTrack(Inst).GetColor();}
	void SetInstColor(int InstID, COLORREF Color) { GetTrack(InstID).SetColor(Color); }

	void SetMidiOutDeviceId(int InstID,int MidiOutID) { GetTrack(InstID).SetMidiOutDeviceID(MidiOutID); }
	int GetMidiOutDeviceId(int InstID) { return GetTrack(InstID).GetMidiOutDeviceID(); }

	COLORREF GetTextColor(int InstID) { return GetTrack(InstID).GetTextColor(); }
	void SetTextColor(int InstID, COLORREF cTc) { GetTrack(InstID).SetTextColor(cTc); }

	CMyBitmap* GetBitmap(int InstID) { return GetTrack(InstID).GetBitmap(); }

	void SetTitle(char* Title){
		strcpy_s(m_SongTitle, 32, Title);
	}
	char* GetTitle() { return m_SongTitle; }

	bool IsTrackEnable(int block, int track);
	void SetTrackEnable(int b, int v, int Inst);

	void Print(FILE *pO);
	void Save(FILE* pO);
	void UpdateTrackBitmap(int TrackID);
};

