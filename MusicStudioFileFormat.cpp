#include "pch.h"

CHORD ChordsLUT[NUMBER_OF_CHORDS] = {
	{INTERVAL_M3,INTERVAL_P5,INTERVAL_NONE,INTERVAL_NONE,CString("Triad"),CString("k")},		//Triad
	{INTERVAL_M3,INTERVAL_P5,INTERVAL_M6,INTERVAL_NONE,CString("Major 6th"),CString("kM6")},		//Major 6th
	{INTERVAL_M3,INTERVAL_P5,INTERVAL_m7,INTERVAL_NONE,CString("Dominant 7th"),CString("k7")},		//Dominant 7th
	{INTERVAL_M3,INTERVAL_P5,INTERVAL_M7,INTERVAL_NONE,CString("Major 7th"),CString("kMaj7")},		//Major 7th
	{INTERVAL_M3,INTERVAL_A5,INTERVAL_NONE,INTERVAL_NONE,CString("Augmented Triad"),CString("kaug")},		//Augmented Triad
	{INTERVAL_M3,INTERVAL_A5,INTERVAL_m7,INTERVAL_NONE,CString("Augmented 7th"),CString("kaug7")},		//Augmented 7th
	{INTERVAL_m3,INTERVAL_P5,INTERVAL_NONE,INTERVAL_NONE,CString("Minor Triad"),CString("km")},		//Minor Triad
	{INTERVAL_m3,INTERVAL_P5,INTERVAL_M6,INTERVAL_NONE,CString("Minor 6th"),CString("km6")},		//Minor 6th
	{INTERVAL_m3,INTERVAL_P5,INTERVAL_m7,INTERVAL_NONE,CString("Minor 7th"),CString("km7")},		//Minor 7th
	{INTERVAL_m3,INTERVAL_P5,INTERVAL_M7,INTERVAL_NONE,CString("Minor Major 7th"),CString("kmM7")},		//Minor Major 7th
	{INTERVAL_m3,INTERVAL_d5,INTERVAL_NONE,INTERVAL_NONE,CString("Diminished Triad"),CString("kdim")},		//Diminished Triad
	{INTERVAL_m3,INTERVAL_d5,INTERVAL_d7,INTERVAL_NONE,CString("Diminished 7th"),CString("kdim7")},		//Diminished 7th
	{INTERVAL_m3,INTERVAL_d5,INTERVAL_m7,INTERVAL_NONE,CString("Half Diminished 7th"),CString("km7")}		//Half Diminished 7th
};


IntervalLUTentry IntervalLUT[NUMBER_OF_INTERVALS]{
	{CString("Root"),CString("R"), 0},
	{CString("Minor 2nd"),CString("m2"), 1},
	{CString("Major 2nd"),CString("M2"), 2},
	{CString("Augmented 2nd"),CString("A2"), 3},
	{CString("Diminished 2nd"),CString("d2"), 1},
	{CString("Minor 3rd"),CString("m3"), 3},
	{CString("Major 3rd"),CString("M3"), 4},
	{CString("Augmented 3rd"),CString("A3"), 5},
	{CString("Diminshed 3rd"),CString("d3"), 1},
	{CString("Perfect 4th"),CString("P4"), 5},
	{CString("Augmented 4th"),CString("A4"), 6},
	{CString("Diminished 4th"),CString("d4"), 4},
	{CString("Perfect 5th"),CString("P5"), 7},
	{CString("Augmented 5th"),CString("A5"), 8},
	{CString("Diminished 5th"),CString("d5"), 6},
	{CString("Minor 6th"),CString("m6"),8 },
	{CString("Major 6th"),CString("M6"), 9},
	{CString("Augmented 6th"),CString("A6"), 10},
	{CString("Diminished 6th"),CString("R"), 7},
	{CString("Minor 7th"),CString("m7"), 10},
	{CString("Major 7th"),CString("M7"), 11},
	{CString("Augmented 7th"),CString("A7"), 12},
	{CString("Diminished 7th"),CString("d7"), 9},
};
