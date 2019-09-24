#include "diagnoseHandler.h"
#include "Windows.h"
#include <QDebug>
#include <QSysInfo>
#include <QSettings>
#include "jsonhandler.h"
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include "PiLog/WinCommon.h"
#include "PiLog/StringUniversal.h"
#include <qcoreapplication.h>

#define NVIDIA_VERSION_HEADER_VERSION_MIN 22 // version > 21.X.major.minor IS OK!
#define NVIDIA_VERSION_MAJOR_MIN 13
#define NVIDIA_VERSION_MINOR_MIN 8233

#define AMD_VERSION_HEADER_VERSION 22 // version > 21.X.X.X IS OK!

#define AMD_VERSION_4K_BLACKLIST_SUM 1
const static QString AMD_VERSION_4K_BLACKLIST[AMD_VERSION_4K_BLACKLIST_SUM] = {
    "23.20.15017.3010"
};
#define AMD_VERSION_8K_BLACKLIST_SUM 1
const static QString AMD_VERSION_8K_BLACKLIST[AMD_VERSION_8K_BLACKLIST_SUM] = {
    "23.20.15017.3010"
};

#define NVIDIA_VERSION_4K_BLACKLIST_SUM 1
const static QString NVIDIA_VERSION_4K_BLACKLIST[NVIDIA_VERSION_4K_BLACKLIST_SUM] = {
    "21.0.2.0"
};
#define NVIDIA_VERSION_8K_BLACKLIST_SUM 1
const static QString NVIDIA_VERSION_8K_BLACKLIST[NVIDIA_VERSION_8K_BLACKLIST_SUM] = {
    "21.0.2.0"
};

#define AMD_4K_SUPPORT_CNT 23
#define NVIDIA_4K_SUPPORT_NVIDIA_CNT 11
const static QString AMD_4K_SUPPORT[AMD_4K_SUPPORT_CNT] = {
    "HD 77",	  "HD 78",     "HD 79",       "R7 240",
    "R7 250",   "R7 260",   "R7 265",        "R9 270",
    "R9 280",   "R9 285",      "R9 290",     "R9 295X2",
    "R7 350",      "R7 360",      "R9 370",     "R9 380",
    "R9 390",   "R9 Fury",  "R9 Fury Nano",  "Pro Duo",
    "RX 460",	  "RX 470",		"RX 480"
};

const static QString NVIDIA_4K_SUPPORT[NVIDIA_4K_SUPPORT_NVIDIA_CNT] = {
    "NVIDIA GeForce GTX 960",	  "NVIDIA GeForce GTX 970",     "NVIDIA GeForce GTX 980",       "NVIDIA GeForce GTX 980Ti",
    "NVIDIA GeForce GTX TITAN X",   "NVIDIA GeForce GTX 1050",   "NVIDIA GeForce GTX 1050Ti",   "NVIDIA GeForce GTX 1060",
    "NVIDIA GeForce GTX 1070",   "NVIDIA GeForce GTX 1080",      "NVIDIA GeForce GTX 1080Ti"
};

#define AMD_8K_SUPPORT_CNT 2
#define NVIDIA_8K_SUPPORT_NVIDIA_CNT 11
const static QString AMD_8K_SUPPORT[AMD_8K_SUPPORT_CNT] = {
"RX 470",		"RX 480"
};

const static QString NVIDIA_8K_SUPPORT[NVIDIA_8K_SUPPORT_NVIDIA_CNT] = {
    "NVIDIA GeForce GTX 960",	  "NVIDIA GeForce GTX 970",     "NVIDIA GeForce GTX 980",       "NVIDIA GeForce GTX 980Ti",
    "NVIDIA GeForce GTX TITAN X",   "NVIDIA GeForce GTX 1050",   "NVIDIA GeForce GTX 1050Ti",   "NVIDIA GeForce GTX 1060",
    "NVIDIA GeForce GTX 1070",   "NVIDIA GeForce GTX 1080",      "NVIDIA GeForce GTX 1080Ti"
};

DiagnoseHandler::DiagnoseHandler(QObject *parent) : QObject(parent)
{
    workDir = QCoreApplication::instance()->applicationDirPath().replace("/","\\");
    isNoteBook = false;
    cpu = "unknow";
    memory = 0;
    os_version = "unknow";
    sp1_missed = false;
    gpuDriverInstalled = true;
    gpuVersionValid = true;
    gpuOnlyIntelCard = false;
    initialize();
}

void DiagnoseHandler::initialize()
{
    appComponentMissed = false;
    usb3_0 = true;
    dfu_driver_installed = true;
    gpus.clear();
    gpuVrReadyFor4K = true;
    gpuVersionValidFor4K = true;
    gpuVrReadyFor8K = true;
    gpuVersionValidFor8K = true;
    missedPatchs.clear();
    missedDlls = false;
    vcMissed = false;
    vcMissedVersion.clear();
    dxFileMissed = false;
    steamvrValid = true;
    serviceRunning = true;
    connected = false;
    usbPlugin = false;
    videoPlugin = false;
    integratedGraphicsPlugin = false;
    videoOutputType = ConnectorType_UNKNOWN;
    hmdName = "unknow";
    hmdSN = "unknow";
    hmdDriverVersion = "unknow";
    hmdMode = 0;
}

bool DiagnoseHandler::startService()
{
    auto ret = WinCommon::startProcess(TEXT("Diagnose.exe"),true,workDir.toStdWString(),false,TEXT("admin start_server PiServiceLauncher"));
    if(!ret.first){
        QProcess p(0);
        p.setWorkingDirectory(workDir);
        QStringList args;
        args.append("admin");
        args.append("start_server");
        args.append("PiServiceLauncher");
        p.start("Diagnose.exe",args);
        p.waitForFinished();
    }
    return true;
}

bool DiagnoseHandler::stopServiceByName(QString name)
{
    QProcess p(0);
    p.setWorkingDirectory(workDir);
    QStringList args;
    args.append("admin");
    args.append("stop_server");
    args.append(name);
    p.start("Diagnose.exe",args);
    p.waitForFinished();
    return true;
}

bool DiagnoseHandler::ovrServiceAlived()
{
    if(!WinCommon::processAlived("OVRServiceLauncher.exe")){
        qCritical()<<"ovr service is stoped!";
        return false;
    }
    return true;
}

bool DiagnoseHandler::startServiceByName(QString name)
{
    QProcess p(0);
    p.setWorkingDirectory(workDir);
    QStringList args;
    args.append("admin");
    args.append("start_server");
    args.append(name);
    p.start("Diagnose.exe",args);
    p.waitForFinished();
    return true;
}

bool DiagnoseHandler::LaunchDfuDriverSetupPackage()
{
    QProcess process;
    QString command = workDir + "\\dfudriver\\stm_dfu_3040_64.exe";
    process.startDetached("\""+command+"\"");
    return true;
}

void DiagnoseHandler::hardwareDiagnose()
{
    auto ret = WinCommon::startProcess(TEXT("Diagnose.exe"),true,workDir.toStdWString(),false,TEXT("diagnose_hardware"));
    if(!ret.first){
        QProcess p(0);
        p.setWorkingDirectory(workDir);
        QStringList args;
        args.append("diagnose_hardware");
        p.start("Diagnose.exe",args);
        p.waitForFinished();
    }
    //std::string jsonPath;
    //StringUniversal::GetLocalAppDataPath(jsonPath);
    //jsonPath += "\\Pimax\\PiService\\diagnose.json";

    QString jsonPath;
    jsonPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    jsonPath = jsonPath.replace("PiTool","Pimax")+"\\PiService\\diagnose.json";

    QJsonObject object;
    if(!JsonHandler::loadJson(jsonPath,object)){
        return;
    }
    parseHardwareDiagnose(object);
}

void DiagnoseHandler::softwareDiagnose()
{
    initialize();
    auto ret = WinCommon::startProcess(TEXT("Diagnose.exe"),true,workDir.toStdWString(),false,TEXT("diagnose_software"));
    if(!ret.first){
        QProcess p(0);
        p.setWorkingDirectory(workDir);
        QStringList args;
        args.append("diagnose_software");
        p.start("Diagnose.exe",args);
        p.waitForFinished();
    }

    QString jsonPath;
    jsonPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    jsonPath = jsonPath.replace("PiTool","Pimax")+"\\PiService\\diagnose.json";

    QJsonObject object;
    if(!JsonHandler::loadJson(jsonPath,object)){
        qCritical() << "load json failed:"<<jsonPath;
        return;
    }
    parseSoftwareDiagnose(object);
}

void DiagnoseHandler::allDiagnose()
{
    qCritical() << "diagnose";
    initialize();
    if(!WinCommon::fileExists(workDir.toStdString() + "\\Diagnose.exe")){
        appComponentMissed = true;
        qCritical() << "diagnose missed";
        return;
    }

    auto ret = WinCommon::startProcess(TEXT("Diagnose.exe"),true,workDir.toStdWString(),false);
    if(!ret.first){
        QProcess p(0);
        p.setWorkingDirectory(workDir);
        p.start("Diagnose.exe");
        p.waitForFinished();
    }

    QString jsonPath;
    jsonPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    jsonPath = jsonPath.replace("PiTool","Pimax")+"\\PiService\\diagnose.json";

    QJsonObject object;
    if(!JsonHandler::loadJson(jsonPath,object)){
        qCritical() << "load json failed:"<<jsonPath;
        return;
    }
    //parse diagnose result;
    parseDiagnoseResult(object);
}

void DiagnoseHandler::parseHardwareDiagnose(QJsonObject object)
{
    QJsonObject pcObject;
    if(JsonHandler::getJsonObjectValueByKey(object,"pc",pcObject)){
        JsonHandler::getJsonValueByKey(pcObject,"cpu",cpu);
        JsonHandler::getJsonValue_longByKey(pcObject,"memory",memory);
        JsonHandler::getJsonValueByKey(pcObject,"notebook",isNoteBook);
        JsonHandler::getJsonValueByKey(pcObject,"os_version",os_version);
        JsonHandler::getJsonValueByKey(pcObject,"sp1_missed",sp1_missed);
        JsonHandler::getJsonValueByKey(pcObject,"gpu_driver_installed",gpuDriverInstalled);
        emit gpuDriverInstalledChanged(gpuDriverInstalled);
    }
}

void DiagnoseHandler::parseSoftwareDiagnose(QJsonObject object)
{
    QJsonObject pcObject;
    if(JsonHandler::getJsonObjectValueByKey(object,"pc",pcObject)){
        JsonHandler::getJsonValueByKey(pcObject,"usb3_0",usb3_0);
        JsonHandler::getJsonValueByKey(pcObject,"dfu_driver_installed",dfu_driver_installed);
        JsonHandler::getJsonValueByKey(pcObject,"gpu_only_intel_card",gpuOnlyIntelCard);
        QJsonArray array;
        if(JsonHandler::getJsonValueByKey(pcObject,"gpu",array)){
            bool versionValid = gpuVersionValid = false;//gpuVersionValid
            for(int i=0;i<array.size();i++){
                QJsonObject jsonObject = array.at(i).toObject();
                gpuInfo gpuItem;
                if(JsonHandler::getJsonValueByKey(jsonObject,"name",gpuItem.name)){
                    JsonHandler::getJsonValueByKey(jsonObject,"version",gpuItem.version);
                    if(JsonHandler::getJsonValueByKey(jsonObject,"versionValid",versionValid) && versionValid){
                        gpuVersionValid = true;
                    }
                    gpus.push_back(gpuItem);
                }
            }
            emit gpuVersionValidChanged(gpuVersionValid);
            emit gpuSumChanged(gpus.size());
        }
        QJsonArray patchArray;
        if(JsonHandler::getJsonValueByKey(pcObject,"miss_patch",patchArray)){
            for(int i=0;i<patchArray.size();i++){
                QString patchStr = patchArray.at(i).toString();
                missedPatchs.push_back(patchStr);
            }
        }
    }

    QJsonObject appObject;
    if(JsonHandler::getJsonObjectValueByKey(object,"app",appObject)){
        JsonHandler::getJsonValueByKey(appObject,"missed",appComponentMissed);
        emit appComponentMissStateChanged(appComponentMissed);
    }

    QJsonObject systemObject;
    if(JsonHandler::getJsonObjectValueByKey(object,"system",systemObject)){
        JsonHandler::getJsonValueByKey(systemObject,"missed",missedDlls);
        emit missedDllsChanged(missedDlls);
    }

    QJsonObject vcObject;
    if(JsonHandler::getJsonObjectValueByKey(object,"vc",vcObject)
            && JsonHandler::getJsonValueByKey(vcObject,"missed",vcMissed) && vcMissed){
        /*QJsonArray array;
        if(JsonHandler::getJsonValueByKey(vcObject,"missed_versions",array)){
            for(int i=0;i<array.size();i++){
                QString vcStr = array.at(i).toString();
                vcMissedVersion.push_back(vcStr);
            }
        }*/
    }

    QJsonObject dxObject;
    if(JsonHandler::getJsonObjectValueByKey(object,"dx",dxObject)){
        JsonHandler::getJsonValueByKey(dxObject,"missed",dxFileMissed);
        emit dxFileMissedChanged(dxFileMissed);
    }

    QJsonObject steamvrObject;
    if(JsonHandler::getJsonObjectValueByKey(object,"steamvr",steamvrObject)){
        JsonHandler::getJsonValueByKey(steamvrObject,"valid",steamvrValid);
        emit steamvrValidChanged(steamvrValid);
    }

    QJsonObject serviceObject;
    if(JsonHandler::getJsonObjectValueByKey(object,"server",serviceObject)){
        JsonHandler::getJsonValueByKey(serviceObject,"running",serviceRunning);
        emit serviceRunningChanged(serviceRunning);
    }

    QJsonObject hmdObject;
    if(JsonHandler::getJsonObjectValueByKey(object,"hmd",hmdObject)){
        JsonHandler::getJsonValueByKey(hmdObject,"connected",connected);
        emit connectedChanged(connected);
        JsonHandler::getJsonValueByKey(hmdObject,"usb",usbPlugin);
        emit usbPluginChanged(usbPlugin);
        JsonHandler::getJsonValueByKey(hmdObject,"hdmi/dp",videoPlugin);
        emit videoPluginChanged(videoPlugin);
        JsonHandler::getJsonValueByKey(hmdObject,"name",hmdName);
        JsonHandler::getJsonValueByKey(hmdObject,"driver",hmdDriverVersion);
        JsonHandler::getJsonValueByKey(hmdObject,"sn",hmdSN);
        JsonHandler::getJsonValueByKey(hmdObject,"mode",hmdMode);
        if(JsonHandler::getJsonValueByKey(hmdObject,"integrated_graphics",integratedGraphicsPlugin)){
            emit integratedGraphicsPluginChanged(integratedGraphicsPlugin);
        }
        if(JsonHandler::getJsonValueByKey(hmdObject,"output_type",videoOutputType)){
            emit videoOutputTypeChanged(videoOutputType);
        }
    }

    analysisGpuDriver(gpus);
}

void DiagnoseHandler::parseDiagnoseResult(QJsonObject object)
{
    parseHardwareDiagnose(object);
    parseSoftwareDiagnose(object);
}

void DiagnoseHandler::analysisGpuDriver(const QList<gpuInfo> gpuList)
{
    bool mGpuVrReadyFor4K = false;
    bool mGpuVersionValidFor4K = false;
    bool mGpuVrReadyFor8K = false;
    bool mGpuVersionValidFor8K = false;

    for(int i = 0; i < gpuList.size(); i++){
        gpuInfo mGpuInfo = gpuList[i];
        //amd
        if(mGpuInfo.name.contains("Radeon")){
            //vrready for supported video card
            if(containStr(mGpuInfo.name,AMD_4K_SUPPORT,AMD_4K_SUPPORT_CNT)){
                mGpuVrReadyFor4K = true;
                //valid version check
                mGpuVersionValidFor4K = amdVersionSupport4K(mGpuInfo.version)?true:mGpuVersionValidFor4K;
            }
            if(containStr(mGpuInfo.name,AMD_8K_SUPPORT,AMD_8K_SUPPORT_CNT)){
                mGpuVrReadyFor8K = true;
                //valid version check
                mGpuVersionValidFor8K = amdVersionSupport8K(mGpuInfo.version)?true:mGpuVersionValidFor8K;
            }
        }
        //nvidia
        if(gpuList[i].name.contains("NVIDIA")){
            //vrready for supported video card
            if(containStr(mGpuInfo.name,NVIDIA_4K_SUPPORT,NVIDIA_4K_SUPPORT_NVIDIA_CNT)){
                mGpuVrReadyFor4K = true;
                //valid version check
                mGpuVersionValidFor4K = nvidiaVersionSupport4K(mGpuInfo.version)?true:mGpuVersionValidFor4K;
            }
            if(containStr(mGpuInfo.name,NVIDIA_8K_SUPPORT,NVIDIA_8K_SUPPORT_NVIDIA_CNT)){
                mGpuVrReadyFor8K = true;
                //valid version check
                mGpuVersionValidFor8K = nvidiaVersionSupport8K(mGpuInfo.version)?true:mGpuVersionValidFor8K;
            }
        }
    }

    gpuVrReadyFor4K = mGpuVrReadyFor4K;
    gpuVersionValidFor4K = mGpuVersionValidFor4K;
    gpuVrReadyFor8K = mGpuVrReadyFor8K;
    gpuVersionValidFor8K = mGpuVersionValidFor8K;

}

bool DiagnoseHandler::containStr(QString mainStr,const QString* minorStrArray, int arraySum)
{
    for(int i = 0; i < arraySum; i++){
        if(mainStr.contains(minorStrArray[i])){
            return true;
        }
    }
    return false;
}

bool DiagnoseHandler::amdVersionSupport4K(QString version)
{
    //check blacklist
    if(containStr(version,AMD_VERSION_4K_BLACKLIST,AMD_VERSION_4K_BLACKLIST_SUM)){
        return false;
    }

    QString headPart = version.section('.',0,0);
    int headPartValue = headPart.toInt();
    if(headPartValue >= AMD_VERSION_HEADER_VERSION){
        return true;
    }
    return false;
}

bool DiagnoseHandler::amdVersionSupport8K(QString version)
{
    //check blacklist
    if(containStr(version,AMD_VERSION_8K_BLACKLIST,AMD_VERSION_8K_BLACKLIST_SUM)){
        return false;
    }

    QString headPart = version.section('.',0,0);
    int headPartValue = headPart.toInt();
    if(headPartValue >= AMD_VERSION_HEADER_VERSION){
        return true;
    }
    return false;
}


bool DiagnoseHandler::nvidiaVersionSupport4K(QString version)
{
    //check blacklist
    if(containStr(version,NVIDIA_VERSION_4K_BLACKLIST,NVIDIA_VERSION_4K_BLACKLIST_SUM)){
        return false;
    }

    int headPartValue = version.section('.',0,0).toInt();
    int majorPartValue = version.section('.',2,2).toInt();
    int minorPartValue = version.section('.',3,3).toInt();
    if(headPartValue >= NVIDIA_VERSION_HEADER_VERSION_MIN
            && majorPartValue >= NVIDIA_VERSION_MAJOR_MIN
            && minorPartValue >= NVIDIA_VERSION_MINOR_MIN){
        return true;
    }
    return false;
}

bool DiagnoseHandler::nvidiaVersionSupport8K(QString version)
{
    //check blacklist
    if(containStr(version,NVIDIA_VERSION_8K_BLACKLIST,NVIDIA_VERSION_8K_BLACKLIST_SUM)){
        return false;
    }

    int headPartValue = version.section('.',0,0).toInt();
    int majorPartValue = version.section('.',2,2).toInt();
    int minorPartValue = version.section('.',3,3).toInt();
    if(headPartValue >= NVIDIA_VERSION_HEADER_VERSION_MIN
            && majorPartValue >= NVIDIA_VERSION_MAJOR_MIN
            && minorPartValue >= NVIDIA_VERSION_MINOR_MIN){
        return true;
    }
    return false;
}
