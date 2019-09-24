#ifndef PISVC_DESC_H
#define PISVC_DESC_H
//-----------------------------------------------------------------------------------
// ***** PISERVICE_ALIGNAS
//-----------------------------------------------------------------------------------
#if !defined(PISERVICE_ALIGNAS)
#if defined(__GNUC__) || defined(__clang__)
#define PISERVICE_ALIGNAS(n) __attribute__((aligned(n)))
#elif defined(_MSC_VER) || defined(__INTEL_COMPILER)
#define PISERVICE_ALIGNAS(n) __declspec(align(n))
#elif defined(__CC_ARM)
#define PISERVICE_ALIGNAS(n) __align(n)
#else
#error Need to define PISERVICE_ALIGNAS
#endif
#endif

#if !defined(SVC_EXTERN_C)
#ifdef __cplusplus
#define SVC_EXTERN_C extern "C"
#else
#define SVC_EXTERN_C
#endif
#endif

#ifndef SVC_DLL
#define SVC_DLL __declspec(dllimport)
#else
// SVC_DLL had defined for dllexport
#endif

#include "piSvcType.h"

/************************************************************************/
/* DESC                                                                 */
/************************************************************************/

typedef struct PISERVICE_ALIGNAS(8) piSvcHmdInfo_
{
	char ProductName[64];
	char Manufacturer[64];

	// SN&lcd type、version
	char        SerialNumber[64];
	char        version_generation[64];
	int         version_major, version_Minor;
	char DisplayPiSvcHmdiceName[64];

}piSvcHmdInfo;

typedef struct PISERVICE_ALIGNAS(8) piSvcControllerInfo_
{
	char ProductName[64];
	char Manufacturer[64];

	// SN、type、version
	char SerialNumber[64];
	char  version_generation[64];
	int   version_major, version_Minor;
	char DisplayPiSvcControllericeName[64];

}piSvcControllerInfo;

typedef struct PISERVICE_ALIGNAS(8) piSvcTrackingInfo_
{
	char ProductName[64];
	char Manufacturer[64];

	// SN、type、version
	char  SerialNumber[64];
	char  version_generation[64];
	int   version_major, version_Minor;
	char DisplayPiSvcTrackingiceName[64];

}piSvcTrackingInfo;

typedef struct PISERVICE_ALIGNAS(4) piVector3f_
{
	float x, y, z;
} piVector3f;


typedef void* piSvcHandle;

#endif


