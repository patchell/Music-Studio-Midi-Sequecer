#pragma once

constexpr auto DISPATCHEDIT_NUM_MIDI_CMDS = 23;

class CMidi
{
public:
	//----------------------------------
	// Midi channel Messages
	//----------------------------------
	enum class MidiChannelCmds : int {
		NOTEOFF = 0x80,
		NOTEON = 0x90,
		POLYPRESS = 0xa0,
		CTRLCHNG = 0xb0,
		PGMCHANGE = 0xc0,
		CHNLPRESS = 0xd0,
		PITCHBEND = 0xe0
	};
private:
	struct ChannelMsgItem {
		MidiChannelCmds m_Status;
		int m_ByteCount;
		const char* m_pName;
	};
	inline static ChannelMsgItem ChannelMsgLUT[] = {
		{ MidiChannelCmds::NOTEOFF, 3, "Note Off" },
		{ MidiChannelCmds::NOTEON, 3, "Note On" },
		{ MidiChannelCmds::POLYPRESS, 3, "Polyphonic Key Pressure" },
		{ MidiChannelCmds::CTRLCHNG, 3, "Control Change" },
		{ MidiChannelCmds::PGMCHANGE, 2, "Program Change" },
		{ MidiChannelCmds::CHNLPRESS, 2, "Channel Pressure" },
		{ MidiChannelCmds::PITCHBEND, 3, "Pitch Bend Change" }
	};
public:
	//-------------------------------------
	// Midi Real Time Messages
	//-------------------------------------

	enum class MidiRealTimeMsgs : int {
		CLOCK = 0xf8,
		RSVRD_F9,
		START,
		CONTINUE,
		STOP,
		RSVRD_FD,
		ACTIVE_SENSING,
		RESET
	};
private:
	struct RealTimeMsgItem {
		MidiRealTimeMsgs m_Status;
		const char* m_pName;
	};
	inline static RealTimeMsgItem RealTimeMsgLUT[] = {
		{ MidiRealTimeMsgs::CLOCK, "Clock" },
		{ MidiRealTimeMsgs::RSVRD_F9, "Reserved F9" },
		{ MidiRealTimeMsgs::START, "Start" },
		{ MidiRealTimeMsgs::CONTINUE, "Continue" },
		{ MidiRealTimeMsgs::STOP, "Stop" },
		{ MidiRealTimeMsgs::RSVRD_FD, "Reserved FD" },
		{ MidiRealTimeMsgs::ACTIVE_SENSING, "Active Sensing" },
		{ MidiRealTimeMsgs::RESET, "Reset" }
	};
public:
	//--------------------------------------
	// Midi System Messages
	// --------------------------------------
	enum class MidiSystemCmds : int {
		SYSEX = 0xf0,
		TIMECODE,
		SONG_POSITION,
		SONG_SELECT,
		RSVRD_F4,
		RSVRD_F5,
		TUNE_REQUEST,
		SYSEXEND
	};
private:
	struct SystemMsgItem {
		MidiSystemCmds m_Status;
		const char* m_pName;
	};
	inline static SystemMsgItem SystemMsgLUT[] = {
		{ MidiSystemCmds::SYSEX, "SysEx" },
		{ MidiSystemCmds::TIMECODE, "Time Code" },
		{ MidiSystemCmds::SONG_POSITION, "Song Position" },
		{ MidiSystemCmds::SONG_SELECT, "Song Select" },
		{ MidiSystemCmds::RSVRD_F4, "Reserved F4" },
		{ MidiSystemCmds::RSVRD_F5, "Reserved F5" },
		{ MidiSystemCmds::TUNE_REQUEST, "Tune Request" },
		{ MidiSystemCmds::SYSEXEND, "SysEx End" }
	};
public:
	//-------------------------------------
	// Submessages for MIDI_CTRLCHNG
	//-------------------------------------
	enum class MidiCC : int {
		BANK_SELECT_MSB = 0,
		MODWHEEL_MSB = 1,
		BREATHCTRL_MSB = 2,
		FOOTCTRL_MSB = 4,
		PORTAMENTO_TIME_MSB = 5,
		DATA_ENTRY_MSB = 6,
		VOLUME_MSB = 7,
		PAN_MSB = 10,
		EXPRESSION_MSB = 11,
		EFFECT1_MSB = 12,
		EFFECT2_MSB = 13,
		GENERAL_PURPOSE1_MSB = 16,
		GENERAL_PURPOSE2_MSB = 17,
		GENERAL_PURPOSE3_MSB = 18,
		GENERAL_PURPOSE4_MSB = 19,

		BANK_SELECT_LSB = 32,
		MODWHEEL_LSB = 33,
		BREATHCTRL_LSB = 34,
		FOOTCTRL_LSB = 36,
		PORTAMENTO_TIME_LSB = 37,
		DATA_ENTRY_LSB = 38,
		VOLUME_LSB = 39,
		PAN_LSB = 42,
		EXPRESSION_LSB = 43,
		EFFECT1_LSB = 44,
		EFFECT2_LSB = 45,
		GENERAL_PURPOSE1_LSB = 48,
		GENERAL_PURPOSE2_LSB = 49,
		GENERAL_PURPOSE3_LSB = 50,
		GENERAL_PURPOSE4_LSB = 51,

		DAMPER_PEDAL = 64,
		PORTAMENTO_ONOFF = 65,
		SOSTENUTO_PEDAL = 66,
		SOFT_PEDAL = 67,
		LEGATO_FOOTSWITCH = 68,
		HOLD2 = 69,
		SOUND_CONTROLLER1 = 70,
		SOUND_CONTROLLER2 = 71,
		SOUND_CONTROLLER3 = 72,
		SOUND_CONTROLLER4 = 73,
		SOUND_CONTROLLER5 = 74,
		SOUND_CONTROLLER6 = 75,
		SOUND_CONTROLLER7 = 76,
		SOUND_CONTROLLER8 = 77,
		SOUND_CONTROLLER9 = 78,
		SOUND_CONTROLLER10 = 79,
		GENERAL_PURPOSE5 = 80,
		GENERAL_PURPOSE6 = 81,
		GENERAL_PURPOSE7 = 82,
		GENERAL_PURPOSE8 = 83,
		PORTAMENTO_CONTROL = 84,
		EFFECTS1_DEPTH = 91,
		EFFECTS2_DEPTH = 92,
		EFFECTS3_DEPTH = 93,
		EFFECTS4_DEPTH = 94,
		EFFECTS5_DEPTH = 95,
		DATA_INCREMENT = 96,
		DATA_DECREMENT = 97,
		NON_REGISTERED_PARAM_MSB = 98,
		NON_REGISTERED_PARAM_LSB = 99,
		REGISTERED_PARAM_MSB = 100,
		REGISTERED_PARAM_LSB = 101,
		ALL_SOUND_OFF = 120,
		RESET_ALL_CONTROLLERS = 121,
		LOCAL_CONTROL = 122,
		ALL_NOTES_OFF = 123,
		OMNI_MODE_ON = 124,
		OMNI_MODE_OFF = 125,
		MONO_MODE_ON = 126,
		POLY_MODE_ON = 127
	};
private:
	struct MidiCCItem {
		MidiCC m_Ctrl;
		const char* m_pName;
	};
	inline static MidiCCItem MidiCC_LUT[] = {
		{ MidiCC::BANK_SELECT_MSB, "Bank Select MSB" },
		{ MidiCC::MODWHEEL_MSB, "Mod Wheel MSB" },
		{ MidiCC::BREATHCTRL_MSB, "Breath Control MSB" },
		{ MidiCC::FOOTCTRL_MSB, "Foot Control MSB" },
		{ MidiCC::PORTAMENTO_TIME_MSB, "Portamento Time MSB" },
		{ MidiCC::DATA_ENTRY_MSB, "Data Entry MSB" },
		{ MidiCC::VOLUME_MSB, "Volume MSB" },
		{ MidiCC::PAN_MSB, "Pan MSB" },
		{ MidiCC::EXPRESSION_MSB, "Expression MSB" },
		{ MidiCC::EFFECT1_MSB, "Effect 1 Depth MSB" },
		{ MidiCC::EFFECT2_MSB, "Effect 2 Depth MSB" },
		{ MidiCC::GENERAL_PURPOSE1_MSB, "General Purpose Controller 1 MSB" },
		{ MidiCC::GENERAL_PURPOSE2_MSB, "General Purpose Controller 2 MSB" },
		{ MidiCC::GENERAL_PURPOSE3_MSB, "General Purpose Controller 3 MSB" },
		{ MidiCC::GENERAL_PURPOSE4_MSB, "General Purpose Controller 4 MSB" },
		{ MidiCC::BANK_SELECT_LSB, "Bank Select LSB" },
		{ MidiCC::MODWHEEL_LSB, "Mod Wheel LSB" },
		{ MidiCC::BREATHCTRL_LSB, "Breath Control LSB" },
		{ MidiCC::FOOTCTRL_LSB, "Foot Control LSB" },
		{ MidiCC::PORTAMENTO_TIME_LSB, "Portamento Time LSB" },
		{ MidiCC::DATA_ENTRY_LSB, "Data Entry LSB" },
		{ MidiCC::VOLUME_LSB, "Volume LSB" },
		{ MidiCC::PAN_LSB, "Pan LSB" },
		{ MidiCC::EXPRESSION_LSB, "Expression LSB" },
		{ MidiCC::EFFECT1_LSB, "Effect 1 Depth LSB" },
		{ MidiCC::EFFECT2_LSB, "Effect 2 Depth LSB" },
		{ MidiCC::GENERAL_PURPOSE1_LSB, "General Purpose Controller 1 LSB" },
		{ MidiCC::GENERAL_PURPOSE2_LSB, "General Purpose Controller 2 LSB" },
		{ MidiCC::GENERAL_PURPOSE3_LSB, "General Purpose Controller 3 LSB" },
		{ MidiCC::GENERAL_PURPOSE4_LSB, "General Purpose Controller 4 LSB" },
		{ MidiCC::DAMPER_PEDAL, "Damper Pedal (Sustain)" },
		{ MidiCC::PORTAMENTO_ONOFF, "Portamento On/Off" },
		{ MidiCC::SOSTENUTO_PEDAL, "Sostenuto Pedal" },
		{ MidiCC::SOFT_PEDAL, "Soft Pedal" },
		{ MidiCC::LEGATO_FOOTSWITCH, "Legato Footswitch" },
		{ MidiCC::HOLD2, "Hold 2" },
		{ MidiCC::SOUND_CONTROLLER1, "Sound Controller 1" },
		{ MidiCC::SOUND_CONTROLLER2, "Sound Controller 2" },
		{ MidiCC::SOUND_CONTROLLER3, "Sound Controller 3" },
		{ MidiCC::SOUND_CONTROLLER4, "Sound Controller 4" },
		{ MidiCC::SOUND_CONTROLLER5, "Sound Controller 5" },
		{ MidiCC::SOUND_CONTROLLER6, "Sound Controller 6" },
		{ MidiCC::SOUND_CONTROLLER7, "Sound Controller 7" },
		{ MidiCC::SOUND_CONTROLLER8, "Sound Controller 8" },
		{ MidiCC::SOUND_CONTROLLER9, "Sound Controller 9" },
		{ MidiCC::SOUND_CONTROLLER10, "Sound Controller 10" },
		{ MidiCC::GENERAL_PURPOSE5, "General Purpose Controller 5" },
		{ MidiCC::GENERAL_PURPOSE6, "General Purpose Controller 6" },
		{ MidiCC::GENERAL_PURPOSE7, "General Purpose Controller 7" },
		{ MidiCC::GENERAL_PURPOSE8, "General Purpose Controller 8" },
		{ MidiCC::PORTAMENTO_CONTROL, "Portamento Control" },
		{ MidiCC::EFFECTS1_DEPTH, "Effects Depth 1 (Reverb Send)" },
		{ MidiCC::EFFECTS2_DEPTH, "Effects Depth 2 (Chorus Send)" },
		{ MidiCC::EFFECTS3_DEPTH, "Effects Depth 3 (Celeste Detune)" },
		{ MidiCC::EFFECTS4_DEPTH, "Effects Depth 4 (Phaser Depth)" },
		{ MidiCC::EFFECTS5_DEPTH, "Effects Depth 5 (Data 5)" },
		{ MidiCC::DATA_INCREMENT, "Data Increment" },
		{ MidiCC::DATA_DECREMENT, "Data Decrement" },
		{ MidiCC::NON_REGISTERED_PARAM_MSB, "Non-Registered Parameter Number MSB" },
		{ MidiCC::NON_REGISTERED_PARAM_LSB, "Non-Registered Parameter Number LSB" },
		{ MidiCC::REGISTERED_PARAM_MSB, "Registered Parameter Number MSB" },
		{ MidiCC::REGISTERED_PARAM_LSB, "Registered Parameter Number LSB" },
		{ MidiCC::ALL_SOUND_OFF, "All Sound Off" },
		{ MidiCC::RESET_ALL_CONTROLLERS, "Reset All Controllers" },
		{ MidiCC::LOCAL_CONTROL, "Local Control On/Off" },
		{ MidiCC::ALL_NOTES_OFF, "All Notes Off" },
		{ MidiCC::OMNI_MODE_ON, "Omni Mode On" },
		{ MidiCC::OMNI_MODE_OFF, "Omni Mode Off" },
		{ MidiCC::MONO_MODE_ON, "Mono Mode On" },
		{ MidiCC::POLY_MODE_ON, "Poly Mode On" }
	};
	//---------------------------------------
	// Bitmap Tables for music editor
	//---------------------------------------

	//------------------------------
	// Midi In Dispatcher Midi
	// Status Byte Look Up Table
	//------------------------------
public:
	struct MIDI_STATUS {
		CString m_csName;
		unsigned m_MidiStatus;
		bool m_Data1_flag;
		bool m_Data2_flag;
		bool m_Channel_flag;
		CString m_csData1;
		CString m_csData2;
		MIDI_STATUS() {
			m_MidiStatus = 0;
			m_Data1_flag = false;
			m_Data2_flag = false;
			m_Channel_flag = false;
		}
		MIDI_STATUS(CString name, unsigned status, bool data1flag, bool data2flag, bool channelflag, CString data1name, CString data2name) {
			m_csName = name;
			m_MidiStatus = status;
			m_Data1_flag = data1flag;
			m_Data2_flag = data2flag;
			m_Channel_flag = channelflag;
			m_csData1 = data1name;
			m_csData2 = data2name;
		}
	};
private:
	inline static MIDI_STATUS MidiStatusCommands[DISPATCHEDIT_NUM_MIDI_CMDS] = {
		//------------ Channel Status ---------------------
		{CString("Note Off"), (unsigned)MidiChannelCmds::NOTEOFF,1,1,1, CString("Note"), CString("Velocity")},
		{CString("Note On"), (unsigned)CMidi::MidiChannelCmds::NOTEON,1,1,1, CString("Note"), CString("Velocity")},
		{CString("Poly AftrTch"), (unsigned)CMidi::MidiChannelCmds::POLYPRESS,1,1,1, CString("Note"), CString("Pres")},
		{CString("Control/Mode"),(unsigned)CMidi::MidiChannelCmds::CTRLCHNG,1,1,1, CString("Type"), CString("Value")},
		{CString("Prog Cnance"),(unsigned)CMidi::MidiChannelCmds::PGMCHANGE,1,0,1, CString("Prgm"), CString("-----")},
		{CString("After Touch"), (unsigned)CMidi::MidiChannelCmds::CHNLPRESS,1,0,1, CString("Pres"), CString("----")},
		{CString("Pitch Bend"),(unsigned)CMidi::MidiChannelCmds::PITCHBEND,1,1,1, CString("LSB"), CString("MSB")},
		//----- System Status ----------------------
		{CString("TimeCode"),(unsigned)CMidi::MidiSystemCmds::TIMECODE, 0, 0, 0, CString("----"), CString("----")},
		{CString("Song Pos"),(unsigned)CMidi::MidiSystemCmds::SONG_POSITION, 1, 1, 0, CString("LSB"), CString("MSB")},
		{CString("Song Sel"),(unsigned)CMidi::MidiSystemCmds::SONG_SELECT, 1, 0, 0, CString("Song #"), CString("----")},
		{CString("Reserved"),(unsigned)CMidi::MidiSystemCmds::RSVRD_F4, 0, 0, 0, CString("----"), CString("----")},
		{CString("Reserved"),(unsigned)CMidi::MidiSystemCmds::RSVRD_F5, 0, 0, 0, CString("----"), CString("----")},
		{CString("Tune Request"),(unsigned)CMidi::MidiSystemCmds::TUNE_REQUEST, 0, 0, 0, CString("----"), CString("----")},
		{CString("Timing Clock"),(unsigned)CMidi::MidiRealTimeMsgs::CLOCK, 0, 0, 0, CString("----"), CString("----")},
		{CString("Reserved"),(unsigned)CMidi::MidiRealTimeMsgs::RSVRD_F9, 0, 0, 0, CString("----"), CString("----")},
		{CString("Start"),(unsigned)CMidi::MidiRealTimeMsgs::START, 0, 0, 0, CString("----"), CString("----")},
		{CString("Continue"),(unsigned)CMidi::MidiRealTimeMsgs::CONTINUE, 0, 0, 0, CString("----"), CString("----")},
		{CString("Stop"),(unsigned)CMidi::MidiRealTimeMsgs::STOP, 0, 0, 0, CString("----"), CString("----")},
		{CString("Reserved"),(unsigned)CMidi::MidiRealTimeMsgs::RSVRD_FD, 0, 0, 0, CString("----"), CString("----")},
		{CString("Active Sensing"),(unsigned)CMidi::MidiRealTimeMsgs::ACTIVE_SENSING, 0, 0, 0, CString("----"), CString("----")},
		{CString("Reset"),(unsigned)CMidi::MidiRealTimeMsgs::RESET, 0, 0, 0, CString("----"), CString("----")},
		{CString("SYSEX"),(unsigned)CMidi::MidiSystemCmds::SYSEX, 0, 0, 0, CString("----"), CString("----")},
		{CString("SYSEX END"),(unsigned)CMidi::MidiSystemCmds::SYSEXEND, 0, 0, 0, CString("----"), CString("----")}
	};
public:
	//-----------------------------------------------------
	// Midi In Dispatcher
	//-------------------------------------------------
	enum class MidiStatusDispatchIDs : int {
		DISPATCH_STATUS_ID_MIDI_NOTEOFF,
		DISPATCH_STATUS_ID_MIDI_NOTEON,
		DISPATCH_STATUS_ID_MIDI_POLYPRESS,
		DISPATCH_STATUS_ID_MIDI_CTRLCHNG,
		DISPATCH_STATUS_ID_MIDI_PGMCHANGE,
		DISPATCH_STATUS_ID_MIDI_CHNLPRESS,
		DISPATCH_STATUS_ID_MIDI_PITCHBEND,
		DISPATCH_STATUS_ID_MIDI_TIMECODE,
		DISPATCH_STATUS_ID_MIDI_SONGPOSITION,
		DISPATCH_STATUS_ID_MIDI_SONGSELECT,
		DISPATCH_STATUS_ID_MIDI_RESERVED_F4,
		DISPATCH_STATUS_ID_MIDI_RESERVED_F5,
		DISPATCH_STATUS_ID_MIDI_TUNEREQUEST,
		DISPATCH_STATUS_ID_MIDI_CLOCK,
		DISPATCH_STATUS_ID_MIDI_RTM_RSVRD_F9,
		DISPATCH_STATUS_ID_MIDI_START,
		DISPATCH_STATUS_ID_MIDI_CONTINUE,
		DISPATCH_STATUS_ID_MIDI_STOP,
		DISPATCH_STATUS_ID_MIDI_RTM_RSVRD_FD,
		DISPATCH_STATUS_ID_MIDI_ACTIVE_SENSING,
		DISPATCH_STATUS_ID_MIDI_RESET,
		DISPATCH_STATUS_ID_MIDI_SYSEX,
		DISPATCH_STATUS_ID_MIDI_SYSEXEND,
		DISPATCH_STATUS_ID_MIDI_ERROR
	};
private:
	struct MidiStatusDispatchItem {
		MidiStatusDispatchIDs m_StatusID;
		const char* m_pName;
	};
	inline static MidiStatusDispatchItem MidiStatusDispatchLUT[] = {
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_NOTEOFF, "Note Off" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_NOTEON, "Note On" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_POLYPRESS, "Polyphonic Key Pressure" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CTRLCHNG, "Control Change" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_PGMCHANGE, "Program Change" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CHNLPRESS, "Channel Pressure" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_PITCHBEND, "Pitch Bend Change" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_TIMECODE, "Time Code" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SONGPOSITION, "Song Position" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SONGSELECT, "Song Select" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RESERVED_F4, "Reserved F4" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RESERVED_F5, "Reserved F5" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_TUNEREQUEST, "Tune Request" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CLOCK, "Clock" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RTM_RSVRD_F9, "Reserved F9" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_START, "Start" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CONTINUE, "Continue" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_STOP, "Stop" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RTM_RSVRD_FD, "Reserved FD" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_ACTIVE_SENSING, "Active Sensing" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RESET, "Reset" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SYSEX, "SysEx" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SYSEXEND, "SysEx End" },
		{ MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_ERROR, "Error" }
	};
public:
	enum class GeneralMidiPatchID {
		AcousticGrandPiano,
		BrightAcousticPiano,
		ElectricGrandPiano,
		HonkyTonkPiano,
		ElectricPiano1,
		ElectricPiano2,
		Harpsicord,
		Clavicord,
		Celesta,
		Glockenspiel,
		MusicBox,
		Vibraphone,
		Marimba,
		Xylophone,
		TubularBells,
		Dulcimer,
		DrawbarOrgan,
		PercusiveOrgan,
		RockOrgan,
		ChurchOrgan,
		ReedOrgan,
		Accordian,
		Harmonica,
		TangoAccordian,
		NylonAcousticGuitar,
		SteelAcousticGuitar,
		JazzElectricGuitar,
		CleanElectricGuitar,
		MutedElectricGuitar,
		OverdrivenGuitar,
		DistortionGuitar,
		GuitarHarmonics,
		AcousticBass,
		ElectricBassFingered,
		ElectricBassPicked,
		FretlessBass,
		SlapBass1,
		SlapBass2,
		SynthBass1,
		SynthBass2,
		Violin,
		Viola,
		Cello,
		Contrabass,
		TremoloStrings,
		PizzicatoStrings,
		OrchestralHarp,
		Timpani,
		StringEnsemble1,
		StringEnsemble2,
		SynthStrings1,
		SynthStrings2,
		ChoirAahs,
		VoiceOohs,
		SynthVoice,
		OrchestraHit,
		Trumpet,
		Trombone,
		Tuba,
		MutedTrumpet,
		FrenchHorn,
		BrassSection,
		SynthBrass1,
		SynthBrass2,
		SopranoSax,
		AltoSax,
		TenorSax,
		BaratoneSax,
		Oboe,
		EnglishHorn,
		Bassoon,
		Clarinet,
		Picolo,
		Flute,
		Recorder,
		PanFlute,
		BlownBottle,
		Shakulachi,
		Wistle,
		Ocarina,
		SquareLead,
		SawtoothLead,
		CaliopeLead,
		ChiffLead,
		CharangLead,
		VoiceLead,
		FifthsLead,
		BassPlusLead,
		NewAgePad,
		WarmPad,
		PolysynthPad,
		ChoirPad,
		BowedPad,
		MetalicPad,
		HaloPad,
		SweepPad,
		FX1Rain,
		FX2Soundtrack,
		FX3Crystal,
		FX4Atmosphere,
		FX5Brightness,
		FX6Goblins,
		FX7Echoes,
		FX8SciFi,
		Sitar,
		Banjo,
		Shanisen,
		Koto,
		Kalinba,
		BagPipe,
		Fiddle,
		Shania,
		TinkleBell,
		Agogo,
		SteelDrums,
		WoodBlocks,
		TaikoDrum,
		MelodicTom,
		SynthDrum,
		ReverseCymbal,
		GuitarFretNoise,
		BreathNoise,
		SeaShore,
		BirdTweets,
		TelephoneRing,
		Heliocopter,
		Applause,
		Gunshot
	};
	inline static const char* CZ101PatchNames[16] = {
		//-------------------------------
		// CZ-101 Patch Names
		//-------------------------------
		"Brass Ensemble",	//0
		"Strings",			//1
		"Pipe Organ",		//2
		"Electric Piano",	//3
		"Harpsichord",		//4
		"Synth Bass",		//5
		"Cosmic Sound",		//6
		"Metal Synth",		//7
		"Fantasy",			//8
		"Funky Pop",		//9
		"Trumpet",			//10
		"Saxophone",		//11
		"Violin",			//12
		"Flute",			//13
		"Glocken",			//14
		"Drum Set"			//15
	};
	//-------------------------------
	// General Midi Patch Names
	//-------------------------------
	inline static const char* GenMidiPatchNames[128] = {
			"Acoustic Grand Piano",		//0
			"Bright Acoustic Piano",	//1
			"Electric Grand Piano",		//2
			"Honky-Tonk Piano",			//3
			"Electric Piano 1",			//4
			"Electric Piano 2",			//5
			"Harpsicord",				//6
			"Clavicord",				//7
			"Celesta",					//8
			"Glockenspiel",				//9
			"Music Box",				//10
			"Vibraphone",				//11
			"Marimba",					//12
			"Xylophone",				//13
			"Tubular Bells",			//14
			"Dulcimer",					//15
			"Drawbar Organ",			//16
			"Percusive Organ",			//17
			"Rock Organ",				//18
			"Church Organ",				//19
			"Reed Organ",				//20
			"Accordian",				//21
			"Harmonica",				//22
			"Tango Accordian",			//23
			"Nylon Acoustic Guitar",	//24
			"Steel Acoustic Guitar",	//25
			"Jazz Electric Guitar",		//26
			"Clean Electric Guitar",	//27
			"Muted Electric Guitar",	//28
			"Overdriven Guitar",		//29
			"Distortion Guitar",		//30
			"Guitar Harmonics",			//31
			"Acoustic Bass",			//32
			"Electric Bass Fingered",	//33
			"Electric Bass Picked",		//34
			"Fretless Bass",			//35
			"Slap Bass 1",				//36
			"Slap Bass 2",				//37
			"Synth Bass 1",				//38
			"Synth Bass 2",				//39
			"Violin",					//40
			"Viola",					//41
			"Cello",					//42
			"Contrabass",				//43
			"Tremolo Strings",			//44
			"Pizzicato Strings",		//45
			"Orchestral Harp",			//46
			"Timpani",					//47
			"String Ensemble 1",		//48
			"String Ensemble 2",		//49
			"Synth Strings 1",			//50
			"Synth Strings 2",			//51
			"Choir Aahs",				//52
			"Voice Oohs",				//53
			"Synth Voice",				//54
			"Orchestra Hit",			//55
			"Trumpet",					//56
			"Trombone",					//57
			"Tuba",						//58
			"Muted Trumpet",			//59
			"French Horn",				//60
			"Brass Section",			//61
			"Synth Brass 1",			//62
			"Synth Brass 2",			//63
			"Soprano Sax",				//64
			"Alto Sax",					//65
			"Tenor Sax",				//66
			"Baratone Sax",				//67
			"Oboe",						//68
			"English Horn",				//69
			"Bassoon",					//70
			"Clarinet",					//71
			"Picolo",					//72
			"Flute",					//73
			"Recorder",					//74
			"Pan Flute",				//75
			"Blown Bottle",				//76
			"Shakulachi",				//77
			"Wistle",					//78
			"Ocarina",					//79
			"Square Lead",				//80
			"Sawtooth Lead",			//81
			"Caliope Lead",				//82
			"Chiff Lead",				//83
			"Charang Lead",				//84
			"Voice Lead",				//85
			"Fifths Lead",				//86
			"Bass + Lead",				//87
			"New Age Pad",				//88
			"Warm Pad",					//89
			"Polysynth Pad",			//90
			"Choir Pad",				//91
			"Bowed Pad",				//92
			"Metalic Pad",				//93
			"Halo Pad",					//94
			"Sweep Pad",				//95
			"FX 1 (rain)",				//96
			"FX 2 (soundtrack)",		//97
			"FX 3 (Crystal)",			//98
			"FX 4 (atmosphere)",		//99
			"FX 5 (brightness)",		//100
			"FX 6 (goblins)",			//101
			"FX 7 (echoes)",			//102
			"FX 8 (sci-fi)",			//103
			"Sitar",					//104
			"Banjo",					//105
			"Shanisen",					//106
			"Koto",						//107
			"Kalinba",					//108
			"Bag Pipe",					//109
			"Fiddle",					//110
			"Shania",					//111
			"Tinkle Bell",				//112
			"Agogo",					//113
			"Steel Drums",				//114
			"Wood Blocks",				//115
			"Taiko Drum",				//116
			"Melodic Tom",				//117
			"Synth Drum",				//118
			"Reverse Cymbal",			//119
			"Guitar Fret Noise",		//120
			"Breath Noise",				//121
			"SeaShore",					//122
			"Bird Tweets",				//123
			"Telephone Ring",			//124
			"Heliocopter",				//125
			"Applause",					//126
			"Gunshot"					//127
	};
public:
	enum class GeneralMidiPercussionKey : int {
		AcousticBassDrum = 35,
		BassDrum1 = 36,
		SideStick = 37,
		AcousticSnare = 38,
		HandClap = 39,
		ElectricSnare = 40,
		LowFloorTom = 41,
		ClosedHiHat = 42,
		HighFloorTom = 43,
		PedaledHiHat = 44,
		LowTom = 45,
		OpenHiHat = 46,
		LowMidTom = 47,
		Tom1 = 48,
		HighMidTom = 49,
		Tom2 = 50,
		SplashCymbal = 51,
		Cowbell = 52,
		CloseCymbal1 = 53,
		Vibraslap = 54,
		CloseCymbal2 = 55,
		SustainCymbal = 56,
		CounterhoopCymbal = 57,
		OpenCymbal1 = 58,
		OpenCymbal2 = 59,
		HiBongo = 60,
		LowBongo = 61,
		MuteHiConga = 62,
		OpenHiConga = 63,
		LowConga = 64,
		HighTimbale = 65,
		LowTimbale = 66,
		HighAgogo = 67,
		LowAgogo = 68,
		Cabasa = 69,
		Maracas = 70,
		ShortWhistle = 71,
		LongWhistle = 72,
		ShortGuiro = 73,
		LongGuiro = 74,
		Claves = 75,
		HiWoodBlock = 76,
		LowWoodBlock = 77,
		MuteCuica = 78,
		OpenCuica = 79,
		LowCuica = 80,
		HighTriangle = 81,
		LowTriangle = 82
	};
	struct GeneralMidiPercussionItem {
		GeneralMidiPercussionKey m_Key;
		const char* m_pName;
	};
	inline static GeneralMidiPercussionItem GenMidiPercussionLUT[] = {
		{ GeneralMidiPercussionKey::AcousticBassDrum, "Acoustic Bass Drum" },
		{ GeneralMidiPercussionKey::BassDrum1, "Bass Drum 1" },
		{ GeneralMidiPercussionKey::SideStick, "Side Stick" },
		{ GeneralMidiPercussionKey::AcousticSnare, "Acoustic Snare" },
		{ GeneralMidiPercussionKey::HandClap, "Hand Clap" },
		{ GeneralMidiPercussionKey::ElectricSnare, "Electric Snare" },
		{ GeneralMidiPercussionKey::LowFloorTom, "Low Floor Tom" },
		{ GeneralMidiPercussionKey::ClosedHiHat, "Closed Hi-Hat" },
		{ GeneralMidiPercussionKey::HighFloorTom, "High Floor Tom" },
		{ GeneralMidiPercussionKey::PedaledHiHat, "Pedaled Hi-Hat" },
		{ GeneralMidiPercussionKey::LowTom, "Low Tom" },
		{ GeneralMidiPercussionKey::OpenHiHat, "Open Hi-Hat" },
		{ GeneralMidiPercussionKey::LowMidTom, "Low-Mid Tom" },
		{ GeneralMidiPercussionKey::Tom1, "Tom 1" },
		{ GeneralMidiPercussionKey::HighMidTom, "High-Mid Tom" },
		{ GeneralMidiPercussionKey::Tom2, "Tom 2" },
		{ GeneralMidiPercussionKey::SplashCymbal, "Splash Cymbal" },
		{ GeneralMidiPercussionKey::Cowbell, "Cowbell" },
		{ GeneralMidiPercussionKey::CloseCymbal1, "Closed Cymbal 1" },
		{ GeneralMidiPercussionKey::Vibraslap, "Vibraslap" },
		{ GeneralMidiPercussionKey::CloseCymbal2, "Closed Cymbal 2" },
		{ GeneralMidiPercussionKey::SustainCymbal, "Sustain Cymbal" },
		{ GeneralMidiPercussionKey::CounterhoopCymbal, "Counterhoop Cymbal" },
		{ GeneralMidiPercussionKey::OpenCymbal1, "Open Cymbal 1" },
		{ GeneralMidiPercussionKey::OpenCymbal2, "Open Cymbal 2" },
		{ GeneralMidiPercussionKey::HiBongo, "Hi Bongo" },
		{ GeneralMidiPercussionKey::LowBongo, "Low Bongo" },
		{ GeneralMidiPercussionKey::MuteHiConga, "Mute Hi Conga" },
		{ GeneralMidiPercussionKey::OpenHiConga, "Open Hi Conga" },
		{ GeneralMidiPercussionKey::LowConga, "Low Conga" },
		{ GeneralMidiPercussionKey::HighTimbale, "High Timbale" },
		{ GeneralMidiPercussionKey::LowTimbale, "Low Timbale" },
		{ GeneralMidiPercussionKey::HighAgogo, "High Agogo" },
		{ GeneralMidiPercussionKey::LowAgogo, "Low Agogo" },
		{ GeneralMidiPercussionKey::Cabasa, "Cabasa" },
		{ GeneralMidiPercussionKey::Maracas, "Maracas" },
		{ GeneralMidiPercussionKey::ShortWhistle, "Short Whistle" },
		{ GeneralMidiPercussionKey::LongWhistle, "Long Whistle" },
		{ GeneralMidiPercussionKey::ShortGuiro, "Short Guiro" },
		{ GeneralMidiPercussionKey::LongGuiro, "Long Guiro" },
		{ GeneralMidiPercussionKey::Claves, "Claves" },
		{ GeneralMidiPercussionKey::HiWoodBlock, "Hi Wood Block" },
		{ GeneralMidiPercussionKey::LowWoodBlock, "Low Wood Block" },
		{ GeneralMidiPercussionKey::MuteCuica, "Mute Cuica" },
		{ GeneralMidiPercussionKey::OpenCuica, "Open Cuica" },
		{ GeneralMidiPercussionKey::LowCuica, "Low Cuica" },
		{ GeneralMidiPercussionKey::HighTriangle, "High Triangle" },
		{ GeneralMidiPercussionKey::LowTriangle, "Low Triangle" }
	};
public:	//public functions
	static CMidi::MIDI_STATUS* GetMidiStatusCommands() {
		return CMidi::MidiStatusCommands;
	}
	static bool IsChannelStatus(CMidi::MidiStatusDispatchIDs StatusID);
	static MidiStatusDispatchIDs MidiStatusToMidiID(int status);

	static CMidi::GeneralMidiPatchID CZ101PatchToGenMidiPatch(int cz101Patch)
	{
		//---------------------------------------
		// Maps CZ-101 patch numbers to
		// General Midi Patch numbers
		//---------------------------------------
		static const GeneralMidiPatchID CZ101ToGenMidiLut[16] = {
			GeneralMidiPatchID::BrassSection,	//Brass Ensemble -> Brass Section
			GeneralMidiPatchID::StringEnsemble1, //Strings -> String Ensemble 1
			GeneralMidiPatchID::ChurchOrgan, //Pipe Organ -> Church Organ
			GeneralMidiPatchID::ElectricPiano1,  //Electric Piano -> Electric Piano 1
			GeneralMidiPatchID::Clavicord,  //Harpsichord -> Clavicord
			GeneralMidiPatchID::SynthBass1, //Synth Bass -> Synth Bass 1
			GeneralMidiPatchID::WarmPad, //Cosmic Sound -> Warm Pad
			GeneralMidiPatchID::SawtoothLead, //Metal Synth -> Sawtooth Lead
			GeneralMidiPatchID::NewAgePad, //Fantasy -> New Age Pad
			GeneralMidiPatchID::CleanElectricGuitar, //Funky Pop -> Clean Electric Guitar
			GeneralMidiPatchID::Trumpet, //Trumpet -> Trumpet
			GeneralMidiPatchID::AltoSax, //Saxophone -> Alto Sax
			GeneralMidiPatchID::Violin, //Violin -> Violin
			GeneralMidiPatchID::Flute, //Flute -> Flute
			GeneralMidiPatchID::Glockenspiel,  //Glocken -> Glockenspiel
			GeneralMidiPatchID::SynthDrum //Drum Set -> Synth Drum
		};
		if (cz101Patch < 0 || cz101Patch > 15)
			return GeneralMidiPatchID::BrassSection; //default to Acoustic Grand Piano
		return CZ101ToGenMidiLut[cz101Patch];
	}
	static const char* GeneralMidiPercusionNote(int note) {
		for (const auto& item : GenMidiPercussionLUT) {
			if ((int)item.m_Key == note)
				return item.m_pName;
		}
		return "Unknown Percussion Note";
	}
};

