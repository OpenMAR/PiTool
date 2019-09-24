#ifndef ONLINE_REQUEST_H
#define ONLINE_REQUEST_H

#include <QObject>
#include "OnlineItem.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class OnlineRequest : public QObject
{
    Q_OBJECT
public:
    OnlineRequest(QObject *parent = 0);
    ~OnlineRequest();
    void getHttpRequest(QString request,QString param="");
    void postHttpRequest(QString host,QString request,QString data,QString dataFormat="application/x-www-form-urlencoded");
    QString getError();

signals:
    void parseOnlineData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);
    void postOnlineData(OnlineRequest* request,QByteArray data);

private slots:
    void httpFinished();
    void postFinished();
    void httpReadyRead();
    void httpError(QNetworkReply::NetworkError);

private:
    QNetworkAccessManager *m_pNetworkManager;
    QNetworkReply *m_pReply;
    QString mError;
    QByteArray mReadBytes;
    void parseData(QByteArray &byte_array);

};

#endif // RESOURCE_MGR_H
