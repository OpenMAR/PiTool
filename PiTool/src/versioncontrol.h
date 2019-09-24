#ifndef VERSIONCONTROL_H
#define VERSIONCONTROL_H
#include <QObject>
#include "VersionModel.h"

class VersionControl : public QObject
{
    Q_OBJECT
public:
    static VersionControl* getInstance();
    explicit VersionControl(QObject *parent = 0);

signals:

public slots:

public:
    bool shouldEnforcingUpgrade(){return mEnforingVersion;}
    QString getCurrentVersion(){return mCurrentVersion;}
    QString getNewestVersion(){return mNewestVersion;}
    QString getEnforcingVersion(){return mEnforcingVersion;}
    bool canUpgradingVersion(){return mUpgradeVersion;}
    void checkVersion();
private:
    int compareVersion(QString version1, QString version2);

private:
    VersionModel* mVersionModel;
    QString mCurrentVersion;
    QString mNewestVersion;
    QString mEnforcingVersion;
    bool mUpgradeVersion;
    bool mEnforingVersion;
};

#endif // VERSIONCONTROL_H
