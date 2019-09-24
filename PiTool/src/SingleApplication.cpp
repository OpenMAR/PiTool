#include "SingleApplication.h"
#include <QApplication>
#include <QLocalSocket>
#include <QFile>
#include <QWidget>
#include <QKeyEvent>


SingleApplication::SingleApplication(int argc, char **argv):QApplication(argc,argv)
{
    _isRunning=false;
    hMutex = NULL;
    qmlRootObject = nullptr;
    QCoreApplication::setApplicationName("PiTool");
    setApplicationDisplayName("PiTool");
    QString serverName= QCoreApplication::applicationName();
    QLocalSocket socket;
    socket.connectToServer(serverName);

    if(socket.waitForConnected(500))
    {

        _isRunning=true;

        return;
    }

    qDebug()<<"Can't connect to server,build a server";
    server=new QLocalServer(this);
    connect(server,SIGNAL(newConnection()),this,SLOT(newLocalConnection()));

    if(!server->listen(serverName)){
        if(server->serverError()==QAbstractSocket::AddressInUseError)
        {
            QLocalServer::removeServer(serverName);
            server->listen(serverName);
        }
    }
}

void SingleApplication::newLocalConnection()
{
    _activateWindow();
}


bool SingleApplication::isRunning()
{
    hMutex = CreateMutex(nullptr, FALSE, TEXT("pitool_window"));
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return true;
    }
    return _isRunning;
}

void SingleApplication::_activateWindow(){
    if(qmlRootObject){
        QVariant returnedValue;
        QMetaObject::invokeMethod(qmlRootObject, "showWindow", Q_RETURN_ARG(QVariant, returnedValue));
    }
}


void SingleApplication::stopLocalServer(){
    server->close();
    delete server;
    _isRunning = false;
    if(hMutex){
        ReleaseMutex(hMutex);
    }
}

bool SingleApplication::notify(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Backslash && (keyEvent->modifiers() == Qt::AltModifier))
        {
            emit cmdKeyPress();
            return true;
        }
    }

    return QApplication::notify(watched, event);
}
