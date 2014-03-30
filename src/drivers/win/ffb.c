#include "ffb.h"
#include <dinput.h>

#define MAX_JOYSTICKS   32
#define kDI_MaxEffects  30		// Hard coded limit (not a DX limit)

extern LPDIRECTINPUTDEVICE7 Joysticks[MAX_JOYSTICKS];

// Force Feedback Effect Data
// --------------------------
REFGUID effectGUID[kMaxEffectSubTypes] =
{
	&GUID_ConstantForce,
	&GUID_RampForce,
	&GUID_CustomForce,
	// period
	&GUID_Square,
	&GUID_Sine,
	&GUID_Triangle,
	&GUID_SawtoothUp,
	&GUID_SawtoothDown,
	// condition
	&GUID_Spring,
	&GUID_Damper,
	&GUID_Inertia,
	&GUID_Friction
};

typedef union {
	DICONSTANTFORCE constant;
	DIRAMPFORCE ramp;
	DIPERIODIC period;
	DICONDITION condition;
	DICUSTOMFORCE custom;
} tEffectClasses;

typedef struct tEffect {
	DIEFFECT general;
	tEffectClasses specific;
	DIENVELOPE envelope;
	LONG direction[2];
} tEffect;

static tEffect Effect[kDI_MaxEffects];

// DInput Data
// -----------
static LPDIRECTINPUTEFFECT DIE_hEffect[kDI_MaxEffects];
static int numEffects = 0;

// ---------
// ffb_Pause
// -------------------------------------------------------------------
// Purpose:
//    Pause the FFB output on the given device.  Use ffb_Continue to
//    continue where you left off.
// -------------------------------------------------------------------
void
ffb_Pause(short dev) {
	IDirectInputDevice7_SendForceFeedbackCommand(Joysticks[dev], DISFFC_PAUSE);
}

// ------------
// ffb_Continue
// -------------------------------------------------------------------
// Purpose:
//    Unpause the FFB output on the given device.  Complimentary to
//    ffb_Pause.
// -------------------------------------------------------------------
void
ffb_Continue(short dev) {
	IDirectInputDevice7_SendForceFeedbackCommand(Joysticks[dev], DISFFC_CONTINUE);
}

// ----------
// ffb_Enable
// -------------------------------------------------------------------
// Purpose:
//    Must be called after initialization in order to activate the
//    device.
//    Use ffb_Pause & ffb_Continue if you want disable forces
//    temporarily and resume later.
// -------------------------------------------------------------------
void
ffb_Enable(short dev) {
	IDirectInputDevice7_SendForceFeedbackCommand(Joysticks[dev], DISFFC_SETACTUATORSON);
}

// -----------
// ffb_Disable
// -------------------------------------------------------------------
// Purpose:
//    Turns off FFB, but effects still play on processor.
// -------------------------------------------------------------------
void
ffb_Disable(short dev) {
	IDirectInputDevice7_SendForceFeedbackCommand(Joysticks[dev], DISFFC_SETACTUATORSOFF);
}

void
ffb_simpleJolt(short dev) {
	tFFB_Effect effect;
	short eID;
	// Constant Effect (simple jolt)
	// -----------------------------
	effect.Type = kConstant;
	effect.TypeInfo.Constant.Mag = 10000;
	effect.Duration = 1000000 >> 1;				// half second
	effect.Period = 1000;
	effect.Gain = 10000;
	effect.Trigger = kNoButton;
	effect.TriggerRepeatTime = 0;
	effect.Direction = 0;

	eID = ffb_effectCreate(dev, &effect);
}

// ----------------
// ffb_effectCreate
// -------------------------------------------------------------------
// Purpose:
//    Create a single effect for future playback.
//    Effect is given a logical ID
// -------------------------------------------------------------------
int
ffb_effectCreate(short dev, tFFB_Effect* eff) {
	HRESULT hr;

	// Important stuff
	Effect[numEffects].general.dwDuration = eff->Duration;
	Effect[numEffects].general.dwSamplePeriod = eff->Period;
	Effect[numEffects].general.dwGain = eff->Gain;
	Effect[numEffects].general.dwTriggerButton = (eff->Trigger == kNoButton ? DIEB_NOTRIGGER : DIJOFS_BUTTON(eff->Trigger));
	Effect[numEffects].general.dwTriggerRepeatInterval = eff->TriggerRepeatTime;
	Effect[numEffects].direction[0] = eff->Direction;
	memcpy(&Effect[numEffects].specific, &eff->TypeInfo, sizeof(tEffectClasses));

	// Wacky COM related overhead
	Effect[numEffects].general.dwSize = sizeof(DIEFFECT);
	Effect[numEffects].general.cAxes = 2;
	Effect[numEffects].general.rgdwAxes = 0;
	Effect[numEffects].general.dwFlags = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
	Effect[numEffects].general.lpvTypeSpecificParams = &Effect[numEffects].specific;
	Effect[numEffects].general.rglDirection = &Effect[numEffects].direction;
	Effect[numEffects].direction[1] = 0L;
	Effect[numEffects].general.lpEnvelope = NULL;
	Effect[numEffects].general.lpvTypeSpecificParams = &Effect[numEffects].specific;


	switch (eff->Type) {
	case kConstant:
		Effect[numEffects].general.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		break;
	case kRamp:
		Effect[numEffects].general.cbTypeSpecificParams = sizeof(DIRAMPFORCE);
		break;
	case kCustom:
		Effect[numEffects].general.cbTypeSpecificParams = sizeof(DICUSTOMFORCE);
		break;
	case kWave_Square:
	case kWave_Sine:
	case kWave_Triange:
	case kWave_SawUp:
	case kWave_SawDown:
		Effect[numEffects].general.cbTypeSpecificParams = sizeof(DIPERIODIC);
		break;
	case kCondition_Spring:
	case kCondition_Damper:
	case kCondition_Inertia:
	case kCondition_Friction:
		Effect[numEffects].general.cbTypeSpecificParams = sizeof(DICONDITION);
		break;
	default:
		return(-1);
	}


	hr = IDirectInputDevice7_CreateEffect(Joysticks[dev],
										  effectGUID[eff->Type],
										  (LPCDIEFFECT)&Effect[numEffects],
										  (LPDIRECTINPUTEFFECT*)&DIE_hEffect[numEffects], 0);
	switch (hr) {
	case DI_OK:
		ffb_effectUnload(numEffects);
		++numEffects;
		return(numEffects - 1);

	case DIERR_DEVICENOTREG:
		return(-1);

	case DIERR_DEVICEFULL:
		return(-1);
	}
	return(-1);
}

// --------------
// ffb_effectPlay
// -------------------------------------------------------------------
// Purpose:
//    Play an effect that was previously created.
// -------------------------------------------------------------------
void
ffb_effectPlay(short eID) {
	IDirectInputEffect_Start(DIE_hEffect[eID], 1, 0);
}

// --------------
// ffb_effectStop
// -------------------------------------------------------------------
// Purpose:
//    Stop a single effect.
// -------------------------------------------------------------------
void
ffb_effectStop(short eID) {
	IDirectInputEffect_Stop(DIE_hEffect[eID]);
}

// -----------------
// ffb_effectStopAll
// -------------------------------------------------------------------
// Purpose:
//    Stops all forces on the given device.
// -------------------------------------------------------------------
void
ffb_effectStopAll(short dev) {
	IDirectInputDevice7_SendForceFeedbackCommand(Joysticks[dev], DISFFC_STOPALL);
}

// ----------------
// ffb_effectUnload
// -------------------------------------------------------------------
// Purpose:
//    Unload a single effect...  Necessary to make room for other
//    effects.
// -------------------------------------------------------------------
void
ffb_effectUnload(short eID) {
	IDirectInputEffect_Unload(DIE_hEffect[eID]);
}


// ----------------
// ffb_effectModify
// -------------------------------------------------------------------
// Purpose:
//    Modifies a single effect, only if the given parameters are
//    different from what's currently loaded.
// -------------------------------------------------------------------
void
ffb_effectModify(short eID, int*        Direction, int* Duration, int* Gain,
				 int* Period, tEffInfo* TypeInfo, tEffEnvelope* Envelope) {
	int flags = 0;

	return;
	if (Direction) {
		if (Effect[eID].direction[0] != *Direction) {
			Effect[eID].direction[0] = *Direction;
			flags |= DIEP_DIRECTION;
		}
	}
	if (Duration) {
		if (Effect[eID].general.dwDuration != *Duration) {
			Effect[eID].general.dwDuration = *Duration;
			flags |= DIEP_DURATION;
		}
	}
	if (Gain) {
		if (Effect[eID].general.dwGain != *Gain) {
			Effect[eID].general.dwGain = *Gain;
			flags |= DIEP_GAIN;
		}
	}
	if (Period) {
		if (Effect[eID].general.dwSamplePeriod != *Period) {
			Effect[eID].general.dwSamplePeriod = *Period;
			flags |= DIEP_SAMPLEPERIOD;
		}
	}
	if (TypeInfo) {
		if (!memcmp(&Effect[eID].specific, TypeInfo, Effect[eID].general.cbTypeSpecificParams)) {
			memcpy(&Effect[eID].specific, TypeInfo, Effect[eID].general.cbTypeSpecificParams);
			flags |= DIEP_TYPESPECIFICPARAMS;
		}
	}
	if (Envelope) {
		if (Effect[eID].envelope.dwAttackLevel != Envelope->AttackLevel ||
			Effect[eID].envelope.dwAttackTime != Envelope->AttackTime ||
			Effect[eID].envelope.dwFadeLevel != Envelope->FadeLevel ||
			Effect[eID].envelope.dwFadeTime != Envelope->FadeTime) {
			Effect[eID].envelope.dwAttackLevel = Envelope->AttackLevel;
			Effect[eID].envelope.dwAttackTime = Envelope->AttackTime;
			Effect[eID].envelope.dwFadeLevel = Envelope->FadeLevel;
			Effect[eID].envelope.dwFadeTime = Envelope->FadeTime;
			flags |= DIEP_ENVELOPE;
		}
	}

	if (!flags)
		return;

	IDirectInputEffect_SetParameters(DIE_hEffect[eID], (LPCDIEFFECT)&Effect[eID], flags);
}
