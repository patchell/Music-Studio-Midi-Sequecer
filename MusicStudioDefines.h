#pragma once


//-------------------------------
// Fundamental Constants
//-------------------------------
constexpr auto EVENT_WIDTH = 40;	// width of each event5;
constexpr auto STAVE_LINE_SPACING = 8;
constexpr auto STAFF_HEIGHT = STAVE_LINE_SPACING * 20;	// Distance from top line of
														// treble stave to bottom line
														// of bass stave
constexpr auto BLACK_KEYS_PER_OCTAVE = 5;
constexpr auto WHITE_KEYS_PER_OCTAVE = 7;
constexpr auto HALF_STEPS_PER_OCTAVE = 12;
constexpr auto TOTAL_WHITE_KEYS = WHITE_KEYS_PER_OCTAVE * 4 + 1;
constexpr auto SELECTION_BAR_HEIGHT = 24;
constexpr auto NOTE_STEM_LENGTH = 24;
constexpr auto UPPER_SELECTION_BAR_HEIGHT = SELECTION_BAR_HEIGHT;
constexpr auto UPPER_DRAW_RECT_HEIGHT = (NOTE_STEM_LENGTH);
constexpr auto EDIT_RECT_HEIGHT = 15 * STAVE_LINE_SPACING;
constexpr auto LOWER_DRAW_RECT_HEIGHT = (NOTE_STEM_LENGTH);
constexpr auto LOWER_SELECTION_BAR_HEIGHT = SELECTION_BAR_HEIGHT;
//constexpr auto FIRST_EVENT_FROM_CLIENT_X = 32;	// distance from left edge;
												// of the client rect to
												// where the first event is
constexpr auto CLIENT_TO_TOP_UPPER_SELECT_RECT = 48;	// distance from top of client rect




//-------------------------------
// Constants that are used to
// draw the music staff and events
// in the client view.
//-------------------------------


//constexpr auto CLIENT_TO_STAVE_TOP_Y = 132;	//DISTANCE from top of client rect
											// to the top of the treble stave																				// to the top line of the treble stave
constexpr auto EVENT_HEIGHT = 	// height of each event
					UPPER_SELECTION_BAR_HEIGHT +
					UPPER_DRAW_RECT_HEIGHT +
					EDIT_RECT_HEIGHT +
					LOWER_DRAW_RECT_HEIGHT +
					LOWER_SELECTION_BAR_HEIGHT;
constexpr auto EVENT_DRAW_AREA_HEIGHT =
				UPPER_DRAW_RECT_HEIGHT +
				EDIT_RECT_HEIGHT +
				LOWER_DRAW_RECT_HEIGHT;

constexpr auto CLIENT_Y_TO_TOP_OF_EDIT_RECT =	CLIENT_TO_TOP_UPPER_SELECT_RECT + 
												UPPER_SELECTION_BAR_HEIGHT +
												UPPER_DRAW_RECT_HEIGHT;
constexpr auto TOP_LINE_OF_STAVE_FROM_TOP_OF_EDIT_RECT = STAVE_LINE_SPACING * 3;
constexpr auto UPPER_SEL_RECT_TO_TOP_EDIT_RECT = CLIENT_TO_TOP_UPPER_SELECT_RECT + SELECTION_BAR_HEIGHT;
constexpr auto EDIT_RECT_TO_TOP_LOWER_SELECT_RECT = UPPER_SEL_RECT_TO_TOP_EDIT_RECT + EVENT_HEIGHT;
// to top of upper selection rect
constexpr auto SINGLE_STAVE_HEIGHT = STAVE_LINE_SPACING * 4;	// Distance from top line to bottom line of a single stave
constexpr auto STAVE_HEIGHT = STAVE_LINE_SPACING * 10;	// Distance from top line of;
														// The treble stave to the
														// bottom line of the base stave
//----------------------------------------------
// Song Drawing Constants
//----------------------------------------------
constexpr auto TREBLE_TOP_LINE = (3 * STAVE_LINE_SPACING) + NOTE_STEM_LENGTH;	//Fix?
constexpr auto BASS_TOP_LINE = TREBLE_TOP_LINE + SINGLE_STAVE_HEIGHT + STAVE_LINE_SPACING * 2;

constexpr auto HIGH_C = NOTE_STEM_LENGTH + STAVE_LINE_SPACING /2;	//how high high C is below top of event rectangle 
constexpr auto Y_CENTER_OF_TREBLE = (TREBLE_TOP_LINE);
constexpr auto Y_CENTER_OF_BASS = (BASS_TOP_LINE);
constexpr auto X_CENTER_OF_EVENT = EVENT_WIDTH / 2;
//------------------------------------
// Selection Bars
//------------------------------------

#define SELECTION_BAR_WIDTH(ClientWidth) ((ClientWidth) - FIRST_EVENT_FROM_CLIENT_X)
constexpr auto SELECTION_BAR_X = EVENT_WIDTH;

//-------------------------------
// Status Bar Constants
//-------------------------------

constexpr auto STATUS_BAR_HEIGHT = 25;

//-------------------------------
// MyDrop Combo Box Constants
//-------------------------------

constexpr auto DROPBOX_ARROW_WIDTH = 16;

//-------------------------------
// State Changes between regions
// in the Music Studio View.
//-------------------------------

enum class MouseRegionTransitionState : int {
	MOUSE_TRANSITION_NONE,
	//-------------------------------
	MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW,
	MOUSE_TRANSITION_EDIT_TO_OUTSIDE,
	MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW,
	//-------------------------------
	MOUSE_TRANSITION_LOWERSEL_TO_LOWER_DRAW,
	MOUSE_TRANSITION_LOWERSEL_TO_OUTSIDE,
	//-------------------------------
	MOUSE_TRANSITION_UPPERSEL_TO_UPPER_DRAW,
	MOUSE_TRANSITION_UPPERSEL_TO_OUTSIDE,
	//-------------------------------
	MOUSE_TRANSITION_OUTSIDE_TO_LOWERSEL,
	MOUSE_TRANSITION_OUTSIDE_TO_UPPERSEL,
	MOUSE_TRANSITION_OUTSIDE_TO_EDIT,
	MOUSE_TRANSITION_OUTSIDE_TO_LOWER_DRAW,
	MOUSE_TRANSITION_OUTSIDE_TO_UPPER_DRAW,
	//-------------------------------
	MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT,
	MOUSE_TRANSITION_UPPER_DRAW_TO_OUTSIDE,
	MOUSE_TRANSITION_UPPER_DRAW_TO_UPPER_SEL,
	//-------------------------------
	MOUSE_TRANSITION_LOWER_DRAW_TO_EDIT,
	MOUSE_TRANSITION_LOWER_DRAW_TO_OUTSIDE,
	MOUSE_TRANSITION_LOWER_DRAW_TO_LOWER_SEL,
	//-------------------------------
	MOUSE_TRANSITION_ERROR
};

//-------------------------------
// Regions tha the mouse can be
// in the Music Studio Staff
// View.
//-------------------------------

enum class MouseRegions : int {
	MOUSE_OUTSIDE,
	MOUSE_IN_EDITREG,
	MOUSE_IN_UPPERSEL,
	MOUSE_IN_UPPERDRAW,
	MOUSE_IN_LOWERSEL,
	MOUSE_IN_LOWERDRAW,
	MOUSE_NONE
};

//-------------------------------
// State changes while mouse is
// in the Music Studio Staff
// view Edit Region
//-------------------------------

enum class StaffMouseStates : int {
	MOUSE_STAFF_STATE_NONE,			// No Changes
	MOUSE_STAFF_STATE_NOTE_CHANGE,	// The note has changed
	MOUSE_STAFF_STATE_EVENT_CHANGE,	// The event has changed
	MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE,	// Both note and event have changed
	END_STATE
};