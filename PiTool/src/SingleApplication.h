#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H
#include <QLocalServer>
#include <QApplication>
#include <QWidget>
#include <Windows.h>
#include <QObject>

class SingleApplication:public QApplication
{
    Q_OBJECT
public:
    SingleApplication(int argc,char **argv);
    QObject *qmlRootObject;
    bool isRunning();
    void stopLocalServer();

protected:
    bool notify(QObject* watched, QEvent* event);
    bool keyPressEvent();

signals:
    void cmdKeyPress();

private slots:
    void newLocalConnection();

private:
    QLocalServer *server;
    void _activateWindow();
    bool _isRunning;
    HANDLE hMutex;
};

#endif // MY_APPLICATION_H
