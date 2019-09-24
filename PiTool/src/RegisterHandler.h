#ifndef REGISTERHANDLER_H
#define REGISTERHANDLER_H

#include <QObject>

class RegisterHandler : public QObject
{
    Q_OBJECT
public:
    static RegisterHandler& getInstance(QObject *parent = 0);
    QString getRegisterValue(QString dir,QString key,QString defValue);
    void setRegisterValue(QString dir,QString key,QString value);
    int getRegisterValue(QString dir,QString key,int defValue);
    void setRegisterValue(QString dir,QString key,int value);

signals:

public slots:

private:
    explicit RegisterHandler(QObject *parent = 0);
};

#endif // REGISTERHANDLER_H
