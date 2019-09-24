#include "VersionModel.h"
#include <QDir>
#include <QStandardPaths>
#include <QProcess>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "quazip/JlCompress.h"

#define CODE_SUCCESS 8888
VersionItem::VersionItem(QObject *parent) : QObject(parent)
{
    mTotalBytes = 100;
    mReadBytes = 0;
    m_pReply = nullptr;
    m_pNetworkManager = nullptr;
    mStatus = STATUS_INIT;
    mUpgradeFlag = 0;
}

void VersionItem::setTotalBytes(qint64 totalBytes){
    if(mTotalBytes!=totalBytes+mDownloadedSize){
        mTotalBytes = totalBytes+mDownloadedSize;
        emit totalBytesChanged(mTotalBytes);
    }
}

void VersionItem::setReadBytes(qint64 readBytes){
    mReadBytes = readBytes+mDownloadedSize;
    if(mReadBytes>mTotalBytes){
        mTotalBytes = mReadBytes*100;
        emit totalBytesChanged(mTotalBytes);
    }
    emit readBytesChanged(mReadBytes);
}

void VersionItem::setStatus(int status)
{
    mStatus=status;
    emit statusChanged(mStatus);
}

bool VersionItem::load(QJsonObject item){
    mAppName = JsonHandler::getJsonValue(item,"app_id","");
    mVersion = JsonHandler::getJsonValue(item,"version","");
    mBuildVersion = JsonHandler::getJsonValue(item,"buildVersion",0);
    mDownloadUrl = JsonHandler::getJsonValue(item,"download_url","");
    if(mAppName.isEmpty()||mVersion.isEmpty()||mBuildVersion==0||mDownloadUrl.isEmpty()){
        qInfo()<<"VersionItem::load"<<mAppName<<mVersion<<mBuildVersion<<mDownloadUrl;
        setStatus(STATUS_CHECKED);
        setUpgradeFlag(UPGRADE_FLAG_NOUPGRADE);
        return false;
    }
    mBetaFlag = JsonHandler::getJsonValue(item,"beta_flag",0);
    mUpgradeContent = JsonHandler::getJsonValue(item,"update_content","");
    int upgradeFlag = JsonHandler::getJsonValue(item,"upgrade_flag",0);
    qInfo()<<"VersionItem::load"<<mAppName<<mVersion<<mBuildVersion<<upgradeFlag;
    emit versionChanged(mVersion);
    emit upgradeContentChanged(mUpgradeContent);


    QString postFix = getPosfix(mDownloadUrl);
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).replace("/","\\");
    QDir().mkdir(filePath);
    mFileName = filePath+"\\"+mAppName+"_"+mVersion+"."+postFix;
    setStatus(STATUS_CHECKED);
    setUpgradeFlag(upgradeFlag);
    qInfo()<<"VersionItem::load mFileName,"<<mDownloadUrl<<mFileName<<mUpgradeFlag<<mStatus;
    if(mAppName!="piplay"){
        return true;
    }

    QString releaseNote = filePath+"\\release_"+mVersion+".txt";
    QFile hFile(releaseNote);
    if(!hFile.open(QIODevice::WriteOnly)){
        return true;
    }
    hFile.write(mUpgradeContent.toLocal8Bit());
    hFile.close();
    return true;
}

bool VersionItem::startDownload(){
    if(mStatus!=STATUS_CHECKED&&mStatus!=STATUS_DOWNLOAD_FAILED){
        return false;
    }
    if(QFile(mFileName).exists()&&!mFileName.contains(".tmp")){
        mTotalBytes = QFile(mFileName).size();
        mReadBytes = mTotalBytes;
        emit totalBytesChanged(mTotalBytes);
        emit readBytesChanged(mReadBytes);
        setStatus(STATUS_DOWNLOADED);
        return true;
    }
    if(mFileName.contains(".tmp")==0){
        mFileName = mFileName +".tmp";
    }
    if (m_pNetworkManager == nullptr){
        m_pNetworkManager = new QNetworkAccessManager(this);
    }
    QNetworkRequest request;
    request.setUrl(mDownloadUrl);
    m_hFile = new QFile(mFileName);
    if(!m_hFile->open(QIODevice::WriteOnly|QIODevice::Append)){
        qCritical()<<"startDownload:file open error,"<<mFileName;
        delete m_hFile;
        m_hFile = nullptr;
        return false;
    }
    if(mDownloadedSize>mTotalBytes&&mTotalBytes>100){
        httpFinished();
        return false;
    }
    mDownloadedSize = m_hFile->size();
    m_hFile->seek(mDownloadedSize);
    mTotalBytes = mDownloadedSize;
    request.setRawHeader("Range",tr("bytes=%1-").arg(mDownloadedSize).toUtf8());
    setStatus(STATUS_DOWNLOADING);
    m_pReply = m_pNetworkManager->get(request);
    connect(m_pReply,SIGNAL(finished()),this,SLOT(httpFinished()));
    connect(m_pReply,SIGNAL(downloadProgress(qint64,qint64)), this,SLOT(downloadProgress(qint64,qint64)));
    connect(m_pReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(httpError(QNetworkReply::NetworkError)));
    connect(m_pReply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
    return true;
}

bool VersionItem::startDownloadByUrl(QString url,QString name)
{
    mDownloadUrl = url;
    mAppName = name;
    QString postFix = getPosfix(mDownloadUrl);
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).replace("/","\\");
    QDir().mkdir(filePath);
    mFileName = filePath+"\\"+mAppName+"."+postFix;
    setStatus(STATUS_CHECKED);
    qInfo()<<"VersionItem::load mFileName,"<<mDownloadUrl<<mFileName<<mStatus;
    return startDownload();
}

bool VersionItem::stopDownload(){
    qInfo()<<"stopDownload"<<mFileName;
    if(mStatus==STATUS_DOWNLOADING){
        if(m_pReply!=nullptr){
            m_pReply->abort();
            m_pReply->deleteLater();
        }
        if(m_hFile!=nullptr&&m_hFile->isOpen()){
            m_hFile->close();
            delete m_hFile;
            m_hFile=nullptr;
        }
        setStatus(STATUS_CHECKED);
        return true;
    }
    return false;
}

bool VersionItem::install(){
    if(mStatus!=STATUS_DOWNLOADED){
        return false;
    }
    if(getPosfix(mFileName)=="zip"){
        QStringList fileList = JlCompress::getFileList(mFileName);
        QString tempPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).replace("/","\\");
        for(int i=0;i<fileList.size();i++){
            QString fileName = fileList.at(i);//
            if(getPosfix(fileName)=="exe"){
                QString exeFile = tempPath+"\\"+fileName;
                if(!QFile(exeFile).exists()){
                    JlCompress::extractFile(mFileName,fileName,exeFile);
                }
                QProcess process;
                process.startDetached("\""+tempPath+"\\"+fileName+"\"");
                return true;
            }
        }
        return false;
    }else if(getPosfix(mFileName)=="exe"){
        QProcess process;
        process.startDetached("\""+mFileName+"\"");
        return true;
    }
    return false;
}

QString VersionItem::getVersionFile(){
    return mFileName;
}

void VersionItem::clearData(){
    QFile(mFileName).remove();
    setStatus(0);
    setUpgradeFlag(0);
}



void VersionItem::httpFinished(){
    qInfo()<<("httpFinished");
    if(m_pReply!=nullptr){
        m_pReply->deleteLater();
        m_pReply = nullptr;
    }
    if(m_hFile!=nullptr){
        m_hFile->flush();
        m_hFile->close();
        delete m_hFile;
        m_hFile = nullptr;
    }
    if(mStatus==STATUS_DOWNLOAD_FAILED){
        qInfo()<<"download finished, mStatus==STATUS_DOWNLOAD_FAILED";
        QFile(mFileName).remove();
        return;
    }
    if((mReadBytes>=mTotalBytes||mDownloadedSize>=mTotalBytes)&&mTotalBytes>100){
        qInfo()<<"download finished, rename exe file"<<mReadBytes<<mTotalBytes;
        mFileName = mFileName.remove(".tmp");
        QFile(mFileName+".tmp").rename(mFileName);
        if(getPosfix(mFileName)=="zip"){//解压
            QStringList fileList = JlCompress::getFileList(mFileName);
            QString tempPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).replace("/","\\");
            JlCompress::extractDir(mFileName,tempPath);
        }
        qInfo()<<"download finished, set status downloaded";
        setStatus(STATUS_DOWNLOADED);
    }else{
        setStatus(STATUS_CHECKED);
        qInfo()<<"download finished, set status checked";
    }
}

void VersionItem::httpReadyRead(){
    if(mStatus!=STATUS_DOWNLOADING){
        qCritical()<<"download failed"<<m_pReply->readAll();
        return;
    }
    QByteArray byteArray = m_pReply->readAll();
    QString data = QString(byteArray).toLower();
    if(data.indexOf("error")>=0||data.indexOf("not found")>=0){
        qCritical()<<"download failed"<<byteArray;
        setStatus(STATUS_DOWNLOAD_FAILED);
        return;
    }
    m_hFile->write(byteArray);
}

void VersionItem::downloadProgress(qint64 readBytes, qint64 totalBytes){
    if(mStatus!=STATUS_DOWNLOADING){
        return;
    }
    setTotalBytes(totalBytes);
    setReadBytes(readBytes);
}

void VersionItem::httpError(QNetworkReply::NetworkError code){
    qCritical()<<"httpError"<<code;
    if(code==QNetworkReply::OperationCanceledError){
        return;
    }
    if(code==QNetworkReply::UnknownContentError){
        httpFinished();
        return;
    }
    setStatus(STATUS_DOWNLOAD_FAILED);
    stopDownload();
}


QString VersionItem::getPosfix(QString url){
    QString posfix = url.mid(url.lastIndexOf(".")+1);
    if(posfix.indexOf("?")>0){
        posfix = posfix.left(posfix.indexOf("?"));
    }
    if(posfix.isEmpty()){
        return "exe";
    }
    return posfix.toLower();
}


VersionModel::VersionModel(QObject *parent) : QObject(parent)
{
    mPiPlayVersion = new VersionItem();
    mFirmwareVersion = new VersionItem();
    mPihomeVersion = new VersionItem();
    mLeapMotionVersion = new VersionItem();
    clearCache();
}

VersionModel::~VersionModel(){
    delete mPiPlayVersion;
    delete mFirmwareVersion;
    delete mPihomeVersion;
    delete mLeapMotionVersion;
}


QString VersionModel::getError(){
    return mError;
}

void VersionModel::clearCache(){
    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).replace("/","\\");
    QDir dir(cacheDir);
    if(!dir.exists()){
        return;
    }
    QStringList nameFilters;
    nameFilters.append("*.exe");
    nameFilters.append("*.bin");
    nameFilters.append("*.dfu");
    for(QString fileName:dir.entryList(nameFilters)){
        qInfo()<<"clearCache delete file"<<fileName;
        QFile file(cacheDir+"\\"+fileName);
        file.remove();
    }
}

void VersionModel::checkPiPlayVersion(int buildVersion,bool supportBeta){
    qInfo()<<"VersionItem::load>>>>>>>>>>>>>>>>>>>>>>>"<<buildVersion;
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parsePiPlayData(OnlineRequest *,int,QString,QString,QJsonObject)));
    QString param = QString("app=pihelp&version=%1&supportBeta=%2")
            .arg(buildVersion).arg(supportBeta?1:0);
    request->getHttpRequest("version/check",param);
}

void VersionModel::checkPiHomeVersion(int buildVersion,bool supportBeta){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parsePiHomeData(OnlineRequest *,int,QString,QString,QJsonObject)));
    QString param = QString("app=pihome&version=%1&supportBeta=%2")
            .arg(buildVersion).arg(supportBeta?1:0);
    request->getHttpRequest("version/check",param);
}

void VersionModel::checkFirmwareVersion(int buildVersion,bool supportBeta,QString hmdName){
    if(mFirmwareVersion->status()>2){
        return;
    }
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseFirmwareData(OnlineRequest *,int,QString,QString,QJsonObject)));
    hmdName=hmdName.toLower().remove("pimax").trimmed();
    QString param = QString("app=dfu-%1&version=%2&supportBeta=%3")
            .arg(hmdName).arg(buildVersion).arg(supportBeta?1:0);
    qInfo()<<"checkFirmwareVersion"<<buildVersion<<supportBeta<<hmdName;
    request->getHttpRequest("version/check",param);
}

void VersionModel::parsePiPlayData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject){
    qInfo()<<"parsePiPlayData"<<code<<type<<message;
    delete request;
    if(code!=CODE_SUCCESS){
        mError = message+QString::number(code);
        qDebug()<<"parseUpgradeData code!=8888"<<mError;
        mPiPlayVersion->setStatus(VersionItem::STATUS_CHECK_FAILED);
        return;
    }
    QJsonObject defValue;
    mPiPlayVersion->load(JsonHandler::getJsonObjectValue(dataObject,"item",defValue));
    emit piPlayVersionChanged(mPiPlayVersion);
}

void VersionModel::parsePiHomeData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject){
    delete request;
    if(code!=CODE_SUCCESS){
        mError = message+QString::number(code);
        qDebug()<<"parseUpgradeData code!=8888"<<mError;
        mPihomeVersion->setStatus(VersionItem::STATUS_CHECK_FAILED);
        return;
    }
    QJsonObject defValue;
    mPihomeVersion->load(JsonHandler::getJsonObjectValue(dataObject,"item",defValue));
    emit piPlayVersionChanged(mPihomeVersion);
}
void VersionModel::parseFirmwareData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject){
    delete request;
    if(code!=CODE_SUCCESS){
        mError = message+QString::number(code);
        qDebug()<<"parseUpgradeData code!=8888"<<mError;
        mFirmwareVersion->setStatus(VersionItem::STATUS_CHECK_FAILED);
        return;
    }
    QJsonObject defValue;
    mFirmwareVersion->load(JsonHandler::getJsonObjectValue(dataObject,"item",defValue));
    emit piPlayVersionChanged(mFirmwareVersion);
}

QString VersionModel::release_note(QString version){
    if(mReleaseNote.isEmpty()){
        QString releaseFile = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).replace("/","\\")+"\\release_"+version+".txt";
        if(!QFile(releaseFile).exists()){
            return "";
        }
        QFile hFile(releaseFile);
        if(!hFile.open(QIODevice::ReadOnly)){
            return "";
        }
        mReleaseNote = QString::fromLocal8Bit(hFile.readAll());
        hFile.close();
        QFile(releaseFile).remove();
    }
    return mReleaseNote;
}


