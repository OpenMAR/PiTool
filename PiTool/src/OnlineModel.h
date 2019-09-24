#ifndef ONLINE_MODEL_H
#define ONLINE_MODEL_H

#include <QObject>
#include "OnlineItem.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include "OnlineRequest.h"

class OnlineModel : public QObject
{
    Q_OBJECT
public:
    explicit OnlineModel(QObject *parent = 0);
    Q_PROPERTY(QString helpMore READ helpMore NOTIFY helpMoreChanged)
    Q_PROPERTY(HelpItem* help READ help NOTIFY helpChanged)
    Q_PROPERTY(QList<QObject*> gameFilter READ gameFilter NOTIFY gameFilterChanged)
    Q_PROPERTY(QList<QObject*> videoFilter READ videoFilter NOTIFY videoFilterChanged)

    Q_INVOKABLE void getFilters();
    Q_INVOKABLE void getGameHome();
    Q_INVOKABLE void getGameDetail(QString id);
    Q_INVOKABLE void getAllGame(int page,QString orderField,bool desc);
    Q_INVOKABLE void searchGame(QString keyword,int pageIndex, QString orderField,bool desc);
    Q_INVOKABLE void filterGame(QString filter,int pageIndex, QString orderField,bool desc);
    Q_INVOKABLE void getVideoHome();
    Q_INVOKABLE void getVideoDetail(QString id);
    Q_INVOKABLE void getAllVideo(int page,QString orderField,bool desc);
    Q_INVOKABLE void searchVideo(QString keyword,int pageIndex, QString orderField,bool desc);
    Q_INVOKABLE void filterVideo(QString filter,int pageIndex, QString orderField,bool desc);
    Q_INVOKABLE void catalogVideo(int catalogId,int pageIndex);
    Q_INVOKABLE void getHelp();
    Q_INVOKABLE bool getInternetConnectState();

    Q_INVOKABLE QString getError();
signals:
    void gameHomeList(bool bResult,GameItem* banner,CatalogItem* catalogs);
    void gameList(bool bResult,GameItem* games=nullptr,int itemSum=0,int pageSize=0,int pageIndex=0,QString type="allGame",QString param="",QString title="",CatalogItem* recommend=nullptr);
    void gameDetail(bool bResult, GameItem *detail=nullptr,CatalogItem* recommend=nullptr);
    void videoHomeList(bool bResult,VideoItem* banner,CatalogItem* catalogs);
    void videoList(bool bResult,VideoItem* videos=nullptr,int itemSum=0,int pageSize=0,int pageIndex=0,QString type="allVideo",QString param="",CatalogItem* recommend=nullptr);
    void videoDetail(bool bResult, VideoItem *detail=nullptr,CatalogItem* recommend=nullptr);
    void helpList(bool bResult, HelpItem *help=nullptr, QString more="");
    void helpMoreChanged(QString helpMore);
    void helpChanged(HelpItem *item);
    void gameFilterChanged(QList<QObject*>);
    void videoFilterChanged(QList<QObject*>);
private slots:
    void parseFiltersData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);
    void parseGameHomeData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);
    void parseGameDetailData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);
    void parseGameSearchData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);
    void parseVideoHomeData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);
    void parseVideoDetailData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);
    void parseVideoSearchData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);
    void parseHelpData(OnlineRequest* request,int code,QString type,QString message,QJsonObject dataObject);

private:
    QString mError;
    HelpItem *mHelpList;
    QString mHelpMore;
    QString mRequestParam;

private:
    QString helpMore();
    HelpItem *help();
    QList<QObject*> gameFilter();
    QList<QObject*> videoFilter();
};

#endif // RESOURCE_MGR_H
