#ifndef PISVC_CAPABILITY_H
#define PISVC_CAPABILITY_H

#include "piSvcType.h"

typedef enum piHmdCapabilityMask_
{
    // Read-only flags.
	piHmdCap_ModeCtrl = 0x00000001,
	piHmdCap_BackLightCtrl = 0x00000002,
	piHmdCap_ResolutionCtrl = 0x00000004,
	piHmdCap_ToneCtrl = 0x00000008,
	piHmdCap_BrightCtrl = 0x00000010,
	piHmdCap_IpdAdjust = 0x00000020,
	piHmdCap_BlueLightCtrl = 0x00000040,
    piHmdCap_ProximityCtrl = 0x00000080,
	piHmdCap_GyroCtrl = 0x00000100,	/// 
	piHmdCap_EyePoseCtrl = 0x00000200,
	piHmdCap_AudioCtrl = 0x00000400,
	piHmdCap_DisplayQualityCtrl = 0x00000800,
	piHmdCap_Lens_SeparationCtrl = 0x00001000,
	piHmdCap_RebootCtrl = 0x00008000,
	piHmdCap_BussinessFlagCtrl = 0x00010000,
	piHmdCap_Unknow = 0x00000000,
}piHmdCapabilityMask;

typedef enum Mode_Type_ {
	mode_State_pimax_smart = 0x0001,
	mode_State_video_p = 0x0004,
	mode_State_dfu = 0x0008,

	mode_State_unknow = 0x0,
}Mode_Type;

typedef enum piResolutionCapabilityFlag_
{
	e_1080_1920 = 0x01,
	e_1440_2560 = 0x02,
	e_1200_2160 = 0x04,
	e_2160_3840 = 0x08,
	e_unknow
}piResolutionCapabilityFlag;

typedef enum ToneState_
{
	TONE_WARM = 0x00,
	TONE_COOL = 0x01,
	TONE_UNKNOW
}ToneState;

typedef enum BrightState_
{
	LEVEL_0,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	LEVEL_4,
	LEVEL_5,
	LEVEL_6 = 6,
	LEVEL_7,
	LEVEL_8 = 8,
	LEVEL_9,
	LEVEL_10 = 10,
	LEVEL_11,
    LEVEL_12 = 12,

	LEVEL_INVALID = -1
}BrightState;

//tracked device.
typedef enum TrackedDeviceType_
{
	TrackedDevice_UNKNOW = 0x0000,
	TrackedDevice_HMD = 0x0001,
	TrackedDevice_LeftController = 0x0002,
	TrackedDevice_RightController = 0x0004,
	TrackedDevice_Tracker0 = 0x0008,
	TrackedDevice_Tracker1 = 0x0010,
} TrackedDeviceType;

typedef enum TrackedDeviceBateryLevel_
{
	TrackedDeviceBateryLevel_NotSupport = 0,
	TrackedDeviceBateryLevel_Low,
	TrackedDeviceBateryLevel_Middle,
	TrackedDeviceBateryLevel_High,
	TrackedDeviceBateryLevel_Max
}TrackedDeviceBateryLevel;

typedef enum LogLevel_
{
	LOG_NONE = 0, //output nothing
	LOG_NORMAL, 
	LOG_DEBUG,
}LogLevel;

#endif


