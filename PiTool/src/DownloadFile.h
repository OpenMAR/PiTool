#ifndef DOWNLOAD_FILE_H
#define DOWNLOAD_FILE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class DownloadFile : public QObject
{
    Q_OBJECT
public:
    DownloadFile(QObject *parent = 0);
    ~DownloadFile();
    void download(QString url,QString targetFile);
    QString getError();

signals:
    void downloadFinished(DownloadFile* pDownload,QString targetFile);

private slots:
    void httpFinished();
    void httpReadyRead();
    void httpError(QNetworkReply::NetworkError);

private:
    QNetworkAccessManager *m_pNetworkManager;
    QNetworkReply *m_pReply;
    QString mTargetFile;
    QFile *m_hFile;
};

#endif // RESOURCE_MGR_H
