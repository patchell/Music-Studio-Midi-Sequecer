// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

//------------------------------
// Usefull Macros
//------------------------------

#define STATUS(x) (x & 0x000000FF)	
#define CHANNEL(x) ((x & 0x0000000F) + 1)
#define CHANNELMSG(x) (x & 0x000000F0)
#define SECONDBYTE(x) ((x & 0x0000FF00) >> 8)
#define THIRDBYTE(x) ((x & 0x00FF0000) >> 16)

//------------------------------

#include "framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mmsystem.h>
#include <afxwin.h>
#include "afxdialogex.h"
#include <afxmt.h>
#include "Midi.h"

extern FILE* LogFile();

//--------------------------------------
// Color Palette
//--------------------------------------

struct SColorPalette {
	COLORREF color_BackGround;
	COLORREF color_StaffLines;
	COLORREF color_UpperSelBar;
	COLORREF color_LowerSelBar;
	COLORREF color_LastEventBKG;
	COLORREF color_DefinedEvent;
	COLORREF color_EventCursor;
	COLORREF color_MeasureBars;
	COLORREF color_SelectedNotes;
	COLORREF color_SelectionRect;
	COLORREF color_Text;
	COLORREF color_Highlight;
	SColorPalette()
	{
		color_BackGround = RGB(50, 50, 50);
		color_StaffLines = RGB(127, 127, 127);
		color_UpperSelBar = RGB(88, 128, 88);
		color_LowerSelBar = RGB(88, 128, 88);
		color_LastEventBKG = RGB(60, 100, 224);
		color_DefinedEvent = RGB(92, 0, 28);
		color_EventCursor = RGB(16, 16, 16);
		color_MeasureBars = RGB(192, 192, 192);
		color_SelectedNotes = RGB(200, 200, 255);
		color_SelectionRect = RGB(0, 0, 255);
		color_Text = RGB(192, 192, 192);
		color_Highlight = RGB(255, 0, 0);
	}
};

//---------------------------------------
// Draw State Machine
//---------------------------------------
enum class DRAWSTATE {
	NA,
	SET_ATTRIBUTES,
	WAITFORMOUSE_DOWN,
	MOVE_OBJECT_AROUND,
	PLACE,
	TIE_FIRSTNOTE,
	TIE_SECONDNOTE,
	GLISSANDO_FIRST_NOTE,
	GLISSANDO_SECOND_NOTE,
	GLISSANDO_END
};

//---------------------------------------
// Music Editor
//---------------------------------------
constexpr auto APP_NUM_TIMESIG = 7;
constexpr auto APP_NUM_INSTRUMENTS = 15;
constexpr auto APP_NUM_NOTETYPES = 6;
constexpr auto APP_NUM_RESTTYPES = 6;
constexpr auto APP_NUM_DECORATIONS = 4;
constexpr auto APP_NUM_ACCIDENTALTYPES = 4;
constexpr auto APP_NUM_BLOCKOPTYPES = 11;
constexpr auto APP_NUM_MISC = 7;
constexpr auto APP_NUM_KEYSIGNATURES = 15;
//-----------------------------------------
// TX816 Editor
//-----------------------------------------
constexpr auto APP_TX816_NUM_LFO_WAVES = 6;
constexpr auto APP_TX816_NUM_ALG_BITMAPS = 32;
//-----------------------------------------
#include "MusicStudioDefines.h"
#include "ComboBoxDefines.h"
#include "MusicStudioFileFormat.h"
#include "WindowMessages.h"
#include "Polygon.h"
#include "MyEvent.h"
#include "MMtimer.h"
#include "MyMMtimer.h"
#include "MyRgn.h"
#include "MyBitmap.h"
#include "resource.h"
#include "LaunchKey25.h"
#include "MidiControllerObject.h"
#include "MidiOutDevice.h"
#include "MidiOutTable.h"
#include "MidiInMessageDispatcher.h"
#include "MidiInDevice.h"
#include "MidiInTable.h"
#include "SysExTX816.h"
#include "StaticBitmap.h"
#include "MsTrack.h"
#include "targetver.h"
#include "MyComboBox.h"
#include "MyListBox.h"
#include "ComboBitmap.h"
#include "ComboBoxItem.h"
#include "MyOwnSlider.h"
#include "MyOwnCheckBox.h"
#include "ComboDropUp.h"
#include "ComboDropDown.h"
#include "ComboDropDownToggle.h"
#include "StaticCheckBitmap.h"
#include "StaticStatus.h"
#include "SelectMidi.h"
#include "MyButton.h"
#include "MsObject.h"	//Base class for all music objects, including staff objects and events
#include "MsStackObject.h"
#include "MsStack.h"
#include "MsEvent.h"
#include "MsEventDirectory.h"
#include "MsEventChain.h"
//------------- Music Staff Objects ----------------
#include "MsNote.h"
#include "MsBar.h"
#include "MsBassCleff.h"
#include "MsChord.h"
#include "MsChordNote.h"
#include "MsEndBar.h"
#include "MsFlat.h"
#include "MsGlisando.h"
#include "MsKeySignature.h"
#include "MsGlissandoEnd.h"
#include "MsLoudness.h"
#include "MsNatural.h"
#include "MsPortamentoEnd.h"
#include "MsPortamentoStart.h"
#include "MsRepeatEnd.h"
#include "MsRepeatStart.h"
#include "MsSharp.h"
#include "MsTempo.h"
#include "MsTimeSig.h"
#include "MsTrebleCleff.h"
//--------------------------------------
#include "MsSongInfo.h"
#include "MsSong.h"
#include "MidiSeqMS.h"
#include "MsPlayerQueueItem.h"
#include "MsPlayerQueue.h"
#include "ChildViewBase.h"
#include "ChildFrameBase.h"
#include "ChildViewStaff.h"
#include "ChildFrmStaff.h"
#include "ChildViewYamahaTX816.h"
#include "ChildFrameYamahaTX816.h"
#include "ChildViewLaunchControlXL.h"
#include "ChildFrameLaunchControlXL.h"
#include "MainFrm.h"
#include "ContextMenuObject.h"
#include "ContextMenuList.h"
#include "MyScrollBar.h"
#include "MainFrm.h"
#include "DlgGlissandoProperties.h"
#include "NoteComboBox.h"
#include "DlgSelectMidiOut.h"
#include "DlgSelectMidiIn.h"
#include "NotePropertiesDlg.h"
#include "NumEdit.h"
#include "EnterStringDlg.h"
#include "ParamDlg.h"
#include "SelDurDlg.h"
#include "SelectorDlg.h"
#include "SelectStringDlg.h"
#include "DlgMidiInfo.h"
#include "DlgMidiInDispatchEditor.h"


#endif //PCH_H
