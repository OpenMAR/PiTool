#include "piservice.h"
#include <QProcess>
#include <QDebug>
#include <QFile>
#include "piSvc\\piSvcCAPI.h"
#include "windows.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include "hwObject.h"
#include <QStandardPaths>
#include "PiLog/WinCommon.h"
#include "RegisterHandler.h"
#include <thread>
#include <QDir>

#define REG_PITOOL_PATH "HKEY_CURRENT_USER\\SOFTWARE\\PiTool"
#define REG_KEY_AUDIO "audio"
#define REG_KEY_GYRO "gyro"
#define BRIGHTLEVEL_LEN 4
BrightState brightLevelArrayFor5K[BRIGHTLEVEL_LEN] = {LEVEL_12,LEVEL_10,LEVEL_8};
BrightState brightLevelArrayFor8K[BRIGHTLEVEL_LEN] = {LEVEL_10,LEVEL_8,LEVEL_6};

PiService *gInstance=nullptr;
#define RET_RESULT if(m_nResult==svc_success){return true;}else{return false;}
PiService:: PiService(QObject *parent) : QObject(parent)
{
    qCritical()<<"PiService";
    gInstance = this;
    mSpaceCalibrated = true;
    mHeadTracked = false;
    mController0Tracked = false;
    mController1Tracked = false;
    mBase0Tracked = false;
    mBase1Tracked = false;
    controllerPairSupport = false;
    pairStatus = Paired_Unknow;
    pairIndex = 0;
    leapMotionPlugin = false;
    leapMotionInstalled = false;
    leapMotionServiceReady = false;
    leapMotionDeviceReady = false;
    lighthouseEnabled = false;
    compatibleSteamvrUsingParallel = false;
    fovType = 0;
    hiddenAreanEnabled = false;
    m_Handle = nullptr;
    mbConnected = false;
    mbUsbConnect = false;
    mHmdInfoAvaliable = false;
    mForceUpgrading = false;
    mForcePath = "";
    mbServiceConnect = false;
    mbHdmiConnect = false;
    mControllerState = false;
    mControllerName = "";
    mTrackedDeviceState = 0;
    mController1Battery = 0;
    mController2Battery = 0;
    mTracker1Battery = 0;
    mTracker2Battery = 0;
    recenterSupported = false;
    mControllerPlugin = false;
    mSwitchingMode = false;
    mDisplayQuality = 1.0;
    mIpd = 63.0;
    mToneState =TONE_UNKNOW;
    pvrHomeEnabled = RegisterHandler::getInstance().getRegisterValue(REG_PITOOL_PATH, "pvr_Home_Enabled", 0);
    mAudioPimax = RegisterHandler::getInstance().getRegisterValue(REG_PITOOL_PATH,REG_KEY_AUDIO,1)==1;
    mEnableGyro = RegisterHandler::getInstance().getRegisterValue(REG_PITOOL_PATH,REG_KEY_GYRO,0)==1;
    mMinBrightLevel = RegisterHandler::getInstance().getRegisterValue(REG_PITOOL_PATH,"min_bright",LEVEL_1);
    mMaxBrightLevel = RegisterHandler::getInstance().getRegisterValue(REG_PITOOL_PATH,"max_bright",LEVEL_3);
    mNoloDisplayMode = RegisterHandler::getInstance().getRegisterValue(REG_PITOOL_PATH,"nolo_display_mode",0);

    setModeType(0);
    setEnableGyro(false);
    setToneState(TONE_UNKNOW);
    setBrightState(LEVEL_INVALID);
    setResolution(e_unknow);
    setDisplayQuality(1);

    mNoDfuDriver = false;
    m_errorCode = NONE;
    if(!WinCommon::processAlived("PiServiceLauncher.exe")){
        qCritical()<<"start service:PiServiceLauncher"<<QProcess().execute("sc start PiServiceLauncher");
        m_errorCode = SERVICE_NOT_RUNNING;
    }
    clearDeviceParam();
    checkConnectStatus();
    switchScreenOrient();
    writeDeviceInfo();
}

PiService::~PiService(){
    qCritical()<<"~PiService";
    shutdown();
    WinCommon::killProcessByName("AutoplayVR.exe");
    WinCommon::killProcessByName("PiVideoPlayer.exe");
    WinCommon::killProcessByName("Kodi.exe");
    WinCommon::killProcessByName("Pihome.exe");
}

void PiService::checkConnectStatus(){
    if(!WinCommon::isServiceAlive((WCHAR*)TEXT("PiService.exe"))){
        m_errorCode = SERVICE_NOT_RUNNING;
        emit errorCodeChanged(m_errorCode);
        setUsbState(false);
        setHdmiState(false);
        setServiceState(false);
        setControllerConnectState(false);
        return;
    }

    if(m_Handle!=nullptr){
        if(svc_success==svc_getUsbState(m_Handle)){
            setUsbState(true);
        }else{
            setUsbState(false);
        }

        if(svc_success==svc_getHdmiState(m_Handle)){
            setHdmiState(true);
        }else{
            setHdmiState(false);
        }

        Mode_Type mode;
        if(svc_success==svc_getServiceConnection(m_Handle,&mode)){
            setServiceState(true);
            setModeType(mode);
            if(mode != mode_State_pimax_smart){
                m_errorCode = MODE_NOT_SUPPORTED;
            }else{
                m_errorCode = NONE;
            }

            recenterSupported = supportRecenter();
            emit recenterSupportedChanged(recenterSupported);

            int hiddenEnabled = 0;
            if(svc_success == svc_getIntConfig(m_Handle,"dbg_hidden_area_enable",&hiddenEnabled)){
                hiddenAreanEnabled = hiddenEnabled == 0?false:true;
            }
            emit hiddenAreanEnabledChanged(hiddenAreanEnabled);

            int pvrEnabled = 0;
            if(svc_success == svc_getIntConfig(m_Handle,"enable_pvr_home",&pvrEnabled)){
                pvrHomeEnabled = pvrEnabled == 0?false:true;
            }
            emit pvrHomeEnabledChanged(pvrHomeEnabled);

            updateRefreshBuf();
            piVector3f vector;
            m_brightnessLeft = vector.x;
            emit brightnessLeftChanged(m_brightnessLeft);

            svc_getVector3fConfig(m_Handle, "color_brightness_1_rgb", &vector);
            m_brightnessRight = vector.x;
            emit brightnessRightChanged(m_brightnessRight);

            svc_getVector3fConfig(m_Handle, "color_contrast_0_rgb", &vector);
            m_contrastLeft = vector.x;
            emit contrastLeftChanged(m_contrastLeft);

            svc_getVector3fConfig(m_Handle, "color_contrast_1_rgb", &vector);
            m_contrastRight = vector.x;
            emit contrastRightChanged(m_contrastRight);

            if(mHmdName.contains("8K")){
                int lhEnabled = 0;
                if(svc_success == svc_getIntConfig(m_Handle,"enable_lighthouse_tracking",&lhEnabled)){
                    lighthouseEnabled = lhEnabled == 0?false:true;
                }
                int compatibleEnabled = 0;
                if(svc_success == svc_getIntConfig(m_Handle,"steamvr_use_native_fov",&compatibleEnabled)){
                    compatibleSteamvrUsingParallel = compatibleEnabled == 0?true:false;
                }
                emit compatibleSteamvrUsingParallelChanged(compatibleSteamvrUsingParallel);

                if(svc_success == svc_getIntConfig(m_Handle,"fov_level",&fovType)){
                    emit fovTypeChanged(fovType);
                }

                getSmartSmoothingStatus();

            }else{
                lighthouseEnabled = false;
            }
            emit lighthouseEnabledChanged(lighthouseEnabled);

            getFoveatedStatusInfo();
        }else{
            setServiceState(false);
            if(svc_success != svc_getIntConfig(m_Handle,"error_code",&m_errorCode)){
                m_errorCode = UNKNOW;
            }
        }
        emit errorCodeChanged(m_errorCode);

        //pitool only support pimax mode
        if(mode != mode_State_video_p && mode != mode_State_pimax_smart){
            changeModeType(0);
        }

        return;
    }
    if(!supportModeSwitch()){
        m_nResult = svc_start(&m_Handle,NULL);//,mode_State_pimax_smart);
    }else {
        m_nResult = svc_start(&m_Handle,NULL);//&PiService::PiService_Callback
    }
    if(m_nResult==svc_success&&m_Handle!=nullptr){
        qInfo() << "PiService::start,success";
        loadDeviceParam();
    }else{
        qCritical() << "PiService::start,failed"<<getError();
        m_Handle = nullptr;
    }
}

void __stdcall PiService::PiService_Callback(piServiceMessage message,piWPARAM wParam,piLPARAM lParam){
    qInfo()<<"PiService_Callback,message="<<message<<wParam<<lParam;
    if(gInstance==nullptr){
        return;
    }
    if(message==SERVICE_USB){
        if(wParam==svc_success){
            gInstance->setUsbState(true);
        }else if(wParam==svc_usb_no_connect){
            gInstance->setUsbState(false);
        }
    }else if(message==SERVICE_HDMI){
        if(wParam==svc_success){
            gInstance->setHdmiState(true);
        }else if(wParam==svc_hdmi_no_connect){
            gInstance->setHdmiState(false);
        }
    }else if(message==SERVICE_SERVER_STATE){
        if(wParam==svc_success){
            gInstance->setServiceState(true);
            gInstance->setModeType(lParam);
        }else {//if(wParam==svc_service_failed || wParam==svc_usb_no_connect || wParam==svc_hdmi_no_connect){
            gInstance->setServiceState(false);
        }
    }else if(message==SERVICE_DFU_DRIVER_NOT_FOUND){
        qCritical()<<"dfu driver is not found";
        gInstance->setDfuDriverState(true);
    }else if(message==SERVICE_SERVER_NOT_START){
        gInstance->setUsbState(false);
        gInstance->setHdmiState(false);
        gInstance->setServiceState(false);
        gInstance->setControllerConnectState(false);
    }
}

void PiService::getHmdInfo()
{
    piSvcHmdInfo hmdInfo;
    int nRet = 0;
    if(svc_success==(nRet=svc_getSvcHmdDesc(m_Handle,&hmdInfo))){
        setHmdName(QString(hmdInfo.DisplayPiSvcHmdiceName));
        setProductName(QString(hmdInfo.ProductName));
        setSerialNumber(QString(hmdInfo.SerialNumber));
        setFmVersion(QString(hmdInfo.version_generation));
        setFmBuildVersion(hmdInfo.version_major*1000+hmdInfo.version_Minor);
        mHmdInfoAvaliable = true;
        qCritical()<<"hmdInfo"<<mHmdName<<mProductName<<mSerialNumber<<mFmVersion<<mFmBuildVersion;
    }else{
        qCritical()<<"svc_getSvcHmdDesc failed"<<nRet;
        mHmdInfoAvaliable = false;
    }
}

void PiService::clearDeviceParam(){
    setHmdCap(0);
    setHmdName("");
    setProductName("");
    setSerialNumber("");
    setFmVersion("");
    setFmBuildVersion(0);

    setTrackedDeviceState(0);
    setControllerConnectState(false);
}
void PiService::loadDeviceParam(){
    if(m_Handle==nullptr){
        checkConnectStatus();
        if(m_Handle==nullptr){
            return;
        }
    }
    if(!mbConnected){
        if(svc_success==svc_getUsbState(m_Handle)){
            setUsbState(true);
        }
        if(svc_success==svc_getHdmiState(m_Handle)){
            setHdmiState(true);
        }
        Mode_Type mode;
        if(svc_success==svc_getServiceConnection(m_Handle,&mode)){
            setServiceState(true);
            setModeType(mode);
            if(!supportModeSwitch()&&(mode_State_video_p != mode && mode_State_pimax_smart != mode)){
                changeModeType(0);
            }
        }
        return;
    }

    int caps = 0;
    if(svc_success==svc_getHmdCaps(m_Handle,&caps)){
        setHmdCap(caps);
    }

    getHmdInfo();

    mDisplayName = mHmdName;
    char deviceName[100] = {'\0'};
    if(svc_success==svc_getTrackerInfo(m_Handle,TrackedDevice_HMD,deviceName,sizeof(deviceName)-1)){
        mDisplayName = deviceName;
        int bussiness_flag = 0;
        if((mFmBuildVersion >= 2173) && (svc_success == svc_getIntConfig(m_Handle,"get_bussiness_flag",&bussiness_flag))){

            if(bussiness_flag){
                if (mDisplayName == "Pimax 5K") {
                    mDisplayName += " XR";
                } else {
                    mDisplayName += " BE";
                }
            }
        }

    }
    emit displayHmdNameChanged(mDisplayName);

    int nRet = 0;
    bool enable=false;
    if(gyroCtrl()&&svc_success==(nRet=svc_getGyroState(m_Handle,&enable))){
        qCritical()<<"svc_getGyroState"<<enable;
//        setEnableGyro(enable);
        if(mEnableGyro!=enable){
            mEnableGyro = enable;
            writeEnableGyro(!mEnableGyro);
        }
        emit enableGyroChanged(mEnableGyro);
    }else{
        qCritical()<<"svc_getGyroState failed"<<gyroCtrl()<<nRet;
    }
    ToneState tone=TONE_UNKNOW;
    if(toneCtrl()&&svc_success==(nRet=svc_getToneState(m_Handle,&tone))){
        qCritical()<<"svc_getToneState"<<tone;
        setToneState(tone);
    }else{
        qCritical()<<"svc_getToneState failed"<<toneCtrl()<<nRet;
    }
    BrightState bright=LEVEL_INVALID;
    if(brightCtrl()&&svc_success==(nRet=svc_getBrightState(m_Handle,&bright))){
        qCritical()<<"svc_getBrightState"<<bright;
        setBrightState(bright);
        if(!mHmdName.contains("8K")){
            if(bright<mMinBrightLevel){
                writeBrightLevel(mMinBrightLevel);
            }else if(bright>mMaxBrightLevel){
                writeBrightLevel(mMaxBrightLevel);
            }
        }
    }else{
        qCritical()<<"svc_getBrightState failed"<<brightCtrl()<<nRet;
    }
    piResolutionCapabilityFlag flag=e_unknow;
    if(resolutionCtrl()&&svc_success==(nRet=svc_getCurrentResolution(m_Handle,&flag))){
        qCritical()<<"svc_getCurrentResolution"<<flag;
        setResolution(flag);
    }else{
        qCritical()<<"svc_getCurrentResolution failed"<<resolutionCtrl()<<nRet;
    }

    float quality=1.0;
    if(displayQualityCtrl()&&svc_success==(nRet=svc_getDisplayQualityValue(m_Handle,&quality))){
        qCritical()<<"svc_getDisplayQualityValue"<<quality;
        setDisplayQuality(quality);
    }else{
        qCritical()<<"svc_getDisplayQualityValue failed"<<nRet<<displayQualityCtrl();
    }

    float ipd = 0;
    if(svc_success==(nRet=svc_getLensValue(m_Handle, &ipd))){
        qCritical()<<"svc_getLensValue"<<ipd;
        setIpdValue(ipd);
    }else{
        qCritical()<<"svc_getLensValue failed"<<nRet<<ipdCtrl();
    }

    svc_getFloatConfig(m_Handle, "lens_separation_offset", &m_lensSeparation);
    emit lensSeparationChanged(m_lensSeparation);

    enable = true;
    if(audioCtrl()&&svc_success==(nRet=svc_getPimaxDefaultAudioConfig(m_Handle,&enable))){
        qCritical()<<"svc_getPimaxDefaultAudioConfig"<<enable;
        if(mAudioPimax!=enable){
            mAudioPimax = enable;
            writeAudioPimax(!mAudioPimax);
        }
        emit audioPimaxChanged(mAudioPimax);
    }else{
        qCritical()<<"svc_getPimaxDefaultAudioConfig failed"<<nRet<<audioCtrl();
    }
    checkControllerState();
}

void PiService::setPairStatus(int state)
{
    if(state > Paired_Unknow || state < 0){
        return;
    }
    pairStatus = state;
}

void PiService::setPairIndex(int index)
{
    pairIndex = index;
}

void PiService::stopPairController()
{
    //WinCommon::killProcessByName("ControllerPair.exe");
    HANDLE quitEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("ControllerPairQuitEvent"));
    SetEvent(quitEvent);
    CloseHandle(quitEvent);
}

void PiService::startPairController()
{
    pairStatus = Pairing;
    emit pairStatusChanged(pairStatus);
    std::thread a = std::thread([this]() {
        QString path = QCoreApplication::instance()->applicationDirPath().replace("/","\\");
        auto ret = WinCommon::startProcess(L"ControllerPair.exe", true, path.toStdWString(), false, pairIndex==0? L"0 30" : L"1 30");
        if (ret.first) {
            qCritical()<<"startPairController>>>>:"<<ret.second;
            if (ret.second == 0) {
                pairStatus = pairIndex == 0 ? Paired_Zero : Paired_One;
                emit pairStatusChanged(pairStatus);
                return;
            }else if(ret.second == 2){
                pairStatus = Paired_Dongle_Count;
                emit pairStatusChanged(pairStatus);
                return;
            }
        }
        pairStatus = Paired_Failed;
        qCritical()<<"startPairController:"<<ret.second;
        emit pairStatusChanged(pairStatus);
    });
    a.detach();
}

void PiService::startRoomSettingNoteInHmd(bool isChinese)
{
    QProcess p(0);
    p.setWorkingDirectory(QCoreApplication::instance()->applicationDirPath().replace("/","\\"));
    QStringList args;
    args.append("/i:0");
    args.append(isChinese?"/l:1":"/l:0");
    p.startDetached("pi_infomation.exe", args);
}

void PiService::stopRoomSettingNoteInHmd()
{
    WinCommon::killProcessByName("pi_infomation.exe");
}

bool PiService::getHmdPosition()
{
    return svc_success==svc_getHmdPosition(m_Handle,&hmdX,&hmdY,&hmdZ);
}

bool PiService::supportRecenter()
{
    int mSupport = 0;
    if(svc_getIntConfig(m_Handle,"support_recenter",&mSupport) == svc_success){
        //qCritical()<<">>>>> recenter support:" << mSupport;
        return mSupport != 0;
    }
    return false;
}

bool PiService::setRefreshByIndex(int index)
{
     if(svc_success == svc_setIntConfig(m_Handle, "display_timing_selection", index)){
         return true;
     }
     return false;
}

int PiService::getRefreshCounts()
{
    int mCount = 0;
    svc_getIntConfig(m_Handle,"display_timing_count",&mCount);
    return mCount;
}

int PiService::getRefreshIndex()
{
    int mIndex = 0;
    svc_getIntConfig(m_Handle,"display_timing_selection",&mIndex);
    return mIndex;
}

void PiService::updateRefreshBuf()
{
    int m_counts = getRefreshCounts();
    refreshBuf.clear();
    if(m_counts > 1){
        for(int i = 0; i < m_counts; i++){
            char key[50];
            sprintf_s(key, "display_timing_name_%d", i);
            char buf[50];
            svc_getStringConfig(m_Handle, key, buf, sizeof(buf));
            //qCritical()<<">>>>>>>>>>>>>>>> buf:" << buf;
            refreshBuf.append(buf);
        }

    }
    emit refreshCountsChanged(m_counts);
    emit refreshIndexChanged(getRefreshIndex());
    emit refreshValuesChanged(refreshBuf);
}

bool PiService::hmdNoMoving()
{
    float mX,mY,mZ;
    if(svc_success==svc_getHmdPosition(m_Handle,&mX,&mY,&mZ)
            && (abs(mX-hmdX)>deviation||abs(mY-hmdY)>deviation||abs(mZ-hmdZ)>deviation))
    {
        hmdX = mX;
        hmdY = mY;
        hmdZ = mZ;
        return false;
    }
    return true;
}

#define LEAP_MOTION_REG "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Leap Services"
void PiService::checkControllerState(){
    if(m_Handle==nullptr||!mbConnected){
        setControllerConnectState(false);
        setTrackedDeviceState(0);
        return;
    }

    //check leap motion state
    int mLeapMotionPlugin = 0;
    if(svc_getIntConfig(m_Handle,"leap:device_connected",&mLeapMotionPlugin) == svc_success){
        leapMotionPlugin = mLeapMotionPlugin == 0?false:true;
        emit leapMotionPluginChanged(leapMotionPlugin);
        if(leapMotionPlugin){
            QString leapPath = RegisterHandler::getInstance().getRegisterValue(LEAP_MOTION_REG,"InstallLocation","");
            if(leapPath.isEmpty()){
                leapMotionInstalled = false;
            }else{
                leapMotionInstalled = QDir(leapPath.replace("\"","")).exists()?true:false;
            }
            emit leapMotionIntalledChanged(leapMotionInstalled);
            if(leapMotionInstalled){
                int mLeapMotionServiceState = 0;
                if(svc_getIntConfig(m_Handle,"leap:service_ready",&mLeapMotionServiceState) == svc_success){
                    leapMotionServiceReady = mLeapMotionServiceState == 0?false:true;
                    emit leapMotionServiceReadyChanged(leapMotionServiceReady);
                    int mLeapMotionDeviceStat = 0;
                    if(svc_getIntConfig(m_Handle,"leap:device_ready",&mLeapMotionDeviceStat) == svc_success){
                        leapMotionDeviceReady = mLeapMotionDeviceStat == 0?false:true;
                        emit leapMotionDeviceReadyChanged(leapMotionDeviceReady);
                    }
                }
            }
        }

    }

    int mControllerPairSupportState = 0;
    if(svc_getIntConfig(m_Handle,"support_controller_pair",&mControllerPairSupportState) == svc_success){
        //qCritical()<<"support_controller_pair:"<<mControllerPairSupportState;
        controllerPairSupport = mControllerPairSupportState == 0?false:true;
        emit controllerPairSupportChanged(controllerPairSupport);
    }

    //get space calibration state
    int calibrated = 0;
    if(svc_getIntConfig(m_Handle,"space_calibrate",&calibrated) == svc_success){
        mSpaceCalibrated = calibrated == 0?false:true;
        //qCritical()<<"mSpaceCalibrated:"<<mSpaceCalibrated;
        int headTracked = 0;
        svc_getIntConfig(m_Handle,"head_tracking_state",&headTracked);
        mHeadTracked = headTracked == 0?false:true;
        emit headTrackedChanged(mHeadTracked);
        emit spaceCalibratedChanged(mSpaceCalibrated);
    }

    int device = 0;
    if(svc_success==svc_getConnectedDevices(m_Handle,&device)){
        setTrackedDeviceState(device);
        if(device>1){
            setControllerConnectState(true);
            int pressed = 0;
            if(svc_getIntConfig(m_Handle,"trigger_press",&pressed) == svc_success){
                triggerPressed = pressed != 0?true:false;
            }else{
                triggerPressed = false;
            }
            emit triggerPressedChanged(triggerPressed);

        }else{
            setControllerConnectState(false);
        }
    }else{
        setControllerConnectState(false);
        setTrackedDeviceState(0);
    }
    if(tracker1()){
        char deviceName[100] = {'\0'};
        if(svc_success==svc_getTrackerInfo(m_Handle,TrackedDevice_Tracker0,deviceName,sizeof(deviceName)-1)){
            mControllerName = QString(deviceName);
        }else{
            mControllerName = "";
        }
    }else{
        mControllerName = "";
    }
    emit controllerNameChanged(mControllerName);

    if(controller1()){
        int percent = 0;
        if(svc_success==svc_getDeviceBatteryPercent(m_Handle,TrackedDevice_LeftController,&percent)){
            mController1Battery = percent;
            emit controller1BatteryChanged(percent);
        }
    }
    if(controller2()){
        int percent = 0;
        if(svc_success==svc_getDeviceBatteryPercent(m_Handle,TrackedDevice_RightController,&percent)){
            mController2Battery = percent;
            emit controller2BatteryChanged(percent);
        }
    }
    if(tracker1()){
        int percent = 0;
        if(svc_success==svc_getDeviceBatteryPercent(m_Handle,TrackedDevice_Tracker0,&percent)){
            mTracker1Battery = percent;
            emit tracker1BatteryChanged(percent);
        }
    }
    if(tracker2()){
        int percent = 0;
        if(svc_success==svc_getDeviceBatteryPercent(m_Handle,TrackedDevice_Tracker1,&percent)){
            mTracker2Battery = percent;
            emit tracker2BatteryChanged(percent);
        }
    }
    if(controllerState() && mControllerName.contains("Nolo")){
        setNoloDisplayMode(mNoloDisplayMode);
    }
}

void PiService::startMagnetCalibrate(){
    if(m_Handle&&mbConnected){
        svc_magnetismCalibrationHmd(m_Handle);
    }
}

bool PiService::isCalibrateFinished(){
    if(svc_success==svc_magnetismCalibrationed(m_Handle)){
        return true;
    }
    return false;
}

bool PiService::spaceCalibration(float height)
{
    qCritical()<<">>>>> height:" << height;
    return svc_success == svc_setFloatConfig(m_Handle, "space_calibrate", height);
}

bool PiService::centerCalibration()
{
    return svc_success == svc_setFloatConfig(m_Handle, "center_calibrate", 1);
}

bool PiService::floorCalibration(float height)
{
    qCritical()<<"floor height:" << height;
    return svc_success == svc_setFloatConfig(m_Handle, "floor_calibrate", height);
}

void PiService::switchScreenOrient(){
    QDesktopWidget *desktop = QApplication::desktop();
    int screen_count = desktop->screenCount();
    int primaryScreen = desktop->primaryScreen();
    for(int i=0;i<screen_count;i++){
        if(i==primaryScreen){
            continue;
        }
        LPCWSTR deviceName;
        if(i==0){
            deviceName = L"\\\\.\\DISPLAY1";
        }else{
            deviceName = L"\\\\.\\DISPLAY2";
        }
        DEVMODE devMode;
        bool bResult = EnumDisplaySettings(deviceName,ENUM_CURRENT_SETTINGS,&devMode);
        if(bResult){
            qCritical()<<"EnumDisplaySettings success"<<devMode.dmPelsWidth<<devMode.dmPelsHeight<<devMode.dmDisplayOrientation;
        }else{
            qCritical()<<"EnumDisplaySettings failed";
        }
        if(devMode.dmDisplayOrientation==DMDO_270||devMode.dmDisplayOrientation==DMDO_90){
            continue;
        }
        memset(&devMode,0,sizeof(devMode));
        devMode.dmSize = sizeof(devMode);
        if(mResolution==e_1080_1920){
            devMode.dmPelsWidth    = 1920;
            devMode.dmPelsHeight   = 1080;
        }else if(mResolution==e_1440_2560){
            devMode.dmPelsWidth    = 2560;
            devMode.dmPelsHeight   = 1440;
        }else if(mResolution==e_2160_3840){
            devMode.dmPelsWidth    = 3840;
            devMode.dmPelsHeight   = 2160;
        }else if(mResolution==e_1200_2160){
            devMode.dmPelsWidth    = 2160;
            devMode.dmPelsHeight   = 1200;
        }
        devMode.dmDisplayOrientation = DMDO_90;
        devMode.dmFields       = DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYORIENTATION;
        long nRet = ChangeDisplaySettingsEx(deviceName,&devMode,NULL,CDS_RESET,NULL);
        qCritical()<<"ChangeDisplaySettings result="<<nRet<<devMode.dmPelsWidth<<devMode.dmPelsHeight<<devMode.dmDisplayOrientation;
    }
}

void PiService::setConnect(bool connect){
    if(mbConnected!=connect){
        mbConnected = connect;
        if(!mbConnected){
            clearDeviceParam();
        }else{
            loadDeviceParam();
            //get space calibration state
            int calibrated = 0;
            if(svc_getIntConfig(m_Handle,"space_calibrate",&calibrated) == svc_success){
                mSpaceCalibrated = calibrated == 0?false:true;
                int headTracked = 0;
                svc_getIntConfig(m_Handle,"head_tracking_state",&headTracked);
                mHeadTracked = headTracked == 0?false:true;
                emit headTrackedChanged(mHeadTracked);
                emit spaceCalibratedChanged(mSpaceCalibrated);
            }
        }
        emit connectChanged(mbConnected);
    }
}
void PiService::setServiceState(bool connect){
    if(mbServiceConnect!=connect){
        mbServiceConnect = connect;
        emit serviceStateChanged(connect);
        setConnect(mbUsbConnect&&mbHdmiConnect&&mbServiceConnect);
    }
}
#define FORCE_DFU_VERSION 2212
void PiService::setUsbState(bool usbState){
    if(mbUsbConnect!=usbState || !mHmdInfoAvaliable){
        mbUsbConnect = usbState;
        emit usbStateChanged(usbState);
        if(mbUsbConnect){
            mForceUpgrading = false;
            emit forceUpgradingChanged(mForceUpgrading);
            getHmdInfo();
#if 1//for 8k fm issue
            if(mHmdName.contains("8K") && mFmBuildVersion < FORCE_DFU_VERSION && mFmBuildVersion > 0){
                QString path = QCoreApplication::instance()->applicationDirPath().replace("/","\\");
                path += "\\dfudriver\\p2.dfu";
                mForceUpgrading = true;//upgradeFirmware(path);
                mForcePath = path;
                emit forceUpgradingChanged(mForceUpgrading);
            }
#endif
        }else{
            mHmdInfoAvaliable = false;
        }
        setConnect(mbUsbConnect&&mbHdmiConnect&&mbServiceConnect);
    }
}
void PiService::setHdmiState(bool hdmiState){
    if(mbHdmiConnect!=hdmiState){
        mbHdmiConnect = hdmiState;
        emit hdmiStateChanged(hdmiState);
        setConnect(mbUsbConnect&&mbHdmiConnect&&mbServiceConnect);
    }
}

void PiService::setControllerConnectState(bool state){
    if(mControllerState!=state){
        mControllerState = state;
        emit controllerStateChanged(state);
    }
}
void PiService::setControllerPlugin(bool state){
    if(mControllerPlugin!=state){
        mControllerPlugin = state;
        emit controllerPluginChanged(state);
    }
}


void PiService::setTrackedDeviceState(int state){
    if(true/*mTrackedDeviceState!=state*/){
        mTrackedDeviceState = state;
        bool m_controller0 = controller1();
        bool m_controller1 = controller2();
        bool m_tracker0 = tracker1();
        bool m_tracker1 = tracker2();
        if(m_controller0){
            int Tracked = 0;
            svc_getIntConfig(m_Handle,"controller0_tracking_state",&Tracked);
            mController0Tracked = Tracked == 0?false:true;
        }
        if(m_controller1){
            int Tracked = 0;
            svc_getIntConfig(m_Handle,"controller1_tracking_state",&Tracked);
            mController1Tracked = Tracked == 0?false:true;
        }
        if(m_tracker0){
            int Tracked = 0;
            svc_getIntConfig(m_Handle,"tracker0_state",&Tracked);
            mBase0Tracked = Tracked == 0?false:true;
        }
        if(m_tracker1){
            int Tracked = 0;
            svc_getIntConfig(m_Handle,"tracker1_state",&Tracked);
            mBase1Tracked = Tracked == 0?false:true;
        }
        emit controller1Changed(m_controller0);
        emit controller2Changed(m_controller1);
        emit tracker1Changed(m_tracker0);
        emit tracker2Changed(m_tracker1);
        emit controller0TrackedChanged(mController0Tracked);
        emit controller1TrackedChanged(mController1Tracked);
    }
}
void PiService::setDfuDriverState(bool state){
    if(mNoDfuDriver!=state){
        mNoDfuDriver=state;
        emit noDfuDriverChanged(state);
    }
}

void PiService::setSwitchingMode(bool state){
    if(mSwitchingMode!=state){
        mSwitchingMode = state;
        emit switchingModeChanged(state);
    }
}

void PiService::setHmdCap(int cap){
    if(mHmdCaps!=cap){
        mHmdCaps = cap;
        qCritical()<<"setHmdCap"<<mHmdCaps;
        emit modeCtrlChanged(modeCtrl());
        emit backlightCtrlChanged(backlightCtrl());
        emit resolutionCtrlChanged(resolutionCtrl());
        emit toneCtrlChanged(toneCtrl());
        emit brightCtrlChanged(brightCtrl());
        emit ipdCtrlChanged(ipdCtrl());
        emit bluelightCtrlChanged(bluelightCtrl());
        emit proximityCtrlChanged(proximityCtrl());
        emit gyroCtrlChanged(gyroCtrl());
        emit eyePoseCtrlChanged(eyePoseCtrl());
        emit audioCtrlChanged(audioCtrl());
        emit displayQualityCtrlChanged(displayQualityCtrl());
        emit rebootCtrlChanged(rebootCtrl());
    }

    mResolutionCap = 0;
    if(resolutionCtrl()){
        svc_getResolutionCaps(m_Handle,&mResolutionCap);
        emit resolutionCapChanged(mResolutionCap);
        initResolutionVector();
    }
}

void PiService::setHmdName(QString name){
    mHmdName = name;
    emit hmdNameChanged(name);
}
void PiService::setProductName(QString product_name){
    mProductName = product_name;
    emit productNameChanged(product_name);
}
void PiService::setSerialNumber(QString serial_number){
    mSerialNumber = serial_number;
    emit serialNumberChanged(serial_number);
}
void PiService::setFmVersion(QString version){
    mFmVersion = version;
    emit fmVersionChanged(version);
}
void PiService::setFmBuildVersion(int version){
    mFmBuildVersion = version;
    emit fmBuildVersionChanged(version);
}

void PiService::setModeType(int mode){
    if(mModeType!=mode){
        mModeType = mode;
        emit modeTypeChanged(modeType());
    }
}
void PiService::setEnableGyro(bool enable){
    mEnableGyro = enable;
    emit enableGyroChanged(enable);
}
void PiService::setToneState(ToneState state){
    mToneState = state;
    emit coolToneChanged(coolTone());
}

int PiService::brightLevel()
{
    int mBrightDisplay = 0;
    if(mDisplayName.contains("8K")){
        for(int i = 0;i<BRIGHTLEVEL_LEN;i++){
            if(mBrightState >= brightLevelArrayFor8K[i]){
                mBrightDisplay = i;
                break;
            }
        }
    }else if(mDisplayName.contains("5K")){
        for(int i = 0;i<BRIGHTLEVEL_LEN;i++){
            if(mBrightState >= brightLevelArrayFor5K[i]){
                mBrightDisplay = i;
                break;
            }
        }
    }else{
        mBrightDisplay = mBrightState;
    }
    qCritical()<<"mDisplayName:"<<mDisplayName;
    qCritical()<<"mBrightDisplay:"<<mBrightState;
    return mBrightDisplay;
}
void PiService::setBrightState(BrightState state){
    mBrightState = state;
    emit brightLevelChanged(brightLevel());
}
void PiService::setResolution(int value){
    if(mResolution!=value){
        mResolution = value;
        //emit lowerResolutionChanged(lowerResolution());
        emit resolutionChanged(mResolution);
        mResolutionIndex = getResolutionIndex(mResolution);
        emit resolutionIndexChanged(mResolutionIndex);
    }
}
void PiService::setDisplayQuality(float value){
    mDisplayQuality = value;
    emit displayQualityChanged(value);
}

void PiService::setIpdValue(float value)
{
    mIpd = value;
    emit ipdChanged(value);
}

void PiService::setAudioPimax(bool enable){
    mAudioPimax = enable;
    emit audioPimaxChanged(enable);
}

void PiService::shutdown(){
    typedef piSvcResult(*FUN)(piSvcHandle handle);
    if(m_Handle&&mPiSvrLib.isLoaded()){
        FUN svc_shutdown = (FUN)mPiSvrLib.resolve("svc_shutdown");
        svc_shutdown(m_Handle);
        m_Handle = nullptr;
        setConnect(false);
        setUsbState(false);
        setHdmiState(false);
    }
}

bool PiService::hasNoError(){
    if(m_nResult==svc_success){
        return true;
    }
    return false;
}

QString PiService::getError(){
    switch(m_nResult){
    case svc_success:
        return "";
    case svc_usb_no_connect:
        return tr("usb no connect").toLocal8Bit();
    case svc_hdmi_no_connect:
        return tr("svc_hdmi_no_connect");
    case svc_handle_controller_no_connect:
        return tr("svc_handle_controller_no_connect");
    case svc_hid_failed:
        return tr("svc_hid_failed");
    case svc_driver_failed:// maybe i2c suspend
        return tr("driver failed");
    case svc_firmware_failed:
        return tr("firmware failed");
    case svc_path_not_found:
        return tr("path not found");
    case svc_state_not_connected:
        return tr("not connected");
    case svc_param_error:
        return tr("param error");
    case svc_timeout:
        return tr("time out");
    case svc_dll_failed:
        return tr("dll error");
    case svc_unknow:
        return tr("unknow error");
    }
    return m_nResult;
}

void PiService::rebootPiService(){
    if(m_Handle){
        svc_reconnectServer(m_Handle);
    }
}

bool PiService::rebootHmd(){
    if(m_Handle&&rebootCtrl()){
        m_nResult = svc_rebootHmd(m_Handle);
        if(m_nResult==svc_success){
            qCritical() << "rebootHmd success";
            return true;
        }else{
            qCritical() << "rebootHmd failed"<<m_nResult;
            return false;
        }
    }
    return false;
}
bool PiService::resetEyePose(){
    if(m_Handle&&mbConnected&&eyePoseCtrl()){
        m_nResult = svc_resetEyePose(m_Handle);
        if(m_nResult==svc_success){
            qCritical() << "resetEyePose success";
            return true;
        }else{
            qCritical() << "resetEyePose failed" << m_nResult;
        }
    }
    return false;
}

bool PiService::changeModeType(int mode){
    if(m_Handle&&mbConnected){
        Mode_Type modeType = (mode==1)?mode_State_video_p:mode_State_pimax_smart;
        if(mModeType==modeType){
            return true;
        }

        QProcess *process = new QProcess();
        if(mode==0){
            WinCommon::killProcessByName("Kodi.exe");
        }else{
            WinCommon::killProcessByName("AutoplayVR.exe");
            WinCommon::killProcessByName("PiVideoPlayer.exe");
            WinCommon::killProcessByName("Pihome.exe");
        }
        delete process;
        m_nResult = svc_changeMode(m_Handle,modeType);
        if(m_nResult==svc_success){
            mSwitchingMode = true;
            qCritical() << "svc_changeMode success"<<modeType;
            return true;
        }else{
            qCritical() << "svc_changeMode failed" << m_nResult;
            return false;
        }
    }
    return false;
}

int PiService::getModeType(){
    Mode_Type mode;
    if(svc_success==svc_getServiceConnection(m_Handle,&mode)){
        setServiceState(true);
        setModeType(mode);
        return modeType();
    }
    return -1;
}

bool PiService::resetFactory(){
    if(m_Handle&&mbConnected){
        m_nResult = svc_resetFactorySettings(m_Handle);
        if(m_nResult==svc_success){
            qCritical() << "resetFactory success";
            return true;
        }else{
            qCritical() << "resetFactory failed" << m_nResult;
            return false;
        }
    }
    return false;
}

bool PiService::slopeCalibration(){
    if(m_Handle&&mbConnected){
        m_nResult = svc_slopeCalibrationHmd(m_Handle);
        if(m_nResult==svc_success){
            qCritical() << "slopeCalibration success";
            return true;
        }else{
            qCritical() << "slopeCalibration failed" << m_nResult;
            return false;
        }
    }
    return false;
}

bool PiService::setLighthouseEnabled(bool enabled)
{
      if(m_Handle&&mbConnected){
          m_nResult = svc_setIntConfig(m_Handle,"enable_lighthouse_tracking",enabled);
          if(m_nResult==svc_success){
            lighthouseEnabled = enabled;
          }
          emit lighthouseEnabledChanged(lighthouseEnabled);
          return true;
      }
      return false;
}

bool PiService::setPvrHomeEnabled(bool enabled)
{
    if(m_Handle){
        m_nResult = svc_setIntConfig(m_Handle,"enable_pvr_home",enabled);
        if(m_nResult==svc_success){
          pvrHomeEnabled = enabled;
        }
        emit pvrHomeEnabledChanged(pvrHomeEnabled);
        return true;
    }
    return false;
}

bool PiService::setCompatibleSteamvrUsingParallel(bool enabled)
{
    if(m_Handle&&mbConnected){
        m_nResult = svc_setIntConfig(m_Handle,"steamvr_use_native_fov",!enabled);
        if(m_nResult==svc_success){
          compatibleSteamvrUsingParallel = enabled;
        }
        emit compatibleSteamvrUsingParallelChanged(compatibleSteamvrUsingParallel);
        return true;
    }
    return false;
}

bool PiService::setFovType(int type)
{
    if(m_Handle&&mbConnected){
        m_nResult = svc_setIntConfig(m_Handle,"fov_level",type);
        if(m_nResult==svc_success){
          fovType = type;
        }
        emit fovTypeChanged(fovType);
        return true;
    }
    return false;
}

bool PiService::EnablehiddenArea(bool enabled)
{
    if(m_Handle&&mbConnected){
        m_nResult = svc_setIntConfig(m_Handle,"dbg_hidden_area_enable",enabled?1:0);
        if(m_nResult==svc_success){
          hiddenAreanEnabled = enabled;
        }
        emit hiddenAreanEnabledChanged(hiddenAreanEnabled);
        return true;
    }
    return false;
}

bool PiService::writeEnableGyro(bool enable){
    if(m_Handle&&gyroCtrl()){
        if(mEnableGyro==enable){
            return true;
        }
        m_nResult = svc_enableGyro(m_Handle,enable);
        if(m_nResult==svc_success){
            qCritical() << "writeEnableGyro success"<<enable;
            mEnableGyro = enable;
            RegisterHandler::getInstance().setRegisterValue(REG_PITOOL_PATH,REG_KEY_GYRO,mEnableGyro?1:0);
            return true;
        }else{
            qCritical() << "writeEnableGyro failed" << m_nResult;
            return false;
        }
    }
    return false;
}

bool PiService::writeCoolTone(bool enable){
    if(m_Handle&&toneCtrl()){
        ToneState state = enable?TONE_COOL:TONE_WARM;
        if(mToneState==state){
            return true;
        }
        m_nResult = svc_setToneState(m_Handle,state);
        if(m_nResult==svc_success){
            qCritical() << "writeCoolTone success"<<state;
            mToneState = state;
            return true;
        }else{
            qCritical() << "writeCoolTone failed" << m_nResult;
            return false;
        }
    }
    return false;
}

bool PiService::writeBrightLevel(int value){
    if(m_Handle&&brightCtrl()&&value<=3){
        BrightState state;
        if(mDisplayName.contains("8K")){
            state = brightLevelArrayFor8K[value];
        }else if(mDisplayName.contains("5K")){
            state = brightLevelArrayFor5K[value];
        }else{
            state = (BrightState)value;
        }

        if(mBrightState==state){
            return true;
        }
        m_nResult = svc_setBrightState(m_Handle,state);
        if(m_nResult==svc_success){
            qCritical() << "writeBrightLevel success"<<state;
            mBrightState = state;
            return true;
        }else{
            qCritical() << "writeBrightLevel failed" << m_nResult;
            return false;
        }
    }
    return false;
}
bool PiService::setNoloDisplayMode(int mode){
    if(!controllerState()){
        return false;
    }
    if(m_Handle){
        m_nResult = svc_setIntConfig(m_Handle,"nolo_diplay_mode",mode);
        if(m_nResult==svc_success){
            qCritical() << "svc_setIntConfig success"<<mode;
            mNoloDisplayMode = mode;
            RegisterHandler::getInstance().setRegisterValue(REG_PITOOL_PATH,"nolo_display_mode",mNoloDisplayMode);
            emit noloDisplayModeChanged(mNoloDisplayMode);
            return true;
        }else{
            qCritical() << "svc_setIntConfig failed" << m_nResult;
            return false;
        }
    }
    return false;
}

void PiService::enableMaxBrightLevel(){
    mMinBrightLevel = LEVEL_0;
    mMaxBrightLevel = LEVEL_4;
    RegisterHandler::getInstance().setRegisterValue(REG_PITOOL_PATH,"min_bright",mMinBrightLevel);
    RegisterHandler::getInstance().setRegisterValue(REG_PITOOL_PATH,"max_bright",mMaxBrightLevel);
    emit minBrightLevelChanged(mMinBrightLevel);
    emit maxBrightLevelChanged(mMaxBrightLevel);
}

bool PiService::writeResolution(int resolution){
    if(m_Handle&&resolutionCtrl()){
        if(mResolution==resolution){
            return true;
        }

        m_nResult = svc_setResolution(m_Handle,resolutionValueToFlag(resolution));

        if(m_nResult==svc_success){
            qCritical() << "writeResolution success "<<resolution;
            setResolution(resolution);
            return true;
        }else{
            qCritical() << "writeResolution failed" << m_nResult;
            return false;
        }
    }
    return false;
}

void PiService::initResolutionVector()
{
    int i = 0;
    mResolutionVector.clear();
    mResolutionVector.resize(5);
    if(mResolutionCap & e_1080_1920){
        mResolutionVector[i++] = e_1080_1920;
    }
    if(mResolutionCap & e_1440_2560){
        mResolutionVector[i++] = e_1440_2560;
    }
    if(mResolutionCap & e_1200_2160){
        mResolutionVector[i++] = e_1200_2160;
    }
    if(mResolutionCap & e_2160_3840){
        mResolutionVector[i++] = e_2160_3840;
    }
}

int PiService::getResolutionIndex(int resolution)
{
    for(int i = 0; i < mResolutionVector.size(); i++){
        if(mResolutionVector[i] == resolution){
            qCritical() << "mResolutionVector" << i;
            return i;
        }
    }
    return 0;
}

piResolutionCapabilityFlag PiService::resolutionValueToFlag(int resolution)
{
    switch(resolution){
    case 0x1:
        return e_1080_1920;
    case 0x02:
        return e_1440_2560;
    case 0x4:
        return e_1200_2160;
    case 0x8:
        return e_2160_3840;
    default:
        return e_unknow;
    }
}


bool PiService::writeDisplayQuality(float value){
    if(m_Handle&&displayQualityCtrl()){
        if(mDisplayQuality==value){
            return true;
        }
        m_nResult = svc_setDisplayQualityValue(m_Handle,value);
        if(m_nResult==svc_success){
            qCritical() << "writeDisplayQuality success"<<value;
            mDisplayQuality = value;
            return true;
        }else{
            qCritical() << "writeDisplayQuality failed" << m_nResult<<value;
            return false;
        }
    }
    return false;
}

bool PiService::writeIpdValue(float value)
{
    if(m_Handle){
        if(mIpd==value&&ipdCtrl()){
            return true;
        }
        m_nResult = svc_setLensValue(m_Handle,value);
        if(m_nResult==svc_success){
            qCritical() << "writeIpdValue success"<<value;
            mDisplayQuality = value;
            mIpd = value;
            return true;
        }else{
            qCritical() << "writeIpdValue failed" << m_nResult<<value;
            return false;
        }
    }
    return false;
}

bool PiService::writeAudioPimax(bool enable){
    if(m_Handle&&audioCtrl()){
        if(mAudioPimax==enable){
            return true;
        }
        m_nResult = svc_setPimaxDefaultAudioDevice(m_Handle,enable);
        if(m_nResult==svc_success){
            qCritical() << "svc_setPimaxDefaultAudioDevice success"<<enable;
            mAudioPimax = enable;
            RegisterHandler::getInstance().setRegisterValue(REG_PITOOL_PATH,REG_KEY_AUDIO,mAudioPimax?1:0);
            return true;
        }else{
            qCritical() << "svc_setPimaxDefaultAudioDevice failed" << m_nResult<<enable;
            return false;
        }
    }
    return false;
}

bool PiService::setSmartSmoothingEnable(bool enable)
{
    if(svc_setIntConfig(m_Handle, "dbg_asw_enable", enable) == svc_success) {
        qCritical() << "svc_setIntConfig abg_as_enable success";
        getSmartSmoothingStatus();
        return true;
    } else {
        qCritical() << "svc_setIntConfig abg_as_enable failed " << enable;
        return false;
    }
}

void PiService::getSmartSmoothingStatus()
{
    int smartSmoothEnable = 0;
    int smartSmoothAvailable = 0;
    int smartSmoothActive = 0;
    if(svc_getIntConfig(m_Handle,"dbg_asw_enable",&smartSmoothEnable) != svc_success){
        emit smartSmoothingEnableChanged(false);
        return;
    }
    m_smartSmoothingEnable = smartSmoothEnable;
    if(svc_getIntConfig(m_Handle,"asw_available",&smartSmoothAvailable) != svc_success){
       emit smartSmoothAvalibleStateChanged(false);
       return;
    }
    m_smartSmoothingAvailable = smartSmoothAvailable;
    if(svc_getIntConfig(m_Handle,"asw_active",&smartSmoothActive) != svc_success){
       emit smartSmoothActiveStateChanged(false);
       return;
    }
    m_smartSmoothingActive = smartSmoothActive;
    emit smartSmoothingEnableChanged(m_smartSmoothingEnable);
    emit smartSmoothAvalibleStateChanged(m_smartSmoothingAvailable);
    emit smartSmoothActiveStateChanged(m_smartSmoothingActive);
}

bool PiService::supportFoveated()
{
    int support = 0;
    if(svc_success == svc_getIntConfig(m_Handle, "support_foveated_rendering", &support)){
        if (support == 0)
            supportFoveatedStatus = false;
        else
            supportFoveatedStatus = true;
    }
    emit foveatedSupportChange(supportFoveatedStatus);
    return supportFoveatedStatus;
}

void PiService::setFoveatedEnable(bool enable)
{
    if (svc_setIntConfig(m_Handle, "enable_foveated_rendering", enable) == svc_success)
    {
        foveatedEnable = enable;
        qCritical() << "svc_setIntConfig enable_foveated_rendering success";
        getFoveatedStatusInfo();
    }
    else
    {
        qCritical() << "svc_setIntConfig enable_foveated_rendering failed";
    }
}

int PiService::getFoveatedStatusInfo()
{
    if(!supportFoveated()){
        return 0;
    }
    int enabled = 0;
    if (svc_getIntConfig(m_Handle, "enable_foveated_rendering", &enabled) == svc_success
        && svc_getIntConfig(m_Handle, "foveated_rendering_level", &foveatedLevel) == svc_success)
    {
        foveatedEnable = enabled == 0?false:true;
        int actived = 0;
         if(svc_success == svc_getIntConfig(m_Handle, "fovated_rendering_active", &actived)){
             foveatedActive = actived == 0?false:true;
            if (foveatedActive == false)
                foveatedStatus = 1;
            else
                foveatedStatus = 2;
         }
    }
    else
    {
        foveatedStatus = 0;
    }
    qCritical() << "enable_foveated_rendering status:" << enabled;
    emit foveatedLevelChange(foveatedLevel);
    emit foveatedEnalbeChange(foveatedEnable);
    emit foveatedStatusChange(foveatedStatus);

    return foveatedStatus;
}

void PiService::setFoveatedLevel(int level)
{
    if (level != foveatedLevel)
    {
        if (svc_setIntConfig(m_Handle, "foveated_rendering_level", level) == svc_success)
        {
            foveatedLevel = level;
            qCritical() << "svc_setIntConfig foveated_rendering_level success";
        }
        else
        {
            qCritical() << "svc_setIntConfig foveated_rendering_level failed";
        }
    }
}

bool PiService::forceUpgradeFirmware()
{
    return upgradeFirmware(mForcePath);
}

bool PiService::upgradeFirmware(QString fmFile){
    if(m_Handle&&mbUsbConnect){//mbConnected){
        QByteArray ba = fmFile.toLatin1();
        char *dfuPath = ba.data();
        qCritical()<<"upgradeFirmware"<<dfuPath;
        m_nResult = svc_startFirmwareUpgrade(m_Handle,dfuPath);
        if(m_nResult==svc_success){
            qCritical() << "upgradeFirmware success";
            return true;
        }else{
            qCritical() << "upgradeFirmware failed" << m_nResult;
            return false;
        }
    }else{
        return false;
    }
}

bool PiService::isNotebook(){
    SYSTEM_POWER_STATUS systemPowerSatus;
    GetSystemPowerStatus(&systemPowerSatus);
    int remaindPower=(int)systemPowerSatus.BatteryLifePercent;
    qInfo()<<"isNotebook battery"<<remaindPower;
    return remaindPower<=100;
}

PiService* PiService::getInstance(){
    return gInstance;
}

#define LINE1 "##############################################################\n"
#define LINE2 "==============================================================\n"

void PiService::writeDeviceInfo(){
    QString deviceInfo("");
    deviceInfo +=QString::fromLocal8Bit("Equipment  information\n");
    deviceInfo +=LINE1;

    deviceInfo +=QString::fromLocal8Bit("连接信息\n");
    deviceInfo += LINE2;
    deviceInfo += QString::fromLocal8Bit("连接状态: ")+(mbConnected?"Connected\n":"Disconnected\n");
    if(connect()){
        deviceInfo += QString::fromLocal8Bit("当前模式：");
        switch(mModeType){
        case mode_State_pimax_smart:
            deviceInfo+="pimax_smart";
            break;
        case mode_State_video_p:
            deviceInfo+="video_p";
            break;
        case mode_State_dfu:
            deviceInfo+="dfu";
            break;
        default:
            deviceInfo+=QString::number(mModeType);
            break;
        break;
        }
        deviceInfo+="\n";
//        deviceInfo += QString::fromLocal8Bit("当前系统支持模式：")+"\n";
        deviceInfo += QString::fromLocal8Bit("设备序列号：")+mSerialNumber+"\n";
        deviceInfo += QString::fromLocal8Bit("固件版本：")+mFmVersion+"\n";
        deviceInfo += QString::fromLocal8Bit("色调信息：")+(coolTone()?"Cool\n":"Warm\n");
        deviceInfo += QString::fromLocal8Bit("陀螺仪状态：")+(mEnableGyro?"Close\n":"Open\n");
        deviceInfo += QString::fromLocal8Bit("亮度信息：LEVEL_HIGH_")+QString::number(mBrightState)+"\n";
        if(bluelightCtrl()){
            bool enable=false;
            int nRet=0;
            if(svc_success==(nRet=svc_getBlueLightState(m_Handle,&enable))){
                qCritical()<<"svc_getBlueLightState"<<enable;
            }else{
                qCritical()<<"svc_getBlueLightState failed"<<nRet;
            }

            deviceInfo += QString::fromLocal8Bit("蓝光信息：")+(enable?"Open\n":"Close\n");
        }else{
            deviceInfo += QString::fromLocal8Bit("蓝光信息：不支持")+"\n";
        }
        if(backlightCtrl()){
            bool enable=false;
            int nRet=0;
            if(svc_success==(nRet=svc_getBackLightState(m_Handle,&enable))){
                qCritical()<<"svc_getBackLightState"<<enable;
            }else{
                qCritical()<<"svc_getBackLightState failed"<<nRet;
            }

            deviceInfo += QString::fromLocal8Bit("背光信息：")+(enable?"Open\n":"Close\n");

        }else{
            deviceInfo += QString::fromLocal8Bit("背光信息：不支持")+"\n";
        }
    }
    deviceInfo +=LINE1;

    deviceInfo +=QString::fromLocal8Bit("Screen信息\n");
    deviceInfo += LINE2;

    QDesktopWidget *desktop = QApplication::desktop();
    int screen_count = desktop->screenCount();
    int primaryScreen = desktop->primaryScreen();
    for(int i=0;i<screen_count;i++){
        LPCWSTR deviceName;
        if(i==0){
            deviceName = L"\\\\.\\DISPLAY1";
        }else if(i==1){
            deviceName = L"\\\\.\\DISPLAY2";
        }else if(i==2){
            deviceName = L"\\\\.\\DISPLAY3";
        }else{
            deviceName = L"\\\\.\\DISPLAY4";
        }
        DEVMODE devMode;
        bool bResult = EnumDisplaySettings(deviceName,ENUM_CURRENT_SETTINGS,&devMode);
        if(bResult){
            qCritical()<<"EnumDisplaySettings success"<<devMode.dmPelsWidth<<devMode.dmPelsHeight;
            deviceInfo +=QString::fromLocal8Bit("Screen %1：").arg(i+1)+((i==primaryScreen)?"Primary\n":"Extend\n");
            deviceInfo +=QString::fromLocal8Bit("分辨率：")+QString("%1x%2").arg(devMode.dmPelsWidth).arg(devMode.dmPelsHeight)+"\n";
            deviceInfo +=QString::fromLocal8Bit("刷新率：")+QString("%1").arg(devMode.dmDisplayFrequency).arg(devMode.dmPelsHeight)+"\n\n";
        }else{
            qCritical()<<"EnumDisplaySettings failed";
        }
    }

    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString fileName=dir.replace("PiPlay","Pimax")+"\\pi_device.txt";
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        qCritical()<<"write system file error"<<fileName;
        return;
    }
    file.write(deviceInfo.toUtf8());
    file.close();
}

bool PiService::SetBrightDefault()
{
    int value = 1;
    if(writeBrightLevel(value)){
        emit brightLevelChanged(value);
        return true;
    }
    return false;
}
bool PiService::SetRenderDefault()
{
	float value = 1.0;
	if (m_Handle&&displayQualityCtrl()) {
		m_nResult = svc_setDisplayQualityValue(m_Handle, value);
		if (m_nResult == svc_success) {
			qCritical() << "SetRenderDefault success" << value;
			setDisplayQuality(value);
			return true;
		}
		else {
			qCritical() << "SetRenderDefault failed" << m_nResult << value;
			return false;
		}
	}
	return false;
}

bool PiService::SetIpdDefault()
{
    float value = 63.0;
    if (m_Handle&&ipdCtrl()) {
        m_nResult = svc_setLensValue(m_Handle, value);
		if (m_nResult == svc_success) {
			qCritical() << "SetIpdDefault success" << value;
			mDisplayQuality = value;
			setIpdValue(value);
			return true;
		}
		else {
			qCritical() << "SetIpdDefault failed" << m_nResult << value;
			return false;
		}
	}
	return false;
}

void PiService::sendCmdShall(QString value)
{
    m_nResult = svc_setStringConfig(m_Handle, "firmware_shell_cmd", QByteArray(value.toLocal8Bit()).data());
    if (m_nResult = svc_success)
    {
        qCritical() << "firmware shell cmd success " << value;
    }
    else
    {
        qCritical() << "firmware shaell cmd error " << value;
    }
}

bool PiService::setContrastLeft(float value)
{
    if (value != m_contrastLeft)
    {
        m_nResult = svc_setVector3fConfig(m_Handle, "color_contrast_0_rgb", {value,value,value});
        if (m_nResult == svc_success)
        {
            qCritical() << "set color_contrast left success " << value;
        }
        else
        {
            qCritical() << "set color_contrast left error " << value;
            return false;
        }
        m_contrastLeft = value;
        emit contrastLeftChanged(value);
    }
    return true;
}

bool PiService::setContrastRight(float value)
{
    if (value != m_contrastRight)
    {
        m_nResult = svc_setVector3fConfig(m_Handle, "color_contrast_1_rgb", {value,value,value});
        if (m_nResult == svc_success)
        {
            qCritical() << "set color_contrast right success " << value;
        }
        else
        {
            qCritical() << "set color_contrast left error " << value;
            return false;
        }
        m_contrastRight = value;
        emit contrastRightChanged(value);
    }
    return true;
}

bool PiService::setBrightnessLeft(float value)
{
    if(value != m_brightnessLeft)
    {
        m_nResult = svc_setVector3fConfig(m_Handle, "color_brightness_0_rgb", {value,value,value});
        if (m_nResult == svc_success)
        {
            qCritical() << "set color_brightness left success " << value;
        }
        else
        {
            qCritical() << "set color_birghtness left error " << value;
            return false;
        }
        m_brightnessLeft = value;
        emit brightnessLeftChanged(value);
    }
    return true;
}

bool PiService::setBrightnessRight(float value)
{
    if (value != m_brightnessRight)
    {
        m_nResult = svc_setVector3fConfig(m_Handle, "color_brightness_1_rgb", {value,value,value});
        if (m_nResult == svc_success)
        {
            qCritical() << "set color_brightness right success " << value;
        }
        else
        {
            qCritical() << "set color_brightness right error " << value;
            return false;
        }
        m_brightnessRight = value;
        emit brightnessRightChanged(value);
    }
    return true;
}

bool PiService::setLenSeparation(float value)
{
    if(m_lensSeparation!=value){
        m_nResult = svc_setFloatConfig(m_Handle, "lens_separation_offset", value);
        if(m_nResult==svc_success)
        {
            qCritical() << "set lens_separation_offset success"<< value;
            m_lensSeparation = value;
            emit lensSeparationChanged(value);
        }
        else
        {
            qCritical() << "set lens_separation_offset failed" << value;
            return false;
        }
    }
    return true;
}

bool PiService::setSupportViveGame(bool enable)
{
    if(enable != m_supportViveGanme)
    {
        m_nResult = svc_setIntConfig(m_Handle, "support_vive_only_games", enable);
        if (m_nResult == svc_success)
        {
            qCritical() << "set support_vive_only_games success " << enable;
            m_supportViveGanme = enable;
            emit supportViveGanmeChanged(enable);
        }
        else
        {
            qCritical() << "set support_vive_only_games failed " << enable;
            return false;
        }
    }
    return true;
}
