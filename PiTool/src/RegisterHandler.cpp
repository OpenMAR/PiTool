#include "RegisterHandler.h"
#include <QSettings>

RegisterHandler& RegisterHandler::getInstance(QObject *parent)
{
    static RegisterHandler regHandler(parent);
    return regHandler;
}

RegisterHandler::RegisterHandler(QObject *parent) : QObject(parent)
{

}


QString RegisterHandler::getRegisterValue(QString dir,QString key,QString defValue){
    QSettings reg(dir,QSettings::NativeFormat);
    if(reg.contains(key)){
        return reg.value(key).toString().trimmed();
    }
    return defValue;
}

void RegisterHandler::setRegisterValue(QString dir,QString key,QString value){
    QSettings reg(dir,QSettings::NativeFormat);
    reg.setValue(key,value);
}

int RegisterHandler::getRegisterValue(QString dir,QString key,int defValue){
    QSettings reg(dir,QSettings::NativeFormat);
    if(reg.contains(key)){
        return reg.value(key).toInt();
    }
    return defValue;
}

void RegisterHandler::setRegisterValue(QString dir,QString key,int value){
    QSettings reg(dir,QSettings::NativeFormat);
    reg.setValue(key,value);
}
