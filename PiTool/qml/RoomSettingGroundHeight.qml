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
        if(floorHeight >= 0){
            inputHeight.text = floorHeight;
        }
        if(visible && floorCalibration){
            roomSettingCalibrationButton.visible = false;
            progress = 100;
        }
    }

    onTriggerChanged: {
        if(trigger&&roomSettingCalibrationDialog.visible&&roomSettingCalibrationButton.visible){
            if(inputHeight.text.length > 0){
                roomSettingCalibrationButton.visible = false;
                piService.getHmdPosition();
                waitTimer.start();
                piService.floorCalibration(inputHeight.text/100);
            }
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
            if(piService.hmdNoMoving()){
                progress += 5;
            }else{
                progress -= 5;
            }

            if(progress >= 100){
                waitTimer.stop();
                floorCalibration = true;
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
                text:qsTr("第2步 定位地面");
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
                text:qsTr("将头盔放在稳定的平面上，\n接着测量该平面和地面的垂直距离，\n并在下方输入。");
            }
            MyLabel{
                id:hmdStatusNote
                anchors.top: roomStatusNote.bottom;anchors.topMargin: 15
                text:qsTr("如果您将头盔放在地面上，\n请输入0，然后点击校准地面按钮。");
            }
            MyTextField{
                id: inputHeight
                focus: true
                readOnly:false
                bold:true
                font.pixelSize:18
                width: 240
                height: 30
                anchors.bottom: roomSettingCalibrationButton.top;anchors.bottomMargin: 12;
                validator: IntValidator {bottom: 0;}
                maximumLength: 10
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }
            MyLabel{
                id:inputUnit
                visible: inputHeight.visible
                anchors.right: inputHeight.right;anchors.rightMargin:5
                anchors.verticalCenter: inputHeight.verticalCenter
                font.pixelSize:18
                color:blue
                text: "cm"
            }

            TextButton{
                id:roomSettingCalibrationButton
                anchors.bottom: parent.bottom;
                fixedRect:true
                mWidth:240
                mHeight:30
                round:false
                fixedColor: true
                text: qsTr("校准地面")
                mouseArea.onClicked:{
                    if(inputHeight.text.length > 0){
                        roomSettingCalibrationButton.visible = false;
                        piService.getHmdPosition();
                        waitTimer.start();
                        piService.floorCalibration(inputHeight.text/100);
                    }
                }
            }
            Rectangle{
                id:progressBar
                visible: !roomSettingCalibrationButton.visible
                height: 30;
                width: 240;
                anchors.bottom: parent.bottom;
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
                source:"qrc:/resource/room_step3.png"
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
                    floorCalibration = false;
                    if(inputHeight.text.length > 0){
                        floorHeight = inputHeight.text;
                    }
                    changeRoomSettingSource("RoomSettingCalibration.qml");
                }
            }
            TextButton{
                id:roomSettingNextButton
                anchors.bottom: parent.bottom;anchors.bottomMargin: 40
                anchors.right: parent.right
                size:large
                enabled: floorCalibration
                type:enabled?primary:common
                text: qsTr("下一步")
                mouseArea.onClicked:{
                    floorCalibration = true;
                    floorHeight = inputHeight.text;
                    changeRoomSettingSource("RoomSettingCompleted.qml");
                }
            }

        }
    }
}
