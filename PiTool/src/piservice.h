#ifndef PISERVICE_H
#define PISERVICE_H

#include <QObject>
#include <QLibrary>
#include <QVector>
#include <QString>
#include "piSvc/piSvcDesc.h"
#include "piSvc/piSvcCapability.h"
#include "piSvc/piSvcType.h"

class PiService : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(bool connect READ connect NOTIFY connectChanged)
    Q_PROPERTY(bool serviceState READ serviceState NOTIFY serviceStateChanged)
    Q_PROPERTY(bool usbState READ usbState NOTIFY usbStateChanged)
    Q_PROPERTY(bool hdmiState READ hdmiState NOTIFY hdmiStateChanged)
    Q_PROPERTY(bool noDfuDriver READ noDfuDriver NOTIFY noDfuDriverChanged)
    Q_PROPERTY(bool switchingMode READ switchingMode WRITE setSwitchingMode NOTIFY switchingModeChanged)
    Q_PROPERTY(bool forceUpgrading READ forceUpgrading NOTIFY forceUpgradingChanged)
    //controller connect state
    Q_PROPERTY(bool controllerPlugin READ controllerPlugin NOTIFY controllerPluginChanged)
    Q_PROPERTY(bool controllerState READ controllerState NOTIFY controllerStateChanged)
    Q_PROPERTY(QString controllerName READ controllerName NOTIFY controllerNameChanged)
    Q_PROPERTY(bool controller1 READ controller1 NOTIFY controller1Changed)
    Q_PROPERTY(bool controller2 READ controller2 NOTIFY controller2Changed)
    Q_PROPERTY(bool tracker1 READ tracker1 NOTIFY tracker1Changed)
    Q_PROPERTY(bool tracker2 READ tracker2 NOTIFY tracker2Changed)
    Q_PROPERTY(int controller1Battery READ controller1Battery NOTIFY controller1BatteryChanged)
    Q_PROPERTY(int controller2Battery READ controller2Battery NOTIFY controller2BatteryChanged)
    Q_PROPERTY(int tracker1Battery READ tracker1Battery NOTIFY tracker1BatteryChanged)
    Q_PROPERTY(int tracker2Battery READ tracker2Battery NOTIFY tracker2BatteryChanged)
    Q_PROPERTY(int noloDisplayMode READ noloDisplayMode NOTIFY noloDisplayModeChanged)
    Q_PROPERTY(bool supportModeSwitch READ supportModeSwitch NOTIFY supportModeSwitchChanged)
    Q_PROPERTY(bool trigger READ triggerPressedState NOTIFY triggerPressedChanged)

    Q_PROPERTY(bool modeCtrl READ modeCtrl NOTIFY modeCtrlChanged)
    Q_PROPERTY(bool backlightCtrl READ backlightCtrl NOTIFY backlightCtrlChanged)
    Q_PROPERTY(bool resolutionCtrl READ resolutionCtrl NOTIFY resolutionCtrlChanged)
    Q_PROPERTY(bool toneCtrl READ toneCtrl NOTIFY toneCtrlChanged)
    Q_PROPERTY(bool brightCtrl READ brightCtrl NOTIFY brightCtrlChanged)
    Q_PROPERTY(bool ipdCtrl READ ipdCtrl NOTIFY ipdCtrlChanged)
    Q_PROPERTY(bool bluelightCtrl READ bluelightCtrl NOTIFY bluelightCtrlChanged)
    Q_PROPERTY(bool proximityCtrl READ proximityCtrl NOTIFY proximityCtrlChanged)
    Q_PROPERTY(bool gyroCtrl READ gyroCtrl NOTIFY gyroCtrlChanged)
    Q_PROPERTY(bool eyePoseCtrl READ eyePoseCtrl NOTIFY eyePoseCtrlChanged)
    Q_PROPERTY(bool audioCtrl READ audioCtrl NOTIFY audioCtrlChanged)
    Q_PROPERTY(bool displayQualityCtrl READ displayQualityCtrl NOTIFY displayQualityCtrlChanged)
    Q_PROPERTY(bool rebootCtrl READ rebootCtrl NOTIFY rebootCtrlChanged)
    Q_PROPERTY(bool smartSmoothingEnable READ getSmartSmoothingEnable NOTIFY smartSmoothingEnableChanged)
    Q_PROPERTY(bool smartSmoothAvalible READ getSmartSmoothAvalibleState NOTIFY smartSmoothAvalibleStateChanged)
    Q_PROPERTY(bool smartSmoothActive READ getSmartSmoothActiveState NOTIFY smartSmoothActiveStateChanged)
    Q_PROPERTY(bool foveatedEnalbe READ getFoveatedEnable NOTIFY foveatedEnalbeChange)
    Q_PROPERTY(int foveatedStatus READ getFoveatedStatus NOTIFY foveatedStatusChange)
    Q_PROPERTY(int foveatedLevel READ getFoveatedLevel NOTIFY foveatedLevelChange)
    Q_PROPERTY(int foveatedSupport READ getFoveatedSupport NOTIFY foveatedSupportChange)

    Q_PROPERTY(QString hmd_name READ hmd_name NOTIFY hmdNameChanged)
    Q_PROPERTY(QString product_name READ product_name NOTIFY productNameChanged)
    Q_PROPERTY(QString serial_number READ serial_number NOTIFY serialNumberChanged)
    Q_PROPERTY(QString fmVersion READ fmVersion NOTIFY fmVersionChanged)
    Q_PROPERTY(int fmBuildVersion READ fmBuildVersion NOTIFY fmBuildVersionChanged)
    Q_PROPERTY(QString displayHmdName READ displayHmdName NOTIFY displayHmdNameChanged)

    Q_PROPERTY(int modeType READ modeType NOTIFY modeTypeChanged)
    Q_PROPERTY(bool pimaxMode READ isPimaxMode)
    Q_PROPERTY(bool enableGyro READ enableGyro NOTIFY enableGyroChanged)
    Q_PROPERTY(bool coolTone READ coolTone NOTIFY coolToneChanged)
    Q_PROPERTY(int brightLevel READ brightLevel NOTIFY brightLevelChanged)
    Q_PROPERTY(int minBrightLevel READ minBrightLevel NOTIFY minBrightLevelChanged)
    Q_PROPERTY(int maxBrightLevel READ maxBrightLevel NOTIFY maxBrightLevelChanged)
    Q_PROPERTY(int resolutionCap READ getResolutionCap NOTIFY resolutionCapChanged)
    Q_PROPERTY(int resolution READ getResolution NOTIFY resolutionChanged)
    Q_PROPERTY(int resolutionIndex READ getResolutionIndexForQml NOTIFY resolutionIndexChanged)
    Q_PROPERTY(int refreshCounts READ getRefreshCounts NOTIFY refreshCountsChanged)
    Q_PROPERTY(int refreshIndex READ getRefreshIndex NOTIFY refreshIndexChanged)
    Q_PROPERTY(QStringList refreshValues READ getRefreshValues NOTIFY refreshValuesChanged)


    Q_PROPERTY(bool lowerResolution READ lowerResolution NOTIFY lowerResolutionChanged)
    Q_PROPERTY(float displayQuality READ displayQuality NOTIFY displayQualityChanged)
    Q_PROPERTY(float ipd READ getIpdValue NOTIFY ipdChanged)
    Q_PROPERTY(bool audioPimax READ audioPimax NOTIFY audioPimaxChanged)
    Q_PROPERTY(bool spaceCalibrated READ spaceCalibrated NOTIFY spaceCalibratedChanged)
    Q_PROPERTY(bool headTracked READ headTracked NOTIFY headTrackedChanged)
    Q_PROPERTY(bool controller0Tracked READ controller0Tracked NOTIFY controller0TrackedChanged)
    Q_PROPERTY(bool controller1Tracked READ controller1Tracked NOTIFY controller1TrackedChanged)
    Q_PROPERTY(bool base0Tracked READ base0Tracked NOTIFY base0TrackedChanged)
    Q_PROPERTY(bool base1Tracked READ base1Tracked NOTIFY base1TrackedChanged)
    Q_PROPERTY(bool controllerPairSupport READ getControllerPairSupport NOTIFY controllerPairSupportChanged)
    Q_PROPERTY(int pairStatus READ getPairStatus WRITE setPairStatus NOTIFY pairStatusChanged)
    Q_PROPERTY(bool leapMotionPlugin READ getLeapMotionPluginStatus NOTIFY leapMotionPluginChanged)
    Q_PROPERTY(bool leapMotionInstalled READ getLeapMotionIntalledStatus NOTIFY leapMotionIntalledChanged)
    Q_PROPERTY(bool leapMotionServiceReady READ getLeapMotionServiceReadyStatus NOTIFY leapMotionServiceReadyChanged)
    Q_PROPERTY(bool leapMotionDeviceReady READ getLeapMotionDeviceReadyStatus NOTIFY leapMotionDeviceReadyChanged)
    Q_PROPERTY(bool lighthouseEnabled READ getLighthouseEnabledStatus WRITE setLighthouseEnabledStatus NOTIFY lighthouseEnabledChanged)
    Q_PROPERTY(bool pvrHomeEnabled READ getPvrHomeEnabledStatus NOTIFY pvrHomeEnabledChanged)
    Q_PROPERTY(bool compatibleSteamvrUsingParallel READ getCompatibleSteamvrUsingParallel NOTIFY compatibleSteamvrUsingParallelChanged)
    Q_PROPERTY(int fovType READ getFovType NOTIFY fovTypeChanged)
    Q_PROPERTY(bool hiddenAreanEnabled READ gethiddenAreanEnabled NOTIFY hiddenAreanEnabledChanged)
    Q_PROPERTY(bool recenterSupported READ getRecenterSupported NOTIFY recenterSupportedChanged)
    Q_PROPERTY(int errorCode READ getErrorCode NOTIFY errorCodeChanged)

    Q_PROPERTY(float m_contrastLeft READ contrastLeft NOTIFY contrastLeftChanged)
    Q_PROPERTY(float m_contrastRight READ contrastRight NOTIFY contrastRightChanged)
    Q_PROPERTY(float m_brightnessLeft READ brightnessLeft NOTIFY brightnessLeftChanged)
    Q_PROPERTY(float m_brightnessRight READ brightnessRight NOTIFY brightnessRightChanged)
    Q_PROPERTY(float m_lensSeparation READ lensSeparation NOTIFY lensSeparationChanged)
    Q_PROPERTY(bool m_supportViveGanme READ supportViveGanme NOTIFY supportViveGanmeChanged)

    Q_INVOKABLE bool setContrastLeft(float value);
    Q_INVOKABLE bool setContrastRight(float value);
    Q_INVOKABLE bool setBrightnessLeft(float value);
    Q_INVOKABLE bool setBrightnessRight(float value);
    Q_INVOKABLE bool setLenSeparation(float value);
    Q_INVOKABLE void startPairController();
    Q_INVOKABLE void stopPairController();
    Q_INVOKABLE void startRoomSettingNoteInHmd(bool isChinese);
    Q_INVOKABLE void stopRoomSettingNoteInHmd();
    Q_INVOKABLE bool getHmdPosition();
    Q_INVOKABLE bool hmdNoMoving();
    Q_INVOKABLE bool supportRecenter();
    Q_INVOKABLE void rebootPiService();
    Q_INVOKABLE void checkConnectStatus();
    Q_INVOKABLE bool rebootHmd();
    Q_INVOKABLE bool resetEyePose();
    Q_INVOKABLE bool changeModeType(int mode);
    Q_INVOKABLE bool resetFactory();
    Q_INVOKABLE bool writeEnableGyro(bool enable);
    Q_INVOKABLE bool writeCoolTone(bool enable);
    Q_INVOKABLE bool writeResolution(int resolution);
    Q_INVOKABLE bool writeBrightLevel(int value);
    Q_INVOKABLE bool setRefreshByIndex(int index);
    Q_INVOKABLE void enableMaxBrightLevel();
    Q_INVOKABLE bool writeDisplayQuality(float value);
    Q_INVOKABLE bool writeIpdValue(float value);
    Q_INVOKABLE bool writeAudioPimax(bool enable);
    Q_INVOKABLE bool setNoloDisplayMode(int mode);
    Q_INVOKABLE bool slopeCalibration();
    Q_INVOKABLE bool setLighthouseEnabled(bool enabled);
    Q_INVOKABLE bool setPvrHomeEnabled(bool enabled);
    Q_INVOKABLE bool setCompatibleSteamvrUsingParallel(bool enabled);
    Q_INVOKABLE bool setFovType(int type);
    Q_INVOKABLE bool EnablehiddenArea(bool enabled);

    Q_INVOKABLE bool upgradeFirmware(QString fmFile);
    Q_INVOKABLE bool forceUpgradeFirmware();
    Q_INVOKABLE int getModeType();
    Q_INVOKABLE void checkControllerState();
    Q_INVOKABLE void startMagnetCalibrate();
    Q_INVOKABLE bool isCalibrateFinished();
    Q_INVOKABLE bool spaceCalibration(float height);
    Q_INVOKABLE bool centerCalibration();
    Q_INVOKABLE bool floorCalibration(float height);
    Q_INVOKABLE void switchScreenOrient();

    Q_INVOKABLE void setPairIndex(int index);
    Q_INVOKABLE QString getError();
    Q_INVOKABLE bool hasNoError();

    Q_INVOKABLE bool isNotebook();
    Q_INVOKABLE void writeDeviceInfo();

	Q_INVOKABLE bool SetBrightDefault();
	Q_INVOKABLE bool SetRenderDefault();
    Q_INVOKABLE bool SetIpdDefault();

    Q_INVOKABLE bool setSmartSmoothingEnable(bool enable);
    Q_INVOKABLE void setFoveatedEnable(bool enalbe);
    Q_INVOKABLE bool supportFoveated();
    Q_INVOKABLE void setFoveatedLevel(int level);
    Q_INVOKABLE void sendCmdShall(QString value);
    Q_INVOKABLE bool setSupportViveGame(bool enable);

    static PiService* getInstance();

public:
    explicit PiService(QObject *parent = 0);
    ~PiService();

private:
    piResolutionCapabilityFlag resolutionValueToFlag(int resolution);
    int getResolutionIndex(int resolution);
    void initResolutionVector();
    void getHmdInfo();
    void clearDeviceParam();
    void loadDeviceParam();
    void shutdown();
    void updateRefreshBuf();
    void getSmartSmoothingStatus();
    int getFoveatedStatusInfo();

signals:
    void connectChanged(bool connect);
    void serviceStateChanged(bool state);
    void usbStateChanged(bool usbState);
    void hdmiStateChanged(bool hdmiState);
    void noDfuDriverChanged(bool state);
    void switchingModeChanged(bool state);
    void forceUpgradingChanged(bool state);
    //controller connect state
    void controllerPluginChanged(bool state);
    void controllerStateChanged(bool state);
    void controllerNameChanged(QString name);
    void controller1Changed(bool state);
    void controller2Changed(bool state);
    void tracker1Changed(bool state);
    void tracker2Changed(bool state);
    void controller1BatteryChanged(int battery);
    void controller2BatteryChanged(int battery);
    void tracker1BatteryChanged(int battery);
    void tracker2BatteryChanged(int battery);
    void noloDisplayModeChanged(int mode);
    void supportModeSwitchChanged(bool state);
    void triggerPressedChanged(bool state);

    void modeCtrlChanged(bool enable);
    void backlightCtrlChanged(bool enable);
    void resolutionCtrlChanged(bool enable);
    void toneCtrlChanged(bool enable);
    void brightCtrlChanged(bool enable);
    void ipdCtrlChanged(bool enable);
    void bluelightCtrlChanged(bool enable);
    void proximityCtrlChanged(bool enable);
    void gyroCtrlChanged(bool enable);
    void eyePoseCtrlChanged(bool enable);
    void audioCtrlChanged(bool enable);
    void displayQualityCtrlChanged(bool enable);
    void rebootCtrlChanged(bool enable);

    void hmdNameChanged(QString hmd_name);
    void productNameChanged(QString product_name);
    void serialNumberChanged(QString serial_number);
    void fmVersionChanged(QString version);
    void fmBuildVersionChanged(int buildVersion);
    void displayHmdNameChanged(QString name);

    void modeTypeChanged(int mode);
    void enableGyroChanged(bool enable);
    void coolToneChanged(bool enable);
    void brightLevelChanged(int value);
    void minBrightLevelChanged(int value);
    void maxBrightLevelChanged(int value);
    void lowerResolutionChanged(bool enable);
    void refreshCountsChanged(int value);
    void refreshIndexChanged(int index);
    void refreshValuesChanged(QStringList value);
    void displayQualityChanged(float value);
    void ipdChanged(float value);
    void audioPimaxChanged(bool enable);
    void resolutionCapChanged(int cap);
    void resolutionChanged(int resolution);
    void resolutionIndexChanged(int index);
    void spaceCalibratedChanged(bool value);
    void headTrackedChanged(bool value);
    void controller0TrackedChanged(bool value);
    void controller1TrackedChanged(bool value);
    void base0TrackedChanged(bool value);
    void base1TrackedChanged(bool value);
    void controllerPairSupportChanged(bool value);
    void pairStatusChanged(int value);
    void leapMotionPluginChanged(bool);
    void leapMotionIntalledChanged(bool);
    void leapMotionServiceReadyChanged(bool);
    void leapMotionDeviceReadyChanged(bool);
    void lighthouseEnabledChanged(bool);
    void pvrHomeEnabledChanged(bool);
    void compatibleSteamvrUsingParallelChanged(bool);
    void fovTypeChanged(int);
    void hiddenAreanEnabledChanged(bool);
    void recenterSupportedChanged(bool);
    void errorCodeChanged(int value);
    void smartSmoothingEnableChanged(bool);
    void smartSmoothAvalibleStateChanged(bool);
    void smartSmoothActiveStateChanged(bool);
    void foveatedEnalbeChange(bool);
    void foveatedAvctiveChanged(bool);
    void foveatedStatusChange(int);
    void foveatedLevelChange(int);
    void foveatedSupportChange(bool);
    void contrastLeftChanged(float);
    void contrastRightChanged(float);
    void brightnessLeftChanged(float);
    void brightnessRightChanged(float);
    void lensSeparationChanged(float);
    void supportViveGanmeChanged(bool);

public slots:

public:
    void setConnect(bool connect);
    void setServiceState(bool connect);
    void setUsbState(bool usbState);
    void setHdmiState(bool hdmiState);

public:
    bool connect(){return mbConnected;}
    bool serviceState(){return mbServiceConnect;}
    bool usbState(){return mbUsbConnect;}
    bool hdmiState(){return mbHdmiConnect;}
    bool noDfuDriver(){return mNoDfuDriver;}
    bool switchingMode(){return mSwitchingMode;}
    bool supportModeSwitch(){return false;}
    bool triggerPressedState (){return triggerPressed;}
    bool forceUpgrading(){return mForceUpgrading;}
    //controller connect state
    bool controllerPlugin(){return mControllerPlugin;}
    bool controllerState(){return mControllerState;}
    QString controllerName(){return mControllerName;}
    bool controller1(){return mTrackedDeviceState&TrackedDevice_LeftController;}
    bool controller2(){return mTrackedDeviceState&TrackedDevice_RightController;}
    bool tracker1(){return mTrackedDeviceState&TrackedDevice_Tracker0;}
    bool tracker2(){return mTrackedDeviceState&TrackedDevice_Tracker1;}
    int controller1Battery(){return mController1Battery;}
    int controller2Battery(){return mController2Battery;}
    int tracker1Battery(){return mTracker1Battery;}
    int tracker2Battery(){return mTracker2Battery;}
    int noloDisplayMode(){return mNoloDisplayMode;}

    bool modeCtrl(){return (mHmdCaps&piHmdCap_ModeCtrl)>0;}
    bool backlightCtrl(){return (mHmdCaps&piHmdCap_BackLightCtrl)>0;}
    bool resolutionCtrl(){return (mHmdCaps&piHmdCap_ResolutionCtrl)>0;}
    bool toneCtrl(){return (mHmdCaps&piHmdCap_ToneCtrl)>0;}
    bool brightCtrl(){return (mHmdCaps&piHmdCap_BrightCtrl)>0;}
    bool ipdCtrl(){return (mHmdCaps&piHmdCap_IpdAdjust)>0;}
    bool bluelightCtrl(){return (mHmdCaps&piHmdCap_BlueLightCtrl)>0;}
    bool proximityCtrl(){return (mHmdCaps&piHmdCap_ProximityCtrl)>0;}
    bool gyroCtrl(){return (mHmdCaps&piHmdCap_GyroCtrl)>0;}
    bool eyePoseCtrl(){return (mHmdCaps&piHmdCap_EyePoseCtrl)>0;}
    bool audioCtrl(){return (mHmdCaps&piHmdCap_AudioCtrl)>0;}
    bool displayQualityCtrl(){return (mHmdCaps&piHmdCap_DisplayQualityCtrl)>0;}
    bool rebootCtrl(){return (mHmdCaps&piHmdCap_RebootCtrl)>0;}

    QString hmd_name(){return mHmdName;}
    QString product_name(){return mProductName;}
    QString serial_number(){return mSerialNumber;}
    QString fmVersion(){return mFmVersion;}
    int fmBuildVersion(){return mFmBuildVersion;}
    QString displayHmdName(){return mDisplayName;}

    int modeType(){return mModeType==mode_State_video_p?1:0;}
    bool isPimaxMode(){return mModeType==mode_State_pimax_smart;}
    bool enableGyro(){return mEnableGyro;}
    bool coolTone(){return mToneState==TONE_COOL;}
    int brightLevel();//{return mBrightState;}
    int minBrightLevel(){return mMinBrightLevel;}
    int maxBrightLevel(){return mMaxBrightLevel;}
    bool lowerResolution(){return mResolution==e_1080_1920;}
    int getRefreshCounts();
    int getRefreshIndex();
    QStringList getRefreshValues(){return refreshBuf;}
    float displayQuality(){return mDisplayQuality;}
    float getIpdValue(){return mIpd;}
    bool audioPimax(){return mAudioPimax;}
    bool spaceCalibrated(){return mSpaceCalibrated;}
    bool headTracked(){return mHeadTracked;}
    bool controller0Tracked(){return mController0Tracked;}
    bool controller1Tracked(){return mController1Tracked;}
    bool base0Tracked(){return mBase0Tracked;}
    bool base1Tracked(){return mBase1Tracked;}
    bool getControllerPairSupport(){return controllerPairSupport;}
    int getPairStatus(){return pairStatus;}
    void setPairStatus(int state);
    bool getLeapMotionPluginStatus(){return leapMotionPlugin;}
    bool getLeapMotionIntalledStatus(){return leapMotionInstalled;}
    bool getLeapMotionServiceReadyStatus(){return leapMotionServiceReady;}
    bool getLeapMotionDeviceReadyStatus(){return leapMotionDeviceReady;}
    bool getLighthouseEnabledStatus(){return lighthouseEnabled;}
    void setLighthouseEnabledStatus(bool enabled){lighthouseEnabled = enabled;}
    bool getPvrHomeEnabledStatus(){return pvrHomeEnabled;}
    bool getCompatibleSteamvrUsingParallel(){return compatibleSteamvrUsingParallel;}
    int getFovType(){return fovType;}
    bool gethiddenAreanEnabled(){return hiddenAreanEnabled;}
    bool getRecenterSupported(){return recenterSupported;}
    int getErrorCode(){return m_errorCode;}
    int getResolutionCap(){return mResolutionCap;}
    int getResolution(){return mResolution;}
    int getResolutionIndexForQml(){return mResolutionIndex;}
    bool getSmartSmoothingEnable(){return m_smartSmoothingEnable;}
    bool getSmartSmoothAvalibleState(){return m_smartSmoothingAvailable;}
    bool getSmartSmoothActiveState(){return m_smartSmoothingActive;}
    bool getFoveatedEnable() {return foveatedEnable;}
    int getFoveatedStatus(){return foveatedStatus;}
    int getFoveatedLevel() {return foveatedLevel;}
    int getFoveatedSupport(){return supportFoveatedStatus;}
    float contrastLeft() {return m_contrastLeft;}
    float contrastRight() {return m_contrastRight;}
    float brightnessLeft() {return m_brightnessLeft;}
    float brightnessRight() {return m_brightnessRight;}
    float lensSeparation() {return m_lensSeparation;}
    bool supportViveGanme() {return m_supportViveGanme;}
    //controller connect state
    void setControllerConnectState(bool state);
    void setControllerPlugin(bool state);
    void setTrackedDeviceState(int state);
    void setDfuDriverState(bool state);
    void setSwitchingMode(bool state);
    void setHmdCap(int cap);
    void setHmdName(QString name);
    void setProductName(QString product_name);
    void setSerialNumber(QString serial_number);
    void setFmVersion(QString version);
    void setFmBuildVersion(int version);

    void setModeType(int mode);
    void setEnableGyro(bool enable);
    void setToneState(ToneState state);
    void setBrightState(BrightState state);
    void setResolution(int value);
    void setDisplayQuality(float value);
    void setIpdValue(float value);
    void setAudioPimax(bool enable);

    static void __stdcall PiService_Callback(piServiceMessage message,piWPARAM wParam,piLPARAM lParam);

    typedef enum PairStatus_{
        Pairing,
        Paired_Zero,
        Paired_One,
        Paired_Failed,
        Paired_Dongle_Count,

        Paired_Unknow
    }PairStatus;

    typedef enum ErrCode_{
        NONE,
        SERVICE_NOT_RUNNING = 30000,
        START_RPC_FAILED = 30100,
        MODE_NOT_SUPPORTED = 30200,

        UNKNOW = NONE
    }ErrCode;

private:
    QLibrary mPiSvrLib;
    piSvcHandle m_Handle;
    piSvcResult m_nResult;

    bool mbConnected;
    bool mbServiceConnect;
    bool mbUsbConnect;
    bool mbHdmiConnect;
    bool mNoDfuDriver;
    bool mSwitchingMode;
    bool mForceUpgrading;
    QString mForcePath;
    //TrackedDeviceType
    bool mControllerState;
    bool mControllerPlugin;
    QString mControllerName;
    int mTrackedDeviceState;
    int mController1Battery;
    int mController2Battery;
    int mTracker1Battery;
    int mTracker2Battery;
    int mNoloDisplayMode;
    int mHmdCaps;
    bool mHmdInfoAvaliable;
    QString mHmdName;
    QString mProductName;
    QString mSerialNumber;
    QString mFmVersion;
    int mFmBuildVersion;
    QString mDisplayName;

    int mModeType;
    bool mEnableGyro;
    ToneState mToneState;
    BrightState mBrightState;
    int mMinBrightLevel;
    int mMaxBrightLevel;
    int mResolution;
    int mResolutionIndex;
    int mResolutionCap;
    QVector<int> mResolutionVector;
    float mDisplayQuality;
    float mIpd;
    bool mAudioPimax;
    bool mSpaceCalibrated;
    bool mHeadTracked;
    bool mController0Tracked;
    bool mController1Tracked;
    bool mBase0Tracked;
    bool mBase1Tracked;
    bool controllerPairSupport;
    int pairStatus;
    int pairIndex;

    //leap motion
    bool leapMotionPlugin;
    bool leapMotionInstalled;
    bool leapMotionServiceReady;
    bool leapMotionDeviceReady;

    //lighthouse
    bool lighthouseEnabled;

    bool recenterSupported;

    int fovType;
    bool hiddenAreanEnabled;

    //pvr home
    bool pvrHomeEnabled;

    //steamvr game compatible
    bool compatibleSteamvrUsingParallel;

    //for room setting
    float hmdX;
    float hmdY;
    float hmdZ;
    const float deviation = 0.1;

    bool triggerPressed;

    //error code
    int m_errorCode;
    QStringList refreshBuf;

    //smart fill the frame
    bool m_smartSmoothingEnable;
    bool m_smartSmoothingActive;
    bool m_smartSmoothingAvailable;

    bool foveatedEnable;
    bool foveatedActive;
    int foveatedStatus; //0 close 1 inactive 2 active
    int foveatedLevel; 
    bool supportFoveatedStatus;

    float m_contrastLeft;
    float m_contrastRight;
    float m_brightnessLeft;
    float m_brightnessRight;
    float m_lensSeparation;
    bool m_supportViveGanme;
};

#endif // PISERVICE_H
