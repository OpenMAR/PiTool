#include "ResourceModel.h"
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
#include "diagnoseHandler.h"

#define RESOURCE_FILE "resource.json"
ResourceModel *resourceModel=nullptr;
#define RESOURCE_TYPE_GAME "game"
#define RESOURCE_TYPE_VIDEO "video"
#define RESOURCE_TYPE_PICTURE "picture"
QString gLaunchedApp="";

ResourceModel::ResourceModel(QObject *parent) : QObject(parent){
    qInfo()<<"ResourceModel";
    mRpcServer = nullptr;
    if(resourceModel){
        return;
    }
    initResource();
    mRpcServer = new RpcServer(parent);
    mRpcServer->setCallback(&ResourceModel::rpcServer_CallbackSt);
    resourceModel = this;
}
ResourceModel::~ResourceModel(){
    qInfo()<<"~ResourceModel";
    if(mRpcServer){
        delete mRpcServer;
        mRpcServer = nullptr;
    }
    mResourceList.clear();
    mSearchResultList.clear();
}

bool __stdcall ResourceModel::rpcServer_CallbackSt(QWebSocket *pSocket,QString process,QJsonObject param){
    if(resourceModel){
        return resourceModel->rpcServer_Callback(pSocket,process,param);
    }
    return false;
}

void ResourceModel::sendMessage_addResource(QString resId){
    qInfo()<<"sendMessage_addResource "<<resId;
    if(resourceModel){
        QString param = QString("{\"id\":\"%1\"}").arg(resId);
        resourceModel->mRpcServer->sendMessage("addResource",param);
    }else{
        qInfo()<<"sendMessage_addResource resourceModel is null";
    }
}

void ResourceModel::sendMessage_deleteResource(QString resId){
    qInfo()<<"sendMessage_deleteResource "<<resId;
    if(resourceModel){
        QString param = QString("{\"id\":\"%1\"}").arg(resId);
        resourceModel->mRpcServer->sendMessage("deleteResource",param);
    }else{
        qInfo()<<"sendMessage_deleteResource resourceModel is null";
    }
}

void ResourceModel::sendMessage_statusChanged(QString resId,int status){
    if(resourceModel){
        QString param = QString("{\"id\":\"%1\",\"status\":%2}").arg(resId).arg(status);
        resourceModel->mRpcServer->sendMessage("statusChanged",param);
    }else{
        qInfo()<<"sendMessage_statusChanged resourceModel is null";
    }
}

void ResourceModel::sendMessage_progressChanged(QString resId,int status,qint64 total_bytes,qint64 downloaded_bytes){
//    qInfo()<<"sendMessage_progressChanged "<<resId<<status;
    if(resourceModel){
        QString param = QString("{\"id\":\"%1\",\"status\":%2,\"total_bytes\":%3,\"downloaded\":%4}")
                .arg(resId).arg(status).arg(total_bytes).arg(downloaded_bytes);
        resourceModel->mRpcServer->sendMessage("progressChanged",param);
    }else{
        qInfo()<<"sendMessage_progressChanged resourceModel is null";
    }
}


bool ResourceModel::rpcServer_Callback(QWebSocket *pSocket,QString process,QJsonObject param){
    qDebug()<<"rpcServer_Callback"<<process<<param;
    if(process=="downloadResource"){
        QString resId = JsonHandler::getJsonValue(param,"id","");
        QString third_id = JsonHandler::getJsonValue(param,"third_id","");
        QString type = JsonHandler::getJsonValue(param,"type","");
        QString sub_type = JsonHandler::getJsonValue(param,"sub_type","");
        QString download_url = JsonHandler::getJsonValue(param,"download_url","");
        QString image_url = JsonHandler::getJsonValue(param,"image_url","");
        QString title = JsonHandler::getJsonValue(param,"title","");
        QString desc = JsonHandler::getJsonValue(param,"desc","");
        if(downloadResouce(resId,third_id,type,sub_type,download_url,image_url,title,desc)){
            mRpcServer->sendCallback(pSocket,process,true);
        }else{
            mRpcServer->sendCallback(pSocket,process,false,mError);
        }
        return true;
    }
    if(process=="deleteResource"){
        QString resId = JsonHandler::getJsonValue(param,"id","");
        bool deleteFile = JsonHandler::getJsonValue(param,"delete_file",false);
        if(deleteResource(resId,deleteFile)){
            mRpcServer->sendCallback(pSocket,process,true);
        }else{
            mRpcServer->sendCallback(pSocket,process,false,mError);
        }
        return true;
    }
    if(process=="startDownload"){
        QString resId = JsonHandler::getJsonValue(param,"id","");
        startDownload(resId);
        mRpcServer->sendCallback(pSocket,process,true);
        return true;
    }
    if(process=="stopDownload"){
        QString resId = JsonHandler::getJsonValue(param,"id","");
        stopDownload(resId);
        mRpcServer->sendCallback(pSocket,process,true);
        return true;
    }
    if(process=="playGame"){
        QString resId = JsonHandler::getJsonValue(param,"id","");
        if(playGame(resId)){
            mRpcServer->sendCallback(pSocket,process,true);
        }else{
            mRpcServer->sendCallback(pSocket,process,false,mError);
        }
        return true;
    }
    if(process=="playVideo"){
        QString resId = JsonHandler::getJsonValue(param,"id","");
        if(playVideoEx(resId)){
            mRpcServer->sendCallback(pSocket,process,true);
        }else{
            mRpcServer->sendCallback(pSocket,process,false,mError);
        }
        return true;
    }
    if(process=="getStatus"){
        QString resId = JsonHandler::getJsonValue(param,"id","");
        QObject *object = getItem(resId,"");
        QJsonObject json;
        if(object==nullptr){
            json.insert("status",-1);
        }else{
            ResourceItem *item = (ResourceItem*)object;
            json.insert("status",item->status());
            json.insert("total_bytes",item->totalBytes());
            json.insert("downloaded",item->readBytes());
        }
        QJsonDocument document;
        document.setObject(json);
        QByteArray byteArray = document.toJson(QJsonDocument::Compact);
        QString values = QString(byteArray);
        mRpcServer->sendCallback(pSocket,process,QString("true"),values);
        return true;
    }
    if(process=="openUrl"){
        QString url = JsonHandler::getJsonValue(param,"url","");
        if(url!=""){
            QDesktopServices::openUrl(QUrl(url));
            mRpcServer->sendCallback(pSocket,process,true);
        }
        return true;
    }
    if(process=="openSteam"){
        if(!steamVrIsInstalled()){
            Setting::callQmlFunc("showInstallSteamVr");
            return true;
        }
        launchSteamVr();
    }
    return false;
}

void ResourceModel::initResource(){
    QString roamingPiToolDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString manifestDir = roamingPiToolDir+"\\manifest";
    QDir dir(manifestDir);
    if(!dir.exists()){
        QDir().mkpath(manifestDir);
    }

    QStringList nameFilters;
    nameFilters.append("*.json");
    for(QString fileName:dir.entryList(nameFilters)){
        if(fileName.contains("steam.") || fileName.contains("ocs_")){
            continue;
        }
        ResourceItem *item = new ResourceItem();
        if(item->loadFromFile(manifestDir+"\\"+fileName)){
            mResourceList.append(item);
        }else{
            QFile(manifestDir+"\\"+fileName).remove();
            delete item;
        }
    }
    qInfo()<<"initResource tool size"<<mResourceList.size();

    QString piplayManifestDir = roamingPiToolDir.remove("PiTool") + "PiPlay\\manifest";

    QDir oldDir(piplayManifestDir);
    if(oldDir.exists()){
        QStringList nameFilters;
        nameFilters.append("*.json");
        for(QString fileName:oldDir.entryList(nameFilters)){
            if(fileName.contains("steam.") || fileName.contains("ocs_")){
                continue;
            }
            ResourceItem *item = new ResourceItem();
            if(item->loadFromFile(piplayManifestDir+"\\"+fileName)){
                mResourceList.append(item);
            }else{
                QFile(piplayManifestDir+"\\"+fileName).remove();
                delete item;
            }
        }
    }
    qInfo()<<"initResource piplay size:"<<mResourceList.size();

    checkSteamGame();
    checkOculusGame();
    loadPiplay1Data();
}

void ResourceModel::sortList(QList<QObject*> *list,int sortType){
    if(sortType==SORT_BY_DATE){
        qSort(list->begin(),list->end(),compareResourceItem);
    }else if(sortType==SORT_BY_NAME){
        qSort(list->begin(),list->end(),compareResourceItemByName);
    }else if(sortType==SORT_BY_PLAYTIME){
        qSort(list->begin(),list->end(),[](QObject *itemA,QObject *itemB){
            ResourceItem *item1 = (ResourceItem*)itemA;
            ResourceItem *item2 = (ResourceItem*)itemB;
            qInfo()<<"item1 paly time:"<<item1->play_time()<<" ;name:"<<item1->title();
            qInfo()<<"item2 paly time:"<<item2->play_time()<<" ;name:"<<item2->title();
            return (item1->play_time() - item2->play_time() >0);
        });
//        qSort(list->begin(),list->end(),compareResourceItemByPlayTime);
    }else if(sortType==SORT_BY_BOUGHT_TIME){
        qSort(list->begin(),list->end(),[](QObject *itemA,QObject *itemB){
            ResourceItem *item1 = (ResourceItem*)itemA;
            ResourceItem *item2 = (ResourceItem*)itemB;
            return (item1->bought_time()- item2->bought_time()>0);
        });
    }else if(sortType==SORT_BY_RELEASE_DATE){
        qSort(list->begin(),list->end(),[](QObject *itemA,QObject *itemB){
            ResourceItem *item1 = (ResourceItem*)itemA;
            ResourceItem *item2 = (ResourceItem*)itemB;
            QByteArray temp1 = item1->releaseDate().toLocal8Bit();
            const char *date1 = temp1.constData();
            QByteArray temp2 = item2->releaseDate().toLocal8Bit();
            const char *date2 = temp2.constData();
            return (strcmp(date1,date2)>0);
        });
    }
}

void ResourceModel::searchResource(QString keyword,int resType,int sortType,bool downloading,bool upgrading){
    mSearchResultList.clear();
    keyword = keyword.toLower();
    QString types[]={"game","video","picture"};
    for(int i=0;i<mResourceList.size();i++){
        ResourceItem *item = (ResourceItem*)mResourceList.at(i);
        if(item->status()==ResourceItem::STATUS_DELETED){
            continue;
        }
        if(downloading&&item->status()==0){
            continue;
        }
        if(!downloading&&item->status()>0){
            continue;
        }
        if(upgrading&&item->upgradeFlag()==0){
            continue;
        }
        if(resType>0&&resType<4&&types[resType-1]!=item->type()){
            continue;
        }
        if(!keyword.isEmpty()
                &&item->title().toLower().indexOf(keyword)==-1
                &&item->subType()!=keyword
                ){
            continue;
        }
        mSearchResultList.append(mResourceList.at(i));
    }
    sortList(&mSearchResultList,sortType);
    emit searchResultListChanged(mSearchResultList);
}

void ResourceModel::searchResource(QString keyword,int resType,int sortType,int searchType, int gameType){
    mSearchResultList.clear();
    keyword = keyword.toLower();
    QString types[]={"game","video","picture"};
    for(int i=0;i<mResourceList.size();i++){
        ResourceItem *item = (ResourceItem*)mResourceList.at(i);
        if(item->status()==ResourceItem::STATUS_DELETED){
            continue;
        }
        if(searchType==0&&item->status()>0){
            continue;
        }
        if(searchType==1&&(item->status()==ResourceItem::STATUS_LOCAL||item->status()==ResourceItem::STATUS_ONLINE)){
            continue;
        }
        if(searchType==2&&item->upgradeFlag()==0){
            continue;
        }
        if(searchType==3&&!item->bought()){
            continue;
        }
        if(resType>0&&resType<4&&types[resType-1]!=item->type()){
            continue;
        }
        if(!keyword.isEmpty()
                &&item->title().toLower().indexOf(keyword)==-1
                &&item->subType()!=keyword
                ){
            continue;
        }

        if(((SORT_BY_GAME_STEAMVR == gameType) && (item->subType() != "steam")) ||
                ((SORT_BY_GAME_OCULUS == gameType) && (item->subType() != "oculus")) ||
                ((SORT_BY_GAME_LOCAL == gameType) && (item->subType() != "pimax"))){
            continue;
        }
        mSearchResultList.append(mResourceList.at(i));
    }

    sortList(&mSearchResultList,sortType);
    emit searchResultListChanged(mSearchResultList);
}

bool ResourceModel::compareResourceItem(QObject *itemA,QObject *itemB){
    ResourceItem *item1 = (ResourceItem*)itemA;
    ResourceItem *item2 = (ResourceItem*)itemB;
    qint64 create_date1 = item1->create_time();
    qint64 create_date2 = item2->create_time();
    if(create_date1>create_date2){
        return true;
    }else if(create_date1<create_date2){
        return false;
    }else{
        return item1->id().compare(item2->id())>0;
    }
}

bool ResourceModel::compareResourceItemByName( QObject *itemA,QObject *itemB){
    ResourceItem *item1 = (ResourceItem*)itemA;
    ResourceItem *item2 = (ResourceItem*)itemB;
    QByteArray temp1 = item1->title().toLower().toLocal8Bit();
    const char *title1 = temp1.constData();
    QByteArray temp2 = item2->title().toLower().toLocal8Bit();
    const char *title2 = temp2.constData();

    qInfo()<<"item1 paly name:"<<title1;
    qInfo()<<"item2 paly name:"<<title2;
    if(strcmp(title1,title2)>0){
        return false;
    }else{
        return true;
    }
}
bool ResourceModel::compareResourceItemByPlayTime( QObject *itemA,QObject *itemB){
    ResourceItem *item1 = (ResourceItem*)itemA;
    ResourceItem *item2 = (ResourceItem*)itemB;
    qint64 time1 = item1->play_time();
    qint64 time2 = item2->play_time();
    if(time1>time2){
        return true;
    }else if(time1<time2){
        return false;
    }else{
        return item1->id().compare(item2->id())>0;
    }
}
QString supportFilePosfixs=",exe,mp4,mkv,rm,rmvb,wmv,avi,3gp,flv,mpeg,vod,mov,jpg,png,jpeg,bmp,svg,swf";
ResourceItem* ResourceModel::loadLocalResouce(QString resourceType, QString fileName,QString name,bool showError,QString resId){
    if(fileName.indexOf("file:///")==0){
        fileName = fileName.right(fileName.length()-QString("file:///").length());
    }
    QString filePosfix = fileName.mid(fileName.lastIndexOf(".")+1).toLower();
    qDebug()<<"loadLocalResouce "<<fileName<<filePosfix;
    if(supportFilePosfixs.indexOf(","+filePosfix+",")==-1){
        if(showError){
            Setting::showAlertEx("import.invalid");
        }
        return nullptr;
    }
    QList<QObject*> list = mResourceList;
    for(int i=0;i<list.length();i++){
        ResourceItem *item = (ResourceItem *)list.at(i);
        if(item->status()==ResourceItem::STATUS_DELETED){
            continue;
        }
        if(item->file()==fileName){
            qDebug()<<"loadLocalResouce exist: "<<i;
            if(showError){
                Setting::showAlertEx("import.exist");
            }
            return nullptr;
        }
    }
    QString title="Local";
    if(name!=""){
        title = name;
    }else{
        int pos1 = fileName.lastIndexOf('/');
        int pos2 = fileName.lastIndexOf('.');
        title = fileName.mid(pos1+1,pos2-pos1-1);
    }
    QDateTime now = QDateTime::currentDateTime();
    static int seq_id = 100;
    QString id = QString::number(now.toSecsSinceEpoch())+QString::number(seq_id++);
    if(!resId.isEmpty()){
        id = resId;
    }
    ResourceItem *resource = new ResourceItem();
    resource->setId(id);
    resource->setTitle(title);
    resource->setFile(fileName);
    resource->setType(resourceType);
    resource->setSubType(resourceType==RESOURCE_TYPE_GAME?"pimax":"2d");
    resource->setStatus(ResourceItem::STATUS_LOCAL);
    if(resourceType==RESOURCE_TYPE_PICTURE){
        QPixmap pixmap(fileName);
        pixmap = pixmap.scaled(200,135);
        QString imageFile = fileName.replace("."+filePosfix,"_thumbnail."+filePosfix);
        if(pixmap.save(imageFile)){
            resource->setImageUrl("file:///"+imageFile);
        }else{
            resource->setImageUrl("qrc:/resource/video_default.png");
        }

    }else{
        resource->setImageUrl(resourceType==RESOURCE_TYPE_GAME?"qrc:/resource/game_default.png":"qrc:/resource/video_default.png");
    }
    resource->setCreateTime();
    resource->saveToFile();

    mResourceList.insert(0,resource);
    emit resCountChanged(getResCount());
    sendMessage_addResource(id);
    return resource;
}

bool ResourceModel::loadLocalResouce(QString resourceType, QString fileName){
    if(loadLocalResouce(resourceType,fileName,"",false,"")){
        return true;
    }else{
        return false;
    }
}

int ResourceModel::getDlingCount(){
    int count = 0;
    for(int i=0;i<mResourceList.length();i++){
        ResourceItem *item = (ResourceItem*)mResourceList.at(i);
        if(item->status()>0
                &&item->status()!=ResourceItem::STATUS_DELETED
                &&item->status()!=ResourceItem::STATUS_ONLINE){
            count++;
        }
    }
    return count;
}

int ResourceModel::getResCount(){
    int count = 0;
    for(int i=0;i<mResourceList.length();i++){
        ResourceItem *item = (ResourceItem*)mResourceList.at(i);
        if(item->status()!=ResourceItem::STATUS_DELETED){
            count++;
        }
    }
    return count;
}

QList<QObject*> ResourceModel::searchResultList(){
    return mSearchResultList;
}

QObject* ResourceModel::getItem(QString id,QString third_id) const{
    for(int i=0;i<mResourceList.length();i++){
        ResourceItem *item = (ResourceItem*)mResourceList.at(i);
        if(item->id()==id ||
                (!third_id.isEmpty()&&item->thirdId()==third_id)){
            return (QObject*)item->getObject();
        }
    }
    return nullptr;
}

bool ResourceModel::isSteamApp(QString appFile){
    qInfo()<<"isSteamApp"<<appFile;
    QString appDir = appFile.left(appFile.toLower().lastIndexOf(".exe"))+"_Data";

    for(int index = 0; index < m_oculusAppPathList.size(); index++){
        m_oculusAppPath = m_oculusAppPathList.at(index);
        if(appDir.contains(m_oculusAppPath)){
            qInfo()<<"is oculus home app";
            return false;
        }
    }

    if(QDir(appDir).exists()&&QFile(appDir+"\\Plugins\\openvr_api.dll").exists()){
        return true;
    }
    return false;
}

bool ResourceModel::launchApp(QString id,QString app,QString param,bool bChecked){
    if(!PiService::getInstance()->connect()){
        Setting::callQmlFunc("showDisconnectMessage");
        return true;
    }

    if(bChecked&&!QFile(app).exists()){
        m_nError = ERROR_FILE_NOT_EXIST;
        return false;
    }
    app = app.replace("/","\\");
    QString appName = app.mid(app.lastIndexOf("\\")+1);

    QString playUrl = "";
    QString workingDirectory="";
    if(!isSteamApp(app)&&PiService::getInstance()->modeType()==0&&app.indexOf("UnityPVRPlayer")==-1){
        playUrl = Setting::getOvrLauncher()+" "+app.replace("/","\\")+"";
        if(!param.isEmpty()){
            playUrl = playUrl+" \""+param.replace("/","\\")+"\"";
        }
    }else{
        playUrl = "\""+app.replace("/","\\")+"\"";
        if(!param.isEmpty()){
            playUrl = playUrl+" \""+param.replace("/","\\")+"\"";
        }else{
            workingDirectory = app.left(app.lastIndexOf("\\"));
            qInfo()<<"workingDirectory "<<workingDirectory;
        }
    }
    killSteamVr();
    killAllProcess();
    gLaunchedApp = appName;
    QProcess *process = new QProcess();
    process->setWorkingDirectory(workingDirectory);
    process->start(playUrl,QStringList());
    qInfo()<<"launchApp"<<appName<<playUrl;
    addProcess(id,process);
    return true;
}

void ResourceModel::processFinished(int exitCode, QProcess::ExitStatus exitStatus){
    QProcess *process = qobject_cast<QProcess*>(sender());
    qInfo()<<"processFinished,exitCode="<<exitCode<<"exitStatus="<<exitStatus;
    qInfo()<<"program"<<process->program();
    for(int i=0;i<mProcessList.size();i++){
        MyProcess *myProcess = mProcessList.at(i);
        if(myProcess->process==process){
            mProcessList.removeAt(i);
            delete myProcess;
        }
    }
    delete process;
}

void ResourceModel::killSteamVr(){
    mSteamModel.killSteamVr();
}

bool ResourceModel::playGame(QString id){
    for(int i=0;i<mResourceList.length();i++){
        ResourceItem *item = (ResourceItem *)mResourceList.at(i);
        if(item->id()==id){
            if(item->subType()=="steam"&&!QFile(item->file()).exists()){
                qInfo()<<"openItem "<<item->download_url();
                killAllProcess();
                gLaunchedApp = "";
                item->setPlayTime();
                QDesktopServices::openUrl(QUrl(item->download_url()));
                return true;
            }
            if(item->subType()=="oculus"){
                qInfo()<<"oculus game"<<item->file();
                item->setPlayTime();
                return launchApp(id,item->file(),"",false);
            }
            if(!QFile(item->file()).exists()){
                m_nError = ERROR_FILE_NOT_EXIST;
                mError = tr("game file not exist!");
                return false;
            }
            item->setPlayTime();
            return launchApp(id,item->file(),"");
        }
    }
    return true;
}
bool ResourceModel::playVideoEx(QString resId){
    ResourceItem *foundItem = nullptr;
    for(int i=0;i<mResourceList.length();i++){
        ResourceItem *item = (ResourceItem *)mResourceList.at(i);
        if(item->id()==resId){
            foundItem = item;
            break;
        }
    }
    if(foundItem){
        if(foundItem->subType()=="2D"||foundItem->subType()=="3D"){
            return play2DVideo(resId);
        }else{
            return playVideo(resId);
        }
    }
    return true;
}

bool ResourceModel::playVideo(QString id){
    return true;
}

bool ResourceModel::play2DVideo(QString id){
    return true;
}

void ResourceModel::playOnlineVideo(QString url){
    qInfo()<<"playOnlineVideo"<<url;
    if(PiService::getInstance()->modeType()==1){
        //launchApp("",Setting::getVideoPlayer2D(),url);
        return;
    }
    QString videoPlayer = "";//Setting::getVideoPlayer();
    if(!videoPlayer.contains("UnityPVRPlayer",Qt::CaseInsensitive)){
        videoPlayer = "";//Setting::getInstallPath()+"\\UnityPVRPlayer\\UnityPVRPlayer.exe";
    }
    QString playUrl = "\""+videoPlayer+"\" "+url;
    qInfo()<<"playOnlineVideo"<<playUrl;
    QProcess().start(playUrl);
}

void ResourceModel::playResource(QString fileName){
    if(fileName.indexOf("file:///")==0){
        fileName = fileName.right(fileName.length()-QString("file:///").length());
    }
    QString filePosfix = fileName.mid(fileName.lastIndexOf(".")+1).toLower();
    qInfo()<<"playResource "<<fileName<<filePosfix;
    if(supportFilePosfixs.indexOf(","+filePosfix+",")==-1){
        Setting::showAlertEx("import.invalid");
        return;
    }
    if(filePosfix=="exe"){//game
        launchApp("",fileName,"");
    }else if(PiService::getInstance()->modeType()==1){
        //launchApp("",Setting::getVideoPlayer2D(),fileName);
    }else{
        //launchApp("",Setting::getVideoPlayer(),fileName);
    }
}

bool ResourceModel::deleteResource(QString resId,bool deleteFile){
    qInfo()<<"deleteResource"<<resId<<deleteFile<<mResourceList.length();
    ResourceItem *item = getResourceItem(resId);
    if(item!=nullptr){
        item->deleteItem(deleteFile);
        if(item->bought()){
            item->setStatus(ResourceItem::STATUS_ONLINE);
        }else{
            mResourceList.removeOne(item);
        }
        emit resCountChanged(getResCount());
        emit dlingCountChanged(getDlingCount());
        return true;
    }
    mError = tr("Resource is not exist");
    return false;
}

bool ResourceModel::downloadResouce(QString id, QString thirdId,QString type,QString subType,QString download_url,QString image_url,QString title,QString desc){
    if(!QDir(Setting::getDownloadDir()).exists()){
        Setting::showAlertEx("download.path.removed");
        mError = tr("download path removed");

        return false;
    }
    qInfo()<<"downloadResource"<<id<<thirdId<<type<<subType<<download_url<<image_url<<title<<desc;
    ResourceItem *resource = getResourceItem(id);
    if(resource!=nullptr){
        if(resource->status()!=ResourceItem::STATUS_ONLINE){
            return false;
        }
        resource->setId(id);
        resource->setDownloadUrl(download_url);
        resource->setFile(getFileName(id,download_url,type));
        resource->setCreateTime();
        resource->setStatus(ResourceItem::STATUS_READY_DOWNLOAD);
    }else{
        resource = new ResourceItem(id, thirdId, type, subType, download_url, image_url, title, desc);
        mResourceList.append(resource);
        resource->setFile(getFileName(id,download_url,type));
        resource->setStatus(ResourceItem::STATUS_READY_DOWNLOAD);
        sendMessage_addResource(id);
    }
    emit dlingCountChanged(getDlingCount());
    emit resCountChanged(getResCount());
    connect(resource,SIGNAL(downloadFinished(ResourceItem*)),this,SLOT(downloadFinished(ResourceItem*)));
    resource->startDownload();
    return true;
}

QString ResourceModel::getFileName(QString id,QString url,QString type){
    QFileInfo info(url);
    QString fileName(info.fileName());
    QString filePath = Setting::getDownloadDir();
    if(type=="game"){
        filePath+="\\games";
    }else if(type=="video"){
        filePath+="\\videos";
    }else{
        filePath+="\\apps";
    }
    QDir().mkdir(filePath);
    if(fileName.isEmpty()){
        if(type=="video"){
            fileName = id+".mp4.tmp";
        }else{
            fileName = id+".zip.tmp";
        }
    }else{
        fileName = fileName+".tmp";
    }
    return filePath + "\\" + fileName;
}

void ResourceModel::startDownload(QString id){
    qInfo()<<"startDownload "<<id;
    for(int i=0;i<mResourceList.length();i++){
        ResourceItem *item = (ResourceItem *)mResourceList.at(i);
        if(item->id().compare(id)==0){
            connect(item,SIGNAL(downloadFinished(ResourceItem*)),this,SLOT(downloadFinished(ResourceItem*)));
            item->startDownload();
        }
    }
}

void ResourceModel::stopDownload(QString id){
    qInfo()<<"stopDownload "<<id;
    for(int i=0;i<mResourceList.length();i++){
        ResourceItem *item = (ResourceItem *)mResourceList.at(i);
        if(item->id().compare(id)==0){
            item->stopDownload();
        }
    }
}

void ResourceModel::downloadFinished(ResourceItem *item){
    qInfo()<<"downloadFinished "<<item->id()<<item->status();
    Setting::showMessage("\""+item->title()+"\""+tr("Download Finished"));
    emit dlingCountChanged(getDlingCount());
    emit resCountChanged(getResCount());
}

QString ResourceModel::formatBytes(quint64 bytes){
    if(bytes<100*1024){
        return QString::number((double)bytes/1024,'f',2)+"KB";
    }else if(bytes<1024*1024*1024){
        return QString::number((double)bytes/1024/1024,'f',2)+"MB";
    }
    return QString::number((double)bytes/1024/1024/1024,'f',2)+"GB";
}

bool ResourceModel::steamVrIsInstalled(){
    return mSteamModel.steamVrIsInstalled();
}

void ResourceModel::loadSteamVrGame(){
    m_steamGames.clear();
    if(!steamVrIsInstalled()){
        qInfo()<<"steamvr is not installed";
        return;
    }
    int oldSize = mResourceList.size();
    QJsonObject obj = JsonHandler::loadJsonObject(mSteamModel.m_steamManifestPath);
    QJsonArray defaultArray;
    QJsonArray jsonApps =JsonHandler::getJsonValue(obj,"applications",defaultArray);
    qInfo()<<"steamvr app:"<<mSteamModel.m_steamManifestPath<<" size:"<<jsonApps.size();
    for(int i=0;i<jsonApps.size();i++){
        QJsonObject jsonObject = jsonApps.at(i).toObject();
        ResourceItem *resource = new ResourceItem();
        if(resource->loadSteamVr(jsonObject)){//&&!mSteamModel.steamGameIsDownloding(resource->id())){
            resource->setFile(mSteamModel.getExeFile(resource->id()));
            m_steamGames.append(resource->id());
            if(!appendLocalResource(resource)){
                 qInfo()<<"append local resource failed.";
                delete resource;
            }else{
                resource->downloadImage();
                emit resCountChanged(getResCount());
                sendMessage_addResource(resource->id());
            }
        }else{
            delete resource;
            qInfo()<<"delete steamvr resource";
        }
    }
    if(oldSize!=mResourceList.size()){
        emit resCountChanged(getResCount());
    }
}

void ResourceModel::launchSteamVr(){
    if(steamVrIsInstalled()){
        killSteamVr();
        QString exe = "\""+mSteamModel.m_steamVRFile+"\"";
        QProcess().startDetached(exe);
        qInfo()<<"launchSteamVr()"<<exe;
    }
//    QDesktopServices::openUrl(QUrl("steam://rungameid/250820"));
}

void ResourceModel::reService()
{
    DiagnoseHandler handle;
    handle.stopServiceByName("PiServiceLauncher");
    handle.startService();

    if (WinCommon::processAlived("vrserver.exe") ||
             WinCommon::processAlived("vrmonitor.exe") ||
             WinCommon::processAlived("vrcompositor.exe") ||
             WinCommon::processAlived("vrdashboard.exe"))
    {
        launchSteamVr();
    }
}

void ResourceModel::launchPihome(bool bStart){
    if(!pihomeIsExist()){
        return;
    }
    QString appName = "pihome.exe";
    QString exeFile = "";//"\""+Setting::getInstallPath()+"\\Pihome\\"+appName+"\"";
    if(bStart){
        if(WinCommon::processAlived(appName.toStdString())){
            WinCommon::killProcessByName(appName.toStdString());
        }
        qInfo()<<"launchPihome"<<exeFile;
        QProcess().startDetached(exeFile);
    }else{
        if(WinCommon::processAlived(appName.toStdString())){
            qInfo()<<"launchPihome"<<bStart;
            WinCommon::killProcessByName(appName.toStdString());
        }
    }
}

bool ResourceModel::pihomeIsExist(){
    //QString exeFile = Setting::getInstallPath()+"\\Pihome\\pihome.exe";
    //return QFile(exeFile).exists();
    return false;
}


bool ResourceModel::appendLocalResource(ResourceItem *resource){
    qInfo()<<"appendLocalResource "<<mResourceList.length();
    for(int i=0;i<mResourceList.length();i++){
        if(((ResourceItem*)mResourceList.at(i))->id()==resource->id()){
            //return false;
        }
    }
    qInfo()<<"append new Local resource "<<resource->id()<<mResourceList.length();
    mResourceList.append(resource);
    resource->saveToFile();
    return true;
}

void ResourceModel::checkSteamGameIsDeleted(){    
    int size = mResourceList.size();
    for(int i=size-1;i>=0;i--){
        ResourceItem *item = (ResourceItem *)mResourceList.at(i);
        if(item->subType()=="steam"&&!m_steamGames.contains(item->id())){
            qInfo() << "delete steam game "<<m_steamGames.size()<<" "<<item->id();
            item->deleteItem();
            mResourceList.removeOne(item);
            delete item;
        }
    }
    if(size!=mResourceList.size()){
        emit resCountChanged(getResCount());
    }
}

void ResourceModel::checkSteamGame(){
    loadSteamVrGame();
    checkSteamGameIsDeleted();
}
#define REG_PATH_OCULUS_HOME "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Oculus VR, LLC\\Oculus"
#define REG_PATH_OCULUS_APPS "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Oculus VR, LLC\\Oculus\\Config"
#define REG_ITEM_INSTALL_PATH "InstallPath"
bool ResourceModel::oculusHomeIsInstalled(){
    if(m_oculusPath!=""&&m_oculusAppPath!=""){
        if(QDir(m_oculusPath).exists()&&QDir(m_oculusAppPath).exists()
                &&QFile(m_oculusPath + "\\Support\\oculus-client\\OculusClient.exe").exists()){
            return true;
        }
    }
    m_oculusPath = "";
    m_oculusAppPath = "";

    QSettings reg(REG_PATH_OCULUS_HOME,QSettings::NativeFormat);
    QStringList keyList=reg.childKeys();
    foreach(QString key,keyList){
        if(key=="Base"){
            m_oculusPath = reg.value(key).toString().trimmed();
            qInfo() << "oculusHomeIsInstalled m_oculusPath " << m_oculusPath;
            break;
        }
    }
    if(m_oculusPath==""||!QDir(m_oculusPath).exists()){
//        qInfo() << "oculusHomeIsInstalled " << m_oculusPath << " is not exist";
        return false;

    }
    QString oculusClient = m_oculusPath + "\\Support\\oculus-client\\OculusClient.exe";
    if(!QFile(oculusClient).exists()){
//        qInfo() << "oculusHomeIsInstalled " << oculusClient << " is not exist";
        return false;
    }

    QSettings reg2(REG_PATH_OCULUS_APPS,QSettings::NativeFormat);
    QStringList keyList2=reg2.childKeys();
    foreach(QString key,keyList2){
        if(key=="InitialAppLibrary"){
            m_oculusAppPath = reg2.value(key).toString().trimmed()+"\\Software";
            break;
        }
    }
    if(m_oculusAppPath==""){
        m_oculusAppPath = m_oculusPath+"\\Software";
    }
    qInfo() << "oculusHome Is Installed " << m_oculusAppPath;
    return true;
}

std::wstring ResourceModel::getHLKMRegStringVal(std::wstring path, std::wstring key)
{
    HKEY hKEY;
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, path.c_str(), 0, KEY_READ, &hKEY))
    {
        TCHAR dwValue[MAX_PATH + 1] = { 0 };
        DWORD dwSzType = REG_SZ;
        DWORD dwSize = sizeof(dwValue);

        if (::RegQueryValueEx(hKEY, key.c_str(), 0, &dwSzType, (LPBYTE)&dwValue, &dwSize) == ERROR_SUCCESS)
        {
            return dwValue;
        }
        ::RegCloseKey(hKEY);
    }
    return{};
}

QStringList ResourceModel::getOCHomeDefaultLibraryPath()
{
    LONG error = ERROR_SUCCESS;
    WCHAR keyPath[MAX_PATH] = { L"Software\\Oculus VR, LLC\\Oculus\\Libraries\\" };
    HKEY oculusKey;
    error = RegOpenKeyExW(HKEY_CURRENT_USER, keyPath, 0, KEY_READ, &oculusKey);
    if (error != ERROR_SUCCESS)
    {
        qCritical() << "Unable to open Libraries key.";
        return{};
    }
    DWORD dwIndex = 0, NameSize, NameCnt, NameMaxLen, Type;
    DWORD KeySize, KeyCnt, KeyMaxLen, DateSize, MaxDateLen;

    if (RegQueryInfoKeyW(oculusKey, NULL, NULL, NULL, &KeyCnt, &KeyMaxLen, NULL, &NameCnt, &NameMaxLen, &MaxDateLen, NULL, NULL) != ERROR_SUCCESS)
    {
        qCritical() << "failed to get sub info.";
        return{};
    }

    std::vector<std::wstring> guids;
    for (dwIndex = 0; dwIndex < KeyCnt; dwIndex++)
    {
        KeySize = KeyMaxLen + 1;
        wchar_t* szKeyName = new wchar_t[KeySize];
        RegEnumKeyExW(oculusKey, dwIndex, szKeyName, &KeySize, NULL, NULL, NULL, NULL);
        guids.push_back(szKeyName);
        delete szKeyName;
    }

    WCHAR guid[40] = { L'\0' };
    DWORD guidSize = sizeof(guid);
    error = RegQueryValueExW(oculusKey, L"DefaultLibrary", NULL, NULL, (PBYTE)guid, &guidSize);
    RegCloseKey(oculusKey);
    if (error == ERROR_SUCCESS)
    {
        for (auto it = guids.begin(); it != guids.end(); it++)
        {
            if (*it == guid) {
                guids.erase(it);
                break;
            }
        }
        guids.insert(guids.begin(), guid);
    }
    QStringList paths;
    for (auto guid:guids)
    {
        std::wstring subKeyPath = keyPath + guid;
        error = RegOpenKeyExW(HKEY_CURRENT_USER, subKeyPath.c_str(), 0, KEY_READ, &oculusKey);
        if (error != ERROR_SUCCESS)
        {
            qCritical() << "Unable to open Library path key.";
            return{};
        }
        DWORD pathSize;
        error = RegQueryValueExW(oculusKey, L"Path", NULL, NULL, NULL, &pathSize);
        PWCHAR volumePath = (PWCHAR)malloc(pathSize);
        error = RegQueryValueExW(oculusKey, L"Path", NULL, NULL, (PBYTE)volumePath, &pathSize);
        RegCloseKey(oculusKey);
        if (error != ERROR_SUCCESS)
        {
            free(volumePath);
            qCritical() << "Unable to read Library path.";
            continue;
        }
        DWORD total;
        WCHAR volume[50] = { L'\0' };
        wcsncpy_s(volume, volumePath, 49);
        WCHAR path[MAX_PATH + 1] = { L'\0' };
        GetVolumePathNamesForVolumeNameW(volume, path, MAX_PATH, &total);
        wcsncat_s(path, MAX_PATH, volumePath + 49, MAX_PATH);
        free(volumePath);
        paths.append(QString::fromStdWString(path));
    }

    return paths;
}

bool ResourceModel::getOculusGamePathList()
{
    m_oculusAppPathList = getOCHomeDefaultLibraryPath();
    if (!m_oculusAppPathList.isEmpty()) {
        return true;
    }

    std::wstring appPath;
    appPath = getHLKMRegStringVal(TEXT("SOFTWARE\\Wow6432Node\\Oculus VR, LLC\\Oculus\\Config"), TEXT("InitialAppLibrary"));
    if (!appPath.empty()) {
        m_oculusAppPathList.append(QString::fromStdWString(appPath));
        return true;
    }
    appPath = getHLKMRegStringVal(TEXT("SOFTWARE\\Oculus VR, LLC\\Oculus\\Config"), TEXT("InitialAppLibrary"));
    if (!appPath.empty()) {
        m_oculusAppPathList.append(QString::fromStdWString(appPath));
        return true;
    }
    appPath = getHLKMRegStringVal(TEXT("SOFTWARE\\Wow6432Node\\Oculus VR, LLC\\Oculus"), TEXT("Base"));
    if (!appPath.empty()) {
        appPath += L"\\Software";
        m_oculusAppPathList.append(QString::fromStdWString(appPath));
        return true;
    }
    appPath = getHLKMRegStringVal(TEXT("SOFTWARE\\Oculus VR, LLC\\Oculus"), TEXT("Base"));
    if (!appPath.empty()) {
        appPath += L"\\Software";
        m_oculusAppPathList.append(QString::fromStdWString(appPath));
        return true;
    }

    return m_oculusAppPathList.isEmpty();
}

void ResourceModel::loadOculusGame(){
    m_oculusGames.clear();

    QSettings reg(REG_PATH_OCULUS_HOME,QSettings::NativeFormat);
    QStringList keyList=reg.childKeys();
    foreach(QString key,keyList){
        if(key=="Base"){
            m_oculusPath = reg.value(key).toString().trimmed();
            qInfo() << "oculusHomeIsInstalled m_oculusPath " << m_oculusPath;
            break;
        }
    }

    if(!getOculusGamePathList()){//(!oculusHomeIsInstalled()){
        return;
    }

    int oldSize = mResourceList.size();
    for(int index = 0; index < m_oculusAppPathList.size(); index++){
    m_oculusAppPath = m_oculusAppPathList.at(index);
    qInfo()<<"m_oculusAppPath "<<m_oculusAppPath;
    QDir dir(m_oculusAppPath+"\\Manifests");
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString fileName = fileInfo.fileName();
        QString posfix = fileName.right(fileName.length()-fileName.lastIndexOf(".")-1);
        if(posfix=="json"&&fileName.indexOf("_assets")==-1){
            QJsonObject root = JsonHandler::loadJsonObject(m_oculusAppPath+"\\Manifests\\"+fileName);
            QString appId = JsonHandler::getJsonValue(root,"appId","");
            QString path = JsonHandler::getJsonValue(root,"canonicalName","");
            QString launchFile = JsonHandler::getJsonValue(root,"launchFile","");
            QString launchParameters = JsonHandler::getJsonValue(root,"launchParameters","");
            QString launchPathFile = m_oculusAppPath+"\\Software\\"+path+"\\"+launchFile;
            if(appId==""||path==""||launchFile==""||!QFile(launchPathFile).exists()){
                qInfo()<<"loadOculusGame "<<appId<<path<<launchFile<<launchPathFile;
                continue;
            }
            if(launchParameters!=""&&launchParameters!="null"){
                launchPathFile = launchPathFile+" "+launchParameters;
            }

            ResourceItem *resource = new ResourceItem();
            resource->setId("ocs_"+appId);
            resource->setThirdId(appId);
            resource->setTitle(path.replace("-"," "));
            resource->setType("game");
            resource->setSubType("oculus");
            resource->setFile(launchPathFile);
            resource->setStatus(ResourceItem::STATUS_LOCAL);
            resource->setCreateTime();
            QString pathForImage = path.replace(" ","-");
            QString imageUrl =m_oculusAppPath+"\\Software\\StoreAssets\\"+pathForImage+"_assets\\cover_landscape_image.jpg";
            if(QFile(imageUrl).exists()){
                resource->setImageUrl("file:///"+imageUrl.replace("\\","/"));//.replace(" ","%20"));
            }else{
                imageUrl = imageUrl.replace("SoftWare","Software\\Software");
                if(QFile(imageUrl).exists()){
                    resource->setImageUrl("file:///"+imageUrl.replace("\\","/"));//.replace(" ","%20"));
                }else if(m_oculusPath != "" && QDir(m_oculusPath).exists()){
                    imageUrl = m_oculusPath + "\\CoreData\\Software\\StoreAssets\\" + pathForImage+"_assets\\cover_landscape_image.jpg";
                    if(QFile(imageUrl).exists()){
                        resource->setImageUrl("file:///"+imageUrl.replace("\\","/"));
                    }else{
                        resource->setImageUrl("qrc:/resource/game_default.png");
                    }
                }else{
                    resource->setImageUrl("qrc:/resource/game_default.png");
                }
                qInfo()<<"loadOculusGame "<<imageUrl;
            }
            m_oculusGames.append(resource->id());
            if(!appendLocalResource(resource)){
                delete resource;
            }else{
                sendMessage_addResource(resource->id());
            }
        }
    }
    }
    if(oldSize!=mResourceList.size()){
        emit resCountChanged(getResCount());
    }
}

void ResourceModel::checkOculusGameIsDeleted(){
    int size = mResourceList.size();
    for(int i=size-1;i>=0;i--){
        ResourceItem *item = (ResourceItem *)mResourceList.at(i);
        if(item->subType()=="oculus"&&!m_oculusGames.contains(item->id())){
            qInfo() << "delete oculus game "<<mResourceList.size()<<" "<<item->id();
            item->deleteItem();
            mResourceList.removeOne(item);
            delete item;
        }
    }
    if(size!=mResourceList.size()){
        emit resCountChanged(getResCount());
    }
}

void ResourceModel::checkOculusGame(){
    loadOculusGame();
    checkOculusGameIsDeleted();
}


void ResourceModel::loadPiplay1Data(){
    QStringList dirList = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    QString programDataDir;
    for(QString dir:dirList){
        programDataDir = dir.remove("PiTool");
        if(programDataDir.indexOf("ProgramData",Qt::CaseInsensitive)>0){
            break;
        }
    }
    QString dbDir = programDataDir+"pimax\\pidb";
    dbDir.replace("/","\\");
    qInfo()<<"loadPiplay1Data"<<programDataDir<<dbDir;
    QString dbFile = dbDir+"\\pi.db";
    if(!QDir(dbDir).exists()){
        qInfo()<<dbDir<<"is not exist";
        return;
    }
    if(!QFile(dbFile).exists()){
        qInfo()<<dbFile<<"is not exist";
        return;
    }
    if(QFile(dbDir+"\\imported.txt").exists()){
        qInfo()<<"Piplay1 data has been imported before";
        return;
    }
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbFile);

    if(!database.open()){
        qInfo()<<"loadPiplay1Data"<<database.lastError();
        qInfo()<<"loadPiplay1Data open database failed";
        return;
    }
    qInfo()<<"loadPiplay1Data open database success";
    //load local database
    QString sqlString = "select id,type,name,localpath,image,info,info_us from pilocal where state=1";
    QSqlQuery sql_query;
    sql_query.prepare(sqlString);
    if(!sql_query.exec()){
        qInfo()<<"loadPiplay1Data query sql error"<<sqlString<<sql_query.lastError();
        database.close();
        return;
    }
    while(sql_query.next()){
        QString id = sql_query.value("id").toString();
        QString type = sql_query.value("type").toString();
        QString name = sql_query.value("name").toString();
        QString localPath = sql_query.value("localpath").toString();
        QString info = sql_query.value("info").toString();
        QStringList infoList = info.split("\r\n");
        if(infoList.size()>=3){
            name = infoList.at(0);
            name = name.mid(4,name.length()-5);
            info = infoList.at(2);
            info = info.mid(3);
        }
        qInfo()<<"piplay1 data record:"<<type<<name<<info<<localPath<<infoList;
        if(!QFile(localPath).exists()){
            continue;
        }
        QString resourceType=RESOURCE_TYPE_GAME;
        if(type=="vrVideo"){
            resourceType = RESOURCE_TYPE_VIDEO;
        }
        ResourceItem* resource = loadLocalResouce(resourceType,localPath,name,false,"piplay1."+id);
        if(resource){
            QByteArray imageData = sql_query.value("image").toByteArray();
            QString posfix = ".jpg";
            if(imageData.at(0)=='B'&&imageData.at(1)=='M'){
                posfix = ".bmp";
            }
            QString imageFile = dbDir+"\\"+resource->id()+posfix;
            QFile file(imageFile);
            if(imageData.length()>0&&file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
                file.write(imageData);
                file.close();
                resource->setImageUrl("file:///"+imageFile.replace("\\\\","\\"));
                resource->saveToFile();
            }
        }
    }
    //load import database
    sqlString = "select id,type,name,localpath from piImport";
    sql_query;
    sql_query.prepare(sqlString);
    if(!sql_query.exec()){
        qInfo()<<"loadPiplay1Data query sql error"<<sqlString<<sql_query.lastError();
        database.close();
        return;
    }
    while(sql_query.next()){
        QString id = sql_query.value("id").toString();
        QString type = sql_query.value("type").toString();
        QString name = sql_query.value("name").toString();
        QString localPath = sql_query.value("localpath").toString();
        qInfo()<<"piplay1 data record:"<<type<<name<<localPath;
        if(!QFile(localPath).exists()){
            continue;
        }
        QString resourceType=RESOURCE_TYPE_GAME;
        if(type=="vrVideo"){
            resourceType = RESOURCE_TYPE_VIDEO;
        }
        loadLocalResouce(resourceType,localPath,name,false,"piplay1.import."+id);
    }

    database.close();
    QFile(dbDir+"\\imported.txt").open(QIODevice::ReadWrite);
}

QString ResourceModel::getError(){
    switch (m_nError) {
    case ERROR_FILE_NOT_EXIST:
        return tr("file not exists");
        break;
    default:
        break;
    }
    if(m_nError){

    }
    return mError;
}
int ResourceModel::getErrorCode(){
    return m_nError;
}

ResourceItem *ResourceModel::getResourceItem(QString id){
    for(int i=0;i<mResourceList.size();i++){
        ResourceItem *item = (ResourceItem*)mResourceList.at(i);
        if(item->id()==id){
            return item;
        }
    }
    return nullptr;
}

void ResourceModel::parseOnlineBoughtData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject){
    delete request;
    if(code!=8888){
        return;
    }
    QJsonArray defValue;
    QJsonArray jsonArray = JsonHandler::getJsonValue(dataObject,"items",defValue);
    for(int i=0;i<jsonArray.size();i++){
        QJsonObject json = jsonArray.at(i).toObject();
        QString id = JsonHandler::getJsonValue(json,"id","");
        if(id == ""){
            continue;
        }
        ResourceItem *item = getResourceItem(id);
        if(item==nullptr){
            item = new ResourceItem();
            item->setId(id);
            item->setStatus(ResourceItem::STATUS_ONLINE);
            item->setTitle(JsonHandler::getJsonValue(json,"title", ""));
            item->setThirdId(JsonHandler::getJsonValue(json,"third_id",""));
            item->setDownloadUrl(JsonHandler::getJsonValue(json,"url", ""));
            item->setType(JsonHandler::getJsonValue(json,"type", ""));
            item->setSubType(JsonHandler::getJsonValue(json,"subtype", ""));
            item->setImageUrl(JsonHandler::getJsonValue(json,"image_url", ""));
            mResourceList.append(item);
        }
        item->setBought(true);
        item->setBoughtTime(JsonHandler::getJsonValue(json,"bought_time", double(0)));
        item->setReleaseDate(JsonHandler::getJsonValue(json,"release", ""));
    }
}

void ResourceModel::searchBoughtResource(QString access_token){
    clearBoughtResource();
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseOnlineBoughtData(OnlineRequest *,int,QString,QString,QJsonObject)));
    request->getHttpRequest("/userProduct/search",QString("access_token=%1&pageSize=200&pageIndex=1").arg(access_token));
}

void ResourceModel::clearBoughtResource(){
    for(int i=0;i<mResourceList.size();i++){
        ResourceItem *item = (ResourceItem*)mResourceList.at(i);
        if(item->status()==ResourceItem::STATUS_ONLINE){
            item->setStatus(ResourceItem::STATUS_DELETED);
            mResourceList.removeOne(item);
        }else if(item->bought()){
            item->setBought(false);
        }
    }
}

void ResourceModel::killAllProcess(){
    if(!gLaunchedApp.isEmpty()){
        WinCommon::killProcessByName(gLaunchedApp.toStdString());
    }
    for(int i=0;i<mProcessList.size();i++){
        MyProcess *myProcess = mProcessList.at(i);
        myProcess->process->kill();
        qInfo()<<"kill process"<<myProcess->process->program();
    }
}

void ResourceModel::addProcess(QString id,QProcess *process){
    connect(process,SIGNAL(finished(int, QProcess::ExitStatus)),this,SLOT(processFinished(int, QProcess::ExitStatus)));
    MyProcess *myProcess = new MyProcess(id,process);
    mProcessList.append(myProcess);
}
