#include "OnlineItem.h"
#include "ResourceModel.h"
#include <QDebug>

ListObject::ListObject(QObject *parent) : QObject(parent){

}
int ListObject::count(){
    return mList.size();
}

QList<QObject*> ListObject::listModel(){
    return mList;
}

void ListObject::append(QObject *object){
    mList.append(object);
    emit countChanged(mList.size());
    emit listModelChanged(mList);
}
void ListObject::clear(){
    for(int i=0;i<mList.size();i++){
        delete mList.at(i);
    }
    mList.clear();
    emit countChanged(mList.size());
    emit listModelChanged(mList);
}

QObject* ListObject::get(int index){
    return mList.at(index);
}


FilterItem::FilterItem(ListObject *parent): ListObject(parent){
    mGroup = false;
}


int FilterItem::id(){
    return mId;
}

QString FilterItem::title(){
    return mTitle;
}

bool FilterItem::group(){
    return mGroup;
}

void FilterItem::setGroup(bool group){
    mGroup = group;
    emit groupChanged(mGroup);
}

bool FilterItem::loadFromJson(QJsonObject json){
    mId = JsonHandler::getJsonValue(json,"id",0);
    if(mId==0){
        return false;
    }
    mTitle = JsonHandler::getJsonValue(json,"title", "");
    return true;
}

bool FilterItem::loadFromJsonArray(QJsonArray array){
    if(array.size()==0){
        return false;
    }
    for(int i=0;i<array.size();i++){
        QJsonObject json = array.at(i).toObject();
        FilterItem *item = new FilterItem();
        if(item->loadFromJson(json)){
            append(item);
        }else{
            delete item;
            continue;
        }
        QJsonArray defValue;
        QJsonArray items = JsonHandler::getJsonValue(json,"items",defValue);
        if(items.size()==0){
            continue;
        }
        item->setGroup(true);
        for(int i=0;i<items.size();i++){
            FilterItem *item = new FilterItem();
            if(item->loadFromJson(items.at(i).toObject())){
                append(item);
            }else{
                delete item;
            }
        }
    }
    return true;
}


OnlineItem::OnlineItem(ListObject *parent) : ListObject(parent)
{
}
void OnlineItem::copyItem(OnlineItem *item){
    mId = item->mId;
    mThirdId = item->mThirdId;
    mTitle = item->mTitle;
    mAuthor = item->mAuthor;
    mDesc = item->mDesc;
    mType = item->mType;

    mSubType = item->mSubType;
    mImageUrl = item->mImageUrl;
    mDownloadUrl = item->mDownloadUrl;
    mExtendUrl = item->mExtendUrl;
    mExtendUrlTitle = item->mExtendUrlTitle;
    mPlayCount = item->mPlayCount;

    mScore = item->mScore;
    mTotalBytes = item->mTotalBytes;
    mTags = item->mTags;
    mDownloadFlag = item->mDownloadFlag;
}


QString OnlineItem::id(){
    return mId;
}

QString OnlineItem::thirdId(){
    return mThirdId;
}

QString OnlineItem::title(){
    return mTitle;
}

QString OnlineItem::author(){
    return mAuthor;
}

QString OnlineItem::desc(){
    return mDesc;
}

QString OnlineItem::type(){
    return mType;
}
QString OnlineItem::subType(){
    return mSubType;
}
QString OnlineItem::downloadUrl(){
    return mDownloadUrl;
}

QString OnlineItem::imageUrl(){
    return mImageUrl;
}

QString OnlineItem::extendUrl(){
    return mExtendUrl;
}

QString OnlineItem::extendUrlTitle(){
    return mExtendUrlTitle;
}

qint64 OnlineItem::playCount(){
    return mPlayCount;
}

QString OnlineItem::scoreStr(){
    return QString::number(qreal(mScore)/10,'f',1);
}
int OnlineItem::score(){
    return mScore;
}

qint64 OnlineItem::totalBytes(){
    return mTotalBytes;
}

QString OnlineItem::totalBytesFmt(){
    ResourceModel resModel;
    return resModel.formatBytes(mTotalBytes);
}


QStringList OnlineItem::tags(){
    return mTags;
}

bool OnlineItem::canDownload(){
    return mDownloadFlag==1;
}


bool OnlineItem::loadFromJson(QJsonObject json){
    mId = JsonHandler::getJsonValue(json,"id","");
    if(mId==""){
        return false;
    }
    mThirdId = JsonHandler::getJsonValue(json,"third_id","");
    mTitle = JsonHandler::getJsonValue(json,"title", "");
    mAuthor = JsonHandler::getJsonValue(json,"author","");
    mType = JsonHandler::getJsonValue(json,"type", "");
    mSubType = JsonHandler::getJsonValue(json,"subtype", "");
    mDesc = JsonHandler::getJsonValue(json,"desc", "");
    mDownloadUrl = JsonHandler::getJsonValue(json,"url", "");
    mExtendUrl = JsonHandler::getJsonValue(json,"extend_url", "");
    mExtendUrlTitle = JsonHandler::getJsonValue(json,"extend_url_title", "");
    mImageUrl = JsonHandler::getJsonValue(json,"image_url", "");
    mPlayCount = JsonHandler::getJsonValue(json,"play", 0);
    mTotalBytes = JsonHandler::getJsonValue_long(json,"totalBytes",0);
    mScore = JsonHandler::getJsonValue(json,"score",70);
    mDownloadFlag = JsonHandler::getJsonValue(json,"download_flag",1);
    QString tags = JsonHandler::getJsonValue(json,"tags","");
    mTags = tags.split(",");
    return true;
}

GameItem::GameItem(OnlineItem *parent) : OnlineItem(parent){

}


void GameItem::appendItems(GameItem* items){
    for(int i=0;i<items->count();i++){
        mList.append(items->get(i));
    }
    emit listModelChanged(mList);
}



QString GameItem::os(){
    return mOS;
}

QString GameItem::cpu(){
    return mCPU;
}

QString GameItem::displayCard(){
    return mDisplayCard;
}

QString GameItem::memory(){
    return mMemory;
}

bool GameItem::controller(){
    return mbController;
}

bool GameItem::gamepad(){
    return mbGamepad;
}

bool GameItem::keyboard(){
    return mbKeyboard;
}

bool GameItem::mouse(){
    return mbMouse;
}

bool GameItem::sit(){
    return mbSit;
}

bool GameItem::stand(){
    return mbStand;
}

bool GameItem::space(){
    return mbSpace;
}
QString GameItem::operateGuide(){
    return mOperateGuide;
}

QString GameItem::guideBBS(){
    return mGuideBBS;
}

QString GameItem::guideUrl(){
    return mGuideUrl;
}
QStringList GameItem::imageUrlList(){
    return mImageUrlList;
}
QString GameItem::guideContent(){
    return mGuideContent;
}

bool GameItem::loadFromJson(QJsonObject json){
    if(!OnlineItem::loadFromJson(json)){
        return false;
    }
    QString imageUrlHead = JsonHandler::getJsonValue(json,"imageUrlHead","");
    QString imageUrls = JsonHandler::getJsonValue(json,"imageUrls","");
    QStringList imageUrlList = imageUrls.split(",");
    mImageUrlList.clear();
    for(int i=0;i<imageUrlList.size();i++){
        if(!imageUrlHead.isEmpty()&&!imageUrlList.at(i).isEmpty()){
            mImageUrlList.append(imageUrlHead+"/"+imageUrlList.at(i));
        }
    }
    if(mImageUrlList.size()==0){
        mImageUrlList.append(imageUrl());
    }
    QJsonObject defValue;
    QJsonObject value = JsonHandler::getJsonObjectValue(json,"require",defValue);
    mOS = JsonHandler::getJsonValue(value,"os","");
    mCPU = JsonHandler::getJsonValue(value,"cpu","");
    mDisplayCard = JsonHandler::getJsonValue(value,"gpu","");
    mMemory = JsonHandler::getJsonValue(value,"memory","");

    mbController = JsonHandler::getJsonValue(value,"controller",false);
    mbGamepad = JsonHandler::getJsonValue(value,"gamepad",false);
    mbKeyboard = JsonHandler::getJsonValue(value,"keyboard",false);
    mbMouse = JsonHandler::getJsonValue(value,"mouse",false);

    mbSit = JsonHandler::getJsonValue(value,"sit",false);
    mbStand = JsonHandler::getJsonValue(value,"stand",false);
    mbSpace = JsonHandler::getJsonValue(value,"space",false);

    value = JsonHandler::getJsonObjectValue(json,"guide",defValue);
    mOperateGuide = JsonHandler::getJsonValue(value,"operate","");
    mGuideBBS = JsonHandler::getJsonValue(value,"bbs","");
    mGuideUrl = JsonHandler::getJsonValue(value,"url","");
    mGuideContent = JsonHandler::getJsonValue(value,"content","");
    return true;
}

VideoItem::VideoItem(OnlineItem *parent) : OnlineItem(parent){

}

void VideoItem::appendItems(VideoItem* items){
    for(int i=0;i<items->count();i++){
        mList.append(items->get(i));
    }
    emit countChanged(count());
    emit listModelChanged(mList);
}

QString VideoItem::os(){
    return mOS;
}

QString VideoItem::cpu(){
    return mCPU;
}

QString VideoItem::displayCard(){
    return mDisplayCard;
}

QString VideoItem::memory(){
    return mMemory;
}

bool VideoItem::loadFromJson(QJsonObject json){
    if(!OnlineItem::loadFromJson(json)){
        return false;
    }
    QJsonObject defValue;
    QJsonObject value = JsonHandler::getJsonObjectValue(json,"require",defValue);
    mOS = JsonHandler::getJsonValue(value,"os","");
    mCPU = JsonHandler::getJsonValue(value,"cpu","");
    mDisplayCard = JsonHandler::getJsonValue(value,"gpu","");
    mMemory = JsonHandler::getJsonValue(value,"memory","");
    return true;
}


CatalogItem::CatalogItem(ListObject *parent) : ListObject(parent){
    mGameList = nullptr;
    mVideoList = nullptr;
}

CatalogItem::~CatalogItem(){
    if(mGameList!=nullptr){
        mGameList->clear();
        delete mGameList;
        mGameList = nullptr;
    }
    if(mVideoList!=nullptr){
        mVideoList->clear();
        delete mVideoList;
        mVideoList = nullptr;
    }
}


//CatalogItem* CatalogItem::get(int index){
//    return (CatalogItem*)ListObject::get(index);
//}

QString CatalogItem::title(){
    return mTitle;
}
QString CatalogItem::type(){
    return mType;
}
GameItem* CatalogItem::gameList(){
    return mGameList;
}
VideoItem* CatalogItem::videoList(){
    return mVideoList;
}

int CatalogItem::id(){
    return mId;
}

QString CatalogItem::linkTitle(){
    return mLinkTitle;
}

QString CatalogItem::linkType(){
    return mLinkType;
}

QString CatalogItem::linkParam(){
    return mLinkParam;
}

QString CatalogItem::imageUrl(){
    return mImageUrl;
}

bool CatalogItem::loadFromJson(QJsonObject json){
    mId = JsonHandler::getJsonValue(json,"id",0);
    mType = JsonHandler::getJsonValue(json,"type","");
    mTitle = JsonHandler::getJsonValue(json,"title","");
    mLinkTitle = JsonHandler::getJsonValue(json,"link_title","");
    mLinkType = JsonHandler::getJsonValue(json,"link_type","");
    mLinkParam = JsonHandler::getJsonValue(json,"link_param","");
    mImageUrl = JsonHandler::getJsonValue(json,"image_url","");
    if(mType.isEmpty()){
        mType = mLinkType;
    }

    QJsonArray defValue;
    QJsonArray items = JsonHandler::getJsonValue(json,"games",defValue);
    if(items.size()>0){
        mGameList = new GameItem();
        for(int i=0;i<items.size();i++){
                GameItem *item = new GameItem();
                if(item->loadFromJson(items.at(i).toObject())){
                    mGameList->append(item);
                }else{
                    delete item;
                }
        }
    }
    items = JsonHandler::getJsonValue(json,"videos",defValue);
    if(items.size()>0){
        mVideoList = new VideoItem();
        for(int i=0;i<items.size();i++){
            VideoItem *item = new VideoItem();
            if(item->loadFromJson(items.at(i).toObject())){
                mVideoList->append(item);
            }else{
                delete item;
            }
        }
    }
    return true;
}

HelpItem::HelpItem(ListObject *parent) : ListObject(parent){
    mGroup = false;
}
//HelpItem* HelpItem::get(int index){
//    return (HelpItem*)ListObject::get(index);
//}


QString HelpItem::title(){
    return mTitle;
}
QString HelpItem::url(){
    return mUrl;
}
bool HelpItem::group(){
    return mGroup;
}

bool HelpItem::loadFromJson(QJsonObject json){
    mTitle = JsonHandler::getJsonValue(json,"title","");
    mUrl = JsonHandler::getJsonValue(json,"url","");

    QJsonArray defValue;
    QJsonArray items = JsonHandler::getJsonValue(json,"items",defValue);
    mGroup = (items.size()>0);
    if(mTitle.isEmpty()||(mUrl.isEmpty()&&!mGroup)){
        return false;
    }
    qInfo()<<"HelpItem::loadFromJson "<<mTitle<<mUrl<<mGroup;
    return true;
}

