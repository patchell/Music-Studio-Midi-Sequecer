#pragma once

//---------------------------------
// time signature bitmap indexes
//---------------------------------
enum {
	BM_TIMESIG_2_2,
	BM_TIMESIG_3_2,
	BM_TIMESIG_2_4,
	BM_TIMESIG_3_4,
	BM_TIMESIG_4_4,
	BM_TIMESIG_5_4,
	BM_TIMESIG_6_8
};

//-------------------------------
// rest bitmap indexes
//------------------------------

enum {
	BM_REST_INDEX_WHOLE,
	BM_REST_INDEX_HALF,
	BM_REST_INDEX_QUARTER,
	BM_REST_INDEX_EIGTH,
	BM_REST_INDEX_SIXTEENTH,
	BM_REST_INDEX_THIRTYSECOND
};

//---------------------------
// Note bitmap indexes
//---------------------------

enum {
	BM_NOTE_INDEX_WHOLE,
	BM_NOTE_INDEX_HALF,
	BM_NOTE_INDEX_QUARTER,
	BM_NOTE_INDEX_EIGTH,
	BM_NOTE_INDEX_SIXTEENTH,
	BM_NOTE_INDEX_THIRTYSECOND
};

enum {
	BM_MISC_INDEX_MEASUREBAR,
	BM_MISC_INDEX_NOTETIE,
	BM_MISC_INDEX_LOUDNESS,
	BM_MISC_INDEX_TEMPO,
	BM_MISC_INDEX_INSTUMENT_CHANGE
};

//------------------------------------
//	ComboBox Indexes for Misc Stuff
//------------------------------------

enum {
	COMBO_MISC_MEASUREBAR,
	COMBO_MISC_NOTETIE,
	COMBO_MISC_LOUDNESS,
	COMBO_MISC_TEMPO,
	COMBO_MISC_INST_CHANGE,
	COMBO_MISC_ENDBAR

};
//-------------------------------------
//	ComboBox Indexes for Block Operations
//-------------------------------------

enum {
	COMBO_BLOCK_COPY,
	COMBO_BLOCK_MOVE,
	COMBO_BLOCK_CHANGEINSTRUMENT,
	COMBO_BLOCK_CHANGEDUR,
	COMBO_BLOCK_INCREASE,
	COMBO_BLOCK_DECREASE,
	COMBO_BLOCKUP,
	COMBO_BLOCKDOWN,
	COMBO_BLOCK_REPEAT,
	COMBO_BLOCK_INSERT,
	COMBO_BLOCK_MIDIINPUT
};

//-------------------------------------
// ComboBox Indexes for Accidentals
//-------------------------------------

enum {
	COMBO_ACCIDENTAL_INKEY,
	COMBO_ACCIDENTAL_SHARP,
	COMBO_ACCIDENTAL_FLAT,
	COMBO_ACCIDENTAL_NATURAL

};
//-------------------------------------
// ComboBox Indexes for Rest Types
//-------------------------------------

enum {
	COMBO_REST_WHOLE,
	COMBO_REST_HALF,
	COMBO_REST_QUARTER,
	COMBO_REST_EIGHTH,
	COMBO_REST_SIXTEENTH,
	COMBO_REST_THIRTYSEC
};
//-------------------------------------
//	ComboBox Indicies for Time Signiture
//-------------------------------------

enum {
	COMBO_TIMESIG_NONE,
	COMBO_TIMESIG_2_2,
	COMBO_TIMESIG_3_2,
	COMBO_TIMESIG_2_4,
	COMBO_TIMESIG_3_4,
	COMBO_TIMESIG_4_4,
	COMBO_TIMESIG_5_4,
	COMBO_TIMESIG_6_8
};
//---------------------------------
//	Combo Box Indexes for Instgrument
//----------------------------------

enum {
	COMBO_INST_1,
	COMBO_INST_2,
	COMBO_INST_3,
	COMBO_INST_4,
	COMBO_INST_5,
	COMBO_INST_6,
	COMBO_INST_7,
	COMBO_INST_8,
	COMBO_INST_9,
	COMBO_INST_10,
	COMBO_INST_11,
	COMBO_INST_12,
	COMBO_INST_13,
	COMBO_INST_14,
	COMBO_INST_15
};
//----------------------------------
//	Combo Box Indexes for Note Types
//----------------------------------

enum {
	COMBO_NOTE_WHOLE,
	COMBO_NOTE_HALF,
	COMBO_NOTE_QUARTER,
	COMBO_NOTE_EIGTH,
	COMBO_NOTE_SIXTEENTH,
	COMBO_NOTE_THIRTYSECOND
};

//----------------------------------
// Decorations Combo Box Indexes
//----------------------------------

enum {
	COMBO_DECORATION_ACCENT,
	COMBO_DECORATION_NONE,
	COMBO_DECORATION_DOT,
	COMBO_DECORATION_TRIPLET
};

enum {
	COMBO_DECORATION_CHECK_DIS,
	COMBO_DECORATION_CHECK_EN
};

//------------------------------------
// General CComboDropDownToggle defines
//------------------------------------

// Item flags
#define COMBOTOGGLE_TOGGLE_ENABLE		0X01
#define	COMBOTOGGLE_TOGGLE_EXCLUSIVE	0X02
#define	COMBOTOGGLE_TOGGLE_CHECKED		0X04

union CBToggle {
	struct {
		unsigned value : 8;
		unsigned command : 8;
		unsigned index : 8;
	};
	WPARAM Wparam;
};

// command values

enum {
	CBT_SETVALUE,
	CBT_INDEX_ONLY
};

