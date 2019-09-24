#ifndef STEAM_MODEL_H
#define STEAM_MODEL_H

#include <QObject>
#include "ResourceItem.h"
#include <QJsonArray>
#include "RpcServer.h"
#include <QJsonObject>
#include "OnlineRequest.h"
#include <QProcess>
#include <qmap.h>

class SteamModel : public QObject
{
    Q_OBJECT
public:
    explicit SteamModel(QObject *parent = 0);
    ~SteamModel();
    //SteamVR Support

    bool steamVrIsInstalled();
    void initSteamPath();
    void getInstallFolder();
    QString getExeFile(QString steamAppId);
    bool steamGameIsDownloding(QString gameid);
    bool isSteamApp(QString appFile);
    void killSteamVr();

public:
    QString m_steamAppFolder;
    QStringList m_steamInstallFolder;
    QString m_steamVRFile;

    QString m_steamManifestPath;
    QStringList m_steamGames;
};

#endif // STEAM_MODEL_H
