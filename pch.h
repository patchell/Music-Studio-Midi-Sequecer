// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include "framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mmsystem.h>
#include <afxwin.h>
#include "afxdialogex.h"
#include <afxmt.h>

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

//-----------------------------------------------------------
// TX816 Editor Bitmap IDs
//-----------------------------------------------------------
extern UINT TX816_LFO_WaveBmAPP_TX816_NUM_LFO_WAVES[APP_TX816_NUM_LFO_WAVES];
extern UINT TX816_AlgrithmBitmapID[APP_TX816_NUM_ALG_BITMAPS];
extern CString MidiNoteToNoteString[127];
extern CString TX816_OP_KEYBORD_Level_Scaling_Curve[4];

//-----------------------------------------------------
// Midi In Dispatcher
//-------------------------------------------------

enum MidiStatusIDs {
	STATUS_ID_MIDI_NOTEOFF,
	STATUS_ID_MIDI_NOTEON,
	STATUS_ID_MIDI_POLYPRESS,
	STATUS_ID_MIDI_CTRLCHNG,
	STATUS_ID_MIDI_PGMCHANGE,
	STATUS_ID_MIDI_CHNLPRESS,
	STATUS_ID_MIDI_PITCHBEND,
	STATUS_ID_MIDI_TIMECODE,
	STATUS_ID_MIDI_SONGPOSITION,
	STATUS_ID_MIDI_SONGSELECT,
	STATUS_ID_MIDI_RESERVED_F4,
	STATUS_ID_MIDI_RESERVED_F5,
	STATUS_ID_MIDI_TUNEREQUEST,
	STATUS_ID_MIDI_CLOCK,
	STATUS_ID_MIDI_RTM_RSVRD_F9,
	STATUS_ID_MIDI_START,
	STATUS_ID_MIDI_CONTINUE,
	STATUS_ID_MIDI_STOP,
	STATUS_ID_MIDI_RTM_RSVRD_FD,
	STATUS_ID_MIDI_ACTIVE_SENSING,
	STATUS_ID_MIDI_RESET,
	STATUS_ID_MIDI_SYSEX,
	STATUS_ID_MIDI_SYSEXEND
};

extern int MidiStatusToMidiID(int status);
extern int IsChannelStatus(int StatusID);
extern const char* GenMidiPatchNames[128];
//------------------------------------------
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
#include "MidiInfo.h"
#include "MsObject.h"
#include "MsStackObject.h"
#include "MsStack.h"
#include "MsRepeatStart.h"
#include "MsEvent.h"
#include "MsEventChain.h"
#include "MsSong.h"
#include "MidiSeqMS.h"
#include "MsGlisando.h"
#include "MsPortamentoEnd.h"
#include "MsPortamentoStart.h"
#include "MsBar.h"
#include "MsEndBar.h"
#include "MsFlat.h"
#include "MsLoudness.h"
#include "MsNatural.h"
#include "MsNote.h"
#include "MsKeySignature.h"
#include "MsChordNote.h"
#include "MsRepeatEnd.h"
#include "MsSharp.h"
#include "MsTempo.h"
#include "MsChord.h"
#include "MsTimeSig.h"
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
