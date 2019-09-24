#include "OnlineRequest.h"
#include <QDir>
#include <QStandardPaths>
#include <QProcess>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "Windows.h"
#include "Setting.h"

#define CODE_SUCCESS 8888
#define PAGE_SIZE 30

OnlineRequest::OnlineRequest(QObject *parent) : QObject(parent)
{
    m_pReply = nullptr;
    m_pNetworkManager = nullptr;
}
OnlineRequest::~OnlineRequest(){
    if(m_pNetworkManager!=nullptr){
        delete m_pNetworkManager;
        m_pNetworkManager = nullptr;
    }
    if(m_pReply!=nullptr){
        delete m_pReply;
        m_pReply = nullptr;
    }
    mReadBytes.clear();
}

QString OnlineRequest::getError(){
    return mError;
}
QString languages[]={"cn","en","jp","ge"};
void OnlineRequest::getHttpRequest(QString command,QString param){
    QString url = Setting::getHttpRequestAddr()+"/"+command;
    url = url+"?lang="+languages[Setting::getInstance()->getLanguage()];
    url = url + QString("&lcid=%1").arg(GetSystemDefaultLCID());
    if(!param.isEmpty()){
        url = url+"&"+param;
    }
    qInfo()<<"getHttpRequest"<<url;
    mReadBytes.clear();
    if (m_pNetworkManager == nullptr){
        m_pNetworkManager = new QNetworkAccessManager(this);
    }
    QNetworkRequest request;
    request.setUrl(url);
    m_pReply = m_pNetworkManager->get(request);

    connect(m_pReply,SIGNAL(finished()),this,SLOT(httpFinished()));

    connect(m_pReply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
    connect(m_pReply,SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
}

void OnlineRequest::httpFinished(){
    qInfo()<<"httpFinished";
    if(m_pReply!=nullptr){
        m_pReply->deleteLater();
        m_pReply = nullptr;
    }
    parseData(mReadBytes);
}

void OnlineRequest::httpReadyRead(){
    mReadBytes.append(m_pReply->readAll());
    qInfo()<<"httpReadyRead"<<mReadBytes.length();
}

void OnlineRequest::httpError(QNetworkReply::NetworkError code){
    qCritical()<<"httpError"<<code;
    if(m_pReply!=nullptr){
        m_pReply->deleteLater();
        m_pReply = nullptr;
    }
    QJsonObject jsonDefault;
    emit parseOnlineData(this,-1,"","get data error",jsonDefault);
}

void OnlineRequest::parseData(QByteArray &byte_array){
    QJsonObject jsonDefault;
    if(byte_array.length()==0){
        qCritical()<<"OnlineRequest get data error";
        emit parseOnlineData(this,-1,"","get data error",jsonDefault);
        return;
    }
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
    mReadBytes.clear();
    if(json_error.error == QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            QJsonObject obj = parse_doucment.object();
            int code = JsonHandler::getJsonValue(obj,"code",0);
            QString type = JsonHandler::getJsonValue(obj,"type","");
            QString message = JsonHandler::getJsonValue(obj,"message","");
            QJsonObject jsonData = JsonHandler::getJsonObjectValue(obj,"data",jsonDefault);
            qInfo()<<"OnlineRequest parseData success"<<code<<type<<message;
            emit parseOnlineData(this,code,type,message,jsonData);
        }else{
            qCritical()<<"OnlineRequest json data format error";
            emit parseOnlineData(this,-2,"",tr("json data format error"),jsonDefault);
        }
    }else{
        qCritical()<<"OnlineRequest json data format error"<<json_error.errorString();
        emit parseOnlineData(this,-3,"",json_error.errorString(),jsonDefault);
    }
}

void OnlineRequest::postHttpRequest(QString host,QString command,QString data,QString dataFormat){
    QString url = "http://"+host+command;
    if(data.isEmpty()){
        return;
    }
    QByteArray postData;
    postData.append(data);
    mReadBytes.clear();
    if (m_pNetworkManager == nullptr){
        m_pNetworkManager = new QNetworkAccessManager(this);
    }
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("host",host.toLocal8Bit());
    request.setRawHeader("Content-Type",dataFormat.toLocal8Bit());
    m_pReply = m_pNetworkManager->post(request,postData);

    connect(m_pReply,SIGNAL(finished()),this,SLOT(postFinished()));

    connect(m_pReply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
    connect(m_pReply,SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
}

void OnlineRequest::postFinished(){
    if(m_pReply!=nullptr){
        m_pReply->deleteLater();
        m_pReply = nullptr;
    }
    emit postOnlineData(this,mReadBytes);
    //mReadBytes.clear();
}

