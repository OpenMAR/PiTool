import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyDialog2 {
    id:dfuDialog
    property int wait: 0;
    property int progress: 0;
    property bool forceUpgrading:piService.forceUpgrading;
    title:qsTr("固件更新");
    onVisibleChanged: {
        if(visible){
            piService.forceUpgradeFirmware();
            waitTimer.start();
        }
    }

    Rectangle{//checking
        parent:clientArea;
        width: parent.width;height: parent.height;
        color:translucent;
        visible: true;
        Timer{
            id:waitTimer;
            interval: 1000;
            repeat: true;
            triggeredOnStart: true;
            onTriggered: {
                progress++;
                if(!forceUpgrading){
                    progress = 100;
                }

                if(progress>100){//两分钟超时
                    waitTimer.stop();
                }
            }
        }

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
                        return parent.width*(progress>100?100:progress)/100;
                    }
                    color: blue
                }
            }

            MyLabel{
                id:labelDownloading;
                text:forceUpgrading?qsTr("正在升级"):qsTr("升级成功")
                font.pixelSize: 28
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.bottom: progressBar.top;anchors.bottomMargin: mMargin+10;
            }
            MyLabel{
                text:qsTr("固件升级中，请勿拔插头盔")
                visible: forceUpgrading
                font.pixelSize: 18;
                color: blue;
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: progressBar.top;anchors.bottomMargin: mMargin/2;
            }

            TextButton{
                id:btnOK;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.bottom: parent.bottom;anchors.bottomMargin: mMargin;
                text:qsTr("确 定")
                mouseArea.onClicked: {
                    dfuDialog.close();
                    wait = 0;
                    progress = 0;
                    waitTimer.stop();
                }
                size:large;
                type:primary;
                visible: !forceUpgrading;
            }
        }


    }

}
