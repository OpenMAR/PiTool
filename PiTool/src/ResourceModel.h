#ifndef RESOURCE_MODEL_H
#define RESOURCE_MODEL_H

#include <QObject>
#include "ResourceItem.h"
#include <QJsonArray>
#include "RpcServer.h"
#include <QJsonObject>
#include "OnlineRequest.h"
#include <QProcess>
#include <qmap.h>
#include <Windows.h>
#include "SteamModel.h"
class MyProcess{
public:
    MyProcess(QString id,QProcess *process){this->id=id;this->process=process;}
    QString id;
    QProcess *process;
};

class ResourceModel : public QObject
{
    Q_OBJECT
public:
    explicit ResourceModel(QObject *parent = 0);
    ~ResourceModel();
    Q_PROPERTY(QList<QObject*> searchResultList READ searchResultList NOTIFY searchResultListChanged)
    Q_PROPERTY(int resCount READ getResCount NOTIFY resCountChanged)
    Q_PROPERTY(int dlingCount READ getDlingCount NOTIFY dlingCountChanged)
    //local resource
    Q_INVOKABLE bool loadLocalResouce(QString resourceType, QString fileName);
    Q_INVOKABLE QObject* getItem(QString id,QString third_id) const;
    Q_INVOKABLE bool playGame(QString id);
    Q_INVOKABLE bool playVideo(QString id);
    Q_INVOKABLE bool play2DVideo(QString id);
    Q_INVOKABLE void playOnlineVideo(QString url);
    Q_INVOKABLE void playResource(QString fileUrl);
    Q_INVOKABLE bool deleteResource(QString resId,bool deleteFile);
    //download resource
    Q_INVOKABLE bool downloadResouce(QString id, QString thirdId,QString type,QString subType,
                                     QString download_url,QString image_url,QString title,QString desc);
    Q_INVOKABLE void startDownload(QString id);
    Q_INVOKABLE void stopDownload(QString id);
    Q_INVOKABLE QString formatBytes(quint64 bytes);
    //research resource,resType(0:all,1:game,2:videos,3:picture),sortType(0:date,1:name)
    Q_INVOKABLE void searchResource(QString keyword,int resType,int sortType,bool downloading,bool upgrading);
    Q_INVOKABLE void searchResource(QString keyword,int resType,int sortType,int searchType, int gameType = 0);
    //SteamVR Support
    Q_INVOKABLE bool steamVrIsInstalled();
    Q_INVOKABLE void checkSteamGame();
    Q_INVOKABLE bool oculusHomeIsInstalled();
    Q_INVOKABLE void checkOculusGame();
    Q_INVOKABLE void launchSteamVr();
    Q_INVOKABLE void reService();
    Q_INVOKABLE void launchPihome(bool bStart);
    Q_INVOKABLE bool pihomeIsExist();
    //
    Q_INVOKABLE void searchBoughtResource(QString access_token);
    Q_INVOKABLE void clearBoughtResource();

    Q_INVOKABLE QString getError();
    Q_INVOKABLE int getErrorCode();
    Q_ENUMS(ErrorCode)    
    Q_ENUMS(SortType)

signals:
    void dlingCountChanged(int dlingCount);
    void resCountChanged(int resCount);
    void gameListChanged(QList<QObject*> list);
    void videoListChanged(QList<QObject*> list);
    void pictureListChanged(QList<QObject*> list);
    void dlingListChanged(QList<QObject*> list);
    void searchResultListChanged(QList<QObject*> list);

private slots:
    void downloadFinished(ResourceItem *item);
    void parseOnlineBoughtData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

public:
    int getDlingCount();
    int getResCount();
    QList<QObject*> gameList();
    QList<QObject*> videoList();
    QList<QObject*> pictureList();
    QList<QObject*> dlingList();
    QList<QObject*> searchResultList();

    enum{
        ERROR_NO_CONNECT=0,
        ERROR_FILE_NOT_EXIST,
        ERROR_DOWNLOAD_DIR_NOT_EXIST
    } ErrorCode;
    enum{
        SORT_BY_PLAYTIME=0,
        SORT_BY_DATE=1,
        SORT_BY_NAME=2,
        SORT_BY_BOUGHT_TIME=3,
        SORT_BY_RELEASE_DATE=4
    } SortType;
    enum{
        SORT_BY_GAME_ALL=0,
        SORT_BY_GAME_STEAMVR=1,
        SORT_BY_GAME_OCULUS=2,
        SORT_BY_GAME_LOCAL=3
    }GameType;
    static bool __stdcall rpcServer_CallbackSt(QWebSocket *pSocket,QString function,QJsonObject param);
    bool rpcServer_Callback(QWebSocket *pSocket,QString function,QJsonObject param);
    static void sendMessage_addResource(QString resId);
    static void sendMessage_deleteResource(QString resId);
    static void sendMessage_statusChanged(QString resId,int status);
    static void sendMessage_progressChanged(QString resId,int status,qint64 total_bytes,qint64 downloaded_bytes);
    RpcServer *mRpcServer;

private:
    void initResource();
    void loadSteamVrGame();
    void checkSteamGameIsDeleted();
    std::wstring getHLKMRegStringVal(std::wstring path, std::wstring key);
    QStringList getOCHomeDefaultLibraryPath();
    bool getOculusGamePathList();
    void loadOculusGame();
    void checkOculusGameIsDeleted();
    bool isSteamApp(QString appFile);
    bool launchApp(QString id,QString app,QString param,bool bChecked=true);
    void killSteamVr();
    void loadPiplay1Data();
    bool playVideoEx(QString resId);
    ResourceItem* loadLocalResouce(QString resourceType, QString fileName,QString name,bool showError,QString resId);
    void sortList(QList<QObject*> *list,int sortType);
    ResourceItem *getResourceItem(QString id);

    static bool compareResourceItem( QObject *itemA,QObject *itemB);
    static bool compareResourceItemByName( QObject *itemA,QObject *itemB);
    static bool compareResourceItemByPlayTime( QObject *itemA,QObject *itemB);

    QList<QObject*> mSearchResultList;
    QList<QObject*> mResourceList;

    QString getFileName(QString id,QString url,QString type);
    bool appendLocalResource(ResourceItem *resource);

    SteamModel mSteamModel;
//    QString m_steamPath;
//    QString m_steamManifestPath;
//    QString m_steamInstallFolder;
    QStringList m_steamGames;

    QStringList m_oculusAppPathList;

    QString m_oculusPath;
    QString m_oculusAppPath;
    QStringList m_oculusGames;
    QString mError;
    int m_nError;

    QList<MyProcess*> mProcessList;
    void killAllProcess();
    void addProcess(QString id,QProcess *process);
};

#endif // RESOURCE_MGR_H
