#include "pch.h"

CHORD ChordsLUT[NUMBER_OF_CHORDS] = {
	{INTERVAL_M3,INTERVAL_P5,INTERVAL_NONE,INTERVAL_NONE,_T("Triad"),_T("k")},		//Triad
	{INTERVAL_M3,INTERVAL_P5,INTERVAL_M6,INTERVAL_NONE,_T("Major 6th"),_T("kM6")},		//Major 6th
	{INTERVAL_M3,INTERVAL_P5,INTERVAL_m7,INTERVAL_NONE,_T("Dominant 7th"),_T("k7")},		//Dominant 7th
	{INTERVAL_M3,INTERVAL_P5,INTERVAL_M7,INTERVAL_NONE,_T("Major 7th"),_T("kMaj7")},		//Major 7th
	{INTERVAL_M3,INTERVAL_A5,INTERVAL_NONE,INTERVAL_NONE,_T("Augmented Triad"),_T("kaug")},		//Augmented Triad
	{INTERVAL_M3,INTERVAL_A5,INTERVAL_m7,INTERVAL_NONE,_T("Augmented 7th"),_T("kaug7")},		//Augmented 7th
	{INTERVAL_m3,INTERVAL_P5,INTERVAL_NONE,INTERVAL_NONE,_T("Minor Triad"),_T("km")},		//Minor Triad
	{INTERVAL_m3,INTERVAL_P5,INTERVAL_M6,INTERVAL_NONE,_T("Minor 6th"),_T("km6")},		//Minor 6th
	{INTERVAL_m3,INTERVAL_P5,INTERVAL_m7,INTERVAL_NONE,_T("Minor 7th"),_T("km7")},		//Minor 7th
	{INTERVAL_m3,INTERVAL_P5,INTERVAL_M7,INTERVAL_NONE,_T("Minor Major 7th"),_T("kmM7")},		//Minor Major 7th
	{INTERVAL_m3,INTERVAL_d5,INTERVAL_NONE,INTERVAL_NONE,_T("Diminished Triad"),_T("kdim")},		//Diminished Triad
	{INTERVAL_m3,INTERVAL_d5,INTERVAL_d7,INTERVAL_NONE,_T("Diminished 7th"),_T("kdim7")},		//Diminished 7th
	{INTERVAL_m3,INTERVAL_d5,INTERVAL_m7,INTERVAL_NONE,_T("Half Diminished 7th"),_T("km7")}		//Half Diminished 7th
};


IntervalLUTentry IntervalLUT[NUMBER_OF_INTERVALS]{
	{_T("Root"),_T("R"), 0},
	{_T("Minor 2nd"),_T("m2"), 1},
	{_T("Major 2nd"),_T("M2"), 2},
	{_T("Augmented 2nd"),_T("A2"), 3},
	{_T("Diminished 2nd"),_T("d2"), 1},
	{_T("Minor 3rd"),_T("m3"), 3},
	{_T("Major 3rd"),_T("M3"), 4},
	{_T("Augmented 3rd"),_T("A3"), 5},
	{_T("Diminshed 3rd"),_T("d3"), 1},
	{_T("Perfect 4th"),_T("P4"), 5},
	{_T("Augmented 4th"),_T("A4"), 6},
	{_T("Diminished 4th"),_T("d4"), 4},
	{_T("Perfect 5th"),_T("P5"), 7},
	{_T("Augmented 5th"),_T("A5"), 8},
	{_T("Diminished 5th"),_T("d5"), 6},
	{_T("Minor 6th"),_T("m6"),8 },
	{_T("Major 6th"),_T("M6"), 9},
	{_T("Augmented 6th"),_T("A6"), 10},
	{_T("Diminished 6th"),_T("R"), 7},
	{_T("Minor 7th"),_T("m7"), 10},
	{_T("Major 7th"),_T("M7"), 11},
	{_T("Augmented 7th"),_T("A7"), 12},
	{_T("Diminished 7th"),_T("d7"), 9},
};
