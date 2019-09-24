#include "SettingModel.h"
#include "Windows.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QDebug>
#include <QSettings>
#include <QTranslator>
#include <QCoreApplication>
#include <QMetaObject>
#include "quazip/JlCompress.h"
#include <QNetworkInterface>
#include <QList>
#include <QProcess>
#include <QDateTime>
#include "SingleApplication.h"

#define REG_PIPLAY_PATH "HKEY_CURRENT_USER\\SOFTWARE\\PiPlay2"
#define REG_PIMAX_PATH "HKEY_LOCAL_MACHINE\\SOFTWARE\\Pimax"
#define REG_INSTALL "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\PiPlayDesktop.exe"

#define HTTP_REQUEST_ADDR "http://piserver.pimaxvr.com:17731/piplay_v3/"
//#define HTTP_REQUEST_ADDR "http://118.89.106.249:8080/piplay/"

QString mInstallPath=""; //程序安装路径
QString mDownloadPath="";//文件下载路径
QString mVideoPlayer="";
QString mVideoPlayer2d="";
QString mOVRLancher = "";
QString mHttpRequestAddr = "";
QObject *mQmlRootObject = nullptr;
SettingModel *gInstances = nullptr;

quint64 SettingModel::getDiskFreeSpace(QString driver)
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

SettingModel::SettingModel(QObject *parent) : QObject(parent)
{
    qDebug()<<"GetSystemDefaultLCID"<<GetSystemDefaultLCID();
    gInstances = this;
    mLanguage = -1;
    mVersion = "";
    mBuildVersion = 0;
    getLanguage();
    getInstallPath();
}

SettingModel* SettingModel::getInstance(){
    if(!gInstances){
       static SettingModel settingModel;
       gInstances = &settingModel;
    }
    return gInstances;
}

#define KEY_INSTALL_PATH "InstallPath"
QString SettingModel::getInstallPath(){
    if(mInstallPath.isEmpty()){
        mInstallPath = getRegisterValue(REG_PIMAX_PATH,KEY_INSTALL_PATH,"");
    }
    if(mInstallPath.isEmpty()){
        mInstallPath = getRegisterValue(REG_PIPLAY_PATH,KEY_INSTALL_PATH,"");
        setRegisterValue(REG_PIMAX_PATH,KEY_INSTALL_PATH,mInstallPath);
    }

    if(!QDir(mInstallPath+"\\PiPlay2").exists()){
        mInstallPath = "";
    }
    if(mInstallPath.isEmpty()){
        mInstallPath = QCoreApplication::instance()->applicationDirPath().replace("/","\\").remove("\\Runtime");
        setRegisterValue(REG_PIMAX_PATH,KEY_INSTALL_PATH,mInstallPath);
    }
    qDebug()<<"getInstallPath"<<mInstallPath;
    return mInstallPath;
}

//version
void SettingModel::loadVersion(){
    mBuildVersion = 1;
    mVersion = "2.0.1";

    QString versionFile = ":/version.txt";
    QJsonObject jsonRoot = SettingModel::loadJsonObject(versionFile);
    mBuildVersion = SettingModel::getJsonValue(jsonRoot,"build",mBuildVersion);
    QString version = SettingModel::getJsonValue(jsonRoot,"version","2.0.1");
//            QString build_date = SettingModel::getJsonValue(jsonRoot,"build_date","");
    mVersion = QString("%1.%2").arg(version).arg(mBuildVersion);

}
QString SettingModel::version(){
    if(mVersion.isEmpty()){
        loadVersion();
    }
    return mVersion;
}
int SettingModel::buildVersion(){
    if(mBuildVersion==0){
        loadVersion();
    }
    return mBuildVersion;
}

#define KEY_LANGUAGE "Language"
int SettingModel::getLanguage(){
    if(mLanguage==-1){
        mLanguage = getRegisterValue(REG_PIPLAY_PATH,KEY_LANGUAGE,-1);
        if(mLanguage==-1){
            int lcid = GetSystemDefaultLCID();
            if(lcid==2052||lcid==3076||lcid==1028){//中文（香港） 3076 中文（中国） 2052 中文（台湾） 1028
                mLanguage = LANGUAGE_CHINESE;
            }else if(lcid==1041){
                mLanguage = LANGUAGE_JAPANESE;
//          }else if(lcid==1031){
//                mLanguage = LANGUAGE_GERMAN;
            }else{
                mLanguage = LANGUAGE_ENGLISH;
            }
        }
    }
    return mLanguage;
}
void SettingModel::setLanguage(int language){
    if(mLanguage!=language&&language>=0&&language<LANGUAGE_COUNT){
        mLanguage = language;
        setRegisterValue(REG_PIPLAY_PATH,KEY_LANGUAGE,language);
    }
}

void SettingModel::loadTranslate(){
    int mLanguage = getRegisterValue(REG_PIPLAY_PATH,KEY_LANGUAGE,-1);
    if(mLanguage==-1){
        int lcid = GetSystemDefaultLCID();
        if(lcid==2052||lcid==3076||lcid==1028){//中文（香港） 3076 中文（中国） 2052 中文（台湾） 1028
            mLanguage = LANGUAGE_CHINESE;
        }else if(lcid==1041){
            mLanguage = LANGUAGE_JAPANESE;
//      }else if(lcid==1031){
//          mLanguage = LANGUAGE_GERMAN;
        }else{
            mLanguage = LANGUAGE_ENGLISH;
        }
    }

    QString appPath = QCoreApplication::instance()->applicationDirPath().replace("/","\\");

    QString translatorFileName = appPath+"\\language\\piplay_en.qm";
    if(mLanguage==LANGUAGE_CHINESE){
        translatorFileName = appPath+"\\language\\piplay_cn.qm";
    }else if(mLanguage==LANGUAGE_JAPANESE){
        translatorFileName = appPath+"\\language\\piplay_jp.qm";
    }else if(mLanguage==LANGUAGE_GERMAN){
        translatorFileName = appPath+"\\language\\piplay_ge.qm";
    }
    QTranslator *translator = new QTranslator(QCoreApplication::instance());
    if (translator->load(translatorFileName)){
        qDebug()<<"load translator success"<<translatorFileName;
        QCoreApplication::instance()->installTranslator(translator);
    }else{
        qCritical()<<"load translator error"<<translatorFileName;
    }
}


#define KEY_DOWNLOAD_PATH "DownloadPath"
QString SettingModel::getDownloadDir(){
    if(mDownloadPath.isEmpty()){
        mDownloadPath = getRegisterValue(REG_PIPLAY_PATH,KEY_DOWNLOAD_PATH,"");
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
        setRegisterValue(REG_PIPLAY_PATH,KEY_DOWNLOAD_PATH,mDownloadPath);
    }
    QDir().mkpath(mDownloadPath);
    return mDownloadPath;
}
void SettingModel::setDownloadDir(QString dir){
    if(dir.indexOf("file:///")==0){
        dir = dir.right(dir.length()-QString("file:///").length());
        dir = dir.replace("/","\\");
    }
    if(QDir(dir).exists()&&dir.compare(mDownloadPath,Qt::CaseInsensitive)!=0){
        mDownloadPath = dir;
        setRegisterValue(REG_PIPLAY_PATH,KEY_DOWNLOAD_PATH,mDownloadPath);
        emit downloadDirChanged(mDownloadPath);
    }
}

#define KEY_VIDEO_PLAYER "VideoPlayer"
QString SettingModel::getVideoPlayer(){
    if(mVideoPlayer.isEmpty()){
        mVideoPlayer = getRegisterValue(REG_PIPLAY_PATH,KEY_VIDEO_PLAYER,"");
        if(mVideoPlayer.isEmpty()||!QFile(mVideoPlayer).exists()){
            mVideoPlayer = getInstallPath()+"\\AutoplayVR\\AutoplayVR.exe";
            setRegisterValue(REG_PIPLAY_PATH,KEY_VIDEO_PLAYER,mVideoPlayer);
        }
    }
    return mVideoPlayer;
}
void SettingModel::setVideoPlayer(QString videoPlayer){
    if(videoPlayer.indexOf("file:///")==0){
        videoPlayer = videoPlayer.right(videoPlayer.length()-QString("file:///").length());
        videoPlayer = videoPlayer.replace("/","\\");
    }
    if(QFile(videoPlayer).exists()){
        mVideoPlayer = videoPlayer;
        setRegisterValue(REG_PIPLAY_PATH,KEY_VIDEO_PLAYER,mVideoPlayer);
        emit videoPlayerChanged(mVideoPlayer);
    }
}

#define KEY_VIDEO_PLAYER_2D "VideoPlayer3D"
QString SettingModel::getVideoPlayer2D(){
    if(mVideoPlayer2d.isEmpty()){
        mVideoPlayer2d = getRegisterValue(REG_PIPLAY_PATH,KEY_VIDEO_PLAYER_2D,"");
        if(mVideoPlayer2d.isEmpty()||!QFile(mVideoPlayer2d).exists()){
            mVideoPlayer2d = getInstallPath()+"\\UnityPVRPlayer\\UnityPVRPlayer.exe";
            setRegisterValue(REG_PIPLAY_PATH,KEY_VIDEO_PLAYER_2D,mVideoPlayer2d);
        }
    }
    return mVideoPlayer2d;
}

void SettingModel::setVideoPlayer2D(QString videoPlayer){
    if(videoPlayer.indexOf("file:///")==0){
        videoPlayer = videoPlayer.right(videoPlayer.length()-QString("file:///").length());
        videoPlayer = videoPlayer.replace("/","\\");
    }
    if(QFile(videoPlayer).exists()){
        mVideoPlayer2d = videoPlayer;
        setRegisterValue(REG_PIPLAY_PATH,KEY_VIDEO_PLAYER_2D,mVideoPlayer2d);
        emit videoPlayer2DChanged(mVideoPlayer2d);
    }
}

#define REG_AUTORUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
bool SettingModel::isAutoRun(){
    QSettings reg(REG_AUTORUN,QSettings::NativeFormat);
    reg.remove("PiPlay2");
    QString application = getRegisterValue(REG_AUTORUN,"PiPlay","");
    if(QFile(application).exists()){
        return true;
    }
    return false;
}

void SettingModel::setAutoRun(bool autoRun){
    if(mAutoRun==autoRun){
        return;
    }
    QSettings reg(REG_AUTORUN,QSettings::NativeFormat);
    if(autoRun){
        reg.setValue("PiPlay",QCoreApplication::instance()->applicationFilePath().replace("/","\\"));
    }else{
        reg.remove("PiPlay");
    }
    mAutoRun = autoRun;
}

#define KEY_BETA_TEST "Beta_Test"
bool SettingModel::isBetaTest(){
    mBetaTest =  getRegisterValue(REG_PIPLAY_PATH,KEY_BETA_TEST,1)==1;
    return mBetaTest;
}

void SettingModel::setBetaTest(bool betaTest){
    if(mBetaTest!=betaTest){
        mBetaTest = betaTest;
        setRegisterValue(REG_PIPLAY_PATH,KEY_BETA_TEST,mBetaTest?1:0);
    }
}

#define KEY_PIPLAY_BETA_TEST "Piplay_Beta_Test"
bool SettingModel::isPiplayBetaTest(){
    mPiplayBetaTest =  getRegisterValue(REG_PIPLAY_PATH,KEY_PIPLAY_BETA_TEST,1)==1;
    return mPiplayBetaTest;
}

void SettingModel::setPiplayBetaTest(bool piplayBetaTest){
    if(mPiplayBetaTest!=piplayBetaTest){
        mPiplayBetaTest = piplayBetaTest;
        setRegisterValue(REG_PIPLAY_PATH,KEY_PIPLAY_BETA_TEST,mPiplayBetaTest?1:0);
    }
}

#define KEY_CLOSE_TO_NOTIBAR "Close_To_Notibar"
bool SettingModel::isCloseToNotibar(){
    mCloseToNotibar =  getRegisterValue(REG_PIPLAY_PATH,KEY_CLOSE_TO_NOTIBAR,1)==1;
    return mCloseToNotibar;
}

void SettingModel::setCloseToNotibar(bool closeToNotibar){
    if(mCloseToNotibar!=closeToNotibar){
        mCloseToNotibar = closeToNotibar;
        setRegisterValue(REG_PIPLAY_PATH,KEY_CLOSE_TO_NOTIBAR,mCloseToNotibar?1:0);
    }
}

QString SettingModel::getOvrLauncher(){
    if(mOVRLancher.isEmpty()){
        mOVRLancher = "\""+getInstallPath()+"\\Plugin\\OCSupport\\OVR_Launcher_x64.exe\"";
    }
    return mOVRLancher;
}

double SettingModel::getJsonValue(QJsonObject json,QString key,double defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isDouble()){
            return value.toDouble();
        }
    }
    return defValue;
}

int SettingModel::getJsonValue(QJsonObject json,QString key,int defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isDouble()){
            return value.toInt();
        }
    }
    return defValue;
}

quint64 SettingModel::getJsonValue_long(QJsonObject json,QString key,quint64 defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isDouble()){
            return (quint64)value.toVariant().toULongLong();
        }
    }
    return defValue;
}


bool SettingModel::getJsonValue(QJsonObject json,QString key,bool defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isBool()){
            return value.toBool();
        }
    }
    return defValue;
}

QString SettingModel::getJsonValue(QJsonObject json,QString key,QString defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isString()){
            return value.toString();
        }else if(value.isDouble()){
            return QString::number(value.toInt());
        }
    }
    return defValue;
}

QString SettingModel::getJsonValue(QJsonObject json,QString key,const char *defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isString()){
            return value.toString();
        }else if(value.isDouble()){
            return QString::number(value.toInt());
        }
    }
    return QString(defValue);
}

QJsonArray SettingModel::getJsonValue(QJsonObject json,QString key,QJsonArray defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isArray()){
            return value.toArray();
        }
    }
    return defValue;
}

QJsonObject SettingModel::getJsonObjectValue(QJsonObject json,QString key,QJsonObject defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isObject()){
            return value.toObject();
        }
    }
    return defValue;
}

QJsonObject SettingModel::loadJsonObject(QString fileName){
    QJsonObject defObject;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        return defObject;
    }
    QByteArray byte_array = file.readAll();
    file.close();
    if(byte_array.length()==0){
        return defObject;
    }
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
    if(json_error.error == QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            return parse_doucment.object();
        }
    }
    return defObject;
}

QString SettingModel::getHttpRequestAddr(){
    if(mHttpRequestAddr.isEmpty()){
        mHttpRequestAddr = getRegisterValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\PiPlay2","http_url",HTTP_REQUEST_ADDR);
    }
    return mHttpRequestAddr;
}

QString SettingModel::privacy(){
    if(mPrivacy.isEmpty()){
        QString readmeFile = ":/privacy_en.txt";
        if(mLanguage==0){
            readmeFile = ":/privacy.txt";
        }
        qInfo()<<readmeFile;
        QFile file(readmeFile);
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
            qDebug()<<"getPrivacye open file failed";
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

QString SettingModel::getRegisterValue(QString dir,QString key,QString defValue){//读取注册表项
    QSettings reg(dir,QSettings::NativeFormat);
    if(reg.contains(key)){
        return reg.value(key).toString().trimmed();
    }
    return defValue;
}

void SettingModel::setRegisterValue(QString dir,QString key,QString value){//写注册表项
    QSettings reg(dir,QSettings::NativeFormat);
    reg.setValue(key,value);
}

int SettingModel::getRegisterValue(QString dir,QString key,int defValue){//读取注册表项
    QSettings reg(dir,QSettings::NativeFormat);
    if(reg.contains(key)){
        return reg.value(key).toInt();
    }
    return defValue;
}

void SettingModel::setRegisterValue(QString dir,QString key,int value){//写注册表项
    QSettings reg(dir,QSettings::NativeFormat);
    reg.setValue(key,value);
}

QString SettingModel::loadText(QString fileName){
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
QString SettingModel::getValueFromText(QString text,QString key){
    int find = text.indexOf(key);
    if(find==-1){
        return "";
    }
    int find2 = text.indexOf("\n",find+key.length());
    if(find2>find){
        QString value = text.mid(find+key.length()+1,find2-find-key.length()-1);
        return value.trimmed();
    }
    return "";
}
QString SettingModel::getValueFromText(QString text, QString key1,QString key2){
    int find = text.indexOf(key1);
    if(find==-1){
        return "";
    }
    int find1 = text.indexOf(key2,find);
    if(find1==-1){
        return "";
    }
    int find2 = text.indexOf("\n",find1+key2.length());
    if(find2>find1){
        QString value = text.mid(find1+key2.length()+1,find2-find1-key2.length()-1);
        return value.trimmed();
    }
    return "";
}

QString SettingModel::formatBytes(qint64 bytes){
    if(bytes<100*1024){
        return QString::number((double)bytes/1024,'f',2)+"KB";
    }else if(bytes<1024*1024*1024){
        return QString::number((double)bytes/1024/1024,'f',2)+"MB";
    }
    return QString::number((double)bytes/1024/1024/1024,'f',2)+"GB";
}
void SettingModel::setQMlRootObject(QObject *qmlObject){
    mQmlRootObject = qmlObject;
}
void SettingModel::showAlert(QString message){
    if(mQmlRootObject){
        QVariant returnedValue;
        QMetaObject::invokeMethod(mQmlRootObject, "showAlertPopup", Qt::DirectConnection,Q_RETURN_ARG(QVariant, returnedValue),
                                  Q_ARG(QVariant,message));
    }
}

void SettingModel::showAlertEx(QString message_id){
    QJsonObject jsonRoot = SettingModel::loadJsonObject(":/QtString.txt");
    if(gInstances->getLanguage()==LANGUAGE_ENGLISH){
        message_id = message_id+".en";
    }
    if(gInstances->getLanguage()==LANGUAGE_JAPANESE){
        message_id = message_id+".jp";
    }
    QString message = SettingModel::getJsonValue(jsonRoot,message_id,"");
    if(message.isEmpty()){
        return;
    }
    showAlert(message);
}

void SettingModel::showMessage(QString message){
    if(mQmlRootObject){
        QVariant returnedValue;
        QMetaObject::invokeMethod(mQmlRootObject, "showTrayMessage", Qt::DirectConnection,Q_RETURN_ARG(QVariant, returnedValue),
                                  Q_ARG(QVariant,message));
    }
}
void SettingModel::callQmlFunc(const char *func){
    if(mQmlRootObject){
        QVariant returnedValue;
        QMetaObject::invokeMethod(mQmlRootObject, func, Q_RETURN_ARG(QVariant, returnedValue));
    }
}


void SettingModel::showMessageEx(QString message_id){
    QJsonObject jsonRoot = SettingModel::loadJsonObject(":/QtString.txt");
    if(gInstances->getLanguage()==LANGUAGE_ENGLISH){
        message_id = message_id+".en";
    }else if(gInstances->getLanguage()==LANGUAGE_JAPANESE){
        message_id = message_id+".jp";
    }
    QString message = SettingModel::getJsonValue(jsonRoot,message_id,"");
    if(message.isEmpty()){
        return;
    }
    showMessage(message);
}

int SettingModel::checkDirectory(QString dir){
    dir = dir.replace("file:///","").replace("/","\\");
    QByteArray bytes = dir.toLocal8Bit();
    //仅支持数字，字母，下划线，冒号和斜杠
    for(int i=0;i<bytes.length();i++){
        unsigned char ch =  bytes.at(i);
        if(ch==':'||ch=='\\'||ch=='/'||ch=='_'||ch==' '||ch=='.'){
            continue;
        }
        if(ch>='0'&&ch<='9'){
            continue;
        }
        if(ch>='a'&&ch<='z'){
            continue;
        }
        if(ch>='A'&&ch<='Z'){
            continue;
        }
        qInfo()<<"checkDirectory"<<ch;
        return -1;
    }
    return 0;
}

bool SettingModel::exportLog(QString zipFile){
    zipFile = zipFile.replace("file:///","").replace("/","\\");
    if(QFile(zipFile).exists()){
        QFile(zipFile).remove();
    }
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    dir = dir.replace("PiPlay","Pimax");
    qInfo()<<"exportLog "<<dir;
    return JlCompress::compressDir(zipFile,dir);
}

void SettingModel::restart(){
    SingleApplication *app = (SingleApplication*)QApplication::instance();
    app->stopLocalServer();

    QProcess p(0);
    p.setWorkingDirectory(QCoreApplication::instance()->applicationDirPath());//指定进程的工作目录
    p.startDetached("PiTool.exe", QStringList());
}

void SettingModel::restoreToDefault(){
    setAutoRun(true);
    emit autoRunChanged(true);
    setBetaTest(true);
    emit betaTestChanged(true);
    setPiplayBetaTest(true);
    emit piplayBetaTestChanged(true);
    setCloseToNotibar(true);
    emit closeToNotibarChanged(true);
    setVideoPlayer(getInstallPath()+"\\AutoplayVR\\AutoplayVR.exe");
    setVideoPlayer2D(getInstallPath()+"\\UnityPVRPlayer\\UnityPVRPlayer.exe");
}

bool SettingModel::processIsRunning(QString processName){
    QString taskList = QString("tasklist /FI \"imagename eq %1\"").arg(processName);
    QProcess process;
    process.start(taskList);
    if(process.waitForFinished()){
        QByteArray byteArray = process.readAll();
        QString result = QString::fromLocal8Bit(byteArray);
//        qInfo()<<"processIsRunning"<<processName<<result;
        if(result.indexOf(processName,0,Qt::CaseInsensitive)>=0){
            return true;
        }
        return false;
    }
    return false;
}

void SettingModel::killProcess(QString processName){
    if(processIsRunning(processName)){
        QString command = "taskkill /F /IM "+processName;
        QProcess().execute(command);
    }
}

QString SettingModel::getLangAndLcid(){
    QString languages[]={"cn","en","jp","ge"};
    QString lang = "lang="+languages[getLanguage()];
    QString lcid = QString("lcid=%1").arg(GetSystemDefaultLCID());
    return lang+"&"+lcid;

}

#define REG_DRPI_PATH "\\HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{2AE8DF30-DD95-4473-957F-6F6F263AE8E5}_is1"
bool SettingModel::drPiIsInstalled(){
    QString path = SettingModel::getRegisterValue(REG_DRPI_PATH,"InstallLocation","");
    if(path.isEmpty()){
        return false;
    }
    QString exe = path+"\\Dr.Pi.exe";
    if(!QFile(exe).exists()){
        return false;
    }
    return true;
}

void SettingModel::openDrPi(){
    QString path = SettingModel::getRegisterValue(REG_DRPI_PATH,"InstallLocation","");
    if(path.isEmpty()){
        return;
    }
    QString exe = path+"\\Dr.Pi.exe";
    if(!QFile(exe).exists()){
        return;
    }
    QProcess().startDetached(exe,QStringList(),path);
}

