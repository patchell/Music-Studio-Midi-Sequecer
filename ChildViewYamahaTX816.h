#pragma once

//-----------
// Operators Control IDs
//-----------
constexpr auto DX7_CONTROL_ID_EG_SEGMENT_LEVEL_1 = 801;
constexpr auto DX7_CONTROL_ID_EG_SEGMENT_LEVEL_2 = 802;
constexpr auto DX7_CONTROL_ID_EG_SEGMENT_LEVEL_3 = 803;
constexpr auto DX7_CONTROL_ID_EG_SEGMENT_LEVEL_4 = 804;
constexpr auto DX7_CONTROL_ID_EG_SEGMENT_RATE_1 = 805;
constexpr auto DX7_CONTROL_ID_EG_SEGMENT_RATE_2 = 806;
constexpr auto DX7_CONTROL_ID_EG_SEGMENT_RATE_3 = 807;
constexpr auto DX7_CONTROL_ID_EG_SEGMENT_RATE_4 = 808;
constexpr auto DX7_CONTROL_ID_KEYBOARD_LEVEL_SCALING_BREAKPOINT = 809;
constexpr auto DX7_CONTROL_ID_KEYBOARD_LEVEL_SCALING_LEFT_DEPTH = 810;
constexpr auto DX7_CONTROL_ID_KEYBOARD_LEVEL_SCALING_RIGHT_DEPTH = 811;
constexpr auto DX7_CONTROL_ID_KEYBOARD_LEVEL_SCALING_LEFT_CURVE = 812;
constexpr auto DX7_CONTROL_ID_KEYBOARD_LEVEL_SCALING_RIGHT_CURVE = 813;
constexpr auto DX7_CONTROL_ID_KEYBOARD_RATE_SCALING = 814;
constexpr auto DX7_CONTROL_ID_AMPLITUDE_MODULATION_SENSITIVITY = 815;
constexpr auto DX7_CONTROL_ID_KEY_VELOCITY_SENSITIVITY = 816;
constexpr auto DX7_CONTROL_ID_OPERATOR_OUTPUT_LEVEL = 817;
constexpr auto DX7_CONTROL_ID_OSCILLATOR_MODE = 818;
constexpr auto DX7_CONTROL_ID_OSCILLATOR_FREQUENCY_COARSE = 819;
constexpr auto DX7_CONTROL_ID_OSCILLATOR_FREQUENCY_FINE = 820;
constexpr auto DX7_CONTROL_ID_OSCILLATOR_DETUNE = 821;
//-------------------------
// operator common control IDs
//-------------------------
constexpr auto DX7_CONTROL_EG_PITCH_ID_RATE_1 = 822;
constexpr auto DX7_CONTROL_EG_PITCH_ID_RATE_2 = 823;
constexpr auto DX7_CONTROL_EG_PITCH_ID_RATE_3 = 824;
constexpr auto DX7_CONTROL_EG_PITCH_ID_RATE_4 = 825;
constexpr auto DX7_CONTROL_EG_PITCH_ID_LEVEL_1 = 826;
constexpr auto DX7_CONTROL_EG_PITCH_ID_LEVEL_2 = 827;
constexpr auto DX7_CONTROL_EG_PITCH_ID_LEVEL_3 = 828;
constexpr auto DX7_CONTROL_EG_PITCH_ID_LEVEL_4 = 829;

constexpr auto DX7_CONTROL_ID_ALGORITHM_SELECT = 830;
constexpr auto DX7_CONTROL_ID_FEEDBACK = 831;
constexpr auto DX7_CONTROL_ID_OSCILLATOR_KEY_SYNC = 832;
constexpr auto DX7_CONTROL_ID_LFO_SPEED = 833;
constexpr auto DX7_CONTROL_ID_LFO_DELAY = 834;
constexpr auto DX7_CONTROL_ID_LFO_PITCH_MODULATION_DEPTH = 835;
constexpr auto DX7_CONTROL_ID_LFO_AMPLITUDE_MODULATION_DEPTH = 836;
constexpr auto DX7_CONTROL_ID_LFO_KEY_SYNC = 837;
constexpr auto DX7_CONTROL_ID_LFO_WAVEFORM = 838;
constexpr auto DX7_CONTROL_ID_LFO_PITCH_MODULATION_SENSITIVITY = 839;
constexpr auto DX7_CONTROL_ID_TRANSPOSE = 840;
//----------------------------------
// PERFORMANCE CONTROL IDs
//----------------------------------
constexpr auto DX7_CONTROL_ID_MIDICHAN_SELECT = 841;
constexpr auto DX7_CONTROL_ID_POLY_MONO = 842;
constexpr auto DX7_CONTROL_ID_PITCH_BEND_RANGE = 843;
constexpr auto DX7_CONTROL_ID_PITCH_BEND_STEP = 844;
constexpr auto DX7_CONTROL_ID_PORTAMENTO_TIME = 845;
constexpr auto DX7_CONTROL_ID_PORTAMENTO_GLISANDO = 846;
constexpr auto DX7_CONTROL_ID_PORTAMENTO_MODE = 847;

constexpr auto DX7_CONTROL_ID_MODULATION_WHEEL_SENSITIVITY = 848;
constexpr auto DX7_CONTROL_ID_MODULATION_WHEEL_ASSIGN_EGBIAS = 849;
constexpr auto DX7_CONTROL_ID_MODULATION_WHEEL_ASSIGN_AMPLITUDE = 850;
constexpr auto DX7_CONTROL_ID_MODULATION_WHEEL_ASSIGN_PITCH = 851;

constexpr auto DX7_CONTROL_ID_FOOT_CONTROL_SENSITIVITY = 852;
constexpr auto DX7_CONTROL_ID_FOOT_CONTROL_ASSIGN_EGBIAS = 853;
constexpr auto DX7_CONTROL_ID_FOOT_CONTROL_ASSIGN_AMPLITUDE = 854;
constexpr auto DX7_CONTROL_ID_FOOT_CONTROL_ASSIGN_PITCH = 855;

constexpr auto DX7_CONTROL_ID_AFTER_TOUCH_SENSITIVITY = 856;
constexpr auto DX7_CONTROL_ID_AFTER_TOUCH_ASSIGN_EGBIAS = 857;
constexpr auto DX7_CONTROL_ID_AFTER_TOUCH_ASSIGN_AMPLITUDE = 858;
constexpr auto DX7_CONTROL_ID_AFTER_TOUCH_ASSIGN_PITCH = 859;

constexpr auto DX7_CONTROL_ID_BREATH_CONTROLLER_SENSITIVITY = 860;
constexpr auto DX7_CONTROL_ID_BREATH_CONTROLLER_ASSIGN_EGBIAS = 861;
constexpr auto DX7_CONTROL_ID_BREATH_CONTROLLER_ASSIGN_AMPLITUDE = 862;
constexpr auto DX7_CONTROL_ID_BREATH_CONTROLLER_ASSIGN_PITCH = 863;

constexpr auto DX7_CONTROL_ID_MASTER_TUNING = 864;
//buttons
constexpr auto DX7_BUTTON_ID_NEXT = 865;
constexpr auto DX7_BUTTON_ID_PREV = 866;
constexpr auto DX7_BUTTON_ID_HOME = 867;
//--- edit controls
constexpr auto DX7_CONTROL_ID_VOICE_NAME = 868;
constexpr auto DX7_CONTROL_ID_PERFOMANCE_NAME = 869;

//---------------------------------------------
// Dump Request Types
//---------------------------------------------

constexpr auto DX7_MIDI_SYSEX_DUMPREQ_MESSAGE_SIZE = (5);

constexpr auto DX7_MIDI_SYSEX_DUMPREQ_EDITBUFFER_VOICE = (0);
constexpr auto DX7_MIDI_SYSEX_DUMPREQ_EDITBUFFER_PERFORMANCE = (1);
constexpr auto DX7_MIDI_SYSEX_DUMPREQ_BANK_PERFORMANCE = (2);
constexpr auto DX7_MIDI_SYSEX_DUMPREQ_BANK_VOICE = (9);
constexpr auto DX7_MIDI_SYSEX_DUMPREQ_DEVOCE_INFO = (125);
// base values for each operator, add to above
constexpr auto DX7_OP_PARAMETER_NUMBER_BASE_6 = 0;
constexpr auto DX7_OP_PARAMETER_NUMBER_BASE_5 = (21);
constexpr auto DX7_OP_PARAMETER_NUMBER_BASE_4 = 42;
constexpr auto DX7_OP_PARAMETER_NUMBER_BASE_3 = (63);
constexpr auto DX7_OP_PARAMETER_NUMBER_BASE_2 = (84);
constexpr auto DX7_OP_PARAMETER_NUMBER_BASE_1 = (105);

constexpr auto DX7_ALG_RECT = 0X1000;		//Point x,y Size cx,cy
constexpr auto DX7_ALG_RECT_LINE_COLOR = 0X1001;
constexpr auto DX7_ALG_RECT_FILL_COLOR = 0X1002;
constexpr auto DX7_ALG_TEXT_LOCATION = 0X1003;
constexpr auto DX7_ALG_TEXT_COLOR = 0X1004;
constexpr auto DX7_ALG_TEXT_CHARACTER = 0X1005;
constexpr auto DX7_ALG_LINE = 0X1006;				//Point1 x,y  Point2 x,y
constexpr auto DX7_ALG_LINE_COLOR = 0X1007;
constexpr auto DX7_ALG_BACKGROUND_COLOR = 0X1008;
constexpr auto DX7_ALG_STRING_ADD_CHAR = 0X1009;
constexpr auto DX7_ALG_STRING = 0X100A;

constexpr auto DX7_DRAWMODE_ALG = 0;
constexpr auto DX7_DRAWMODE_OPERATOR_1 = 1;
constexpr auto DX7_DRAWMODE_OPERATOR_2 = 2;
constexpr auto DX7_DRAWMODE_OPERATOR_3 = 3;
constexpr auto DX7_DRAWMODE_OPERATOR_4 = 4;
constexpr auto DX7_DRAWMODE_OPERATOR_5 = 5;
constexpr auto DX7_DRAWMODE_OPERATOR_6 = 6;
constexpr auto DX7_DRAWMODE_COMMON = 7;
constexpr auto DX7_DRAWMODE_PERFORMANCE = 8;

constexpr auto DX7_OPERATOR_1 = 1;
constexpr auto DX7_OPERATOR_2 = 2;
constexpr auto DX7_OPERATOR_3 = 3;
constexpr auto DX7_OPERATOR_4 = 4;
constexpr auto DX7_OPERATOR_5 = 5;
constexpr auto DX7_OPERATOR_6 = 6;
constexpr auto DX7_OPERATOR_COMMON = 7;
constexpr auto DX7_PERFORMANCE = 8;


//-------------------------------------------------
// voice Bulk Data (function = 9) stuff
//------------------------------------------------

struct BulkVoiceDataOperator {
	unsigned char m_EgRate1;							// 0
	unsigned char m_EgRate2;							// 1
	unsigned char m_EgRate3;							// 2
	unsigned char m_EgRate4;							// 3
	unsigned char m_EgLevel1;							// 4
	unsigned char m_EgLevel2;							// 5
	unsigned char m_EgLevel3;							// 6
	unsigned char m_EgLevel4;							// 7
	unsigned char m_KeyboardLevelScalingBreakpoint;		// 8
	unsigned char m_KeyboardLevelScalingLeftDepth;		// 9
	unsigned char m_KeyboardLevelScalingRightDepth;		// 10
	unsigned char m_KbLevelScaling_L_curve:2;			// 11---Bit Fields, yuck!
	unsigned char m_KbLevelScaling_R_curve : 2;
	unsigned char m_KbRateScaling:4;					// 12	
	unsigned char m__OscDetune : 3;
	unsigned char m_AMSensitivity:2;					// 13
	unsigned char m_KeyVelSensitivity:3;
	unsigned char m_OperatorOutputLevel;				// 14
	unsigned char m_OscMode:1;							// 15
	unsigned char m_OscFrequencyCoarse : 6;
	unsigned char m_OscillatorFrequencyFine;			// 16
	BulkVoiceDataOperator() {
		memset(this, 0, sizeof(BulkVoiceDataOperator));
	}
};

struct BulkVoiceData_F_equal_9 {
	BulkVoiceDataOperator Operators[6];
};

//-------------------------------------------------
// TX816 voice data objects
//-------------------------------------------------
struct TX816CommonParams {
	unsigned char m_PitchEgRate1;
	unsigned char m_PitchEgRate2;
	unsigned char m_PitchEgRate3;
	unsigned char m_PitchEgRate4;
	unsigned char m_PitchEgLevel1;
	unsigned char m_PitchEgLevel2;
	unsigned char m_PitchEgLevel3;
	unsigned char m_PitchEgLevel4;
	unsigned char m_AlgorithmSelect;
	unsigned char m_Feedback;
	unsigned char m_OsacillatorKeySync;
	unsigned char m_LfoSpeed;
	unsigned char m_LfoDelay;
	unsigned char m_LfoPitchModulationDepth;
	unsigned char m_LfoAmplitudeModulationDepth;
	unsigned char m_LfoKeySync;
	unsigned char m_LfoWave;
	unsigned char m_LfoPitchModulationSensitivity;
	unsigned char m_Transpose;
	unsigned char m_VoiceName[10];
	TX816CommonParams() {
		memset(this, 0, sizeof(TX816CommonParams));
	}
};

struct TX816OperatorParams {
	unsigned char m_EgRate1;							// 0
	unsigned char m_EgRate2;							// 1
	unsigned char m_EgRate3;							// 2
	unsigned char m_EgRate4;							// 3
	unsigned char m_EgLevel1;							// 4
	unsigned char m_EgLevel2;							// 5
	unsigned char m_EgLevel3;							// 6
	unsigned char m_EgLevel4;							// 7
	unsigned char m_KeyboardLevelScalingBreakpoint;		// 8
	unsigned char m_KeyboardLevelScalingLeftDepth;		// 9
	unsigned char m_KeyboardLevelScalingRightDepth;		// 10
	unsigned char m_KeyboardLevelScalingLeftCurve;		// 11
	unsigned char m_KeyboardLevelScalingRightCurve;		// 12
	unsigned char m_KeyboardRateScaling;				// 13
	unsigned char m_AmplitudeModulationSensitivity;		// 14
	unsigned char m_KeyVelocitySensitivity;				// 15
	unsigned char m_OperatorOutputLevel;				// 16
	unsigned char m_OscillatorMode;						// 17
	unsigned char m_OscillatorFrequencyCoarse;			// 18
	unsigned char m_OscillatorFrequencyFine;			// 19
	unsigned char m_OscillatorDetune;								// 20
	TX816OperatorParams() {
		memset(this, 0, sizeof(TX816OperatorParams));
	}
};

struct TX816VoiceParams {
	TX816OperatorParams m_Operators[6];
	TX816CommonParams m_CommonOpParams;
	unsigned char m_OperatorOnOff;
	unsigned char m_OperatorSelect;
	//------------------------------------
	TX816VoiceParams() {
		m_OperatorOnOff = 0;
		m_OperatorSelect = 0;
	}
};

//-----------------------------
// Draw Modes
// ---------------------------


//----------------------------------------
// Handy Macros for drawing algorithms
// ---------------------------------------

constexpr auto DX7_OPERATOR_DIM = 40;
#define DX7_OPERATOR_SIZE	(CSize(DX7_OPERATOR_DIM,DX7_OPERATOR_DIM))
constexpr auto DX7_OP_TEXTOLOR = (RGB(0, 0, 0));
constexpr auto DX7_OP_BKCOLOR = (RGB(192, 48, 168));
constexpr auto DX7_OP_LINECOLOR = (RGB(192, 96, 48));
constexpr auto DX7_OP_COL_SPACING = (64);
constexpr auto DX7_OP_COL_BETWEEN = (DX7_OP_COL_SPACING - DX7_OPERATOR_DIM);
constexpr auto DX7_OP_COL_1 = (40);
constexpr auto DX7_OP_COL_2 = (DX7_OP_COL_1 + DX7_OP_COL_SPACING);
constexpr auto DX7_OP_COL_3 = (DX7_OP_COL_2 + DX7_OP_COL_SPACING);
constexpr auto DX7_OP_COL_4 = (DX7_OP_COL_3 + DX7_OP_COL_SPACING);
constexpr auto DX7_OP_COL_5 = (DX7_OP_COL_4 + DX7_OP_COL_SPACING);
constexpr auto DX7_OP_COL_6 = (DX7_OP_COL_5 + DX7_OP_COL_SPACING);
constexpr auto DX7_OP_ROW_SPACING = (58);
constexpr auto DX7_OP_ROW_BETWEEN = (DX7_OP_ROW_SPACING - DX7_OPERATOR_DIM);
constexpr auto DX7_OP_ROW_1 = (190);
constexpr auto DX7_OP_ROW_2 = (DX7_OP_ROW_1 - DX7_OP_ROW_SPACING);
constexpr auto DX7_OP_ROW_3 = (DX7_OP_ROW_2 - DX7_OP_ROW_SPACING);
constexpr auto DX7_OP_ROW_4 = (DX7_OP_ROW_3 - DX7_OP_ROW_SPACING);
constexpr auto DX7_COMMON_ROW = (DX7_OP_ROW_3);
constexpr auto DX7_PERFORMANCE_ROW = (DX7_COMMON_ROW - DX7_OP_ROW_SPACING);
constexpr auto DX7_COMMON_COL = (DX7_OP_COL_4);
constexpr auto DX7_OP_COL_CENTER_1 = (DX7_OP_COL_1 + (DX7_OPERATOR_DIM / 2));
constexpr auto DX7_OP_COL_CENTER_2 = (DX7_OP_COL_2 + (DX7_OPERATOR_DIM / 2));
constexpr auto DX7_OP_COL_CENTER_3 = (DX7_OP_COL_3 + (DX7_OPERATOR_DIM / 2));
constexpr auto DX7_OP_COL_CENTER_4 = (DX7_OP_COL_4 + (DX7_OPERATOR_DIM / 2));
constexpr auto DX7_OP_COL_CENTER_5 = (DX7_OP_COL_5 + (DX7_OPERATOR_DIM / 2));
constexpr auto DX7_OP_COL_CENTER_6 = (DX7_OP_COL_6 + (DX7_OPERATOR_DIM / 2));
constexpr auto DX7_OP_ROW_CENTER_1 = (DX7_OP_ROW_1 + (DX7_OPERATOR_DIM / 2));
constexpr auto DX7_OP_ROW_CENTER_2 = (DX7_OP_ROW_2 + (DX7_OPERATOR_DIM / 2));
constexpr auto DX7_OP_ROW_CENTER_3 = (DX7_OP_ROW_3 + (DX7_OPERATOR_DIM / 2));
constexpr auto DX7_OP_ROW_CENTER_4 = (DX7_OP_ROW_4 + (DX7_OPERATOR_DIM / 2));
constexpr auto DX7_OP_ROW_BOT_1 = (DX7_OP_ROW_1 + DX7_OPERATOR_DIM);
constexpr auto DX7_OP_ROW_BOT_2 = (DX7_OP_ROW_2 + DX7_OPERATOR_DIM);
constexpr auto DX7_OP_ROW_BOT_3 = (DX7_OP_ROW_3 + DX7_OPERATOR_DIM);
constexpr auto DX7_OP_ROW_BOT_4 = (DX7_OP_ROW_4 + DX7_OPERATOR_DIM);
constexpr auto DX7_OP_COL_RHSIDE_1 = (DX7_OP_COL_1 + DX7_OPERATOR_DIM);
constexpr auto DX7_OP_COL_RHSIDE_2 = (DX7_OP_COL_2 + DX7_OPERATOR_DIM);
constexpr auto DX7_OP_COL_RHSIDE_3 = (DX7_OP_COL_3 + DX7_OPERATOR_DIM);
constexpr auto DX7_OP_COL_RHSIDE_4 = (DX7_OP_COL_4 + DX7_OPERATOR_DIM);
constexpr auto DX7_OP_COL_RHSIDE_5 = (DX7_OP_COL_5 + DX7_OPERATOR_DIM);
constexpr auto DX7_OP_COL_RHSIDE_6 = (DX7_OP_COL_6 + DX7_OPERATOR_DIM);
constexpr auto DX7_OP_LINE_SPACING_X = (DX7_OP_COL_BETWEEN / 2);
constexpr auto DX7_OP_LINE_SPACING_Y = (DX7_OP_ROW_BETWEEN / 2);
constexpr auto DX7_OP_TEXTOFFSET_X = (6); //((DX7_OPERATOR_DIM / 2) - (DX7_OPERATOR_DIM / 4))
constexpr auto DX7_OP_TEXTOFFSET_Y = (0); //((DX7_OPERATOR_DIM / 8))

//---------------------------------------------------
// Misc defines
//---------------------------------------------------
constexpr auto DX7_VERTICAL_OFFSET = 10;
constexpr auto DX7_SLIDER_SPACING = (46);
constexpr auto DX7_ROW_1 = DX7_VERTICAL_OFFSET;
constexpr auto DX7_ROW_2 = DX7_VERTICAL_OFFSET + DX7_SLIDER_SPACING;
constexpr auto DX7_ROW_3 = DX7_VERTICAL_OFFSET + 2 * DX7_SLIDER_SPACING;
constexpr auto DX7_ROW_4 = DX7_VERTICAL_OFFSET + 3 * DX7_SLIDER_SPACING;
constexpr auto DX7_ROW_5 = DX7_VERTICAL_OFFSET + 4 * DX7_SLIDER_SPACING;
constexpr auto DX7_ROW_6 = DX7_VERTICAL_OFFSET + 5 * DX7_SLIDER_SPACING;
constexpr auto DX7_ROW_7 = DX7_VERTICAL_OFFSET + 6 * DX7_SLIDER_SPACING;

constexpr auto DX7_HORIZONTAL_OFFSET = 10;
constexpr auto DX7_SLIDER_HORZ_SPACING = 170;
constexpr auto DX7_SLIDER_COL_1 = DX7_HORIZONTAL_OFFSET;
constexpr auto	DX7_SLIDER_COL_2 = DX7_HORIZONTAL_OFFSET + DX7_SLIDER_HORZ_SPACING;
constexpr auto DX7_SLIDER_COL_3 = DX7_HORIZONTAL_OFFSET + 2 * DX7_SLIDER_HORZ_SPACING;
constexpr auto DX7_SLIDER_COL_4 = DX7_HORIZONTAL_OFFSET + 3 * DX7_SLIDER_HORZ_SPACING;

constexpr auto DX7_FREQ_DISPLAY_X = 416;
constexpr auto DX7_FREQ_DISPLAY_Y = 262;

//--------- Checkbox Stuff ----------------
constexpr auto DX7_CB_CONTROL_HEIGHT = 16;
constexpr auto DX7_CB_NL_CONTROL_WIDTH = 16;
constexpr auto DX7_CB_CONTROL_VERT_CENTER = DX7_SLIDER_SPACING/2 - DX7_CB_CONTROL_HEIGHT/2;
constexpr auto DX7_CB_SPACING = 40;
constexpr auto DX7_CB_EG_BIAS = 20;
constexpr auto DX7_CB_AMPLITUDE = DX7_CB_EG_BIAS + DX7_CB_SPACING;
constexpr auto DX7_CB_PITCH = DX7_CB_AMPLITUDE + DX7_CB_SPACING;
//----------------------------------------
// CChildViewYamahaTX816
//-----------------------------------------

class CChildViewYamahaTX816 : public CChildViewBase
{
	TX816VoiceParams m_VoiceEditBuffer;
	CMidiInDevice* m_pMidiIn;
	CMidiOutDevice* m_pMidiOut;
	int m_nCurrnetAlg;
	CFont m_fontOperator;
	CFont m_fontFrequency;
	CButton m_ButtonNext;
	CButton m_ButtonPrev;
	CButton m_ButtonHome;
	int m_DrawMode;
	//----------------------------------
	// Operator view controls
	//----------------------------------
	CMyOwnSlider m_sliderEGSeg1Rate;	//0->99
	CMyOwnSlider m_sliderEGSeg2Rate;	//0->99
	CMyOwnSlider m_sliderEGSeg3Rate;	//0->99
	CMyOwnSlider m_sliderEGSeg4Rate;	//0->99
	CMyOwnSlider m_sliderEGSeg1Level;	//0->99
	CMyOwnSlider m_sliderEGSeg2Level;	//0->99
	CMyOwnSlider m_sliderEGSeg3Level;	//0->99
	CMyOwnSlider m_sliderEGSeg4Level;	//0->99
	//-------- Keyboard Level/Rate Scaling ------------------
	CMyOwnSlider m_sliderKeyboardLevelScalingBreakPoint;	//0->99
	CMyOwnSlider m_sliderKeyboardLevelScalingLeftDepth;	//0->99
	CMyOwnSlider m_sliderKeyboardLevelScalingRightDepth;	//0->99
	CMyOwnSlider m_sliderKeyboardLevelScalingRightCurve;	//0->3
	CMyOwnSlider m_sliderKeyboardLevelScalingLeftCurve;	//0->3
	CMyOwnSlider m_sliderKeyboardRateScaling;	//0->7
	//------------ Misc --------------------------------
	CMyOwnSlider m_sliderAmplitudeModulationSensitivity;	//0->3
	CMyOwnSlider m_sliderKeyVelocitySensitivity;	//0->7
	CMyOwnSlider m_sliderOutputLevel;	//0->99
	//--------- Oscilator ------------------------
	CMyOwnCheckBox m_checkboxOscillatorMode;	//0->1
	CMyOwnSlider m_sliderOscilatorFrequencyCoarsep;	//0->31
	CMyOwnSlider m_sliderOscilatorFrequencyFine;	//0->99
	CMyOwnSlider m_sliderOscilatorDetune;			//0->14  7 = OnTune
	//-------------------------------------------
	// controls that are common to all operators
	//-------------------------------------------
	CMyOwnSlider m_sliderEGPitchRate_1;	//0->99
	CMyOwnSlider m_sliderEGPitchRate_2;	//0->99
	CMyOwnSlider m_sliderEGPitchRate_3;	//0->99
	CMyOwnSlider m_sliderEGPitchRate_4;	//0->99

	CMyOwnSlider m_sliderEGPitchLevel_1;	//0->99
	CMyOwnSlider m_sliderEGPitchLevel_2;	//0->99
	CMyOwnSlider m_sliderEGPitchLevel_3;	//0->99
	CMyOwnSlider m_sliderEGPitchLevel_4;	//0->99

//	CMyOwnSlider m_sliderAlgoRithmSelect;	//IS NOT REALLY NEEDED
	CMyOwnSlider m_sliderFeedback;	//0->7
	CMyOwnCheckBox m_checkOscillatorKeySync;	//true false
	CMyOwnSlider m_sliderLFOSpeed;	//0->99
	CMyOwnSlider m_sliderLFODelay;	//0->99
	CMyOwnSlider m_sliderLFOPitchModulationDepth;	//0->99
	CMyOwnSlider m_sliderLFOAmplitudeModulationDepth;	//0->9
	CMyOwnCheckBox m_checkboxLFOKeySync; ;//true false
	CMyOwnSlider m_sliderLFOWaveForm;	//0->5
	CMyOwnSlider m_sliderLFOPitchModulationSensitivity;	//0->7
	CMyOwnSlider m_sliderTranspose;	//0->48
	CEdit m_editVoiceName;	// 10 characters
	//--------------------------------------------
	// Performance controls
	//-------------------------------------------
	CMyOwnSlider m_sliderMidiChannelSlect;	// 1->16
	CMyOwnCheckBox m_checkPolyMono;	// mono poly
	CMyOwnSlider m_sliderPitchBendRange;	//0->12
	CMyOwnSlider m_sliderPitchBendStep;		//0->12
	CMyOwnSlider m_sliderPortamentoTime;	//0->99
	CMyOwnCheckBox m_checkPortamentoGlisando;	//on-off
	CMyOwnCheckBox m_checkPortamentoMode;		//retain follow

	CMyOwnSlider m_sliderModulationWheelSensitivity;	//0->15
	CMyOwnCheckBox m_checkModulationWheelAssignEGBias;	//on-off
	CMyOwnCheckBox m_checkModulationWheelAssignAmplitude;//on-off
	CMyOwnCheckBox m_checkModulationWheelAssignPitch;	//on-off

	CMyOwnSlider m_sliderFootControllerSensitivity;	//0->15
	CMyOwnCheckBox m_checkFootControllerAssignEGBias;	//on-off
	CMyOwnCheckBox m_checkFootControllerAssignAmplitude;	//on-off
	CMyOwnCheckBox m_checkFootControllerAssignPitch;	//on-off

	CMyOwnSlider m_sliderAfterTouchSensitivity;	//0->15
	CMyOwnCheckBox m_checkAfterTouchAssignEGBias;	//on-off
	CMyOwnCheckBox m_checkAfterTouchAssignAmplitude;	//on-off
	CMyOwnCheckBox m_checkAfterTouchAssignPitch;	//on-off

	CMyOwnSlider m_sliderBreathControllerSensitivity;	//0->15
	CMyOwnCheckBox m_checkBreathControllerAssignEGBias;	//on-off
	CMyOwnCheckBox m_checkBreathControllerAssignAmplitude;	//on-off
	CMyOwnCheckBox m_checkBreathControllerAssignPitch;	//on-off

	CMyOwnSlider m_sliderMasterTuning;	//0->127
	CEdit m_editPerformanceName;
	//-------------------------------------------
	CRect m_rectOperatpr[8];	//rects of the six operators
	//-----------------------------------
	DECLARE_DYNAMIC(CChildViewYamahaTX816)
public:
	CChildViewYamahaTX816();
	virtual ~CChildViewYamahaTX816();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMessageButtonHome();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMessageButtonNext();
	afx_msg void OnMessageButtonPrev();
	void OnInitialUpdate();
	void UpdateColors();
	void OnDraw(CDC* pDC);
	void DrawAlg(CDC* pDC, int alg);
	void DrawOperatorControls(CDC* pDC);
	void GetAlgRectangles(int alg);
	int MouseInOperatorRect(CPoint pt);
	void EnableOperatorControls(int enableflag);
	void DrawPerformanceControls(CDC* pDC);
	void EnablePerformanceControls(int enableflag);
	void EnableCommonControls(int enableflag);
	void DrawCommonControls(CDC* pDC);
	void InitializeOperatorControls(); 
	void InitializeCommonOperatorControls();
	void InitializePerformanceControls();
	void SetOperatorValues(int opIndex);
	void FreqToString(CString& oCS, int coarse, int fine, int mode);
	void DrawBMAlg(CDC* pDC, int x, int y);
	LRESULT OnMyOwnScroollbarMessage(WPARAM wp, LPARAM);
	int OperatorToIndex(int op);
	//------------------------------------------------
	// midi Functions
	//------------------------------------------------
	void TX816DumpRequst(int dumpType, int channel);
	//----------------------------
// control IDs for the
// editor pane
//-----------------------------

	//-----------------------------------------
	// Parameter Numbers
	//-----------------------------------------
	enum class DX7_ParameterNumbers{
		DX7_OP_PARAMETER_NUMBER_EG_SEGMENT_LEVEL_1,
		DX7_OP_PARAMETER_NUMBER_EG_SEGMENT_LEVEL_2,
		DX7_OP_PARAMETER_NUMBER_EG_SEGMENT_LEVEL_3,
		DX7_OP_PARAMETER_NUMBER_EG_SEGMENT_LEVEL_4,
		DX7_OP_PARAMETER_NUMBER_EG_SEGMENT_RATE_1,
		DX7_OP_PARAMETER_NUMBER_EG_SEGMENT_RATE_2,
		DX7_OP_PARAMETER_NUMBER_EG_SEGMENT_RATE_3,
		DX7_OP_PARAMETER_NUMBER_EG_SEGMENT_RATE_4,
		DX7_OP_PARAMETER_NUMBER_KEYBOARD_LEVEL_SCALING_BREAKPOINT,
		DX7_OP_PARAMETER_NUMBER_KEYBOARD_LEVEL_SCALING_LEFT_DEPTH,
		DX7_OP_PARAMETER_NUMBER_KEYBOARD_LEVEL_SCALING_RIGHT_DEPTH,
		DX7_OP_PARAMETER_NUMBER_KEYBOARD_LEVEL_SCALING_LEFT_CURVE,
		DX7_OP_PARAMETER_NUMBER_KEYBOARD_LEVEL_SCALING_RIGHT_CURVE,
		DX7_OP_PARAMETER_NUMBER_KEYBOARD_RATE_SCALING,
		DX7_OP_PARAMETER_NUMBER_AMPLITUDE_MODULATION_SENSITIVITY,
		DX7_OP_PARAMETER_NUMBER_KEY_VELOCITY_SENSITIVITY,
		DX7_OP_PARAMETER_NUMBER_OPERATOR_OUTPUT_LEVEL,
		DX7_OP_PARAMETER_NUMBER_OSCILLATOR_MODE,
		DX7_OP_PARAMETER_NUMBER_OSCILLATOR_FREQUENCY_COARSE,
		DX7_OP_PARAMETER_NUMBER_OSCILLATOR_FREQUENCY_FINE,
		DX7_OP_PARAMETER_NUMBER_OSCILLATOR_DETUNE
	};
	//-----------------------------
	// Operation codes for Drawing
	// The algorithm diagram
	//-----------------------------



	enum class Algorithms {
		DX7_ALG_1,
		DX7_ALG_2,
		DX7_ALG_3,
		DX7_ALG_4,
		DX7_ALG_5,
		DX7_ALG_6,
		DX7_ALG_7,
		DX7_ALG_8,
		DX7_ALG_9,
		DX7_ALG_10,
		DX7_ALG_11,
		DX7_ALG_12,
		DX7_ALG_13,
		DX7_ALG_14,
		DX7_ALG_15,
		DX7_ALG_16,
		DX7_ALG_17,
		DX7_ALG_18,
		DX7_ALG_19,
		DX7_ALG_20,
		DX7_ALG_21,
		DX7_ALG_22,
		DX7_ALG_23,
		DX7_ALG_24,
		DX7_ALG_25,
		DX7_ALG_26,
		DX7_ALG_27,
		DX7_ALG_28,
		DX7_ALG_29,
		DX7_ALG_30,
		DX7_ALG_31,
		DX7_ALG_32
	};


	enum class DX7_OperatorIndexValue {
		DX7_INDEX_OP_6,
		DX7_INDEX_OP_5,
		DX7_INDEX_OP_4,
		DX7_INDEX_OP_3,
		DX7_INDEX_OP_2,
		DX7_INDEX_OP_1
	};
};


