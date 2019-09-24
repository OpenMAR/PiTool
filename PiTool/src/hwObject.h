#ifndef HWOBJECT_H
#define HWOBJECT_H

#include <QObject>
class HWLevelInfo: public QObject{
    Q_OBJECT
public:
    explicit HWLevelInfo(QObject *parent = 0);
    QString mMinLevel;
    QString mMaxLevel;
    QString mPrompt;
    int mMin;
    int mMax;
    bool loadJson(QJsonObject json);
};

class HWCapacityInfo: public QObject{
    Q_OBJECT
public:
    explicit HWCapacityInfo(QObject *parent = 0);
    bool loadJson(QJsonObject json);
    QStringList mHWEnum;
    QList<HWLevelInfo*> mLevelList;
};
class HWObject : public QObject
{
    Q_OBJECT
public:
    explicit HWObject(QObject *parent = 0);
    Q_PROPERTY(QString os READ os WRITE setOs NOTIFY osChanged)
    Q_PROPERTY(QString cpu READ cpu WRITE setCpu NOTIFY cpuChanged)
    Q_PROPERTY(QString memory READ memory WRITE setMemory NOTIFY memoryChanged)
    Q_PROPERTY(QString gpu READ gpu WRITE setGpu NOTIFY gpuChanged)
    Q_PROPERTY(QString gpuDriver READ gpuDriver WRITE setGpuDriver NOTIFY gpuDriverChanged)
    Q_PROPERTY(QString conclusion READ conclusion NOTIFY conclusionChanged)
    Q_PROPERTY(QString subConclusion READ subConclusion NOTIFY subConclusionChanged)
    Q_PROPERTY(QString osPrompt READ osPrompt NOTIFY osPromptChanged)
    Q_PROPERTY(QString cpuPrompt READ cpuPrompt NOTIFY cpuPromptChanged)
    Q_PROPERTY(QString gpuPrompt READ gpuPrompt NOTIFY gpuPromptChanged)
    Q_PROPERTY(QString driverPrompt READ driverPrompt NOTIFY driverPromptChanged)
    Q_PROPERTY(QString driverUrl READ driverUrl NOTIFY driverUrlChanged)
    Q_PROPERTY(bool osEnabled READ osEnabled NOTIFY osEnabledChanged)
    Q_PROPERTY(bool cpuEnabled READ cpuEnabled NOTIFY cpuEnabledChanged)
    Q_PROPERTY(bool gpuEnabled READ gpuEnabled NOTIFY gpuEnabledChanged)
    Q_PROPERTY(bool driverEnabled READ driverEnabled NOTIFY driverEnabledChanged)
    Q_PROPERTY(QString usbPrompt READ usbPrompt NOTIFY usbPromptChanged)
    Q_PROPERTY(QString hdmiPrompt READ hdmiPrompt NOTIFY hdmiPromptChanged)

    Q_PROPERTY(int capLevel READ capLevel NOTIFY capLevelChanged)
    Q_PROPERTY(bool checked READ checked NOTIFY checkedChanged)
    Q_INVOKABLE void checkPcConfigure();
    Q_INVOKABLE void initLevelInfo(int language);
    Q_INVOKABLE void writeSystemInfo();
    static bool bOnlyHasInterDisplayCard();

signals:
    void osChanged(QString os);
    void cpuChanged(QString cpu);
    void memoryChanged(QString memory);
    void gpuChanged(QString gpu);
    void gpuDriverChanged(QString gpuDriver);
    void conclusionChanged(QString conclusion);
    void subConclusionChanged(QString subConclusion);
    void capLevelChanged(int level);
    void checkedChanged(bool checked);
    void osPromptChanged(QString osPrompt);
    void cpuPromptChanged(QString cpuPrompt);
    void gpuPromptChanged(QString gpuPrompt);
    void driverPromptChanged(QString driverPrompt);
    void osEnabledChanged(bool enabled);
    void cpuEnabledChanged(bool enabled);
    void gpuEnabledChanged(bool enabled);
    void driverEnabledChanged(bool enabled);
    void driverUrlChanged(QString url);
    void usbPromptChanged(QString prompt);
    void hdmiPromptChanged(QString prompt);
public slots:

private:
    QString os();
    void setOs(QString os);
    QString cpu();
    void setCpu(QString cpu);
    QString memory();
    void setMemory(QString memory);
    QString gpu();
    void setGpu(QString gpu);
    QString gpuDriver();
    void setGpuDriver(QString gpuDriver);
    bool checked();
    QString conclusion();
    QString subConclusion();
    int capLevel();
    void setCapLevel();
    void setCpuLevel(int level);
    void setGpuLevel(int level);
    QString  osPrompt();
    QString  cpuPrompt();
    QString  gpuPrompt();
    QString  driverPrompt();
    QString  driverUrl();
    bool osEnabled();
    bool cpuEnabled();
    bool gpuEnabled();
    bool driverEnabled();

    void getgpu();
    int checkCpuLevel();
    int checkgpuLevel();
    bool checkDriverEnabled();
    bool checkOsEnabled();

    int getCapacityIndex(QStringList all,QString current);
    QString usbPrompt();
    QString hdmiPrompt();
    QString getComputeInfo();
    QString getMainboardInfo();
    QString getMemoryInfo();
    QString getHardDiskInfo();
    QString getCpuInfo();

private:
    int mCpuLevel;
    int mGpuLevel;
    int mCapLevel;
    bool mDriverEnabled;
    bool mOsEnabled;

    QString mOS;
    QString mCpu;
    QString mMemory;
    QString mGpu;
    QString mGpuVendor;
    QString mgpuDriver;
    HWCapacityInfo mGpuCap;
    HWCapacityInfo mCpuCap;
    QStringList mConclusion;
    QStringList mSubConclusion;

    int mDriverNVidia;
    int mDriverAmd;
    QString mDriverPrompt;
    QString mDriverPropmpt_I;
    QString mDriverPrompt_ok;
    QString mDriverURL_A;
    QString mDriverURL_N;
    QString mOsPropmpt;

    QString mUsbPropmpt;
    QString mHdmiPropmpt;
};

#endif // HWOBJECT_H
