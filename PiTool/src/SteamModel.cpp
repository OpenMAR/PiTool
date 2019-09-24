#include "SteamModel.h"
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QProcess>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QDesktopServices>
#include "quazip/JlCompress.h"
#include <QDebug>
#include "piservice.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QPixmap>
#include "OnlineModel.h"
#include "PiLog/WinCommon.h"


SteamModel::SteamModel(QObject *parent) : QObject(parent){
    qInfo()<<"SteamModel";
    initSteamPath();
}
SteamModel::~SteamModel(){
}

#define REG_PATH_STEAM_APP "HKLM\\SOFTWARE\\Wow6432Node\\Valve\\Steam"
#define REG_ITEM_INSTALL_PATH "InstallPath"
void SteamModel::initSteamPath(){
    m_steamAppFolder = RegisterHandler::getInstance().getRegisterValue(REG_PATH_STEAM_APP,"InstallPath","");
    if(m_steamAppFolder==""||!QDir(m_steamAppFolder).exists()){
        qInfo() << "Steam App is not installed";
        return;
    }

    getInstallFolder();

    for(int i=0;i<m_steamInstallFolder.size();i++){
        QString file1 = m_steamInstallFolder.at(i) + "\\steamapps\\common\\SteamVR\\bin\\win64\\vrstartup.exe";
        QString file2 = m_steamInstallFolder.at(i) + "\\steamapps\\common\\SteamVR\\bin\\win64\\vrpathreg.exe";
        if(QFile(file1).exists()&&QFile(file2).exists()){
            m_steamVRFile = file1;
            break;
        }
    }
    if(m_steamVRFile.isEmpty()){
        return;
    }
    m_steamManifestPath = m_steamAppFolder + "\\config\\steamapps.vrmanifest";
}

void SteamModel::getInstallFolder(){
    m_steamInstallFolder.clear();
    m_steamInstallFolder.append(m_steamAppFolder);

    QString configFile = m_steamAppFolder + "\\config\\config.vdf";
    QString content = Setting::loadText(configFile);
    for(int i=1;i<10;i++){
        int find = content.indexOf(QString("BaseInstallFolder_%1").arg(i));
        if(find>0){
            int end = content.indexOf("\n",find);
            QString subString = content.mid(find,end-find).replace("\"","").replace("\t"," ");
            QString path = subString.right(subString.length()-subString.lastIndexOf(" ")-1).trimmed();
            m_steamInstallFolder.append(path);
        }else{
            break;
        }
    }
    if(m_steamInstallFolder.size()==1){
        int find = content.indexOf("BaseInstallFolder");
        if(find>0){
            int end = content.indexOf("\n",find);
            QString subString = content.mid(find,end-find).replace("\"","").replace("\t"," ");
            QString path = subString.right(subString.length()-subString.lastIndexOf(" ")-1).trimmed();
            m_steamInstallFolder.append(path);
        }
    }
    qInfo()<<"getInstallFolder,"<<m_steamInstallFolder;
}


bool SteamModel::isSteamApp(QString appFile){
    qInfo()<<"isSteamApp"<<appFile;
    QString appDir = appFile.left(appFile.toLower().lastIndexOf(".exe"))+"_Data";
    if(QDir(appDir).exists()&&QFile(appDir+"\\Plugins\\openvr_api.dll").exists()){
        return true;
    }
    return false;
}

void SteamModel::killSteamVr(){
    int runApp = 0;
    runApp += WinCommon::processAlived("vrserver.exe")?1:0;
    runApp += WinCommon::processAlived("vrmonitor.exe")?1:0;
    runApp += WinCommon::processAlived("vrcompositor.exe")?1:0;
    runApp += WinCommon::processAlived("vrdashboard.exe")?1:0;
    if(runApp>0&&runApp<4){
        WinCommon::killProcessByName("vrserver.exe");
        WinCommon::killProcessByName("vrmonitor.exe");
        WinCommon::killProcessByName("vrcompositor.exe");
        WinCommon::killProcessByName("vrdashboard.exe");
    }
}

bool SteamModel::steamVrIsInstalled(){
    return m_steamVRFile!=""&&QFile(m_steamVRFile).exists();
}
bool SteamModel::steamGameIsDownloding(QString steamAppId){
    qInfo()<<"steamvr app download check:"<<steamAppId;
    for(int i=0;i<m_steamInstallFolder.size();i++){
        QString appid = steamAppId.right(steamAppId.length()-steamAppId.lastIndexOf(".")-1);
        QString path = m_steamInstallFolder.at(i)+"\\steamapps\\downloading\\"+appid;
        if(QDir(path).exists()){
            qCritical()<<"steamvr app download path :"<<path;
            return true;
        }
    }
    return false;
}

QString SteamModel::getExeFile(QString steamAppId){
    QString id = steamAppId.right(steamAppId.length()-steamAppId.lastIndexOf(".")-1);
    for(int i=0;i<m_steamInstallFolder.size();i++){
        QString installFolder = m_steamInstallFolder.at(i);
        QString appmanifest =installFolder+QString("\\steamapps\\appmanifest_%1.acf").arg(id);
        QString content = Setting::loadText(appmanifest);
        int find = content.indexOf("installdir");
        if(find>0){
            int end = content.indexOf("\n",find);
            QString subString = content.mid(find,end-find).replace("\"","").replace("\t"," ");
            QString exePath = subString.right(subString.length()-subString.lastIndexOf(" ")-1).trimmed();
            QString exeFile = installFolder+QString("\\steamapps\\common\\%1\\%1.exe").arg(exePath);
            if(QFile(exeFile).exists()){
                return exeFile;
            }
        }
    }
    return "";
}
