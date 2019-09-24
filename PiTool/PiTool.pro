requires(qtConfig(accessibility))

QT += qml quick core network widgets sql websockets
CONFIG += c++11 qt warn_on

SOURCES += src/main.cpp \
    src/piservice.cpp \
    src/ResourceItem.cpp \
    src/ResourceModel.cpp \
    quazip/JlCompress.cpp \
    quazip/qioapi.cpp \
    quazip/quaadler32.cpp \
    quazip/quacrc32.cpp \
    quazip/quagzipfile.cpp \
    quazip/quaziodevice.cpp \
    quazip/quazip.cpp \
    quazip/quazipdir.cpp \
    quazip/quazipfile.cpp \
    quazip/quazipfileinfo.cpp \
    quazip/quazipnewinfo.cpp \
    quazip/unzip.c \
    quazip/zip.c \
    src/OnlineRequest.cpp \
    src/OnlineModel.cpp \
    src/OnlineItem.cpp \
    src/DownloadFile.cpp \
    src/hwObject.cpp \
    src/SingleApplication.cpp \
    src/Setting.cpp \
    src/RpcServer.cpp \
    src/SteamModel.cpp \
    src/VersionModel.cpp \
    src/RegisterHandler.cpp \
    src/diagnoseHandler.cpp \
    src/jsonhandler.cpp \
    src/versioncontrol.cpp \
    src/CustomTrayicon.cpp

RESOURCES += qml.qrc \
    icon.qrc

TRANSLATIONS+=pitool_cn.ts pitool_en.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    piSvc/piSvcCapability.h \
    piSvc/piSvcCAPI.h \
    piSvc/piSvcDesc.h \
    piSvc/piSvcType.h \
    src/piservice.h \
    src/ResourceModel.h \
    src/ResourceItem.h \
    quazip/crypt.h \
    quazip/ioapi.h \
    quazip/JlCompress.h \
    quazip/quaadler32.h \
    quazip/quachecksum32.h \
    quazip/quacrc32.h \
    quazip/quagzipfile.h \
    quazip/quaziodevice.h \
    quazip/quazip.h \
    quazip/quazip_global.h \
    quazip/quazipdir.h \
    quazip/quazipfile.h \
    quazip/quazipfileinfo.h \
    quazip/quazipnewinfo.h \
    quazip/unzip.h \
    quazip/zconf.h \
    quazip/zip.h \
    quazip/zlib.h \
    src/OnlineRequest.h \
    src/OnlineModel.h \
    src/OnlineItem.h \
    src/DownloadFile.h \
    src/hwObject.h \
    src/SingleApplication.h \
    src/Setting.h \
    src/RpcServer.h \
    src/SteamModel.h \
    src/VersionModel.h \
    src/RegisterHandler.h \
    src/diagnoseHandler.h \
    quazip/JlCompress.h \
    PiLog/Exception.h \
    PiLog/StringUniversal.h \
    PiLog/WinCommon.h \
    PiLog/JsonHandler.h \
    src/jsonhandler.h \
    src/versioncontrol.h \
    src/CustomTrayicon.h

RC_FILE += icon.rc \

LIBS += User32.lib NETAPI32.LIB Advapi32.lib PiLog.lib
LIBS +=  -L"./piSvc" -lPiSvc
LIBS +=  -L"./PiLog"

DISTFILES +=
