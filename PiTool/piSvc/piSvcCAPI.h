#ifndef PISVC_CAPI_H
#define PISVC_CAPI_H

#include "piSvcDesc.h"
#include "piSvcCapability.h"

#define PIMAX_PUBLIC_FUNCTION SVC_EXTERN_C SVC_DLL piSvcResult

//svc_start must be called first.
//No other functions can call before svc_start succeeds or after svc_shutdown.
PIMAX_PUBLIC_FUNCTION svc_start(piSvcHandle* handle, callback_function fun);
PIMAX_PUBLIC_FUNCTION svc_shutdown(piSvcHandle handle);
PIMAX_PUBLIC_FUNCTION svc_getSvcHmdDesc(piSvcHandle handle, piSvcHmdInfo* hmdInfo);
//caps represent all the capabilitys of connected hmd, each capacity value corresponds to the piHmdCapabilityMask
PIMAX_PUBLIC_FUNCTION svc_getHmdCaps(piSvcHandle handle, int* caps);
PIMAX_PUBLIC_FUNCTION svc_getResolutionCaps(piSvcHandle handle, int* resolutionCaps);
PIMAX_PUBLIC_FUNCTION svc_getCurrentResolution(piSvcHandle handle, piResolutionCapabilityFlag* resolution);
PIMAX_PUBLIC_FUNCTION svc_setResolution(piSvcHandle handle, piResolutionCapabilityFlag resolution);
PIMAX_PUBLIC_FUNCTION svc_getUsbState(piSvcHandle handle);
PIMAX_PUBLIC_FUNCTION svc_getHdmiState(piSvcHandle handle);
//devices represent all connected TrackedDeviceType devices
PIMAX_PUBLIC_FUNCTION svc_getConnectedDevices(piSvcHandle handle, int* devices);
PIMAX_PUBLIC_FUNCTION svc_getTrackerInfo(piSvcHandle handle, TrackedDeviceType trackerType, char* info, int size);
PIMAX_PUBLIC_FUNCTION svc_getDeviceBatteryLevel(piSvcHandle handle, TrackedDeviceType device, TrackedDeviceBateryLevel* level);
PIMAX_PUBLIC_FUNCTION svc_getDeviceBatteryPercent(piSvcHandle handle, TrackedDeviceType device, int* percent/*percent is 0 to 100*/);
PIMAX_PUBLIC_FUNCTION svc_getServiceConnection(piSvcHandle handle, Mode_Type* mode);
PIMAX_PUBLIC_FUNCTION svc_startFirmwareUpgrade(piSvcHandle handle, char* dfuPath);
PIMAX_PUBLIC_FUNCTION svc_changeMode(piSvcHandle handle, Mode_Type mode);
PIMAX_PUBLIC_FUNCTION svc_getModeCaps(piSvcHandle hanlde, int* caps);
//restart current server
PIMAX_PUBLIC_FUNCTION svc_reconnectServer(piSvcHandle handle);
PIMAX_PUBLIC_FUNCTION svc_rebootHmd(piSvcHandle handle);
PIMAX_PUBLIC_FUNCTION svc_getToneState(piSvcHandle handle, ToneState* tone);
PIMAX_PUBLIC_FUNCTION svc_setToneState(piSvcHandle handle, ToneState tone);
PIMAX_PUBLIC_FUNCTION svc_getBrightState(piSvcHandle handle, BrightState* bright);
PIMAX_PUBLIC_FUNCTION svc_setBrightState(piSvcHandle handle, BrightState bright);
PIMAX_PUBLIC_FUNCTION svc_getBlueLightState(piSvcHandle handle, bool* enable);
PIMAX_PUBLIC_FUNCTION svc_enableBlueLight(piSvcHandle handle, bool enable);
PIMAX_PUBLIC_FUNCTION svc_getBackLightState(piSvcHandle handle, bool* enable);
PIMAX_PUBLIC_FUNCTION svc_enableBackLight(piSvcHandle handle, bool enable);
PIMAX_PUBLIC_FUNCTION svc_getProximityState(piSvcHandle handle, bool* enable);
PIMAX_PUBLIC_FUNCTION svc_enableProximity(piSvcHandle handle, bool enable);
PIMAX_PUBLIC_FUNCTION svc_getGyroState(piSvcHandle handle, bool* enable);
PIMAX_PUBLIC_FUNCTION svc_enableGyro(piSvcHandle handle, bool enable);
PIMAX_PUBLIC_FUNCTION svc_getIpdValue(piSvcHandle handle, float* value);
PIMAX_PUBLIC_FUNCTION svc_setIpdValue(piSvcHandle handle, float value);
PIMAX_PUBLIC_FUNCTION svc_getLensValue(piSvcHandle handle, float* value);
PIMAX_PUBLIC_FUNCTION svc_setLensValue(piSvcHandle handle, float value);
PIMAX_PUBLIC_FUNCTION svc_getDisplayQualityValue(piSvcHandle handle, float* value);
PIMAX_PUBLIC_FUNCTION svc_setDisplayQualityValue(piSvcHandle handle, float value);
PIMAX_PUBLIC_FUNCTION svc_resetEyePose(piSvcHandle handle);
PIMAX_PUBLIC_FUNCTION svc_getPimaxDefaultAudioConfig(piSvcHandle handle, bool* enable);
PIMAX_PUBLIC_FUNCTION svc_setPimaxDefaultAudioDevice(piSvcHandle handle,bool enable);
PIMAX_PUBLIC_FUNCTION svc_resetFactorySettings(piSvcHandle handle);
PIMAX_PUBLIC_FUNCTION svc_slopeCalibrationHmd(piSvcHandle handle);
PIMAX_PUBLIC_FUNCTION svc_setIntConfig(piSvcHandle handle, const char* key, int value);
PIMAX_PUBLIC_FUNCTION svc_getIntConfig(piSvcHandle handle, const char* key, int* value);
PIMAX_PUBLIC_FUNCTION svc_setFloatConfig(piSvcHandle handle, const char* key, float value);
PIMAX_PUBLIC_FUNCTION svc_getFloatConfig(piSvcHandle handle, const char* key, float* value);
PIMAX_PUBLIC_FUNCTION svc_setStringConfig(piSvcHandle handle, const char* key, const char* value);
PIMAX_PUBLIC_FUNCTION svc_getStringConfig(piSvcHandle handle, const char* key, char* value, int size);
PIMAX_PUBLIC_FUNCTION svc_getVector3fConfig(piSvcHandle handle, const char* key, piVector3f* value);
PIMAX_PUBLIC_FUNCTION svc_setVector3fConfig(piSvcHandle handle, const char* key, piVector3f value);
PIMAX_PUBLIC_FUNCTION svc_getHmdPosition(piSvcHandle handle, float* x, float* y, float* z);
/**********************************************
start calibrate magnetism.
then call svc_magnetismCalibrationed() 
function to check succussful
**********************************************/
PIMAX_PUBLIC_FUNCTION svc_magnetismCalibrationHmd(piSvcHandle handle);
PIMAX_PUBLIC_FUNCTION svc_magnetismCalibrationed(piSvcHandle handle);

PIMAX_PUBLIC_FUNCTION svc_getInterfaceVersion(piSvcHandle handle, int* major, int* minor);
PIMAX_PUBLIC_FUNCTION svc_getServiceVersion(piSvcHandle handle, int* major, int* minor);
PIMAX_PUBLIC_FUNCTION svc_getServiceLogLevel(piSvcHandle handle, LogLevel* level);
PIMAX_PUBLIC_FUNCTION svc_setServiceLogLevel(piSvcHandle handle, LogLevel level);
#endif


