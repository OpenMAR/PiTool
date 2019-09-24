#ifndef JSONHANDL_H
#define JSONHANDL_H
#include <QJsonObject>
#include <QJsonArray>

class JsonHandler
{
public:
    JsonHandler();
    static bool getJsonValue(QJsonObject json,QString key,bool defValue);
    static QString getJsonValue(QJsonObject json,QString key,QString defValue);
    static QString getJsonValue(QJsonObject json,QString key,const char *defValue);
    static double getJsonValue(QJsonObject json,QString key,double defValue);
    static int getJsonValue(QJsonObject json,QString key,int defValue);
    static quint64 getJsonValue_long(QJsonObject json,QString key,quint64 defValue);
    static QJsonArray getJsonValue(QJsonObject json,QString key,QJsonArray defValue);
    static QJsonObject getJsonObjectValue(QJsonObject json,QString key,QJsonObject defValue);
    static QJsonObject loadJsonObject(QString fileName);
    static bool loadJson(QString file, QJsonObject& object);
    static bool getJsonValueByKey(QJsonObject json,QString key,bool& value);
    static bool getJsonValueByKey(QJsonObject json,QString key,QString& value);
    static bool getJsonValueByKey(QJsonObject json,QString key,double& value);
    static bool getJsonValueByKey(QJsonObject json,QString key,int& value);
    static bool getJsonValue_longByKey(QJsonObject json,QString key,quint64& value);
    static bool getJsonValueByKey(QJsonObject json,QString key,QJsonArray& value);
    static bool getJsonObjectValueByKey(QJsonObject json,QString key,QJsonObject& value);
};

#endif // JSONHANDLER_H
