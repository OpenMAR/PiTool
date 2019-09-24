#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include "RegisterHandler.h"
#include "VersionModel.h"

class Setting : public QObject
{
    Q_OBJECT
public:
    explicit Setting(QObject *parent = 0);
    static Setting* getInstance(QObject *parent = 0);
    Q_PROPERTY(bool mAutoRun READ isAutoRun WRITE setAutoRun NOTIFY aotuRunChanged)
    Q_PROPERTY(int mLanguage READ getLanguageByStr WRITE setLanguageStr NOTIFY languageChanged)
    Q_PROPERTY(QStringList mLanguageList READ getLanguageList)
    Q_PROPERTY(bool mCloseToNotibar READ isCloseToNotibar WRITE closeToNotibar NOTIFY closeToNotibarSettingChanged)
    Q_PROPERTY(QString privacy READ privacy)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(int buildVersion READ buildVersion NOTIFY buildVersionChanged)
    Q_PROPERTY(int startupHide READ getStartupHide)
    Q_PROPERTY(bool mPvrHomeEnabled READ getPvrHomeEnabled WRITE setPvrHomeEnabled)

    Q_INVOKABLE QString getLangAndLcid();
    Q_INVOKABLE bool exportLog(QString zipFile);
    Q_INVOKABLE void restart();
    Q_INVOKABLE void restoreToDefault();
    Q_INVOKABLE QString formatBytes(qint64 bytes);
    Q_INVOKABLE void versionCheck();
signals:
    void aotuRunChanged(bool aotoRun);
    void languageChanged(int language);
    void closeToNotibarSettingChanged(bool hide);
    void versionChanged(QString ver);
    void buildVersionChanged(int buildVer);
public slots:

public:
    bool isAutoRun();
    void setAutoRun(bool autoRun);
    int getLanguage();
    int getLanguageByStr();
    QStringList getLanguageList();
    void setLanguage(int language);
    void setLanguageStr(int language);
    bool isCloseToNotibar();
    void closeToNotibar(bool hide);
    static void setQMlRootObject(QObject *qmlObject);
    static void loadTranslate();
    static void loadTranslate_old();
    static QString loadText(QString fileName);
    static void showAlert(QString message);
    static void showAlertEx(QString message_id);
    static QString getDownloadDir();
    static QString getHttpRequestAddr();
    static quint64 getDiskFreeSpace(QString driver);
    static void showMessage(QString message);
    static void callQmlFunc(const char *func);
    static QString getInstallPath();
    static QString getOvrLauncher();
    static void setStartupHide(bool hide);
    QString privacy_old();
    QString privacy();
    QString version(){return mVersion;}
    int buildVersion(){return mBuildVersion;}
    bool getStartupHide();
    bool getPvrHomeEnabled();
    void setPvrHomeEnabled(bool pvrHomeEnabled);


    typedef enum Language_{
        LANGUAGE_CHINESE,
        LANGUAGE_ENGLISH,
        LANGUAGE_COUNT
    }Language;

private:
    int mLanguage;
    bool mAutoRun;
    bool mCloseToNotibar;//true:hide ui;false:close app;
    QString mPrivacy;
    QString mVersion;
    int mBuildVersion;
    bool mPvrHomeEnabled; //pvr home

};

#endif // SETTING_H
