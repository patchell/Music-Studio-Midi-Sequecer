#pragma once

//**************************************
// Controllers on the Launchkey 25
//***************************************

constexpr auto MIDI_CTRLCHNG_KNOB_INSTRUMENT = 21;
constexpr auto MIDI_CTRLCHG_DECORATION = 22;
constexpr auto MIDI_CTRLCHNG_KNOB_NOTE_DUR = 23;
constexpr auto MIDI_CTRLCHNG_KNOB_REST_DUR = 24;
constexpr auto MIDI_CTRLCHNG_ACCIDENTAL = 25;
constexpr auto MIDI_CTRLCHNG_KEY = 26;

constexpr auto MIDI_CTRLCHNG_ROUNDUPPER = 104;
constexpr auto MIDI_CTRLCHGN_ROUNDLOWER = 105;

constexpr auto MIDI_CTRLCHNG_REWIND = 112;
constexpr auto MIDI_CTRLCHNG_FASTFORWARD = 113;
constexpr auto MIDI_CTRLCHNG_STOP = 114;
constexpr auto MIDI_CTRLCHNG_PLAY = 115;
constexpr auto MIDI_CTRLCHNG_PAUSE = 116;
constexpr auto MIDI_CTRLCHNG_RECORD = 117;
//------------------------------------------
// Midi channel defines for Launchkey25
//------------------------------------------
constexpr auto KEYBOARD_MIDI_CHANNEL = 0;	//midi channel for keyboard;
constexpr auto BUTTON_MIDI_CHANNEL = 9;	//midi chanel of button pushes;
//-----------------------------------------
// Note numbers for Buttons
//-----------------------------------------
//---------------------------------------
// Note ON/OFF Midi CH 9
//---------------------------------------
constexpr auto LK25_BUTTON_TOP1 = 40;
constexpr auto LK25_BUTTON_TOP2 = 41;
constexpr auto LK25_BUTTON_TOP3 = 42;
constexpr auto LK25_BUTTON_TOP4 = 43;
constexpr auto LK25_BUTTON_TOP5 = 48;
constexpr auto LK25_BUTTON_TOP6 = 49;
constexpr auto LK25_BUTTON_TOP7 = 50;
constexpr auto LK25_BUTTON_TOP8 = 51;

constexpr auto LK25_BUTTON_BOT1 = 36;
constexpr auto LK25_BUTTON_BOT2 = 37;
constexpr auto LK25_BUTTON_BOT3 = 38;
constexpr auto LK25_BUTTON_BOT4 = 39;
constexpr auto LK25_BUTTON_BOT5 = 44;
constexpr auto LK25_BUTTON_BOT6 = 45;
constexpr auto LK25_BUTTON_BOT7 = 46;
constexpr auto LK25_BUTTON_BOT8 = 47;

constexpr auto LK25_CH9_BUTTONS_BASEVALUE = 0x100;
//---------------------------------------
// Control Change Midi Ch 0
//---------------------------------------

constexpr auto LK25_BUTTONRND_TOP = 104;
constexpr auto LK25_BUTTONRND_BOT = 105;
constexpr auto LK25_KNOB_TOP1 = 21;
constexpr auto LK25_KNOB_TOP2 = 22;
constexpr auto LK25_KNOB_TOP3 = 23;
constexpr auto LK25_KNOB_TOP4 = 24;
constexpr auto LK25_KNOB_TOP5 = 25;
constexpr auto LK25_KNOB_TOP6 = 26;
constexpr auto LK25_KNOB_TOP7 = 27;
constexpr auto LK25_KNOB_TOP8 = 28;

constexpr auto LK25_SLIDER = 7;
constexpr auto LK25_BUTTON_REW = 112;
constexpr auto LK25_BUTTON_FFW = 113;
constexpr auto LK25_BUTTON_PAUSE = 114;
constexpr auto LK25_BUTTON_PLAY = 115;
constexpr auto LK25_BUTTON_CIRC = 116;
constexpr auto LK25_BUTTON_REC = 117;

constexpr auto LK25_WHEEL_PBND = 256;		//there is only one PB;
constexpr auto LK25_WHEEL_MOD = 1;

