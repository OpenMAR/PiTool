#include "hwObject.h"
#include "Windows.h"
#include <QDebug>
#include <QSysInfo>
#include <QSettings>
#include "JsonHandler.h"
#include <QTime>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include "RegisterHandler.h"
#include "Setting.h"

HWLevelInfo::HWLevelInfo(QObject *parent) : QObject(parent){
    mMin = 0;
    mMax = 0;
}

bool HWLevelInfo::loadJson(QJsonObject json){
    mMinLevel = JsonHandler::getJsonValue(json,"min","");
    mMaxLevel = JsonHandler::getJsonValue(json,"max","");
    if(mMinLevel.isEmpty()&&mMaxLevel.isEmpty()){
        qInfo()<<"HWLevelInfo::loadJson min or max empty";
        return false;
    }
    mPrompt = JsonHandler::getJsonValue(json,"prompt","");
    if(mPrompt.isEmpty()){
        qInfo()<<"HWLevelInfo::loadJson prompt or prompt_en empty";
        return false;
    }
    return true;
}

HWCapacityInfo::HWCapacityInfo(QObject *parent) : QObject(parent)
{
    mHWEnum.empty();
    mLevelList.empty();
}

bool HWCapacityInfo::loadJson(QJsonObject json){
    QString ladderFile = JsonHandler::getJsonValue(json,"ladder","");
    if(ladderFile.isEmpty()){
        qInfo()<<"HWCapacityInfo::loadJson ladder is empty";
        return false;
    }
    QString ladder = Setting::loadText(ladderFile).toLower();
    if(ladder.isEmpty()){
        qInfo()<<"HWCapacityInfo::loadJson ladder is empty";
        return false;
    }
    ladder = ladder.replace("\n","");
    mHWEnum = ladder.split(",");
    QJsonArray jsonDef;
    QJsonArray jsonLevel = JsonHandler::getJsonValue(json,"level",jsonDef);
    mLevelList.clear();
    for(int i=0;i<jsonLevel.size();i++){
        HWLevelInfo *level = new HWLevelInfo();
        if(level->loadJson(jsonLevel.at(i).toObject())){
            mLevelList.append(level);
            if(!level->mMinLevel.isEmpty()&&mHWEnum.contains(level->mMinLevel)){
                level->mMin = mHWEnum.indexOf(level->mMinLevel);
            }else {
                level->mMin = mHWEnum.size()+2;
            }
            if(!level->mMaxLevel.isEmpty()&&mHWEnum.contains(level->mMaxLevel)){
                level->mMax = mHWEnum.indexOf(level->mMaxLevel);
            }else {
                level->mMax = 0;
            }
            qInfo()<<level->mPrompt<<level->mMin<<level->mMax;
        }else{
            delete level;
        }
    }
    if(mHWEnum.size()==0||mLevelList.size()==0){
        qInfo()<<"HWCapacityInfo::loadJson mHWEnum or mLevelList size is empty";
        return false;
    }
    return true;
}

HWObject::HWObject(QObject *parent) : QObject(parent)
{
    mOS = "";
    mCpu = "";
    mGpu = "";
    mgpuDriver = "";
    mCpuLevel = 0;
    mGpuLevel = 0;
    mCapLevel = 0;
    mDriverEnabled = false;
    mOsEnabled = false;
//    initLevelInfo(Setting::getLanguage());
}
void HWObject::initLevelInfo(int language){
    QString fileName = ":/performance.txt";
    if(language==1){
        fileName = ":/performance_en.txt";
    }
    qInfo()<<fileName;
    QJsonObject jsonRoot = JsonHandler::loadJsonObject(fileName);
    jsonRoot = JsonHandler::getJsonObjectValue(jsonRoot,"performance",jsonRoot);
    QJsonObject jsonDef;
    QJsonArray jsonArrayDef;
    mGpuCap.loadJson(JsonHandler::getJsonObjectValue(jsonRoot,"gpu",jsonDef));
    mCpuCap.loadJson(JsonHandler::getJsonObjectValue(jsonRoot,"cpu",jsonDef));
    QJsonObject jsonPrompt = JsonHandler::getJsonObjectValue(jsonRoot,"prompt",jsonDef);
    QJsonArray jsonConclusion = JsonHandler::getJsonValue(jsonPrompt,"conclusion",jsonArrayDef);
    mConclusion.clear();
    for(int i=0;i<jsonConclusion.size();i++){
        mConclusion.append(JsonHandler::getJsonValue(jsonConclusion.at(i).toObject(),"prompt",""));
    }
    QJsonArray jsonSubConclusion = JsonHandler::getJsonValue(jsonPrompt,"subconclusion",jsonArrayDef);
    mSubConclusion.clear();
    for(int i=0;i<jsonSubConclusion.size();i++){
        mSubConclusion.append(JsonHandler::getJsonValue(jsonSubConclusion.at(i).toObject(),"prompt",""));
    }
    QJsonObject jsonOs = JsonHandler::getJsonObjectValue(jsonRoot,"os",jsonDef);
    mOsPropmpt = JsonHandler::getJsonValue(jsonOs,"prompt","");

    QJsonObject jsonDriver = JsonHandler::getJsonObjectValue(jsonRoot,"driver",jsonDef);
    mDriverNVidia = JsonHandler::getJsonValue(jsonDriver,"nvidia",0);
    mDriverAmd = JsonHandler::getJsonValue(jsonDriver,"amd",0);
    mDriverPrompt = JsonHandler::getJsonValue(jsonDriver,"prompt","");\
    mDriverPropmpt_I = JsonHandler::getJsonValue(jsonDriver,"prompt_i","");\
    mDriverPrompt_ok = JsonHandler::getJsonValue(jsonDriver,"prompt_ok","");
    mDriverURL_N = JsonHandler::getJsonValue(jsonDriver,"url_n","");
    mDriverURL_A = JsonHandler::getJsonValue(jsonDriver,"url_a","");

    mUsbPropmpt = JsonHandler::getJsonValue(jsonRoot,"usb_prompt","");
    mHdmiPropmpt = JsonHandler::getJsonValue(jsonRoot,"hdmi_prompt","");
}

QString HWObject::os(){
    return mOS;
}
void HWObject::setOs(QString os){
    if(mOS!=os){
        qInfo()<<"setOs "<<os;
        mOS = os;
        emit osChanged(mOS);
        emit checkedChanged(checked());
    }
}
QString HWObject::cpu(){
    return mCpu;
}
void HWObject::setCpu(QString cpu){
    if(mCpu!=cpu){
        mCpu = cpu;
        emit cpuChanged(mCpu);
        emit checkedChanged(checked());
    }
}
QString HWObject::memory(){
    return mMemory;
}
void HWObject::setMemory(QString memory){
    if(mMemory!=memory){
        mMemory = memory;
        emit memoryChanged(mMemory);
        emit checkedChanged(checked());
    }
}
QString HWObject::gpu(){
    return mGpu;
}
void HWObject::setGpu(QString gpu){
    if(mGpu!=gpu){
        mGpu = gpu;
        emit gpuChanged(mGpu);
        emit checkedChanged(checked());
    }
}
QString HWObject::gpuDriver(){
    return mgpuDriver;
}
void HWObject::setGpuDriver(QString gpuDriver){
    if(mgpuDriver!=gpuDriver){
        mgpuDriver = gpuDriver;
        emit gpuDriverChanged(mgpuDriver);
        emit checkedChanged(checked());
    }
}
bool HWObject::checked(){
    return mOS!=""&&mCpu!=""&&mGpu!=""&&mgpuDriver!="";
}
QString HWObject::conclusion(){
    return mConclusion.at(mCapLevel);
}

QString HWObject::subConclusion(){
    return mSubConclusion.at(mCapLevel);
}


void HWObject::setCpuLevel(int level){
    if(mCpuLevel!=level){
        mCpuLevel = level;
        emit cpuEnabledChanged(cpuEnabled());
        emit cpuPromptChanged(cpuPrompt());
    }
}

void HWObject::setGpuLevel(int level){
    if(mGpuLevel!=level){
        mGpuLevel = level;
        emit gpuEnabledChanged(gpuEnabled());
        emit gpuPromptChanged(gpuPrompt());
    }
}

int HWObject::capLevel(){
    return mCapLevel;
}

void HWObject::setCapLevel(){
    int capLevel = mGpuLevel>mCpuLevel?mCpuLevel:mGpuLevel;
    if(capLevel!=mCapLevel){
        mCapLevel = capLevel;
        emit capLevelChanged(capLevel);
        emit conclusionChanged(conclusion());
        emit subConclusionChanged(subConclusion());
    }
}

QString  HWObject::osPrompt(){
    if(mOsEnabled){
        return "";
    }
    return mOsPropmpt;
}

QString  HWObject::cpuPrompt(){
    return mCpuCap.mLevelList.at(mCpuLevel)->mPrompt;
}

QString  HWObject::gpuPrompt(){
    return mGpuCap.mLevelList.at(mGpuLevel)->mPrompt;
}

QString  HWObject::driverPrompt(){
    if(mGpu.isEmpty()||mgpuDriver.isEmpty()){
        return "";
    }
    QString lower = mGpu.toLower();
    QString prompt = mDriverPrompt;
    if(mDriverEnabled){
        prompt =mDriverPrompt_ok;
    }
    if(lower.contains("nvidia")){
        return prompt.replace("%1",mDriverURL_N);
    }else if(lower.contains("amd")){
        return prompt.replace("%1",mDriverURL_A);
    }else{
        return mDriverPropmpt_I;
    }
}

QString  HWObject::driverUrl(){
    QString lower = mGpu.toLower();
    if(lower.contains("amd")){
        return mDriverURL_A;
    }else if(lower.contains("nvidia")){
        return mDriverURL_N;
    }else{
        return "";
    }
}


bool HWObject::osEnabled(){
    return mOsEnabled;
}
bool HWObject::cpuEnabled(){
    return mCpuLevel>1;
}
bool HWObject::gpuEnabled(){
    return mGpuLevel>1;
}
bool HWObject::driverEnabled(){
    return mDriverEnabled;
}


#define REG_OS "HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"
#define REG_CPU "HKLM\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"
void HWObject::checkPcConfigure(){
    if(mOS.isEmpty()){
        QSettings reg(REG_OS,QSettings::NativeFormat);
        QString os = reg.value("ProductName").toString().trimmed();
        QString currentVersion = reg.value("CurrentVersion","").toString().trimmed();
        QString buildNumber = reg.value("CurrentBuildNumber","0").toString().trimmed();
        QString csdVersion = reg.value("CSDVersion","").toString();
        int major = reg.value("CurrentMajorVersionNumber",0).toInt();
        int minor = reg.value("CurrentMinorVersionNumber",0).toInt();
        if(major>0){
            setOs(QString("%1(%2.%3.%4)").arg(os).arg(major).arg(minor).arg(buildNumber));
        }else if(!csdVersion.isEmpty()){
            setOs(QString("%1 %2(%3.%4)").arg(os).arg(csdVersion).arg(currentVersion).arg(buildNumber));
        }else{
            setOs(QString("%1(%2.%3)").arg(os).arg(currentVersion).arg(buildNumber));
        }
        if(currentVersion<="6.1"&&csdVersion.isEmpty()){
            mOsEnabled = false;
        }else{
            mOsEnabled = true;
        }
        emit osEnabledChanged(mOsEnabled);
        emit osPromptChanged(osPrompt());
    }
    if(mCpu.isEmpty()){
        QSettings reg(REG_CPU,QSettings::NativeFormat);
        QStringList keyList=reg.childKeys();
        foreach(QString key,keyList){
            if(key=="ProcessorNameString"){
                setCpu(reg.value(key).toString().trimmed());
                setCpuLevel(checkCpuLevel());
                setCapLevel();
                break;
            }
        }
    }
    getgpu();
}
#define REG_VIDEOCARD "HKLM\\HARDWARE\\DEVICEMAP\\VIDEO"
#define REG_CONTROL_VIDEO "HKLM\\SYSTEM\\CurrentControlSet\\Control\\Video"
void HWObject::getgpu(){
    if(mGpu.isEmpty()){
        QSettings reg(REG_VIDEOCARD,QSettings::NativeFormat);
        int objectNumber = reg.value("MaxObjectNumber",0).toInt();
        if(objectNumber=0){
            return;
        }
        QSettings regGroup(REG_CONTROL_VIDEO,QSettings::NativeFormat);
        qInfo()<<regGroup.childGroups();
        foreach (QString group, regGroup.childGroups()) {
            QSettings regSub(QString(REG_CONTROL_VIDEO)+"\\"+group,QSettings::NativeFormat);
            foreach (QString childGroup, regSub.childGroups()) {
                QSettings regSub1(QString(REG_CONTROL_VIDEO)+"\\"+group+"\\"+childGroup,QSettings::NativeFormat);
                QStringList keyList = regSub1.childKeys();
                if((keyList.contains("DriverVersion") || keyList.contains("RadeonSoftwareVersion"))&&
                        (keyList.contains("DriverDesc")|| keyList.contains("HardwareInformation.AdapterString"))){
                    QString driver = regSub1.value("DriverVersion","").toString().trimmed();
                    QString chipType = regSub1.value("HardwareInformation.ChipType","").toString().trimmed();
                    QString providerName = regSub1.value("ProviderName","").toString().trimmed();
                    mGpuVendor = providerName;
                    if(providerName.toLower().contains("advanced")){
                        providerName = "AMD";
                    }
                    providerName = providerName.toUpper();
                    qInfo()<<"getgpu 1"<<chipType;
                    chipType = RegisterHandler::getInstance().getRegisterValue(QString(REG_CONTROL_VIDEO)+"\\"+group+"\\"+childGroup+"\\Settings","Device Description",chipType);
                    qInfo()<<"getgpu 2"<<chipType;
                    if(!driver.isEmpty()&&!chipType.isEmpty()){
                        if(chipType.contains(providerName)){
                            setGpu(chipType);
                        }else{
                            setGpu(providerName+" "+chipType);
                        }
                        setGpuDriver(driver);
                        qInfo()<<"gpu driver"<<chipType<<driver;
                        setGpuLevel(checkgpuLevel());
                        setCapLevel();
                        mDriverEnabled = checkDriverEnabled();
                        emit driverEnabledChanged(mDriverEnabled);
                        emit driverPromptChanged(driverPrompt());
                        if(providerName.indexOf("AMD")>=0||providerName.indexOf("NVIDIA")>=0){
                            return;
                        }
                    }
                }
            }
        }
    }
}

int HWObject::checkCpuLevel(){
    int index = getCapacityIndex(mCpuCap.mHWEnum,mCpu);
    qInfo()<<"checkCpuLevel,index="<<index<<mCpu;
    for(int i=0;i<mCpuCap.mLevelList.size();i++){
        if(index<=mCpuCap.mLevelList.at(i)->mMin
                &&index>mCpuCap.mLevelList.at(i)->mMax){
            return i;
        }
    }
    return 0;
}

int HWObject::checkgpuLevel(){
    int index = getCapacityIndex(mGpuCap.mHWEnum,mGpu);
    for(int i=0;i<mGpuCap.mLevelList.size();i++){
        if(index<=mGpuCap.mLevelList.at(i)->mMin
                &&index>mGpuCap.mLevelList.at(i)->mMax){
            return i;
        }
    }
    return 0;
}

int HWObject::getCapacityIndex(QStringList all,QString current){
    QString lowerCurrent = current.toLower();
    if(all.contains(lowerCurrent)){
        return all.indexOf(lowerCurrent);
    }
    for(int i=0;i<all.size();i++){
        QStringList strings = all.at(i).split(" ");
        bool bMatched=true;
        for(int j=0;j<strings.size();j++){
            if(!lowerCurrent.contains(strings.at(j))){
                bMatched = false;
                break;
            }
        }
        if(bMatched){
            return i;
        }
    }
    return all.size()+2;
}

bool HWObject::checkDriverEnabled(){
    QString lower = mGpu.toLower();
    if(lower.contains("nvidia")){
        QString driver = mgpuDriver;
        driver = driver.replace(".","").right(5).left(3);
        int nDriver = driver.toInt();
        return nDriver>mDriverNVidia;
    }else if(lower.contains("amd")){
        QString driver = mgpuDriver.left(2);
        int nDriver = driver.toInt();
        return nDriver>mDriverAmd;
    }else{
        return false;
    }
    return false;
}

QString HWObject::usbPrompt(){
    if(mGpu.isEmpty()){
        return "";
    }
    QString lower = mGpu.toLower();
    QString prompt = mUsbPropmpt;
    if(lower.contains("nvidia")){
        QString link = QString("<a href='%1'>%1</a>").arg(mDriverURL_N);
        return prompt.replace("%1",link);
    }else if(lower.contains("amd")){
        QString link = QString("<a href='%1'>%1</a>").arg(mDriverURL_A);
        return prompt.replace("%1",link);
    }else{
        return prompt.replace("%1","");
    }
}

QString HWObject::hdmiPrompt(){
    if(mGpu.isEmpty()){
        return "";
    }
    QString lower = mGpu.toLower();
    QString prompt = mHdmiPropmpt;
    if(lower.contains("nvidia")){
        QString link = QString("<a href='%1'>%1</a>").arg(mDriverURL_N);
        return prompt.replace("%1",link);
    }else if(lower.contains("amd")){
        QString link = QString("<a href='%1'>%1</a>").arg(mDriverURL_A);
        return prompt.replace("%1",link);
    }else{
        return prompt.replace("%1","");
    }
}

bool HWObject::bOnlyHasInterDisplayCard(){
    QSettings reg(REG_VIDEOCARD,QSettings::NativeFormat);
    int objectNumber = reg.value("MaxObjectNumber",0).toInt();
    if(objectNumber=0){
        return true;
    }
    QSettings regGroup(REG_CONTROL_VIDEO,QSettings::NativeFormat);
    qInfo()<<regGroup.childGroups();
    foreach (QString group, regGroup.childGroups()) {
        QSettings regSub(QString(REG_CONTROL_VIDEO)+"\\"+group,QSettings::NativeFormat);
        foreach (QString childGroup, regSub.childGroups()) {
            QSettings regSub1(QString(REG_CONTROL_VIDEO)+"\\"+group+"\\"+childGroup,QSettings::NativeFormat);
            QStringList keyList = regSub1.childKeys();
            if(keyList.contains("DriverVersion")&&keyList.contains("DriverDesc")){
                QString driver = regSub1.value("DriverVersion","").toString().trimmed();
                QString chipType = regSub1.value("HardwareInformation.ChipType","").toString().trimmed();
                QString providerName = regSub1.value("ProviderName","").toString().trimmed();
                if(providerName.toLower().contains("advanced")){
                    providerName = "AMD";
                }
                providerName = providerName.toUpper();
                qInfo()<<"getgpu 1"<<chipType;
                chipType = RegisterHandler::getInstance().getRegisterValue(QString(REG_CONTROL_VIDEO)+"\\"+group+"\\"+childGroup+"\\Settings","Device Description",chipType);
                qInfo()<<"getgpu 2"<<chipType;
                if(!driver.isEmpty()&&!chipType.isEmpty()){
                    qInfo()<<"gpu driver"<<chipType<<driver;
                    if(providerName.indexOf("AMD")>=0||providerName.indexOf("NVIDIA")>=0){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

#define LINE1 "##############################################################\n"
#define LINE2 "==============================================================\n"

void HWObject::writeSystemInfo(){
    checkPcConfigure();
    QString systemInfo("");
    systemInfo +=QString::fromLocal8Bit("系统信息\n");
    systemInfo +=LINE1;

    systemInfo +=QString::fromLocal8Bit("显卡信息\n");
    systemInfo += LINE2;
    systemInfo += QString::fromLocal8Bit("显卡厂商: ")+mGpuVendor+"\n";
    systemInfo += QString::fromLocal8Bit("显卡型号：")+mGpu+"\n";
    systemInfo += QString::fromLocal8Bit("显卡驱动版本：")+mgpuDriver+"\n";
    systemInfo +=LINE1;

    systemInfo +=QString::fromLocal8Bit("操作系统\n");
    systemInfo +=LINE2;
    systemInfo +=getComputeInfo();
    systemInfo +=LINE1;

    systemInfo +=QString::fromLocal8Bit("主板信息\n");
    systemInfo +=LINE2;
    systemInfo += getMainboardInfo();
    systemInfo +=LINE1;

    systemInfo +=QString::fromLocal8Bit("处理器信息\n");
    systemInfo +=LINE2;
    systemInfo += getCpuInfo();
    systemInfo +=LINE1;

    systemInfo +=QString::fromLocal8Bit("硬盘信息\n");
    systemInfo +=LINE2;
    systemInfo += getHardDiskInfo();
    systemInfo +=LINE1;

    systemInfo +=QString::fromLocal8Bit("内存信息\n");
    systemInfo +=LINE2;
    systemInfo += getMemoryInfo();
    systemInfo +=LINE1;

    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString fileName=dir.replace("PiPlay","Pimax")+"\\pi_system.txt";
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        qCritical()<<"write system file error"<<fileName;
        return;
    }
    file.write(systemInfo.toUtf8());
    file.close();

}

QString HWObject::getComputeInfo(){
    QString info("");
    QProcess process;
    process.start("wmic OS get Caption");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("Caption").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("操作系统：")+data+"\n";
    }

    QSettings reg(REG_OS,QSettings::NativeFormat);
    QString osName = reg.value("ProductName").toString().trimmed();
    QString osVersion="";
    QString currentVersion = reg.value("CurrentVersion","").toString().trimmed();
    QString buildNumber = reg.value("CurrentBuildNumber","0").toString().trimmed();
    QString csdVersion = reg.value("CSDVersion","").toString();
    int major = reg.value("CurrentMajorVersionNumber",0).toInt();
    int minor = reg.value("CurrentMinorVersionNumber",0).toInt();
    if(major>0){
        osVersion = QString("%1.%2.%3").arg(major).arg(minor).arg(buildNumber);
    }else if(!csdVersion.isEmpty()){
        osVersion = QString("%1.%2.%3").arg(csdVersion).arg(currentVersion).arg(buildNumber);
    }else{
        osVersion = QString("%1.%2").arg(currentVersion).arg(buildNumber);
    }
    QString systemRoot = reg.value("SystemRoot").toString().trimmed();

    process.start("wmic OS get MUILanguages");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("MUILanguages").replace("\n","").replace("{","").replace("}","").trimmed();
        info = info + QString::fromLocal8Bit("操作系统语言：")+data+"\n";
    }

    QString osBit("");
    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
            si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
    {
        osBit = QString::fromLocal8Bit("64 位");
    }else{
        osBit = QString::fromLocal8Bit("32 位");
    }

    info +=QString::fromLocal8Bit("操作系统结构：")+osBit+"\n";

    process.start("wmic OS get Version");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("Version").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("操作系统版本：")+data+"\n";
    }

    process.start("wmic OS get WindowsDirectory");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("WindowsDirectory").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("系统目录：")+data+"\n";
    }
    return info;
}

QString HWObject::getMainboardInfo(){
    QString info;
    QProcess process;
    process.start("wmic BaseBoard get Product");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("Product").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("主板型号：")+data+"\n";
    }
    process.start("wmic BaseBoard get Manufacturer");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("Manufacturer").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("主板厂商：")+data+"\n";
    }
    process.start("wmic BaseBoard get Version");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("Version").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("主板驱动版本：")+data+"\n";
    }
    return info;
}

QString HWObject::getMemoryInfo(){
    QString info;
    QProcess process;
    process.start("wmic MEMORYCHIP get Capacity");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("Capacity").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("内存大小：")+data+"\n";
    }
    process.start("wmic MEMORYCHIP get Speed");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("Speed").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("存储速率：")+data+"\n";
    }
    return info;
}

QString HWObject::getHardDiskInfo(){
    QString info;
    QProcess process;
    process.start("wmic DISKDRIVE get Caption");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("Caption").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("硬盘型号：")+data+"\n";
    }
    process.start("wmic DISKDRIVE get Size");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("Size").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("硬盘大小：")+data+"\n";
    }
    return info;
}

QString HWObject::getCpuInfo(){
    QString info;
    QProcess process;
    process.start("wmic CPU get Name");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("Name").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("处理器型号：")+data+"\n";
    }
    process.start("wmic CPU get MaxClockSpeed");
    if(process.waitForFinished()){
        QString data = QString::fromLocal8Bit(process.readAll());
        data = data.remove("MaxClockSpeed").replace("\n","").trimmed();
        info = info + QString::fromLocal8Bit("处理器主频：")+data+"MHz\n";
    }
    return info;
}
