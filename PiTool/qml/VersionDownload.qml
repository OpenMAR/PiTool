import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import pimaxvr.VersionItem 1.0

MyDialog2 {
    id:dialog
    property int mMargin: 30;
    property int status: versionModel.piPlayVersion.status;
    property var piPlayVersion: versionModel.piPlayVersion
    onVisibleChanged: {
        if(visible){
            piPlayVersion.startDownload();
        }
    }
    onStatusChanged: {
        if(status == VersionItem.STATUS_DOWNLOADED && (piPlayVersion.upgradeFlag==1 || piPlayVersion.upgradeFlag == 2)){
            piPlayVersion.install();
            Qt.quit();
        }
    }

    title:qsTr("PiTool更新");
    btnClose.onClicked: {
        versionDownloadDialogCloseHandle();
    }

    Rectangle{
        parent:clientArea;
        width: parent.width;height: parent.height;
        color:translucent
        Rectangle{//进度条
            id:progressBar
            height: 30;
            width: parent.width-mMargin*2;
            anchors.centerIn: parent;
            color:white;
            Rectangle{
                height: parent.height;
                width: status==5?parent.width:parent.width*piPlayVersion.readBytes/piPlayVersion.totalBytes;
                color: blue
            }
            MyLabel{
                anchors.centerIn: parent;
                text:piPlayVersion.readBytes==0?"":settingModel.formatBytes(piPlayVersion.readBytes)+'/'+settingModel.formatBytes(piPlayVersion.totalBytes);
                font{pixelSize: 18;bold:true}
                color:black
            }
        }

        MyLabel{
            id:labelDownloading;
            text:{
                if(status==VersionItem.STATUS_DOWNLOADED)
                    return qsTr("下载完成");
                else if(status==VersionItem.STATUS_DOWNLOAD_FAILED)
                    return qsTr("下载失败");
                else
                    return qsTr("正在下载");
            }
            font.pixelSize: 28
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottom: progressBar.top;anchors.bottomMargin: mMargin;
        }
        TextButton{
            id:btnCancel;
            anchors.left: parent.horizontalCenter;
            anchors.leftMargin: (status==VersionItem.STATUS_DOWNLOADED||status==VersionItem.STATUS_DOWNLOAD_FAILED)?mMargin/2:-btnCancel.width/2;
            anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
            text:qsTr("取 消")
            size:large;
            mouseArea.onClicked: {
                versionDownloadDialogCloseHandle();
            }
        }
        TextButton{
            id: btnInstall;
            text:status==VersionItem.STATUS_DOWNLOAD_FAILED?qsTr("重试"):qsTr("立即安装")
            visible: status==VersionItem.STATUS_DOWNLOADED || status==VersionItem.STATUS_DOWNLOAD_FAILED;
            anchors.right: parent.horizontalCenter;anchors.rightMargin: mMargin/2;
            anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
            size:large;
            type:primary;
            mouseArea.onClicked: {
                if(status==VersionItem.STATUS_DOWNLOAD_FAILED){
                    piPlayVersion.startDownload();
                    return;
                }

                if(status==VersionItem.STATUS_DOWNLOADED){
                    piPlayVersion.install();
                    Qt.quit();
                    return;
                }
                if(piPlayVersion.upgradeFlag==1 || piPlayVersion.upgradeFlag == 2){
                    Qt.quit();
                }else{
                    dialog.close();
                }
            }
        }
    }

    function versionDownloadDialogCloseHandle(){
        if(status==VersionItem.STATUS_DOWNLOADING){//
            piPlayVersion.stopDownload();
        }
        if(piPlayVersion.upgradeFlag==1 || piPlayVersion.upgradeFlag == 2){
            Qt.quit();
        }else{
            dialog.close();
        }
    }

}
