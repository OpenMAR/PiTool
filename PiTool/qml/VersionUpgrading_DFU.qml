import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import pimaxvr.VersionItem 1.0

MyDialog2 {
    id:dialog
    property var firmwareVersion: versionModel.firmwareVersion
    property int status: versionModel.firmwareVersion.status;
    property bool connect: piService.connect;
    property int wait: 0;
    property int progress: 0;
    onVisibleChanged: {
        if(visible){
            console.log("VersionUpgrading DFU onVisibleChanged,status&connect&wait",status,connect,progress);
            if(status==VersionItem.STATUS_CHECKED&&firmwareVersion.upgradeFlag>0){
                firmwareVersion.startDownload();
            }else if(status==VersionItem.STATUS_DOWNLOADED){//下载完成
                if(piService.upgradeFirmware(firmwareVersion.getVersionFile())){
                    wait = 1;
                }else{
                    firmwareVersion.status = 7;
                }
            }
        }
    }

    onStatusChanged: {
        if(!visible){
            return;
        }
        console.log("VersionUpgrading DFU onStatusChanged,status",status);
        if(status==VersionItem.STATUS_DOWNLOADED){//下载完成
            if(piService.upgradeFirmware(firmwareVersion.getVersionFile())){
                wait = 1;
                waitTimer.start();
            }else{
                firmwareVersion.status = 7;
            }
        }else if(status==VersionItem.STATUS_UPGRADED){
            analyticsModel.upgradeFirmware(firmwareVersion.version);
        }
    }
    onConnectChanged: {
        wait++;
        if(wait>2){
            firmwareVersion.status = VersionItem.STATUS_UPGRADED;
            firmwareVersion.upgradeFlag=0;
        }
    }
    Timer{
        id:waitTimer;
        interval: 500;
        repeat: true;
        triggeredOnStart: true;
        onTriggered: {
            progress++;
            if(piService.noDfuDriver){
                showNoDfuDriverDialog();
                waitTimer.stop();
                firmwareVersion.status=VersionItem.STATUS_UPGRADE_FAILED;
                return;
            }
            if(progress>240){//两分钟超时
                firmwareVersion.status=VersionItem.STATUS_UPGRADE_FAILED;
            }
        }
    }
    title:qsTr("固件更新")
    showBtnClose:status==VersionItem.STATUS_DOWNLOAD_FAILED;
    Rectangle{
        parent:clientArea;
        width:parent.width;height: parent.height
        color:translucent;
        Rectangle{
            id:progressBar
            height: 30;
            width: parent.width-mMargin*2;
            anchors.centerIn: parent;
            color:white;
            Rectangle{
                height: parent.height;
                width: {
                    if(status==5){
                        var tempProgress = 50+progress;
                        return parent.width*(tempProgress>100?100:tempProgress)/100;
                    }else if(status==6){
                        return parent.width;
                    }else{
                        return parent.width*firmwareVersion.readBytes/firmwareVersion.totalBytes/2;
                    }
                }
                color: blue
            }
        }

        MyLabel{
            id:labelDownloading;
            text:{
                if(status==VersionItem.STATUS_UPGRADED){
                    return qsTr("更新完成");
                }else if(status==VersionItem.STATUS_UPGRADE_FAILED){
                    return qsTr("更新失败");
                }else if(status==VersionItem.STATUS_DOWNLOAD_FAILED){
                    return qsTr("下载失败");
                }else{
                    return qsTr("正在更新")
                }
            }
            font.pixelSize: 28
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottom: progressBar.top;anchors.bottomMargin: mMargin+10;
        }
        MyLabel{
            text:{
                if(status==VersionItem.STATUS_UPGRADED){
                    return "";
                }else if(status==VersionItem.STATUS_UPGRADE_FAILED){
                    return qsTr("请联系客服");
                }else if(status==VersionItem.STATUS_DOWNLOAD_FAILED){
                    return qsTr("请点击重试");
                }else{
                    return qsTr("固件升级中，请勿拔插头盔")
                }
            }
            font.pixelSize: 18;
            color: blue;
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: progressBar.top;anchors.bottomMargin: mMargin/2;
        }

        TextButton{
            id:btnOK;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
            text:status==VersionItem.STATUS_DOWNLOAD_FAILED?qsTr("重 试"):qsTr("确 定")
            mouseArea.onClicked: {
                if(status==VersionItem.STATUS_DOWNLOAD_FAILED){
                    firmwareVersion.startDownload();
                    return;
                }

                dialog.close();
                wait = 0;
                progress = 0;
                waitTimer.stop();
                firmwareVersion.clearData();
            }
            size:large;
            type:primary;
            visible: status>=6;
        }
    }

}
