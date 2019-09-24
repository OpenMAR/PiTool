#ifndef DIAGNOSEHANDLER_H
#define DIAGNOSEHANDLER_H

#include <QObject>
#include <QList>
#include <QQmlListProperty>

class gpuInfo{
public:
    QString name = "unknow";
    QString version = "unknow";
};


class DiagnoseHandler : public QObject
{
    Q_OBJECT
public:
    explicit DiagnoseHandler(QObject *parent = 0);

    Q_PROPERTY(bool appComponentMissed READ getAppComponentMissedState NOTIFY appComponentMissStateChanged)
    Q_PROPERTY(bool isNoteBook READ isNoteBookPC)
    Q_PROPERTY(QString cpu READ getCpuName)
    Q_PROPERTY(quint64 memory READ getMemoryCapacity)
    Q_PROPERTY(QString os_version READ getOsVersion)
    Q_PROPERTY(bool sp1_missed READ getSp1Missed)
    Q_PROPERTY(bool usb3_0 READ usb3_0Port NOTIFY usb3_0PortChanged)
    Q_PROPERTY(bool dfu_driver_installed READ dfuDriverInstalled NOTIFY dfuDriverChanged)
    Q_PROPERTY(bool gpuOnlyIntelCard READ onlyIntelCard NOTIFY onlyIntelCardChanged)
    Q_PROPERTY(bool gpuDriverInstalled READ haveGpuDriver NOTIFY gpuDriverInstalledChanged)
    Q_PROPERTY(int gpuSum READ getGpuSum NOTIFY gpuSumChanged)
    Q_PROPERTY(bool gpuVersionValid READ getGpuVersionValidState NOTIFY gpuVersionValidChanged)
    Q_PROPERTY(bool gpuVrReadyFor4K READ getGpuVrReadyFor4K NOTIFY gpuVrReadyFor4KChanged)
    Q_PROPERTY(bool gpuVersionValidFor4K READ getGpuVersionValidFor4K NOTIFY gpuVersionValidFor4KChanged)
    Q_PROPERTY(bool gpuVrReadyFor8K READ getGpuVrReadyFor8K NOTIFY gpuVrReadyFor8KChanged)
    Q_PROPERTY(bool gpuVersionValidFor8K READ getGpuVersionValidFor8K NOTIFY gpuVersionValidFor8KChanged)
    Q_PROPERTY(int patchMissedSum READ getPatchMissedSum NOTIFY patchMissedSumChanged)
    Q_PROPERTY(bool missedDlls READ systemDllsMissed NOTIFY missedDllsChanged)
    Q_PROPERTY(bool vcMissed READ getVcMissedStatus NOTIFY vcMissedChanged)
    Q_PROPERTY(int vcMissedCount READ vcListCount NOTIFY vcMissedCountChanged)
    Q_PROPERTY(bool dxFileMissed READ getDxFileMissedStatus NOTIFY dxFileMissedChanged)
    Q_PROPERTY(bool steamvrValid READ getSteamvrValidStatus NOTIFY steamvrValidChanged)
    Q_PROPERTY(bool serviceRunning READ serviceIsRunning NOTIFY serviceRunningChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(bool usbPlugin READ usbPlugedin NOTIFY usbPluginChanged)
    Q_PROPERTY(bool videoPlugin READ videoPluginedin NOTIFY videoPluginChanged)
    Q_PROPERTY(bool integratedGraphicsPlugin READ getIntegratedGraphicsPluginState NOTIFY integratedGraphicsPluginChanged)
    Q_PROPERTY(int videoOutputType READ getVideoOutputType NOTIFY videoOutputTypeChanged)
    Q_PROPERTY(QString hmdName READ getHmdName NOTIFY hmdNameChanged)
    Q_PROPERTY(QString hmdSN READ getHmdSN NOTIFY hmdSNChanged)
    Q_PROPERTY(QString hmdDriverVersion READ getHmdDriverVersion NOTIFY hmdDriverVersionChanged)
    Q_PROPERTY(int hmdMode READ getHmdMode NOTIFY hmdModeChanged)
    Q_INVOKABLE void allDiagnose();
    Q_INVOKABLE void hardwareDiagnose();
    Q_INVOKABLE void softwareDiagnose();
    Q_INVOKABLE QString vcList(int index){return vcMissedVersion.at(index);}
    Q_INVOKABLE QString patchList(int index){return missedPatchs.at(index);}
    Q_INVOKABLE QString getGpuName(int index){return gpus.at(index).name;}
    Q_INVOKABLE QString getGpuVersion(int index){return gpus.at(index).version;}
    Q_INVOKABLE bool startService();
    Q_INVOKABLE bool startServiceByName(QString name);
    Q_INVOKABLE bool LaunchDfuDriverSetupPackage();
    Q_INVOKABLE bool ovrServiceAlived();
    Q_INVOKABLE bool stopServiceByName(QString name);

public:
    bool getAppComponentMissedState(){return appComponentMissed;}
    bool isNoteBookPC(){return isNoteBook;}
    QString getCpuName(){return cpu;}
    quint64 getMemoryCapacity(){return memory;}
    QString getOsVersion(){return os_version;}
    bool getSp1Missed(){return sp1_missed;}
    bool usb3_0Port(){return usb3_0;}
    bool dfuDriverInstalled(){return dfu_driver_installed;}
    bool onlyIntelCard(){return gpuOnlyIntelCard;}
    bool haveGpuDriver(){return gpuDriverInstalled;}
    int getGpuSum(){return gpus.size();}
    bool getGpuVersionValidState(){return gpuVersionValid;}
    bool getGpuVrReadyFor4K(){return gpuVrReadyFor4K;}
    bool getGpuVersionValidFor4K(){return gpuVersionValidFor4K;}
    bool getGpuVrReadyFor8K(){return gpuVrReadyFor8K;}
    bool getGpuVersionValidFor8K(){return gpuVersionValidFor8K;}
    int getPatchMissedSum(){return missedPatchs.size();}
    bool systemDllsMissed(){return missedDlls;}
    bool getVcMissedStatus(){return vcMissed;}
    int vcListCount(){return vcMissedVersion.size();}
    bool getDxFileMissedStatus(){return dxFileMissed;}
    bool getSteamvrValidStatus(){return steamvrValid;}
    bool serviceIsRunning(){return serviceRunning;}
    bool isConnected(){return connected;}
    bool usbPlugedin(){return usbPlugin;}
    bool videoPluginedin(){return videoPlugin;}
    bool getIntegratedGraphicsPluginState(){return integratedGraphicsPlugin;}
    int getVideoOutputType(){return videoOutputType;}
    QString getHmdName(){return hmdName;}
    QString getHmdSN(){return hmdSN;}
    QString getHmdDriverVersion(){return hmdDriverVersion;}
    int getHmdMode(){return hmdMode;}

    typedef enum VideoConnectType_{
        ConnectorType_UNKNOWN,
        ConnectorType_VGA,
        ConnectorType_DVI_D,
        ConnectorType_DVI_I,
        ConnectorType_HDMI,
        ConnectorType_DP
    }VideoConnectType;

signals:
    void appComponentMissStateChanged(bool);
    void usb3_0PortChanged(bool);
    void dfuDriverChanged(bool);
    void onlyIntelCardChanged(bool);
    void gpuDriverInstalledChanged(bool);
    void gpuSumChanged(int);
    void gpuVersionValidChanged(bool);
    void gpuVrReadyFor4KChanged(bool);
    void gpuVersionValidFor4KChanged(bool);
    void gpuVrReadyFor8KChanged(bool);
    void gpuVersionValidFor8KChanged(bool);
    void patchMissedSumChanged(int);
    void missedDllsChanged(bool);
    void vcMissedChanged(bool);
    void vcMissedCountChanged(int);
    void dxFileMissedChanged(bool);
    void steamvrValidChanged(bool);
    void serviceRunningChanged(bool);
    void connectedChanged(bool);
    void usbPluginChanged(bool);
    void videoPluginChanged(bool);
    void integratedGraphicsPluginChanged(bool);
    void videoOutputTypeChanged(bool);
    void hmdNameChanged(QString);
    void hmdSNChanged(QString);
    void hmdDriverVersionChanged(QString);
    void hmdModeChanged(int);

public slots:

private:
    void initialize();
    void parseHardwareDiagnose(QJsonObject object);
    void parseSoftwareDiagnose(QJsonObject object);
    void parseDiagnoseResult(QJsonObject object);
    void analysisGpuDriver(const QList<gpuInfo> gpuList);
    bool containStr(QString mainStr, const QString* minorStrArray, int arraySum);
    bool amdVersionSupport4K(QString version);
    bool amdVersionSupport8K(QString version);
    bool nvidiaVersionSupport4K(QString version);
    bool nvidiaVersionSupport8K(QString version);

private:
    QString workDir;
    bool appComponentMissed;
    //pc:
    bool isNoteBook;
    QString cpu;
    quint64 memory;
    QString os_version;
    bool usb3_0;
    bool dfu_driver_installed;
    QList<gpuInfo> gpus;
    bool gpuOnlyIntelCard;
    bool gpuVersionValid;
    bool gpuVrReadyFor4K;//no used
    bool gpuVersionValidFor4K;//no used
    bool gpuVrReadyFor8K;//no used
    bool gpuVersionValidFor8K;//no used
    QList<QString> missedPatchs;
    bool sp1_missed;
    bool gpuDriverInstalled;
    //system
    bool missedDlls;
    //vc
    bool vcMissed;
    QList<QString> vcMissedVersion;
    //dx
    bool dxFileMissed;
    //steamvr
    bool steamvrValid;
    QString steamvrLogPath;
    QString steamvrConfigPath;
    //service
    bool serviceRunning;
    //hmd
    bool connected;
    bool usbPlugin;
    bool videoPlugin;
    bool integratedGraphicsPlugin;
    int videoOutputType;
    QString hmdName;
    QString hmdSN;
    QString hmdDriverVersion;
    int hmdMode;
};

#endif // DIAGNOSEHANDLER_H
