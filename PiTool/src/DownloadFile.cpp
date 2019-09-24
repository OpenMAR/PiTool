#include "DownloadFile.h"
#include <QDir>
#include <QStandardPaths>
#include <QProcess>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

DownloadFile::DownloadFile(QObject *parent) : QObject(parent)
{
    m_pReply = nullptr;
    m_pNetworkManager = nullptr;
}
DownloadFile::~DownloadFile(){
    if(m_pNetworkManager!=nullptr){
        delete m_pNetworkManager;
        m_pNetworkManager = nullptr;
    }
    if(m_pReply!=nullptr){
        delete m_pReply;
        m_pReply = nullptr;
    }
}

void DownloadFile::download(QString url,QString targetFile){
    qInfo()<<"downloadFile:"<<url<<targetFile;
    mTargetFile = targetFile;
    if (m_pNetworkManager == nullptr){
        m_pNetworkManager = new QNetworkAccessManager(this);
    }
    m_hFile = new QFile(mTargetFile);
    if(!m_hFile->open(QIODevice::WriteOnly|QIODevice::Truncate)){
        qCritical()<<"downloadFile error"<<url<<targetFile;
        delete m_hFile;
        m_hFile = nullptr;
        emit downloadFinished(this,"");
        return;
    }
    QNetworkRequest request;
    request.setUrl(url);
    m_pReply = m_pNetworkManager->get(request); //send request
    //after downloaded
    connect(m_pReply,SIGNAL(finished()),this,SLOT(httpFinished()));
    //have ready data
    connect(m_pReply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
    connect(m_pReply,SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
}
void DownloadFile::httpFinished(){
    qInfo()<<"downloadFile httpFinished";
    if(m_pReply!=nullptr){
        m_pReply->deleteLater();
        m_pReply = nullptr;
    }
    m_hFile->flush();
    m_hFile->close();
    delete m_hFile;
    emit downloadFinished(this,mTargetFile);
}

void DownloadFile::httpReadyRead(){
    m_hFile->write(m_pReply->readAll());
}

void DownloadFile::httpError(QNetworkReply::NetworkError code){
    qCritical()<<"downloadFile httpError"<<code;
    if(m_pReply!=nullptr){
        m_pReply->deleteLater();
        m_pReply = nullptr;
    }
    emit downloadFinished(this,"");
}
