#include "Setting.h"
#include "SingleApplication.h"
#include <QProcess>
#include "Windows.h"
#include <QDir>
#include <QFile>
#include <QSettings>
#include "versioncontrol.h"
#include <QTranslator>
#include "PiLog/JsonHandler.h"
#include <QStandardPaths>
#include "jsonhandler.h"
#include "quazip/JlCompress.h"
#include "PiLog/StringUniversal.h"
#include "piservice.h"
#include <Windows.h>
#include <ShlObj.h>

#define REG_PITOOL_PATH "HKEY_CURRENT_USER\\SOFTWARE\\Pimax"

bool mStartupHide = false;
QString mOVRLancher = "";
QString mDownloadPath="";//download file path
QString mHttpRequestAddr = "";
QObject *mQmlRootObject = nullptr;
//profile
QString config = "";
QJsonObject config_object;
QStringList language_list;
int language_index = -1;
Setting *gInstances = nullptr;
Setting* Setting::getInstance(QObject*)
{
    return gInstances;
}

Setting::Setting(QObject *parent) : QObject(parent)
{
    qCritical()<<"Setting init";
    gInstances = this;
    mAutoRun = isAutoRun();
    if(config.isEmpty()){
        config = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        config = config.replace("PiTool","Pimax")+"\\PiTool";
        if(!QDir(config).exists()){
            QDir().mkdir(config);
        }
        config += "\\config.json";
    }
    bool aotu = true;
    JsonCommonHandler::ReadJsonValue(config.toStdWString(),TEXT("autoRun"),aotu);
    setAutoRun(aotu);
}

void Setting::setStartupHide(bool hide)
{
    mStartupHide = hide;
}

bool Setting::getStartupHide()
{
    return mStartupHide;
}

#define REG_AUTORUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define REG_LOCAL_AUTORUN "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
bool Setting::isAutoRun(){
    //QSettings reg(REG_AUTORUN,QSettings::NativeFormat);
    QString application = RegisterHandler::getInstance().getRegisterValue(REG_AUTORUN,"PiTool","");
    if(!application.isEmpty()){
        return true;
    }

    application = RegisterHandler::getInstance().getRegisterValue(REG_LOCAL_AUTORUN,"PiTool","");
    if(!application.isEmpty()){
        return true;
    }

    return false;
}

void Setting::setAutoRun(bool autoRun){
    qCritical()<<"setAutoRun:"<<autoRun<<mAutoRun;
    if(mAutoRun==autoRun){
        return;
    }
    QSettings reg(REG_AUTORUN,QSettings::NativeFormat);
    if(autoRun){
        QString command = QCoreApplication::instance()->applicationFilePath().replace("/","\\")+" hide";
        reg.setValue("PiTool",command);
    }else{
        reg.remove("PiTool");
        QSettings localReg(REG_LOCAL_AUTORUN,QSettings::NativeFormat);
        localReg.remove("PiTool");
    }
    mAutoRun = autoRun;
    JsonCommonHandler::CoverJsonValue(config.toStdWString(),TEXT("autoRun"),mAutoRun);
}

QStringList Setting::getLanguageList()
{
    QStringList list(language_list);
    int index = list.indexOf("pitool_cn");
    if (index >= 0){
        QString str = QString::fromLocal8Bit("简体中文");
        list.replace(index,str);
    }
    index = list.indexOf("pitool_en");
    if (index >= 0){
        list.replace(index,"English");
    }
    return list;
}

#define KEY_LANGUAGE "Language"
int Setting::getLanguage()
{
    mLanguage = RegisterHandler::getInstance().getRegisterValue(REG_PITOOL_PATH,KEY_LANGUAGE,-1);
    if(mLanguage ==-1){
        int lcid = GetSystemDefaultLCID();
        if(lcid==2052||lcid==3076||lcid==1028){//chinese(hongkong) 3076 chinese(china) 2052 chinese(taiwang) 1028
            mLanguage = LANGUAGE_CHINESE;
        }else{
            mLanguage = LANGUAGE_ENGLISH;
        }
    }
    return mLanguage;
}

int Setting::getLanguageByStr()
{
    return language_index == -1 ? 0 : language_index;
}

void Setting::setLanguage(int language)
{
    if(mLanguage!=language&&language>=0&&language<LANGUAGE_COUNT){
        mLanguage = language;
        RegisterHandler::getInstance().setRegisterValue(REG_PITOOL_PATH,KEY_LANGUAGE,language);
    }
}

void Setting::setLanguageStr(int language)
{
    if(language_list.size() == 0)
    {
        setLanguage(language);
        return;
    }
    JsonCommonHandler::CoverJsonValue(config.toStdWString(),TEXT("language"),language_list[language].toStdString());
}

#define KEY_PVRHOMEENABLE "pvr_Home_Enabled"
bool Setting::getPvrHomeEnabled()
{
    mPvrHomeEnabled = RegisterHandler::getInstance().getRegisterValue(REG_PITOOL_PATH, KEY_PVRHOMEENABLE, 1) == 1;
    PiService::getInstance()->setPvrHomeEnabled(mPvrHomeEnabled);

    return mPvrHomeEnabled;
}

void Setting::setPvrHomeEnabled(bool pvrHomeEnabled)
{
    if (pvrHomeEnabled != mPvrHomeEnabled) {
        mPvrHomeEnabled = pvrHomeEnabled;
        PiService::getInstance()->setPvrHomeEnabled(mPvrHomeEnabled);
        RegisterHandler::getInstance().setRegisterValue(REG_PITOOL_PATH, KEY_PVRHOMEENABLE, mPvrHomeEnabled);
    }
}

void Setting::setQMlRootObject(QObject *qmlObject){
    mQmlRootObject = qmlObject;
}

void Setting::loadTranslate_old(){
    int language = RegisterHandler::getInstance().getRegisterValue(REG_PITOOL_PATH,KEY_LANGUAGE,-1);
    if(language==-1){
        int lcid = GetSystemDefaultLCID();
        if(lcid==2052||lcid==3076||lcid==1028){//chinese(hongkong) 3076 chinese(china) 2052 chinese(taiwang) 1028
            language = LANGUAGE_CHINESE;
        }else{
            language = LANGUAGE_ENGLISH;
        }
    }


    QString appPath = QCoreApplication::instance()->applicationDirPath().replace("/","\\");

    QString translatorFileName = appPath+"\\language\\pitool_en.qm";
    if(language==LANGUAGE_CHINESE){
        translatorFileName = appPath+"\\language\\pitool_cn.qm";
    }
    QTranslator *translator = new QTranslator(QCoreApplication::instance());
    if (translator->load(translatorFileName)){
        qDebug()<<"load translator success"<<translatorFileName;
        QCoreApplication::instance()->installTranslator(translator);
    }else{
        qCritical()<<"load translator error"<<translatorFileName;
    }
}

void Setting::loadTranslate(){
    config = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    config = config.replace("PiTool","Pimax")+"\\PiTool";
    if(!QDir(config).exists()){
        QDir().mkdir(config);
    }
    config += "\\config.json";

    if(!JsonHandler::loadJson(config,config_object)){
        qCritical() << "load json failed:"<<config;
    }
    //load all qm files
    QString appPath = QCoreApplication::instance()->applicationDirPath().replace("/","\\");
    QString translatorPath = appPath+"\\language\\";

    QDir dir(translatorPath);
    if(!dir.exists())
    {
        return;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    int file_count = list.count();
    if(file_count <= 0)
    {
        return;
    }

    for(int i=0; i<file_count; i++)
    {
        QFileInfo file_info = list.at(i);
        QString suffix = file_info.suffix();
        if(QString::compare(suffix, QString("qm"), Qt::CaseInsensitive) == 0)
        {
            qCritical()<<"load translator file name >>>>>>>>>>>>>>"<<file_info.baseName();
            language_list.append(file_info.baseName());
        }
    }


    QString language_str;
    if(JsonHandler::getJsonValueByKey(config_object,"language",language_str)){
        language_index = language_list.indexOf(language_str);
        if(language_index >= 0){
            QString appPath = QCoreApplication::instance()->applicationDirPath().replace("/","\\");
            QString translatorFileName = appPath+"\\language\\" + language_str + ".qm";
            QTranslator *translator = new QTranslator(QCoreApplication::instance());
            if (translator->load(translatorFileName)){
                qDebug()<<"load translator success"<<translatorFileName;
                QCoreApplication::instance()->installTranslator(translator);
            }else{
                qCritical()<<"load translator error"<<translatorFileName;
            }
            return;
        }
    }
    return loadTranslate_old();
}

QString Setting::getLangAndLcid(){
    QString languages[]={"cn","en","jp","ge"};
    QString lang = "lang="+languages[getLanguage()];
    QString lcid = QString("lcid=%1").arg(GetSystemDefaultLCID());
    return lang+"&"+lcid;
}

QString Setting::loadText(QString fileName){
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
//        qDebug()<<"open file failed,"<<fileName;
        return "";
    }
    QByteArray byte_array = file.readAll();
    file.close();
    if(byte_array.length()==0){
//        qDebug()<<"file length=0,"<<fileName;
        return "";
    }
    return QString(byte_array);
}

QString Setting::formatBytes(qint64 bytes){
    if(bytes<100*1024){
        return QString::number((double)bytes/1024,'f',2)+"KB";
    }else if(bytes<1024*1024*1024){
        return QString::number((double)bytes/1024/1024,'f',2)+"MB";
    }
    return QString::number((double)bytes/1024/1024/1024,'f',2)+"GB";
}

#define REG_UNINSTALL "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{0D1DA8F2-89A7-4DAC-A9EF-B55E82CDA462}}_is1"
#define DISPLAY_VERSION "DisplayVersion"
void Setting::versionCheck()
{
    qCritical()<<"versionCheck:"<<mVersion;
    mVersion = RegisterHandler::getInstance().getRegisterValue(REG_UNINSTALL,DISPLAY_VERSION,"1.0.0.0");
    mBuildVersion = mVersion.section('.',3,3).toInt();
}

QString Setting::getInstallPath()
{
    QString appPath = QCoreApplication::instance()->applicationDirPath().replace("/","\\");
    appPath = appPath.remove(appPath.right(8));
    return appPath;
}

QString Setting::getOvrLauncher(){
    if(mOVRLancher.isEmpty()){
        mOVRLancher = "\""+getInstallPath()+"\\Plugin\\OCSupport\\OVR_Launcher_x64.exe\"";
    }
    return mOVRLancher;
}

bool Setting::exportLog(QString zipFile)
{
    zipFile = zipFile.replace("file:///","").replace("/","\\");
    if(QFile(zipFile).exists()){
        QFile(zipFile).remove();
    }

    QString pimaxZip = zipFile;
    pimaxZip = pimaxZip.replace(".zip","-pimax.zip");
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    dir = dir.replace("PiTool","Pimax");
    qInfo()<<"exportLog "<<dir;
    bool ret = JlCompress::compressDir(pimaxZip,dir);
    if(!ret){
        return false;
    }

    QString pimax1Zip = zipFile;
    pimax1Zip = pimax1Zip.replace(".zip","-pimax1.zip");
    char cdir[20];
    SHGetFolderPathA(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, cdir);
    QString dir1(cdir);
    dir1.append("\\pimax");
    qInfo()<<"exportLog "<<dir;
    bool ret1 = JlCompress::compressDir(pimax1Zip,dir1);
    if(!ret1){
      return false;
  }

    QString steamZipFile = zipFile;
    steamZipFile = steamZipFile.replace(".zip","-steamvr.zip");;
    std::string jsonPath;
    StringUniversal::GetLocalAppDataPath(jsonPath);
    jsonPath += "\\openvr\\openvrpaths.vrpath";
    std::vector<std::wstring> logPaths;
    JsonCommonHandler::ReadJsonValue(jsonPath,TEXT("log"),logPaths);
    if(logPaths.size() > 0 && QDir(StringUniversal::to_string(logPaths[0]).c_str()).exists()){
        ret = JlCompress::compressDir(steamZipFile,StringUniversal::to_string(logPaths[0]).c_str());
    }

    QStringList zipFiles;
    zipFiles.append(steamZipFile);
    zipFiles.append(pimaxZip);
    zipFiles.append(pimax1Zip);
    ret = JlCompress::compressFiles(zipFile,zipFiles);
    if(QFile(steamZipFile).exists()){
        QFile(steamZipFile).remove();
    }
    if(QFile(pimaxZip).exists()){
        QFile(pimaxZip).remove();
    }
    if(QFile(pimax1Zip).exists()) {
        QFile(pimax1Zip).remove();
    }
    return true;
}


void Setting::restart(){
    SingleApplication *app = (SingleApplication*)QApplication::instance();
    app->stopLocalServer();

    QProcess p(0);
    p.setWorkingDirectory(QCoreApplication::instance()->applicationDirPath().replace("/","\\"));
    QStringList args;
    args.append("restart");
    p.startDetached("PiTool.exe", args);
}

void Setting::restoreToDefault()
{
    setAutoRun(true);
    emit aotuRunChanged(true);
    closeToNotibar(true);
    emit closeToNotibarSettingChanged(true);
}


#define KEY_CLOSE_TO_NOTIBAR "Close_To_Notibar"
bool Setting::isCloseToNotibar()
{
    mCloseToNotibar = RegisterHandler::getInstance().getRegisterValue(REG_PITOOL_PATH,KEY_CLOSE_TO_NOTIBAR,1) == 1;
    return mCloseToNotibar;
}

void Setting::closeToNotibar(bool hide)
{
    if(mCloseToNotibar != hide){
        mCloseToNotibar = hide;
        RegisterHandler::getInstance().setRegisterValue(REG_PITOOL_PATH,KEY_CLOSE_TO_NOTIBAR,mCloseToNotibar?1:0);
    }
}


//TODO
quint64 Setting::getDiskFreeSpace(QString driver)
{
    LPCWSTR lpcwstrDriver=(LPCWSTR)driver.utf16();
    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
    if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
    {
        qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";
        return 0;
    }
    return (quint64) liTotalFreeBytes.QuadPart/1024;
}

#define REG_PIPLAY_PATH "HKEY_CURRENT_USER\\SOFTWARE\\PiPlay2"
#define KEY_DOWNLOAD_PATH "DownloadPath"
QString Setting::getDownloadDir(){
    if(mDownloadPath.isEmpty()){
        mDownloadPath = RegisterHandler::getInstance().getRegisterValue(REG_PIPLAY_PATH,KEY_DOWNLOAD_PATH,"");
        if(!mDownloadPath.isEmpty()){
            QDir().mkpath(mDownloadPath);
            return mDownloadPath;
        }
        QFileInfoList driveList = QDir::drives();
        QString defaultDrive = "";
        quint64 size = 0;
        for(QFileInfo fileInfo:driveList){
            if(fileInfo.absoluteFilePath().toLower().contains("c")){
                continue;
            }
            quint64 driveSize = getDiskFreeSpace(fileInfo.absolutePath());
            if(driveSize>size){
                defaultDrive = fileInfo.absolutePath();
                size = driveSize;
            }
        }
        if(defaultDrive.isEmpty()){
            defaultDrive = "C:\\";
        }
        mDownloadPath = defaultDrive.replace("/","\\") + "Pimax\\Downloads";
        RegisterHandler::getInstance().setRegisterValue(REG_PIPLAY_PATH,KEY_DOWNLOAD_PATH,mDownloadPath);
    }
    QDir().mkpath(mDownloadPath);
    return mDownloadPath;
}

#define HTTP_REQUEST_ADDR "http://piserver.pimaxvr.com:17731/piplay_v3/"
QString Setting::getHttpRequestAddr(){
    if(mHttpRequestAddr.isEmpty()){
        mHttpRequestAddr = RegisterHandler::getInstance().getRegisterValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\PiPlay2","http_url",HTTP_REQUEST_ADDR);
    }
    return mHttpRequestAddr;
}

void Setting::callQmlFunc(const char *func){
    if(mQmlRootObject){
        QVariant returnedValue;
        QMetaObject::invokeMethod(mQmlRootObject, func, Q_RETURN_ARG(QVariant, returnedValue));
    }
}

QString Setting::privacy_old(){
    if(mPrivacy.isEmpty()){
        QString readmeFile = ":/privacy_en.txt";
        if(!language_list.isEmpty()){
            QString lang_str = language_list.at(getLanguageByStr());
            if(lang_str.compare("pitool_cn") == 0)
                readmeFile = ":/privacy.txt";
        }
        qInfo()<<readmeFile;
        QFile file(readmeFile);
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
            qDebug()<<"getPrivacy open file failed";
            return "";
        }
        QByteArray byte_array = file.readAll();
        file.close();
        if(byte_array.length()==0){
            qDebug()<<"getPrivacy open byte_array==0";
            return "";
        }
        mPrivacy.prepend(byte_array);
    }
    return mPrivacy;
}

QString Setting::privacy(){
    if(mPrivacy.isEmpty() && !language_list.isEmpty()){
        QString lang_str = language_list.at(getLanguageByStr());
        QString appPath = QCoreApplication::instance()->applicationDirPath().replace("/","\\");
        QString privacy_path = appPath+"\\language\\privacy\\" + lang_str + "_privacy.txt";
        QFile file(privacy_path);
        if(file.exists() && file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QByteArray byte_array = file.readAll();
            file.close();
            mPrivacy.prepend(byte_array);
            return mPrivacy;
        }
    }
    return privacy_old();
}

void Setting::showAlert(QString message){
    if(mQmlRootObject){
        QVariant returnedValue;
        QMetaObject::invokeMethod(mQmlRootObject, "showAlertPopup", Qt::DirectConnection,Q_RETURN_ARG(QVariant, returnedValue),
                                  Q_ARG(QVariant,message));
    }
}

void Setting::showAlertEx(QString message_id){
    QJsonObject jsonRoot = JsonHandler::loadJsonObject(":/QtString.txt");
    if(gInstances->getLanguage()==LANGUAGE_ENGLISH){
        message_id = message_id+".en";
    }
    QString message = JsonHandler::getJsonValue(jsonRoot,message_id,"");
    if(message.isEmpty()){
        return;
    }
    showAlert(message);
}

void Setting::showMessage(QString message){
    if(mQmlRootObject){
        QVariant returnedValue;
        QMetaObject::invokeMethod(mQmlRootObject, "showTrayMessage", Qt::DirectConnection,Q_RETURN_ARG(QVariant, returnedValue),
                                  Q_ARG(QVariant,message));
    }
}
