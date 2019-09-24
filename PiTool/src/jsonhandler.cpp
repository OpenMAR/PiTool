#include "jsonhandler.h"
#include <QFile>
#include <QJsonParseError>
#include <QVariant>

JsonHandler::JsonHandler()
{

}

double JsonHandler::getJsonValue(QJsonObject json,QString key,double defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isDouble()){
            return value.toDouble();
        }
    }
    return defValue;
}

int JsonHandler::getJsonValue(QJsonObject json,QString key,int defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isDouble()){
            return value.toInt();
        }
    }
    return defValue;
}

quint64 JsonHandler::getJsonValue_long(QJsonObject json,QString key,quint64 defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isDouble()){
            return (quint64)value.toVariant().toULongLong();
        }
    }
    return defValue;
}


bool JsonHandler::getJsonValue(QJsonObject json,QString key,bool defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isBool()){
            return value.toBool();
        }
    }
    return defValue;
}

QString JsonHandler::getJsonValue(QJsonObject json,QString key,QString defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isString()){
            return value.toString();
        }else if(value.isDouble()){
            return QString::number(value.toInt());
        }
    }
    return defValue;
}

QString JsonHandler::getJsonValue(QJsonObject json,QString key,const char *defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isString()){
            return value.toString();
        }else if(value.isDouble()){
            return QString::number(value.toInt());
        }
    }
    return QString(defValue);
}

QJsonArray JsonHandler::getJsonValue(QJsonObject json,QString key,QJsonArray defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isArray()){
            return value.toArray();
        }
    }
    return defValue;
}

QJsonObject JsonHandler::getJsonObjectValue(QJsonObject json,QString key,QJsonObject defValue){
    if(json.contains(key)){
        QJsonValue value = json.take(key);
        if(value.isObject()){
            return value.toObject();
        }
    }
    return defValue;
}

QJsonObject JsonHandler::loadJsonObject(QString fileName){
    QJsonObject defObject;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        return defObject;
    }
    QByteArray byte_array = file.readAll();
    file.close();
    if(byte_array.length()==0){
        return defObject;
    }
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
    if(json_error.error == QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            return parse_doucment.object();
        }
    }
    return defObject;
}

bool JsonHandler::loadJson(QString fileName, QJsonObject& object)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        return false;
    }
    QByteArray byte_array = file.readAll();
    file.close();
    if(byte_array.length()==0){
        return false;
    }
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
    if(json_error.error == QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            object = parse_doucment.object();
            return true;
        }
    }
    return false;
}

bool JsonHandler::getJsonValueByKey(QJsonObject json,QString key,double& value){
    if(json.contains(key)){
        QJsonValue jsonValue = json.take(key);
        if(jsonValue.isDouble()){
            value = jsonValue.toDouble();
            return true;
        }
    }
    return false;
}

bool JsonHandler::getJsonValueByKey(QJsonObject json,QString key,int& value){
    if(json.contains(key)){
        QJsonValue jsonValue = json.take(key);
        if(jsonValue.isDouble()){
            value = jsonValue.toInt();
            return true;
        }
    }
    return false;
}

bool JsonHandler::getJsonValue_longByKey(QJsonObject json,QString key,quint64& value){
    if(json.contains(key)){
        QJsonValue jsonValue = json.take(key);
        if(jsonValue.isDouble()){
            value = (quint64)jsonValue.toVariant().toULongLong();
            return true;
        }
    }
    return false;
}

bool JsonHandler::getJsonValueByKey(QJsonObject json,QString key,bool& value){
    if(json.contains(key)){
        QJsonValue jsonValue = json.take(key);
        if(jsonValue.isBool()){
            value = jsonValue.toBool();
            return true;
        }
    }
    return false;
}

bool JsonHandler::getJsonValueByKey(QJsonObject json,QString key,QString& value){
    if(json.contains(key)){
        QJsonValue jsonValue = json.take(key);
        if(jsonValue.isString()){
            value = jsonValue.toString();
            return true;
        }
    }
    return false;
}

bool JsonHandler::getJsonValueByKey(QJsonObject json,QString key,QJsonArray& value){
    if(json.contains(key)){
        QJsonValue jsonValue = json.take(key);
        if(jsonValue.isArray()){
            value = jsonValue.toArray();
            return true;
        }
    }
    return false;
}

bool JsonHandler::getJsonObjectValueByKey(QJsonObject json,QString key,QJsonObject& value){
    if(json.contains(key)){
        QJsonValue jsonValue = json.take(key);
        if(jsonValue.isObject()){
            value = jsonValue.toObject();
            return true;
        }
    }
    return false;
}

