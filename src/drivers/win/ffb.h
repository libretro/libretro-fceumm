#ifndef _DXINPUT_H_
#define _DXINPUT_H_

#define kMAX_Str                80
#define kMAX_Axis                        8
#define kMAX_Sliders                2
#define kMAX_POVs                        4

#define kInfinite_Duration 0xFFFFFF
#define HZ_to_uS(hz)                ((int)(1000000.0 / (double)(hz) + 0.5))

typedef signed char tSC;
typedef signed int tSI;
typedef signed long tSL;

typedef unsigned char tUC;
typedef unsigned int tUI;
typedef unsigned long tUL;

typedef enum {
	kJoy1 = 0,
	kJoy2,
	kJoy3,
	kJoy4,
	kJoy5,
	kJoy6,
	kJoy7,
	kJoy8,
	kJoy9,
	kJoy10,
	kJoy11,
	kJoy12,
	kJoy13,
	kJoy14,
	kJoy15,
	kJoy16,
	kMaxJoy,
	kMouse,
	kKeyBoard,
	kAllDevices
} tDevice;

typedef enum {
	kNoButton = -1,
	kButton0 = 0,
	kButton1,
	kButton2,
	kButton3,
	kButton4,
	kButton5,
	kButton6,
	kButton7,
	kButton8,
	kButton9,
	kButton10,
	kButton31 = 31,
	kButtonMax
} tJoyButtons;

typedef enum {
	kStick = 1 << 0,
	kWheel = 1 << 1,
	kGamePad = 1 << 2,
} tDeviceMask;

typedef struct {
	int ButtonMask;
	int AxisMask;
	tDeviceMask DevType;
	char Name[kMAX_Str];
} tJoyInfo;

typedef struct {
	int ButtonMask;
	tSL X_pos;
	tSL Y_pos;
	tSL Z_pos;

	tSL X_rot;
	tSL Y_rot;
	tSL Z_rot;

	tSL Slide_pos[kMAX_Sliders];
	tUL POV_dir[kMAX_POVs];
} tJoyData;

typedef enum {
	kDontPlayNow = 0,
	kPlayNow,
	kPlayNowIfModified,
} tLoadEffect;

typedef enum {
	kConstant,
	kRamp,
	kCustom,

	kWave_Square,
	kWave_Sine,
	kWave_Triange,
	kWave_SawUp,
	kWave_SawDown,

	kCondition_Spring,
	kCondition_Damper,
	kCondition_Inertia,
	kCondition_Friction,

	kMaxEffectSubTypes
} tEffType;

typedef struct tEffConstant {
	long Mag;
} tEffConstant;

typedef struct tEffRamp {
	long Start;
	long End;
} tEffRamp;

typedef struct tEffWave {
	int Mag;
	long Offset;
	int Phase;
	int Period;
} tEffWave;

typedef struct tEffCondition {
	long Offset;
	long PositiveCoefficient;
	long NegativeCoefficient;
	int PositiveSaturation;
	int NegativeSaturation;
	long DeadBand;
} tEffCondition;

typedef struct tEffCustom {
	int Channels;
	int Period;
	int Samples;
	long *ForceData;
} tEffCustom;

typedef union tEffectInfo {
	tEffConstant Constant;
	tEffRamp Ramp;
	tEffWave Wave;
	tEffCondition Condition;
	tEffCustom Custom;
} tEffInfo;

typedef struct tEffectEnvelope {
	int AttackLevel;
	int AttackTime;
	int FadeLevel;
	int FadeTime;
} tEffEnvelope;

typedef struct tFFB_Effect {
	tEffType Type;
	tEffInfo TypeInfo;
	int Duration;
	int Period;
	int Gain;
	tJoyButtons Trigger;
	int TriggerRepeatTime;
	long Direction;
	tEffEnvelope Envelope;
} tFFB_Effect;


// ===================================================================
//                                                                        Function Prototypes
// ===================================================================
// Init
// ----
int            ffb_Init(void);

// Use
// ---

void        ffb_Pause(short dev);
void        ffb_Continue(short dev);

void        ffb_Enable(short dev);
void        ffb_Disable(short dev);

int            ffb_effectCreate(short dev, tFFB_Effect* eff);
void        ffb_effectPlay(short effectID);
void        ffb_effectStop(short effectID);
void        ffb_effectStopAll(short dev);
void        ffb_effectModify(short effectID, int*        Direction, int* Duration, int* Gain,
							 int* Period, tEffInfo* TypeInfo, tEffEnvelope* Envelope);

#endif	// _DXINPUT_H_