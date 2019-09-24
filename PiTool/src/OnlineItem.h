#ifndef ONLINE_ITEM_H
#define ONLINE_ITEM_H

#include "JsonHandler.h"
#include <QObject>
#include <QJsonObject>
class ListObject:public QObject{
    Q_OBJECT
public:
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QList<QObject*> listModel READ listModel NOTIFY listModelChanged)
    Q_INVOKABLE QObject* get(int index);
    Q_INVOKABLE void clear();

signals:
    void countChanged(int count);
    void listModelChanged(QList<QObject*>);
public:
    QList<QObject*> mList;
    Q_INVOKABLE void append(QObject*);
    int count();
    QList<QObject*> listModel();
    ListObject(QObject *parent = 0);
};

class FilterItem:public ListObject{
    Q_OBJECT
public:
    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(bool group READ group NOTIFY groupChanged)
//    Q_INVOKABLE FilterItem* get(int index);

signals:
    void idChanged(int id);
    void titleChanged(QString title);
    void groupChanged(bool bGroup);

public:
    FilterItem(ListObject *parent = 0);

public:
    QString title();
    int id();
    bool group();
    void setGroup(bool group);
    bool loadFromJson(QJsonObject json);
    bool loadFromJsonArray(QJsonArray array);
public:
    int mId;
    QString mTitle;
    bool mGroup;
};

class OnlineItem:public ListObject{
    Q_OBJECT
public:
    Q_PROPERTY(QString id READ id NOTIFY idChanged)//resource ID
    Q_PROPERTY(QString thirdId READ thirdId NOTIFY thirdIdChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString author READ author NOTIFY authorChanged)
    Q_PROPERTY(QString desc READ desc NOTIFY descChanged)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString subType READ subType NOTIFY subTypeChanged)
    Q_PROPERTY(QString url READ downloadUrl NOTIFY urlChanged)
    Q_PROPERTY(QString imageUrl READ imageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QString extendUrl READ extendUrl NOTIFY extendUrlChanged)
    Q_PROPERTY(QString extendUrlTitle READ extendUrlTitle NOTIFY extendUrlTitleChanged)
    Q_PROPERTY(qint64 playCount READ playCount NOTIFY playCountChanged)
    Q_PROPERTY(QString scoreStr READ scoreStr NOTIFY scoreStrChanged)
    Q_PROPERTY(int score READ score NOTIFY scoreChanged)
    Q_PROPERTY(qint64 totalBytes READ totalBytes NOTIFY totalBytesChanged)
    Q_PROPERTY(QString totalBytesFmt READ totalBytesFmt NOTIFY totalBytesFmtChanged)
    Q_PROPERTY(QStringList tags READ tags NOTIFY tagChanged)
    Q_PROPERTY(bool canDownload READ canDownload NOTIFY canDownloadChanged)

public:
    OnlineItem(ListObject *parent = 0);
    void copyItem(OnlineItem *item);

signals:
    void idChanged(QString id);
    void titleChanged(QString title);
    void authorChanged(QString author);
    void descChanged(QString desc);
    void typeChanged(QString type);
    void subTypeChanged(QString type);
    void urlChanged(QString url);
    void imageUrlChanged(QString imageUrl);
    void extendUrlChanged(QString extendUrl);
    void extendUrlTitleChanged(QString title);
    void playCountChanged(qint64 playCount);
    void scoreStrChanged(QString score);
    void scoreChanged(int score);
    void totalBytesChanged(qint64 totalBytes);
    void totalBytesFmtChanged(QString totalBytesFmt);
    void tagChanged(QStringList tags);
    void thirdIdChanged(QString thirdId);
    void canDownloadChanged(bool flag);
public:
    QString id();
    QString thirdId();
    QString title();
    QString author();
    QString desc();
    QString type();
    QString subType();
    QString downloadUrl();
    QString imageUrl();
    QString extendUrl();
    QString extendUrlTitle();
    qint64 playCount();
    QString scoreStr();
    int score();
    qint64 totalBytes();
    QString totalBytesFmt();
    QStringList tags();
    bool canDownload();

    bool loadFromJson(QJsonObject json);
private:
    QString mId;
    QString mThirdId;
    QString mTitle;
    QString mAuthor;
    QString mDesc;
    QString mType;
    QString mSubType;
    QString mImageUrl;
    QString mDownloadUrl;
    QString mExtendUrl;
    QString mExtendUrlTitle;
    int mPlayCount;
    int mScore;
    quint64 mTotalBytes;
    QStringList mTags;
    int mDownloadFlag;
};

class GameItem:public OnlineItem{
    Q_OBJECT
public:
    Q_PROPERTY(QString os READ os NOTIFY osChanged)
    Q_PROPERTY(QString cpu READ cpu NOTIFY cpuChanged)
    Q_PROPERTY(QString displayCard READ displayCard NOTIFY displayCardChanged)
    Q_PROPERTY(QString memory READ memory NOTIFY memoryChanged)
    Q_PROPERTY(bool controller READ controller NOTIFY controllerChanged)
    Q_PROPERTY(bool gamepad READ gamepad NOTIFY gamepadChanged)
    Q_PROPERTY(bool keyboard READ keyboard NOTIFY keyboardChanged)
    Q_PROPERTY(bool mouse READ mouse NOTIFY mouseChanged)
    Q_PROPERTY(bool sit READ sit NOTIFY sitChanged)
    Q_PROPERTY(bool stand READ stand NOTIFY standChanged)
    Q_PROPERTY(bool space READ space NOTIFY spaceChanged)
    Q_PROPERTY(QString operateGuide READ operateGuide NOTIFY operateGuideChanged)
    Q_PROPERTY(QString guideBBS READ guideBBS NOTIFY guideBBSChanged)
    Q_PROPERTY(QString guideUrl READ guideUrl NOTIFY guideUrlChanged)
    Q_PROPERTY(QString  guideContent READ guideContent NOTIFY guideContentChanged)
    Q_PROPERTY(QStringList imageUrlList READ imageUrlList NOTIFY imageUrlListChanged)

//    Q_INVOKABLE GameItem* get(int index);
    Q_INVOKABLE void appendItems(GameItem* items);

public:
    GameItem(OnlineItem *parent = 0);
//    void copyItem(GameItem *item);

signals:
    void osChanged(QString os);
    void cpuChanged(QString cpu);
    void displayCardChanged(QString displayCard);
    void memoryChanged(QString memory);
    void controllerChanged(bool controller);
    void gamepadChanged(bool gamepad);
    void keyboardChanged(bool keyboard);
    void mouseChanged(bool mouse);
    void sitChanged(bool sit);
    void standChanged(bool stand);
    void spaceChanged(bool space);
    void operateGuideChanged(QString operateGuide);
    void guideBBSChanged(QString guideBBS);
    void guideUrlChanged(QString guideUrl);
    void imageUrlListChanged(QStringList imageUrlList);
    void guideContentChanged(QString guideContent);

public:
    QString os();
    QString cpu();
    QString displayCard();
    QString memory();
    bool controller();
    bool gamepad();
    bool keyboard();
    bool mouse();
    bool sit();
    bool stand();
    bool space();
    QString operateGuide();
    QString guideBBS();
    QString guideUrl();
    QStringList imageUrlList();
    QString  guideContent();

    bool loadFromJson(QJsonObject json);
private:
    QString mOS;
    QString mCPU;
    QString mDisplayCard;
    QString mMemory;

    bool mbController;
    bool mbGamepad;
    bool mbKeyboard;
    bool mbMouse;

    bool mbSit;
    bool mbStand;
    bool mbSpace;

    QString mOperateGuide;
    QString mGuideBBS;
    QString mGuideUrl;
    QString mGuideContent;

    QStringList mImageUrlList;
};


class VideoItem:public OnlineItem{
    Q_OBJECT
public:
    Q_PROPERTY(QString os READ os NOTIFY osChanged)
    Q_PROPERTY(QString cpu READ cpu NOTIFY cpuChanged)
    Q_PROPERTY(QString displayCard READ displayCard NOTIFY displayCardChanged)
    Q_PROPERTY(QString memory READ memory NOTIFY memoryChanged)

//    Q_INVOKABLE VideoItem* get(int index);
    Q_INVOKABLE void appendItems(VideoItem* items);

public:
    VideoItem(OnlineItem *parent = 0);
//    VideoItem *copyItem(VideoItem *item);
signals:
    void osChanged(QString os);
    void cpuChanged(QString cpu);
    void displayCardChanged(QString displayCard);
    void memoryChanged(QString memory);

public:
    QString os();
    QString cpu();
    QString displayCard();
    QString memory();

    bool loadFromJson(QJsonObject json);
private:
    QString mOS;
    QString mCPU;
    QString mDisplayCard;
    QString mMemory;
};


class CatalogItem:public ListObject{
    Q_OBJECT
public:
    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString linkTitle READ linkTitle NOTIFY linkTitleChanged)
    Q_PROPERTY(QString linkType READ linkType NOTIFY linkTypeChanged)
    Q_PROPERTY(QString linkParam READ linkParam NOTIFY linkParamChanged)
    Q_PROPERTY(QString imageUrl READ imageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(GameItem* gameList READ gameList  NOTIFY gameListChanged)
    Q_PROPERTY(VideoItem* videoList READ videoList NOTIFY videoListChanged)
//    Q_INVOKABLE CatalogItem* get(int index);

public:
    CatalogItem(ListObject *parent = 0);
    ~CatalogItem();

signals:
    void idChanged(int id);
    void titleChanged(QString title);
    void typeChanged(QString type);
    void linkTitleChanged(QString linkTitle);
    void linkTypeChanged(QString linkType);
    void linkParamChanged(QString linkParam);
    void imageUrlChanged(QString imageUrl);
    void gameListChanged(GameItem* gameList);
    void videoListChanged(VideoItem* videoList);

public:
    int id();
    QString title();
    QString type();
    QString linkTitle();
    QString linkType();
    QString linkParam();
    QString imageUrl();
    GameItem* gameList();
    VideoItem* videoList();

    bool loadFromJson(QJsonObject json);
private:
    int mId;
    QString mTitle;
    QString mType;
    QString mLinkTitle;
    QString mLinkType;
    QString mLinkParam;
    QString mImageUrl;
    GameItem* mGameList;
    VideoItem* mVideoList;
};

class HelpItem:public ListObject{
    Q_OBJECT
public:
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString url READ url NOTIFY urlChanged)
    Q_PROPERTY(bool group READ group NOTIFY groupChanged)
//    Q_INVOKABLE HelpItem* get(int index);

public:
    HelpItem(ListObject *parent = 0);
signals:
    void titleChanged(QString title);
    void urlChanged(QString type);
    void groupChanged(bool group);

public:
    QString title();
    QString url();
    bool group();
    bool loadFromJson(QJsonObject json);

private:
    QString mTitle;
    QString mUrl;
    bool mGroup;
};

#endif /*RESOURCE_ITEM_H*/
