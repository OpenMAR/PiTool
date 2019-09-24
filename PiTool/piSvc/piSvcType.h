#ifndef PISVC_TYPE_H
#define PISVC_TYPE_H

typedef enum piSvcResult_
{
	svc_success = 0,
	svc_failed = -1,

	svc_not_initialize = 100,
	svc_not_ready,
	svc_usb_no_connect,
	svc_hdmi_no_connect,
	svc_handle_controller_no_connect,
	svc_hid_failed,
	svc_driver_failed,// maybe i2c suspend
	svc_firmware_failed,
	svc_path_not_found,
	svc_state_not_connected,
	svc_capability_no_support,
	svc_param_error,
	svc_timeout,
	svc_dll_failed,
	svc_rpc_failed,
	svc_mode_no_support,
	svc_interface_no_support,
	svc_no_completed,
	svc_no_sp1,
	svc_gpu_version_blacklist,

	svc_service_failed = 400,
	svc_service_no_start,
	svc_service_display_lost,
	svc_service_init_failed,
	svc_service_create_session_failed,
	svc_service_should_quit,

	svc_unknow,
}piSvcResult;	


typedef enum piServiceMessage_
{
	SERVICE_USB,
	SERVICE_HDMI,
	SERVICE_SERVER_STATE,
	SERVICE_SERVER_NOT_START,
	SERVICE_DFU_DOWNLOAD_START,
	SERVICE_DFU_DOWNLOAD_COMPLETED,
	SERVICE_DFU_DRIVER_NOT_FOUND,
	SERVICE_UNKNOW,
}piServiceMessage;

typedef void* piSvcHandle;
typedef int piWPARAM, piLPARAM;
typedef void(__stdcall *callback_function)(piServiceMessage,piWPARAM,piLPARAM);

#endif


