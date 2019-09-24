#ifndef SETTING_MODEL_H
#define SETTING_MODEL_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
class SettingModel: public QObject
{
    Q_OBJECT
public:
    explicit SettingModel(QObject *parent = 0);
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(int buildVersion READ buildVersion NOTIFY buildVersionChanged)
    Q_PROPERTY(int language READ getLanguage WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString downloadDir READ getDownloadDir WRITE setDownloadDir NOTIFY downloadDirChanged)
    Q_PROPERTY(QString videoPlayer READ getVideoPlayer WRITE setVideoPlayer NOTIFY videoPlayerChanged)
    Q_PROPERTY(QString videoPlayer2D READ getVideoPlayer2D WRITE setVideoPlayer2D NOTIFY videoPlayer2DChanged)
    Q_PROPERTY(bool autoRun READ isAutoRun WRITE setAutoRun NOTIFY autoRunChanged)
    Q_PROPERTY(bool betaTest READ isBetaTest WRITE setBetaTest NOTIFY betaTestChanged)
    Q_PROPERTY(bool piplayBetaTest READ isPiplayBetaTest WRITE setPiplayBetaTest NOTIFY piplayBetaTestChanged)
    Q_PROPERTY(bool closeToNotibar READ isCloseToNotibar WRITE setCloseToNotibar NOTIFY closeToNotibarChanged)
    Q_PROPERTY(QString privacy READ privacy NOTIFY privacyChanged)

    Q_INVOKABLE QString formatBytes(qint64 bytes);
    Q_INVOKABLE int checkDirectory(QString dir);
    Q_INVOKABLE bool exportLog(QString zipFile);
    Q_INVOKABLE void restart();
    Q_INVOKABLE void restoreToDefault();
    Q_INVOKABLE QString getLangAndLcid();
    Q_INVOKABLE bool drPiIsInstalled();
    Q_INVOKABLE void openDrPi();

    static void loadTranslate();
    static bool getJsonValue(QJsonObject json,QString key,bool defValue);
    static QString getJsonValue(QJsonObject json,QString key,QString defValue);
    static QString getJsonValue(QJsonObject json,QString key,const char *defValue);
    static double getJsonValue(QJsonObject json,QString key,double defValue);
    static int getJsonValue(QJsonObject json,QString key,int defValue);
    static quint64 getJsonValue_long(QJsonObject json,QString key,quint64 defValue);
    static QJsonArray getJsonValue(QJsonObject json,QString key,QJsonArray defValue);
    static QJsonObject getJsonObjectValue(QJsonObject json,QString key,QJsonObject defValue);
    static QJsonObject loadJsonObject(QString fileName);
    static QString loadText(QString fileName);
    static QString getHttpRequestAddr();
    static QString getRegisterValue(QString dir,QString key,QString defValue);//读取注册表项
    static void setRegisterValue(QString dir,QString key,QString value); //写注册表项
    static int getRegisterValue(QString dir,QString key,int defValue);//读取注册表项
    static void setRegisterValue(QString dir,QString key,int value); //写注册表项
    static QString getInstallPath();
    static QString getDownloadDir();
    static QString getOvrLauncher();
    static QString getVideoPlayer();
    static QString getVideoPlayer2D();
    static QString getValueFromText(QString text,QString key);
    static QString getValueFromText(QString text, QString key1,QString key2);
    static void setQMlRootObject(QObject *qmlObject);
    static void showAlert(QString message);
    static void showAlertEx(QString messageId);
    static void showMessage(QString message);
    static void showMessageEx(QString message);
    static void callQmlFunc(const char *func);

    static bool processIsRunning(QString processName);
    static void killProcess(QString processName);

    static SettingModel* getInstance();

signals:
    void buildVersionChanged(int buildVersion);
    void versionChanged(QString version);
    void languageChanged(int language);
    void downloadDirChanged(QString downloadDir);
    void videoPlayerChanged(QString videoPlayer);
    void videoPlayer2DChanged(QString videoPlayer);
    void autoRunChanged(bool autoRun);
    void betaTestChanged(bool betaTest);
    void piplayBetaTestChanged(bool piplayBetaTest);
    void closeToNotibarChanged(bool closeToNotibar);
    void privacyChanged(QString privacy);


public slots:

private:
    int mLanguage;
    bool mAutoRun;//开机自动运行
    bool mBetaTest;//参与Beta版本测试
    bool mPiplayBetaTest;//参与piplay beta版本测试
    bool mCloseToNotibar;//关闭窗口时，隐藏到任务栏通知区
    QString mPrivacy;
    QString mVersion;
    int mBuildVersion;

public:
    typedef enum Language_
    {
        LANGUAGE_CHINESE = 0,
        LANGUAGE_ENGLISH = 1,
        LANGUAGE_JAPANESE =2,
        LANGUAGE_GERMAN = 3,
        LANGUAGE_COUNT = 4
    } Language;

    void loadVersion();
    QString version();
    int buildVersion();

    int getLanguage();
    void setLanguage(int language);

    void setDownloadDir(QString dir);
    void setVideoPlayer(QString videoPlayer);
    void setVideoPlayer2D(QString videoPlayer);

    bool isAutoRun();
    void setAutoRun(bool autoRun);

    bool isBetaTest();
    void setBetaTest(bool betaTest);

    bool isPiplayBetaTest();
    void setPiplayBetaTest(bool piplayBetaTest);

    bool isCloseToNotibar();
    void setCloseToNotibar(bool closeToNotibar);

    QString privacy();
    static quint64 getDiskFreeSpace(QString driver);
};

#endif // SETTING_MODEL_H
