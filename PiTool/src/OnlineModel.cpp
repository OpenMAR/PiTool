#include "OnlineModel.h"
#include <QDir>
#include <QStandardPaths>
#include <QProcess>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QLibrary>

FilterItem *mGameFilters = nullptr;
FilterItem *mVideoFilters = nullptr;

#define CODE_SUCCESS 8888
#define PAGE_SIZE 200
OnlineModel::OnlineModel(QObject *parent) : QObject(parent)
{
    if(mGameFilters==nullptr){
        mGameFilters = new FilterItem();
    }
    if(mVideoFilters==nullptr){
        mVideoFilters = new FilterItem();
    }
    mHelpList = new HelpItem();
     qCritical()<<"OnlineModel init";
}

QString OnlineModel::getError(){
    return mError;
}

void OnlineModel::getFilters(){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),this,SLOT(parseFiltersData(OnlineRequest *,int,QString,QString,QJsonObject)));
    request->getHttpRequest("filter/all","");
}

void OnlineModel::parseFiltersData(OnlineRequest *request,int code,QString type,QString error,QJsonObject dataObject){
    if(request){
        delete request;
    }
    if(code!=CODE_SUCCESS){
        mError = error+QString::number(code);
        return;
    }

    QJsonArray defValue;
    QJsonArray jsonArray = JsonHandler::getJsonValue(dataObject,"gameFilters",defValue);
    mGameFilters->loadFromJsonArray(jsonArray);
    jsonArray = JsonHandler::getJsonValue(dataObject,"videoFilters",defValue);
    mVideoFilters->loadFromJsonArray(jsonArray);
}


QList<QObject*> OnlineModel::gameFilter(){
    return mGameFilters->listModel();
}

QList<QObject*> OnlineModel::videoFilter(){
    return mVideoFilters->listModel();
}

void OnlineModel::getGameHome(){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),this,SLOT(parseGameHomeData(OnlineRequest *,int,QString,QString,QJsonObject)));
    request->getHttpRequest("game/home","");
}

void OnlineModel::parseGameHomeData(OnlineRequest *request,int code,QString type,QString error,QJsonObject dataObject){
    delete request;
    if(code!=CODE_SUCCESS){
        mError = error+QString::number(code);
        qDebug()<<"parseGameHomeData code!=8888"<<mError;
        emit gameHomeList(false,nullptr,nullptr);
        return;
    }
    GameItem* banner = new GameItem();
    CatalogItem* catalogs = new CatalogItem();

    QJsonArray defValue;
    QJsonArray jsonArrayBanner = JsonHandler::getJsonValue(dataObject,"banners",defValue);
    for(int i=0;i<jsonArrayBanner.size();i++){
        GameItem *item = new GameItem();
        if(item->loadFromJson(jsonArrayBanner.at(i).toObject())){
            banner->append(item);
        }else{
            delete item;
        }
    }
    QJsonArray jsonArrayColumn = JsonHandler::getJsonValue(dataObject,"catalogs",defValue);
    for(int i=0;i<jsonArrayColumn.size();i++){
        CatalogItem *item = new CatalogItem();
        if(item->loadFromJson(jsonArrayColumn.at(i).toObject())){
            catalogs->append(item);
        }else{
            delete item;
        }
    }
    if(banner->count()==0||catalogs->count()==0){
        mError = tr("banner or catalogs empty");
        delete banner;
        delete catalogs;
        emit gameHomeList(false,nullptr,nullptr);
        return;
    }
    qDebug()<<"parseGameHomeData"<<banner->count()<<catalogs->count();
    emit gameHomeList(true,banner,catalogs);
}

void OnlineModel::getGameDetail(QString id){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseGameDetailData(OnlineRequest *,int,QString,QString,QJsonObject)));
    request->getHttpRequest("game/detail",QString("id=%1").arg(id));
}

void OnlineModel::parseGameDetailData(OnlineRequest *request,int code,QString type,QString error,QJsonObject dataObject){
    delete request;
    if(code!=CODE_SUCCESS){
        mError = error+QString::number(code);
        qDebug()<<"OnlineModel  parseGameDetailData failed"<<mError;
        emit gameDetail(false,nullptr,nullptr);
        return;
    }

    GameItem* detail = new GameItem();
    QJsonObject defValue;
    QJsonObject jsonObjectDetail = JsonHandler::getJsonObjectValue(dataObject,"detail",defValue);
    if(!detail->loadFromJson(jsonObjectDetail)){
        delete detail;
        mError = tr("get game detail error!");
        qDebug()<<"OnlineModel  parseGameDetailData failed"<<mError;
        emit gameDetail(false,nullptr,nullptr);
        return;
    }
    CatalogItem* recommend = new CatalogItem();
    QJsonObject jsonObjectRecommend = JsonHandler::getJsonObjectValue(dataObject,"recommend",defValue);
    if(!recommend->loadFromJson(jsonObjectRecommend)){
        delete detail;
        delete recommend;
        mError = tr("get game recommend list error!");
        qDebug()<<"OnlineModel  parseGameDetailData failed"<<mError;
        emit gameDetail(false,nullptr,nullptr);
        return;
    }
    qDebug()<<"OnlineModel  parseGameDetailData success"<<recommend->listModel().size();
    emit gameDetail(true,detail,recommend);
}

void OnlineModel::getAllGame(int page,QString orderField,bool desc){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseGameSearchData(OnlineRequest *,int,QString,QString,QJsonObject)));
    mRequestParam = orderField;
    QString param = QString("pageSize=%1&pageIndex=%2&orderField=%3&desc=%4")
            .arg(PAGE_SIZE).arg(page).arg(orderField).arg(desc?"true":"false");
    if(mGameFilters->count()==0){
        param=param+"&getFilters=true";
    }
    request->getHttpRequest("game/all",param);
}

void OnlineModel::parseGameSearchData(OnlineRequest *request,int code,QString type,QString error,QJsonObject dataObject){
    delete request;
    if(code!=CODE_SUCCESS){
        mError = error+QString::number(code);
        emit gameList(false);
        return;
    }
    GameItem *games= new GameItem();
    QJsonArray defValue;
    QJsonArray jsonArray = JsonHandler::getJsonValue(dataObject,"items",defValue);
    for(int i=0;i<jsonArray.size();i++){
        GameItem *item = new GameItem();
        if(item->loadFromJson(jsonArray.at(i).toObject())){
            games->append(item);
        }else{
            delete item;
        }
    }
    int itemSum = JsonHandler::getJsonValue(dataObject,"itemSum",0);
    int pageIndex = JsonHandler::getJsonValue(dataObject,"pageIndex",0);
    int pageSize = JsonHandler::getJsonValue(dataObject,"pageSize",0);

    CatalogItem* recommend = new CatalogItem();
    QJsonObject defObject;
    QJsonObject jsonObjectRecommend = JsonHandler::getJsonObjectValue(dataObject,"recommend",defObject);
    recommend->loadFromJson(jsonObjectRecommend);
    QString title = JsonHandler::getJsonValue(dataObject,"title","");
    if(mGameFilters->count()==0){
        parseFiltersData(nullptr,code,type,error,dataObject);
    }
    emit gameList(true,games,itemSum, pageSize,pageIndex,type,mRequestParam,title,recommend);
}

void OnlineModel::searchGame(QString keyword,int pageIndex, QString orderField,bool desc){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseGameSearchData(OnlineRequest *,int,QString,QString,QJsonObject)));
    mRequestParam = keyword;
    QString param = QString("pageSize=%1&pageIndex=%2&orderField=%3&desc=%4&keyword=%5")
            .arg(PAGE_SIZE).arg(pageIndex).arg(orderField).arg(desc?"true":"false").arg(keyword);
    if(mGameFilters->count()==0){
        param=param+"&getFilters=true";
    }
    request->getHttpRequest("game/search",param);
}

void OnlineModel::filterGame(QString filter,int pageIndex, QString orderField,bool desc){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseGameSearchData(OnlineRequest *,int,QString,QString,QJsonObject)));
    mRequestParam = filter;
    QString param = QString("pageSize=%1&pageIndex=%2&orderField=%3&desc=%4&filters=%5")
            .arg(PAGE_SIZE).arg(pageIndex).arg(orderField).arg(desc?"true":"false").arg(filter);
    if(mGameFilters->count()==0){
        param=param+"&getFilters=true";
    }
    request->getHttpRequest("game/filter",param);
}

void OnlineModel::getVideoHome(){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseVideoHomeData(OnlineRequest *,int,QString,QString,QJsonObject)));
    request->getHttpRequest("video/home","");
}
void OnlineModel::parseVideoHomeData(OnlineRequest *request,int code,QString type,QString error,QJsonObject dataObject){
    delete request;
    if(code!=CODE_SUCCESS){
        mError = error+QString::number(code);
        emit videoHomeList(false,nullptr,nullptr);
        return;
    }
    VideoItem* banner = new VideoItem();
    CatalogItem* catalogs = new CatalogItem();

    QJsonArray defValue;
    QJsonArray jsonArrayBanner = JsonHandler::getJsonValue(dataObject,"banners",defValue);
    for(int i=0;i<jsonArrayBanner.size();i++){
        VideoItem *item = new VideoItem();
        if(item->loadFromJson(jsonArrayBanner.at(i).toObject())){
            banner->append(item);
        }else{
            delete item;
        }
    }
    QJsonArray jsonArrayColumn = JsonHandler::getJsonValue(dataObject,"catalogs",defValue);
    for(int i=0;i<jsonArrayColumn.size();i++){
        CatalogItem *item = new CatalogItem();
        if(item->loadFromJson(jsonArrayColumn.at(i).toObject())){
            catalogs->append(item);
        }else{
            delete item;
        }
    }
    if(banner->count()==0||catalogs->count()==0){
        mError = tr("banner or catalogs empty");
        delete banner;
        delete catalogs;
        emit videoHomeList(false,nullptr,nullptr);
        return;
    }
    qDebug()<<"parseVideoHomeData,banner->count,catalogs->count"<<banner->count()<<catalogs->count();
    emit videoHomeList(true,banner,catalogs);
}

void OnlineModel::getVideoDetail(QString id){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseVideoDetailData(OnlineRequest *,int,QString,QString,QJsonObject)));
    request->getHttpRequest("video/detail",QString("id=%1").arg(id));
}

void OnlineModel::parseVideoDetailData(OnlineRequest *request,int code,QString type,QString error,QJsonObject dataObject){
    delete request;
    if(code!=CODE_SUCCESS){
        mError = error+QString::number(code);
        qDebug()<<"OnlineModel  parseVideoDetailData failed"<<mError;
        emit videoDetail(false,nullptr,nullptr);
        return;
    }

    VideoItem* detail = new VideoItem();
    QJsonObject defValue;
    QJsonObject jsonObjectDetail = JsonHandler::getJsonObjectValue(dataObject,"detail",defValue);
    qDebug()<<"parseGameDetailData"<<jsonObjectDetail;
    if(!detail->loadFromJson(jsonObjectDetail)){
        delete detail;
        mError = tr("获取视频详情出错!");
        qDebug()<<"OnlineModel  parseVideoDetailData failed"<<mError;
        emit videoDetail(false,nullptr,nullptr);
        return;
    }
    CatalogItem* recommend = new CatalogItem();
    QJsonObject jsonObjectRecommend = JsonHandler::getJsonObjectValue(dataObject,"recommend",defValue);
    if(!recommend->loadFromJson(jsonObjectRecommend)){
        delete detail;
        delete recommend;
        mError = tr("获取视频推荐列表出错!");
        qDebug()<<"OnlineModel  parseVideoDetailData failed"<<mError;
        emit videoDetail(false,nullptr,nullptr);
        return;
    }
    qDebug()<<"OnlineModel  parseVideoDetailData success"<<recommend->listModel().size();
    emit videoDetail(true,detail,recommend);
}

void OnlineModel::getAllVideo(int page,QString orderField,bool desc){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseVideoSearchData(OnlineRequest *,int,QString,QString,QJsonObject)));
    mRequestParam = orderField;
    QString param = QString("pageSize=%1&pageIndex=%2&orderField=%3&desc=%4")
            .arg(PAGE_SIZE).arg(page).arg(orderField).arg(desc?"true":"false");
    if(mGameFilters->count()==0){
        param=param+"&getFilters=true";
    }
    request->getHttpRequest("video/all",param);
}

void OnlineModel::parseVideoSearchData(OnlineRequest *request,int code,QString type,QString error,QJsonObject dataObject){
    delete request;
    if(code!=CODE_SUCCESS){
        mError = error+QString::number(code);
        emit videoList(false);
        return;
    }
    VideoItem *videos = new VideoItem();
    QJsonArray defValue;
    QJsonArray jsonArray = JsonHandler::getJsonValue(dataObject,"items",defValue);
    for(int i=0;i<jsonArray.size();i++){
        VideoItem *item = new VideoItem();
        if(item->loadFromJson(jsonArray.at(i).toObject())){
            videos->append(item);
        }else{
            delete item;
        }
    }
    int itemSum = JsonHandler::getJsonValue(dataObject,"itemSum",0);
    int pageIndex = JsonHandler::getJsonValue(dataObject,"pageIndex",0);
    int pageSize = JsonHandler::getJsonValue(dataObject,"pageSize",0);
    CatalogItem* recommend = new CatalogItem();
    QJsonObject defObject;
    QJsonObject jsonObjectRecommend = JsonHandler::getJsonObjectValue(dataObject,"recommend",defObject);
    recommend->loadFromJson(jsonObjectRecommend);
    if(mGameFilters->count()==0){
        parseFiltersData(nullptr,code,type,error,dataObject);
    }

    emit videoList(true,videos,itemSum, pageSize,pageIndex,type,mRequestParam,recommend);
}

void OnlineModel::searchVideo(QString keyword,int pageIndex, QString orderField,bool desc){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseVideoSearchData(OnlineRequest *,int,QString,QString,QJsonObject)));
    mRequestParam = keyword;
    QString param = QString("pageSize=%1&pageIndex=%2&orderField=%3&desc=%4&keyword=%5")
            .arg(PAGE_SIZE).arg(pageIndex).arg(orderField).arg(desc?"true":"false").arg(keyword);
    if(mGameFilters->count()==0){
        param=param+"&getFilters=true";
    }
    request->getHttpRequest("video/search",param);
}

void OnlineModel::filterVideo(QString filter,int pageIndex, QString orderField,bool desc){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseVideoSearchData(OnlineRequest *,int,QString,QString,QJsonObject)));
    mRequestParam = filter;
    QString param = QString("pageSize=%1&pageIndex=%2&orderField=%3&desc=%4&filters=%5")
            .arg(PAGE_SIZE).arg(pageIndex).arg(orderField).arg(desc?"true":"false").arg(filter);
    if(mGameFilters->count()==0){
        param=param+"&getFilters=true";
    }
    request->getHttpRequest("video/filter",param);
}

void OnlineModel::catalogVideo(int catalogId,int pageIndex){
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseVideoSearchData(OnlineRequest *,int,QString,QString,QJsonObject)));
    mRequestParam = catalogId;
    QString param = QString("pageSize=%1&pageIndex=%2").arg(PAGE_SIZE).arg(pageIndex);
    request->getHttpRequest("video/catalog",param);
}

void OnlineModel::getHelp(){
    if(mHelpList->count()>0){
        emit helpMoreChanged(mHelpMore);
        emit helpChanged(mHelpList);
        return;
    }
    qDebug()<<"getHelp()";
    OnlineRequest *request = new OnlineRequest();
    connect(request,SIGNAL(parseOnlineData(OnlineRequest *,int,QString,QString,QJsonObject)),
            this,SLOT(parseHelpData(OnlineRequest *,int,QString,QString,QJsonObject)));
    request->getHttpRequest("help/all");
}

void OnlineModel::parseHelpData(OnlineRequest *request,int code,QString type,QString error,QJsonObject dataObject){
    delete request;
    if(code!=CODE_SUCCESS){
        mError = error+QString::number(code);
        emit helpList(false);
        qDebug()<<"parseHelpData failed"<<mError;
        return;
    }
    QJsonArray defValue;
    QJsonArray jsonHelps = JsonHandler::getJsonValue(dataObject,"help",defValue);
    for(int i=0;i<jsonHelps.size();i++){
        HelpItem *helpGroup= new HelpItem();
        if(helpGroup->loadFromJson(jsonHelps.at(i).toObject())){
            mHelpList->append(helpGroup);
            if(helpGroup->group()){
                QJsonArray jsonItems = JsonHandler::getJsonValue(jsonHelps.at(i).toObject(),"items",defValue);
                for(int j=0;j<jsonItems.size();j++){
                    HelpItem *item = new HelpItem();
                    if(item->loadFromJson(jsonItems.at(j).toObject())){
                        mHelpList->append(item);
                    }else{
                        delete item;
                    }
                }
            }
        }else{
            delete helpGroup;
        }
    }
    mHelpMore = JsonHandler::getJsonValue(dataObject,"more","");
    emit helpMoreChanged(mHelpMore);
    emit helpChanged(mHelpList);
    qDebug()<<"parseHelpData success "<<mHelpList->count()<<mHelpMore;
}

HelpItem* OnlineModel::help(){
    return mHelpList;
}

QString OnlineModel::helpMore(){
    return mHelpMore;
}

bool OnlineModel::getInternetConnectState(){
    return true;
}



