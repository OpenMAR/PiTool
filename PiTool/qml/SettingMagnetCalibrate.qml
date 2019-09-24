import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyDialog2 {
    id:calibrateDialog
    property int calibrate: 0;//0--calibrating,1:calibrated,2:failed;
    property int progress: 0;
    property bool connect: piService.connect;
    onConnectChanged: {
        if(!connect){
            waitTimer.stop();
            calibrateDialog.close();
        }
    }
    onVisibleChanged: {
        if(visible){
            progress = 0;
            calibrate = 0;
            waitTimer.start();
        }
    }
    title: qsTr("磁校准");
    clientWidth: 600;
    clientHeight: 500;
    Rectangle{
        parent:clientArea;
        width: parent.width;height: parent.height
        color:translucent;
        Timer{
            id:waitTimer;
            interval: 1000;
            repeat: true;
            triggeredOnStart: true;
            onTriggered: {
                progress++;
                if(progress>100){//100s超时
                    waitTimer.stop();
                    calibrate = 2;
                }else{
                    if(piService.isCalibrateFinished()){
                        calibrate = 1;
                    }
                }
            }
        }

        MyLabel{
            id: guide;
            text:qsTr("请手持头盔，大幅度上下、左右翻转头盔，进行校准")
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.top;anchors.topMargin: mMargin;
        }

        Image{
            id:image_bg;
            source: "qrc:/resource/magnet-calibrate.png";
            anchors.right: parent.right;anchors.rightMargin: 70;
            anchors.top: guide.bottom;anchors.topMargin: mMargin;
        }
        Image{
            id:image_hmd
            width:164;height: 125;
            source: {
                if(piService.hmd_name.indexOf("4K")>0){
                    return "qrc:/resource/pi_device_4k.png"
                }else if(piService.hmd_name.indexOf("8K")>0){
                    return "qrc:/resource/pi_device_8k.png"
                }else if(piService.hmd_name.indexOf("B1")>0){
                    return "qrc:/resource/pi_device_b1.png"
                }else{
                    return "qrc:/resource/pi_device_4k.png"
                }
            }
            anchors.top: image_bg.verticalCenter;anchors.topMargin: -30;
            anchors.right: image_bg.left;anchors.rightMargin: -30;
        }

        MyLabel{
            id: status;
            text:{
                if(calibrate==0){
                    return qsTr("正在进行磁校准…");
                }else if(calibrate==1){
                    return qsTr("磁校准完成！");
                }else{
                    return qsTr("磁校准失败，请关闭后重试！");
                }
            }
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: image_bg.bottom;anchors.topMargin: mMargin;
        }
        TextButton{
            text: qsTr("完成")
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: status.bottom;anchors.topMargin: mMargin;
            size:large
            mouseArea.onClicked: {
                calibrateDialog.close();
            }
            visible: calibrate>0;
        }
    }

}
