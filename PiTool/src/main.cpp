#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include "piservice.h"
#include "OnlineItem.h"
#include "ResourceModel.h"
#include "Setting.h"
#include "OnlineItem.h"
#include "OnlineModel.h"
#include "hwObject.h"
#include "VersionModel.h"
#include <QScreen>
#include <QStandardPaths>
#include "quazip/JlCompress.h"
#include <QApplication>
#include <QIcon>
#include <QSplashScreen>
#include <QProcess>
#include "SingleApplication.h"
#include <QQmlContext>
#include <QAbstractNativeEventFilter>
#include <Windows.h>
#include <ostream>
#include <QMutex>
#include "PiLog/Exception.h"
#include "diagnoseHandler.h"
#include "VersionControl.h"
#include "CustomTrayicon.h"


QString mLogDir="";

LONG WINAPI applicationCrashHandler(EXCEPTION_POINTERS *ExceptionInfo)
{
    Exception::WriteStacktrace(ExceptionInfo,"PiTool");
    Exception::WriteMinidump(ExceptionInfo,"PiTool");
    return ExceptionInfo->ExceptionRecord->ExceptionCode;
}

void outputMessage(QtMsgType type, const QMessageLogContext &/*context*/, const QString &msg){
    QString text;
    bool writeFile = true;
    switch(type){
    case QtDebugMsg:
        text = QString("Debug:");
        writeFile = false;
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        //writeFile = false;
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
        break;
    case QtInfoMsg:
        text = QString("Info:");
        writeFile = false;
        break;
    }

    static QMutex mutex;
    QString current_date_time = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString current_date = QString("%1 ").arg(current_date_time);
    QString message = QString("%1 %2 %3\r\n").arg(current_date).arg(text).arg(msg);
    mutex.lock();
    if(writeFile){
        if(mLogDir.isEmpty()){
            mLogDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
            mLogDir = mLogDir.replace("PiTool","Pimax")+"\\PiTool";
            if(!QDir(mLogDir).exists()){
                QDir().mkdir(mLogDir);
            }
        }
        QFile file(mLogDir+QString("\\pitool_%1.log").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream text_stream(&file);
        text_stream << message;
        file.flush();
        file.close();
    }
    mutex.unlock();

    QByteArray data = message.toLocal8Bit();
    printf(data.data());

}

int main(int argc, char *argv[])
{
    bool forceSetup = false;
    bool hide = false;
    if(argc > 1){
        QString param1 =  argv[1];
        if(param1.compare("hide") == 0){
            Setting::setStartupHide(true);
            hide = true;
        }else if(param1.compare("restart") == 0){
            forceSetup = true;
        }
    }
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    SingleApplication app(argc, argv);
    if(app.isRunning() && !forceSetup){
        return 0;
    }

    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)applicationCrashHandler);

    QFont font;
    font.setFamily("Noto Sans CJK SC DemiLight");
    app.setFont(font);
    //register MessageHandler
    qInstallMessageHandler(outputMessage);
    qCritical()<<"PiTool start";
    QSplashScreen *splash = new QSplashScreen;
    if(!hide){
        splash->setPixmap(QPixmap(":/resource/PiPlay_logo.png"));
        splash->show();
    }
    app.processEvents();
    qmlRegisterType<Setting>("pimaxvr.Setting", 1, 0, "Setting");
    qmlRegisterType<PiService>("pimaxvr.PiService", 1, 0, "PiService");
    qmlRegisterType<ResourceItem>("pimaxvr.ResourceItem", 1, 0, "ResourceItem");
    qmlRegisterType<ResourceModel>("pimaxvr.ResourceModel", 1, 0, "ResourceModel");
    qmlRegisterType<OnlineModel>("pimaxvr.OnlineModel", 1, 0, "OnlineModel");
    qmlRegisterType<HWObject>("pimaxvr.HWObject", 1, 0, "HWObject");
    qmlRegisterType<VersionModel>("pimaxvr.VersionModel", 1, 0, "VersionModel");
    qmlRegisterType<VersionItem>("pimaxvr.VersionItem", 1, 0, "VersionItem");
    qmlRegisterType<DiagnoseHandler>("pimaxvr.DiagnoseHandler", 1, 0, "DiagnoseHandler");
    qmlRegisterType<VersionControl>("pimaxvr.VersionControl", 1, 0, "VersionControl");
    qmlRegisterType<MyMenu>("pimaxvr.utility", 1, 0, "MyMenu");
    qmlRegisterType<MyAction>("pimaxvr.utility", 1, 0, "MyAction");
    qmlRegisterType<MySeparator>("pimaxvr.utility", 1, 0, "MySeparator");
    qmlRegisterType<CustomTrayicon>("pimaxvr.utility", 1, 0, "CustomTrayicon");

    QQmlApplicationEngine engine;
    app.setWindowIcon(QIcon(":/icon.ico"));
    Setting::loadTranslate();
    engine.load(QUrl(QLatin1String("qrc:/qml/main_tools.qml")));
    qCritical()<<"load qml end";
    splash->finish(app.activeWindow());
    delete splash;
    app.qmlRootObject = engine.rootObjects().first();
    Setting::setQMlRootObject(engine.rootObjects().first());
    QObject::connect(&app, SIGNAL(cmdKeyPress()), app.qmlRootObject, SLOT(onCmdKeyPress()));
    qCritical()<<"init end";
    return app.exec();
}
