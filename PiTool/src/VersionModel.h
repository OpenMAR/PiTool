#ifndef VERSION_MODEL_H
#define VERSION_MODEL_H

#include <QObject>
#include "OnlineItem.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include "OnlineRequest.h"

class VersionItem:public QObject{
    Q_OBJECT
public:
    Q_PROPERTY(QString appName READ appName NOTIFY appNameChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(int buildVersion READ buildVersion NOTIFY buildVersionChanged)
    Q_PROPERTY(QString downloadUrl READ downloadUrl NOTIFY downloadUrlChanged)
    Q_PROPERTY(QString upgradeContent READ upgradeContent NOTIFY upgradeContentChanged)
    Q_PROPERTY(int upgradeFlag READ upgradeFlag WRITE setUpgradeFlag NOTIFY upgradeFlagChanged)
    Q_PROPERTY(int status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(qint64 totalBytes READ totalBytes WRITE setTotalBytes NOTIFY totalBytesChanged)
    Q_PROPERTY(qint64 readBytes READ readBytes WRITE setReadBytes NOTIFY readBytesChanged)
    Q_INVOKABLE bool startDownload();
    Q_INVOKABLE bool startDownloadByUrl(QString url,QString name);
    Q_INVOKABLE bool stopDownload();
    Q_INVOKABLE bool install();
    Q_INVOKABLE QString getVersionFile();
    Q_INVOKABLE void clearData();

    Q_ENUMS(UpgradeFlag)
    Q_ENUMS(Status)
public:
    VersionItem(QObject *parent = 0);
    enum UpgradeFlag
    {
        UPGRADE_FLAG_NOUPGRADE = 0,
        UPGRADE_FLAG_FORCE = 1,
        UPGRADE_FLAG_MINVERSION_FORCE = 2,
        UPGRADE_FLAG_OPTIONAL = 3

    };
    enum Status{
        STATUS_INIT = 0,
        STATUS_CHECKING = 1,
        STATUS_CHECKED = 2,
        STATUS_CHECK_FAILED = 3,
        STATUS_DOWNLOADING = 4,
        STATUS_DOWNLOADED = 5,
        STATUS_UPGRADED = 6,
        STATUS_UPGRADE_FAILED = 7,
        STATUS_DOWNLOAD_FAILED = 8
    };

    bool load(QJsonObject item);
    void setStatus(int status);
    int status(){return mStatus;}

signals:
    void appNameChanged(QString appName);
    void versionChanged(QString version);
    void buildVersionChanged(int buildVersion);
    void downloadUrlChanged(QString downloadUrl);
    void upgradeContentChanged(QString content);
    void upgradeFlagChanged(int upgradeFlag);
    void statusChanged(int status);
    void totalBytesChanged(qint64 totalBytes);
    void readBytesChanged(qint64 readBytes);

private slots:
    void httpFinished();
    void httpReadyRead();
    void downloadProgress(qint64 bytesRead, qint64 totalBytes);
    void httpError(QNetworkReply::NetworkError code);

private:
    QString appName(){return mAppName;}
    QString version(){return mVersion;}
    int buildVersion(){return mBuildVersion;}
    QString downloadUrl(){return mDownloadUrl;}
    QString upgradeContent(){return mUpgradeContent;}
    int upgradeFlag(){return mUpgradeFlag;}
    void setUpgradeFlag(int flag){mUpgradeFlag=flag;emit upgradeFlagChanged(mUpgradeFlag);}
    qint64 totalBytes(){return mTotalBytes;}
    void setTotalBytes(qint64 totalBytes);
    qint64 readBytes(){return mReadBytes;}
    void setReadBytes(qint64 readBytes);
    QString getPosfix(QString url);

private:
    QString mAppName;
    QString mVersion;
    int mBuildVersion;
    QString mDownloadUrl;
    QString mUpgradeContent;
    int mUpgradeFlag;
    int mStatus;
    qint64 mTotalBytes;
    qint64 mReadBytes;
    qint64 mDownloadedSize;
    int mBetaFlag;
    QString mFileName;

    QNetworkReply *m_pReply;
    QNetworkAccessManager *m_pNetworkManager;
    QFile *m_hFile;

};



class VersionModel : public QObject
{
    Q_OBJECT
public:
    explicit VersionModel(QObject *parent = 0);
    ~VersionModel();
    Q_PROPERTY(VersionItem* piPlayVersion READ piPlayVersion NOTIFY piPlayVersionChanged)
    Q_PROPERTY(VersionItem* pihomeVersion READ pihomeVersion NOTIFY pihomeVersionChanged)
    Q_PROPERTY(VersionItem* firmwareVersion READ firmwareVersion NOTIFY firmwareVersionChanged)
    Q_PROPERTY(VersionItem* leapMotionVersion READ leapMotionVersion NOTIFY leapMotionVersionChanged)


    Q_INVOKABLE void checkPiPlayVersion(int buildVersion,bool supportBeta);
    Q_INVOKABLE void checkPiHomeVersion(int buildVersion,bool supportBeta);
    Q_INVOKABLE void checkFirmwareVersion(int buildVersion,bool supportBeta,QString hmdName);

    Q_INVOKABLE QString getError();
    Q_INVOKABLE QString release_note(QString version);
signals:
    void piPlayVersionChanged(VersionItem* item);
    void pihomeVersionChanged(VersionItem* item);
    void firmwareVersionChanged(VersionItem* item);
    void leapMotionVersionChanged(VersionItem* item);

private slots:
    void parsePiPlayData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);
    void parsePiHomeData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);
    void parseFirmwareData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);

private:
    VersionItem* piPlayVersion(){return mPiPlayVersion;}
    VersionItem* pihomeVersion(){return mPihomeVersion;}
    VersionItem* firmwareVersion(){return mFirmwareVersion;}
    VersionItem* leapMotionVersion(){return mLeapMotionVersion;}
    void clearCache();

    QString mError;
    VersionItem *mPiPlayVersion;
    VersionItem *mFirmwareVersion;
    VersionItem *mPihomeVersion;
    VersionItem *mLeapMotionVersion;
    QString mReleaseNote;
};

#endif // RESOURCE_MGR_H
