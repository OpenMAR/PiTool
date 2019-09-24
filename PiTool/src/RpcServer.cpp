#include "RpcServer.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "JsonHandler.h"

RpcServer::RpcServer(QObject *parent):QObject(parent){
    mCallback = nullptr;
    m_pServer = new QWebSocketServer("PiPlayRpc",QWebSocketServer::NonSecureMode,this);
    int port = 17926;
    if(m_pServer->listen(QHostAddress::Any,port)){
        qDebug()<<"RpcServer listen at port"<<port;
        connect(m_pServer,SIGNAL(newConnection()), this, SLOT(newConnection()));
        connect(m_pServer, SIGNAL(closed()), this, SLOT(closed()));;
    }else{
        qCritical()<<"RpcServer listen error,port:"<<port;
    }
}
RpcServer::~RpcServer(){
    m_pServer->close();
    delete m_pServer;
}

void RpcServer::setCallback(RpcServer_Callback callback){
    mCallback = callback;
}

void RpcServer::newConnection(){
    qDebug()<<"RpcServer newConnection";
    QWebSocket *pSocket=m_pServer->nextPendingConnection();
    if(!pSocket){
        return;
    }
    mClientList.append(pSocket);
    connect(pSocket,SIGNAL(textMessageReceived(const QString &)),this,SLOT(textMessageReceived(QString)));
    connect(pSocket,SIGNAL(binaryMessageReceived(const QByteArray &)), this, SLOT(processBinaryMessage(QByteArray)));
    connect(pSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
}

void RpcServer::closed(){
    qDebug()<<"RpcServer closed";
}

void RpcServer::textMessageReceived(QString message){
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qInfo()<<"RpcServer textMessageReceived "<<pClient<<message;
    processMessage(pClient,message);
}

void RpcServer::processBinaryMessage(QByteArray message){
    QString strMessage = QString(message);
    qInfo() << "RpcServer Binary Message received:" << message;
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    processMessage(pClient,strMessage);
}

void RpcServer::processMessage(QWebSocket *pClient,QString message){
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(message.toUtf8(), &json_error);
    if(json_error.error == QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            QJsonObject obj = parse_doucment.object();
            QString process = JsonHandler::getJsonValue(obj,"process","");
            QJsonObject jsonDefault;
            QJsonObject jsonParam = JsonHandler::getJsonObjectValue(obj,"param",jsonDefault);
            if(process==""){
                QString message="{\"error\":\"data format error,no process field\"}";
                pClient->sendTextMessage(message);
            }else if(mCallback){
                bool result = mCallback(pClient,process,jsonParam);
                if(!result){
                    QString message=QString("{\"error\":\"process %1 is not exist\"}").arg(process);
                    pClient->sendTextMessage(message);
                }
            }
        }else{
            qInfo()<<"RpcServer::textMessageReceived json data format error";
            QString message="{\"error\":\"json data format error\"}";
            pClient->sendTextMessage(message);
        }
    }else{
        qDebug()<<"RpcServer::textMessageReceived json data format error"<<json_error.errorString();
        QString message=QString("{\"error\":\"%1\"}").arg(json_error.errorString());
        pClient->sendTextMessage(message);
    }
}


void RpcServer::socketDisconnected(){
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        qDebug() << "RpcServer socketDisconnected:" << pClient;
        mClientList.removeOne(pClient);
        pClient->deleteLater();
    }
}

void RpcServer::sendMessage(QString process,QString param){
    QString message = QString("{\"process\":\"%1\",\"param\":%2}").arg(process).arg(param);
    for(QWebSocket *pClient : mClientList){
        pClient->sendTextMessage(message);
    }
}

void RpcServer::sendCallback(QWebSocket *pClient,QString process,QString result,QString value){
    if(mClientList.contains(pClient)){
        QString message = QString("{\"callback\":\"%1\"").arg(process);
        if(!result.isEmpty()){
            message += QString(",\"result\":\"%1\"").arg(result);
        }
        if(!value.isEmpty()){
            message += ",\"param\":"+value;
        }
        message +="}";
        qInfo()<<"sendCallback"<<message;
        pClient->sendTextMessage(message);
    }
}

void RpcServer::sendCallback(QWebSocket *pClient,QString process,bool result,QString error){
    if(mClientList.contains(pClient)){
        QString message = QString("{\"callback\":\"%1\",\"result\":%2").arg(process).arg(result?"true":"false");
        if(!error.isEmpty()){
            message += ",\"param\":"+QString("{\"error\":\"%1\"}").arg(error);
        }
        message +="}";
        pClient->sendTextMessage(message);
    }
}





