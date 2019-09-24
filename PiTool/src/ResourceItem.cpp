#include "ResourceItem.h"
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QCoreApplication>
#include "quazip/JlCompress.h"
#include "ResourceModel.h"
#include "Setting.h"

ResourceItem::ResourceItem(QObject *parent) : QObject(parent)
{
    mTotalBytes = 100;
    mReadBytes = 0;
    m_pReply = nullptr;
    m_pNetworkManager = nullptr;
    mSpeed = 0;
    mDownloadedSize = 0;
    mCopyObject = nullptr;
    mJsonFile = "";
    mStatus = STATUS_LOCAL;
    mVersion = "1.0.1";
    mUpgradeFlag = 0;
    mBought = false;
}

ResourceItem::ResourceItem(QString id, QString thirdId,QString type,QString subType,QString download_url,QString image_url,QString title,QString desc){
    mTotalBytes = 100;
    mReadBytes = 0;
    m_pReply = nullptr;
    m_pNetworkManager = nullptr;
    mSpeed = 0;
    mDownloadedSize = 0;
    mCopyObject = nullptr;
    setId(id);
    mThirdId = thirdId;
    mType = type;
    mSubType = subType;
    mDownloadUrl = download_url;
    mImageUrl = image_url;
    mTitle = title;
    mDesc = desc;
    setCreateTime();
    setPlayTime(false);
    mStatus = STATUS_READY_DOWNLOAD;
    mVersion = "1.0.1";
    mUpgradeFlag = 0;
    mBought = false;
    saveToFile();
}

ResourceItem::~ResourceItem(){
    emit released();
}

ResourceItem *ResourceItem::copyItem(ResourceItem *item){
    mJsonFile = item->mJsonFile;
    mId = item->mId;
    mThirdId = item->mThirdId;
    mType = item->mType;
    mSubType = item->mSubType;
    mStatus = item->mStatus;
    mDir = item->mDir;
    mFile = item->mFile;
    mDownloadUrl = item->mDownloadUrl;
    mTitle = item->mTitle;
    mDesc = item->mDesc;
    mImageUrl = item->mImageUrl;
    mCreateTime = item->mCreateTime;
    mPlayTime = item->mPlayTime;
    mTotalBytes = item->mTotalBytes;
    mReadBytes = item->mReadBytes;
    mSpeed = item->mSpeed;
    mDownloadedSize = item->mDownloadedSize;
    mVersion = item->mVersion;
    mUpgradeFlag = item->mUpgradeFlag;
    return this;
}

void ResourceItem::setId(QString id){
    mId = id;
    if(mJsonFile.isEmpty()){
        QString manifestDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).replace("/","\\")+"\\manifest\\";
        if(!QDir(manifestDir).exists()){
            QDir().mkdir(manifestDir);
        }
        if(!QDir(manifestDir+"PiTool").exists()){
            QDir().mkdir(manifestDir+"PiTool");
        }
        if(!QDir(manifestDir+"PiHome").exists()){
            QDir().mkdir(manifestDir+"PiHome");
        }
        mJsonFile = manifestDir + id+".json";
    }
}

void ResourceItem::setStatus(ResourceStatus status){
    if(mStatus!=status){
        mStatus=status;
        emit statusChanged(mStatus);
        saveToFile();
        if(mCopyObject){
            mCopyObject->setStatus(status);
        }
        ResourceModel::sendMessage_statusChanged(mId,status);
    }
}

QString ResourceItem::create_date(){
    QDateTime now;
    now.setSecsSinceEpoch(mCreateTime);
    return now.toString("yyyy-MM-dd");
}

qint64 ResourceItem::create_time(){
    return mCreateTime;
}


void ResourceItem::setCreateTime(){
    QDateTime now = QDateTime::currentDateTime();
    mCreateTime = now.toSecsSinceEpoch();
    emit createDateChanged(create_date());
}

void ResourceItem::setPlayTime(bool bSave){
    QDateTime now = QDateTime::currentDateTime();
    mPlayTime = now.toSecsSinceEpoch();
    emit playTimeChanged(mPlayTime);
    if(bSave){
        saveToFile();
    }
}

qint64 ResourceItem::play_time(){
    return mPlayTime;
}

QString ResourceItem::boughtDate(){
    QDateTime now;
    now.setSecsSinceEpoch(mBoughtTime);
    return now.toString("yyyy-MM-dd");
}

qint64 ResourceItem::totalBytes(){
    return mTotalBytes;
}

void ResourceItem::setTotalBytes(qint64 totalBytes){
    if(totalBytes==0){
        return;
    }
    if(mTotalBytes!=totalBytes+mDownloadedSize){
        mTotalBytes = totalBytes+mDownloadedSize;
        emit totalBytesChanged(mTotalBytes);
        saveToFile();
    }
    if(mCopyObject){
        mCopyObject->setTotalBytes(totalBytes);
    }
}

qint64 ResourceItem::readBytes(){
    return mReadBytes;
}

void ResourceItem::setReadBytes(qint64 readBytes){
    mReadBytes = readBytes+mDownloadedSize;
    if(mReadBytes>mTotalBytes){
        mTotalBytes = mReadBytes*100;
        emit totalBytesChanged(mTotalBytes);
    }
    emit readBytesChanged(mReadBytes);
    if(mCopyObject){
        mCopyObject->setReadBytes(readBytes);
    }
    ResourceModel::sendMessage_progressChanged(mId,mStatus,mTotalBytes,mReadBytes);
}

qint64 ResourceItem::speed(){
    return mSpeed;
}

void ResourceItem::setSpeed(qint64 speed){
    if(speed>0){
        mSpeed = speed;
        emit speedChanged(mSpeed);
    }
}

bool ResourceItem::loadFromFile(QString jsonFile){
    mJsonFile = jsonFile;
    QJsonObject json = JsonHandler::loadJsonObject(jsonFile);
    mId = JsonHandler::getJsonValue(json,"id","");
    if(mId.isEmpty()){
        qDebug()<<"loadFromFile error,mId empty"<<mId;
        return false;
    }
    mFile = JsonHandler::getJsonValue(json,"file","");
    mDownloadUrl = JsonHandler::getJsonValue(json,"download_url", "");
    if(mFile.isEmpty()&&mDownloadUrl.isEmpty()){
        qDebug()<<"loadFromFile error,mFile and mDownloadUrl empty"<<mFile<<mDownloadUrl;
        return false;
    }

    mStatus = (ResourceStatus)JsonHandler::getJsonValue(json,"status",0);
    if(mStatus<0||mStatus>=STATUS_COUNT){
        qDebug()<<"loadFromFile error,mStatus error"<<mStatus;
        return false;
    }
    if(mStatus==STATUS_DOWNLOADING||mStatus==STATUS_INSTALLING){
        mStatus=STATUS_READY_DOWNLOAD;
    }
    mType = JsonHandler::getJsonValue(json,"type", "");
    if(mType!="game"&&mType!="video"&&mType!="picture"){
        qDebug()<<"loadFromFile error,mType error"<<mType;
        return false;
    }
    mSubType = JsonHandler::getJsonValue(json,"sub_type", "");
    mThirdId = JsonHandler::getJsonValue(json,"third_id","");

    mTitle = JsonHandler::getJsonValue(json,"title", "");
    mDesc = JsonHandler::getJsonValue(json,"desc", "");
    mImageUrl = JsonHandler::getJsonValue(json,"image_url", "");
    QDateTime now = QDateTime::currentDateTime();
    mCreateTime = JsonHandler::getJsonValue(json,"create_time",(double)now.toSecsSinceEpoch());
    mPlayTime = JsonHandler::getJsonValue(json,"play_time",(double)mCreateTime);
    mVersion = JsonHandler::getJsonValue(json,"version","1.0.1");
    mUpgradeFlag = JsonHandler::getJsonValue(json,"upgrade_flag",0);

    mTotalBytes = JsonHandler::getJsonValue(json,"totalBytes",(double)100);
    QFile hFile(mFile);
    if(hFile.open(QIODevice::ReadOnly)){
        mReadBytes = hFile.size();
        hFile.close();
        if(mReadBytes>mTotalBytes){
            mReadBytes = 0;
        }
    }else{
        mReadBytes = 0;
    }

    return true;
}

void ResourceItem::saveToFile(){
    QJsonObject json;
    json.insert("id",mId);
    json.insert("third_id",mThirdId);
    json.insert("type", mType);
    json.insert("sub_type", mSubType);
    json.insert("status", mStatus);
    json.insert("file", mFile);
    json.insert("download_url", mDownloadUrl);
    json.insert("title", mTitle);
    json.insert("desc", mDesc);
    json.insert("image_url", mImageUrl);
    json.insert("create_time",mCreateTime);
    json.insert("play_time",mPlayTime);
    json.insert("totalBytes",mTotalBytes);
    json.insert("version",mVersion);
    json.insert("upgrade_flag",mUpgradeFlag);
    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QFile file(mJsonFile);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        qDebug()<<"save file error"<<mJsonFile;
        return;
    }
    file.write(byte_array);
    file.close();
    qInfo()<<"save file success"<<mJsonFile<<mStatus;
}

void ResourceItem::deleteItem(bool deleteFile){
    stopDownload();
    QFile(mJsonFile).remove();
    if(!bought()){
        QFile(mImageUrl.remove("file:///")).remove();
    }
    setStatus(STATUS_DELETED);
    if(deleteFile){
        if(mType=="game"&&mFile.indexOf(".tmp")==-1){
            deleteGame();
        }else{
            QFile(mFile).remove();
        }
    }
    ResourceModel::sendMessage_deleteResource(mId);
}

void ResourceItem::deleteGame(){
    QString dataPath = mFile.toLower().replace(".exe","_data");
    if(QDir(dataPath).exists()){
        qInfo()<<"deleteGame, remove path"<<dataPath;
        delDir(dataPath);
    }
    QFile(mFile).remove();
}

bool ResourceItem::delDir(const QString &path)
{
    if (path.isEmpty()){
        return false;
    }
    QDir dir(path);
    if(!dir.exists()){
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo file, fileList){
        if (file.isFile()){
            file.dir().remove(file.fileName());
        }else{
           delDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath());
}


bool ResourceItem::loadSteamVr(QJsonObject json){
    QString id = JsonHandler::getJsonValue(json,"app_key", "");
    if(id.isEmpty()){
        qDebug() << "loadSteamVr failed app_key is empty";
        return false;
    }
    setId(id);
    mThirdId = id;
    QJsonObject defValue;
    QJsonObject strings = JsonHandler::getJsonObjectValue(json,"strings",defValue);
    QJsonObject en_us = JsonHandler::getJsonObjectValue(strings,"en_us",defValue);
    mTitle = JsonHandler::getJsonValue(en_us,"name", "");
    if(mTitle.isEmpty()){
        qDebug() << "loadSteamVr failed mtitle is empty";
        return false;
    }
    mType = "game";
    mSubType = "steam";
    mImageUrl = JsonHandler::getJsonValue(json,"image_path", "");
    mDownloadUrl = JsonHandler::getJsonValue(json,"url", "");
    mStatus = STATUS_LOCAL;
    QDateTime now = QDateTime::currentDateTime();
    mCreateTime = JsonHandler::getJsonValue(json,"create_time",(double)now.toSecsSinceEpoch());
    mPlayTime = JsonHandler::getJsonValue(json,"create_time",(double)mCreateTime);
    return true;
}


bool ResourceItem::startDownload(){
    qCritical()<<"startDownload:"<<mImageUrl;
    if(mImageUrl.contains("http")){
        downloadImage();
    }
    if(mStatus!=STATUS_READY_DOWNLOAD
            &&mStatus!=STATUS_DOWNLOAD_FAILED){
        return false;
    }
    QString fileName = mFile;
    if(QFile(fileName.remove(".tmp")).exists()){
        mFile = fileName.remove(".tmp");
        qInfo()<<"startDownload, file is exist"<<mFile;
        emit downloadFinished(this);
        setStatus(STATUS_LOCAL);
        saveToFile();
        return true;
    }
    if (m_pNetworkManager == nullptr){
        m_pNetworkManager = new QNetworkAccessManager(this);
    }
    QNetworkRequest request;
    request.setUrl(mDownloadUrl);
    m_hFile = new QFile(file());
    if(!m_hFile->open(QIODevice::WriteOnly|QIODevice::Append)){
        qCritical()<<"file open error";
        delete m_hFile;
        m_hFile = nullptr;
        Setting::showAlertEx("download.failed.file.removed");
        return false;
    }
    if(mDownloadedSize>mTotalBytes&&mTotalBytes>100){
        qCritical()<<"httpFinished mDownloadedSize>mTotalBytes"<<mDownloadedSize<<mTotalBytes;
        httpFinished();
        return true;
    }
    mDownloadedSize = m_hFile->size();
    if(mCopyObject){
        mCopyObject->mDownloadedSize = mDownloadedSize;
    }
    m_hFile->seek(mDownloadedSize);
    request.setRawHeader("Range",QString("bytes=%1-").arg(mDownloadedSize).toUtf8());
    setStatus(STATUS_DOWNLOADING);
    mDlingTime = getNowTime();
    m_pReply = m_pNetworkManager->get(request);
    connect(m_pReply,SIGNAL(finished()),this,SLOT(httpFinished()));
    connect(m_pReply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
    connect(m_pReply,SIGNAL(downloadProgress(qint64,qint64)), this,SLOT(downloadProgress(qint64,qint64)));
    connect(m_pReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(httpError(QNetworkReply::NetworkError)));
    return true;
}

bool ResourceItem::stopDownload(){
    if(mStatus==STATUS_DOWNLOADING){
        qInfo()<<"stopDownload";
        if(m_pReply!=nullptr){
            m_pReply->abort();
            m_pReply->deleteLater();
        }
        if(m_pNetworkManager!=nullptr){
            m_pNetworkManager->clearAccessCache();
            m_pNetworkManager->deleteLater();
            delete m_pNetworkManager;
            m_pNetworkManager = nullptr;
        }

        if(m_hFile!=nullptr&&m_hFile->isOpen()){
            m_hFile->close();
            delete m_hFile;
            m_hFile=nullptr;
        }

        setStatus(STATUS_READY_DOWNLOAD);
        return true;
    }
    return false;
}

void ResourceItem::httpFinished(){    
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
        return;
    }
    if(mReadBytes>=mTotalBytes&&mTotalBytes>100){
       qInfo()<<"httpFinished,mReadBytes>=mTotalBytes"<<mReadBytes<<mTotalBytes;
        setStatus(STATUS_INSTALLING);
        qApp->processEvents();
        QString fileName = file().remove(".tmp");
        QFile(fileName+".tmp").rename(fileName);
        setFile(fileName);
        if(fileName.indexOf(".zip")>0){//解压
            QString path = Setting::getDownloadDir()+"\\games";
            QDir().mkdir(path);
            qInfo() << "downloadFinished extractDir "<<fileName<<path;
            QStringList fileList = JlCompress::extractDir(fileName,path);
            for(int i=0;i<fileList.size();i++){
                QString file = fileList.at(i);
                if(file.indexOf(".exe")>0){
                    setFile(file.replace("/","\\"));
                    QString dir = file.remove(".exe")+"_Data";
                    if(QDir(dir).exists()){
                        qInfo() << "downloadFinished setFile "<<dir<<file;
                        break;
                    }
                }
            }
            QFile(fileName).remove();
        }
        setStatus(STATUS_LOCAL);
        saveToFile();
        emit downloadFinished(this);
    }else{
        setStatus(STATUS_READY_DOWNLOAD);
    }
}

void ResourceItem::httpReadyRead(){
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

void ResourceItem::downloadProgress(qint64 readBytes, qint64 totalBytes){
    if(mStatus!=STATUS_DOWNLOADING){
        return;
    }
    qint64 time = getNowTime()-mDlingTime;
    int speed = readBytes*1000/time;
    setSpeed(speed);
    setTotalBytes(totalBytes);
    setReadBytes(readBytes);
    if(mSpeed>0){
        emit remainTimeChanged(getDLRemainTime());
    }
    emit percentChanged(getDLPercent());
}

void ResourceItem::httpError(QNetworkReply::NetworkError code){
    if(code==QNetworkReply::OperationCanceledError){
        return;
    }
    qCritical()<<"httpError"<<mDownloadUrl<<code;
    setStatus(STATUS_DOWNLOAD_FAILED);
    stopDownload();
    Setting::showMessage("\""+mTitle+"\""+tr("Download failed"));

}


qint64 ResourceItem::getNowTime(){
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

QString ResourceItem::getDLRemainTime(){
    if(mSpeed==0||mTotalBytes==100){
        return "";
    }
    int seconds = (mTotalBytes-mReadBytes)/mSpeed;
    if(seconds<60){
        return QString::number(seconds)+tr("sec");
    }else if(seconds>=3600){
        return QString::number(seconds/3600)+tr("hour")+QString::number((seconds%3600)/60)+tr("min");
    }else{
        return QString::number(seconds/60)+tr("min")+QString::number(seconds%60)+tr("sec");
    }
}

QString ResourceItem::getDLPercent(){
    double percent = (mReadBytes*100.0)/mTotalBytes;
    return QString::number(percent,'f',1)+"%";
}

void ResourceItem::downloadImage(){
    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).replace("/","\\");
    downloadPath = downloadPath.replace("PiTool","Pimax")+"\\PiTool"+"\\"+mType+"s";
    if(!QDir(downloadPath).exists()){
        QDir().mkdir(downloadPath);
    }
    QString imageFile = downloadPath+"\\"+mId+"."+getPosfix(mImageUrl);
    DownloadFile *pDownload = new DownloadFile();
    connect(pDownload,SIGNAL(downloadFinished(DownloadFile *,QString)),this,SLOT(downloadImageFinished(DownloadFile *,QString)));
    pDownload->download(mImageUrl,imageFile);//.replace("https","http")
}

void ResourceItem::downloadImageFinished(DownloadFile *pDownload,QString imageFile){
    qInfo()<<"downloadImageFinished:"<<imageFile;
    delete pDownload;
    if(imageFile.isEmpty()){
        return;
    }
    mImageUrl = "file:///"+imageFile;
    saveToFile();
    emit imageUrlChanged(mImageUrl);
}


QString ResourceItem::getPosfix(QString url){
    QString posfix = url.mid(url.lastIndexOf(".")+1);
    if(posfix.indexOf("?")>0){
        posfix = posfix.left(posfix.indexOf("?"));
    }
    if(posfix.isEmpty()){
        return "jpg";
    }
    return posfix;
}

bool ResourceItem::isSteamApp(){
    qInfo()<<"isSteamApp"<<mFile;
    QString appDir = mFile.left(mFile.toLower().lastIndexOf(".exe"))+"_Data";
    if(QDir(appDir).exists()&&QFile(appDir+"\\Plugins\\openvr_api.dll").exists()){
        return true;
    }
    return false;
}

ResourceItem* ResourceItem::getObject(){
    mCopyObject = new ResourceItem();
    connect(mCopyObject,SIGNAL(released()),this,SLOT(copyObjectRelease()));
    mCopyObject->copyItem(this);
    return mCopyObject;
}
void ResourceItem::deleteObject(){
    if(mCopyObject!=nullptr){
        delete mCopyObject;
        mCopyObject = nullptr;
    }
}

void ResourceItem::copyObjectRelease(){
    mCopyObject = nullptr;
}


