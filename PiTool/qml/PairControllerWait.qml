import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import pimaxvr.PiService 1.0

MyDialog2{
    id:pairWaitDialog
    property int wait: 0;
    property int progress: 0;
    property bool connect: piService.connect;
    property int status: piService.pairStatus;
    onVisibleChanged: {
        if(visible){
            wait = 0;
            progress = 0;
            waitTimer.start();
        }
    }

    onStatusChanged: {
        if(status > 0){
            progress = 100;
            waitTimer.stop();
        }
    }

    Timer{
        id:waitTimer;
        interval: 500;
        repeat: true;
        triggeredOnStart: true;
        onTriggered: {
            progress++;
            if(progress>100){//50s超时
                waitTimer.stop();
            }
        }
    }
    clientWidth: 700;
    clientHeight: 370;
    title:qsTr("配对")
    Rectangle{
        parent: clientArea;
        width:parent.width;height: parent.height;
        color:translucent

        Rectangle{
            id:progressBar
            height: 30;
            width: parent.width*3/4;
            anchors.centerIn: parent;
            color:gray;
            Rectangle{
                height: parent.height;
                width: parent.width*(progress>100?100:progress)/100;
                color: blue
            }
        }

        MyLabel{
            id:labelDownloading;
            text:{
                if(status == 3){
                    return qsTr("配对失败");
                }else if(status == 1 || status == 2){
                    return qsTr("配对成功");
                }else
                {
                    return qsTr("正在配对...");
                }
            }
            font.pixelSize: 18
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottom: progressBar.top;anchors.bottomMargin: 50;
        }

        TextButton{
            id:btnOK;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: progressBar.bottom;anchors.topMargin: 40;
            text:qsTr("确 定")
            width: 130;height: 30;
            mouseArea.onClicked: {
                waitTimer.stop();
                pairWaitDialog.close();
            }
            visible: status>0;
        }
    }
}
