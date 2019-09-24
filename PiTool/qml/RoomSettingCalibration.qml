import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2

MyLocationDialog {
    id:roomSettingCalibrationDialog
    property int mMargin: 40
    property int mLeftMargin:55
    property int mRightMargin:55
    property int progress:0
    property bool trigger:piService.trigger
    title: qsTr("房间设置向导");
    clientWidth: 750;
    clientHeight: 500;
    property bool connect: piService.connect;
    onConnectChanged: {
        if(!connect){
            roomSettingCalibrationDialog.close();
        }
    }
    onVisibleChanged: {
        if(visible && centerCalibration){
            roomSettingCalibrationButton.visible = false;
            progress = 100;
        }
    }

    onTriggerChanged: {
        if(trigger&&roomSettingCalibrationDialog.visible&&roomSettingCalibrationButton.visible){
            roomSettingCalibrationButton.visible = false;
            piService.getHmdPosition();
            waitTimer.start();
            piService.centerCalibration();
        }
    }

    btnClose.onClicked: {
        piService.stopRoomSettingNoteInHmd();
        waitTimer.stop();
        roomSettingCalibrationDialog.close();
    }
    Timer{
        id:waitTimer;
        interval: 100;
        repeat: true;
        triggeredOnStart: true;
        onTriggered: {
            if(!piService.headTracked || !piService.connect){
                progress = 0;
            }
            else if(piService.hmdNoMoving()){
                progress += 5;
            }else{
                progress -= 5;
            }

            if(progress >= 100){
                waitTimer.stop();
                centerCalibration = true;
            }
        }
    }

    Rectangle{
        parent:clientArea
        anchors.top:parent.top;anchors.topMargin: mMargin
        Rectangle{
            id:statusRect
            width: 240
            height: 270
            anchors.left: parent.left;anchors.leftMargin:mLeftMargin
            MyLabel{
                id:roomStatusTitle
                bold:true
                font.pixelSize: 18
                color: black
                text:qsTr("第2步 校准您的空间");
            }
            Rectangle{//line
                id:roomStatusLine;
                width: parent.width;height: 1;
                color: black;
                anchors.top:roomStatusTitle.bottom
            }
            MyLabel{
                id:roomStatusNote
                color: "#949494"
                anchors.top: roomStatusLine.bottom;anchors.topMargin: 15
                text:qsTr("站在腾出空间的正中央，同时举着头盔\n朝向您在VR体验中想要面向的默认方位。");
            }
            MyLabel{
                id:hmdStatusNote
                anchors.top: roomStatusNote.bottom;anchors.topMargin: 15
                text:qsTr("然后点击校准中心或扣动扳机键，校\n准完成之前保持不动。");
            }
            TextButton{
                id:roomSettingCalibrationButton
                anchors.top: hmdStatusNote.bottom;anchors.topMargin: 30
                anchors.horizontalCenter: parent.horizontalCenter
                fixedRect:true
                mWidth:135
                mHeight:30
                round:false
                fixedColor: true
                text: qsTr("校准中心")
                mouseArea.onClicked:{
                    roomSettingCalibrationButton.visible = false;
                    piService.getHmdPosition();
                    waitTimer.start();
                    piService.centerCalibration();
                }
            }
            Rectangle{
                id:progressBar
                visible: !roomSettingCalibrationButton.visible
                height: 30;
                width: 135;
                anchors.top: hmdStatusNote.bottom;anchors.topMargin: 30
                anchors.horizontalCenter: parent.horizontalCenter
                color:gray;
                Rectangle{
                    height: parent.height;
                    width: parent.width*(progress>100?100:progress)/100;
                    color: blue
                }
            }
        }
        Rectangle{
            id:pictureRect
            width:360
            height: 270
            anchors.left: statusRect.right;anchors.leftMargin: mMargin
            border.color: white;
            border.width: 1;
            radius: rRADIUS;
            Image{
                id:roomstep0Image
                source:"qrc:/resource/room_step2.png"
                anchors.centerIn: parent;
            }
        }
        Rectangle{
            id:buttonRect
            width: 640
            height: 150
            anchors.left: parent.left;anchors.leftMargin:mLeftMargin
            anchors.top: statusRect.bottom
            TextButton{
                id:roomSettingPreviousButton
                anchors.bottom: parent.bottom;anchors.bottomMargin: 40
                size:large
                text: qsTr("上一步")
                mouseArea.onClicked:{
                    //roomSettingCalibrationDialog.close();
                    centerCalibration = false;
                    changeRoomSettingSource("RoomSettingPositioning.qml");
                }
            }
            TextButton{
                id:roomSettingNextButton
                anchors.bottom: parent.bottom;anchors.bottomMargin: 40
                anchors.right: parent.right
                size:large
                enabled: centerCalibration
                type:enabled?primary:common
                text: qsTr("下一步")
                mouseArea.onClicked:{
                    changeRoomSettingSource("RoomSettingGroundHeight.qml");
                }
            }

        }
    }
}
