#include "pch.h"

CMidiInfo::CMidiInfo()
{
    //ctor
	memset(m_SongTitle,0,sizeof(m_SongTitle));
	strcpy_s(m_SongTitle,32,"Song Title");
}

CMidiInfo::~CMidiInfo()
{
    //dtor
}

int Patch[16] = {
	-1,		// Not used
	23,		//1	Harmonica
	67,		//2 Sax
	27,		//3	Gutar
	72,		//4 Clarinet
	74,		//5 Flute
	90,		//6 Pads
	1,		//7	Piano
	17,		//8	Organ
	57,		//9	Trumpet
	51,		//10	Strings
	12,		//11	Vibes
	15,		//12	Bells
	47,		//13	Harp
	39,		//14	Bass
	1		//15	Percusion
};

int Channel[16] = {
	-1,		//0	
	9,		//1
	2,		//2
	3,		//3
	4,		//4
	5,		//5
	6,		//6
	7,		//7
	8,		//8
	10,		//9
	11,		//10
	12,		//11
	13,		//12
	14,		//13
	15,		//14
	1		//15
};

int Range[16] = {
	-1,		//0
	2,		//1
	3,		//2
	3,		//3
	3,		//4
	3,		//5
	3,		//6
	3,		//7
	3,		//8
	3,		//9
	3,		//10
	3,		//11
	3,		//12
	3,		//13
	3,		//14
	1		//15
};

void CMidiInfo::Create(CDC* pDC)
{
	for (int InstID = 1; InstID < 16; ++InstID)	//from instrument 1 to 15
	{
		const char* pName;
		CRect rect(0, 0, 24, 24);

		if (9 == Channel[InstID])
			pName = "Percussion";
		else
			pName = GenMidiPatchNames[Patch[InstID]];
		GetTrack(InstID).Create(
			pDC,
			InstID,
			rect,
			0,
			Range[InstID],
			Channel[InstID],
			Patch[InstID]
		);
		SetInstrumentName(InstID,pName);
	}
}

/*
void CMidiInfo::Create(char *MidiInfo)
{
	MIDI_INFO *pMI = (MIDI_INFO *)MidiInfo;

	int InstID = 1;
	for(InstID=1;InstID<16;++InstID)
	{
		GetTrack(InstID).SetChanel(pMI->chan_range[InstID] & 0x0f);
		if (GetTrack(InstID).GetChanel() == 0)
			GetTrack(InstID).SetChanel(15);
		else
			GetTrack(InstID).SetChanel(GetTrack(InstID).GetChanel() - 1);
		GetTrack(InstID).SetPitchRange((pMI->chan_range[InstID] & 0xf0) >> 4);
		GetTrack(InstID).SetPatch(pMI->patch[InstID]);
		if (GetTrack(InstID).GetChanel() == 9)//midi channel 10
			SetInstrumentName(InstID, "Percusion");
		else
			SetInstrumentName(InstID, pMI->Name[InstID].Name);
		int j;
		unsigned UINT mask = (1 << InstID);
		for (j = 0; j < 4; ++j)
			SetEnable(InstID, j, (pMI->TrackEnables[j] & mask) ? 1 : 0);
	}
	SetTitle(pMI->Title);
}
*/

bool CMidiInfo::IsTrackEnable(int block, int track)
{
	//---------------------------------------
	// IsTrackEnable
	//
	//	Determines if queried track is enabled.
	//
	// parameters:
	//	block.....block of enables to check
	//	track.....Instrument number, 1-15
	//
	// returns true if enabled
	//-------------------------------------------
    bool rV = false;
	if (block < 4 && track < 16 && track > 0)
	{
		rV = GetEnable(track, block) ? true : false;
	}
	else
		::MessageBox(NULL, _T("Bad Track Number"), _T("ERROR"), MB_OK);
    return rV;
}

void CMidiInfo::Print(FILE *pO)
{

}

void CMidiInfo::SetTrackEnable(int Bank,int v,int InstID)
{
	//-----------------------------------------
	// SetTrackEnable
	//	This function is used to set
	// or clear the track enable bit
	// for each instrument
	//
	// parameters:
	//		bank....track enable bank 0..3
	//		v.......value to set bit 0..1
	//		Inst....Instrument number 1..15
	//----------------------------------------
	//subtract 1 from Inst to get index
	GetTrack(InstID).SetEnable(Bank, v);
}

void CMidiInfo::Save(FILE *pO)
{
	int i,j;

	for(j=1;j<16;++j)
	{
		for(i=0;i<9;++i)
		{
			fputc(GetTrack(j).GetInstrumentName()[i],pO);
		}
		fputc(0,pO);	//Null terminator for string
	}
	for (j = 1; j < 16; ++j)
	{
		int chan = GetTrack(j).GetChanel();
		++chan;
		if (chan == 16) chan = 0;
		fputc(chan | (GetTrack(j).GetPitchRange() << 4), pO);
	}
	for(j=1;j<16;++j)
		fputc(GetTrack(j).GetPatch(),pO);
	for(i=0;i<20;++i) fputc(0,pO);	///pointers to lyrics
	for(i=0;i<32;++i) fputc(m_SongTitle[i],pO);
	for(i=0;i<8;++i) fputc(0xff,pO);	//track enables
}

void CMidiInfo::UpdateTrackBitmap(int TrackID)
{
	GetTrack(TrackID).UpdateTrackBitmap();
}

