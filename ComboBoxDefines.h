#pragma once




//------------------------------------
// General CComboDropDownToggle defines
//------------------------------------

// Item flags
constexpr auto  COMBOTOGGLE_TOGGLE_ENABLE		= 0X01;
constexpr auto  COMBOTOGGLE_TOGGLE_EXCLUSIVE	= 0X02;
constexpr auto  COMBOTOGGLE_TOGGLE_CHECKED		= 0X04;

union CBToggle {
	struct {
		unsigned value : 8;
		unsigned command : 8;
		unsigned index : 8;
	};
	WPARAM Wparam;
};

// command valuesb ???

enum {
	CBT_SETVALUE,
	CBT_INDEX_ONLY
};

//-------------------------------------
//	ComboBox Indexes for Block Operations
//-------------------------------------

constexpr auto COMBO_BLOCK_COPY = 0;
constexpr auto COMBO_BLOCK_MOVE = 1;
constexpr auto COMBO_BLOCK_CHANGEINSTRUMENT = 2;
constexpr auto COMBO_BLOCK_CHANGEDUR = 3;
constexpr auto COMBO_BLOCK_INCREASE = 4;
constexpr auto COMBO_BLOCK_DECREASE = 5;
constexpr auto COMBO_BLOCKUP = 6;
constexpr auto COMBO_BLOCKDOWN = 7;
constexpr auto COMBO_BLOCK_REPEAT = 8;
constexpr auto COMBO_BLOCK_INSERT = 9;
constexpr auto COMBO_BLOCK_MIDIINPUT = 10;

//------------------------------------
//	ComboBox Indexes for Misc Stuff
//------------------------------------

constexpr auto 	COMBO_MISC_MEASUREBAR = 0;
constexpr auto 	COMBO_MISC_NOTETIE = 1;
constexpr auto 	COMBO_MISC_LOUDNESS = 2;
constexpr auto 	COMBO_MISC_TEMPO = 3;
constexpr auto 	COMBO_MISC_INST_CHANGE = 4;
constexpr auto 	COMBO_MISC_ENDBAR = 5;
constexpr auto  COMBO_MISC_GLISSANDO = 6;



//-------------------------------------
// ComboBox Indexes for Accidentals
//-------------------------------------

constexpr auto COMBO_ACCIDENTAL_INKEY = 0;
constexpr auto COMBO_ACCIDENTAL_SHARP = 1;
constexpr auto COMBO_ACCIDENTAL_FLAT = 2;
constexpr auto COMBO_ACCIDENTAL_NATURAL = 3;

//-------------------------------------
// ComboBox Indexes for Rest Types
//-------------------------------------

constexpr auto COMBO_REST_WHOLE = 0;
constexpr auto COMBO_REST_HALF = 1;
constexpr auto COMBO_REST_QUARTER = 2;
constexpr auto COMBO_REST_EIGHTH = 3;
constexpr auto COMBO_REST_SIXTEENTH = 4;
constexpr auto COMBO_REST_THIRTYSEC = 5;

//-------------------------------------
//	ComboBox Indicies for Time Signiture
//-------------------------------------

constexpr auto COMBO_TIMESIG_NONE = -1;
constexpr auto COMBO_TIMESIG_2_2 = 0;
constexpr auto COMBO_TIMESIG_3_2 = 1;
constexpr auto COMBO_TIMESIG_2_4 = 2;
constexpr auto COMBO_TIMESIG_3_4 = 3;
constexpr auto COMBO_TIMESIG_4_4 = 4;
constexpr auto COMBO_TIMESIG_5_4 = 5;
constexpr auto COMBO_TIMESIG_6_8 = 6;

//---------------------------------
//	Combo Box Indexes for Instgrument
//----------------------------------

constexpr auto COMBO_Index_INST_1 = 0;
constexpr auto COMBO_Index_INST_2 = 1;
constexpr auto COMBO_Index_INST_3 = 2;
constexpr auto COMBO_Index_INST_4 = 3;
constexpr auto COMBO_Index_INST_5 = 4;
constexpr auto COMBO_Index_INST_6 = 5;
constexpr auto COMBO_Index_INST_7 = 6;
constexpr auto COMBO_Index_INST_8 = 7;
constexpr auto COMBO_Index_INST_9 = 8;
constexpr auto COMBO_Index_INST_10 = 9;
constexpr auto COMBO_Index_INST_11 = 10;
constexpr auto COMBO_Index_INST_12 = 11;
constexpr auto COMBO_Index_INST_13 = 12;
constexpr auto COMBO_Index_INST_14 = 13;
constexpr auto COMBO_Index_INST_15 = 14;

//----------------------------------
//	Combo Box Indexes for Note Types
//----------------------------------

constexpr auto COMBO_NOTE_WHOLE = 0;
constexpr auto COMBO_NOTE_HALF = 1;
constexpr auto COMBO_NOTE_QUARTER = 2;
constexpr auto COMBO_NOTE_EIGTH = 3;
constexpr auto COMBO_NOTE_SIXTEENTH = 4;
constexpr auto COMBO_NOTE_THIRTYSECOND = 5;

//----------------------------------
// Decorations Combo Box Indexes
//----------------------------------

enum class ComboBoxCheckBox {
	COMBO_DECORATION_CHECK_DIS,
	COMBO_DECORATION_CHECK_EN
};

constexpr auto COMBO_DECORATION_ACCENT = 0;
constexpr auto COMBO_DECORATION_NONE = 1;
constexpr auto COMBO_DECORATION_DOT = 2;
constexpr auto COMBO_DECORATION_TRIPLET = 3;
