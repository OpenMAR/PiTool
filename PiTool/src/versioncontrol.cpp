#include "versioncontrol.h"
#include "RegisterHandler.h"

#define REG_UNINSTALL "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{0D1DA8F2-89A7-4DAC-A9EF-B55E82CDA462}}_is1"
#define DISPLAY_VERSION "DisplayName"

VersionControl* instance = nullptr;
VersionControl* VersionControl::getInstance()
{
    if(!instance){
        static VersionControl versionCtl;
        instance = &versionCtl;
    }
    return instance;
}

VersionControl::VersionControl(QObject *parent) : QObject(parent)
{
    instance = this;
    mVersionModel = new VersionModel();
}

void VersionControl::checkVersion()
{
    mCurrentVersion = RegisterHandler::getInstance().getRegisterValue(REG_UNINSTALL,DISPLAY_VERSION,"unknow");
    mVersionModel->checkPiPlayVersion(0,true);
}

int VersionControl::compareVersion(QString version1, QString version2)
{
    return 0;
}
