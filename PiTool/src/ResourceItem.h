#ifndef RESOURCE_ITEM_H
#define RESOURCE_ITEM_H

#include "Setting.h"
#include "DownloadFile.h"

#include <QObject>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
class ResourceItem:public QObject{
    Q_OBJECT
public:
    Q_PROPERTY(QString id READ id NOTIFY idChanged)
    Q_PROPERTY(QString thirdId READ thirdId NOTIFY thirdIdChanged)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)//"game","video","app"
    Q_PROPERTY(QString subType READ subType NOTIFY subTypeChanged) //steam,oculus,pimax,2d,3d,view
    Q_PROPERTY(ResourceStatus status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString file READ file NOTIFY filedChanged)
    Q_PROPERTY(QString download_url READ download_url NOTIFY downloadUrlChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString desc READ desc)
    Q_PROPERTY(QString image_url READ image_url WRITE setImageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QString create_date READ create_date NOTIFY createDateChanged)

    Q_PROPERTY(qint64 totalBytes READ totalBytes WRITE setTotalBytes NOTIFY totalBytesChanged)
    Q_PROPERTY(qint64 readBytes READ readBytes WRITE setReadBytes NOTIFY readBytesChanged)
    Q_PROPERTY(qint64 speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(QString remainTime READ getDLRemainTime NOTIFY remainTimeChanged)
    Q_PROPERTY(QString percent READ getDLPercent NOTIFY percentChanged)

    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(int upgradeFlag READ upgradeFlag WRITE setUpgradeFlag NOTIFY upgradeFlagChanged)

    Q_PROPERTY(bool bought READ bought WRITE setBought NOTIFY boughtChanged)
    Q_PROPERTY(QString boughtDate READ boughtDate NOTIFY boughtDateChanged)
    Q_PROPERTY(QString releaseDate READ releaseDate WRITE setReleaseDate NOTIFY releaseDateChanged)

    Q_INVOKABLE bool isSteamApp();
    Q_INVOKABLE ResourceItem* getObject();
    Q_INVOKABLE void deleteObject();

    Q_ENUMS(ResourceType)
    Q_ENUMS(ResourceStatus)

public:
    ResourceItem(QObject *parent = 0);
    ResourceItem(QString id, QString thirdId,QString type,QString subType,QString download_url,QString image_url,QString title,QString desc);
    ResourceItem *copyItem(ResourceItem *item);
    ~ResourceItem();

    enum ResourceStatus
    {
        STATUS_LOCAL = 0,           //local or downloaded
        STATUS_READY_DOWNLOAD = 1,
        STATUS_DOWNLOADING = 2,
        STATUS_INSTALLING = 3,
        STATUS_DELETED = 4,
        STATUS_DOWNLOAD_FAILED = 5,
        STATUS_ONLINE = 6,
        STATUS_COUNT=7
    };
    enum ResourceType{
        TYPE_GAME_PIMAX = 0x10,
        TYPE_GAME_STEAM= 0x11,
        TYPE_GAME_OCULUS = 0x12,
        TYPE_GAME_UNKOWN = 0x13,
        TYPE_VIDEO_2D = 0x20,
        TYPE_VIDEO_3D = 0x21,
        TYPE_VIDEO_VR = 0x22,
        TYPE_VIDEO_UNKOWN = 0x23,
    };

signals:
    void signalList(QList<QObject*> data);

    void idChanged(QString id);
    void thirdIdChanged(QString thirdId);
    void typeChanged(QString type);
    void subTypeChanged(QString subType);
    void statusChanged(ResourceStatus status);
    void filedChanged(QString file);
    void titleChanged(QString title);
    void downloadUrlChanged(QString download_url);
    void imageUrlChanged(QString image_url);
    void versionChanged(QString version);
    void upgradeFlagChanged(int upgradeFlag);
    void createDateChanged(QString date);
    void playTimeChanged(qint64 play_time);
    void totalBytesChanged(qint64 totalBytes);
    void readBytesChanged(qint64 readBytes);
    void speedChanged(qint64 speed);
    void remainTimeChanged(QString time);
    void percentChanged(QString percent);
    void boughtChanged(bool bought);
    void boughtDateChanged(QString date);
    void releaseDateChanged(QString date);
    void downloadFinished(ResourceItem* pItem);
    void released();

private slots:
    void httpFinished();
    void httpReadyRead();
    void downloadProgress(qint64 bytesRead, qint64 totalBytes);
    void downloadImageFinished(DownloadFile *,QString targetFile);
    void httpError(QNetworkReply::NetworkError);
    void copyObjectRelease();

public:
    QString id(){return mId;}
    void setId(QString id);
    QString thirdId(){return mThirdId;}
    void setThirdId(QString thridId){mThirdId=thridId;}
    QString type(){return mType;}
    void setType(QString type){mType=type;}
    QString subType(){return mSubType;}
    void setSubType(QString type){mSubType=type;}
    ResourceStatus status(){return mStatus;}
    void setStatus(ResourceStatus status);
    QString file(){return mFile;}
    void setFile(QString file){mFile = file;}
    QString download_url(){return mDownloadUrl;}
    void setDownloadUrl(QString file_url){mDownloadUrl=file_url;}
    QString title(){return mTitle;}
    void setTitle(QString title){mTitle=title;}
    QString desc(){return mDesc;}
    void setDesc(QString desc){mDesc=desc;}
    QString image_url(){return mImageUrl;}
    void setImageUrl(QString image_url){mImageUrl=image_url;}
    QString version(){return mVersion;}
    void setVersion(QString version){mVersion=version;}
    int upgradeFlag(){return mUpgradeFlag;}
    void setUpgradeFlag(int upgradeFlag){mUpgradeFlag = upgradeFlag;}

    bool bought(){return mBought;}
    void setBought(bool bought){mBought=bought;}
    QString releaseDate(){return mReleaseDate;}
    void setReleaseDate(QString date){mReleaseDate = date;}
    QString boughtDate();
    void setBoughtTime(qint64 bought_time){mBoughtTime = bought_time;}
    qint64 bought_time(){return mBoughtTime;}

    QString create_date();
    void setCreateTime();
    qint64 create_time();
    void setPlayTime(bool bSave=true);
    qint64 play_time();
    qint64 totalBytes();
    void setTotalBytes(qint64 totalBytes);
    qint64 readBytes();
    void setReadBytes(qint64 readBytes);
    qint64 speed();
    void setSpeed(qint64 speed);
    QString getDLRemainTime();
    QString getDLPercent();

    bool startDownload();
    bool stopDownload();
    qint64 getNowTime();

    bool loadSteamVr(QJsonObject json);
    bool loadFromFile(QString jsonFile);
    void saveToFile();
    void deleteItem(bool deleteFile=false);
    bool delDir(const QString &path);
    void deleteGame();
    void downloadImage();
    QString getPosfix(QString url);

private:
    QString mJsonFile;
    QString mDLingJsonFile;
    QString mId;
    QString mThirdId;
    QString mType;
    QString mSubType;
    ResourceStatus mStatus;
    QString mDir;
    QString mFile;
    QString mDownloadUrl;
    QString mTitle;
    QString mDesc;
    QString mImageUrl;
    qint64 mCreateTime;
    qint64 mPlayTime;
    QString mVersion;
    int mUpgradeFlag;//0:no update，1：need update
    bool mBought;
    qint64 mBoughtTime;
    QString mReleaseDate;


    qint64 mTotalBytes;
    qint64 mReadBytes;
    qint64 mSpeed;//(byte/s)
    qint64 mDownloadedSize;

    QNetworkReply *m_pReply;
    QNetworkAccessManager *m_pNetworkManager;
    QFile *m_hFile;
    qint64 mDlingTime;
    ResourceItem *mCopyObject;

};

#endif /*RESOURCE_ITEM_H*/
