#include "StatisticsBase.h"
#include "Windows.h"
#include "JsonHandler.h"
#include "piservice.h"
#include "OnlineRequest.h"
#include "RegisterHandler.h"
#include <QDebug>
#include <QCoreApplication>
#include <QApplication>
#include <QNetworkInterface>
#include <QUuid>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDir>

//#define PIPLAY_HOST "118.89.106.249:8080"
#define PIPLAY_HOST "piserver.pimaxvr.com:17731"

StatisticsBase::StatisticsBase(QObject *parent) : QObject(parent){\
    mCacheDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"\\cache";
    QDir dir(mCacheDir);
    if(!dir.exists()){
        QDir().mkpath(mCacheDir);
    }
}
QString StatisticsBase::getGUID(){
    QUuid id = QUuid::createUuid();
    QString guid = id.toString().replace("-","").replace("{","").replace("}","");
    return guid;
}
#define REG_PIPLAY_PATH "HKEY_CURRENT_USER\\SOFTWARE\\PiPlay2"

QString StatisticsBase::getDeviceId(){
    if(mDeviceId.isEmpty()){
        mDeviceId = RegisterHandler::getInstance().getRegisterValue(REG_PIPLAY_PATH,"device_id","");
        if(!mDeviceId.isEmpty()){
            return mDeviceId;
        }

        QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
        int nCnt = nets.count();
        QString strMacAddr = "";
        for(int i = 0; i < nCnt; i ++){
            //如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
            if(nets[i].flags().testFlag(QNetworkInterface::IsUp)
                    && nets[i].flags().testFlag(QNetworkInterface::IsRunning)
                    && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack)){
                strMacAddr = nets[i].hardwareAddress();
                break;
            }
        }
        strMacAddr = strMacAddr.replace(":","-");
        QStringList splits = strMacAddr.split("-");
        if(splits.length()<6){
            return "";
        }
        qInfo()<<"getUIID 3"<<splits.length();
        QString strUUID = splits.at(3)+splits.at(1)+splits.at(2)+splits.at(0)+"-"+splits.at(5)+splits.at(4)
                +"-"+splits.at(1)+splits.at(0)+"-"+splits.at(3)+splits.at(1)+"-"+splits.at(2)+splits.at(0)+splits.at(5)+splits.at(4)+splits.at(1)+splits.at(0);
        mDeviceId = strUUID.toLower();
        RegisterHandler::getInstance().setRegisterValue(REG_PIPLAY_PATH,"device_id",mDeviceId);
    }
    return mDeviceId;
}

void StatisticsBase::saveEventData(QString event,QString param){
    QString id = getGUID();
    QJsonObject json;
    json.insert("id",id);
    json.insert("name",event);
    json.insert("param",param);
    QDateTime now = QDateTime::currentDateTime();
    json.insert("time",now.toSecsSinceEpoch());

    //保存文件
    QString jsonFile = mCacheDir + "\\"+id+".dat";
    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QFile file(jsonFile);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        return;
    }
    file.write(byte_array);
    file.close();
}

void StatisticsBase::parseResponseData(OnlineRequest* /*request*/,QByteArray data){
    qInfo()<<"parseResponseData "<<QString(data);
//    delete request;
    for(int i=0;i<mSendingEvent.size();i++){
        QFile(mSendingEvent.at(i)).remove();
    }
    mSendingEvent.clear();
}

void StatisticsBase::postEventData(QString data){
    QString device_id = getDeviceId();
    if(device_id.isEmpty()){
        mSendingEvent.clear();
        return;
    }
    QDateTime now = QDateTime::currentDateTime();
    QString command = QString("/piplay_v3/stat/event?device_id=%1&time=%2").arg(device_id).arg(now.toSecsSinceEpoch());
    qInfo()<<"postEventData command="<<command;
    OnlineRequest *request =  new OnlineRequest();
    connect(request,SIGNAL(postOnlineData(OnlineRequest *,QByteArray)),this,SLOT(parseResponseData(OnlineRequest *,QByteArray)));
    request->postHttpRequest(PIPLAY_HOST,command,data,"application/json");
}

void StatisticsBase::postEventData(){
    //qInfo()<<"postEventData";
    if(mSendingEvent.size()>0){
        return;
    }
    QDir dir(mCacheDir);
    if(!dir.exists()){
        return;
    }
    QJsonArray jsonArray;
    QStringList nameFilters;
    nameFilters.append("*.dat");
    for(QString fileName:dir.entryList(nameFilters)){
        QJsonObject object = JsonHandler::loadJsonObject(mCacheDir+"\\"+fileName);
        jsonArray.append(object);
        mSendingEvent.append(mCacheDir+"\\"+fileName);
    }
    if(jsonArray.size()==0){
        return;
    }
    qInfo()<<"post event data";
    postEventData(toString(jsonArray));
}

QString StatisticsBase::toString(QJsonObject jsonObject){
    QJsonDocument document;
    document.setObject(jsonObject);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    return QString(byte_array);
}

QString StatisticsBase::toString(QJsonArray jsonArray){
    QJsonDocument document;
    document.setArray(jsonArray);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    return QString(byte_array);
}
